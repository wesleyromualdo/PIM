<?php
namespace Simec;

/**
 * Classe de representação do sistema SIMEC
 * @author Calixto Jorge <calixto.jorge@gmail.com>
 */
class Simec
{
    const producao = 1;
    const desenvolvimento = 2;
    const controle = 'Controle';
    const visao = 'Visao';
    const ajudante = 'Ajudante';

    protected static $roteador;

    /**
     * Método construtor do Simec
     */
    public function __construct()
    {
        self::$roteador = new Roteador;
    }

    /**
     * Roda o sistema "se possível"
     * @return boolean
     */
    public function rodar()
    {
        $rota = $_REQUEST['modulo'];
        if (!self::$roteador->possuiRecurso($rota)) {
            return false;
        }
        self::$roteador->rotear($rota);
        return true;
    }

    /**
     * Retorna o caminho dos arquivos das classes
     * @param string $stClasse nome da classe
     * @param string $tipo tipo da classe
     * @return string
     */
    public static function pegarCaminho($stClasse, $tipo = null)
    {
        if (preg_match('/Simec[\\\\](.*)/', $stClasse, $match)) {
            $arPath = explode('\\', $match[1]);
            $arPath[0] = strtolower($arPath[0]);
            array_unshift($arPath, strtolower($arPath[0]));
            $arPath[1] = 'MecCoder';
            if ($tipo) {
                $arPath[2] = $tipo;
                if ($tipo == 'Ajudante') {
                    $arPath[4] = 'Visao';
                }
            }
            $name = implode('\\', $arPath);
            return PHP_OS != "WINNT" ? str_replace('\\', '/', $name) : $name;
        }
        return '';
    }

    /**
     * Retorna o caminho do arquivo de controle
     * @param string $stClasse nome da classe
     * @return string
     */
    public static function pegarCaminhoControle($stClasse)
    {
        return self::pegarCaminho($stClasse, Simec::controle);
    }

    /**
     * Retorna o caminho do arquivo de visão
     * @param string $stClasse
     * @return string
     */
    public static function pegarCaminhoVisao($stClasse)
    {
        return self::pegarCaminho($stClasse, Simec::visao);
    }

    /**
     * Retorna o caminho do arquivo de visão de um helper
     * @param string $stClasse
     * @return string
     */
    public static function pegarCaminhoAjudante($stClasse)
    {
        return self::pegarCaminho($stClasse, Simec::ajudante);
    }

    /**
     * Retorna o arquivo liberado requisitado pela url
     * @param string $file caminho do arquivo
     */
    public static function liberarArquivoPublico($file)
    {
        $file = isset($_GET['file']) ? APPRAIZ . $_GET['file'] : '';
        if (is_file($file) && preg_match('/.*\.(css|js)$/', $file, $match)) {
            if ($match[1] == 'css') {
                header("Content-type: text/css; charset=iso-8859-1", true);
            } else {
                header("Content-Type: application/javascript; charset=iso-8859-1", true);
            }
            echo "/* Arquivo:{$file} */\n\n";
            echo file_get_contents($file);
        }
    }
    
    /**
     * Retorna o ambiente atual
     * @return string
     */
    public static function ambiente(){
        return $_SESSION['baselogin'] == 'simec_desenvolvimento' ? self::desenvolvimento : self::producao;
    }
    
    /**
     * Retorna se o ambiente atual é produção
     * @return boolean
     */
    public static function producao(){
        return self::ambiente() == self::producao;
    }
    
    /**
     * Retorna se o ambiente atual é desenvolvimento
     * @return boolean
     */
    public static function desenvolvimento(){
        return self::ambiente() == self::desenvolvimento;
    }
}
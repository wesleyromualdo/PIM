<?php
//include_once(APPRAIZ . 'MecCoder/Util/Console/Console.php');
namespace Simec;

class Terminal
{

    public $arquivo;
    public $comando;
    public $argumentos;

    public function __construct()
    {
        global $argv;
        foreach ($argv as $idx => $arg) {
            if ($idx > 1) {
                $ar = explode('=', $arg);
                $i = array_shift($ar);
                $this->argumentos[$i] = array_shift($ar);
            } else {
                $this->arquivo = $argv[0];
                $this->comando = isset($argv[1]) ? $argv[1] : false;
            }
        }
    }

    protected static function mostrar($texto, $cor = null, $fundo = null)
    {
        echo ($texto);
    }

    /**
     * Imprime este manual
     */
    public function ajuda()
    {
        $mapear = function($comment) {
            $arComentario = explode("\n", $comment);
            foreach ($arComentario as $parte) {
                if (preg_match('/\*\s+(\@|)(call|param|return|)(.*)$/', $parte, $match)) {
                    switch($match[2]){
                        case 'call':
                            $this->mostrar("    Chamada de execução: {$match[3]} \n");
                            break;
                        case 'param':
                            $args = explode(' ',$match[3]);
                            array_shift($args);
                            $this->mostrar(sprintf("    Parâmetro de chamada: [%s %s] %s \n", array_shift($args), array_shift($args), implode(' ', $args)));
                            break;
                        case 'return':
                            $this->mostrar("    Retorno da execução: {$match[3]} \n");
                            break;
                    }
                }
            }
        };
        $reflection = new \ReflectionClass($this);
        $this->mostrar("\n ****************** Ajuda **********************\n");
        foreach ($reflection->getMethods() as $method) {
            if ($method->isPublic() && !in_array($method->getName(),['ajuda','inicializar','__construct'])) {
                $this->mostrar("\n\n");
                $this->mostrar('#### '.$method->getName() . ": \n");
                $mapear($method->getDocComment());
            }
        }
        $this->mostrar("\n ***********************************************\n");
        $this->mostrar("\n\n");
    }

    /**
     * Inicializa o terminal do sistema para uma execução caso não exista chama ajuda
     */
    public function inicializar()
    {
        $reflection = new \ReflectionClass($this);
        if ($this->comando && $reflection->hasMethod($this->comando)) {
            $this->{$this->comando}();
        } else {
            $this->ajuda();
        }
    }

    /**
     * Executa a chamada de um método de uma classe do sistema
     * @call executar classe=<<minhaClasse>> metodo=<<meuMetodo>> 
     * @return mixed
     */
    public function executar()
    {
        $classe = $this->argumentos['classe'];
        $metodo = $this->argumentos['metodo'];
        $obj = new $classe();
        return call_user_func_array([$obj, $metodo], $this->argumentos);
    }

    /**
     * Cria a estrutura na pasta um módulo do simec.
     * @call gerarModulo nome=<<nomeModulo>>
     * @param string $nome Nome da pasta módulo do simec
     */
    public function gerarModulo()
    {
        Corporativo\Gerador\Modulo::gerarModulo($this->argumentos['nome']);
    }

    /**
     * Retorna a saída de uma classe gerada pelo esquema e nome da tabela
     * @param string $esquema Nome do esquema no banco
     * @param string $tabela Nome da tabela no banco
     * @call gerarEstrutura esquema=<<meuEsquema>> tabela=<<minhaTabela>> 
     * @return string Conteúdo da classe de dados da tabela referente
     */
    public function gerarEstrutura()
    {
        echo (new \Simec\Corporativo\Gerador\Dado())->gerar($this->argumentos['esquema'], $this->argumentos['tabela']);
        die;
    }
}

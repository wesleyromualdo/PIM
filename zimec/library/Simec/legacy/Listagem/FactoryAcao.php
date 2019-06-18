<?php
/**
 * $Id: FactoryAcao.php 94527 2015-02-26 19:50:34Z maykelbraz $
 */

/**
 *
 */
require_once dirname(__FILE__) . '/Acao.php';
/**
 *
 */
require_once dirname(__FILE__) . '/AcaoComID.php';

/**
 *
 */
class Simec_Listagem_FactoryAcao
{
    /**
     * Identificação das propriedades comuns de uma ação. Qualquer configuração adicional é definida<br />
     * através de Simec_Listagem_Acao::setConfig() e tratada pela própria ação.
     *
     * @var array
     */
    protected static $configsBasicasAcao = array(
        'func' => null, 'id-composto' => null, 'extra-params' => null, 'external-params' => null, 'condicao' => null,
        'titulo' => null
    );

    private function __construct()
    {
    }

    public static function getAcao($acao, $config)
    {
        if (!self::arquivoDeClasseExiste($acao)) {
            throw new Exception("A classe da ação solicitada ({$acao}) ainda não foi implementada.");
        }
        require_once(self::getCaminhoArquivo($acao));
        $nomeClasseAcao = 'Simec_Listagem_Acao_' . ucfirst($acao);

        // -- Instanciando a nova ação
        $objAcao = new $nomeClasseAcao();

        // -- Callback JS
        if (is_string($config)) {
            return $objAcao->setCallback($config);
        } else {
            $objAcao->setCallback($config['func']);
        }

        // -- Composição de ID de identificação da ação
        if (key_exists('id-composto', $config)) {
            $objAcao->setPartesID($config['id-composto']);
        }

        // -- Parametros extras
        if (key_exists('extra-params', $config)) {
            $objAcao->addParams(Simec_Listagem_Acao::PARAM_EXTRA, $config['extra-params']);
        }

        // -- Parametros externos
        if (key_exists('external-params', $config)) {
            $objAcao->addParams(Simec_Listagem_Acao::PARAM_EXTERNO, $config['external-params']);
        }

        // -- Condições
        if (key_exists('condicao', $config) && is_array($config['condicao'])) {
            foreach ($config['condicao'] as $condicao) {
                $objAcao->addCondicao($condicao);
            }
        }

        // -- Título da ação
        if (key_exists('titulo', $config)) {
            $objAcao->setTitulo($config['titulo']);
        }

        // -- Configurações adicionais - tratadas diretamente na ação
        $objAcao->setConfig(array_diff_key($config, self::$configsBasicasAcao));

        return $objAcao;
    }

    /**
     * Verifica se um arquivo de classe de ação existe. O caminho é computado com base no caminho
     * atual deste arquivo.
     *
     * @param string $acao O nome da ação.
     * @return boolean
     */
    protected static function arquivoDeClasseExiste($acao)
    {
        return is_file(self::getCaminhoArquivo($acao));
    }

    protected static function getCaminhoArquivo($acao)
    {
        return dirname(__FILE__) . '/Acao/' . ucfirst($acao) . '.php';
    }

}

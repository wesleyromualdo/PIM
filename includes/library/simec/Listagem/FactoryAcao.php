<?php
/**
 * Implementação da classe de criação de ações da lista de dados.
 *
 * @version $Id: FactoryAcao.php 141842 2018-07-18 13:08:08Z victormachado $
 * @filesource
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
 * Recebe requisições para criação de ações que são anexadas às linhas da listagem.
 *
 * Não há necessidade de utilizar esta classe diretamente, ela é utilizada pela Simec_Listagem
 * para criação de suas ações.
 *
 * Basicamente, as ações derivam de duas classes base Simec_Listagem_Acao e
 * Simec_Listagem_AcaoComID. Inicialmente, as implementações de Simec_Listagem_Acao são
 * ações mais simples, enquanto Simec_Listagem_AcaoComID implementam ações mais avançadas
 * e que executam algumas tarefas de manipulação da lista.
 *
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 * @package Simec\View\Listagem
 *
 * @uses Simec_Listagem_Acao
 */
class Simec_Listagem_FactoryAcao
{
    /**
     * Identificação das propriedades comuns de uma ação.
     *
     * Qualquer configuração adicional é setada através de Simec_Listagem_Acao::setConfig() e tratada pela própria ação.
     * As configurações padrões são:
     * * func: Nome da função executada pela ação.
     * * id-composto: Lista com os campos que compoe o id de uma ação.
     * * extra-params: Lista de campos adicionais que devem ser passados para a função.
     * * external-params: Lista de valores adicionais que devem ser passados para a função.
     * * condicao: Lista com as condições necessárias para exibição da ação.
     * * titulo: Tipo da ação, exibido quando o mouse é colocado sobre a ação.
     *
     * @var mixed[]
     */
    protected static $configsBasicasAcao = array(
        'func' => null,
        'id-composto' => null,
        'extra-params' => null,
        'external-params' => null,
        'condicao' => null,
        'titulo' => null
    );

    /**
     * Classe sem construtor. Para criar uma nova ação o método Simec_Listagem_FactoryAcao::getAcao() deve ser utilizado.
     *
     * @see self::getAcao()
     */
    private function __construct()
    {
    }

    /**
     * Método utilizado para criar uma ação.
     *
     * A classe da ação solicitada é instanciada e tem suas configurações setadas e então é retornada para a Simec_Listagem.
     * Veja a lista completa das ações veja as subclasses de Simec_Listagem_Acao e Simec_Listagem_AcaoComId.
     *
     * @param string $acao Nome da ação que será criada.
     * @param mixed[] $config Configurações de criação da ação.
     * @return Simec_Listagem_Acao
     * @throws Exception
     *
     * @see Simec_Listagem_Acao
     * @see Simec_Listagem_AcaoComID
     */
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

        // -- Regras
        if (key_exists('regra', $config) && is_array($config['regra'])) {
            $objAcao->addRegra($config['regra']);
        }

        // -- Título da ação
        if (key_exists('titulo', $config)) {
            $objAcao->setTitulo($config['titulo']);
        }

        // -- Cor do bottão da ação
        if (key_exists('cor', $config)) {
            $objAcao->setCor($config['cor']);
        }

        // -- Configurações adicionais - tratadas diretamente na ação
        $objAcao->setConfig(array_diff_key($config, self::$configsBasicasAcao));

        return $objAcao;
    }

    /**
     * Verifica se o arquivo da implementação da ação existe.
     *
     * Os arquivos de implementação de ações devem estar dentro da pasta Simec\Listagem\Acao.
     *
     * @param string $acao O nome da ação para verificação.
     * @return boolean
     */
    protected static function arquivoDeClasseExiste($acao)
    {
        return is_file(self::getCaminhoArquivo($acao));
    }

    /**
     * Constrói o caminho do arquivo de implementação de uma ação.
     *
     * @param string $acao O nome da ação.
     * @return string
     */
    protected static function getCaminhoArquivo($acao)
    {
        return dirname(__FILE__) . '/Acao/' . ucfirst($acao) . '.php';
    }
}

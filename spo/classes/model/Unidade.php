<?php
/**
 * Classe de mapeamento da entidade public.unidade.
 *
 * $Id: Unidade.php 100919 2015-08-06 19:01:37Z maykelbraz $
 */

/**
 * Mapeamento da entidade public.unidade.
 *
 * @see Modelo
 */
class Spo_Model_Unidade extends Modelo
{
    /**
     * Código do Órgão ao qual o MEC está associado.
     */
    const CODIGO_ORGAO = '26000';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.unidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'unicod',
        'unitpocod',
        'unicod',
        'unitpocod',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'gstcod' => array('tabela' => 'financeiro.gestao', 'pk' => 'gstcod'),
        'gunid' => array('tabela' => 'grupounidade', 'pk' => 'gunid'),
        'orgcod, organo' => array('tabela' => 'orgao', 'pk' => 'orgcod, organo'),
        'tpocod' => array('tabela' => 'tipoorgao', 'pk' => 'tpocod'),
        'ungcodresponsavel' => array('tabela' => 'unidadegestora', 'pk' => 'ungcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'unicod' => null,
        'unitpocod' => null,
        'orgcod' => null,
        'organo' => null,
        'tpocod' => null,
        'uniano' => null,
        'unidsc' => null,
        'unistatus' => null,
        'uniid' => null,
        'uniabrev' => null,
        'gunid' => null,
        'ungcodresponsavel' => null,
        'gstcod' => null,
        'orgcodsupervisor' => null,
        'uniddd' => null,
        'unitelefone' => null,
        'uniemail' => null,
        'unidataatualiza' => null,
    );

    /**
     * @var string[] Lista de unidade orçamentárias obrigatórias
     */
    protected static $obrigatorias = array(
        '26101',
        '26291',
        '26290',
        '26298',
        '26443',
        '74902',
        '73107'
    );

    public static function getObrigatorias($retornarString = false, $separador = ', ')
    {
        if ($retornarString) {
            return "'" . implode("'{$separador}'", self::$obrigatorias) . "'";
        }

        return self::$obrigatorias;
    }

    public static function queryCombo($obrigatorias = false, $whereUO = '')
    {
        if ($obrigatorias) {
            $obrigatorias = ' AND uni.unicod NOT IN(' . self::getObrigatorias(true) . ')';
        } else {
            $obrigatorias = '';
        }

        return <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || unidsc AS descricao
  FROM public.unidade uni
  WHERE (uni.orgcod = '26000' OR uni.unicod IN('74902', '73107'))
    AND uni.unistatus = 'A'
    {$whereUO}
    {$obrigatorias}
ORDER BY uni.unicod
DML;
    }
}
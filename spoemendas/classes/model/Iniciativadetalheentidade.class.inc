<?php
/**
 * Classe de mapeamento da entidade emenda.iniciativadetalheentidade
 *
 * @category Class
 * @package  A1
 * @author   WERTER DIAS ALMEIDA <werter.almeida@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-02-2016
 * @link     no link
 */

/**
 * Spoemendas_Model_Iniciativadetalheentidade
 *
 * @category Class
 * @package  A1
 * @author   WERTER DIAS ALMEIDA <werter.almeida@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-02-2016
 * @link     no link
 */
class Spoemendas_Model_Iniciativadetalheentidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emenda.iniciativadetalheentidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ideid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'edeid' => array('tabela' => 'emenda.emendadetalheentidade', 'pk' => 'edeid'),
        'iniid' => array('tabela' => 'emenda.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ideid' => null,
        'edeid' => null,
        'iniid' => null,
        'idestatus' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'ideid' => array(  'Digits'  ),
            'edeid' => array(  'Digits'  ),
            'iniid' => array(  'Digits'  ),
            'idestatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
        );
    }

    /**
     * @param $emeid
     * @return array|string
     */
    public function getDetalhesIniciativa($emeid)
    {
        if (!$emeid) return [];

        $strSQL = <<<DML
SELECT DISTINCT ini.iniid,
                emd.emeid,
                ini.ininome,
                ini.gndcod,
                ini.iniresolucao
  FROM emenda.emendadetalhe emd
    JOIN emenda.iniciativaemendadetalhe ind USING (emdid)
    JOIN emenda.iniciativa ini USING (iniid)
  WHERE emeid = {$emeid}
    AND ind.iedstatus = 'A'
    and emdstatus = 'A'
    AND ini.inistatus = 'A'
  ORDER BY ini.ininome
DML;
        return $strSQL;
    }

    public function findBeneficiariosByEmeid($emeid)
    {
        $sql = <<<DML
SELECT ide.*
  FROM emenda.emendadetalheentidade ede
    LEFT JOIN {$this->stNomeTabela} ide USING(edeid)
    INNER JOIN emenda.emendadetalhe emd USING(emdid)
  WHERE emd.emeid = {$emeid} and emdstatus = 'A'
DML;

        return $this->carregar($sql);
    }
}
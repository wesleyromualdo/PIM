<?php
/**
 * Classe de mapeamento da entidade emenda.iniciativaemendadetalhe
 *
 * @category Class
 * @package  A1
 * @author   WERTER DIAS ALMEIDA <werter.almeida@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-02-2016
 * @link     no link
 */



/**
 * Spoemenda_Model_Iniciativaemendadetalhe
 *
 * @category Class
 * @package  A1
 * @author   WERTER DIAS ALMEIDA <werter.almeida@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 18-02-2016
 * @link     no link
 */
class Spoemenda_Model_Iniciativaemendadetalhe extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emenda.iniciativaemendadetalhe';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'iedid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'emdid' => array('tabela' => 'emenda.emendadetalhe', 'pk' => 'emdid'),
        'iniid' => array('tabela' => 'emenda.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'iedid' => null,
        'emdid' => null,
        'iniid' => null,
        'iedstatus' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'iedid' => array(  'Digits'  ),
            'emdid' => array(  'Digits'  ),
            'iniid' => array(  'Digits'  ),
            'iedstatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
        );
    }

    /**
     * @return bool
     * @throws Exception
     */
    public function deleteIniciativas($emeid)
    {
        $strSQL = "
            DELETE FROM emenda.iniciativaemendadetalhe WHERE emdid IN (
              SELECT emdid FROM emenda.emendadetalhe WHERE emeid= %d and emdstatus = 'A'
            ) AND iniid = %d
        ";

        $this->executar(sprintf($strSQL, $emeid, $this->arAtributos['iniid']));
        return $this->commit();
    }

    /**
     * @return string
     */
    public function getIniciativas($emeid)
    {
       $responsavel =  retornaDadosEmenda($emeid)['responsavel'];
       $AND = "AND resdsc = '{$responsavel}'";

        $strSQL = "
            SELECT
                i.iniid                                                       AS codigo,
                i.iniid || ' - '|| ininome || ' ('||resdsc||') GND: '||gndcod AS descricao
            FROM emenda.iniciativa i
            JOIN emenda.iniciativaresponsavel ir ON ir.iniid = i.iniid
            JOIN emenda.responsavel r ON ir.resid = r.resid
            WHERE inistatus ='A' AND irestatus = 'A' {$AND}
            ORDER BY 2
        ";

        return $strSQL;
    }
}
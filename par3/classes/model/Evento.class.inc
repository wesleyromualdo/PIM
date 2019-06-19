<?php
/**
 * Classe de mapeamento da entidade par3.evento
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 22-12-2015
 * @link     no link
 */



/**
 * Evento
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 22-12-2015
 * @link     no link
 */
class Par3_Model_Evento extends Modelo
{
    const K_TIPO_GERAL                = 'G';
    const K_TIPO_REGIONAL             = 'R';
    const K_TIPO_ESTADUAL             = 'E';
    const K_TIPO_MUNICIPAL            = 'M';
    const K_TIPO_INSTRUMENTO_UNIDADE  = 'I';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.evento';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'eveid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'evtid' => array('tabela' => 'par3.eventotipo', 'pk' => 'evtid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'eveid' => null,
        'evetitulo' => null,
        'evedsc' => null,
        'evecor' => null,
        'evedatainicio' => null,
        'evedatafim' => null,
        'evtid' => null,
        'evestatus' => null,
        'evetipo' => null,
        'inuid' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'eveid' => array(  'Digits'  ),
            'evetitulo' => array(  new Zend_Validate_StringLength(array('max' => 300))  ),
            'evedsc' => array( 'allowEmpty' => true ),
            'evecor' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 7))  ),
            'evedatainicio' => array(  ),
            'evedatafim' => array( 'allowEmpty' => true ),
            'evtid' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)


    public function recuperarLocalizacao()
    {
        $aRetorno = array('regiao'=>array(),'estado'=>array(), 'municipio'=>array());
        if($this->eveid){
            $sql = "select ev.eveid, ev.evetipo,
                            case
                                    when ev.evetipo = 'R' then r.regcod
                                    when ev.evetipo = 'E' then e.regcod
                                    when ev.evetipo = 'M' then emun.regcod
                            end as regcod,
                            case
                                    when ev.evetipo = 'R' then null
                                    when ev.evetipo = 'E' then e.estuf
                                    when ev.evetipo = 'M' then m.estuf
                            end as estuf,
                            case
                                    when ev.evetipo = 'R' then null
                                    when ev.evetipo = 'E' then null
                                    when ev.evetipo = 'M' then m.muncod
                            end as muncod
                    from par3.eventolocalizacao el
                            inner join par3.evento ev on ev.eveid = el.eveid

                            -- Tipo Regional
                            left join territorios.regiao r on r.regcod::int = el.regcod

                            -- Tipo Estadual
                            left join territorios.estado e on e.estuf = el.estuf

                            -- Tipo Municipal
                            left join territorios.municipio m on m.muncod = el.muncod
                            left join territorios.estado emun on emun.estuf = m.estuf
                    where ev.eveid = {$this->eveid} ";

            $dados = $this->carregar($sql);
            $dados = $dados ? $dados : array();
            foreach ($dados as $dado) {
                $aRetorno['regiao'][trim($dado['regcod'])] = trim($dado['regcod']);
                $aRetorno['estado'][$dado['estuf']] = $dado['estuf'];
                $aRetorno['municipio'][$dado['muncod']] = $dado['muncod'];
            }
        }
        return $aRetorno;
    }
}//end Class
?>
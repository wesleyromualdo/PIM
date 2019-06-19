<?php
/**
 * Classe de mapeamento da entidade par3.auditoria_unidade
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 24-11-2015
 * @link     no link
 */



/**
 * Auditoria_unidade
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 24-11-2015
 * @link     no link
 */
class Par3_Model_AuditoriaUnidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.auditoria_unidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'aduid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
        'adutipo' => array('tabela' => 'par3.auditoria_tipo', 'pk' => 'adtid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'aduid' => null,
        'aducpf' => null,
        'adutipo' => null,
        'adudata' => null,
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
            'aduid' => array(  'Digits'  ),
            'aducpf' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'adutipo' => array( 'allowEmpty' => true, 'Digits'  ),
            'adudata' => array( 'allowEmpty' => true ),
            'inuid' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)

    public function verificarInicioPar($inuid){
        $sql = "select * from par3.auditoria_unidade where inuid = {$inuid} and adutipo = 1";
        $dados = $this->carregar($sql, 0, 30);

        return empty($dados) ? true : false;
    }

    public function verificarInicioDiagnostico($inuid){
        $sql = "select * from par3.auditoria_unidade where inuid = {$inuid} and adutipo = 2";
        $dados = $this->carregar($sql);

        return empty($dados) ? false : true;
    }

}//end Class
?>
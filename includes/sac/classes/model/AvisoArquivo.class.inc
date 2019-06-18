<?php
/**
 * Classe de mapeamento da entidade sac.avisoarquivo
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-03-2016
 * @link     no link
 */



/**
 * Sac_ModelAvisoarquivo
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-03-2016
 * @link     no link
 */
class Sac_Model_AvisoArquivo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sac.avisoarquivo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'avaid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'avaid' => null,
        'aviid' => null,
        'avadsc' => null,
        'arqid' => null,
        'avastatus' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'avaid' => array(  'Digits'  ),
            'avadsc' => array(  new Zend_Validate_StringLength(array('max' => 200))  ),
            'arqid' => array(  'Digits'  ),
            'avastatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
        );
    }//end getCamposValidacao($dados)

    public function pegarArqid($aviid){
        $sql = "select arqid,avadsc from $this->stNomeTabela where aviid = $aviid";
        $rows = $this->carregar($sql);
        return $rows;
    }
    public function excluirPorArqid($arqid){
        if ($arqid) {
                $sql = "DELETE FROM ".$this->stNomeTabela." WHERE arqid = ".$arqid.";";
                $this->executar($sql);
                $this->commit();
        }
        return true;
    }

}//end Class
?>
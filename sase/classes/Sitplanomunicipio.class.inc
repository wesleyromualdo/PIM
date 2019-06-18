<?php

/**
 * Sitplanomunicipio
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO MORAIS CARVALHO <eduardo.carvalho@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-11-2015
 * @link     no link
 */
 
 require_once APPRAIZ . "includes/library/simec/Listagem.php";
 
class Sitplanomunicipio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sase.sitplanomunicipio';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'spmid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'spmid' => null,
        'spmdsc' => null,
        'spccor' => null,
        'spcstatus' => null,
        'esdid' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'spmid' => array(  'Digits'  ),
            'spmdsc' => array(  new Zend_Validate_StringLength(array('max' => 100))  ),
            'spccor' => array(  new Zend_Validate_StringLength(array('max' => 20))  ),
            'spcstatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'esdid' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)


	public function getLista(){
		global $db;
		$sql = <<<DML
			select
				'<a style=\"cursor:pointer\" onclick=\"editSituacao( ' || spmid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>' as acao,
				spmdsc,
				'<span style="background: ' || spccor || '" class="elementoCor">&nbsp;&nbsp;&nbsp;</span>' as spccor
			from sase.sitplanomunicipio spc
			order by spmid
DML;
		$cabecalho   = array('A&ccedil;&atilde;o', 'Descri&ccedil;&atilde;o', 'Cor');
		$alinhamento = array('center', 'left', 'center');
		$larguras    = array('5%', '90%', '5%');
		$db->monta_lista($sql,$cabecalho,30, 5,'N','','N','listaSituacao',$larguras,$alinhamento);
	}
}//end Class
?>
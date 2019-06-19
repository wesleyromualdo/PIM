<?php
/**
 * Classe de mapeamento da entidade par3.tutorial
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 22-12-2015
 * @link     no link
 */

class Par3_Model_Tutorial extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.tutorial';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'evtid',
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
        'tutid' => null,
        'tutdescricao' => null,
        'tutdetalhe' => null,
        'tutstatus' => null,
        'tuttipo' => null,
        'arqid' => null
    );
    
    protected $arChaveEstrangeira = array(
    		'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid')
    );
    
    /**
     * Função excluirPorArqid
     *
     */
    public function excluirPorArqid( $arqid )
    {
    	if ($arqid) {
    		$sql = "DELETE FROM ".$this->stNomeTabela." WHERE arqid = ".$arqid.";";
    		$this->executar($sql);
    		$this->commit();
    	}
    	return true;
    }//end excluirPorArqid()

    /**
     * Função Anexar arquivo
     *
     */
    public function uploadArquivoTutorial(){

    	if( $_FILES["arquivo"]["name"] ){
    		   	
    		$file = new FilesSimec("tutorial");
    		
    		if(is_file($_FILES["arquivo"]["tmp_name"])){
    			
    			$arquivoSalvo = $file->setUpload($_FILES["arquivo"]["name"], "arquivo");
				
    			return true;
    		}
    	}
    }
}//end Class
?>
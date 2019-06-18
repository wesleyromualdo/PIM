<?php
/**
 * Classe de controle do tipo_entidade
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   Thiago Tasca <thiago.tasca@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 26-06-2018
 * @link     no link
 */



/**
 * Par3_Controller_TipoDocumentoHabilita
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 26-06-2018
 * @link     no link
 */
class Par3_Controller_TipoDocumentoSubstitutos
{
    private $model;

    public function __construct()
    {
        $dcsid = validarInteiro($_REQUEST['dcsid'])? $_REQUEST['dcsid']:'';
        $this->model = new Par3_Model_HabilitacaoTipoDocumentoSubstitutivos($dcsid);
  //      $this->modelHistorico = new Par3_Model_IniciativaIniciativasProgramasHistorico();
    }
    
    /**
     * Função gravar
     * - grava os dados
     *
     */

    public function salvar($arrPost, $tdcid)
    {
        // Inativa antes de inserir
       
        $inativaViculacoes = $this->model->inativarPorTipoDocumento($tdcid);
        //ver($arrPost); die();
        // Salvar
        if(is_array($arrPost['docvinculados']) ){
            foreach( $arrPost['docvinculados'] as $chave=>$dados ){
                if ($arrPost['dcsid']) {
                    $arrayDocumentoSubstitutos['dcsid'] = $arrPost['dcsid'];
                } else {
                    // Inclusão
                    $arrayDocumentoSubstitutos['dcsid'] = null;
                }
                $arrayDocumentoSubstitutos['tdcid'] = $tdcid;
                $arrayDocumentoSubstitutos['dcstdcidvinculado'] = $dados;
                $arrayDocumentoSubstitutos['dcsstatus'] = 'A';
                $arrayDocumentoSubstitutos['dcscpfalteracao'] = $_SESSION['usucpf'];
                $arrayDocumentoSubstitutos['dcsdataalteracao'] = date('Y-m-d H:i:s');
                
               if($dados){
                $this->model = new Par3_Model_HabilitacaoTipoDocumentoSubstitutivos($arrayDocumentoSubstitutos['dcsid']);
                $this->model->popularDadosObjeto($arrayDocumentoSubstitutos);
                $id = $this->model->salvar();
                $this->model->commit();
               }
                
            }
           // echo 'entrou'; die();
            return $id;
        }else{
           // ver('Entrou'); die();
            return true;
        }
    	
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($dcsid)
    {
        global $url;
        $id = $dcsid;
        $this->model = new Par3_Model_HabilitacaoTipoDocumentoSubstitutivos($id);

            try{
                 $this->model->dcsstatus = 'I';
                 $sucesso = $this->model->salvar();
                 $this->model->commit();
                 return $sucesso;
            } catch (Simec_Db_Exception $e) {
                return 'erro';
            }

    }


    public function recuperar($dcsid)
    {
        $id = $dcsid;
        $this->model = new Par3_Model_HabilitacaoTipoDocumentoSubstitutivos($id);

        return $documentosVinculados;
    }
    
    public function recuperarDocumentosVinculados()
    {
        return $this->model;
    }
    
    public function pegarSelectCombo()
    {
        return $this->model->pegarSelectCombo();
    }

}
?>
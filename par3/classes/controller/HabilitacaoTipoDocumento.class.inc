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
class Par3_Controller_TipoDocumentoHabilita
{
    private $model;
    private $controleSubstitutivos;

    public function __construct()
    {
        $tdcid = validarInteiro($_REQUEST['tdcid'])? $_REQUEST['tdcid']:'';
        $this->model = new Par3_Model_HabilitacaoTipoDocumento($tdcid);
        $this->controleSubstitutivos = new Par3_Controller_TipoDocumentoSubstitutos();
  
    }
    
    public function form($arrPost)
    {
        global $simec;
        
        $sqlDocumentosVinculados = $this->model->pegarSelectCombo($this->model->tdcid);
        $this->controleSubstitutivos = new Par3_Model_HabilitacaoTipoDocumentoSubstitutivos();
        /*
        $documentosVinculados = $this->controleSubstitutivos->recuperarPorTipoDocumento($this->model->tdcid);
        if(is_array( $documentosVinculados )){
            foreach ($documentosVinculados as $key => $value) { 
                $_REQUEST['docvinculados'][] = $value['dcsid'];
            }
        }
        */

        $arrDocumentosVinculados = Array();
        
        if($this->model->tdcid){
            $mRelacao = new Par3_Model_HabilitacaoTipoDocumentoSubstitutivos();
            $arrTipoNaturezasSalvas = $mRelacao->getTipoDocumentosVinculados($this->model->tdcid);
            $arrSalvas = postgres_to_php_array($arrTipoNaturezasSalvas['tipos']);
        }
        
       // ver($_POST['docvinculados']); die();
       // $_REQUEST['situacaoempenho']? $_REQUEST['situacaoempenho'] : 0
        
       // $teste = $docvinculados[]

        echo '<input type="hidden" name="tdcid" value="'.$this->model->tdcid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->tdcid.'" class="form-control"></div>';
        echo '</div>';
        
        echo $simec->input('tdcnome', 'Nome do Tipo de Documento', $this->model->tdcnome, array('maxlength' => '140'));
        echo $simec->radio('tdcvigencia', 'Vigência',  $this->model->tdcvigencia ? $this->model->tdcvigencia : 'S', array('S' => 'Sem Vigência', 'N' => 'Não Obrigatório', 'O' => 'Obrigatório'));
        echo $simec->textarea('tdcdica', 'Dica', $this->model->tdcdica, array('maxlength' => '400'));
        echo $simec->select('docvinculados[]', 'Documentos Vinculados - Substitutivos', $arrSalvas, $sqlDocumentosVinculados, array('maxlength' => '255','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '9'));
    }


    
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        // Alteraçao
        if ($arrPost['tdcid']) {
            $arrayTipoDocumento['tdcid']             = $arrPost['tdcid'];

        } else {
            // Inclusão
            $arrayTipoDocumento['tdcid'] = null;

        }
        $arrayTipoDocumento['tdcnome']           = trim(($arrPost['tdcnome']));
        $arrayTipoDocumento['tdcdica']           = trim(($arrPost['tdcdica']));
        $arrayTipoDocumento['tdcvigencia']         = $arrPost['tdcvigencia'];
        $arrayTipoDocumento['tdcstatus']         = 'A';
        $this->model = new Par3_Model_HabilitacaoTipoDocumento($arrayTipoDocumento['tdcid']);
        $this->model->popularDadosObjeto($arrayTipoDocumento);
        $id = $this->model->salvar();
        $this->model->commit();
    	
    	return $id;
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($request)
    {
        global $url;
        $id = $request['tdcid'];
        $this->controleSubstitutivos = new Par3_Model_HabilitacaoTipoDocumentoSubstitutivos();
        $documentosVinculados = $this->controleSubstitutivos->recuperarPorTipoDocumento($id);
        if(is_array($documentosVinculados)){
            foreach ($documentosVinculados as $key => $value) {
               $dcsid = $value['dcsid'];
               $this->controleSubstitutivos = new Par3_Controller_TipoDocumentoSubstitutos();
               $inativadoDocSubstituto = $this->controleSubstitutivos->inativar($dcsid);
            }
        }

            try{
                 $this->model->tdcstatus = 'I';
                 $sucesso = $this->model->salvar();
                 $this->model->commit();
                 return $sucesso;
            } catch (Simec_Db_Exception $e) {
                return 'erro';
            }

    }


    public function recuperar()
    {
        return $this->model;
    }
    
    public function listar($arrPost)
    { //echo 'sadfsdf'; return false;
        global $disabled;

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'tdcid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'tdcnome';
                break;
            case 'documentossubstitutos':
                $arrQuery['ordenacao']['campo'] = 'tdcnome';
                break;
            case 'tdcvigencia':
                $arrQuery['ordenacao']['campo'] = 'tdcvigencia';
                break;
            default:
                $arrQuery['ordenacao'] = '';
            break;
        }
       
//ver($this->model,d);
        $sql = $this->model->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Tipo de Documento', 'Vigência Obrigatória?', 'Texto de dica');
        $esconderColunas = array('id','qtd');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
        }
        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setId('listTipoObjeto');
        $listagem->setCabecalho($cabecalho);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->addCallbackDeCampo('tpestatus','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'remover');
            $listagem->addAcao('edit', 'editar');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
    
    public function pegarSelectCombo()
    {
        return $this->model->pegarSelectCombo();
    }

}
?>
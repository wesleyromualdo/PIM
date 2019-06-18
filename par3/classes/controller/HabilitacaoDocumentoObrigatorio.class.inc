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
class Par3_Controller_TipoControleDocumentoHabilita
{
    private $model;

    public function __construct()
    {
         $dcoid = validarInteiro($_REQUEST['dcoid'])? $_REQUEST['dcoid']:'';
        $this->model = new Par3_Model_HabilitacaoDocumentoObrigatorio($dcoid);
  //      $this->modelHistorico = new Par3_Model_IniciativaIniciativasProgramasHistorico();
    }
    
    public function form($arrPost)
    {
        global $simec;

        echo '<input type="hidden" name="dcoid" value="'.$this->model->dcoid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->dcoid.'" class="form-control"></div>';
        echo '</div>';
    }
    
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {   
        
        $arrayTipoVinculaDocumento['cteid'] = $arrPost['categoriaEntidade'];
        $arrSelecaoForm = $arrPost['selecao'];
        if( !is_array($arrSelecaoForm) ){
            return 'erro1'; //false; //'Necessário selecionar algum dado para salvar.'; // false;
        }
        foreach ($arrSelecaoForm as $value) {
            if(!$value['form'] && ($value['obrigatorio'] || $value['habilita'] ) ){
                return 'erro2';  //false;
            }
        }


        $delecaoPorNatureza = $this->model->inativarPorCategoriaEntidade($arrayTipoVinculaDocumento['cteid']);
        foreach ($arrSelecaoForm as $chave => $value) {
            if($value['obrigatorio'] == 'on'){
                $arrayTipoVinculaDocumento['dcoobrigatorio'] = 'true';
            }else{
                $arrayTipoVinculaDocumento['dcoobrigatorio'] = 'false';
            }
            if($value['habilita'] == 'on'){
                $arrayTipoVinculaDocumento['dcohabilitacao'] = 'true';
            }else{
                $arrayTipoVinculaDocumento['dcohabilitacao'] = 'false';
            }
            
            $arrayTipoVinculaDocumento['dcoid'] = null;
            $arrayTipoVinculaDocumento['dcostatus'] = 'A';
            $arrayTipoVinculaDocumento['tdcid'] = $chave;
            $arrayTipoVinculaDocumento['dcocpfalteracao'] = $_SESSION['usucpf'];
            $arrayTipoVinculaDocumento['dcodataalteracao'] = date('Y-m-d H:i:s');

            $this->model = new Par3_Model_HabilitacaoDocumentoObrigatorio();
            $this->model->popularDadosObjeto($arrayTipoVinculaDocumento);
            //ver($this->model->popularDadosObjeto($arrayTipoVinculaDocumento)); die();
            $id = $this->model->salvar();
            $this->model->commit();
            
        }

        if($id && $delecaoPorNatureza ){
            $this->model->commit();
        }else{
            alert('Ocorreu um erro ao tentar salvar os dados.');
        }

        return $id;
       
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['tdcid'];
        

            try{
                 $this->model->tdcstatus = 'I';
                 $sucesso = $this->model->salvar();
                 $this->model->commit();
                 return $sucesso;
            } catch (Simec_Db_Exception $e) {
                return 'erro';
            }

    }

    public function recuperarDocTipoObrigatorio($id = null)
    {   
       
        $sql = $this->model->sqlRecuperarDocTipoObrigatorio($id);
        $dados = $this->model->carregar($sql);

        return $dados;
    }

    public function listarTiposDocumentos()
    {
        $sql    = $this->model->sqllistarTiposDocumentos();
        $dados  = $this->model->carregar($sql);

        return $dados;
    }


    public function recuperar()
    {
        return $this->model;
    }

    
    
    public function listar($arrPost)
    {
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
            default:
                $arrQuery['ordenacao'] = '';
            break;
        }
//ver($this->model,d);
        $sql = $this->model->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Tipo de Documento');
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
    
    public function pegarDocumentosPorTipo($cteid){
        
        return $this->model->pegarDocumentosPorTipo($cteid);
    }

}
?>
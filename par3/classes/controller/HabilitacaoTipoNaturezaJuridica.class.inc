<?php
/**
 * Classe de controle do tipo_entidade
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   VICTOR BENZI <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 26-06-2018
 * @link     no link
 */



/**
 * Par3_Controller_TipoNaturezaJuridicaHabilita
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 26-06-2018
 * @link     no link
 */
class Par3_Controller_TipoNaturezaJuridicaHabilita
{
    private $model;

    public function __construct()
    {
        $tnjid = validarInteiro($_REQUEST['tnjid'])? $_REQUEST['tnjid']:'';
        $this->model = new Par3_Model_HabilitacaoTipoNaturezaJuridica($tnjid);
  //      $this->modelHistorico = new Par3_Model_IniciativaIniciativasProgramasHistorico();
    }
    
    public function form($arrPost)
    {
        global $simec;

        echo '<input type="hidden" name="tnjid" value="'.$this->model->tnjid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->tnjid.'" class="form-control"></div>';
        echo '</div>';
        
        echo $simec->input('tnjcodigo', 'Código do Tipo da Natureza Jurídica', $this->model->tnjcodigo, array('maxlength' => '20'));
        echo $simec->input('tnjnome', 'Nome do Tipo da Natureza Jurídica', $this->model->tnjnome, array('maxlength' => '80'));
    }
    
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {

        // Alteraçao
        if ($arrPost['tnjid']) {
            $arrayTipoNatureza['tnjid']             = $arrPost['tnjid'];
//            $arrayTipoNatureza['usucpfalteracao']   = $_SESSION['usucpf'];
//            $arrayTipoNatureza['tpedtalteracao']    = date('Y-m-d H:i:s');
        } else {
            // Inclusão
            $arrayTipoNatureza['tnjid'] = null;
//            $arrayTipoNatureza['usucpfinclusao']    = $_SESSION['usucpf'];
//            $arrayTipoNatureza['tpedtinclusao']     = date('Y-m-d H:i:s');
        }
        $arrayTipoNatureza['tnjcodigo']         = trim(($arrPost['tnjcodigo']));
    	$arrayTipoNatureza['tnjnome']           = trim(($arrPost['tnjnome']));
        $arrayTipoNatureza['tnjstatus']         = 'A';
        
        $this->model = new Par3_Model_HabilitacaoTipoNaturezaJuridica($arrayTipoNatureza['tnjid']);
        $this->model->popularDadosObjeto($arrayTipoNatureza);
        $id = $this->model->salvar();
        $this->model->commit();
    	
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
        $id = $_GET['tnjid'];
        
//        include_once APPRAIZ . 'par3/classes/controller/HabilitacaoNaturezaJuridica.class.inc';
//        $controllerTipoNaturezaJuridica = new Par3_Controller_NaturezaJuridicaHabilita();
//        
//        $verifica = $controllerTipoNaturezaJuridica->verificaTipoNaturezaUtilizada($id);
//        
//        if($verifica){
            try{
                 $this->model->tnjstatus = 'I';
                 $sucesso = $this->model->salvar();
                 $this->model->commit();
                 return $sucesso;
            } catch (Simec_Db_Exception $e) {
                return 'erro';
            }
//        } else {
//            return 'erro';
//        }
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
                $arrQuery['ordenacao']['campo'] = 'tnjid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'tnjnome';
                break;
            case 'codigonatureza':
                $arrQuery['ordenacao']['campo'] = 'tnjcodigo';
                break;
            default:
                $arrQuery['ordenacao'] = '';
            break;
        }
//ver($this->model,d);
        $sql = $this->model->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Código do Tipo de Natureza', 'Tipo de Natureza');
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
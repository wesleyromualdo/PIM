<?php
/**
 * Classe de controle do NaturezaJuridica
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   VICTOR BENZI <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 22-09-2018
 * @link     no link
 */



/**
 * Par3_Controller_NaturezaJuridicaHabilita
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 26-06-2018
 * @link     no link
 */
class Par3_Controller_NaturezaJuridicaHabilita
{
    private $model;

    public function __construct()
    {
        $ntjid = validarInteiro($_REQUEST['ntjid'])? $_REQUEST['ntjid']:'';
        $this->model = new Par3_Model_HabilitacaoNaturezaJuridica($ntjid);
        $this->modelNaturezaJuridicaRelacionada = new Par3_Model_HabilitacaoNaturezaJuridicaRelacionada();
        
    }
    
    public function form($arrPost)
    {
        global $simec;
        
        include_once APPRAIZ . 'par3/classes/controller/HabilitacaoTipoNaturezaJuridica.class.inc';
        $controllerTipoNaturezaJuridica = new Par3_Controller_TipoNaturezaJuridicaHabilita();
        
        $arrTipoNaturezasSalvas = Array();
        
        if($arrPost['ntjid']){
            $mRelacao = new Par3_Model_HabilitacaoNaturezaJuridicaRelacionada();
            $arrTipoNaturezasSalvas = $mRelacao->getTipoNaturezasRelacionadas($arrPost['ntjid']);
            $arrSalvas = postgres_to_php_array($arrTipoNaturezasSalvas['tipos']);
        }
        
        echo '<input type="hidden" name="ntjid" value="'.$this->model->ntjid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->ntjid.'" class="form-control"></div>';
        echo '</div>';
        
        echo $simec->input('ntjnome', 'Descrição da Natureza Jurídica', $this->model->ntjnome, array('maxlength' => '80'));
        echo $simec->select('tnjid[]', 'Tipo da Natureza Jurídica', $arrSalvas, $controllerTipoNaturezaJuridica->pegarSelectCombo(), array('maxlength' => '255','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '9'));
    }
    
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        // Alteraçao
        if ($arrPost['ntjid']) {
            $arrayNatureza['ntjid']             = $arrPost['ntjid'];
//            $arrayNatureza['usucpfalteracao']   = $_SESSION['usucpf'];
//            $arrayNatureza['tpedtalteracao']    = date('Y-m-d H:i:s');
        } else {
            // Inclusão
            $arrayNatureza['ntjid'] = null;
//            $arrayNatureza['usucpfinclusao']    = $_SESSION['usucpf'];
//            $arrayNatureza['tpedtinclusao']     = date('Y-m-d H:i:s');
        }
    	$arrayNatureza['ntjnome']           = trim(($arrPost['ntjnome']));
        $arrayNatureza['ntjstatus']         = 'A';
        
        $this->model = new Par3_Model_HabilitacaoNaturezaJuridica($arrayNatureza['ntjid']);
        $this->model->popularDadosObjeto($arrayNatureza);
        $id = $this->model->salvar();
        $this->model->commit();
        
        // Após salvar, salvo os tipos de natureza juridica selecionados
        $mRelacao = new Par3_Model_HabilitacaoNaturezaJuridicaRelacionada();
        $mRelacao->salvarRelacao($id, $arrPost['tnjid']);
    	
    	return $id;
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($arrPost)
    {
        global $url;
        $id = $arrPost['ntjid'];
        
        try{
             $this->model->ntjstatus = 'I';
//             $this->model->tpedtalteracao = date('Y-m-d H:i:s');
//             $this->model->usucpfalteracao = $_SESSION['usucpf'];

             $sucesso = $this->model->salvar();
             $this->model->commit();
             
             $mRelacao = new Par3_Model_HabilitacaoNaturezaJuridicaRelacionada();
             $mRelacao->inativarRelacao($id);
        
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
    {
        global $disabled;

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'ntjid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'ntjnome';
                break;
            case 'tipo':
                $arrQuery['ordenacao']['campo'] = 'tipo';
                break;
            default:
                $arrQuery['ordenacao'] = '';
            break;
        }
//ver($this->model,d);
        $sql = $this->model->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Natureza Jurídica', 'Tipo da Natureza Jurídica');
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
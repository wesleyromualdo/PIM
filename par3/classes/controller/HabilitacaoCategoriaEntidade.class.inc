<?php
/**
 * Classe de controle do Categoria Entidade
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   Thiago Tasca <thiagotasca@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2018
 * @link     no link
 */



/**
 * Par3_Controller_CategoriaEntidadeHabilita
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 26-06-2018
 * @link     no link
 */
class Par3_Controller_CategoriaEntidadeHabilita
{
    private $model;

    public function __construct()
    {
        $cteid = validarInteiro($_REQUEST['cteid'])? $_REQUEST['cteid']:'';
        $this->model = new Par3_Model_HabilitacaoCategoriaEntidade($cteid);
//        $this->modelCategoriaNaturezaJuridica = new Par3_Model_HabilitacaoCategoriaNaturezaJuridica();
    }
    
    public function form($arrPost)
    {
        global $simec;
        
        include_once APPRAIZ . 'par3/classes/controller/HabilitacaoNaturezaJuridica.class.inc';
        $controllerNaturezaJuridica = new Par3_Controller_NaturezaJuridicaHabilita();
        
        $arrNaturezasSalvas = Array();
        
        if($arrPost['cteid']){
            include_once APPRAIZ . 'par3/classes/model/HabilitacaoCategoriaNaturezaJuridica.class.inc';
            $mRelacao = new Par3_Model_HabilitacaoCategoriaNaturezaJuridica();
            $arrNaturezasSalvas = $mRelacao->getNaturezasRelacionadas($arrPost['cteid']);
            $arrSalvas = postgres_to_php_array($arrNaturezasSalvas['naturezas']);
        }

        echo '<input type="hidden" name="cteid" value="'.$this->model->cteid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->cteid.'" class="form-control"></div>';
        echo '</div>';
        
        echo $simec->input('ctecodigo', 'Código da Categoria', $this->model->ctecodigo, array('maxlength' => '20'));
        echo $simec->input('ctenome', 'Nome da Categoria', $this->model->ctenome, array('maxlength' => '80'));
        echo $simec->select('ntjid[]', 'Natureza Jurídica', $arrSalvas, $controllerNaturezaJuridica->pegarSelectCombo(), array('maxlength' => '255','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '9'));
    }
    
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {

        // Alteraçao
        if ($arrPost['cteid']) {
            $arrayCategoria['cteid']             = $arrPost['cteid'];
//            $arrayCategoria['usucpfalteracao']   = $_SESSION['usucpf'];
//            $arrayCategoria['tpedtalteracao']    = date('Y-m-d H:i:s');
        } else {
            // Inclusão
            $arrayCategoria['cteid'] = null;
//            $arrayCategoria['usucpfinclusao']    = $_SESSION['usucpf'];
//            $arrayCategoria['tpedtinclusao']     = date('Y-m-d H:i:s');
        }

        $arrayCategoria['ctecodigo']         = trim(($arrPost['ctecodigo']));
    	$arrayCategoria['ctenome']           = trim(($arrPost['ctenome']));
        $arrayCategoria['ctestatus']         = 'A';
        //ver($arrayCategoria);
        //die();
        
        $this->model = new Par3_Model_HabilitacaoCategoriaEntidade($arrayCategoria['cteid']);
        $this->model->popularDadosObjeto($arrayCategoria);
        $id = $this->model->salvar();
        $this->model->commit();
        
        // Após salvar, salvo as naturezas juridicas selecionados
        $mRelacao = new Par3_Model_HabilitacaoCategoriaNaturezaJuridica();
        $mRelacao->salvarRelacao($id, $arrPost['ntjid']);
    	
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
        $id = $_GET['cteid'];
        
//        include_once APPRAIZ . 'par3/classes/controller/HabilitacaoNaturezaJuridica.class.inc';
//        $controllerTipoNaturezaJuridica = new Par3_Controller_NaturezaJuridicaHabilita();
//        
//        $verifica = $controllerTipoNaturezaJuridica->verificaTipoNaturezaUtilizada($id);
//        
//        if($verifica){
            try{
                 $this->model->ctestatus = 'I';
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
                $arrQuery['ordenacao']['campo'] = 'cteid';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'ctenome';
                break;
            case 'codigonatureza':
                $arrQuery['ordenacao']['campo'] = 'ctecodigo';
                break;
            default:
                $arrQuery['ordenacao'] = '';
            break;
        }
//ver($this->model,d);
        $sql = $this->model->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Código da Categoria', 'Categoria da Entidades');
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
    
    // Carrega Combo de Categorias
    public function pegarSelectCombo()
    {
        return $this->model->pegarSelectCombo();
    }
    
    public function pegarSelectComboTipo($tipo){
        return $this->model->pegarSelectComboTipo($tipo);
    }

    public function recuperarCategoriaEntidade($entid){
        return $this->model->recuperarCategoriaEntidade($entid);
    }

}
?>
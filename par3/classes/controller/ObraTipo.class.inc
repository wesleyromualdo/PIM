<?php
/**
 * Classe de controle do  obra_tipo
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */


/**
 * Par3_Controller_Obra_tipo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-07-2017
 * @link     no link
 */
class Par3_Controller_ObraTipo extends Controle
{
    public $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ObraTipo($_REQUEST['otpid']);
        $this->modelHistorico = new Par3_Model_ObraTipoHistorico();
        $this->modelCategoriaObra = new Par3_Model_ObraCategoria();
        $this->ensinoEtapa = new Par3_Model_EnsinoEtapa();
        $this->ObraTipoEtapaEnsino = new Par3_Model_ObraTipoEtapaEnsino();
    }

    public function recuperar($id = null)
    {
        if($id){
           return new Par3_Model_ObraTipo($id);
        }
        return $this->model;
    }


    public function pegarSelectCombo()
    {
        return $this->model->pegarSQLSelectCombo($tipid);
    }

    public function pegarSelectCategoriasComboArray($octid = null, $executar = true)
    {
        if($executar){
            $sql = $this->model->pegarSQLSelectComboCategorias($octid);
            return $this->model->carregar($sql);
        }
        return $this->model->pegarSQLSelectComboCategorias($octid);
    }

    public function getDetalhe()
    {
        if($this->model->otpdetalhe != ''){
        	return $this->model->otpdetalhe;
        }else{
            return 'Detalhamento não informado.';
        }
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {

        global $url;
        $this->model->popularDadosObjeto();
        $this->model->otpstatus = 'A';
        $url .= '&menu=listar_tipo_obra';

        $acao = Par3_Model_ObraTipo::CREATE;
        if ($_POST['otpid'])
            $acao = Par3_Model_ObraTipo::UPDATE;


        try {
            $id = $this->model->salvar(true, true, array('otptipoampliacao', 'otptipoprojeto', 'otpalunosatendidos'));
            $this->model->commit();

            $this->ObraTipoEtapaEnsino->salvarEnsinoTipoObra($id,$_POST['otpensinoetapa']);

            $model = new Par3_Model_ObraTipo($id);
            $this->modelHistorico->gravarHistorico($model, $acao);

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if ($id) {
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }

    public function getComboTipologiaObras2()
    {
        return $this->model->getComboTipologiaObras2();
    }

    public function getComboGrupoItens()
    {
        return $this->model->getComboGrupoItens();
    }

    public function getDescricao()
    {
        return $this->model->otpdsc;
    }

    public function getComboCategoriaObras()
    {
        $dados = $this->modelCategoriaObra->recuperarTodosFormatoInput('octdsc', array("octstatus = 'A' "));
        $arr = array();
        foreach ($dados as $valor) {
            $arr[$valor['codigo']] = $valor['descricao'];
        }
        return $arr;
    }

    public function listar($arrPost = array())
    {
        global $disabled;

        $arrQuery = $this->model->getOrdenacaoLista($arrPost);

        $sql = $this->model->montarSQLSimples($arrQuery);

        $disabled = 'disabled';

        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        if ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') {
            $cabecalho = array('QTD', 'Código', 'Descrição', 'Tipologia Obras', 'Situação', 'Esfera', 'Categoria', 'Alunos Atendidos', 'Detalhe', 'Tipo de Projeto');
            $esconderColunas = array('id');
        }else{
            $cabecalho = array('Código', 'Descrição', 'Tipologia Obras', 'Situação');
            $esconderColunas = array('id', 'qtd', 'esfera', 'categoria', 'alunosatendidos', 'otpdetalhe', 'otptipoprojeto');
        }

        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;

        if ($arrPost['requisicao'] == 'imprimir') {
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
            $disabled = true;
        }//desabilitar opções para a impressão

        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setId('listTipoObra');
        $listagem->turnOnPesquisator();
        $listagem->setCabecalho($cabecalho);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        if ($disabled == '') {
            if($arrPost['remover'] != 'N') $listagem->addAcao('remove', 'remover');
            $listagem->addAcao('edit', 'editar');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    public function remover($arrPost)
    {
        $array['otpid'] = $arrPost['otpid'];
        $array['otpstatus'] = 'I';

        try {
            $this->model->popularDadosObjeto($array);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelObraTipo = new Par3_Model_ObraTipo($arrPost['otpid']);

            $this->modelHistorico->gravarHistorico($modelObraTipo, Par3_Model_ObraTipo::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    public function getComboCategoriaAmpliacao()
    {
        return $this->modelCategoriaObra->getComboCategoriaAmpliacao();
    }

    public function getComboEnsinoEtapa()
    {
        return $this->ensinoEtapa->pegarSQLSelectCombo();
    }

    public function getSQLSelectComboNivelEtapa()
    {
        if(!empty($_REQUEST['otpid'])){
            return $this->ObraTipoEtapaEnsino->pegarSQLSelectComboNivelEtapa($_REQUEST['otpid']);
        }else{
            return "";
        }
        
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param null $otpid
     * @return string
     * @description 'Esta função retorna o sql para ser utilizado no simec->select()'
     */
    public function  getSQLInputTipoObrasTipo($otpid = null){
        return $this->model->getSQLInputTipoObrasTipo($otpid);
    }



}

?>
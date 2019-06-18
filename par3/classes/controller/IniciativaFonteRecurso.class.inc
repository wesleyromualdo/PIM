<?php
/**
 * Classe de controle do  iniciativa_fonte_recurso
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   HEMERSON EXPEDITO DORNELAS MORAIS <hemerson.moraiss@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 09-10-2017
 * @link     no link
 */


/**
 * Par3_Controller_Iniciativa_fonte_recurso
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 09-10-2017
 * @link     no link
 */
class Par3_Controller_IniciativaFonteRecurso
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaFonteRecurso($_GET['ifrid']);
        $this->modelHistorico = new Par3_Model_IniciativaFonteRecursoHistorico();
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        global $url;
        $url .= '&ifrid=' . $this->model->ifrid;


        //define acão para guardar em histórico
        $acao = Par3_Model_IniciativaFonteRecursoHistorico::CREATE;
        if ($arrPost['ifrid']) {
            $arrayFonteRecurso['ifrid'] = trim($arrPost['ifrid']);
            $acao = Par3_Model_IniciativaFonteRecursoHistorico::UPDATE;
        }

        $arrayFonteRecurso['inplintid'] = $arrPost['inplintid'];
        $arrayFonteRecurso['ifrcodigofonte'] = trim(($arrPost['ifrcodigofonte']));;
        $arrayFonteRecurso['ifrdescicaofonte'] = trim(($arrPost['ifrdescicaofonte']));
        $arrayFonteRecurso['ifrano'] = trim($arrPost['ifrano']);

        if($this->verificaDuplicidade($arrayFonteRecurso) > 0 ){

            return 'erro';
        }
        try {

            $this->model->popularDadosObjeto($arrayFonteRecurso);
            $sucesso = $this->model->salvar();
            $this->model->commit();
            $this->modelHistorico->gravarHistorico($this->model, $acao);
            $this->modelHistorico->commit();

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        if ($sucesso) {
            return (int)$sucesso;
        }
        return 'erro';
    }


    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($arrPost)
    {
        global $url;
        $id = $arrPost['ifrid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&ifrid={$id}";
        $iniciativa_fonte_recurso = new Par3_Model_IniciativaFonteRecurso($id);
        try {
            $iniciativa_fonte_recurso->ifrstatus = 'I';
            $iniciativa_fonte_recurso->ifrsituacao = 'I';
            $iniciativa_fonte_recurso->ifrdtinativacao = date('Y-m-d H:i:s');
            $iniciativa_fonte_recurso->ifrusucpfinativacao = $_SESSION['usucpf'];

            $sucesso = $iniciativa_fonte_recurso->salvar();
            $iniciativa_fonte_recurso->commit();

            $this->modelHistorico->gravarHistorico($iniciativa_fonte_recurso, 'DELETE');
            $this->modelHistorico->commit();

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        if ($sucesso) {
            return (int)$sucesso;
        }
        return 'erro';

    }

    public function listar($arrPost)
    {

        global $disabled;

        $arrQuery = $arrPost;
        $sql = $this->model->montarSQLSimples($arrQuery);
        $disabled = 'disabled';

        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('Código', 'Plano Interno', 'PTRES', 'Fonte', 'Descrição', 'Ano', 'Situação');

        if ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') {

            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
            $disabled = true;
        }

        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if ($arrPost['requisicao'] == 'imprimir') {
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
            $disabled = true;
        }
        ///desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('modalidade');
        $listagem->esconderColunas('ifrid');
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

        $listagem->setTamanhoPagina(50);


        $listagem->addCallbackDeCampo('ifrsituacao', 'formataStatusSwuitchery');

        if ($arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls') {
            $listagem->addCallbackDeCampo('ifrsituacao', 'formata_status');
        }


        if ($disabled == '') {
            $listagem->addAcao('remove', 'removerFonteRecurso');
            $listagem->addAcao('edit', 'editarFonteRecurso');
            $listagem->turnOnPesquisator();
        }

        $listagem->render($tratamentoListaVazia);
    }

    public function recuperar($arrDados)
    {
        global $db;
        $sql = "SELECT * FROM par3.iniciativa_fonte_recurso WHERE ifrid = {$arrDados['ifrid']}";
        $result = $db->executar($sql);
        $array = pg_fetch_assoc($result);
        return $array;

    }

    public function alteraSituacao($arrDados)
    {
        global $db;

        if ($arrDados['situacao'] == 'A') {
            $situacao = 'I';
        } else if ($arrDados['situacao'] == 'I') {
            $situacao = 'A';
        }
        $sql = "UPDATE  par3.iniciativa_fonte_recurso  set ifrsituacao = '{$situacao}' WHERE ifrid = {$arrDados['ifrid']}";
        $db->executar($sql);
        $db->commit();

    }


    public function verificaDuplicidade($arrayFonteRecurso)
    {
        global $db;
        $condigoFont = $arrayFonteRecurso['ifrcodigofonte'];
        $sql = "SELECT * FROM  par3.iniciativa_fonte_recurso WHERE inplintid = {$arrayFonteRecurso['inplintid']} 
                AND ifrcodigofonte = '{$condigoFont}' AND ifrano = {$arrayFonteRecurso['ifrano']} AND ifrstatus='A'";
        return pg_num_rows($db->executar($sql));

    }


}

?>
<?php
/**
 * Interface de edição dos dados da emenda.
 *
 * @version $Id: editarsolicitacao.inc 109158 2016-03-14 20:36:42Z maykelbraz $
 */
$fm = new Simec_Helper_FlashMessage('solicitacoes');
include_once APPRAIZ . 'includes/workflow.php';
$solicitacao = new Spoemendas_Model_Solicitacoes();
$solicitacaoEmenda = new Spoemendas_Model_SolicitacoesEmendas();
$anexo = new Spoemendas_Model_Anexogeral();
/*
 * Carrega o registro atual
 */
if (isset($_REQUEST['solid']) && $_REQUEST['solid'] <> '') {
    $solicitacao->carregarPorId($_REQUEST['solid']);
    $dados = $solicitacao->getDados();
}
if (isset($_REQUEST['requisicao'])) {
    switch ($_REQUEST['requisicao']) {
        case 'salvar':
            if ($_REQUEST['geral']['solid'] == '') {
                $_REQUEST['geral']['docid'] = wf_cadastrarDocumento(WF_TPDID_SPOEMENDAS_SOLICITACOES, 'Solicitação de informação (SPO - Emendas Parlamentares)');
            }
            if ($_REQUEST['geral']['emeid'] == '')
                $_REQUEST['geral']['emeid'] = null;
            if ($_REQUEST['geral']['autid'] == '')
                $_REQUEST['geral']['autid'] = null;
            $solicitacao->popularDadosObjeto($_REQUEST['geral']);
            $solid = $solicitacao->salvar();
            if (is_array($_REQUEST['geral']['emeid'])) {
                if ($solid) {
                    $solicitacaoEmenda->limparPorSolid($solid);
                    $solicitacaoEmenda->commit();
                }
                $emendas = array();
                foreach ($_REQUEST['geral']['emeid'] as $value) {
                    $emendas['solid'] = $solid;
                    $emendas['emeid'] = $value;
                    $solicitacaoEmenda = new Spoemendas_Model_SolicitacoesEmendas();
                    $solicitacaoEmenda->popularDadosObjeto($emendas);
                    $solicitacaoEmenda->salvar();
                    $solicitacaoEmenda->commit();
                }
            }

            if ($solicitacao->commit()) {
                $fm->addMensagem('Solicitação gravada com Sucesso', Simec_Helper_FlashMessage::SUCESSO);
            } else {
                $fm->addMensagem('Erro ao grava solicitação', Simec_Helper_FlashMessage::ERRO);
            }
            header("Location: {$_SERVER['REQUEST_URI']}&solid={$solid}");
            break;
        case 'apagar':
            $solicitacao->excluir($_REQUEST['solid']);
            if ($solicitacao->commit()) {
                $fm->addMensagem('Solicitação apgada com Sucesso', Simec_Helper_FlashMessage::SUCESSO);
            } else {
                $fm->addMensagem('Erro ao apagar solicitação', Simec_Helper_FlashMessage::ERRO);
            }
            header("Location: spoemendas.php?modulo=principal/solicitacoes/listarsolicitacoes&acao=A");
            break;
        case 'salvarAnexo':
            $solid = $_REQUEST['anexos']['solid'];
            $angdsc = $_REQUEST['anexos']['angdsc'];
            if ($_FILES['anexos_anexo']["size"] > 0) {
                $anexo->cadastrarAnexo($solid, $angdsc);
                if ($anexo->commit()) {
                    $fm->addMensagem('Anexo gravado com Sucesso', Simec_Helper_FlashMessage::SUCESSO);
                } else {
                    $fm->addMensagem('Erro ao grava anexo', Simec_Helper_FlashMessage::ERRO);
                }
            }
            header("Location: {$_SERVER['REQUEST_URI']}&solid={$solid}");
            break;

        case 'apagar_anexo':
            $anexo->excluir($_REQUEST['arqid']);
            if ($anexo->commit()) {
                $fm->addMensagem('Anexo apagado com Sucesso', Simec_Helper_FlashMessage::SUCESSO);
            } else {
                $fm->addMensagem('Erro ao apagar anexo', Simec_Helper_FlashMessage::ERRO);
            }
            header("Location: spoemendas.php?modulo=principal/solicitacoes/editarsolicitacao&acao=A&aba=anexos&solid={$_REQUEST['solid']}");
            break;
        case 'baixar_anexo':
            $anexo->downloadAnexo($_REQUEST['arqid']);
            break;
        case 'retornarEmendasAutor':
            global $db;
            $autid = $_REQUEST['autid'];
            if($autid && $autid <>''){
            $sql = "SELECT
                    e.emeid as codigo,
                    e.emecod || '/' || e.emeano || ' ( '||a.autnome||' )' AS descricao
                FROM
                    emenda.emenda e
                JOIN
                    emenda.autor a
                USING
                    (autid)
                WHERE e.autid = {$autid}
                ORDER BY
                    e.emeano DESC,
                    emecod";
            
            echo simec_json_encode($db->carregar($sql));
            } else {
                echo simec_json_encode(array());
            }
            die;
            break;
            
        default:
            ver($_REQUEST, d);
    }

    die();
}

/**
 * Cabeçalho do SIMEC.
 */
include APPRAIZ . "includes/cabecalho.inc";

(new Simec_View_Breadcrumb())
        ->add('Solicitações de informação ou alteração', 'spoemendas.php?modulo=principal/solicitacoes/listarsolicitacoes&acao=A')
        ->add('Solicitação')
        ->render();

echo $fm;
?>

<div class="col-md-10">
    <?php
    $abas = new Simec_Abas(
            "spoemendas.php?modulo=principal/solicitacoes/editarsolicitacao&acao=A&solid={$_REQUEST['solid']}"
    );
    $caminhoBaseAbas = dirname(__FILE__) . '/editarsolicitacao/';
    $abas->adicionarAba('geral', 'Solicitação', "{$caminhoBaseAbas}geral.inc");
    if ($_REQUEST['solid']) {
        $abas->adicionarAba('anexos', 'Anexos', "{$caminhoBaseAbas}anexos.inc");
    }
    $abas->definirAbaDefault('geral')
            ->render();
    echo $fm->getMensagens();
    ?>
</div>
<div class="col-md-2">
    <?php
    echo wf_desenhaBarraNavegacaoBootstrap(
            $dados['docid'], array(
        'solid' => $dados['solid']
            )
    );
    ?>
</div>
<?php

/**
 * Sistema de Publicação.
 * werteralmeida
 */
include_once APPRAIZ . 'includes/workflow.php';
$infoemenda = new Spoemendas_Model_Emendainformacoes();
/*
 * Verifica se já existe um DOCID para a emenda, senão, cadastra um novo.
 */

if ($_REQUEST['ajax'] == 'aprovaEmendaReitor') {
    salvarAprovacaoReitor($_REQUEST['emenda'], $_REQUEST['emeid']);
    exit();
}
if ($_REQUEST['emeid']) {
    global $db;
    $sql = "SELECT
                docid
            FROM
                spoemendas.emendainformacoes
            WHERE
            emeid = {$_REQUEST['emeid']}";
    if (!$db->pegaUm($sql)) {
        cadastrarDocid($informacoes, $infoemenda, $_REQUEST['emeid']);
    }
}

// download de arquivo
if ($_REQUEST['requisicao'] == 'baixar_anexo') {

	    if (!empty($_REQUEST['arqid'])) {
	    	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	        (new FilesSimec(null, null, "emenda"))->getDownloadArquivo($_REQUEST['arqid']);
	        exit;
	    }
}
        $fm = new Simec_Helper_FlashMessage('spoemendas/principal/emendas/editaremenda');
        $dados = $_REQUEST;
        $perfis = pegaPerfilGeral();
        $informacoes = $infoemenda->findByEmeid($dados['emeid']);
        $formaEx = recuperaFormaExecucao($dados['emeid']);

        $paramPermissao = [
        'tipo' => $_REQUEST['aba'] ? $_REQUEST['aba'] : 'beneficiarios',
        'emeid' => $_REQUEST['emeid'],
        'einid' => $informacoes['einid']
        ];

        if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao'])) {
            $requisicao = $_REQUEST['requisicao'];
            include_once 'iniciativas-request.inc';
            include_once 'beneficiarios-request.inc';
            include_once 'iniciativa-beneficiario-request.inc';
            include_once 'responsaveispta-request.inc';
            include_once 'subacoes-request.inc';
            include_once 'impedimentos-request.inc';
            switch ($requisicao) {
                case 'retornar-subacoes-par-por-iniciativa':

            $sql = "SELECT DISTINCT
                        par.retornacodigopropostasubacao(sba.ppsid)||' - '||ps.ppsdsc AS descricao
                    FROM
                        par.propostasubacaoiniciativaemenda psi
                    JOIN
                        par.subacao sba
                    USING
                        (ppsid)
                    JOIN
                        par.propostasubacao ps
                    USING
                        (ppsid)
                    WHERE
                        psi.iniid = {$_REQUEST['iniid']}
                    ORDER BY
                    1";
                    $arrColunas = array('Subação do PAR');
                    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);
                    $listagem->addCallbackDeCampo(array('descricao'), 'alinhaParaEsquerda');
                    $listagem->setCabecalho($arrColunas)->setQuery($sql);
                    echo $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
                    die;
                    default:
                    break;
                }
        }

            $aprovacaoReitor = buscaAprovacaoReitor($_GET['emeid']);

/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
/*
 * Modal de detalhe das subações
 */
bootstrapPopup('Subações do PAR para a Iniciativa', 'lista-subacoes-par-por-iniciativa', '', ['fechar'], array('tamanho' => 'lg'));
$bc = new Simec_View_Breadcrumb('C');
if(strpos($_SERVER['HTTP_REFERER'],'spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=filtrar&filtro=none&emendas[etoid][]=1&emendas[etoid][]=3&emendas[etoid][]=5')){
	$bc->add('Emendas', 'spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=filtrar&filtro=none&emendas[etoid][]=1&emendas[etoid][]=3&emendas[etoid][]=5');
}else{
	$bc->add('Emendas', 'spoemendas.php?modulo=principal/emendas/listaremendas&acao=A');

};
$bc->add('Lista de Emendas')->render();

$perfil = [
    PFL_REITOR,
    PFL_SUPER_USUARIO,
    PFL_CGO_EQUIPE_ORCAMENTARIA
];
$estadoAtual = wf_pegarEstadoAtual($informacoes['docid']);
?>

    <script type="text/javascript" lang="javascript">
        $(document).ready(function () {
            $(function () {
                $('[data-toggle="tooltip"]').tooltip();
            });
        });
        /*
         * Retorar as subações do PAR para cada Iniciativa
         */
        function verSubacoesParIniciativa(iniid) {
            $.ajax(window.location.href + '&requisicao=retornar-subacoes-par-por-iniciativa&iniid=' + iniid).done(function (res) {
                $('#lista-subacoes-par-por-iniciativa .modal-body').html(res);
                $('#lista-subacoes-par-por-iniciativa').modal();
            });
        }
        var aprovacaoReitor = '<?php echo $aprovacaoReitor->emeaprovacaoreitor ?>';
        if (aprovacaoReitor == 't') {
            $("input[type=checkbox][name='aproReitor']").attr('checked', 'checked');
            $(".msg-reitor-ap").css('display', 'block');
            $(".msg-reitor-pe").css('display', 'none');
        }
        $("input[type=checkbox][name='aproReitor']").click(function () {
            var resp = $("input[type=checkbox][name='aproReitor']:checked").val();
            if (resp == 'true') {
                $(".msg-reitor-ap").css('display', 'block');
                $(".msg-reitor-pe").css('display', 'none');
                ajaxAprovacaoReitor('true');

            } else {
                $(".msg-reitor-pe").css('display', 'block');
                $(".msg-reitor-ap").css('display', 'none');
                ajaxAprovacaoReitor('false');

            }
        });

        function ajaxAprovacaoReitor(emenda) {
            $.ajax({
                type: "POST",
                url: "/spoemendas/spoemendas.php?modulo=principal/emendas/editaremenda/editaremenda&acao=A",
                data: {
                    ajax: 'aprovaEmendaReitor',
                    emenda: emenda,
                    emeid: <?php echo $_GET['emeid'] ?>
                },
                success: function (result) {
                    window.location.reload();
                }
            });
        }

        // Função responsável por imprimir emenda
        function imprimirModalEmenda(emeid) {
            window.open('spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=imprimirModalEmenda&emeid=' + emeid);
        }
    </script>
<?php

/*
 * Mensagens de Retorno
 */
$dados['emeid'] = $_REQUEST['emeid'];
echo $fm;
?>

<div class="row">
        <div class="col-md-2 menu-scroller" style="padding-right: 0; margin-right: 0;">
        <?php
            //Busca as permissoes de abas que o perfil pode acessar
            $modelAbasAcesso = new Spoemendas_Controller_Acessoperfilabas();
            $abasHabilitadas = $modelAbasAcesso->getPermissaoAbas($formaEx);
            
            if($abasHabilitadas) {
                $baseUrl = 'spoemendas.php?modulo=principal/emendas/editaremenda/editaremenda&acao=A&emeid=' . $dados['emeid'];
                $abas = new Simec_Abas($baseUrl, 'aba', false, Simec_Abas::ABA_VERTICAL);
                if (in_array('iniciativa', $abasHabilitadas)) {
                    if (!in_array(PFL_PARLAMENTAR, $perfis)) {
                        $abas->adicionarAba('iniciativa', '&nbsp; Iniciativas', dirname(__FILE__) . '/iniciativas/iniciativas.inc', 'tag');
                    }
                }

                if (in_array('beneficiarios', $abasHabilitadas)) {
                	$abas->adicionarAba('beneficiarios', 'Entidades Beneficiadas', dirname(__FILE__) . '/beneficiarios/beneficiarios.inc', 'home');
                }
                
                if (in_array('responsaveis', $abasHabilitadas)) {
                    $abas->adicionarAba('responsaveis', 'Passo 1: Responsáveis pela elaboração do Programa de Trabalho', dirname(__FILE__) . '/responsaveispta/responsaveispta.inc', 'user');
                }
                
                if (in_array('iniciativa-beneficiario', $abasHabilitadas)) {
                	$abas->adicionarAba('iniciativa-beneficiario', 'Passo 2: Indicação de Iniciativas', dirname(__FILE__) . '/iniciativa-beneficiario/iniciativa-beneficiario.inc', 'tags');
                }
                
                if (in_array('subacoes', $abasHabilitadas)) {
                    $abas->adicionarAba('subacoes', 'Passo 3: Subações do PAR', dirname(__FILE__) . '/subacoes/subacoes.inc', 'ok-sign');
                }
                
                if (in_array('impedimentos', $abasHabilitadas)) {
                    $abas->adicionarAba('impedimentos', 'Programa de Trabalho / Impedimento', dirname(__FILE__) . '/impedimentos/impedimentos.inc', 'th-list');
                }

                if (in_array('anexos-oficio', $abasHabilitadas)) {
                    $abas->adicionarAba('anexos', 'Anexo de Ofícios', dirname(__FILE__) . '/anexos/abaanexo.inc', 'upload');
                }

                $permissaoFormaEx = $modelAbasAcesso->permissaoEditEmenda($formaEx);
                
                if (!$permissaoFormaEx) {
                    $fm->addMensagem("Esse perfil não tem permissão para editar esse emenda.", Simec_Helper_FlashMessage::ERRO);
                    echo $fm->getMensagens();
                    exit;
                }

                $abas->definirAbaDefault($abasHabilitadas[0]);
                $abas->render();
            }else{
                $fm->addMensagem('Seu perfil não possui permissão de acesso as abas!','danger');
                echo $fm;
            }

            include_once APPRAIZ . 'includes/workflow.php';
            echo wf_desenhaBarraNavegacaoBootstrap(
                $informacoes['docid'], [
            		'solid' => '',
                    'emeid' => $dados['emeid']
                ]
             );
        ?>
        </div>
        <div class="col-md-8" style="padding-right: 0; margin-right: 0; padding-left: 0; margin-left: 1; overflow-x: auto;">
            <ul class="nav nav-tabs" style="margin-left: 1em;">
                <li role="presentation" class="active">
                    <a href="#"><?= $abas->getAbas()[$abas->getAbaAtiva()]['titulo']; ?></a>
                </li>
            </ul>
            <br style="clear:both">
            <?php
                require $abas->getAbas()[$abas->getAbaAtiva()]['require'];
            ?>
        </div>
        <div class="col-md-2 col-scroll-spy" style="padding-right: 0; margin-right: 0; padding-left: 0; margin-left: 1; overflow-x: auto;">
            <?php
                if (in_array(PFL_PARLAMENTAR, $perfis)) {
                    painelInformacoesEmendaParlamentar($dados['emeid'], 12, 4, 8);
                } else {
                    painelInformacoesEmenda($dados['emeid'], 12);
                }
            ?>
            <div class="panel panel-default">
                <div class="panel-body">
                    <div class="form-group">
                        <button class="btn btn-warning"
                                onClick="javascript:imprimirModalEmenda(<?=$dados['emeid'];?>)"
                                type="button">
                            <span class="glyphicon glyphicon-print"></span> Imprimir
                        </button>
                    </div>
                </div>
            </div>
        </div>
</div>

<style type="text/css">
.spy-auto-scroll {
    width: 350px;
    position: absolute;
    height: 1200px;
    margin-left: 1080px;
    right: 0;
    z-index: 100000;
}
</style>

<script type="text/javascript">
$(function(){
    var
        $colScrollPpy = $(".col-scroll-spy"),
        LIMITE_ALTURA = 55,
        $baseRef = $(".navbar-fixed-top"),
        $window = $(window),
        $menuScroller = $(".menu-scroller")
    ;

    $window.on("scroll", function(){
        if ($window.scrollTop() > LIMITE_ALTURA) {
            $colScrollPpy.stop().animate({marginTop: $window.scrollTop()});
            $menuScroller.stop().animate({marginTop: $baseRef.offset().top});
        } else {
            $colScrollPpy.stop().animate({marginTop:0});
            $menuScroller.stop().animate({marginTop:0});
        }
    });
});
</script>
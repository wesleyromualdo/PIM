<?php
include_once APPRAIZ . "includes/workflow.php";

if($_REQUEST['indCabechalho'] === '0'){
    include_once 'cabecalho_bootstrap_v3_limpo.inc';
}else{
require APPRAIZ . 'includes/cabecalho.inc';
}



if( $_REQUEST['requisicao'] == 'altera_ano_obra' ){
	$sql = "update par3.obra set obrano = '".date('Y')."' where obrid = {$_REQUEST['obrid']}";
	$db->executar($sql);
	return $db->commit();
	exit();
}

$modelUnidade            = new Par3_Model_InstrumentoUnidade();
$usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();
$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

$arrPerfilPermitido = array(Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST);

// if (!$arrPermissao || !$arrPermissao['booVisualizar']) {
if ((!$arrPermissao || !$arrPermissao['booVisualizar']) && !$usuarioResponsabilidade->possuiPerfil($arrPerfilPermitido)) {
    simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
}

//ver($_SESSION['filtroObras']); exit();
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$obrid = $_GET['obrid'];
$inuid = $_GET['inuid'];
$inpid = $_REQUEST['inpid'];
$helperUrlToReturn = new Par3_Helper_UrlToReturnHelper();

$mdObra = new Par3_Model_Obra($obrid, true, $_GET);
$obrid = $obrid ? $obrid : $mdObra->obrid;

$carregaAbaEmenda = false;
if($obrid){
    $emenda = new Par3_Controller_EmendasParlamentares();
    $arrEmenda = $emenda->recuperarEmendaPorObra($obrid,true);
    if($arrEmenda){
        $carregaAbaEmenda = true;
    }
}

$esd = wf_pegarEstadoAtual($mdObra->docid);

/**
 * VALIDAÇÃO DE PERFIL PARA O ESTADO DOCUMENTO
 */
$edicaoDesabilitada = $mdObra->habilitarEdicaoPorPerfilEstadoDocumento($_GET['obrid']);
$disabled = '';
if ($edicaoDesabilitada == false) {
    $_GET['disabled'] = 'disabled';
}

if (!$_GET['obrid']) {
    echo "<script>window.location.href = window.location.href+'&obrid=$obrid';</script>";
    die();
}

$boEditar = $mdObra->verificaEdicaoObras($obrid);

$mdPlanejamento = new Par3_Model_IniciativaPlanejamento($mdObra->inpid);
$mdIniciativa = new Par3_Model_Iniciativa($mdPlanejamento->iniid);
$mdIniciativaDsc = new Par3_Model_IniciativaDescricao($mdIniciativa->indid);

$categoria = $mdPlanejamento->retornaCategoriaIniciativa();

if (!$_REQUEST['menu']) {
    if ($categoria['octid'] != 3) {
        $_REQUEST['menu'] = 'estudo_demanda';
    } else {
        $_REQUEST['menu'] = 'dados_terreno';
    }
}

$mdConfiguracaoAba = new Par3_Model_ObraConfiguracaoAba();
$mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
$arrAbas = array();
if ($mdObra->otpid) {
    $orderCase = " case oabid when 1 then 1 when 2 then 2 when 3 then 3 when 4 then 4 when 5 then 5 when 6 then 6 when 7 then 7 when 9 then 8 when 8 then 9 end";
    $arrAbas = $mdConfiguracaoAba->recuperarTodos('*', array("otpid = " . $mdObra->otpid, "ocastatus = 'A'"), $orderCase);
}
$returnToObrasList = '';
if(isset($_REQUEST['formListaObra'])){
    $returnToObrasList = '&formListaObra='.$_REQUEST['formListaObra'];
}else{
    $returnToObrasList = '';
}
$iconNaoPreenchido = '<span class="fa fa-times-circle danger" ></span>&nbsp';
$iconPreenchido = '<span class="fa fa-check iconUnidade"' . 'style="color:green !important;' . 'background-color: transparent;"></span>&nbsp';
?>
<script src="js/par3.js"></script>
<style>
    a {
        color: inherit;
    }

    a:hover {
        color: inherit;
    }

    a:visited {
        color: inherit;
    }

    a:active {
        color: inherit;
    }

    a:link {
        color: inherit;
    }
</style>
<link rel="stylesheet" type="text/css" href="css/menu_tabela_apoio.css"/>

<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
            </div>
            <div class="ibox-content">
                <div class="ibox-content-round-gray ibox-content">
                    <div class="row">
                        <div class="col-lg-1">
                            <h5 id="btn-menu-tabelaapoio" style="cursor: pointer;">
                                <i class="fa fa-bars" title="clique para esconder/mostar os menus"></i>
                            </h5>
                        </div>
                    </div>
                    <div class="row">
                        <div id="side-menu-tabelaapoio" class="col-lg-2 col-md-2 colunaMenu noprint">
                           <?php echo $helperUrlToReturn->renderLinkVoltarObras(); ?>
                            <br><br>
                            <h4><b>Iniciativa: <?php echo $mdIniciativaDsc->inddsc ?></b></h4>
                            <div class="space-25"></div>
                            <!--Guia de Iniciativas-->
                            <h5 class="menuopcoes" id="titulo-obras"><b><i class="fa fa-minus"></i> Obras</b></h5>
                            <div id="div-obrasmenu" class="guiamenu col-md-offset-1 col-lg-offset-1 col-sm-offset-1">
                                <?php if ($categoria['octid'] != 3) { ?>
                                    <a href="?modulo=principal/planoTrabalho/obras&acao=A&menu=estudo_demanda&inuid=<?php echo $_GET['inuid']; ?>&inpid=<?php echo $inpid; ?>&obrid=<?php echo $_GET['obrid']; ?><?php echo $helperUrlToReturn->renderComplementoUrl(); ?>">
                                        <div class="menuopcoes menuopcao <?php echo($_REQUEST['menu'] === 'estudo_demanda' ? 'menuSelecionado' : '') ?>"
                                             id="estudo_demanda" tipo="estudo_demanda">
                                            <?php echo $mdObra->validarPendencias($_GET['obrid'], Par3_Model_ObraAba::ESTUDO_DEMANDA) ? $iconPreenchido : $iconNaoPreenchido; ?>
                                            Estudo de Demanda
                                        </div>
                                    </a>
                                    <?php if ($mdObra->otpid != '') { ?>
                                        <a href="?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid=<?php echo $_GET['inuid']; ?>&inpid=<?php echo $inpid; ?>&obrid=<?php echo $_GET['obrid']; ?><?php echo $helperUrlToReturn->renderComplementoUrl(); ?>">
                                            <div class="menuopcoes menuopcao <?php echo($_REQUEST['menu'] === 'dados_terreno' ? 'menuSelecionado' : '') ?>"
                                                 id="dados_terreno" tipo="dados_terreno">
                                                <?php echo $mdObra->validarPendencias($_GET['obrid'], Par3_Model_ObraAba::DADOS_TERRENO) ? $iconPreenchido : $iconNaoPreenchido; ?>&nbspDados do Terreno
                                            </div>
                                        </a>
                                    <?php } ?>
                                <?php } else { ?>
                                    <a href="?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid=<?php echo $_GET['inuid']; ?>&inpid=<?php echo $inpid; ?>&obrid=<?php echo $_GET['obrid']; ?><?php echo $helperUrlToReturn->renderComplementoUrl(); ?>">
                                        <div class="menuopcoes menuopcao <?php echo($_REQUEST['menu'] === 'dados_terreno' ? 'menuSelecionado' : '') ?>" id="dados_terreno" tipo="dados_terreno">
                                            <?php echo $mdObra->validarPendencias($_GET['obrid'], Par3_Model_ObraAba::DADOS_TERRENO) ? $iconPreenchido : $iconNaoPreenchido; ?>Dados do Terreno
                                        </div>
                                    </a>

                                <?php }
                                //,'historico_tramitacao'
                                $arrAbasPermitidas = array('estudo_demanda', 'dados_terreno');
                                $jaPossuiAbaAnalise = false;
                                if ($obrid && $mdObra->tplid != '') {
                                    foreach ($arrAbas as $aba) {//LISTAR ABAS CADASTRADAS
                                        if ($aba['oabid'] < '3') continue;
                                        $mdAba = new Par3_Model_ObraAba($aba['oabid']);
                                        if ($mdAba->oabstatus != 'A') continue;
                                        $arrAbasPermitidas[] = $_REQUEST['menu'];
                                        if(!$mAnalise->mostrarPendenciasEmAnalise($_GET['obrid']) && $mdAba->oabid == Par3_Model_ObraAba::ANALISE_ENGENHARIA) continue;
                                        ?>
                                        <?php
                                        if($mdAba->oabid == Par3_Model_ObraAba::ENVIAR_ANALISE || $mdAba->oabid == Par3_Model_ObraAba::ANALISE_ENGENHARIA){
                                            if(!$jaPossuiAbaAnalise || $mdAba->oabid != Par3_Model_ObraAba::ANALISE_ENGENHARIA) {
                                        ?>
                                        <a href="?modulo=principal/planoTrabalho/obras&acao=A&menu=<?php echo $mdAba->oabarquivo; ?>&inuid=<?php echo $_GET['inuid']; ?>&inpid=<?php echo $inpid; ?>&obrid=<?php echo $_GET['obrid']; ?><?php echo $helperUrlToReturn->renderComplementoUrl(); ?>">
                                        <div
                                            class="menuopcoes menuopcao <?php echo($_REQUEST['menu'] === $mdAba->oabarquivo ? 'menuSelecionado' : '') ?>"
                                            id="<?php echo $mdAba->oabarquivo ?>"
                                            tipo="<?php echo $mdAba->oabarquivo ?>"
                                        >
                                            <?php echo $mdAba->oabdsc;?>
                                        </div>
                                        </a>
                                        <?php
                                            }
                                            if($mdAba->oabid == Par3_Model_ObraAba::ANALISE_ENGENHARIA) {
                                                $jaPossuiAbaAnalise = true;
                                            }
                                        }
                                        else { ?>
                                            <a href="?modulo=principal/planoTrabalho/obras&acao=A&menu=<?php echo $mdAba->oabarquivo; ?>&inuid=<?php echo $_GET['inuid']; ?>&inpid=<?php echo $inpid; ?>&obrid=<?php echo $_GET['obrid']; ?><?php echo $helperUrlToReturn->renderComplementoUrl(); ?>">
                                                <div
                                                    class="menuopcoes menuopcao <?php echo($_REQUEST['menu'] === $mdAba->oabarquivo ? 'menuSelecionado' : '') ?>"
                                                    id="<?php echo $mdAba->oabarquivo ?>"
                                                    tipo="<?php echo $mdAba->oabarquivo ?>"
                                                >
                                                    <?php echo ($mdObra->validarPendencias($_GET['obrid'], $mdAba->oabid) ? $iconPreenchido : $iconNaoPreenchido). ' ' . $mdAba->oabdsc;?>
                                                </div>
                                            </a>
                                        <?php }
                                        ?>
                                        <?php
                                    }
                                }
                                ?>

                                <?php if(array_intersect(pegaPerfilGeral($_SESSION['usucpf']),$mAnalise->perfisEdicao())):?>
                                <a href="?modulo=principal/planoTrabalho/obras&acao=A&menu=historico_tramitacao&inuid=<?php echo $_GET['inuid']; ?>&inpid=<?php echo $inpid; ?>&obrid=<?php echo $_GET['obrid']; ?><?php echo $helperUrlToReturn->renderComplementoUrl(); ?>">
                                    <div class="menuopcoes menuopcao <?php echo($_REQUEST['menu'] === 'historico_tramitacao' ? 'menuSelecionado' : '') ?>"
                                         id="historico_tramitacao" tipo="historico_tramitacao"> Histórico de Tramitação
                                    </div>
                                </a>
                                <?php endif; ?>
                                <?php if ( $esd['esdid'] != $mdObra::ESDID_EM_CADASTRAMENTO && $esd['esdid'] != $mdObra::ESDID_PAR3_AGUARDANDO_ANALISE_FNDE ): ?>
                                <a href="?modulo=principal/planoTrabalho/obras&acao=A&menu=analise_pf_fnde&inuid=<?php echo $_GET['inuid']; ?>&inpid=<?php echo $inpid; ?>&obrid=<?php echo $_GET['obrid']; ?><?php echo $helperUrlToReturn->renderComplementoUrl(); ?>">
                                    <div class="menuopcoes menuopcao <?php echo($_REQUEST['menu'] === 'analise_pf_fnde' ? 'menuSelecionado' : '') ?>"
                                         id="analise_pf_fnde" tipo="analise_pf_fnde"> Análise PF-FNDE
                                    </div>
                                </a>
                                <?php endif; ?>
                                <?php if ($carregaAbaEmenda): ?>
                                    <a href="?modulo=principal/planoTrabalho/obras&acao=A&menu=recurso_emenda&inuid=<?php echo $_GET['inuid']; ?>&inpid=<?php echo $inpid; ?>&obrid=<?php echo $_GET['obrid']; ?><?php echo $helperUrlToReturn->renderComplementoUrl(); ?>">
                                        <div class="menuopcoes menuopcao <?php echo($_REQUEST['menu'] === 'recurso_emenda' ? 'menuSelecionado' : '') ?>"
                                             id="recurso_emenda" tipo="recurso_emenda"> Recurso Emenda
                                        </div>
                                    </a>
                                <?php endif; ?>
                                <br/>
                            </div>

                        </div>

                        <div class="col-lg-10 col-md-10 colunaForm" id="print_area">
                            <?php
                            $pastaObras = 'par3/modulos/principal/planoTrabalho/obras/';
                            if (in_array($_REQUEST['menu'], $arrAbasPermitidas)) {
                                switch ($_REQUEST['menu']) {
                                    case 'estudo_demanda':
                                    case 'dados_terreno':
                                    case 'relatorio_vistoria':
                                    case 'cadastro_foto_terreno':
                                    case 'cronograma_contrapartida':
                                    case 'documento_anexo':
                                    case 'analise_engenharia':
                                    case 'planilha_orcamentaria':
                                    case 'tramitacao':
                                    case 'historico_tramitacao':
                                    case 'analise_pf_fnde':
                                    case 'recurso_emenda':
                                    	
                                    	if ( $_REQUEST['menu'] == 'analise_pf_fnde' && ($esd['esdid'] == $mdObra::ESDID_EM_CADASTRAMENTO || $esd['esdid'] == $mdObra::ESDID_PAR3_AGUARDANDO_ANALISE_FNDE) ){
                                    		header("location: par3.php?modulo=inicio&acao=C");
                                    		die;
                                    	}
                                    	
                                        include APPRAIZ . $pastaObras . $_REQUEST['menu'] . '.inc';
                                        break;
                                }
                            }
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<script>
    $(document).ready(function () {
        $('#btn-menu-tabelaapoio').on('click',function(evt) {
            evt.preventDefault();
            $('#side-menu-tabelaapoio').toggle("slide");
            $('#print_area').toggleClass(function() {
                if($(this).is('.col-lg-10')) {
                    $(this).removeClass('col-lg-10');
                    $(this).removeClass('col-md-10');
                    return 'col-lg-12 col-md-12';
                }
                $(this).removeClass('col-lg-12');
                $(this).removeClass('col-md-12');
                return 'col-lg-10 col-md-10';
            });
        });

        $('.menuopcao').click(function () {
            var menuAtual = '<?php echo $_REQUEST['menu']; ?>';
            var menu = $(this).attr('tipo');
            var url = 'par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=' + menu + "&inuid=<?=$inuid?>&inpid=<?=$inpid ?>&obrid=<?=$obrid?>&<?=$helperUrlToReturn->renderComplementoUrl();?>";

            if (menu != menuAtual) {
                $(location).attr('href', url);
            }
        });

        $('#titulo-obras').click(function () {
            $('#div-obrasmenu').slideToggle();
            $(this).find('i').toggleClass(function () {
                if ($(this).is(".fa-plus")) {
                    $(this).removeClass('fa-plus');
                    return 'fa-minus';
                }
                $(this).removeClass('fa-minus');
                return 'fa-plus';
            });
        });

    });

    function js_alterarAnoObra( obrid ){
        
    	var caminho = window.location.href;
        var action = '&requisicao=altera_ano_obra&obrid=' + obrid;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                window.location.href = window.location;
            }
        });
    }
<?
if($_REQUEST['indCabechalho'] === '0'){
    echo '$(\'#side-menu-tabelaapoio\').hide();';
}
?>
</script>
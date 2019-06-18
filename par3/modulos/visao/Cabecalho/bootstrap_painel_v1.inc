<?php
// Verifica se a sessao expirou.
if( ! isset($_SESSION['usucpf']) )
{
    session_unset();
    $_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
    header('Location: login.php');
    exit();
}

include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';

$acessoRapidoCabecalhoIncluido = true;
if ($_POST['carregarUsuariosSistema'])
{
    $usuariosSistema = carregarUsuariosSistema(true);

    $listing = new Listing();
    $listing->setTypePage('M');
    $listing->setActions(array('eye-open' => 'simularUsuariosSistemas'));
    $listing->setHead(array('CPF', 'Nome', 'Email'));
    $listing->listing($usuariosSistema);
    exit;
}

$sistemas = carregarSistemas();

if ($_POST['simularUsuariosSistema'])
{
    $cpf = $_POST['cpf'];

    global $db;

    $sql = "select distinct
				u.usucpf,
				u.usunome,
				u.usuemail
				--u.susdataultacesso,
				--u.usuacesso,
				from seguranca.usuario u
				inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
				inner join seguranca.perfil p on p.pflcod = pf.pflcod and	p.sisid = " . $_SESSION['sisid'] . "
				inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and	us.sisid = p.sisid
				where us.suscod = 'A' and p.pflnivel >= (
					select min(pflnivel) from seguranca.perfil
					inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
					and perfil.sisid = " . $_SESSION['sisid'] . " )
					and u.usucpf = '$cpf';";

    $usuario = $db->pegaLinha($sql);

    if ($usuario) {
        $_SESSION['usucpf'] = $usuario['usucpf'];
        $_SESSION['usunome'] = $usuario['usunome'];
        $_SESSION['usuemail'] = $usuario['usuemail'];
        unset($_SESSION['testa_superuser']);
        unset($_SESSION['testa_superuser_cache']);
        unset($_SESSION['acl']);
        unset($_SESSION['sistemas']);
        echo 'ok';
    }

    exit;
}

// // Definindo o tema do sistema.
// if (isset($_POST['theme'])) {
//     $theme = $_POST['theme'];
//     setcookie("theme", $theme, time() + 60 * 60 * 24 * 30, "/");
// } else if (isset($_COOKIE["theme"])) {
//     $theme = $_COOKIE["theme"];
// } else {
//     $theme = '';
// }

// $arrTheme = array('default', 'default-inverse', 'ameliaa', 'cerulean', 'cosmo', 'cyborg', 'flatly', 'journal', 'readable', 'simplex', 'slate', 'spacelab', 'united', 'yeti');
$sistemas = carregarSistemas();
$menus = carregarMenus($_SESSION['sisid'], $_SESSION['usucpf']);
$menuHtml = montaMenuBootstrap($menus);

if ($_SESSION['sisdiretorio'] && $_SESSION["sisexercicio"] == 't') {
    $sql = "select 
                schemaname , relname 
            from 
                pg_stat_user_tables
			where 
                schemaname = '{$_SESSION['sisdiretorio']}' and 
                relname = 'programacaoexercicio'
			order by 
                relname;";
    $tabelaExercicio = $db->pegaLinha($sql);

    if ($tabelaExercicio) {
        $sql = "select prsano as codigo, prsano as descricao,prsexerccorrente,prsexercicioaberto from " . $_SESSION['sisdiretorio'] . ".programacaoexercicio order by 1";
        $arrAnoExercicio = $db->carregar($sql);
    }
}

if (!$_SESSION['exercicioaberto'] && $_SESSION['sisexercicio'] == 't' && $tabelaExercicio) {
    $sql = sprintf("SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'", $_SESSION['sisdiretorio'], $_SESSION['exercicio']);
    $_SESSION['exercicioaberto'] = $db->pegaUm($sql);
}
// altera o ano de exercício (caso o usuário solicite)
if ($_REQUEST['exercicio'] AND $_SESSION['exercicio'] != $_REQUEST['exercicio']) {

    if ($arrAnoExercicio) {
        foreach ($arrAnoExercicio as $anoExercicio) {
            $arrAno[] = $anoExercicio['codigo'];
            if ($anoExercicio['prsexerccorrente'] == "t") {
                $ano_corrente = $anoExercicio['codigo'];
            }
        }
    }
    if (is_array($arrAno)) {
        if (in_array($_REQUEST['exercicio'], $arrAno)) {
            $_SESSION['exercicio'] = $_REQUEST['exercicio'];
        } else {
            $_SESSION['exercicio'] = $ano_corrente;
        }
    }
    $exercicio = $_SESSION['exercicio'];

    $sql = sprintf("SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'", $_SESSION['sisdiretorio'], $_REQUEST['exercicio']);
    $_SESSION['exercicioaberto'] = $db->pegaUm($sql);
}

if (!isset($_COOKIE['navbar'])) {
    $_COOKIE['navbar'] = 'false';
}
$navBar = $_COOKIE['navbar']=='false' ? 'mini-navbar' : null;

// // pega a página inicial do sistema
// $sql = sprintf("SELECT TRIM( paginainicial ) AS paginic FROM seguranca.sistema WHERE sisid = %d", $_SESSION['sisid']);
// $paginic = $db->pegaUm($sql);
?>
    <!DOCTYPE html>
    <html>
    <head>
        <title>SIMEC - Sistema Integrado de Monitoramento Execução e Controle</title>
        <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <!-- Fonts  -->
        <link href="/zimec/public/temas/simec/fonts/awesome/font-awesome.css?t=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/glyphicons/glyphicons.css?t=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/material/material.css?t=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/inconsolata/inconsolata.css?t=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/opensans/opensans.css?t=1.0" rel="stylesheet">

        <!-- Mainly scripts -->
        <script src="/zimec/public/temas/simec/js/jquery-1.11.3.js"></script>
        <script src="/zimec/public/temas/simec/js/bootstrap.min.js"></script>
        <script src="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.min.js"></script>
        <script src="/estrutura/js/funcoes.js"></script>
        <script src="/includes/funcoes.js"></script>
        <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <script src="../includes/libs_jquery/chosen.js/docsupport/prism.js" type="text/javascript"></script>



        <!-- Data picker -->
        <script src="/zimec/public/temas/simec/js/plugins/datapicker/bootstrap-datepicker.js"></script>
        <script src="/zimec/public/temas/simec/js/plugins/datapicker/locales/bootstrap-datepicker.pt-BR.js"></script>

		<link href="/zimec/public/temas/simec/css/plugins/summernote/summernote-bs3.css" rel="stylesheet">
		<link href="/zimec/public/temas/simec/css/plugins/summernote/summernote.css" rel="stylesheet">

		<link href="/zimec/public/temas/simec/css/plugins/chosen/chosen.css" rel="stylesheet">

		<link href="/zimec/public/temas/simec/css/plugins/jstree/style.min.css" rel="stylesheet">

        <link href="/zimec/public/temas/simec/css/plugins/dropzone/dropzone.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/dropzone/basic.css" rel="stylesheet">




        <!-- jQuery UI custom -->
        <script src="/zimec/public/temas/simec/js/jquery-ui.custom.min.js"></script>

        <!-- Sweet Aler -->
        <link href="/zimec/public/temas/simec/css/plugins/sweetalert/sweetalert.css" rel="stylesheet">
        <script src="/zimec/public/temas/simec/js/plugins/sweetalert/sweetalert.min.js"></script>

        <link href="/zimec/public/temas/simec/css/bootstrap.min.css" rel="stylesheet" media="all">
        <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.theme.min.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.structure.min.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.min.css" rel="stylesheet">
               
        
        <!--  
        <link href="/zimec/public/temas/simec/css/plugins/switchery/switchery.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/bootstrap-switch/bootstrap-switch.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/iCheck/custom.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/awesome-bootstrap-checkbox/awesome-bootstrap-checkbox.css" rel="stylesheet">
        -->
        
        <link href="/zimec/public/temas/simec/css/plugins/nanoscroll/nanoscroller.css" rel="stylesheet">
        
        <link href="/zimec/public/temas/simec/css/style.min.css?t=1.0" rel="stylesheet" media="all">
        <link href="/zimec/public/temas/simec/css/simec.css?t=1.0" rel="stylesheet" media="all">
<!--  
        <link href="/zimec/public/temas/simec/css/animate.css" rel="stylesheet">
-->        

        <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
        <!--[if lt IE 9]>
        <!--<script src="js/html5shiv.js"></script>-->
        <!--<script src="js/respond.min.js"></script>-->
        <!--[endif]-->

        <link href="/zimec/public/temas/simec/css/print.css?t=1.0" rel="stylesheet" media="print" />

        <script data-pace-options='{ "ajax": false }' src="/zimec/public/temas/simec/js/plugins/pace/pace.min.js"></script>
		<?php 
		echo $this->montarIncludeCSS(true);
		echo $this->montarIncludeJS(true);
		?>
        <?php (IS_PRODUCAO ? require_once APPRAIZ . 'includes/google_analytics.php' : ''); ?>
    </head>
<body class="fixed-nav <?php echo $navBar; ?>">
    <script>
        Pace.on("done", function(){
            $("#loading").fadeOut(250);
        });
        Pace.start;
    </script>
    <div class="loading-dialog" id="loading"></div>
<div id="wrapper">
    <nav id="simec-navbar" class="navbar-default navbar-static-side" role="navigation">
        <div class="sidebar-collapse">
            <?php echo $menuHtml ?>
        </div>
    </nav>
    <div id="page-wrapper" class="gray-bg">
    <div class="row border-bottom">
    <nav class="navbar navbar-fixed-top" role="navigation" style="margin: 0px; height: 61px; border-bottom: 1px solid #273A4A !important; background: #273A4A") no-repeat">
    <div class="navbar-left">
        <ul class="header-nav header-nav-options">
            <li class="header-nav-brand" style="cursor:pointer;" onclick="window.location='<?=$_SESSION['sisarquivo'] ?>.php?modulo=inicio&acao=C';"><img src="/zimec/public/img/logo-simec.png" class="img-responsive" style="width: 139px; margin: 10px 0px;"></li>
            <li><a class="navbar-minimalize btn btn-primary" href="#"><i class="fa fa-bars"></i></a></li>
        </ul>
    </div>
    <div class="navbar-center hidden-md hidden-lg">
        <ul class="header-nav header-nav-profile">
            <li class="dropdown ms-hover">
                <a href="javascript:void(0);" class="dropdown-toggle ink-reaction" data-toggle="dropdown">
                    <img alt="image" class="profile-pic" src="/seguranca/imagemperfil.php">
                </a>
                <ul class="dropdown-menu animation-dock">
                    <li>
                        <a class="select-sistemas">
                            <select id="chosen-sistema" onclick="" class="form-control" data-placeholder="Selecione" onchange="javascript:changeSystem(this.value)">
                                <?php foreach ($sistemas as $sistema): ?>
                                    <?php $selected = ($sistema['sisid'] == $_SESSION['sisid']) ? 'selected="selected"' : null; ?>
                                    <option <?php echo $selected; ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                                <?php endforeach ?>
                            </select>
                            <script type="text/javascript">
                                $(document).ready(function() {
                                    $('.dropdown-menu select, .dropdown-menu a.select-sistemas').click(function(e) {
                                        e.stopPropagation();
                                    });
                                })
                            </script>
                        </a>
                    </li>
                    <?php if ($_SESSION['superuser'] == 1): ?>
                        <li class="dropdown-header">Super Usuário</li>
                    <?php else: ?>
                        <li class="dropdown-header">Opções Usuário</li>
                    <?php endif; ?>
                    <?php if ($_SESSION['superuser'] == 1 || $_SESSION['usucpf'] != $_SESSION['usucpforigem'] || $exibirSimular == true || $db->testa_uma() || $_SESSION['usuuma']): ?>
                        <li>
                            <?php if ($_SESSION['usucpf'] == $_SESSION['usucpforigem']): ?>
                                <a data-toggle="modal" class="btSimularUsuario">
                                    <i class="glyphicon glyphicon-eye-open"></i> Simular Usuário
                                </a>
                            <?php else: ?>
                                <a class="voltar_usuario_origem">
                                    <i  class="glyphicon glyphicon-eye-close"></i> Voltar Usuário
                                </a>
                            <?php endif; ?>
                        </li>
                    <?php endif; ?>
                    <li>
                        <a id="btUser" data-toggle="modal" data-target="#modal-user">
                            <i class="glyphicon glyphicon-wrench"></i> Gerenciar seus dados
                        </a>
                    </li>
                    <?php if ($_SESSION['superuser'] == 1 || $exibirGraficoWorflow == true): ?>
                        <li>
                            <?php $sql = "select tpdid from workflow.tipodocumento where sisid = '{$_SESSION['sisid']}'"; ?>
                            <?php $tipoDocumento = $db->pegaUm($sql); ?>
                            <?php if ($tipoDocumento): ?>
                                <a data-toggle="modal" class="btPainelWorkflow">
                                    <i class="glyphicon glyphicon-stats"></i> Painel do workflow
                                </a>
                            <?php endif ?>
                        </li>
                    <?php endif; ?>
                    <li class="divider"></li>
                    <li>
                        <a onclick="javascript:window.location.href = '/logout.php'">
                            <i class="fa fa-sign-out"></i> Sair
                        </a>
                    </li>
                </ul><!--end .dropdown-menu -->
            </li><!--end .dropdown -->
        </ul>
    </div>
    <div class="navbar-center hidden-xs">
        <ul class="header-nav header-nav-options">
            <li style="width:55%">
                <select id="chosen-sistema" class="form-control chosen-select" data-placeholder="Selecione" onchange="javascript:changeSystem(this.value)">
                    <?php foreach ($sistemas as $sistema): ?>
                        <?php $selected = ($sistema['sisid'] == $_SESSION['sisid']) ? 'selected="selected"' : null; ?>
                        <option <?php echo $selected; ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                    <?php endforeach ?>
                </select>
            </li>
            <?php if ($arrAnoExercicio && is_array($arrAnoExercicio)): ?>
                <li style="width:15%">
                    <select id="chosen-exercicio" name="chosen-exercicio" class="form-control" data-placeholder="Exercicio" onchange="javascript:alteraExercicio(this.value)">
                        <?php foreach ($arrAnoExercicio as $anoExercicio): ?>
                            <?php $exercselected = ($anoExercicio['codigo'] == $_SESSION['exercicio']) ? 'selected="selected"' : null; ?>
                            <option <?php echo $exercselected; ?> value="<?php echo $anoExercicio['codigo'] ?>"><?php echo $anoExercicio['descricao'] ?></option>
                        <?php endforeach ?>
                    </select>
                    <form class="form-horizontal" id="setexerccorrente" name="setexerccorrente" action="<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>" method="post">
                        <input type="hidden" id="exercicio" name="exercicio" value=""  class="form-control" />
                    </form>
                    <script>
                        function alteraExercicio(id) {
                            $('#exercicio').val(id);
                            document.getElementById('setexerccorrente').submit();
                        }
                    </script>
                </li>
            <?php endif ?>
            <?php if (!IS_PRODUCAO): ?>
                <?php if (IS_TREINAMENTO): ?>
                    <h1 style="float: left; margin: 10px 0 0 30px; border: 3px dotted yellow; color: orange; padding: 3px;">TREINAMENTO</h1>
                <?php else: ?>
                    <?php $labelBanco = $_SESSION['baselogin'] == 'simec_espelho_producao'? 'Espelho': 'Desenvolvimento'; ?>
                    <li style="width:28%">
                                        <span class="database-info">
                                            <i class="fa fa-database"></i> <?php echo $labelBanco; ?><br>
                                            <i class="fa fa-cube"></i> <?php echo $nome_bd; ?>
                                        </span>
                    </li>
                <?php endif ?>
            <?php endif ?>
        </ul>
    </div>
    <div class="navbar-right hidden-xs">
        <ul class="header-nav header-nav-profile">
            <li class="dropdown ms-hover">
                <a href="javascript:void(0);" class="dropdown-toggle ink-reaction" data-toggle="dropdown">
                    <img alt="image" class="profile-pic" src="/seguranca/imagemperfil.php">
                    <span class="profile-info">
											<?php echo $_SESSION['usunome']; ?>
                        <small><?php echo $_SESSION['usufuncao']; ?></small>
										</span>
                </a>
                <ul class="dropdown-menu animation-dock">
                    <?php if ($_SESSION['superuser'] == 1): ?>
                        <li class="dropdown-header">Super Usuário</li>
                    <?php else: ?>
                        <li class="dropdown-header">Opções Usuário</li>
                    <?php endif; ?>
                    <?php if ($_SESSION['superuser'] == 1 || $_SESSION['usucpf'] != $_SESSION['usucpforigem'] || $exibirSimular == true || $db->testa_uma() || $_SESSION['usuuma']): ?>
                        <li>
                            <?php if ($_SESSION['usucpf'] == $_SESSION['usucpforigem']): ?>
                                <a data-toggle="modal" class="btSimularUsuario">
                                    <i class="glyphicon glyphicon-eye-open"></i> Simular Usuário
                                </a>
                            <?php else: ?>
                                <a class="voltar_usuario_origem">
                                    <i  class="glyphicon glyphicon-eye-close"></i> Voltar Usuário
                                </a>
                            <?php endif; ?>
                        </li>
                    <?php endif; ?>
                    <li>
                        <a id="btUser" data-toggle="modal" data-target="#modal-user">
                            <i class="glyphicon glyphicon-wrench"></i> Gerenciar seus dados
                        </a>
                    </li>
                    <?php if ($_SESSION['superuser'] == 1 || $exibirGraficoWorflow == true): ?>
                        <li>
                            <?php $sql = "select tpdid from workflow.tipodocumento where sisid = '{$_SESSION['sisid']}'"; ?>
                            <?php $tipoDocumento = $db->pegaUm($sql); ?>
                            <?php if ($tipoDocumento): ?>
                                <a data-toggle="modal" class="btPainelWorkflow">
                                    <i class="glyphicon glyphicon-stats"></i> Painel do workflow
                                </a>
                            <?php endif ?>
                        </li>
                    <?php endif; ?>
                    <li class="divider"></li>
                    <li>
                        <a onclick="javascript:window.location.href = '/logout.php'">
                            <i class="fa fa-sign-out"></i> Sair
                        </a>
                    </li>
                </ul><!--end .dropdown-menu -->
            </li><!--end .dropdown -->
        </ul>
        </nav>
    </div>

    <div class="ibox float-e-margins animated modal" id="modal_bootstrap_v1" role="dialog" aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <!--        <form method="post" name="formIniciativaDescricao" id="formIniciativaDescricao" class="form form-horizontal">-->
            <div class="modal-content">
                <div class="ibox-title">
                    <span id="titulo-modal"></span>
                    <div class="pull-right">
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    </div>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                </div>
                <div class="ibox-footer">
                </div>
            </div>
            <!--        </form>-->
        </div>
    </div>
<?php if (IS_TREINAMENTO): ?>
    <h3 id="msg_ambiente" style="color: yellow; background-color: red; text-align: center; padding: 10px; font-weight: normal;">
        ATENÇÃO! Este ambiente é destinado para TREINAMENTO do uso do sistema SIMEC.
    </h3>
<?php endif ?>

<?php
if ($acessoRapidoCabecalhoIncluido) {
    $acessoRapidoControlador = AcessoRapido\Controlador::pegarInstancia();
    if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
        echo $acessoRapidoControlador->montarCaixaBootstrap();
    } else {
        echo $acessoRapidoControlador->montarCaixa();
    }
}
?>
<script>
    $( document ).ajaxComplete(function() {
        $(".chosen-select").chosen();
    });
</script>

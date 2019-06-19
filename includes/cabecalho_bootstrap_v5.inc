<?php
// Verifica se a sessao expirou.
if (!isset($_SESSION['usucpf'])) {
    session_unset();
    $_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
    header('Location: login.php');
    exit();
}

if ($_POST['carregarSituacaoPainel']):

    if ($_POST['tpdid']):

        $sql = "select esdid as codigo, esddsc as descricao
        from workflow.estadodocumento
        where tpdid = {$_POST['tpdid']}
        order by descricao
        ";
        $estadosDocumento = $db->carregar($sql);
        ?>
        <select id="estado-documento-situacao-workflow" name="esdid[]" class="form-control sel_chosen chosen" multiple="multiple" data-placeholder="Selecione">
            <option value=""></option>
            <?php if ($estadosDocumento): ?>
                <?php foreach ($estadosDocumento as $estadoDocumento): ?>
                    <option value="<?php echo $estadoDocumento['codigo'] ?>"><?php echo $estadoDocumento['descricao'] ?></option>
                <?php endforeach; ?>
            <?php endif ?>
        </select>
    <?php else : ?>
        <select id="" name="esdid[]" class="form-control sel_chosen chosen" multiple="multiple" data-placeholder="Selecione">
        </select>
    <?php  endif; exit; ?>
<?php elseif ($_POST['gerarGraficoPainel']): ?>
    <?php include_once APPRAIZ . "includes/library/simec/Grafico.php"; ?>
    <div  style="padding: 15px; ">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td>
                    <?php
                    $dt_inicio = $_REQUEST['dt_inicio'] ? formata_data_sql($_REQUEST['dt_inicio']) : date('Y') . '-01-01';
                    $dt_fim = $_REQUEST['dt_fim'] ? formata_data_sql($_REQUEST['dt_fim']) : date('Y-m-d');
                    $tpdid = $_POST['tpdid'];
                    $esdid = $_POST['esdid'];


                    switch ($_REQUEST['tipo_exibicao']) {
                        case('D'):
                            $formatoBusca = 'YYYYMMDD';
                            $formatoExibicao = 'DD/MM/YYYY';
                            break;
                        case('A'):
                            $formatoBusca = 'YYYY';
                            $formatoExibicao = 'YYYY';
                            break;
                        default:
                            $formatoBusca = 'YYYYMM';
                            $formatoExibicao = 'MM/YYYY';
                            break;
                    }

                    if ($esdid && is_array($esdid)) {
                        $esdid = 'and aed.esdiddestino in (' . implode(', ', array_filter($esdid)) . ')';
                    } else {
                        $esdid = '';
                    }

                    if ($tpdid == 44) {
                        // Se for fluxo PAR
                        $innerPar = 'inner join par.instrumentounidade iu on iu.docid = d.docid';
                    } else {
                        $innerPar = '';
                    }

                    $sql = "select count(*) as valor, to_char(h.htddata, '{$formatoBusca}') as data, to_char(h.htddata, '{$formatoExibicao}') as categoria, aed.esdiddestino, e.esddsc as descricao
                            from
                                workflow.historicodocumento h
                                inner join  workflow.documento d  on h.docid = d.docid
                                inner join workflow.acaoestadodoc aed on aed.aedid = h.aedid
                                inner join workflow.estadodocumento e on aed.esdiddestino = e.esdid
                                {$innerPar}
                            where h.htddata BETWEEN '{$dt_inicio} 00:00:00' and '{$dt_fim} 23:59:59'
                            and e.tpdid = {$tpdid}
                            and e.esdstatus = 'A'
                            and aed.aedstatus = 'A'
                            $esdid
                            $esdid
                            group by data, categoria, aed.esdiddestino, descricao
                            order by data, descricao";

                    $sql2 = "select count (d.docid) as valor, esd.esddsc  descricao
                                FROM workflow.documento d
                                inner join workflow.estadodocumento esd on esd.esdid = d.esdid
                              {$innerPar}
                                where esd.tpdid = {$tpdid}
                                group by esd.esddsc,esd.tpdid";

                    $grafico = new Grafico(NULL, false);
                    ?>
                    <table class="tabela">
                        <tr>
                            <td>
                                <?php $grafico->setTipo(Grafico::K_TIPO_LINHA)->setWidth('50%')->setHeight('300px')->setTitulo('Histórico')->gerarGrafico($sql); ?>
                            </td>
                            <td>
                                <?php $grafico->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('40%')->setHeight('300px')->setTitulo('Situação atual')->gerarGrafico($sql2); ?>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
    </div>

    <?php
    exit;
endif;

if ($_POST['carregarUsuariosSistema']) {
    include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';

    $usuariosSistema = carregarUsuariosSistema(true);
    $listing = new Listing();
    $listing->setTypePage('M');
    $listing->setActions(array('eye-open' => 'simularUsuariosSistemas'));
    $listing->setHead(array('CPF', 'Nome', 'Email'));
    $listing->listing($usuariosSistema);
    exit;
}

if ($_POST['simularUsuariosSistema']) {
    $cpf = $_POST['cpf'];

    global $db;

    $sql = <<<DML
SELECT DISTINCT u.usucpf,
                u.usunome,
                u.usuemail
  FROM seguranca.usuario u
    INNER JOIN seguranca.perfilusuario pf ON (pf.usucpf = u.usucpf)
    INNER JOIN seguranca.perfil p ON (p.pflcod = pf.pflcod
                                      AND p.sisid = {$_SESSION['sisid']})
    INNER JOIN seguranca.usuario_sistema us ON (us.usucpf = u.usucpf
                                                AND us.sisid = p.sisid)
  WHERE us.suscod = 'A'
    AND p.pflnivel >= (SELECT MIN(pflnivel)
                         FROM seguranca.perfil
                           INNER JOIN seguranca.perfilusuario ON (perfil.pflcod = perfilusuario.pflcod
                                                                  AND perfil.sisid = {$_SESSION['sisid']}))
    AND u.usucpf = '{$cpf}'
DML;

    $usuario = $db->pegaLinha($sql);
    if ($usuario) {
        $_SESSION['usucpf'] = $usuario['usucpf'];
        $_SESSION['usunome'] = $usuario['usunome'];
        $_SESSION['usuemail'] = $usuario['usuemail'];
        $_SESSION['acl'] = [];
        echo 'ok';
    }

    exit;
}

// Definindo o tema do sistema.
if (isset($_POST['theme'])) {
    $theme = $_POST['theme'];
    setcookie("theme", $theme, time() + 60 * 60 * 24 * 30, "/");
} else if (isset($_COOKIE["theme"])) {
    $theme = $_COOKIE["theme"];
} else {
    
}

$theme = '';

$arrTheme = array( );

$sistemas = carregarSistemas();
$menus = menuHtml(); //montaMenuBootstrap(carregarMenus($_SESSION['sisid'], $_SESSION['usucpf'])); //

if ($_SESSION['sisdiretorio'] && $_SESSION["sisexercicio"] == 't') {
    $sql = "select schemaname , relname from pg_stat_user_tables
                where schemaname = '{$_SESSION['sisdiretorio']}'
                and relname = 'programacaoexercicio'
                order by relname;";
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

// pega a página inicial do sistema
$sql = sprintf("SELECT TRIM( paginainicial ) AS paginic FROM seguranca.sistema WHERE sisid = %d", $_SESSION['sisid']);
$paginic = $db->pegaUm($sql);
?>
<!DOCTYPE html>
<html>
    <head>
        <title>SIMEC - Sistema Integrado de Monitoramento Execução e Controle</title>
        <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <link href="/zimec/public/temas/simec/fonts/awesome/font-awesome.css?t=1.0" rel="stylesheet">

        <link rel='stylesheet' type='text/css' href='../includes/loading.css'/>
        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css'/>
        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>

        <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/blueimp/css/blueimp-gallery.min.css">
        <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload.css">
        <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui.css">
        <link  type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

        <!-- CSS adjustments for browsers with JavaScript disabled -->
        <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-noscript.css"></noscript>
        <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui-noscript.css"></noscript>
        <link href="../library/sliding_panel/css/slide.css" rel="stylesheet" media="screen">
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" > 


        <link href="/library/simec/css/custom.css" rel="stylesheet" media="screen">
        <link href="/library/simec/css/css_reset.css" rel="stylesheet">
        <link href="/library/simec/css/barra_brasil.css" rel="stylesheet">
        <link href="../includes/Estilo.css" rel="stylesheet" type="text/css"/>
        <link href='../includes/listagem.css' rel='stylesheet' type='text/css'/>
        <!--Inclue do arquivo que carrega o CSS da pagina Inicio do Obras-->
        <link rel="stylesheet" type="text/css" media="screen, print" href="../includes/layoutNovoObras2.css?<?= time(); ?>">
        <link href="/library/simec/css/custom.css" rel="stylesheet" media="screen">
        <link href="/includes/css/custom-layout-v5.css?id<?=microtime(true)?>" rel="stylesheet" media="screen">
        <link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
        <!-- <link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" media="screen"> -->



        <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="UTF-8"></script>
        <!-- Bootstrap JS -->
        <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
        <script src="/zimec/public/temas/simec/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
        <script src="../library/chosen-1.0.0/chosen.jquery.js"></script>
        <script language="javascript">$1_11 = $.noConflict();</script>

        <!-- jQuery JS -->
        <script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery-isloading.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery.simple-color.js" type="text/javascript" charset="ISO-8895-1"></script>
        

        <script language="JavaScript" src="/includes/funcoes.js"></script> 
        <script language="JavaScript" src="/estrutura/js/funcoes.js"></script> 

        <script language="javascript" src="/library/bootbox/bootbox.min.js"></script>
        <script type="text/javascript" language="javascript">bootbox.setDefaults({locale:'pt'});</script>

        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>



 
        <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
        <!--[if lt IE 9]>-->
        <!--<script src="js/html5shiv.js"></script>-->
        <!--<script src="js/respond.min.js"></script>-->
        <![endif]-->

        <!-- Correções para o bootstrap 3 -->
        <link rel="stylesheet" href="../library/pim/pim.css?<?= time(); ?>">

    </head>
    <body>
        <?php include_once APPRAIZ . 'includes/cabecalho_css.php'; ?>
        <!-- begin loader -->
        <div class="loading-dialog notprint" id="loading">
            <div id="overlay" class="loading-dialog-content">
                <div class="ui-dialog-content">
                    <img src="../library/simec/img/loading.gif">
                    <span>
                        O sistema esta processando as informações. <br/>
                        Por favor aguarde um momento...
                    </span>
                </div>
            </div>
        </div>
        <!-- end loader -->

        <script lang="javascript">
            $(document).ready(function(){
                $("#btUser").click(function(){
                        $(".mensagensAvisos").hide("slow");
                        url = window.location.href;
                        $.get(url,{limparAvisos:"true"},function(response){});
                    }
                );
                $(".confirmarAviso").click(function(e){
                    e.preventDefault();
                    url = $(this).attr('href');
                    bootbox.confirm('Deseja abrir a localização da referência agora? <br> Trabalhos não salvos serão perdidos.', function(opcao){
                        if(opcao){
                            location.href = url;
                        }
                    });
                }
                );


            });
            function changeSystem(system)
            {
                location.href = "../muda_sistema.php?sisid=" + system;
            } 
        </script>

        <!-- <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen" >
            <tr bgcolor="#ffffff">
                <td><img src="/zimec/public/img/logo-semPapel.png" ></td>
                <td height="20" nowrap>
                    SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação<br>
                    Ministério da Educação / SE - Secretaria Executiva<br>
                    DTI - Diretoria de Tecnologia da Informação<br>
                </td>
                <td height="20" align="right" style="font-size:9px;">
                    Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
                    Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
                </td>
            </tr>
        </table> -->
       <!-- <div style="height: 70px;" class="espacador-simec" ></div> -->

        <div class="navbar navbar-default navbar-fixed-top " style="box-shadow: #888 0px 0px 8px -3px">
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <ul class="nav">
                    <li>
                        <a class="navbar-brand" href="#" onclick="javascript:changeSystem(<?php echo $_SESSION['sisid'] ?>)" style="left: -200px">
                             
                            <img style="width: 85px; margin-left: 10px;" border="0"  src="/zimec/public/img/logo-semPapel.png">
                        </a>
                    </li>
                </ul>
            </div>
            <form id="setperfil" name="setperfil" action="<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>" method="post">
                <input type="hidden" id="exercicio" name="exercicio" value=""  class="form-control" />
            </form>
            <div class="collapse navbar-collapse">
                <div class="row">
                    <div class="col-md-4 nav navbar-nav nav-dropdowns">

                        <?php echo $menus ?>

                    </div>
                    <div class="col-md-3 nav navbar-nav nav-sistemas">
                        <div class="navbar-brand" style="padding: 5px 0px 5px 0px; width: 100%; text-align: center !important;">
                            <select  data-placeholder="Escolha um módulo do sistema..." class="chosen-select select-change-system"  tabindex="2" onchange="javascipt:changeSystem(this.value);">
                                <option value="">Escolha um módulo do sistema...</option>
                                <?php foreach ($sistemas as $sistema): ?>
                                    <option <?php if ($_SESSION['sisid'] == $sistema['sisid']) echo 'selected="true"' ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                                <?php endforeach; ?>
                            </select>
                        </div>
                    </div>
                    <div class="col-md-4 nav navbar-right navbar-btn" style="text-align: right">
                         
                            <div class="btn-group adm-users-bar" >
                                    
                                    <button id="btUser" data-toggle="modal" data-target="#modal-user" class="btn btn-sm btn-default">
                                        <i class="glyphicon glyphicon-user"></i> &nbsp;
                                            <?php
                                            if (!empty($_SESSION['usunome'])) {
                                                $nomeUsuario = explode(' ', $_SESSION['usunome']);
                                                $nomeUsuario = reset($nomeUsuario) . ' ' . end($nomeUsuario);
                                                echo $nomeUsuario;
                                            } 
                                            $sql = "SELECT
                                             COUNT( 0 )
                                            FROM
                                             public.avisousuario
                                            WHERE
                                             usucpf = '{$_SESSION['usucpf']}'
                                            AND
                                             avbstatus = 'A'";
                                             $QtdAvisos = $db->pegaUm($sql);
                                             if($QtdAvisos > 0){
                                                 echo "&nbsp; <spam class=\"mensagensAvisos\">$QtdAvisos</spam>";
                                             }
                                             /* Marcar os avisos com lidos, ao pressionar o botão de ler*/
                                             if($_REQUEST['limparAvisos']){
                                                 $sql = "UPDATE public.avisousuario SET avbstatus = 'L' WHERE usucpf = '{$_SESSION['usucpf']}'";
                                                 $db->executar($sql);
                                             }
                                            ?>
                                    </button> 



                                <!-- Workflow -->
                                <?php if ($_SESSION['superuser'] == 1 || $_SESSION['usucpf'] != $_SESSION['usucpforigem'] || $exibirSimular == true || $db->testa_uma() || $_SESSION['usuuma']): ?>
                                        <?php if ($_SESSION['usucpf'] == $_SESSION['usucpforigem']): ?>
                                            <button data-toggle="modal" class="btn btn-sm btn-default" id="btSimularUsuario">
                                                <i class="glyphicon glyphicon-eye-open pd-3" title="Simular Usuário"></i>
                                            </button>
                                        <?php else: ?>
                                            <button class="btn btn-sm btn-default" id="voltar_usuario_origem" title="Voltar Usuário">
                                                <i  class="glyphicon glyphicon-eye-close pd-3"></i>
                                            </button>
                                        <?php endif; ?> 
                                        <button onclick="javascript:window.location.href = '/seguranca/seguranca.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf=<?php echo $_SESSION['usucpf'] ?>'" class="btn btn-sm btn-default" title="Seus dados">
                                            <i class="glyphicon glyphicon-wrench pd-3"></i>
                                        </button> 
                                <?php endif ?> 
                                <a href="/logout.php" title="Sair">
                                    <button class="btn btn-sm btn-default">
                                        <i style="color: red;" class="glyphicon glyphicon-off pd-3" ></i>
                                    </button>
                                </a>

                            </div> 
                    </div>
                </div>
            </div><!--/.nav-collapse -->
            <!--</div>-->
        </div>
        <div id="top-shadow" style="display: none; ;height: 51px; background-color: red; margin-bottom: 30px; box-shadow: #000 0px -1px 18px; position: fixed; top: 0; width: 100%; z-index: 1;">
        </div>
<script>
function simularUsuariosSistemas(cpf)
{
    $.post(window.location.href, {simularUsuariosSistema: 'true', cpf: cpf}, function(html) {
        if (html == 'ok') {
            document.location.reload(true);
        }
    });
}

$('#voltar_usuario_origem').click(function() {
    simularUsuariosSistemas('<?php echo $_SESSION['usucpforigem']?>');
});

$('.navbar-brand').animate({left: 0}, 0);
$(window).scroll(function() {
    var coordenada = $(document).scrollTop();
    if (coordenada > 0) {
        $('#top-shadow').fadeIn('fast');
    } else {
        $('#top-shadow').fadeOut('fast');
    }
});
</script>
        <div class="view-container">
<?php

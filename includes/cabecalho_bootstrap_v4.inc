<?php

use AcessoRapido\Controlador;

require_once APPRAIZ . 'includes/cabecalho_bootstrap_v4_init.inc';
?>
    <!DOCTYPE html>
    <html>
    <head>
        <title>Sem papel - Governo do Estado de São Paulo</title>
        <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <!-- Fonts  -->
        <link href="/zimec/public/temas/simec/fonts/awesome/font-awesome.css?t=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/glyphicons/glyphicons.css?t=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/inconsolata/inconsolata.css?t=1.0" rel="stylesheet">
        <link href="/zimec/public/temas/simec/fonts/opensans/opensans.css?t=1.0" rel="stylesheet">

        <!-- Mainly scripts -->
        <script src="/zimec/public/temas/simec/v291/js/jquery-3.1.1.min.js"></script>
        <script src="/zimec/public/temas/simec/v291/js/popper.min.js"></script>
        <script src="/zimec/public/temas/simec/v291/js/bootstrap.min.js"></script>
        <script src="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.min.js"></script>

        <!-- Data picker -->
        <script src="/zimec/public/temas/simec/js/plugins/datapicker/bootstrap-datepicker.js"></script>
        <script src="/zimec/public/temas/simec/js/plugins/datapicker/locales/bootstrap-datepicker.pt-BR.js"></script>

        <!-- jQuery UI custom -->
        <script src="/zimec/public/temas/simec/js/jquery-ui.custom.min.js"></script>

        <!-- Sweet Aler -->
        <script src="/zimec/public/temas/simec/js/plugins/sweetalert/sweetalert.min.js"></script>

        <script src="/zimec/public/temas/simec/js/plugins/touchspin/jquery.bootstrap-touchspin.min.js"></script>
        <link href="/zimec/public/temas/simec/css/plugins/summernote/summernote-bs3.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/v291/css/bootstrap.css" rel="stylesheet" media="all">

        <!-- Highcharts -->
        <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts.js"></script>
        <script language="javascript" src="../includes/Highcharts-4.0.3/js/highcharts-more.js"></script>
        <script language="javascript" src="../includes/Highcharts-4.0.3/js/modules/solid-gauge.src.js"></script>

        <!-- Toastr style -->
        <link href="/zimec/public/temas/simec/v291/css/plugins/toastr/toastr.min.css" rel="stylesheet">

        <!-- Gritter -->
        <link href="/zimec/public/temas/simec/v291/js/plugins/gritter/jquery.gritter.css" rel="stylesheet">

        <link href="/zimec/public/temas/simec/v291/css/animate.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/v291/css/style.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.theme.min.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.structure.min.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.min.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/iCheck/custom.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/chosen/chosen.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/gritter/jquery.gritter.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/nouslider/jquery.nouislider.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/datapicker/datepicker3.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/switchery/switchery.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/bootstrap-tour/bootstrap-tour.min.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/summernote/summernote.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/sweetalert/sweetalert.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/bootstrap-switch/bootstrap-switch.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/awesome-bootstrap-checkbox/awesome-bootstrap-checkbox.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/awesome-bootstrap-checkbox/awesome-bootstrap-checkbox.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/nanoscroll/nanoscroller.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/plugins/touchspin/jquery.bootstrap-touchspin.min.css" rel="stylesheet">
        <link href="/zimec/public/temas/simec/css/simec.css?t=1.0" rel="stylesheet" media="all">

        <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
        <!--[if lt IE 9]>
        <!--<script src="js/html5shiv.js"></script>-->
        <!--<script src="js/respond.min.js"></script>-->
        <!--[endif]-->
        <link href="/zimec/public/temas/simec/css/print.css?t=1.0" rel="stylesheet" media="print" />
        <script data-pace-options='{ "ajax": false }' src="/zimec/public/temas/simec/js/plugins/pace/pace.min.js"></script>

        <?php
        if(is_object($this)){
            echo $this->montarIncludeCSS(true);
            echo $this->montarIncludeJS(true);
        }
        ?>
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

<?php
    require_once APPRAIZ . 'includes/cabecalho_bootstrap_v4_navbar.inc';

    initAutoload();

    /** @var array $arrSistemasSemMensagens sistemas que não devem fazer a consulta de verificação de mensagens */
    $arrSistemasSemMensagens = array(); //[231];

    if (!in_array($_SESSION['sisid'], $arrSistemasSemMensagens)) {
        include_once APPRAIZ .'sac/classes/controller/Aviso.class.inc';
        $controllerAviso = new Sac_Controller_Aviso();
        $controllerAviso->verificarMensagens();
    }


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
        $(document).ajaxComplete(function () {
            $(".chosen-select").chosen();
        });
    </script>

<div class="wrapper wrapper-content"> 
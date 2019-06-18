<?php
$arrTheme = array('default', 'default-inverse', 'ameliaa', 'cerulean', 'cosmo', 'cyborg', 'flatly', 'journal', 'readable', 'simplex', 'slate', 'spacelab', 'united', 'yeti');

if (!isset($_COOKIE['navbar'])) {
    $_COOKIE['navbar'] = 'false';
}
$navBar = $_REQUEST['obra'] ? null : 'mini-navbar';
?>
<!DOCTYPE html>
<html>
<head>
    <title>SIMEC - Sistema Integrado de Monitoramento Execução e Controle</title>
    <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <!-- Mainly scripts -->
    <script src="/zimec/public/temas/simec/js/jquery-1.11.3.js"></script>
    <script src="/zimec/public/temas/simec/js/bootstrap.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.min.js"></script>
    <script src="/estrutura/js/funcoes.js"></script>
    <script src="/includes/funcoes.js"></script>

    <!-- Highcharts -->
    <script language="javascript" src="/includes/Highcharts-4.0.3/js/highcharts.js"></script>
    <script language="javascript" src="/includes/Highcharts-4.0.3/js/highcharts-more.js"></script>
    <script language="javascript" src="/includes/Highcharts-4.0.3/js/modules/solid-gauge.src.js"></script>

    <!-- Data picker -->
    <script src="/zimec/public/temas/simec/js/plugins/datapicker/bootstrap-datepicker.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/datapicker/locales/bootstrap-datepicker.pt-BR.js"></script>

    <!-- Ion Range Slider -->
    <script src="/zimec/public/temas/simec/js/plugins/ionRangeSlider/ion.rangeSlider.min.js"></script>


    <!-- textext -->
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.core.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.tags.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.autocomplete.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.focus.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.prompt.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/textext/css/textext.plugin.arrow.css" rel="stylesheet">

    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.core.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.tags.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.autocomplete.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.suggestions.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.filter.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.focus.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.prompt.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.ajax.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.arrow.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/textext/js/textext.plugin.arrow.js"></script>

    <!-- Switchery -->
    <script src="/zimec/public/temas/simec/js/plugins/switchery/switchery.js"></script>

    <script src="js/snap.svg.js"></script>
    <script src="js/jquery.browser.mobile.js"></script>
    <script src="js/liquid.meter.js"></script>
    <script src="js/Mapas.js"></script>


    <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
    <!--<script src="js/html5shiv.js"></script>-->
    <!--<script src="js/respond.min.js"></script>-->
    <![endif]-->

    <link href="/zimec/public/temas/simec/fonts/awesome/font-awesome.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/fonts/glyphicons/glyphicons.css" rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/bootstrap.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.theme.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.structure.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/js/plugins/jquery-ui/jquery-ui.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/iCheck/custom.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/chosen/chosen.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/colorpicker/bootstrap-colorpicker.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/gritter/jquery.gritter.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/nouslider/jquery.nouislider.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/datapicker/datepicker3.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/ionRangeSlider/ion.rangeSlider.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/ionRangeSlider/ion.rangeSlider.skinHTML5.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/ionRangeSlider/normalize.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/switchery/switchery.css" rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/plugins/summernote/summernote.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/summernote/summernote-bs3.css" rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/plugins/sweetalert/sweetalert.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/dropzone/dropzone.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/dropzone/basic.css" rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/plugins/bootstrap-switch/bootstrap-switch.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/awesome-bootstrap-checkbox/awesome-bootstrap-checkbox.css"
          rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/plugins/awesome-bootstrap-checkbox/awesome-bootstrap-checkbox.css"
          rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/nanoscroll/nanoscroller.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/jstree/style.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/animate.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/style.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/simec.css" rel="stylesheet">
</head>
<body class="fixed-nav pace-done">
<div id="wrapper" style="background: #273A4A !important;">
    <nav class="navbar-default navbar-static-side" role="navigation" >
        <div class="sidebar-collapse">
            <?php echo $menuHtml ?>
        </div>
    </nav>
    <div id="page-wrapper">
        <div class="row border-bottom">
            <nav class="navbar navbar-fixed-top" role="navigation"
                 style="margin: 0px; height: 61px; border-bottom: 1px solid #273A4A !important; background: #273A4A"
                 no-repeat
            ">
            <div class="navbar-left">
                <ul class="header-nav header-nav-options">
                    <li class="header-nav-brand">
                        <img src="/zimec/public/img/logo-simec.png" class="img-responsive"
                                                      style="width: 170px; padding: 5px;"></li>

                </ul>
            </div>

            <style>
                .tarja-aviso {
                    margin-top: 15px;
                    word-spacing: 10px;
                }


                .tarja-aviso span {
                    /*color: #fff14f !important;*/
                    color: #FFC600 !important;
                    font-size: 20px !important;
                }

                .header-nav .header-nav-brand{height: 58px !important;background: #273A4A }
                .footer{display: none}

                .gray-bg{background: #D2C6C6}
                .blue-bg{background: #273A4A}

                dl{margin-bottom: 10px !important;}

            </style>

            <div class="navbar-center" style="width: 70%;">
                <ul class="header-nav header-nav-options">
                    <li style="padding: 0px 0 14px 12px; width:100%" class="text-center tarja-aviso hidden-xs left">
                        <link href='https://fonts.googleapis.com/css?family=Candal' rel='stylesheet' type='text/css'>
                        <span style="font-size: 26px !important; font-family: 'Candal', sans-serif;', serif;">Programa Nacional de Apoio ao Transporte Escolar - PNATE</span>
                    </li>
                </ul>
            </div>


            </nav>
        </div>
<?php session_start(); ?>
<!DOCTYPE html>
<html>

    <head>

        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <title>SIMEC | 500 Error</title>

        <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
        <link href="library/font-awesome/css/font-awesome.css" rel="stylesheet">

        <link href="library/bootstrap-animate/animate.css" rel="stylesheet">
        <link href="library/main-style.css" rel="stylesheet">

        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta name="description" content="">
        <meta name="author" content="">

        <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

        <!-- Styles Boostrap -->
        <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
        <link href="library/bootstrap-3.0.0/css/portfolio.css" rel="stylesheet">
        <link href="library/chosen-1.0.0/chosen.css" rel="stylesheet">
        <link href="library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">

        <!-- Custom CSS -->
        <link href="estrutura/temas/default/css/css_reset.css" rel="stylesheet">
        <link href="estrutura/temas/default/css/estilo.css" rel="stylesheet">
        <link href="library/simec/css/custom_login.css" rel="stylesheet">

        <!-- Custom Fonts -->
        <link href="library/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
        <link href="http://fonts.googleapis.com/css?family=Source+Sans+Pro:300,400,700,300italic,400italic,700italic"
              rel="stylesheet" type="text/css">

        <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
        <!--[if lt IE 9]>
        <script src="estrutura/js/html5shiv.js"></script>
        <![endif]-->
        <!--[if IE]>
        <link href="estrutura/temas/default/css/styleie.css" rel="stylesheet">
        <![endif]-->

        <!-- Boostrap Scripts -->
        <script src="library/jquery/jquery-1.10.2.js"></script>
        <script src="library/jquery/jquery.maskedinput.js"></script>
        <script src="library/bootstrap-3.0.0/js/bootstrap.min.js"></script>
        <script src="library/chosen-1.0.0/chosen.jquery.min.js"></script>
        <script src="library/bootstrap-switch/js/bootstrap-switch.min.js"></script>

        <!-- Custom Scripts -->
        <script type="text/javascript" src="../includes/funcoes.js"></script>

        <!-- FancyBox -->
        <script type="text/javascript" src="library/fancybox-2.1.5/source/jquery.fancybox.js?v=2.1.5"></script>
        <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/jquery.fancybox.css?v=2.1.5"
              media="screen"/>
        <script type="text/javascript" src="library/fancybox-2.1.5/lib/jquery.mousewheel-3.0.6.pack.js"></script>

        <!-- Add Button helper (this is optional) -->
        <link rel="stylesheet" type="text/css"
              href="library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.css?v=1.0.5"/>
        <script type="text/javascript"
        src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.js?v=1.0.5"></script>

        <!-- Add Thumbnail helper (this is optional) -->
        <link rel="stylesheet" type="text/css"
              href="library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.css?v=1.0.7"/>
        <script type="text/javascript"
        src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.js?v=1.0.7"></script>

        <!-- Add Media helper (this is optional) -->
        <script type="text/javascript"
        src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-media.js?v=1.0.6"></script>
    </head>

    <body class="page-index">
        <!-- Login -->
        <section id="login" class="login">
            <div class="content">
                <div class="middle-box text-center animated fadeInDown">
                    <div class="row">
                        <div class="col-md-4 col-md-offset-4">
                            <div class="middle-box text-center animated fadeInDown">
                                <div class="panel panel-default">
                                    <div class="panel-heading">
                                        <span class="glyphicon glyphicon-lock"></span> ERRO 500 (Erro Interno da Aplicação)
                                    </div>
                                    <div class="panel-body">
                                        <h1 class="font-bold"></h1>
                                        <div class="error-desc">
                                            <div class="error-desc">
                                                <p>Estamos com problemas para apresentar a página solicitada, nossos analistas já foram informados e retornaremos com a normalização desta página em breve<i class="js-show-erro">.</i></p>
                                                <a class="btn btn-primary" href="<?php echo $_SESSION['sisarquivo'] . '/' . $_SESSION['sisarquivo']; ?>.php?modulo=inicio&acao=C">Voltar</a>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                    </div>
                </div>
            </div>
        </section>
        <div class="row" id="error" style="display:none;">
            <pre>

                <?php if(isset($_SESSION['__ERRO'])){
                    echo $_SESSION['__ERRO'];
                    unset($_SESSION['__ERRO']);
                }
                ?>
            </pre>
        </div>
        <script>
            $(document).ready(function(){
                $('.js-show-erro').on('click',function(event){
                    if(event.ctrlKey){
                        $('#error').show();
                    }
                });
            });
        </script>
    </body>
</html>

<?php
include_once('configuracao.inc.php');

$ide = new Par3_Model_Ide();
$controllerIde = new Par3_Controller_Ide();

$tipo = $_GET['tipo'];
$ide->popularDadosObjeto($_GET);
include_once('menu.inc.php');
include_once('cabecalho.inc.php');
?>
    <nav class="navbar-default navbar-static-side" role="navigation" style="background: #273A4A;width: 300px;height: 785px; overflow:scroll;">
        <div class="sidebar-collapse">
            <?php echo $menuHtml ?>
        </div>
    </nav>
    <div id="page-wrapper" class="gray-bg" style="margin-left: 300px;">
        <div class="row border-bottom">
            <nav class="navbar navbar-fixed-top" role="navigation" style="margin: 0px; height: 61px; border-bottom: 1px solid #273A4A !important; background: #273A4A no-repeat">
                <div class="navbar-left">
                    <ul class="header-nav header-nav-options">
                        <li class="header-nav-brand">
                            <a href="http://simec.mec.gov.br/"><img src="/zimec/public/img/logo-simec.png" class="img-responsive" style="width: 170px; padding: 5px;"></a>
                        </li>
                    </ul>
                </div>
                <div class="navbar-center" style="margin-left: 60px; width: 80%;">
                    <ul class="header-nav header-nav-options">
                        <li style="width:100%" class="text-left tarja-aviso hidden-xs left">
                            <h1 style="margin: 0px 0px 0px 20px;color:#fff;">Indicadores Demográficos e Educacionais <?php $ide->getTituloEstado(); ?> <?php $ide->getTituloMunicipio(); ?></h1>
                        </li>
                    </ul>
                </div>
            </nav>
        </div>
        <div class="wrapper wrapper-content animated fadeIn">

            <div class="row">
                <div class="col-lg-12">
                    <div class="panel panel-default">
                        <div class="panel-body">

                            <p>
                                Este conjunto de tabelas traz informações sobre população, Produto Interno Bruto (PIB),
                                Índice de Desenvolvimento Humano (IDH), Índice de Desenvolvimento da Infância (IDI) e taxa de analfabetismo.
                            </p>

                            <p>
                                Também há estatísticas sobre a educação no Estado. Observe o título de cada tabela que indicará se a
                                informação refere-se às Redes de Educação Municipais no estado ou à Rede de Educação Estadual.
                            </p>

                            <p>
                                Os indicadores cuja fonte dos dados não é indicada nas tabelas foram gerados pelo Inep / MEC.
                            </p>

                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela3.inc.php'; ?>
                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela6.inc.php'; ?>
                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela7.inc.php'; ?>
                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela8.inc.php'; ?>
                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela9.inc.php'; ?>
                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela10.inc.php'; ?>
                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela11.inc.php'; ?>
                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela12.inc.php'; ?>
                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela13.inc.php'; ?>
                            <?php include_once APPRAIZ . 'par3/classes/model/Ide/view/tbIdeTabela14.inc.php'; ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

<?php include_once('rodape.inc.php'); ?>
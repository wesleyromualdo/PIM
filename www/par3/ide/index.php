<?php
include_once('configuracao.inc.php');

$ide = new Par3_Model_Ide();
$controllerTerritorios = new Territorios_Controller_Territorios();

switch ($_GET['requisicao']) {
    case 'carregarMunicipio':
        echo $controllerTerritorios->recuperarMunicipiosPorUf($_REQUEST['estuf']);
        die;
}
include_once('cabecalho.inc.php');
?>
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

                    <div class="row">
                        <div class="col-lg-4 text-center">
                            <h3>Consulte os Indicadores Demográficos e Educacionais dos Estados e Municípios</h3>
                            <br>

                            <p>Conheça o perfil da população e da rede de ensino de cada um dos estados e municípios brasileiros.
                                Aqui estão disponíveis indicadores populacionais e educacionais reunidos e sistematizados, por estado e município.</p>

                            </p> Para melhor entendimento dos dados, acesse o Glossário de Termos.</p>
                        </div>
                        <div class="col-lg-4">

                            <div class="panel panel-success">
                                <div class="panel-heading">
                                    <h4>ESTADO</h4>
                                </div>
                                <div class="panel-body">
                                    <form method="get" name="form-ide-est" id="form-ide-est" action="indicador.php" class="form form-horizontal">
                                        <input type="hidden" name="tipo" value="estado">
                                        <?php echo $simec->select('estuf', 'UF', $_GET['estuf'], $ide->getEstados(), array('required', 'id' => 'uf')); ?>
                                        <div class="form-group">
                                            <div class="col-lg-12 text-right">
                                                <button type="submit" class="btn btn-sm btn-success btn-lg btnPesquisar">
                                                    <span class="glyphicon glyphicon-search"> </span> Consultar
                                                </button>
                                            </div>
                                        </div>
                                    </form>
                                </div>
                            </div>
                        </div>

                        <div class="col-lg-4">
                            <div class="panel panel-primary">
                                <div class="panel-heading">
                                    <h4>MUNICÍPIO</h4>
                                </div>
                                <div class="panel-body">
                                    <form method="get" name="form-ide-mun" id="form-ide-mun" action="indicador.php" class="form form-horizontal">
                                        <input type="hidden" name="tipo" value="municipio">
                                        <?php echo $simec->select('estuf', 'UF', $_GET['estuf'], $ide->getEstados(), array('required')); ?>
                                        <div id="div_municipios">
                                            <?php echo $ide->montarMunicipios($_GET['estuf'], $_GET['muncod']); ?>
                                        </div>
                                        <div class="form-group">
                                            <div class="col-lg-12 text-right">
                                                <button type="submit" class="btn btn-sm btn-success btn-lg btnPesquisar">
                                                    <span class="glyphicon glyphicon-search"> </span> Consultar
                                                </button>
                                            </div>
                                        </div>
                                    </form>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<?php include_once('rodape.inc.php'); ?>

<style>
    #page-wrapper {
        margin-left: 0px !important;
    }
</style>
<script type="text/javascript">
    $(document).ready(function () {
        $('#form-ide-mun').on('change', '#estuf', function () {
            carregarSelectPorJson('?requisicao=carregarMunicipio&estuf=' + $('#estuf').val(),
                '#muncod', 'muncod', 'mundescricao', null, 'Selecione');
        });
    });
</script>
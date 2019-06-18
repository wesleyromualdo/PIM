<?php
include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ . 'includes/classes/modelo/questionario/QQuestionario.class.inc';
include_once APPRAIZ . 'par3/classes/model/TipoConfiguracoesPar.class.inc';

if ($_POST['atualizar']) {
    $atualiza = updateDataVigencia($_POST);
}
//ver($_REQUEST, d);
$controllerTipoConfiguracoes    = new Par3_Controller_TipoConfiguracoesParControle();
$sqlTipoConfiguracoes           = Par3_Model_TipoConfiguracoesPar::pegarSQLSelectCombo();
$controllerConfiguracoesPar     = new Par3_Controller_ConfiguracoesParControle();

switch ($_REQUEST['req']) {
    case 'salvarTipoConf':
        $controllerTipoConfiguracoes->salvar($_REQUEST);
        break;
    case 'salvarConf':
        $controllerConfiguracoesPar->salvar($_REQUEST);
        break;
    default:
        break;
}
?>
<?php echo $simec->title('Configurações PAR', ''); ?>
<div class="ibox-content">
    <?php if ($atualiza) : ?>
        <?php if ($atualiza['sucesso']) : ?>
            <div class="alert alert-success" id="sucesso">
                <b><?= $atualiza['sucesso'] ?></b>
            </div>
        <?php else : ?>
            <div class="alert alert-danger" id="erro">
                <b><?= $atualiza['erro'] ?></b>
            </div>
        <?php endif; ?>
    <?php endif; ?>

    <form name="formulario-configuracoes-par" id="formulario-configuracoes-par">
        <div class="row">
            <div class="col-lg-6 col-md-6 center">
                <?php echo $simec->select("tcpid", 'Tipo de Configuração', '', $sqlTipoConfiguracoes, array('maxlength' => '255', 'required')); ?>
            </div>
            <div class="col-md-2">
                <button type="button" class="btn btn-success novo  pull-left" data-toggle="modal" data-target="#modal"
                        title="Adicionar novo tipo de Configuração">
                    <i class="fa fa-plus-square-o"></i>
                </button>
            </div>
        </div>
        <br>
        <div class="row">
            <div class="col-lg-6 col-md-6 center">
                <?php echo $simec->input('copvalor', 'Valor', "", array('maxlength' => '255', $disabled, 'required','type'=>'number')); ?>
            </div>
        </div>
        <br>
    </form>
    <div class="row">
        <div class="col-lg-3 col-md-3 col-md-offset-4 col-lg-offset-4">
            <button class="btn btn-primary" id="pesquisar"><i class="fa fa-search"></i>Pesquisar
            </button>
            <button class="btn btn-success" id="btn-salvar"><i class="fa fa-plus"></i>Incluir
            </button>
        </div>
    </div>
</div>
<div class="ibox-content">
    <div class="row">
        <div class="col-md-6">
            <h3>Lista de Configurações PAR</h3>
        </div>
    </div>
    <?php
//    $_REQUEST['copstatus'] = 'A';
    $controllerConfiguracoesPar->listaConfiguracoesPar($_REQUEST);
    ?>
</div>

<?php require_once 'cadastroTipoConfiguracao_par.inc';//modal ?>

<script>
    jQuery(function () {
        jQuery('#pesquisar').click(function (evt) {
            window.location.assign(window.location.href + "&req=pesquisar&" + jQuery('#formulario-configuracoes-par').serialize());
        });

        jQuery('#btn-salvar').click(function (evt) {
            evt.preventDefault();
            $('#formulario-configuracoes-par').isValid(salvar);
        });

        function salvar(isValid) {
            if (isValid) {
                window.location.assign(window.location.href + "&req=salvarConf&" + jQuery('#formulario-configuracoes-par').serialize());
            }
            return false;
        }
    });

</script>
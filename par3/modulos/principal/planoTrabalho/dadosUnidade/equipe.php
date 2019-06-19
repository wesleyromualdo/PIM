<?php
/**
 * Tela de dados da prefeitura
 *
 * @category visao
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
$renderEntidade         = new Par3_Controller_Entidade();
$controllerEntidade     = new Par3_Controller_InstrumentoUnidadeEntidade();
$controllerEquipeLocal  = new Par3_Controller_EquipeLocal();
$modelEquipeLocal       = new Par3_Model_EquipeLocal();

$inuid = $_REQUEST['inuid'];
$tenid = Par3_Model_InstrumentoUnidadeEntidade::EQUIPE;

switch ($_REQUEST['req']) {
    case 'carregarCargos':
        ob_clean();
        $modelCargo = new Par3_Model_EquipeLocalCargo();
        echo $modelCargo->carregarJSONCombo($_REQUEST);
        die();
        break;
    case 'salvar':
        $controllerEquipeLocal->salvar($_REQUEST);
        break;
    case 'inativar':
        $controllerEquipeLocal->invativar($_REQUEST);
        break;
    default:
        $arrParamHistorico = array();
        $arrParamHistorico['inuid'] = $inuid;
        $arrParamHistorico['tenid'] = $tenid;
        $arrParamHistorico['booCPF'] = true;
        break;
}
?>

<input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid ?>"/>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Equipe Local - Integrantes</h3>
            </div>
            <div class="col-md-6">
                <?php if($disabled == ''){ ?>
                <button class="btn btn-success novo  pull-left" data-toggle="modal" data-target="#modal">
                    <i class="fa fa-plus-square-o"></i> Inserir Integrante
                </button>
                <?php }?>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <?php
            $_REQUEST['elostatus'] = 'A';
            $controllerEquipeLocal->listaEquipe($_REQUEST);
        ?>
    </div>
    <div class="ibox-footer">
        <div class="row">
            <div class="col-lg-6 text-left">
                <button type="button" class="btn btn-success previous" >Anterior</button>
            </div>
            <div class="col-lg-6 text-right">
                <button type="button" class="btn btn-success next" >Próximo</button>
            </div>
        </div>
    </div>
</div>
<?php require_once 'equipeIntegrante.php'; ?>
<div class="ibox collapsed">
    <div class="ibox-title">
        <h5>Equipe Local - Histórico Modificações</h5>
        <div class="ibox-tools">
            <a class="collapse-link">
                <i class="fa fa-chevron-down"></i>
            </a>
        </div>
    </div>
    <?php $controllerEntidade->formHistorico($arrParamHistorico); ?>
</div>

<script>
    function inativaIntegranteEquipe(id) {
        var inuid = $('#inuid').val();
        var url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid=' + inuid + '&menu=equipe';
        var action = '&req=inativar';
        window.location.href = url + action + '&eloid=' + id;
    }

    $(document).ready(function() {
        $('.collapse-link').click( function() {
            var ibox = $(this).closest('div.ibox');
            var button = $(this).find('i');
            var content = ibox.find('div.ibox-content');
        });

        $('.previous').click(function(){
            var url = window.location.href.toString().replace('equipe', 'dirigente');
            window.location.href = url;
        });

        $('.next').click(function(){
            var url = window.location.href.toString().replace('equipe', 'nutricionista');
            window.location.href = url;
        });
    });
</script>
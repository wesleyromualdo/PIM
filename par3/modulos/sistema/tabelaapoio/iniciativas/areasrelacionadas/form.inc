<?php
require_once APPRAIZ . "includes/funcoesspo_componentes.php";

$controllerUnidadeOrcamentaria  = new Par3_Controller_IniciativaUnidadeOrcamentaria();
$iar                            = new Par3_Model_IniciativaAreas($_REQUEST['iarid']);
$mPrograma                      = new Par3_Model_IniciativaProgramas();
$rsPrograma                     = $mPrograma->recuperarTodos('prgid as codigo, prgdsc as descricao',array("prgstatus = 'A'"),'2 ASC');
$controllerIniciativaProgramas  = new Par3_Controller_IniciativaAreasRelacionadasProgramas();
$mIar                           = new Par3_Model_IniciativaAreas();
$rsIar = array();
if($iar->iarid){
    $rsIar = $mIar->pegarSQLSelectComboSigla(array('iuoid'=> $iap->iuoid));
}
$disabled = $iar->iarid?"disabled='disabled'":'';
global $simec;
switch($_REQUEST['reqarearelacionada']){
    case 'salvar':
        ob_clean();
        $resp = $controllerIniciativaProgramas->salvar($_REQUEST);
        echo simec_json_encode($resp);die;
        break;
//    case 'vincularprogramas':
//        ob_clean();
//        $resp = $mAreaRelacionadaProgramas->salvar($_REQUEST);
//        echo simec_json_encode($resp);die;
//        break;
    case 'getarea':
        ob_clean();
        if ($_REQUEST['iuoid']) {
            echo simec_json_encode($mIar->lista(array('iarid', 'iarsigla'), array("iuoid = {$_REQUEST['iuoid']}")));
        }
        die;
        break;
    case 'recuperarprograma':
        ob_clean();
        $resp = $controllerIniciativaProgramas->recuperarPrograma($_REQUEST);
        echo simec_json_encode($resp);die;
        break;
    case 'desvincular':
        ob_clean();
        $resp = $controllerIniciativaProgramas->desvincular($_REQUEST['iapid']);
        echo simec_json_encode($resp);die;
        break;
//    case 'marcaropcao':
//        ob_clean();
//        $resp = $mAreaRelacionadaProgramas->marcarOpcao($_REQUEST['iapid'],$_REQUEST['opcao']);
//        echo simec_json_encode($resp);die;
//        die;
        break;
    default:
        break;
}
?>

<div class="ibox" id="painelItens">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <a id="cancelar-arearelacionada"><i class="fa fa-arrow-left"></i> voltar para a pesquisa</a>
                <h3 class="center">Area Relacionada - <?php echo $iar->iarid ? 'Alterar' : 'Incluir'; ?></h3>
            </div>
        </div>
    </div>

    <div class="ibox-content">
        <form name="form-item" id="form-arearelacionada" class="form-horizontal">
            <div class="row">
                <?php if($iar->iarid):?>
                <input type="hidden" name="iarid" value="<?php echo $iar->iarid; ?>"/>
                <input type="hidden" name="iuoid" value="<?php echo $iar->iuoid; ?>"/>
                <? endif; ?>
                <?php
                echo $simec->select('iuoid', 'Unidade Orçamentária', $iar->iuoid, $controllerUnidadeOrcamentaria->pegarSelectCombo(), array('maxlength' => '255', 'required',$disabled,'title' => 'Selecione uma Unidade Gestora'));//,array('class' => 'col-md-3')
                echo $simec->select('iarid', 'Área', $iar->iarid,$rsIar, array('maxlength' => '255', 'required',$disabled,'title' => 'Selecione uma Área'));//,array('class' => 'col-md-3')
    //            echo $simec->radio('irrsituacao', 'Situação', ($iap->irrsituacao ? $iap->irrsituacao : "A"), array('A' => 'Ativo', 'I' => 'Inativo'), array('', 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
                echo '<div class="form-group"><label class="col-lg-3 control-label">Programa:*</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
                echo inputCombo("prgid[]", $rsPrograma, '', 'prgid', array('multiple' => 'multiple', 'titulo' => 'Selecione'));
                echo '</div></div>';
                ?>
            </div>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" class="btn btn-success" id="btn-vincularprogramas"><i class="fa fa-link"></i> Vincular</button>
                    <button type="button" class="btn btn-primary" id="btn-vincularprogramas" style="width:110px;"><i class="fa fa-file-excel-o"></i> XLS</button>
                     | <button type="button" class="btn btn-warning" id="btn-vincularprogramas"><i class="fa fa-print"></i> Imprimir</button>
                    <button type="button" class="btn btn-primary" id="btn-vincularprogramas"><i class="fa fa-search"></i> Pesquisar</button>
                </div>
            </div>
        </form>
<?php if($iar->iarid):?>

    <div class="ibox-content" id="listaprogramasvinculados">
        <?php echo $controllerIniciativaProgramas->listarProgramas($_REQUEST); ?>
    </div>
<?php endif; ?>
</div>
<script type="text/javascript" src="js/sistema/tabelaapoio/iniciativas/areasrelacionadas.js"></script>
<script>
    $(document).on('click', '#cancelar-arearelacionada', function (evt) {
        evt.preventDefault();
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=areasrelacionadas');
    });
jQuery('#salvar-arearelacionada').click(function (evt) {
    evt.preventDefault();
    var iapid = $('#form-arearelacionada').find('[name=iapid]').val();
    var msg = 'Incluir';
    if (iapid) {
        msg = 'Alterar';
    }
    swal({
            title: "Confirmar",
            text: "Deseja <b>" + msg + "</b> esta Área Relacionada?",
            type: "success",
            html: true,
            showCancelButton:  true,
            cancelButtonText:  "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                salvarAreaRelacionada();
            }
            return false;
        }
    );
});

jQuery('#btn-vincularprogramas').click(function (evt) {
    evt.preventDefault();
    swal({
            title: "Confirmar",
            text: "Confirma a <b>Vinculação</b> de Programas a esta Área Relaciondada?",
            type: "success",
            html: true,
            showCancelButton:  true,
            cancelButtonText:  "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                vicularProgramas();
            }
            return false;
        }
    );
});

$('#btn-vincularprogramas').click(function(evt){
    evt.preventDefault();

});

$("#form-arearelacionada").on('change', "#iuoid", function () {
    var iuoid = $('#form-arearelacionada').find("#iuoid").val();
    var caminho = window.location.href;
    var options = $('#form-arearelacionada').find("#iarid");
    var action = '&reqarearelacionada=getarea&iuoid=' + iuoid;
    $.getJSON(caminho + action, function (dados) {
        options.empty();
        if (!dados) {
            options.append(new Option("", ""));
        } else {
            $.each(dados, function (index, iar) {
                options.append(new Option(iar.iarsigla, iar.iarid));
            });
        }
        options.focus();
        options.trigger('chosen:updated');
    });
}).change();
</script>
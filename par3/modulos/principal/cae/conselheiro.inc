<?php
/**
 * Cadastro de conselheiro
 *
 * @category Cadastro
 * @package  A1
 * @author   Fellipe Esteves <fellipesantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN:
 * @link     no link
 */
require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';

$renderNutricionista    = new Par3_Controller_Entidade();
$controleUnidade        = new Par3_Controller_InstrumentoUnidade();
$controllerCAE          = new Par3_Controller_CAE();
$modelCategoria   	= new Par3_Model_CAEConselheiroCategoria();
$modelCAE		= new Par3_Model_CAE();

$disabled = '';

switch ($_REQUEST['req']) {
    case 'salvar':
    	$controllerCAE->salvarConselheiroExterno($_POST);
        break;
	case 'listarSubCategorias':
    	ob_clean();
    	print $modelCategoria->carregarJSONComboSubCategoria($_POST);
        die;
        break;
    default:
    	$objCAE = $modelCAE->carregarConselheiroCPF($_SESSION['usucpf']);
        break;
}

echo $simec->title('Cadastro de Conselheiro CAE', '');
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <form method="post" name="formulario" id="formulario" class="form form-horizontal">
            <input type="hidden" name="req" value="salvar"/>
            <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Conselheiro</h3>
                </div>
                <div class="ibox-content">
                	<?php $controllerCAE->formConselheiroExterno($disabled, $objCAE, 'formularioConselheiro'); ?>
                </div>
                <div class="ibox-footer">
                    <div class="row col-md-offset-3">
						<button type="submit" class="btn btn-success salvar"><i class="fa fa-save"></i> Enviar informações</button>
					</div>
                </div>
            </div>
        </form>
    </div>
</div>
<style>
.help-block { float: right; width: 88%; }
</style>
<script>
$(document).ready(function() {
	$('#cctid').change(function(e) {
		e.preventDefault();
        var options = $("#cscid").trigger('chosen:update');
        $.ajax({
            url: 'par3.php?modulo=principal/cae/conselheiro&acao=A&req=listarSubCategorias',
            data: {'cctid' : $(this).val()},
            method: 'post',
            success: function (result) {
                if (result != 'false') {
                	options.closest('.form-group').removeClass('hidden');
	                options.empty();
	                var result = JSON.parse(result);
	                console.log(result);
	                $.each(result, function() {
	                    options.append(new Option(this.descricao, this.codigo));
	                });
	                options.trigger('chosen:updated');
                } else {
                	options.closest('.form-group').addClass('hidden');
                }
            }
        });
	});
	
    swal({
        title: "<span style='font-size: 27px'>Prezado(a) Conselheiro</span>",
        text: "O FNDE gostaria de conhecer melhor os conselheiros de alimentação escolar, " +
			"a fim de melhor apoia-los na realização das suas atividades. " +
			"Pedimos que preencham todas as informações antes de validar e encaminhar o formulário. " + 
			"Caso deseje alterar alguma informação, basta acessar novamente o " + 
			"Plano de Ações Articuladas no SIMEC e editar as informações pessoais.",
        html: true
    });
});
</script>
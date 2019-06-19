<?php echo $fm->getMensagens(); ?>

<div class="col-xs-12 col-sm-12 col-md-12 col-lg-12">
	<div class="form-group">
		<button class="btn btn-primary btn-novo" type="button" id="nova-iniciativa-emenda">
			<span class="glyphicon glyphicon-plus"></span>
			Vincular iniciativa
		</button>
	</div>
</div>

<div class="col-xs-12 col-sm-12 col-md-12 col-lg-12">
	<input type="hidden" name="emeid" id="emeid" value="<?= $_REQUEST['emeid']; ?>">
	<?php
    $dados['emeid'] = $_REQUEST['emeid'];
    require_once APPRAIZ . 'spoemendas/classes/model/Iniciativadetalheentidade.class.inc';
    $modeloIniciativa = new Spoemendas_Model_Iniciativadetalheentidade();
    $arrColunas = array('Iniciativa', 'GND', 'Resolução');
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);
    $listagem->addCallbackDeCampo(array('ininome'), 'alinhaParaEsquerda');
	$listagem->esconderColuna(array('emeid'));
    $listagem->setId('tabelainiciativa');
    $permissaoEdicao = new Spoemendas_Controller_Permissaoedicaoemenda($_REQUEST['emeid']);
    $podeSalvar = $permissaoEdicao->geraPermissoesEdicao();
	if ($podeSalvar) {
		$listagem->addAcao('delete', 'apagarIniciativa');
	} else {
		$listagem->esconderColuna(array('emeid', 'iniid'));
	}

    $listagem->setCabecalho($arrColunas)->setQuery($modeloIniciativa->getDetalhesIniciativa($dados['emeid']));
    $dadosIndicacoes = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    ?>
</div>

<script type="text/javascript">

$(function(){
	$('#nova-iniciativa-emenda').click(function () {
		$("#modal-nova-iniciativa-emenda").modal();
	});
});

var apagarIniciativa = function(id) {

	bootbox.confirm({
	    message: "Tem certeza de que deseja apagar o registro?",

	    callback: function (result) {

	        if (result && id && $("#emeid").val()) {

		    var urlBase = "spoemendas.php?modulo=principal/emendas/editaremenda/editaremenda",

			    ajaxConfig = {
			    	url:urlBase+"&acao=A&emeid="+$("#emeid").val()+"&aba=iniciativa",
			    	data:{requisicao:'apagar-emenda', iniid:id, emeid:$("#emeid").val()}
				},

				$promisse = $.ajax(ajaxConfig);
				$promisse.done(function(data) {
					location.reload();
				});
	    	}
		}
	});
};

function onCadastroiniciativaSubmit()
{
	if ("" == $('#cadastroiniciativa_iniid option:selected').text()) {
		bootbox.alert('O campo "Iniciativa" é obrigatório e não pode ser deixado em branco.');
		return false;
	}

	return true;
}
</script>

<?php
require_once APPRAIZ . 'spoemendas/classes/model/Iniciativaemendadetalhe.class.inc';
$model = new Spoemenda_Model_Iniciativaemendadetalhe();
$form = new Simec_View_Form('cadastroiniciativa');
$htmlForm = $form->setRequisicao('nova-iniciativa-emenda')
->addHidden('emdid', $db->pegaUm("SELECT emdid FROM emenda.emendadetalhe WHERE emeid = {$_GET['emeid']} and emdstatus = 'A'"))
->addCombo('Iniciativa', 'iniid', $model->getIniciativas($_REQUEST['emeid']), ['multiple' => true])
->render(false);

bootstrapPopup('Vincular iniciativas', 'modal-nova-iniciativa-emenda', $htmlForm, ['fechar', 'confirmar']);

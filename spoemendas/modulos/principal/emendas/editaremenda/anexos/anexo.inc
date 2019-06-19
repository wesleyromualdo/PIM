<?php
require(APPRAIZ . 'includes/cabecalho.inc');
include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/workflow_bootstrap.php';

$fm = new Simec_Helper_FlashMessage('spoemendas/principal/emendas/editaremenda');
$erro = new Simec_Helper_FlashMessage('spoemendas/principal/emendas/editaremenda/anexos');

// Verifica permissão para acesar tela
if(!verificaPerfil(array(PFL_CGO_EQUIPE_ORCAMENTARIA, PFL_SUPER_USUARIO, PFL_ASPAR))) {
	echo "<script>alert('Você não possui permissão para acessar esta página!')</script>";
	echo "<script>window.location.href='spoemendas.php?modulo=principal/emendas/editaremenda/editaremenda&acao=A&emeid={$_GET['emeid']}&aba=anexos';</script>";
	die(); 
}

switch ($_POST['requisicao']) {
	
	case 'anexar-arquivos':
		
		if($_GET['ofiid']) {
			$erro->addMensagem('Não é possível alterar este anexo!', Simec_Helper_FlashMessage::ERRO);
			break;
		}
		
		// fix para o file simec que apresenta erro com cpf com zero a esquerda.
		$_POST['anexo']['usucpf'] = "'".$_POST['anexo']['usucpf']."'";
		
		// Verifica se existe no minimo 1 tipo de solicitação selecionado.
		if(count($_POST['anexo']['tiposol']) < 1) {
			$erro->addMensagem('Selecione no mínimo um tipo de solicitação.', Simec_Helper_FlashMessage::ERRO);
			break;
		}
		
		$tiposol = $_POST['anexo']['tiposol'];
		
		// Remove campos
		unset($_POST['anexo']['autor']);
		unset($_POST['anexo']['tiposol']);
		
		// Cadastra documento workflow
		$_POST['anexo']['docid'] = wf_cadastrarDocumento(WF_TPDID_SPOEMENDAS_ANEXO_OFICIO, "Spoemendas - Fluxo do anexo de oficio", ESD_RECEBIDO);
		
		// Cadastra anexo na tabela emenda.oficio
		$file = new FilesSimec("oficio", $_POST['anexo'], "emenda");
		
		if ($file->setUpload($_FILES['anexo_oficio']['name'], '', true, 'ofiid')) {
			
			// Insere os tipos de solicitação
			foreach ($tiposol as $values) {
				(new Spoemendas_Model_TipoSolicitacao())
						->addOficioTipoSolicitacao($file->getCampoRetorno(), $values);
			}
			
			$fm->addMensagem('Anexo salvo com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
			
		} else {
		    $fm->addMensagem('Erro ao tentar anexar ofício.', Simec_Helper_FlashMessage::ERRO);
		}

		header("Location: spoemendas.php?modulo=principal/emendas/editaremenda/editaremenda&acao=A&emeid={$_POST['anexo']['emeid']}&aba=anexos");
		break;
		
}
?>

<div class="container-fluid">

    <ol class="breadcrumb">
        <li><a href="spoemendas.php?modulo=inicio&acao=C">SPO - Emendas Parlamentares</a></li>
        <li><a href="spoemendas.php?modulo=principal/emendas/listaremendas&acao=A">Lista de Emendas</a></li>
        <li>Anexo de ofício</li>
    </ol>

    <div class="col-xs-12 col-sm-12 col-md-12 col-lg-12">

		<div class="form-group">
		    <ul class="nav nav-tabs">
		      <li role="presentation" class="active"><a href="#">Anexo de Ofícios</a></li>
		    </ul>
		</div>

	</div>

	<div class="row">
		<div class="col-xs-12 col-sm-12 col-md-12 col-lg-<?php echo $_GET['ofiid'] ? '10' : '12' ?>">
			
			<?php echo $erro->getMensagens(); ?>
			
			<?php
				
				// Carrega dados
				$dados = (new Spoemendas_Model_Emenda())->autorEmenda($_REQUEST['emeid']);
				$dados['tiposol'] = (new Spoemendas_Model_TipoSolicitacao())->tipoSelected($_GET['ofiid']);
		
				$form = (new Simec_View_Form('anexo'))
						->carregarDados($dados)
						->setRequisicao('anexar-arquivos')
			            ->addHidden('emeid', $_REQUEST['emeid'])
			            ->addHidden('usucpf', $_SESSION['usucpf'])
			            ->addHidden('autid', (new Spoemendas_Model_Emenda())->autorEmenda($_REQUEST['emeid'])['autid'])
			            ->addTexto('Solicitante', 'autor', null, [
			            	'somentetexto' => true
						])
			            ->addChoices('Tipo de solicitação', 'tiposol', (new Spoemendas_Model_TipoSolicitacao())->listChoice(), [
			            	'type' => 'checkbox',
			            	'habil' => $_GET['ofiid'] ? 'N' : ''
						])
			            ->addFile("Enviar Ofício assinado*", 'oficio')
			            ->addBotao('salvar')
			            ->render();
			?>
			<span class="pull-right">(*) Campo obrigatório</span>
		</div>
		<?php if($_GET['ofiid']) :?>
			<div class="col-xs-12 col-sm-12 col-md-12 col-lg-2">
				<?php wf_desenhaBarraNavegacaoBootstrap((new Spoemendas_Model_TipoSolicitacao())->pegaDocid($_GET['ofiid']), array()); ?>
			</div>
		<?php endif; ?>
    </div>
</div>

<script>

	$(document).ready(function() {

		// insere o botão de "Baixar modelo de Ofício"
		$('#anexo .btn-group').parent().append(
          	'<button id="bmo" type="button" class="btn btn-primary pull-right">'+
          	'<i class="glyphicon glyphicon-download"></i>'+
          	' Baixar modelo de Ofício</button></div>'
      	);

		// recebe o clique do botão "Baixar modelo de Ofício" e baixa o arquivo
		$('#bmo').click(function(){
			window.location.href="modelos/"+$('input[name="anexo[tiposol]"]:checked').val()+".docx";
		});

		// valida o formulário
		$('.btn-salvar').click(function(event) {
			event.preventDefault();

			if($('#anexo_autid').selected().val() && $('#anexo_oficio').val()){
				$('#anexo').submit();
			}

			$('#anexo_oficio').parent().parent().addClass('has-error');
		});

		// Desabilita os campos caso seja anexo existente
		$("#anexo :input").prop("disabled", <?php echo $_GET['ofiid'] ? 'true' : 'false' ?>);
		
	});
</script>

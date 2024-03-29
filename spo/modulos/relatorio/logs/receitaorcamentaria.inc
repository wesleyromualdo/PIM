<?php
/**
 * Relatório de comunicação com o serviço de Receita Orçamentária da SOF.
 *
 * $Id$
 */

if ($_REQUEST['download']) {
	$file = md5(time()) . '.xml';
	header('Content-type: text/xml');
	header('Content-Disposition: attachment; filename="' . $file . '"');
	echo stripslashes($_REQUEST['xml']);
	die;
}

require_once APPRAIZ . 'www/spo/_funcoes.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
require_once APPRAIZ . 'includes/funcoesspo.php';
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';
require_once APPRAIZ . 'spo/ws/sof/Qualitativo.php';

$mapServiços = array
(
	'Consultar disponibilidade captação base externa' => array('ws' => 'consultarDisponibilidadeCaptacaoBaseExterna'),
	'Captar base externa' => array('ws' => 'captarBaseExterna'),
	'Consultar execução orçamentária' => array('ws' => 'consultarExecucaoOrcamentaria')
);

ksort($mapServiços);

$fm = new Simec_Helper_FlashMessage('spo/relatorio/logs/listar');
?>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script>
$(document).ready(function() {
	$('#codeModal').on('show.bs.modal', function (event) {
	  var button = $(event.relatedTarget);
	  var code = button.data('code');
	  var xml = button.data('xml');
	  var modal = $(this);
	  modal.find('.modal-body').find('.code').text(formatXml(code)).prettify();
	  modal.find('.modal-body').find('.xml').val(formatXml(xml));
	});
});
</script>
<div class="col-lg-12">
    <div class="page-header">
        <ol class="breadcrumb">
            <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li>SIOP (SOF)</li>
            <li class="active">Relatório de comunicação de receitas orçamentárias</li>
        </ol>
    </div>
    <?php echo $fm->getMensagens(); ?>
    <div class="well">
        <fieldset>
            <form id="frmConsulta" name="frmConsulta" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
				<div class="form-group">
                    <label for="servico" class="col-lg-2 control-label">
                       Serviço:
                    </label>
                    <div class="col-lg-6">
                    	<?php
	                    	$dados = array();

	                    	foreach ($mapServiços as $name => $info) {
	                    		$dados[] = array('codigo' => $info['ws'], 'descricao' => $name);
	                    	}

	                        $options = array(
	                            'titulo' => 'Selecione ao menos um serviço'
	                        );

	                        inputCombo('servico', $dados, $_REQUEST['servico'], 'servico', $options);
	                    ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-0">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="btnListar" type="submit"><i class="glyphicon glyphicon-search"></i> Exibir dados da tabela</button>
                    </div>
                </div>
            </form>

        </fieldset>
    </div>
    <?php
    	if ($_REQUEST['servico'])
    	{
	    	$sql = "SELECT lwsid as \"ID\",
	    			       lwsrequestheader as \"Requisição Header\",
	    			       lwsresponseheader as \"Resposta Header\",
	    			       lwsrequestcontent as \"Requisição XML\",
	    			       lwsresponsecontent as \"Resposta XML\",
	    			       to_char(lwsrequesttimestamp,'DD/MM/YYYY HH24:MI:SS') as \"Requisição Data\",
	    			       to_char(lwsresponsetimestamp,'DD/MM/YYYY HH24:MI:SS') as \"Resposta Data\"
					  FROM spo.logws
					  WHERE lwsmetodo = '{$_REQUEST['servico']}'
	    			  ORDER BY lwsrequesttimestamp DESC";

			$listagem = new Simec_Listagem();
	        $listagem->setQuery($sql);
	        $listagem->turnOnPesquisator();
	        $listagem->setCabecalho('auto');
	        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
	        $listagem->addCallbackDeCampo('Requisição Header', 'mascaraHeader');
	        $listagem->addCallbackDeCampo('Resposta Header', 'mascaraHeader');
	        $listagem->addCallbackDeCampo('Requisição XML', 'mascaraCode');
	        $listagem->addCallbackDeCampo('Resposta XML', 'mascaraCode');
	        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    	}
	?>
</div>
<div class="modal fade" id="codeModal" tabindex="-1" role="dialog" aria-hidden="true">
  <div class="modal-dialog" style="width: 70%">
    <div class="modal-content">
    	<form id="frmDownload" name="frmDownload" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
	      <div class="modal-header">
	        <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>
	        <h4 class="modal-title" id="label">XML</h4>
	      </div>
	      <div class="modal-body">
	     	<input type="hidden" name="download" value="true" />
    		<input type="hidden" name="xml" class="xml" />
	        <pre class="prettyprint linenums code"></pre>
	      </div>
	      <div class="modal-footer">
	        <button type="submit" class="btn btn-success">Download</button>
	      	<button type="button" class="btn btn-danger" data-dismiss="modal">Ok</button>
	      </div>
	    </form>
    </div>
  </div>
</div>
<?php 
$controllerAgendamentoemail = new Seguranca_Controller_Agendamentoemail();

switch ($_POST['requisicao']){
	case 'atualizarStatus':
		$retorno = $controllerAgendamentoemail->atualizarStatus($_POST);
		echo ($retorno ? 'true' : 'false');
		die();
}

include_once APPRAIZ."includes/cabecalho.inc";
?>
<style>
@media print{
    .notprint{
        display: none
    }
}
</style>
<div class="ibox">
	<div class="ibox-title">
		<div class="row">
			<div class="col-md-12">
				<h2 class="center">Listagem de agendamento de email</h2>
			</div>
		</div>
	</div>
	<?php echo $simec->tab ( $controllerAgendamentoemail->criarAbaAgendamentoEmail() ); ?>
	<div class="ibox-content notprint">
		<div class="row">
			<form name="formListaEnvioEmail" id="formListaEnvioEmail" class="form form-horizontal" method="post">
				<input type="hidden" name="requisicao" id="requisicao" value="salvar">
				<input type="hidden" name="aemid" id="aemid" value="<?php echo $dado['aemid']; ?>">
    			<?php 
    			echo $simec->input('aemtitulo', 'Título', $_POST['aemtitulo'], array('maxlength' => '255'), array('input-size' => '7'));
    			echo $simec->input('aemassunto', 'Assunto', $_POST['aemassunto'], array('maxlength' => '255'), array('input-size' => '7'));
    			echo $simec->checkbox('agsperiodicidade[]', 'Periodicidade', $_POST['agsperiodicidade'], ['mensal' => 'Mensal', 'semanal' => 'Semanal', 'diario' => 'Diária'], ['']);
    			echo $simec->radio('aemstatus', 'Ativo', $_POST['aemstatus'], ['' =>'Todos', 'A' => 'Ativo', 'I' => 'Inativo'], []);
    			?>
			</form>
		</div>
	</div>
	<div class="ibox-footer notprint">
		<div class="center">
			<button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-primary btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button>
			<button type="reset" class="btn btn-success" id="btn-limpar"><i class="fa fa-eraser"></i> Limpar</button>
            <button type="button" class="btn btn-warning" style="width:110px;" onclick="javascript:$('.btn-pesquisar').click(); $('#div_cabecalho').show(); window.print(); $('#div_cabecalho').hide(); ">
            	<i class="glyphicon glyphicon-print"></i> Imprimir </button>        
            </div>
	</div>
	<!-- Listagem  -->
    <div class="float-e-margins" id="listagem-obras">
    	<div class="ibox-content" id="div-listagem-obras" style="display: block;">
        	<div class="table-responsive" >
                <div id="testeSQL">
                    <div id="div_cabecalho" class="row col-lg-offset-2" style="display:none;">
                        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
                            <tr bgcolor="#ffffff">
                                <td valign="top" width="50" rowspan="2">
                                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                                </td>
                                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                                    MEC/FNDE <br>
                                    Listagem de agendamento de email <br>
                                </td>
                                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                                    Impresso por: <b> <?=$_SESSION['usunome'];?></b><br/>
                                    Hora da Impressão: <?=date( 'd/m/Y - H:i:s' );?><br />
                                </td>
                            </tr>
                        </table>
                        <hr style="color:black; width:95%; margin-left:-18px;">
                    </div>
                
                    <div class="clearfix" style="margin-bottom:10px;"></div>
				<?php
                $controllerAgendamentoemail->listar($_POST);
                ?>
            	</div>
			</div>
            <br>
            <br>
		</div>
	</div>
</div>
<div class="row">&nbsp;</div>
<script type="text/javascript">
jQuery(document).ready(function (){
	jQuery(".btn-pesquisar").click(function () {
		$btn = jQuery(this).button('loading');
	    jQuery('[name="formListaEnvioEmail"]').submit();
	});

	jQuery("#btn-limpar").click(function (){
		window.location = '<?php echo $urlPadrao; ?>';
	});
		
	jQuery('input[agendamento-email-id]').change(function (){
		var aemid 	= jQuery(this).attr('agendamento-email-id');
		var status 	= jQuery(this).is(':checked');
        var caminho = '<?php echo $urlPadrao;?>';
        var action 	= {requisicao:'atualizarStatus', status:status, aemid:aemid};
        $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    if(resp == 'true'){
                        swal("Sucesso", "Status alterado com sucesso", "success");
                    }else{
                        swal("Erro", "error");
                    }
                }
            });
	});
});

function editarAgendamentoEmail(aemid){
	location.href="<?php echo $urlPadrao ?>&menu=cadastroAgendamento&aemid=" + aemid;
}
</script>
<style>
.switchery {
	background-color: #ed5565;
}                            
</style>

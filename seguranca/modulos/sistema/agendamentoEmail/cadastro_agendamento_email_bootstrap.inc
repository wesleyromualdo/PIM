<?php 
$controllerAgendamentoemail = new Seguranca_Controller_Agendamentoemail();

if ($_GET['aemid'] && is_numeric($_GET['aemid'])){
	$dado = $controllerAgendamentoemail->pegarDadosEdicao($_GET['aemid']);
	if ( !$dado ){
		header("location:$urlPadrao");
		die;
	}
}

switch ($_POST['requisicao']) {
	case 'salvar':
		$controllerAgendamentoemail->salvar($_POST);
		die;
	case 'validarSql':
		$dado = $controllerAgendamentoemail->validarSql($_POST);
		echo json_encode($dado);
		die;
	case 'visualizarConteudo':
		$dado = $controllerAgendamentoemail->preVisualizacaoEmail($_POST);
		echo ($dado);
		die;
}

require APPRAIZ . 'includes/cabecalho.inc';
?>
<style>
.ibox{
	padding-bottom: 50px;
}
</style>

<div class="ibox">
	<div class="ibox-title">
		<div class="row">
			<div class="col-md-12">
				<h2 class="center">Cadastro de agendamento de email</h2>
			</div>
		</div>
	</div>
	<?php echo $simec->tab ( $controllerAgendamentoemail->criarAbaAgendamentoEmail() ); ?>	
	<div class="ibox-content">
		<div class="row">
			<form name="formEnvioEmail" id="formEnvioEmail" class="form form-horizontal" method="post">
				<input type="hidden" name="requisicao" id="requisicao" value="salvar">
				<input type="hidden" name="aemid" id="aemid" value="<?php echo $dado['aemid']; ?>">
    			<?php 
    			echo $simec->input('aemtitulo', 'Título', $dado['aemtitulo'], array('maxlength' => '255', 'required'), array('input-size' => '8'));
    			echo $simec->radio('agsperiodicidade', 'Periodicidade', $dado['agsperiodicidade'], [/*'imediatamente' => 'Imediatamente', 'A' => 'Na hora do agendamento',*/ 'mensal' => 'Mensal', 'semanal' => 'Semanal', 'diario' => 'Diária'], ['required']);
    			
    			for ($i=1; $i<=31; $i++){
	    			$arOpcao[$i] = $i;
    			}
    			echo $simec->checkbox('diames[]', 'Dia(s) do mês', $dado['diames'], $arOpcao, [0=>'required', 'class'=>'diames']);
    			
    			unset($arOpcao);
    			$arOpcao = ['0'=>'domingo', '1'=>'segunda', '2'=>'terça', '3'=>'quarta', '4'=>'quinta', '5'=>'sexta', '6'=>'sábado'];
    			echo $simec->checkbox('diasemana[]', 'Dia(s) da semana', $dado['diasemana'], $arOpcao, ['required', 'class'=>'diasemana']);
    			
    			unset($arOpcao);
    			for ($i=0; $i<=23; $i++){
					$indice = str_pad($i, 2, "0", STR_PAD_LEFT);
	    			$arOpcao[$indice] = $i.'h';
    			}
    			echo $simec->checkbox('horadia[]', 'Hora(s) do dia', $dado['horadia'], $arOpcao, [0=>'required', 'class'=>'horadia']);
				
    			echo $simec->textarea('aemsql', 'Sql com destinatários', $dado['aemsql'], array('rows'=>10, 'cols'=>'100%', 'required', 'onchange' => 'validarSql(this.value);'), array('input-size' => '8'));
    			?>
    			<div class="form-group" id="div_substituicao_sql" style="display:none;">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Tag de substituição do SQL: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                    <div class="col-sm-8 col-md-8 col-lg-8 " id="tag_substituicao_sql">
                        
                    </div>
                    <div style="clear:both"></div>
                </div>
    			<div class="form-group ">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Tag de substituição padrão: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                    <div class="col-sm-8 col-md-8 col-lg-8 ">
						<span>{data}</span>
						<span>{datahora}</span>
						<span>{hora}</span>
						<span>{horaminuto}</span>
						<span>{assinatura}</span>
                    </div>
                    <div style="clear:both"></div>
                </div>
    			<?php
    			echo $simec->input('aemassunto', 'Assunto do email', $dado['aemassunto'], array('maxlength' => '255', 'required'), array('input-size' => '8'));
    			echo $simec->textarea('aemconteudo', 'Conteúdo do email', $dado['aemconteudo'], array('class'=>'summernote', 'rows'=>30, 'cols'=>'100%', 'required'), array('input-size' => '8'));
    			?>
			</form>
		</div>
	</div>
	<div class="ibox-footer">
		<div class="form-actions col-md-offset-3">
			<button type="button" id="btn_salvar" class="btn btn-success">
				<i class="fa fa-save"></i> Salvar
			</button>
			<button type="button" id="btn_view_conteudo" class="btn btn-primary" onclick="abrirModalVisualizarConteudo();">
				<i class="glyphicon glyphicon-eye-open"></i> Visualizar
			</button>
			<button type="button" class="btn btn-secondary" onclick="location.href='<?php echo $urlPadrao; ?>';">
				<i class="fa fa-arrow-left"></i> Voltar
			</button>
		</div>
	</div>
</div>
<div class="ibox float-e-margins animated modal fade" id="modal_view_conteudo" tabdesex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
		<div class="modal-content">
        	<div class="ibox-title">
            	<button type="button" class="close" data-dismiss="modal" aria-label="Close">
            		<span aria-hidden="true">&times;</span>
            	</button>
                <h3 id="modal_titulo">Visualização de conteúdo de email</h3>
			</div>
			<div class="ibox-content">
	        	<div id="conteudo-modal" style="height: 500px;overflow-y:scroll;"></div>
			</div>
		</div>
	</div>
</div>

<script type="text/javascript">
jQuery(document).ready(function (){
	jQuery('[name*=agsperiodicidade]').change(function (){
		switch (jQuery('[name*=agsperiodicidade]:checked').val()){
			case 'mensal':
				jQuery(".diames").parents("div.form-group").slideDown();
				jQuery(".diasemana").parents("div.form-group").slideUp();
				jQuery(".horadia").parents("div.form-group").slideUp();
				break;
			case 'semanal':	
				jQuery(".diames").parents("div.form-group").slideUp();
				jQuery(".diasemana").parents("div.form-group").slideDown();
				jQuery(".horadia").parents("div.form-group").slideUp();
				break;
			case 'diario':	
				jQuery(".diames").parents("div.form-group").slideUp();
				jQuery(".diasemana").parents("div.form-group").slideUp();
				jQuery(".horadia").parents("div.form-group").slideDown();
				break;
			default:
				jQuery(".diames").parents("div.form-group").slideUp();
				jQuery(".diasemana").parents("div.form-group").slideUp();
				jQuery(".horadia").parents("div.form-group").slideUp();
		}
	});

	jQuery('[name*=agsperiodicidade]:first').change();

	if ( jQuery('#aemsql').val().trim() != '' ){
		jQuery('#aemsql').change();
	} 
	
	jQuery('#btn_salvar').click(function (){

		if (jQuery('#tag_substituicao_sql').find('.alert-danger').length > 0){
			swal("SQL", 'Digite um sql válido, antes de salvar.', "error");
			return false;
		}
		
		var msg = '';
		
		if (jQuery('[name=aemtitulo]').val().trim() == '' ){
			msg += "Título\n";
		}
		
		if (jQuery('[name*=agsperiodicidade]:checked').length == 0){
			msg += "Periodicidade\n";
		}
		
		if (jQuery('[name*=diames]:checked').length == 0 && jQuery('[name*=agsperiodicidade]:checked').val() == 'mensal'){
			msg += "Dia(s) do mês\n";
		}
		
		if (jQuery('[name*=diasemana]:checked').length == 0 && jQuery('[name*=agsperiodicidade]:checked').val() == 'semanal'){
			msg += "Dia(s) da semana\n";
		}
		
		if (jQuery('[name*=horadia]:checked').length == 0 && jQuery('[name*=agsperiodicidade]:checked').val() == 'diario'){
			msg += "Dia(s) da semana\n";
		}
		
		if (jQuery('[name=aemsql]').val().trim() == '' ){
			msg += "Sql com destinatários\n";
		}
		
		if (jQuery('[name=aemassunto]').val().trim() == '' ){
			msg += "Assunto do email\n";
		}

		var aemconteudo = jQuery('[name=aemconteudo]').code().trim();
		aemconteudo = (aemconteudo == '<p><br></p>' ? true : false); 
		if (aemconteudo ){
			msg += "Conteúdo do email\n";
		}
		
		if ( msg ){
			msg = "Os campos abaixo são de preenchimento obrigatório:\n\n" + msg;
			swal("Formulário", msg, "error");
			return false;
		}
		
		jQuery('#formEnvioEmail').submit();
	});
	
	jQuery('.diames, .diasemana, .horadia').hide();
});

function validarSql(sql){
	if (sql){
	    var caminho = '<?php echo $urlComMenu;?>';
	    var action 	= {requisicao:'validarSql', sql:sql};
	    $.ajax({
	            type: "POST",
	            url: caminho,
	            data: action,
	            async: false,
	            success: function (dado) {
					jQuery('#div_substituicao_sql').slideDown();
	                try{
	                	dado = jQuery.parseJSON(dado);
						dado = "{" + dado.join("} {") + "}";
						if (dado.indexOf('{email}') == -1){
		                	jQuery('#tag_substituicao_sql').html('<div class="alert alert-danger" role="alert">' +
		                			  							 '	<span class="glyphicon glyphicon-exclamation-sign" aria-hidden="true"></span>' +
		                			  							 '	<span class="sr-only">Error:</span>' +
		                			  							 '		O SQL deve apresentar a coluna <b>email</b> a qual será usada como destinatário do email.' +
		                										 '</div>');
						}else{
							jQuery('#tag_substituicao_sql').html(dado);
						}
	                }catch(e){
	                	jQuery('#tag_substituicao_sql').html('<div class="alert alert-danger" role="alert">' +
	                			  							 '	<span class="glyphicon glyphicon-exclamation-sign" aria-hidden="true"></span>' +
	                			  							 '	<span class="sr-only">Error:</span>' +
	                			  							 '		O SQL apresentou erro na execução.' +
	                										 '</div>');
	                	swal("SQL", dado.substr(0, 200) + '...', "error");
					}
	            }
	        });
	}else{
		jQuery('#tag_substituicao_sql').html('');
		jQuery('#div_substituicao_sql').slideUp();
	}
}

function abrirModalVisualizarConteudo(){
	if (jQuery('#tag_substituicao_sql').find('.alert-danger').length > 0 || jQuery('[name=aemsql]').val().trim() == ''){
		swal("SQL", 'Digite um sql válido, antes de pré-visualizar o email.', "error");
		return false;
	}
	
	var aemconteudo = jQuery('[name=aemconteudo]').code().trim();
	aemconteudo = (aemconteudo == '<p><br></p>' ? true : false); 
	if (aemconteudo ){
		swal("Erro", 'Preencha o campo: Conteúdo do email. Antes de pré-visualizar o email.', "error");
		return false;
	}
		
	jQuery("#modal_view_conteudo").modal("show");
}

jQuery('#modal_view_conteudo').on('show.bs.modal', function (e) {
    var caminho = '<?php echo $urlComMenu;?>';
    var action 	= {requisicao:'visualizarConteudo', sql:jQuery('[name=aemsql]').val().trim(), conteudo:jQuery('[name=aemconteudo]').code().trim()};

    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (dado) {
			jQuery('#conteudo-modal').html( dado );
        }
    });
});

</script>
<?php
$objOrientacao  = new Par3_Model_Orientacoesprograma();
$objExcecao     = new Par3_Model_Excecaoescolasprograma();

$perfis = pegaPerfilGeral($_SESSION['usucpf']);
$prgid = $_REQUEST['prgid'];

$modelProadesao = new Par3_Model_Proadesao();
$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($prgid, $_SESSION['par3']['adpano_ciclo']);

$bloqueiaSave = false;

if((strtotime($_SESSION['par3']['mais_escolas']['vigencia'][$_SESSION['par3']['adpano_ciclo']]. ' 23:59:00') < strtotime(date('Y-m-d G:i:s'))))
{
	$bloqueiaSave = true;
	$disabledCarga = 'disabled ';
}
	
?>
<script>
function negaExcecao(eepid)
{
	$('#justifica_negacao_'+eepid).show();
	$('#botao_aceita_'+eepid).hide();
}

function confirmaExcecao(eepid)
{

	if( $('#justifica_negacao_textarea_'+eepid).val() == '')
	{
		alert('Campo Justificativa obrigatório');
		return false;
	}
	if (confirm('Confirma a não aceitação da solicitação Nº'+ eepid+'?'))
	{

		var param = new Array();

        param.push(
    	        {name: 'requisicao', value: 'aceita_solicitacao_excecao'},
    	        {name: 'decisao', value: 'N'},
    	        {name: 'justificativa', value: $('#justifica_negacao_textarea_'+eepid).val() },
    	        {name: 'eepid', value: eepid }
		);

        $.ajax({
            type: "POST",
            dataType: "json",
            url: window.location.href,
            data: param,
            success: function (data) {
	            
            	if(data[0].status ==  "success")
            	{
            		$('#botao_aceita_'+eepid).hide();
        			$('#botao_nega_'+eepid).hide();
        			$('#justifica_negacao_'+eepid).hide();
        			$('#sucesso_negado_'+eepid).show();
        			$('#just_'+eepid).html('<b>Justificativa:</b>' + $('#justifica_negacao_textarea_'+eepid).val()) 
            	}
            	else
            	{
					alert('Erro ao validar solicitação');
            	}
            },
       		 error: function (data) {
	        	
        	}
        });
        
	
	}
}
function cancelaExcecao(eepid)
{
	$('#justifica_negacao_'+eepid).show();
	$('#botao_aceita_'+eepid).show();
	$('#justifica_negacao_'+eepid).hide();
}

function aceitaExcecao(eepid)
{
	if(confirm('Confirma que está ciente da solicitação Nº'+ eepid+'?'))
	{
		var param = new Array();

        param.push(
    	        {name: 'requisicao', value: 'aceita_solicitacao_excecao'},
    	        {name: 'decisao', value: 'S'},
    	        {name: 'justificativa', value: '' },
    	        {name: 'eepid', value: eepid }
		);
		
		$.ajax({
	            type: "POST",
	            dataType: "json",
	            url: window.location.href,
	            data: param,
	            success: function (data) {
		            
	            	if(data[0].status ==  "success")
	            	{
	            		$('#botao_aceita_'+eepid).hide();
	            		$('#botao_nega_'+eepid).hide();
	        			$('#sucesso_aceita_'+eepid).show();
	            	}
	            	else
	            	{
						alert('Erro ao validar solicitação');
	            	}
	            },
	       		 error: function (data) {
		        	
	        	}
	        });
	        
		
	}
}

function validaEnvio()
{
	var submete = true;
	jQuery('form#formulario_escolas_excecao :input').each(function() 
	{
		if(jQuery(this).attr('type') == 'text')
		{
			if( jQuery(this).attr('id') == "cod_inep" )
			{
				if( jQuery(this).val() == "" )
				{
					alert('O campo "Código INEP" é obrigatório');
					submete = false;
				}
			}
			if( jQuery(this).attr('id') == "justificativa" )
			{
				if( jQuery(this).val() == "" )
				{
					alert('O campo "Justificativa" é obrigatório');
					submete = false;
				}
			}
		}
		else if (jQuery(this).attr('type') == 'hidden')
		{

			if( jQuery(this).attr('id') == "nome_escola" )
			{
				if( jQuery(this).val() == "" )
				{
					alert('Para inserir as escolas é necessário que todos os códigos INEP informados retornem informações válidas para esta unidade');
					submete = false;
				}
			}
		}
		else
		{
			if( (jQuery(this).attr('id') == "aepcargahoraria") || (jQuery(this).attr('id') == "aeacargahoraria") )
			{
				if(jQuery(this).prop('disabled') == false)
				{
					if( jQuery(this).val() == "" )
					{
						alert('Informe a "Carga Horária" para a escola ' + $(this).parent().parent().find("#cod_inep").val());
						
						submete = false;
					}
				}
			}
		}

		
	}); 
	 	  	
	if(submete == true)
	{
		document.formulario_escolas_excecao.submit();
	}
	
	    
				    
}


function removeLinha(input)
{
		if($('#tabelaEscolas tr').length > 3)
		{
			var tr = jQuery(input).closest('tr');
			tr.remove();
			return false;
		}
		else
		{
			return false;
		}
}
function retornaInfoEscola(input)
{
	if($(input).val().length == 8){
		var inuid 		= $("#inuid").val();
		var adpid 		= $("#adpid").val();
		var cod_inep 	= $(input).val()

		var param = new Array();

        param.push(
    	        {name: 'requisicao', value: 'retorna_dados_escola'},
    	        {name: 'inuid', value: inuid },
    	        {name: 'adpid', value: adpid },
    	        {name: 'cod_inep', value: cod_inep }
		);

        $.ajax({
            type: "POST",
            dataType: "json",
            url: window.location.href,
            data: param,
            success: function (data) {
            	if(data[0].status ==  "success")
            	{
	            	if(data[0].grupo != 0)
		            {
						$(input).parent().parent().find("input[tag=nome_escola_atual]").val(data[0].nome);
						$(input).parent().parent().find("input[tag=grupo]").val(data[0].grupo);
						$(input).parent().parent().find("p[tag=label_grupo]").html(data[0].grupo);
						$(input).parent().parent().find("p[tag=label_escola]").html(data[0].nome)

						if(data[0].resposta_carga_horaria == 'S')
						{
							$(input).parent().parent().find("select[tag=carga_horaria_atual]").removeAttr('disabled');
							$(input).parent().parent().find("input[tag=arrPossuiCargaHoraria]").val('s');
							
						}
						else
						{
							$(input).parent().parent().find("select[tag=carga_horaria_atual]").attr('disabled','disabled');
							$(input).parent().parent().find("input[tag=arrPossuiCargaHoraria]").val('n');
						}
		            }
	            	else
	            	{
	            		alert('Escola não apta');
	            	}
					
					
					
            	}
            	else
            	{
            		$(input).val("");
            		$(input).parent().parent().find("input[tag=nome_escola_atual]").val("");
					$(input).parent().parent().find("p[tag=label_escola]").html("")
					$(input).parent().parent().find("select[tag=carga_horaria_atual]").attr('disabled','disabled');
					$(input).parent().parent().find("select[tag=carga_horaria_atual]").val('');
					
					alert('Código INEP não encontrado');
            	}
            },
       		 error: function (data) {
	        	
        	}
        });
		
	}
	else
	{
		$(input).val("");
		$(input).parent().parent().find("input[tag=nome_escola_atual]").val("");
		$(input).parent().parent().find("p[tag=label_escola]").html("")
		$(input).parent().parent().find("select[tag=carga_horaria_atual]").attr('disabled','disabled');
		$(input).parent().parent().find("select[tag=carga_horaria_atual]").val('');
		alert('O código INEP deve ser composto por 8 dígitos');
	}	
}

$(document).ready(function(){

	$('.add').click(function(){
		
		var $campos = $('#tbodyTabela');
		console.log($campos);
		
				$tr = $campos.find('tr:first').clone();
				
				$tr.find("input").val("");
				$tr.find("select").attr('disabled','disabled');;
				$tr.find("select").val("");
				$tr.find("hidden").val("");
				$tr.find("label").html("");
				$tr.find("p").html("");
			$campos.append($tr);
		return false;
	});
	
 	$('.btn-xls').hide();
	$('.btn-query').hide();

	if( $("#tipoPesquisa2").val() == 'lista')
	{
		window.location.hash = '#resultado';
	}

	$('.ordenavel').click(function(){
		var campo = $(this).attr('data-campo-ordenacao');
    	$('#ordenacao').val(campo);
    	$('#filtrar_pesquisa').click()
    	
    });
	   
	
});
</script>
<div class="ibox-content" style="padding:40px">
	<div class="panel-body">
<!--  
	<div class="tabs-container">
    	<ul class="nav nav-tabs">
        	<li id="li1" class="active"><a data-toggle="tab" href="#pop-1" aria-expanded="true">Orientações</a></li>
            <li id="li2" class=""><a data-toggle="tab" href="#pop-2" aria-expanded="false">Escolas</a>
            <?php 
            if(	 
                in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis) ||
                in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis)
            ){
            ?>
            <li id="li3" class=""><a data-toggle="tab" href="#pop-3" aria-expanded="false">Validar Solicitações</a>
            <li id="li4" class=""><a data-toggle="tab" href="#pop-4" aria-expanded="false">Solicitações Validadas</a>
            <?php 
            }
            ?>
		</ul>
        <div class="tab-content">
        	<div id="pop-1" class="tab-pane active">
            	<div class="panel-body">
--> 
                <?php 
                if ($prgid == PRGID_PROEMI){	
            		$objOrientacao->getOrientacaoByID(ORIENTACOES_PROEMI_EXCESAO_ESCOLAS, "Escolas");
            	}else if($prgid == PRGID_ABCMAIS){
            		$objOrientacao->getOrientacaoByID(ORIENTACOES_ABCMAIS_EXCESAO_ESCOLAS, "Escolas");
            	}
            	?>
				</div>
<!--  
			</div>
            <div id="pop-2" class="tab-pane">
				<div class="panel-body">
-->				
                <?php 
                if(!$bloqueiaSave){
                ?>	

            	<form method="post" name="formulario_escolas_excecao" id="formulario_escolas_excecao" class="form form-horizontal">
            		<input type="hidden" value="salvar_escolas_excecao" id="salvar_escolas_excecao" name="requisicao" />
            		<input type="hidden" value="<?php echo  $_REQUEST['inuid']; ?>" id="inuid">
            		<input type="hidden" value="<?php echo  $_REQUEST['adpid']; ?>" id="adpid" name="adpid" >
            		
            		<table id="tabelaEscolas"  cellspacing="2" cellpadding="2" border="0" align="center" width="95%" >
            			<thead>
            				<tr align="center" style=" background: #ccc; color: #000;">
            					<td colspan="6" style="font-weight: bold; font-size: 15px;">Escolas</td>
            				</tr>
            				<tr align="center">
            					<td width="10%">&nbsp;</td>
            					<td width="15%">Código INEP</td>
            					<td width="40%">Nome da Escola</td>
            					<td width="15%">Grupo Escola</td>
            					<td width="20%">Carga Horária</td>
            				</tr>
            			</thead>
            			<tbody id="tbodyTabela">
            				
            				<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245); height: 40px;" id="linha_1" align="center">
            					<td>
            							 <h3 onclick="removeLinha(this);"  id="1" alt="Excluir" title="Excluir" class="glyphicon glyphicon-remove danger" aria-hidden="true"> Excluir</h3> 
            						
            					</td>
            					<td>
            						<input name="cod_inep[]" id="cod_inep" value="" class="form-control" maxlength="8" required="required" type="text" onchange="retornaInfoEscola(this)">
            					</td>
            					<td>
            						<p id="label_escola" tag="label_escola" >
            							
            						</p>
            							<input type="hidden" name="nome_escola[]" id="nome_escola" tag="nome_escola_atual">
            							<input type="hidden" name="grupo_escola[]" id="grupo_escola" tag="grupo">
            					</td>
            					<td>
            						<p id="label_grupo" tag="label_grupo" >
            							
            						</p>
            					</td>
            					<td>
            					    
            						<?php 				
            							// PRGID_ABCMAIS 5 E 15
            							if( $prgid == PRGID_ABCMAIS)
            							{
            								
            								?>
            								
            								<select name="aeacargahoraria[]" id="aeacargahoraria"  tag="carga_horaria_atual" data-placeholder="Selecione" disabled=disabled>
            								    <option value="" label=""></option>
            								    <option value="5" label="5">5</option>
            								    <option value="15" label="15">15</option>
            								</select>
            								
            								<input type="hidden" name="arrPossuiCargaHoraria[]" tag="arrPossuiCargaHoraria" >
            								
            								<?php 
            							}
            							// PRGID_PROEMI 5 e 7e
            							if( $prgid == PRGID_PROEMI)
            							{
            								?>
            															
            								<select name="aepcargahoraria[]" id="aepcargahoraria"  tag="carga_horaria_atual"  data-placeholder="Selecione" disabled>
            								    <option value="" label=""></option>
            								    <option value="5" label="5">5</option>
            								    <option value="7" label="7">7</option>
            								</select>
            								
            								<input type="hidden" name="arrPossuiCargaHoraria[]" tag="arrPossuiCargaHoraria" >
            								<?php 
            							}
            					 		?>
            					</td>
            				</tr>
            				
            			</tbody>
            		</table>
            		<span class="add" style="margin-left:5px; cursor:pointer; padding-left: 2%; ">
            			<h3 class="glyphicon glyphicon-plus success" aria-hidden="true">
            			Inserir Escola
            			</h3>
            		</span>
            		<table id="tabelaJustificativa"  cellspacing="2" cellpadding="2" border="0" align="center" width="95%" height="50px;" >
            			<thead>
            				<tr align="center" style=" background: #ccc; color: #000;">
            					<td colspan="6" style="font-weight: bold; font-size: 15px;">Justificativa</td>
            				</tr>
            			</thead>
            			<tbody id="tbodyTabela">
            				<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245); height: 40px;" id="linha_1" align="center">
            					<td width="5%" style="height: 100px;">&nbsp;</td>
            					<td width="95%">				
            	<? 
            					echo	$simec->textarea('justificativa', "Justificativa",  $textoOrientacao, array('cols' => 96, 'rols' => 5));
            	?>
            					</td>
            				</tr>
            				<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245); height: 40px;" id="linha_1" align="center">
            					<td width="100%" colspan="2" style="height: 100px;">
            						<input type="button" onclick="validaEnvio()" class="btn btn-success salvar" value="Enviar">
            					</td>
            					
            				</tr>
            				</tbody>
            				
            		</table>
            		</form>
            <?php 
            }
            ?>
<!--  
            	</div>
			</div>
            <div id="pop-3" class="tab-pane">
				<div class="panel-body">
-->            
            		<?php
//             		$_REQUEST['validado'] = 'IS NULL';
            		$arrRegistros = $objExcecao->retornaListaSolicitacoes($_REQUEST);
            		
            		if(count($arrRegistros) > 0 )
            		{
            			$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            			$cabecalho = array("ID da solicitação", "CPF e Nome do Solicitante", "Data", "justificativa", "Validação", "Escolas Selecionadas");
            			$listagem->setCabecalho($cabecalho);
            			$listagem->setDados($arrRegistros);
            			$listagem->turnOffForm();
            			$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS, "*");
            			
            			$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
            		}
            		else
            		{?>
            			<div style="" class="text-center " id="tb_render"><b>Nenhum registro encontrado</b></div>
            		<?php 
            		}
            	   ?>
<!--  
				</div>
			</div>
            <div id="pop-4" class="tab-pane">
				<div class="panel-body">
            		<?php
            		$_REQUEST['validado'] = 'IS NOT NULL';
            		$arrRegistros = $objExcecao->retornaListaSolicitacoes($_REQUEST);
            		
            		if(count($arrRegistros) > 0 )
            		{
            			$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            			$cabecalho = array("ID da solicitação", "CPF e Nome do Solicitante", "Data", "justificativa", "Validação", "Escolas Selecionadas");
            			$listagem->setCabecalho($cabecalho);
            			$listagem->setDados($arrRegistros);
            			$listagem->turnOffForm();
            			$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS, "*");
            			
            			$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
            		}
            		else
            		{?>
            			<div style="" class="text-center " id="tb_render"><b>Nenhum registro encontrado</b></div>
            		<?php 
            		}
            	   ?>
				</div>
			</div>
		</div>
	</div>
-->        
	</div>    	   
</div>
<?php die(); ?>
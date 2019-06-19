<?php

switch ( $_REQUEST['ajax'] ){
	case 'carregaEmpresaAndListaObra':
		$param 					 = array();
		$param['obrigatorio'] 	 = 'S';
		carregaEmpresaAndListaObra( $param );
		die;
}

if($_POST['requisicao'] == 'salvarTecnico'){
	$tecnico = new Tecnico_Empresa();
	$tecnico->salvarTecnico();
}
if($_GET['temid']){
	$tecnico = new Tecnico_Empresa($_GET['temid']);
	extract($tecnico->recuperaDados());	
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc"; 


monta_titulo_obras( ($_GET['temid'] ? "Editar" : "Cadastrar")." Técnico da Empresa", '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );

//Se o perfil não for super usuário, apenas as empresas ligadas ao usuário pela responsabilidade podem ser visualizadas.
//Se houver apenas uma empresa, não é necessário visualizar a combo para escolher a empresa.
if( !possui_perfil(array(PFLCOD_SUPER_USUARIO)) ){
	$usuarioResp = new UsuarioResponsabilidade();
	$arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida( $_SESSION['usucpf'] );
	if ( count($arEntidEmpresa) ){
		$grupoEmpresa  = new Supervisao_Grupo_Empresa();
		$param = array('sgrid' => $sgrid,
					   'entid' => $arEntidEmpresa);
		foreach($grupoEmpresa->pegaSgeid( $param ) as $grupo){
			$arrGrupo[] = $grupo;
		}
	}
	$arrEmpresa = $usuarioResp->pegaSgeidEmpresaPermitida( $_SESSION['usucpf'] );
}
if($db->testa_superuser()){
	$perfil_super_user = true;
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<style>
	.hidden{display:none}
	.link{cursor:pointer}
</style>
<form id="formulario" name="formulario" method="post" action="" >
<input type="hidden" name="requisicao" id="requisicao" value="" />
<input type="hidden" name="temid" id="temid" value="<?php echo $temid ?>" />
<div class="row form-filters">
	<?php
        $countArrGrupo = (is_array($arrGrupo) ? count($arrGrupo) : 0);
        if($perfil_super_user || $countArrGrupo > 1):  ?>
		<div class="row">
			<div class="col-md-4 text-right">
				Grupo
			</div>
			<div class="col-md-4">
				<?php
	                $grupo = new Supervisao_Grupo();
	                $dados = $grupo->listaCombo();
	                
	            	$db->monta_combo("sgrid", $dados, $somenteLeitura, "Selecione...", "carregaDependenciaGrupo", '', '', '', 'S', 'sgrid');
	            ?>
			</div>
		</div>
	<?php else: ?>
		<div class="row hidden">
			<div class="col-md-4 text-right">
				<input type="hidden" name="sgrid" id="sgrid" value="<?php echo $arrGrupo[0] ?>" />
			</div>
		</div>
	<?php endif; ?>
	<?php if($perfil_super_user || count($arrEmpresa) > 1):  ?>
		<div class="row">
	    <div class="col-md-4 text-right text-right"> Empresa
	    </div>
	    <div class="col-md-4 " id="tdComboEmpresa">
				<?php
                $pkCount = (is_array($arrGrupo) ? count($arrGrupo) : 0);
				if($pkCount == 1){
					$sgrid = $sgrid ? $sgrid : $arrGrupo[0];
				}else{
					$sgrid = $sgrid ? $sgrid : null;
				}
				if ( !empty($sgrid) ){
	                $grupoEmpresa = new Supervisao_Grupo_Empresa();
	                $dados = $grupoEmpresa->listaCombo( array('sgrid' => $sgrid) );
	            	$db->monta_combo("sgeid", $dados, $somenteLeitura, "Selecione...", "", '', '', '98%', 'S', 'sgeid');
				}else{
					echo "Selecione o grupo.";	
				}
	            ?>
			</div>
		</div>
	<?php else: ?>
		<div class="row hidden">
			<div class="col-md-4 text-right">
				<input type="hidden" name="sgeid" id="sgeid" value="<?php echo $arrEmpresa[0] ?>" />
			</div>
		</div>
	<?php endif; ?>
		<div class="row">
			<div class="col-md-4 text-right">
				CPF
			</div>

			<div class="col-md-8">
				<?php echo campo_texto('temcpf', 'S', "S", '', 20, 14, '[###.###.###-##]', '', '', '', 0, 'id="temcpf"', ''); ?>
			</div>
		</div>

		<div class="row">
			<div class="col-md-4 text-right">
				Nome
			</div>
			<div class="col-md-8">
			<?php echo campo_texto('temnome', 'S', "S", '', 60, 255, '', '', '', '', 0, 'id="temnome"', ''); ?>
			</div>
		</div>	
		<div class="row">
			<div class="col-md-4 text-right">Cargo
			</div>
			<div class="col-md-8">
				<?php echo campo_texto('temcargo', 'S', "S", '', 60, 255, '', '', '', '', 0, 'id="temcargo"', ''); ?>
			</div>
		</div>
		<div class="row">
			<div class="col-md-4 text-right">Nº CREA/CAU 
			</div>
			<div class="col-md-8">
				<?php echo campo_texto('temnumcreacau', 'S', "S", '', 40, 255, '', '', '', '', 0, 'id="temnumcreacau"', ''); ?>
			</div>
		</div>
		<div class="row">
			<div class="col-md-4 text-right">
				Data de Capacitação
			</div>
			<div class="col-md-8">
			<?php echo campo_data2( 'temdtcapacitacao', 'N', 'S', '', 'S','','' ); ?>
			</div>
		</div>
		<div class="row">
			<div class="col-md-4 text-right">
				Status
			</div>
			<div class="col-md-8">
				<div class="btn-group" data-toggle="buttons">
					<label class="btn btn-default active">
						<input type="radio" name="codigo" value="A" checked="checked" />
						Ativo
					</label>

					<label class="btn btn-default">
						<input type="radio" name="codigo" value="I" />
						Inativo
					</label>

				</div> 
			</div>
	</div>
</div>

<div class="form-filters">
	<div class="col-md-12 text-center"> 
		<button type="button" value="" id="btn_salvar" class="btn btn-info" onclick="salvarTecnico();"><span class="glyphicon glyphicon-floppy-disk"></span> Salvar
    </button>

    <d  class="btn btn-primary"  onclick="location.href='?modulo=principal/tecnicoEmpresa&acao=A';">
            	<span class="glyphicon glyphicon-arrow-left"></span> Voltar
    </d>

	</div>

 </div>
</form>
<script type="text/javascript">
function carregaDependenciaEmpresa( sgeid ){
	
}

function carregaDependenciaGrupo( sgrid ){
	if ( sgrid ){
		var orgid = $('#orgid').val();
		$.post("?modulo=principal/cadTecnicoEmpresa&acao=A", {"sgrid":sgrid, "ajax":"carregaEmpresaAndListaObra", "orgid":orgid, "not(listaObras)":true}, function(data){
			var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
			$('#tdComboEmpresa').html( comboEmpresa );
			
//			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
//			$('#divListaObra').html( listaObra );
			
		});
	}else{
		$('#tdComboEmpresa').html('Selecione o grupo.');
	}
}

function salvarTecnico()
{
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value || this.value == "Selecione..."){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
	if($("#temcpf").val() && !validar_cpf($("#temcpf").val()))
	{
		erro = 1;
		alert('CPF inválido!');
		return false;
	}
	if(erro == 0){
		$('#requisicao').val( 'salvarTecnico' );
    	$('#formulario').submit();
	}
}
</script>
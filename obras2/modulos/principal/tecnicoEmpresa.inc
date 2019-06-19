<?php
switch ( $_REQUEST['ajax'] ){
	case 'carregaEmpresaAndListaObra':
		$param 					 = array();
		$param['obrigatorio'] 	 = 'N';
		carregaEmpresaAndListaObra( $param );
		die;
}

if($_POST['requisicao'] == "excluirTecnico"){
	$tecnico = new Tecnico_Empresa($_POST['temid']);
	$tecnico->excluirTecnico($_POST['temid']);
	$tecnico->commit();

	die('<script type="text/javascript">
			alert(\'Operação realizada com sucesso!\');
			location.href=\'?modulo=principal/tecnicoEmpresa&acao=A\';
		 </script>');
}

if($_POST['requisicao'] == "ativacaoTecnico"){
	$tecnico = new Tecnico_Empresa($_POST['temid']);
	$tecnico->ativacaoTecnico( $_POST['temid'], $_POST['ativacao'] );
	$tecnico->commit();

	die('<script type="text/javascript">
			alert(\'Operação realizada com sucesso!\');
			location.href=\'?modulo=principal/tecnicoEmpresa&acao=A\';
		 </script>');
}

if($_GET['requisicao'] == "exibirHistoricoTecnico"){
	monta_titulo( 'Histórico de Alterações', '' );
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script>
		function exibirDadosTecnico(hteid)
		{
			$.ajax({
			   type: "POST",
			   url: "obras2.php?modulo=principal/tecnicoEmpresa&acao=A",
			   data: "requisicaoAjax=exibirHistoricoTecnico&classe=Historico_Tecnico_Empresa&hteid="+hteid,
			   success: function(msg){
			   		$('#div_dados_tecnico').html( msg );
			   }
			 });
		}
	</script>
	<?php
	$historico_tecnico = new Historico_Tecnico_Empresa();
	$historico_tecnico->listaHistoricoTecnico($_GET['temid']);
	echo "<div id=\"div_dados_tecnico\" ></div>";
	echo "<center><input type='button' name='tbn_fechar' value='Fechar' onclick='window.close()' /></center>";
	exit;
}
if($_REQUEST['requisicaoAjax']){
	$n = new $_REQUEST['classe']();
	$n->$_REQUEST['requisicaoAjax']();
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc"; 

monta_titulo_Obras( 'Técnicos da Empresa', '' );

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
<form id="formulario_pesquisa" name="formulario_pesquisa" method="post" action="" >
<input type="hidden" name="requisicao" id="requisicao" value="" />
<input type="hidden" name="ativacao" id="ativacao" value="" />
<input type="hidden" name="temid" id="temid" value="" />
<input type="hidden" name="sgrid" id="sgrid" value="12" />
<div class="row form-filters">
	<div class="col-md-6">

	<?php if($perfil_super_user || count($arrGrupo) > 1):  ?>
 
	<?php else: ?>
		<div class="row">
			<div class="col-md-12">
				<input type="hidden" name="sgrid" id="sgrid" value="<?php echo $arrGrupo[0] ?>" />
			</div>
		</div>
	<?php endif; ?>



		<?php if($perfil_super_user || count($arrEmpresa) > 1):  ?>
			<div class="row">
				<div class="col-md-4">Empresa
				</div>

				<div class="col-md-8" id="tdComboEmpresa">
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
		            	$db->monta_combo("sgeid", $dados, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'sgeid');
					}else{
	                    $supervisao_Grupo_Empresa = new Supervisao_Grupo_Empresa();
	                    $empresas = $supervisao_Grupo_Empresa->listaComboEntidades(); 
	                    $db->monta_combo("sgeid", $empresas, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'sgeid');
					}
		            ?>
				</div>
			</div>

		<?php else: ?>
			<div class="row">
				<div class="col-md-4">
					<input type="hidden" name="sgeid" id="sgeid" value="<?php echo $arrEmpresa[0] ?>" />
				</div>
			</div>
		<?php endif; ?>

		
		<div class="row">
				<div class="col-md-4">Nº CREA/CAU
				</div>
				<div class="col-md-8">
				<?php echo campo_texto('temnumcreacau', 'N', "S", '', 40, 255, '', '', '', '', 0, 'id="temnumcreacau"', ''); ?>
				</div>
		</div>
		<div class="row">
				<div class="col-md-4">Data de Capacitação
				</div>
				<div class="col-md-8">
						<?php $temdtcapacitacao = formata_data_sql($_POST['temdtcapacitacao']) ?>
						<?php echo campo_data2( 'temdtcapacitacao', 'N', 'S', '', 'S','','' ); ?>
				</div>
		</div>
		<div class="row">
				<div class="col-md-4"> Status
				</div>
				<div class="col-md-8">
					<div class="btn-group" data-toggle="buttons">
							<label class="btn btn-default <?= ($temvigente == 'A'? 'active' : '') ?>" for="temvigente_ativo">
								<input type="radio" name="temvigente" id="temvigente_ativo" value="A" <?php echo ($temvigente == 'A'? "checked='checked'" : '') ?>>
								Ativo
							</label>
						<label class="btn btn-default <?= ($temvigente == 'I'? 'active' : '') ?>"  for="temvigente_inativo">
							<input type="radio" name="temvigente" id="temvigente_inativo" value="I" <?php echo ($temvigente == 'I' ? "checked='checked'" : '') ?>>
							Inativo
						</label> 

						<label class="btn btn-default <?= (empty($temvigente)? 'active' : '') ?>"  for="temvigente_inativo">
							<input type="radio" name="temvigente" id="temvigente_inativo" value="" <?php echo ($temvigente == 'I' ? "checked='checked'" : '') ?>>
							Todos
						</label> 

					</div>
				</div>
		</div>
</div>	

<div class="col-md-6">
		<div class="row">
				<div class="col-md-4">CPF
				</div>
				<div class="col-md-8">
					<?php echo campo_texto('temcpf', 'N', "S", '', 20, 14, '[###.###.###-##]', '', '', '', 0, 'id="temcpf"', ''); ?>
				</div>
		</div>
		<div class="row">
				<div class="col-md-4">Nome
				</div>
				<div class="col-md-8">
					<?php echo campo_texto('temnome', 'N', "S", '', 60, 255, '', '', '', '', 0, 'id="temnome"', ''); ?>
				</div>
		</div>
		<div class="row">
				<div class="col-md-4">
					Cargo
				</div>
				<div class="col-md-8">
					<?php echo campo_texto('temcargo', 'N', "S", '', 60, 255, '', '', '', '', 0, 'id="temcargo"', ''); ?>
				</div>
		</div>
	
</div>	
</div> 

		<div  class="row form-filters">
			<div class="row">
					<div class="col-md-12 text-center">
		          
		          <button class="btn btn-info btn-novo" type="button" value="Limpar"    name="btn_novo" >
		          <span class="glyphicon glyphicon-plus"></span> Novo Técnico </button> 


		          <button class="btn btn-success" type="button" name="pesquisar" class="pesquisar" id="pesquisar" value="Pesquisar" onclick="validaFiltro();" >
		          	<span class="glyphicon glyphicon-search"></span> Pesquisar </button>

		          <button class="btn btn-primary" type="button" value="Limpar"    name="btn_limpar"    onclick="limparTecnico()"> 
		          	<span class="glyphicon glyphicon-stop"></span> Limpar </button>


			    </div>
			</div>
		</div>

</form>
<script>
$1_11(document).ready(function () {
    $1_11('select[name="sgeid"]').chosen();
		$1_11('select[name="sgeid_disable"]').chosen();


		$1_11('.btn-novo').click(function(){
			location.href='?modulo=principal/cadTecnicoEmpresa&acao=A'
		})

});
</script>
<?php

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {

    $arrFiltros = array(
        $sgrid = $_POST['sgrid_disable'],
        $sgeid =  isset($_POST['sgeid_disable']) ? $_POST['sgeid_disable'] : $_POST['sgeid'],
        $temcpf = $_POST['temcpf'],
        $temnome = $_POST['temnome'],
        $temcargo = $_POST['temcargo'],
        $temnumcreacau = $_POST['temnumcreacau'],
        $temdtcapacitacao = $_POST['temdtcapacitacao'],
        $temvigente_ativo = $_POST['temvigente_ativo'],
        $temvigente_inativo = $_POST['temvigente_inativo']
    );

    $ocorrenciasFiltro = 0;
    $filtrosQuatidade = count($arrFiltros);

    for ($i = 0; $i < $filtrosQuatidade; $i++) {

        if ($arrFiltros[$i] != '') {
            $ocorrenciasFiltro++;
        }
    }

    if ($ocorrenciasFiltro > 0) {
        $_POST['sgrid'] = $_POST['sgrid_disable'] ? $_POST['sgrid_disable'] : $_POST['sgrid'];

        if(!empty( $_POST['sgeid_disable'])){
            $_POST['entid'] =  $_POST['sgeid_disable'];
            $_POST['sgeid_disable'] = null;
        }
        if(!empty( $_POST['sgeid'])){
            $_POST['sgeid'] =  $_POST['sgeid_disable'];
            $_POST['sgeid_disable'] = null;
        }

        $tecnico = new Tecnico_Empresa();
        extract($_POST);
        $arrWhere = $_POST;

        $sql = $tecnico->listaSql($arrWhere);

        $arrCabecalho = array("Ação", "Grupo", "Empresa", "CPF", "Nome", "Cargo", "Nº CREA/CAU", "Data de Cadastro", "Data de Capacitação", "Status");

     

        $db->monta_lista($sql, $arrCabecalho, 100, 10, "N", "center", "N", "formulario", '', '', $cache, array('classTable' => '', 'table-responsive' => true));

    } else {
        echo '<script>alert("Para realizar a pesquisa deve ser preenchido ao menos um filtro");</script>';
    }

}
?>
<script type="text/javascript">

    function validaFiltro() {

        var sgrid = document.getElementById('sgrid').value;
        var sgeid = document.getElementById('sgeid').value;
        var temcpf = document.getElementById('temcpf').value;
        var temnome = document.getElementById('temnome').value;
        var temcargo = document.getElementById('temcargo').value;
        var temnumcreacau = document.getElementById('temnumcreacau').value;
        var temdtcapacitacao = document.getElementById('temdtcapacitacao').value;
        var temvigente_ativo = document.getElementById('temvigente_ativo').checked;
        var temvigente_inativo = document.getElementById('temvigente_inativo').checked;

        var filtros =
            [sgrid,
                sgeid,
                temcpf,
                temnome,
                temcargo,
                temnumcreacau,
                temdtcapacitacao,
                temvigente_ativo,
                temvigente_inativo];

        var filtrosQuatidade = filtros.length;

        var ocorrenciasFiltro = 0;
        for (i = 0; i < filtrosQuatidade; i++) {

            if (filtros[i] != '' || filtros[i] != false) {
                ocorrenciasFiltro++;
            }
        }

        if (ocorrenciasFiltro > 0) {
            document.getElementById('requisicao').value = 'pesquisar';
            document.formulario_pesquisa.submit();
        } else {
            alert('Para realizar a pesquisa deve ser preenchido ao menos um filtro.');
            return false;
        }
    }

$(function() {
	<?php if($_SESSION['obras2']['tecnico_empresa']['alert']): ?>
		alert('<?php echo $_SESSION['obras2']['tecnico_empresa']['alert'] ?>');
		<?php unset($_SESSION['obras2']['tecnico_empresa']['alert']) ?>
	<?php endif; ?>
});

function editarTecnico( temid ){
	location.href = 'obras2.php?modulo=principal/cadTecnicoEmpresa&acao=A&temid=' + temid;
}

function excluirTecnico( temid ){
    if(confirm("Deseja realmente excluir definitivamente este Técnico?")){
    	$('#temid').val( temid );
    	$('#requisicao').val( 'excluirTecnico' );
    	$('#formulario_pesquisa').submit();
    }
}

function inativarTecnico( temid, ativacao ){
	var txtMsg = (ativacao == 'A' ? 'ativar' : 'desativar');
    if(confirm("Deseja realmente " + txtMsg + " este Técnico?")){
    	$('#temid').val( temid );
    	$('#requisicao').val( 'ativacaoTecnico' );
    	$('#ativacao').val( ativacao );
    	$('#formulario_pesquisa').submit();
    }
}

function carregaDependenciaEmpresa( sgeid ){

}

function carregaDependenciaGrupo( sgrid ){
	if ( sgrid ){
		var orgid = $('#orgid').val();
		$.post("?modulo=principal/tecnicoEmpresa&acao=A", {"sgrid":sgrid, "ajax":"carregaEmpresaAndListaObra", "orgid":orgid, "not(listaObras)":true}, function(data){
			var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
			$('#tdComboEmpresa').empty().html( comboEmpresa );

//			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
//			$('#divListaObra').html( listaObra );

		});
	}else{
		$('#tdComboEmpresa').html('Selecione o grupo.');
	}
}

function limparTecnico()
{
	window.location.href=window.location;
}

function historicoTecnico(temid)
{
	window.open('?modulo=principal/tecnicoEmpresa&acao=A&requisicao=exibirHistoricoTecnico&temid='+temid,
			'TecnicoEmpresa',
			"height=600,width=800,scrollbars=yes,top=50,left=200" ).focus();
}
</script>

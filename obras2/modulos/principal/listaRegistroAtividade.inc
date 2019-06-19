<?php
verificaSessao( 'obra' );
require APPRAIZ . 'obras2/includes/principal/listaRegistroAtividade/ctrl.php';

?> 
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" name="excluirRegistro" id="excluirRegistro" value="excluir" />
		<table id="idRegistroAtividade" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
			<tr>
				<td style="font-weight:bold; color:black; font-family:Arial,Verdana; background-color:#E0EEEE; text-align:left; " colspan="2">
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Argumentos de Pesquisa
				</td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Data da Criação:</td>
				<td>
					<?php 
					echo "Início " . campo_data2('rgadtinclusaoinicio', 'N', 'S', '', 'N');
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
					echo "Fim " . campo_data2('rgadtinclusaofim', 'N', 'S', '', 'N' );												
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Usuário:</td>
				<td>
					<?php 
					echo campo_texto( 'usunome', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, '');
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Descrição simplificada:</td>
				<td>
					<?php 
					echo campo_texto( 'rgadscsimplificada', 'N', 'S', '', 47 , 60, '', '', 'left', '', 0, '');	
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 190px;">Descrição detalhada:</td>
				<td>
					<?php 
					echo campo_texto( 'rgadsccompleta', 'N', 'S', '' , 47 , 60, '', '', 'left', '', 0, '');	
					?>
				</td>
			</tr>
			<tr class="divTituloPadrao">
				<td style="width: 190px;"></td>
				<td>
					<input type="button" name="visualizarRegAtividades" value="Visualizar" onclick="this.disabled=true; document.getElementById('formulario').submit();" style="cursor: pointer;"/>
					<input type="button" name="verTodosRegAtividades" value="Ver Todas" onclick="this.disabled=true; window.location='?modulo=principal/listaRegistroAtividade&acao=O'" style="cursor: pointer;"/>
				</td>
			</tr>
	</table>
</form>
<?php
$param = $_POST;
if ( $_GET['acao'] == 'A' ){
	$param['empid'] = $_SESSION['obras2']['empid'];
}else{
	$param['obrid'] = $_SESSION['obras2']['obrid'];
}
$param['habilitado'] = $habilitado;

$registro  = new RegistroAtividade();
$sql 	   = $registro->listaSql( $param );

$cabecalho = array( "Ação", "Seq.", "Descrição", "Tipo", "Data", "Arquivo", "Inserido por" );
$db->monta_lista($sql,$cabecalho,100,5,'N','center','');

if ( $habilitado ):
?>
<table class="tabela" bgcolor="#FFFFFF" cellspacing="1" cellpadding=3 align="center">
	<tr>
		<td style="font-weight: bold;">
			<a style="cursor: pointer;" onclick="javascript:popupRegAtividades();" title="Clique para inserir um novo Registro de Atividades">
			<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/>Inserir novo</a>
		</td>
	</tr>
</table>
<?php
endif;
?>
<script type="text/javascript">
    <?php
	    $objObras = new Obras();
	    $objObras->carregarPorIdCache($_SESSION['obras2']['obrid']);
	    $blockEdicao = $objObras->verificaObraVinculada();

	    if($blockEdicao){
	        echo 'var blockEdicao = true;';
	    }else{
	        echo 'var blockEdicao = false;';
	    }
    ?>
    var acao = '<?=$_GET['acao'] ?>';
</script>

<?php 
    require APPRAIZ . 'obras2/includes/principal/listaRegistroAtividade/js.php';
?>

<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );

switch ( $_POST['op'] ){
	case 'salvar':
		
		$iobid = $_REQUEST['iobid'];
		$estuf = $_REQUEST['estuf'];
		$muncod = $_REQUEST['muncod'];
		
		$sql = "INSERT INTO obras2.declaracaoconformidadefuncobra
		(usucpf, iobid, estuf, muncod  )
		VALUES
		('{$_SESSION['usucpf']}', $iobid, '$estuf', '$muncod'  );"
		;
		
		$db->executar($sql);
		$db->commit();					   
		echo "<script> alert(Declaração de Conformidade Salva com sucesso); </script>";
}

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=utf-8');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;		
}

function buscaRegistroDeclaracao($iobid){
	
	global $db;
	
	$registro = $db->pegaLinha("select dcfid, est.estdescricao, est.estuf, mun.muncod, mun.mundescricao, usu.usunome, formata_cpf_cnpj(usu.usucpf) as usucpf,
			TO_CHAR(dcfdataassinatura , 'DD/MM/YYYY às HH24:MI:SS') as data_assinatura
	FROM  
		obras2.declaracaoconformidadefuncobra  dcf
	INNER JOIN territorios.municipio mun ON mun.muncod = dcf.muncod
	INNER JOIN territorios.estado est ON est.estuf = dcf.estuf
	INNER JOIN seguranca.usuario usu ON usu.usucpf = dcf.usucpf
	WHERE iobid = {$iobid} AND dcfstatus= 'A'");
	return (is_array($registro)) ?  $registro : array();
	
}
function buscaLocal($obrid){
	
	global $db;

	$result = $db->pegaLinha("select ende.estuf, ende.muncod, mun.mundescricao from obras2.obras obr
	inner join entidade.endereco ende ON ende.endid = obr.endid
	left join territorios.municipio mun ON mun.muncod = ende.muncod
	WHERE obrid = $obrid");
	
	return $result;
}

$iobid = $_REQUEST['iobid'];
$obrid = $_REQUEST['obrid'];


if( (!$iobid) && (!$obrid)  )
{
	exit("
        <script type=\"text/javascript\">
    		alert('Faltam Parâmetros Obrigatórios');
    		window.close();
        </script>
    ");
}


$registro = buscaRegistroDeclaracao($iobid);

if(count($registro) > 0 ){
	$dcfid 			= $registro['dcfid'];
	$estdescricao 	= $registro['estdescricao'];
	$dscestuf 		= $registro['estuf'];
	$municipioDsc	= $registro['mundescricao'];
	$usunome		= $registro['usunome'];
	$usucpfDsc		= $registro['usucpf'];	
	$dataAssinatura	= $registro['data_assinatura'];	
}
else
{
	$dcfid = false;
}


$dadosLocal = buscaLocal($obrid);

$DescMun 	= $dadosLocal['mundescricao'];
$muncod 	= $dadosLocal['muncod'];
$estuf 		= $dadosLocal['estuf'];

//$dadoUser = $declaracaoConformidade->dadosUsuarioCadastrante( $dlcid );

if( $dcfid ){
    $desabilita = "disabled = 'disabled'";
    $hab        = 'N';
    $disable	= true;
}else{
    $hab        = 'S';
    $desabilita = '';
    
}
?>
<html>
<head>
	<title>Declaração de Conformidade</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<script type="text/javascript">
	function carregarMunicipio( estuf ){
		var td	= $('#td_municipio');
		if ( estuf != '' ){
			var url = location.href;
			$.ajax({
				  url  		 : url,
				  type 		 : 'post',
				  data 		 : {ajax  : 'municipio', 
				  		  	    estuf : estuf},
				  dataType   : "html",
				  async		 : false,
				  beforeSend : function (){
				  	divCarregando();
					td.find('select option:first').attr('selected', true);
				  },
				  error 	 : function (){
				  	divCarregado();
				  },
				  success	 : function ( data ){
				  	td.html( data );
				  	divCarregado();
				  }
			});	
		}else{
			td.find('select option:first').attr('selected', true);
			td.find('select').attr('selected', true)
							 .attr('disabled', true);
		}			
	}
	
	function validaForm(){
		var msg = '';
		
		if ($('#estuf').val() == ''){
			msg += 'UF \n';
		}	
		
		if ($('#muncod').val() == ''){
			msg += 'Município \n';
		}	
		
		if (msg){
			msg = 'Os seguintes campos devem ser preenchidos:\n\n' + msg;
			alert(msg);
			return false;
		}
		
		$('#form_declaracao').submit();
	}
	</script>
</head>
<body topmargin="0" leftmargin="0">
<?php 
monta_titulo( "Declaração de Conformidade", "" );
?>
<form method="post" action="" name="form_declaracao" id="form_declaracao" enctype="multipart/form-data">
<input type="hidden" name="op" id="op" value="salvar">
<input type="hidden" name="iobid" id="iobid" value="<?php echo $iobid;?>">
<input type="hidden" name="obrid" id="obrid" value="<?php echo $obrid;?>">
<table class="Tabela" align="center">
	<tr>
		<td class="SubTituloDireita">UF:</td>
		<td>
			<?php 
					
			$uf = new Estado();
			$db->monta_combo("estuf", $uf->listaCombo(), $hab,'Selecione...','carregarMunicipio', '', '',200,'S','estuf');
			?>			
			
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Município:</td>
		<td id="td_municipio">
		<?php 
		
		if ($estuf){
			$municipio = new Municipio( $muncod );
			$dado 	   = $municipio->listaCombo( array('estuf' => $estuf) );	
			$habMun    = 'S';
		}else{
			$dado   = array();
			$habMun = 'N';
		}		
		$habMun = ($disable == 'N' ? $disable : $habMun);
		echo $db->monta_combo("muncod", $dado, $habMun,'Selecione...','', '', '',200,'S','muncod'); 
		?>
		</td>
	</tr>
  	<tr style="" id="trButton" bgcolor="#C0C0C0">
  		<td colspan="2" align="center">
  			<input type="button" <?php echo $desabilita;?> value="Salvar" onclick="validaForm();">
  		</td>
  	</tr>
<?php 
	if ( $dcfid ):
?>  	
	<tr>
	    <td class="SubTituloDireita" colspan="2">
	    &nbsp;
		</td>
	</tr>
	<tr>
	    <td colspan="2">
		DECLARAÇÃO
		<br>
		<br>
		Declaro que o Estado / Município de <?php echo $estuf ?>/<?php echo $municipioDsc ?>  cumpriu todas as determinações para aquisição do mobiliário conforme preconizado em termo de compromisso. 
		<br>
		<br>
		Assinatura eletrônica	    
		<br>
		<?php
		echo $usucpfDsc . ' - ' . $usunome	. '. - ' . $dataAssinatura   
		?>
		</td>
	</tr>
<?php
	endif;
?>	
</table>
</form>
</body>
</html>


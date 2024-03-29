<?php 
//Teste XLS
if($_POST['limpaSession']){
	unset($_SESSION['emenda']['post']);
	$_POST['limpaSession'] = "";
}

if( empty($_SESSION['emenda']['post']) ){
	$_SESSION['emenda']['post'] = $_POST;
}

montaBotaoForm();

echo '<script>
		$(\'loader-container\').hide();
	  </script>';

function montaBotaoForm(){
	echo '	<html>
			<head>
				<style>
				
				#loader-container,
				#LOADER-CONTAINER{
				    background: transparent;
				    position: absolute;
				    width: 100%;
				    text-align: center;
				    z-index: 8000;
				    height: 100%;
				}
				
				
				#loader {
				    background-color: #fff;
				    color: #000033;
				    width: 300px;
				    border: 2px solid #cccccc;
				    font-size: 12px;
				    padding: 25px;
				    font-weight: bold;
				    margin: 150px auto;
				}
				</style>
				</head>
			<script type="text/javascript" src="/includes/prototype.js"></script>
			<body>
			<style type="">
				@media print {.notprint { display: none } .div_rolagem{display: none} }	
				@media screen {.notscreen { display: none; }
				
				.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
				
			</style>
			<div id="loader-container" style="display: none">
		   		<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
			</div>
			<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
			<table  align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td style="height: 20px;"></td>
				</tr>
				<tr>
					<td style="text-align: center;" class="div_rolagem">
						<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
						<input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
					</td>
				</tr>
			</table>
			</form>
			</body>
			<script type="text/javascript">				
				function exportarExcel(){
					params = "&exporta=1";
					document.getElementById(\'formulario\').action 	 = "emenda.php?modulo=relatorio/popUpRelatorioConvenio&acao=A" + params;
					document.getElementById(\'formulario\').submit();
				}
				$(\'loader-container\').show();
			</script>
			</html>';
}



//Cabeçalho
$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
				"<td colspan=\"100\">" .			
					monta_cabecalho_relatorio('100') .
				"</td>" .
			  "</tr>
			  </table>";

echo $cabecalhoBrasao;

//Rececendo os valores dos filtros
extract($_SESSION['emenda']['post']);
$where = array();
$arJoin = array();
//N° PTA
if( $n_pta[0] && ( $n_pta_campo_flag || $n_pta_campo_flag == '1' )){
	$where[] = "(pt.ptrid || '/' || pt.ptrexercicio) " . (( $n_pta_campo_excludente == null || $n_pta_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $n_pta ) . "') ";		
}
//Valor do Convênio
if( $convenio[0] && ($convenio_campo_flag || $convenio_campo_flag == '1' )){
	$where[] = "ef.exfvalor " . (( $convenio_campo_excludente == null || $convenio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $convenio ) . "') ";		
}
//Valor Pago
if( $pago[0] && ( $pago_campo_flag || $pago_campo_flag == '1' )){
	$where[] = "ob.orbvalorpagamento " . (( $pago_campo_excludente == null || $pago_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $pago ) . "') ";		
}
//Nº da Nota de Empenho
if( $empenho[0] && ( $empenho_campo_flag || $empenho_campo_flag == '1' )){
	$where[] = "ef.exfnumempenhooriginal " . (( $empenho_campo_excludente == null || $empenho_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $empenho ) . "') ";		
}
//Nº do Convênio no Siafi
if( $siafi[0] && ( $siafi_campo_flag || $siafi_campo_flag == '1' )){
	$where[] = "pc.pmcnumconveniosiafi " . (( $siafi_campo_excludente == null || $siafi_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $siafi ) . "') ";		
}
//Nº do Convênio no FNDE
if( $fnde[0] && ( $fnde_campo_flag || $fnde_campo_flag == '1' )){
	$where[] = "(pt.ptrnumconvenio || '/' || pt.ptranoconvenio) " . (( $fnde_campo_excludente == null || $fnde_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $fnde ) . "') ";		
}
//Nº do Processo
if( $processo[0] && ( $processo_campo_flag || $processo_campo_flag == '1' )){
	$where[] = "pt.ptrnumprocessoempenho " . (( $processo_campo_excludente == null || $processo_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $processo ) . "') ";		
}
//Unidade
if( $uniid[0] && ( $uniid_campo_flag || $uniid_campo_flag == '1' )){
	$where[] = "an.uniid " . (( $uniid_campo_excludente == null || $uniid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $uniid ) . "') ";
	$arJoin[] = "LEFT JOIN emenda.analise as an on an.ptrid=pt.ptrid and an.anastatus = 'A'";		
}

if( $pagamentoefetivado == 'S' ){ 
	$where[] = "pt.ptrid in (select ef.ptrid    
							from emenda.execucaofinanceira ef 
								inner join emenda.ordembancaria ob on ob.exfid = ef.exfid
							where ef.exfstatus = 'A' 
								and ef.exfnumsolempenho is not null
							    and ob.spgcodigo = '2')";
} else if( $pagamentoefetivado == 'N' ){
	$where[] = "pt.ptrid in (select ef.ptrid    
							from emenda.execucaofinanceira ef 
								inner join emenda.ordembancaria ob on ob.exfid = ef.exfid
							where ef.exfstatus = 'A' 
								and ef.exfnumsolempenho is not null
							    and ob.spgcodigo <> '2' or ob.spgcodigo is null)";
}

if( $geradoconvenio == 'S' ){
	$where[] = "pc.pmcnumconveniosiafi <> ''";
}else if( $geradoconvenio == 'N' ){
	$where[] = "pc.pmcnumconveniosiafi = ''";
}	

if( $foipublicado == 'S' ){ 
	$where[] = "ptp.pubdatapublicacao is not null";
	$arJoin[] = "LEFT JOIN emenda.ptpublicacao ptp on ptp.pmcid = pc.pmcid and ptp.pubstatus = 'A'";
}else if( $foipublicado == 'N' ){
	$where[] = "ptp.pubdatapublicacao is null";	
	$arJoin[] = "LEFT JOIN emenda.ptpublicacao ptp on ptp.pmcid = pc.pmcid and ptp.pubstatus = 'A'";
}
	
if( $convertidosiaf == 'S' ){
	$where[] = "pc.pmcdataconversaosiafi is not null";
}else if( $convertidosiaf == 'N' ){
	$where[] = "pc.pmcdataconversaosiafi is null";
}	
	
$arrCol = $_SESSION['emenda']['post']['agrupador'];

$arCols = array();

if( (in_array('n_da_nota_de_empenho', $arrCol)) || $empenho[0] && ( $empenho_campo_flag || $empenho_campo_flag == '1' ) || $convenio[0] && ($convenio_campo_flag || $convenio_campo_flag == '1' ) ){
	if( in_array('valor_pago', $arrCol) ){
		$arJoin[] = "left join(
							select ef.exfnumempenhooriginal,
							    sum(ef.exfvalor) as exfvalor,
							    sum(ob.orbvalorpagamento) as valor_pago,
							    ef.ptrid
							from emenda.execucaofinanceira ef
								left join emenda.ordembancaria ob on ob.exfid = ef.exfid
							where ef.exfstatus = 'A'
								and ef.exfnumsolempenho is not null
							group by ef.exfnumempenhooriginal, ef.ptrid
						)ef on ef.ptrid = pt.ptrid";
	} else {
		$arJoin[] = "left join(
							select ef.exfnumempenhooriginal,
								sum(ef.exfvalor) as exfvalor,
								ef.ptrid
						    from emenda.execucaofinanceira ef
						    where ef.exfstatus = 'A'
								and ef.exfnumsolempenho is not null
						    group by ef.exfnumempenhooriginal, ef.ptrid
					    ) ef on ef.ptrid = pt.ptrid";
	}
}

foreach( $_SESSION['emenda']['post']['agrupador'] as $coluna ){
	if($coluna == "n_pta"){ 
		$cabecalho[] = "PTA"; 
		$arCols[] = "(pt.ptrcod || '/' || pt.ptrexercicio) as n_pta";
		//$group[] = 'pt.ptrcod';
		//$group[] = 'pt.ptrexercicio';
	}
	
	if($coluna == "valor_pago" ) { 
		$cabecalho[] = "Valor Pago";
		if( in_array('n_da_nota_de_empenho', $arrCol) ){
			$arCols[] = "ef.valor_pago";
		} else {
			$arCols[] = "(select sum(ob.orbvalorpagamento) from emenda.ordembancaria ob 
					    	inner join emenda.execucaofinanceira ef1 on ef1.exfid = ob.exfid and ef1.exfstatus = 'A'
					    where ef1.ptrid = pt.ptrid) as valor_pago";
		}
	}
	if($coluna == "n_da_nota_de_empenho"){ 
		$cabecalho[] = "Nº da Nota de Empenho";
		$arCols[] = 'ef.exfnumempenhooriginal as n_da_nota_de_empenho';
		//$group[] = 'ef.exfnumempenhooriginal'; 
	}
	if($coluna == "n_do_convenio_no_siafi"){
		$cabecalho[] = "Nº do Convênio no SIAFI";
		$arCols[] = 'pc.pmcnumconveniosiafi as n_do_convenio_no_siafi'; 
		//$group[] = 'pc.pmcnumconveniosiafi';
	}
	if($coluna == "n_do_convenio_no_fnde"){
		$cabecalho[] = "Nº do Convênio do FNDE";
		$arCols[] = "(pt.ptrnumconvenio || '/' || pt.ptranoconvenio) as n_do_convenio_no_fnde"; 
		$group[] = 'pt.ptrnumconvenio';
		//$group[] = 'pt.ptranoconvenio';
	}
	if($coluna == "n_do_processo"){ 
		$cabecalho[] = "Nº do Processo";
		$arCols[] = 'pt.ptrnumprocessoempenho as n_do_processo'; 
		//$group[] = 'pt.ptrnumprocessoempenho';
	}
	if($coluna == "valor_do_convenio"){ 
		$cabecalho[] = "Valor do Convênio";
		$arCols[] = '(select sum(pede.pedvalor) from emenda.ptemendadetalheentidade pede  where pede.ptrid = pt.ptrid) + pt.ptrvalorproponente as valor_do_convenio';
	}
	if($coluna == "siglas"){ 
		$cabecalho[] = "Unidades";
		$arCols[] = "array_to_string(array(SELECT DISTINCT unisigla  FROM emenda.unidades u
												INNER JOIN emenda.analise a on a.uniid = u.uniid
											WHERE a.anastatus = 'A' AND  unistatus = 'A' AND a.ptrid = pt.ptrid), ', ') as siglas"; 
	}	
	if($coluna == "enbnome"){ 
		$cabecalho[] = "Entidade";
		$arCols[] = 'enb.enbnome'; 
		//$group[] = 'enb.enbnome';
	}
	if($coluna == "inivigencia"){ 
		$cabecalho[] = "Data de Celebração";
		$arCols[] = "to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as inivigencia"; 
		//$group[] = 'ptv.vigdatainicio';
	}
	if($coluna == "fimvigencia"){ 
		$cabecalho[] = "Final da vigência";
		$arCols[] = "to_char(ptv.vigdatafim, 'DD/MM/YYYY') as fimvigencia"; 
		//$group[] = 'ptv.vigdatafim';
	}
	if($coluna == "programa"){ 
		$cabecalho[] = "Programa";
		$arCols[] = "(select pro.prgdsc
				        from emenda.ptemendadetalheentidade pe
				        inner join emenda.v_emendadetalheentidade ve on ve.edeid = pe.edeid and ve.edestatus = 'A'
				        inner join emenda.v_funcionalprogramatica vf on vf.acaid = ve.acaid and vf.acastatus = 'A'
				        inner join monitora.programa pro on pro.prgid = vf.prgid
				        where pe.ptrid = pt.ptrid limit 1) as programa"; 
		//$group[] = 'pro.prgdsc';
	}
	if($coluna == "estuf"){ 
		$cabecalho[] = "UF";
		$arCols[] = 'enb.estuf'; 
		$group[] = 'enb.estuf';
	}
	if($coluna == "valor_concedente"){ 
		$cabecalho[] = "Valor Concedente";
		$arCols[] = '(select sum(pede.pedvalor) from emenda.ptemendadetalheentidade pede  where pede.ptrid = pt.ptrid) as valor_concedente';
		//$group[] = 'pt.ptrvalorproponente';
	}
	if($coluna == "exfvalorempenho"){
		$cabecalho[] = "Valor Empenhado";		
		if( in_array('n_da_nota_de_empenho', $arrCol) ){ 		
			$arCols[] = "ef.exfvalor as exfvalorempenho";
		} else {
			$arCols[] = "(select sum(exfvalor) from emenda.execucaofinanceira where ptrid = pt.ptrid and exfstatus = 'A' and exfnumsolempenho is not null) as exfvalorempenho";
		}		
		//$group[] = 'ef.exfvalor';
	}
	if($coluna == "ptrvalorproponente"){ 
		$cabecalho[] = "Valor da Contrapartida";
		$arCols[] = 'pt.ptrvalorproponente as ptrvalorproponente';
		//$group[] = 'pt.ptrvalorproponente';
	}
	if($coluna == "valor_projeto"){ 
		$cabecalho[] = "Valor Projeto";
		$arCols[] = '(select sum(pede.pedvalor) from emenda.ptemendadetalheentidade pede  where pede.ptrid = pt.ptrid) + pt.ptrvalorproponente as valor_projeto';
		//$group[] = 'pti.ptivalortotal'; 
	}
	if($coluna == "status_pagamento"){ 
		$cabecalho[] = "Status de Pagamento";
		if( in_array("valor_pago", $arrCol) ){
		//$arCols[] = 'CASE WHEN sp.spgdescricao is null  THEN  \'Consulta de pagamento não realizado.\' ELSE sp.spgdescricao END as status_pagamento ';
		$cols = ", CASE WHEN valor_pago is null  THEN  'Consulta de pagamento não realizado.' ELSE (select sp.spgdescricao
				      from emenda.ordembancaria ob 
				          inner join emenda.situacaopagamento sp on sp.spgcodigo = ob.spgcodigo
				          inner join emenda.execucaofinanceira f on f.exfid = ob.exfid and f.exfstatus = 'A'
				      where f.ptrid = foo.ptrid
				      order by ob.orbdatapagamento desc limit 1) end as status_pagamento";
		} else {
			$arCols[] = 'CASE WHEN pg.spgdescricao is null  THEN  \'Consulta de pagamento não realizado.\' ELSE pg.spgdescricao END as status_pagamento ';
			$arJoin[] = "left join (
		                	select sp.spgdescricao, f.ptrid, max(ob.orbdatainclusao) as data
						      from emenda.ordembancaria ob 
						          inner join emenda.situacaopagamento sp on sp.spgcodigo = ob.spgcodigo
						          inner join emenda.execucaofinanceira f on f.exfid = ob.exfid and f.exfstatus = 'A'          	
		                      group by sp.spgdescricao, f.ptrid
		                ) pg on pg.ptrid = pt.ptrid";
		} 
	}
}

$stColunas = is_array($arCols) ? implode(',', $arCols) : "*";

$sql  = "select * $cols from(
		SELECT DISTINCT
				pt.ptrid,
				$stColunas
			FROM emenda.planotrabalho as pt
			    inner join emenda.entidadebeneficiada enb on enb.enbid = pt.enbid
			    left join emenda.ptvigencia ptv on ptv.ptrid = pt.ptrid and ptv.vigstatus = 'A'
			    left join emenda.ptminutaconvenio as pc on pc.ptrid = pt.ptrid and pc.pmcstatus = 'A'
            ".(!empty($arJoin) ? implode(' ', $arJoin) : '' )."
			WHERE
				pt.ptrstatus = 'A'
            	AND pt.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = pt.ptrid and tt.ptrstatus = 'A')
				and pt.ptrexercicio = '".$_SESSION['exercicio']."'
				".(!empty($where) ? ' and '.implode( ' and ' , $where) : '')."
			ORDER BY
				pt.ptrid 
		) as foo";
		//ver($sql,d);	
$arDados = $db->carregar( $sql );
//ver($arDados,$arrCol,d);
$ptrid = '';
$unidades = array();

$arRegistro = array();
if($arDados){  
	foreach ($arDados as $key => $v) {
		//$ptrid = explode( '/', $v['n_pta'] );
			
		/*$sql = "SELECT DISTINCT 
					unisigla 
				FROM emenda.unidades u
				INNER JOIN emenda.analise a on a.uniid = u.uniid
				WHERE
					a.anastatus = 'A'
				AND 
					unistatus = 'A'
				AND 
					a.ptrid = ".$v['ptrid'];
	
		$arUnidade = (array) $db->carregarColuna( $sql );*/
		
		if( $v['n_do_processo'] ){
			$v['n_do_processo'] = substr($v['n_do_processo'],0,5) . "." .
			                      substr($v['n_do_processo'],5,6) . "/" .
								  substr($v['n_do_processo'],11,4) . "-" .
								  substr($v['n_do_processo'],15,2);
		}	
		$array = array();
		foreach ($arrCol as $col){
			$array[] = $v[$col]; 		
		}
		array_push($arRegistro, $array);
	}
} 
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<?php 
print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
monta_titulo( 'Relatório de Convênio', '<br>');
 
//$cabecalho 	    = array("Nº PTA", "Valor do Convênio", "Valor Pago", "Nº da Nota de Empenho", "Nº do Convênio no SIAFI", "Nº do Convênio no FNDE", "Nº do Processo", "Unidades Gestoras");
//$tamanho		= array('12%', '13%', '12%', '12%', '12%', '12%', '15%', '12%');															
//$alinhamento	= array('center', 'right', 'center', 'center', 'center', 'center', 'center', 'center');
$db->monta_lista_array($arRegistro, $cabecalho, 100000, 1, 'N', 'center', '');													
//$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
//gera relatorio XLS

if($_GET['exporta'] == 1){
	global $db;
	ob_clean();
	header('content-type: text/html; charset=utf-8');

	$db->sql_to_excel($arRegistro, 'relEmendasPTA', $cabecalho);
	exit;
	$exporta = "false";
}

?>
</body>
</html>
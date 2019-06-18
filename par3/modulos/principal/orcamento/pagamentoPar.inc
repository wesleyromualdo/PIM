<?
set_time_limit( 0 );
include_once '_funcoes_orcamento.php';

if($_REQUEST['requisicao']) {
	header('content-type: text/html; charset=utf-8');
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
?>
<script src="/includes/funcoes.js"></script>
<script type="text/javascript">

function carregarDadosPlanoInterno(pi,ptres) {
	ajaxatualizar('requisicao=carregarFonteRecurso&inplintdsc='+pi+'&inplintptres='+ptres,'dvfonterecurso');
}

function carregarDadosPtres(id, proid) {
	ajaxatualizar('requisicao=carregarPlanoInterno&proid='+proid+'&inplintptres='+id,'dvplanointerno');
}


function confirmarPagamento() {
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: $('#fmconfirmarpagamento').serialize(),
   		async: false,
   		success: function(html){
   			if(html!='ok') {
   				swal('', html, 'error');
   			} else {
   				swal('', 'Pagamento enviado com sucesso');
   			}

   			$('#dvResult').html(html);

   			listaComposicaoEmpenho('S');
   			
   		}
	});
	
	$('#dvconfirmarpagamento').modal('toggle');
	$('#modal-form-large').modal('show');
}



function visualizarXMLSigef() {

	if($('#entcnpj').val()=='') {
		alert('CNPJ é obrigatório');
		return false;
	}

	if($('#pronumeroprocesso').val()=='') {
		alert('Número do Processo é obrigatório');
		return false;
	}

	if($('#valortotalempenho').val()=='') {
		alert('Valor Total do Empenho é obrigatório');
		return false;
	}

	if($('#ptres').val()=='') {
		alert('PTRes é obrigatório');
		return false;
	}

	if($('#planointerno').val()=='') {
		alert('Plano Interno é obrigatório');
		return false;
	}

	if($('#fonterecurso').val()=='') {
		alert('Fonte de Recurso é obrigatório');
		return false;
	}

	if($('#centrogestao').val()=='') {
		alert('Centro de Gestão é obrigatório');
		return false;
	}

	ajaxatualizar('visualizarxmlsigef=1&'+$('#fmconfirmarempenho').serialize(),'dvdadosvisualizarxmlsigef');
	
	$('#dvvisualizarxmlsigef').modal();

	
}

function selecionarSubacaoPagamento(id) {
	if($('input[id=chk_'+id+']:checked').length) {
		$('#valor_pagar_'+id).prop("readonly",false);
		$('#porc_pagar_'+id).prop("readonly",false);
	} else {
		$('#valor_pagar_'+id).val('');
		$('#porc_pagar_'+id).val('');
		$('#valor_pagar_'+id).prop("readonly",true);
		$('#porc_pagar_'+id).prop("readonly",true);
		calcularValorTotalPagamento();
	}
}

function ajaxatualizar(params,iddestinatario) {
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: params,
   		async: false,
   		success: function(html){
   			if(iddestinatario!='') {
   				document.getElementById(iddestinatario).innerHTML = html;
   			}
   		}
	});

}

function carregarProcesso(proid) {
	ajaxatualizar('requisicao=solicitarPagamentoPar&proid='+proid,'dv-form-large');
	$("#modal-form-large").modal();
}

function trocarIcone() {
	if($("#demo").attr('class')=='ibox-content collapse in') {
		$("#ic_avancadas").attr('class', 'fa fa-chevron-up');
	} else {
		$("#ic_avancadas").attr('class', 'fa fa-chevron-down');
	}
}

function calcularIniciativas(id, tipo) {

	switch(tipo) {
		case 'valor':
			var vlr_tot = parseFloat($('#totalitem_'+id).val());
			if($('#valorpago_'+id).val()==''){var vlr_pag = 0;}else{var vlr_pag = parseFloat($('#valorpago_'+id).val());}
			if($('#porcpago_'+id).val()=='') {var por_pmo = 0;} else {var por_pmo = parseInt($('#porcpago_'+id).val());}
			var vlr_ini = parseFloat(replaceAll(replaceAll($('#valor_empenhar_'+id).val(),'.',''),',','.'));
			var saldo = (vlr_tot-vlr_pag);
			if(vlr_ini >= saldo) {
				$('#valor_pagar_'+id).val(mascaraglobal('###.###.###.###,##', saldo.toFixed(2)));
				vlr_ini = saldo;
				var por_pag = (100-por_pmo);
			} else {var por_pag = Math.floor((vlr_ini/vlr_tot)*100);}
			$('#porc_pagar_'+id).val(por_pag);
			break;
		case 'porcentagem':
			
			var vlr_tot = parseFloat($('#totalitem_'+id).val());
			
			if($('#valorpago_'+id).val()=='') {var vlr_pag = 0;} else {var vlr_pag = parseFloat($('#valorpago_'+id).val());}
			if($('#porcpago_'+id).val()=='') {var por_pmo = 0;} else {var por_pmo = parseInt($('#porcpago_'+id).val());}
			
			var por_pag = parseInt($('#porc_pagar_'+id).val());
			var vlr_ini = vlr_tot*(por_pag/100);
			var saldo = (vlr_tot-vlr_pag);

			if((por_pmo+por_pag)==100) {
				$('#valor_pagar_'+id).val(mascaraglobal('###.###.###.###,##', saldo.toFixed(2)));
			} else if(vlr_ini > saldo) {
				$('#valor_pagar_'+id).val(mascaraglobal('###.###.###.###,##', saldo.toFixed(2)));
				calcularIniciativas(id, 'valor');
			} else {
				$('#valor_pagar_'+id).val(mascaraglobal('###.###.###.###,##', vlr_ini.toFixed(2)));
			}
			
			break;
	}

	calcularValorTotalPagamento();
}

function calcularValorTotalPagamento() {

	var total = 0;
	
	$("[id^='chk_']:checked").each(function() {
		var vlr_ini = parseFloat(replaceAll(replaceAll($('#valor_pagar_'+$(this).val()).val(),'.',''),',','.'));

		total += vlr_ini;
		
	});
	
	if(total > 0) {
		$('#valortotalpagamento').val(mascaraglobal('###.###.###.###,##', total.toFixed(2)));
	} else {
		$('#valortotalpagamento').val('');
	}
}

function solicitarPagamento() {

	if($('#valortotalpagamentop').val()=='') {
		swal("Alerta", "Selecione as iniciativas a serem pagas", "error")
		return false;
	}
	
	$('#dvconfirmarpagamento').modal();
	
}


function listaComposicaoEmpenho(empid) {

	$('#dvComposicaoEmpenho').html('');
	$("[id^='btn_empenhosel_']").each(function() {
		$(this).attr('class', 'btn btn-primary');
	});
	
	if(empid!='S') {
		$('#btn_empenhosel_'+empid).attr('class', 'btn btn-success');
		ajaxatualizar('requisicao=listaComposicaoEmpenho&empid='+empid,'dvComposicaoEmpenho');
	}

}

function exibirComposicaoIniciativaPar(ppcid) {

	ajaxatualizar('requisicao=exibirComposicaoIniciativaPar&ppcid='+ppcid,'dvdadoscomposicaoiniciativa');
	$('#dvcomposicaoiniciativa').modal();

}

function exibirComposicaoIniciativaObra(pocid) {

	ajaxatualizar('requisicao=exibirComposicaoIniciativaObra&pocid='+pocid,'dvdadoscomposicaoiniciativa');
	$('#dvcomposicaoiniciativa').modal();

}


</script>

<div id="modal-form" class="modal fade" aria-hidden="true">
    <div id="modal-form-large" class="modal fade" aria-hidden="true">
    	<div class="modal-content" id="dv-form-large"></div>
    </div>
</div>

<?
 
if($_POST['anoprocesso']) $wh[] = "substring(pro.pronumeroprocesso,12,4)='".$_POST['anoprocesso']."'";
if($_POST['estuf']) 	  $wh[] = "m.estuf='".$_POST['estuf']."'";
if($_POST['municipio'])   $wh[] = "m.mundescricao ilike removeacento('%".$_POST['municipio']."%')";
if($_POST['programa'])    $wh[] = "pro.proid IN(SELECT ppc.proid FROM par3.processoparcomposicao ppc 
			  									INNER join par3.iniciativa_planejamento inp ON ppc.inpid = inp.inpid AND inp.inpstatus = 'A'
			  									INNER JOIN par3.iniciativa_iniciativas_programas prg ON inp.iniid = prg.iniid
			  									WHERE ppc.ppcstatus = 'A' AND prg.prgid='".$_POST['programa']."')";
if($_POST['esfera']) {
	if($_POST['esfera']=='E') $wh[] = "inu.estuf IS NOT NULL";
	if($_POST['esfera']=='M') $wh[] = "inu.muncod IS NOT NULL";
}


?>

<div class="wrapper wrapper-content">
	<div class="row">
		<div class="ibox-content">
			
			<form method="post" name="formulario" id="formulario" class="form form-horizontal">
			<input type="hidden" name="tipovisualizacao" id="tipovisualizacao">
			<? 
			
			$filtro = simec_htmlentities( $_POST['numeroprocesso'] );
			$numeroprocesso = $filtro;
			echo $simec->input('numeroprocesso', 'Número do Processo', $numeroprocesso, array('maxlength' => '21', 'class' => 'inteiro'));
			
			?>
			
    		<div id="demo" class="collapse <?=(($wh)?'in':'')?>">
			<?

			$tipoobjetos = "(SELECT 'P' as codigo, 'TIPO OBJETO 1' as descricao) UNION ALL 
						(SELECT '2' as codigo, 'TIPO OBJETO 2' as descricao)";
			echo $simec->select('tipoobjeto', 'Tipo de Objeto', $_POST['tipoobjeto'], $tipoobjetos, array());
			
			$programas = "SELECT prgid as codigo, trim(prgabreviatura)||' - '||prgdsc as descricao FROM par3.programa WHERE prgstatus='A'";
			echo $simec->select('programa', 'Programa', $_POST['programa'], $programas, array());
			
			$anosprocesso = "SELECT DISTINCT substring(pronumeroprocesso,12,4) as codigo,  substring(pronumeroprocesso,12,4) as descricao FROM par3.processo WHERE substring(pronumeroprocesso,12,4) <> '' AND prostatus = 'A' ";
			echo $simec->select('anoprocesso', 'Ano', $_POST['anoprocesso'], $anosprocesso, array());
			
			$empenhos = "(SELECT '1' as codigo, 'TIPO EMPENHO 1' as descricao) UNION ALL
						(SELECT '2' as codigo, 'TIPO EMPENHO 2' as descricao)";
			echo $simec->select('empenho', 'Empenho', $_POST['empenho'], $empenhos, array());
			
			$estados = "SELECT e.estuf as codigo, e.estdescricao as descricao FROM territorios.estado e ORDER BY e.estdescricao ASC";
			echo $simec->select('estuf', 'Estado', $_POST['estuf'], $estados, array());
			
			$filtro = simec_htmlentities( $_POST['municipio'] );
			$municipio = $filtro;
			echo $simec->input('municipio', 'Município', $municipio, array('maxlength' => '255'));
			
			$esfera = $_POST['esfera'];
			$esferas = "( SELECT 'E' as codigo, 'Estadual' as descricao ) UNION ALL ( SELECT 'M' as codigo, 'Municipal' as descricao )";
			echo $simec->select('esfera', 'Esfera', $esfera, $esferas, array());
			
			?>
			</div>
			
			<div class="ibox-tools">
			
				<a data-toggle="collapse" data-target="#demo" onclick="trocarIcone();">Opções avançadas <i id="ic_avancadas" class="fa fa-chevron-<?=(($wh)?'down':'up')?>"></i></a>
			
			</div>
			
				
				
				
			<div class="ibox-footer">
				<div class="form-actions col-md-offset-4">
					<button type="submit" class="btn btn-success" name="pesquisar" onclick="$('#tipovisualizacao').val('html');"><i class="fa fa-search" aria-hidden="true"></i> Pesquisar</button> | 
					<button type="submit" class="btn btn-info" name="excel" onclick="$('#tipovisualizacao').val('excel');"><i class="fa fa-file-excel-o" aria-hidden="true"></i> Gerar Excel</button> | 
					<button type="button" class="btn btn-warning" name="imprimir"><i class="fa fa-print" aria-hidden="true"></i> Imprimir</button> | 
					<button type="button" class="btn btn-success" name="limpar" onclick="window.location='par3.php?modulo=principal/orcamento/empenhoPar&acao=A';"><i class="fa fa-eraser" aria-hidden="true"></i> Limpar</button>
				</div>
			</div>
				
				
			</form>
		</div>
	</div>
<?php

if($_POST['tipovisualizacao']) {

	if($_POST['tipovisualizacao']=='excel') {
		relatorioEmpenhoExcel($_POST);
		exit;
	}

	echo '<div class="row">';
	echo '<div class="ibox-content">';
	echo '<br><br><br><br><br><br>';
	
	if($_POST['numeroprocesso']) $wh[] = "pro.pronumeroprocesso='".$_POST['numeroprocesso']."'";

	$sql = "(
			SELECT
					pro.proid, 
					pro.pronumeroprocesso as numeroprocesso,
					inu.estuf as uf,
					CASE WHEN inu.muncod IS NOT NULL THEN m.mundescricao 
						 WHEN inu.muncod IS NULL THEN e.estdescricao END as descricao,
					CASE WHEN COALESCE(SUM(ipi.ipivalorreferencia*coalesce(ipi.ipiquantidadeaprovada,0)),0) > 0 THEN trim(to_char(COALESCE(SUM(ipi.ipivalorreferencia*coalesce(ipi.ipiquantidadeaprovada,0)),0.00),'999g999g999d99')) ELSE '0,00' END as valorprocesso,
					CASE WHEN COALESCE(emp.empvalorempenho,0) > 0 THEN trim(to_char(COALESCE(emp.empvalorempenho,0.00),'999g999g999d99')) ELSE '0,00' END as valorempenhado,
					CASE WHEN COALESCE(pag.pagvalorparcela,0) > 0 THEN trim(to_char(COALESCE(pag.pagvalorparcela,0.00),'999g999g999d99')) ELSE '0,00' END as valorpagamento,
					CASE WHEN COALESCE(emp.empvalorempenho,0) > 0 THEN '<div style=\"width: '||floor((pag.pagvalorparcela/COALESCE(emp.empvalorempenho,0))*100)||'%\" aria-valuemax=\"100\" aria-valuemin=\"0\" aria-valuenow=\"'||floor((pag.pagvalorparcela/COALESCE(emp.empvalorempenho,0))*100)||'\" role=\"progressbar\" class=\"progress-bar progress-bar-primary\"><span style=\"color:#676a6c;\"><b>'||floor((pag.pagvalorparcela/COALESCE(emp.empvalorempenho,0))*100)||'%</b></span></div>' ELSE '-' END as bprogress,
			  		'PAR GENÉRICO' as tipoobjeto

			FROM par3.processo pro 
			INNER JOIN par3.processoparcomposicao ppc ON ppc.proid = pro.proid 
			inner join par3.empenho e on e.empnumeroprocesso = pro.pronumeroprocesso and e.empstatus = 'A'
			INNER JOIN par3.instrumentounidade inu ON inu.inuid = pro.inuid AND inu.inustatus = 'A'
		    INNER JOIN par3.iniciativa_planejamento inp on inp.inpid = ppc.inpid AND inp.inuid = pro.inuid AND inp.inpstatus = 'A' 
		    INNER JOIN par3.iniciativa ini on ini.iniid = inp.iniid AND ini.inistatus = 'A' 
			LEFT JOIN par3.iniciativa_tipos_objeto tpo on tpo.intoid = ini.intoid
			INNER JOIN par3.iniciativa_itenscomposicao_grupo iig on iig.iniid = inp.iniid --AND iig.iigsituacao = 'A'
			LEFT JOIN par3.itenscomposicao itc on itc.itcid = iig.itcid AND itc.itcstatus = 'A'
			LEFT JOIN par3.itenscomposicao_grupos igr ON igr.igrid = iig.igrid
			INNER JOIN par3.v_analise_planejamento_item_composicao ipi on ipi.iigid = iig.iigid AND ipi.ipistatus = 'A' AND ipi.ipiano = ppc.ppcano AND ipi.inpid = inp.inpid 
			LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod 
			LEFT JOIN territorios.estado e ON e.estuf = inu.estuf 
			LEFT JOIN ( SELECT ipe.ipiid, SUM(ipe.ipequantidade) as quantidade FROM par3.iniciativa_planejamento_item_composicao_escola ipe WHERE ipe.ipestatus = 'A' GROUP BY ipe.ipiid ) esc ON esc.ipiid = ipi.ipiid
			LEFT JOIN ( SELECT empnumeroprocesso, COALESCE(SUM(empvalorempenho),0)::numeric as empvalorempenho FROM par3.empenho WHERE empstatus='A' GROUP BY empnumeroprocesso ) emp ON emp.empnumeroprocesso = pro.pronumeroprocesso 
			LEFT JOIN ( SELECT e.empnumeroprocesso, COALESCE(SUM(p.pagvalorparcela),0)::numeric as pagvalorparcela FROM par3.empenho e INNER JOIN par3.pagamento p ON e.empid = p.empid WHERE p.pagstatus='A' AND e.empstatus='A' GROUP BY e.empnumeroprocesso ) pag ON pag.empnumeroprocesso = pro.pronumeroprocesso
			".(($wh)?"WHERE ".implode(" AND ", $wh):"")." 
			GROUP BY  pro.proid, pro.pronumeroprocesso, inu.estuf, inu.muncod, m.mundescricao, e.estdescricao, emp.empvalorempenho, pag.pagvalorparcela 
			ORDER BY inu.estuf, descricao, pro.pronumeroprocesso
			
			) UNION ALL (

												  			
			SELECT
				pro.proid, 
                pro.pronumeroprocesso as numeroprocesso,
				inu.estuf as uf,
				CASE WHEN inu.muncod IS NOT NULL THEN m.mundescricao 
					 WHEN inu.muncod IS NULL THEN e.estdescricao END as descricao,
	  			CASE WHEN COALESCE(SUM(obr.obrvalor - COALESCE(obr.obrvalor_contrapartida,0)),0) > 0 THEN trim(to_char(COALESCE(SUM(obr.obrvalor - COALESCE(obr.obrvalor_contrapartida,0)),0.00),'999g999g999d99')) ELSE '0,00' END as valorprocesso,
				CASE WHEN COALESCE(emp.empvalorempenho,0) > 0 THEN trim(to_char(COALESCE(emp.empvalorempenho,0.00),'999g999g999d99')) ELSE '0,00' END as valorempenhado,
				CASE WHEN COALESCE(pag.pagvalorparcela,0) > 0 THEN trim(to_char(COALESCE(pag.pagvalorparcela,0.00),'999g999g999d99')) ELSE '0,00' END as valorpagamento,
				CASE WHEN COALESCE(emp.empvalorempenho,0) > 0 THEN '<div style=\"width: '||floor((pag.pagvalorparcela/COALESCE(emp.empvalorempenho,0))*100)||'%\" aria-valuemax=\"100\" aria-valuemin=\"0\" aria-valuenow=\"'||floor((pag.pagvalorparcela/COALESCE(emp.empvalorempenho,0))*100)||'\" role=\"progressbar\" class=\"progress-bar progress-bar-primary\"><span style=\"color:#676a6c;\"><b>'||floor((pag.pagvalorparcela/COALESCE(emp.empvalorempenho,0))*100)||'%</b></span></div>' ELSE '-' END as bprogress,
				'OBRA' as tipoobjeto
			FROM 
                par3.obra obr
			INNER JOIN par3.processoobracomposicao poc ON poc.obrid = obr.obrid AND pocstatus = 'A'
			INNER JOIN par3.processo pro ON pro.proid = poc.proid AND pro.prostatus = 'A'
			inner join par3.empenho e on e.empnumeroprocesso = pro.pronumeroprocesso and e.empstatus = 'A'
			INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = obr.inpid AND inpstatus = 'A' 
			INNER JOIN par3.instrumentounidade inu ON inu.inuid = pro.inuid 
			LEFT JOIN territorios.municipio m ON m.muncod = inu.muncod 
			LEFT JOIN territorios.estado e ON e.estuf = inu.estuf 
			LEFT JOIN ( SELECT empnumeroprocesso, COALESCE(SUM(empvalorempenho),0)::numeric as empvalorempenho FROM par3.empenho WHERE empstatus='A' GROUP BY empnumeroprocesso ) emp ON emp.empnumeroprocesso = pro.pronumeroprocesso
			LEFT JOIN ( SELECT e.empnumeroprocesso, COALESCE(SUM(p.pagvalorparcela),0)::numeric as pagvalorparcela FROM par3.empenho e INNER JOIN par3.pagamento p ON e.empid = p.empid WHERE p.pagstatus='A' AND e.empstatus='A' GROUP BY e.empnumeroprocesso ) pag ON pag.empnumeroprocesso = pro.pronumeroprocesso 
			".(($wh)?"WHERE ".implode(" AND ", $wh):"")."
			GROUP BY  pro.proid, pro.pronumeroprocesso, inu.estuf, inu.muncod, m.mundescricao, e.estdescricao, emp.empvalorempenho, pag.pagvalorparcela 
			ORDER BY inu.estuf, descricao, pro.pronumeroprocesso
					
			)";

	$cabecalho = array("Número do Processo","UF","Local","Valor do Processo","Valor Empenhado","Valor Pago","% Pago","Tipo");

	$listagem = new Simec_Listagem();
	$listagem->setCabecalho($cabecalho);
	$listagem->setQuery($sql);
	$listagem->addAcao('edit', 'carregarProcesso', array('titulo' => 'Editar'));
	$listagem->setFormFiltros('formulario');
	$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
	$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
	
	echo '</div>';
	echo '</div>';
	
}

?>
</div>
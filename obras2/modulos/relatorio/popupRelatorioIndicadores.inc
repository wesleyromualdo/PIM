<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
$where = array();
extract($_REQUEST);

// Programa
if( $prfid[0] && $prfid_campo_flag ){
	if ( !$prfid_campo_excludente ){
		array_push($where, " ep.prfid  IN (" . implode( ',', $prfid ) . ") ");	
	}else{
		array_push($where, " ( ep.prfid  NOT IN (" . implode( ',', $prfid ) . ") OR ep.prfid is null ) ");
	}
	
}
// Fonte
if( $tooid[0] && $tooid_campo_flag ){
	if ( !$tooid_campo_excludente ){
		array_push($where, " oi.tooid  IN (" . implode( ',', $tooid ) . ") ");	
	}else{
		array_push($where, " ( oi.tooid  NOT IN (" . implode( ',', $tooid ) . ") OR oi.tooid IS NULL ) ");
	}
	
}

if( !empty( $obrid ) ) {
    $obridTmp = removeAcentos(str_replace("-"," ",$obrid));
    $where[] = " ( UPPER(public.removeacento(oi.obrnome)) ILIKE ('%" . $obridTmp . "%') OR
    oi.obrid::CHARACTER VARYING ILIKE UPPER('%" . $obrid . "%') ) ";
}
if( !empty( $preid ) ) array_push($where, "oi.preid = $preid");

if( !empty( $estuf ) ) array_push($where, "edo.estuf = '{$estuf}'");

if ( $muncod ){
    $muncod  = (array) $muncod;
    $where[] 		  = "mun.muncod IN('" . implode("', '", $muncod) . "')";
}

if ( $empesfera ){
    $empesfera = (array) $empesfera;
    $where[] = "eP.empesfera IN('" . implode("', '", $empesfera) . "')";
}

$sql = "SELECT DISTINCT
            (select
                -- MIN(pag.pagdatapagamentosiafi) as data_primeiro_pagamento,
                TO_CHAR(MIN(pag.pagdatapagamentosiafi) + 720, 'DD/MM/YYYY') as prazo
            from
                par.pagamentoobra po
            inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
            where
                po.preid = pre.preid) vigencia,
            COALESCE(oi.obrvalorprevisto, 0.00) as obrvalorprevisto,
            ocrvalorexecucao,
            (SELECT MAX(TO_CHAR(ter.terdatainclusao, 'DD/MM/YYYY')) 
                FROM par.termocompromissopac ter
                LEFT JOIN par.termoobra tob ON tob.terid = ter.terid
                WHERE tob.terid = pre.preid AND ter.terstatus='A') terdatainclusao,
            (SELECT MAX(ter.terid || '/' || TO_CHAR(ter.terdatainclusao, 'YYYY'))
                FROM par.termocompromissopac ter
                LEFT JOIN par.termoobra tob ON tob.terid = ter.terid
                WHERE tob.terid = pre.preid AND ter.terstatus='A') datainclusao,
			oi.obrid,
		    oi.preid,
		    oi.obrnome,
		    pf.prfdesc,
		    too.toodescricao,
		    ed.esddsc as situacao,
            oi.obrpercentultvistoria || '%' obrpercentultvistoria,
            TO_CHAR(oi.obrdtultvistoria, 'DD/MM/YYYY') obrdtultvistoria,
            ep.emppercentultvistoriaempresa || '%' emppercentultvistoriaempresa,
            to_char(ep.empdtultvistoriaempresa, 'DD/MM/YYYY') as suedtatualizacao,
            CASE WHEN oi.preid IS NULL THEN
                '-'
            ELSE
              (p.totalpago)::varchar
            END as valorpago,
            (
                SELECT
                    TO_CHAR(MIN(pag.pagdatapagamentosiafi), 'DD/MM/YYYY') AS prazo
                FROM
                    par.pagamentoobra po
                INNER JOIN par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
                WHERE
                    po.preid = pre.preid

            ) pagdatapagamentosiafi

		FROM obras2.obras oi

		LEFT JOIN obras2.empreendimento ep ON oi.empid = ep.empid
		LEFT JOIN workflow.documento doc ON doc.docid = oi.docid 
        LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = doc.esdid
			
		LEFT JOIN obras2.obrascontrato 		 oc ON oc.obrid = oi.obrid AND oc.ocrstatus = 'A'
        LEFT JOIN entidade.entidade ent ON ent.entid = ep.entidunidade
        LEFT JOIN obras2.tipoorigemobra too ON too.tooid = oi.tooid
        LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid
        LEFT JOIN obras.preobra pre ON pre.preid = oi.preid

        LEFT JOIN entidade.endereco edo on edo.endid = oi.endid
        LEFT JOIN territorios.municipio mun on mun.muncod = edo.muncod

        LEFT JOIN par.vm_pagamento_total_por_obra p ON p.obrid = oi.obrid

		    $inner
		WHERE
			oi.obrstatus = 'A' AND
			oi.obridpai IS NULL
			".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )."
        order by oi.obrid";

$cabecalho = array( "Vigência", "Valor previsto", "Valor contratado", "Ano do termo de compromisso", "Numero do termo de compromisso", "ID da obra",
"ID pré-obra", "Nome da obra", "Programa", "Fonte", "Situação da obra", "Supervisão Unidade", "Última supervisão unidade", "Supervisão EMPRESA", "Última supervisão empresa", "Valor pago", "Data Pagamento SIAFI");
// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == 'xls' ){
	$db->sql_to_xml_excel($sql, 'relatorio_indicadores', $cabecalho);
}


?>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
	<script type="text/javascript" src="/includes/remedial.js"></script>
	<script type="text/javascript" src="/includes/superTitle.js"></script>
<html>
	<head>
		<title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
	</body>
	<script type="text/javascript">
		var u='/obras2/obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A&titleFor=';
		function obrIrParaCaminhoPopUp( obrid, tipo, orgid, arqid ){
			switch( tipo ){			
				case "cadastro":
					<?$_SESSION['obras']['orgid'] = 3; ?>
					window.opener.location.href = "obras2.php?modulo=principal/cadastro&acao=A&obrid=" + obrid+"&orgid=3";
					window.close(); 
				break;			
			}
		
		}
			
	</script>
</html>
<?
//if( $where ){
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '', '', '','');
//}
?>
<center>
	<p><input type="button" id="btnGeraExcel" value="Gerar Arquivo Excel"></p>
</center>
<?php
echo '<form id="request_Form" method="post">';
if(is_array($_POST)){
	if(count($_POST)){		
		$naoProcessar = array('pesquisa');
		foreach($_POST as $k => $v){
			if(!in_array($k, $naoProcessar)){
				if(is_array($v)){
					foreach($v as $vv){
						echo '<input type="hidden" name="'.$k.'[]" value="'.$vv.'" />';	
					}			
				}else{
					echo '<input type="hidden" name="'.$k.'" value="'.$v.'" />';	
				}
			}
		}
	}	
}
echo "</form>";
?>
<div id="dv_sql"></div>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	
	$('#btnGeraExcel').click(function(){	
		window.open ('obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A&pesquisa=xls&'+$('#request_Form').serialize(),
					"geraXls");		
	});
	
});
</script>
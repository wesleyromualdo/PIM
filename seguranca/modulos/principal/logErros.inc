<?php

if( $_REQUEST['requisicao'] == 'mostraErroTXT' ){
    $sql = "SELECT errtipo,
           CASE
                WHEN errtipo = 'DB' THEN 'Banco de dados'
                WHEN errtipo = 'PR' THEN 'Programação'
                WHEN errtipo = 'QB' THEN 'Queda no banco'
                WHEN errtipo = 'WS' THEN 'WebService'
                WHEN errtipo = 'EN' THEN 'Encoding no banco'
                WHEN errtipo = 'PD' THEN 'Erro na Conexão'
                WHEN errtipo = 'DC' THEN 'Diretório Cheio'
                WHEN errtipo = 'AI' THEN 'Arquivo inexistente'
                WHEN errtipo = 'DV' THEN 'Diversos'
                ELSE errtipo
           END as tipo_erro,
           e.errdescricaocompleta,
           TO_CHAR(e.errdata, 'DD/MM/YYYY HH24:MI:SS') as errdata,
           e.errarquivo,
           e.errlinha,
           e.usucpf,
           u.usunome,
           s.sisdsc 
    FROM seguranca.erro e 
    LEFT JOIN  seguranca.sistema s USING(sisid)
    LEFT JOIN seguranca.usuario u USING(usucpf) 
    WHERE e.errid = {$_REQUEST['errid']}";
    $arr = $db->carregar($sql);
    extract($arr[0]);
    if( strlen($usucpf) == 11 ){
        $usucpf = substr($usucpf, 0, 3).'.'.
                  substr($usucpf, 3, 3).'.'.
                  substr($usucpf, 6, 3).'-'.
                  substr($usucpf, 9, 2);

    }
    $logErro .= "<div style='font-size: 14px; font-family: tahoma; font-weight: bold; margin-bottom: 2px'>Tipo de Erro: {$tipo_erro}</div>";
    $logErro .=  "<div style='font-size: 14px; font-family: tahoma; font-weight: bold; ; margin-bottom: 2px'>Sistema: {$sisdsc}</div>";
    $logErro .=  "<div style='font-size: 13px; font-family: tahoma; margin-bottom: 2px'>Data do Erro: {$errdata}</div>";
    $logErro .=  "<div style='font-size: 13px; font-family: tahoma; margin-bottom: 2px'>Erro na Linha: {$errlinha}</div>";
    $logErro .=  "<div style='font-size: 13px; font-family: tahoma; margin-bottom: 2px'>Erro no Arquivo: {$errarquivo}</div>";
    $logErro .=  "<div style='font-size: 13px; font-family: tahoma; margin-bottom: 2px'>Usuário: {$usunome} - {$usucpf}</div>";
    $logErro .=  "<br>";
    $logErro .= $errdescricaocompleta;

	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
		<tr>
			<th style="text-align: center;">Log Erro</th>
		</tr>
		<tr>
			<td style="font-size: 12px"><pre><?=$logErro; ?></pre></td>
		</tr>
	</table>
<?
	exit();
}


include APPRAIZ ."includes/cabecalho.inc";
require_once APPRAIZ . 'includes/library/simec/view/Helper.php';

$simec = new Simec_View_Helper();

$_REQUEST['aba'] = ($_REQUEST['aba'] ? $_REQUEST['aba'] : '1');

if($_REQUEST['aba'] == 1){
    $url = 'seguranca/seguranca.php?modulo=principal/logErros&acao=A&aba=1';
}

if($_REQUEST['aba'] == 2){
    $url = 'seguranca/seguranca.php?modulo=principal/logFalhaInesperada&acao=A&aba=2';
}
$abasQuestoes= array(
		0 => array( "descricao" => "Log de Erros", "link"	=> "seguranca.php?modulo=principal/logErros&acao=A&aba=1"),
		1 => array( "descricao" => "Lista Erros Agrupados", "link"	=> "seguranca.php?modulo=principal/logFalhaInesperada&acao=A&aba=2")
);
echo '<br>';
echo $simec->tab ( $abasQuestoes , $url );
echo '<br>';
echo '<br>';
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Log de Erros", "");

$_REQUEST['errdata'] = (empty($_REQUEST['errdata']) ? date('d/m/Y') : $_REQUEST['errdata']);

$filtro = '';
if( $_REQUEST['requisicao'] == 'pesquisa' ){
	if( !empty($_REQUEST['sisid']) ){
		$filtro .= " and e.sisid = {$_REQUEST['sisid']} ";
	}
	if( !empty($_REQUEST['usucpf']) ){
		$filtro .= " and e.usucpf = '{$_REQUEST['usucpf']}' ";
	}
}

?>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" value="pesquisa" id="requisicao"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td colspan="4" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="15%">Sistema:</td>
			<td width="33%">
				<?php 
					$sisid = $_REQUEST["sisid"];
					$sql = "select e.sisid as codigo, e.sisdsc as descricao from seguranca.sistema e order by e.sisid";
					$db->monta_combo("sisid",$sql,'S',"Todas",'carregaDados','','','500','N','sisid', '', '', '', 'class="sel_chosen"');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="15%">Data do Erro:</td>
			<td width="33%">
				<?php 
				
					$errdata = $_REQUEST['errdata'];
					echo campo_data2('errdata', 'S', 'S', 'Data do Erro', '', '', '')
				?>
			</td>
		</tr>
<?php 	if($_REQUEST['sisid']){?>
		<tr>
			<td class="subtitulodireita" width="15%">Usuário:</td>
			<td width="33%">
				<?php 
				if( $_REQUEST['errdata'] ){
					$filData = " and cast(to_char(er.errdata, 'YYYY-MM-DD') as date) = cast('{$_REQUEST['errdata']}' as  date) ";
				}
				$usucpf = $_REQUEST["usucpf"];
				$sql = "select distinct u.usucpf as codigo, u.usunome as descricao
						from seguranca.usuario u 
							inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
							inner join seguranca.erro er on er.sisid = us.sisid and er.usucpf = u.usucpf $filData
						where us.suscod = 'A'
							and us.sisid = {$_REQUEST['sisid']}
						order by u.usunome";
				
				$db->monta_combo("usucpf",$sql,'S',"Todas",'','','','500','N','usucpf', '', '', '', 'class="sel_chosen"');
				?>
			</td>
		</tr>
<?php 	}?>
		<tr>
			<td class="subtitulodireita">Tipo de Consulta:</td>
			<td>
				<input type="radio" value="S" id="tipo_consulta" name="tipo_consulta" <? if($_REQUEST["tipo_consulta"] == "S" || $_REQUEST["tipo_consulta"] == "") { echo "checked"; } ?> /> Com Agrupamento 
				<input type="radio" value="N" id="tipo_consulta" name="tipo_consulta" <? if($_REQUEST["tipo_consulta"] == "N") { echo "checked"; } ?> /> Sem Agrupamento
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" align="center">
				<input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisaErros();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<div id="dialog_recurso" title="Visualizar XML" style="display: none;" >
	<div style="padding:5px;text-align:justify;" id="mostraRetornoRecurso"></div>
</div>
<script type="text/javascript">
jq('.sel_chosen').chosen({allow_single_deselect:true});

function pesquisaErros(){
	if( jQuery('[name="sisid"]').val() == '' ){
		alert('O sistema é Obrigatório');
		return false;
	}
	if( jQuery('[name="errdata"]').val() == '' ){
		alert('A Data do Erro é Obrigatório');
		return false;
	}
	jQuery('[name="formulario"]').submit();
}

function carregaDados(obj){
	window.location.href = window.location+'&sisid='+obj;
}

function mostraErroTXT( errid ){
	
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "requisicao=mostraErroTXT&errid="+errid,
		async: false,
		success: function(msg){
			
			jQuery("#mostraRetornoRecurso").css('height', '600px');			
			jQuery("#mostraRetornoRecurso").css('overflow', 'auto');
			
			jQuery( "#dialog_recurso" ).show();
			jQuery( "#mostraRetornoRecurso" ).html(msg);
			jQuery( '#dialog_recurso' ).dialog({
					resizable: false,
					width: 1000,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Fechar': function() {
							jQuery( this ).dialog( 'close' );
						}
					}
			});
		}
	});
}
</script>
<?php
if( $_REQUEST['requisicao'] == 'pesquisa' ){
		//e.errid
	
	if( $_REQUEST['tipo_consulta'] == 'N' ){
		$sql = "select codigo, sisabrev, '<a onclick=\"mostraErroTXT('||errid||')\" >'||errdescricao||'</a>' as errdescricao, descricao, errarquivo, linha, to_char(errdata, 'DD/MM/YYYY HH24:MI:SS') as errdata
				from(
					select distinct '<span style=\"color:#0066cc;\">'||e.sisid||'</span>' as codigo, sisabrev, 
							e.errid,
							e.errdescricao, 
							e.sisid,
							e.errdata,
							case
				                    when errtipo = 'DB' then 'Banco de dados'
				                    when errtipo = 'PR' then 'Programação'
				                    when errtipo = 'QB' then 'Queda no banco'
				                    when errtipo = 'WS' then 'WebService'
				                    when errtipo = 'EN' then 'Encoding no banco'
				                    when errtipo = 'PD' then 'Erro na Conexão'
				                    when errtipo = 'DC' then 'Diretório Cheio'
				                    when errtipo = 'AI' then 'Arquivo inexistente'
				                    when errtipo = 'DV' then 'Diversos'
				                    else errtipo
				                end as descricao,
				                errarquivo, '<span style=\"color:#0066cc;\">'||errlinha||'</span>' as linha
					from seguranca.erro e
						left join seguranca.sistema s on s.sisid = e.sisid
					where cast(to_char(errdata, 'YYYY-MM-DD') as date) = cast('{$_REQUEST['errdata']}' as  date)
				    	$filtro
					order by e.sisid, errdata
		        ) as foo";
	} else {        	 
		$sql = "select codigo, sisabrev, '<a onclick=\"mostraErroTXT('||max_errid||')\" >'||errdescricao||'</a>' as errdescricao, descricao, errarquivo, linha, total
				from(
					select distinct '<span style=\"color:#0066cc;\">'||e.sisid||'</span>' as codigo, sisabrev, 
						e.errdescricao, 
						e.sisid, (select max(errid) from seguranca.erro 
									where errarquivo = e.errarquivo
										and errdescricao = e.errdescricao
									    and errlinha = e.errlinha
									    and errtipo = e.errtipo) as max_errid,
			                case
			                    when errtipo = 'DB' then 'Banco de dados'
			                    when errtipo = 'PR' then 'Programação'
			                    when errtipo = 'QB' then 'Queda no banco'
			                    when errtipo = 'WS' then 'WebService'
			                    when errtipo = 'EN' then 'Encoding no banco'
			                    when errtipo = 'PD' then 'Erro na Conexão'
			                    when errtipo = 'DC' then 'Diretório Cheio'
			                    when errtipo = 'AI' then 'Arquivo inexistente'
			                    when errtipo = 'DV' then 'Diversos'
			                    else errtipo
			                end as descricao,
			                errarquivo, '<span style=\"color:#0066cc;\">'||errlinha||'</span>' as linha, '<span style=\"color:#0066cc;\">'||count(*)||'</span>' as total
			        from seguranca.erro e
			                left join seguranca.sistema s on s.sisid = e.sisid
			        where cast(to_char(errdata, 'YYYY-MM-DD') as date) = cast('{$_REQUEST['errdata']}' as  date)
			        	$filtro
			        group by errdescricao, errtipo, sisabrev, e.sisid, errarquivo, errlinha
			        order by e.sisid
		        ) as foo";
	}
	
	echo '<h3 style="margin-top: 20px; color: #ff0000; text-align: center;">Erros ÚNICOS de (' . $errdata . ')</h3>';
	$cabecalho = array('ID', 'Módulo', 'Erro', 'Tipo', 'Arquivo', 'Linha', 'Qtd Erros');
	$db->monta_lista($sql, $cabecalho, 50, 4, 'N','Center');
}
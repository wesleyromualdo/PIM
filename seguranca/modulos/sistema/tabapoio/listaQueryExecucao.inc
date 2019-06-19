<?php
if( $_REQUEST['requisicao'] == 'executar' ){
    
    
    foreach ($_REQUEST['pid'] as $pid) {
        $db->executar("select pg_terminate_backend('".$pid."')");
    }
    $db->commit();
    $db->sucesso('sistema/tabapoio/listaQueryExecucao');
    exit();
}

if( $_REQUEST['requisicao'] == 'mostraQuery' ){
    
    $sql = "select query 
        from pg_stat_activity
        where pid = {$_REQUEST['pid']}";
    $query = $db->pegaUm($sql);
    
    ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
		<tr>
			<th style="text-align: center;">QUERY</th>
		</tr>
		<tr>
			<td style="font-size: 12px; font-family: 'courier new';"><pre><?=simec_htmlentities($query); ?></pre></td>
		</tr>
	</table>
	<?
	exit();
}

$_REQUEST['query_idle'] = ($_REQUEST['query_idle'] ? $_REQUEST['query_idle'] : 'N' );
$_REQUEST['query_dat_pos'] = ($_REQUEST['query_dat_pos'] ? $_REQUEST['query_dat_pos'] : 'N' );

include  APPRAIZ."includes/cabecalho.inc";
?>
<style>
    .glyphicon{
        cursor: pointer;
    }
</style>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
	
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtitulodireita" width="30%">IP:</td>
			<td width="70%"><? echo campo_texto( 'IP', 'S', 'S', 'IP', 50, 50, '', '','','','','id="ip"'); ?></td>
		</tr>
		<tr>
		<td class="subtitulodireita">Query IDLE?</td>
    		<td>
    			<input type="radio" name="query_idle" id="query_idle_s" value="S" <?php echo ( ($_REQUEST['query_idle'] == 'S') ? 'checked="checked"' : '') ?> > Sim
    			<input type="radio" name="query_idle" id="query_idle_n" value="N" <?php echo ( ($_REQUEST['query_idle'] == 'N' || empty($_REQUEST['query_idle']) ) ? 'checked="checked"' : '') ?> > Não
    			<input type="radio" name="query_idle" id="query_idle_t" value="T" <?php echo ( ($_REQUEST['query_idle'] == 'T') ? 'checked="checked"' : '') ?> > Todos
    		</td>
		</tr>
		<td class="subtitulodireita">Query datname postgres?</td>
    		<td>
    			<input type="radio" name="query_dat_pos" id="query_dat_pos_s" value="S" <?php echo ( ($_REQUEST['query_dat_pos'] == 'S') ? 'checked="checked"' : '') ?> > Sim
    			<input type="radio" name="query_dat_pos" id="query_dat_pos_n" value="N" <?php echo ( ($_REQUEST['query_dat_pos'] == 'N' || empty($_REQUEST['query_dat_pos']) ) ? 'checked="checked"' : '') ?> > Não
    			<input type="radio" name="query_dat_pos" id="query_dat_pos_t" value="T" <?php echo ( ($_REQUEST['query_dat_pos'] == 'T') ? 'checked="checked"' : '') ?> > Todos
    		</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" align="center">
				<input type="button" value="Pesquisar" name="btnPesquisar" id="btnPesquisar" style="cursor: pointer;">
				<input type="button" value="Executar" name="btnExecutar" id="btnExecutar" style="cursor: pointer;">
			</td>
		</tr>
	</table>

<?


    $where = "";
    if( $_REQUEST['query_idle'] == 'N' ) $where .= " and query not like  '%IDLE%' ";
    if( $_REQUEST['query_dat_pos'] == 'N' ) $where .= " and datname not in ('postgres') ";
    
    if( $_REQUEST['query_idle'] == 'S' ) $where .= " and query like  '%IDLE%' ";
    if( $_REQUEST['query_dat_pos'] == 'S' ) $where .= " and datname in ('postgres') ";
    
    $sql = "select '' as acao, datname, pid, usename, waiting, query, 
                  client_addr, (now() - backend_start) as tempo_backend, (now() - query_start) as tempo_query,
                  ((substring(replace((now() - query_start)::interval::varchar,':',''),1,2)::integer*3600)+(substring(replace((now() - query_start)::interval::varchar,':',''),3,2)::integer*60)+ (substring(replace((now() - query_start)::interval::varchar,':',''),5,9)::float)) dur_segundos
            from pg_stat_activity
            where 1=1
                $where
            order by tempo_query desc;";
    
    $arrDados = $db->carregar ( $sql );
    $arrDados = $arrDados ? $arrDados : array();
    ?>
    <table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    	<tr>
    		<td class="subtitulocentro">Lista de Publicador</td>
    	</tr>
    	<tr>
    		<td>
    	<?
    	$cabecalho = array( '<div>
                        	<input type="checkbox" id="seleciona_todos" value="" onclick="selecionaTodos();">
                        	<label for="seleciona_todos"><b>Todos</b></label>
                        	</div>', "query", "pid", "datname", "usename", "waiting", "client_addr", "tempo_backend", "tempo_query", "dur_segundos");
    		$html = '';
    		if ($arrDados) {
    		    $html .= '
            <table id="tblform" class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="right">
    		<thead>
                <tr>';
    		    foreach ($cabecalho as $cab) {
    		        $html .= '<td align="Center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
                				onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>'.$cab.'</strong></td>';
    		    }
    $html .= '
    		</tr>
    		</thead>';
    		    $total = 0;
    		    foreach ( $arrDados as $key => $valor ) {
    		        
    		        $key % 2 ? $cor = "#FFFFFF" : $cor = "";
    		        $html .= '
    			  <tr bgcolor="' . $cor . '" id="tr_' . $key . '" onmouseout="this.bgColor=\'' . $cor . '\';" onmouseover="this.bgColor=\'#ffffcc\';">
    			  	<td style="text-align: center;">
                        <input id="pid_' . $valor ['pid'] . '" type="checkbox" class="chkbox" name="pid[]" value="' . $valor ['pid'] . '" ></td>
    			  	<td style="text-align: center; cursor:pointer; color: rgb(0, 102, 204) !important;"><a href="#" onclick="abreQuery(' . $valor ['pid'] . ')">' . substr($valor ['query'], 0, 50) . '</a></td>
    			  	<td style="text-align: center; color: rgb(0, 102, 204);">' . $valor ['pid'] . '</td>
    			  	<td style="text-align: center;">' . $valor ['datname'] . '</td>
    			  	<td style="text-align: center;">' . $valor ['usename'] . '</td>
    			  	<td style="text-align: center;">' . $valor ['waiting'] . '</td>
    			  	<td style="text-align: center; color: rgb(0, 102, 204);">' . $valor ['client_addr'] . '</td>
    			  	<td style="text-align: center; color: rgb(0, 102, 204);">' . $valor ['tempo_backend'] . '</td>
    			  	<td style="text-align: center; color: rgb(0, 102, 204);">' . $valor ['tempo_query'] . '</td>
    			  	<td style="text-align: center; color: rgb(0, 102, 204);">' . $valor ['dur_segundos'] . '</td>
    			  </tr>';
    		    }
    		    $html.= '<tfoot>
					<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
                	<tbody>
                		<tr bgcolor="#ffffff">
                			<td>
                				<b>Total de Registros: '.sizeof($arrDados).'</b>
                			</td>
                			<td></td>
                		</tr>
                	</tbody>
                </table>
				</tfoot>';
    		} else {
    		    $html .= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
    		    $html .= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
    		}
    		$html .= '</table>';
    		echo $html;
    		?>
    		</td>
    	</tr>
    </table>
</form>

<div id="dialog_recurso" title="Visualizar Query" style="display: none;" >
	<div style="padding:5px;text-align:justify;" id="mostraRetornoRecurso"></div>
</div>

<script type="text/javascript">
jQuery(document).ready(function() {
    var $chkboxes = jQuery('.chkbox');
    var lastChecked = null;

    jQuery('.chkbox').click(function(e) {
        if(!lastChecked) {
            lastChecked = this;
            return;
        }

        if(e.shiftKey) {
            var start = jQuery('.chkbox').index(this);
            var end = jQuery('.chkbox').index(lastChecked);

            jQuery('.chkbox').slice(Math.min(start,end), Math.max(start,end)+ 1).attr('checked', lastChecked.checked);

        }

        lastChecked = this;
    });
});

function abreQuery( pid ){
	
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "requisicao=mostraQuery&pid="+pid,
		async: false,
		success: function(msg){
			jQuery("#mostraRetornoRecurso").css('height', '500px');
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

jQuery('#btnExecutar').click(function(){
	if( jQuery('[name="pid[]"]:checked').length == 0 ){
		alert('Selecione pelo menos uma query!');
		return false;
	}
	jQuery('[name="requisicao"]').val('executar');
	jQuery('[name="formulario"]').submit();
	
});

jQuery('#btnPesquisar').click(function(){
	jQuery('[name="requisicao"]').val('pesquisar');
	jQuery('[name="formulario"]').submit();
	
});

function selecionaTodos() {

	var selecionaTodos 	= document.getElementById("seleciona_todos");
	var inpid 			= document.getElementsByName("pid[]");
	
	for(var i=0; i<inpid.length; i++) {
		if(selecionaTodos.checked) {
			if(!inpid[i].checked && !inpid[i].disabled){
				 inpid[i].checked = true;
			}
		} else {
			if(inpid[i].checked){ 
				inpid[i].checked = false;
			}
			
		}
	}
}

</script>
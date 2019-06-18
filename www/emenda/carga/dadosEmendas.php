<?php

set_time_limit(30000);
ini_set("memory_limit", "3000M");

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once "../_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
header('content-type: text/html; charset=utf-8');

$array_libera = array('00797370137');

if( !in_array($_SESSION['usucpforigem'], $array_libera) ) {
	die();
}
unset($_SESSION['emenda']);
$_SESSION['usunome'] = 'Adminstrador de sistema';
$_SESSION['usuemail'] = 'simec.spo@mec.gov.br';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if( $_REQUEST['requisicao'] == 'carregaTabelas' ){    
    $sql = "SELECT DISTINCT tablename AS codigo, tablename AS descricao FROM pg_catalog.pg_tables WHERE schemaname = '{$_REQUEST['schemaname']}' ORDER BY tablename ASC;";
    echo $db->monta_combo("tablename",$sql,'S',"Todas",'carregaAtributo','','','500','N','tablename', '', '', '', 'class="sel_chosen"');
    exit();    
}

if( $_REQUEST['requisicao'] == 'carregaAtributo' ){
    
    $sql = "SELECT '<center><input type=\"checkbox\" class=\"check_attribute\" name=\"attribute[]\" id=\"attribute[]\" value=\"'||a.attname||'\" onclick=\"pegaAttribute()\"></center>', 
                a.attname as campo, pg_catalog.format_type(a.atttypid, a.atttypmod) as tipo_de_dado, case when attnotnull = true then 'TRUE' else 'FALSE' end AS NOTNULL
             FROM pg_namespace n, pg_class c, 
                  pg_attribute a, pg_type t
             WHERE n.oid = c.relnamespace
               and c.relkind = 'r'     -- no indices
               and n.nspname not like 'pg\\_%' -- no catalogs
               and n.nspname != 'information_schema' -- no information_schema
               and a.attnum > 0        -- no system att's
               and not a.attisdropped   -- no dropped columns
               and a.attrelid = c.oid
               and a.atttypid = t.oid 
            	and n.nspname = '{$_REQUEST['schemaname']}' AND c.relname = '{$_REQUEST['tablename']}'
            ORDER BY a.attnum, nspname, relname, attname";
    //echo $db->monta_combo("attname",$sql,'S',"Todas",'carregaAtributo','','','500','N','tablename', '', '', '', 'class="sel_chosen"');
    $cabecalho = array('', 'campo', 'tipo_de_dado', 'not null');
    $db->monta_lista($sql, $cabecalho, 2000000, 4, 'N','Center','','form');
    exit();    
}

$_POST['script'] = str_replace( "\'", "'", $_POST['script'] );
if( $_POST['carregacolunas'] ){
	$sql = "SELECT DISTINCT
                    pg_class.relname AS tabela,
                    pg_attribute.attname AS coluna
                FROM 
                    pg_class
                JOIN 
                    pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
                JOIN 
                    pg_attribute ON pg_attribute.attrelid = pg_class.oid AND pg_attribute.attisdropped = 'f'
                JOIN
                    pg_type ON pg_type.oid = pg_attribute.atttypid
                JOIN 
                    pg_index ON pg_index.indrelid=pg_class.oid
                LEFT JOIN
                    pg_constraint ON (pg_attribute.attrelid = pg_constraint.conrelid AND pg_constraint.conkey[1] = pg_attribute.attnum AND pg_constraint.contype != 'u')
                WHERE 
                    pg_namespace.nspname = 'emenda'
                AND 
                    pg_attribute.attnum > 0
                AND 
                    pg_attribute.attrelid = pg_class.oid
                AND 
                    pg_attribute.atttypid = pg_type.oid
               	AND
                    pg_class.relname = '".$_POST['tabela']."'
                ORDER BY
                    pg_class.relname,
                    pg_attribute.attname";
	$arDados = $db->carregar( $sql );
	foreach ($arDados as $v) {
		echo '<input type="checkbox" name="colunas[]" id="colunas[]" value="'.$v['coluna'].'">'.$v['coluna'];
	}
	
	die;	
}

$script = $_POST['script'];

monta_titulo( 'Administração do Banco de Dados Emenda', '');
?>
<script type="text/javascript" src="../../includes/funcoes.js"></script>
<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>

<link href="../../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">



<script src="../../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<script src="../../includes/libs_jquery/chosen.js/docsupport/prism.js" type="text/javascript"></script>
<link href="../../library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen" >

<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
<form id="formulario" method="post" action="">
<input type="hidden" name="action" id="action" value="" />
<style>
        a.chosen-single {
            -webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;
            /*background-color: #FFFFFF !important;*/
            /*border: 1px solid #AAAAAA !important;*/
            border-radius: 5px !important;
            box-shadow: 0 0 3px #FFFFFF inset, 0 1px 1px rgba(0, 0, 0, 0.1) !important;
            /*color: #444444 !important;*/
            display: block !important;
            height: 25px !important;
            line-height: 25px !important;
            overflow: hidden !important;
            padding: 0 0 0 8px !important;
            position: relative !important;
            /*text-align: center !important;*/
            text-decoration: none !important;
            white-space: nowrap !important;
        }

        .chosen-container-multi .chosen-choices li.search-field input[type="text"] {
            margin: 1px 0;
            padding: 5px !important;
            height: 25px !important;
            outline: 0 !important;
            border: 0 !important;
            background: transparent !important;
            box-shadow: none !important;
            color: #666 !important;
            font-size: 100% !important;
            font-family: sans-serif !important;
            line-height: normal !important;
            border-radius: 0 !important;
        }

        .chosen-container-active.chosen-with-drop .chosen-single {
            border: 1px solid #aaa!important;
            -moz-border-radius-bottomright: 0 !important;
            border-bottom-right-radius: 0 !important;
            -moz-border-radius-bottomleft: 0 !important;
            border-bottom-left-radius: 0 !important;
            background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(20%, #eeeeee), color-stop(80%, #ffffff))!important;
            background-image: -webkit-linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            background-image: -moz-linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            background-image: -o-linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            background-image: linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            box-shadow: 0 1px 0 #fff inset!important;
        }

        .cabecalho a.chosen-single{
            -webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;
            background-color: #FFFFFF !important;
            border: 1px solid #AAAAAA !important;
            border-radius: 5px !important;
            box-shadow: 0 0 3px #FFFFFF inset, 0 1px 1px rgba(0, 0, 0, 0.1) !important;
            color: #444444 !important;
            display: block !important;
            height: 30px !important;
            line-height: 29px !important;
            overflow: hidden !important;
            padding: 0 0 0 8px !important;
            position: relative !important;
            text-align: center !important;
            text-decoration: none !important;
            white-space: nowrap !important;
        }

    </style>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" style="width: 15%;"><b>schemaname:</b></td>
        <td><input type="hidden" name="nome_tabela" value="<?php echo $_REQUEST['tablename']; ?>">
<?php       $sql = "SELECT DISTINCT schemaname AS codigo, schemaname AS descricao FROM pg_catalog.pg_tables";
            echo $db->monta_combo("schemaname",$sql,'S',"Todas",'carregaTabelas','','','500','N','schemaname', '', $_REQUEST['schemaname'], '', 'class="sel_chosen"'); ?>
        </td>
	</tr>
	<tr style="display: <?php echo ($_REQUEST['schemaname'] ? '' : 'none');?>;" id="tr_tablename">
		<td class="SubTituloDireita" style="width: 15%;"><b>tablename:</b></td>
        <td id="td_tablename">
    <?php 
        if( $_REQUEST['schemaname'] ){
            $sql = "SELECT DISTINCT tablename AS codigo, tablename AS descricao FROM pg_catalog.pg_tables WHERE schemaname = '{$_REQUEST['schemaname']}' ORDER BY tablename ASC;";
            echo $db->monta_combo("tablename",$sql,'S',"Todas",'carregaAtributo','','','500','N','tablename', '', $_REQUEST['tablename'], '', 'class="sel_chosen"');
        }
    ?>
        </td>
	</tr>
	<tr style="display: none;" id="tr_sql">
		<td class="SubTituloDireita" style="width: 15%;"><b>Gerar script SQL:</b></td>
        <td>
			<input type="radio" value="S" id="gerar_sql" name="gerar_sql" onclick="gerarSQL('S');"/> <b>SELECT</b>
        	<input type="radio" value="I" id="gerar_sql" name="gerar_sql" onclick="gerarSQL('I');"/> <b>INSERT</b>
			<input type="radio" value="U" id="gerar_sql" name="gerar_sql" onclick="gerarSQL('U');"/> <b>UPDATEL</b>
			<input type="radio" value="D" id="gerar_sql" name="gerar_sql" onclick="gerarSQL('D');"/> <b>DELETE</b>
        </td>
	</tr>
	<tr style="display: none;" id="tr_attribute">
		<td class="SubTituloDireita" style="width: 15%;"><b>attribute:</b></td>
        <td id="td_attribute">
        </td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width: 15%;"><b>Tipo de Execução:</b></td>
        <td>
        	<input type="radio" value="E" id="tipoexecucao" name="tipoexecucao" <?php echo ( ($_REQUEST['tipoexecucao'] == 'E') ? 'checked' : '');?>/> EXECUTAR
			<input type="radio" value="C" id="tipoexecucao" name="tipoexecucao" <?php echo ( ($_REQUEST['tipoexecucao'] == 'C' || empty($_REQUEST['tipoexecucao']) ) ? 'checked' : '');?>/> CARREGAR
			<input type="radio" value="X" id="tipoexecucao" name="tipoexecucao" <?php echo ( ($_REQUEST['tipoexecucao'] == 'X') ? 'checked' : '');?>/> GERAR EXCEL
        </td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>SQL:</b></td>
        <td><?=campo_textarea('script', 'N', 'S', 'SQL', 200, 25, 0, '', '', '', '', 'SQL');?></td>
	</tr>
	<tr>
		<td colspan="2" style="text-align: center;"><input type="button" name="botao" value="Executar" onclick="submeterDados();"></td>
	</tr>
</table>
<div id="debug"></div>
<script type="text/javascript">

function gerarSQL( tipo ){
	var schemaname = jQuery('[name="schemaname"]').val();
	var tablename = jQuery('[name="tablename"]').val();
	var sql = '';
	if( tipo == 'S' ){
		var atributo = retornarAtributo('N', 7);
		sql = "SELECT "+atributo+"\nFROM "+schemaname+"."+tablename;
	}
	if( tipo == 'I' ){
		var atributo = retornarAtributo('N', 7);
		var atributoAspa = retornarAtributo('S', 7);
		sql = "INSERT INTO "+schemaname+"."+tablename+"("+atributo+") \nVALUES('"+atributoAspa+"')";
	}
	if( tipo == 'U' ){
		var chave = '';
		jQuery('[name="attribute[]"]').each(function(){			
			if( chave == '' ){
				chave = jQuery(this).val();
			}
		});		
		var atributo = retornarAtributoUpdate();
		sql = "UPDATE "+schemaname+"."+tablename+" SET\n"+atributo+"\nWHERE "+chave+" = 0";
	}
	if( tipo == 'D' ){
		var chave = '';
		jQuery('[name="attribute[]"]').each(function(){			
			if( chave == '' ){
				chave = jQuery(this).val();
			}
		});
		sql = "DELETE FROM "+schemaname+"."+tablename+" WHERE "+chave+" = 0";
	}
	jQuery('.check_attribute').attr('checked', false);
	jQuery('[name="script"]').val( sql );
}

function retornarAtributoUpdate(){
	var atributo = '';
	jQuery('[name="attribute[]"]').each(function(){
		//console.log( jQuery(this).val() );
		
		if( atributo == '' ){
			atributo = '\t'+jQuery(this).val();
		} else {
			atributo = atributo+" = '',\n\t"+jQuery(this).val();
		}
	});

	return atributo;
}

function retornarAtributo(aspa, qtd_quebra){
	var atributo = '';
	var cont = 0;
	var quebra = '';
	jQuery('[name="attribute[]"]').each(function(){
		//console.log( jQuery(this).val() );
		cont++;
		if( atributo == '' ){
			atributo = jQuery(this).val();
		} else {
			if( aspa == 'S' ) atributo = atributo+"',"+quebra+" '"+jQuery(this).val();
			else atributo = atributo+","+quebra+" "+jQuery(this).val();
			quebra = '';
		}
		
		if( cont == qtd_quebra ){
			quebra = '\n\t';
			cont = 0;
		}
	});

	return atributo;
}

function pegaAttribute(){
	var schemaname = jQuery('[name="schemaname"]').val();
	var tablename = jQuery('[name="tablename"]').val();

	var atributo = '';
	var sql = '';
	
	jQuery('[name="attribute[]"]:checked').each(function(){
		//console.log( jQuery(this).val() );
		if( atributo == '' ){
			atributo = jQuery(this).val();
		} else {
			atributo = atributo+', '+jQuery(this).val();
		}
	});
	if( atributo != '' ){
		sql = "SELECT "+atributo+" FROM "+schemaname+"."+tablename;
	} else {
		sql = "SELECT * FROM "+schemaname+"."+tablename;
	}
	
	jQuery('[name="script"]').val( sql );
}

jQuery(document).ready(function(){
	var tablename = jQuery('[name="tablename"]').val();
	var schemaname = jQuery('[name="schemaname"]').val();

	if( tablename != '' && schemaname != '' ){
		carregaAtributo( tablename );
	}
	
});

	function submeterDados(){
		if( jQuery('[name="tipoexecucao"]:checked').length == 0 ){
			alert('Selecione o Tipo de Execução!');
			return false;
		}
		document.getElementById('action').value = 'executar';
		document.getElementById('formulario').submit();
	}

	function carregaTabelas( schemaname ){
		var caminho = window.location.href;
        var action = '&requisicao=carregaTabelas&schemaname='+schemaname;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery('#td_tablename').html(resp);
                jQuery('#tr_tablename').show();
            }
        });
        var jq = jQuery.noConflict();
        jq('.sel_chosen').chosen({allow_single_deselect:true});
	}
	
	function carregaAtributo( tablename ){

		var schemaname = jQuery('[name="schemaname"]').val();
		
		var caminho = window.location.href;
        var action = '&requisicao=carregaAtributo&tablename='+tablename+'&schemaname='+schemaname;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery('#td_attribute').html(resp);
                jQuery('#tr_attribute').show();
                jQuery('#tr_sql').show();
            }
        });
        var jq = jQuery.noConflict();
        jq('.sel_chosen').chosen({allow_single_deselect:true});

        var schemaname = jQuery('[name="schemaname"]').val();

    	jQuery('[name="script"]').val( "SELECT * FROM "+schemaname+"."+tablename );
	}
</script>
</form>
<?

if( $_POST['action'] == 'executar' && in_array($_SESSION['usucpforigem'], $array_libera) ){
	$dataInicio = date("H:i:s");
	
	$sql = $_POST['script'];
	
	$posicao = strpos(trim($sql), ' ');
	$tipo = substr( trim($sql), 0, $posicao );

	if( $_POST['tipoexecucao'] == "E" ){
	    
		if( in_array(strtolower(trim($tipo)), array('delete', 'update', 'insert', 'create', 'alter', 'drop', 'comment', 'refresh', 'do', 'grant') ) ){
			executar( $sql );
			$db->commit();
			echo "<script>
					alert('Operação realizada com sucesso');
					window.location.href = window.location;
				</script>";
			die;
		} else {
			echo "<script>
					alert('Comando não válido!!!');
				</script>";
			die;
		}
	} else {
		//if( in_array(strtolower(trim($tipo)), array('select', 'with') ) ){
			if( $_POST['tipoexecucao'] == "X" ){
				ob_clean();
				header('content-type: text/html; charset=utf-8');
				
				$sql = str_replace( '\"', '"', $sql );				
				$sql = str_replace( "\'", "'", $sql );	
				
				$arrDados = $db->carregar($sql);
				$arrDados = $arrDados ? $arrDados : array();
				$cabecalho = array();
				if( $arrDados[0] ){
					foreach ($arrDados[0] as $cab => $val) {
						array_push($cabecalho, $cab);
					}
				}
				$db->sql_to_excel($arrDados, 'rel_dados', $cabecalho, $formato, true);
				exit;
			} else {
				$arrDados = $db->carregar($sql);
				$arrDados = $arrDados ? $arrDados : array();
				$cabecalho = array();
				if( $arrDados[0] ){
					foreach ($arrDados[0] as $cab => $val) {
						array_push($cabecalho, $cab);
					}
				}
				$db->monta_lista($arrDados, $cabecalho, 2000000, 4, 'N','Center','','form');
			}
		//}
	}
	$dataFim = date("H:i:s");
	$tempo = $db->pegaUm("select cast('".$dataFim."' as time) - cast('".$dataInicio."' as time)");
	?>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita" style="width: 15%;"><b>Tempo de Execução:</b></td>
	        <td><?php echo $tempo; ?></td>
		</tr>
	</table>
	<?php 
}

function executar($SQL)
{
	global $db;
	if (gettype( cls_banco::$link[$db->nome_bd] ) != "resource") {
		cls_banco::$link[$db->nome_bd] = null;
		cls_banco::cls_banco();
	}

	$SQL = trim($SQL);
	//detecta operacao e tabela (Insert, Update ou Delete)
	preg_match('/(CREATE\s+TABLE|ALTER\s+TABLE|DROP\s+TABLE|SELECT.*FROM|INSERT\s+INTO|UPDATE|DELETE\s+FROM)\s+([A-Za-z0-1.]+).*/smui', ($SQL), $matches);
	$audtipoCompleto = strtoupper($matches[1]);
	$audtipo         = substr($audtipoCompleto, 0, 1);

	$_SESSION['sql'] = $SQL;

	// Inicia a transação quando nao estiver iniciada e obrigatoriamente quando
	// a operação for diferente de SELECT
	if (!isset($_SESSION['transacao']) && $audtipo != 'S') {
		$db->resultado = pg_query(cls_banco::$link[$db->nome_bd], 'begin transaction; ');
		$_SESSION['transacao'] = '1';
	}

	$db->resultado = @pg_query(cls_banco::$link[$db->nome_bd], $SQL);

	if ($db->resultado == null)
		throw new Exception( $SQL . pg_errormessage( cls_banco::$link[$db->nome_bd] ) );

		return $db->resultado;
}
$db->close();
?>
<script lang="javascript">
	
jQuery('.sel_chosen').chosen({allow_single_deselect:true});

</script>
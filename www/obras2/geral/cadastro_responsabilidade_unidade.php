<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo:seleciona_unid_perfilresp.php
  
   */
include "config.inc";
header('content-type: text/html; charset=utf-8');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];
$orgid  = $_REQUEST['orgid'];
$estuf  = $_REQUEST['estuf'];
$muncod = $_REQUEST['muncod'];
$gravar = $_REQUEST['gravar'];
$unicod = $_REQUEST["uniresp"];

$perfilSuperUser = $db->testa_superuser(); //testa se o usuário é super usuário

if ( !$perfilSuperUser && !$orgid ){
	$sql = "SELECT
			    oo.orgid
			FROM
				obras2.orgao oo
			INNER JOIN obras2.usuarioresponsabilidade ur ON ur.orgid = oo.orgid
			WHERE
				orgstatus = 'A' AND				
				rpustatus = 'A' AND
				ur.estuf IS NULL AND
				ur.entid IS NULL AND
				ur.usucpf = '{$_SESSION["usucpf"]}'";
	$orgid = $db->pegaUm($sql);
	
	if(!$orgid){
		$sql = "SELECT entid FROM obras2.usuarioresponsabilidade WHERE usucpf = '{$_SESSION["usucpf"]}';";
		$entid = $db->pegaUm($sql);
		if($entid){
			$sql = "SELECT orgid FROM obras2.obrainfraestrutura WHERE entidunidade = $entid limit 1";
			$orgid = $db->pegaUm($sql);
		}
	}
	
	if (!$orgid){
		die('<script type="text/javascript">
				alert(\'Seu perfil não permite liberar acesso ao sistema!\');
				window.close();
			 </script>');	
	}
}

if ($orgid == 1){
	$funid = '12';	
}elseif ($orgid == 2){
	$funid = '11,14';	
} elseif($orgid == 5) {
	$funid = '16';
}elseif ($muncod){
	$funid = '1,3,7';
}else{
	$funid = '1,6,43,3,44,42';
}

if ($_POST && $gravar == 1){
	atribuiUnidade($usucpf, $pflcod, $unicod, $orgid);
}

function recuperaOrgao ($orgid = null){
	global $db;
	
	if ($db->testa_superuser()){

		$sql = "SELECT
					orgdesc 
				FROM
					obras2.orgao
				WHERE
					orgstatus = 'A' AND
					orgid = {$orgid}";
		
	}else{
		
		$sql = "SELECT
					orgdesc 
				FROM
					obras2.orgao oo
				INNER JOIN obras2.usuarioresponsabilidade ur ON ur.orgid = oo.orgid AND
																ur.rpustatus = 'A' AND
																ur.usucpf = '{$_SESSION["usucpf"]}'		
				WHERE
					oo.orgstatus = 'A'";
		
	}
	
	return $dsc = $db->pegaUm($sql);
}

/**
 * Função que lista as unidades
 *
 */
function listaUnidades(){
	global $db, $funid, $estuf, $muncod, $orgid, $entid;
		
	$where  = array();
	$campo  = array();
	$from   = array();
	 
	if ($db->testa_superuser() && !$orgid){
		echo "<tr>
				<td style='color:red;'>Faça sua busca...</td>
			  </tr>";
		return;
	}
	if ($orgid == 3 && !$estuf){
		echo "<tr>
				<td style='color:red;'>Selecione uma unidade federativa para continuar a busca...</td>
			  </tr>";
		return;
		
	}
	
	if ($orgid == 3):
		
		###### Monta "from" filtro de Estado e Município para o SQL ######
		if( $estuf && $muncod ){
			$campo[] = 'ed.estuf, m.mundescricao, ';			
		} elseif( $estuf || $muncod ) {
			$campo[] = 'ed.estuf,';			
		}
		 
		###### Monta filtro de Estado para o SQL ######
		if ($estuf)
			$where[] = "ed.estuf IN('{$estuf}')"; 
	
		###### Monta filtro de Município para o  SQL ######
		if ($muncod)
			$where[] = "ed.muncod IN ('{$muncod}')";
		$where[] = "em.orgid = 3";
	endif;	
	
	// SQL para buscar unidades existentes
	if ($db->testa_superuser()){
		$sql = "SELECT DISTINCT
					e.entid,
					" . implode("", $campo) . "
					e.entnome as entnome,
					e.entcodent
				FROM 
					entidade.entidade e
				LEFT  JOIN obras2.empreendimento em ON em.entidunidade = e.entid AND 
													   em.empstatus = 'A'
				LEFT  JOIN entidade.endereco 	   ed ON ed.endid = em.endid 
			    LEFT  JOIN territorios.municipio   m ON m.muncod = ed.muncod
				INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
				WHERE
					entstatus='A' AND
					funid IN ('".str_replace(",","','",$funid)."')
					".($where ? " AND ".implode(" AND ", $where) : '')."
				ORDER BY 
					entnome";
		
		$unidadesExistentes = $db->carregar($sql);
		
	} elseif(!$db->testa_superuser() && $entid) {
		$unidadesExistentes = $db->carregar("SELECT DISTINCT
												e.entid,
												" . implode("", $campo) . "
												e.entnome,
												e.entcodent
											FROM
												obras2.empreendimento em
											INNER JOIN entidade.entidade e on e.entid = em.entidunidade AND
																			  e.entid = '$entid'
											LEFT JOIN entidade.endereco ed ON ed.endid = em.endid 
											LEFT JOIN territorios.municipio m ON m.muncod = ed.muncod
											WHERE
												em.empstatus = 'A'
											ORDER BY
												2;");
	} else {
		if( $orgid != 5 ) $filtroUser = " and ur.usucpf = '{$_SESSION["usucpf"]}'";
		
		$sql = "SELECT DISTINCT
					e.entid,
					" . implode("", $campo) . "
					e.entnome,
					e.entcodent
				FROM 
					entidade.entidade e
				LEFT JOIN obras2.empreendimento em ON em.entidunidade = e.entid AND 
													  em.orgid = {$orgid} 
				" . implode("",$from) . "
				INNER JOIN entidade.funcaoentidade 			ef ON ef.entid = e.entid
				LEFT JOIN obras2.orgaofuncao 				og ON og.funid = ef.funid
				LEFT JOIN obras2.usuarioresponsabilidade 	ur ON ur.orgid = og.orgid AND 
																  ur.rpustatus = 'A'
				LEFT JOIN entidade.endereco 				ed ON ed.endid = em.endid 
				LEFT JOIN territorios.municipio 			 m ON m.muncod = ed.muncod
				WHERE
					entstatus='A' AND
					ef.funid IN ('".str_replace(",","','",$funid)."') 
					$filtroUser
					".($where ? " AND ".implode(" AND ", $where) : '')."
				ORDER BY 
					entnome";
		$unidadesExistentes = $db->carregar($sql);

	}
	if (!$unidadesExistentes){
		echo "<tr>
				<td style='color:red;'>Busca não retornou registros...</td>
			  </tr>";
		return;
	}	
	
	$count = count($unidadesExistentes);

	$orgdsc = recuperaOrgao($orgid);
	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $unidadesExistentes[$i]["entid"];
		$descricao = $unidadesExistentes[$i]["entnome"];
		$municipio = $unidadesExistentes[$i]["mundescricao"];
		$codigoUf  = $unidadesExistentes[$i]["estuf"];
		$codinep   = $unidadesExistentes[$i]["entcodent"];
		//dbg($codinep, 1);
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		$html.= "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input class=\"chkbox\" type=\"checkbox\" name=\"unicod\" id=\"".$codigo."\" value=\"$orgid|$codigo\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"unidsc\" value=\"".($funid ==1 ? $orgdsc . " - " . $descricao . " - " . $municipio . " - " .  $codigoUf : $descricao . " - " . $orgdsc)."\">
				</td>
				<td>
					".( ( $orgid == 3 && !empty($codinep) ) ? $codinep . " - " . $descricao : $descricao )."
				</td>
			</tr>";
	}
	$html.= '<thead>
				<tr>
					<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Total de Registros: '.sizeof($unidadesExistentes).'</strong></td>		
				</tr>
			</thead>';
	echo $html;
}

/**
 * Função que atribui a responsabilidade de uma unidade ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $unicod
 */
function atribuiUnidade($usucpf, $pflcod, $entid, $orgid = 'null'){
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	$sql = "UPDATE obras2.usuarioresponsabilidade SET 
				rpustatus = 'I' 
		   	WHERE 
				usucpf = '{$usucpf}' AND 
				pflcod = '{$pflcod}' AND 
				entid IS NOT NULL";
	$sql_zera = $db->executar($sql);
	
	if (is_array($entid) && !empty($entid[0])){
		$count = count($entid);
		
		// Insere a nova unidade
		$sql_insert = "INSERT INTO obras2.usuarioresponsabilidade (
							entid, 
							usucpf, 
							rpustatus, 
							rpudata_inc, 
							pflcod,
							orgid -- , 
							-- prsano
					   )VALUES";
		
		for ($i = 0; $i < $count; $i++){
			
			list($orgid,$entidade) = explode("|", $entid[$i]);
			
			if ( $entidade != $entidade_antiga ){
				$arrSql[] = "(
								'{$entidade}',
								'{$usucpf}', 
								'A', 
								'{$data}', 
								'{$pflcod}',
								'{$orgid}' -- , 
							--	'{$_SESSION["exercicio"]}'
							 )";
			}
			
			$entidade_antiga = $entidade;
			
		}

		$sql_insert = (string) $sql_insert.implode(",",$arrSql);
		$db->executar($sql_insert);
	}
	$db->commit();
	die("<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.href = window.opener.location;
			self.close();
		 </script>");
	
}

function buscaUnidadesCadastradas($usucpf, $pflcod){
	
	global $db, $unicod;
	
	if (!$_POST['gravar'] && $_REQUEST["uniresp"][0]){
		foreach ($_REQUEST["uniresp"] as $v){
			list(,$entid[]) = explode('|', $v );
		}
		$where = " e.entid IN (".implode(',',$entid).") AND ef.funid in (1, 3, 6, 7, 11, 12, 14, 16, 34, 43, 42, 44)";
	}else{
		$where = " (ur.usucpf = '{$usucpf}' AND 
			 	    ur.pflcod = {$pflcod})  AND ef.funid in (1, 3, 6, 7, 11, 12, 14, 16, 34, 43, 42, 44)";	
	}
	
	$perfilSuperUser = $db->testa_superuser(); //testa se o usuário é super usuário
	
	if ( !$perfilSuperUser ){
		$sql = "SELECT
				    oo.orgid
				FROM
					obras2.orgao oo
				INNER JOIN obras2.usuarioresponsabilidade ur ON ur.orgid = oo.orgid
				WHERE
					orgstatus = 'A' AND				
					rpustatus = 'A' AND
					ur.estuf IS NULL AND
					ur.entid IS NULL AND
					ur.usucpf = '{$_SESSION["usucpf"]}'";
		$orgid = $db->pegaUm($sql);
		
		if(!$orgid){
			$sql = "SELECT entid FROM obras2.usuarioresponsabilidade WHERE usucpf = '{$_SESSION["usucpf"]}';";
			$entid = $db->pegaUm($sql);
			if($entid){
				$where = " e.entid = $entid AND ef.funid in (1, 3, 6, 7, 11, 12, 14, 16, 34, 43, 42, 44)";
			}
		}
	}

	$sql = "SELECT DISTINCT 
				e.entid as codigo, 
				CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
					e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf ELSE 
					e.entnome END as descricao,
				ef.funid 
			FROM 
		    	entidade.entidade e
		    INNER JOIN entidade.funcaoentidade 		 ef ON ef.entid = e.entid
		    LEFT  JOIN entidade.endereco 			 ed ON ed.entid = e.entid
			LEFT  JOIN territorios.municipio 		  m ON m.muncod = ed.muncod
		    LEFT  JOIN obras2.usuarioresponsabilidade ur ON e.entid = ur.entid AND ur.rpustatus = 'A'
			WHERE 
			 ".$where;
	
	$RS = @$db->carregar($sql);
	
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			$arDescricao = array();
			for ($i=0; $i<=$nlinhas;$i++) {
				
				foreach($RS[$i] as $k=>$v){ 
					${$k}=$v;
				}
				if ($funid == 12){
					$orgid = 1;	
				}elseif($funid == 11 || $funid == 14 ){
					$orgid = 2;
				}elseif($funid == 16){
					$orgid = 5;
				}else{
					$orgid = 3;
				}
				
				
				$orgdsc[$funid] = $orgdsc[$funid] ? $orgdsc[$funid] : recuperaOrgao($orgid);  
				if ( in_array("{$orgdsc[$funid]} - {$descricao}", $arDescricao) ){
					continue;
				}
				$arDescricao[]  = "{$orgdsc[$funid]} - {$descricao}";
				
				
	    		print " <option value=\"$orgid|$codigo\">{$orgdsc[$funid]} - $descricao</option>";
	    		
			}
		}
	} else{
		print '<option value="">Clique faça o filtro para selecionar.</option>';
		
	}
}

?><html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Unidades</title>
                <script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.js"></script>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		
		<form name="formulario" action="<?=$_SERVER['REQUEST_URI'] ?>" method="post">
		<table style="width:100%; display:none;" id="filtro" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="subtitulodireita">Tipo de Ensino:</td>
				<td>
				<?php
					if ($db->testa_superuser()){

						$sql = "SELECT
									orgid AS codigo, 
									orgdesc AS descricao
								FROM
									obras2.orgao
								WHERE
									orgstatus = 'A'
									and orgid <> 4";

						$db->monta_combo('orgid',$sql,'S',"-- Selecione para filtrar --",'filtroFunid','');
					}elseif($entid && $orgid){
						
						$sql = "SELECT DISTINCT
									oo.orgid AS codigo, 
									oo.orgdesc AS descricao
								FROM
									obras2.orgao oo
								INNER JOIN obras2.empreendimento em ON em.orgid = oo.orgid AND
																	   em.entidunidade = $entid
								WHERE
									oo.orgstatus = 'A' AND
									oo.orgid <> 4
								LIMIT
									1";
						$db->monta_combo('orgid',$sql,'S',"",'filtroFunid','');
							
					}else{
						
						$sql = "SELECT distinct
									oo.orgid AS codigo, 
									oo.orgdesc AS descricao
								FROM
									obras2.orgao oo
								INNER JOIN obras2.usuarioresponsabilidade ur ON ur.orgid = oo.orgid
								WHERE
									orgstatus = 'A' AND
									rpustatus = 'A' AND
									usucpf = '{$_SESSION["usucpf"]}' AND 
									oo.orgid <> 4";

						$db->monta_combo('orgid',$sql,'S',"",'filtroFunid','');
						
					}
				
					echo '&nbsp;<img src="/imagens/obrig.gif" title="Indica campo obrigatório">';					
				?>
				</td>
			</tr>
<? if ($orgid == 3): ?>			
			<tr>
				<td class="subtitulodireita">Unidade Federativa:</td>
				<td>
				<?php
				$sql = "SELECT
							 estuf AS codigo,
							 estuf || ' - ' || estdescricao AS descricao
						FROM
						 	territorios.estado
						ORDER BY
						 	estuf";
				
				$db->monta_combo('estuf',$sql,'S',"-- Selecione para filtrar --",'limpaMuncod(); filtroFunid','');
				echo '&nbsp;<img src="/imagens/obrig.gif" title="Indica campo obrigatório">';					
				?>
				</td>
			</tr>
			<? if ($estuf): ?>			
			<tr>
				<td class="subtitulodireita">Município:</td>
				<td>
				<?php
				$sql = "SELECT
						 	muncod AS codigo,
						 	mundescricao AS descricao
						FROM
						 	territorios.municipio
						WHERE
						 	estuf = '{$estuf}'
						ORDER BY
						 	mundescricao";
				
				$db->monta_combo('muncod',$sql,'S',"-- Selecione para filtrar --",'filtroFunid','');
				?>
				</td>
			</tr>	
			<? endif; ?>
<? endif; ?>								
		</table>		
		<!-- Lista de Unidades -->
		<div id="tabela" style="overflow:auto; width:496px; height:270px; border:2px solid #ececec; background-color: #ffffff;">	
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<script language="JavaScript">
						//document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td class="" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; width: 10px; padding-left: 26px;" colspan="1"><input id="checkPai" type="checkbox"</td>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="2"><strong>Selecione a(s) Unidade(s)</strong></td>		
						</tr>
					</thead>
					<?php listaUnidades(); ?>
				</table>
		</div>
		<script language="JavaScript">
			document.getElementById('filtro').style.display = 'block';
		</script>
		<!-- Unidades Selecionadas -->
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="uniresp[]" id="uniresp" style="width:500px;" onkeydown="javascript:combo_popup_remove_selecionados( event, 'uniresp' );" class="CampoEstilo" onchange="//moveto(this);">				
				<?php 
					buscaUnidadesCadastradas($usucpf, $pflcod);
				?>
			</select>
		<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect); document.getElementsByName('gravar')[0].value=1; document.formulario.submit();" id="ok">
					<input type="hidden" name="gravar" value="">
				</td>
			</tr>
		</table>
</form>
<script type="text/JavaScript">

document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.display  = 'block';

var campoSelect = document.getElementById("uniresp");

<?
if ($funid):
?>
if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++){
		var id = campoSelect.options[i].value.split('|');
		
		if (document.getElementById(id[1])){
			document.getElementById(id[1]).checked = true;
		}
	}
}
<?
endif;
?>


function abreconteudo(objeto)
{
if (document.getElementById('img'+objeto).name=='+')
	{
	document.getElementById('img'+objeto).name='-';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
	document.getElementById(objeto).style.visibility = "visible";
	document.getElementById(objeto).style.display  = "";
	}
	else
	{
	document.getElementById('img'+objeto).name='+';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
	document.getElementById(objeto).style.visibility = "hidden";
	document.getElementById(objeto).style.display  = "none";
	}
}



function retorna(objeto)
{

	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
		if(document.formulario.unicod[objeto]) {
		if (document.formulario.unicod[objeto].checked == true){
			campoSelect.options[tamanho] = new Option(document.formulario.unidsc[objeto].value, document.formulario.unicod[objeto].value, false, false);
			sortSelect(campoSelect);
		}
		else {
			for(var i=0; i<=campoSelect.length-1; i++){
				if (document.formulario.unicod[objeto].value == campoSelect.options[i].value)
					{campoSelect.options[i] = null;}
				}
				if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Unidade.', '', false, false);}
				sortSelect(campoSelect);
		}
	} else {
		// qunado possui apenas 1 registro
		if (document.formulario.unicod.checked == true){
			campoSelect.options[tamanho] = new Option(document.formulario.unidsc.value, document.formulario.unicod.value, false, false);
			sortSelect(campoSelect);
		}
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}

function filtroFunid (id) {

	var d 	   = document;
	var orgid  = d.getElementsByName('orgid')[0]  ? d.getElementsByName('orgid')[0].value : '';
	var estuf  = d.getElementsByName('estuf')[0]  ? d.getElementsByName('estuf')[0].value : '';;
	var muncod = d.getElementsByName('muncod')[0] ? d.getElementsByName('muncod')[0].value : '';

	if (!orgid){
		alert('Selecione um "tipo de ensino" afim de efetuar o filtro!');
		return false;
	}
	
	selectAllOptions(campoSelect);
	d.formulario.submit();
}

function limpaMuncod(){
	if (document.getElementsByName('muncod')[0]) {
		document.getElementsByName('muncod')[0].value='';
	}
}

$(function(){
    $('#checkPai').change(function(){
        $('.chkbox').each(function(i){
            $(this).attr('checked', $('#checkPai').attr('checked'))
        })
    });
})
</script>
	</body>
</html>
<?php
include "config.inc";
header('content-type: text/html; charset=utf-8');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Visao.class.inc";
include_once APPRAIZ . "includes/library/simec/Listagem.php";
include_once '../autoload.php';

$db = new cls_banco();

include APPRAIZ."includes/funcoes_espelhoperfil.php";

if ($_POST['ajax'] == 'par3') {
    $dados = verificaResponsabilidadeUnicas($_POST);
    header('Content-type: application/json');
    echo simec_json_encode(['usu' => ($dados) ? $dados : 'false']);
    die;
}


$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/
if( $_POST['requisicao'] == 'regraperfilpar' ){
	if( $_REQUEST['pflcod'] == '1350'){
		$GLOBALS['nome_bd'] = 'simec_desenvolvimento';
		$GLOBALS['servidor_bd'] = '192.168.222.45';
		$GLOBALS['porta_bd'] = '5433';
		$GLOBALS['usuario_db'] = 'simec';
		$GLOBALS['senha_bd'] = 'phpsimecao';
		$arAux = array();
		$obModelUsuarioResp = new Par3_Model_UsuarioResponsabilidade();
		$CPFRegraSecretario = $obModelUsuarioResp->recuperarCPFUsuarioResponsabilidade( $_REQUEST['pflcod'], '', $_REQUEST['estuf'] );
		if( $CPFRegraSecretario){
			$sql = "SELECT usunome FROM seguranca.usuario WHERE usucpf = '$CPFRegraSecretario'";
			$pessoa = $db->pegaUm($sql);
			$arAux['boValidaRegraSecretario'] = "1";
			$arAux['msgRegraSecretario'] = "Atenção! Já existe um perfil de Secretário atribuido para este estado. Favor remover o atual ($pessoa) antes de cadastrar um novo Secretário para este estado.";
		}else{
			$arAux['boValidaRegraSecretario'] = "0";
		}
	}else{
		$arAux['boValidaRegraSecretario'] = "0";
	}
	echo simec_json_encode($arAux);
	die;
}

function verificaResponsabilidadeUnicas(array $arrParams) {
    global $db;

    $perfis = Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL.','.
        Par3_Model_UsuarioResponsabilidade::PREFEITO.','.
        Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL;

    $strSQL = "
        select
            responsabilidade.*, usu.usunome, perfil.pfldsc
        from par3.usuarioresponsabilidade responsabilidade
        join seguranca.usuario usu on (usu.usucpf = responsabilidade.usucpf)
        join seguranca.perfil perfil on (perfil.pflcod = responsabilidade.pflcod)
        where responsabilidade.pflcod = %d AND
        responsabilidade.usucpf <> '%s' AND
        responsabilidade.pflcod in (%s) AND
    ";

    $strSQL .= (strlen($arrParams['muncod']) > 2) ? " responsabilidade.muncod = '%s'" : " responsabilidade.estuf = '%s'";
    $strSQL .= " order by 1 desc limit 1";

    $stmt = sprintf($strSQL, $arrParams['pflcod'], $arrParams['usucpf'], $perfis, $arrParams['muncod']);
    //ver($stmt, $arrParams, d);

    $rs = $db->pegaLinha($stmt);
    return ($rs) ? $rs : false;
}

if(is_array($_POST['usuunidresp'])) {

    if ($_REQUEST['exclude']) {
        $strSQL = "
            UPDATE par3.usuarioresponsabilidade
                SET rpustatus = 'I'
            WHERE estuf = '%s' AND usucpf = '%s' AND pflcod = %d
        ";
        foreach ($_REQUEST['exclude'] as $paramMunicipio) {
            foreach ($paramMunicipio as $linha) {
                $params = explode(',', $linha);
                $stmt = sprintf($strSQL, $params[0], $params[1], $params[2]);
                $db->executar($stmt);
            }
        }
    }

	$sql = "update
			 par3.usuarioresponsabilidade 
			set
			 rpustatus = 'I' 
			where
			 usucpf = '$usucpf'  
			 and pflcod = $pflcod ";
	$db->executar($sql);
	
	if($_POST['usuunidresp'][0]){
		foreach($_POST['usuunidresp'] as $estuf){
			
			$at_entid = "SELECT MAX(ent.entid) FROM entidade.entidade ent 
						 INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
						 WHERE fen.funid=6 AND entnumcpfcnpj IN(SELECT entcnpj 
			FROM par3.instrumentounidade_entidade ie INNER JOIN par3.instrumentounidade iu ON iu.inuid = ie.inuid WHERE estuf='$estuf' AND iu.itrid=1)";
			
			
			$sql = "INSERT INTO par3.usuarioresponsabilidade (estuf, usucpf, rpustatus, rpudata_inc, pflcod, entid) 
																   VALUES ('$estuf', '$usucpf', 'A',  now(), '$pflcod', ($at_entid))";
			if( $_REQUEST['pflcod'] == '1350' ){
				$GLOBALS['nome_bd'] = 'simec_desenvolvimento';
				$GLOBALS['servidor_bd'] = '192.168.222.45';
				$GLOBALS['porta_bd'] = '5433';
				$GLOBALS['usuario_db'] = 'simec';
				$GLOBALS['senha_bd'] = 'phpsimecao';
				$obModelUsuarioResp = new Par3_Model_UsuarioResponsabilidade();
				$boValidaRegraSecretario = $obModelUsuarioResp->validarUsuarioResponsabilidade('', $pflcod, $muncod );
				
				if( !$boValidaRegraPrefeito ){
					$db->executar($sql);
				}else{
					$db->executar($sql);
				}
			}else{
				$db->executar($sql);
			}
		}		
	}
	
	atualizarResponsabilidadesSlaves($usucpf,$pflcod);
	
	$db->commit();
?>
	<script>
	window.parent.opener.location.reload();self.close();
	</script>
<?
	exit();
}

/*
*** FIM REGISTRO RESPONSABILIDADES ***
*/
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Instituição</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?
//flush();
?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<form name="formulario" id="formulario">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o(s) estado(s)</strong></td>
</tr>
<tr>
<?
	  $cabecalho = 'Selecione o(s) estado(s)';
	  $sql = "select
				estuf, estdescricao
			from territorios.estado
			order by estuf, estdescricao";
	  
	  $RS = @$db->carregar($sql);
	  $nlinhas = count($RS)-1;
	  for ($i=0; $i<=$nlinhas;$i++)
		 {
			extract($RS[$i]);
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
	   ?>
	   		
		   		<tr bgcolor="<?=$cor?>"">
                    <td align="right" width="5px">
                        <input
                            class="checkProfile"
                            type="Checkbox"
                            name="estuf"
                            id="<?=$estuf?>"
                            value="<?=$estuf?>"
                            onclick="retorna(<?=$i?>); verificaRegraPerfilPar( '<?=$estuf?>', '<?php echo $_REQUEST['pflcod'];?>' );">
                        <input type="Hidden" name="estdescricao" value="<?=$estuf.' - '.$estdescricao?>">
                    </td>
                    <td align="right" width="10px" style="color:blue;"><?=$estuf?></td>
                    <td><?=$estdescricao?></td>
				</tr>
	   
	   <?}
?>
</table>
</form>
</div>
<form name="formassocia" id="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="usuunidresp[]" id="usuunidresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?
$sql = "select distinct u.estuf as codigo, u.estuf||' - '||u.estdescricao as descricao from par.usuarioresponsabilidade ur inner join territorios.estado u on ur.estuf=u.estuf where ur.rpustatus='A' and ur.usucpf = '$usucpf' and ur.pflcod=$pflcod";
$RS = @$db->carregar($sql);
if(is_array($RS)) {
	$nlinhas = count($RS)-1;
	if ($nlinhas>=0) {
		for ($i=0; $i<=$nlinhas;$i++) {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    		echo " <option value=\"$codigo\">$codigo - $descricao</option>";
		}
	}
} else {?>
<option value="">Clique no estado.</option>
<?
}
?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
</td></tr>
</table>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";

var campoSelect = document.getElementById("usuunidresp");

if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++){
		document.getElementById(campoSelect.options[i].value).checked = true;
	}
}

function verificaRegraPerfilPar(estuf, perfil){

	$.ajax({
		type: "POST",
		url: window.location,
		data: "requisicao=regraperfilpar&estuf="+estuf+"&perfil="+perfil,
		dataType: 'json',
		success: function(response){
			if(response.boValidaRegraSecretario== '0'){
				return true;
			}else{
				if( response.boValidaRegraSecretario == '1' ){
					alert(response.msgRegraSecretario);

					var objEstado = document.getElementById(estuf);
					objEstado.checked = false;
					return false;
				}
			}
				
		}
	});
	
}

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
	if (document.formulario.estuf[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.estdescricao[objeto].value, document.formulario.estuf[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.estuf[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no estado.', '', false, false);}
			sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}

$(function(){
    $(".checkProfile").change(function(){
        var $element = $(this);
        if ($element.attr("checked") == true) {
            var params = {
                muncod: $element.val(),
                pflcod: <?=$_GET['pflcod'];?>,
                usucpf: <?=$_GET['usucpf'];?>,
                ajax: 'par3'
            }

            if (params) {
                $.ajax({
                    url:location.href,
                    type:'post',
                    data:params,
                    success: function(data) {
                        if (data.usu == 'false') return;

                        var messageConfirm = "O usuário: '"+data.usu.usunome+"' já possuí o perfil de "+data.usu.pfldsc+" " +
                            "para o estado selecionado, Dejesa subsituir o perfil?";

                        if (confirm(messageConfirm)) {
                            $("#formassocia").append("<input type='hidden' name='exclude["+$element.val()+"][]' value='"+$element.val()+","+data.usu.usucpf+","+data.usu.pflcod+"' />");
                            $element.attr("checked", true);
                        } else {
                            $element.attr("checked", false);

                            for (var i = 0; i < campoSelect.options.length; i++) {
                                if (campoSelect.options[i].value == params.muncod) {
                                    campoSelect.options[i] = null;
                                }
                            }
                        }
                    }
                });
            }
        } else {
            $("#formassocia").find("[name='exclude["+$element.val()+"][]']").remove();
        }
    });
});
</script>
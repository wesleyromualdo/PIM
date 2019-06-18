<?
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

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Visao.class.inc";
include_once APPRAIZ . "par3/classes/model/InstrumentoUnidadeEntidade.class.inc";
include_once APPRAIZ . "par3/classes/model/UsuarioResponsabilidade.class.inc";
include_once APPRAIZ . "includes/library/simec/Listagem.php";
include_once '../autoload.php';

$db = new cls_banco();

include APPRAIZ."includes/funcoes_espelhoperfil.php";

$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

$arrPerfilUnico = array(
    Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL,
    Par3_Model_UsuarioResponsabilidade::PREFEITO,
    Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
);

$tipo = 'checkbox';
if(in_array($pflcod, $arrPerfilUnico)){
    $tipo = 'radio';
}

if ($_POST['ajax'] == 'par3') {
    $dados = verificaResponsabilidadeUnicas($_POST);
    header('Content-type: application/json');
    echo simec_json_encode(['usu' => ($dados) ? $dados : 'false']);
    die;
}

if ( $_POST['requisicao'] == 'municipio' ) {
	$estuf = $_POST['estado'];
	$sql = "select muncod||'#'||mundescricao from territorios.municipio where estuf = '$estuf' order by mundescricao";
	$municipios = $db->carregarColuna($sql);
	$muncod = implode('|', $municipios);
	echo $muncod;
	exit();
}

if ($_POST['requisicao'] == 'regraperfilpar') {
    if ($_REQUEST['pflcod'] == '1349') {
        $GLOBALS['nome_bd'] = 'simec_desenvolvimento';
        $GLOBALS['servidor_bd'] = '192.168.222.45';
        $GLOBALS['porta_bd'] = '5433';
        $GLOBALS['usuario_db'] = 'simec';
        $GLOBALS['senha_bd'] = 'phpsimecao';
        $arAux = array();

        $obModelUsuarioResp = new Par3_Model_UsuarioResponsabilidade();
        $CPFRegraPrefeito = $obModelUsuarioResp->recuperarCPFUsuarioResponsabilidade($_REQUEST['pflcod'], $_REQUEST['muncod']);

        if ($CPFRegraPrefeito) {
            $sql = "SELECT usunome FROM seguranca.usuario WHERE usucpf = '$CPFRegraPrefeito'";
            $pessoa = $db->pegaUm($sql);
            $arAux['boValidaRegraPrefeito'] = "1";
            $arAux['msgRegraPrefeito'] = "Atenção! Já existe um perfil de prefeito atribuido para este município. Favor remover o atual ($pessoa) antes de cadastrar um novo prefeito para este município.";
        } else {
            $arAux['boValidaRegraPrefeito'] = "0";
        }

    } else {
        $arAux['boValidaRegraPrefeito'] = "0";
    }
    echo simec_json_encode($arAux);
    die;
}
/*
 *** INICIO REGISTRO RESPONSABILIDADES ***
 */

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
        JOIN seguranca.perfilusuario perfilusu ON (perfilusu.usucpf = usu.usucpf AND responsabilidade.pflcod = perfilusu.pflcod)
        where responsabilidade.pflcod = %d AND
        rpustatus = 'A' AND
        responsabilidade.usucpf <> '%s' AND
        responsabilidade.pflcod in (%s) AND
    ";

    $strSQL .= (strlen($arrParams['muncod']) > 2) ? " responsabilidade.muncod = '%s'" : " responsabilidade.estuf = '%s'";
    $strSQL .= " order by 1 desc limit 1";

    $stmt = sprintf($strSQL, $arrParams['pflcod'], $arrParams['usucpf'], $perfis, $arrParams['muncod']);

    $rs = $db->pegaLinha($stmt);
    return ($rs) ? $rs : false;
}

if (isset($_REQUEST['enviar'])) {

    $sql = "UPDATE par3.usuarioresponsabilidade SET rpustatus = 'I'
            WHERE usucpf = '{$usucpf}' and pflcod = {$pflcod};";
    $db->executar($sql);
    $db->commit();

    if( $_POST['usuunidresp'][0] ){
        if($tipo == 'radio'){
            $sql = "SELECT DISTINCT
                        e.entid as codigo
                    FROM
                        entidade.entidade e
                    INNER JOIN entidade.funcaoentidade 		ef ON ef.entid = e.entid
                    LEFT  JOIN entidade.endereco 			ed ON ed.entid = e.entid
                    LEFT  JOIN territorios.municipio 		m  ON m.muncod = ed.muncod
                    WHERE
                        ef.funid in (1)
                        AND e.entstatus = 'A'
                        AND m.muncod = '{$_POST['usuunidresp'][0]}'";
            $entid = $db->pegaUm($sql);

            $sql = "INSERT INTO par3.usuarioresponsabilidade(muncod, usucpf, rpustatus, rpudata_inc, pflcod, entid)
                    VALUES('{$_POST['usuunidresp'][0]}', '{$usucpf}', 'A',  now(), '{$pflcod}', $entid);";
            $db->executar($sql);
            $db->commit();
        }else{
            foreach( $_POST['usuunidresp'] as $muncod ){

                $sql = "SELECT DISTINCT
                            e.entid as codigo
                        FROM
                        	entidade.entidade e
                        INNER JOIN entidade.funcaoentidade 		ef ON ef.entid = e.entid
                        LEFT  JOIN entidade.endereco 			ed ON ed.entid = e.entid
                        LEFT  JOIN territorios.municipio 		m  ON m.muncod = ed.muncod
                        WHERE
                        	ef.funid in (1)
                        	AND e.entstatus = 'A'
                        	AND m.muncod = '$muncod'";
                $entid = $db->pegaUm($sql);

                $sql = "INSERT INTO par3.usuarioresponsabilidade(muncod, usucpf, rpustatus, rpudata_inc, pflcod, entid)
                        VALUES('{$muncod}', '{$usucpf}', 'A',  now(), '{$pflcod}', $entid);";
                $db->executar($sql);
                $db->commit();
            }
        }
    }
    atualizarResponsabilidadesSlaves($usucpf, $pflcod);

    $db->commit();
?>
<script>
	window.parent.opener.location.reload(); self.close();
</script>
<?php
	exit(0);
}
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Estados e Municípios</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">

<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
    <font color=blue size="2">Aguarde! Carregando Dados...</font>
</div>

<form name="formulario" id="formulario" method="post" action="">
    <div style="overflow: auto; width: 496px; height: 350px; border: 2px solid #ececec; background-color: white;">
        <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">

        <script language="JavaScript">
            document.getElementById('tabela').style.visibility = "hidden";
            document.getElementById('tabela').style.display  = "none";
        </script>

        <thead>
            <tr>
                <td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3">
                    <strong>Selecione o(s) estado(s)</strong>
                </td>
            </tr>
            <tr>
                <?php
                    $cabecalho = 'Selecione o(s) estado(s)';
                    $sql = "
                        select
                                estuf, estuf, estdescricao
                        from territorios.estado
                        order by estuf, estdescricao
                    ";
                    $RS = @$db->carregar($sql);
                    $nlinhas = count($RS) - 1;
                    $j = 0;
                    for ($i = 0; $i <= $nlinhas; $i++) {
                        foreach ($RS[$i] as $k => $v)
                            ${$k} = $v;
                        if (fmod($i, 2) == 0)
                            $cor = '#f4f4f4';
                        else
                            $cor = '#e0e0e0';
                ?>
                <tr bgcolor="<?= $cor ?>">
                    <td width="20" align="right"><img src="/imagens/mais.gif" id="<?= $estuf . "_img" ?>" onclick="mostraEsconde('<?= $estuf ?>')">&nbsp;</td>
                    <td align="left" style="color: blue;"><?= $estuf . ' - ' . $estdescricao ?></td>
                </tr>
                <tr>
                    <td style="height: 0"></td>
                    <td style="height: 0">
                        <div id="<?= $estuf ?>" style="display: none;">
                            <table width="100%" cellpadding="0" cellspacing="0" border="0">
                                <?php
                                    if ($tipo == 'checkbox'){ /* não exibe o checkbox "todos" para dirigente municipal */
                                ?>
                                        <tr bgcolor="#f4f4f4">
                                            <td align="left" style="border: 0">
                                                <input type="checkbox" name="muncod" id="<?= $estuf ?>" value="<?= $estuf ?>" onclick="selecionaTodos( this, '<?= $estuf ?>' );" />Todos

                                            </td>
                                        </tr>
                                <?php
                                    };

                                    $sql = "select mundescricao, muncod from territorios.municipio where estuf = '$estuf' order by mundescricao";
                                    $municipios = $db->carregar($sql);

                                    foreach ($municipios as $municipio) {
                                        if ($cor2 == '#e0e0e0')
                                            $cor2 = '#f4f4f4';
                                        else
                                            $cor2 = '#e0e0e0';
                                ?>
                                    <tr bgcolor="<?= $cor2 ?>">
                                        <td align="left" style="border: 0">
                                            <input class="checkProfile" type="<?php echo $tipo;?>" name="muncod" id="<?=$municipio['muncod']?>" value="<?= $municipio['muncod']?>"
                                                   onclick="
                                                       retorna( this, '<?=$municipio['muncod']?>', '<?=$estuf.' - '.addslashes($municipio['mundescricao']) ?>' );
                                                       verificaRegraPerfilPar('<?=$municipio['muncod']?>', '<?=$_REQUEST['pflcod'];?>' );
                                                    "
                                            />
                                            <?= $municipio['mundescricao'] ?>
                                        </td>
                                    </tr>
                                <?php
                                    }
                                ?>
                            </table>
                        </div>
                    </td>
                </tr>
                <?php
                    }
                ?>
        </table>
    </div>
</form>

<form name="formassocia" id="formassocia" style="margin: 0px;" method="POST">
    <input type="hidden" name="usucpf" value="<?=$usucpf?>">
    <input type="hidden" name="pflcod" value="<?=$pflcod?>">
    <input type="hidden" name="enviar" value="">

    <select multiple size="8" onclick="mostraMunicipio(this);" name="usuunidresp[]" id="usuunidresp" style="width: 500px;" class="CampoEstilo">
	<?php
            $sql = "
                select
                    distinct m.muncod as codigo, m.estuf||' - '||m.mundescricao as descricao
                from
                    par3.usuarioresponsabilidade ur
                inner join territorios.municipio m on ur.muncod = m.muncod
                where ur.rpustatus='A' and ur.usucpf = '$usucpf' and ur.pflcod=$pflcod
            ";
            $RS = $db->carregar($sql);
            if (is_array($RS)) {
                $nlinhas = count($RS) - 1;
                if ($nlinhas >= 0) {
                    for ($i = 0; $i <= $nlinhas; $i++) {
                        foreach ($RS[$i] as $k => $v)
                            ${$k} = $v;
                        print " <option value=\"$codigo\">$descricao</option>";
                    }
                }
            }
        ?>
    </select>
</form>

<div id="erro"></div>
<table width="100%" align="center" border="0" cellspacing="0"
       cellpadding="2" class="listagem">
    <tr bgcolor="#c0c0c0">
        <td align="right" style="padding: 3px;" colspan="3">
            <input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);enviarFormulario();" id="ok">
        </td>
    </tr>
</table>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

<script language="JavaScript">
    document.getElementById('aguarde').style.visibility = "hidden";
    document.getElementById('aguarde').style.display  = "none";
    document.getElementById('tabela').style.visibility = "visible";
    document.getElementById('tabela').style.display  = "";

    //selecionaTodos
    function selecionaTodos(check, estado){
	$.ajax({
            type: "POST",
            url: window.location,
            data: "requisicao=municipio&estado="+estado,
            success: function(msg){
                var arrMuncod = msg.split('|');

                for(i=0; i<arrMuncod.length; i++){
                    var arrMunicipio = arrMuncod[i].split('#');

                    if(check.checked == true){
                        if( document.getElementById(arrMunicipio[0]).checked == false ){
                                document.getElementById(arrMunicipio[0]).checked = true;
                                retorna( check, arrMunicipio[0], estado+' - '+arrMunicipio[1] );
                        }
                    } else {
                        document.getElementById(arrMunicipio[0]).checked = false;
                        retorna( check, arrMunicipio[0], estado+' - '+arrMunicipio[1]);
                    }
                }
            }
	});
    }

    function verificaRegraPerfilPar(muncod, perfil){
	$.ajax({
            type: "POST",
            url: window.location,
            data: "requisicao=regraperfilpar&muncod="+muncod+"&perfil="+perfil,
            dataType: 'json',
            success: function(response){
                if(response.boValidaRegraPrefeito == '0'){
                    return true;
                }else{
                    if( response.boValidaRegraPrefeito == '1' ){
                        alert(response.msgRegraPrefeito);

                        var objMunicipio = document.getElementById(muncod);
                        objMunicipio.checked = false;
                        return false;
                    }
                }
            }
	});
    }

    function mostraEsconde(estado){
        if (!estado)
            return false;

        var estadoAtual = document.getElementById(estado).style.display;
        var objImagem = document.getElementById(estado+'_img');
        if(estadoAtual == 'none'){
            document.getElementById(estado).style.display = 'block';
            objImagem.src = '/imagens/menos.gif';
        }else{
            document.getElementById(estado).style.display = 'none';
            objImagem.src = '/imagens/mais.gif';
        }
    }

    var campoSelect = document.getElementById("usuunidresp");

    if (campoSelect.options[0] && campoSelect.options[0].value != ''){
        for(var i=0; i<campoSelect.options.length; i++){
            document.getElementById(campoSelect.options[i].value).checked = true;
        }
    }

    function enviarFormulario(){
        document.formassocia.enviar.value=1;
        document.formassocia.submit();
    }

    function mostraMunicipio(objSelect){
	for( var i = 0; i < objSelect.options.length; i++ ){
            if ( objSelect.options[i].value == objSelect.value ){
                var estado = objSelect.options[i].innerHTML.substring(0,2);
                break;
            }
	}
        if (!estado)
            return false;

	var estadoAtual = document.getElementById(estado).style.display;
	if(estadoAtual != 'block'){
            mostraEsconde(estado);
	}
	document.getElementById(objSelect.value).focus();
    }

    function retorna( check, muncod, mundescricao ){
	if ( check.checked ){
            // põe
            <?php if($pflcod != Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL): ?>
		campoSelect.options[campoSelect.options.length] = new Option( mundescricao, muncod, false, false );
            <?php else: ?>
                campoSelect.options[0] = new Option( mundescricao, muncod, false, false );
            <?php endif; ?>

	}else{
            // tira
            for( var i = 0; i < campoSelect.options.length; i++ ){
                if ( campoSelect.options[i].value == muncod ){
                    campoSelect.options[i] = null;
                }
            }
	}
	sortSelect( campoSelect );
    }

    $(function(){
        $(".checkProfile").change(function(){
            var $element = $(this);

            if( $element.attr("checked") == true ){
                var params = {
                    muncod: $element.val(),
                    pflcod: <?=$_GET['pflcod'];?>,
                    usucpf: <?=$_GET['usucpf'];?>,
                    ajax: 'par3'
                }

                if( params ){
                    $.ajax({
                        url:location.href,
                        type:'post',
                        data:params,
                        success: function(data){
                            if (data.usu == 'false') return;

                            var messageConfirm = "O usuário: '"+data.usu.usunome+"' já possuí o perfil de "+data.usu.pfldsc+" " + "para o municipio selecionado, Dejesa subsituir o perfil?";

                            if (confirm(messageConfirm)) {
                                //#LIMPA AS OPÇÕEAS SELECIONADAS ANTERIORMENTE CASO EXISTA;
                                $('#formassocia').find('.hid_option').remove();

                                $("#formassocia").append("<input type='hidden' class='hid_option' name='exclude["+$element.val()+"][]' value='"+$element.val()+","+data.usu.usucpf+","+data.usu.pflcod+"' />");
                                $element.attr("checked", true);
                            } else {
                                alert('ops');
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

<?php
//função jquery para impedir que mais de uma checkbox seja marcada para o dirigente municipal
if($pflcod == Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL):
?>
$(".checkProfile").click( function(){
    console.log(this);
    $('input[type="checkbox"]').not(this).attr('checked', false);
});
<?php endif; ?>

</script>
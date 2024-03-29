<?php
function inserirEspelhamento($dados) {
	global $db;
	
	$sql = "INSERT INTO seguranca.espelhoperfil(
            pflcodmaster, sisidmaster, pflcodslave, sisidslave, eplstatus, 
            urcampo,servidormaster,servidorslave)
    		VALUES ('".$dados['pflcodmaster']."', '".$dados['sisidmaster']."', '".$dados['pflcodslave']."', '".$dados['sisidslave']."', 'A', ".(($dados['urcampo'])?"'".$dados['urcampo']."'":"NULL").",'".$dados['rdo_servidor_master']."','".$dados['rdo_servidor_slave']."');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Regra de espelhamento inserido com sucesso');
			window.location='seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A';
		  </script>";
}

function atualizarEspelhamento($dados) {
	global $db;
	
	$sql = "UPDATE seguranca.espelhoperfil
   			SET pflcodmaster='".$dados['pflcodmaster']."', sisidmaster='".$dados['sisidmaster']."', 
   				pflcodslave='".$dados['pflcodslave']."', sisidslave='".$dados['sisidslave']."', 
       			urcampo=".(($dados['urcampo'])?"'".$dados['urcampo']."'":"NULL").",
       			servidormaster = '".$dados['rdo_servidor_master']."',
       			servidorslave = '".$dados['rdo_servidor_slave']."'
 			WHERE eplid='".$dados['eplid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Regra de espelhamento atualizada com sucesso');
			window.location='seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A';
		  </script>";
}

function carregarResponsabilidade($dados) {
	global $db;

	$servidor_master = $_POST['servidor_master'];
	$servidor_slave = $_POST['servidor_slave'];
	
	if($servidor_master == $servidor_slave){
		$sql = "SELECT DISTINCT
						pg_attribute.attname AS codigo,
						pg_attribute.attname AS descricao
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
						pg_class.relname = 'usuarioresponsabilidade'
					AND
						pg_namespace.nspname IN( SELECT sisdiretorio FROM seguranca.sistema WHERE sisstatus='A' AND sisid = '{$dados['sisidmaster']}' )
					AND 
						pg_attribute.attnum > 0
					AND 
						pg_attribute.attrelid = pg_class.oid
					AND 
						pg_attribute.atttypid = pg_type.oid 
					AND 
						pg_attribute.attname NOT IN('rpuid','usucpf','rpudata_inc','rpustatus','pflcod')
						
				INTERSECT ALL (
				
					SELECT DISTINCT
						pg_attribute.attname AS codigo,
						pg_attribute.attname AS descricao
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
						pg_class.relname = 'usuarioresponsabilidade'
					AND
						pg_namespace.nspname IN( SELECT sisdiretorio FROM seguranca.sistema WHERE sisstatus='A' AND sisid = '{$dados['sisidslave']}' )
					AND 
						pg_attribute.attnum > 0
					AND 
						pg_attribute.attrelid = pg_class.oid
					AND 
						pg_attribute.atttypid = pg_type.oid 
					AND 
						pg_attribute.attname NOT IN('rpuid','usucpf','rpudata_inc','rpustatus','pflcod'))";
		
		if($servidor_master == "simec"){
			$db->monta_combo('urcampo', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'urcampo', false, $dados['urcamposel']);
		} else{
		    $aryPDE = adapterConnection::pddeinterativo()->carregar($sql);
		    $db->monta_combo('urcampo', $aryPDE, 'S', 'Selecione', '', '', '', '', 'N', 'urcampo', false, $dados['urcamposel']);
		}
	} else {
	    
		$sql = "SELECT DISTINCT
						pg_attribute.attname AS codigo,
						pg_attribute.attname AS descricao
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
						pg_class.relname = 'usuarioresponsabilidade'
					AND
						pg_namespace.nspname IN( SELECT sisdiretorio FROM seguranca.sistema WHERE sisstatus='A' AND sisid = '{$dados['sisidmaster']}')
					AND 
						pg_attribute.attnum > 0
					AND 
						pg_attribute.attrelid = pg_class.oid
					AND 
						pg_attribute.atttypid = pg_type.oid 
					AND 
						pg_attribute.attname NOT IN('rpuid','usucpf','rpudata_inc','rpustatus','pflcod')";
		
		if($servidor_master == "simec"){
			$arrDadosMaster = $db->carregarColuna($sql);
		} else {
		    $arrDadosMaster = adapterConnection::pddeinterativo()->carregar($sql);
		    $arrDadosMaster = carregarColunaPDE($arrDadosMaster);
		}

		$sql = "SELECT DISTINCT
						pg_attribute.attname AS codigo,
						pg_attribute.attname AS descricao
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
						pg_class.relname = 'usuarioresponsabilidade'
					AND
						pg_namespace.nspname IN( SELECT sisdiretorio FROM seguranca.sistema WHERE sisstatus='A' AND sisid = '{$dados['sisidslave']}')
					AND 
						pg_attribute.attnum > 0
					AND 
						pg_attribute.attrelid = pg_class.oid
					AND 
						pg_attribute.atttypid = pg_type.oid 
					AND 
						pg_attribute.attname NOT IN('rpuid','usucpf','rpudata_inc','rpustatus','pflcod')";
		
		if($servidor_slave == "simec"){
			$arrDadosSlave = $db->carregarColuna($sql);
		} else {
		    $arrDadosSlave = adapterConnection::pddeinterativo()->carregar($sql);
		    $arrDadosSlave = carregarColunaPDE($arrDadosSlave);
		}
		
		$arrDadosMaster = $arrDadosMaster ? $arrDadosMaster : array();
		$arrDadosSlave = $arrDadosSlave ? $arrDadosSlave : array();
		
		foreach($arrDadosMaster as $perfil){
			if(in_array($perfil,$arrDadosSlave)){
				$arrDados[$perfil] = $perfil;
			}
		}
		
		foreach($arrDadosSlave as $perfil){
			if(in_array($perfil,$arrDadosMaster)){
				$arrDados[$perfil] = $perfil;
			}
		}
		
		$arrDados = $arrDados ? $arrDados : $arrDados;
		$n=0;
		
		if(is_array($arrDados)){ 
    		foreach($arrDados as $arr){
    			$arrPerfil[$n]['codigo'] = $arr;
    			$arrPerfil[$n]['descricao'] = $arr;
    			$n++;
    		}
		} 
		
		$arrPerfil = $arrPerfil ? $arrPerfil : array();
		$db->monta_combo('urcampo', $arrPerfil, 'S', 'Selecione', '', '', '', '', 'N', 'urcampo', false, $dados['urcamposel']);
	}
}

function carregarColunaPDE($res, $coluna = '', $var = null)
{
    if ($var != null) {
        global ${$var};
        ${$var} = $res;
    }
    
    if (!$res) {
        return array();
    }
    
    $result = array();
    
    foreach ($res as $row) {
        if (!$coluna) {
            array_push($result, reset($row));
        } else {
            array_push($result, $row[$coluna]);
        }
    }
    
    return $result;
}

function carregarPerfil($dados) {
	global $db;
	
	$sql = "SELECT pflcod as codigo, pfldsc as descricao FROM seguranca.perfil WHERE pflstatus='A' AND sisid = '{$dados['sisid']}' ORDER BY pfldsc";
	$servidor = $_POST['servidor'];
	
	switch($servidor){
		case "simec":
			$db->monta_combo($dados['name'], $sql, 'S', 'Selecione', '', '', '', '', 'S', $dados['name'], false, $dados['pflcodsel']);
		break;
		case "pdeinterativo":
		    $aryPDE = adapterConnection::pddeinterativo()->carregar($sql);
		    $db->monta_combo($dados['name'], $aryPDE, 'S', 'Selecione', '', '', '', '', 'S', $dados['name'], false, $dados['pflcodsel']);
		break;
	}
}

function excluirEspelhamento($dados) {
	global $db;
	
	$sql = "DELETE FROM seguranca.espelhoperfil WHERE eplid = '{$dados['eplid']}'";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Regra de espelhamento removida com sucesso');
			window.location='seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A';
		  </script>";
	
}

function executarEspelhamentoTodos($dados) {
	global $db;
	
	$sql = "SELECT pflcodmaster FROM seguranca.espelhoperfil GROUP BY pflcodmaster";
	$pflcods = $db->carregarColuna($sql);
	
	if($pflcods) {
		foreach($pflcods as $pflcod) {
			executarEspelhamento(array("pflcod"=>$pflcod));
		}
	}
}

function executarEspelhamento($dados) {
	global $db;
	
	include_once APPRAIZ.'includes/funcoes_espelhoperfil.php';
	
	/* configurações do relatorio - Memoria limite de 1024 Mbytes */
	ini_set("memory_limit", "2048M");
	set_time_limit(0);
	/* FIM configurações - Memoria limite de 1024 Mbytes */

	$sql = "SELECT u.usucpf, p.pflcod FROM seguranca.usuario u INNER JOIN seguranca.perfilusuario p ON p.usucpf = u.usucpf WHERE p.pflcod = '{$dados['pflcod']}'";
	$usrs = $db->carregar($sql);
	
	if($usrs[0]) {
		foreach($usrs as $usr) {
			inserirPerfisSlaves($usr['usucpf'], $usr['pflcod']);
			atualizarResponsabilidadesSlaves($usr['usucpf'], $usr['pflcod']);
		}
	}
	
	echo "<script>
			alert('Execução Efetuada');
			window.location='seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A';
		  </script>";
	
}

function mudaServidorSlave($dados) {
	global $db;
	
	$sql = "SELECT sisid AS codigo, sisabrev AS descricao FROM seguranca.sistema WHERE sisstatus ='A' ORDER BY sisabrev";
	
	switch($dados['tipo']){
		case "simec":
		    $db->monta_combo('sisidslave', $sql, 'S', 'Selecione', 'carregarPerfilSlave', '', '', '', 'S', 'sisidslave', false, $espelhoperfil['sisidslave']);
		break;
		case "pdeinterativo":
		    $aryPDE = adapterConnection::pddeinterativo()->carregar($sql);
		    $db->monta_combo('sisidslave', $aryPDE, 'S', 'Selecione', 'carregarPerfilSlave', '', '', '', 'S', 'sisidslave', false, $espelhoperfil['sisidslave']);
	    break;
	}
}

function mudaServidorMaster($dados) {
	global $db;
	
	$sql = "SELECT sisid AS codigo, sisabrev AS descricao FROM seguranca.sistema WHERE sisstatus ='A' ORDER BY sisabrev";
	
	switch($dados['tipo']){
		case "simec":
			$db->monta_combo('sisidmaster', $sql, 'S', 'Selecione', 'carregarPerfilMaster', '', '', '', 'S', 'sisidmaster', false, $espelhoperfil['sisidmaster']);
		break;
		case "pdeinterativo":
		    $aryPDE = adapterConnection::pddeinterativo()->carregar($sql);
		    $db->monta_combo('sisidmaster', $aryPDE, 'S', 'Selecione', 'carregarPerfilMaster', '', '', '', 'S', 'sisidmaster', false, $espelhoperfil['sisidmaster']);
		break;
	}
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Espelhamento de perfis", "Gerenciamento de espelhamento de perfis. Tudo que for inserido/alterado no master sera replicado no slave");

if($_REQUEST['eplid']) {
	$requisicao = "atualizarEspelhamento";
	$sql = "SELECT * FROM seguranca.espelhoperfil WHERE eplid='".$_REQUEST['eplid']."'";
	$espelhoperfil = $db->pegaLinha($sql);
	
} else {
	$requisicao = "inserirEspelhamento";
} ?>

<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/seguranca.js"></script>
<script>
jQuery.noConflict();

<?php if($espelhoperfil) : ?>
jQuery(document).ready(function() {
	carregarPerfilMaster('<?=$espelhoperfil['sisidmaster'] ?>');
	carregarPerfilSlave('<?=$espelhoperfil['sisidslave'] ?>');
});
<? endif; ?>

function carregarPerfilMaster(sisid) {
	var servidor = jQuery("[name='rdo_servidor_master']:checked").val();
	jQuery.ajax({
   		type: "POST",
   		url: "seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A",
   		data: "requisicao=carregarPerfil&name=pflcodmaster<?=(($espelhoperfil['pflcodmaster'])?"&pflcodsel=".$espelhoperfil['pflcodmaster']:"") ?>&sisid="+sisid+"&servidor="+servidor,
   		async: false,
   		success: function(html){jQuery('#td_perfilmaster').html(html);}
 		});
 		
 	if(jQuery('#sisidmaster').val()!='' && jQuery('#sisidslave').val()!='') {
 		carregarComboColunasUsuarioResponsabilidade(jQuery('#sisidmaster').val(), jQuery('#sisidslave').val());
 	}
}

function carregarPerfilSlave(sisid) {
	var servidor = jQuery("[name='rdo_servidor_slave']:checked").val();
	jQuery.ajax({
   		type: "POST",
   		url: "seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A",
   		data: "requisicao=carregarPerfil&name=pflcodslave<?=(($espelhoperfil['pflcodslave'])?"&pflcodsel=".$espelhoperfil['pflcodslave']:"") ?>&sisid="+sisid+"&servidor="+servidor,
   		async: false,
   		success: function(html){jQuery('#td_perfilslave').html(html);}
 		});
 		
 	if(jQuery('#sisidmaster').val()!='' && jQuery('#sisidslave').val()!='') {
 		carregarComboColunasUsuarioResponsabilidade(jQuery('#sisidmaster').val(), jQuery('#sisidslave').val());
 	}
}

function carregarComboColunasUsuarioResponsabilidade(sisidmaster, sisidslave) {
	var servidor_master = jQuery("[name='rdo_servidor_master']:checked").val();
	var servidor_slave  = jQuery("[name='rdo_servidor_slave']:checked").val();
	jQuery.ajax({
   		type: "POST",
   		url: "seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A",
   		data: "requisicao=carregarResponsabilidade<?=(($espelhoperfil['urcampo'])?"&urcamposel=".$espelhoperfil['urcampo']:"") ?>&sisidmaster="+sisidmaster+"&sisidslave="+sisidslave+"&servidor_master="+servidor_master+"&servidor_slave="+servidor_slave,
   		async: false,
   		success: function(html){jQuery('#td_responsabilidade').html(html);}
	});
}

function salvarEspelhoPerfil() {
	if(jQuery('#sisidmaster').val()=='') {
		alert('Selecione o sistema master');
		return false;
	}
	if(jQuery('#pflcodmaster').val()=='') {
		alert('Selecione o perfil master');
		return false;
	}
	if(jQuery('#sisidslave').val()=='') {
		alert('Selecione o sistema slave');
		return false;
	}
	if(jQuery('#pflcodslave').val()=='') {
		alert('Selecione o perfil slave');
		return false;
	}
	
	document.getElementById('formulario').submit();
}

function excluirEspelhamento(eplid) {
	var conf = confirm('Deseja realmente excluir?');
	if(conf) {
		window.location='seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A&requisicao=excluirEspelhamento&eplid='+eplid;
	}
}

function executarEspelhamento(pflcod,servidor) {
	var conf = confirm('Este processo irá executar o espelhamento de todos os usuários com o perfil selecionado. Este processo é lento (5 minutos de execução), deseja continuar?');
	if(conf) {
		divCarregando();
		window.location='seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A&requisicao=executarEspelhamento&pflcod='+pflcod+'&servidor='+servidor;
	}
}

function mudaServidorMaster(tipo){
	jQuery.ajax({
   		type: "POST",
   		url: "seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A",
   		data: "requisicao=mudaServidorMaster&tipo="+tipo,
   		async: false,
   		success: function(html){jQuery('#td_sistema_master').html(html);}
 	});
}

function mudaServidorSlave(tipo){
	jQuery.ajax({
   		type: "POST",
   		url: "seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A",
   		data: "requisicao=mudaServidorSlave&tipo="+tipo,
   		async: false,
   		success: function(html){jQuery('#td_sistema_slave').html(html);}
 	});
}
</script>

<form method="post" name="formulario" id="formulario">
<input type="hidden" name="requisicao" value="<?=$requisicao; ?>">
<?php
if($espelhoperfil){
	extract($espelhoperfil);
}
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
    <tr>
    	<td class='SubTituloDireita' width="25%" >Sistema Master - <input onclick="mudaServidorMaster('simec')" type="radio" <?php echo !$espelhoperfil['servidormaster'] || $espelhoperfil['servidormaster'] == "simec" ? "checked='checked'" : "" ?> name="rdo_servidor_master" value="simec" />SIMEC <input <?php echo $espelhoperfil['servidormaster'] == "pdeinterativo" ? "checked='checked'" : "" ?> onclick="mudaServidorMaster('pdeinterativo')" type="radio" name="rdo_servidor_master" value="pdeinterativo" disabled />PDE Interativo:</td>
    	<td id="td_sistema_master">
    	<?
    	$sql = "SELECT sisid AS codigo, sisabrev AS descricao FROM seguranca.sistema WHERE sisstatus = 'A' ORDER BY sisabrev";
    	
    	if($espelhoperfil['servidormaster'] == "pdeinterativo"){
    	    $aryPDE = adapterConnection::pddeinterativo()->carregar($sql);
    	    $db->monta_combo('sisidmaster', $aryPDE, 'S', 'Selecione', 'carregarPerfilMaster', '', '', '', 'S', 'sisidmaster', false, $espelhoperfil['sisidmaster']);
    	} else {
    		$db->monta_combo('sisidmaster', $sql, 'S', 'Selecione', 'carregarPerfilMaster', '', '', '', 'S', 'sisidmaster', false, $espelhoperfil['sisidmaster']);
    	}
    	?>
    	</td>
    </tr>
    <tr>
    	<td class='SubTituloDireita'>Perfil Master:</td>
    	<td id="td_perfilmaster"><b>Selecione sistema master</b></td>
    </tr>
    <tr>
    	<td class='SubTituloDireita'>Sistema Slave - <input onclick="mudaServidorSlave('simec');" type="radio" <?php echo !$espelhoperfil['servidorslave'] || $espelhoperfil['servidorslave'] == "simec" ? "checked='checked'" : "" ?> name="rdo_servidor_slave" value="simec" />SIMEC <input <?php echo $espelhoperfil['servidorslave'] == "pdeinterativo" ? "checked='checked'" : "" ?> onclick="mudaServidorSlave('pdeinterativo')" type="radio" name="rdo_servidor_slave" value="pdeinterativo" />PDE Interativo:</td>
    	<td id="td_sistema_slave">
    	<?php
    	$sql = "SELECT sisid AS codigo, sisabrev AS descricao FROM seguranca.sistema WHERE sisstatus = 'A' ORDER BY sisabrev";
    	
    	if($espelhoperfil['servidorslave'] == "pdeinterativo"){
    	    $aryPDE = adapterConnection::pddeinterativo()->carregar($sql);
    	    $db->monta_combo('sisidslave', $aryPDE, 'S', 'Selecione', 'carregarPerfilSlave', '', '', '', 'S', 'sisidslave', false, $espelhoperfil['sisidslave']);
    	} else {
    		$db->monta_combo('sisidslave', $sql, 'S', 'Selecione', 'carregarPerfilSlave', '', '', '', 'S', 'sisidslave', false, $espelhoperfil['sisidslave']);
    	}
    	?>
    	</td>
    </tr>
    <tr>
    	<td class='SubTituloDireita'>Perfil Slave:</td>
    	<td id="td_perfilslave"><b>Selecione sistema slave</b></td>
    </tr>
    <tr>
    	<td class='SubTituloDireita'>Responsabilidade transferida:</td>
    	<td id="td_responsabilidade"><b>Selecione sistema master/slave</b></td>
    </tr>
    <tr>
    	<td class='SubTituloCentro' colspan="2"><input type="button" name="botao" value="Salvar" onclick="salvarEspelhoPerfil();"> <input type="button" name="novo" value="Novo" onclick="window.location='seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A';"></td>
    </tr>
</table>
</form>

<?php
$sql = "SELECT   	'<img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"window.location=\"seguranca.php?modulo=principal/gerenciarespelhoperfil&acao=A&eplid='||eplid||'\"\";> 
        			 <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirEspelhamento('||eplid||');\";> <img src=../imagens/refresh2.gif style=cursor:pointer; onclick=\"executarEspelhamento(' ||ep.pflcodmaster|| ' , ' || ep.servidormaster || ')\">' as acao,
        			 sm.sisdsc || ' (Banco: ' || ep.servidormaster || ')' as sistemamaster,
        			 pm.pfldsc as perfilmaster,
        			 pm.pflcod as pflcodmaster,
        			 ss.sisdsc || ' (Banco: ' || ep.servidorslave || ')' as sistemaslave,
        			 ep.pflcodslave,
        			 ps.pfldsc as perfilslave,
        			 ep.urcampo,
        			 ep.servidormaster,
        			 ep.servidorslave,
        			 ep.sisidmaster,
        			 ep.sisidslave
		FROM         seguranca.espelhoperfil ep 
		LEFT JOIN    seguranca.sistema sm ON ep.sisidmaster = sm.sisid 
		LEFT JOIN    seguranca.sistema ss ON ep.sisidslave  = ss.sisid 
		LEFT JOIN    seguranca.perfil pm  ON pm.pflcod = ep.pflcodmaster
		LEFT JOIN    seguranca.perfil ps  ON ps.pflcod = ep.pflcodslave 
		ORDER BY     eplid"; 

$arrDados = $db->carregar($sql);

if($arrDados){
	$n=0;
	foreach($arrDados as $dado){
		$arrDadosFinal[$n] = $dado;
		if($dado['servidormaster'] == "pdeinterativo"){
			$sql = "select sisdsc || ' (Banco: pdeinterativo)' from seguranca.sistema where sisid = ".$dado['sisidmaster'];
			$arrDadosFinal[$n]['sistemamaster'] = adapterConnection::pddeinterativo()->pegaUm($sql);
			
			if($dado['pflcodmaster']) {
				$sql = "select pfldsc from seguranca.perfil where pflcod = ".$dado['pflcodmaster'];
				$arrDadosFinal[$n]['perfilmaster'] = adapterConnection::pddeinterativo()->pegaUm($sql);
			}
		}
		if($dado['servidorslave'] == "pdeinterativo"){
			$sql = "select sisdsc || ' (Banco: pdeinterativo)' from seguranca.sistema where sisid = ".$dado['sisidslave'];
			$arrDadosFinal[$n]['sistemaslave'] = adapterConnection::pddeinterativo()->pegaUm($sql);
			
			if($dado['pflcodslave']) {
				$sql = "select pfldsc from seguranca.perfil where pflcod = ".$dado['pflcodslave'];
				$arrDadosFinal[$n]['perfilslave'] = adapterConnection::pddeinterativo()->pegaUm($sql);
			}
			
		}
		unset($arrDadosFinal[$n]['servidormaster']);
		unset($arrDadosFinal[$n]['servidorslave']);
		unset($arrDadosFinal[$n]['sisidmaster']);
		unset($arrDadosFinal[$n]['sisidslave']);
		unset($arrDadosFinal[$n]['pflcodslave']);
		unset($arrDadosFinal[$n]['pflcodmaster']);
		
		$n++;
	}
}

$cabecalho = array("&nbsp;","Sistema Master","Perfil Master","Sistema Slave","Perfil Slave","Responsabilidade");
$db->monta_lista($arrDadosFinal,$cabecalho,100,5,'N','95%','center');
?>

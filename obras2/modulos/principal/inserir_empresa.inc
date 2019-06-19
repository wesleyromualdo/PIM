<?
require_once APPRAIZ . "includes/classes/entidades.class.inc";

if( $_REQUEST["tipo"] == "supervisao" ){
	$tipo = "&tipo=supervisao";
}
if($_REQUEST['entid'] != ''){
    $_SESSION['obras']['entid'] = $_REQUEST['entid'];
    $_SESSION['obras']['obrid'] = $_REQUEST['obrid'];
}

if($_REQUEST['opt'] == 'salvarRegistro') {

	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->salvar();
    unset($_SESSION['obras']['entid']);
	$nome = $_REQUEST['entnome'] != '' ? $_REQUEST['entnome'] : $_REQUEST['entrazaosocial'];
	$cnpj = $_REQUEST['entnumcpfcnpj'] != '' ? '('.$_REQUEST['entnumcpfcnpj'].') ': '';
	if( $_REQUEST["tipo"] == "supervisao" ){
	    
	    echo '<script type="text/javascript">
			      window.opener.document.getElementById("entnome").value = \'' .$cnpj.$nome . '\';
			      window.opener.document.getElementById("entid").value   = \'' . $entidade->getEntid() . '\';
			      window.close();
			  </script>';
		
	}else{
	    if( $_SESSION['obras']['obrid']){
            echo '<script type="text/javascript">
			      window.opener.document.getElementById("entnomeempresa_"+"'. $_SESSION['obras']['obrid'].'").innerHTML = \'' .$cnpj.$nome . '\';
			      window.opener.document.getElementById("entidempresa").value       = \'' . $entidade->getEntid() . '\';
			      window.close();
			  </script>';
            unset($_SESSION['obras']['obrid']);
        }else{
            echo '<script type="text/javascript">
			      window.opener.document.getElementById("entnomeempresa").innerHTML = \'' .$cnpj.$nome . '\';
			      window.opener.document.getElementById("entidempresa").value       = \'' . $entidade->getEntid() . '\';
			      window.close();
			  </script>';
        }

	}
	
    exit;
    
}

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

define("ID_EMPRESACONTRATADA", 46);
?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title><?= $titulo ?></title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/entidades.js"></script>
<script type="text/javascript" src="/includes/estouvivo.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<script type="text/javascript">
this._closeWindows = false;
</script> <?php (IS_PRODUCAO ? require_once APPRAIZ . 'includes/google_analytics.php' : ''); ?>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<div>
<?php
$entidade = new Entidades();
if($_REQUEST['entid'])
  	$entidade->carregarPorEntid($_REQUEST['entid']);
echo $entidade->formEntidade("obras2.php?modulo=principal/inserir_empresa&acao=A&opt=salvarRegistro{$tipo}",
							 array("funid" => ID_EMPRESACONTRATADA, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );
?>
    </div>

<script type="text/javascript">
document.getElementById('tr_entcodent').style.display = 'none';
document.getElementById('tr_entnuninsest').style.display = 'none';
document.getElementById('tr_entungcod').style.display = 'none';
document.getElementById('tr_tpctgid').style.display = 'none';
document.getElementById('tr_entunicod').style.display = 'none';
/*
 * DESABILITANDO O NOME DA ENTIDADE
 */
document.getElementById('entnome').readOnly = true;
document.getElementById('entnome').className = 'disabled';
document.getElementById('entnome').onfocus = "";
document.getElementById('entnome').onmouseout = "";
document.getElementById('entnome').onblur = "";
document.getElementById('entnome').onkeyup = "";



function enviaForm() {
    var erro =0;
    if (trim($F('entnumcpfcnpj')) == '') {
        erro = 1;
        alert('CNPJ é obrigatório.');
        return false;
    }
    if (trim($F('entnome')) == '' && trim($F('entrazaosocial')) == '') {
        erro = 1;
        alert('O nome ou razão social da entidade é obrigatório.');
        return false;
    }

    <?php if( $_REQUEST["tipo"] == "supervisao" ): ?>
    if (trim($F('entemail')) == '') {
        erro = 1;
        alert('O email da entidade é obrigatório.');
        return false;
    }

    <?php endif;?>
    if (erro == 0) {
        document.frmEntidade.submit();
        return true;
    }
}
</script>
  </body>
</html>

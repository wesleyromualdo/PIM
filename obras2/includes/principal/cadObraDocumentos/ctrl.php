<?php 

require APPRAIZ . 'obras2/includes/principal/cadObraDocumentos/funcoes.php';

switch ( $_REQUEST['requisicao'] ){
       case 'tornarimportante':


        $regAtividade = new ObrasArquivos( $_REQUEST['rgaid'] );

        $regAtividade->oarimp = 't';
        $regAtividade->alterar($dados['rgaimp']);
        $regAtividade->salvar();
        break;

    case 'retirarimportancia':
        $regAtividade = new ObrasArquivos( $_REQUEST['rgaid'] );

        $regAtividade->oarimp = 'f';
        $regAtividade->alterar($dados['rgaimp']);
        $regAtividade->salvar();
        break;
}



if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];
if( $_GET['acao'] != 'V' ){
	//Chamada de programa
	include  APPRAIZ."includes/cabecalho.inc";
	echo "<br>";
	if( !$_SESSION['obras2']['obrid'] && !$_SESSION['obras2']['empid'] ){
		$db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP,$url,$parametros);
	}elseif( $_SESSION['obras2']['obrid'] ){
		if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
			$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
		}else{
			$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
		}
	}else{
		$db->cria_aba(ID_ABA_CADASTRA_OBRA,$url,$parametros);
	}
	
	$habilitado = true;
	$habilita 	= 'S';
	
}else{
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
        <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<?php
	$db->cria_aba($abacod_tela,$url,$parametros);

	$habilitado = false;
	$habilita 	= 'N';
}

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
	$habilitado = false;
	$habilita = 'N';
}

$obrasArquivos 	= new ObrasArquivos();
$orgid 			= $_SESSION['obras2']['orgid'];

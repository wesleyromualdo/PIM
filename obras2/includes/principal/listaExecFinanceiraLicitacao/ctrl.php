<?php

require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraLicitacao/funcoes.php';

// empreendimento || obra || orgao
verificaSessao( 'obra' );

$orgid = $_SESSION['obras2']['orgid'];
$obrid = $_SESSION['obras2']['obrid'];

if (empty($obrid)) {
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}

switch ( $_REQUEST['requisicao'] ){
	case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die("<script>
		        history.go(-1);
			 </script>");
	case "excluirLicitacao":
		ob_clean();
        $lieid = $_REQUEST['lieid'];

        if(!verificaSeTemContrato($lieid)) {
            if (excluirLicitacao($lieid)) {
                echo "Licitação excluída com sucesso.";
                exit;
            }
            echo "Falha ao excluir a licitação";
            exit;
        }
        else{
            echo "Não é possível excluir a licitação, pois ela já possui registro em uma construtora.";
            exit;
        }
        break;
}

$obra  = new Obras( $obrid );
$estadoObra = $obra->pegaEstadoObra($obrid);



if( $_GET['acao'] != 'V' ){ 
	include APPRAIZ . 'includes/cabecalho.inc'; 
	echo '<br>';
 
	$arMenuBlock = array();

	if( $orgid == ORGID_EDUCACAO_BASICA ){
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros,$arMenuBlock);
	}else{
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros,$arMenuBlock);
	}

	$esdid = pegaEstadoObra( $obra->docid );
	if ( $esdid == ESDID_OBJ_LICITACAO || $esdid == ESDID_OBJ_AGUARDANDO_1_REPASSE ){
		$habilitado = true;
		$habilita 	= 'S';
	}else{
		$habilitado = false;
		$habilita 	= 'N';
	}
}else{
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<?php
	$db->cria_aba($abacod_tela,$url,$parametros); 
	$habilitado = false;
	$habilita 	= 'N';

}


if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
	$habilitado = false;
	$habilita 	= 'N';
}


$docid = pegaDocidObra( $obrid );

echo cabecalhoObra($obrid); 

monta_titulo( $titulo_modulo, '' );


$licitacao = new Licitacao();
$dados = $licitacao->pegaDadosPorObra( $obrid );

$execucaoFinanceira = new ExecucaoFinanceira();
 
echo $execucaoFinanceira->criaSubAba($url, $habilitado, $obrid);
 


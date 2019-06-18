<?php /* ****arquivo vazio**** */

require APPRAIZ . "obras2/includes/principal/exibeLicitacao/funcoes.php";


$orgid = $_SESSION['obras2']['orgid'];
$obrid = $_SESSION['obras2']['obrid'];

switch ( $_REQUEST['requisicao'] ){
	case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die("<script>
		        history.go(-1);
			 </script>");
}

$obra  = new Obras();
$obra->carregarPorIdCache($obrid);
if( $_GET['acao'] != 'V' ){
	// InclusÃ£o de arquivos padrÃ£o do sistema
	include APPRAIZ . 'includes/cabecalho.inc';
	// Cria as abas do mÃ³dulo
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
	$habilita 	= 'N';
}







if(!$obra->obrperccontratoanterior) {

/*Regra de bloqueio retirada a pedido do FERDINANDO FURLANI na ss 4114
 * 
 *     $habilitado = false;
    $habilita   = 'N';*/
}
$execucaoFinanceira = new ExecucaoFinanceira();
$seloLicitacaoAdicional = ($execucaoFinanceira->existeLicitacaoExtra($obrid)) ? '<div style="position: absolute; top: 540px; right: 130px; z-index:1;">
                    <img border="0" title="Esta obra possui licitaÃ§Ã£o adicional na aba ExecuÃ§Ã£o Financeira." src="../imagens/carimbo-licitacao.png">
                </div>' : '';

$docid = pegaDocidObra( $obrid );


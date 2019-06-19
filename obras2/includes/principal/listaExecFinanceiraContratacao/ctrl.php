<?php
require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraContratacao/funcoes.php';

verificaSessao( 'obra' );

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

if (empty($obrid) && empty($empid)) {
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}

switch ( $_REQUEST['op'] ){
	case 'download':
		$arqid = $_GET['arqid'];
		if ( $arqid ){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file 		  = new FilesSimec(null, null, "obras2");
			$file->getDownloadArquivo($arqid);
		}
		die();
}

switch ($_REQUEST['requisicao']) {

    case 'excluirConstrutora':
        ob_clean();
        $cexid = $_REQUEST['cexid'];
        if(excluirConstrutora($cexid)){
            echo "Construtora excluída com sucesso.";exit;
        }
        echo "Falha ao excluir a construtora";exit;
        break;
    case "download":
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $arqid = $_REQUEST['arqid'];
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($arqid);
        echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
        exit;
        break;
    case 'excluirAditivo':
        ob_clean();
        $crtid = $_REQUEST['crtid'];
        if(excluirAditivo($crtid)){
            echo "Aditivo excluído com sucesso.";exit;
        }
        echo "Falha ao excluir o aditivo";exit;
        break;
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$obra     = new Obras( $obrid );
$crtid    = $obra->pegaContratoPorObra( $obrid );
$estadoObra = $obra->pegaEstadoObra($obrid);
$contrato = new Contrato( $crtid );
$dados    = $contrato->getDados();
$execucaoFinanceira = new ExecucaoFinanceira();
$arrObrids = $execucaoFinanceira->retornaObrids($obrid);
$arrObridVinculado = array_diff($arrObrids, [$obrid]);
$arrConstrutoraExtra = $execucaoFinanceira->buscaConstrutorasExtra($obrid);
$verificarExecucaoFinanceira = $execucaoFinanceira->verificarExecucaoFinanceiraFinalizada($obrid);
$execucaoFinanceiraFinalizada = $verificarExecucaoFinanceira > 0 ? true : false;




//dbg($dados, d);
if ( $dados['crtid'] ){
	$dados['crtdtassinatura']  = formata_data($dados['crtdtassinatura']);
	$dados['dt_cadastro']      = formata_data($dados['dt_cadastro']);
	$dados['crtdttermino'] 	   = formata_data($dados['crtdttermino']);
	$dados['crtvalorexecucao'] = number_format($dados['crtvalorexecucao'], 2, ',', '.');
	$dados['crtpercentualdbi'] = number_format($dados['crtpercentualdbi'], 2, ',', '.');
//ver($dados, d);
//	extract( $dados );
	$obrvalorprevisto = number_format($obra->obrvalorprevisto, 2, ',', '.');

	$empresa 	= new Entidade( $dados['entidempresa'] );
//    $empresa->entnumcpfcnpj = '';
//    $empresa->entnome = '';
    $habilitaPesquisaCNPJ = 'S';
    if($empresa->entnumcpfcnpj != '' && $empresa->entnome != ''){
        $habilitaPesquisaCNPJ = 'N';
    }
	$entnomeempresa = "(". mascaraglobal($empresa->entnumcpfcnpj,"##.###.###/####-##") .") ".$empresa->entnome;

	$entidempresa 	= $empresa->getPrimaryKey();
}

$habilitaAditivo = false;
if( $_GET['acao'] != 'V' ){
	// Inclusão de arquivos padrão do sistema
	include APPRAIZ . 'includes/cabecalho.inc';
	// Cria as abas do módulo
	echo '<br>';

//	$arMenuBlock = bloqueiaMenuObjetoPorSituacao( $obrid );
	$arMenuBlock = array();

	if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros,$arMenuBlock);
	}else{
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros,$arMenuBlock);
	}

//	$obra  = new Obras( $obrid );
	$esdid = pegaEstadoObra( $obra->docid );
	if ( $esdid == ESDID_OBJ_CONTRATACAO /*|| $esdid == ESDID_OBJ_ADITIVO*/ ){
		$habilitado = true;
		$habilita 	= 'S';
	}else{
		$habilitado = false;
		$habilita 	= 'N';
	}
	if ( $esdid == ESDID_OBJ_EXECUCAO || $esdid == ESDID_OBJ_ADITIVO ){
		$habilitaAditivo = true;
	}
}else{
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

        <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
        <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
        <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>


	<?php
	$db->cria_aba($abacod_tela,$url,$parametros);
	$habilitado = false;
	$habilita 	= 'N';

}

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO, PFLCOD_GESTOR_MEC) ) ){
	$habilitado = false;
	$habilita = 'N';
}

$docid = pegaDocidObra( $obrid );

echo cabecalhoObra($obrid);
//echo '<br>';
monta_titulo( $titulo_modulo, '' );
$execucaoFinanceira = new ExecucaoFinanceira();
echo $execucaoFinanceira->criaSubAba($url, $habilitado, $obrid);


<?php 
require APPRAIZ . 'obras2/includes/principal/inauguracao/funcoes.php';


verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$obra = new Obras();
$obra->carregarPorIdCache($_SESSION['obras2']['obrid']);
$obra->pegaPercentualTotalVistoria($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];

if ($_GET['download']) {
    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid($_GET['download']);
    $eschema = 'obras2';

    $file = new FilesSimec(null,null,$eschema);
    $file->getDownloadArquivo($_GET['download']);

    die('<script type="text/javascript">
        window.close();
        </script>');
}

if($_REQUEST['req'] == 'ajaxDeclaracaoConformidade')
{
	$iobid = $_REQUEST['iobid'];
	
	global $db;
	if($iobid)
	{
		$result = $db->pegaUm("select dcfid from obras2.declaracaoconformidadefuncobra where dcfstatus = 'A' and iobid = $iobid");
		if($result)
		{
			die('ok');
		}
		else
		{
			die('erro');
		}
	}
	else
	{
		die('erro');
	}
		
}


if($_REQUEST['downloadHabitese'] == 'S'){

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($arqid);
	echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
	exit;

}


if ($_POST['requisicao'] == 'salvar_inauguracao') {
    salvarDados($_POST);
}

$habilitado = true;
$habil = 'S';

// Tratamento para garantir que a variável esteja igual o cálculo da lista, a pedido do usuário na demanda HISTÓRIAS 6565 e 6568
$percentual = $db->pegaUm("select ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) as percentual_execucao
from obras2.obras where obrid = {$obrid}");

if( $percentual != 0 ){
	if($percentual == $obra->percInstAcumulado)	{
		$percentualAtual = $obra->percInstAcumulado;
		
	}
	else 
	{
		$percentualAtual = $percentual;
	}
}
else
{
	$percentualAtual = $obra->percInstAcumulado;
}


if ($percentualAtual < 80) {
    $habilitado = false;
    $habil = 'N';
}
if (possui_perfil(PFLCOD_CALL_CENTER)) {
    $habilitado = false;
    $habil = 'N';
}
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
if($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros,array());
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros,array());
}
print cabecalhoObra($obrid);
monta_titulo_obras( 'Funcionamento da Obra', '');

$arrDadosInauguracao = importaDadosInauguracao($obrid);
$arrDadosFotos = importaFotosDadosInauguracao($arrDadosInauguracao['iobid']);
$arrArquivosHabitese = importaDadosHabiteSe($arrDadosInauguracao['iobid']);

$arrArquivosHabitese = ($arrArquivosHabitese) ? $arrArquivosHabitese : Array();


<?php
require_once APPRAIZ . 'includes/workflow.php';

if (!is_numeric($_GET['inuid']) || $_GET['inuid'] < 1 || !is_numeric($_GET['prgid']) || $_GET['prgid'] < 1) {
    header('location: /par3/par3.php?modulo=principal/listaMunicipios&acao=A');
}
//termo&acao=A&aceite=S&prgid=1&inuid=3803&prgid=54
$modelProadesao                     = new Par3_Model_Proadesao();
$modelAdesaoPrograma                = new Par3_Model_AdesaoPrograma();
$modelInstrumentoUnidadeEntidade    = new Par3_Model_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidade            = new Par3_Model_InstrumentoUnidade($_GET['inuid']);
$modalEscolasPrioritarias           = new Par3_Model_EscolasPrioritarias();
$modalInstrumentoUnidade            = new Par3_Model_InstrumentoUnidade($_GET['inuid']);
$controllerInstrumentoUnidade       = new Par3_Controller_InstrumentoUnidade();

$itrid                                  = $controllerInstrumentoUnidade->pegarItrid($_GET['inuid']);
$inunome = $modelInstrumentoUnidadenome = $controllerInstrumentoUnidade->pegarNomeEntidade($_GET['inuid']);

$programas      = $modelProadesao->pegarProgramas($_GET['inuid']);
$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($_GET['prgid']);
$programa       = $modelProadesao->pegarPrograma($_GET['prgid']);
$pfaid          = $proadesaoDados['pfaid'];

if(!$pfaid){
    echo "
        <script>
            alert('Adesão não cadastrada. Contate o Administrador do Sistema.');    
            window.history(-1);
        </script>";
    die;
}

if(isset($_REQUEST['prgid']) && $_REQUEST['prgid'] == 54){
    include_once APPRAIZ . "par3/modulos/principal/adesao/termos/apoio_emergencial_municipio/apoio_emergencial_controller.inc";
}

$termoincompleto = false;

if ($itrid === '2') {
	$tenid = Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE;
} else {
	$tenid = Par3_Model_InstrumentoUnidadeEntidade::SECRETARIO_ESTADUAL_EDUCACAO;
}

$objPessoaFisica = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($_GET['inuid'], $tenid);

if (empty($objPessoaFisica->entcpf)) {
	$termoincompleto = true;
}

if(!empty($_GET['inuid'])) {
	$aceite = $modelAdesaoPrograma->recuperaPorInuid($_GET['inuid'], $pfaid);
} else {
	$termoincompleto = true;
}

if (!empty($dados['pfaid'])) {
	$modelAdesaoPrograma =  new Par3_Model_AdesaoPrograma($dados['pfaid']);
} else {
	$modelAdesaoPrograma =  new Par3_Model_AdesaoPrograma();
}

require APPRAIZ . 'includes/cabecalho.inc';

if ($_REQUEST['salvar_pergunta_ppp'] == 1)
{
    if(empty($_POST['pppresp1']))
    {
        $_POST['pppresp1'] = 0;
    }

    $prp = new Par3_Model_ProadesaoRespostasPnfcd($_POST['pppid']);

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $file = new FilesSimec("proadesao_respostas_pnfcd ", 'pppresp1arqid', "par3");
    $file->setUpload('Lei', 'pppresp1arqid', false);

    if (empty($_POST['pppid'])) {
        $_POST['docid'] = wf_cadastrarDocumento(258, 'Documento de Adesão');
    }

    $_POST['pppresp1arqid'] = $file->getIdArquivo();

    $prp->popularDadosObjeto($_POST);
    $prp->salvar();
    $prp->commit();

    $url = "par3.php?modulo=principal/adesao/termo&acao=A&prgid=".$_POST['prgid']."&inuid=".$_POST['inuid'];

    simec_redirecionar($url, 'success');
    die();
}

if ($_GET['importar_excel'])
{
    ob_clean();
    $row = 0;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $arq1 = $file->getDownloadArquivo('19882872');

    if (($handle = fopen($arq1, 'r')) !== FALSE) {
        while (($data = fgetcsv($handle, 1000, ',')) !== FALSE) {
            $dados = explode(';', $data[0]);

            if ($row > 0) {
               echo "INSERT INTO par3.proadesao_lista_escolas_prioritarias
                    (pepescola, co_entidade, muncod, estuf, tp_dependencia)
                     VALUES ('$dados[7]', $dados[6],  $dados[4],'$dados[3]', $dados[8]);"."<br>";
            }

            $row++;
        }
        fclose($handle);
    }
    die();
}

if ($_GET['imprimir'] == '1')
{
	echo $aceite['padtermopreenchido'];
	echo '<script>window.print();</script>';
	die;
}

/**
 * PROGRAMA FPM
 */

$arquivo = trim($programa['prgarquivo']);

require_once APPRAIZ. "par3/modulos/principal/adesao/termos/{$arquivo}";

$url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid=' . $_REQUEST['inuid'];
$urlCalendario = 'par3.php?modulo=principal/calendario&acao=A&inuid=' . $_REQUEST['inuid'];
?>

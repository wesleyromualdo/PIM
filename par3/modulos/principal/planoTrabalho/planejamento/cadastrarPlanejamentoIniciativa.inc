<?php
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
require_once APPRAIZ . 'par3/classes/controller/IniciativaDescricaoController.class.inc';
require_once APPRAIZ . 'par3/classes/controller/Iniciativa_planejamento_atividadeetapa.class.inc';
require_once APPRAIZ . 'par3/classes/controller/Iniciativa_planejamento_detalheetapa.class.inc';
require_once APPRAIZ . 'par3/classes/controller/Iniciativa_planejamento_informacoesiniciativa.class.inc';
include_once APPRAIZ . 'par3/classes/controller/Iniciativa_planejamento_insumosiniciativa.class.inc';
include_once APPRAIZ . 'par3/classes/controller/AnalisePlanejamentoIniciativa.class.inc';
require_once 'componentes-indicador-funcoes.inc';
include_once "config.inc";
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . 'includes/library/simec/view/Helper.php';
initAutoload();
include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";
require_once APPRAIZ . "par3/classes/model/IndicadorDemografico.class.inc";
require_once APPRAIZ . "par3/classes/model/Iniciativa_indicador_demografico_status.class.inc";
require_once APPRAIZ . "par3/classes/model/IniciativaIndicadorDemograficoIndicadorStatus.class.inc";
$url = 'par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=' . $_REQUEST['inuid'];

$mInp = new Par3_Controller_IniciativaPlanejamento();
$controllerIniciativaPlanejamento = new Par3_Controller_IniciativaPlanejamento();
$controllerIniciativaPlanejamentoDesdobramentos = new Par3_Controller_IniciativaPlanejamentoDesdobramento();

$inp = $mInp->recuperar();

$obrid = validarInteiro($_GET['obrid']) ? $_GET['obrid'] : '';

//apenas para iniciativas de obras do proinfância
if($obrid > 0 && !$inp->inpid){
    $inp = $mInp->recuperaIniciativaProinfancia($obrid);
    $_REQUEST['inpid'] = $inp->inpid;
}

$mdDocPl = new Workflow_Model_Documento($inp->docid);

$mDim = new Par3_Model_Dimensao();
$iniid = new Par3_Model_Iniciativa();
$arrIniciativa = array();
$arrIniciativaDados = array('eta' => array());
$arrMod = array();
$arrDes = array();
$desdobramentos = array();
$mItr = new Par3_Controller_InstrumentoUnidade();
//instrumentounidade
$modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$modelDocumento = new Workflow_Model_Documento($modelInstrumentoUnidade->docid);

//
$controllerIniciativa = new Par3_Controller_Iniciativa();
$mdIniciativa = new Par3_Model_Iniciativa();

$arrItrid['itrid'] = $mItr->pegarItrid($_GET['inuid']);


//verifica se existe pendencia ou Restricao
$controllerRestricao = new Par3_Controller_Restricao();
$existeRestricao = $controllerRestricao->existeRestricaoIntrumentoUnidadeById($_GET['inuid']);
$existePendenciaRestricao = false; // true = possui pendências ou restrições, false = não possui restrições ou pendências

//caso existe pendências ou restrições, recebe true para travar o btn salvar, no cadastro da iniciativa no planejamento;
if ($existePendenciaGeral == 1 || $existeRestricao == true) {
    $existePendenciaRestricao = true;
}
//$existePendenciaRestricao = $mInp->verificaPendencias();
//$existePendenciaRestricao = count($_SESSION[$name]);
$existePendenciaRestricao = $pendenciasImpeditivas;
switch ($_REQUEST['requisicao']) {
    case 'salvarItensComposicaoFormacao':
        if ($_REQUEST['identificador'] == 'informacoesgerais') {
            $controllerIniciativaInformacoes = new Par3_Controller_Iniciativa_planejamento_informacoesiniciativa();
            ob_clean();
            $resp = $controllerIniciativaInformacoes->salvar($_REQUEST);
            $listaInformacoesGerais = $controllerIniciativaInformacoes->listaInformacoesGerais($_REQUEST['inpid'], true);

            echo simec_json_encode(array('resp'=>$resp, 'lista'=>$listaInformacoesGerais));
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'detalheEtapa') {
            $controllerIniciativaDetalhe = new Par3_Controller_Iniciativa_planejamento_detalheetapa();
            ob_clean();
            $resp = $controllerIniciativaDetalhe->salvar($_REQUEST);
            $listaIniciativaDetalhe = $controllerIniciativaDetalhe->listaDetalhe($_REQUEST['inpid'], true);
             
            echo simec_json_encode(array('resp'=>$resp, 'lista'=>$listaIniciativaDetalhe));
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'atividadeEtapas') {

            $controllerIniciativaAtividade = new Par3_Controller_Iniciativa_planejamento_atividadeetapa();
            ob_clean();
            $resp = $controllerIniciativaAtividade->salvar($_REQUEST);
            $listaIniciativaAtividade = $controllerIniciativaAtividade->listarAtividade($_REQUEST['inpid'], true);
            
            $totalCarga = $controllerIniciativaAtividade->totalCargaHoraria($_REQUEST['inpid']);

            echo simec_json_encode(array('resp'=>$resp, 'lista'=>$listaIniciativaAtividade, 'totalCargaHoraria'=>$totalCarga));

            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'insumoIniciativa') {

            $controllerIniciativaInsumos = new Par3_Controller_Iniciativa_planejamento_insumosiniciativa();
            ob_clean();
            $resp = $controllerIniciativaInsumos->salvar($_REQUEST);
            $listaIniciativaInsumos = $controllerIniciativaInsumos->listaInsumosIniciativa($_REQUEST['inpid'], true);
            echo simec_json_encode(array('resp'=>$resp, 'lista'=>$listaIniciativaInsumos));
            die;
            break;
        }

    case 'editarItensComposicaoFormacao':
        if ($_REQUEST['identificador'] == 'informacoesgerais') {
            $controllerIniciativaInformacoes = new Par3_Controller_Iniciativa_planejamento_informacoesiniciativa();
            ob_clean();
            echo $controllerIniciativaInformacoes->formInformacoesGerais($_REQUEST);
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'detalheEtapa') {
            $controllerIniciativaDetalhe = new Par3_Controller_Iniciativa_planejamento_detalheetapa();
            ob_clean();
            echo $controllerIniciativaDetalhe->formDetalheEtapa($_REQUEST);
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'atividadeEtapas') {
            $controllerIniciativaAtividade = new Par3_Controller_Iniciativa_planejamento_atividadeetapa();
            ob_clean();
            echo $controllerIniciativaAtividade->formAtividade($_REQUEST);
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'insumoIniciativa') {
            $controllerIniciativaInsumos = new Par3_Controller_Iniciativa_planejamento_insumosiniciativa();
            ob_clean();
            echo $controllerIniciativaInsumos->formInsumosIniciativa($_REQUEST);
            die;
            break;
        }

    case 'removerItensComposicaoFormacao':
        if ($_REQUEST['identificador'] == 'informacoesgerais') {
            $controllerIniciativaInformacoes = new Par3_Controller_Iniciativa_planejamento_informacoesiniciativa();
            ob_clean();
            $resp = $controllerIniciativaInformacoes->inativar($_REQUEST);
            $listaInformacoesGerais = $controllerIniciativaInformacoes->listaInformacoesGerais($_REQUEST['inpid'], true);

            echo simec_json_encode(array('resp'=>$resp, 'lista'=>$listaInformacoesGerais));

            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'detalheEtapa') {
             
            $controllerIniciativaDetalhe = new Par3_Controller_Iniciativa_planejamento_detalheetapa();
            ob_clean();
            $resp = $controllerIniciativaDetalhe->inativar($_REQUEST);
            $listaIniciativaDetalhe = $controllerIniciativaDetalhe->listaDetalhe($_REQUEST['inpid'], true);
            $count = $controllerIniciativaDetalhe->verificaQntDetalhes($_REQUEST['inpid']);
            echo simec_json_encode(array('resp'=>$resp, 'lista'=>$listaIniciativaDetalhe, 'count'=>$count));
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'atividadeEtapas') {
            $controllerIniciativaAtividade = new Par3_Controller_Iniciativa_planejamento_atividadeetapa();
            ob_clean();
            $resp = $controllerIniciativaAtividade->inativar($_REQUEST);
            $listaIniciativaAtividade = $controllerIniciativaAtividade->listarAtividade($_REQUEST['inpid'], true);
            $totalCarga = $controllerIniciativaAtividade->totalCargaHoraria($_REQUEST['inpid']);

            echo simec_json_encode(array('resp'=>$resp, 'lista'=>$listaIniciativaAtividade, 'totalCargaHoraria'=>$totalCarga));
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'insumoIniciativa') {
            $controllerIniciativaInsumos = new Par3_Controller_Iniciativa_planejamento_insumosiniciativa();
            ob_clean();
            $resp = $controllerIniciativaInsumos->inativar($_REQUEST);
            $listaIniciativaInsumos = $controllerIniciativaInsumos->listaInsumosIniciativa($_REQUEST['inpid'], true);
            
            echo simec_json_encode(array('resp'=>$resp, 'lista'=>$listaIniciativaInsumos));
            die;
            break;
        }

    case 'atualizarlistagemItensComposicaoFormacao':
        if ($_REQUEST['identificador'] == 'informacoesgerais') {
            $controllerIniciativaInformacoes = new Par3_Controller_Iniciativa_planejamento_informacoesiniciativa();
            ob_clean();
            echo $controllerIniciativaInformacoes->listaInformacoesGerais($inp->inpid);
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'detalheEtapa') {
            $controllerIniciativaDetalhe = new Par3_Controller_Iniciativa_planejamento_detalheetapa();
            ob_clean();
            echo $controllerIniciativaDetalhe->listaDetalhe($inp->inpid);
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'atividadeEtapas') {
            $controllerIniciativaAtividade = new Par3_Controller_Iniciativa_planejamento_atividadeetapa();
            ob_clean();
            echo $controllerIniciativaAtividade->listarAtividade($inp->inpid);
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'insumoIniciativa') {
            $controllerIniciativaInsumos = new Par3_Controller_Iniciativa_planejamento_insumosiniciativa();
            ob_clean();
            echo $controllerIniciativaInsumos->listaInsumosIniciativa($inp->inpid);
            die;
            break;
        }
        if ($_REQUEST['identificador'] == 'cargaHoraria') {
            $controllerIniciativaAtividade = new Par3_Controller_Iniciativa_planejamento_atividadeetapa();
            ob_clean();
            echo $controllerIniciativaAtividade->totalCargaHoraria($inp->inpid);
            die;
            break;
        }
    case 'buscaDescItensComposicaoFormacao':
        if ($_REQUEST['identificador'] == 'atividadeetapa') {
            $controllerIniciativaAtividade = new Par3_Controller_Iniciativa_planejamento_atividadeetapa();
            ob_clean();
            echo $controllerIniciativaAtividade->buscaDsc($_REQUEST['iddetalhe']);
            die;
            break;
        }
    case 'removerObra':
        $controllerObra = new Par3_Controller_Obra;
        ob_clean();
        $resp = $controllerObra->remover($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'salvarValorEstimado':
        $controllerObra = new Par3_Controller_Obra;
        ob_clean();
        $resp = $controllerObra->salvarValorEstimado($_REQUEST['obrid'], $_REQUEST['obrvalor']);
        echo simec_json_encode($resp);
        die;
        break;
    case 'getiniciativa':
        $inp = new Par3_Controller_IniciativaPlanejamento();
        $resp = $inp->getIniciativa($_REQUEST);
        ob_clean();
        echo simec_json_encode($resp);
        die;
        break;

    case 'getiniciativadados':
        $inp = new Par3_Controller_IniciativaPlanejamento();
        $resp = $inp->getIniciativaDados($_REQUEST);
        ob_clean();
        echo simec_json_encode($resp);
        die;
        break;

    case 'listarModalidades':
        $inp = new Par3_Controller_IniciativaPlanejamento();
        $arrMod = $inp->getIniciativaModalidades($_REQUEST);
        ob_clean();
        echo simec_json_encode($arrMod);
        die;
        break;

    case 'listarDesdobramentos':
        $inp = new Par3_Controller_IniciativaPlanejamento();
        $arrDes = $inp->getIniciativaDesdobramentos($_REQUEST);
        ob_clean();
        echo simec_json_encode($arrDes);
        die;
        break;

    case 'salvar':
        ob_clean();
        $inp = new Par3_Controller_IniciativaPlanejamento();
        $resp = $inp->salvar($_REQUEST);
        $inpic = new Par3_Controller_IniciativaPlanejamentoItemComposicao();
        $inpic->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'enviarplano':
        ob_clean();
        echo simec_json_encode($mInp->enviarPlano($_REQUEST));
        die();
        break;
    case 'getcomponentesindicador':
        ob_clean();
        require 'componentes-indicador.inc';
        die;
        break;
    case 'carregarsubmetascomponentesindicador':
        ob_clean();
        require_once 'componentes-indicador-funcoes.inc';
        echo listarSubMetasComponentesIndicador($_GET['metid'], $_GET['cont']);
        die;
        break;
    case 'buscaLinhaItemComposicao':
        ob_clean();
        $inpic = new Par3_Controller_IniciativaPlanejamentoItemComposicao();
        if($inp->obrid){
            $resp = $inpic->buscaLinhasProinfancia($_REQUEST);
        }else{
            $resp = $inpic->buscaLinha($_REQUEST);
        }
        echo $resp;
        die;
        break;
    case 'buscaModalEscolas':
        ob_clean();
        $inpice = new Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola();
        $resp = $inpice->getFormEscolas($_REQUEST);
        echo $resp;
        die;
        break;
    case 'montaFiltroIndicadorDemograficoinputSelect':
        ob_clean();
        $controllerGuiaPlanejamentoInformacaoDemograficaEducacional = new Par3_Controller_GuiaPlanejamentoInformacaoDemograficaEducacional();
        echo $controllerGuiaPlanejamentoInformacaoDemograficaEducacional->montaCampoSelectDescricaoIndicadorByIndicativa($_REQUEST['iniid'], 1);
        die;
        break;
    case 'excluirItensComposicao':
        ob_clean();
        $inpic = new Par3_Controller_IniciativaPlanejamentoItemComposicao();
        echo $inpic->excluirItem($_REQUEST);
        die;
        break;
    case 'atualizaAnosCronograma':
        ob_clean();
        $arMeses = array(
            '1' => 'Janeiro',
            '2' => 'Fevereiro',
            '3' => 'Março',
            '4' => 'Abril',
            '5' => 'Maio',
            '6' => 'Junho',
            '7' => 'Julho',
            '8' => 'Agosto',
            '9' => 'Setembro',
            '10' => 'Outubro',
            '11' => 'Novembro',
            '12' => 'Dezembro'
        );

        $sqlAnos = $mdIniciativa->sqlAnosIniciativaCombo($_REQUEST['iniid']);
        ?>
        <div class="form-group">
            <label class="col-sm-1" style="text-align: right;"> Cronograma Físico: </label>
            <?php echo $simec->select('inpcronogramamesinicial', null, $inp->inpcronogramamesinicial, $arMeses, array(), array('input-size' => '2')); ?>
            <label class="col-sm-1" style="text-align: center;"> a </label>
            <?php echo $simec->select('inpcronogramamesfinal', null, $inp->inpcronogramamesfinal, $arMeses, array(), array('input-size' => '2')); ?>
            <label class="col-sm-1" style="width: 8%;"> Ano Inicial: </label>
            <?php echo $simec->select('inpcronogramaanoinicial', '', $inp->inpcronogramaanoinicial, $sqlAnos, array(), array('input-size' => '2')); ?>
            <label class="col-sm-1" style="text-align: right;"> Ano de término: </label>
            <?php echo $simec->select('inpcronogramaano', '', $inp->inpcronogramaano, $sqlAnos, array(), array('input-size' => '2')); ?>
        </div>
        <?php
        die();
        break;
    case 'ajaxMontaTabelaIndicadorDemografico':
        ob_clean();
        $controllerGuiaPlanejamentoDemograficoEducacional = new Par3_Controller_GuiaPlanejamentoInformacaoDemograficaEducacional();
        $ide = new Par3_Model_Ide();
        $modelIniciativaIndicadorDemografico = new Par3_Model_IniciativaIniciativasEstados();
        $modelIndicadorDemografico = new Par3_Model_IndicadorDemografico();
        $ide->tipo = $tipo;
        $getDadosMontaTabelasIde = array();
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
        $getInstrumentoUnidade = $modelInstrumentoUnidade->getInstrumentoUnidadeById($_GET['inuid']);

        if (count($_POST['inidesc']) > 0) {
            $arrInd = $_POST['inidesc'];
            foreach ($arrInd as $a) {
                $getIndicadorDemografico = $modelIndicadorDemografico->getIndicadorDemograficoByCod(trim(strtolower($a)));
                if ($tipo == 'estado') {
                    $ide = new Par3_Model_Ide();
                    $ide->tipo = 'estado';
                    $ide->estuf = $getInstrumentoUnidade[0]['estuf'];
                    $controllerGuiaPlanejamentoDemograficoEducacional->headTabelaTitulo(trim(strtolower($a)));
                    $ide->getTitulo($getIndicadorDemografico['indtabcorede'], $getIndicadorDemografico['indtabuf'], $getIndicadorDemografico['indtabmunic']);
                    $controllerGuiaPlanejamentoDemograficoEducacional->footerTabelaTitulo();
                    $controllerGuiaPlanejamentoDemograficoEducacional->headTabelaTabela();
                    $ide->getTabela($getIndicadorDemografico['indtabcorede'], $getIndicadorDemografico['indtabuf'], $getIndicadorDemografico['indtabmunic']);
                    $controllerGuiaPlanejamentoDemograficoEducacional->footerTabelaTabela();
                } elseif ($tipo == 'municipio') {
                    $ide = new Par3_Model_Ide();
                    $ide->tipo = 'municipio';
                    $getInstrumentoUnidade = $modelInstrumentoUnidade->getInstrumentoUnidadeByIdMunic($_GET['inuid']);
                    $ide->estuf = $getInstrumentoUnidade['estuf'];
                    $ide->muncod = $getInstrumentoUnidade['muncod'];
                    $controllerGuiaPlanejamentoDemograficoEducacional->headTabelaTitulo(trim(strtolower($a)));
                    $ide->getTitulo($getIndicadorDemografico['indtabcorede'], $getIndicadorDemografico['indtabuf'], $getIndicadorDemografico['indtabmunic']);
                    $controllerGuiaPlanejamentoDemograficoEducacional->footerTabelaTitulo();
                    $controllerGuiaPlanejamentoDemograficoEducacional->headTabelaTabela();
                    $ide->getTabela($getIndicadorDemografico['indtabcorede'], $getIndicadorDemografico['indtabuf'], $getIndicadorDemografico['indtabmunic']);
                    $controllerGuiaPlanejamentoDemograficoEducacional->footerTabelaTabela();
                }
            }
        }
        die;
        break;
    case 'verificaExistenciaParaSalvar':
        ob_clean();
        if (isset($_POST)) {
            $existe = "t"; //se no final for igual a f = não existe, logo pode salvar, se for igual a t já existe
            if (isset($_POST['iniid']) && isset($_POST['inuid']) && isset($_POST['dimid']) && isset($_POST['etaid']) && isset($_POST['modid'])) {
                // verifica se a combinação de dimensão + iniciativa + area + modalidade já existe
                $arrayIni = $controllerIniciativaPlanejamento->verificaExistenciaParaSalvar($_POST);
                $countDimensaoIniciativaAreaModalidade = count($arrayIni);
                if ($countDimensaoIniciativaAreaModalidade > 0) {
                    //se existir
                    if (isset($arrayIni[0]['inuid'])) {
                        //verifica se no post existe os desdobramentos
                        if (isset($_POST['desid'])) {
                            foreach ($_POST['desid'] as $key => $value) {
                                $existeOsDesdobramentos = $controllerIniciativaPlanejamentoDesdobramentos->verificaExistenciaDoDesdobramentoByIdDesdobramento($_POST, $value);
                                if (!isset($existeOsDesdobramentos[0])) {
                                    $existe = "f";
                                    echo $existe;
                                    exit();
                                } elseif (isset($existeOsDesdobramentos[0])) {
                                    $existe = "t";
                                }
                            }
                            if ($existe != "f") {
                                echo "t";
                                exit();
                            }
                        }
                    } else {
                        echo "f";
                        exit();
                    }
                }
            }
            echo $existe;
            exit();
        }
        die;
        break;
    case 'getIniciativaCodDesc':
        ob_clean();
        if (isset($_POST['iniid'])) {

            $jaExiste = false;
            //Unidade possui iniciativa cadastrada ?
            $getIniciativa = $controllerIniciativa->getIniciativaById($_POST['iniid']);
            $msg = $getIniciativa['iniid'] . " - " . $getIniciativa['inddsc'];
            echo $msg;
        }

        die;
        break;
    case 'ajaxModalHistoricoAnalise':
        ob_clean();
        Par3_Controller_AnalisePlanejamentoIniciativa::modalHistorico($_REQUEST);
        die;
        break;
    case 'salvarDocumentoPlanejamento':
        ob_clean();
        $controllerIniciativaPlanejamento->salvarDocumentoPlanejamento($_POST);
        break;
    case 'removerDocumentoPlanejamento':
        ob_clean();
        $controllerIniciativaPlanejamento->removerDocumentoPlanejamento($_POST['arqid']);
        break;
    case 'downloadDocumentoPlanejamento':
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec();
        $file->getDownloadArquivo($_GET['arqid']);
        die;
        break;
    default:
        if ($_GET['inpid']) {
            //Recupera iniciativa disponível para o instrumento unidade
            $arrIniciativa = $mInp->getIniciativa(['inuid' => $_GET['inuid'], 'iniid' => $inp->iniid, 'consulta' => 'true'], true);
            //Recupera os dados da iniciativa selecionada
            $arrIniciativaDados = $mInp->getIniciativaDados(array('iniid' => $inp->iniid, 'inuid' => $_GET['inuid']));
            //Recupera as modalidades de acordo com a etapa selecionada
            $arrMod = $mInp->getIniciativaModalidades(array('iniid' => $inp->iniid, 'etaid' => $inp->etaid));
            //recupera os desdobramentos de acordo com a iniciativa,nível e etapa selecionados
            $arrDes = $mInp->getIniciativaDesdobramentoConsulta(array('iniid' => $inp->iniid, 'etaid' => $inp->etaid, 'modid' => $inp->modid), true);
            //Recupera lista de Desdobramentos
            $desdobramentos = $mInp->getDesdobramentos($inp->inpid);
        }

        if ($inp->inpid) {
            $controllerIniciativaPlanejamentoItemComposicao = new Par3_Controller_IniciativaPlanejamentoItemComposicao();
            $modelIniciativaTipoObjeto = new Par3_Model_IniciativaTiposObjeto();
            $objeto = $controllerIniciativaPlanejamentoItemComposicao->buscaObjeto($inp->inpid);
        }
        break;
}
$arrDim = $mDim->listarSelect($arrItrid);
$disabled = '';
if ($_REQUEST['inpid'] != '') {
    $disabled = 'disabled';
}

?>
<style>
    /*fix para pintar os inputs desabilitados*/
    .chosen-disabled > .chosen-single {
        background-color: #eee;
    !important;
    }
</style>

<div class="ibox">
	<div class="ibox-content">
		<div class="row">
        	<div class="col-md-12 orcamento">
			<?php
			if( $inp->inpid ){
				$obProcesso = new Par3_Controller_Processo();
				echo $obProcesso->carregaDadosOrcamentario($_GET['inuid'], $inp->inpid);
			}
            ?>
			</div>
		</div>
	</div>
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Iniciativas</h3>
            </div>
        </div>
    </div>
    <!-- Análise - Município - Responder Diligência. Historia: 15499 -->
    <div class="ibox-content hidden">
        <div class="col-md-12">
            <h3 class="center" style="color:#EEAD0E;">A Iniciativa foi Diligênciada na Análise, por favor corrigir as
                informações solicitadas.</h3>
        </div>
        <div style="margin-left: 80px; display: table;">
            <div style="float:left; width: 10%;">
                <label>Motivo da Diligência: </label>
            </div>
            <div align="justify" style="margin-left:10%;"> Lorem Ipsum é simplesmente uma simulação de texto da
                indústria tipográfica e de impressos, e vem sendo utilizado desde o século XVI, quando um impressor
                desconhecido pegou uma bandeja de tipos e os embaralhou para fazer um livro de modelos de tipos. Lorem
                Ipsum sobreviveu não só a cinco séculos, como também ao salto para a editoração eletrônica, permanecendo
                essencialmente inalterado. Se popularizou na década de 60, quando a Letraset lançou decalques contendo
                passagens de Lorem Ipsum, e mais recentemente quando passou a ser integrado a softwares de editoração
                eletrônica como Aldus PageMaker.
            </div>
            <br>
            <div style="margin-left: -14px; width: 54%; float: left;">
                <?php echo $simec->input('arqid', 'Arquivos comprobatórios', null, array('type' => 'file', null, 'style' => 'width: 344px; margin-left:58px;')); ?>
            </div>
            <div style="margin-left:10%;">
                <label>Técnico: Fulano da Silva - 04/10/2013 - 19h35</label>
            </div>
        </div>
    </div>

    <?php
     /*
      * Solicitação de mobiliário de obras, pelo programa PROINFANCIA. Dimensão será sempre 4 (infraestrutura física..).
      * Iniciativa deverá ser sempre a específica do proinfancia.
      */

     if($obrid > 0){
         $inp->dimid = $controllerIniciativa->getDadosSolicitacaoMobiliario($arrItrid['itrid']);
         $iniciativa = $controllerIniciativa->getIniciativaObraProinfancia();

         if(!$iniciativa[0]['codigo']){
             echo '<script>swal("", "Não foram encontradas iniciativas cadastradas relacionadas ao Proinfância", "warning");</script>';
         }else{
             $inp->iniid = $iniciativa[0]['codigo'];
             $arrIniciativaDados = $mInp->getIniciativaDados(array('iniid'=>$inp->iniid, 'inuid'=>$_GET['inuid']));
             $arrIniciativa= simec_preparar_array($iniciativa);
         }

     }
    ?>

    <div class="ibox-content">
        <div class="row">
            <form name="form-iniciativaplanejamento" id="form-iniciativaplanejamento" class="form-horizontal">
                <div class="row">
                    <div class="col-lg-6">
                        <input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
                        <input type="hidden" name="obridproinf" id="obridproinf" value="<?php echo $inp->obrid ?: $obrid; ?>"/>
                        <input type="hidden" name="obrid_par3" id="obrid_par3" value="<?php echo $_GET['obrid_par3']; ?>"/>
                        <input type="hidden" name="inpid" id="inpid" value="<?php echo $inp->inpid; ?>"/>
                        <?php echo $simec->select('dimid', 'Dimensão', $inp->dimid, $arrDim, array('maxlength' => '255', 'required', $disabled), array('input-size' => '8'), array('class' => '')); ?>
                        <?php echo $simec->select('iniid', 'Iniciativas', $inp->iniid, $arrIniciativa, array('maxlength' => '255', 'required', $disabled, 'onchange' => 'js_returnIdIniciativa(this.value);'), array('input-size' => '8')); ?>
                        <div class="form-group" id="cic-div">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label">Ciclo: </label>
                            <div class="col-md-8 col-md-8 col-lg-8 control-label"><p class="text-left"
                                                                                     id="cic"><?php echo $arrIniciativaDados['cic']; ?></p>
                            </div>
                        </div>
                        <div class="form-group" id="anos-div">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label">Anos: </label>
                            <div class="col-md-8 col-md-8 col-lg-8 control-label"><p class="text-left"
                                                                                     id="anos"><?php echo $arrIniciativaDados['anos']; ?></p>
                            </div>
                        </div>
                        <div class="form-group" id="into-div">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label">Tipo de
                                Objeto: </label>
                            <div class="col-md-8 col-md-8 col-lg-8 control-label"><p class="text-left"
                                                                                     id="into"><?php echo $arrIniciativaDados['into']; ?></p>
                            </div>
                        </div>
                        <div class="form-group" id="inta-div">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label">Tipo
                                de Atendimento: </label>
                            <div class="col-md-8 col-md-8 col-lg-8 control-label"><p class="text-left"
                                                                                     id="inta"><?php echo $arrIniciativaDados['inta']; ?></p>
                            </div>
                        </div>
                        <div class="form-group" id="esfera-div">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label">Esfera: </label>
                            <div class="col-md-8 col-md-8 col-lg-8 control-label"><p class="text-left"
                                                                                     id="esfera"><?php echo $arrIniciativaDados['esfera']; ?></p>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-6" id="">
                        <div class="form-group" id="iar-div">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label">Áreas
                                Relacionadas: </label>
                            <div class="col-md-8 col-md-8 col-lg-8 control-label"><p class="text-left"
                                                                                     id="iar"><?php echo $arrIniciativaDados['iar']; ?></p>
                            </div>
                        </div>
                        <div class="form-group" id="prg-div">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label">Programa: </label>
                            <div class="col-md-8 col-md-8 col-lg-8 control-label"><p class="text-left"
                                                                                     id="prg"><?php echo $arrIniciativaDados['prg']; ?></p>
                            </div>
                        </div>
                        <div class="form-group" id="pro-div">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label">Projeto: </label>
                            <div class="col-md-8 col-md-8 col-lg-8 control-label"><p class="text-left"
                                                                                     id="pro"><?php echo $arrIniciativaDados['pro']; ?></p>
                            </div>
                        </div>
                        <div class="form-group" id="niv-div">
                            <input type="hidden" name="nivid" value="1"/>
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label">Nível: </label>
                            <div class="col-md-8 col-md-8 col-lg-8 control-label"><p class="text-left">Educação
                                    Básica</p></div>
                        </div>
                        <?php echo $simec->select('etaid', 'Etapa', $inp->etaid, $arrIniciativaDados['eta'], array('maxlength' => '255', 'required', $disabled), array('input-size' => '8')); ?>
                        <?php echo $simec->select('modid', 'Modalidade', $inp->modid, $arrMod, array('maxlength' => '255', 'required', $disabled), array('input-size' => '8')); ?>
                        <?php echo $simec->select('desid[]', 'Desdobramento', $desdobramentos, simec_preparar_array($arrDes), array('maxlength' => '255', 'required', $disabled, 'title' => 'Selecione um programa', 'multiple' => 'multiple', 'placeholder' => 'Selecione'), array('input-size' => '8'));
                        ?>
                    </div>
                </div>
                <?php

                // Informações para Planejamento de Obras Proinfância
                if($obrid || $inp->obrid) {
                    $obrid = $obrid ?: $inp->obrid;

                    echo '<div style="padding: 0 10px;"> <span style="margin-top: 25px;position: absolute; font-weight: bold;">Dados da obra: </span>';

                    $controllerIniciativaPlanejamento->getDadosObraProinfancia($obrid);

                    echo '</div>';
                }
                ?>
            </form>
        </div>
    </div>
    <?php if ($_REQUEST['inpid']) { ?>
        <div class="float-e-margins" id="componentes">
            <div class="ibox-title" id="btn-componentes-indicador">
                <div class="ibox-tools">
                    <a>Opções Avançadas <i id="i-componentes-indicador" class="fa fa-chevron-down"></i></a>
                </div>
                <h3 class="">Componentes Indicador</h3>

            </div>
            <div id="div-componentes-indicador" style="<?php echo !$_GET['areid'] ? 'display: none;' : '' ?>">
                <?php
                if ($inp->inpid) {
                    $_GET['dimid'] = $inp->dimid;
                    $_GET['iniid'] = $inp->iniid;
                    require_once('componentes-indicador.inc');
                }
                ?>
            </div>
        </div>

        <div class="float-e-margins" id="informacoes-demograficas">
            <div class="ibox-title" id="btn-informacoes-demograficas">
                <div class="ibox-tools">
                    <a>Opções Avançadas <i id="i-informacoes-demograficas" class="fa fa-chevron-down"></i></a>
                </div>
                <h3 class="">Informações Demográficas e Educacionais</h3>
            </div>
            <div class="ibox-content" id="div-informacoes-demograficas" style="display: none;">
                <form class="form-horizontal" id="form_filtro_indicadores" name="form_filtro_indicadores" method="POST">
                    <div class="row">
                        <div id="field_indicador">
                            <?php //ver($_SESSION); ?>
                            <div class="col-md-3">
                                <label class="control-label active pull-right ">Descrição do Grupo/Indicador:</label>
                            </div>
                            <div class="col-md-4">
                                <style>
                                    .dropdown-menu {
                                        z-index: 10000 !important;
                                    }
                                </style>

                                <div class="form-group ">
                                    <input class="form-control" style="width:400px" id="exampleInputName2"
                                           placeholder="Selecione..." disabled="disabled" type="text">
                                </div>
                            </div>
                            <div class="col-md-3">
                                <button disabled type="button" class="btn btn-primary pull-left"
                                        id="pesquisar_indicadores" value="pesquisar_indicadores"><i
                                            class="fa fa-search"></i>
                                    Pesquisar
                                </button>
                                &nbsp;&nbsp;
                                <button type="reset" class="btn btn-success" onclick="js_limpaIndicadoresEtabelas();"
                                        name="limpar_form_indicadores" id="limpar_form_indicadores"
                                        style="margin-left: 10px;"><i class="fa fa-eraser"></i> Limpar
                                </button>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div id="tb_filtroIndicadores">
                            <!-- Resultado ajax montar tabela -->
                        </div>
                    </div>
                </form>
            </div>
        </div>

        <div class="float-e-margins" id="cronograma-execucao">
            <div class="ibox-title" id="btn-cronograma-execucao">
                <div class="ibox-tools">
                    <a>Opções Avançadas <i id="i-cronograma-execucao" class="fa fa-chevron-down"></i></a>
                </div>
                <h3 class="">Quantidades e Cronograma de Execução</h3>
            </div>
            <div class="ibox-content" id="div-cronograma-execucao" style="display: none;">
                <?php require_once('formCronogramaFisico.inc'); ?>
            </div>
        </div>

        <div class="float-e-margins" id="itens-composicao">
            <div class="ibox-title" id="btn-itens-composicao">
                <div class="ibox-tools">
                    <a>Opções Avançadas <i id="i-itens-composicao" class="fa fa-chevron-up"></i></a>
                </div>
                <h3 class="">Itens de Composição</h3>
            </div>

            <div class="ibox-content" id="div-itens-composicao">
                <?php
                if ($inp->inpid && $objeto != $modelIniciativaTipoObjeto::OBRA) {
                    require_once('formItemComposicao.inc');
                }
                ?>
                <?php
                if ($inp->inpid && $objeto == $modelIniciativaTipoObjeto::OBRA) {

                    $anos = explode(",", $arrIniciativaDados['anos']);
                    for ($cont = 0; $cont < count($anos); $cont++) {
                        $controllerObra = new Par3_Controller_Obra;
                        $controllerObra->criaAbaObra($anos[$cont], $inp->inpid, $_GET['inuid']);
                    }
                }
                ?>
                <div class="ibox float-e-margins animated modal" id="modal_obrasQuantidades" role="dialog"
                     aria-hidden="true">
                    <div class="modal-dialog">
                        <form method="post" name="formInformacoesGerais" id="formInformacoesGerais"
                              class="form form-horizontal">
                            <div class="modal-content">
                                <div class="ibox-title">
                                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                                aria-hidden="true">&times;</span></button>
                                    <h3 id="modal_titulo">Informações Gerais da Iniciativa- Incluir</h3>
                                </div>
                                <div class="ibox-content" id="conteudo-modal-informacoes">
                                    <?php //echo // $controllerIniciativaInformacoes->formInformacoesGerais($_REQUEST);//CHAMA FORM PASSANDO ID DA INICIATIVA ?>
                                </div>
                                <div class="ibox-footer">
                                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                                        <button type="submit" id="cancelarInformacoesGerais" data-dismiss="modal" class="btn btn-default"> <i class="fa fa-times-circle-o"></i> Cancelar</button>
                                        <button type="submit" id="salvarInformacoesGerais" class="btn btn-success"> <i class="fa fa-plus-square-o"></i> Salvar </button>
                                    </div>
                                </div>
                            </div>
                        </form>
                    </div>
                </div>
            </div>
        </div>
    <?php } ?>
<?php
	$obAnalise = new Par3_Controller_AnalisePlanejamentoIniciativa();
	$arrDados = $obAnalise->carregarAnalisePlanejamentoDiligencia($inp->inpid, false);
	
if( !empty($arrDados[0]['anaid']) ){
	
	$attribus = array('maxlengh' => 4000, 'readonly' => 'readonly');
	$attribusCombo = array('maxlength' => '255', '', '', 'text-align' => 'left', 'disabled' => 'disabled');
	$disabledText = 'disabled';
	?>
	
	<div class="float-e-margins" id="parecer-analise">
            <div class="ibox-title" id="btn-parecer-analise">
                <div class="ibox-tools">
                    <a>Opções Avançadas <i id="i-parecer-analise" class="fa fa-chevron-down"></i></a>
                </div>
                <h3 class="">Parecer da Análise</h3>
            </div>
            <div class="ibox-content" id="div-parecer-analise" style="display: none;">
<?php 			foreach ($arrDados as $analise) {
				echo '<div class="float-e-margins" id="parecer-analise-'.$analise['anaano'].'">
				            <div class="ibox-title" id="btn-parecer-analise-'.$analise['anaano'].'">
				                <div class="ibox-tools">
				                    <a>Opções Avançadas <i id="i-parecer-analise-'.$analise['anaano'].'" class="fa fa-chevron-down"></i></a>
				                </div>
				                <h3 class="">'.$analise['anaano'].'</h3>
				            </div>
			            	<div class="ibox-content" id="div-parecer-analise-'.$analise['anaano'].'" style="display: none;">';
							echo $simec->textarea('anaconsideracaoent', 'Considerações sobre a Entidade Proponente', $analise['anaconsideracaoent'], $attribus, array('label-size' => '2'));
							echo $simec->textarea('anaconsideracaoprop', 'Considerações sobre a Proposta', $analise['anaconsideracaoprop'], $attribus, array('label-size' => '2'));
							echo $simec->textarea('anaconsideracaoproj', 'Considerações sobre o Projeto', $analise['anaconsideracaoproj'], $attribus, array('label-size' => '2'));
							echo $simec->textarea('anaconsideracaoobj', 'Considerações sobre o Objetivo', $analise['anaconsideracaoobj'], $attribus, array('label-size' => '2'));
							echo $simec->textarea('anaconsideracaojus', 'Considerações sobre a Justificativa', $analise['anaconsideracaojus'], $attribus, array('label-size' => '2'));
							echo $simec->textarea('anaconsideracaoval', 'Considerações sobre os Valores', $analise['anaconsideracaoval'], $attribus, array('label-size' => '2'));
							echo $simec->textarea('anaoutrasconsideracoes', 'Outras Considerações Cabíveis', $analise['anaoutrasconsideracoes'], $attribus, array('label-size' => '2'));
							echo $simec->textarea('anatextoparecer', 'Texto do parecer', $analise['anatextoparecer'], $attribus, array('label-size' => '2'));
							$arParecer = array('A' => 'Aprovado',
									'R' => 'Reprovado',
									'D' => 'Diligência'
							);
							echo $simec->select('anaparecer', 'Situação do Parecer', $analise['anaparecer'], $arParecer, $attribusCombo, array('label-size' => '2'));

							echo '<div class="form-group ">
                                    <div class="col-sm-10 col-md-10 col-lg-10 col-sm-offset-2 col-md-offset-2 col-lg-offset-2">
                                      <button type="button" class="btn btn-info" onclick="exibeHistoricoAnalise( '. $analise['inpid'] . ', \''.$analise['anaano'].'\');"><span class="glyphicon glyphicon-time" aria-hidden="true"></span> Histórico de análise(s) em '.$analise['anaano'].'</button>
                                    </div>
                                </div>
                                <div style="clear:both"></div>';
				echo '</div>
			          </div>';
}?>
            </div>
        </div>

    <?php
}
if($inp->inpid > 0){
?>
    <div class="float-e-margins" id="arquivos">
        <div class="ibox-title" id="btn-arquivos">
            <div class="ibox-tools">
                <a>Opções Avançadas <i id="i-arquivos" class="fa fa-chevron-up"></i></a>
            </div>
            <h3 class="">Arquivos</h3>
        </div>
        <div class="ibox-content" id="div-arquivos">
            <?php
                $controllerIniciativaPlanejamento->getListaArquivosPlanejamento($inp->inpid);
            ?>
        </div>
    </div>
<?php } //fim lista de arquivos ?>

    <div class="ibox-footer">
        <div class="center">

            <?php
                //Mostra botão salvar somente se o estado da iniciativa for "Em Cadastramento" ou  "Em dirigência" ou esta Cadastrando uma iniciativa
                if(($mdDocPl->esdid == PAR3_ESDID_CADASTRAMENTO || $mdDocPl->esdid == PAR3_ESDID_EM_DILIGENCIA) || $mdDocPl->esdid == null ) {
                    //Libera o botão salvar caso o planejamento esteja  da uninade não tenha sido iniciado ou já esteja em elaboração e a iniciativa esteja em cadastramento ou em dirigência
                    if (($modelDocumento->esdid == PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO || $modelDocumento->esdid == PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO) && ($mdDocPl->esdid == PAR3_ESDID_CADASTRAMENTO || $mdDocPl->esdid == PAR3_ESDID_EM_DILIGENCIA || $mdDocPl->esdid == null)) {
                        ?>
                        <button type="button" class="btn btn-success" id="btn-incluir-iniplanejamento"><i
                                    class="fa fa-plus"></i> Salvar
                        </button>
                    <?php } else { ?>
                        <button type="button" class="btn btn-success" onclick="js_travaSalvar();"><i
                                    class="fa fa-plus"></i>
                            Salvar
                        </button>
                    <?php }
                }?>

            <?php
            $temPendencia = false;
            if( $_REQUEST['inuid'] && $inp->iniid ){
                $obPendenciaBloqueio = new Par3_Controller_Pendencia();
                $temPendencia = $obPendenciaBloqueio->verificaPendenciaBloqueio('planejamento', $_REQUEST['inuid'], $inp->iniid, '', false, true, true, true, $inp->inpid);
            }
            //ver($temPendencia);
            /*
             * Verifica se o municipio ou o estado não precisa validar as pendecias para finalizar a iniciativa*/
            if( $temPendencia == false ) $existePendenciaRestricao = 0;
			
            //Mostra botão Finalizar somente se o estado da iniciativa for "Em Cadastramento" ou  "Em dirigência"
            if($mdDocPl->esdid == PAR3_ESDID_CADASTRAMENTO || $mdDocPl->esdid == PAR3_ESDID_EM_DILIGENCIA) {
                if ($inp->inpid) {
                    //Botão Finalizar não aparce para iniciativas do tipo Obra
                    if ($inp->inpid && $objeto != $modelIniciativaTipoObjeto::OBRA) {
                        //lIBERA o botão finalizar caso a unidade NÃO tenha restrições ou pendencias  E  iniciativa  seja IGUAL  em cadastramento
                        if (($mdDocPl->esdid == PAR3_ESDID_CADASTRAMENTO || $mdDocPl->esdid == PAR3_ESDID_EM_DILIGENCIA) && $existePendenciaRestricao == 0) { ?>

							<button type="button" class="btn btn-primary" id="btn-enviarplano"
                                    inpid="<?php echo $inp->inpid; ?>"><i class="fa fa-check"></i>Finalizar Iniciativa
                            </button>
                            
                        <?php } else { ?>
                            <button type="button" class="btn btn-primary" onclick="js_travaFinalizar();"><i
                                        class="fa fa-check"></i>Finalizar Iniciativa
                            </button>
                        <?php }
                    }
                }
            }?>
            <button type="reset" class="btn btn-default" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
        </div>
    </div>
</div>
</div>
<div id="biscuits">


</div>

<!--modal histórico de análise-->
<div class="ibox float-e-margins animated modal" id="modal_historico_analise" role="dialog" aria-hidden="true">
    <div class="modal-dialog" style="width:800px;">
        <form method="post" name="formTramitarSituacao" id="formTramitarSituacao" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo_historico"></h3>
                </div>
                <div class="ibox-content" id="conteudo-modal-historico" style="overflow: auto;"></div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal"
                                class="btn btn-danger">
                            <i class="fa fa-times-circle-o"></i> Fechar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<!--modal anexo de arquivos-->
<div class="ibox float-e-margins animated modal" id="modal_anexo_arquivos" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                <h3 id="modal_titulo">Documentos Anexo - Incluir</h3>
            </div>
            <div class="ibox-content" id="conteudo-modal">
                <?php
                    echo $simec->input('documentoPlanejamento', 'Anexo', null, array('type' => 'file'));
                    echo $simec->input('ipadsc', 'Descrição', null, array('maxlength'=>100));
                ?>
            </div>
            <div class="ibox-footer">
                <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                    <button type="button" id="cancelar" data-dismiss="modal"
                            class="btn btn-default">
                        <i class="fa fa-times-circle-o"></i> Cancelar
                    </button>
                    <button type="button" id="salvarArquivo"
                            class="btn btn-success" onclick="salvarDocumentoPlanejamento()">
                        <i class="fa fa-plus-square-o"></i> Salvar
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>

<script>

    //Ajuste dos select que estavam quebrando...
    $('.control-label').removeClass('col-lg-3');

    function js_buscaObrid(obrid, obrvalor) {

        obrvalor = obrvalor.replace(".", ""); // retira ponto
        obrvalor = obrvalor.replace(",", ""); // retira virgula

        if (obrvalor == "") { // seta null para vazio
            obrvalor = null;
        }

        var caminho = window.location.href;
        var action = '&requisicao=salvarValorEstimado&obrid=' + obrid + '&obrvalor=' + obrvalor;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {


            }
        });

    }


    //mostra/esconde ibox
    jQuery("#btn-componentes-indicador").click(function () {
        $('#div-componentes-indicador').slideToggle();
        //chevron up/down
        $('#i-componentes-indicador').toggleClass(function () {
            if ($('#i-componentes-indicador').is(".fa-chevron-down")) {
                $('#i-componentes-indicador').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-componentes-indicador').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    //mostra/esconde ibox
    jQuery("#btn-informacoes-demograficas").click(function () {
        $('#div-informacoes-demograficas').slideToggle();
        //chevron up/down
        $('#i-informacoes-demograficas').toggleClass(function () {
            if ($('#i-informacoes-demograficas').is(".fa-chevron-down")) {
                $('#i-informacoes-demograficas').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-informacoes-demograficas').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    //mostra/esconde ibox
    jQuery("#btn-cronograma-execucao").click(function () {
        $('#div-cronograma-execucao').slideToggle();
        //chevron up/down
        $('#i-cronograma-execucao').toggleClass(function () {
            if ($('#i-cronograma-execucao').is(".fa-chevron-down")) {
                $('#i-cronograma-execucao').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-cronograma-execucao').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });
    
    jQuery("#btn-parecer-analise").click(function () {
        $('#div-parecer-analise').slideToggle();
        //chevron up/down
        $('#i-parecer-analise').toggleClass(function () {
            if ($('#i-parecer-analise').is(".fa-chevron-down")) {
                $('#i-parecer-analise').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-parecer-analise').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });
    jQuery("#btn-parecer-analise-2017").click(function () {
        $('#div-parecer-analise-2017').slideToggle();
        //chevron up/down
        $('#i-parecer-analise-2017').toggleClass(function () {
            if ($('#i-parecer-analise-2017').is(".fa-chevron-down")) {
                $('#i-parecer-analise-2017').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-parecer-analise-2017').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });
    
    jQuery("#btn-parecer-analise-2018").click(function () {
        $('#div-parecer-analise-2018').slideToggle();
        //chevron up/down
        $('#i-parecer-analise-2018').toggleClass(function () {
            if ($('#i-parecer-analise-2018').is(".fa-chevron-down")) {
                $('#i-parecer-analise-2018').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-parecer-analise-2018').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });
    jQuery("#btn-parecer-analise-2019").click(function () {
        $('#div-parecer-analise-2019').slideToggle();
        //chevron up/down
        $('#i-parecer-analise-2019').toggleClass(function () {
            if ($('#i-parecer-analise-2019').is(".fa-chevron-down")) {
                $('#i-parecer-analise-2019').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-parecer-analise-2019').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });
    jQuery("#btn-parecer-analise-2020").click(function () {
        $('#div-parecer-analise-2020').slideToggle();
        //chevron up/down
        $('#i-parecer-analise-2020').toggleClass(function () {
            if ($('#i-parecer-analise-2020').is(".fa-chevron-down")) {
                $('#i-parecer-analise-2020').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-parecer-analise-2020').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });
    
    //mostra/esconde ibox
    jQuery("#btn-itens-composicao").click(function () {
        $('#div-itens-composicao').slideToggle();
        //chevron up/down
        $('#i-itens-composicao').toggleClass(function () {
            if ($('#i-itens-composicao').is(".fa-chevron-down")) {
                $('#i-itens-composicao').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-itens-composicao').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    //mostra/esconde ibox arquivos
    jQuery("#btn-arquivos").click(function () {
        $('#div-arquivos').slideToggle();
        //chevron up/down
        $('#i-arquivos').toggleClass(function () {
            if ($('#i-arquivos').is(".fa-chevron-down")) {
                $('#i-arquivos').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-arquivos').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    //controle da classe has-error para o campo de detalhamento
    $('#inpitemcomposicaodetalhamento').on('keyup', function(){
        var formgroup = $(this).closest('.form-group');
        if($(formgroup).hasClass('has-error')){
            if($(this).val() != ''){
                $(this).closest('.form-group').removeClass('has-error');
            }
        }
    });

    $(document).ready(function () {
        $("#form-iniciativaplanejamento").on('change', "#dimid", function () {
            var dimid = $("#form-iniciativaplanejamento").find("#dimid").val();
            var caminho = window.location.href;
            var options = $('#form-iniciativaplanejamento').find("#iniid");
            var action = '&requisicao=getiniciativa&dimid=' + dimid;
            console.log(dimid);
            limparCampos();
            options.empty();
            if (dimid == '') {
                return false;
            }
            $.getJSON(caminho + action, function (dados) {
                if (!dados) {
                    options.append(new Option("", ""));
                } else {
                    options.append(new Option("Selecione", ""));
                    $.each(dados, function (index, ini) {
                        console.log(index + " - " + ini);
                        options.append(new Option(ini, index));
                    });
                }
                options.focus();
                options.trigger('chosen:updated');
            });
        });

        //busca dados da iniciativa e etapas
        $("#form-iniciativaplanejamento").on('change', "#iniid", function () {
            var iniid = $('#form-iniciativaplanejamento').find("#iniid").val();
            var dimid = $('#form-iniciativaplanejamento').find("#dimid").val();

            var caminho = window.location.href;
            var action = '&requisicao=getiniciativadados&iniid=' + iniid;
            var options = $("#form-iniciativaplanejamento").find('#etaid');
            console.log('iniciativa:' + iniid);
            if (iniid == '') {
                limparCampos();
                return false;
            }
            options.empty();
            $.ajax({
                type: "GET",
                url: caminho,
                data: action,
                async: true,
                success: function (resp) {
                    var res = $.parseJSON(resp);
                    console.log(res);
                    var etapas = '';
                    $.each(res, function (index, dado) {
                        if (index == 'eta') {
                            etapas = dado;
                            console.log(etapas);
                        }
                        $('#' + index + '').html(dado);
                    });

                    if (!etapas) {
                        options.append(new Option("", ""));
                    } else {
                        options.append(new Option('Selecione', '', 'disabled'));
                        $.each(etapas, function (key, index) {
                            options.append(new Option(index, key));
                        });
                    }
                    options.focus();
                    options.trigger('chosen:updated');
                    buscarComponentesIndicadorIniciativa(iniid, dimid);
                    return false;
                }
            });
             });

        //busca por modalidades
        $("#form-iniciativaplanejamento").on('change', "#etaid", function () {
            var form = $("#form-iniciativaplanejamento");
            var etaid = form.find("#etaid").val();
            var iniid = form.find("#iniid").val();
            var caminho = window.location.href;
            var options = form.find("#modid");
            var action = '&requisicao=listarModalidades&etaid=' + etaid + '&iniid=' + iniid;
            if (etaid == '') {
                return false;
            }
            $.getJSON(caminho + action, function (dados) {
                options.empty();
                if (!dados) {
                    options.append(new Option("", ""));
                } else {
                    options.append(new Option('Selecione', '', 'disabled'));
                    $.each(dados, function (index, mod) {
                        options.append(new Option(mod, index));
                    });
                }
                options.focus();
                options.trigger('chosen:updated');
            });
        });

        $(document).on('click', '.btn-meta-componente-indicador', function () {
            var meta = $(this).attr('meta');
            var cont = $(this).attr('area');

            var abaMetas = $('#div-metasaba' + cont);
            abaMetas.find('.btn').removeClass('btn-primary-active').addClass('btn-primary');
            abaMetas.find('#btn-meta' + meta).removeClass('btn-primary').addClass('btn-primary-active');
            var action = '&requisicao=carregarsubmetascomponentesindicador&metid=' + meta + '&cont=' + cont;
            var path     = window.location.href;
            $('#div-submetas' + cont).load(path + action);
            console.log(path + action);
        });


        //busca desdobramentos
        $("#form-iniciativaplanejamento").on('change', "#modid", function () {
            var form = $("#form-iniciativaplanejamento");
            var etaid = form.find("#etaid").val();
            var modid = form.find("#modid").val();
            var iniid = form.find("#iniid").val();
            var caminho = window.location.href;
            var options = form.find("#desid");
            var action = '&requisicao=listarDesdobramentos&etaid=' + etaid + '&iniid=' + iniid + '&modid=' + modid;
            if (modid == '') {
                return false;
            }
            $.getJSON(caminho + action, function (dados) {
                options.empty();
                if (!dados) {
                    options.append(new Option("", ""));
                } else {
                    $.each(dados, function (index, des) {
                        options.append(new Option(des, index));
                    });
                }
                options.focus();
                options.trigger('chosen:updated');
            });
        });
        setTimeout(jQueryShowBtnProxReturn, 1000); //desabilita o btn proseguir do menu quando entram diretamente no planejamento

    });

    $('#btn-incluir-iniplanejamento').click(function (evt) {

        evt.preventDefault();

        var inpid = $('#inpid').val();

        //valida os campos só no cadastro
        if(inpid == '' && !verificarCamposVazios()){
            return false;
        }

        var inprgid = $('#form-iniciativaplanejamento').find('[name=inpid]').val();
        var msg = 'Incluir';
        if (inprgid) {
            msg = 'Alterar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este Planejamento?",
                type: "success",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    js_verificaExistencia();
                }
                return false;
            }
        );

    });

    <?php if($inp->inpid):?>
    $("#btn-enviarplano").click(function (evt) {
        //Verifica se o campo valor total = 0
        if (document.getElementById('valorTotalHidden').value == 0) {
            swal("Ação não Permitida!", "Iniciativa sem Item de Composição Cadastrado!", "error");
            return false
        }
        var campoDetalhamento = document.getElementById('inpitemcomposicaodetalhamento');
        if(document.getElementById('obridproinf').value > 0 && trim(campoDetalhamento.value) == ''){
            swal("Ação não Permitida!", "O detalhamento é obrigatório!", "error");
            $(campoDetalhamento).closest('.form-group').addClass('has-error');
            $(campoDetalhamento).focus();
            return false;
        }

        // salva antes de finalizar
        salvarPlanejamento();

        evt.preventDefault();
        var inpid = $(this).attr('inpid');
        var caminho = window.location.href;
        var action = 'requisicao=enviarplano&inpid=' + inpid;
        swal({
                title: "<b>Finalizar Iniciativa</b>",
                text: "<b>Tem certeza que deseja finalizar esta iniciativa?</b><br>Não será possível editar as informações após finalizada.",
                type: "warning",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    $.ajax({
                        type: "POST",
                        url: caminho,
                        data: action,
                        async: false,
                        success: function (resp) {
                            var res = $.parseJSON(resp);
                            console.log(res);
                            if (!isNaN(parseInt(res)) && isFinite(res)) {
                                msgSuccess('<?php echo $url; ?>&aba=consulta', "Iniciativa Finalizada Com Sucesso!");
                            }
                            return false;
                        }
                    });
                }
                return false;
            }
        );
    });
    <?php endif;?>
    $('#limpar').click(function () {
        window.location = '<?php echo $url . '&aba=cadastro&inpid=' . $_GET['inpid']
                                           . ($_GET['obrid'] ? '&obrid='.$_GET['obrid'] : '')
                                           . ($_GET['obrid_par3'] ? '&obrid_par3='.$_GET['obrid_par3'] : '') ?>';
    });

    $('#btn_gerar_novo').click(function () {
        window.location = '<?php echo $url . '&aba=cadastro'; ?>';
    });

    function salvarPlanejamento() {
        var form = $('#form-iniciativaplanejamento').serialize();
        var formCronograma = $('#form-coronograma-iniciativa').serialize();
        var formItemComposicao = $('#form-itens-composicao').serialize();

        var caminho = window.location.href;
        var action = '&requisicao=salvar&' + form + '&' + formCronograma + '&' + formItemComposicao;

        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //verfica se existe a combinação de dimensão  + inciativa + etapa + modalidade, pois a mesma não pode existir

                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    msgSuccess(caminho + '&inpid=' + parseInt(res), "Planejamento salvo com sucesso");
                    return false;
                }
                swal.close();
                $.each(res, function (index, erro) {//retorna mensagens de erro em json
                    $.each(erro, function (err, errMsg) {
                        if (errMsg < err.length) {
                            return;//se a mensagem for vazia não retorna nada
                        }
                        var divFormInput = $("#form-iniciativaplanejamento").find("#" + err).parent("div");
                        if (divFormInput.length == 0) {
                            divFormInput = $("#form-coronograma-iniciativa").find("#" + err).parent("div");
                            $('#div-cronograma-execucao').show(300);
                        }
                        divFormInput.closest('.form-group').addClass('has-error');
                        divFormInput.append("<div id='div-" + err + "'></div>");
                        $("#div-" + err).html("<span class='danger'>" + errMsg + "</span>");
                    });
                });
                return false;


            }
        });
        return false;
    }

    function buscarComponentesIndicadorIniciativa(iniid, dimid, url = '') {
        var caminho = window.location.href;
        var action = '&requisicao=getcomponentesindicador&iniid=' + iniid + '&dimid=' + dimid + url;
        $('#div-componentes-indicador').load(caminho + action);
    }

    $(document).on('click', '.menuIndicadores', function () {
        var dimid = $(this).attr('dimid');
        var areid = $(this).attr('areid');
        var indid = $(this).attr('indid');
        var form = $("#form-iniciativaplanejamento");
        var iniid = form.find("#iniid").val();
        var indidAntigo = $('#indid').val();
        var url = '<?php echo($_GET['inpid'] ? '&inpid=' . $_GET['inpid'] : ''); ?>' + '&areid=' + areid + '&indid=' + indid;
        // console.log(url);
        if (indid != indidAntigo) {
            buscarComponentesIndicadorIniciativa(iniid, dimid, url);
        }
    });

    function limparCampos() {
        $('#cic').html('');
        $('#anos').html('');
        $('#into').html('');
        $('#inta').html('');
        $('#esfera').html('');
        $('#prg').html('');
        $('#iar').html('');
        $('#pro').html('');
        $('#iniid').val('').trigger('chosen:updated');
        $('#nivid').val('').trigger('chosen:updated');
        $('#etaid').val('').trigger('chosen:updated');
        $('#modid').val('').trigger('chosen:updated');
        $('#desid').val('').trigger('chosen:updated');
        $('#div-componentes-indicador').html('');
    }

    /**
     * @author LeoKenzley
     * @description Monta um campo do tipo select com muliplas opções de indicadores ligados a iniciativa
     */
    function js_returnIdIniciativa() {
        var iniid = $("#iniid").val();
        if (iniid != '') {
            var caminho = window.location.href;
            var action = '&requisicao=montaFiltroIndicadorDemograficoinputSelect&iniid=' + iniid;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                beforeSend: function () {
                        $('.loading-dialog-par3').show();
                     },
                async: true,
                success: function (resp) {
                    $('#field_indicador').html(resp);
                    $("#tb_filtroIndicadores").html(" ");
                }
            });
            action = '&requisicao=atualizaAnosCronograma&iniid=' + iniid;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: true,
                success: function (resp) {
                    $('#form-coronograma-iniciativa').html(resp);
                },
                complete: function(){
                     $('.loading-dialog-par3').hide();
                },
            })
        }
    }
    /**
     * Funcionalidade que pega a ação de excluir item de composicao
     */

    $("#form-itens-composicao").on('click', '.removerItem', function () {
        var btnClicada = $(this).parent().parent().parent();
        var id = $(this).data('id');

        swal({
            title: "Remover Item de Composição",
            text: "Tem certeza que deseja remover este Item de Composição?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var iigid = id;
                var inpid = $("#inpid").val();
                var caminho = window.location.href;
                var action = '&requisicao=excluirItensComposicao&iigid=' + iigid + '&inpid=' + inpid;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        swal('Sucesso', 'Item excluído com sucesso!', 'success');
                        btnClicada.remove();
                        atualizaListagemItensComposicao();
                    }
                });
            }
        })
    });

    $(document).on('change', '[name^="inpcronogramaano"]', function () {
        var inicio = $('#inpcronogramaanoinicial').val();
        var fim = $('#inpcronogramaano').val();

        if (inicio != '' && fim != '') {
            if (parseFloat(inicio) > parseFloat(fim)) {
                swal('Erro', 'O ano inicial deve ser menor ou igual ao ano final', 'error');
                $('#inpcronogramaano').val('').trigger("chosen:updated");
            }
        }
    });

    /**
     * @author LeoKenzley
     * @description Monta as tabelas dos indicadores demográficos e apresenata detro da div "tb_filtroIndicadores"
     */
    function ajax_getIndicadoresMontarTabela() {
        var caminho = window.location.href;
        var iniid = $("#iniid").val();
        $.ajax({
            url: caminho,
            type: "POST",
            data: '&requisicao=ajaxMontaTabelaIndicadorDemografico&iniid=' + iniid + "&" + $("#form_filtro_indicadores").serialize(),
            success: function (resp) {
                $("#tb_filtroIndicadores").html(resp);
            }
        });
    }
    /**
     * @author LeoKenzley
     * @description para limpar os dados do campo do tipo select onde consta os indicadores que estão ligados a iniciativa,
     * esta funçãpo
     */
    function js_limpaIndicadoresEtabelas() {
        js_returnIdIniciativa();
        var resp = " ";
        $("#tb_filtroIndicadores").html(resp);
    }

    /**
     * @author Hmerson Morais
     * @description pRemove Obra
     *
     */

    function removerObra(id) {
        swal({
            title: "Remover Obra",
            text: "Tem certeza que deseja remover?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var caminho = window.location.href;
                var action = '&requisicao=removerObra&obrid=' + id;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                    
                        if (resp == '"erro_situacao"')
						{
                            swal("Erro.", "Somente Obras na situação 'Em cadastramento' podem ser removidas.", "error");
                            return false;
                        }
                        if (( resp != '"erro"' ) && ( resp != '"erro_situacao"' ))
                        {
                            msgSuccess(caminho, "Obra Removido com Sucesso");
                            return false;
                        }
                        swal("Erro.", "Ocorreu um erro ao remover a Atividade.", "error");
                    }
                });
            }
        })
    }

    /**
     * @author Leo Kenzley <leo.oliveira@cast.com.br>
     * @description esta função verifica a existencia do desdobramento + dimensão + iniciativa + área + modalidade
     */
    function js_verificaExistencia() {
        var dadosForm = $('#form-iniciativaplanejamento').serialize();
        var caminho = window.location.href;
        var iniid = $("#iniid").val();
        var action = '&requisicao=verificaExistenciaParaSalvar&' + dadosForm;
        var inpid = '<?php echo $_REQUEST['inpid']?>';

        if (inpid != '') {
            salvarPlanejamento();
        } else {
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
            }).done(function (resp) {
                if (resp == "f") { //não está cadastrada ainda
                    salvarPlanejamento();
                }

                if (resp == "t") { //já está cadastrada ainda
                    var msgIniciativa = js_montaMsgIniciativaCodDesc();
                    swal("Erro.", "Já existe Iniciativa com as mesmas informações (Dimensão, Etapa, Modalidalidade e Desdobramentos) cadastradas, " + msgIniciativa, "error");
                }
                return resp;
            });
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@cast.com.br>
     * @description esta função recupera código e descrição da iniciativa pelo id
     */
    function js_montaMsgIniciativaCodDesc() {
        var iniid = $("#iniid").val();
        var action = '&requisicao=getIniciativaCodDesc&iniid=' + iniid;
        var caminho = window.location.href;
        var result = "";
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                result = resp;
            },
        });
        return result;
    }

    function js_travaSalvar() {
        swal("Ação não Permitida!", "Esta unidade não iniciou o planejamento ou já foi finalizado!", "error");
    }

    function js_travaFinalizar() {
        swal("Ação não Permitida!", "Não é possível finalizar o planejamento. Para Finalizar, será necessário resolver as Pendências do PAR", "error");
    }

    /**
     * @author Djalma Rodrigues <djalma.rodrigues@mec.gov.br>
     * Exibe a modal com o histórico de análises
     * @param inpid
     * @param anaano
     */
    function exibeHistoricoAnalise( inpid, anaano ){
        var caminho = window.location.href;
        var action =  '&requisicao=ajaxModalHistoricoAnalise&inpid=' + inpid + '&anaano=' + anaano;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var obj = JSON.parse(resp);
                $('#conteudo-modal-historico').html( obj.html );
                $('#modal_titulo_historico').text( "Histórico de análise(s) em " + anaano );
                $('#modal_historico_analise').modal();
            }
        });
    }

    function anexarDocumentoPlanejamento() {
        $("#modal_anexo_arquivos").modal("show");
    }

    function alertaQuantidadeMaxima(){
        swal({
                title: "Atenção",
                html: true,
                text: "Não é possível anexar mais documentos, a quantidade máxima de arquivos permitia é 5.",
                type: "warning",
                allowEscapeKey: false
            },
            function() {
            }
        );
    }

    function salvarDocumentoPlanejamento() {

        var tamanhoMaximo = 5 * 1024 * 1024;
        var ipadsc = $('#ipadsc').val();

        if($('#documentoPlanejamento').val() == '') {
            alert('Adicione um arquivo');
            return false;
        }

        var file = $("#documentoPlanejamento")[0].files[0];

        if(file.size > tamanhoMaximo){
            alert("O arquivo não pode ser maior que 5MB!");
            return false;
        }

        var formData = new FormData();
        formData.append('documentoPlanejamento', file);
        formData.append('ipadsc', ipadsc);
        formData.append('requisicao', 'salvarDocumentoPlanejamento');

        jQuery.ajax({
            url: window.location.href,
            data: formData,
            cache: false,
            contentType: false,
            processData: false,
            method: 'POST',
            type: 'POST', // For jQuery < 1.9
            success: function (data) {
                if(trim(data) === 'emptyfile'){
                    swal("Erro", "Adicione um arquivo!", "error");
                }else if(trim(data) === 'maxsize'){
                    swal("Erro", "Por favor, adicione um arquivo de no máximo 5MB.", "error");
                }else if(trim(data) === 'error'){
                    swal("Erro", "Erro ao salvar arquivo, tente novamente ou contate o administrador do sistema.", "error");
                }else{
                    $("#modal_anexo_arquivos").modal('hide');
                    $('#documentoPlanejamento').val('');
                    $('#ipadsc').val('');
                    $('#ipaobs').val('');
                    $('#div-arquivos').html(data);
                    swal("Sucesso", "Arquivo salvo com sucesso!", "success");
                }
            }
        });
    }

    function removerDocumentoPlanejamento(arqid){
        swal({
                title: "Confirmar",
                text: "Deseja <b>Remover</b> esse Documento?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim"
            }, function (isConfirm) {
                if (isConfirm) {

                    $.post(window.location.href, {
                            requisicao: "removerDocumentoPlanejamento",
                            arqid: arqid
                         },
                        function (data) {
                        if(trim(data) === 'error'){
                            swal("Falha", "Erro ao excluir, tente novamente ou contate o administrador do sistema.", "error");
                            return false;
                        }else{
                            $('#div-arquivos').html(data);
                            swal("Sucesso", "Arquivo removido com sucesso!", "success");
                        }
                    });

//                    $('#requisicao').val('removerDocumentoPlanejamento');
//                    $('#arqidDocumentoPlanejamento').val(arqid);
//                    $('#formDocumentoPlanejamento').submit();

                }
                return false;
            }
        );
    }

    function downloadDocumentoPlanejamento(arqid){
        window.location.href = window.location.href+'&requisicao=downloadDocumentoPlanejamento&arqid=' + arqid;
    }

    function verificarCamposVazios(){
        var campos = document.getElementById('form-iniciativaplanejamento').querySelectorAll("[required]");
        var erro = 0;
        $.each(campos, function(index, campo){
            if($(campo).val() == null || $(campo).val() == '') {
                erro++;
                $(campo).closest('.form-group').addClass('has-error');
                $(campo).closest('div').append("<div id='div-" + $(campo).attr('id') + "'></div>");
                $("#div-" + $(campo).attr('id')).html("<span class='danger'>O campo não pode ser vazio</span>");
            }else{
                var formGroup = $(campo).closest('.form-group');
                if(formGroup.hasClass('has-error')){
                    formGroup.removeClass('has-error');
                    $('#div-' + $(campo).attr('id')).html('');
                }
            }
        });

        if(erro > 0){
            swal({
                title: "Atenção",
                text: "Por favor, preencha os campos obrigatórios!",
                type: "warning"
            });
            return false;
        }
        return true;
    }

    //Carrega os indicadores demográficos, quando a tela é carregada
    js_returnIdIniciativa();

</script>

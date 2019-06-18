<?php
$modelUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

if (!$arrPermissao || !$arrPermissao['booVisualizar']) {
    simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
}

switch ($_REQUEST['requisicao']) {
	case 'salvarItemComposicaoEscolas':
		ob_clean();
		$inpice = new Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola();
		$resp = $inpice->salvar($_REQUEST);
		echo $resp;
	die;
	break;
	case 'salvarUnicoItemComposicaoEscolas':
		ob_clean();
		$inpice = new Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola();
		$resp = $inpice->salvar($_REQUEST);
		echo $resp;
	die;
	break;
	case 'atualizarValorItemComposicao':
		ob_clean();
		$controlleINC = new Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola();
		echo $controlleINC->atualizarValorItemComposicao($_REQUEST);
	die;
	break;
        case 'ajaxImprimePlanejado':
            ?>
            <div class="modal-dialog modal-lg" style="width: 90%;">
                <div class="modal-content animated">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal">
                            <span aria-hidden="true">×</span>
                            <span class="sr-only">Fechar</span>
                        </button>
                    </div>
                    <div class="modal-body">
                        <?php require_once "planejamento/impressao-planejado.inc"; ?>
                    </div>
                </div>
            </div>
            <?php
            die;
            break;
}
switch ($_REQUEST['requisicao_analise']) {
	case 'salvar-acao-workflow':
		$anaid = $_REQUEST['anaid'];
		$obAnalise = new Par3_Controller_AnalisePlanejamentoIniciativa($anaid);
		$arrRegistro = array();
		parse_str($_REQUEST['form_itens_composicao'], $arrRegistro);
		$_REQUEST['form-item-composicao'] =  $arrRegistro;
		
		$obAnalise->executar_acao_workflow_analise($_REQUEST);
		die();
	break;
	case 'salvar-analise':
		$anaid = $_REQUEST['anaid'];
		$obAnalise = new Par3_Controller_AnalisePlanejamentoIniciativa($anaid);
		$arrRegistro = array();
		parse_str($_REQUEST['form_itens_composicao'], $arrRegistro);
		$_REQUEST['form-item-composicao'] =  $arrRegistro;
		
		$obAnalise->salvar($_REQUEST);
		die();
	break;
	case 'vincula-processo':
		require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
		$anaid = $_REQUEST['anaid'];
		$obProcesso = new Par3_Controller_Processo();
		$arrRegistro = array();
		parse_str($_REQUEST['form_itens_composicao'], $arrRegistro);
		$_REQUEST['form-item-composicao'] =  $arrRegistro;
		
		$obProcesso->carregaProcessosPorTipoObjeto($_REQUEST);
		die();
	break;
	case 'vincula-processo-existente':
		require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
		$obProcesso = new Par3_Controller_Processo();
		$obProcesso->vinculaIniciativaProcessoExistente($_REQUEST);
		die();
	break;
	case 'autentica-processo':
		require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
		$obProcesso = new Par3_Controller_Processo();
		echo $obProcesso->autenticacaoModalProcesso($_REQUEST);
		die();
	break;
	case 'gerar-processo-fnde':
		require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
		$obProcesso = new Par3_Controller_Processo();
		echo $obProcesso->gerarProcesoFNDE($_REQUEST);
		die();
	break;
	case 'pegaOptionEntidadeExecutora':
	    require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
	    $obProcesso = new Par3_Controller_Processo();
	    echo $obProcesso->pegaOptionEntidadeExecutora($_REQUEST);
    die();
    break;
}

require_once APPRAIZ.'/includes/workflow.php';
$obModelUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$arrEsdDiag     = $obModelUnidade->retornaEstadosPrePlanejamento();
$mdWorkdlow     = new Workflow_Model_Documento($obModelUnidade->docid);
// $mdWorkdlow->esdid = 1683;

// Techo comentado para nao verificar preenchimento do questionario
//if(in_array($mdWorkdlow->esdid, $arrEsdDiag)||$mdWorkdlow->esdid==''){
//    $url = "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid={$_REQUEST['inuid']}&resumo=1";
//    simec_redirecionar($url, 'success', 'Para iniciar o planejamento primeiro <b>FINALIZE O DIAGNÓSTICO.</b>');
//    die();
//}
//

checkParamInuid();
$helperPendencia = new Par3_Helper_PendenciasEntidade();
$controleUnidade            = new Par3_Controller_InstrumentoUnidade();
$controleIndicadorINEP      = new Par3_Controller_IndicadorINEP();
$controleUnidadeEntidade    = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelAuditoriaUnidade      = new Par3_Model_AuditoriaUnidade();
$oIndicador                 = new Par3_Model_Indicador();
$oIndicadorMeta             = new Par3_Model_IndicadorMeta();
$oAreaController            = new Par3_Controller_Area();
$oPontuacaoController       = new Par3_Controller_Pontuacao();
$oPontuacao                 = new Par3_Model_Pontuacao();
$oPontuacaoMeta             = new Par3_Model_PontuacaoMeta();
$modelUnidade               = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
//$mInp                       = new Par3_Model_IniciativaPlanejamento();
$disabled = '';
//ver('teste', d);

$itrid = $controleUnidade->pegarItrid($_REQUEST['inuid']);

if( !in_array($_REQUEST['aba'], array('pesquisaAnalise', 'analisarPlanejamento') ) ){
   $existePendenciaGeral = $_SESSION['visualizar-sintese-diagnostico'] ? FALSE : Par3_Controller_Diagnostico::verificarPendencia($_REQUEST['inuid']);
   $verificaInicioDiagnostico = ($modelAuditoriaUnidade->verificarInicioDiagnostico($_REQUEST['inuid']) && !$existePendenciaGeral);

   $pendenciasImpeditivas = $helperPendencia->controlePendenciasBoxImpeditivas();
}

$esdid = $controleUnidadeEntidade->recuperarEstadoWorkflow($_REQUEST['inuid']);
if ($itrid == 2) {
    $nivel = "município";
    $tipo  = "municipio"; //carregar o IDE
} else if ($itrid == 3) {
    $nivel = "distrito";
    $tipo  = "distrito"; //carregar o IDE
} else {
    $nivel = "estado";
    $tipo  = "estado";//carregar o IDE
}

if ($_REQUEST['visualizar-sintese-diagnostico'])

    $iconNaoPreenchido = '<span class="fa fa-circle" ></span>';
$iconPreenchido = '<span class="fa fa-check iconUnidade"' .
    'style="color:green !important;' .
    'background-color: transparent;"></span>';

$a = 0;
$arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($itrid);
require APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';

?>
<script src="js/bootstrap-tour.min.js"></script>
<script src="js/tour/indicadoresQualitativos.js"></script>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <input type="hidden" id="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>

            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']);?></h5>
            </div>
            <div class="ibox-content">
                <?php  if($_REQUEST['aba'] != 'analisarPlanejamento') if(!$_REQUEST['requisicao']) $controleUnidade->cabecalhoUnidade();  ?>
                <div class="ibox-content-round-gray ibox-content">
                    <?php
                    //                    $arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($itrid, $dimid);

                    //                    $_REQUEST['dimid'] = $_REQUEST['dimid'] ? $_REQUEST['dimid'] : $arDimensoes[0]['dimid'];

                    //                    $aba = $_REQUEST['resumo'] ? "&resumo=1" : "&dimid=" . $_REQUEST['dimid'];

                    //                    $url = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=' . $_REQUEST['inuid'] . $aba;
                    ?>
                    <div class="tabs-container">
                        <div class="tab-content">
                            <script src="js/par3.js"></script>
                            <input type="hidden" id="itrid" name="itrid"
                                   value="<?php echo $itrid; ?>">
                            <div class="wrapper wrapper-content animated fadeIn">
                                <div class="row">
                                    <?php
                                    if($_REQUEST['aba'] != 'analisarPlanejamento'){
	                                                                        if (!$_REQUEST['aba']) {
                                        $_REQUEST['aba'] = 'consulta';
                                    }
//                                    $existeIniciativa = $mInp->verificarExisteIniciativa($_GET['inuid']);
                                    $url = 'par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=' . $_REQUEST['inuid'];
                                    $abasGuias = array();
                                    $abasGuias[] = array("descricao" => "Consulta", "link" => $url . '&aba=consulta');
                                    $abasGuias[] = array("descricao" => "Cadastro", "link" => $url . '&aba=cadastro');
                                    $abasGuias[] = array("descricao" => "Síntese  Planejamento", "link" => $url . '&aba=sintese');

//                                    if($existeIniciativa){
//                                        $abasGuias[] = array("descricao" => "Enviar Plano", "link" => $url . '&aba=enviar');
//                                    }

                                    echo $simec->tab($abasGuias, $url . '&aba=' . $_REQUEST['aba']);
                                    }
                                    switch ($_REQUEST['aba']) {
                                        case 'consulta':
                                            require_once('planejamento/buscarPlanejamentoIniciativa.inc');
                                            break;
                                        case 'cadastro':
                                            require_once('planejamento/cadastrarPlanejamentoIniciativa.inc');
                                            break;
                                        case 'sintese':
                                        	require_once('planejamento/sintesePlanejamentoIniciativa.inc');
                                            break;
//                                        case 'enviar':
//                                            if($existeIniciativa){
//                                                require_once('planejamento/enviarplanejamento.inc');
//                                            }
                                        case 'pesquisaAnalise':
                                            require_once('planejamento/pesquisarIndicadorAnalise.inc');
                                            break;
                                        case 'analisarPlanejamento':
                                            require_once('planejamento/analisarPlanejamentoIniciativa.inc');
                                            break;
                                        default:
                                            require_once('planejamento/cadastrarPlanejamentoIniciativa.inc');
                                            break;
                                    }
                                    ?>
                                    <div class="ibox float-e-margins">
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<div id="modal-form" class="modal fade" aria-hidden="true">
    <div class="modal-dialog">
        <div class="ibox-title">
            <h5 id="html_modal-title"></h5>
            <button type="button" class="close" data-dismiss="modal"
                    aria-label="Close">
                <span aria-hidden="true">&times;</span>
            </button>
        </div>
        <div id="html_modal-form"></div>
    </div>

    <div id="modal-form-large" class="modal fade" aria-hidden="true">
        <div class="ibox-title">
            <h5 id="html_modal-title-large"></h5>
            <button type="button" class="close" data-dismiss="modal"
                    aria-label="Close">
                <span aria-hidden="true">&times;</span>
            </button>
        </div>
        <div id="html_modal-form-large"></div>
    </div>
</div>
<script>
    function popupGuia(acao, tipo, id, itrid) {
        var titulo = 'Gerenciar dimensão';

        switch (tipo) {
            case 'area':
                titulo = 'Gerenciar área';
                break;
            case 'indicador':
                titulo = 'Gerenciar indicador';
                break;
            case 'criterio':
                titulo = 'Gerenciar componentes do indicador';
                break;
        }

        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=principal/configuracao/popupGuia&acao=A',
            data: '&acaoGuia=' + acao + '&tipoGuia=' + tipo + '&codigo=' + id + '&itrid=' + itrid,
            async: false,
            success: function (resp) {
                $('#html_modal-form').html(resp);
                $('#html_modal-title').html(titulo);
                $('#modal-form').modal();
            }
        });
    }
</script>
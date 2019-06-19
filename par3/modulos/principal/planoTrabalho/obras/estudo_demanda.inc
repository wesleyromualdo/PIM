<?php
require      APPRAIZ . 'includes/cabecalho.inc';
require      APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "includes/classes/questionario/TelaBootstrap.class.inc";
require_once APPRAIZ . "par3/modulos/principal/planoTrabalho/planejamento/componentes-indicador-funcoes.inc";
require_once APPRAIZ . '/includes/workflow.php';
include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

$renderNutricionista = new Par3_Controller_Entidade();

if($_GET['inpid'] == null){
    $mObra = new Par3_Model_Obra($_GET['obrid']);
    $mInp  = new Par3_Controller_IniciativaPlanejamento($mObra->inpid);
    $inp   = new Par3_Model_IniciativaPlanejamento($mObra->inpid);
}else{
    $mInp = new Par3_Controller_IniciativaPlanejamento($_GET['inpid']);
    $inp  = new Par3_Model_IniciativaPlanejamento($_GET['inpid']);
}
$oct = $inp->recuperarTipoObra();

$mItr = new Par3_Controller_InstrumentoUnidade();
$simec = new Simec_View_Helper();

$ctrObraTipo = new Par3_Controller_ObraTipo();
$sqlTipoObra = $ctrObraTipo->pegarSelectCombo();

//pega se é do tipo estado uo municipio
$tipo = $mItr->pegarDescricaoEsferaDif($_GET['inuid']);

function getPendencia($ordem,$obrid,$octid)
{
    ob_start();
    $mPergunta  = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
    $mPergunta->carregarPerguntaEstudoDemanda(Par3_Model_ObraAba::ESTUDO_DEMANDA,$ordem,$octid,$obrid);
    return ob_get_clean();
}
//title
switch ($_REQUEST['requisicao']) {
    case 'salvaTipo';
        $mdObra->otpid = $_REQUEST['otpid'];
        $mdObra->obralunos_atendidos = str_replace(".", "", $_REQUEST['obralunos_atendidos']);
        $valor = trim($_REQUEST['obrvalor_estimado']);
        $valor = str_replace(".", "", $valor);

        $modelObraXQuestionario = new Par3_Model_ObraXQuestionario($obqeid = null, $_REQUEST['obrid'], $oabid);
        if ($modelObraXQuestionario && $modelObraXQuestionario->qrpid) {
            //ver($modelObraXQuestionario);http_response_code(404);die();
            $modelObraXQuestionario->qrpid = null;
            $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
            $mAnalise->validarAnaliseEstudoDemanda($_GET['obrid'],8);
            $modelObraXQuestionario->salvar(true, true, array('obqeid', 'obrid_reforma', 'escid', 'cod_escola'));
        }

        $mdObra->obrvalor_estimado = str_replace(",", ".", $valor);
        $mdObra->salvar();
        $mdObra->commit();
        if(isset($_REQUEST['toReturn'])){
            simec_redirecionar("par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid={$_REQUEST['inuid']}&inpid={$_REQUEST['inpid']}&obrid={$_REQUEST['obrid']}&toReturn={$_REQUEST['toReturn']}", 'success');
        }else{
            simec_redirecionar("par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid={$_REQUEST['inuid']}&inpid={$_REQUEST['inpid']}&obrid={$_REQUEST['obrid']}", 'success');
        }
        break;
    case 'buscaDetalheTipoObra':
        ob_clean();
        $ctr = $ctrObraTipo->recuperar();
        if(empty($ctr->octid)){
            die;
        }
        require_once 'estudo_demanda/obratipo_painel.inc';
        die;
        break;
    case 'getcomponentesindicador':
        ob_clean();
        require_once(APPRAIZ . "par3/modulos/principal/planoTrabalho/planejamento/componentes-indicador.inc");
        die;
        break;
    case 'carregarsubmetascomponentesindicador':
        ob_clean();
        require_once APPRAIZ . "par3/modulos/principal/planoTrabalho/planejamento/componentes-indicador-funcoes.inc";
        echo listarSubMetasComponentesIndicador($_GET['metid'],$_GET['cont']);die;
        break;
    case 'montaFiltroIndicadorDemograficoinputSelect':
        ob_clean();
        $controllerGuiaPlanejamentoInformacaoDemograficaEducacional = new Par3_Controller_GuiaPlanejamentoInformacaoDemograficaEducacional();
        echo $controllerGuiaPlanejamentoInformacaoDemograficaEducacional->montaCampoSelectDescricaoIndicadorByIndicativa($inp->iniid, 1);
        die;
        break;
    case 'carregarAnalise':
        ob_clean();
        $mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();

        $dados['template'] = '';
        $dados['pendencia'] = '';
        if($_GET['ordem'] == 'undefined'){
            echo simec_json_encode($dados);die;
        }
        $dados['template'] = getPendencia($_GET['ordem'],$_GET['obrid'],$octid['octid']);
        $dados['pendencia'] = $mpergunta->verificarAnalisePendencia(Par3_Model_ObraAba::ESTUDO_DEMANDA,$_GET['ordem'],$_GET['obrid']);
        echo simec_json_encode($dados);die;
        break;
    case 'checarPendenciasAnalise':
        ob_clean();
        $mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
        $dados     = $mpergunta->listarPendenciasPorAba(Par3_Model_ObraAba::ESTUDO_DEMANDA, $_GET['obrid']);
        if(!is_array($dados)){
           die;
        }
        echo simec_json_encode($dados);die;
        break;
    case 'salvaranalise':
            $mAnalise = new Par3_Controller_AnaliseEngenhariaObraAbaRespostas();
            ob_clean();
            echo $mAnalise->salvar($_POST);
            die;
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
    case 'salvarestudodemanda':
        $mOed = new Par3_Controller_ObraEstudoDemanda();

        if($_GET['disabled']){
            ob_clean();
            echo $_REQUEST['oedid'];die;
        }
        $oedid = $mOed->salvarPasso($_REQUEST);
        ob_clean();
        echo $oedid;
        die;
        break;
    case 'carregartipoobra':
        ob_clean();
        $arrOtp =array();
        $mInu = new Par3_Controller_InstrumentoUnidade();
        $qtdalunos = $_GET['qtdalunos'];
        $octid     = $_GET['octid'];
        $otpesfera = $mInu->buscaEsferaUnidade($_GET['inuid']);
        $mOed = new Par3_Model_ObraEstudoDemanda($_GET['oedid']);
        $tipoAmpliacao = '';
        $arrOtp = $mOed->listarTiposObra($qtdalunos,$otpesfera,$octid,$inpid,$mOed->oedquadra);
        $mpergunta       = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
        $disabledAnalise = $mpergunta->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::ESTUDO_DEMANDA,8,$_GET['obrid']);
        echo $simec->select('otpid', '',$mdObra->otpid, simec_preparar_array($arrOtp), array($disabledAnalise,'class' => ' dadostipoobra',$_GET['disabled']), array());
        die;
        break;
    default:
        $disabled = '';
        //Recupera a dimensão
        $arrItrid['itrid'] = $mItr->pegarItrid($_GET['inuid']);
        $mDim = new Par3_Model_Dimensao();
        $arrDim = $mDim->listarSelect($arrItrid);

//         $qrpid = $controllerObraXQuestionario->getQuestionario($obrid);
        //Recupera iniciativa disponível para o instrumento unidade
        $arrIniciativa = $mInp->getIniciativa(['inuid' => $_GET['inuid'], 'iniid' => $inp->iniid],true);
        //Recupera os dados da iniciativa selecionada
        $arrIniciativaDados = $mInp->getIniciativaDados(array('iniid' => $inp->iniid,'inuid' => $_GET['inuid']));
        //Recupera as modalidades de acordo com a etapa selecionada
        $arrMod = $mInp->getIniciativaModalidades(array('iniid' => $inp->iniid, 'etaid' => $inp->etaid), true);
        //recupera os desdobramentos de acordo com a iniciativa,nível e etapa selecionados
        $arrDes = $mInp->getIniciativaDesdobramentos(array('iniid' => $inp->iniid, 'etaid' => $inp->etaid, 'modid' => $inp->modid), true);
        //busca Etapa
        $mEta = new Par3_Model_EnsinoEtapa($inp->etaid);
        //Recupera  Desdobramentos selecionado
        $desdobramentos = $mInp->getDesdobramentosSelecionado($inp->inpid);
        break;
}

?>
<style>
    .tabela_box_azul_escuro,.colunaIndicadores{
        -webkit-transform:scale(0.95);
        -moz-transform:scale(0.95);
        -ms-transform:scale(0.95);
        transform:scale(0.95);
    }

    .tabela_box_azul_escuro{
        -webkit-transform:scale(0.75);
        -moz-transform:scale(0.75);
        -ms-transform:scale(0.75);
        transform:scale(0.75);
        margin-bottom: -100px;
        margin-left: -100px;
        margin-right: -100px;
        margin-top: -80px;
    }
</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center"> Estudo de Demanda </h2>
                <h3 class="center"><?= (empty($_GET['obrid']) ? 'Cadastrar' : "Pré-Obra ID: {$_GET['obrid']}") ?></h3>
            </div>
            <!-- janela de impressão -->
            <form id="formImpressao" name="formImpressao" action="" method="POST" target="relatorioImpressao">
                <input type="hidden" id="aegid" name="aegid" value="">
                <input type="hidden" id="req" name="req" value="imprimir">
            </form>
        </div>
    </div>
    <div class="ibox-content">
        <div class="ibox float-e-margins" id="iniciativa">
            <div class="ibox-title" id="btn-iniciativa">
                <div class="ibox-tools">
                    <a><i id="i-iniciativa" class="fa fa-chevron-down"></i></a>
                </div>
                <h3 class="">Iniciativa: <?php echo $arrIniciativa[$inp->iniid]; ?></h3>
            </div>
            <div class="ibox-content" id="div-iniciativa" style="display: none;">
                <form name="form-iniciativaplanejamento" id="form-iniciativaplanejamento" class="form-horizontal">
                    <div class="row">
                        <div class="col-lg-6">

                            <?php /*echo $simec->select('dimid', 'Dimensão',$inp->dimid,$arrDim, array('maxlength' => '255','required'),array('input-size'=>'8')); ?>
                            <?php echo $simec->select('iniid', 'Iniciativas',$inp->iniid,$arrIniciativa, array('maxlength' => '255','required','onchange' => 'js_returnIdIniciativa(this.value);'),array('input-size'=>'8')); */ ?>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Dimensão: </label>
                                <div class="col-md-8 col-md-8 col-lg-8 control-label"><p
                                            class="text-left"><?php echo $arrDim[$inp->dimid]; ?></p></div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Iniciativas: </label>
                                <input type="hidden" id="iniid" value="<?php echo $inp->iniid; ?>">
                                <input type="hidden" id="iniidDescr" value="<?php echo $arrIniciativa[$inp->iniid]; ?>">
                                <div class="col-md-8 col-md-8 col-lg-8 control-label"><p
                                            class="text-left"><?php echo $arrIniciativa[$inp->iniid]; ?></p></div>
                            </div>

                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Ciclo: </label>
                                <div class="col-md-8 col-md-8 col-lg-8 control-label"><p
                                            class="text-left"><?php echo $arrIniciativaDados['cic']; ?></p></div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Anos: </label>
                                <div class="col-md-8 col-md-8 col-lg-8 control-label"><p
                                            class="text-left"><?php echo $arrIniciativaDados['anos']; ?></p></div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Tipo de
                                    Objeto: </label>
                                <div class="col-md-8 col-md-8 col-lg-8"><p
                                            class="text-left"><?php echo $arrIniciativaDados['into']; ?></p></div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label"
                                       style="right:6px;">Tipo de Atendimento: </label>
                                <div class="col-md-8 col-md-8 col-lg-8"><p
                                            class="text-left"><?php echo $arrIniciativaDados['inta']; ?></p></div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Esfera: </label>
                                <div class="col-md-8 col-md-8 col-lg-8"><p
                                            class="text-left"><?php echo $arrIniciativaDados['esfera']; ?></p></div>
                            </div>
                        </div>
                        <div class="col-lg-6" id="">
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label"
                                       style="right:6px;">Áreas Relacionadas: </label>
                                <div class="col-md-8 col-md-8 col-lg-8 control-label"><p
                                            class="text-left"><?php echo trim($arrIniciativaDados['iar']); ?></p></div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Programa: </label>
                                <div class="col-md-8 col-md-8 col-lg-8 control-label"><p
                                            class="text-left"><?php echo $arrIniciativaDados['prg']; ?></p></div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Projeto: </label>
                                <div class="col-md-8 col-md-8 col-lg-8 control-label"><p
                                            class="text-left"><?php echo $arrIniciativaDados['pro']; ?></p></div>
                            </div>
                            <div class="form-group">
                                <input type="hidden" name="nivid" value="1"/>
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Nível: </label>
                                <div class="col-md-8 col-md-8 col-lg-8"><p class="text-left">Educação Básica</p></div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Etapa: </label>
                                <div class="col-md-8 col-md-8 col-lg-8">
                                    <div class="control-label"><p class="text-left"><?php echo $mEta->etadsc; ?></p>
                                    </div>
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label">Modalidade: </label>
                                <div class="col-md-8 col-md-8 col-lg-8 control-label"><p
                                            class="text-left"><?php echo $arrMod; ?></p></div>
                            </div>
                            <div class="form-group">
                                <label class="col-lg-4 control-label"><span class="text-right">Desdobramento:</span></label>
                                <div class="col-md-8 col-md-8 col-lg-8 control-label"><p
                                            class="text-left"><?php echo implode(' , ', $desdobramentos); ?></p></div>
                            </div>
                        </div>
                    </div>
                </form>
            </div>
        </div>
        <div class="ibox float-e-margins" id="componentes">
            <div class="ibox-title" id="btn-componentes-indicador">
                <div class="ibox-tools">
                    <a> <i id="i-componentes-indicador" class="fa fa-chevron-down"></i></a>
                </div>
                <h3 class="">Componentes Indicador</h3>

            </div>
            <div class="ibox-content shrink" id="div-componentes-indicador" style="display: none;">
                <?php
                $_GET['dimid'] = $inp->dimid;
                $_GET['iniid'] = $inp->iniid;
                require_once(APPRAIZ . "par3/modulos/principal/planoTrabalho/planejamento/componentes-indicador.inc");
                ?>
            </div>
        </div>
        <!-- COMENTADO APRA APRESENTAÇÃO.... -->
        <div class="ibox float-e-margins" id="demograficas">
            <div class="ibox-title" id="btn-informacoes-demograficas">
                <div class="ibox-tools">
                    <a> <i id="i-informacoes-demograficas" class="fa fa-chevron-down"></i></a>
                </div>
                <h3 class="">Informações Demográficas e Educacionais</h3>
            </div>
            <div class="ibox-content" id="div-informacoes-demograficas" style="display: none;">
                <form class="form-horizontal" id="form_filtro_indicadores" name="form_filtro_indicadores" method="POST">
                    <div class="row">
                        <div id="field_indicador">
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
                            <!--Resultado ajax montar tabela -->
                        </div>
                    </div>
                </form>
            </div>
        </div>
        <div class="ibox float-e-margins" id="questionario-estudo-demanda">
            <div class="ibox-title" id="btn-questionario-estudo-demanda">
                <div class="ibox-tools">
                    <a><i id="i-questionario-estudo-demanda" class="fa fa-chevron-up"></i></a>
                </div>
                <h3 class="">Questionário Estudo de Demanda <?php echo $oct['octdsc']?' - '.$oct['octdsc']:'<span class="danger">(Não Possui Tipo de Obra cadastrado para essa Iniciativa)</span>'; ?></h3>
            </div>
            <div class="ibox-content" id="div-questionario-estudo-demanda" style="">
                <?php
                if(!empty($oct['octid'])){
                    $_GET['etaid']  = $inp->etaid;
                    $_GET['octid']  = $oct['octid'];
                    $_GET['octdsc'] = $oct['octdsc'];
                    require_once 'estudo_demanda/questionario.inc';
                }
                ?>
            </div>
        </div>
    </div>
</div>
<script>
    //esconde o botão de enviar tipo obra ao carregar componente de questionário
    //Libera para enviar somente após chegar no último passo
    $('.wizard').ready(function () {
        $(document).find('a[href$="finish"]').parent('li').hide();
    });

    //mostra/esconde ibox
    jQuery("#btn-iniciativa").click(function () {
        $('#div-iniciativa').slideToggle();
        //chevron up/down
        $('#i-iniciativa').toggleClass(function () {
            if ($('#i-iniciativa').is(".fa-chevron-down")) {
                $('#i-iniciativa').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-iniciativa').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

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
    jQuery("#btn-questionario-estudo-demanda").click(function () {
        $('#div-questionario-estudo-demanda').slideToggle();
        //chevron up/down
        $('#i-questionario-estudo-demanda').toggleClass(function () {
            if ($('#i-questionario-estudo-demanda').is(".fa-chevron-down")) {
                $('#i-questionario-estudo-demanda').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-questionario-estudo-demanda').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    function buscarComponentesIndicadorIniciativa(iniid, dimid, url = '') {
        var inuid = $("#inuid").val();
        var caminho = window.location.href;
        var action = '&requisicao=getcomponentesindicador&iniid=' + iniid + '&dimid=' + dimid + url;

        $('#div-componentes-indicador').load(caminho + action);
    }

    $(document).on('click','.btn-meta-componente-indicador',function(){
        var meta  = $(this).attr('meta');
        var cont  = $(this).attr('area');

        var abaMetas = $('#div-metasaba'+cont);
        abaMetas.find('.btn').removeClass('btn-primary-active').addClass('btn-primary');
        abaMetas.find('#btn-meta'+meta).removeClass('btn-primary').addClass('btn-primary-active');
        var path     = window.location.href;
        var action   = '&requisicao=carregarsubmetascomponentesindicador&metid='+meta+'&cont='+cont;
        $('#div-submetas'+cont).load(path+action);
        console.log(path+action);
    });

    $(document).on('click', '.menuIndicadores', function () {
        var dimid = $(this).attr('dimid');
        var areid = $(this).attr('areid');
        var indid = $(this).attr('indid');
        var form = $("#form-iniciativaplanejamento");
        var iniid = form.find("#iniid").val();
        var indidAntigo = $('#indid').val();
        var url = '<?php echo($_GET['inpid'] ? '&inpid=' . $_GET['inpid'] : ''); ?>' + '&areid=' + areid + '&indid=' + indid;
        console.log(url);
        if (indid != indidAntigo) {
            buscarComponentesIndicadorIniciativa(iniid, dimid, url);
        }
    });

    $(document).on('change', '#otpid', function () {
        var otpid = $(this).val();
        var caminho = window.location.href;
        var action = '&requisicao=buscaDetalheTipoObra&otpid=' + otpid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#detalheTipoObra').html(' ');
                $('#detalheTipoObra').html(resp);

                return false;
            }
        });
    });

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
                async: false,
                success: function (resp) {
                    document.getElementById('field_indicador').innerHTML = resp;
                    $("#tb_filtroIndicadores").html(" ");
                }
            })
        }
    }

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

    //carrega função
    if ($("#inid").val() != "") {
        js_returnIdIniciativa();
    }
</script>

<?php

require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/planejamento/componentes-indicador-funcoes.inc';
require_once APPRAIZ . 'par3/classes/controller/Iniciativa_planejamento_informacoesiniciativa.class.inc';
require_once APPRAIZ . 'par3/classes/controller/Iniciativa_planejamento_detalheetapa.class.inc';
require_once APPRAIZ . 'par3/classes/controller/Iniciativa_planejamento_atividadeetapa.class.inc';
require_once APPRAIZ . 'par3/classes/controller/Iniciativa_planejamento_insumosiniciativa.class.inc';
require_once APPRAIZ . 'par3/classes/controller/AnalisePlanejamentoIniciativa.class.inc';
require_once APPRAIZ . 'par3/classes/controller/Processo.class.inc';
require_once APPRAIZ . 'par3/classes/controller/WS_Servico_FNDE.class.inc';

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

$mInp = new Par3_Controller_IniciativaPlanejamento();
$mDim = new Par3_Model_Dimensao();

$arrIniciativa = array();
$mdIniciativa = new Par3_Model_Iniciativa();
$inp = $mInp->recuperar();

if (empty($_REQUEST['inpid']) || empty($_REQUEST['iniano'])) {
    echo "<script>
            alert('Faltam parametros necessários!');
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarIndicadorAnalise&acao=A';
		  </script>";
    die();
}

switch ($_REQUEST['requisicao']) {
    case 'montaFiltroIndicadorDemograficoinputSelect':
        ob_clean();
        $controllerGuiaPlanejamentoInformacaoDemograficaEducacional = new Par3_Controller_GuiaPlanejamentoInformacaoDemograficaEducacional();
        echo $controllerGuiaPlanejamentoInformacaoDemograficaEducacional->montaCampoSelectDescricaoIndicadorByIndicativa($_REQUEST['iniid'], 1);
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
            <?php echo $simec->select('inpcronogramamesinicial', null, $inp->inpcronogramamesinicial, $arMeses, array('disabled' => 'true'), array('input-size' => '2')); ?>
            <label class="col-sm-1" style="text-align: center;"> a </label>
            <?php echo $simec->select('inpcronogramamesfinal', null, $inp->inpcronogramamesfinal, $arMeses, array('disabled' => 'true'), array('input-size' => '2')); ?>
            <label class="col-sm-1" style="width: 8%;"> Ano Inicial: </label>
            <?php echo $simec->select('inpcronogramaanoinicial', '', $inp->inpcronogramaanoinicial, $sqlAnos, array('disabled' => 'true'), array('input-size' => '2')); ?>
            <label class="col-sm-1" style="text-align: right;"> Ano de término: </label>
            <?php echo $simec->select('inpcronogramaano', '', $inp->inpcronogramaano, $sqlAnos, array('disabled' => 'true'), array('input-size' => '2')); ?>
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
    case 'buscaModalEscolas':
        ob_clean();
        $inpice = new Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola();
        $resp = $inpice->getFormEscolas($_REQUEST);
        echo $resp;
        die;
        break;
    case 'ajaxModalHistoricoAnalise':
        ob_clean();
        Par3_Controller_AnalisePlanejamentoIniciativa::modalHistorico($_REQUEST);
        die;
        break;
    case 'detalharProcessoViculado':
        ob_clean();
        $proid = $_REQUEST['dados'][0];
        $obProcesso = new Par3_Controller_Processo();
        $obProcesso->carregaAnaliseVinculadaProcesso($proid);
        die;
        break;
    case 'downloadDocumentoPlanejamento':
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec();
        $file->getDownloadArquivo($_GET['arqid']);
        die;
        break;
    /*case 'carregaModalDiligencia':
        ob_clean();
        echo 'teste';
        die;
    break;*/
}
//administrarProcesso
require APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';

$url = 'par3.php?modulo=principal/planoTrabalho/planejamento/analisarPlanejamentoIniciativa&acao=A';
echo $simec->tab(criaAbaIniciativaAnalise(), $url);

if ($_REQUEST['inpid']) {
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

    $controllerIniciativaPlanejamento = new Par3_Controller_IniciativaPlanejamento();

    $controllerIniciativaPlanejamentoItemComposicao = new Par3_Controller_IniciativaPlanejamentoItemComposicao();
    $modelIniciativaTipoObjeto = new Par3_Model_IniciativaTiposObjeto();
    $objeto = $controllerIniciativaPlanejamentoItemComposicao->buscaObjeto($inp->inpid);

    if (empty($_REQUEST['anaid'])) {
        $obAnaliseIniciativa = new Par3_Controller_AnalisePlanejamentoIniciativa($_REQUEST['anaid']);

        $anaid = $obAnaliseIniciativa->verificaExisteAnalisePlanejamento($_REQUEST['inpid'], $_REQUEST['iniano']);
        $_REQUEST['anaid'] = $anaid;
        unset($obAnaliseIniciativa);

        if (empty($anaid)) {
            $obAnaliseIniciativa = new Par3_Controller_AnalisePlanejamentoIniciativa($_REQUEST['anaid']);
            $anaid = $obAnaliseIniciativa->pegaIdWorkFlow($_REQUEST['anaid']);
        }
        echo "<script>
					window.location.href = 'par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inpid=" . $_REQUEST['inpid'] . "&inuid=" . $_REQUEST['inuid'] . "&iniano=" . $_REQUEST['iniano'] . "&aba=analisarPlanejamento&anaid=" . $anaid . "';
				</script>";
        exit();
    }

    $obAnaliseIniciativa = new Par3_Controller_AnalisePlanejamentoIniciativa($_REQUEST['anaid']);
    $arAnalise = $obAnaliseIniciativa->carregarAnalisePlanejamento($_REQUEST['anaid']);

    $arEstato = $obAnaliseIniciativa->carregaEsdidAtual($arAnalise['docid']);
    $esdid = $arEstato['esdid'];
    $esddsc = $arEstato['esddsc'];

    //$arTipoAtendimento = $mInp->pegaTipoAtendimentoAnalise($inp->inpid);
    $arTipoObjeto = $mInp->pegaTipoObjetoAnalise($inp->inpid);
    $intoid = $arTipoObjeto['intoid'];
}

$arrDim = $mDim->listarSelect($arrItrid);

$disabled = 'disabled';

$arDes = simec_preparar_array($arrDes);
//www\library\simec\js\listagem.js
?>
<style>
    /*fix para pintar os inputs desabilitados*/
    .chosen-disabled > .chosen-single {
        background-color: #eee; !important;
    }
</style>
<div class="ibox">
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-12 orcamento">
                <?php
                if ($inp->inpid && $arAnalise['intaid'] == 2) {
                    $obEmenda = new Par3_Controller_EmendasParlamentares();
                    echo $obEmenda->pegaBeneficiarioSelecionado('', $inp->inpid, $_REQUEST['inuid']);
                    if ($_REQUEST['anaid']) {
                        $emecod = $obEmenda->pegaCodigoEmenda($_REQUEST['anaid']);
                    }
                } ?>
            </div>
        </div>
        <div class="row">
            <div class="col-md-12 orcamento">
                <?php
                if ($inp->inpid) {
                    $obProcesso = new Par3_Controller_Processo();
                    echo $obProcesso->carregaDadosOrcamentario($_GET['inuid'], $inp->inpid, $_REQUEST['anaid']);
                } ?>
            </div>
        </div>
    </div>
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h1 class="center" style="font-weight: bold;">Analisar Iniciativa</h1>
            </div>
        </div>
    </div>
</div>
<div class="container-fluid">
    <form name="form-iniciativaplanejamento" id="form-iniciativaplanejamento" class="form-horizontal" method="post">
        <div class="row">
            <input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
            <input type="hidden" name="emecod" value="<?php echo $emecod; ?>"/>
            <input type="hidden" name="inpid" id="inpid" value="<?php echo $inp->inpid; ?>"/>
            <div class="form-group">
                <div class="col-md-2" style="text-align: justify;"><h4>Dimensão:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrDim[$inp->dimid]; ?></p></div>
                <div class="col-md-2" style="text-align: justify;"><h4>Áreas Relacionadas:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrIniciativaDados['iar']; ?></p>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="form-group">
                <div class="col-md-2" style="text-align: justify;"><h4>Iniciativas:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrIniciativa[$inp->iniid]; ?></p>
                    <input type="hidden" name="iniid" id="iniid" value="<?php echo $inp->iniid; ?>"></div>
                <div class="col-md-2" style="text-align: justif;"><h4>Programa:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrIniciativaDados['prg']; ?></p>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="form-group">
                <div class="col-md-2" style="text-align: justify;"><h4>Ciclo:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrIniciativaDados['cic']; ?></p>
                </div>
                <div class="col-md-2" style="text-align: justify;"><h4>Projeto:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrIniciativaDados['pro']; ?></p>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="form-group">
                <div class="col-md-2" style="text-align: justify;"><h4>Anos:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrIniciativaDados['anos']; ?></p>
                </div>
                <div class="col-md-2" style="text-align: justify;"><h4>Nível:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo 'EducaçãoBásica'; ?></p></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group">
                <div class="col-md-2" style="text-align: justify;"><h4>Tipo de Objeto:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrIniciativaDados['into']; ?></p>
                </div>
                <div class="col-md-2" style="text-align: justify;"><h4>Etapa:</h4></div>
                <div class="col-md-4"><p
                            style="text-align: justify;"><?php echo $arrIniciativaDados['eta'][$inp->etaid]; ?></p>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="form-group">
                <div class="col-md-2" style="text-align: justify;"><h4>Tipo de Atendimento:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrIniciativaDados['inta']; ?></p>
                </div>
                <div class="col-md-2" style="text-align: justify;"><h4>Modalidade:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrMod[$inp->modid]; ?></p></div>
            </div>
        </div>
        <div class="row">
            <div class="form-group">
                <div class="col-md-2" style="text-align: justify;"><h4>Esfera:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arrIniciativaDados['esfera']; ?></p>
                </div>
                <div class="col-md-2" style="text-align: justify;"><h4>Desdobramento:</h4></div>
                <div class="col-md-4"><p style="text-align: justify;"><?php echo $arDes[$desdobramentos[0]]; ?></p>
                </div>
            </div>
        </div>
    </form>
    <?php
    if($inp->obrid) {

        echo '<div style="padding: 0 10px;"> <span style="margin-top: 25px;position: absolute; font-weight: bold;">Dados da obra: </span>';

        $controllerIniciativaPlanejamento->getDadosObraProinfancia($inp->obrid);

        echo '</div>';
    }
    ?>
</div>
<?php if ($_GET['inpid']) { ?>
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
                        <div class="col-md-3"><label class="control-label active pull-right ">Descrição do
                                Grupo/Indicador:</label></div>
                        <div class="col-md-4">
                            <style>
                                .dropdown-menu {
                                    z-index: 10000 !important;
                                }
                            </style>
                            <div class="form-group "><input class="form-control" style="width:400px"
                                                            id="exampleInputName2" placeholder="Selecione..."
                                                            disabled="disabled" type="text"></div>
                        </div>
                        <div class="col-md-3">
                            <button disabled type="button" class="btn btn-primary pull-left" id="pesquisar_indicadores"
                                    value="pesquisar_indicadores"><i class="fa fa-search"></i>Pesquisar
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
            <div class="ibox-tools"><a>Opções Avançadas <i id="i-cronograma-execucao"
                                                           class="fa fa-chevron-down"></i></a></div>
            <h3 class="">Quantidades e Cronograma de Execução</h3>
            <div class="ibox-content" id="div-cronograma-execucao" style="display: none;">
                <?php require_once('formCronogramaFisico.inc'); ?>
            </div>
        </div>
    </div>

    <div class="float-e-margins" id="itens-composicao">
        <div class="ibox-title" id="btn-itens-composicao">
            <div class="ibox-tools"><a>Opções Avançadas <i id="i-itens-composicao" class="fa fa-chevron-down"></i></a>
            </div>
            <h3 class="">Itens de Composição - <?php echo $_REQUEST['iniano']?></h3>
        </div>

        <div class="ibox-content" id="div-itens-composicao" style="display: none;">
            <?php
            if ($inp->inpid && $objeto != $modelIniciativaTipoObjeto::OBRA) {
                require_once('formItemComposicao.inc');
            }
            ?>
            <?php
            if ($inp->inpid && $objeto == $modelIniciativaTipoObjeto::OBRA) {
                $anos = array($_REQUEST['iniano']); //explode(",", $arrIniciativaDados['anos']);

                for ($cont = 0; $cont < count($anos); $cont++) {
                    $controllerObra = new Par3_Controller_Obra;
                    $controllerObra->criaAbaObra($anos[$cont], $inp->inpid, $_GET['inuid']);
                }
            }
            ?>

        </div>
    </div>
    <div class="float-e-margins" id="arquivos">
        <div class="ibox-title" id="btn-arquivos">
            <div class="ibox-tools">
                <a>Opções Avançadas <i id="i-arquivos" class="fa fa-chevron-down"></i></a>
            </div>
            <h3 class="">Arquivos</h3>
        </div>
        <div class="ibox-content" id="div-arquivos" style="display: none;">
            <?php
                $controllerIniciativaPlanejamento->getListaArquivosPlanejamento($inp->inpid);
            ?>
        </div>
    </div>
    <table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td style="width: 100%">
                <div class="ibox-content">
                    <div class="row">
                        <div class="form-group" id="cic-div">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-2 control-label"
                                   style="font-size: 18px; font-weight: bold;">Parecer da Análise:</label>
                        </div>
                    </div>
                </div>
                <form name="form-iniciativa-analise-parecer" id="form-iniciativa-analise-parecer"
                      class="form-horizontal" method="post">
                    <input type="hidden" name="ano_iniciativa" id="ano_iniciativa" value="<?php echo $_REQUEST['iniano']; ?>">
                    <input type="hidden" name="form_itens_composicao" id="form_itens_composicao" value="">
                    <input type="hidden" name="requisicao_analise" id="requisicao_analise" value="">
                    <input type="hidden" name="docid_analise" id="docid_analise" value="">
                    <input type="hidden" name="aedid_analise" id="aedid_analise" value="">
                    <input type="hidden" name="intoid" id="intoid" value="<?php echo $intoid; ?>">
                    <input type="hidden" name="intaid" id="intaid" value="<?php echo $arAnalise['intaid']; ?>">
                    <input type="hidden" name="esdid" id="esdid" value="<?php echo $esdid; ?>">
                    <?php
                    $estadoBloqueio = array(PAR3_ESDID_AGUARDANDO_VALIDACAO_COORDENADOR, PAR3_ESDID_EM_VALIDACAO_COORDENADOR, PAR3_ESDID_ANALISE_PLANEJAMENTO_APROVADO);

                    if (in_array($esdid, $estadoBloqueio)) {
                        $attribus = array('maxlengh' => 4000, 'readonly' => 'readonly');
                        $attribusCombo = array('maxlength' => '255', '', '', 'text-align' => 'left', 'disabled' => 'disabled');
                        $disabledText = 'disabled';
                    } else {
                        $attribusCombo = array('maxlength' => '255', '', '', 'text-align' => 'left');
                        $attribus = array('maxlengh' => 4000);
                        $disabledText = '';
                    }

                    echo $simec->textarea('anaconsideracaoent', 'Considerações sobre a Entidade Proponente', $arAnalise['anaconsideracaoent'], $attribus, array('label-size' => '2'));
                    echo $simec->textarea('anaconsideracaoprop', 'Considerações sobre a Proposta', $arAnalise['anaconsideracaoprop'], $attribus, array('label-size' => '2'));
                    echo $simec->textarea('anaconsideracaoproj', 'Considerações sobre o Projeto', $arAnalise['anaconsideracaoproj'], $attribus, array('label-size' => '2'));
                    echo $simec->textarea('anaconsideracaoobj', 'Considerações sobre o Objetivo', $arAnalise['anaconsideracaoobj'], $attribus, array('label-size' => '2'));
                    echo $simec->textarea('anaconsideracaojus', 'Considerações sobre a Justificativa', $arAnalise['anaconsideracaojus'], $attribus, array('label-size' => '2'));
                    echo $simec->textarea('anaconsideracaoval', 'Considerações sobre os Valores', $arAnalise['anaconsideracaoval'], $attribus, array('label-size' => '2'));
                    echo $simec->textarea('anaoutrasconsideracoes', 'Outras Considerações Cabíveis', $arAnalise['anaoutrasconsideracoes'], $attribus, array('label-size' => '2'));

                    $arParecer = array('A' => 'Aprovado',
                        'R' => 'Reprovado',
                        'D' => 'Diligência'
                    );
                    echo $simec->select('anaparecer', 'Situação do Parecer', $arAnalise['anaparecer'], $arParecer, $attribusCombo, array('label-size' => '2'));
                    ?>
                    <div class="form-group ">
                        <label for="" class="col-sm-2 col-md-2 col-lg-2 control-label">Texto do parecer: </label>
                        <div class="col-sm-10 col-md-10 col-lg-10 ">
                            <textarea name="anatextoparecer" id="anatextoparecer" type="text" class="form-control"
                                      maxlengh="4000" <?php echo $disabledText; ?> label-for="anatextoparecer"
                                      placeholder="Considerando o exposto na Lei nº 12.695/2012, que dispõe sobre o apoio técnico ou financeiro da União no âmbito do Plano de Ações Articuladas, após análise da solicitação, esta Coordenação posiciona-se favorável a pactuação por Termo de Compromisso e ao repasse de recursos, aprovando a iniciativa proposta pelo ente governamental."><?= $arAnalise['anatextoparecer'] ?></textarea>
                        </div>
                        <div style="clear:both"></div>
                    </div>

                    <div class="ibox-content">
                        <div class="row">
                            <div class="ibox-footer">
                                <div class="center">
                                    <?php
                                    // Histórico de análise(s)
                                    echo '<button type="button" class="btn btn-info" onclick="exibeHistoricoAnalise( ' . $_REQUEST['inpid'] . ', \'' .  $_REQUEST['iniano'] . '\');">
                                            <span class="glyphicon glyphicon-time" aria-hidden="true"></span> Histórico de análise(s) em ' . $_REQUEST['iniano'] .
                                          '</button>';

                                    if ($esdid == PAR3_ESDID_ANALISE_PLANEJAMENTO_APROVADO) { ?>
                                        <button type="button" id="btn-carrega-login1" data-dismiss="modal" class="btn btn-primary" onclick="btn_carrega_login();" data-loading-text="Vinculando processo, aguarde ..."><i class="fa fa-times-circle-o"></i> Vincular Processo</button>
                                    <?php } else { ?>
                                        <?php
                                        $boAcaoSalvar = array(PAR3_ESDID_ANALISE_DILIGENCIA, PAR3_ESDID_PLANEJAMENTO_ANALISE, PAR3_ESDID_PLANEJAMENTO_AGUARDANDO_ANALISE);

                                        if (in_array($esdid, $boAcaoSalvar)) {
                                            echo '<button type="button" id="btn-salvar-analise" data-dismiss="modal" class="btn btn-success" data-loading-text="Salvando, aguarde ..."><i class="fa fa-times-circle-o"></i> Salvar</button>&nbsp;&nbsp;';
                                        }
                                        //echo $obAnaliseIniciativa->carregaBotaoAcaoWorkFlow();
                                        ?>
                                    <?php } ?>
                                </div>
                            </div>
                        </div>
                    </div>
                    <br>
                </form>
            </td>
            <td style="text-align: center;" valign="center"><?php
                echo wf_desenhaBarraNavegacaoBootstrap($arAnalise['docid'], array('inpid' => $inp->inpid, 'anaid' => $_REQUEST['anaid'], 'iniano' => $_REQUEST['iniano']), $ocultar, 'Fluxo');
            ?></td>
        </tr>
    </table>
<?php } ?>
<div class="ibox float-e-margins animated modal" id="modal-login-processo" tabdesex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-content" style="text-align: center;">
                <div class="row">
                    <div id="modal-autentica-processo" style="height: auto;"></div>
                </div>
            </div>
            <div class="ibox-footer" style="text-align: center;">
                <button type="button" id="btn-vincula-processo" data-dismiss="modal" class="btn btn-default"><i
                            class="fa fa-times-circle-o"></i> Vincular Processo Existente
                </button>
                <button type="button" id="btn-cancelar-vinculo" data-dismiss="modal" class="btn btn-default"><i
                            class="fa fa-times-circle-o"></i> Cancelar
                </button>
                <button type="button" id="btn-gera-processo" data-dismiss="modal" class="btn btn-success"
                        onclick="btn_gear_processo_fnde();"><i class="fa fa-plus"></i> Gerar Processo FNDE
                </button>
            </div>
        </div>
    </div>
</div>

<div class="ibox float-e-margins animated modal" id="modal_vincula_proceso" tabdesex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 1100px;">
        <div class="modal-content">
            <div class="ibox-content" style="text-align: center;">
                <div class="row">
                    <div id="conteudo-modal-processo"></div>
                </div>
            </div>
            <div class="ibox-footer" style="text-align: center;">
                <button type="button" id="btn-cancelar-vinculo" data-dismiss="modal" class="btn btn-default"><i
                            class="fa fa-times-circle-o"></i> Cancelar
                </button>
                <button type="button" id="btn-carrega-login3" data-dismiss="modal" class="btn btn-success"
                        onclick="btn_carrega_login();"><i class="fa fa-plus"></i> Gerar Processo FNDE
                </button>
            </div>
        </div>
    </div>
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

<div id="debug"></div>
<script>
function carregaMSG(){
	jQuery('#div_entidade_executora_msg').show();	
}

function carregarEmendaAnalise(emecod, inpid, anaid, emeano, inuid){
    window.location.href = "par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inpid="+inpid+"&inuid="+inuid+"&iniano="+emeano+"&anaid="+anaid+"&aba=analisarPlanejamento";
}

    jQuery(document).ready(function(){
        var emenda = jQuery('[name="emecod"]').val();
        jQuery('table[id=tb_render]').eq(0).find('div:contains("'+emenda+'")').closest('tr').css('color', 'blue');
        jQuery('table[id=tb_render]').eq(0).find('div:contains("'+emenda+'")').closest('tr').css('font-weight', 'bold');
    });

    function vinculaProcessoExistente(proid, processo) {

        swal({
                title: "Confirmar",
                text: "Tem certeza que deseja vincular o planejamento <?php echo $_REQUEST['inpid']?>, ano <?php echo $_REQUEST['iniano']?>, ao processo " + processo + "?",
                type: "success",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    jQuery('#form-vincula-processo #proid').val(proid);
                    jQuery('[name="requisicao_analise"]').val('vincula-processo-existente');
                    
                    jQuery('#form-vincula-processo').submit();
                }
                return false;
            }
        );
    }

    function btn_gear_processo_fnde() {
        jQuery('[name="requisicao_analise"]').val('gerar-processo-fnde');

        if (jQuery('[name="ws_usuario"]').val() == '') {
            alert('Informe o usuário.');
            return false;
        }

        if (jQuery('[name="ws_senha"]').val() == '') {
            alert('Informe a senha.');
            return false;
        }
        jQuery('#btn-gera-processo').attr('disabled', true);

        jQuery('#form-autentica-processo').submit();
    }

    jQuery("#btn-vincula-processo").click(function () {
        jQuery('#form-autentica-processo #requisicao_analise').remove();

        var caminho = window.location.href;
        var action = '&requisicao_analise=vincula-processo&' + jQuery('#form-autentica-processo').serialize();
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery("#conteudo-modal-processo").html(resp);
            }
        });
        $("#modal-login-processo").modal("hide");
        $("#modal_vincula_proceso").modal("show");
    });

    function btn_carrega_login() {
        var inpid = jQuery('[name="form-iniciativaplanejamento"]').find('[name="inpid"]').val();
        var inuid = jQuery('[name="form-iniciativaplanejamento"]').find('[name="inuid"]').val();
        var ano = jQuery('#ano_iniciativa').val();
        var intoid = jQuery('#intoid').val();
        var intaid = jQuery('#intaid').val();

        var caminho = window.location.href;
        var action = '&requisicao_analise=autentica-processo&inpid=' + inpid + '&ano=' + ano + '&inuid=' + inuid + '&intoid=' + intoid+'&intaid='+intaid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery("#modal-autentica-processo").html(resp);
            }
        });

    	$('#div_entidade_executora_msg').hide();
    	
    	var caminho = window.location.href;
        var action  = '&requisicao_analise=pegaOptionEntidadeExecutora&inuid='+inuid;
       	jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                if( resp == 'N' ){
                	jQuery("#div_main_entidade").hide();                
                } else {
                	jQuery("#div_main_entidade").show();
                	jQuery("#div_entidade_executora").html(resp);
                }
            }
        });
        
        $("#modal-login-processo").modal("show");
    }

    /*jQuery("#btn-carrega-login").click(function () {

    });*/
    
    function atualizaValoresItens(iigid, iniano) {
        var qtd_aprovado = jQuery('[name="qtd-item-aprovado[' + iigid + '][' + iniano + ']"]').val();
        var qtd_aprovado_salvo = jQuery('[name="qtd-item-aprovado-s[' + iigid + '][' + iniano + ']"]').val();
        var qtd_item = jQuery('[name="qtd-item[' + iigid + '][' + iniano + ']"]').val();
        var qtd_item_emenda = jQuery('[name="qtd-item-emenda[' + iigid + '][' + iniano + ']"]').val();
        var valor_referencia = jQuery('[name="valor_referencia[' + iigid + '][' + iniano + ']"]').val();
        var qtd_total_aprovada = jQuery('[name="qtd_total_aprovada[' + iigid + '][' + iniano + ']"]').val();
        
        var total = parseFloat(valor_referencia) * parseFloat(qtd_aprovado);
        var emecod = jQuery('[name="emecod"]').val();

        var label = "A QTD Aprovado não pode ser maior que a quantidade de Item";
        if( emecod != '' ){
            qtd_item = qtd_item_emenda;
            label = "A QTD Aprovado não pode ser maior que a quantidade de Item(Emenda)";
        }

        /*if ( parseFloat(qtd_item) <  parseFloat(qtd_aprovado) ) {
            jQuery('[name="qtd-item-aprovado[' + iigid + '][' + iniano + ']"]').val(qtd_aprovado_salvo);
            swal("Ação não Permitida!", "A QTD Aprovada informada não pode ser maior que a QTD Total Aprovada", "error");
            return false;
        }*/

        if ( parseFloat(qtd_aprovado) > parseFloat(qtd_item)) {
            jQuery('[name="qtd-item-aprovado[' + iigid + '][' + iniano + ']"]').val(qtd_aprovado_salvo);
            swal("Ação não Permitida!", label, "error");
            //atualizaValoresItens(iigid, iniano);
            return false;
        }
 
        jQuery('#div_vlr_aprovado_' + iigid + '_' + iniano).html(number_format(total, 2, ",", "."));
    }

    function checarItem(iigid, iniano) {
        /*var chk_item_aprovado = jQuery('[name="chk-item-aprovado[' + iigid + '][' + iniano + ']"]:checked').val();
        var qtd_item = jQuery('[name="qtd-item[' + iigid + '][' + iniano + ']"]').val();

        if (chk_item_aprovado == 'S') {
            jQuery('[name="qtd-item-aprovado[' + iigid + '][' + iniano + ']"]').attr('readonly', true);
            jQuery('[name="qtd-item-aprovado[' + iigid + '][' + iniano + ']"]').val(qtd_item);
            atualizaValoresItens(iigid, iniano);
        }
        if (chk_item_aprovado == 'N') {
            jQuery('[name="qtd-item-aprovado[' + iigid + '][' + iniano + ']"]').attr('readonly', true);
            jQuery('[name="qtd-item-aprovado[' + iigid + '][' + iniano + ']"]').val('0');
            jQuery('#div_vlr_aprovado_' + iigid + '_' + iniano).html('0,00');
        }
        if (chk_item_aprovado == 'T') {
            jQuery('[name="qtd-item-aprovado[' + iigid + '][' + iniano + ']"]').attr('readonly', false);
        }*/
    }

    jQuery("#btn-salvar-analise").click(function () {

        $btn = jQuery(this).button('loading');

        var $dataForm = $('#form-itens-composicao').serialize();
        jQuery('[name="form_itens_composicao"]').val($dataForm);
        jQuery('[name="requisicao_analise"]').val('salvar-analise');

        jQuery('#form-iniciativa-analise-parecer').submit();
        $btn.button('reset');
    });

    function pegaAcaoWorkFlow(aedid, docid) {

        var $dataForm = $('#form-itens-composicao').serialize();
        jQuery('[name="form_itens_composicao"]').val($dataForm);
        jQuery('[name="requisicao_analise"]').val('salvar-acao-workflow');
        jQuery('[name="docid_analise"]').val(docid);
        jQuery('[name="aedid_analise"]').val(aedid);

        jQuery('#form-iniciativa-analise-parecer').submit();
    }

    jQuery(document).ready(function () {
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
        $('#div-itens-composicao').slideToggle();
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

    function js_limpaIndicadoresEtabelas() {
        js_returnIdIniciativa();
        var resp = " ";
        $("#tb_filtroIndicadores").html(resp);
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
                    $('#field_indicador').html(resp);
                    $("#tb_filtroIndicadores").html(" ");
                }
            })
            action = '&requisicao=atualizaAnosCronograma&iniid=' + iniid;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $('#form-coronograma-iniciativa').html(resp);
                }
            })
        }
    }

    function downloadDocumentoPlanejamento(arqid){
        window.location.href = window.location.href+'&requisicao=downloadDocumentoPlanejamento&arqid=' + arqid;
    }

    js_returnIdIniciativa();
</script>

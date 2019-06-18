<?php
require_once APPRAIZ . 'includes/workflow.php';
require_once APPRAIZ . 'includes/workflow_botao.php';

/**
 * Cabecalho do SIMEC.
 *
 * @see cabecalho.inc
 */

$fm = new Simec_Helper_FlashMessage('solicitacao');

if ($_POST['acao'] == 'salvar') {
    $_POST['requisicao'] = $_POST['acao'];
    $_POST['solicitacao'] = [
        'sfngrupodespesa' => $_POST['sfngrupodespesa'],
        'emeid'           => $_POST['emeid'],
        'autid'           => $_POST['autid'],
        'ptrid'           => $_POST['ptrid'],
        'sfninteressado'  => $_POST['intid'],
        'exercicio'       => $_POST['exercicio'],
        'unicod'          => $_POST['unicod'],
    ];
}

// caso a tela seja acessada pelo endereço, sem ter passado pela primeira etapa
// o usuário é redirecionado para a primeira etapa.
if (empty($_POST) && empty($_GET['sfnid'])) {
    header('Location: spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A');
    die();
}
switch ($_POST['requisicao']) {
    case 'salvar':
        try {
            $modTesouroGerencial = new Spoemendas_Model_Tesourogerencial($_POST['solicitacao']['sfnid']);
            $modSolFinanceira = new Spoemendas_Model_Solicitacaofinanceira($_POST['solicitacao']['sfnid']);
            $modSolFHistorico = new Spoemendas_Model_Solicitacaofinanceirahistorico();
            $periodoAtual = (new Spoemendas_Model_Periodosolicitacao())->retornaPeriodoAtual();
            if (!is_array($periodoAtual) && $_POST['solicitacao']['copiarSolicitacao'] == 1) {
                $fm->addMensagem('Não existe período cadastrado.', Simec_Helper_FlashMessage::ERRO);
                header('Location: spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A');
                die();
            }
            $existsNE = $modSolFinanceira->existsNE($_POST['solicitacao']['sfnnotaempenho'], $_POST['solicitacao']['sfnugexecutora']);
            $estado = ESD_NAO_ENVIADO;

            // Tratar valores
            $dados = $_POST['solicitacao'];
            $dados['sfpvalorpedido_hidden'] = removeMascaraMoeda($_POST['solicitacao']['sfpvalorpedido_hidden']);
            $dados['sfpvalorpedido'] = removeMascaraMoeda($_POST['solicitacao']['sfpvalorpedido']);

            $validapreenchimento = condValidaPreenchimento($dados);

            if ($_POST['solicitacao']['sfnid']) { // editando
                $estado = pegaSituacaoAtual($_POST['solicitacao']['sfnid']);

                if ((ESD_ENVIADO == $estado) && (is_string($validapreenchimento))) {
                    throw new Exception($validapreenchimento);
                }

                if ($existsNE && $modSolFinanceira->sfnid != $_POST['solicitacao']['sfnid']) {
                    throw new Exception('Já existe uma solicitação com esta Nota de Empenho');
                }

                $msg = 'Alterações executadas com sucesso.';
                $qStr = '&edit=true';
                $_POST['solicitacao']['sfnultimaedicao'] = 'now()';

                // -- Verifica se a solicitação possui documento
                if (empty($_POST['solicitacao']['docid'])) {
                    $_POST['solicitacao']['docid'] = (new simec_Workflow())
                        ->cadastraDocumento(WF_TPDID_SPOEMENDAS_SOLICITACOES_FINANCEIRAS, 'Nova solicitação de financeiro')
                        ->getDocid();
                }

                // verifica se algo foi alterado, se sim grava histórico
                $modSolFHistorico->historicoSolicitacao($_POST['solicitacao']);
            } else { // criando
                if ($existsNE) {
                    throw new Exception('Já existe uma solicitação com esta Nota de Empenho');
                }

                $_POST['solicitacao']['prsid'] = $periodoAtual['prsid'];
                $_POST['solicitacao']['docid'] = (new simec_Workflow())
                    ->cadastraDocumento(WF_TPDID_SPOEMENDAS_SOLICITACOES_FINANCEIRAS, 'Nova solicitação de financeiro')
                    ->getDocid();
                $msg = 'Solicitação criada com sucesso.';
                $qStr = '&add=true';
            }

            $modSolFinanceira->popularDadosObjeto($_POST['solicitacao']);
            $modTesouroGerencial->popularDadosObjeto($_POST['solicitacao']);
            $modSolFinanceira->salvar(true, true, ['sfpvalorpedido', 'sfninteressado', 'sfncontratorepasse', 'sfnpropostasiconv', 'sfnconveniosiafi', 'sfnobjetivo', 'sfnnumeroreferenciaoutros', 'sfpvalorpedido', 'sfnnotaempenho']);

            $modSolFinanceira->commit();
            $modSolFHistorico->commit();

            if ($estado == ESD_NAO_ENVIADO) {
                (new Spoemendas_Controller_Solicitacaofinanceirapedido())->inativarTodos($modSolFinanceira->sfnid);
                (new Spoemendas_Controller_Solicitacaofinanceirapedido())->salvar($modSolFinanceira->sfnid, $periodoAtual, $_POST['solicitacao']);
            }

            if ($_POST['solicitacao']['enviarSolicitacao'] == '1') {
//                $msgValidacao = condValidaPreenchimento($_POST['solicitacao']);
                if ($validapreenchimento === true) {
                    (new simec_Workflow($modSolFinanceira->docid))
                        ->setDadosAdicionais(['dados' => $_POST['solicitacao']])
                        ->alteraEstado(ESD_ENVIADO);
                } else {
                    $fm->addMensagem($validapreenchimento, Simec_Helper_FlashMessage::ERRO);
                    header('Location: spoemendas.php?modulo=principal/financeiro/solicitacao/cadastrar&acao=A&sfnid=' . $modSolFinanceira->sfnid . $qStr);
                    die();
                }
            }

            if ($_POST['solicitacao']['copiarSolicitacao'] == '1') {
                $copia = new Spoemendas_Model_Solicitacaofinanceira();
                $sfnid = $copia->copiarSolicitacao($modSolFinanceira->sfnid);

                $fm->addMensagem('Solicitação copiada com sucesso', Simec_Helper_FlashMessage::SUCESSO);
                header('Location: spoemendas.php?modulo=principal/financeiro/solicitacao/cadastrar&acao=A&sfnid=' . $copia->sfnid . $qStr);
                die();
            }

            $fm->addMensagem($msg, Simec_Helper_FlashMessage::SUCESSO);
            header('Location: spoemendas.php?modulo=principal/financeiro/solicitacao/cadastrar&acao=A&sfnid=' . $modSolFinanceira->sfnid . $qStr);
            die();

        } catch (Exception $e) {
            $fm->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
        }
        break;

    case 'carregaDadosNE':
        ob_clean();
        $sfnid = $_POST['sfnid'];
        try {
            $tgid = $_POST['tgid'];
            $ptres = $_POST['ptres'];
            $gnd = $_POST['gnd'];
            $sfnnotaempenho = $_POST['sfnnotaempenho'];
            $tg = new Spoemendas_Model_Tesourogerencial();
            $tg->carregarPorPtresGndNe($ptres, $gnd, $sfnnotaempenho);
            $sfn = new Spoemendas_Model_Solicitacaofinanceira();
            $existsNE = $sfn->existsNE($sfnnotaempenho);
            $mensagem = '';

            if ($existsNE && $sfnid != $sfn->sfnid) {
                $mensagem = 'Já existe uma solicitação com esta Nota de Empenho';
            }

            $dados['sfncodvinculacao'] = $tg->tratarResulteof();
            $dados['sfncodvinculacao_disabled'] = (new Spo_Model_Vinculacaopagamento)->carregarVinculacoes($_POST['exercicio'])[$dados['sfncodvinculacao']];
            if (empty($dados['sfncodvinculacao_disabled'])) {
                $dados['sfncodvinculacao_disabled'] = $dados['sfncodvinculacao'];
            }

            $valorLimite = $tg->emeano == date('Y') ? $tg->tgdespempenhadas - $tg->tgdesppagas : $tg->tgrapprocnproc;
            $valorLimite = formata_valor($valorLimite);

            $retorno = [
                'mensagem'           => $mensagem,
                'estuf'              => $tg->regcod,
                'muncod'             => $tg->getMuncod(),
                'tgconvsiafi'        => $tg->tgconvsiafi,
                'tgnreferencia'      => $tg->tgnreferencia,
                'valorlimite'        => $valorLimite,
                'sfncodvinculacao'   => $dados['sfncodvinculacao_disabled'],
                'sfnfontedetalhada'  => $tg->tgfonterecursodetalhada,
                'tgnreferenciaCombo' => formatarCombo($tg->getNumReferenciaDireta($ptres, $_POST['emeid'], $_POST['unicod']), 'codigo', 'descricao')
            ];
            //solicitacao_sfnnumeroreferencia
            echo simec_json_encode($retorno);
        } catch (Exception $e) {
            echo simec_json_encode([
                'mensagem' => $e->getMessage()
            ]);
        }
        exit;

    case 'municipio':
        exit((new Spoemendas_Model_Solicitacaofinanceira())->getMunicipio($_POST['uf'], true));
    case 'atualizarpedido':
        (new Spoemendas_Model_Solicitacaofinanceirapedido())->atualizaValorPedido($_POST);
        header('Location: spoemendas.php?modulo=principal/financeiro/solicitacao/cadastrar&acao=A&sfnid=' . $_GET['sfnid'] . '&edit=true');
        exit;
}

$estado = pegaSituacaoAtual($_REQUEST['sfnid']);
$opcoesCampos = [];
$bloquearCampos = !(
    (
        $estado == ESD_NAO_ENVIADO
        && (new Spoemendas_Model_Periodosolicitacao())->temPeriodoAberto()
    )
    || (
        (in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral()) ||
            in_array(PFL_UO_EQUIPE_FINANCEIRA, pegaPerfilGeral()))
        && (new Spoemendas_Model_Solicitacaofinanceira($_REQUEST['sfnid']))->isPrimeiroPeriodoAberto()
    )
    || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, pegaPerfilGeral())
    || in_array(PFL_CGF_EQUIPE_FINANCEIRA, pegaPerfilGeral())
    || in_array(PFL_SUPER_USUARIO, pegaPerfilGeral())
);
if ($bloquearCampos) {
    $opcoesCampos = ['habil' => 'N'];
}
$editSolicitacao = $_GET['edit'] == 'true';
$addSolicitacao = $_GET['add'] == 'true';

$modSolFinanceira = new Spoemendas_Model_Solicitacaofinanceira();
$modTesouroGerencial = new Spoemendas_Model_Tesourogerencial();

$dataUltAlteracao = '--/--/----';
if (isset($_GET['sfnid']) && is_numeric($_GET['sfnid'])) {
    $formValues2['solicitacao'] = (new Spoemendas_Model_Tesourogerencial())->recuperaLinha((int) $_GET['sfnid']);
    $formValues['solicitacao'] = (new Spoemendas_Model_Solicitacaofinanceira())->recuperaLinha((int) $_GET['sfnid']);
    $formValues['solicitacao'] = array_merge($formValues['solicitacao'], $formValues2['solicitacao'][0]);

    $modSolFinanceira = new Spoemendas_Model_Solicitacaofinanceira($_REQUEST['sfnid']);
    if (isset($formValues['solicitacao']['sfnnotaempenho'])) {
        $valorMax = $modSolFinanceira->inputValorSolicitar($formValues['solicitacao']['sfnid'], $formValues['solicitacao']['sfnnotaempenho']);
        $formValues['solicitacao']['sfpvalorpedido_hidden'] = $valorMax;
    }

    $linha = $modSolFinanceira->detalheSolicitacao((int) $_GET['sfnid']);
    $linha2 = $modTesouroGerencial->detalheSolicitacao((int) $_GET['sfnid']);
    $formValues = $formValues['solicitacao'];
    $formValues = combinarArrays($formValues, $linha, $linhas2);


    $formValues['sfnconveniosiafi'] = ($formValues['sfnconveniosiafi'] == 0 || !is_numeric($formValues['sfnconveniosiafi'])) ? '' : $formValues['sfnconveniosiafi'];
    $formValues['sfninteressadodsc'] = $linha['sfninteressado'];
    $formValues['sfngrupodespesadsc'] = $linha['sfngrupodespesa'];

    if (!is_null($formValues['sfnultimaedicao'])) {
        $dataUltAlteracao = date('d/m/Y', strtotime($formValues['sfnultimaedicao']));
    } else {
        $dataUltAlteracao = date('d/m/Y', time());
    }

}

include APPRAIZ . 'includes/cabecalho.inc';

(new Simec_View_Breadcrumb('C'))
    ->add('Financeiro', $_SESSION['sisdiretorio'] . '.php?modulo=inicioDashboard&acao=C')
    ->add('Solicitação de Financeiro', $_SESSION['sisdiretorio'] . '.php?modulo=principal/financeiro/solicitar_financeiro&acao=A')
    ->add('Cadastrar Solicitação')
    ->render();
?>
<link rel="stylesheet" type="text/css" href="../library/bootstrap-tagsinput-latest/bootstrap-tagsinput.css"/>
<div class="col-xs-12">
    <div style="margin-left: 960px; margin-bottom:10px">Data da última alteração da base de dados: <?php echo $dataUltAlteracao; ?></div>
    <?php if ($addSolicitacao == 'add') : ?>
        <div class="container">
            <div class="col-md-12">
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="panel-title">Solicitação</h3>
                    </div>
                    <table cellspacing="1" cellpadding="3" align="center" class="table table-striped table-bordered table-hover">
                        <tbody>
                        <tr>
                            <td>Unidade Orçamentária:</td>
                            <td><strong style="font-size:14px"><?= $formValues['unicod'] . ' - ' . $formValues['unidsc']; ?></strong></td>
                        </tr>
                        <tr>
                            <td>Autor:</td>
                            <td><strong style="font-size:14px"><?= $formValues['autnome'] ?></strong></td>
                        </tr>
                        <?
                        if (!empty($formValues['sfninteressadodsc'])) {
                            ?>
                            <tr>
                                <td>Interessado:</td>
                                <td><strong style="font-size:14px"><?= $formValues['sfninteressadodsc'] ?></strong></td>
                            </tr>
                            <?
                        }
                        ?>
                        <tr>
                            <td>Exercício:</td>
                            <td><strong style="font-size:14px"><?= $formValues['exercicio'] ?></strong></td>
                        </tr>
                        <tr>
                            <td>Emenda:</td>
                            <td>
                                <strong style="font-size:14px"><?= $formValues['emecod'] ?> <?= ($formValues['emdimpositiva'] == 6) ? '(Impositiva)' : '(Não Impositiva)'; ?></strong>
                            </td>
                        </tr>
                        <tr>
                            <td>GND:</td>
                            <td><strong style="font-size:14px"><?= $formValues['sfngrupodespesadsc'] ?></strong></td>
                        </tr>
                        <tr>
                            <td>PTRES:</td>
                            <td><strong style="font-size:14px"><?= $formValues['ptres'] ?></strong></td>
                        </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
        <?php
    endif;
    echo $fm->getMensagens();
    if (is_array($formValues)) {
        if ((array_key_exists('sfnid', $formValues) && !empty($formValues['sfnid']))
            && condValidaPreenchimento($formValues)
            && ESD_NAO_ENVIADO == pegaSituacaoAtual($formValues['sfnid'])) {
            echo <<<HTML
            <div class="col-md-12">
                <div class="alert alert-danger" role="alert">
                    <h3 class="text-center">Solicitação aguardando envio para a SPO.</h3>
                </div>
            </div>
HTML;
        }
    }

    $newFormValues['solicitacao'] = (new Spoemendas_Model_Solicitacaofinanceira())->recuperaLinha((int) $_GET['sfnid']);
    $tesouro = (new Spoemendas_Model_Tesourogerencial())->recuperaLinha((int) $_GET['sfnid']);

    $webForm = (new Simec_View_Form('solicitacao'))
        ->carregarDados($formValues)
        ->addHidden('requisicao', 'salvar')
        ->addHidden('interessadoObrigatorio', 'false');

    if ($editSolicitacao) {
        $comboPTRES = $modSolFinanceira->comboPTRES(
            $formValues['unicod'],
            $formValues['exercicio'],
            $formValues['emeid'],
            false);

        $webForm
            ->addCombo('Ano', 'exercicio', $modSolFinanceira->comboExercicio(), ['obrig' => 'S'])
            ->addCombo('Unidade Orçamentária (UO)', 'unicod', $modSolFinanceira->comboUnidade($_SESSION['usucpf'], false), ['obrig' => 'S'])
            ->addCombo('Autor', 'autid', $modSolFinanceira->comboAutor($formValues['unicod'], $formValues['exercicio'], false), ['obrig' => 'S'])
            ->addCombo('Interessado', 'sfninteressado', $modSolFinanceira->comboInteressado(null, $formValues['exercicio']), ['obrig' => 'S'])
            ->addCombo('Emenda', 'emeid', $modSolFinanceira->comboEmenda($formValues['autid'], $formValues['exercicio'], $formValues['unicod'], false), ['obrig' => 'S'])
            ->addCombo('PTRES', 'ptrid', $comboPTRES ? $comboPTRES : [], ['obrig' => 'S'])
            ->addCombo('Grupo de despesa (GND)', 'sfngrupodespesa', $modSolFinanceira->comboGND($formValues['emeid'], false), ['obrig' => 'S']);
    } else {
        $webForm
            ->addHidden('unicod', $modSolFinanceira->unicod)
            ->addHidden('autid', $modSolFinanceira->autid)
            ->addHidden('sol_sfninteressado', $modSolFinanceira->sfninteressado)
            ->addHidden('exercicio', $modSolFinanceira->exercicio)
            ->addHidden('ptrid', $formValues['ptrid'])
            ->addHidden('emeid', $modSolFinanceira->emeid)
            ->addHidden('sfngrupodespesa', $formValues['gndcod']);
    }

    $modSolFinanceira = new Spoemendas_Model_Solicitacaofinanceira();
    $modTesouroGerencial = new Spoemendas_Model_Tesourogerencial();
//    $unidades = $modTesouroGerencial->getUnidadesPerfil($_SESSION['usucpf'], [PFL_UO_EQUIPE_TECNICA, PFL_UO_EQUIPE_FINANCEIRA]);

    $webForm->addHidden('sfnid', $_GET['sfnid'])
        ->addCombo('UG Executora', 'sfnugexecutora', $modTesouroGerencial->getUg(), $opcoesCampos)
        ->addCombo('Nota de Empenho', 'sfnnotaempenho', $modSolFinanceira->comboNE($formValues['ptrid'], $formValues['gndcod'], $formValues['exercicio'], $formValues['sfnugexecutora'], false), $opcoesCampos)
        ->addHidden('sfnid', $formValues['sfnid'], 'sfnid')
        ->addHidden('tgid', $formValues['tgid'], 'tgid')
        ->addHidden('copiarSolicitacao', '', 'copiarSolicitacao')
        ->addHidden('enviarSolicitacao', '', 'enviarSolicitacao')
        ->addHidden('situacaoAtual', $estado, 'situacaoAtual')
        ->addHidden('situacaoEnviado', ESD_ENVIADO, 'situacaoEnviado')
        ->addHidden('sfncodvinculacao', $formValues['sfncodvinculacao'], 'sfncodvinculacao')
        ->addHidden('sfpvalorpedido_hidden', number_format($formValues['sfpvalorpedido_hidden'], 2, ',', '.'), 'sfpvalorpedido_hidden')
        ->addHidden('enviado_hidden', pegaSituacaoAtual($formValues['sfnid']), 'enviado_hidden')
        ->addHidden('enviado_const_hidden', ESD_ENVIADO, 'enviado_const_hidden')
        ->addHidden('sfhatual', '', 'sfhatual')
        ->addTexto('Vinculação', 'sfncodvinculacao_disabled', 250, ['habil' => 'N'])
        ->addTexto('Fonte de Recurso Detalhada', 'sfnfontedetalhada', 10, ['masc' => '##########', 'habil' => 'N'])
        ->addTexto('UF', 'estuf', 2, ['habil' => 'N'])
        ->addCombo('Município', 'muncod', $modSolFinanceira->getMunicipio($formValues['estuf']), $opcoesCampos)
        ->addTexto('Contrato Repasse', 'sfncontratorepasse', 150, $opcoesCampos)
        ->addTexto('Proposta no SICONV', 'sfnpropostasiconv', 150, $opcoesCampos)
        ->addTexto('Convênio SIAFI', 'sfnconveniosiafi', 150, $opcoesCampos)
        ->addComboOutros('Nº de Referência constante na nota de empenho no SIAFI', 'sfnnumeroreferencia',
            $modTesouroGerencial->getNumReferencia($_GET['sfnid']),
            array_merge($opcoesCampos, ['class' => 'referencia']), // opções do combo
            $opcoesCampos, // opções do campo outros
            150 //limite do campo outros
        )
        ->addCombo('Máquina/Equipamento', 'sfnmequipamento', [
            ['codigo' => '1', 'descricao' => 'SIM'],
            ['codigo' => '2', 'descricao' => 'NÃO']
        ], $opcoesCampos)
        ->addTexto('Objeto', 'sfnobjetivo', 250, $opcoesCampos)
        ->addCombo('Grau de Prioridade', 'sfngrauprioridade', [
            ['codigo' => '1', 'descricao' => '1 - Alta'],
            ['codigo' => '2', 'descricao' => '2 - Media'],
            ['codigo' => '3', 'descricao' => '3 - Baixa'],
        ], $opcoesCampos)
        ->setRequisicao('salvar');


    if (pegaSituacaoAtual($_GET['sfnid']) == ESD_ENVIADO) {
        $webForm
            ->addHidden('sfpvalorpedido', mascaraMoeda($formValues['sfpvalorpedido'], true, true));

        if (!$bloquearCampos) {
            $webForm->addBotao('salvar', ['label' => 'Salvar e enviar', 'cor' => 'success', 'id' => 'salvarEnviar']);
        }
    } elseif ($_GET['edit'] == 'true') {
        $webForm
            ->addMoeda('Valor a Solicitar', 'sfpvalorpedido', $opcoesCampos)
            ->addBotoes(['salvar']);
    } else {
        $webForm
            ->addMoeda('Valor a Solicitar', 'sfpvalorpedido', $opcoesCampos)
            ->addBotoes(['limpar', 'salvar']);
    }

    $webForm->render();
    $estadoAtualFlag = true;
    wf_desenhaBarraNavegacaoBotao($formValues['docid'], ['dados' => $formValues]);

    if (pegaSituacaoAtual($_GET['sfnid']) == ESD_ENVIADO) {
        (new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO))
            ->setQuery((new Spoemendas_Model_Solicitacaofinanceirapedido())->buscarPorSfnid($_GET['sfnid'], true))
            ->setCabecalho(['Mês de Referência', 'Valor Solicitado', 'Valor Autorizado', 'Data da Autorização', 'Situação do Pedido'])
            ->addAcao('edit',
                ['func' => 'editarPedido', 'extra-params' => ['sfpvalorpedido', 'mes']])
            ->setAcaoComoCondicional('edit', [['campo' => 'periodoatual', 'op' => 'igual', 'valor' => 1]])
            ->esconderColunas(['periodoatual'])
            ->addCallbackDeCampo(['sfpvalorpedido', 'sfpvalorautorizado'], 'mascaraMoedaNula')
            ->addCallbackDeCampo('mes', 'mascaraMesCurtoAno')
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
    ?>
</div>

<!-- modal editar pedido -->
<div class="modal fade" id="mod-edit-pedido" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span>
                </button>
                <h4 class="modal-title" id="myModalLabel">Editar Pedido</h4>
            </div>
            <div class="modal-body">
                <div class="well">
                    <form id="form-mod-edit-pedido" class="form-horizontal" method="POST">
                        <div class="form-group">
                            <label for="mes">Mês de Referencia:</label>
                            <span id="mesreferencia"></span>
                        </div>
                        <div class="form-group">
                            <label class="control-label col-sm-2" for="valorsolicitacao">Valor Solicitado:</label>
                            <div class="col-sm-10">
                                <input type="text" style="text-align:left;" name="valorsolicitacaop"
                                       size="1" maxlength="22" value=""
                                       onkeyup="this.value=mascaraglobal('###.###.###.###.###,##',this.value);"
                                       onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();"
                                       onmouseout="MouseOut(this);"
                                       onblur="MouseBlur(this);this.value=mascaraglobal('###.###.###.###.###,##',this.value);"
                                       id="valorsolicitacaop" class="normal form-control  required"
                                       title="">
                            </div>
                        </div>
                        <input type="hidden" name="requisicao" value="atualizarpedido">
                        <input type="hidden" name="sfpid" id="sfpid">
                    </form>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-danger" data-dismiss="modal">
                        <i class="glyphicon glyphicon-remove"></i> Cancelar
                    </button>

                    <button id="btn-mod-edit-pedido" type="button" class="btn btn-primary">
                        <i class="glyphicon glyphicon-floppy-disk"></i> Salvar
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript" src="jquery.livequery.js"></script>
<script type="text/javascript">

    // [fix] variaveis auxiliares para formar o historico no cadastro da solicitacao
    var axexercicio = '<?= $formValues['exercicio'] ?>';
    var axunidade = '<?= $formValues['unicod'] . ' - ' . $formValues['unidsc']; ?>';
    var axautor = '<?= $formValues['autnome'] ?>';
    var axinteressado = '<?= $formValues['sfninteressado'] ?>';
    var axemenda = '<?= $formValues['emecod'] ?>';
    var axgnd = '<?= $formValues['sfngrupodespesa'] ?>';
    var axptres = '<?= $formValues['ptres'] ?>';

    var unidade = $('#solicitacao_unicod');
    var autor = $('#solicitacao_autid');
    var interessado = $('#solicitacao_sfninteressado');
    var exercicio = $('#solicitacao_exercicio');
    var emenda = $('#solicitacao_emeid');
    var gnd = $('#solicitacao_sfngrupodespesa');
    var ptres = $('#solicitacao_ptrid');
    var uge = $('#solicitacao_sfnugexecutora');
    var ne = $('#solicitacao_sfnnotaempenho');

    var solicitacao = window.solicitacao = {};
    solicitacao.exercicio = '<?php echo $_SESSION['exercicio']; ?>';
    solicitacao.usucpf = '<?php echo $_SESSION['usucpf']; ?>';

    function editarPedido(sfpid, valor, data) {
        var ano = data.split('-')[0];
        var mes = data.split('-')[1];
        $("#mesreferencia").html('');
        $("#mesreferencia").append(mes_extenso(mes) + '/' + ano);

        valor = parseFloat(valor).toFixed(2);

        $("#valorsolicitacaop").val(valor);
        $('#valorsolicitacaop').mask('000.000.000.000.000,00', {reverse: true});
        $("#sfpid").val(sfpid);
        $("#mod-edit-pedido").modal('show');
    }

    function verificaLimite() {
        $("#btn-mod-edit-pedido").removeAttr('disabled');
        if (parseFloat($("#valorsolicitacaop").val().replace(/[.]/gi, '').replace(',', '.'))
            > parseFloat($("#sfpvalorpedido_hidden").val().replace(/[.]/gi, '').replace(',', '.'))) {
            bootbox.confirm({
                message: "O valor solicitado é maior que o Valor Limite. Deseja solicitar o Valor Limite disponível?",
                buttons: {
                    confirm: {
                        label: 'Sim',
                        className: 'btn-success'
                    },
                    cancel: {
                        label: 'Não',
                        className: 'btn-danger'
                    }
                },
                callback: function (result) {
                    if (result) {
                        $("#valorsolicitacaop").val($("#sfpvalorpedido_hidden").val());
                    }
                }
            });
        } else {
            return true;
        }
    }

    //Trata para Busca os mes descrição
    function mes_extenso(mes) {
        if (mes == '01') {
            return 'JAN';
        } else if (mes == '02') {
            return 'FEV';
        } else if (mes == '03') {
            return 'MAR';
        } else if (mes == '04') {
            return 'ABR';
        } else if (mes == '05') {
            return 'MAI';
        } else if (mes == '06') {
            return 'JUN';
        } else if (mes == '07') {
            return 'JUL';
        } else if (mes == '08') {
            return 'AGO';
        } else if (mes == '09') {
            return 'SET';
        } else if (mes == '10') {
            return 'OUT';
        } else if (mes == '11') {
            return 'NOV';
        } else if (mes == '12') {
            return 'DEZ';
        }
    }

    function verificarEmpenhado(result) {
        var textbox = $('#solicitacao_sfpvalorpedido'),
            maxVal = parseFloat(result);
        $('#sfpvalorpedido_hidden').val(maxVal);

        textbox.on("blur", function () {
            var thisVal2 = this.value.replace(/\./g, '').replace(/,/g, ".");
            if (isNaN(thisVal2) || thisVal2 > maxVal) {
                textbox.val('0,00');
                alert("O valor solicitado é maior que o limite");
            }
        });
    }

    window.onload = function () {
        window.scrollTo(0, 0);
    };

    $().ready(function () {
        $.each($(".wf_buttons").find("li"), function (i, el) {
            $("#solicitacao").append($(el).html() + "&nbsp;");
        });
        $(".wf_buttons").remove();

        $(document).keypress(function (e) {
            if (e.which == 13) {
                e.preventDefault();
            }
        });

        var selectedAutor = $("#solicitacao_autid").find(":selected").text();
        if (selectedAutor != 'Relator Geral' &&
            !selectedAutor.includes('Com.') &&
            !selectedAutor.includes('Bancada')) {
            $("#solicitacao_intid").removeAttr('required').parent().parent().hide();
        }

        $("#solicitacao_sfpvalorpedido").focus().blur();

        (function () {
            var options = {
                    size: 12
                },
                $inputTag = $("#solicitacao_sfnnotaempenho"),
                temaplteTag = '<span class="tag label label-info"><span>[%tag%]</span><span data-role="remove"></span></span>',
                allTags = ($inputTag.val()) ? $inputTag.val().split(",") : []
            ;

            function update() {
                $("#tag-container").html("");
                allTags.forEach(function (item) {
                    $("#tag-container").append(temaplteTag.replace("[%tag%]", item));
                });

                $("#solicitacao_tmp_sfnnotaempenho").remove();
                $("#solicitacao").append("<input type='hidden' id='solicitacao_tmp_sfnnotaempenho' name='solicitacao[solicitacao_tmp_sfnnotaempenho]'/>");
                $("#solicitacao_tmp_sfnnotaempenho").val(allTags.join(", "));
            }

            $inputTag.focus(function () {
                if ($(this).val().length <= 6) {
                    $(this).val(exercicio.val() + 'NE');
                }
            });

            $("[data-role='remove']").livequery("click", function () {
                var item = $(this).parent().find("span:eq(0)").html();
                allTags.forEach(function (val, i) {
                    if (val == item) {
                        allTags.splice(i, 1);
                    }
                });
                update();
            });

            $(".bootstrap-tagsinput").css({
                "background-color": "both",
                "border": "none"
            });
        })();
    });
</script>
<script type="text/javascript" src="solicitacao_financeira.js"></script>

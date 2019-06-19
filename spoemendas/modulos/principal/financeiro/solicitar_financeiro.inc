<?php
/**
 * Cabecalho do SIMEC.
 *
 * @see cabecalho.inc
 */
$modSolFinanceira = new Spoemendas_Model_Solicitacaofinanceira();
$modTesGerencial = new Spoemendas_Model_Tesourogerencial();
$msg = new Simec_Helper_FlashMessage('financeiro/solicitar_financeiro');
$fm = new Simec_Helper_FlashMessage('solicitacao');

if ($_POST['requisicao']) {
    $_POST['acao'] = $_POST['requisicao'];
}

switch ($_POST['acao']) {
    case "UO":
        exit($modSolFinanceira->comboUnidade($_POST['usucpf']));

    case "UGE":
        exit(simec_json_encode(formatarCombo($modTesGerencial->getUg($_POST['unicod']), 'codigo','descricao')));

    case "autor":
        exit($modSolFinanceira->comboAutor($_POST['unicod'], $_POST['exercicio']));

    case "interessado":
        exit($modSolFinanceira->comboInteressado(null, $_POST['exercicio'], true));

    case "emenda":
        exit($modSolFinanceira->comboEmenda($_POST['autid'], $_POST['exercicio'], $_POST['unicod']));

    case "ptres":
        exit($modSolFinanceira->comboPTRES($_POST['unicod'], $_POST['exercicio'], $_POST['emeid']));

    case "gnd":
        exit($modSolFinanceira->comboGND($_POST['emenda']));

    case "ne":
        exit($modSolFinanceira->comboNE($_POST['ptres'], $_POST['gnd'], $_POST['exercicio'], $_POST['uge']));

    case "historico":
        exit($modSolFinanceira->historicoAlteracao($_POST['sfnid']));

    case 'detalheHistorico':
        exit($modSolFinanceira->historicoDetalhe($_POST['dados'][0]));

    case 'detalhar-emenda':
        exit(simec_json_encode($modSolFinanceira->detalheSolicitacao($_POST['sfnid'])));

    case 'copySolicitacao':
        try {
            echo $modSolFinanceira->copiarSolicitacao($_POST['sfnid']);
        } catch (Exception $e) {
            echo $e->getMessage();
        }
        exit;
    case 'removerSolicitacao' :
        $modSolFinanceira->removeSolcitacao($_POST['sfnid']);
        exit;
    case 'podeRemover':
        echo $modSolFinanceira->podeRemoverSolicitacaoFinanceira($_POST['sfnid']);
        exit;
}

function form()
{
    require_once APPRAIZ . 'includes/library/simec/Form.php';
    $modSolFinanceira = new Spoemendas_Model_Solicitacaofinanceira();

    (new Simec_Form())
        ->setFormOff()
        ->addHidden('interessadoObrigatorio', 'true')
        ->addSelect('exercicio', 'Ano', null, $modSolFinanceira->comboExercicio(), ['required'])
        ->addSelect('unicod', 'Unidade Orçamentária', null, [], ['required'])
        ->addSelect('autid', 'Autor', null, [], ['required'])
        ->addSelect('intid', 'Interessado', null, $modSolFinanceira->comboInteressado())
        ->addSelect('emeid', 'Emenda', null, [], ['required'])
        ->addSelect('ptrid', 'PTRES', null, [], ['required'])
        ->addSelect('sfngrupodespesa', 'Grupo de despesa (GND)', null, [], ['required'])
        ->render();
}

include APPRAIZ . 'includes/cabecalho.inc';

if ($redirect = filter_input(INPUT_GET, 'redirect', FILTER_SANITIZE_STRING)) {
    if ($redirect == 'dashboard') {
        $esdid = filter_input(INPUT_GET, 'esdid');
        $situacao = filter_input(INPUT_GET, 'situacao');

        echo <<<JAVASCRIPT
<script type="text/javascript">
    $('<form/>',{
        id:"form-redirect-hidden",
        method:"post",
        action:"spoemendas.php?modulo=principal/financeiro/solicitar_financeiro&acao=A"
    }).appendTo("body");

    $("#form-redirect-hidden").append($('<input/>', {name:"emendas[enviado]",value:"{$esdid}",type:'hidden'}))
    $("#form-redirect-hidden").append($('<input/>', {name:"emendas[situacaopedido]",value:"{$situacao}",type:'hidden'}));

    $("#form-redirect-hidden").submit();
</script>
JAVASCRIPT;
        exit(0);
    }
}

(new Simec_View_Breadcrumb('C'))
    ->add('Financeiro', $_SESSION['sisdiretorio'] . '.php?modulo=inicioDashboard&acao=C')
    ->add('Solicitação de Financeiro')
    ->render();
?>

<!-- Formulário de pesquisa -->
<div class="col-md-12">
    <?php

    $options = (new Spoemendas_Model_Solicitacaofinanceirasituacao())->montarCombo();
    unset($options[4]);

    $form = new Simec_View_Form('emendas', $method = Simec_View_Form::POST, $action = '');
    $form->carregarDados($_REQUEST['emendas'])
        ->setTitulo('Consulta de Solicitações')
        ->addCombo('Unidade Orçamentária (UO)', 'unicod', $modSolFinanceira->comboUnidade($_SESSION['usucpf'], false))
        ->addCombo('Autor', 'autid', $modSolFinanceira->comboAutor())
        ->addCombo('Interessado', 'intid', $modSolFinanceira->comboInteressado())
        ->addCombo('Ano', 'exercicio', $modSolFinanceira->comboExercicio(), ['multiple' => true])
        ->addTexto('Emenda', 'emecod', 8, ['masc' => '########'])
        ->addCombo('Grupo de despesa (GND)', 'sfngrupodespesa', $modSolFinanceira->comboGND())
        ->addTexto('PTRES', 'ptres', 6, ['masc' => '######'])
        ->addCombo('Mês de Referência', 'mesreferencia', [
            ['codigo' => 1, 'descricao' => 'Janeiro'],
            ['codigo' => 2, 'descricao' => 'Fevereiro'],
            ['codigo' => 3, 'descricao' => 'Março'],
            ['codigo' => 4, 'descricao' => 'Abril'],
            ['codigo' => 5, 'descricao' => 'Maio'],
            ['codigo' => 6, 'descricao' => 'Junho'],
            ['codigo' => 7, 'descricao' => 'Julho'],
            ['codigo' => 8, 'descricao' => 'Agosto'],
            ['codigo' => 9, 'descricao' => 'Setembro'],
            ['codigo' => 10, 'descricao' => 'Outubro'],
            ['codigo' => 11, 'descricao' => 'Novembro'],
            ['codigo' => 12, 'descricao' => 'Dezembro']
        ])
        ->addCombo('Enviado', 'enviado', [
            ['codigo' => ESD_ENVIADO, 'descricao' => 'Sim'],
            ['codigo' => ESD_NAO_ENVIADO, 'descricao' => 'Não']
        ])

        ->addCombo('Situação do pedido', 'situacaopedido', $options);

    if ((new Spoemendas_Model_Periodosolicitacao())->temPeriodoAberto()) {
        $form->addBotao('avancado', [
            'id'    => 'novaSolicitacao',
            'label' => 'Nova Solicitação',
            'span'  => 'glyphicon glyphicon-plus',
            'extra' => 'onclick="onEmendasNovo()"',
            'class' => 'btn-success'
        ]);
    }

    $form->addBotao('buscar')
        ->addBotao('limpar')
        ->render();

    // Listagem
    $listagem = (new Simec_Listagem())
        ->setCabecalho([
            'Ano Emenda',
            'Emenda',
            'Unidade<br>Orçamentária',
            'Autor',
            'Interessado',
            'Enviado',
            'Nota de Empenho',
            'PTRES',
            'UF - Município',
            'GND',
            'Fonte Recursos<br>Detalhada',
            'Valor a Solicitar (R$)',
            'Situação do Pedido'
        ])
        ->addAcao('edit', 'editarSolicitacao')
        ->addAcao('share', ['func' => 'copySolicitacao', 'extra-params' => ['mensagem'], 'titulo' => 'Criar Nova Solicitação'])
        ->addAcao('info', 'infoSolicitacao')
        ->addAcao('trash', 'removerSolicitacao')
        ->addAcao('history', 'historico')
        ->setQuery((new Spoemendas_Model_Solicitacaofinanceira())->queryDeSolicitacoes($_REQUEST['emendas']))
        ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::PESQUISATOR)
        ->addCallbackDeCampo('unicod', 'formatarCodigoUnidade')
        ->addCallbackDeCampo(['sfnnotaempenho', 'ptres', 'sfninteressado', 'sfnfontedetalhada'], 'formatarVazio')
        ->addCallbackDeCampo('estuf', 'formatarUfMunicipio')
        ->addCallbackDeCampo('sfnvlrsolicitado', 'mascaraMoeda')
        ->esconderColunas('uniabrev', 'unidsc', 'mundescricao', 'periodoaberto', 'mensagem');

    if (in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral())
        || in_array(PFL_UO_EQUIPE_FINANCEIRA, pegaPerfilGeral())) {
        $listagem->setAcaoComoCondicional('edit', [['campo' => 'periodoaberto', 'op' => 'diferente', 'valor' => 0]]);
    }

    echo $fm, $msg;
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

    $htmlDetalhe = <<<HTML
<table class="table">
<tr>
    <td colspan="2"><b>Período:</b> <span id="mdlprsdescricao"></span> <span id="mdlprsdatainicio"></span> a <span id="mdlprsdatafim"></span></td>
    <td colspan="2"><b>UO:</b> <span id="mdlunicod"></span> - <span id="mdlunidsc"></span></td>
</tr>
<tr>
    <td><b>Autor:</b> <span id="mdlautnome"></span></td>
    <td><b>Emenda:</b> <span id="mdlemecod"></span></td>
    <td colspan="2"><b>Nota de Empenho:</b> <span id="mdlne"></span></td>
</tr>
<tr>
    <td><b>Interessado:</b> <span id="mdlsfninteressado"></span></td>
    <td><b>UG Executora:</b> <span id="mdlsfnugexecutora"></span></td>
    <td colspan="2"><b>Grupo de Despesa (GND):</b> <span id="mdlsfngrupodespesa"></span></td>
</tr>
<tr>
    <td colspan="2"><b>Programa de Trabalho Resumido (PTRES):</b> <span id="mdlptres"></span></td>
    <td colspan="2"><b>Vinculação:</b> <span id="mdlsfncodvinculacao"></span></td>
</tr>
<tr>
    <td colspan="2"><b>Fonte de Recursos Detalhada:</b> <span id="mdlsfnfontedetalhada"></span></td>
    <td colspan="2"><b>UF - Município:</b> <span id="mdlestuf"></span> - <span id="mdlmundescricao"></span></td>
</tr>
<tr>
    <td colspan="2"><b>Contrato de Repasse:</b> <span id="mdlsfncontratorepasse"></span></td>
    <td colspan="2"><b>Proposta no SICONV:</b> <span id="mdlsfnpropostasiconv"></span></td>
</tr>
<tr>
    <td colspan="2"><b>Convênio SIAFI:</b> <span id="mdlsfnconveniosiafi"></span></td>
    <td colspan="2"><b>No de Referência:</b> <span id="mdlsfnnumeroreferencia"></span></td>
</tr>
<tr>
    <td colspan="4"><b>Máquina/Equipamento:</b> <span id="mdlsfnmequipamento"></span></td>
</tr>
<tr>
    <td colspan="2"><b>Objeto:</b> <span id="mdlsfnobjetivo"></span></td>
    <td colspan="2"><b>Grau de Prioridade:</b> <span id="mdlsfngrauprioridade"></span></td>
</tr>
<tr>
    <td colspan="3"><b>Valor a Solicitar: R$ </b> <span id="mdlsfnvlrsolicitado"></span></td>
    <td><b>Impositiva: </b> <span id="mdlemdimpositiva"></span></td>
</tr>
</table>
HTML;

    ob_start();
    bootstrapPanel('Informações - Ano: <span id="mdlexercicio"></span>', $htmlDetalhe, 'primary', ['cols' => 12]);
    bootstrapPopup(
        'Detalhamento da solicitação',
        'detalheemenda',
        ob_get_clean(),
        ['fechar'],
        ['tamanho' => 'lg']
    );
    ?>
</div>

<!-- modal nova solicitação -->
<div class="modal fade" id="modNovaSolicitacao" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Nova Solicitação</h4>
            </div>
            <div class="modal-body">

                <div class="well">
                    <form id="formNovaSolicitacop" class="form-horizontal" method="POST" action="spoemendas.php?modulo=principal/financeiro/solicitacao/cadastrar&acao=A">
                        <input type="hidden" name="acao" value="salvar">
                        <?php form() ?>
                    </form>

                    <span class="pull-right">(*) Campos obrigatórios</span>
                </div>

                <div class="modal-footer">

                    <button type="button" class="btn btn-warning" onclick="resetar('formNovaSolicitacop')">
                        <i class="glyphicon glyphicon-minus"></i>
                        Limpar
                    </button>

                    <button type="button" class="btn btn-danger" data-dismiss="modal">
                        <i class="glyphicon glyphicon-chevron-left"></i>
                        Voltar
                    </button>

                    <button id="modalSeguir" type="button" class="btn btn-primary">
                        <i class="glyphicon glyphicon-chevron-right"></i>
                        Seguir
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    <?php if (isset($_GET['edit'])): ?>
    var unidade = $('#solicitacao').find('#solicitacao_unicod');
    var autor = $('#solicitacao').find('#solicitacao_autid');
    var interessado = $('#solicitacao').find('#solicitacao_intid');
    var exercicio = $('#solicitacao').find('#solicitacao_exercicio');
    var emenda = $('#solicitacao').find('#solicitacao_emeid');
    var gnd = $('#solicitacao').find('#solicitacao_sfngrupodespesa');
    var ptres = $('#solicitacao').find('#solicitacao_ptrid');
    var uge = $('#solicitacao_sfnugexecutora');
    <?php else : ?>
    var unidade = $('#modNovaSolicitacao').find('#formulario_unicod');
    var autor = $('#modNovaSolicitacao').find('#formulario_autid');
    var interessado = $('#modNovaSolicitacao').find('#formulario_intid');
    var exercicio = $('#modNovaSolicitacao').find('#formulario_exercicio');
    var emenda = $('#modNovaSolicitacao').find('#formulario_emeid');
    var gnd = $('#modNovaSolicitacao').find('#formulario_sfngrupodespesa');
    var ptres = $('#modNovaSolicitacao').find('#formulario_ptrid');
    var uge = $('#solicitacao_sfnugexecutora');
    <?php endif; ?>
    var ne = $();

    var solicitacao = window.solicitacao = {};
    solicitacao.exercicio = '<?php echo $_SESSION['exercicio']; ?>';
    solicitacao.usucpf = '<?php echo $_SESSION['usucpf']; ?>';
</script>
<script type="text/javascript" src="solicitacao_financeira.js"></script>

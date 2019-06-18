<?php
$dataGrid = new Simec_Listagem();
$dataGrid->setCabecalho([
    'Ano Emenda',
    'Emenda',
    'Unidade<br>Orçamentária',
    'Autor',
    'Situação',
    'Nota de Empenho',
    'PTRES',
    'UF - Município',
    'GND',
    'Solicitante',
    'Fonte Recursos<br>Detalhada',
    'Valor a Solicitar (R$)'
])
    ->addAcao('edit', 'editarSolicitacao')
    ->addAcao('share', array('func' => 'copySolicitacao', 'extra-params' => ['mensagem'], 'titulo' => 'Criar Nova Solicitação'))
    ->addAcao('info', 'infoSolicitacao')
    ->setQuery((new Spoemendas_Model_Solicitacaofinanceira())->queryDeSolicitacoes($_REQUEST['emendas']))
    ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::PESQUISATOR)
    ->addCallbackDeCampo('unicod', 'formatarCodigoUnidade')
    ->addCallbackDeCampo(['ne', 'ptres', 'sfninteressado'], 'formatarVazio')
    ->addCallbackDeCampo('estuf', 'formatarUfMunicipio')
    ->addCallbackDeCampo('sfnvlrsolicitado', 'mascaraMoeda')
    ->esconderColunas('uniabrev', 'unidsc', 'mundescricao', 'periodoaberto', 'mensagem')
    ->setAcaoComoCondicional('edit', [['campo' => 'periodoaberto', 'op' => 'igual', 'valor' => 1]])
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>

<script type="text/javascript">
var editarSolicitacao = function(id) {
    //location.href="spoemendas.php?modulo=principal/financeiro/solicitacao/cadastrar&acao=A&sfnid="+id+"&edit=true";
};
</script>

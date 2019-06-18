<?php
/**
 * Formulário de cadastro e alteração de períodos de referência.
 *
 * $Id: formPeriodoreferencia.inc 97014 2015-05-07 20:07:34Z maykelbraz $
 */
if ($oPerRef)
{
    $dados = $oPerRef->getDados();
}

$form = new Simec_View_Form('periodoreferencia');
$form->addHidden('prfid', $dados['prfid'])
    ->addInputTexto('prfnome', $dados['prfnome'], 'prfnome', 100, false, ['flabel' => 'Título'])
    ->addInputTexto('origenseditaveis', $dados['origenseditaveis'], 'origenseditaveis', 100, false, ['flabel' => 'Origens Editáveis'])
    ->addInputTexto('perfiseditaveis', $dados['perfiseditaveis'], 'perfiseditaveis', 100, false, ['flabel' => 'Perfis Editáveis'])
    ->addChoices('Atual', 'prfatual', ['Sim' => 't', 'Nao' => 'f'])
    ->addInputTextarea('abasvisiveis', $dados['abasvisiveis'], 'abasvisiveis', 500, ['flabel' => 'Abas Visíveis'])
    ->addInputData(
        ['prfdatainicio', 'prfdatafim'],
        [$dados['prfdatainicio'], $dados['prfdatafim']],
        ['prfdatainicio', 'prfdatafim'],
        ['flabel' => 'Período de validade']
    )
    ->addSeparador('<b>Tags de Substituição para Mensagem: </b>[_DATA_INICIAL_]&nbsp;&nbsp;&nbsp;[_DATA_FINAL_]&nbsp;&nbsp;&nbsp;[_DIAS_RESTANTES_]', ['tag' => 'h5 style="margin-bottom: 25px;"', 'classes' => 'col-lg-offset-2'])
    ->addInputTextarea('prfmensagem', $dados['prfmensagem'], 'prfmensagem', 1000, ['flabel' => 'Mensagem para Parlamentares'])
//    ->addInputData(
//        array('prfpreenchimentoinicio', 'prfpreenchimentofim'),
//        array($dados['prfpreenchimentoinicio'], $dados['prfpreenchimentofim']),
//        array('prfpreenchimentoinicio', 'prfpreenchimentofim'),
//        array('flabel' => 'Período de preenchimento')
//    )
    ->addBotoes(['limpar', 'salvar'])
    ->setRequisicao('salvar')
    ->render();

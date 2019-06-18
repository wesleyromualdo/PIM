<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script type="text/javascript">
function verDetalheBeneficiario(edeid, enbid, edecpfresp, edenomeresp, ededddresp, edetelresp, edemailresp)
{
    $('#responsavelpta_edeid').val(edeid);
    $('#responsavelpta_enbid').val(enbid);
    $('#responsavelpta_edecpfresp').val(edecpfresp).blur();
    $('#responsavelpta_edenomerep').val(edenomeresp);

    $('#responsavelpta_telefone').val(
        '(' + ededddresp + ') ' + edetelresp
    );

    $('#responsavelpta_edemailresp').val(edemailresp);
    $('#responsavelpta_enbid').val(enbid);
    $('#modal-responsavelpta').modal();
}

$(function(){
    $('#responsavelpta_edecpfresp').change(function(e){
        $.get(window.location.href, {requisicao:'consultarcpf', 'dados[cpf]':$(e.target).val()}, function(resp){
            $('#responsavelpta_edenomerep').val(resp.dados.no_pessoa_rf);
            $('#responsavelpta_telefone').focus();
        }, 'json');
    });
});

function onResponsavelptaSubmit()
{
    var mensagens = new Array();
    if ('' == $('#responsavelpta_edecpfresp').val()) {
        mensagens.push('O Campo CPF é obrigatório e não pode ficar em branco');
    }
    if ('' == $('#responsavelpta_edenomerep').val()) {
        mensagens.push('O Campo Nome é obrigatório e não pode ficar em branco');
    }
    if ('' == $('#responsavelpta_telefone').val()) {
        mensagens.push('O Campo Telefone é obrigatório e não ficar em branco');
    }
    if ('' == $('#responsavelpta_edemailresp').val()) {
        mensagens.push('O Campo E-Mail é obrigatório e não pode ficar em branco');
    }

    if (mensagens.length > 0) {
        var mensagem = mensagens.join(';</li><li>');
        bootbox.alert('Antes se realizar as alterações, verifique os seguintes itens: <br /><br /><ul><li>' + mensagem + '.</li></ul>');
        return false;
    }

    return true;
}
</script>
<div class="col-md-12">
<?php
/**
 * Sistema de Publicação.
 * werteralmeida
 */
$dados['emeid'] = $_REQUEST['emeid'];
/*
* Beneficiários
*/
$sql = <<<DML
SELECT DISTINCT ede.edeid,
                enb.enbid,
                enb.enbcnpj,
                enb.enbnome,
                ede.edecpfresp,
                ede.edenomerep,
                ede.ededddresp,
                ede.edetelresp,
                edemailresp,
                TO_CHAR(enbdataalteracao, 'DD/MM/YYYY') AS data_alteracao
  FROM emenda.emendadetalhe emd
    JOIN emenda.emendadetalheentidade ede USING(emdid)
    JOIN emenda.entidadebeneficiada enb USING(enbid)
  WHERE emeid = {$dados['emeid']}
    AND enb.enbcnpj <> ''
    and emdstatus = 'A'
    AND enbstatus = 'A'
    AND edestatus = 'A' 
DML;
$arrColunas = array('CNPJ', 'Beneficiário', 'CPF', 'Responsável', 'Contato', 'Úlima Alteração');
$listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);
$listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj');
$listagem->addCallbackDeCampo('edevalor', 'mascaraMoeda');
$listagem->addCallbackDeCampo('edecpfresp', 'formatarcpf');
$listagem->addCallbackDeCampo(array('enbnome'), 'alinhaParaEsquerda')
    ->addCallbackDeCampo('ededddresp', 'formatarContato');
$listagem->setCabecalho($arrColunas)->setQuery($sql);
$permissaoEdicao = new Spoemendas_Controller_Permissaoedicaoemenda($_REQUEST['emeid']);
$podeSalvar = $permissaoEdicao->geraPermissoesEdicao();
if ($podeSalvar) {
    $listagem->addAcao('edit', ['func' => 'verDetalheBeneficiario', 'extra-params' => [
        'enbid',
        'edecpfresp',
        'edenomerep',
        'ededddresp',
        'edetelresp',
        'edemailresp'
    ]]);
} else {
    $listagem->esconderColunas('edeid');
}
$listagem->esconderColunas('enbid', 'edetelresp', 'edemailresp')
    ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('edevalor'));
$dadosBeneficiarios = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);

$form = new Simec_View_Form('responsavelpta');
$htmlForm = $form->setRequisicao('salvar-beneficiariopta')
    ->addHidden('edeid', $dados['edeid'])
    ->addHidden('enbid', $dados['enbid'])
    ->addTexto('CPF', 'edecpfresp', 14, ['masc' => '###.###.###-##'])
    ->addTexto('Nome', 'edenomerep', $limite)
    ->addTexto('Telefone', 'telefone', 14)
    ->addTexto('E-Mail', 'edemailresp', $limite)
    ->render(false);

bootstrapPopup('Responsável pelo Plano de Trabalho', 'modal-responsavelpta', $htmlForm, ['fechar', 'confirmar']);
?>
</div>
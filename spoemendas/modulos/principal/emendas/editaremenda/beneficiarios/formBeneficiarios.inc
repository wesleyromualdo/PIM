<div class="col-md-12">
    <?php
    global $beneficiarioVisaoAdm;

    $arrColunas = array('CNPJ', 'Beneficiário','Prioridade', 'GND', 'Modalidade', 'Indicado (R$)', 'Limite de Empenho do Beneficiário (R$)');

    if ($beneficiarioVisaoAdm) {
        array_push($arrColunas, 'Status DetRec');
    }

    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);
    $listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj');
    $listagem->addCallbackDeCampo(array('enbnome'), 'alinhaParaEsquerda');
    $listagem->setCabecalho($arrColunas)
        ->setQuery(Spoemendas_Model_Entidadebeneficiada::queryBeneficiarioByEmeid(
            $_REQUEST['emeid'],
            $beneficiarioVisaoAdm
        ));
    /*
     * Condição para não alterar a aba de entidade beneficiada, descrito que 
     * não iriam alterar.
     */
    $podeSalvar = true;
    if (!$podeSalvar) {
        $listagem->addAcao('edit', [
            'func' => 'editarBeneficiario',
            'extra-params' => ['enbcnpj', 'enbnome', 'edevalor']
        ])->addAcao('delete', 'apagarBeneficiario')
        ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::ADICIONAR);
    } else {
        $listagem->esconderColunas('enbid', 'edeid');
    }

    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['edevalor','edevalordisponivel'])
        ->addCallbackDeCampo(['edevalor','edevalordisponivel', 'edelimiteempenhobeneficiario'], 'mascaraMoeda')
        ->turnOnPesquisator()
        ->esconderColunas('enbid')
        ->setCampos(['edeid', 'enbcnpj', 'enbnome', 'gndcod', 'mapcod', 'edevalor', 'edelimiteempenhobeneficiario'])
        ->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    ?>
</div>
<script type="text/javascript" src="/ted/js/jquery.livequery.js"></script>
<script type="text/javascript">
    function apagarBeneficiario(edeid)
    {
        bootbox.confirm('Tem certeza que deseja apagar o beneficiário?', function(confirmacao){
            if (confirmacao) {
                var dataRequisicao = {requisicao: 'apagarBeneficiario', edeid: edeid, edestatus: 'I'};
                $.post(window.location.href, dataRequisicao, function(){
                    window.location.href = window.location.href;
                });
            }
        });
    }

    function editarBeneficiario(edeid, enbcnpj, enbnome, edevalor)
    {
        $('#cadastrobeneficiario_edeid').val(edeid);
        $('#cadastrobeneficiario_edevalor').val(edevalor).blur();
        $('label[for="cadastrobeneficiario_enbnome"]').next().children().text(trim(enbnome));
        $('#cadastrobeneficiario_enbcnpj').val(enbcnpj);
        $("#modal-beneficiario-emenda").modal();
    }

    $(function(){
        $('.navbar-listagem .btn-adicionar').click(function(){
            $("#modal-beneficiario-emenda").modal();
        });

        $("#cadastrobeneficiario_enbcnpj").livequery("blur", function () {
            if ($(this).val().length == 18 && $(this).val()) {
                PesquisaCNPJ($(this).val());
            } else {
                bootbox.alert("digite o numero de CNPJ completo");
            }
        });

        $(".btn-fechar").on("click", function () {
            $("#cadastrobeneficiario_enbcnpj").val("");
            $(".form-control-static").html("");
            $("#cadastrobeneficiario_emdid").val("");
            $("#cadastrobeneficiario_emdvalor").val("");
        });
    });
    function PesquisaCNPJ(cnpj) {
        $.post(window.location.href, {requisicao: 'pesquisar_cnpj', cnpj: cnpj}, function (data) {
            $(".form-control-static").html(data);
        });
    }
</script>
<?php
$htmlForm = (new Simec_View_Form('cadastrobeneficiario'))
    ->setRequisicao('nova_entidade_beneficiada')
    ->addHidden('edeid', '')
    ->addHidden('emdid', '')
    ->addTexto('CNPJ', 'enbcnpj', 20, ['masc' => '##.###.###/####-##'])
    ->addTexto('Beneficiario', 'enbnome', 20, ['somentetexto' => true])
    ->addCombo('Detalhe do Recurso', 'emdid', (new Spoemendas_Model_EmendaDetalhe())->getCombo($_REQUEST['emeid']))
    ->addMoeda('Valor', 'edevalor')
    ->render(false);

bootstrapPopup('Detalhes da Emenda Parlamentar', 'modal-beneficiario-emenda', $htmlForm, ['fechar', 'confirmar'], ['tamanho' => 'lg']);

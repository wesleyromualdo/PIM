<div class="col-md-12">
    <?php
    /**
     * Sistema de Publicação.
     * werteralmeida
     */

    /*
     * Beneficiários
     */
    $sql = "SELECT DISTINCT
                edeid,
                enb.enbcnpj,
                enb.enbnome,
                emd.gndcod,
                emd.mapcod,
                edeid as indicadas,
                ede.edevalor
            FROM
                emenda.emendadetalhe emd
            JOIN
                emenda.emendadetalheentidade ede
            USING
                (emdid)
            JOIN
                emenda.entidadebeneficiada enb
            USING
                (enbid)
                WHERE emeid = {$dados['emeid']}
                     AND enb.enbcnpj <> ''
                     and emdstatus = 'A'
                      AND enbstatus = 'A'
                     AND edestatus = 'A' 
            ";
    $arrColunas = array('CNPJ', 'Beneficiário', 'GND', 'Modalidade', 'Iniciativas Indicadas', 'Valor RCL (R$)');
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);
    $listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj');
    $listagem->addCallbackDeCampo('edevalor', 'mascaraMoeda');
    $listagem->addCallbackDeCampo(array('enbnome'), 'alinhaParaEsquerda');
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    $permissaoEdicao = new Spoemendas_Controller_Permissaoedicaoemenda($_REQUEST['emeid']);
    $podeSalvar = $permissaoEdicao->geraPermissoesEdicao();
    if ($podeSalvar) {
        $listagem->addAcao('edit', 'formVincularIniciativasBeneficiario');
    } else {
        $listagem->esconderColunas('edeid');
    }
    $listagem->addCallbackDeCampo(array('indicadas'), 'detalharIniciativaBeneficiario');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('edevalor'));
    $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);

    /*
     * Formulário para cadastrar
     */
    $listaIniciativasBeneficiarios = <<<DML
SELECT DISTINCT iniid AS codigo,
                ini.ininome||' (GND: '||ini.gndcod||')' AS descricao
  FROM emenda.emendadetalhe emd
    JOIN emenda.iniciativaemendadetalhe ind USING(emdid)
    JOIN emenda.iniciativa ini USING(iniid)
  WHERE emeid = {$dados['emeid']}
    AND ind.iedstatus = 'A'
    and emdstatus = 'A'
    AND ini.inistatus = 'A'
  ORDER BY descricao
DML;

    $form = new Simec_View_Form('iniciativa-beneficiario');
    $htmlForm = $form->setRequisicao('salvar-iniciativa-beneficiario')
            ->addHidden('edeid', $dados['edeid'])
            ->addInputCombo('iniid', $listaIniciativasBeneficiarios, '', 'iniid', array('flabel' => 'Iniciativas', 'multiple' => 'true'))
            ->render(false);

    bootstrapPopup('Iniciativas para o Beneficiário', 'form-iniciativa-beneficiario', $htmlForm, ['fechar', 'confirmar']);
    ?>
</div>
<script>
    /*
     *  Chamada da modal para Editar as iniciativas.
     */
    function formVincularIniciativasBeneficiario(edeid) {
        iniciativas = '';
        $.ajax(
            window.location.href + '&requisicao=retornar-iniciativas-beneficiario-ajax&edeid=' + edeid
        , {dataType:'json'}).done(function (res) {
            iniciativas = res;
            $('#iniid').val(iniciativas);
            $('#iniid').trigger('chosen:updated');
            $('#iniciativa-beneficiario_edeid').val(edeid);
            $('#form-iniciativa-beneficiario').modal();
        });
    }
</script>
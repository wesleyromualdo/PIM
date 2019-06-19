<div class="col-md-12">
    <?php
    /**
     * Sistema de Publicação.
     * werteralmeida
     */
    $permissaoEdicao = new Spoemendas_Controller_Permissaoedicaoemenda($_REQUEST['emeid']);
    $podeSalvar = $permissaoEdicao->geraPermissoesEdicao();
    $arrColunas = array('CNPJ', 'Beneficiário','Prioridade', 'GND', 'Modalidade', 'Fonte', 'Impedimento', 'Detalhe do Programa de Trabalho / Impedimento', 'Impedido (R$)', 'Limite de Empenho do Beneficiário (R$)');
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);
    $listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj')
            ->addCallbackDeCampo(['enbnome', 'edidescricao'], 'alinhaParaEsquerda')
            ->addCallbackDeCampo(['edivalor', 'edelimiteempenhobeneficiario'], 'mascaraMoeda')
            ->addCallbackDeCampo('ediimpositivo', 'formatarImpeditivo')
            ->esconderColunas('ediid', 'eibid', 'edeid', 'ireid', 'eibdscobj', 'emiid', 'valor_entidade')
            ->setCabecalho($arrColunas)
            ->setQuery(
                    (new Spoemendas_Model_Emendaimpedimentobeneficiario())
                    ->pegaImpedimentos(['emeid' => $_REQUEST['emeid']], true)
                )
            ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('edivalor'));

    if ($podeSalvar || $permissaoEdicao->getPermissaoImpedimentoFnde()) {
        $listagem->addAcao('edit', ['func' => 'informarImpedimento', 'extra-params' => ['ediid', 'eibid', 'edeid']]);
    } else {
        $listagem->esconderColunas('emdid');
    }

    $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);

    $emenda = (new Spoemendas_Model_EmendaDetalhe())->findByEmeid($_REQUEST['emeid']);

    if (empty($emenda['emdimpositiva']) || $emenda['emdimpositiva'] != 7) {
        $whereListaImpedimentos = ' AND emirp is null ';
    }
    else {
        $whereListaImpedimentos = ' AND emirp = 7 ';
    }

    $listaImpedimentos = <<<SQL
SELECT
      emiid                                  codigo
    , emicodigosiop || ' - ' || emidescricao descricao
FROM
    emenda.emendaimpositivo
WHERE
    emistatus = 'A' {$whereListaImpedimentos}
ORDER BY
    emicodigosiop
SQL;

    /* Lembrar de pegar depois só do mesmo GND do selecionado */
    $listaObjetivos = "SELECT
			                ir.ireid AS codigo,
			                ir.ireid || ' - '|| ininome || ' ('||resdsc||') GND: '||gndcod AS descricao
			            FROM
    						emenda.iniciativa i
			            JOIN
    						emenda.iniciativaresponsavel ir ON ir.iniid = i.iniid
			            JOIN
    						emenda.responsavel r ON ir.resid = r.resid
			            WHERE
    						inistatus ='A' AND irestatus = 'A'
			            ORDER BY 2";

    $htmlForm = (new Simec_View_Form('impedimentos'))
        ->setRequisicao('salvar-impedimentos')
        ->addHidden('emdid', '')
        ->addHidden('ediid', '')
        ->addHidden('eibid', '')
        ->addHidden('edeid', '')
        ->addHidden('eibdscobj', '')
        ->addHidden('valor_entidade', '')
        ->addHidden('edivalor_aux', '')
        ->addChoices('Existe Impedimento', 'ediimpositivo', ['Não há' => 'NH', 'Parcial' => 'PA', 'Total' => 'TO'])
        ->addCombo('Tipo de Impedimento', 'emiid', $listaImpedimentos, ['classdiv' => 'nhpato pa to'])
        ->addCombo('Objeto Impedido', 'ireid', $listaObjetivos, ['classdiv' => 'nhpato pa to'])
        ->addMoeda("Valor do Impedimento (R$)", 'edivalor', ['classdiv' => 'nhpato pa to'])
        ->addTextarea('Justificativa / Programa de Trabalho', 'edidescricao', '500', ['classdiv' => 'nhpatojustificativa'])
        ->addTexto("Anexo - Descrição", 'anxdescricao', '100')
        ->addTexto("Beneficiário", 'beneficiario', null, [
        	'somentetexto' => true
        ])
        ->addFile("Arquivo", 'arquivo')
        ->render(false);
    bootstrapPopup('Programa de Trabalho / Impedimento  para o detalhe', 'form-impedimentos-modal', $htmlForm, ['fechar', 'confirmar'], array('tamanho' => 'lg'));

    $sql = "SELECT
            anx.arqid,
            CASE WHEN anx.anxdsc <> '' THEN anx.anxdsc ELSE ' - ' END AS anxdsc,
            arq.arqnome || '.' || arq.arqextensao as arquivo
            from emenda.anexo anx
            inner join public.arquivo arq on anx.arqid = arq.arqid
            inner join emenda.emendadetalheimpositivo ed on ed.ediid = anx.ediid
            where ed.emdid IN (SELECT emdid FROM emenda.emendadetalhe WHERE emeid = {$_REQUEST['emeid']})
            and ed.edistatus = 'A' ORDER BY 3";
    $arrColunas = array('Descrição', 'Arquivo');
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);

    $listagem->addAcao('download', 'baixarAnexo');
    if ($podeSalvar || $permissaoEdicao->getPermissaoImpedimentoFnde()) {
        $listagem->addAcao('delete', 'apagarAnexo');
    } else {
        $listagem->esconderColunas('arqid');
    }

    $listagem->setCabecalho($arrColunas)
            ->setQuery($sql)
            ->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    ?>
</div>
<script type="text/javascript">
    /**
     * Chamada da modal para Editar os impedimentos.
     */
    function informarImpedimento(emdid, ediid, eibid, edeid)
    {
        var dados = {
            'emd.emdid': emdid,
            'ede.edeid': edeid,
            'edi.ediid': ediid,
            'eib.eibid': eibid
        };

        $.get(window.location.href, {requisicao: 'consultar-impedimentos', dados: dados}, function (data) {
            if (data.ediimpositivo == 'NH') {
                $('.nhpato').css('display', 'none');
                $('.nhpatojustificativa > label').text('PTA:');
            }

            $('#impedimentos_eibdscobj').val(data.eibdscobj);
            $('#impedimentos_edivalor').val(data.edivalor).blur();
            $('#impedimentos_edivalor_aux').val(data.edivalor);
            $('#impedimentos_edidescricao').val(data.edidescricao);
            $('#impedimentos_valor_entidade').val(data.valor_entidade);
            $('#impedimentos_emiid').val(data.emiid).trigger('chosen:updated');
            $('#impedimentos_ireid').val(data.ireid).trigger('chosen:updated');
			$('#impedimentos_beneficiario').html(data.enbnome);

            $('#impedimentos_ediimpositivo').val(data.ediimpositivo);
            $('input[id^="impedimentos_ediimpositivo_"]').parent().removeClass('active');
            $('#impedimentos_ediimpositivo_' + data.ediimpositivo).prop('checked', true).parent().click();

            $('#impedimentos_emdid').val(emdid);
            $('#impedimentos_ediid').val(ediid);
            $('#impedimentos_eibid').val(eibid);
            $('#impedimentos_edeid').val(edeid);

			// Marca a primeira opção do campo Existe Impedimento caso 'data.ediimpositivo' nao exista.
			if(!data.ediimpositivo) {
				$('#impedimentos_ediimpositivo_NH').prop('checked', true).parent().click();
			}

            $('#form-impedimentos-modal').modal();
        }, 'json');
    }

    /**
     * Validação do formulário de impedimento.
     */
    function onImpedimentosSubmit() {
        var ediimpositivo = $('input[id^="impedimentos_ediimpositivo"]:checked').val();

        if (ediimpositivo == undefined) {
            bootbox.alert('<b style="color: red;"> Antes de prosseguir, você deve indicar se existe ou não um impedimento.</b>');
            return false;
        }

        var mensagens = new Array();
        switch (ediimpositivo) {
            case 'NH':
                if ($('#impedimentos_edidescricao').val() == '') {
                	bootbox.alert('<b style="color: red;">O campo PTA é obrigatório.</b>');
                    return false;
                }
                break;
            case 'PA':
            case 'TO':
                if ($('#impedimentos_emiid').val() == '') {
                	bootbox.alert('<b style="color: red;">O campo Tipo de Impedimento é obrigatório.</b>');
                    return false;
                }
                if ($('#impedimentos_ireid').val() == '') {
                	bootbox.alert('<b style="color: red;">O campo Objetivo Impedido é obrigatório.</b>');
                    return false;
                }
                if (($('#impedimentos_emiid').val() == '6' || $('#impedimentos_emiid').val() == '<?=TIPO_IMPEDIMENTO_6_DO_RP7;?>') && $('#impedimentos_edidescricao').val() == '') {
                    bootbox.alert('<b style="color: red;">O campo justificativa é obrigatório</b>');
                    return false;
                }
                if ($('#impedimentos_edivalor').val() == '') {
                	bootbox.alert('<b style="color: red;">O campo Valor do Impedimento (R$) é obrigatório.</b>');
                    return false;
                }
                break;
        }

        return true;
    }


    $(function () {
        $('#impedimentos_ireid').change(function () {
            $('#impedimentos_eibdscobj').val($('#impedimentos_ireid option:selected').text().split(' - ').pop());
        });

        $('#impedimentos_edivalor').keyup(function () {
            var valor = Number($('#impedimentos_edivalor').val().split('.').join('').replace(',', '.')),
                maximo = Number($('#impedimentos_valor_entidade').val());

            if (valor > maximo) {
                $('#impedimentos_edivalor').val(0).blur();
                alert('Valor do Impedimento inserido é maior do que o valor total da entidade beneficiada.');
            } else if (valor == maximo) {
                $('#impedimentos_edivalor').val(0).blur();
                alert('Valor do Impedimento inserido é igual ao valor total da entidade beneficiada.');
            }

        });

        $('#impedimentos_emiid').change(function () {
            if ($('#impedimentos_emiid').val() == '6' || $('#impedimentos_emiid').val() == '<?=TIPO_IMPEDIMENTO_6_DO_RP7;?>') {
                $('.nhpatojustificativa').show();
            } else {
                $('.nhpatojustificativa').hide();
            }
        });

        $('input[id^="impedimentos_ediimpositivo"]').change(function () {
            $('.nhpato').hide();
            var valor = $(this).val();
            switch (valor) {
                case 'NH':
                    $('.nhpatojustificativa > label').text('PTA:');
                    $('.nhpatojustificativa').show();
                break;
                case 'TO':
                case 'PA':
                    $('.nhpatojustificativa > label').text('Justificativa:');
                    $('.nhpato.pa').show();

                    if ($('#impedimentos_emiid').val() == '6' || $('#impedimentos_emiid').val() == '<?=TIPO_IMPEDIMENTO_6_DO_RP7;?>') {
                    	$('.nhpatojustificativa').show();
                    }
                    else {
                    	$('.nhpatojustificativa').hide();
                    }
                    if (valor == 'TO') {
                        $('#impedimentos_edivalor').prop('readonly',true).val($('#impedimentos_valor_entidade').val()).blur();
                    }
                    else {
                        $('#impedimentos_edivalor').prop('readonly',false).val($('#impedimentos_edivalor_aux').val()).blur();
                    }
                break;
            }
        });
    });

    /**
     * apagarAnexo
     */
    function apagarAnexo(arqid) {
        if(confirm('Deseja apagar o arquivo?')){
            location.href= location.href+'&requisicao=apagar-anexo&arqidDel='+arqid;
        }
    }
    /**
     * baixar anexo
     */
    function baixarAnexo(arqid) {
       window.open(location.href+'&requisicao=baixar-arquivo&arqid='+arqid);
    }

</script>
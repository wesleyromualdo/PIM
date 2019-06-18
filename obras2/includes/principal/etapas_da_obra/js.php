<script type="text/javascript">

    jQuery(document).ready(function() {
        jQuery('#aguarde').hide();
    });

    function gerarXls(){
         var formulario = document.formulario;
            document.getElementById('xls').value = 'gerar';
            formulario.submit();
    }

    function inserirEtapas() {
        return windowOpen('obras2.php?modulo=principal/inserir_etapas&acao=A', 'blank', 'height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function gerenteEdificacao() {

        var imgDesabUp = $(document.createElement('img')).attr({src: '/imagens/seta_cimad.gif',
            border: 0,
            title: 'Subir'});

        var imgUp = $(document.createElement('img')).attr({src: '/imagens/seta_cima.gif',
            border: 0,
            title: 'Subir'})
                .css({cursor: 'pointer'});

        var imgDesabDown = $(document.createElement('img')).attr({src: '/imagens/seta_baixod.gif',
            border: 0,
            title: 'Descer'});

        var imgDown = $(document.createElement('img')).attr({src: '/imagens/seta_baixo.gif',
            border: 0,
            title: 'Descer'})
                .css({cursor: 'pointer'});

        this.addLine = function(dataLine) {
            dataLine = dataLine || '';

            if (dataLine.itcid == '') {
                alert('Falha!\nFaltam dados da etapa.');
                return false;
            }

            var idTR = 'tr_etapa_' + dataLine.itcid;

            var clone = $('#tr_matrix').clone()
                    .attr('id', idTR)
                    .show();

            clone.html(replaceAll(clone.html(), 'id="icodtinicioitem[]"', 'id="icodtinicioitem_' + dataLine.itcid + '"'));
            clone.html(replaceAll(clone.html(), "document.getElementById('icodtinicioitem[]')", "document.getElementById('icodtinicioitem_" + dataLine.itcid + "')"));

            clone.html(replaceAll(clone.html(), 'id="icodterminoitem[]"', 'id="icodterminoitem_' + dataLine.itcid + '"'));
            clone.html(replaceAll(clone.html(), "document.getElementById('icodterminoitem[]')", "document.getElementById('icodterminoitem_" + dataLine.itcid + "')"));

            prepareRemoveImg(clone, idTR, dataLine);

            $('#tr_matrix').before(clone);

            if (dataLine != '') {

<?php
if ($habilitado):
    ?>
                    if (dataLine.icoid) {
                        var img = $(document.createElement('img')).attr({src: '/imagens/ico_config_i.gif',
                            border: 0,
                            title: 'Ir para detalhamento da etapa'})
                                .css({cursor: 'pointer'})
                                .click(function() {
                                    location.href = '?modulo=principal/detalhe_da_etapa&acao=A&icoid=' + dataLine.icoid;
                                });
                        clone.find('td')
                                .eq(1)
                                .find('img')
                                .before(img);
                        clone.find('td')
                                .eq(1)
                                .find('img')
                                .eq(1)
                                .before('&nbsp;')
                                .before('&nbsp;');
                    }
    <?php
endif;
?>
                loadData(dataLine, clone);
            }
<?php
if ($habilitado):
    ?>
                prepareArrowImg();
    <?php
endif;
?>
            colorManager();
        }


        this.removeLine = function(id) {
            $('#' + id).remove();
            colorManager();
            prepareArrowImg();

            calculoEdificacao.manager();
        }

        this.moveUp = function(tr) {
            tr = $(tr);
            tr.insertBefore(tr.prev());
            colorManager();
            prepareArrowImg();
        }

        this.moveDown = function(tr) {
            tr = $(tr);
            tr.insertAfter(tr.next());
            colorManager();
            prepareArrowImg();
        }

        function loadData(dataLine, line) {
            var hiddenID = $(document.createElement('input')).attr({name: 'icoid[]',
                type: 'hidden'})
                    .val((dataLine.icoid ? dataLine.icoid : ''));
            line.find('[id*=itcdesc]').html(dataLine.itcdesc)
                    .after(hiddenID);

            line.find('[name*=itcid]').val(dataLine.itcid);
            line.find('[name*=icodtinicioitem]').val(dataLine.icodtinicioitem);
            line.find('[name*=icodterminoitem]').val(dataLine.icodterminoitem);
            line.find('[name*=icovlritem]').val(dataLine.icovlritem);

            calculoEdificacao.manager(line.find('[name*=icovlritem]')[0]);

        }

        function prepareArrowImg() {
            var indMaxTr = ($('#tabela_etapas').find('[id*=tr_etapa_]').length - 1);
            $('#tabela_etapas').find('[id*=tr_etapa_]').each(function(i, obj) {

                var cloneImgUp = (i == 0
                        ? $(imgDesabUp).clone()
                        : $(imgUp).clone()
                        .bind('click', function() {
                            gerenteEdificacao.moveUp(obj);
                        }));
                var cloneImgDown = (i == indMaxTr
                        ? $(imgDesabDown).clone()
                        : $(imgDown).clone()
                        .bind('click', function() {
                            gerenteEdificacao.moveDown(obj);
                        }));

                $(obj).find('td:eq(0)').html('')
                        .append(cloneImgUp)
                        .append('&nbsp;')
                        .append(cloneImgDown);
            });
        }

        function prepareRemoveImg(clone, idTR, dataLine) {
<?php
if ($habilitado == false):
    ?>
                $(clone).find('td:eq(1)').html('');
    <?php
else:
    ?>
                dataLine.qtddetalhamento = dataLine.qtddetalhamento || 0;
                if (typeof dataLine == 'string' || dataLine.qtddetalhamento == 0) {
                    $(clone).find('td:eq(1) span img').bind('click', function() {
                        if (confirm("Deseja remover esse registro?")) {
                            gerenteEdificacao.removeLine(idTR);
                        }
                    });
                } else {
                    $(clone).find('td:eq(1) span img')
                            .attr('src', '/imagens/excluir_01.gif')
                            .attr('title', 'A etapa nÃ£o pode ser excluÃ­da, pois possui detalhamento(s).')
                            .bind('click', function() {
                                alert('A etapa nÃ£o pode ser excluÃ­da, pois possui detalhamento(s).');
                            });
                }
<?php
endif;
?>
        }

        function colorManager() {
            var countLine = 0;
            var colorLine = new Array('#FFFFFF', '#E0E0E0');
            $('#tabela_etapas').find('[id*=tr_etapa_]').each(function(i, obj) {
                $(obj).attr('bgcolor', colorLine[countLine]);
                countLine = (countLine == 0 ? 1 : 0);
            });
        }
    }

    var gerenteEdificacao = new gerenteEdificacao();

    function calculoEdificacao() {
        var obCalc = new Calculo();
        var percentTotal = new Number(0);
        var itemEdicao = '';
        var vlrItemAnterior = '';

        this.manager = function(obj) {
            obj = obj || '';

            if (obj != '') {
                formatarNumeroMonetario(obj);
            }

            calculaTotal();
            calculaRestante();
        }

        this.setDefaultValue = function(obj) {
            if (obj != itemEdicao) {
                itemEdicao = obj;
                vlrItemAnterior = obj.value;
            }
        }

        function calculaPercent(obj, ind) {
            var vlrContrato = obCalc.converteMonetario($('#vl_contrato').val());
            var vlrItem = obCalc.converteMonetario(obj.value);
            var percentItem = mascaraglobal("[###.]###,##", ((vlrItem / vlrContrato) * 100).toFixed(2));
            percentItem = formatarNumeroMonetario({value: percentItem});
            $('[name*=icopercsobreestrutura]').eq(ind)
                    .val(percentItem);

        }

        function calculaRestante() {
            var vlrRest = obCalc.operacao($('#vl_contrato').val(), $('#totalv').val(), '-');
            $('#rest_totalv').val(mascaraglobal("[###.]###,##", vlrRest));
        }

        function calculaTotal() {
            var vlrContrato = obCalc.converteMonetario($('#vl_contrato').val());
            var vlrTotal = new String('0');
            $('[name*=icovlritem]').not(':last')
                    .each(function(i, objItem) {
                        var vlrItem = $(objItem).val();
                        vlrTotal = obCalc.operacao(vlrTotal, vlrItem, '+');
                        vlrTotal = mascaraglobal("[###.]###,##", vlrTotal);

                        calculaPercent(objItem, i);
                    });
            $('#totalv').val(vlrTotal);

            percentTotal = obCalc.operacao(vlrTotal, vlrContrato, '/', 20);
            percentTotal = obCalc.operacao(percentTotal, 100, '*', 20);
            percentTotal = new Number(percentTotal).toFixed(2);
            $('#total').val(formatarNumeroMonetario({value: percentTotal}));

            var percentRest = obCalc.operacao('100', percentTotal, '-');
            percentRest = formatarNumeroMonetario({value: percentRest});
            $('#rest_total').val(percentRest);

            if (obCalc.comparar(vlrTotal, $('#vl_contrato').val(), '>')) {


                var pulaAlert;
                if (typeof itemEdicao != 'object') {
                    pulaAlert = true;
                    itemEdicao = new Object();
                    itemEdicao.value = 0;
                } else {
                    pulaAlert = false;
                }
                vlrItemAnterior = vlrItemAnterior || 0;
                recalculaTotal = (itemEdicao.value != vlrItemAnterior ? true : false);

                if (recalculaTotal || pulaAlert) {
                    alert('A soma das etapas excede o valor do contrato.');
                    itemEdicao.value = vlrItemAnterior;
                }

                if (recalculaTotal) {
                    calculaTotal();
                }
            }

        }

        function formatarNumeroMonetario(obj) {
            var valor = new String(obCalc.retiraZeroEsquerda(obj.value));

            if (valor.length == 2) {
                valor = '0' + valor;
            } else if (valor.length == 1) {
                valor = '00' + valor;
            } else if (valor.length == 0) {
                valor = '000';
            }

            obj.value = mascaraglobal("[###.]###,##", valor);
            return obj.value;
        }

    }

    calculoEdificacao = new calculoEdificacao();

    function verificaDataInicio(data) {
        var obData = new Data();


        if (obData.comparaData(data.value, '<?=$dtLimiteInicioEtapa?>', '<')) {
            alert('A data de inÃ­cio nÃ£o pode ser menor que a data de assinatura da ordem de serviÃ§o.');
            data.value = '';
            data.focus();
            return;
        }

        if (obData.comparaData(data.value, '<?=$dtLimiteFimEtapa?>', '>')) {
            alert('A data de inÃ­cio nÃ£o pode ser maior que a data de tÃ©rmino do projeto.');
            data.value = '';
            data.focus();
            return;
        }

        // verificando se a data de inicio nÃ£o Ã© maior que a data de termino

        var nome = ($(data).attr('id')).split('_');
        if (nome[1]) {
            if (obData.comparaData(data.value, $('#icodterminoitem_' + nome[1]).val(), '>')) {
                alert('A data de inÃ­cio nÃ£o pode ser maior que a data de tÃ©rmino.');
                data.value = '';
                data.focus();
                return;
            }
        }

    }

    function verificaDataTermino(data) {
        var obData = new Data();

        if (obData.comparaData(data.value, '<?=$dtLimiteFimEtapa?>', '>')) {
            alert('A data de tÃ©rmino nÃ£o pode ser maior que a data de tÃ©rmino do contrato.');
            data.value = '';
            data.focus();
            return;
        }

        // verificando se a data de termino nÃ£o Ã© menor que a data de inicio
        var nome = ($(data).attr('id')).split('_');
        if (nome[1]) {
            if (obData.comparaData(data.value, $('#icodtinicioitem_' + nome[1]).val(), '<')) {
                alert('A data de tÃ©rmino nÃ£o pode ser menor que a data de inÃ­cio.');
                data.value = '';
                data.focus();
                return;
            }
        }
    }

</script>




<script type="text/javascript">
    function managerServicoContratado(valor) {
        if (valor == 'N') {
            $('#tr_servico_contratado_justificativa').show();
        } else {
            $('#tr_servico_contratado_justificativa').hide();
            $('#obrcronogramaservicocontratadojustificativa').val('');
        }
    }



    function validaForm() {
        var obCalc = new Calculo();
        var result = true;
        var msg = '';

        var arDtInicio = new Array();
        $('[name*=icodtinicioitem]').not(':last')
                .each(function(i, obj) {
                    arDtInicio.push(obj.value);
                });
        if (jQuery.inArray('', arDtInicio) >= 0) {
            msg += '* O campo Data de InÃ­cio Ã© de preenchimento obrigatÃ³rio.\n';
            result = false;
        }

        var arDtTermino = new Array();
        $('[name*=icodterminoitem]').not(':last')
                .each(function(i, obj) {
                    arDtTermino.push(obj.value);
                });
        if (jQuery.inArray('', arDtTermino) >= 0) {
            msg += '* O campo Data de TÃ©rmino Ã© de preenchimento obrigatÃ³rio.\n';
            result = false;
        }

        $('[name*=icodtinicioitem]').not(':last').each(function(i, obj) {
            var id = new String($(obj).attr('id')).split('_');
            id = id[1];

            var dtInicio = new String($(obj).val()).split('/');
            dtInicio = new Date(dtInicio[2], new Number(dtInicio[1]) - 1, dtInicio[0]);

            var dtTermino = new String($(obj).parent().parent().find('#icodterminoitem_' + id).val()).split('/');
            dtTermino = new Date(dtTermino[2], new Number(dtTermino[1]) - 1, dtTermino[0]);

            if (dtInicio > dtTermino) {
                msg += '* Nenhuma Data de InÃ­cio pode ser maior que a Data de TÃ©rmino.\n';
                result = false;
                return false;
            }
        });

        if ($('[name=obrcronogramaservicocontratado]:checked').val() == 'N') {
            if ($('#obrcronogramaservicocontratadojustificativa').val() == '') {
                msg += 'O campo justificativa Ã© obrigatÃ³rio.\n';
            }
        }

        if (msg != '') {
            alert('ValidaÃ§Ã£o do formulÃ¡rio:\n\n' + msg);
        } else {
            $('#evt').val('salvar');
            document.formulario.submit();
        }
    }
</script>




<?php if($_GET['acao'] == 'E') { ?>

<script type="text/javascript">

    jQuery(document).ready(function() {

        setTimeout(function() {

        var tabela = $('#tabela_etapas');

        var str = '';

        $('[name^=icovlritem]').each(function(){
            $(this).attr('disabled', 'disabled');
        });
        $('[name^=icopercsobreestrutura]').each(function(){
            $(this).attr('disabled', 'disabled');
        });

        $('#tabela_etapas').find('[id*=tr_etapa_]').each(function(i, obj) {
            $(obj).find('td:eq(0)').html('');
            $(obj).find('td:eq(1)').html('');
        });
        var c1 = 0;
//        //Remove 2 primeiras colunas do cabeÃ§alho
        $('#tabela_etapas > thead > tr > td').each(function(){
            if(c1 < 2){
                $(this).remove();
            }
            c1++;
        });

        $('#tabela_etapas').find('[id*=tr_etapa_]').each(function(i, obj) {
            $(obj).find('td:eq(0)').remove('');
            $(obj).find('td:eq(0)').remove('');
        });

        c1 = 0;
        $('#tr_total > td').each(function(){
            if(c1 < 2){
                $(this).remove();
            }
            c1++;
        });
        c1 = 0;
        $('#tr_vlcontrato > td').each(function(){
            if(c1 < 2){
                $(this).remove();
            }
            c1++;
        });

        $('#tr_vlrestante > td').eq(0).attr('colspan', '2');

    });

    }, 500);

</script>

<?php  } ?>





<script type="text/javascript">

    $(function(){
        $('#editar_cronograma').click(function(e){
            e.preventDefault();
            if(confirm("Ã necessÃ¡rio inserir uma vistoria para concluir a ediÃ§Ã£o do cronograma.\nDeseja realmente editar o cronograma?")){
                window.location.href = "/obras2/obras2.php?modulo=principal/editar_cronograma&acao=A&obrid=<?=$obrid?>";
            }
        })
    })

</script>




<?php
if ($fichamonitoramentoEstado->fmecomcoordenadora == null && $fichamonitoramento->fmtcomcoordenadora == null) { ?>
    <script>
        $('#aba_monitoramentos').click(function () {
            $('#modalInfoMonitoramento').modal('show');
        });
    </script>

<?php } ?>

<style>
    #listErrorUploadMonitoramento {
        overflow-y: auto;
    }

    #listErrorUploadMonitoramento li {
        /*background-color: #f2dede;!important;*/
        font-size: 12px;
    }

</style>
<?php
$muncod = ($municipio->muncod == null) ? '' : ",'$municipio->muncod'";

if ($fichamonitoramentoEstado->estuf != null) {
    $aux = "'$fichamonitoramentoEstado->estuf' , '$_GET[fmeid]' ";
    $auxiliar = "estuf: '$fichamonitoramentoEstado->estuf',";
    $auxArquivo = "arquivos/Arquivo de importação para o monitoramento de metas_Estadual.xlsx";
} else {
    $aux = "'$fichamonitoramento->estuf' , '$_GET[fmtid]', '$municipio->muncod' ";
    $auxiliar = "estuf: '$fichamonitoramento->estuf', muncod: '$municipio->muncod',";
    $auxArquivo = "arquivos/Arquivo de importação para o monitoramento de metas_Municipal.xlsx";
}

?>
<div class='container'>
    <div class='text-justify text-center'>
        <h3 style='margin-top: 2%; margin-bottom: 05%'>Monitoramento de Metas</h3>
    </div>
    <div class="row">
        <div class="col-md-8">
            <!--Escondendo botões-->
            <a class="btn" style="padding-left: 0px;">
                <input type="file" id="arquivoMonitoramento" accept=".xlsx,.xls">
            </a>
            <button href="#" class="btn btn-primary" id="addMonitoramento" style="display: none;">
                Salvar Importação <span class="glyphicon glyphicon-send"></span>
            </button>
        </div>
        <div class="col-md-8">
            <span class="label label-primary">
                <a style="color: #ffffff" href="<?= $auxArquivo ?>"> Arquivo modelo para importação </a></span>
        </div>
    </div>
    <div id="tabela_monitoramento"></div>
</div>

<!-- Modal Info-->
<div class="modal fade in" id="modalInfoMonitoramento" tabindex="-1" role="dialog" aria-labelledby="myModalLabel"
     aria-hidden="false" data-backdrop="static">
    <div class="modal-dialog">
        <div class="modal-content text-center">
            <div class="modal-header">
                <h4 class="modal-title text-center" id="modalInfoTitle">Aviso!</h4>
            </div>
            <div class="modal-body">
                <h5 id="modalInfoCorpo">Preencha os dados na aba "Informações Gerais"</h5>
            </div>
            <div class="modal-footer">
                <button class="btn btn-danger" data-dismiss="modal" id="modalInfoOk" onclick="location.reload()">Ok
                </button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div>

<!-- Modal SubMetas e Estratégias -->
<div class="modal  fade" id="modalMonitoramento" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <input type="hidden" id="fbsid" value="">
    <div class="modal-dialog " role="document">
        <div class="modal-content">
            <div class="modal-header">
                Editar Monitoramento
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body">
                <div class="container">
                    <div class="panel-group ">
                        <div class="panel panel-default">
                            <div class="panel-heading" style='background-color:#0b559b; color: white;'>
                                <div name="nucompletosubmeta" id="nucompletosubmeta"></div>
                            </div>
                            <!--panel-body-->
                            <div class="panel-body">
                                <form id="form">
                                    <table id='table_monnn'
                                           class='table table-bordered table-hover table-condensed table-striped '>
                                        <thead>
                                        <tr>
                                            <th class='text-center '>Ano</th>
                                            <th class='text-center ' style="padding: 10px 30px;">Meta Prevista</th>
                                            <th class='text-center '>Meta Executada no Período - Dado Oficial</th>
                                            <th class='text-center '>Meta Executada no Período - <?php echo ((isset($_GET['fmtid'])) ? 'Dado Municipal' : "Dado Estadual") ?></th>
                                        </tr>
                                        </thead>
                                        <tbody>

                                        <?php for ($ano = 2014; $ano <= 2030; $ano++) : ?>
                                            <tr>
                                                <td class='text-center ano' name="fcmano"
                                                    id="fcmano_<?= $ano ?>"><?= $ano ?></td>

                                                <td id="sortable">
                                                    <input class="form-control somenteNumero" data-ano="<?= $ano ?>"
                                                        data-tipo="fcmmetaprevista" name="fcmmetaprevista" maxlength="11"
                                                        size="4" placeholder="" >
                                                </td>
                                                <td>
                                                    <input class="form-control somenteNumero" data-ano="<?= $ano ?>"
                                                           data-tipo="fcmmetaexecutadaoficial"
                                                           name="fcmmetaexecutadaoficial" maxlength="11" placeholder="">
                                                </td>
                                                <td>
                                                    <input class="form-control somenteNumero" data-ano="<?= $ano ?>"
                                                           data-tipo="fcmmetaexecutadaalternativa" maxlength="11" name="fcmmetaexecutadaalternativa" placeholder="">
                                                </td>
                                            </tr>
                                        <?php endfor; ?>
                                        </tbody>
                                    </table>
                                </form>
                            </div><!--fim panel-body-->
                        </div>
                    </div>
                </div>
            </div>
            <div class="modal-footer">
                <button type="" class="btn btn-danger btn-default btn-sm" data-dismiss="modal">
                    Cancelar <span class="glyphicon glyphicon-remove"></span>
                </button>
                <button id="salvarModalMonitoramento" class="btn btn-primary btn-default btn-sm" data-dismiss="modal">
                    Salvar <span class="glyphicon glyphicon-floppy-saved"></span>
                </button>
            </div>
        </div>
    </div>
</div>

<!-- Modal Errors Upload -->
<div class="modal modal-wide fade" id="modalErrorUploadMonitoramento"
     tabindex="-1" role="dialog" aria-labelledby="myModalLabel" data-backdrop="static">

    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h3>Erro! Dados não Salvos!</h3>
            </div>
            <div class="modal-body">
                <p class="lead">Os seguintes erros foram encontrados na planilha enviada:</p>
                <ul class="list-group" id="listErrorUploadMonitoramento">
                </ul>
            </div>
            <div class="modal-footer">
                <button type="" class="btn btn-danger btn-default btn-sm" data-dismiss="modal">
                    Cancelar <span class="glyphicon glyphicon-remove"></span>
                </button>

            </div>
        </div>
    </div>
</div>

<!-- Modal Erro -->
<div class="modal modal-wide fade" id="modalError"
     tabindex="-1" role="dialog" aria-labelledby="myModalLabel" data-backdrop="static">

    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h3>Erro! Dados não Salvos!</h3>
            </div>
            <div class="modal-body">
                <input type="hidden" id="auxHidden" value="">
                <div id="modalErrorMSG"></div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-danger" id="btnCancelarErro" data-dismiss="modal">Ok</button>
            </div>
        </div>
    </div>
</div>

<!--<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-maskmoney/3.0.2/jquery.maskMoney.min.js"></script>-->
<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-filestyle/1.2.3/bootstrap-filestyle.min.js"></script>
<script>
    var hasValue = false;

    $(document).ready(function () {

        /**
         * Limpar modal Editar Monitoramento ao fechar
         */
        $('#modalMonitoramento').on('hidden.bs.modal', function (e) {
            if ($('#auxHidden').val() != 'true'){
                $("form#form :input").each(function () {
                    $(this).val('');
                });
            }
        });

        /**
         * Limpar Modal Error ao fechar
         */
        $('#modalMaiorCem').on('hidden.bs.modal', function (e) {
            $('#auxHidden').val('');
        });

        $('#aba_monitoramentos').click(function () {
            construirTabelaMonitoramento(<?= $aux; ?> );
        });


        construirTabelaMonitoramento(<?= $aux; ?> );

        $('#arquivoMonitoramento').filestyle({
            buttonText: 'Importação de Monitoramento de Metas',
            buttonName: 'btn-primary',
            input: false
        });

        //Aparece o botão salvar importação caso selecione algum arquivo
        $('#arquivoMonitoramento').change(function () {
            $('#addMonitoramento').show();
        });

        $('#addMonitoramento').click(function () {
            var arquivo = $("#arquivoMonitoramento").val();

            if (arquivo != '') {
                var valido = validoFormatoArquivo(arquivo);
                if (valido == 0) {
                    alert("A extensão do arquivo é inválida.");
                    return false;
                }
            } else {
                alert("A extensão do arquivo é inválida.");
                return false;
            }

            var file_data = $('#arquivoMonitoramento').prop('files')[0];
            var form_data = new FormData();
            form_data.append('userfile', file_data);
            form_data.append('estuf', '<?= $appendEstuf ?>');
            form_data.append('muncod', '<?= $appendMuncod ?>');
            form_data.append('usucpf', '<?= $_SESSION['usucpf'] ?>');
            form_data.append('fmaid', '<?= $appendFmaid ?>');
            form_data.append('uploadFichaC', true);

            jQuery.ajax({
                url: '',
                type: 'POST',
                dataType: 'text',
                cache: false,
                contentType: false,
                processData: false,
                data: form_data,

                success: function (dados) {

                    var obj = JSON.parse(dados);

                    switch (obj['resultado']) {
                        case 'erro':
                            alert(obj['msg']);
                            break;
                        case 'erro_dado':
                            popularModalErrosUpload($("#listErrorUploadMonitoramento"), obj['msg']);
                            $('#modalErrorUploadMonitoramento').modal('show');
                            break;
                        case 'success':
                            construirTabelaMonitoramento(<?= $aux; ?> );
                            alert('Dados salvos com sucesso!');
                            break;
                    }
                    return;
                }
            });
        });

        function popularModalErrosUpload(select, data) {
            select.html('');
            var cor = '#f9f9f9';
            var items = [];

            $.each(data, function (id, option) {
                cor = id % 2 === 0 ? '#f9f9f9' : '#fff'
                items.push('<li class="list-group-item" style="background-color:' + cor + '" >' + option + '</li>');
            });
            select.append(items.join(''));
        }

        function validoFormatoArquivo(arquivo) {
            var extensoes, ext, valido;
            extensoes = new Array('.xlsx','.xls');
            ext = arquivo.substring(arquivo.lastIndexOf(".")).toLowerCase();
            valido = false;
            for (var i = 0; i <= arquivo.length; i++) {
                if (extensoes[i] == ext) {
                    valido = true;
                    break;
                }
            }
            if (valido) {
                return 1;
            }
            return 0;
        }
    });

    /**
     * Construir a tabela Monitoramento de Indicadores
     */
    function construirTabelaMonitoramento(estuf, fmaid, muncod = null) {
        $.ajax({
            url: '',
            type: 'POST',
            data: {
                construirTabelaMonitoramento: true,
                fmaid: fmaid,
                estuf: estuf,
                muncod: muncod
            },
            success: function (resposta) {
                $('#tabela_monitoramento').html(resposta);
                $('[data-toggle="popover"]').popover();
                $('#tabMonitoramento').DataTable({
                    "paging": false,
                    "info": false,
                    "bSort": false,
                    "aoColumnDefs": [
                        {
                            'bSortable': false,
                            'aTargets': [-2, -1, 1, 0]
                        },
                    ],
                    "language": {
                        "url": "https://cdn.datatables.net/plug-ins/1.10.12/i18n/Portuguese-Brasil.json"
                    }
                });
            }
        });
    }

    function somenteNumeros(num) {
        var er = /[^0-9.]/;
        er.lastIndex = 0;
        var campo = num;
        if (er.test(campo.value)) {
            campo.value = "";
        }
    }

    /**
     * Preencher o Modal de Monitoramento
     *
     */
    function preencherModalMonitoramento(nucompletosubmeta, fbsid) {
        $('#nucompletosubmeta').html(nucompletosubmeta);
        $('#fbsid').val(fbsid);

        var fbsid = $('#fbsid').val();
        var nucompletosubmeta = $('#nucompletosubmeta').val();

        //verifica se há valores salvo no banco
        $.ajax({
            url: '',
            type: 'POST',
            data: {
                filtroAjaxVerificaMonitoramento: true,
                fbsid: fbsid
            },
            success: function (resposta) {

                var obj = $.parseJSON(resposta);
                $("form#form :input").each(function () {

                    var ano = parseInt($(this).data('ano'));
                    var tipo = $(this).data('tipo');

                    if (obj[ano] != undefined) {

                        $(this).val(obj[ano][tipo]);

                        hasValue = true;
                    }
                });
            }
        });

        //mostra modal
        $('#modalMonitoramento').modal('show');
    }

    /**
     * Salva os dados do Modal Editar Monitoramento
     */
    $('#salvarModalMonitoramento').click(function () {
//        return;

        var isEmpty = true;
        var anoAtual = 2014;
        var dadosObj = {};
        var myObject = {};
        var fbsid = $('#fbsid').val();
        var testDecimal = false;
        var testTamanhoVal = false;

        $("form#form :input").each(function () {

            if ($(this).val() != "") {
                isEmpty = false;
            }

            var ano = parseInt($(this).data('ano'));
            var tipo = $(this).data('tipo');

            if (anoAtual !== ano) {

                dadosObj[anoAtual] = myObject;
                anoAtual++;
                myObject = {};
            }
            //var tipo = $(this).data('tipo');
//            $(this).val(parseFloat($(this).val()).toFixed(2));

            myObject[tipo] = $(this).val();
            var virgula = myObject[tipo].indexOf(",");
            var ponto = myObject[tipo].indexOf(".");

            if (virgula != -1){
                var numero = myObject[tipo].split(",");

                if (numero[1].length > 2){
                    testDecimal = true;
                }
            }

            if (ponto != -1){
                var numero = myObject[tipo].split(".");

                if (numero[1].length > 2){
                    testDecimal = true;
                }
            }

            if ($(this).val() > 2000000){
                testTamanhoVal = true;
            }
        });

        if (testDecimal == true){
            $('#auxHidden').val('true');
            $('#modalErrorMSG').html('O campo não poder ter mais que duas casas decimais após a vírgula.');
            $('#modalError').modal('show');
            return;
        }

        if (testTamanhoVal == true){
            alert('O Campo não poder ser maior que 2000000.');
            return false;
        }

        if (anoAtual !== parseInt($(this).data('ano'))) {
            dadosObj[anoAtual] = myObject;
            anoAtual++;
            myObject = {};
        }

        if (isEmpty === true && hasValue === false) {
            alert('Insira ao menos um valor para uma das Meta ');
            return false;
        }

        jsonInput = JSON.parse(JSON.stringify(dadosObj));

        $.ajax({
            url: "/sase/sase.php?modulo=principal/monitoramentoEstado&acao=A",
            type: 'POST',
            data: {
                salvarAjaxMonitoramento: true,
                dados: jsonInput,
                fbsid: fbsid,
                <?= $auxiliar ?>
            },
            success: function (resposta) {
                if (resposta == 'true') {
                    alert('Dados salvos com sucesso!');
                    return;
                }
            }
        });
    });

    $('#btnCancelarErro').click(function () {
        $('#modalMonitoramento').modal('show');
    });
</script>
<script type="text/javascript">
    $(document).ready(function(){
        $(".somenteNumero").bind("keyup blur focus", function(e) {
            e.preventDefault();
            var expre = /[^\d.,]/g;
            $(this).val($(this).val().replace(expre,''));
        });
    });
</script>
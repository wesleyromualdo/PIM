<!--<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>-->
<!--<link rel="stylesheet" href="js/bootstrap-datetimepicker/bootstrap-datetimepicker.min.css">-->
<link rel="stylesheet"
      href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-datepicker/1.6.4/css/bootstrap-datepicker3.min.css"/>
<link rel="stylesheet" href="https://cdn.datatables.net/1.10.15/css/dataTables.bootstrap.min.css">
<div class="container" style="margin-top: 02%">
    <form class="form-horizontal" name="formAVMonitoramento" id="formAVMonitoramento" role="form" method="POST"
          enctype="multipart/form-data">
        <br><br>
        <div class="form-group">
            <label class="col-sm-4 control-label " for="estauf">Estado: </label>
            <div class="col-md-4">
                <select name="estuf" id="estuf" class="form-control chosen"
                        onchange="desabilitarAnosRelatorio(this.value,'ano'); limpaForm();">
                    <option value="0">Selecione</option>
                    <?php

                    $cpf = $_SESSION['usucpf'];
                    $perfis = arrayPerfil();
                    if (in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $perfis) || in_array(PFLCOD_SASE_EXECUTIVO, $perfis)) {
                        $sql = "SELECT est.estuf, estdescricao
                                FROM territorios.estado est
                                    INNER JOIN sase.usuarioresponsabilidade resp
                                ON resp.estuf = est.estuf
                                WHERE
                                  resp.pflcod IN (" . PFLCOD_SASE_SUPERVISOR_GERAL . "," . PFLCOD_SASE_EXECUTIVO . ")
                                AND rpustatus = 'A'
                                AND usucpf = '{$cpf}'
                                ORDER BY estdescricao";
                    } else {
                        if (in_array(PFLCOD_SASE_ADMINISTRADOR, $perfis) || in_array(PFLCOD_SASE_SUPER_USUARIO,
                                $perfis)) {
                            $sql = "SELECT DISTINCT estuf, estdescricao 
                                FROM territorios.estado ter 
                                ORDER BY estdescricao";
                        }
                    }

                    $estados = $db->carregar($sql);
                    foreach ($estados as $chave => $estado) { ?>
                        <option <?= ($estado['estuf'] == $_POST['estuf']) ? 'selected' : '' ?>
                                value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>

        <div class="form-group">
            <label for="" class="col-sm-4 control-label">Ano Inicial:</label>
            <div class='col-sm-2 input-group date'>
                <input type='text' class="form-control data" name="araanoinicial" id="araanoinicial"/>
                <span class="input-group-addon">
                    <span class="glyphicon glyphicon-calendar"></span>
                </span>
            </div>
            <label for="" class="col-sm-1 control-label ">Ano Final:</label>
            <div class='col-sm-2 input-group date'>
                <input type='text' class="form-control data" name="araanofinal" id="araanofinal"/>
                <span class="input-group-addon">
                    <span class="glyphicon glyphicon-calendar"></span>
                </span>
            </div>
        </div>

        <div class="form-group">
            <label class="col-sm-4 control-label">Relatório de Avaliação dos Planos de Educação:</label>
            <div class="col-md-5">
                <input type="file" class="upload" id="arquivoUp" data-placeholder="">
            </div>
        </div>
        <!-- Button -->
        <div class="form-group">
            <label class="col-sm-4 "></label>
            <div class="col-md-4">
                <button id="salvarRelatorio" name="btnAddMunicipio" type="button"
                        class="btn btn-primary btn-block">
                    Salvar
                </button>
            </div>
        </div> <!--Fim Button -->
    </form>
    <div class="container" id="divAvalicaoCadastrados" style="margin-top: 2%"></div>
</div>

<!--Modal Editar Relatório de Avaliação dos Planos de Educação -->
<form class="form-horizontal" name="formAvaliacaoModal" id="formAvaliacaoModal" role="form" method="POST"
      enctype="multipart/form-data">
    <div class="modal fade" id="modalRelatorio" tabindex="-1" role="dialog" aria-labelledby="myModalLabel"
         data-backdrop="static">
        <input type='hidden' class="form-control data" name="modalArqid" id="modalArqid" value="">
        <input type='hidden' class="form-control data" name="araid" id="modalAraid" value="">
        <input type='hidden' class="form-control data" name="estuf" id="modalEstuf">
        <div class="modal-dialog modal-lg" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span>
                    </button>
                    <h4 class="modal-title text-center" id="myModalLabel">Editar Relatório de Avaliação dos Planos de
                        Educação</h4>
                </div>
                <div class="modal-body">
                    <form class="form-horizontal">
                        <div class="container">
                            <div class="form-group">
                                <label for="gender1" class="col-sm-4 control-label" id="modalUF"></label>
                            </div>
                        </div>
                        <br>

                        <div class="container">
                            <div class="form-group">
                                <label for="gender1" class="col-sm-2 control-label">Data Inicial: </label>
                                <div class="col-sm-4 input-group date">
                                    <input type='text' class="form-control data" name="araanoinicial"
                                           id="araanoinicialModal"/>
                                    <span class="input-group-addon">
                                    <span class="glyphicon glyphicon-calendar"></span>
                                </span>
                                </div>

                                <label for="gender1" class="col-sm-2 control-label">Data Fim: </label>
                                <div class="col-sm-4 input-group date">
                                    <!--                                <input type="hidden" name="estuf" id="estufModal">-->
                                    <input type='text' class="form-control data" name="araanofinal"
                                           id="araanofinalModal"/>
                                    <span class="input-group-addon">
                                    <span class="glyphicon glyphicon-calendar"></span>
                                </span>
                                </div>
                            </div>
                        </div>
                        <br>

                        <div class="form-group">
                            <label class="col-sm-4 control-label">Relatório de Avaliação dos Planos de Educação:</label>
                            <div class="col-md-8">
                                <input type='file' class="form-control upload" id="arqnomeModal"/>
                            </div>
                        </div>
                    </form>
                </div>

                <div class="modal-footer">
                    <button type="reset" class="btn btn-danger btn-default" data-dismiss="modal">
                        Cancelar <span class="glyphicon glyphicon-remove"></span>
                    </button>
                    <a href="#" id="salvarModalAvaliacao" class="btn btn-primary disabled">
                        Salvar <span class="glyphicon glyphicon-floppy-disk"></span>
                    </a>
                </div>
            </div>
        </div>
    </div>
</form><!-- Fim Modal Relatório de Avaliação -->

<!-- Modal Delete -->
<div class="modal fade in" id="modalExcluir" tabindex="-1" role="dialog" aria-labelledby="myModalLabel"
     aria-hidden="false">
    <input type="hidden" id="modalExcluirId" value="">
    <input type="hidden" id="modalExcluirArqid" value="">
    <div class="modal-dialog">
        <div class="modal-content text-center">
            <div class="modal-header">
                <h4 class="modal-title text-center">Deletar Dados</h4>
            </div>
            <div class="modal-body">
                <div id="modalExcluirMsg"></div>
            </div>
            <div class="modal-footer">
                <button class="btn btn-default" data-dismiss="modal" id="modalExcluirCancel">Cancelar</button>
                <button class="btn btn-danger" id="btn-excluirAvaliacao">Deletar</button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- Fim Modal Delete -->


<!-- Componentes datatable CSS -->
<script type="text/javascript" src="../library/bootstrap-datatable/js/jquery.dataTables.js"></script>
<script type="text/javascript" src="../library/bootstrap-datatable/js/dataTables.bootstrap.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-filestyle/1.2.3/bootstrap-filestyle.min.js"></script>
<!-- Componentes datetimepicker JS -->
<script type="text/javascript" src="js/bootstrap-datetimepicker/moment.min.js"></script>
<script type="text/javascript" src="js/bootstrap-datetimepicker/moment.pt-br.js"></script>
<!--<script type="text/javascript" src="js/bootstrap-datetimepicker/bootstrap-datetimepicker.min.js"></script>-->
<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-datepicker/1.6.4/js/bootstrap-datepicker.min.js"></script>
<!--<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-filestyle/1.2.3/bootstrap-filestyle.min.js"></script>-->

<script>
    construirTabelaRelAvaliacao();

    $('[data-toggle="tooltip"]').tooltip();

    $('.data').datepicker({
        startDate: "2014",
        endDate: "2030",
        format: "yyyy",
        startView: 2,
        minViewMode: 2,
        maxViewMode: 2,
        autoclose: true,
        language: "pt-BR",
    });

    $(".upload").filestyle({
        buttonText: "Upload"
    });

    function desabilitarAnosRelatorio(estuf, campo) {
        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {
                desabilitarAnosRelatorio: true,
                estuf: estuf
            },
            success: function (resposta) {
                console.log(resposta);
                if (resposta != 'false') {
                    var objResposta = JSON.parse(resposta);
                    $('.data').datepicker('destroy');

                    $('.data').datepicker({
                        startDate: "2014",
                        endDate: "2030",
                        format: "yyyy",
                        startView: 2,
                        minViewMode: 2,
                        maxViewMode: 2,
                        autoclose: true,
                        language: "pt-BR",
                        beforeShowYear: function (date) {
                            console.log(typeof (objResposta));

                            if (objResposta.indexOf(date.getFullYear()) != -1) {
                                return false;
                            }
                        }
                    });

                    $('#salvarModalAvaliacao').addClass('disabled');

                    if (campo == 'modalAvaliacaoEstuf') {
                        $("#araanoinicialModal").change(function () {
                            $('#salvarModalAvaliacao').removeClass('disabled');
                        });

                        $("#araanofinalModal").change(function () {
                            $('#salvarModalAvaliacao').removeClass('disabled');
                        });

                    }
                }
            }
        });
    }

    function editarModalAvalicacao(araid, arqid, estuf, estdescricao, araanoinicial, araanofinal, arqnome) {
        $('#modalUF').html('Estado: ' + estdescricao);
        $('#modalAraid').val(araid);
        $('#estdescricaoModal').val(estdescricao);
        $('#araanoinicialModal').val(araanoinicial);
        $('#araanofinalModal').val(araanofinal);
        $('#modalArqid').val(arqid);
        $('#modalEstuf').val(estuf);

        $(".upload").filestyle({
            buttonText: "Upload"
        });

        desabilitarAnosRelatorio(estuf, 'modalAvaliacaoEstuf');

        //Recupera o arquivo de Upload no modal
        $("#arqnomeModal").filestyle('placeholder', arqnome);

        limpaForm();
        $('#modalRelatorio').modal('show');
    }

    $('#salvarRelatorio').click(function () {
        var araanoinicial = $('#araanoinicial').val();
        var araanofinal = $('#araanofinal').val();
        var estuf = $('#estuf').val();

        if (estuf == 0) {
            alert("Campo Estado obrigatório");
            return false;
        }

        if (araanoinicial > araanofinal) {
            alert("Data Inicial não pode ser maior que a Data Final");
            return false;
        }

        if (araanoinicial == "") {
            alert("Campo Ano Inicial obrigatório");
            return false;
        }

        if (araanofinal == "") {
            alert("Campo Ano Final obrigatório");
            return false;
        }

        var arquivoUp = $("#arquivoUp").val();
        var file_data = false;

        var form = $('#formAVMonitoramento')[0];

        var data = new FormData(form);

        /**
         * Verifica a Extensão do arquivo .pdf
         */

        if (arquivoUp != '') {
            var valido = validoFormatoArquivo(arquivoUp);
            if (valido == 0) {
                alert("Selecionar arquivo do tipo PDF");
                return false;
            }
            file_data = $('#arquivoUp').prop('files')[0];
        } else {
            alert("Campo Relatório de Avaliação dos Planos de Educação obrigatório");
            return false;
        }

        // If you want to add an extra field for the FormData
        data.append("acao", "salvar_arquivo");
        data.append("nomeTabela", "arquivorelatorioavaliacao");
        data.append("descricao", "SASE - Relatório de Avaliação dos Planos de Educação");
        data.append('userfile', file_data);

        $.ajax({
            type: "POST",
            enctype: 'multipart/form-data',
            url: "",
            data: data,
            processData: false,
            contentType: false,
            cache: false,
            timeout: 600000,
            success: function (resposta) {
                switch (resposta) {
                    case 'true':
                        alert('Salvo com sucesso!');
//                        limpaForm();
                        construirTabelaRelAvaliacao();
                        break;
                    case 'false':
                        alert('Arquivo muito extenso!');
//                        limpaForm();
                        construirTabelaRelAvaliacao();
                        break;
                }
            }
        });
    });

    function validoFormatoArquivo(arquivoUp) {
        var extensoes, ext, valido;
        extensoes = new Array('.pdf');
        ext = arquivoUp.substring(arquivoUp.lastIndexOf(".")).toLowerCase();
        valido = false;
        for (var i = 0; i <= arquivoUp.length; i++) {
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

    function excluirArquivo(araid, arqid, estdescricao) {
        $('#modalExcluirMsg').html('Deseja excluir o relatório do Estado ' + estdescricao + ' ?');
        $('#modalExcluirId').val(araid);
        $('#modalExcluirArqid').val(arqid);

        $('#modalExcluir').modal('show');
    }

    //Deleta Avaliação
    $('#btn-excluirAvaliacao').click(function () {
        var araid = $('#modalExcluirId').val();
        var arqid = $('#modalExcluirArqid').val();

        $.ajax({
            url: '',
            type: 'POST',
            data: {
                acao: 'excluirArquivo',
                nomeTabela: 'arquivorelatorioavaliacao',
                araid: araid,
                arqid: arqid
            },
            success: function (resposta) {
                switch (resposta) {
                    case 'true':
                        $('#modalExcluir').modal('hide');
                        alert('Deletado com sucesso!');
//                        limpaForm();
                        construirTabelaRelAvaliacao();
                        break;
                }
            }
        })
    });

    /**
     * Construir a Tabela de Relatórios Avaliação ao carregar a pagina ou incluir/editar relatórios
     */
    function construirTabelaRelAvaliacao() {
        var estuf = $('#estuf').val();
        $.ajax({
            url: '',
            type: 'POST',
            data: {
                construirTabelaRelAvaliacao: true,
                estuf: estuf
            },
            success: function (resposta) {
                $('#divAvalicaoCadastrados').html(resposta);
                $('.rodape').hide();
                $('[data-toggle="popover"]').popover();

                $('#tabAvaRelatorioEstado').DataTable({
                    "order": [],
                    "language": {
                        "url": "https://cdn.datatables.net/plug-ins/1.10.12/i18n/Portuguese-Brasil.json"
                    }
                });
            }
        });
    }

    function downloadAnexoAvaliacao(arqid, tabela) {
        var url = window.location.href;

        if (url.charAt(url.length - 1) == '#') {
            url = url.substring(0, url.length - 1) + '&nomeTabela=' + tabela + '&acao=downloadAnexo&arqid=' + arqid;
        } else {
            url = window.location.href + '&nomeTabela=' + tabela + '&acao=downloadAnexo&arqid=' + arqid;
        }
        window.location = url;
        return;
    }

    $('#salvarModalAvaliacao').click(function () {
        var araanoinicialModal = $('#araanoinicialModal').val();
        var araanofinalModal = $('#araanofinalModal').val();
        var estuf = $('#estuf').val();
        var arqid = $('#araid').val();

        if (araanoinicialModal > araanofinalModal) {
            alert("Data Inicial não pode ser maior que a Data Final");
            return false;
        }

        if (araanoinicialModal == "") {
            alert("Campo Ano Inicial obrigatório");
            return false;
        }

        if (araanofinalModal == "") {
            alert("Campo Ano Final obrigatório");
            return false;
        }

        /**
         * Verifica a Extensão do arquivo .pdf
         */
        var arqnomeModal = $("#arqnomeModal").val();
        var file_data = false;
        var form = $('#formAvaliacaoModal')[0];

        // Create an FormData object
        var data = new FormData(form);

        if (arqnomeModal != '') {
            var valido = validoFormatoArquivo(arqnomeModal);
            if (valido == 0) {
                alert("Selecionar arquivo do tipo PDF");
                return false;
            }
            file_data = $('#arqnomeModal').prop('files')[0];
        }

        // If you want to add an extra field for the FormData
        data.append("acao", "editar_arquivo");
        data.append("nomeTabela", "arquivorelatorioavaliacao");
        data.append("descricao", "SASE - Relatório de Avaliação dos Planos de Educação");
        data.append('userfile', file_data);

        $.ajax({
            type: "POST",
            enctype: 'multipart/form-data',
            url: "",
            data: data,
            processData: false,
            contentType: false,
            cache: false,
            timeout: 600000,
            success: function (resposta) {
                console.log(resposta);
                switch (resposta) {
                    case 'true':
                        $('#modalRelatorio').modal('hide');
                        alert('Salvo com sucesso!');
                        construirTabelaRelAvaliacao();
                        break;
                    case 'false':
                        alert('Arquivo muito extenso!');
                        construirTabelaRelAvaliacao();
                        break;
                }
            }
        });
    });

    $('#arqnomeModal').change(function () {
        $('#salvarModalAvaliacao').removeClass('disabled');
    });

    function limpaForm() {
        $('#araanoinicial').datepicker('clearDates');
        $('#araanofinal').datepicker('clearDates');
        $(":file").filestyle('clear');
    }
</script>
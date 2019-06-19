<?php ob_start(); ?>

<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-datepicker/1.6.4/css/bootstrap-datepicker3.min.css" />

<div class="container" style="margin-top: 02%">
    <form class="form-horizontal" name="formMonitoramento" id="formMonitoramento" role="form" method="POST" enctype="multipart/form-data">
        <input type="hidden" name="acao" id="acao" value=""/>
        <br><br>
        <div class="form-group">

            <label class="col-sm-4 control-label " for="estauf">Estado: </label>
            <div class="col-sm-3" >
                        <select name="estuf" id="estuf" class="form-control chosen" onchange="filtraMunicipios();desabilitarAnos(this.value,'ano');limpaForm();">
                        <?php
                        $cpf = $_SESSION['usucpf'];

                        if (in_array( PFLCOD_SASE_SUPERVISOR_GERAL ,$perfis) || in_array( PFLCOD_SASE_EXECUTIVO ,$perfis)){
                            $sql = "SELECT DISTINCT est.estuf, est.estdescricao  
                                                    FROM territorios.estado est
                                                      INNER JOIN sase.usuarioresponsabilidade resp ON
                                                          resp.estuf = est.estuf
                                                      WHERE
                                                      resp.pflcod IN (".PFLCOD_SASE_SUPERVISOR_GERAL.",".PFLCOD_SASE_EXECUTIVO.")
                                                    AND rpustatus = 'A'
                                                    AND usucpf = '{$cpf}'
                                                    ORDER BY est.estdescricao";

                        }else if (in_array( PFLCOD_SASE_TECNICO ,$perfis)){
                            $sql = "SELECT DISTINCT est.estuf, est.estdescricao  
                                                    FROM territorios.estado est
                                                    INNER JOIN sase.usuarioresponsabilidade resp ON
                                                      substr(resp.muncod,0,3)  = est.estcod
                                                    WHERE
                                                      resp.pflcod = ".PFLCOD_SASE_TECNICO."
                                                      AND rpustatus = 'A'
                                                      AND usucpf = '{$cpf}' 
                                                      ORDER BY est.estdescricao";

                        }else if (in_array( PFLCOD_SASE_ADMINISTRADOR ,$perfis) || in_array( PFLCOD_SASE_SUPER_USUARIO ,$perfis)){
                            $sql = "SELECT estuf, estdescricao FROM territorios.estado ORDER BY estdescricao ASC";
                            ?> <option value="">Selecione</option> <?php
                        }

                        $estados = $db->carregar($sql);
                        foreach ($estados as $chave => $estado) { ?>
                            <option <?= ($estado['estuf'] == $_POST['estuf']) ? 'selected' : '' ?>
                                    value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                        <?php } ?>
                    </select>

            </div>

        </div>

        <!-- Select Basic -->
        <div class="form-group">
            <label class="col-sm-4 control-label" for="muncod">Município: </label>
            <div class="col-sm-3" id="comboMuncipio">
                <?php
                if (in_array( PFLCOD_SASE_SUPERVISOR_GERAL ,$perfis) || in_array( PFLCOD_SASE_EXECUTIVO ,$perfis)){
                    comboMunicipio($lista,null);
                }else if (in_array( PFLCOD_SASE_TECNICO ,$perfis)){
                    comboMunicipio(null,$lista);
                }else if (in_array( PFLCOD_SASE_ADMINISTRADOR ,$perfis) || in_array( PFLCOD_SASE_SUPER_USUARIO ,$perfis)){
                    ?>
                        <select name="muncod" id="muncod" class="form-control" >
                            <option value="">Selecione</option>
                        </select>
                    <?php
                } ?>
            </div>
        </div>

        <div class="form-group">
            <label for="ano" class="col-sm-4 control-label">Ano:</label>
            <div class='col-sm-3 input-group date'>
                <input onkeydown="return false" type='text' class="form-control data" name="ano" id="ano"  />
                <span class="input-group-addon">
                    <span class="glyphicon glyphicon-calendar"></span>
                </span>
            </div>
        </div>

        <div class="form-group">
            <label class="col-md-4 col-sm-4 control-label">Relatório de Monitoramento dos Planos de Educação:</label>
            <div class="col-md-5 col-sm-6">
                <input type="file" id="arquivo" accept=".pdf">
            </div>
        </div>

        <!-- Button -->
        <div class="form-group">
            <label class="col-sm-4 "></label>
            <div class="col-md-4">
                <input type="submit" name="btnSalvar" id="btnSalvar" value="Salvar" class="btn btn-primary btn-block"/>
            </div>
        </div>
    </form>
    <div  id="divTabRelMonitoramento" style="margin-top: 2%"></div>
</div>

<!--Modal Editar Relatório de Monitoramento dos Planos de Educação -->
<div class="modal fade" id="modalRelatorio" tabindex="-1" role="dialog" aria-labelledby="myModalLabel"
     data-backdrop="static">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span>
                </button>
                <h4 class="modal-title text-center" id="myModalLabel">Editar Relatório de Monitoramento dos Planos de
                    Educação</h4>
            </div>
            <div class="modal-body">
                <form class="form-horizontal" name="formMonitoramentoEditar" id="formMonitoramentoEditar" role="form" method="POST" enctype="multipart/form-data">
                    <div class="container">
                        <div class="form-group">
                            <label class="col-md-4 col-sm-4 control-label" >Município: </label>
                            <label class="col-md-3 col-sm-3 control-label" id="modalUF"></label>
                        </div>
                    </div>
                    <br>

                    <div class="container">
                        <div class="form-group">
                            <label for="modalArmano" class="col-sm-4 control-label">Ano: </label>
                            <div class="col-sm-4 input-group date">
                                <input type='text' class="form-control data" name="modalArmano" id="modalArmano" >
                                <input type='hidden' class="form-control data" name="modalArmid" id="modalArmid" >
                                <input type='hidden' class="form-control data" name="modalArqid" id="modalArqid" >
                                <input type='hidden' class="form-control data" name="estuf" id="modalEstuf" >

<!--                                <input type='hidden' class="form-control data" name="muncod" id="modalMuncod" >-->
                                <span class="input-group-addon">
                                    <span class="glyphicon glyphicon-calendar"></span>
                                </span>
                            </div>
                        </div>
                    </div>
                    <br>

                    <div class="form-group">
                        <label class="col-sm-4 control-label">Relatório de Monitoramento dos Planos de Educação:</label>
                        <div class="col-md-8 col-sm-8">
                            <input type="file" id="modalArquivo" class="form-control upload" accept=".pdf">
                        </div>
                    </div>
                </form>
            </div>

            <div class="modal-footer">
                <button type="reset" class="btn btn-danger btn-default" data-dismiss="modal">
                    Cancelar <span class="glyphicon glyphicon-remove"></span>
                </button>
                <a href="#" class="btn btn-primary disabled"  id="modalBtnEditar" >
                    Salvar <span class="glyphicon glyphicon-floppy-disk"></span>
                </a>
            </div>
        </div>
    </div>
</div><!-- Fim Modal Relatório de Monitoramento -->

<!-- Modal Delete -->
<div class="modal fade in" id="modalExcluir" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="false" >
    <input type="hidden" id="modalExcluirArqid" value="">
    <input type="hidden" id="modalExcluirtabela" value="">
    <input type="hidden" id="modalExcluirEstDescricao" value="">
    <div class="modal-dialog">
        <div class="modal-content text-center">
            <div class="modal-header">
                <h4 class="modal-title text-center">Deletar Dados</h4>
            </div>
            <div class="modal-body">
                <div id="modalExcluirMsg"><h5></h5></div>
            </div>

            <div class="modal-footer">
                <!--                    <button type="button" class="btn btn-default" data-dismiss="modal">Cancel</button>-->
                <button class="btn btn-default" data-dismiss="modal" id="modalExcluirCancel">Cancelar</button>
                <button class="btn btn-danger" id="btnExcluirMonitoramento">Deletar</button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- Fim Modal Delete -->

<!-- Componentes datatable CSS -->
<script type="text/javascript" src="../library/bootstrap-datatable/js/jquery.dataTables.js"></script>
<script type="text/javascript" src="../library/bootstrap-datatable/js/dataTables.bootstrap.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-filestyle/1.2.3/bootstrap-filestyle.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-datepicker/1.6.4/js/bootstrap-datepicker.min.js"></script>

<script>
    construirTabelaRelMonitoramento();

    /**
     * Seta a variável tamanho com o tamanho do arquivo
     */

    var tamanho = 0;

    $('#arquivo').bind('change', function() {
        tamanho = this.files[0].size;
    });

    $("#arquivo").filestyle({
        buttonText: "Escolher arquivo"
    });

    $('#modalArquivo').change( function () {
        $('#modalBtnEditar').removeClass('disabled');
    } );

    /**
     * Salvar
     */
    $("#btnSalvar").click(function (event) {

        event.preventDefault();

        var arquivo = $("#arquivo").val();

        if (arquivo != '') {

            var valido = validoFormatoArquivo(arquivo);
            if (valido == 0) {
                alert("A extensão do arquivo é inválida.");
                return false;
            }
            if (tamanho > 8388608){
                alert("O tamanho do arquivo excede o limite permitido.")
                return false;
            }
        } else {
            alert("O Arquivo PDF de Relatório de Monitoramento dos Planos de Educação é obrigatório!");
            return false;
        }

        if ($('#ano').val() == ''){
            alert('O Campo Ano é obrigatório.');
            return false;
        }

        if ($('#estuf').val() == '') {
            alert ('O campo Estado é obrigatório');
            return false;
        }

        if ($('#estuf').val() == '') {
            alert ('O campo Estado é obrigatório');
            return false;
        }

        var file_data = $('#arquivo').prop('files')[0];

        // Get form
        var form = $('#formMonitoramento')[0];

        // Create an FormData object
        var data = new FormData(form);

        // If you want to add an extra field for the FormData
        data.append("acao", "salvar_arquivo");
        data.append("nomeTabela", "arquivorelatoriomonitoramento");
        data.append("descricao", "SASE - Relatório de Monitoramento dos Planos de Educação");
        data.append('userfile', file_data);

        // disabled the submit button
        $("#btnSubmit").prop("disabled", true);

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
                        construirTabelaRelMonitoramento();
                        limpaForm();
                        $("#estuf").val("");
                        $("#muncod").val("");
                        break;
                }

            }
        });

    });

    /**
     * Editar
     */
    $("#modalBtnEditar").click(function (event) {

        //stop submit the form, we will post it manually.
        event.preventDefault();

        var arquivo = $("#modalArquivo").val();
        var file_data = false;

        // Get form
        var form = $('#formMonitoramentoEditar')[0];

        // Create an FormData object
        var data = new FormData(form);

        if (arquivo != '') {
            var valido = validoFormatoArquivo(arquivo);
            if (valido == 0) {
                alert("A extensão do arquivo é inválida.");
                return false;
            }
            file_data = $('#modalArquivo').prop('files')[0];
        }

        // If you want to add an extra field for the FormData
        data.append("acao", "editar_arquivo");
        data.append("nomeTabela", "arquivorelatoriomonitoramento");
        data.append("descricao", "SASE - Relatório de Monitoramento dos Planos de Educação");
        data.append('userfile', file_data);

        // disabled the submit button
        $("#modalBtnEditar").prop("disabled", true);

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
                        $('#modalRelatorio').modal('hide');
                        alert('Salvo com sucesso!');
                        limpaForm();
                        construirTabelaRelMonitoramento();
                        break;
                }

            }
        });

    });

    /**
     * Excluir
     */
    $('#btnExcluirMonitoramento').click(function () {

        var arqid = $('#modalExcluirArqid').val();
        var tabela = $('#modalExcluirtabela').val();
        var estdescricao = $('#modalExcluirEstDescricao').val();

        $.ajax({
            type: "POST",
            url: "",
            data: {
                acao: 'excluirArquivo',
                nomeTabela: 'arquivorelatoriomonitoramento',
                arqid: arqid
            },

            success: function (resposta) {
                switch (resposta) {
                    case 'true':
                        $('#modalExcluir').modal('hide');
                        alert('Deletado com sucesso!');
                        limpaForm();
                        $("#estuf").val("");
                        $("#muncod").val("");
                        construirTabelaRelMonitoramento();
                        break;
                }

            }
        });

    });

    /**
     * Validar o Formato do arquivo. Somente pdf
     */
    function validoFormatoArquivo(arquivo) {
        var extensoes, ext, valido;
        extensoes = new Array('.pdf');
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

    /**
     * Construir a Tabela de Relatórios ao carregar a pagina ou incluir/editar relatórios
     */
    function construirTabelaRelMonitoramento (){

        $.ajax({
            url: '',
            type: 'POST',
            data: {
                construirTableRelMonMunicipal: true,
                lista: "<?php echo $lista ?>"
            },
            success: function (resposta) {

                $('#divTabRelMonitoramento').html(resposta);
                $('.rodape').hide();
                $('[data-toggle="popover"]').popover();
                $('#tabRelMonitoramento').DataTable({
                    "order": [],
                    "language": {
                        "url": "https://cdn.datatables.net/plug-ins/1.10.12/i18n/Portuguese-Brasil.json"
                    }
                });
            }
        });
    }

    /**
     * Desabilitar anos já salvos por Muncod
     * @param muncod
     * @param campo
     */
    function desabilitarAnos(muncod,campo) {
        $.ajax({
            url: '',
            type: 'POST',
            data: {
                desabilitarAnosMonitoramento: true,
                muncod:muncod
            },
            success: function (resposta) {

                var objResposta = JSON.parse(resposta);
                var arrAnos = [];

                Object.keys(objResposta).forEach(function(key) {
                    arrAnos.push(objResposta[key].armano);
                });
                console.log( arrAnos );

                $('#'+campo).datepicker('destroy');

                $('#'+campo).datepicker({
                    startDate: "2014",
                    endDate: "2030",
                    format: "yyyy",
                    startView: 2,
                    minViewMode: 2,
                    maxViewMode: 2,
                    autoclose: true,
                    language: "pt-BR",
                    beforeShowYear: function (date){
                        if(arrAnos.indexOf(date.getFullYear().toString()) != -1){
                            return false;
                        }
                    }
                });

                $('#modalBtnEditar').addClass('disabled');

                if (campo == 'modalArmano'){
                    $( "#modalArmano" ).change(function() {
                        $('#modalBtnEditar').removeClass('disabled');
                    });
                }
            }

        });

    }

    function downloadAnexo (arqid,tabela){
        var url = window.location.href;

        if (url.charAt(url.length-1) == '#'){
            url = url.substring(0, url.length-1) + '&nomeTabela='+ tabela +'&acao=downloadAnexo&arqid=' + arqid;
        }else{
            url = window.location.href+'&nomeTabela='+ tabela +'&acao=downloadAnexo&arqid=' + arqid;
        }
        window.location=url;
        return;
    }

    function modalDelete (arqid,tabela, mundescricao){

        $('#modalExcluirArqid').val(arqid);
        $('#modalExcluirtabela').val(tabela);

        $('#modalExcluirMsg').html('Deseja excluir o relatório do município ' + mundescricao + ' ?');

        $('#modalExcluir').modal('show');

    };

    function modalEditar(arqid, armano,estdescricao, arqnome,armid,estuf,muncod,mundescricao) {

        $(".upload").filestyle({
            buttonText: "Escolher arquivo"
        });
        desabilitarAnos(muncod,'modalArmano')

        $('#modalUF').html(estuf + '- ' + mundescricao);
        $('#modalArmano').val(armano);
        $('#modalArmid').val(armid);
        $('#modalArqid').val(arqid);
        $('#modalEstuf').val(estuf);
//        $('#modalMuncod').val(muncod);

        //Recupera o arquivo de Upload no modal
        $("#modalArquivo").filestyle('placeholder', arqnome);
        $('#modalRelatorio').modal('show');
    }

    function limpaForm(){
        $('#ano').datepicker('clearDates');
        $(":file").filestyle('clear');
    }

    function filtraMunicipios() {
        var estuf = jQuery('#estuf').val();

        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {combomunicipio: true, estuf: estuf},
            success: function (resposta) {
                jQuery('#comboMuncipio' ).html(resposta);

            }
        });
    }

</script>
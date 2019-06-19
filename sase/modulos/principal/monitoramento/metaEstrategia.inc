<link rel="stylesheet" href="https://cdn.datatables.net/1.10.15/css/dataTables.bootstrap.min.css">
<link rel="stylesheet" href="js/bootstrap-datetimepicker/bootstrap-datetimepicker.min.css">

<?php


if ($fichamonitoramentoEstado->fmecomcoordenadora == null && $fichamonitoramento->fmtcomcoordenadora == null) { ?>
    <script>
        $('#meta_estratedi').click(function () {
            $('#modalInfo').modal('show');
            $('#btnHistoricoMeta').hide();
        });
    </script>

<?php } 


if( $_REQUEST['acao_download'] == 'downloadAnexo'){
            
    include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    $arqid      = $_REQUEST['arqid'];
    if (!empty($fichamonitoramentoEstado->estuf)) {
        $tipo = "listaEstado";
        $dir  = "monitoramentoEstado";
    }
    if (!empty($fichamonitoramentoEstado->muncod)) {
        $tipo = "lista";
        $dir  = "monitoramento";
    }
//    ver($tipo, $dir);die;
    $campos     = array();

    if ($arqid != null) {
        $file = new FilesSimec('arqimportacaofichab_historico',$campos, 'sase');
        $caminho = APPRAIZ."arquivos/sase".'/'.floor($arqid/1000).'/'.$arqid;
        
        if(!$file->Download($caminho)){
             echo "<script>
                var tipo = '{$tipo}';
                var dir = '{$dir}';
                    alert('Arquivo não encontrado');
                    setTimeout(function(){ 
                        window.location.href = 'sase.php?modulo=principal/'+dir+'&acao=A&aba='+tipo;
                   }, 40);
                 </script>";
        };
        $file->getDownloadArquivo($arqid);
        exit();
    }
    die;
}
?>

<style>
    ::-webkit-input-placeholder { /* WebKit, Blink, Edge */
        font-size: 10px;
    }

    :-moz-placeholder { /* Mozilla Firefox 4 to 18 */
        font-size: 10px;
    }

    ::-moz-placeholder { /* Mozilla Firefox 19+ */
        font-size: 10px;
    }

    :-ms-input-placeholder { /* Internet Explorer 10-11 */
        font-size: 10px;
    }

    .modal.modal-wide .modal-dialog {
        width: 90%;
    }

    .modal-wide .modal-body {
        /*overflow-y: auto;*/
    }

    #listErrorUpload {
        overflow-y: auto;
    }

    #listErrorUpload li {
        font-size: 12px;
    }

    .modal-history {
        position: fixed;
        transform: translate(0, 0);
        width: auto;
        left: 0;
        right: 0;
        height: auto;
        top: 0;
        bottom: 0;
        z-index: 990; /* display above everything else */
        padding: 20px;
        overflow-y: scroll;
    }
</style>

<div class="container">
    <div class="text-justify text-center">
        <h3 style="margin-top: 2%">Metas e Estratégias</h3>
    </div>
    <div class="row" style="margin-left: 02%;margin-top: 05%">
        <div class="col-md-8">
            <a href="#" class="btn btn-primary" data-toggle="modal" id="btnCadastrarMeta">
                <span class="glyphicon glyphicon-plus"></span> Cadastrar Meta
            </a>
            <!--Escondendo botões-->
            <a class="btn">
                <input type="file" id="arquivo" accept=".xlsx,.xls">
            </a>

            <button href="#" class="btn btn-primary" id="addDocumento" style="display: none;">
                Salvar Importação <span class="glyphicon glyphicon-send"></span>
            </button>

        </div>
        <div class="col-md-8">
            <span class="label label-primary">
                <a style="color: #ffffff"
                   href="arquivos/Arquivo%20de%20importação%20para%20metas%20e%20estratégias_SASE.xlsx"> Arquivo modelo para importação </a></span>

        </div>
    </div>


    <div id="tabela_meta"></div>
    <div class="col-md-8">
        <a href="#" class="btn btn-primary" data-toggle="modal" id="btnHistoricoMeta">
            <span class="glyphicon glyphicon-plus"></span> Histórico
        </a>
        <a href="#" class="btn btn-primary" data-toggle="modal" id="btnHistoricoImportacao">
            <span class="glyphicon glyphicon-plus"></span> Histórico Importação
        </a>
    </div>

    <!-- Modal Metas -->
    <div class="modal fade" id="modalMeta" tabindex="-1" role="dialog" aria-labelledby="myModalLabel"
         data-backdrop="static">
        <input type="hidden" id="tipoSalvar" value="">
        <input type="hidden" id="fbmid" value="">
        <div class="modal-dialog modal-lg" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h4 class="modal-title text-center" id="myModalLabel">Cadastrar Meta</h4>
                </div>
                <div class="modal-body">
                    <form class="form-horizontal">
                        <div class="container">
                            <div class="form-group">
                                <label for="gender1" class="col-sm-2 control-label">Nº da Meta:</label>
                                <div class="col-sm-4">
                                    <input type="text" onkeyup="this.value = mascaraglobal('[#]', this.value);"
                                           class="form-control" name="fbmnumero" id="fbmnumero" maxlength="10">
                                </div>
                            </div>
                        </div>
                        <br>

                        <div class="container">
                            <div class="form-group">
                                <label for="gender1" class="col-sm-2 control-label">Meta: </label>
                                <div class="col-sm-10">
                                    <textarea type="text" class="form-control meta_cont" name="fbmdescricao"
                                              id="fbmdescricao" rows="6" cols="500"></textarea>
                                    <p><strong>Restam:</strong> <span class="contador">5000</span> caracteres.</span>
                                    </p>
                                </div>
                            </div>
                        </div>
                        <br>
                    </form>
                </div>

                <div class="modal-footer">
                    <button type="reset" class="btn btn-danger btn-default" data-dismiss="modal">
                        Cancelar <span class="glyphicon glyphicon-remove"></span>
                    </button>
                    <a href="#" id="salvarModalMeta" class="btn btn-primary">
                        Salvar <span class="glyphicon glyphicon-send"></span>
                    </a>
                </div>
            </div>
        </div>
    </div><!-- Fim Modal Metas -->

    <!-- Modal Delete -->
    <div class="modal fade in" id="modalExcluir" tabindex="-1" role="dialog" aria-labelledby="myModalLabel"
         aria-hidden="false">
        <input type="hidden" id="modalExcluirTipo" value="">
        <input type="hidden" id="modalExcluirId" value="">
        <input type="hidden" id="modalFbsdescricao" value="">
        <div class="modal-dialog">
            <div class="modal-content text-center">
                <div class="modal-header">
                    <h4 class="modal-title text-center" id="modalExcluirTitle"></h4>
                </div>
                <div class="modal-body"><h5 id="modalExcluirCorpo"></h5></div>
                <div class="modal-footer">
                    <!--                    <button type="button" class="btn btn-default" data-dismiss="modal">Cancel</button>-->
                    <button class="btn btn-default" data-dismiss="modal" id="modalExcluirCancel">Cancelar</button>
                    <button class="btn btn-danger" id="btn-excluirTipo">Deletar</button>
                </div>
            </div><!-- /.modal-content -->
        </div><!-- /.modal-dialog -->
    </div>

    <!-- Modal Info -->
    <div class="modal fade in" id="modalInfo" tabindex="-1" role="dialog" aria-labelledby="myModalLabel"
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
                    <button class="btn btn-danger" data-dismiss="modal" id="modalInfoOk" onclick="location.reload()">
                        Ok
                    </button>
                </div>
            </div><!-- /.modal-content -->
        </div><!-- /.modal-dialog -->
    </div>

    <!-- Modal Indicadores e Estratégias -->
    <div class="modal modal-wide fade" id="modalSubEst" tabindex="-1" role="dialog" aria-labelledby="myModalLabel"
         data-backdrop="static">
        <input type="hidden" id="modalSubEstFbmid" value="">
        <input type="hidden" id="modalSubMetaTipoSalvar" value="insert">
        <input type="hidden" id="modalSubMetaId" value="">
        <input type="hidden" id="modalEstTipoSalvar" value="insert">
        <input type="hidden" id="modalEstId" value="">

        <div class="modal-dialog " role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <ul class="nav nav-tabs nav-justified">
                        <li class="active"><a href="#modalSubmetas" data-toggle="tab" id="titleSubMeta">Indicadores</a>
                        </li>
                        <li><a href="#modalEstrategias" data-toggle="tab" id="titleEstrategia">Estratégias</a></li>
                    </ul>
                </div>
                <div class="modal-body">
                    <div class="tab-content">
                        <div class="tab-pane active" id="modalSubmetas">
                            <!-- FORM SUBMETAS -->
                            <form class="form-horizontal">
                                <div class="form-group">
                                    <label for="" class="col-sm-4 control-label">Nº do Indicadores:</label>
                                    <div class="col-sm-1" data-toggle="tooltip" title="Nº da Meta">
                                        <input type="text" class="form-control" id="modalSubMetaNumeroMeta" disabled>
                                    </div>
                                    <div class="col-sm-2">
                                        <input type="text" class="form-control" id="modalSubMetaNumero" maxlength="10">
                                    </div>

                                </div>
                                <div class="form-group">
                                    <label for="" class="col-sm-4 control-label ">Indicador:</label>
                                    <div class="col-sm-5">
                                        <textarea type="text" class="form-control meta_cont" name="modalSubMetaDesc"
                                                  id="modalSubMetaDesc" rows="6" cols="50"></textarea>
                                        <p><strong>Restam:</strong> <span class="contador">5000</span>
                                            caracteres.</span></p>
                                    </div>

                                </div>
                                <div class="form-group">
                                    <label for="" class="col-sm-4 control-label">Alcançou o Indicador?</label>
                                    <div class="col-sm-2">
                                        <select id="modalSubMetaAlcancada" name="modalSubMetaAlcancada"
                                                class="form-control">
                                            <option value="f">NÃO</option>
                                            <option value="t">SIM</option>
                                        </select>
                                    </div>
                                    <label for="" class="col-sm-1 control-label ">Prazo:</label>
                                    <div class='col-sm-2 input-group date'>
                                        <input type='text' class="form-control prazo" name="modalSubMetaPrazo"
                                               id="modalSubMetaPrazo"/>
                                        <span class="input-group-addon">
                                            <span class="glyphicon glyphicon-calendar"></span>
                                        </span>
                                    </div>
                                </div>
                                <div class="form-group">

                                    <div class='col-sm-4 col-sm-offset-3 input-group '>
                                        <a href="#" id="modalSubMetaSalvar" class=" btn  btn-primary btn-sm"
                                           style="width: 200px; margin-left: 50%">
                                            Salvar <span class="glyphicon glyphicon-send"></span>
                                        </a>
                                    </div>
                                </div>
                            </form>
                            <!--                            <table id="examplesub" class="display" width="100%"></table>-->
                            <div id="tabela_submeta"></div>
                            <div class="col-md-8">
                                <a href="#" class="btn btn-primary" data-toggle="modal" id="btnHistoricoIndicador">
                                    <span class="glyphicon glyphicon-plus"></span> Histórico
                                </a>
                            </div>
                        </div>
                        <div class="tab-pane" id="modalEstrategias">
                            <!-- FORM ESTRATÉGIAS -->
                            <form class="form-horizontal">

                                <div class="form-group">
                                    <label for="" class="col-sm-4 control-label">Nº da Estratégia:</label>
                                    <div class="col-sm-1" data-toggle="tooltip" title="Nº da Meta">
                                        <input type="text" class="form-control" id="modalEstNumeroMeta" disabled>
                                    </div>
                                    <div class="col-sm-2">
                                        <input type="text" class="form-control somenteNumero" id="modalEstNumero" maxlength="10">
                                    </div>

                                </div>

                                <div class="form-group">
                                    <label for="" class="col-sm-4 control-label ">Estratégia:</label>
                                    <div class="col-sm-5">
                                        <textarea type="text" class="form-control meta_cont" name="modalEstDesc"
                                                  id="modalEstDesc" rows="6" cols="50"></textarea>
                                        <p><strong>Restam:</strong> <span class="contador">5000</span>
                                            caracteres.</span></p>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label for="" class="col-sm-4 control-label">Alcançou a Estratégia?</label>
                                    <div class="col-sm-2">
                                        <select id="modalEstAlcancada" name="modalEstAlcancada" class="form-control">
                                            <option value="f">NÃO</option>
                                            <option value="t">SIM</option>
                                        </select>
                                    </div>
                                    <label for="" class="col-sm-1 control-label ">Prazo:</label>
                                    <div class='col-sm-2 input-group date'>
                                        <input type='text' class="form-control prazo" name="modalEstPrazo"
                                               id="modalEstPrazo"/>
                                        <span class="input-group-addon">
                                            <span class="glyphicon glyphicon-calendar"></span>
                                        </span>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label for="" class="col-sm-4 control-label">Previsões Orçamentárias:</label>
                                    <div class="col-sm-3">
                                        <!--                                        <input type="text" class="form-control" id="modalEstPrev"-->
                                        <!--                                               placeholder="LOA/2015 - Objetivo 0596 - Iniciativa 02 BP">-->
                                        <!--                                        <div class="col-sm-5">-->
                                        <textarea type="text" class="form-control" placeholder="LOA/2015 - Objetivo 0596 - Iniciativa 02 BP"
                                                  id="modalEstPrev" rows="3" cols="25" maxlength="500">
                                        </textarea>

                                    </div>

                                    <label class="radio-inline">
                                        <input type="radio" name="opcaoPrevisao" id="naocom" value="1">
                                        Não contemplado
                                    </label>


                                    <label class="radio-inline">
                                        <input type="radio" name="opcaoPrevisao" id="naoapl" value="2">
                                        Não se aplica
                                    </label>


                                </div>

                                <div class="form-group">
                                    <div class='col-sm-4 col-sm-offset-3 input-group '>
                                        <a href="#" id="modalEstSalvar" class=" btn  btn-primary btn-sm"
                                           style="width: 200px; margin-left: 50%">
                                            Salvar <span class="glyphicon glyphicon-send"></span>
                                        </a>
                                    </div>
                                </div>

                            </form>
                            <div id="tabela_estrategia"></div>
                            <div class="col-md-8">
                                <a href="#" class="btn btn-primary" data-toggle="modal" id="btnHistoricoEstrategia">
                                    <span class="glyphicon glyphicon-plus"></span> Histórico
                                </a>
                            </div>
                        </div>
                    </div>
                </div>
                <br><br>
                <div class="modal-footer">
                    <button type="" class="btn btn-danger btn-default btn-sm" data-dismiss="modal">
                        Cancelar <span class="glyphicon glyphicon-remove"></span>
                    </button>

                </div>
            </div>
        </div>
    </div>

    <!-- Modal Errors Upload -->
    <div class="modal modal-wide fade" id="modalErrorUpload"
         tabindex="-1" role="dialog" aria-labelledby="myModalLabel" data-backdrop="static">

        <div class="modal-dialog modal-lg" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <h3>Erro! Dados não Salvos!</h3>
                </div>
                <div class="modal-body">
                    <p class="lead">Os seguintes erros foram encontrados na planilha enviada:</p>
                    <ul class="list-group" id="listErrorUpload">
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

    <!-- Modal Historico Meta -->
    <div class="fade modal" id="modalHistoricoMeta" tabindex="-1" role="dialog">
        <div class="modal-history modal-lg" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h4 class="modal-title">Histórico de Metas</h4>
                </div>
                <br>
                <div class="row">
                    <div class="col-lg-8 col-lg-offset-1">
                        <div class="well">
                            <div class="form-group">
                                <input type="text" id="pesquisameta" name="pesquisameta" class="form-control"
                                       maxlength="500" value=""/><br>
                                <input type="button" value="Pesquisar" id="btn-pesquisar-meta" class="btn btn-success"/>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-body painel_po_body" id="tabela_historico_meta"></div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                </div>
            </div><!-- /.modal-content -->
        </div><!-- /.modal-history -->
    </div><!-- /.fade modal-->

    <!-- Modal Historico Meta -->
    <div class="fade modal" id="modalHistoricoImportacao" tabindex="-1" role="dialog">
        <div class="modal-history modal-lg" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h4 class="modal-title">Histórico de Importação</h4>
                </div>
                <br>
<!--                <div class="row">
                    <div class="col-lg-8 col-lg-offset-1">
                        <div class="well">
                            <div class="form-group">
                                <input type="text" id="pesquisaimportacao" name="pesquisaimportacao" class="form-control" maxlength="500" value=""/><br>
                                <input type="button" value="Pesquisar" id="btn-pesquisar-importacao" class="btn btn-success"/>
                            </div>
                        </div>
                    </div>
                </div>-->
                <div class="modal-body painel_po_body" id="tabela_historico_importacao" > </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                </div>
            </div><!-- /.modal-content -->
        </div><!-- /.modal-history -->
    </div><!-- /.fade modal-->

    <!-- Modal Historico Indicador -->
    <div class="fade modal" id="modalHistoricoIndicador" tabindex="-1" role="dialog">
        <div class="modal-history modal-lg" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h4 class="modal-title">Histórico de Indicadores</h4>
                </div>
                <br>
                <div class="row">
                    <div class="col-lg-8 col-lg-offset-2">
                        <div class="well">
                            <div class="form-group">
                                <input type="text" id="pesquisaindicador" name="pesquisaindicador" class="form-control"
                                       maxlength="500" value=""/><br>
                                <input type="button" value="Pesquisar" id="btn-pesquisar-indicador"
                                       class="btn btn-success"/>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-body painel_po_body" id="tabela_historico_indicador"></div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                </div>
            </div><!-- /.modal-content -->
        </div><!-- /.modal-history -->
    </div><!-- /.fade modal-->
    <!-- Modal Historico Estrategia -->
    <div class="fade modal" id="modalHistoricoEstrategia" tabindex="-1" role="dialog">
        <div class="modal-history modal-lg" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h4 class="modal-title">Histórico de Estratégias </h4>
                </div>
                <br>
                <div class="row">
                    <div class="col-lg-8 col-lg-offset-2">
                        <div class="well">
                            <div class="form-group">
                                <input type="text" id="pesquisaestrategia" name="pesquisaestrategia"
                                       class="form-control" maxlength="500" value=""/><br>
                                <input type="button" value="Pesquisar" id="btn-pesquisar-estrategia"
                                       class="btn btn-success"/>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-body painel_po_body" id="tabela_historico_estrategia"></div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                </div>
            </div><!-- /.modal-content -->
        </div><!-- /.modal-history -->
    </div><!-- /.fade modal-->
</div>

<!-- Componentes datetimepicker JS -->
<script type="text/javascript" src="js/bootstrap-datetimepicker/moment.min.js"></script>
<script type="text/javascript" src="js/bootstrap-datetimepicker/moment.pt-br.js"></script>
<script type="text/javascript" src="js/bootstrap-datetimepicker/bootstrap-datetimepicker.min.js"></script>

<!-- Componentes datatable CSS -->
<script type="text/javascript" src="../library/bootstrap-datatable/js/jquery.dataTables.js"></script>
<script type="text/javascript" src="../library/bootstrap-datatable/js/dataTables.bootstrap.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-filestyle/1.2.3/bootstrap-filestyle.min.js"></script>

<script>
    $(document).ready(function () {

        /**
         * Esconder Modal de exclusão
         */
        $('#modal-alert button:first-child').click(function () {
            $('#modalExcluir').modal('hide')
        });

        <?php
        $muncod = ($municipio->muncod == null) ? '' : ",'$municipio->muncod'";
        $appendMuncod = ($municipio->muncod == null) ? 'null' : "$municipio->muncod";

        if ($fichamonitoramentoEstado->estuf != null) {
            $aux = "'$fichamonitoramentoEstado->estuf' , '$_GET[fmeid]' ";
            $aux2 = "estuf: '$fichamonitoramentoEstado->estuf',";
            $appendEstuf = $fichamonitoramentoEstado->estuf;
            $appendFmaid = $_GET[fmeid];
        } else {
            $aux = "'$fichamonitoramento->estuf' , '$_GET[fmtid]', '$municipio->muncod' ";
            $aux2 = "estuf: '$fichamonitoramento->estuf',muncod: '$municipio->muncod',";
            $appendEstuf = $fichamonitoramento->estuf;
            $appendFmaid = $_GET[fmtid];
        }
        ?>
        construirTabelaMeta(<?= $aux; ?>);
        /**
         * Limpar modal Meta ao fechar
         */
        $('#modalMeta').on('hidden.bs.modal', function (e) {
            $('#tipoSalvar').val('');
            $('#fbmid').val('');
            $('#fbmnumero').val('');
            $('#fbmdescricao').val('');
        });

        /**
         * Limpar modal SubEst ao fechar
         */
        $('#modalSubEst').on('hidden.bs.modal', function (e) {
            $('#modalSubEstFbmid').val('');

            $('#modalSubMetaTipoSalvar').val('insert');
            $('#modalSubMetaNumeroMeta').val('');
            $('#modalSubMetaNumero').val('');
            $('#modalSubMetaDesc').val('');
            $('#modalSubMetaAlcancada').val('f');
            $('#modalSubMetaPrazo').val('');
            $('#modalSubMetaId').val('');

            $('#modalEstTipoSalvar').val('insert');
            $('#modalEstNumeroMeta').val('');
            $('#modalEstNumero').val('');
            $('#modalEstDesc').val('');
            $('#modalEstAlcancada').val('f');
            $('#modalEstPrazo').val('');
            $('#modalEstPrev').val('');
            $('[name="opcaoPrevisao"]').prop('checked', false);

        });

        /**
         * Limpar modal #modalExcluir ao fechar
         */
        $('#modalExcluir').on('hidden.bs.modal', function (e) {
            var tipo = $('#modalExcluirTipo').val();
            if (tipo == 'indicador' || tipo == 'estrategia') {
                $('#modalSubEst').show();
            }
        });

        $('#titleSubMeta').click(function () {
            limparModalSubEst();
        });
        $('#titleEstrategia').click(function () {
            limparModalSubEst();
        });

        $('[name="opcaoPrevisao"]').click(function () {
            $('#modalEstPrev').val('');
        });

        $('#modalEstPrev').focus(function () {
            $('[name="opcaoPrevisao"]').prop('checked', false);
        });

        $('.prazo').datetimepicker({
            minDate: '2014-01-01',
            maxDate: '2030-12-31',
            format: "YYYY"
        });

        $('#btnCadastrarMeta').click(function () {
            $('#tipoSalvar').val('insert');

            //mostra modal
            $('#modalMeta').modal('show');
        });

        //deleta Meta
        $('#btn-excluirTipo').click(function () {
            var tipo = $('#modalExcluirTipo').val();
            var id = $('#modalExcluirId').val();
            var fbsdescricao = $('#modalFbsdescricao').val();

            $.ajax({
                url: '',
                type: 'POST',
                data: {
                    excluirTipo: true,
                    tipo: tipo,
                    fbsdescricao: fbsdescricao,
                    id: id
                },
                success: function (resposta) {
                    if (resposta == 'true') {
                        alert('Deletado com sucesso!');

                        switch (tipo) {
                            case 'meta' :
                                construirTabelaMeta(<?= $aux; ?>);
                                break;
                            case 'indicador' :
                                construirTabelaSubMeta($('#modalSubEstFbmid').val());
                                $('#modalSubEst').show();
                                break;
                            case 'estrategia' :
                                construirTabelaEst($('#modalSubEstFbmid').val());
                                $('#modalSubEst').show();
                                break;
                        }
                        $('#modalExcluir').modal('hide');
                        return;
                    } else if (resposta == 'cascade') {
                        alert('Para excluir esta Meta é necessário excluir todas os Indicadores e Estratégias.');

                    } else if (resposta == 'cascadeIndicador') {
                        alert('Não é possível excluir o indicador "' + fbsdescricao + '", pois há dados  cadastrados em monitoramento de metas.');
                    }
                }
            });
        });

        $('#salvarModalMeta').click(function () {
            var tipo_salvar   = $('#tipoSalvar').val();
            var fbm_numero    = $('#fbmnumero').val();
            var fbm_descricao = $('#fbmdescricao').val();
            var fbmid         = $('#fbmid').val();

            if (fbm_numero == '') {
                alert('Campo Número da Meta Obrigatório');
                return false;
            }

            if (fbm_descricao == '') {
                alert('Campo Meta Obrigatório');
                return false;
            }

            $.ajax({
                url: '',
                type: 'POST',
                data: {
                    salvarAjaxMeta: true,
                    fbm_numero: fbm_numero,
                    fbm_descricao: fbm_descricao,
                    tipo_salvar: tipo_salvar,
                    <?= $aux2; ?>
                    fbmid: fbmid

                },
                success: function (resposta) {
                    if (resposta == 'true') {
                        alert('Salvo com sucesso!');
                        construirTabelaMeta(<?= $aux; ?>);
                        $('#modalMeta').modal('hide');
                        return false;
                    } else if (resposta == 'duplicado') {
                        alert('A meta informada já foi cadastrada');
                        return false;
                    }
                }
            });
        });

        $('#modalEstSalvar').click(function () {
            var fbmid = $('#modalSubEstFbmid').val();

            var tipo_salvar = $('#modalEstTipoSalvar').val();
            var fbenumero = $('#modalEstNumero').val();
            var fbedescricao = $('#modalEstDesc').val();
            var fbeprazo = $('#modalEstPrazo').val();
            var fbealcancou = $('#modalEstAlcancada').val();
            var fbeprevisao = $('#modalEstPrev').val();
            var fbeid = $('#modalEstId').val();
            var opcaoPrevisao = $('input[name=opcaoPrevisao]:checked').val();

            var ponto = fbenumero.indexOf("..");

            if (fbenumero == '') {
                alert('Campo Número da Estratégia Obrigatório');
                return false;
            }

            if (ponto != -1) {
                alert('Não é permitido pontos consecutivos nesta coluna');
                return false;
            }

            if (fbedescricao == '') {
                alert('Campo Estratégia Obrigatório');
                return false;
            }

            if (fbeprazo == '') {
                alert('Campo Prazo Obrigatório');
                return false;
            } else if (fbeprazo < 2014 || fbeprazo > 2030) {
                alert('O ano informado no campo "Prazo" deve ser igual ou maior que 2014 e igual ou menor que 2030');
                return false;
            }

            if (fbeprevisao === '' && opcaoPrevisao === undefined) {
                alert('Informe um valor ou escolha uma das opções para o campo "Previsões Orçamentárias" ');
                return false;
            }

            $.ajax({
                url: '',
                type: 'POST',
                data: {
                    salvarEstrategia: true,
                    fbenumero: fbenumero,
                    fbedescricao: fbedescricao,
                    fbealcancou: fbealcancou,
                    fbeprevisao: fbeprevisao,
                    fbeprazo: fbeprazo,
                    tipo_salvar: tipo_salvar,
                    estuf: '<?= $fichamonitoramentoEstado->estuf; ?>',
                    fbmid: fbmid,
                    fbeid: fbeid,
                    opcaoPrevisao: opcaoPrevisao
                },
                success: function (resposta) {

                    if (resposta == 'true') {
                        alert('Salvo com sucesso!');

                        limparModalSubEst();

                        construirTabelaEst(fbmid);

                    } else if (resposta == 'duplicado') {
                        alert('A Estratégia informada já foi cadastrada');
                    }
                }
            });
        });

        $('#modalSubMetaSalvar').click(function () {

            var fbmid        = $('#modalSubEstFbmid').val();
            var tipo_salvar  = $('#modalSubMetaTipoSalvar').val();
            var fbsnumero    = $('#modalSubMetaNumero').val();
            var fbsdescricao = $('#modalSubMetaDesc').val();
            var fbsprazo     = $('#modalSubMetaPrazo').val();
            var fbsaalcancou = $('#modalSubMetaAlcancada').val();
            var fbsid        = $('#modalSubMetaId').val();

            if (fbsnumero == '') {
                alert('Campo Número do Indicador Obrigatório');
                return false;
            }

            if (fbsdescricao == '') {
                alert('Campo Meta Obrigatório');
                return false;
            }

            if (fbsprazo == '') {
                alert('Campo Prazo Obrigatório');
                return false;
            } else if (fbsprazo < 2014 || fbsprazo > 2030) {
                alert('O ano informado no campo "Prazo" deve ser igual ou maior que 2014 e igual ou menor que 2030');
                return false;
            }

            $.ajax({
                url: '',
                type: 'POST',
                data: {
                    salvarSubMeta: true,
                    fbsnumero: fbsnumero,
                    fbsdescricao: fbsdescricao,
                    fbsaalcancou: fbsaalcancou,
                    fbsprazo: fbsprazo,
                    tipo_salvar: tipo_salvar,
                    estuf: '<?= $fichamonitoramentoEstado->estuf; ?>',
                    fbmid: fbmid,
                    fbsid: fbsid
                },
                success: function (resposta) {
                    if (resposta == 'true') {
                        alert('Salvo com sucesso!');

                        limparModalSubEst();

                        construirTabelaSubMeta(fbmid);

                    } else if (resposta == 'duplicado') {
                        alert('O Indicador informada já foi cadastrada');
                    }
                }
            });
        });

        $(".meta_cont").keyup(function () {
            var limite = 5000
            var tamanho = $(this).val().length;
            if (tamanho > limite)
                tamanho -= 1;

            var contador = limite - tamanho;
            $(".contador").text(contador);

            if (tamanho >= limite) {
                var txt = $(this).val().substring(0, limite);
                $(this).val(txt)
            }
        });

        $('#arquivo').filestyle({
            buttonText: 'Importação de Meta',
            buttonName: 'btn-primary',
            input: false
        });

        //Aparece o botão salvar importação caso selecione algum arquivo
        $('#arquivo').change(function () {
            $('#addDocumento').show();
        });

        $('#addDocumento').click(function () {
            var arquivo = $("#arquivo").val();

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

            var file_data = $('#arquivo').prop('files')[0];
            var form_data = new FormData();
            form_data.append('userfile', file_data);
            form_data.append('estuf', '<?= $appendEstuf ?>');
            form_data.append('muncod', '<?= $appendMuncod ?>');
            form_data.append('usucpf', '<?= $_SESSION['usucpf'] ?>');
            form_data.append('fmaid', '<?= $appendFmaid ?>');
            form_data.append('uploadFichaB', true);

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
                            popularModalErrosUpload($("#listErrorUpload"), obj['msg']);
                            $('#modalErrorUpload').modal('show');
                            break;
                        case 'success':
                            construirTabelaMeta(<?= $aux; ?>);
                            alert('Enviado com sucesso!');
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

    //FUNCOES SOBRE META

    /**
     * Construir a Tabela de Metas ao carregar a pagina ou incluir/editar metas
     */
    function construirTabelaMeta(estuf, fmaid, muncod = null) {
        $.ajax({
            url: '',
            type: 'POST',
            data: {
                construirTabelaMeta: true,
                estuf: estuf,
                fmaid: fmaid,
                muncod: muncod

            },
            success: function (resposta) {
                $('#tabela_meta').html(resposta);
                $('.rodape').hide();
                $('[data-toggle="popover"]').popover();
                $('#tabMeta').DataTable({
                    "paging": false,
                    "info": false,
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

    /**
     * Preencher Modal  #modalMeta com os dados da meta escolhida
     */
    function preencherModalEditarMeta(fbmid, fbmnumero, fbmdescricao) {

        $('#tipoSalvar').val('update');
        $('#fbmid').val(fbmid);
        $('#fbmnumero').val(fbmnumero);
        $('#fbmdescricao').val(fbmdescricao);
        $('#modalMeta').modal('show');
    }

    // FUNCOES SOBRE O MODAL SUBMETAS/ESTRATÉGIA
    /**
     * Preencher #modalSubEst e construir as tabelas
     * com os dados iniciais de Submeta e Estratégia
     */
    function preencherModalSubEst(fbmid, nuMeta) {

        $('#modalSubEstFbmid').val(fbmid);
        $('#modalSubMetaNumeroMeta').val(nuMeta);
        $('#modalEstNumeroMeta').val(nuMeta);

        construirTabelaSubMeta(fbmid);
        construirTabelaEst(fbmid);

        $('#modalSubEst').modal('show');
    }

    /**
     * Retornar o modal #modalSubEst ao estado original
     */

    function limparModalSubEst() {
        $('#modalSubMetaTipoSalvar').val('insert');
        $('#modalSubMetaNumero').val('');
        $('#modalSubMetaDesc').val('');
        $('#modalSubMetaAlcancada').val('f');
        $('#modalSubMetaPrazo').val('');

        $('#modalEstTipoSalvar').val('insert');
        $('#modalEstNumero').val('');
        $('#modalEstDesc').val('');
        $('#modalEstAlcancada').val('f');
        $('#modalEstPrazo').val('');
        $('#modalEstPrev').val('');
        $('[name="opcaoPrevisao"]').prop('checked', false);
    }


    // FUNCOES SOBRE SUBMETA

    /**
     * Construir a tabela #tabSubMeta no modal #modalSubEst
     */
    function construirTabelaSubMeta(fbmid) {

        $.ajax({
            url: '',
            type: 'POST',
            data: {
                construirTabelaSubMeta: true,
                fbmid: fbmid
            },
            success: function (resposta) {

                $('#tabela_submeta').html(resposta);
                $('[data-toggle="popover"]').popover();

                $('#tabSubMeta').DataTable({
                    "destroy": true,
                    "paging": true,
                    "lengthMenu": [[5, 10, -1], [5, 10, "Todos"]],
                    "info": true,
                    "aoColumnDefs": [
                        {
                            'bSortable': false,
                            'aTargets': [-1, -3, -4]
                        },
                    ],
                    "language": {
                        "url": "https://cdn.datatables.net/plug-ins/1.10.12/i18n/Portuguese-Brasil.json"
                    }
                });
            }
        });
    }

    /**
     * Preencher o form para editar submeta
     */
    function preencherFormEditarSubMeta(fbsnumero, fbsdescricao, fbsprazo, fbsaalcancou, fbsid) {

        $('#modalSubMetaTipoSalvar').val('update');
        $('#modalSubMetaNumero').val(fbsnumero);
        $('#modalSubMetaDesc').val(fbsdescricao);
        $('#modalSubMetaAlcancada').val(fbsaalcancou);
        $('#modalSubMetaPrazo').val(fbsprazo);
        $('#modalSubMetaId').val(fbsid);

    }


    // FUNCOES SOBRE ESTRATEGIA


    /**
     * Construir a tabela #tabEst no modal #modalSubEst
     */
    function construirTabelaEst(fbmid) {

        $.ajax({
            url: '',
            type: 'POST',
            data: {
                construirTabelaEst: true,
                fbmid: fbmid
            },
            success: function (resposta) {

                $('#tabela_estrategia').html(resposta);
                $('[data-toggle="popover"]').popover();
                $('#tabEst').DataTable({
                    "destroy": true,
                    "paging": true,
                    "lengthMenu": [[5, 10, -1], [5, 10, "Todos"]],
                    "aoColumnDefs": [
                        {
                            'bSortable': false,
                            'aTargets': [-1, -2, -4, -5]
                        },
                    ],
                    "language": {
                        "url": "https://cdn.datatables.net/plug-ins/1.10.12/i18n/Portuguese-Brasil.json"
                    }
                });
            }
        });


    }

    /**
     * Preencher o form para editar estrategia
     */
    function preencherFormEditarEst(fbenumero, fbedescricao, fbeprazo
        , fbealcancou, fbeid, fbeprevisao, fbencontemplado, fbenaplica) {

        $('#modalEstTipoSalvar').val('update');
        $('#modalEstNumero').val(fbenumero);
        $('#modalEstDesc').val(fbedescricao);
        $('#modalEstAlcancada').val(fbealcancou);
        $('#modalEstPrazo').val(fbeprazo);
        $('#modalEstId').val(fbeid);
        $('#modalEstPrev').val(fbeprevisao);

        if (fbencontemplado == 't') {
            $('#naocom').prop('checked', true);
            $('#naoapl').prop('checked', false);
        } else if (fbenaplica == 't') {
            $('#naoapl').prop('checked', true);
            $('#naocom').prop('checked', false);
        } else {
            $('#naoapl').prop('checked', false);
            $('#naocom').prop('checked', false);

        }

    }


    //OUTRAS FUNCOES


    /**
     * Monta o modal excluir para meta, submeta ou estratégia.
     * @param string tipo Define qual será o tipo excluido (meta, submeta ou estrategia).
     * @param integer id O id que será excluído.
     */

    function modalExcluir(tipo, descricao, id) {
        //preenche modal excluir
        $('#modalExcluirTipo').val(tipo);
        $('#modalExcluirId').val(id);
        $('#modalFbsdescricao').val(descricao);
        $('#modalExcluirTitle').html('Deletar ' + toTitleCase(tipo));
        $('#modalExcluirCorpo').html('Deseja excluir ' + tipo.toLowerCase() + '?');

        //esconde modal subest
        $('#modalSubEst').hide();
        //mostra modal subest
        $('#modalExcluir').modal('show');
    }

    function toTitleCase(str) {
        return str.replace(/\w\S*/g, function (txt) {
            return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase();
        });
    }

    $('[data-toggle="tooltip"]').tooltip();

    $('#btnHistoricoMeta').click(function () {
        jQuery.ajax({
            url: '',
            type: "POST",
            data: {
                recuperarHistorico: 'meta',
                estuf: '<?php echo $fichamonitoramentoEstado->estuf ?>',
                muncod: '<?php echo $municipio->muncod ?>',
            },
            success: function (resposta) {
                $("#tabela_historico_meta").html(resposta);
                $('#modalHistoricoMeta').modal('show');
            }
        });
    });
    $('#btnHistoricoIndicador').click(function () {
        var fbmid = $('#modalSubEstFbmid').val();
        jQuery.ajax({
            url: '',
            type: "POST",
            data: {
                recuperarHistorico: 'indicador',
                fbmid: fbmid,
            },
            success: function (resposta) {
                $("#tabela_historico_indicador").html(resposta);
                $('#modalHistoricoIndicador').modal('show');
            }
        });
    });
    $('#btnHistoricoEstrategia').click(function () {
        var fbmid = $('#modalSubEstFbmid').val();
        jQuery.ajax({
            url: '',
            type: "POST",
            data: {
                recuperarHistorico: 'estrategia',
                fbmid: fbmid,
            },
            success: function (resposta) {
                $("#tabela_historico_estrategia").html(resposta);
                $('#modalHistoricoEstrategia').modal('show');
            }
        });
    });

    $('#btn-pesquisar-meta').click(function () {
        var pesquisameta = $('#pesquisameta').val();
        jQuery.ajax({
            url: '',
            type: "POST",
            data: {
                recuperarHistorico: 'meta',
                pesquisameta: pesquisameta,
                estuf: '<?php echo $fichamonitoramentoEstado->estuf ?>',
                muncod: '<?php echo $municipio->muncod ?>',
            },
            success: function (resposta) {
                $("#tabela_historico_meta").html(resposta);
                $('#modalHistoricoMeta').modal('show');
            }
        });
    });
    $('#btn-pesquisar-indicador').click(function () {
        var pesquisaindicador = $('#pesquisaindicador').val();
        var fbmid = $('#modalSubEstFbmid').val();
        jQuery.ajax({
            url: '',
            type: "POST",
            data: {
                recuperarHistorico: 'indicador', pesquisaindicador: pesquisaindicador, fbmid: fbmid
            },
            success: function (resposta) {
                $("#tabela_historico_indicador").html(resposta);
                $('#modalHistoricoIndicador').modal('show');
            }
        });
    });
    $('#btn-pesquisar-estrategia').click(function () {
        var pesquisaestrategia = $('#pesquisaestrategia').val();
        var fbmid = $('#modalSubEstFbmid').val();
        jQuery.ajax({
            url: '',
            type: "POST",
            data: {
                recuperarHistorico: 'estrategia', pesquisaestrategia: pesquisaestrategia, fbmid: fbmid
            },
            success: function (resposta) {
                $("#tabela_historico_estrategia").html(resposta);
                $('#modalHistoricoEstrategia').modal('show');
            }
        });
    });
$('#btnHistoricoImportacao').click(function () {
    jQuery.ajax({
        url: '',
        type: "POST",
        data: {
            recuperarHistorico: 'importacao',
            estuf: '<?php echo $fichamonitoramentoEstado->estuf ?>',
            muncod: '<?php echo $municipio->muncod ?>',
        },
        success: function (resposta) {
            $("#tabela_historico_importacao").html(resposta);
            $('#modalHistoricoImportacao').modal('show');
        }
    });
});
$('#btn-pesquisar-importacao').click(function(){
    var pesquisaimportacao = $('#pesquisaimportacao').val();
    jQuery.ajax({
        url: '',
        type: "POST",
        data: {
            recuperarHistorico: 'importacao', 
                                pesquisaimportacao: pesquisaimportacao, 
                                estuf: '<?php echo $fichamonitoramentoEstado->estuf ?>',
                                muncod: '<?php echo $municipio->muncod ?>',
        },
        success: function (resposta) {
            $("#tabela_historico_importacao").html(resposta);
            $('#modalHistoricoImportacao').modal('show');
        }
    });
});
function downloadArquivo (arqid){

    var url = window.location.href;
    if (url.charAt(url.length-1) == '#'){
        url = url.substring(0, url.length-1)+'&acao_download=downloadAnexo&arqid='+arqid;
    }else{
        url = window.location.href+'&acao_download=downloadAnexo&arqid='+arqid;
    }
    window.location=url;
    return;
}

    $(document).ready(function(){
        $(".somenteNumero").bind("keyup blur focus", function(e) {
            e.preventDefault();
            var expre = /[^\d.,]/g;
            $(this).val($(this).val().replace(expre,''));
        });
    });
</script>


<!--//informacoesGerais-->

<?php
$estadoIniciativa = true;

if($mdDocPl->esdid == PAR3_ESDID_CADASTRAMENTO || $mdDocPl->esdid == PAR3_ESDID_EM_DILIGENCIA  ){
    $estadoIniciativa = false;
}

?>
<br>
<br>
<hr>
<br>
<table width="100%">
    <tr>
        <td>
             <h4><b>Informações Gerais</b></h4>
        </td>
    </tr>
    <tr>
        <td class="col-lg-12">
            <div id="informacoesgeraisListagem">
                <?php
                $controllerIniciativaInformacoes = new Par3_Controller_Iniciativa_planejamento_informacoesiniciativa();
                $controllerIniciativaInformacoes->listaInformacoesGerais($inp->inpid);
                ?>
            </div>
        </td>
        <td>
             <button style="margin-left: 10px;" type="button" class="btn btn-success" id="btn-incluir-informacoesIniciativa">    <i class="fa fa-plus"></i>
            </button>
        </td>
    </tr>
</table>

<br>
<br>
<hr>
<br>


<!-- div detalheEtapa-->
<table width="100%">
    <tr>
        <td>
            <h4><b>Detalhamento das Etapas</b></h4>
        </td>
    </tr>
    <tr>
        <td class="col-lg-12">
            <div id="detalheEtapaListagem">
                <?php
                $controllerIniciativaDetalhe = new Par3_Controller_Iniciativa_planejamento_detalheetapa();
                $controllerIniciativaDetalhe->listaDetalhe($inp->inpid);
                //conta para saber se existe alguma etapa cadastrada, caso não esconde a atividades
                $count = $controllerIniciativaDetalhe->verificaQntDetalhes($inp->inpid);
                ?>
            </div>
        </td>

        <td>

            <button style="margin-left: 10px;" type="button" class="btn btn-success" id="btn-incluir-detalheEtapas"><i class="fa fa-plus"></i>
            </button>

        </td>

    </tr>
</table>




<!-- div Atividade Etapa-->
<div id='atividadesEtapas' style="display: <?php if($count>0) echo "block"; else  echo "none"; ?>">
    <br>




    <table width="100%">

        <tr>
            <td>
               <h4><b>Atividades das Etapas</b></h4>
            </td>
        </tr>
        <tr>
            <td class="col-lg-12">
                <div id="atividadeEtapaListagem">
                    <?php
                    $controllerIniciativaAtividade = new Par3_Controller_Iniciativa_planejamento_atividadeetapa();
                   
                    $controllerIniciativaAtividade->listarAtividade($inp->inpid);
                    ?>
                </div>

            </td>
            <td>

                <button style="margin-left: 10px;" type="button" class="btn btn-success" id="btn-incluir-AtividadeEtapas"><i class="fa fa-plus"></i>
                </button>

            </td>

        </tr>
        <tr>
            <td>

                <div id='totalCarga' align="right"><h4><span id='totalCargaHoraria'>
                        <? echo 'Carga horária total: '.(int)$controllerIniciativaAtividade->totalCargaHoraria($inp->inpid) ?></span></h4></div>

            </td>
        </tr>

    </table>
</div>

<br>
<br>
<hr>
<br>

<!-- div Insumos Iniciativa-->
<table width="100%">
    <tr>
        <td>
            <h4><b>Insumos da Iniciativa</b></h4>
        </td>
    </tr>
    <tr>
        <td class="col-lg-12">
            <div id="insumosIniciativa">
                <?php
                $controllerIniciativaInsumos = new Par3_Controller_Iniciativa_planejamento_insumosiniciativa();
                $controllerIniciativaInsumos->listaInsumosIniciativa($inp->inpid);
                ?>
            </div>
        </td>
        <td>

            <button style="margin-left: 10px;" type="button" class="btn btn-success" id="btn-incluir-insumosIniciativa"><i class="fa fa-plus"></i>
            </button>

        </td>
    </tr>
</table>


<!--Modal informacoes Gerais-->
<div class="ibox float-e-margins animated modal" id="modal_informacoesgerais" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formInformacoesGerais" id="formInformacoesGerais" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Informações Gerais da Iniciativa - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal-informacoes">
                    <?php $controllerIniciativaInformacoes->formInformacoesGerais($_REQUEST);//CHAMA FORM PASSANDO ID DA INICIATIVA ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarInformacoesGerais" data-dismiss="modal"
                                class="btn btn-default"
                        <i class="fa fa-times-circle-o"></i> Cancelar</button>
                        <button type="submit" id="salvarInformacoesGerais" class="btn btn-success"
                        <i class="fa fa-plus-square-o"></i> Salvar</button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<!--Modal Detalhamento Etapa-->
<div class="ibox float-e-margins animated modal" id="modal_detalheEtapa" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formdetalheEtapa" id="formdetalheEtapa" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_tituloDetalheEtapa">Detalhamento das Etapas - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal-detalheEtapa">
                    <?php $controllerIniciativaDetalhe->formDetalheEtapa($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDetalheEtapa" data-dismiss="modal" class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarDetalheEtapa"
                                class="btn btn-success"
                        <
                        <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<!--<!--Modal Atividade Etapa-->
<div class="ibox float-e-margins animated modal" id="modal_atividadeEtapas" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formatividadeetapas" id="formatividadeetapas" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_tituloAtividadeEtapa">Atividades das Etapas - Incluir</h3>
                </div>
                <div class="ibox-conteint" id="conteudo-modal-atividadeEtapas">
                    <?php $controllerIniciativaAtividade->formAtividade($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarAtividadeEtapas" data-dismiss="modal" class="btn btn-default"
                        <i class="fa fa-times-circle-o"></i> Cancelar</button>
                        <button type="submit" id="salvarAtividadeEtapas" class="btn btn-success"
                        <i class="fa fa-plus-square-o"></i>Salvar</button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<!--Modal Insumos Iniciativa-->
<div class="ibox float-e-margins animated modal" id="modal_insumosIniciativa" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="forminsumoIniciativa" id="forminsumoIniciativa" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_tituloInsumoIniciativa">Insumos da Iniciativa - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal-insumosIniciativa">
                    <?php $controllerIniciativaInsumos->formInsumosIniciativa($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarInsumosIniciativa" data-dismiss="modal"
                                class="btn btn-default"
                        <i class="fa fa-times-circle-o"></i> Cancelar</button>
                        <button type="submit" id="salvarInsumosIniciativa" class="btn btn-success"
                        <i class="fa fa-plus-square-o"></i> Salvar</button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<input type="hidden"  id="estadoIniciativa"  value="<?= $estadoIniciativa ?>" />
<script>

    $("#btn-incluir-informacoesIniciativa").click(function (evt) {

        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }

        var caminho = window.location.href;
        var action = '&requisicao=editarItensComposicaoFormacao&identificador=informacoesgerais&ipiaid=';
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal-informacoes").find("input").val("");
                $("#conteudo-modal-informacoes").html(resp);
                $("#modal_titulo").html("Informações Gerais - Incluir");
                $('.control-label').removeClass('col-lg-3');
                $("#modal_informacoesgerais").modal("show");
                $("#ipifprofessores").focus();
            }
        });

    });


    jQuery('#salvarInformacoesGerais').click(function (evt) {

        evt.preventDefault();
        var ipidid = $('#conteudo-modal-informacoes').find('[name=ipifid]').val();
        var msg = 'Incluir';
        if (ipidid) {
            msg = 'Editar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> esta Informação?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarInformacoesGerais();
                }
                return false;
            }
        );
    });

    function salvarInformacoesGerais() {
        var caminho = window.location.href;
        var action = 'requisicao=salvarItensComposicaoFormacao&identificador=informacoesgerais&' + $("#formInformacoesGerais").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            success: function (resp) {
               
                var res = $.parseJSON(resp);
                
                if (!isNaN(parseInt(res.resp)) && isFinite(res.resp)) {

                    $("#modal_informacoesgerais").modal("hide");
                    $("#informacoesgeraisListagem").html(res.lista);
                    swal.close();
                    //msgSuccess(caminho, 'Informações Salvas!', 'success');
                    return false;
                }
                swal.close();
                return false;
            }
        });
        return false;
    }

    function editarInformacoesGerais(id) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        var caminho = window.location.href;
        var action = '&requisicao=editarItensComposicaoFormacao&identificador=informacoesgerais&ipifid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal-informacoes").find("input").val("");
                $("#conteudo-modal-informacoes").html(resp);
                $("#modal_titulo").html("Informações Gerais - Editar");
                $('.control-label').removeClass('col-lg-3');
                $("#modal_informacoesgerais").modal("show");
                $("#ipifprofessores").select();

            }
        });
    }

    function removerIformacoesGerais(id) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        swal({
            title: "Remover Informação Gerais",
            text: "Tem certeza que deseja remover?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var caminho = window.location.href;
                var action = '&requisicao=removerItensComposicaoFormacao&identificador=informacoesgerais&ipifid=' + id;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        var res = $.parseJSON(resp);
                        
                        if (res.resp != 'erro') {
                            $("#informacoesgeraisListagem").html(res.lista);
                            //msgSuccess(caminho, 'Informações Removidas!', 'success');
                            swal.close();
                            return true;
                        }
                        swal("Erro.", "Ocorreu um erro ao remover a Informações da Iniciativa.", "error");
                    }
                });
            }
        })
    }


    //---------js detalhes etapa-------------------//

    $("#btn-incluir-detalheEtapas").click(function (evt) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        var caminho = window.location.href;
        var action = '&requisicao=editarItensComposicaoFormacao&identificador=detalheEtapa&ipidid=';
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal-detalheEtapa").find("input").val("");
                $("#conteudo-modal-detalheEtapa").html(resp);
                $("#modal_tituloDetalheEtapa").html("Detalhamento das Etapas - Incluir");
                $('.control-label').removeClass('col-lg-3');
                $("#modal_detalheEtapa").modal("show");
                $("#ipidatividade").focus();
            }
        });

    });

    jQuery('#salvarDetalheEtapa').click(function (evt) {
        evt.preventDefault();
        var ipidid = $('#conteudo-modal-detalheEtapa').find('[name=ipidid]').val();
        var msg = 'Incluir';
        if (ipidid) {
            msg = 'Editar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este Detalhe?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarDetalheEtapa();
                }
                return false;
            }
        );
    });

    function salvarDetalheEtapa() {
        var caminho = window.location.href;
        var action = '&requisicao=salvarItensComposicaoFormacao&identificador=detalheEtapa&' + $("#formdetalheEtapa").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);

                 if (!isNaN(parseInt(res.resp)) && isFinite(res.resp)) {
                   $("#modal_detalheEtapa").modal("hide");
                   $("#atividadesEtapas").show();
                   $("#detalheEtapaListagem").html(res.lista);
                   
                    swal.close();
                    return false;
                }
                swal.close();
                return false;
            }
        });
        return false;
    }

    function editarDetalhesEtapa(id) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        var caminho = window.location.href;
        var action = '&requisicao=editarItensComposicaoFormacao&identificador=detalheEtapa&ipidid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal-detalheEtapa").find("input").val("");
                $("#conteudo-modal-detalheEtapa").html(resp);
                $("#modal_tituloDetalheEtapa").html("Detalhamento das Etapas - Editar");
                $('.control-label').removeClass('col-lg-3');
                $("#modal_detalheEtapa").modal("show");
                $("#ipidatividade").select();
            }
        });
    }

    function removerIDetalhesEtapa(id) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        swal({
            title: "Remover Detalhamento das Etapas",
            text: "Tem certeza que deseja remover?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var arrayCronograma = $('#form-coronograma-iniciativa').serialize();
                var caminho = window.location.href + '&' + arrayCronograma;
                var action = '&requisicao=removerItensComposicaoFormacao&identificador=detalheEtapa&ipidid=' + id;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        var res = $.parseJSON(resp);
                             
                        if (res.resp != 'erro') {
                            $("#detalheEtapaListagem").html(res.lista);
                            if(res.count<1){
                                $("#atividadesEtapas").hide();
                            }
                            swal.close();
                            return true;
                        }
                        swal("Erro.", "Existem Atividades Ultilizando o Detalhamento.", "error");
                    }
                });
            }
        })
    }

    //---------js Insumos Iniciativa-------------------//

    $("#btn-incluir-insumosIniciativa").click(function (evt) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        var caminho = window.location.href;
        var action = '&requisicao=editarItensComposicaoFormacao&identificador=insumoIniciativa&ipiinid=';
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal-insumosIniciativa").find("input").val("");
                $("#conteudo-modal-insumosIniciativa").html(resp);
                $("#modal_tituloInsumoIniciativa").html("Insumos da Iniciativa - Incluir");
                $('.control-label').removeClass('col-lg-3');
                $("#modal_insumosIniciativa").modal("show");
                $("#ipiindescricao").focus();
            }
        });

    });

    jQuery('#salvarInsumosIniciativa').click(function (evt) {
        evt.preventDefault();
        var ipiinid = $('#conteudo-modal-insumosIniciativa').find('[name=ipiinid]').val();
        var msg = 'Incluir';
        if (ipiinid) {
            msg = 'Editar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> este Insumo?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarinsumoIniciativa();
                }
                return false;
            }
        );
    });

    function salvarinsumoIniciativa() {
        var caminho = window.location.href;
        var action = '&requisicao=salvarItensComposicaoFormacao&identificador=insumoIniciativa&' + $("#forminsumoIniciativa").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = $.parseJSON(resp);
                
                if (!isNaN(parseInt(res.resp)) && isFinite(res.resp)) {
                   $("#modal_insumosIniciativa").modal("hide");
                   $("#insumosIniciativa").html(res.lista);
                    swal.close();
                    return true;
                }
                swal.close();
                return false;
            }
        });
        return false;
    }
    function editarInsumosIniciativa(id) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        var caminho = window.location.href;
        var action = '&requisicao=editarItensComposicaoFormacao&identificador=insumoIniciativa&ipiinid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal-insumosIniciativa").find("input").val("");
                $("#conteudo-modal-insumosIniciativa").html(resp);
                $("#modal_tituloInsumoIniciativa").html("Insumos da Iniciativa - Editar");
                $('.control-label').removeClass('col-lg-3');
                $("#modal_insumosIniciativa").modal("show");
                $("#ipiindescricao").select();
            }
        });
    }


    function removerInsumo(id) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        swal({
            title: "Remover Insumo",
            text: "Tem certeza que deseja remover?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var caminho = window.location.href;
                var action = '&requisicao=removerItensComposicaoFormacao&identificador=insumoIniciativa&ipiinid=' + id;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        var res = $.parseJSON(resp);
                        if (res.resp != 'erro') {
                            $("#insumosIniciativa").html(res.lista);
                            swal.close();
                            return true;
                        }
                        swal("Erro.", "Ocorreu um erro ao remover os Insumos .", "error");
                    }
                });
            }
        })
    }

    //---------Atividade Etapas-------------------//

    $("#btn-incluir-AtividadeEtapas").click(function (evt) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        var caminho = window.location.href;
        var action = '&requisicao=editarItensComposicaoFormacao&identificador=atividadeEtapas&ipiaid=';
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal-atividadeEtapas").find("input").val("");
                $("#conteudo-modal-atividadeEtapas").html(resp);
                $("#modal_tituloAtividadeEtapa").html("Atividades das Etapas - Incluir");
                $('.control-label').removeClass('col-lg-3');
                $("#modal_atividadeEtapas").modal("show");

            }
        });

    });

    jQuery('#salvarAtividadeEtapas').click(function (evt) {
        evt.preventDefault();
        var ipiaid = $('#conteudo-modal-atividadeEtapas').find('[name=ipiaid]').val();
        var msg = 'Incluir';
        if (ipiaid) {
            msg = 'Editar';
        }
        swal({
                title: "Confirmar",
                text: "Deseja <b>" + msg + "</b> Esta Atividade?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarAtividadeEtapa();
                }
                return false;
            }
        );
    });


    function salvarAtividadeEtapa() {
        var caminho = window.location.href;
        var action = '&requisicao=salvarItensComposicaoFormacao&identificador=atividadeEtapas&' + $("#formatividadeetapas").serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            success: function (resp) {
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res.resp)) && isFinite(res.resp)) {
                   $("#modal_atividadeEtapas").modal("hide");
                   if(res.totalCargaHoraria){
                        $("#totalCargaHoraria").html('Carga horária total: '+res.totalCargaHoraria);
                    }else{
                        $("#totalCargaHoraria").html('Carga horária total: 0');
                    }
                   $("#atividadeEtapaListagem").html(res.lista);
                    swal.close();
                    return true;
                }
                swal("Erro.", "Informe uma Atividade.", "error");
                return false;
            }
        });
        return false;
    }

    function editarAtividadeEtapas(id) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        var caminho = window.location.href;
        var action = '&requisicao=editarItensComposicaoFormacao&identificador=atividadeEtapas&ipiaid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal-atividadeEtapas").find("input").val("");
                $("#conteudo-modal-atividadeEtapas").html(resp);                
                $("#modal_tituloAtividadeEtapa").html("Atividades das Etapas - Editar");
                $('.control-label').removeClass('col-lg-3');
                $("#modal_atividadeEtapas").modal("show");
            }
        });
    }

    function removeratividadeEtapas(id) {
        if(document.getElementById('estadoIniciativa').value == true ){
            swal("Ação não Permitida!","A Iniciativa Não Está Em Cadastramento", "error");
            return false;
        }
        swal({
            title: "Remover Atividade da Etapa",
            text: "Tem certeza que deseja remover?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var caminho = window.location.href;
                var action = '&requisicao=removerItensComposicaoFormacao&identificador=atividadeEtapas&ipiaid=' + id;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        var res = $.parseJSON(resp);
                        if (res.resp != 'erro') {
                            $("#atividadeEtapaListagem").html(res.lista);
                            if(res.totalCargaHoraria){
                                $("#totalCargaHoraria").html('Carga horária total: '+res.totalCargaHoraria);
                            }else{
                                $("#totalCargaHoraria").html('Carga horária total: 0');
                            }
                            swal.close();
                            return true;
                        }
                        
                        swal("Erro.", "Ocorreu um erro ao remover as Atividades.", "error");
                    }
                });
            }
        })
    }

</script>
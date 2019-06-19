<?php

//include_once APPRAIZ . 'par3/classes/controller/IniciativaProgramasController.class.inc';

$controllerIniciativaPlanoInterno = new Par3_Controller_IniciativaPlanoInternoController();


function retornaRelatorio($arrPost)
{
    $controllerIniciativaPlanoInterno = new Par3_Controller_IniciativaPlanoInternoController();
    $controllerIniciativaPlanoInterno->listar($arrPost);
}

$programa = new Par3_Model_IniciativaProgramas();

switch ($_REQUEST['requisicao']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerIniciativaPlanoInterno->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerIniciativaPlanoInterno->listar($_REQUEST);
        die;
        break;
    case 'editar':
        ob_clean();
        echo $controllerIniciativaPlanoInterno->form($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        $resp = $controllerIniciativaPlanoInterno->remover($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerIniciativaPlanoInterno->listar($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerIniciativaPlanoInterno->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Plano_Interno.xls");
        die;
        break;
    case 'atualizarSituacao':
        ob_clean(); 
        $resp = $controllerIniciativaPlanoInterno->alterarSituacao($_REQUEST);
        if($resp){
            echo "true";
        }else{
            echo "false";
        }
        die;
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerIniciativaPlanoInterno->recuperar();
        die;
        break;
    case 'recuperarCompleto':
        ob_clean();
        $resp = $controllerIniciativaPlanoInterno->recuperarCompleto($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'editarPlano':
        ob_clean();
        $resp = $controllerIniciativaPlanoInterno->salvar($_REQUEST, true);
        echo simec_json_encode($resp);
        die;
        break;
    case 'imprimir':
        ob_clean();
        //retornaRelatorio($_REQUEST);
        $controllerIniciativaPlanoInterno->listar($_REQUEST);
        die();
        break;
    default:
        break;
}
?>
<style>
    ::-webkit-input-placeholder{
        text-align: center;
    }    
    .switchery {
    background-color: #ed5565;
    }
    .navbar-listagem {
        margin-top: 0px;
        float: right;
        width: 500px;
    }
}
</style>

<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Plano Interno</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="formPlano" id="formPlano" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-7 center">
                        <input id="inplintid" name="inplintid" value="" type="hidden"></input>
                        <?php echo $simec->select('prgPlano', 'Programa', null, $programa->pegarSQLSelectComboSigla(), array('maxlength' => '255','placeholder'=>'Selecione o Programa', 'required')); ?> 
                        <?php echo $simec->input('ptres', 'PTRES', null,  array('maxlength' => '11', 'required','placeholder'=>'Informe o PTRES')); ?> 
                        <?php echo $simec->input('planoIntern', 'Plano Interno', null, array('required','placeholder'=>'Informe o Plano Interno', 'maxlength' => '11')); ?> 
                        <?php echo $simec->select('ano', 'Ano', null, pegaAnoAFrente(5), array('maxlength' => '255', 'required', 'placeholder'=>'Selecione o Ano')); ?>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar-iniciativa"><i class="fa fa-search"></i> Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir">
                    <i class="fa fa-save"></i> Salvar
                </button> |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel
                </button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir
                </button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar
                </button>

            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="programaListagem">
            <?php
            $controllerIniciativaPlanoInterno->listar($_REQUEST);
            ?>
        </div>
    </div>
</div>


<div id="div-impressao" style="display: none">
    <div id="div_cabecalho" class="row col-lg-offset-2">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Relatório Plano Interno<br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                    Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>
    <div class="clearfix" style="margin-bottom:10px;"></div>
    <div id="impressao-content">
    </div>
</div>
<br>

<script>

    var caminho = '<?php echo $urlGuia;?>';

    $(".js-switch").on("change", function (){
        
        var id = $(this).data('id');
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=atualizarSituacao&inplintid='+id;
        $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    if(resp == 'true'){
                        swal("Sucesso", "Situação alterada com sucesso","success");
                    }else{
                        swal("Erro.", "error");
                    }
                }
            });
    });

        //confirmação ao salvar programa
        jQuery('#salvarPrograma').click(function (evt) {
            evt.preventDefault();
               msg = 'Alterar';
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>"+msg+"</b> este Plano Interno?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if(isConfirm){
                        salvarPrograma();
                    }
                    return false;
                }
            );

        });
    

    $("#btn-incluir").on("click", function () {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=salvar&' + $("#formPlano").serialize(); //console.log(action);
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) { 
                    var res = $.parseJSON(resp);
                    if (!isNaN(parseInt(res)) && isFinite(res)) {
                        msgSuccess(caminho,"Plano Interno salvo com sucesso");
                        return false;
                    }
                    if(res.status == 0){
                         swal({
                                title: "Error",
                                text: "Já existe um Plano Interno com as informações solicitadas.",
                                type: "error",
                                html: true,
                                showCancelButton: false,
                                confirmButtonColor: "#DD6B55",
                                closeOnConfirm: true
                        });   
                        return false;
                    }
                    if(res.status == 1){
                        swal({
                            html: true,
                            title: "Editar Plano Interno",
                            text: "Esse Plano Interno esta vinculado com uma Fonte de Recurso ou a Análise, tem certeza que deseja edita-lo?",
                            type: "warning",
                            showCancelButton: true,
                            confirmButtonColor: "#DD6B55",
                            cancelButtonText: "Não",
                            confirmButtonText: "Sim",
                            closeOnConfirm: false
                        }, function (isConfirm) {
                                if (isConfirm) {
                                    var caminho = '<?php echo $urlGuia;?>';
                                    var action = '&requisicao=editarPlano&' + $("#formPlano").serialize();
                                    $.ajax({
                                        type: "POST",
                                        url: caminho,
                                        data: action,
                                        async: false,
                                        success: function (resp) { 
                                            res = JSON.parse(resp);
                                                msgSuccess(caminho,"Plano Interno editado com sucesso");
                                                return false;
                                        }
                                    });
                                }
                          }

                          ); return false;
                    }
                    swal.close();
                        var res = $.parseJSON(resp);
                        $.each(res,function(index,erro){//retorna mensagens de erro em json
                            $.each(erro,function(err,errMsg){
                                if(errMsg < err.length){
                                    return;//se a mensagem for vazia não retorna nada
                                }
                                var divFormInput =  $("#formPlano").find('[name="'+err+'"]').parent("div");
                                divFormInput.closest('.form-group').addClass('has-error');
                                divFormInput.append("<div id='div-"+err+"'></div>");
                                $("#div-"+err).html("<span class='danger'>"+errMsg+"</span>");
                            });
                        });
                    
                }
            });
    });

    $(".xls").on("click", function () {
        window.location.assign(caminho + "&requisicao=xls&" + jQuery('#formPlano').serialize()+'&'+$('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=imprimir&' + $("#formPlano").serialize()+'&'+$('.form-listagem').serialize();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#impressao-content").html(resp);
                var container =  $("#impressao-content");
                var table = container.find('table');
                var th = container.find('th');
                var td = container.find('td');
                table.css('width','100%')
                table.css('border-collapse','collapse');
                container.find(".popover,button,.modal").remove();
                container.find("table,td,th").css('border','1px solid black');
                container.find('tr:even').css('background-color', 'silver');
                th.css('height','50px');
                td.css('text-align','center');
                    var w = window.open();
                    w.document.write($('#div-impressao').html());
                    w.document.title = 'Relatório - Plano Interno';
                    w.print();
                    w.close();
                $("#impressao-content").html("");
               // atualizarListagem($("#formPlano").serialize()+'&'+$('.form-listagem').serialize());
                return true;
            }
        });
    });

    function editar(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=recuperar&inplintid=' + id;
        $.ajax({
           type: "GET",
           url: caminho,
           data: action,
           async: false,
           success: function (resp) {
               var respt = JSON.parse(resp); 
               $("#prgPlano").val(respt.inplinprog).trigger("chosen:updated");
               $('#ptres').val(trim(respt.inplintptres));
               $('#planoIntern').val(trim(respt.inplintdsc));
               $("#ano").val(respt.inplintano).trigger("chosen:updated");
               $("#inplintid").val(id);

           }
       });
    }

    $("#pesquisar-iniciativa").click(function (evt) {
        pesquisarCampo();
    });

    function pesquisarCampo() {
        var caminho = window.location.href; console.log($("#formPlano").serialize());
        var action = 'requisicao=pesquisar&' + $("#formPlano").serialize();
       $.ajax({
           type: "GET",
           url: caminho,
           data: action,
           async: false,
           success: function (resp) {
               $(".form-listagem").html(resp);
               return true;
           }
       });
    }


    function js_RemoverPlanoInterno(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&requisicao=recuperarCompleto&inplintid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {  
                //console.log(resp);
                var prg = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    html: true,
                    title: "Remover Plano Interno",
                    text: "Tem certeza que deseja remover o Plano Interno: <strong>" + prg.inplintid + " - " + prg.prgdsc + " - " + prg.inplintdsc + "?</strong>",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = '<?php echo $urlGuia;?>';
                        var action = '&requisicao=remover&inplintid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) { 
                                res = JSON.parse(resp);
                                if(res.status == 0){
                                    // Se existir o erro sobre a iniciativa ter sido utilizada no sistema (em município), exibe modal informando:
                                   
                                    /*else if (resp['erros']['existemIniciativasRelacionadas'] !== undefined){
                                        mensagem = "Não é possível excluir o Programa <b>" + prg.prgid + " - " + prg.prgdsc + "</b> pois o mesmo já foi utilizado em iniciativa";
                                    }else {
                                        // Se foi outro erro, exibe informação genérica de erro:
                                        mensagem = "Não foi possível excluir a Iniciativa";
                                    }*/
                                    
                                    swal({
                                        title: "Error",
                                        text: res.mensagem,
                                        type: "error",
                                        html: true,
                                        showCancelButton: false,
                                        confirmButtonColor: "#DD6B55",
                                        closeOnConfirm: true
                                    });
                                    
                                    return false;
                                }else{
                                    msgSuccess(caminho,"Plano Interno removido com sucesso");
                                    return false;
                                }
                            }
                        });
                    }
                })
            }
        });
    }

    jQuery("#limpar").click(function(){
        $('#ptres').val('');
        $('#planoIntern').val('');
        $('#inplintid').val('');
        $("#prgPlano").val('').trigger("chosen:updated");
        $("#ano").val('').trigger("chosen:updated");
        pesquisarCampo();
    });

    /*function atualizarListagem(params ="") {
        if(params){
            params = '&'+params
        }
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarListagem'+params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#programaListagem").html(resp);
                return true;
            }
        });
    }*/

</script>
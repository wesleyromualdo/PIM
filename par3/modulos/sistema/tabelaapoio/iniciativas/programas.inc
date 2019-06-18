<?php

include_once APPRAIZ . 'par3/classes/controller/IniciativaProgramasController.class.inc';

$controllerIniciativaProgramas = new Par3_Controller_IniciativaProgramasController();


function retornaRelatorio($arrPost)
{
    $controllerIniciativaProgramas = new Par3_Controller_IniciativaProgramasController();
    $controllerIniciativaProgramas->listar($arrPost);
}



switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerIniciativaProgramas->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerIniciativaProgramas->listar($_REQUEST);
        die;
        break;
    case 'editar':
        ob_clean();
        echo $controllerIniciativaProgramas->form($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        $resp = $controllerIniciativaProgramas->remover($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerIniciativaProgramas->listar($_REQUEST);
        die;
        break;
    case 'atualizarForm':
        ob_clean();
        echo $controllerIniciativaProgramas->form($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerIniciativaProgramas->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Programa.xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    case 'upload':
        ob_clean();
        if (isset($_FILES)) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file = new FilesSimec("programas", array('prgid' => $_REQUEST['prgid']), "par3");
            $file->setUpload('Imagem', null, false);
            $arqid = $file->getIdArquivo();
            if(is_numeric($arqid) && $arqid > 0 ){
                echo simec_json_encode(array('success' => 'true', 'arqid' => $arqid));
            }else{
                echo simec_json_encode(array('success' => 'false', 'error' => 'Falha ao gravar o arquivo'));
            }
        }else{
            echo simec_json_encode(array('success' => 'false', 'error' => 'Arquivo não enviado.'));
        }
        die;
        break;
    case 'recuperar':
        ob_clean();
        echo $controllerIniciativaProgramas->recuperar();
        die;
        break;
    case 'verificaRemoverPrograma':
        ob_clean();
        if(isset($_POST['prgid'])){
            $verificaExclusao =  $controllerIniciativaProgramas->verificaLigacaoPlanejamentoById($_POST['prgid']);
            if(isset($verificaExclusao[0])){
                $getIniciativaPrograma = $controllerIniciativaProgramas->getIniciativaProgramaById($_POST['prgid']);
                echo "Não é possível excluir o Programa ".$getIniciativaPrograma['prgid']." - ".$getIniciativaPrograma['prgdsc'].", pois o mesmo já foi utilizado no planejamento";
            }else{
                echo "false";
            }
        }
        die;
        break;
    default:
        break;
}
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>

<?php //echo $simec->title('Tabela Apoio - Programas', 'Sistema / Tabela de Apoio / Programas'); ?>
<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Programas</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Programa:</label>
                            <div class="col-sm-7">
                                <input type="text" name="prgdsc" id="prgdsc" maxlength="255" value="<?php echo ($_GET['prgdsc']);?>" class="form-control"/>
                            </div>
                        </div>
                        <!--filtros-->
                            <div id="avancado" style="display: none;">
                                <div class="form-group">
                                    <label class="col-sm-3 control-label">Sigla:</label>
                                    <div class="col-sm-7">
                                        <input type="text" name="prgabreviatura" id="prgabreviatura" class="form-control" value="<?php echo ($_GET['prgabreviatura']); ?>"/>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="col-sm-3 control-label">Código:</label>
                                    <div class="col-sm-7">
                                        <input type="number" name="prgid" id="prgid" class="form-control" value="<?php echo $_GET['prgid']; ?>"/>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-3 control-label">Atendimento Extraordinário:</label>
                                    <div class="col-md-7" style="text-align: left">
                                        <div class="radio radio-default radio-inline ">
                                            <input type="radio" name="prgatd_extraordinario" id="prgatd_extraordinario1" <?php echo ($_GET['prgatd_extraordinario'] == 't'?'checked':'');?> value="t">
                                            <label for="prgatd_extraordinario1"> Sim </label>
                                        </div>
                                        <div class="radio radio-default radio-inline ">
                                            <input type="radio" name="prgatd_extraordinario" id="prgatd_extraordinario2" <?php echo ($_GET['prgatd_extraordinario'] == 'f'?'checked':'');?> value="f">
                                            <label for="prgatd_extraordinario2"> Não </label>
                                        </div>
                                        <div class="radio radio-default radio-inline">
                                            <input type="radio" name="prgatd_extraordinario" id="prgatd_extraordinario3" <?php echo ($_GET['prgatd_extraordinario'] == ''?'checked':'');?> value="">
                                            <label for="prgatd_extraordinario3"> Todos </label>
                                        </div>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-3 control-label">Situação:</label>
                                    <div class="col-md-7" style="text-align: left">
                                        <div class="radio radio-default radio-inline ">
                                            <input type="radio" name="prgsituacao" id="prgsituacao1" <?php echo ($_GET['prgsituacao'] == ''?'checked':'');?> value="A">
                                            <label for="prgsituacao1"> Ativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline ">
                                            <input type="radio" name="prgsituacao" id="prgsituacao2" <?php echo ($_GET['prgsituacao'] == ''?'checked':'');?> value="I">
                                            <label for="prgsituacao2"> Inativo </label>
                                        </div>
                                        <div class="radio radio-default radio-inline">
                                            <input type="radio" name="prgsituacao" id="prgsituacao3" <?php echo ($_GET['prgsituacao'] == ''?'checked':'');?> value="">
                                            <label for="prgsituacao3"> Todos </label>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="ibox-title">
                                <div class="ibox-tools">
                                    <a id="btn-avancado">
                                        Opções Avançadas <i id="chevron" class="fa fa-chevron-down"></i>
                                    </a>
                                </div>
                            </div>
                        <br>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar-iniciativa"><i class="fa fa-search"></i> Pesquisar
                </button>
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal" data-target="#modal_programa">
                    <i class="fa fa-plus"></i> Incluir
                </button> |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel
                </button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Gerar PDF
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
            $controllerIniciativaProgramas->listar($_REQUEST);
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
                    Relatório Programas<br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?=$_SESSION['usunome'];?></b><br/>
                    Hora da Impressão: <?=date( 'd/m/Y - H:i:s' );?><br />
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

<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_programa" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="form" id="form" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Programa - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                        <?php echo $controllerIniciativaProgramas->form($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal" class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarPrograma"
                                class="btn btn-success" <?php echo $disabled; ?>>
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<script>



    var caminho = '<?php echo $urlGuia;?>';

    jQuery(function () {
        //mostra/esconde ibox
        $("#btn-avancado").click(function () {
            $('#avancado').slideToggle();
            //chevron up/down
            $('#chevron').toggleClass(function () {
                if ($(this).is(".fa-chevron-down")) {
                    $('#chevron').removeClass('fa-chevron-down');
                    return 'fa-chevron-up';
                } else {
                    $('#chevron').removeClass('fa-chevron-up');
                    return 'fa-chevron-down';
                }
            });
        });

        jQuery('#form').on('click',"put[name=prgsituacao ][value=I]",function(){
            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
        });

        //confirmação ao salvar programa
        jQuery('#salvarPrograma').click(function (evt) {
            evt.preventDefault();
            var prgid = $('#conteudo-modal').find('[name=prgid]').val();
            var msg = 'Incluir';
            if(prgid){
               msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>"+msg+"</b> este programa?",
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
    });

    function salvarPrograma()
    {
        var atendimento_extraordinario = $('[name=prgatd_extraordinario]').val();
        console.log(atendimento_extraordinario);
        var caminho = '<?php echo $urlGuia;?>';
        $('#form').isValid(function (isValid) {
            if (isValid) {
                var action = '&req=salvar&' + $("#form").serialize();
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        var res = $.parseJSON(resp);
                        if (!isNaN(parseInt(res)) && isFinite(res)) {
                           if(verificarAtendimento(res)){
                               var caminho_atendimento_extraordinario = caminho+"&submenu=atendimento_extraordinario&prgid=" + res;
                               msgSuccess(caminho_atendimento_extraordinario,"Programa salvo com sucesso");
                               return false;
                           }
                            $("#modal_programa").modal("hide");
                            $("#modal_programa").find("input").val("");
                            msgSuccess(caminho,"Programa salvo com sucesso");
                            return false;
                        }
                        swal.close();
                        $.each(res,function(index,erro){//retorna mensagens de erro em json
                            $.each(erro,function(err,errMsg){
                                if(errMsg < err.length){
                                    return;//se a mensagem for vazia não retorna nada
                                }
                                var divFormInput =  $("#form").find("input[name="+err+"]").parent("div");
                                divFormInput.closest('.form-group').addClass('has-error');
                                divFormInput.append("<div id='div-"+err+"'></div>");
                                $("#div-"+err).html("<span class='danger'>"+errMsg+"</span>");
                            });
                        });
                        return false;
                    }
                });
            }
            return false;
        });
    }

    $(".xls").on("click", function () {
        window.location.assign(caminho + "&req=xls&" + jQuery('#form-filtro').serialize()+'&'+$('.form-listagem').serialize());
    });

    $("#imprimir").on("click", function () {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=imprimir&' + $("#form-filtro").serialize()+'&'+$('.form-listagem').serialize();
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
                    w.document.title = 'Relatório - Programas';
                    w.print();
                    w.close();
                $("#impressao-content").html("");
                atualizarListagem($("#form-filtro").serialize()+'&'+$('.form-listagem').serialize());
                return true;
            }
        });
    });

    $("#btn-incluir").click(function (evt) {
        evt.preventDefault();
        atualizarForm();
        $("#modal_programa").modal("show");
    });

    function editar(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&prgid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var prg = $.parseJSON(resp);
                //verificar se é atendimento extraordinário
                if(prg.prgatd_extraordinario === 'f'){
                    var caminho = '<?php echo $urlGuia;?>';
                   var action = '&req=editar&prgid=' + id;
                   $.ajax({
                       type: "POST",
                       url: caminho,
                       data: action,
                       async: false,
                       success: function (resp) {
                           $("#conteudo-modal").find("input").val("");
                           $("#conteudo-modal").html(resp);
                           $("#modal_titulo").html("Programas - Editar");
                           $("#modal_programa").modal("show");
                       }
                   });
               }else{
                    window.location.assign('<?php echo $urlGuia;?>' + "&submenu=atendimento_extraordinario&prgid=" + prg.prgid);
               }
            }
        });
    }

    $("#modal_programa").on("hidden.bs.modal",function(){
        $("#modal_titulo").html("Programas - Incluir");
    });

    function atualizarListagem(params ="") {
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
    }

    function formAtendimentoExtraordinario(id)
    {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=formAtendimentoExtraordinario&prgid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#formAtendimentoExtraordinario").html(resp);
                $("#formAtendimentoExtraordinario").fadeIn();
            }
        });
    }

    $("#pesquisar-iniciativa").click(function (evt) {
        window.location.assign(caminho+'&'+$("#form-filtro").serialize());
//        evt.preventDefault();
//        var caminho = '<?php //echo $urlGuia;?>//';
//        var action = '&req=pesquisar&' + $("#form-filtro").serialize();
//        $.ajax({
//            type: "POST",
//            url: caminho,
//            data: action,
//            async: false,
//            success: function (resp) {
//                $("#programaListagem").html(resp);
//                return true;
//            }
//        });
    });

    function remover(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&prgid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var prg = $.parseJSON(resp);
                var confirmado = false;
                swal({
                    html: true,
                    title: "Remover Programa",
                    text: "Tem certeza que deseja remover: <strong>" + prg.prgid + " - " + prg.prgdsc + "?</strong>",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = '<?php echo $urlGuia;?>';
                        var action = '&req=remover&prgid=' + id;
                        $.ajax({
                            type: "POST",
                            url: caminho,
                            data: action,
                            async: false,
                            success: function (resp) {
                            	console.log('antes parse, stringify:'); console.log(JSON.stringify(resp));
                            	//console.log('antes parse, join:'); console.log(resp.join('\n'));
                            	resp = JSON.parse(resp);
								console.log('resp:'); console.log(resp);
                            	if(resp['erros'] !== undefined){
									// Se existir o erro sobre a iniciativa ter sido utilizada no sistema (em município), exibe modal informando:
									if(resp['erros']['existeIniciativaRelacionada'] !== undefined){
										iniciativa = resp['erros']['existeIniciativaRelacionada'][0];
                                    	mensagem = "Não é possível excluir o Programa <b>" + prg.prgid + " - " + prg.prgdsc + "</b> pois o mesmo já foi utilizado na iniciativa <b>" + iniciativa.descricao +"</b>";
									} else if (resp['erros']['existemIniciativasRelacionadas'] !== undefined){
                                    	mensagem = "Não é possível excluir o Programa <b>" + prg.prgid + " - " + prg.prgdsc + "</b> pois o mesmo já foi utilizado em iniciativa";
                                	}else if(resp['erros']['existeAreaRelacionada'] !== undefined){
                                    	area = resp['erros']['existeAreaRelacionada'][0];
                                    	mensagem = "Não é possível excluir o Programa <b>" + prg.prgid + " - " + prg.prgdsc + "</b> pois o mesmo já foi vinculado em Área Relacionada  <b>" + area.iardsc +"</b>";
                                	} else if(resp['erros']['existemAreasRelacionadas'] !== undefined) {
                                		mensagem = "Não é possível excluir o Programa <b>" + prg.prgid + " - " + prg.prgdsc + "</b> pois o mesmo já foi vinculado em Áreas Relacionadas";
                                    } else {
    									// Se foi outro erro, exibe informação genérica de erro:
                                    	mensagem = "Não foi possível excluir a Iniciativa";
                                    }
                                    
                                    swal({
				                    	title: "Error",
			                            text: mensagem,
			                            type: "error",
			                            html: true,
			                            showCancelButton: false,
			                            confirmButtonColor: "#DD6B55",
			                            closeOnConfirm: true
									});
									
                                	return false;
                                // Se não ocorreu erro, exibe mensagem de sucesso:
                            	} else {
                                    msgSuccess(caminho,"Programa removido com sucesso");
                                    return false;
                                }
                            }
                        });
                    }
                })
            }
        });
    }

    function atualizarForm(params = "") {
        if(params){
            params = '&'+params
        }

        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=atualizarForm'+params;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").html(resp);
                return true;
            }
        });
    }

    function verificarAtendimento(id){
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&prgid=' + id;
        var tipoAtendimento = '';
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var prg = $.parseJSON(resp);
                if(prg.prgatd_extraordinario == 't'){
                    tipoAtendimento = true;
                }else{
                    tipoAtendimento = false;
                }
            }
        });
        return tipoAtendimento;
    }
    function atualizarFormFiltro()
    {
        $("#form-filtro").find('[name=prgdsc]').val("");
        $("#form-filtro").find('[name=prgid]').val("");
        $("#form-filtro").find('[name=prgsigla]').val("");
        $("#form-filtro").find('#prgsituacao1').removeAttr("checked");
        $("#form-filtro").find('#prgsituacao2').removeAttr("checked");
        $("#form-filtro").find('#prgsituacao3').prop('checked',true);
        $("#form-filtro").find('#prgatd_extraordinario1').removeAttr("checked");
        $("#form-filtro").find('#prgatd_extraordinario2').removeAttr("checked");
        $("#form-filtro").find('#prgatd_extraordinario3').prop('checked',true);
    }

    jQuery("#limpar").click(function(){
        window.location.assign(caminho);
        $('#prgdsc').val('');
        $('#prgabreviatura').val('');
        $('#prgid').val('');

        $("#form-filtro").find('#prgatd_extraordinario1').removeAttr("checked");
        $("#form-filtro").find('#prgatd_extraordinario2').removeAttr("checked");
        $("#form-filtro").find('#prgatd_extraordinario3').prop('checked',true);

        $("#form-filtro").find('#prgsituacao1').removeAttr("checked");
        $("#form-filtro").find('#prgsituacao2').removeAttr("checked");
        $("#form-filtro").find('#prgsituacao3').prop('checked',true);

    });


    /**
     * @author Leo Kenzley <leo.oliveira@cast.com.br>
     * @param desid
     * @description <esta função é utilizada para verificar se o programa já está ligado ao planejamento, inviabilizando sua exclusão caso exista a ligação>
     */
    function js_verificaRemoverPrograma(prgid){
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=verificaRemoverPrograma&prgid='+prgid ;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
//                alert(resp);
                if(resp != 'false'){
                    swal("Erro.", resp, "error");
                }else{
                   remover(prgid);
                }
            }
        });
    }
</script>
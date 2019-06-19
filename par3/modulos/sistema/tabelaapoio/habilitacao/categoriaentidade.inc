<?php
include_once APPRAIZ . 'par3/classes/controller/HabilitacaoCategoriaEntidade.class.inc';

$controllerCategoriaEntidade = new Par3_Controller_CategoriaEntidadeHabilita();


function retornaRelatorio($arrPost)
{
    $controllerCategoriaEntidade = new Par3_Controller_CategoriaEntidadeHabilita();
    $controllerCategoriaEntidade->listar($arrPost);
}



switch ($_REQUEST['req']) {
    case 'salvar':
        ob_clean();
        $resp = $controllerCategoriaEntidade->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagem':
        ob_clean();
        echo $controllerCategoriaEntidade->listar($_REQUEST);
        die;
        break;
    case 'editar':
        ob_clean();
        echo $controllerCategoriaEntidade->form($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        $resp = $controllerCategoriaEntidade->inativar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'pesquisar':
        ob_clean();
        echo $controllerCategoriaEntidade->listar($_REQUEST);
        die;
        break;
    case 'atualizarForm':
        ob_clean();
        echo $controllerCategoriaEntidade->form($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $controllerCategoriaEntidade->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Categoria_Entidades.xls");
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
        echo $controllerCategoriaEntidade->recuperar();
        die;
        break;
    case 'verificaRemoverCategoriaEntidade':
        ob_clean();
        if(isset($_POST['cteid'])){
            $verificaExclusao =  $controllerCategoriaEntidade->verificaLigacaoPlanejamentoById($_POST['prgid']);
            if(isset($verificaExclusao[0])){
                $getIniciativaPrograma = $controllerCategoriaEntidade->getIniciativaProgramaById($_POST['prgid']);
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
                <h3 class="center">Categoria de Entidades</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Tipo:</label>
                            <div class="col-sm-7">
                                <input type="text" name="ctenome" id="ctenome" maxlength="80" value="<?php echo ($_GET['ctenome']);?>" class="form-control"/>
                            </div>
                        </div>
                        <!--filtros-->
                            <div id="avancado" style="display: none;">
                                <div class="form-group">
                                    <label class="col-sm-3 control-label">Código:</label>
                                    <div class="col-sm-7">
                                        <input type="number" name="cteid" id="cteid" class="form-control" value="<?php echo $_GET['cteid']; ?>"/>
                                    </div>
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
                <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal" data-target="#modal_categoria_entidades">
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
            $controllerCategoriaEntidade->listar($_REQUEST);
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
                    Relatório de Tipos de Naturezas<br>
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
<div class="ibox float-e-margins animated modal" id="modal_categoria_entidades" tabindex="-1" role="dialog"
     aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="form" id="form" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Categoria de Entidades - Incluir</h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                        <?php echo $controllerCategoriaEntidade->form($_REQUEST); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarCategoriaEntidade" data-dismiss="modal" class="btn btn-default" <?php echo $disabled; ?>>
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="submit" id="salvarCategoriaEntidade"
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

//        jQuery('#form').on('click',"put[name=prgsituacao ][value=I]",function(){
//            swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará mais visível", "warning");
//        });

        //confirmação ao salvar Categoria de Entidades
        jQuery('#salvarCategoriaEntidade').click(function (evt) {
            evt.preventDefault();
            var cteid = $('#conteudo-modal').find('[name=cteid]').val();
            var msg = 'Incluir';
            if(cteid){
               msg = 'Alterar';
            }
            swal({
                    title: "Confirmar",
                    text: "Deseja <b>"+msg+"</b> este tipo de Categoria de Entidade?",
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if(isConfirm){
                        salvarCategoriaEntidade();
                    }
                    return false;
                }
            );

        });
    });

    function salvarCategoriaEntidade()
    {
        var tpehabilitacao = $('[name=tpehabilitacao]').val();
        console.log(tpehabilitacao);
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
                       //alert(resp); return false;
                        var res = $.parseJSON(resp);
                        if (!isNaN(parseInt(res)) && isFinite(res)) {
                           if(verificarAtendimento(res)){
                               var caminho_atendimento_extraordinario = caminho+"&submenu=tpehabilitacao&cteid=" + res;
                               msgSuccess(caminho_atendimento_extraordinario,"Categoria salvo com sucesso");
                               return false;
                           }
                            $("#modal_categoria_entidades").modal("hide");
                            $("#modal_categoria_entidades").find("input").val("");
                            msgSuccess(caminho,"Categoria salvo com sucesso");
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
        $("#modal_categoria_entidades").modal("show");
    });

    function editar(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&cteid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var prg = $.parseJSON(resp);
                //verificar se é atendimento extraordinário
        //        if(prg.prgatd_extraordinario === 'f'){
                    var caminho = '<?php echo $urlGuia;?>';
                   var action = '&req=editar&cteid=' + id;
                   $.ajax({
                       type: "POST",
                       url: caminho,
                       data: action,
                       async: false,
                       success: function (resp) {
                           $("#conteudo-modal").find("input").val("");
                           $("#conteudo-modal").html(resp);
                           $("#modal_titulo").html("Categoria de Entidades - Editar");
                           $("#modal_categoria_entidades").modal("show");
                       }
                   });
//               }else{
//                    window.location.assign('<?php echo $urlGuia;?>' + "&submenu=atendimento_extraordinario&prgid=" + prg.prgid);
//               }
            }
        });
    }

    $("#modal_categoria_entidades").on("hidden.bs.modal",function(){
        $("#modal_titulo").html("Categoria de Entidades - Incluir");
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
    });

    function remover(id) {
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperar&cteid=' + id;
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
                    title: "Remover Categoria",
                    text: "Tem certeza que deseja remover: <strong>" + prg.cteid + " - " + prg.ctenome + "?</strong>",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        var caminho = '<?php echo $urlGuia;?>';
                        var action = '&req=remover&cteid=' + id;
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
                                    msgSuccess(caminho,"Categoria removido com sucesso");
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
    function js_verificaRemoverCategoriaEntidade(cteid){
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=verificaRemoverCategoriaEntidade&cteid='+cteid ;
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
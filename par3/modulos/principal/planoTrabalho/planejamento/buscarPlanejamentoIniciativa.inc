<?php
$mCic = new Par3_Model_CicloPar();
$mInd = new Par3_Model_IniciativaDescricao();
$mDim = new Par3_Model_Dimensao();
$mInp = new Par3_Controller_IniciativaPlanejamento();
$url  = 'par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=' . $_REQUEST['inuid'];
$mItr = new Par3_Controller_InstrumentoUnidade();
//$Analise = new Par3_Controller_PesquisaIniciativaAnalise();

$nomeItr = $mItr->pegarNomeEntidade($_REQUEST['inuid']);
function retornaRelatorio($arrPost)
{
    $controllerIniciativaPlanejamento = new Par3_Controller_IniciativaPlanejamento();
    $controllerIniciativaPlanejamento->listar($arrPost);
}

//$arrSituacao = array();
//$arrSituacao = ['1990' => 'Em Cadastramento'];

switch ($_REQUEST['requisicao']) {
    case 'pesquisar':
        ob_clean();
        echo $mInp->listar($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $mInp->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Planejamento_Iniciativas_".$nomeItr.".xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        retornaRelatorio($_REQUEST);
        die();
        break;
    case 'recuperar':
        ob_clean();
        echo $mInp->recuperar($_REQUEST);
        die;
        break;
    case 'remover':
        ob_clean();
        echo $mInp->remover($_REQUEST);
        die;
        break;
    default:
        break;
}
?>
<style>
    .btn-info {
        background-color: #1c84c6;
        border-color: #1c84c6;
    }
    .navbar-listagem {
        margin-top: 0px;
        float: right;
        width: 500px;
    }
</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Buscar Iniciativas</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div id="div-ciclo">
                        <?php echo $simec->select('cicid', 'Ciclo', '', $mCic->pegarSQLSelectCombo(), array('maxlength' => '255'), array('input-size' => '7')); ?>
                        </div>
                        <!--filtros-->
                        <div class="ibox float-e-margins">
                            <div class="ibox-content" id="div-avancado" style="display: none;">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
                                    <div class="col-md-5  col-md-5 col-lg-5">
                                        <input type="number" name="codigo" id="codigo" class="form-control"/>
                                    </div>
                                </div>
                               <div id="div-dimensao">
                                   <?php echo $simec->select('dimid', 'Dimensão',$inp->dimid,$mDim->listarSelect(array('itrid' => $mItr->pegarItrid($_GET['inuid']))), array('maxlength' => '255'),array('input-size' => '7')); ?>
                               </div>
                                <div id="div-descricao">
                                    <?php echo $simec->input('inddsc', 'Descrição', '', array('maxlength' => '255','placeholder' => 'Informe Descrição da Iniciativa'), array('input-size' => '7')); ?>
                                </div>
                                <div id="div-situacao">
<!--                                    --><?php //echo $simec->select('situacao', 'Situação', '', $arrSituacao, array('maxlength' => '255'), array('input-size' => '7')); ?>
                                    <?php echo $simec->select('situacao', 'Situação', $_GET['situacao'],$mInp->getEstadosIniciativas(), array('maxlength' => '255'), array('input-size' => '7')); ?>
                                </div>
                            </div>
                            <div class="ibox-title">
                                <div class="ibox-tools">
                                    <a id="btn-avancado-planejamento">Opções Avançadas <i id="icone-avancado" class="fa fa-chevron-down"></i></a>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-primary" id="pesquisar-planejamento-iniciativa"><i class="fa fa-search"></i>Pesquisar</button>
                <button type="button" class="btn btn-success" id="btn-incluir"><i class="fa fa-plus"></i> Incluir</button>
                |
                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="planejamentoIniciativaListagem" style="overflow: scroll;">
            <?php $mInp->listar($_REQUEST)?>
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
                    Relatório Descrição de Iniciativas<br>
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

 <!--modal-->
                <div class="ibox float-e-margins animated modal" id="modalListarAnalise" role="dialog" aria-hidden="true">
                    <div class="modal-dialog" style="width:1500px;">
                            <div class="modal-content">
                                <div class="ibox-title">
                                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                                                aria-hidden="true">&times;</span></button>
                                    <h3 id="modal_titulo">Histórico de Análise da Iniciativa</h3>
                                </div>
                                <div class="ibox-footer" align="left">
                                    <label class="col-sm-1 col-md-1 col-lg-1 control-label">Iniciativa: </label>
                                    <div class="">
                                         <span id="iniciativaSpan"></span>
                                    </div>
                                </div>
                                <div class="ibox-content">
                                    <div id="planejamentoIniciativaListagem">
                                        <?php//$Analise->listarAnalises($_REQUEST)?>
                                    </div>
                                </div>
                            </div>
                    </div>
                </div>
<!-- fecha modal -->     
<script type="text/javascript">	

	$("#limpar").click(function(){
	    //$('#cicid').prop('selected', 0);
	    $('#cicid').val('').trigger('chosen:updated');
	    $('#codigo').val('');
	    $('#dimid').val('').trigger('chosen:updated');
	    $('#inddsc').val('').trigger('chosen:updated');
	    $('#situacao').val('').trigger('chosen:updated');
	    $('#pesquisar-planejamento-iniciativa').click();
	});
	
	//mostra/esconde ibox
	jQuery("#btn-avancado-planejamento").click(function () {
	    $('#div-avancado').slideToggle();
	    //chevron up/down
	    $('#icone-avancado').toggleClass(function () {
	        if ($('#icone-avancado').is(".fa-chevron-down")) {
	            $('#icone-avancado').removeClass('fa-chevron-down');
	            return 'fa-chevron-up';
	        } else {
	            $('#icone-avancado').removeClass('fa-chevron-up');
	            return 'fa-chevron-down';
	        }
	    });
	});
	
	$(".xls").on("click", function () {
	    window.location.assign(window.location.href + "&requisicao=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
	});
	
	$("#imprimir").on("click", function () {
	    var caminho = window.location.href;
	    var action = '&requisicao=imprimir&' + $("#form-filtro").serialize() + '&' + $('.form-listagem').serialize();
	    $.ajax({
	        type: "POST",
	        url: caminho,
	        data: action,
	        async: false,
	        success: function (resp) {
	            $("#impressao-content").html(resp);
	            var container = $("#impressao-content");
	            var table = container.find('table');
	            var th = container.find('th');
	            var td = container.find('td');
	            table.css('width', '100%');
	            table.css('border-collapse', 'collapse');
	            container.find(".popover,button,.modal").remove();
	            container.find("table,td,th").css('border', '1px solid black');
	            container.find('tr:even').css('background-color', 'silver');
	            th.css('height', '50px');
	            td.css('text-align', 'center');
	            var w = window.open();
	            w.document.write($('#div-impressao').html());
	            w.document.title = "Relatório - Iniciativa Planejamentos - <?php echo $nomeItr; ?>";
	            w.print();
	            w.close();
	            $("#impressao-content").html("");
	            atualizarListagem($("#form-filtro").serialize() + '&' + $('.form-listagem').serialize());
	            return true;
	        }
	    });
	});
	
	$("#pesquisar-planejamento-iniciativa").click(function (evt) {
	    evt.preventDefault();
	    var caminho = window.location.href;
	    var action = '&requisicao=pesquisar&' + $("#form-filtro").serialize();
	    $.ajax({
	        type: "POST",
	        url: caminho,
	        data: action,
	        async: false,
	        success: function (resp) {
	            $("#planejamentoIniciativaListagem").html(resp);
	            return true;
	        }
	    });
	});
	
	function atualizarListagem(params = '') {
	    if (params) {
	        params = '&' + params;
	    }
	    var caminho = window.location.href;
	    var action = '&requisicao=pesquisar' + params;
	    $.ajax({
	        type: "GET",
	        url: caminho,
	        data: action,
	        async: false,
	        success: function (form) {
	            $('#planejamentoIniciativaListagem').html(form);
	            return false;
	        }
	    });
	}
	function removerIniciativaPlanejamento(id)
	{
	    var caminho = window.location.href;
	    var action = '&requisicao=recuperar&inpid=' + id;
	    $.ajax({
	        type: "GET",
	        url: caminho,
	        data: action,
	        async: true,
	        success: function (resp) {
	            var inp = $.parseJSON(resp);
	            var confirmado = false;
	            swal({
	                title: "Remover Iniciativa",
	                text: "Tem certeza que deseja remover esta Iniciativa do Planejamento?",
	                html: true,
	                type: "warning",
	                showCancelButton: true,
	                cancelButtonText: "Não",
	                confirmButtonText: "Sim",
	                closeOnConfirm: false
	            }, function (isConfirm) {
	                if (isConfirm) {
	                    var caminho = window.location.href;
	                    var action = '&requisicao=remover&inpid=' + id;
	                    $.ajax({
	                        type: "POST",
	                        url: caminho,
	                        data: action,
	                        async: false,
	                        success: function (resp) {
	                            $("#modal_modalidade").modal("hide");
	                            $("#modal_modalidade").find("input").val("");console.log(resp);
	                            if(resp == 'erroIniciativa'){
	                            	swal("Erro.", "Esta iniciativa não pode ser mais excluida!", "error");
	                            	return false;
	                            }if(resp == 'erroObras') {
	                            	swal("Erro.", "Não é possível excluir iniciativa que possui Obra tramitada.", "error");
	                            	return false;
	                            }if(resp == 'erroEmenda') {
	                            	swal("Erro.", "Não é possível excluir iniciativa pois possui indicação de emenda.", "error");
	                            	return false;
	                            }if(resp != 'sucesso') {
	                                msgSuccess(caminho,"Iniciativa removida com sucesso");
	                                return false;
	                            }
	                            swal("Erro.", "Ocorreu um erro ao remover a Iniciativa.", "error");
	                        }
	                    });
	                }
	            })
	        }
	    });
	}
	
	function editarIniciativaPlanejamento(id)
	{
	    window.location.assign("<?php echo $url; ?>&aba=cadastro&inpid="+id);
	}
	
	$('#btn-incluir').click(function(){
	    window.location = '<?php echo $url.'&aba=cadastro'; ?>' ;
	});

	function listarAnalise(id){
   		 $("#iniciativaSpan").html('('+id+') - ');
   		 $("#modalListarAnalise").modal('show');
	}


</script>
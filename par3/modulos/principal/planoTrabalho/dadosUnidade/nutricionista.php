<?php
global $simec;
$controleUnidade                      = new Par3_Controller_InstrumentoUnidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();

switch ($_REQUEST['requisicao']) {
    case 'visualisarNutricionista':
        $danid = $_REQUEST['danid'];
        $tipoId = $_REQUEST['tipo_id'];

        $controllerDadosNutricionista = new Par3_Controller_DadosNutricionista();
        ob_clean();
        $controllerDadosNutricionista->visualisarNutricionista($danid, $tipoId);
        die();
        break;
    case 'cadastrar-nutricionista':
        $controllerInstrumentoUnidadeEntidade->salvarInformacoesNutricionista($_POST);
        break;
    case 'nomearRT':
        $controllerInstrumentoUnidadeEntidade->renomearRT($_REQUEST);
        break;
    case 'formDesvincularNutricionista':
        ob_clean();
        echo simec_json_encode($controllerInstrumentoUnidadeEntidade->formDesvincularNutricionista($_REQUEST));
        die;
        break;
    case 'desvincular-nutricionista':
        $controllerInstrumentoUnidadeEntidade->desvincularNutricionista($_REQUEST);
        break;
    case 'recuperar':
        ob_clean();
        echo simec_json_encode($controllerInstrumentoUnidadeEntidade->recuperarNutricionista($_REQUEST));die;
        break;
    case 'editar':
        ob_clean();
            echo $controllerInstrumentoUnidadeEntidade->formNutricionista($_REQUEST);die;
        break;
    case 'novo':
        ob_clean();
        echo $controllerInstrumentoUnidadeEntidade->formNovoNutricionista($_REQUEST);die;
        break;
    case 'descadastrar':
        $controllerInstrumentoUnidadeEntidade->descadastrarInformacoesNutricionista($_REQUEST);
        break;
    case 'inativar':
        $vinculo = new Par3_Model_VinculacaoNutricionista();
        $vinculo->invativarVinculoNutricionista($_REQUEST['entid']);
	    $controllerInstrumentoUnidadeEntidade->inativaEntidade($_REQUEST);
	    break;
    case 'verifica_cpf':
        ob_clean();
        $duncpf = str_replace("/", "", str_replace("-", "", str_replace(".", "", $_POST['cpf'])));
        $instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $dados = $instrumentoUnidadeEntidade->recuperarNutricionistaResponsavelPorId($duncpf);

        if (!empty($dados)) {
            $dados2 = simec_json_encode($dados);
            echo $dados2;
        } else {
            $usuario = new Seguranca_Model_Usuario();
            $dados = $usuario->recuperarPorCPF($duncpf);
            if (!empty($dados)) {
                $dados = array(array('usunome' => $dados["usunome"], 'entemail' => $dados["usuemail"], 'origem' => "seguranca"));
                echo simec_json_encode($dados);
            } else {
                $resp = recuperarUsuarioReceita($duncpf);
                $dados = array(array('usunome' => $resp['dados']['no_pessoa_rf'], 'entemail' => "", 'origem' => "receita"));
                echo simec_json_encode($dados);
            }
        }
        die();
        break;
    default:
        $arrParamHistorico = array();
        $arrParamHistorico['inuid'] = $_REQUEST['inuid'];
        $arrParamHistorico['tenid'] = '7,8,16';
        $arrParamHistorico['booCPF'] = true;
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        break;
}
//Verifica se há Nutricionista Responsável, se não houver, habilita a opção promover na controller
$nutrucionistaResponsavel = $modelInstrumentoUnidadeEntidade->verificarRTNutricionista($_REQUEST['inuid']);
$_REQUEST['possuiRT'] = $nutrucionistaResponsavel['entid'];
?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3 class="pull-left" style="margin-bottom: 10px">Nutricionistas</h3>
            </div>
            <?php if($disabled == ''){ ?>
            <div class="col-md-4">
                <button class="btn btn-success novo pull-right" data-toggle="modal" data-target="#modal">
                    <i class="fa fa-plus-square-o"></i> Inserir Nutricionista
                </button>
            </div>
            <?php }?>

            <div class="col-md-2"></div>
        </div>
    </div>
    <div class="ibox-content">
        <?php $controllerInstrumentoUnidadeEntidade->recuperarNutricionistas($_REQUEST); ?>
    </div>
    <div class="ibox ">
        <div class="ibox-title">
            <h5>Equipe Local - Histórico Modificações</h5>
            <div class="ibox-tools">
                <a id="btn-show-h-nutricionista"><i id="chevron" class="fa fa-chevron-down"></i>
                </a>
            </div>
        </div>
        <div id="historico-nutricionista" style="display: none">
            <?php $controllerInstrumentoUnidadeEntidade->formHistorico($arrParamHistorico); ?>
        </div>
    </div>
    <div class="ibox-footer">
        <div class="row">
            <div class="col-lg-6 text-left">
                <button type="button" class="btn btn-success previous" >Anterior</button>
            </div>
            <div class="col-lg-6 text-right">
                <button type="button" class="btn btn-success next" >Próximo</button>
            </div>
        </div>
    </div>
</div>
<?php if($disabled == ''){ ?>

<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
		<form method="post" name="formNutricionista" id="formNutricionista" class="form form-horizontal">
            <input type="hidden" name="requisicao" value="cadastrar-nutricionista">
            <input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']?>">
            <input type="hidden" name="menu"  value="<?php echo $_REQUEST['menu']; ?>"/>
            <input type="hidden" name="requisicao" value="cadastrar-nutricionista">
	        <div class="modal-content animated flipInY">
	            <div class="ibox-title esconde " tipo="integrantes">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
	                <h3>Cadastro Nutricionistas</h3>
	            </div>
	            <div class="ibox-content" id="conteudo-modal">
	            </div>
	            <div class="ibox-footer">
            		<div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
            	    	<button type="submit" class="btn btn-success salvar" id="salvar-nutricionista" <?php echo $disabled;?>>
            	    	  <i class="fa fa-save"></i> Salvar
            	    	</button>
            		</div>
	            </div>
	        </div>
		</form>
    </div>
</div>
<?php }?>

<?php if($disabled == ''){ ?>
<div class="ibox float-e-margins animated modal" id="modal-desvincular" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formNutricionista" id="formNutricionista" class="form form-horizontal">
            <input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']?>">
            <input type="hidden" name="menu"  value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="modal-content animated flipInY">
                <div class="ibox-title esconde " tipo="integrantes">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h5 class="" id="titulo-modal"></h5>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-9 col-md-offset-9 col-lg-offset-3 col-lg-offset-3">
                        <button type="submit" class="btn btn-success salvar" <?php echo $disabled;?>>
                            <i class="fa fa-save"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>
<?php }?>

<div class="ibox float-e-margins animated modal" id="modal_detalhe" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            </div>
            <div class="ibox-content" id="conteudo-modal">
            </div>
            <div class="ibox-footer">
            </div>
        </div>
    </div>
</div>

<script>
    //mostra/esconde histórico
    $("#btn-show-h-nutricionista").click(function () {
        $('#historico-nutricionista').slideToggle();
        //chevron up/down
        $(this).find('#chevron').toggleClass(function () {
            if ($(this).is(".fa-chevron-down")) {
                $(this).removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $(this).removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    $(document).ready(function() {
        $('.previous').click(function(){
            var url = window.location.href.toString().replace('nutricionista', 'equipe');
            window.location.href = url;
        });

        $('.next').click(function(){
            var url = window.location.href.toString().replace('nutricionista', 'comite');
            window.location.href = url;
        });
    });

//        $('.ibox').on('change', 'input[name=usucpf]', function () {
//            if (!validar_cpf($(this).val())) {
//                alert("CPF inválido!\nFavor informar um cpf válido!");
//                $(this).val('');
//                $(this).parent().parent().find('input[name=entnome1]').val('');
//                $(this).parent().parent().find("label").html('');
//                return false;
//            }
//            var param = new Array();
//            param.push({name: 'requisicao', value: 'verifica_cpf'},
//                {name: 'cpf', value: $(this).val()});
//
//            var t = $(this);
//
//            $.ajax({
//                type: "POST",
//                dataType: "json",
//                url: window.location.href,
//                data: param,
//                success: function (data) {
//
//                    if (data[0].origem == "instrumentounidade_entidade") {
//                        var locais = [];
//                        for (i = 0; data.length > i; i++) {
//                            locais.push(data[i].inudescricao);
//                        }
//
//                        var lista_locais = locais.join(',');
//
//                        swal({
//                                title: "Você tem certeza?",
//                                text: "O nutricionista <b>(" + data[0].usunome + ")</b> selecionado está vinculado aos município(s): <b>" + lista_locais + "</b>. Deseja fazer o cadastro dele em um novo município!",
//                                type: "warning", showCancelButton: true,
//                                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
//                                closeOnConfirm: "on",
//                                cancelButtonText: "Cancelar",
//                                html: true
//                            },
//                            function (isConfirm) {
//                                if (isConfirm) {
//                                    t.closest(".ibox-content").find('input[name=entnome1]').val(data[0].usunome);
//                                    t.closest(".ibox-content").find('input[name=entemail]').val(data[0].entemail);
//                                }
//                                else {
//                                    t.closest(".ibox-content").find('input[name=usucpf]').val("");
//                                    t.closest(".ibox-content").find('input[name=entemail]').val("");
//                                    t.closest(".ibox-content").find('input[name=entnome1]').val("");
//                                }
//                            });
//
//                    }
//                    if (data[0].origem == "receita") {
//                        t.closest(".ibox-content").find('input[name=entnome1]').val(data[0].usunome);
//                    }
//                    if (data[0].origem == "seguranca") {
//                        t.closest(".ibox-content").find('input[name=entnome1]').val(data[0].usunome);
//                        t.closest(".ibox-content").find('input[name=entemail]').val(data[0].entemail);
//                    }
//                }
//            });
//        });
<?php if($disabled == ''){ ?>
        $(document).ready(function() {
        <?php if ($_REQUEST['possuiRT']) { ?>
        $("#salvar-nutricionista").click(function(evt){
            evt.preventDefault();
           var tenid_old =  $("#formNutricionista").find("input[name=tenid_old]").val();
           var tenid =  $("#formNutricionista").find("input[name=tenid]:checked").val();
//            console.log(tenid);
//            return false;
           if(tenid_old != tenid && tenid == 7){
               swal({
                       title: "Já existe Responsável Técnico cadastrado.",
                       text: "Deseja substituir pelo informado?",
                       type: "warning",
                       showCancelButton: true,
                       confirmButtonColor: "#DD6B55",
                       cancelButtonText: "Não",
                       confirmButtonText: "Sim",
                       closeOnConfirm: false
                   },
                   function(isConfirm) {
                       if (isConfirm) {
                           $("#formNutricionista").submit();

                       } else {
                           $("#modal").modal('hide');
                           $('#modal').on('hidden.bs.modal', function () {
                               $("#conteudo-modal").html("");
                           });
                           return false;
                       }
                   }
               )
           }else{
               $("#formNutricionista").submit();
           }
        });
    <?php } ?>
    });

    function inativaIntegrante(id) {
        var caminho = window.location.href;
        var action = '&requisicao=recuperar&entid='+id;
        var user;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
             var  user =  $.parseJSON(resp);
                swal({
                        title: "Remover registro com CPF: "+user.entcpf,
                        text: "Tem certeza que deseja remover "+user.entnome+" do cadastro de "+user.tendsc+"?",
                        type: "warning",
                        showCancelButton: true,
                        confirmButtonColor: "#DD6B55",
                        cancelButtonText: "Não",
                        confirmButtonText: "Sim",
                        closeOnConfirm: false
                    },
                    function(isConfirm) {
                        if (isConfirm) {
                            var inuid = $('#inuid').val();
                            var action = '&requisicao=inativar';
                            window.location.href += action + '&entid=' + id;
                        }
                    }
                )
            }
        });
    }

    function editarIntegrante(id) {
        var inuid = $('#inuid').val();
        var caminho = window.location.href;
        var action = '&requisicao=editar&entid='+id;
        //window.location.href = url + action + '&entid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
              $("#conteudo-modal").html(resp);
              $("#modal").modal();
            }
        });
        $('#modal').on('hidden.bs.modal', function () {
            $("#conteudo-modal").html("");
        });
    }
    $(".novo").click(function(){
        var inuid = $('#inuid').val();
        var caminho = window.location.href;
        var action = '&requisicao=novo';
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            beforeSend: function () {
                $('#loading').show();
            },
            success: function (resp) {
                $("#conteudo-modal").html(resp);
                $("#modal").modal();
            }
        });
        $('#modal').on('hidden.bs.modal', function () {
            $("#conteudo-modal").html("");
        });
    });

    function nomeiaRT(id) {
        var inuid = $('#inuid').val();
        var action = '&requisicao=nomearRT';
        window.location.href += action + '&entid=' + id;
    }

    function  desvincularIntegrante(id){
        var inuid = $('#inuid').val();
        var caminho = window.location.href;
        var action = '&requisicao=formDesvincularNutricionista&entid='+id;
        //window.location.href = url + action + '&entid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            beforeSend: function () {
                //$('#loading').show();
            },
            success: function (resp) {
                var content = $.parseJSON(resp);
                var modal = $("#modal-desvincular");
                modal.find("#titulo-modal").html(content.titulo);
                modal.find("#conteudo-modal").html(content.html);

                $("#modal-desvincular").modal();
            }
        });
        $('#modal').on('hidden.bs.modal', function () {
            $("#modal-desvincular").find("#conteudo-modal").html("");
        });
    }
    function detalharNutricionista(entid,entcpf,danid) {
        var tipo_id       = danid != '' ?'danid':'cpf';
        var danid_entcpf  = danid != '' ? danid : entcpf;
        console.info(entcpf);
        // return;
        $.ajax({
            type: 'post',
            url: window.location.href,
            data: 'requisicao=visualisarNutricionista&danid='+danid_entcpf+'&tipo_id='+tipo_id,
            async: false,
            success: function (res) {
                $('#modal_detalhe').html(res);
                $('#modal_detalhe').modal();
            }
        });
    }
<?php } ?>
</script>
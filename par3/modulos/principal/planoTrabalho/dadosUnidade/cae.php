<?php
ob_clean();
//retirar cabeçalho quando for solicitada a habilitação deste menu
header("HTTP/1.1 404 Not Found");
header("Location:".$_SERVER['HTTP_REFERER'].'/erro404.php');
die;
global $simec;

$controllerCAE      = new Par3_Controller_CAE();
$controllerEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$controleUnidade    = new Par3_Controller_InstrumentoUnidade();
$modelCAE           = new Par3_Model_CAE();
$modelCAEIntegracao = new Par3_Model_ConselhoCAE();

$inuid = $_REQUEST['inuid'];

switch( $_REQUEST['req'] ){
    case 'cadastrar':
        $controllerCAE->salvarCAE($_POST, $_FILES);
        break;
    case 'cadastrarPresidente':
        $controllerCAE->salvarPresidente($_POST, $_FILES);
        break;
	case 'cadastrarConselheiro':
    	$controllerCAE->salvarConselheiro($_POST);
        break;
    case 'excluirEntidade':
        $controllerCAE->inativaCAE($_REQUEST);
        break;
    case 'atualizaDados':
        $controllerCAE->atualizaDadosCAE($_REQUEST);
        break;
    case 'downloadArquivo': 
    	 $controllerCAE->downloadArquivo($_REQUEST['arqid']);
        exit;
    case 'excluirArquivo': 
    	$controllerCAE->excluirArquivo($_REQUEST);
    	break;
    case 'excluirArquivoConselho':
        $model = new Par3_Model_CAEConselheiro();
        $model->excluirArquivoConselheiro($_REQUEST);
        break;
    case 'verificaConselheiro':
        ob_clean();
        echo $controllerCAE->verificaConselheiro($_REQUEST['cpf']);
        die();
        break;
    case 'downloadAta':
        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
        $file = new FilesSimec('arquivo', null, 'public');
        $file->getDownloadArquivo($_REQUEST['arqid']);
        break;
    default:
        $objInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
    	$objCAE = $modelCAE->carregarDadosCAE($inuid);
        $objCAEPresidente = $modelCAE->carregarDadosPresidenteCAE($inuid);
        $objCAEIntegracao = $modelCAEIntegracao->retonarResponsavelCAE($objInstrumentoUnidade->estuf, $objInstrumentoUnidade->muncod);
        
        $arrListaConselheiros['inuid']     = $inuid;
        $arrListaConselheiros['tenid']     = Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_CAE;
        $arrListaConselheiros['entstatus'] = 'A';

        $arrParamHistorico              = array();
        $arrParamHistorico['inuid']     = $inuid;
        $arrParamHistorico['tenid']     = Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_CAE;
        $arrParamHistorico['booCPF']    = false;
        break;
}

if ($_REQUEST['req'] == 'editarEntidade') {
    $conselheiroEditar = $controllerCAE->getConselheiro($_REQUEST);
    if ($conselheiroEditar) {
        $modalEditarConselheiro = true;
    }
}

?>
<div class="ibox float-e-margins">
    <div class="ibox-title esconde" tipo="integrantes">
        <div class="row">
            <div class="col-md-9">
                <h3>Conselho de Alimentação Escolar (CAE)</h3>
            </div>
            <div class="col-md-3">
				<span style="padding: 6px 12px; font-size: 13px;" class="label label-lg label-primary right">REGULAR</span>
	        </div>
        </div>
    </div>
    <form method="post" name="formulario" id="formulario" class="form form-horizontal" enctype="multipart/form-data">
        <div class="ibox-content">
            <input type="hidden" name="req"   value="cadastrar">
            <input type="hidden" name="menu"  value="cae">
            <input type="hidden" name="inuid" value="<?php echo $inuid; ?>">
    		<?php $controllerCAE->formDadosCAE($disabled, $objCAE, 'cae'); ?>
        </div>
        <?php if (!Par3_Model_UsuarioResponsabilidade::perfilConsulta()) : ?>
        <div class="ibox-footer">
            <div class="row">
                <div class="col-lg-12 col-lg-offset-3">
                    <button type="submit" class="btn btn-success salvar" <?php echo $disabled;?>><i class="fa fa-save"></i> Salvar CAE</button>
                </div>
            </div>
        </div>
        <?php endif; ?>
    </form>
</div>

<?php if ($objCAE->caeid) : ?>
<div class="ibox float-e-margins">
    <div class="ibox-title esconde" tipo="integrantes">
        <div class="row">
            <div class="col-md-10">
                <h3>Presidente do CAE</h3>
            </div>
        </div>
    </div>
    <form method="post" name="formulario" id="formulario" class="form form-horizontal" enctype="multipart/form-data">
        <div class="ibox-content">
            <input type="hidden" name="req"   value="cadastrarPresidente">
            <input type="hidden" name="menu"  value="cae">
            <input type="hidden" name="caeid" value="<?php echo $objCAE->caeid; ?>"/>
            <input type="hidden" name="capid" value="<?php echo $objCAEPresidente->capid; ?>"/>
            <input type="hidden" name="inuid" value="<?php echo $inuid; ?>">
    		<?php $controllerCAE->formDadosPresidenteCAE($disabled, $objCAEPresidente, 'presidente'); ?>
        </div>
        <?php if (!Par3_Model_UsuarioResponsabilidade::perfilConsulta()) : ?>
        <div class="ibox-footer">
            <div class="row">
                <div class="col-lg-12 col-lg-offset-3">
                    <button type="submit" class="btn btn-success salvar" <?php echo $disabled;?>><i class="fa fa-save"></i> Salvar Presidente</button>
                </div>
            </div>
        </div>
        <?php endif; ?>
    </form>
</div>
<?php endif; ?>

<?php if ($objCAE->caeid) : ?>
    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-6">
                    <h3 href="#conselheiroCAE" class="pull-left" style="margin-bottom:10px;">
                        Conselheiros do CAE
                        <small style="font-size: 75%;">&nbsp;É necessário inserir no mínimo 6 conselheiros.</small>
                    </h3>
                </div>
                <div class="col-md-6">
                    <button <?php echo $disabled;?> class="btn btn-success novo pull-left" data-toggle="modal" data-target="#modal">
                        <i class="fa fa-plus-square-o"></i>
                        Inserir Conselheiro
                    </button>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <?php $controllerEntidade->recuperarEntidadesSimplificado($arrListaConselheiros); ?>
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

    <div class="ibox collapsed">
        <div class="ibox-title">
            <h5>CAE - Histórico Modificações</h5>
            <div class="ibox-tools">
                <a class="collapse-link">
                    <i class="fa fa-chevron-down"></i>
                </a>
            </div>
        </div>
        <?php $controllerEntidade->formHistorico($arrParamHistorico); ?>
    </div>
    <?php require_once 'caeConsultor.php'; ?>
<?php endif; ?>

<script>
$(document).ready(function()
{
	$('.entcpfconselheiro').change(function() {
		var cpf   = $('#entcpf').val();
    	var param = '&req=verificaConselheiro&cpf='+cpf;
        var msg   = '';
        
		$.ajax({
            type: "POST",
            url: window.location.href+param,
            success: function (data) {
                if (data) {
                	swal({
                        title: "Você tem certeza?",
                        text: data,
                        type: "warning", showCancelButton: true,
                        confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                        closeOnConfirm: "on",
                        cancelButtonText: "Cancelar",
                        html: true
                    }, function (isConfirm) {
                        if (!isConfirm) {
                        	$('#entcpf, #entnome, #entemail').val('');
                        }
                    });
                }
            }
        });
	});
		
	$('input[name="caedtinicio"]').change(function() {
		var inicio = $('#caedtinicio').val();
		var termino = new Date(inicio.substr(6,4), inicio.substr(3,2), inicio.substr(0,2));
			termino.setFullYear(termino.getFullYear() + 4, termino.getMonth());
		$('#caedttermino').val(inicio.substr(0,2) + '/' + inicio.substr(3,2)  + '/' + termino.getFullYear());
	});

	$('input[name="caenomeacao"]').change(function() {
		if ($(this).val() == 'O') {
			$('#caenomeacaooutro').closest('.form-group').removeClass('hidden');
		} else {
			$('#caenomeacaooutro').closest('.form-group').addClass('hidden');
			$('#caenomeacaooutro').val('');
		}
	})

    $('.atualizar').click(function(){
        swal({
                title: "Você tem certeza?",
                text: "Você deseja atualizar os dados do CAE?",
                type: "warning", showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim!",
                closeOnConfirm: "on",
                cancelButtonText: "Cancelar",
                html: true
            },
            function (isConfirm) {
                if (isConfirm) {
                    var url = window.location.href;
                    var param = '&req=atualizaDados';
                    window.location.href = url+param;
                }
            });
    });

    $('.atualizar').mouseover(function(){
        $('.fa-refresh').addClass('fa-spin');
    });

    $('.atualizar').mouseout(function(){
        $('.fa-refresh').removeClass('fa-spin');
    });

    $('.previous').click(function(){
        var url = window.location.href.toString().replace('cae', 'conselho');
        window.location.href = url;
    });

    $('.next').click(function(){
        window.location.href = 'par3.php?modulo=principal/planoTrabalho/pne&acao=A&inuid=<?php echo $_REQUEST['inuid'] ?>';
    });

    // Torna conteudo do modal "Inserir conselheiro" editável
    <?php if ($modalEditarConselheiro): ?>
        $("html, body").animate({scrollTop: $("[href='#conselheiroCAE']").offset().top}, 1500, "easeInSine");
        setTimeout(function(){
            $("#modal").modal();
        }, 350);
    <?php endif; ?>

    // Evento para modal de "Inserir conselheiro"
    $("#modal").on("hidden.bs.modal", function (e) {
        ["entcpf_old", "entcpf", "entnome", "entemail"].forEach(function(el, i){
            $("#"+el+"-conselheiro").val("");
        });
        $(".arqid-conselheiro-sx").html("");
        $("<input/>", {
            "type": "file",
            "name": "arqid",
            "id": "arqid",
            "required": "required",
        }).appendTo(".arqid-conselheiro-sx");
    });
});

function getEnderecoByCEP1(endcep, tipoendereco) {
    url = '/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP';
    data = '&endcep=' + endcep;
    $.post(
        url
      , data
      , function(resp){
            var dados = resp.split("||");
            $( '#endlogradouro1' ).val(dados[0]);
            $( '#endcomplemento1' ).val('');
            $( '#endnumero1').val('');
            $( '#endbairro1').val(dados[1]);
        }
    );
}

//
//function verificaVicePresidente(){
//    var entcpf      = $('#formularioConselheiro').find('#entcpf-conselheiro').val();
//    var inuid       = $('#formularioConselheiro').find('[name="inuid"]').val();
//    var tenid       = $('#formularioConselheiro').find('[name="tenid"]').val();
//    var cacfuncao   = $('#formularioConselheiro').find('[name="cacfuncao"]:checked').val();
//    
//    var subist;
//
//    if( entcpf != '' ){
//        $.ajax({
//            type: "POST",
//            url: '/par3/_ajax_cae.php',
//            data: "action=verificaVicePresidente&entcpf="+entcpf+"&cacfuncao="+cacfuncao+"&inuid="+inuid+"&tenid="+tenid,
//            async: false,
//            success: function( resp ){
//                if( resp == 'S' ){
//                    swal({
//                        title: "Deseja realmente subistituir o Vice Presitende?",
//                        type: "warning",
//                        showCancelButton: true,
//                        confirmButtonClass: "btn-danger",
//                        confirmButtonText: "Sim, Subistituir!",
//                        cancelButtonText: "Não, Cancelar!",
//                        closeOnConfirm: false,
//                        closeOnCancel: false
//                    },function(isConfirm) {
//                        if( isConfirm ){
//                            console.log('teste-s');
//                            //subist = 'S';
//                            return true;
//                        } else {
//                            console.log('teste-n');
//                            //subist = 'N';
//                            return false;
//                        }
//                    });
//                }
//                console.log('teste-fora');
//                return 'S';
//            }
//        });
//    }
//}

</script>
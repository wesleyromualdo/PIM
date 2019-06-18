<?php

require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';

echo $simec->title ( 'Lista Entidades', '' );

if( $_REQUEST['enfid'] ){
    $enfid = $_REQUEST['enfid'];
    $url = 'pim.php?modulo=principal/listaEntidades&acao=A&enfid='.$_REQUEST['enfid'];
} else {
    $url = 'pim.php?modulo=principal/listaEntidades&acao=A&enfid=1';
    $enfid = 1;
}

$obFuncaoEntidade = new Pim_Controller_FuncaoEntidade();
$arFucao = $obFuncaoEntidade->pegaFuncaoEntidade($enfid);
echo $simec->tab ( criaAbaPar (), $url );
?>
<style>
    .table > thead > tr > th, .table > tbody > tr > th, .table > tfoot > tr > th, .table > thead > tr > td, .table > tbody > tr > td, .table > tfoot > tr > td,.table > td {
        border-top: 1px solid #e7eaec;
        line-height: 1.42857;
        padding: 8px;
        vertical-align: middle;
        text-align:center;
    }
</style>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Filtros de pesquisa</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    	<form method="post" name="formulario" id="formulario">
			<input type="hidden" name="pesquisa" value="1"> 
			<input type="hidden" id="exporta" name="exporta" value="false">
        	<div class="row">
                <div class="row">
                    <div class="col-lg-6">
                    	<?php
						$mundescricao = simec_htmlentities ( $_POST ['mundescricao'] );

						$arrAttr = array (
						    'placeHolder' => $arFucao['enfdsc'],
							'maxlength' => '100'
						);
						echo $simec->input ( 'mundescricao', 'Informe a '.$arFucao['enfdsc'], $mundescricao, $arrAttr, array('label-size' => '4') );
						?>
                    </div>
				</div>
			</div>
		</form>
	</div>
	<div class="ibox-footer">
        <div class="center">
            <button type="reset" class="btn btn-primary btn-pesquisar" style="width: 200px;"><i class="fa fa-search"></i> Pesquisar</button>
        </div>
    </div>
</div>

<?php if(array_key_exists('pesquisar', $_REQUEST)) : ?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Lista de <?php echo $arFucao['enfdsc']; ?></h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    </div>
</div>

    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Lista de municípios</h5>
            <div class="ibox-tools">
                <a class="collapse-link"> <i class="fa fa-chevron-up"></i>
                </a>
            </div>
        </div>
        <div class="ibox-content">
            <div class="">
                <button id="alternarColuna" class="btn btn-default" title="alternar Coluna Planejamento e Diagnostico"><i class="fa fa-exchange"></i></button>
            </div>
            <div id="listaEstados">
                <?php Par3_Controller_InstrumentoUnidade::lista($_REQUEST); ?>
            </div>
        </div>
    </div>
<?php endif; ?>
        

<!--modal situação-->
<div class="ibox float-e-margins animated modal" id="modal_situacao" role="dialog" aria-hidden="true">
    <div class="modal-dialog" style="width: 800px;">
        <form method="post" name="formTramitarSituacao" id="formTramitarSituacao" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo"></h3>
                </div>
                <div class="ibox-content" id="conteudo-modal">
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="submit" id="cancelarDescIniciativa" data-dismiss="modal"
                                class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Cancelar
                        </button>
                        <button type="button" id="tramitarSituacao"
                                class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Enviar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>


<script type="text/javascript">
$(document).ready( function(){
	$( "input[name='municipio']" ).focus();
});


function confirmaTramitacao(nome, situacao){

    var msg = "Tem certeza que deseja voltar a situação do <b>" + nome + "</b> para <b>" + situacao + "</b>?";

    swal({
            title: "Confirmar",
            text:  msg,
            type: "warning",
            html: true,
            showCancelButton: true,
            confirmButtonColor: "#DD6B55",
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                $('#formTramitarSituacao').submit();
            }
            return false;
        }
    );

}

function dadosUnidade( inuid ) {
	var url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='+inuid;
    $(location).attr('href', url);
}

var DELAY = 700, clicks = 0, timer = null;
$(document).ready(function() {
    $("#alternarColuna").on("click", function(e){
        clicks++;
        console.log(clicks);
        var alternar = $("#alternarColunaVal").val();
        if(clicks === 1) {
            timer = setTimeout(function() {
                console.log(alternar);
                switch (alternar){
                    case 'planejamento':alternar = 'diagnostico';break;
                    case 'diagnostico': alternar = 'planejamento';break;
                    default:            alternar = 'diagnostico';break;
                }
                $('#alternarColunaVal').val(alternar);
                clicks = 0;
                $('#listaEstados').load(window.location.href+'&requisicao=alternarcolunas&alternar='+alternar+'&'+$('#formulario').serialize());
            }, DELAY);
        } else {
            clearTimeout(timer);
            alternar = 'todos';
            $('#alternarColunaVal').val('todos');
            clicks = 0;
            $('#listaEstados').load(window.location.href+'&requisicao=alternarcolunas&alternar='+alternar+'&'+$('#formulario').serialize());
        }
    }).on("dblclick", function(e){
        e.preventDefault();
    });
});
</script>
<style>
.tabela-listagem tr td:nth-child(3) {
	text-align: left !important;
}

.tabela-listagem tr th:nth-child(2) {
	width: 60px;
}
</style>
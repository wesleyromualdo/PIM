<?php
/**
 * Lista de Estados
 *
 * @category Lista
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN: 28-09-2015
 * @link     no link
 */
require APPRAIZ.'includes/cabecalho.inc';
require APPRAIZ.'includes/Agrupador.php';

// Municipio.
$_REQUEST['itrid'] = array(1, 3);

$_REQUEST['responsabilidadeEstado'] = pegaResponssabilidade( '1' );

switch ($_REQUEST['requisicao']){
    case 'alternarcolunas':
        ob_clean();
        echo Par3_Controller_InstrumentoUnidade::lista($_REQUEST);die;
        break;
    case 'modalSituacao':
        ob_clean();
        Par3_Controller_InstrumentoUnidade::modalSituacao($_REQUEST);
        die;
        break;
    case 'tramitarSituacao':
        Par3_Controller_InstrumentoUnidade::tramitarSituacao($_REQUEST);
        break;
}

echo $simec->title('Lista de Estados', '');
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
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
		<?php
		$url = 'par3.php?modulo=principal/listaEstados&acao=A';
		echo $simec->tab(criaAbaPar(), $url);
		?>

        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Filtros de pesquisa</h5>
            </div>
            <div class="ibox-content">
                <form method="post" name="formulario" id="formulario">
                    <input type="hidden" id="alternarColunaVal" value="<?php echo $_REQUEST['alternarColunas']?$_REQUEST['alternarColunas']:'diagnostico';?>"/>
                    <input type="hidden" name="pesquisa" value="1">
                    <input type="hidden" id="exporta" name="exporta" value="false">
                    <div class="form-group">
                        <div class="row">
                            <div class="col-md-6">
                                <?php
                                $sql = Territorios_Model_Estado::pegarSQLSelect ( $_POST );

                                $arrAttr = array (
                                    'data-placeholder' => 'Estado'
                                );
                                echo $simec->select ( 'estuf', 'Selecione Estado', $_POST ['estuf'], $sql, $arrAttr ,array('label-size' => 2));
                                ?>
                            </div>
                            <div class="col-md-6">
                                <?php
                               $_POST['tpdid'] = 243;
                               $sql = Workflow_Model_EstadoDocumento::pegarSQLSelect ( $_POST );
                                $mEstadoDocumento = new Workflow_Model_EstadoDocumento();
                                $rsEsd = $mEstadoDocumento->getArrayEstadoDocumentoPAR3();
                                $arrAttr = array (
                                    'data-placeholder' => 'Situação'
                                );
                                echo $simec->select ( 'esdid', 'Selecione Situação', $_POST ['esdid'],  $rsEsd, $arrAttr,array('label-size' => 2));
                                ?>
                            </div>
                        </div>
                        <div class="col-lg-2"><input class="btn btn-primary" type="submit" name="pesquisar" value="Pesquisar" /></div>
                        <br/><br/>
                    </div>
                </form>
            </div>
        </div>

		<div class="ibox float-e-margins">
			<div class="ibox-title">
				<h5>Lista de Estados</h5>
				<div class="ibox-tools">
					<a class="collapse-link"> <i class="fa fa-chevron-up"></i>
					</a>
				</div>
			</div>
            <div class="ibox-content">
               <div class=""><button id="alternarColuna" class="btn btn-default" title="alternar Coluna Planejamento e Diagnostico"><i class="fa fa-exchange"></i></button></div>
                <div id="listaEstados">
                    <?php Par3_Controller_InstrumentoUnidade::lista($_REQUEST); ?>
                </div>
			</div>
		</div>
	</div>
</div>

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
    var alternarColunas = '';
function dadosUnidade( inuid ) {
	var url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='+inuid;
    $(location).attr('href', url);
}

function modalSituacao( inuid ){
    var caminho = '<?php echo $urlGuia;?>';
    var action = '&requisicao=modalSituacao&inuid=' + inuid;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {

            var obj = JSON.parse(resp);
            var dados = JSON.parse(obj.dados);

            $('#conteudo-modal').html(obj.html);
            $('#modal_titulo').text("Retornar fase do " + dados.esfera + " - " + dados.nome);
            if(parseInt(dados.esdid) > 0){
                $('#tramitarSituacao').prop('disabled',false)
                                      .val(dados.esdid)
                                      .attr('onClick', 'confirmaTramitacao("' + dados.esfera + ' ' + dados.nome + '", "' + dados.estadoAnterior + '")');
            }else{
                $('#tramitarSituacao').prop('disabled',true)
                                      .val('');

            }

            $('#modal_situacao').modal();
        }
    });
}

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
.tabela-listagem tr td {
	text-align: left !important;
}
</style>
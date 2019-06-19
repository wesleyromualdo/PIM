<?php
/**
 * Desvinculação Nutricionista
 *
 * @category Lista
 * @package  A1
 * @author   Edaurdo Dunice Neto <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN:
 * @link     no link
 */
$modelDadosnutricionista = new Par3_Model_DadosNutricionista($_REQUEST['dnid']);

$controleUnidade    = new Par3_Controller_InstrumentoUnidade();
$controleEntidade   = new Par3_Controller_InstrumentoUnidadeEntidade();

switch ($_REQUEST['req']) {
    case 'desvincular':
        $vinculo = new Par3_Model_VinculacaoNutricionista($_REQUEST['vnid']);
        $vinculo->vndatadesvinculacao           = $_REQUEST['vndatadesvinculacao'];
        $vinculo->vnmotivodesvinculacao         = $_REQUEST['vnmotivodesvinculacao'];
        $vinculo->vntermodesvinculacao          = $vinculo->gerarTermoDesvinculacao($_REQUEST);;
        $vinculo->vnassinaturadesvinculacao     = 'TRUE';
        $vinculo->vndataassinaturadesvinculacao = 'now()';
        $vinculo->usucpfalteracao               = $_SESSION['usucpf'];
        $vinculo->vnstatus = 'I';
        $vinculo->salvar();
        $vinculo->commit();

        $entidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $entid    = $entidade->recuperarEntidadeUnidadePorCPF($vinculo->inuid, $vinculo->vncpf, $vinculo->tenid);


        try {
            $entidade->carregarPorId($entid);
            $entidade->entstatus           = 'I';
            $entidade->entdtinativacao     = 'now()';
            $entidade->entusucpfinativacao = $_SESSION['usucpf'];
            $entidade->inuid = $vinculo->inuid;
            $entidade->tenid = $vinculo->tenid;
            $entidade->salvar();
            $entidade->commit();
            $vinculo->enviarEmailDesvinculacao($_REQUEST['vnid']);
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $entidade->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
        }
        break;
    case 'gerarTermo':
        $vinculo = new Par3_Model_VinculacaoNutricionista($_REQUEST['vnid']);
        $termo = $vinculo->gerarTermoDesvinculacao($_REQUEST);
        echo $termo;
        die();
        break;
    default:

        $abas = array(
            0 => array("descricao" => "Vinculação",     "link" => "par3.php?modulo=principal/nutricionista/vinculacaoNutricionista&acao=A"),
            1 => array("descricao" => "Dados Pessoais", "link" => "par3.php?modulo=principal/nutricionista/cadNutricionista&acao=A"),
            2 => array("descricao" => "Desvinculação",  "link" => "par3.php?modulo=principal/nutricionista/desvinculacaoNutricionista&acao=A"),
        );

        break;
}

require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';

echo $simec->title('Lista Cadastro Nutricionista', '');
?>
<style>
 .tabela-listagem td {
    	vertical-align: middle !important;
  }
</style>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <input type="hidden" name="req" value="salvarDadosnutricionista"/>
        <?php echo $simec->tab($abas, 'par3.php?modulo=principal/nutricionista/desvinculacaoNutricionista&acao=A'); ?>
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Desvinculação de nutricionista</h5>
                <div class="ibox-tools">
                    <a class="collapse-link"> <i class="fa fa-chevron-up"></i>
                    </a>
                </div>
            </div>
            <div class="ibox-content">
                <input type="hidden" name="req" value="salvar">
                <table class="table tabela-listagem">
                    <thead>
                    <tr>
                        <th>Ação</th>
                        <th>Entidade</th>
                        <th>Data de vinculação</th>
                        <th>Carga Horário Semanal</th>
                        <th>Tipo de vínculo</th>
                        <th>Atuação exclusiva</th>
                    </tr>
                    </thead>
                    <tbody>
                    <?php
                    $iue = new Par3_Model_InstrumentoUnidadeEntidade();
                    $vinculacao = new Par3_Model_VinculacaoNutricionista();
                    $arrFiltro = array(
                                    "vncpf = '{$_SESSION['usucpf']}'",
                                    "vnstatus = 'A'",
                                    );
                    $dados = $vinculacao->recuperarTodos('*', $arrFiltro);
                    foreach ($dados as $key) {
                        $dadosEntidade = $iue->carregarDadosPorCpf($key['vncpf'], $key['inuid']);
                        ?>
                        <tr>
                            <td>
                                <button
                                    class="btn btn-success desvincular"
                                    data-toggle="modal"
                                    data-target="#modal"
                                    vnid="<?php echo $key['vnid']; ?>"
                                    unidade="<?php echo $unidadeAtual = $controleUnidade->pegarNomeEntidade($key['inuid']) ?>" >Desvincular</button>
                            </td>
                            <td><?php echo $unidadeAtual = $controleUnidade->pegarNomeEntidade($key['inuid']) ?></td>
                            <td><?php echo formata_data($key['vndatavinculacao']) ?></td>
                            <td><?php echo $key['vncargahorariasemanal']; ?></td>
                            <td>
                                <?php
                                    $sql = "SELECT
                                                dntvid as codigo,
                                                dntvdescricao as descricao
                                            FROM
                                                par3.dadosnutricionistatipovinculo ";
                                    echo $key['dntvid'] != '' ? $modelDadosnutricionista->pegaUm("SELECT dntvdescricao FROM par3.dadosnutricionistatipovinculo WHERE dntvid = ".$key['dntvid']) : '';
                                ?>
                            </td>
                            <td>
                                <?php
                                if($key['tenid'] == 16){
                                    $mTipoEntidade = new Par3_Model_TipoEntidade($key['tenid']);
                                    echo $mTipoEntidade->tendsc;
                                }else{
                                echo 'Não';
                                }
                                ?>
                            </td>
                        </tr>
                    <?php } ?>
                    </tbody>
                </table>
            </div>
        </div>
    </div>
</div>
<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
    	<form method="post" name="formDesvoncula" id="formDesvoncula" class="form form-horizontal">
            <input type="hidden" name="req"     id="req"     value="" />
            <input type="hidden" name="vnid"    id="vnid"    value="" />
            <input type="hidden" name="unidade" id="unidade" value="" />
	        <div class="modal-content">
	            <div class="ibox-title" tipo="integrantes">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
	                <h3>Desvincular</h3>
	            </div>
	            <div class="ibox-content">
	               <p>Para desfazer o seu vinculo com <label id="lUnidade" ></label> é necessário informar a data de desvinculação e o motivo.</p>
	               <br>
	               <?php
		               echo $simec->data('vndatadesvinculacao', 'Data desvinculação', null, array('required'), array('label-size'=>'4'));
	                   echo $simec->input('vnmotivodesvinculacao', 'Motivo', null, array('required' => 'required'), array('label-size'=>'4'));
	               ?>
	            </div>
	            <div class="ibox-content" id="termo" style="display:none">
	            </div>
	            <div class="ibox-footer text-center">
                	<button type="button" class="btn btn-success" id="gerar" >
                		Gerar Termo
                	</button>
                	<button type="submit" class="btn btn-success" id="assinar" style="display:none;" disabled>
                		Assinar Termo
                	</button>
	            </div>
	        </div>
        </form>
    </div>
</div>
<script>
$("document").ready(function () {
	$('.tabela-listagem').DataTable({
		'bFilter': false,
		'bInfo': false,
		'bLengthChange': false,
		'bSort': false,
		'aoColumnDefs' : [{
			'bSortable' : false,
			'aTargets' : [ 'unsorted' ]
		},{
            'targets': 0,
            'searchable': false,
            'orderable': false,
            'className': 'dt-body-center',
        },{
            "targets": [ 1 ],
            "visible": false,
            "searchable": false
        }],
		'oLanguage' : {
			'sProcessing' : "Processando...",
			'sLengthMenu' : "Mostrar _MENU_ registros",
			'sZeroRecords' : "N&atilde;o foram encontrados resultados",
			'sInfo' : "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
			'sInfoEmpty' : "Mostrando de 0 at&eacute; 0 de 0 registros",
			'sInfoFiltered' : "(filtrado de _MAX_ registros no total)",
			'sInfoPostFix' : ".",
			'sSearch' : "Pesquisar :&nbsp;&nbsp;",
			'sUrl' : "",
			'oPaginate' : {
				'sFirst' : "Primeiro",
				'sPrevious' : "Anterior",
				'sNext' : "Seguinte",
				'sLast' : "&Uacute;ltimo"
			}
		}
	});

	$('.desvincular').click(function(){

	    $('#vnid').val($(this).attr('vnid'));
	    $('#lUnidade').html($(this).attr('unidade'));
	    $('#unidade').val($(this).attr('unidade'));
	});

	$('#assinar').click(function(){

		$('#req').val('desvincular');
	});

	$('#vndatadesvinculacao').change(function(){
		$('#termo').hide();
		$('#assinar').hide();
    	$('#assinar').attr('disabled', true);
	});

	$('#vnmotivodesvinculacao, #vndatadesvinculacao').blur(function(){
		if($(this).val() == '' || $(this).val() == '__/__/____'){
			$('#termo').hide();
			$('#assinar').hide();
        	$('#assinar').attr('disabled', true);
		}
	});

	$('#gerar').click(function(){

		var teste = $('#formDesvoncula').isValid(function(isValid){

			if(isValid){
        		$('#req').val('gerarTermo');

        		$.ajax({
                    type: 'post',
                    url: window.location.href,
                    data: $('#formDesvoncula').serialize(),
                    async: false,
                    success: function (res) {
                    	$('#termo').html(res);
                    	$('#termo').show();
                    	$('#assinar').show();
                    	$('#assinar').attr('disabled', false);
                    }
                });
			}
		});
	});
});
</script>
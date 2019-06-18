<?php
	/*
	 * Para atualizar os arquivos da questão 2 rodar url abaixo
	 * seguranca/scripts_exec/par3_questoesEstrategicas_atualizarQuestao2.php
	 */
	require_once APPRAIZ . 'includes/cabecalho.inc';
	require_once APPRAIZ . "includes/classes/questionario/TelaBootstrap.class.inc";
	require_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/MontatabelaBootstrap.class.inc";
	require_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	require_once APPRAIZ . 'par3/classes/helper/CampoExterno.class.inc';

	$inuid = $_REQUEST['inuid'];

	$controleUnidade = new Par3_Controller_InstrumentoUnidade();
	$modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);

	$disabled = 'disabled';

	$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

	if(!$arrPermissao['booVisualizar']){
	    echo "
            <script>
                alert('Acesso negado.');
                window.location.href = 'par3.php?modulo=inicio&acao=C';
            </script>";
	    die();
	}

	if($arrPermissao['booEscrita']){
	    $disabled = '';
	}

	$habilitado = 'N';
	if($disabled == ''){
	    $habilitado = 'S';
	}

	$queid = QUESTIONARIO_QUESTOES_PNE;

	$qrpidPNE = $modelUnidade->retornarQrpidUnidade($queid);

	if( $_POST['salvar_questionario'] && $_POST['idTabela'] ){
	    $_REQUEST['req'] = 'salvarTabela';
	}

	switch ($_REQUEST['req']){
		case 'salvarEscolaGremio':
		    ob_clean();
		    Par3_Controller_QuestoesEstrategicasEscolasGremio::salvarEscolaGremio($_REQUEST);
		    $modelEscola = new Par3_Model_QuestoesEstrategicasEscolasGremio();
		    echo $modelEscola->retornarQtdEscolas($_REQUEST['qrpid'], $_REQUEST['perid']);
		    die();
		    break;
		case 'formEscolasGremio':
		    ob_clean();
		    Par3_Controller_QuestoesEstrategicasEscolasGremio::formEscolasGremio();
		    die();
		    break;
		case 'salvarEscolaCE':
		    ob_clean();
		    Par3_Controller_QuestoEsestrategicasEscolasCe::salvarEscolaCE($_REQUEST);
		    $modelEscola = new Par3_Model_QuestoesEstrategicasEscolasCe();
		    echo $modelEscola->retornarQtdEscolas($_REQUEST['qrpid'], $_REQUEST['perid']);
		    die();
		    break;
		case 'formEscolasCe':
		    ob_clean();
		    Par3_Controller_QuestoEsestrategicasEscolasCe::formEscolasCe();
		    die();
		    break;
		case 'salvarArquivoQuestionario':
		    Par3_Helper_CampoExterno::salvar();
		    break;
		case 'salvarTabela':
    	    $obMonta = new Montatabela();
    	    $obMonta->salvar();
		    break;
		case 'excluirArquivo':
		    $modelQuestoes = new Par3_Model_QuestoesEstrategicasAnexos();
		    $modelQuestoes->excluirPorArqid($_REQUEST['arqid']);
		    simec_redirecionar('par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid='.$_REQUEST['inuid'], 'success', 'Arquivo excluído.');
		    break;
	}
?>
<style>
.ibox-content-round-gray{
    width:99%;
    background-color: #f3f3f4;
    float: center;
    border-radius: 10px 10px 10px 10px;
    -moz-border-radius: 10px 10px 10px 10px;
    -webkit-border-radius: 10px 10px 10px 10px;
    border: 0px solid #000000;
}

.iconUnidade{
    margin-top: 0px !important;
    margin-left: 0px !important;
    font-size: 15px !important;
    background-color:black;
    border-radius: 20px 20px 20px 20px;
    -moz-border-radius: 20px 20px 20px 20px;
    -webkit-border-radius: 20px 20px 20px 20px;
    color: yellow !important;
}

.menuUnidade{
    border-bottom: 1px solid #e3e3e3;
    padding: 5px;
    cursor:pointer;
}

.menuUnidade:hover{
    font-weight: bold;
}

.menuSelecionado{
    background-color: #F5F5DC;
    cursor: default !important;
}

@media (max-width: 1450px) {
    .ibox-content-round-gray{
        margin-top: 0.5%
    }
}

@media (max-width: 1200px) {
    .i1450 {
        display: inline;
    }
    .custom-col-10{
        padding: 5px 0px 0px 3.8%;
    }
    .marcador{
        position:absolute;
        border-radius: 100% 0% 0% 100%;
        -moz-border-radius: 100% 0% 0% 100%;
        -webkit-border-radius: 100% 0% 0% 100%;
        border: 0px;
        width:50px;
        height:10px;
        margin-top:21px;
        margin-left:-1%;
    }
}
#telaarvore #bloco {
	margin: 0px;
}
#telaarvore #bloco, #telaarvore .dtree {
	font-size: 13px;
}
#telacentral table {
	width: 100%;
}
</style>
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
        <div class="ibox float-e-margins">
            <div class="ibox-content">
                <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']?>"/>
	            <?php echo $simec->title($controleUnidade->pegarNomeEntidade($_REQUEST['inuid'])); ?>
                <br>
                <?php $controleUnidade->cabecalhoUnidade(); ?>
                <?php
                $url = 'par3.php?modulo=principal/planoTrabalho/questoesPNE&acao=A&inuid='.$_REQUEST['inuid'];
                $abasQuestoes= array(
						0 => array( "descricao" => "Questões Estratégicas", "link"	  								=> "par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid=".$_REQUEST['inuid']),
                		1 => array( "descricao" => "Plano Nacional de Educação: Questões Complementares", "link"	=> "par3.php?modulo=principal/planoTrabalho/questoesPNE&acao=A&inuid=".$_REQUEST['inuid']),
                 		2 => array( "descricao" => "Banda Larga", "link"	  										=> "par3.php?modulo=principal/planoTrabalho/bandaLargaQuestionario&acao=A&inuid=".$_REQUEST['inuid'])
                );
                ?>
                <div class="ibox-content-round-gray ibox-content">
		               <?php
							$msgDesabilitadoPNE = 'As questões pontuais só estarão disponíveis para edição no periodo de {dt_inicio} a {dt_fim}';
							echo $simec->tab ( $abasQuestoes , $url );
							$telaPNE = new TelaBootstrap(array("qrpid" => $qrpidPNE, 'tamDivArvore' => 25, 'relatorio' => 'modulo=relatorio/relatorioQuestoesEstrategicas&acao=A', 'habilitado' => $habilitado, 'msgDesabilitadoPNE' => ''));
						?>
    			</div>
			</div>
		</div>
	</div>
</div>
<div id="modal-form" class="modal fade" aria-hidden="true">
	<div class="modal-dialog" style="width: 850px">
        <div class="ibox-title" style="width: 850px">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
		<div id="html_modal-form" style="width: 850px">
		</div>
	</div>
</div>
<script>
function excluirArquivo(arqid){
	var url = '?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A';
    $(window).attr('location', url+'&inuid=<?php echo $inuid; ?>&req=excluirArquivo&arqid='+arqid);
}

function atulizaEscolaCE(entid){
	var qrpid = $('#qrpid').val();
	var perid = $('#perid_escola').val();
	var check = $(this).attr('checked');
	var param = '&req=formEscolasCe&qrpid='+qrpid+'&perid='+perid+'&check='+check;
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: param,
   		async: false,
   		success: function(resp){
			$('#html_modal-form').html(resp);
		    $('#modal-form').modal();
   		}
 	});
}

$(document).ready(function()
{
    $('#telaarvore').find('fieldset').find('.ibox-title').html('<h5>Plano Nacional de Educação: Questões Complementares</h5>');
    $('#sarvore0').html('Plano Nacional de Educação: Questões Complementares');

	$(document).ajaxComplete(function() {
        $('#telaarvore').find('fieldset').find('.ibox-title').html('<h5>Plano Nacional de Educação: Questões Complementares</h5>');
        $('#sarvore0').html('Plano Nacional de Educação: Questões Complementares');
	});

	$(document).on('click','.escolas_ce',function()
    {
		var qrpid = $(this).attr('qrpid');
		var perid = $(this).attr('perid');
	    var param = '&req=formEscolasCe&qrpid='+qrpid+'&perid='+perid;
		$.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: param,
       		async: false,
       		success: function(resp){
    			$('#html_modal-form').html(resp);
    		    $('#modal-form').modal();
       		}
     	});
	});

	$(document).on('click','.escolas_gremio',function()
    {
		var qrpid = $(this).attr('qrpid');
		var perid = $(this).attr('perid');
	    var param = '&req=formEscolasGremio&qrpid='+qrpid+'&perid='+perid;
		$.ajax({
       		type: "POST",
       		url: window.location.href,
       		data: param,
       		async: false,
       		success: function(resp){
    			$('#html_modal-form').html(resp);
    		    $('#modal-form').modal();
       		}
     	});
	});
});
</script>
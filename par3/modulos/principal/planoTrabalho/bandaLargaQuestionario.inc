<?php
require_once APPRAIZ . "includes/classes/questionario/TelaBootstrap.class.inc";
require_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/MontatabelaBootstrap.class.inc";
require_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ."includes/cabecalho.inc";

$inuid = $_REQUEST['inuid'];

$controleUnidade    = new Par3_Controller_InstrumentoUnidade();
$modelUnidade       = new Par3_Model_InstrumentoUnidade($inuid);
$modelBandaLarga    = new Par3_Model_Relatorio_BandaLarga();

switch($_REQUEST['requisicao']){
	case 'carregaMunicipios':
        extract($_REQUEST);

    	$sql = "SELECT muncod as codigo, mundescricao as descricao
        	FROM territorios.municipio
        	WHERE estuf = '$estuf'
        	ORDER BY 2 ASC";

    	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', 'habilitaQuestionario', '' );
    	die();
	    break;
	case 'verificaPreenchimentoQuestionario':
        ob_clean();
        echo $modelBandaLarga->carregarBotaoWorkflow($inuid);
	    die();
	    break;
	case 'finalizarPreenchimentoQuestionario':
        ob_clean();
        $modelBandaLarga->finalizarPreenchimentoQuestionario($inuid);
	    die();
	    break;
	case 'reabrirPreenchimentoQuestionario':
        ob_clean();
        $modelBandaLarga->reabrirPreenchimentoQuestionario($inuid);
	    die();
	    break;
	case 'historicoBootstrap':
        ob_clean();
        include_once APPRAIZ.'includes/workflow.php';
        include_once APPRAIZ.'www/geral/workflow/view_historico_new.php';
	    die();
	    break;
	default:
	    $disabled = 'disabled';

	    $arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

	    $qrpid                 = $modelUnidade->retornarQrpidUnidade(QUESTIONARIO_BANDA_LARGA);
	    $docid                 = $modelUnidade->retornarDocidQuestionario(QUESTIONARIO_BANDA_LARGA);
	    $modelWorkflow         = new Workflow_Model_Documento($docid);
	    $modelEstadoDocumento  = new Workflow_Model_EstadoDocumento($modelWorkflow->esdid);

	    $habilitado = 'N';
	    if($modelWorkflow->esdid == WF_ESDID_INICIADO ){
	        $habilitado = 'S';
	    }
        $arrVigencia = $modelBandaLarga->recuperaVigenciaQuetionario();
	    break;
}
?>
<script language="JavaScript">
$(document).ready(function(){
	$.ajax({
        url: window.location.href+'&requisicao=verificaPreenchimentoQuestionario',
        global: false,
        async: false,
        method: 'POST',
        success: function(res){
        	$('#acao').html(res);
        }
    });
    $(document).on( 'click', '.finalizar', function(){
    	window.location.href = window.location.href+'&requisicao=finalizarPreenchimentoQuestionario';
    });
    $(document).on( 'click', '.reabrir', function(){
    	window.location.href = window.location.href+'&requisicao=reabrirPreenchimentoQuestionario';
    });
    $(document).on( 'click', '.vencido', function(){
    	sweetAlert("Prazo Encerrado", "O questionário só pode ser reaberto dentro do prazo de vigência. (De <?php echo $arrVigencia['dt_inicio'] ?> até <?php echo $arrVigencia['dt_fim'] ?>)", "error");
    });
    $(document).on( 'click', '.historico', function(){
    	$.ajax({
            //url: 'geral/workflow/historico.php?requisicao=historicoBootstrap&docid=<?php echo $docid;?>&sislayoutbootstrap=zimec',
            url: window.location+'&requisicao=historicoBootstrap&docid=<?php echo $docid;?>&sislayoutbootstrap=zimec',
            global: false,
            async: false,
            method: 'POST',
            success: function(res){
            	$('#conteudo_modal').html(res);
            }
        });
    });
});
$(document).ajaxStop(function(){
	$.ajax({
        url: window.location.href+'&requisicao=verificaPreenchimentoQuestionario',
        global: false,
        async: false,
        method: 'POST',
        success: function(res){
        	$('#acao').html(res);
        }
    });
    attachInit();
    attachEvents();
});
$( ".printMenu" ).attr("href", "javascript:imprimeQ(133)");
</script>

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
                $url = 'par3.php?modulo=principal/planoTrabalho/bandaLargaQuestionario&acao=A&inuid='.$_REQUEST['inuid'];
                $abasQuestoes= array(
						0 => array( "descricao" => "Questões Estratégicas", "link"	  								=> "par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid=".$_REQUEST['inuid']),
                		1 => array( "descricao" => "Plano Nacional de Educação: Questões Complementares", "link"	=> "par3.php?modulo=principal/planoTrabalho/questoesPNE&acao=A&inuid=".$_REQUEST['inuid']),
                		2 => array( "descricao" => "Banda Larga", "link"	  										=> "par3.php?modulo=principal/planoTrabalho/bandaLargaQuestionario&acao=A&inuid=".$_REQUEST['inuid'])
                );
                ?>
                <div class="ibox-content-round-gray ibox-content">
                    <?php
						$msgDesabilitadoPNE = 'O questionário de banda larga só estará disponíveis para edição no periodo de {dt_inicio} a {dt_fim}';
						echo $simec->tab ( $abasQuestoes , $url );?>
                    <div class="ibox-title">
                        <div class="row">
                            <div class="col-md-1">
                                Vigência:
                            </div>
                            <div class="col-md-3">
                                <h3>De <?php echo $arrVigencia['dt_inicio'] ?> até <?php echo $arrVigencia['dt_fim'] ?></h3>
                            </div>
                            <div class="col-md-1">
                                Situação:
                            </div>
                            <div class="col-md-1">
                                <h3><?php echo $modelEstadoDocumento->esddsc?></h3>
                            </div>
                            <div class="col-md-1">
                                Ação:
                            </div>
                            <div class="col-md-3" id="acao">
                            </div>
                            <div class="col-md-1">
                                Histórico:
                            </div>
                            <div class="col-md-1" style="color:green;font-size:33px;">
                                <i  class="fa fa-code-fork historico"
                                    aria-hidden="true"
                                    style="cursor:pointer"
                                    title="Histórico de tramitações"
                                    data-toggle="modal"
            						data-target="#modal"></i>
                            </div>
                        </div>
                    </div>
                    <?php
                        $telaPNE = new TelaBootstrap(array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 500, 'relatorio' => 'modulo=relatorio/relatorioQuestoesEstrategicas&acao=A', 'habilitado' => $habilitado, 'msgDesabilitadoPNE' => $msgDesabilitadoPNE));
                    ?>
    			</div>
			</div>
		</div>
	</div>
</div>
<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog  modal-lg" id="modal_html">
        <div class="ibox-content-round-gray ibox-content" id="conteudo_modal">
        </div>
    </div>
</div>
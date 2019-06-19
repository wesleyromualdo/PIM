<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "obras2/classes/modelo/QuestoesCumprimentoObjetoAnexos.class.inc";
include_once APPRAIZ . "obras2/classes/controle/CampoExternoControle.class.inc";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_GESTOR_UNIDADE,PFLCOD_SUPERVISOR_UNIDADE);
$obra = new Obras($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];
$qrpid = $_REQUEST['qrpid'];
$coid = $_REQUEST['coid'];

if( $_REQUEST['requisicao'] == 'download' ){
	ob_get_clean();
	$file = new FilesSimec();
	$file->getDownloadArquivo( $_REQUEST['arqid'] );
	#echo "<script>window.close();</script>";
	die;
}

if(!$coid || !$qrpid){
    die("<script>
                alert('Faltam parâmetros para acessar esta tela!');
                location.href = '?modulo=cadQuestionarioCumprimentoObjeto&acao=C';
             </script>");
}

if( $_REQUEST['requisicao'] == 'formEscolas' ){
    ob_clean();
    $obQuest = new QuestoesCumprimentoObjetoAnexos();
    $obQuest->desenhaFormEscolas($_REQUEST);
    die();
}

if( $_REQUEST['requisicao'] == 'atualizaEscolaObra' ){
    ob_clean();
    $obQuest = new QuestoesCumprimentoObjetoAnexos();
    $obQuest->atualizaEscolaObra($_REQUEST);
    die();
}

if( $_REQUEST['deleta'] && $_REQUEST['arqid'] ){
	$obQuest = new QuestoesCumprimentoObjetoAnexos();
	$obQuest->deletaAnexo( $_REQUEST['arqid'] );
}

if( $_POST['salvar_questionario'] && $_POST['identExterno'] && $_FILES ){
	$obMonta = new CampoExternoControle();
	$obMonta->salvar();
}

$habilitaQuestionario = 'N';
if (possui_perfil($pflcods)) {
    $habilitaQuestionario = 'S';
}

$estadoWorkflowObra = wf_pegarEstadoAtual($obra->docid);
$_SESSION['obras2']['esdid'] = $estadoWorkflowObra;
$habilPag = false;
if($estadoWorkflowObra) {
    if($estadoWorkflowObra['esdid'] != ESDID_OBJ_CONCLUIDO && $estadoWorkflowObra['esdid'] != ESDID_OBJ_INACABADA && $estadoWorkflowObra['esdid'] != ESDID_OBJ_CANCELADO) {
        echo "<script>alert('Este questionário será liberado após a conclusão da obra.'); window.close();</script>";
    }
    $cumprimentoObjetoQuestionario = new CumprimentoObjetoQuestionario();
    $cumprimentoObjetoQuestionario->verificaExistencia($coid);
    $qrpid = $cumprimentoObjetoQuestionario->qrpid;
}
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<script>
    function excluirArquivo( arqid ){
        divCarregando();
        if (confirm('Tem certeza que deseja excluir esse anexo?')){
            jQuery.ajax({
               type: "POST",
               url: "obras2.php?modulo=principal/popupQuestionarioCumprimento&acao=A",
               data: "&deleta=true&arqid="+arqid,
               success: function(msg){
                    alert('Anexo excluído com sucesso!');
                    quest.atualizaTela();
                    divCarregado();
               }
             });
        } else {
            divCarregado();
            return false;
        }
    }
    jQuery(document).ready(function(){
    	jQuery(document).on('click', '.listar_escolas', function(){
    		var qrpid = jQuery(this).attr('qrpid');
    		var perid = jQuery(this).attr('perid');
    		var obrid = <?php echo $_REQUEST['obrid']?>;
    	    var param = '&requisicao=formEscolas&qrpid='+qrpid+'&perid='+perid+'&obrid='+obrid;
    	    jQuery.ajax({
           		type: "POST",
           		url: window.location.href,
           		data: param,
           		async: false,
           		success: function(resp){
           			jQuery('#html_modal-form').html(resp);
           			jQuery('#modal-form .ibox-title').empty().append('<button class="close" aria-label="Close" data-dismiss="modal" type="button"><span aria-hidden="true">×</span></button><h4>Selecione as Escolas.</h4>');
           			jQuery('#modal-form').dialog({
           		      height: 500,
           		      width: 800,
           		      title: 'Selecione a escola.'});
           		}
         	});
        });
    });
</script>
<style>
    fieldset {
        border: 1px solid #c0c0c0!important;
        margin: 0 2px!important;
        padding: 0.35em 0.625em 0.75em!important;
    }
</style>
<body>
<?
$tela = new Tela(
    array(
        'qrpid' => $qrpid,
        'tamDivArvore' => 25,
        'tamDivPx' => 250,
        'habilitado' => $habilitaQuestionario
    )
);
?>
<div id="modal-form" class="modal fade" aria-hidden="true" style="display:none">
	<div class="modal-dialog">
		<div id="html_modal-form">
		</div>
	</div>
</div>
</body>
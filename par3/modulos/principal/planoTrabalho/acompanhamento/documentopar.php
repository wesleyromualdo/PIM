<?php
$inuid = $_REQUEST['inuid'];

$controllerDocumentoTermo               = new Par3_Controller_DocumentoTermo();
$obInstrumento                          = new Par3_Controller_InstrumentoUnidade();
$controllerParLegado                    = new Par3_Controller_DocumentoLegado();

include_once APPRAIZ.'par3/classes/controller/WS_Sigarp_FNDE.class.inc';
require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';

switch ($_REQUEST['requisicao']) {
    case 'carregaDadosBancarios':
        //ver($_REQUEST); die;
        echo "carregaDadosBancarios";

        //echo carregaDadosBancarios($_REQUEST['proid']);
        die;
        break;
    case 'verificaRegrasProrrogacaoPrazo':
        ob_clean();
        $retorno = $controllerDocumentoTermo->verificaRegrasTermos( $_REQUEST['dotid'], 'prazo' );
        
        echo simec_json_encode($retorno);
        exit();
        break;
    case 'verificaRegrasReprogramacaoIniciativa':
        ob_clean();
        $retorno = $controllerDocumentoTermo->verificaRegrasTermos( $_REQUEST['dotid'], 'iniciativa' );        
        echo simec_json_encode($retorno);
        exit();
        break;
    case 'listaHistoricoPAR3':
        ob_clean();
        $_REQUEST['dotid'] = $_POST['dados'][0];
        Par3_Controller_DocumentoTermo::listaHistoricoPAR3($_REQUEST);
        echo "<script>$('#PAR nav').remove();</script>"; //para remover os botões do componente na segunda lista.
        die();
    break;
    case 'visualizarTermoPar3':
        ob_clean();
        $mdoconteudo = $controllerDocumentoTermo->pegaTermoCompromissoArquivo($_REQUEST['dotid']);
        $mdoconteudo = str_ireplace('\"', '"', $mdoconteudo);
        echo simec_html_entity_decode($mdoconteudo);
        
        exit();
        break;
    case 'pegaSituacaoReformulacao':
        ob_clean();
        $sql = "SELECT rd.refid, rd.proid, rd.dotid, es.esddsc
                FROM par3.reformulacao_documento rd
                	INNER JOIN workflow.documento d ON d.docid = rd.docid
                	INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                WHERE rd.refstatus = 'A' and rd.tirid = 2 AND rd.refidpai IS NOT NULL 
                    AND rd.proid = {$_REQUEST['proid']}
                    AND rd.dotid = {$_REQUEST['dotid']}
                ORDER BY refid DESC";
        $arStatus = $db->pegaLinha($sql);
        if( empty($arStatus['esddsc']) ){
            echo '0';
        } else {
            echo simec_json_encode($arStatus);
        }
        
    exit();
    break;
    case 'pegaSituacaoReformulacaoPrazo':
        ob_clean();
        $sql = "SELECT rd.refid, rd.proid, rd.dotid, es.esddsc
                FROM par3.reformulacao_documento rd
                	INNER JOIN workflow.documento d ON d.docid = rd.docid
                	INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                WHERE rd.refstatus = 'A' and rd.tirid = 1 AND rd.refidpai IS NOT NULL 
                    AND rd.proid = {$_REQUEST['proid']}
                    AND rd.dotid = {$_REQUEST['dotid']}
                ORDER BY refid DESC";
        $arStatus = $db->pegaLinha($sql);
        if( empty($arStatus['esddsc']) ){
            echo '0';
        } else {
            echo simec_json_encode($arStatus);
        }
        
    exit();
    break;
    case 'verificaAceiteTermo':
        ob_clean();
        $arProcesso = $controllerDocumentoTermo->permissaoAceiteTermo($_REQUEST['dotid'], $_REQUEST['inuid']);
        
        if( $arProcesso == false ) {
            echo 'S';
        } else {
            echo 'N';
        }
        exit();
        break;
    case 'verificaTemContrapartida':
        ob_clean();
         $arTermo = $db->pegaLinha("SELECT proid, refid FROM par3.documentotermo WHERE dotid = {$_REQUEST['dotid']}");
         $proid = $arTermo['proid'];
         $refid = $arTermo['refid'];
         
         if( $refid ){
             $refvalorentidade = $db->pegaUm("SELECT refvalorentidade FROM par3.reformulacao_documento WHERE refid = {$refid}");
             if( $refvalorentidade > 0 ){
                 echo 'S';
             } else {
                 echo 'N';                 
             }
         } else {         
            $dotvalorcontrapartida = $db->pegaUm("SELECT vlr_contrapartida FROM par3.v_dados_processo WHERE proid = $proid");
            
            if ((float)$dotvalorcontrapartida > (int)0) {
                echo 'S';
            } else {
                echo 'N';
            }
         }
        exit();
        break;
    case 'imprimir':
        ob_clean();
//        if ($_REQUEST['domid']) {
//            $controllerDocumentoTermo->imprimirTermoManifestoPDF($_REQUEST['domid']);

        $controllerDocumentoTermo->imprimirTermoPDF($_REQUEST['dotid']);
        exit();
    break;
    case 'rejeitarTermo_contrapartida':
        ob_clean();
        $db->executar("UPDATE par3.documentotermo SET dotstatus = 'I', dotrejeitado = 'S' WHERE dotid = {$_REQUEST['dotid']}");
        $db->commit();
        exit();
    break;
    case 'aceitarTermo':
        ob_clean();
        $controllerDocumentoTermo->salvarAceiteTermo($_REQUEST['dotid']);
        exit();
    break;
    case 'redirecionamentoPar2':
        $_SESSION['par']['itrid']  = $_POST['itridpar2'];
        $_SESSION['par']['inuid']  = $_POST['inuidpar2'];
        $_SESSION['par']['estuf']  = $_POST['estufpar2'];
        $_SESSION['par']['muncod'] = $_POST['muncodpar2'];
        $urlpar2 = $_POST['urlpar2'] . $_POST['dopid'];
        simec_redirecionar($urlpar2);
        break;
    case 'redirecionamentoObraPar3':
        global $db;

        $sql = "SELECT emp.orgid, o.empid
                FROM obras2.obras o
                LEFT JOIN obras2.empreendimento emp ON emp.empid = o.empid
                WHERE o.obrid = {$_POST['obrid']}";
        $dadosObra = $db->pegaLinha($sql);

        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $_SESSION['obras2']['orgid'] = $dadosObra['orgid'];
        $_SESSION['obras2']['empid'] = $dadosObra['empid'];
        simec_redirecionar('/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid='.$_POST['obrid']);
}

//Requisiçao documentoLegado
switch ($_POST['req']){
    case 'vizualizaDocumento':
        ob_clean();
        $documento = new Par3_Model_DocumentoParLegado();
        $documento->formVizualizaDocumento($_REQUEST);
        die();
        break;
    case 'vizualizaDocumentoPAC':
        ob_clean();
        $documento = new Par3_Model_DocumentoParLegado();
        $documento->formVizualizaDocumentoPAC($_REQUEST);
        die();
        break;
}

$itrid = $obInstrumento->pegarItrid($_REQUEST['inuid']);

$url = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=documentos';
global $db;
$db->cria_aba(58014, $url, '&inuid=' . $inuid, array());
//die;
?>
<script language="javascript" src="js/documentoLegado.js"></script>
<style>
/*  virando o ícone de acompanhamento para o outro lado  */
    .icon-flipped {
        transform: scaleX(1) !important;
        -moz-transform: scaleX(1) !important;
        -webkit-transform: scaleX(1) !important;
        -ms-transform: scaleX(1) !important;
    }

    .termo_detalhe{
        cursor:pointer;
        color:blue;
    }
    .termo_detalhe:hover{
        cursor:pointer;
        color:#87CEFA;
    }
    /*.termo_detalhe_manifesto{
        cursor:pointer;
        color:blue;
    }
    .termo_detalhe_manifesto:hover{
        cursor:pointer;
        color:#87CEFA;
    }*/
    .termo_detalhe_par2{
        cursor:pointer;
        color:blue;
    }
    .termo_detalhe_par2:hover{
        cursor:pointer;
        color:#87CEFA;
    }
</style>
<div id="debug"></div>
<div class="ibox-content-round-gray ibox-content">
<!-- Legenda -->
<div class="row">
    <div class="col-lg-12 pull-left">
        <b>Legenda:</b>
        <div class="fa fa-plus-circle success"></div> Histórico do termo |
        <div class="fa fa-cog danger"></div> Reprogramação |
        <div class="fa fa-share primary"></div> Acompanhamento <i>(Termo de Referência, Contrato, Nota fiscal, Monitoramento)</i> |
        <div class="fa fa-check primary"></div> Validar Termo
    </div>
</div>
<br>
<div class="row">
	<div class="col-lg-12 pull-left">
		<ul style="font-size: 14px;">
            <li><i>* Clique no número do processo para visualizar as informações do processo</i></li>
            <li><i>** Clique no nome do tipo de documento para visualizar o termo</i></li>
        </ul>
	</div>
</div>
    <!-- Documentação Pendente -->
    <div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
        <form method="post" name="formulario" id="formulario" class="form form-horizontal">
        <input type="hidden" name="nome_termo" id="nome_termo" value=""/>
        <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid; ?>"/>

            <?php
            if ($controllerDocumentoTermo->verificaDocumentosPendentes($inuid)) { ?>
                <div class="ibox">
                    <div class="ibox-title">
                        <div class="row">
                            <div class="col-md-12" >
                                <h3 class="center">Documentos pendentes de validação</h3>
                            </div>
                        </div>
                    </div>
                        <div class="ibox-content" id="PAR" style="overflow: auto;">
                            <?php
                                echo $controllerDocumentoTermo->listaPAR3(array('dotstatus' => 'A', 'inuid' => $inuid, 'pendente' => 1));
                            ?>
                        </div>
                </div>
            <?php } ?>

        </form>
    </div>

    <!-- Documentos PAR 3 -->
    <div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
        <form method="post" name="form_par3" id="form_par3" class="form form-horizontal">

            <input type="hidden" name="obrid"  id="obrid"   value=""/>
            <input type="hidden" name="requisicao"  id="requisicao"   value="redirecionamentoObraPar3"/>

            <div class="ibox">
                <div class="ibox-title">
                    <div class="row">
                        <div class="col-md-12" >
                            <h3 id="entidade" class="center">Documentos do PAR 33333</h3>
                        </div>
                    </div>
                </div>
                    <div class="ibox-content" id="PAR" style="overflow: auto;">
                        <?php
                        $localizacao = $db->pegaUm("SELECT estuf FROM par3.instrumentounidade WHERE inuid = {$_REQUEST['inuid']}");


                        echo $controllerDocumentoTermo->listaPAR3(array('dotstatus' => 'A', 'inuid' => $inuid, 'pendente' => 0));
                        ?>
                    </div>
            </div>
        </form>
    </div>
<?php //die; ?>
<!--     Documentos PAR 2 -->
    <div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
        <form method="post" name="form_par2" id="form_par2" class="form form-horizontal">

            <input type="hidden" name="inuidpar2"  id="inuidpar2"     value=""/>
            <input type="hidden" name="dopid"      id="dopid"         value=""/>
            <input type="hidden" name="estufpar2"  id="estufpar2"     value=""/>
            <input type="hidden" name="muncodpar2" id="muncodpar2"    value=""/>
            <input type="hidden" name="itridpar2"  id="itridpar2"     value=""/>
            <input type="hidden" name="requisicao" id="requisicao"    value="redirecionamentoPar2"/>
            <input type="hidden" name="urlpar2"    id="urlpar2"       value=""/>

            <div class="ibox">
                <div class="ibox-title">
                    <div class="row">
                        <div class="col-md-12" >
                            <h3 id="entidade" class="center">Documentos do PAR 2</h3>
                        </div>
                    </div>
                </div>
                    <div class="ibox-content" id="PAR" style="overflow: auto;">
                        <?php
                        
                            if ($inuid) {
                                $modelInstrumentounidade = new Par3_Model_InstrumentoUnidade($inuid);

                                if ($modelInstrumentounidade->itrid == '2') {
                                    $wherePac = "tc.muncod = '{$modelInstrumentounidade->muncod}'";
                                } else {
                                    $wherePac = "tc.estuf = '{$modelInstrumentounidade->estuf}'";
                                }
                            } 
                            //Cria a lista de documentos do PAR 2011
//                            echo Par3_Controller_DocumentoLegado::listaPAR(Array('dopstatus' => 'A', 'inuid' => $inuid, 'wherepac' => $wherePac));
                        ?>
                    </div>
            </div>
        </form>
    </div>

</div>
<br>
<br>
<script>
    $('[data-toggle="popover"]').popover();

    $(document).ready(function()
    {
        $('body').on('click','.termo_detalhe', function(){
            id = $(this).find('.dotid').attr('value');
            visualizarTermoPar3(id);
        });

        $('body').on('click','.termo_detalhe_par2', function(){
            var dopid = $(this).find('.dopid').attr('value');
            if(dopid > 0){
                vizualizarTermo(dopid);
            }else{
                var terid = $(this).find('.terid').attr('value');
                vizualizarTermoPAC(terid);
            }
        });

        jQuery('body').on('click','#btn-prorrogacao', function(){
        	var dotid = jQuery('[name="dotid_reformulacao"]').val();
        	var proid = jQuery('[name="proid_reformulacao"]').val();
        	var inuid = jQuery('[name="inuid"]').val();
        	
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaProrrogacaoPrazo&acao=A&dotid='+dotid+'&inuid='+inuid+'&proid='+proid+'&gerar=S';
        });
        
        jQuery('body').on('click','#btn-reprogramacao', function(){
        	var dotid = jQuery('[name="dotid_reformulacao"]').val();
        	var proid = jQuery('[name="proid_reformulacao"]').val();
        	var inuid = jQuery('[name="inuid"]').val();
        	
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid='+dotid+'&inuid='+inuid+'&proid='+proid+'&gerar=S';
        });

        $(document).find('#loading').remove();

    });
    function verificaRegrasProrrogacaoPrazo(dotid){
    	jQuery("#div_msg_regras_prazo").html('');
    	var caminho = window.location.href;
        var action  = '&requisicao=verificaRegrasProrrogacaoPrazo&dotid='+dotid;
        jQuery.ajax({
             type: "POST",
             url: caminho,
             data: action,
             async: false,
             success: function (resp) {
                 //console.log(resp);
            	 var arRetorno = jQuery.parseJSON(resp);
            	 if( arRetorno['habilita'] == 'N' ){
            		 jQuery('#btn-prorrogacao').attr('disabled', true);
            		 jQuery("#div_msg_regras_prazo").html(arRetorno['msg']);
                 } else {
                	 jQuery('#btn-prorrogacao').attr('disabled', false);
                	 jQuery("#div_msg_regras_prazo").html('');
                 }
             }
         });
    }
    
    function verificaRegrasReprogramacaoIniciativa(dotid){
    	jQuery("#div_msg_regras_iniciativa").html('');
    	var caminho = window.location.href;
        var action  = '&requisicao=verificaRegrasReprogramacaoIniciativa&dotid='+dotid;
        jQuery.ajax({
             type: "POST",
             url: caminho,
             data: action,
             async: false,
             success: function (resp) {
                 
            	 var arRetorno = jQuery.parseJSON(resp);

            	 if( arRetorno['habilita'] == 'N' ){
            		 jQuery('#btn-reprogramacao').attr('disabled', true);
                	 jQuery("#div_msg_regras_iniciativa").html(arRetorno['msg']);
                 } else {
                	 jQuery('#btn-reprogramacao').attr('disabled', false);
                	 jQuery("#div_msg_regras_iniciativa").html('');
                 }
             }
         });
    }
    
    function abreReformulacao(proid, dotid){
    	var inuid = jQuery('[name="inuid"]').val();
    	
        window.location.href = 'par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid='+dotid+'&inuid='+inuid+'&proid='+proid;
    }    
    function abreReformulacaoPrazo(proid, dotid){
    	var inuid = jQuery('[name="inuid"]').val();
    	
        window.location.href = 'par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaProrrogacaoPrazo&acao=A&dotid='+dotid+'&inuid='+inuid+'&proid='+proid;
    }
    
    function modalReprogramacao(id, proid){
    	
    	jQuery('.loading-dialog-par3').show();
    	jQuery('[name="dotid_reformulacao"]').val(id);
    	jQuery('[name="proid_reformulacao"]').val(proid);
    	jQuery("#modal-reformulacao").modal("show");

    	pegaSituacaoReformulacao(id, proid);
    	pegaSituacaoReformulacaoPrazo(id, proid);

    	verificaRegrasProrrogacaoPrazo(id);
    	verificaRegrasReprogramacaoIniciativa(id);
    	jQuery('.loading-dialog-par3').hide();
    }
    
    function pegaSituacaoReformulacao(dotid, proid){
       var caminho = window.location.href;
       var action  = '&requisicao=pegaSituacaoReformulacao&proid='+proid+'&dotid='+dotid;
       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                if( resp != '0' ){
                	var arRetorno = jQuery.parseJSON(resp);                	
                    var html = 'Última: <b>'+arRetorno['esddsc']+'</b>(<a><span onclick="abreReformulacao('+arRetorno['proid']+', '+arRetorno['dotid']+');" style="cursor: pointer;"><i>clique aqui para abrir</i></span></a>)';
                    jQuery("#div_situacao").html(html);
                } else {
                	jQuery("#div_situacao").html('');
                }
            }
        });
    }
    
    function pegaSituacaoReformulacaoPrazo(dotid, proid){
       var caminho = window.location.href;
       var action  = '&requisicao=pegaSituacaoReformulacaoPrazo&proid='+proid+'&dotid='+dotid;
       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //rd.refid, rd.proid, rd.dotid, es.esddsc
                //console.log(resp);
                if( resp != '0' ){
                	var arRetorno = jQuery.parseJSON(resp);                	
                    var html = 'Última: <b>'+arRetorno['esddsc']+'</b>(<a><span onclick="abreReformulacaoPrazo('+arRetorno['proid']+', '+arRetorno['dotid']+');" style="cursor: pointer;"><i>clique aqui para abrir</i></span></a>)';
                    jQuery("#div_situacao_prazo").html(html);
                } else {
                	jQuery("#div_situacao_prazo").html('');
                }
            }
        });
    }
    
    function visualizarTermoPar3(id){

        jQuery('[name="nome_termo"]').val(id);

        jQuery('.btn-aceite-termo').hide();
        jQuery('.btn-rejeitar-termo').hide();
        
        var caminho = window.location.href;
        var action  = '&requisicao=visualizarTermoPar3&dotid='+id;
       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery("#div_conteudo_termo").html(resp);
                jQuery('[name="dotid"]').val(id);
            }
        });
//       jQuery('.btn-aceite-termo-manifesto').hide();
       jQuery("#modal-visualiza-termo").modal("show");
       verificaAceiteTermo( id );
    }

    function verificaAceiteTermo( dotid ){
        
        var caminho = window.location.href;
        var action  = '&requisicao=verificaAceiteTermo&dotid='+dotid;
       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                
                //jQuery("#conteudo_termo").html(resp);
                if( resp == 'S' ){
                    jQuery('.btn-aceite-termo').hide();
                    jQuery('.btn-rejeitar-termo').hide();
                }else{
                    var temContra = verificaTemContrapartida( dotid );
                    
                    if( temContra == 'N'){
                        jQuery('.btn-rejeitar-termo').hide();
                    } else {
                        jQuery('.btn-rejeitar-termo').show();
                    }
                    
                    jQuery('.btn-aceite-termo').show();
                }
            }
        });
    }

    function imprimirTermo(){
        var dotid = jQuery('[name="dotid"]').val();
        var domid = jQuery('[name="domid"]').val();
        window.location.href = window.location+'&requisicao=imprimir&dotid='+dotid+'&domid='+domid;
    }

    function imprimirTermoParAntigo(){
        var dotid = jQuery('[name="dotid"]').val();
        var domid = jQuery('[name="domid"]').val();
        window.location.href = window.location+'&requisicao=imprimir&dotid='+dotid+'&domid='+domid;
    }

    function btn_aceitarTermo(){
        var temContra = verificaTemContrapartida( jQuery('[name="dotid"]').val() );
        
        if( temContra == 'S' ){
            var msg = "Eu li, e estou de acordo com o termo de compromisso e ciente dos valores de contrapartida";
        } else {
            var msg = "Eu li, e estou de acordo com o termo de compromisso.";
        }
        jQuery('.btn-aceite-termo').button('loading');
        swal({
                title: "Confirmar",
                text: msg,
                type: "success",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                 swal.close();
                    aceitarTermo( jQuery('[name="dotid"]').val() );
                }
            }
        );
    }

    function btn_rejeitarTermo(){
        divCarregando();
        
        var dotid = jQuery('[name="dotid"]').val();

        var caminho = window.location.href;
        var action  = '&requisicao=rejeitarTermo_contrapartida&dotid='+dotid;
       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                swal({
                    title: "Confirmar",
                    text: "Termo com contrapartida rejeitado com sucesso",
                    type: "success",
                    html: true,
                    showCancelButton: false,
                    cancelButtonText: "Não",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        swal.close();
                        window.location.href = window.location;
                    }
                }
            );
                
            }
        });
    }

    function verificaTemContrapartida( dotid ){
        var retorno;
        var caminho = window.location.href;
        var action  = '&requisicao=verificaTemContrapartida&dotid='+dotid;
       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //jQuery("#debug").html(resp);
                retorno = resp;
            }
        });
       return retorno;
    }

    function aceitarTermo( dotid ){
        divCarregando();
        window.location.href = window.location+'&requisicao=aceitarTermo&dotid='+dotid;
    }

    function detalharAcompanhamento( dotid, proid, obrid, totalobrid ){
        if(totalobrid > 1){
            window.location.href = window.location.origin + '/obras2/obras2.php?modulo=principal/listaObras&acao=A';
        }else if(obrid > 0){
            // acompanhamento de obra
            jQuery('#form_par3 #obrid').val(obrid);
            jQuery('#form_par3').submit();
        }else{
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=acompanhamento&inuid=' +
                    <?= $_GET['inuid'] ?> + '&proid=' + proid;
        }
    }

    function redirecionarAcompanhamentoPar2(dopid, inuid, estuf, muncod, itrid){
    	jQuery('#dopid').val(dopid);
    	jQuery('#inuidpar2').val(inuid);
    	jQuery('#estufpar2').val(estuf);
    	jQuery('#muncodpar2').val(muncod);
    	jQuery('#itridpar2').val(itrid);
    	jQuery('#urlpar2').val('/par/par.php?modulo=principal/acompanhamentoDocumentos&acao=A&dopid=');
    	jQuery('#form_par2').submit();
    }

    function redirecionarPrestacaoContasPar2(dopid, inuid, estuf, muncod, itrid){
    	jQuery('#dopid').val(dopid);
    	jQuery('#inuidpar2').val(inuid);
    	jQuery('#estufpar2').val(estuf);
    	jQuery('#muncodpar2').val(muncod);
    	jQuery('#itridpar2').val(itrid);
    	jQuery('#urlpar2').val('/par/par.php?modulo=principal/prestacaoContas&acao=A&dopid=');
    	jQuery('#form_par2').submit();
    }
</script>

<!-- Modal Termo -->
<div class="ibox float-e-margins animated modal conteudo" id="modal-visualiza-termo" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%" id="printable">
        <div class="modal-content">
            <div class="ibox-footer notprint" style="text-align: center;">
                <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
                <button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermo();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
                    <button type="button" id="btn-aceite-termo" data-dismiss="modal" class="btn btn-success btn-aceite-termo" onclick="btn_aceitarTermo();" data-loading-text="Aceite, aguarde ..."><i class="fa fa-times-circle-o"></i> Aceitar</button>
                    <button type="button" id="btn-rejeitar-termo" data-dismiss="modal" class="btn btn-danger btn-rejeitar-termo" onclick="btn_rejeitarTermo();" data-loading-text="Rejeitar, aguarde ..."><i class="fa fa-times-circle-o"></i> Rejeitar Contrpartida</button>
            </div>
            <div class="ibox-content">
                <div class="row">
                    <div class="ibox-content" id="conteudo_termo">
                    <?php echo $controllerDocumentoTermo->montaHtmlTermo(); ?>
                    </div>
                </div>
            </div>
            <div class="ibox-footer notprint" style="text-align: center;">
                <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
                <button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermo();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
                    <button type="button" id="btn-aceite-termo" data-dismiss="modal" class="btn btn-success btn-aceite-termo" onclick="btn_aceitarTermo();" data-loading-text="Aceite, aguarde ..."><i class="fa fa-times-circle-o"></i> Aceitar</button>
                    <button type="button" id="btn-rejeitar-termo" data-dismiss="modal" class="btn btn-danger btn-rejeitar-termo" onclick="btn_rejeitarTermo();" data-loading-text="Rejeitar, aguarde ..."><i class="fa fa-times-circle-o"></i> Rejeitar Contrpartida</button>
            </div>
        </div>
    </div>
</div>

<!-- Modal Reprogramação -->
<div class="ibox float-e-margins animated modal conteudo" id="modal-reformulacao" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 50%">
        <div class="modal-content">
            	<div class="ibox-title">
                    <div class="row">
                        <div class="col-md-12">
                            <h2 class="center">Reprogramação</h2>
                        </div>
                    </div>
                </div>
            	<div class="ibox-content" >
                	<div class="row" style="text-align: center;">
                			<input type="hidden" name="dotid_reformulacao" id="dotid_reformulacao" value="">
                			<input type="hidden" name="proid_reformulacao" id="proid_reformulacao" value="">
                    		<div class="col-lg-6">
                					<button type="button" id="btn-prorrogacao" data-dismiss="modal" class="btn btn-success btn-lg" ><i class="fa fa-calendar"></i> Prorrogação de Prazo</button>
                					<div class="row">
    									<div class="col-lg" id="div_situacao_prazo"></div>                	
                					</div>
                    		</div>
                    		<div class="col-lg-6">
            					<button type="button" id="btn-reprogramacao" data-dismiss="modal" class="btn btn-primary btn-lg" ><i class="fa fa-edit"></i> Reprogramação de iniciativa</button>
            					<div class="row">
									<div class="col-lg" id="div_situacao"></div>                	
            					</div>
                    		</div>
                    		
                    </div>
                    <div class="row" style="text-align: center;">
						<div class="col-lg" id="div_msg_regras_prazo" style="color: red; font-weight: bold;"></div>                	
						<div class="col-lg" id="div_msg_regras_iniciativa" style="color: red; font-weight: bold;"></div>                	
					</div>
                </div>
            <div class="ibox-footer notprint" style="text-align: center;">
                <button type="button" id="btn-fechar-reformulacao" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
            </div>
        </div>
    </div>
</div>

<!-- Modal termo par antigo -->
<div id="modal-form" class="modal fade" aria-hidden="true">
    <div class="modal-dialog" style="width: 70%;">
        <div class="ibox-title" style="text-align: center;">
            <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
            <button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermo();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form" class="ibox-content">
        </div>
        <div class="ibox-footer notprint" style="text-align: center;">
            <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
            <button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermo();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
        </div>
    </div>
</div>
<?php
$obEmenda = new Par3_Controller_EmendasParlamentares();

require_once APPRAIZ.'/includes/workflow.php';

switch ($_REQUEST['requisicao']){
	case 'tramitar_emenda':
		ob_clean();
		$obEmenda->tramitarEmendasEntidade($_REQUEST);
		die;
	case 'verificaValorEmpenho':
	    echo $obEmenda->verificaValorEmpenho( $_REQUEST['inpid'], $_REQUEST['edeid'] );
	    die;
	    break;
	case 'SalvarDetalheItensComposicao':
		ob_clean();
		$obEmenda->salvarIniciativaEmendaItemComposicao($_REQUEST);
		die;
	case 'salvar-dados-obras':
		ob_clean();
		echo $obEmenda->salvarDadosObrasEmendas($_REQUEST);
		die;
	case 'salvar-valor-parlamentar':
		ob_clean();
		echo $obEmenda->salvarValorParlamentar($_REQUEST);
		die;
	case 'salvarContraPartida':
		ob_clean();
		echo $obEmenda->salvarContraPartidaIniciativaEmenda($_REQUEST);
		die;
	case 'SalvarDetalheItensComposicaoEscola':
		ob_clean();
		$obEmenda->salvarIniciativaEmendaItemComposicaoEscola($_REQUEST);
		die;
	case 'detalharItensComposicao':
		ob_clean();
		
		$intoid = $_REQUEST['dados'][3];
		if( $intoid == 1 ){
		    echo $obEmenda->listarObrasBeneficiarioComVinculoParlamentar($_REQUEST);
		} else {
		    echo $obEmenda->listarItemComposicaoIniciativaBeneficiarioComVinculoParlamentar($_REQUEST);
		}
// 		echo "<script>
//                 jQuery(document).ready(function(){
//                     if( jQuery('[name=\"bo_vincular\"]').val() == 'N' ){
//                         jQuery('#form-lista-emenda input').attr( 'disabled', true );
//                     }
//                 });
//                 </script>";
		die;
	case 'buscaModalEscolas':
		ob_clean();
		require_once APPRAIZ . 'par3/classes/controller/IniciativaPlanejamentoItemComposicaoEscola.class.inc';
		require_once APPRAIZ . 'par3/classes/model/IniciativaPlanejamentoItemComposicaoEscola.class.inc';
		require_once APPRAIZ . 'par3/classes/model/IniciativaPlanejamentoItemComposicao.class.inc';
		
		$inpice = new Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola();
		$resp = $inpice->getFormItemComposicaoEscolasComVinculoParlamentar($_REQUEST);
		echo $resp;
		die;
	case 'valida-envio-emenda-aceita':
		ob_clean();
		$sql = "SELECT sum(total) FROM (
                	SELECT count(ieiid) AS total, ineid FROM par3.iniciativa_emenda_item_composicao WHERE ieistatus = 'A' AND ieiquantidade > 0 GROUP BY ineid
                	UNION ALL
                	SELECT count(ieoid) AS total, ineid FROM par3.iniciativa_emenda_obra eo WHERE eo.ieostatus = 'A' AND ieovalor > 0 GROUP BY ineid
                ) AS foo
                	INNER JOIN par3.iniciativa_emenda ie ON ie.ineid = foo.ineid
                WHERE ie.edeid = {$_REQUEST['edeid']}
                GROUP BY foo.ineid";
		echo $db->pegaUm($sql);
		die;
	case 'valor-total-indicado':
		ob_clean();
		$totObra = 0;
		$totValorObra = 0;
		if( $_REQUEST['obrid'] ){
    		$totObra = $db->pegaUm("SELECT sum(ieovalor) FROM par3.iniciativa_emenda_obra WHERE obrid = ".$_REQUEST['obrid']." AND ieostatus = 'A'");
    		$totPlaneja = $db->pegaUm("SELECT sum(ieovalor) FROM par3.iniciativa_emenda_obra WHERE inpid = ".$_REQUEST['inpid']." AND ieostatus = 'A'");
    		$totValorObra = $db->pegaUm("SELECT obrvalor FROM par3.obra WHERE obrid = ".$_REQUEST['obrid']);
		} else {
		    $totPlaneja = $db->pegaUm("SELECT sum(ieovalor) FROM par3.iniciativa_emenda_obra WHERE inpid = ".$_REQUEST['inpid']." AND ieostatus = 'A'");
		}
		$contraPatida = ($totValorObra - $totPlaneja);
		if( $totObra == 0 ) $contraPatida = 0;
		echo json_encode(array($totObra, $totPlaneja, ( ($contraPatida < 0) ? 0 : $contraPatida) ));
		die;
	case 'valor-total-ja-informado':
		ob_clean();
    		$totInformado = $db->pegaUm("SELECT sum(eo.ieovalor) FROM par3.iniciativa_emenda_obra eo
                                    	INNER JOIN par3.iniciativa_emenda ie ON ie.ineid = eo.ineid
                                    WHERE eo.inpid = ".$_REQUEST['inpid']." AND ie.emeid = ".$_REQUEST['emeid']." AND ie.inestatus = 'A' AND eo.ieostatus = 'A'");
    		
    			
    		echo $totInformado;
		die;
}

if(empty($_REQUEST['emeid'])){
    simec_redirecionar('par3.php?modulo=principal/emendas/listaEmendasMunicipio&acao=A', 'error', 'Faltam parametros necessários. Selecione uma emenda!');
}

require APPRAIZ . 'includes/cabecalho.inc';

if( empty($_REQUEST['inuid']) ){
    $db->cria_aba( $abacod_tela, $url, '' );
} else {
    $controleUnidade         = new Par3_Controller_InstrumentoUnidade();
}

$arDadosEmenda = $obEmenda->pegaDadosEmendas($_REQUEST['emeid']);
$arrPerfilRetorno = $obEmenda->verificaPerfilEmendasEntidade( $_REQUEST['edeid'] );

$arrPerfil = pegaPerfils($_SESSION['usucpf']);

$arEstado = $obEmenda->pegaEstadoAtualEmenda($_REQUEST['edeid']);
?>
<style>
.panel-title {
    font-size: 15px;
}
.panel-little {
    font-size: 12px;
}

.table > thead > tr > th, .table > tbody > tr > th, .table > tfoot > tr > th, .table > thead > tr > td, .table > tbody > tr > td, .table > tfoot > tr > td{
    vertical-align: middle;
}
</style>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
<?php   if( empty($_REQUEST['inuid']) ){?>
            <div class="row">
                <div class="col-md-12">
                    <h2 class="center">Detalhar Iniciativa do Parlamentar</h2>
                </div>
            </div>
<?php   } else {?>
        	<h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
<?php   }?>
    </div>
    <div class="ibox-content">
<?php   if( !empty($_REQUEST['inuid']) ){?>
        	<?php echo $controleUnidade->cabecalhoUnidade(); ?>
        	<div class="row">&nbsp;</div>
<?php   }?>
        <div class="row">
            <div class="col-md-12">
            	<table>
                	<tbody>
                		<tr>
                			<td class="style_h5" style="text-align: right !important;"><b>Parlamentar:</b></td>
                			<td class="style_h5">&nbsp;<?php echo $arDadosEmenda['autor']; ?></td>
                		</tr>
                		<tr>
                			<td class="style_h5" style="text-align: right !important;"><b>Ação:</b></td>
                			<td class="style_h5">&nbsp;<?php echo $arDadosEmenda['acao']; ?></td>
                		</tr>
                	</tbody>
                </table>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
			<div class="row">
            	<div class="col-lg-12">
                	<div class="panel panel-default" style="text-align: left;">
                    	<div class="panel-heading"> <h3 class="panel-title">Emenda Selecionada</h3> </div>
                        <div class="panel-body"> <?php echo $obEmenda->pegaBeneficiarioSelecionado($_REQUEST['edeid']); ?> </div>
					</div>
				</div>
			</div>
			<table width="100%" border="0" align="center">
				<tr>
					<td width="90%">
                    <form name="form-lista-emenda" id="form-lista-emenda" class="form-horizontal" method="post">
                    	<input type="hidden" name="requisicao" id="requisicao" value=""/>
                    	<input type="hidden" name="bo_vincular" id="bo_vincular" value="<?php echo $arrPerfilRetorno['vincular']; ?>"/>
                    	<input type="hidden" name="bo_tramitar" id="bo_tramitar" value="<?php echo $arrPerfilRetorno['tramitar']; ?>"/>
                    	<input type="hidden" name="edeid" id="edeid" value="<?php echo $_REQUEST['edeid']; ?>"/>
                    	<input type="hidden" name="emeid" id="emeid" value="<?php echo $_REQUEST['emeid']; ?>"/>
                    	
                    	<input type="hidden" name="aedid" id="aedid" value="<?php echo $_REQUEST['aedid']; ?>"/>
                    	<input type="hidden" name="esdid_atual" id="esdid_atual" value="<?php echo $arEstado['esdid']; ?>"/>
                    	<input type="hidden" name="esdiddestino" id="esdiddestino" value="<?php echo $_REQUEST['esdiddestino']; ?>"/>
                        <div class="row">
                        	<div class="col-lg-12">
                                <div class="panel panel-warning" style="text-align: left;">
                                	<div class="panel-heading"> <h3 class="panel-title">Detalhar Iniciativa do Parlamentar</h3> <span class="panel-little">*Somente as iniciativas indicadas pelo <b>parlamentar</b> serão apresentadas</span></div>
                                	<div class="panel-body"> <?php echo $obEmenda->carregaIniciativaBeneficiarioComVinculoParlamentar($_REQUEST['edeid'])?> </div>
                                </div>
                            </div>
                        </div>
                    </form>
            		</td>
<?php               $arrAcoes = $obEmenda->pegaAcaoWorkFlowEmenda( $arEstado['esdid'] ); ?>
<?php               $arPodeEnviar = $obEmenda->verificaPreenchimentoIniciativa($_REQUEST['edeid']); ?>
<?php               $boEmpenho = $obEmenda->verificaTemEmpenho($_REQUEST['edeid']); ?>
            		
            		<td>&nbsp;&nbsp;</td>
            		<td style="text-align: center;" valign="middle">
            			<div class="panel panel-default">
                            <div class="panel-heading">Fluxo</div>
                            <div class="panel-body" style="text-align: center;">
                                <dl>
                                    <dd>Estado atual</dd>
                                    <dt><?php echo $arEstado['esddsc']; ?></dt>
                                </dl>
<?php                       foreach ($arrAcoes as $v) {

                                if( $v['esdiddestino'] == PAR3_ESD_EMENDA_ACEITA && (float)$arPodeEnviar['qtd'] <= (float)0 ){
                                    $v['boperfil'] = 0;
                                    $strMSG = 'Verifique o preencimento das inicativas!';
                                }
                                
                                /*if( $v['aedid'] == 5368 && $boEmpenho == 'S' ){
                                    $v['boperfil'] = 0;
                                    $strMSG = 'Não é possivel retornar a emenda para análise, pois a emenda já tem empenho solicitado!';
                                }*/

                                $iconBtn = 'btn-success';
                                $iconSpan = 'glyphicon-thumbs-down';
                                if( $v['aedicone'] == '2.png' ){
                                    $iconBtn = 'btn-danger';
                                    $iconSpan = 'glyphicon-thumbs-down';
                                }
                                if( $v['aedicone'] == '3.png' ){
                                    $iconBtn = 'btn-warning';
                                    $iconSpan = 'glyphicon-share-alt';
                                }
                                
                                if( (int)$v['boperfil'] > (int)0 ){?>
                                	<div style="white-space:nowrap;width:90%">
                                        <button type="button" class="btn <?php echo $iconBtn; ?>" data-toggle="popover" style="white-space:normal!important;margin-bottom:5px;width:100%;font-size:10px;padding:5px!important" data-aedobs="" title="" data-original-title="" onclick="wf_informacaoTramitacaoEmenda(<?php echo $v['aedid']; ?>, <?php echo $v['esdiddestino']; ?>, <?php echo $_REQUEST['edeid']; ?>);">
                                            <span class="glyphicon  <?php echo $iconSpan;?>"></span> <?php echo $v['aeddscrealizar']; ?>
                                        	<span style="color:#428bca;cursor:pointer"></span>
                                        </button>
                                    </div>
<?php                           } else {?>
                                    <div style="white-space:nowrap;width:90%">
                                        <button type="button" class="btn btn-default" data-toggle="popover" style="white-space:normal!important;margin-bottom:5px;width:100%;font-size:10px;padding:5px!important" data-placement="top" data-content="<?php echo $strMSG;?>" data-aedobs="" title="Erro" data-original-title="">
                                            <span class="glyphicon  <?php echo $iconSpan;?>"></span> <?php echo $v['aeddscrealizar']; ?>
                                        	<span style="color:#428bca;cursor:pointer"></span>
                                        </button>
                                    </div>
<?php                          }?>
<?php                       }?>
<?php                       if( in_array(PAR3_PERFIL_SUPER_USUARIO, $arrPerfil) ) {?>
                                <div style="margin-top:7px">
                                    <button style="width:100%;font-size:10px;padding:5px!important;" onclick="wf_historicoBootstrap_emenda('<?php echo $arEstado['docid']; ?>');" type="button" class="btn btn-info">
                                        <span class="glyphicon glyphicon-time" aria-hidden="true"></span> Histórico
                                    </button>
                                </div>
<?php                       } ?>
                            </div>
                        </div>
            		</td>
            	</tr>
            </table>
        	<div class="row">
        		<div class="col-lg-12">
                	<div class="ibox-footer">
                    	<div class="center">
<?php                   if( $arrPerfilRetorno['vincular'] == 'S' && ($arEstado['esdid'] == PAR3_ESD_EMENDA_ANALISE_ENTIDADE) ){?>
                    		<button type="button" id="btn-salvar" data-dismiss="modal" class="btn btn-primary btn-salvar" data-loading-text="Salvando, aguarde ..."><i class="fa fa-search"></i> Salvar</button>
<?php                   }?>
                        	<button type="reset" class="btn btn-success" id="btn-voltar"><i class="fa fa-eraser"></i> Voltar</button>
                    	</div>
                	</div>
            	</div>
        	</div>
        </div>
    </div>
    
</div>
<br>

<script type="text/javascript">

jQuery(document).ready(function(){

	if( jQuery('[name="bo_vincular"]').val() == 'N' ){
		jQuery('#form-lista-emenda .valor_vinculado').attr( "disabled", true );
	}
	
	jQuery('.glyphicon-plus').on('click', function(){
		if( jQuery('[name="bo_vincular"]').val() == 'N' ){
			jQuery('#form-lista-emenda input').attr( "readonly", "readonly");
// 			jQuery('#form-lista-emenda input').attr( "disabled", true );
		}
	});
	
	jQuery('.valor_vinculado').each(function( index ) {
		var inpid = jQuery(this).attr('inpid');
		jQuery('#valor_vinculado_'+inpid).val( number_format(jQuery( this ).val(), 2, ",", ".") );
	});


	//Ao Selecionar um município na MODAL DE ESCOLA
    $(document).on('change','[name=escmuncod]',function(){
        var caminho = window.location.href;
        var muncod = $(this).val();
        var ineid  = $('#dadosbuscaescola').find('[name=ineid]').val();
        var inpid  = $('#dadosbuscaescola').find('[name=inpid]').val();
        var iniano = $('#dadosbuscaescola').find('[name=iniano]').val();
        var iigid  = $('#dadosbuscaescola').find('[name=iigid]').val();
        var inuid  = $('#dadosbuscaescola').find('[name=inuid]').val();
        var ipiid  = $('#dadosbuscaescola').find('[name=ipiid]').val();
        var action  = '&requisicao=buscaModalEscolas&inpid='+inpid+'&ineid='+ineid+'&iniano='+iniano+'&iigid='+iigid+'&inuid='+inuid+'&muncod='+muncod+'&ipiid='+ipiid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").html(resp);
            }
        });
        $("#conteudo-modal").slideDown();
        return false;
    });
	
});

	jQuery('#btn-voltar').click(function(){
<?php   if( !empty($_REQUEST['inuid']) ){?>
			window.location.href = "par3.php?modulo=principal/emendas/listaEmendasMunicipio&acao=A&inuid=<?php echo $_REQUEST['inuid'];?>";
<?php   } else {?>
			window.location.href = "par3.php?modulo=principal/emendas/listaEmendasMunicipio&acao=A";
<?php   }?>
	});

	jQuery('#btn-salvar').click(function (){
		jQuery(this).button('loading');
		window.location = location.href;
	});

jQuery('.valor_vinculado').on('focus', function(){
	if( parseInt( jQuery(this).val() ) == '0' ){
		jQuery(this).val('');
	}
});

jQuery('.valor_vinculado').on('blur', function(){
	if( jQuery(this).val() == '' ){
		jQuery(this).val('0,00');
	}
});

function calculaTotalParlamentar(inpid, ineid){
	var valor_designado = jQuery('[name="valor_designado"]').val();

	var valor_paralmentar = retiraPontosPAR3Emenda( jQuery('#valor_vinculado_'+inpid).val() );
	var vlrTotalObra = valorTotalJaInformadoObra(inpid);

	if( parseFloat(vlrTotalObra) > parseFloat(valor_paralmentar) ){
		swal("Ação não Permitida!", "O valor total do Parlamentar já foi distribuido!", "error");
		jQuery('#valor_vinculado_'+inpid).val( number_format(vlrTotalObra, 2, ",", ".") );
		return false;
	}

	var valor_vinculado = pegaValorTotalParlamentar();

	//var saldo = (parseFloat(valor_designado) - (parseFloat(valor_vinculado) - parseFloat( retiraPontosPAR3Emenda(jQuery('#valor_vinculado_'+inpid).val()) )));
	if( parseFloat(valor_vinculado) > parseFloat(valor_designado) ){
		swal("Ação não Permitida!", "Valor Total Vinculado pelo Parlamentar(R$ "+number_format(valor_vinculado, 2, ",", ".")+") não pode ser maior que o Valor Designado ao município(R$ "+number_format(valor_designado, 2, ",", ".")+")!", "error");
		//jQuery('#valor_vinculado_'+inpid).val( number_format(saldo, 2, ",", ".") );
		jQuery('#valor_vinculado_'+inpid).val( '0,00' );
		return false;
	} else {
		if( jQuery('[name="bo_vincular"]').val() == 'S' ){
    		var caminho = window.location.href;
    	    var action = '&requisicao=salvar-valor-parlamentar&inpid='+inpid+'&ineid='+ineid+'&valor='+jQuery('#valor_vinculado_'+inpid).val();
    	    jQuery.ajax({
    	        type: "POST",
    	        url: caminho,
    	        data: action,
    	        async: false,
    	        success: function (resp) {
    	        	//jQuery('#debug').html(resp);
    	        	if (resp != 'ok'){
    		        	swal({title: "Erro", text: "Falha ao vincular a obra à emenda", type: "error"},
    					   function(){
    							//location.href = location.href;
    					   }
    					);
    				}
    	        	jQuery('#valor_vinculado_'+inpid).val( number_format(retiraPontosPAR3Emenda(jQuery('#valor_vinculado_'+inpid).val()), 2, ",", ".") );
    	        }
    	    });
		}
	}
}

function pegaValorTotalParlamentar(){
	var valor_vinculado = 0;
	
	jQuery('.valor_vinculado').each(function( index ) {
		var inpid = jQuery(this).attr('inpid');
		valor_vinculado = parseFloat(valor_vinculado) + parseFloat(retiraPontosPAR3Emenda(jQuery('#valor_vinculado_'+inpid).val()));
	});
	return valor_vinculado;
}

function pegaValorTotalObraInformado(inpid){
	var valor = 0;
	
	jQuery('.valorindicadoobra_'+inpid).each(function( index ) {
		var obrid = jQuery(this).attr('obrid');

		var valorindicadoobra = retiraPontosPAR3Emenda( jQuery('[name="valorindicadoobra['+obrid+']"]').val() );
		
		valor = parseFloat(valor) + parseFloat(valorindicadoobra);
	});
	return valor;
}

function pegaValorTotalObraInformado(inpid){
	var valor = 0;
	
	jQuery('.valorindicadoobra_'+inpid).each(function( index ) {
		var obrid = jQuery(this).attr('obrid');

		var valorindicadoobra = retiraPontosPAR3Emenda( jQuery('[name="valorindicadoobra['+obrid+']"]').val() );
		
		valor = parseFloat(valor) + parseFloat(valorindicadoobra);
	});
	return valor;
}

function salvarDadosObrasAjax(obrid, inpid){

	var totalValorIndicadoObra = pegaValorTotalObraInformado( inpid );
	var valor_vinculado = retiraPontosPAR3Emenda( jQuery('#valor_vinculado_'+inpid).val() );

	if( parseFloat(totalValorIndicadoObra) > parseFloat(valor_vinculado) ){
		swal("Ação não Permitida!", "Valor Total Indicado(R$ "+number_format(totalValorIndicadoObra, 2, ",", ".")+") não pode ser maior que o Valor Vinculado pelo Parlamentar(R$ "+number_format(valor_vinculado, 2, ",", ".")+")!", "error");
		jQuery('[name="valorindicadoobra['+obrid+']"]').val('0,00');
		return false;
	}

	var valorindicadoobra = retiraPontosPAR3Emenda(jQuery('[name="valorindicadoobra['+obrid+']"]').val());

	if( jQuery('[name="bo_vincular"]').val() == 'S' ){
    	var caminho = window.location.href;
        var action = '&requisicao=salvar-dados-obras&obrid_get='+obrid+'&'+jQuery('[name="form-dados-obras"]').serialize();
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            	if (resp != 'ok'){
    	        	swal({title: "Erro ao vincular a obra a emenda", text: "", type: "error"},
    				   function(){
    						//location.href = location.href;
    				   }
    				);
    			} else {
    				valorTotalJaIndicado(obrid, inpid);
    			}
            }
        });
	}
}

function valorTotalJaIndicado(obrid, inpid){
	
	var caminho = window.location.href;
    var action = '&requisicao=valor-total-indicado&obrid='+obrid+'&inpid='+inpid;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	jQuery('#debug').html(resp);
        	
        	var myArray = jQuery.parseJSON(resp);
        	jQuery('#vlr_total_indicado_'+obrid).html( number_format(myArray[0], 2, ",", ".") );
        	jQuery('#total_geral_indicado').html( number_format(myArray[1], 2, ",", ".") );

        	jQuery('#valorcontrapartida_'+obrid).val( number_format(myArray[2], 2, ",", ".") );
        }
    });
}

function valorTotalJaInformadoObra(inpid){

	var valor = 0;
	var caminho = window.location.href;
    var action = '&requisicao=valor-total-ja-informado&inpid='+inpid;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	//jQuery('#debug').html(resp);
        	valor = resp;
        }
    });
    return valor;
}

function retiraPontosPAR3Emenda(v){
	if( v != 0 ){
		var valor = v.replace(/\./gi,"");
		valor = valor.replace(/\,/gi,".");
	} else {
		var valor = v;
	}
	
	return valor;
}

function wf_historicoBootstrap_emenda(docid){
    $('#modal-alert .modal-title').html('Histórico de Tramitações');
    $('#modal-alert .modal-body').html('Não constam informações.');
        $('.modal-dialog').attr('style', 'width: 80% !important;');
    var url = '/geral/workflow/historico.php' +
    '?modulo=principal/tramitacao' +
    '&acao=C' +
    '&requisicao=historicoBootstrap' +
    '&docid=' + docid;
    
    $.ajax({
    url: url,
    context: document.body
    }).done(function (result) {
    <?php
    global $aumentaModalHistorico;
       if ($aumentaModalHistorico) {
           ?>
    	$('.modal-dialog').attr('style', 'width: 48% !important;');
    	$('.modal-content').attr('style', 'width: 750px !important;');
    	<?php
    
       } ?>
    $('#modal-alert .modal-body').html(result);
    });
    
    $('#modal-alert').modal();
}

function wf_informacaoTramitacaoEmenda(aedid, esdiddestino, edeid){
	jQuery('#aedid').val(aedid);
	jQuery('#esdiddestino').val(esdiddestino);

	if( esdiddestino == 2117 ){
		if(validaEnvioAceitaEmenda() < 1 ){
			swal("Ação não Permitida!", "É necessário distribuir o Valor Vinculado pelo Parlamentar!", "info");
			return false;
		}
	}
	divCarregando();
	jQuery('[name="requisicao"]').val('tramitar_emenda');
	jQuery('[name="form-lista-emenda"]').submit();
	
}

function validaEnvioAceitaEmenda(){
	var valor = 0;
	var caminho = window.location.href;
    var action = '&requisicao=valida-envio-emenda-aceita&edeid='+jQuery('[name="edeid"]').val();
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	//jQuery('#debug').html(resp);
        	valor = resp;
        }
    });

    return valor;
}

function verificaValorEmpenho(inpid){
	var edeid   = jQuery('[name="edeid"]').val();
    var valor 	= 0;
	var caminho = window.location.href;
    var action  = '&requisicao=verificaValorEmpenho&inpid='+inpid+'&edeid='+edeid;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	valor = parseFloat(resp);
        	//jQuery("#debug").html(resp+'-'+valordestinado);
        	/*if( parseFloat(valordestinado) <  parseFloat(resp) ){
        		swal("Ação não Permitida!", "Já existe empenho para este planejamento. O valor informado de R$ "+number_format(valordestinado, 2, ",", ".")+" não pode ser menor que o Valor Empenhado: R$ "+number_format(resp, 2, ",", ".")+"!", "error");
        		jQuery('#valordestinado_'+inpid).val(number_format(resp, 2, ",", "."));
            }*/
        }
    });
    return valor;
}
</script>


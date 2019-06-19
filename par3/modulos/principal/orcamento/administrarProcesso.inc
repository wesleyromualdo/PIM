<?php

/**
    * Arquivos que vão popular os campos
    * Totas as consulta devem trazer apenas dados de iniciativas cadastradas
*/
$pesquisaProcesso = new Par3_Controller_Processo();
$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$arrIniciativaAno = Par3_Model_IniciativaPlanejamento::getInicitivaAno();

switch ($_REQUEST['requisicao']){
	case 'inativaprocesso':
		ob_clean();
		$sql = "UPDATE par3.processo SET prostatus = 'I' WHERE proid = {$_REQUEST['proid']}";
		$db->executar($sql);
		
		if( $_REQUEST['tipo'] == 'par' ){
			$sql = "UPDATE par3.processoparcomposicao SET ppcstatus = 'I' WHERE proid = {$_REQUEST['proid']}";
		} else {
			$sql = "UPDATE par3.processoobracomposicao SET pocstatus = 'I' WHERE proid = {$_REQUEST['proid']}";
		}
		$db->executar($sql);
		$db->commit();
		simec_redirecionar('par3.php?modulo=principal/orcamento/administrarProcesso&acao=A', 'success', 'Processo cancelado com sucesso');
		die;
	break;
	case 'detalharProcesso':
		ob_clean();
		$proid = $_REQUEST['dados'][0];
		$tipo = $_REQUEST['dados'][1];
		if( $tipo == 'obra' ){
			echo $pesquisaProcesso->listaComposicaoObra($proid);
		} else {
			echo $pesquisaProcesso->listaComposicao($proid);
		}
		die();
		break;
	case 'autentica-processo':
		ob_clean();
		require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
		include_once APPRAIZ.'includes/classes/ProcessoFNDE.class.php';
		require_once APPRAIZ.'par3/classes/controller/WS_Servico_FNDE.class.inc';
		include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
		
		$proid = $_REQUEST['proid'];
		
		$sql = "SELECT inuid, procnpj, probanco as banco, proagencia as agencia, pronumeroprocesso as processo, seq_conta_corrente, proseqconta, nu_conta_corrente FROM par3.processo WHERE proid = {$proid} AND prostatus = 'A'";
		$arrParam = $db->pegaLinha( $sql );
		
		$sql = "SELECT sisid, tprid, tipprogramafnde, tipnumerosistemasigef, tiptipoprocessosigef FROM execucaofinanceira.tipoprocesso where tipid = 12 and tipstatus = 'A'";
		$arTipProcesso = $db->pegaLinha($sql);
		
		$arrParamProcesso = array(
				'an_processo' 			=> date("Y"),
				'nu_processo' 			=> $arrParam['processo'],
				'tp_processo' 			=> $arTipProcesso['tiptipoprocessosigef'],
				'co_programa_fnde' 		=> $arTipProcesso['tipprogramafnde'],
				'nu_sistema'			=> $arTipProcesso['tipnumerosistemasigef'],
				'nu_cnpj_favorecido' 	=> $arrParam['procnpj'],
				'proid' 				=> $proid,
				'ws_usuario'			=> $_REQUEST['ws_usuario'],
				'ws_senha' 				=> $_REQUEST['ws_senha'],
				'nu_banco' 				=> $arrParam['banco'],
				'nu_agencia'			=> $arrParam['agencia'],
		);
		
		$obServicoFnde = new Par3_Controller_WS_Servico_FNDE( $_REQUEST['ws_usuario'], $_REQUEST['ws_senha'] );
		$obProcesso = new Par3_Controller_Processo();
		$obProcessoFNDE = new ProcessoFNDE( $_REQUEST['ws_usuario'], $_REQUEST['ws_senha'], false );
				
		if( $arrParam['seq_conta_corrente'] ){
			$arrConta = $obServicoFnde->consultaContaCorrenteProcessoFNDE( $proid );
			
			if( is_array($arrConta) || is_object($arrConta) ){
				$obProcesso->atualizaContaCorrentePAR($proid, (object)$arrConta);
				simec_redirecionar($_SERVER['HTTP_REFERER'], 'success');
			} else {
				simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', $arrConta);
			}
		} else {
			$res_sp = $obProcessoFNDE->solicitarProcesso($arrParamProcesso, true);
			if($res_sp){
				$arrConta = $obServicoFnde->solicitaAberturaContaCorrente( $proid );
				
				if( is_array($arrConta) || is_object($arrConta) ){
					$obProcesso->atualizaContaCorrentePAR($proid, (object)$arrConta);
					simec_redirecionar($_SERVER['HTTP_REFERER'], 'success');
				} else {
					simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', substr($arrConta, 0, 200));
				}
			} else {
				simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', $res_sp);
			}
		}
		die();
		break;
	case 'carrega-vinculo-processo':
		ob_clean();
		$proid = $_REQUEST['proid'];
		$tipo = $_REQUEST['tipo'];
		$inuid = $_REQUEST['inuid'];
		$intoid = $_REQUEST['intoid'];
		$intaid = $_REQUEST['intaid'];

		if( $tipo == 'obra' ){
		    echo $pesquisaProcesso->listaObrasSemProcesso($proid, $inuid, $intoid);
		} else {
		    echo $pesquisaProcesso->listaIniciativaPlanejamentoSemProcesso($proid, $inuid, $intoid, $intaid);
		}
		die();
	break;
	case 'vincula-processo':
		ob_clean();
		$tipo = $pesquisaProcesso->pegaTipoProcesso($_REQUEST['proid']);
		
		if( $tipo == 'obra' ){
			echo $pesquisaProcesso->vinculaObraProcessoExistente($_REQUEST);
		} else {
			echo $pesquisaProcesso->vinculaIniciativaPlanejadaProcessoExistente($_REQUEST);
		}
		die();
	break;
	case 'carregaMunicipios':
		$municipio = new Territorios_Model_Municipio();
		print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'"))); die;
		break;
	case 'desvincularProcesso':
	    ob_clean();
	    
	    $ppcid = $_REQUEST['ppcid'];
	    
	    if( $_REQUEST['tipo'] == 'par' ){
	       $update = "UPDATE par3.processoparcomposicao SET ppcstatus = 'I' WHERE ppcid = {$ppcid}";
	    } else {
	       $update = "UPDATE par3.processoobracomposicao SET pocstatus = 'I' WHERE proid = {$ppcid}";
	    }
	    $db->executar($update);
	    
	    $url_redirecionar = 'par3.php?modulo=principal/orcamento/administrarProcesso&acao=A';
	    
	    if( $db->commit()){
	        simec_redirecionar($url_redirecionar, 'success', 'Processo cancelado com sucesso');
	    } else {
	        simec_redirecionar($url_redirecionar, 'error', 'Ocorreu um erro desconhecido');
	    }

	    die;
	break;
}
require APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';
$db->cria_aba( $abacod_tela, $url, '' );
?>
<style>
    .navbar-listagem {
        margin-top: 0px;
        float: right;
        width: 500px;
    }
    .anoClass label{
        margin-left: -4px;
    }
    .estadoClass a.chosen-single.chosen-default{
        width: 170px ;
        margin-left: 21px;
    }
    .estadoClass a.chosen-single.chosen-single-with-deselect{
        width: 170px ;
        margin-left: 21px;
    }
   /*.estadoClass .chosen-container-single .chosen-search .chosen-drop {
        width: 200px ;
    }*/
    .estadoClass .chosen-drop {
        width: 180px ;
        margin-left: 21px;
    }
    .estadoClass label{
        margin-left: -21px;
    }
    .linhaInferior .divdtPedito {
        width: 323px;
    }
    .linhaInferior {
        margin-left: 6.9%;
    }
    .divSitua a.chosen-single.chosen-default{
        top: 10px;
    }
    .divTecnico{
        top: 10px;
    }
    .municipio_detalhe{
        cursor:pointer;
        color:blue;
    }
</style>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Administração de Processos</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-administra-processo" id="form-administra-processo" class="form-horizontal" method="post">
            	<input type="hidden" name="requisicao" id="requisicao" value="filtrar"/>
                <div class="row">
                    <div class="col-lg-6">
                        <?php
                        $filtro = simec_htmlentities( $_POST['numeroprocesso'] );
                        $numeroprocesso = $filtro;

                        $config = array();
                        $attribs = array('maxlength' => '255',
                            'onkeyup' => "this.value=mascaraglobal('#####.######/####-##',this.value)",
                            'onblur' => "MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);", 'autofocus');

                        echo $simec->input('numeroprocesso', 'Número de Processo', $numeroprocesso, $attribs, array('input-size' => '6'));
                        ?>
                        <?php echo $simec->input('dotnumero', 'Número do Termo', $_REQUEST['dotnumero'], array('type' => 'number'),array('input-size' => '6')); ?>
                        <?php echo $simec->input('proid', 'Código do Processo', $_REQUEST['proid'], array('type' => 'number'),array('input-size' => '6')); ?>
                    	<?php echo $simec->input('codigo', 'Código Obra/Iniciativa', $_REQUEST['codigo'], array('type' => 'number'),array('input-size' => '6')); ?>



                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termo Validado:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="termo_validado">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termo_validado' id='termo_validado_s' value="S" <?= $_REQUEST['termo_validado'] == 'S'?'checked':'';?>>
                                        <label for="termo_validado_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termo_validado" id="termo_validado_n" value="N"  <?= $_REQUEST['termo_validado'] == 'N'?'checked':'';?>>
                                        <label for="termo_validado_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termo_validado" id="termo_validado_t" value="T"  <?= $_REQUEST['termo_validado'] != 'S' && $_REQUEST['termo_validado'] != 'N'?'checked':'';?>>
                                        <label for="termo_validado_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termos com contrato:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="pagsituacaopagamento_group_id">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termocontrato' id='termocontrato_s' value="S" <?= $_REQUEST['termocontrato'] == 'S'?'checked':'';?>>
                                        <label for="termocontrato_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termocontrato" id="termocontrato_n" value="N"  <?= $_REQUEST['termocontrato'] == 'N'?'checked':'';?>>
                                        <label for="termocontrato_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termocontrato" id="termocontrato_t" value="T"  <?= $_REQUEST['termocontrato'] != 'S' && $_REQUEST['termocontrato'] != 'N'?'checked':'';?>>
                                        <label for="termocontrato_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termos com nota fiscal:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="termonota">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termonota' id='termonota_s' value="S" <?= $_REQUEST['termonota'] == 'S'?'checked':'';?>>
                                        <label for="termonota_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termonota" id="termonota_n" value="N"  <?= $_REQUEST['termonota'] == 'N'?'checked':'';?>>
                                        <label for="termonota_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termonota" id="termonota_t" value="T"  <?= $_REQUEST['termonota'] != 'S' && $_REQUEST['termonota'] != 'N'?'checked':'';?>>
                                        <label for="termonota_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-6">
                        <?php echo $simec->radio('itrid', 'Esfera', ($_REQUEST['itrid']?:0), [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'], array('maxlength' => '255')); ?>
                        <div class="row">
                            <div class="col-lg-5" style="margin-left:55.5px;">
                                <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255'),array('input-size' => '6', 'label-size' => '5')); ?>
                            </div>
                            <div class="col-lg-5" id="div-muncod" style="display:<?= $_REQUEST['itrid'] == 1 ?'none':''?>;">
                                <?php
                                $_REQUEST['muncod'] = tratarArrayParaMultiSelect($_REQUEST['muncod']);
                                if($_REQUEST['estuf']){
                                    $municipio = new Territorios_Model_Municipio();
                                    $arrMunicipio = simec_preparar_array($municipio->lista(array('muncod as codigo', 'mundescricao as descricao'), array("estuf = '{$_REQUEST['estuf']}'")));
                                }
                                echo $simec->select('muncod[]', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255','multiple' => 'multiple'), array()); ?>
                            </div>
                        </div>

                        <?php echo $simec->select('iniano', 'Ano', $_REQUEST['iniano'], $arrIniciativaAno, array('maxlength' => '255'), array('input-size' => '4')); ?>
                        <?php /*
                            $_REQUEST['tipo_processo'] = (empty($_REQUEST['tipo_processo']) ? 'P' : $_REQUEST['tipo_processo']);
                            $arrOptions = array('O' => 'Obras', 'P' => 'PAR');
                            echo $simec->radio('tipo_processo', 'Tipo de Processo', $_REQUEST['tipo_processo'], $arrOptions, array(), array());*/ ?>
                        <div>
                            <?php
                                $arrTipoObjeto = Par3_Model_IniciativaPlanejamento::getTipoObjetoporIniciativa();
                                echo $simec->select('intoid', 'Tipo do Objeto', $_REQUEST['intoid'], $arrTipoObjeto, array('maxlength' => '255'), array('input-size' => '4'));
                            ?>
                            <?php
                            $arrOptions = array('O' => 'Obras', 'P' => 'PAR','T' => 'Todos');
                            echo $simec->radio('tipo_processo', 'Tipo de Processo', $_REQUEST['tipo_processo']?:'T', $arrOptions, array(), array());
                            ?>
                        </div>
                        <div id="divTiposAssistencia">
                            <?php
                            $controllerIniciativaTiposAssistencia = new Par3_Controller_IniciativaTiposAssistenciaController();
                            $mInta = $controllerIniciativaTiposAssistencia->recuperar();
                            $arrTiposAssistencia = simec_preparar_array($mInta->recuperarTodosFormatoInput('intadsc'));
                            $arrTiposAssistencia[0] = 'Todos';
                            echo $simec->select('intaid','Tipos de Assistência',$_REQUEST['intaid'],$arrTiposAssistencia,['placeHolder' => 'Selecione'],array('input-size' => '4','label-size' => '3'));
                            ?>
                        </div>
                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                	<button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-info btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button> |
                    <button type="reset" class="btn btn-default-bright" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
                </div>
            </div>
           <!-- Listagem  -->
            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras" style="display: block;">
                    <div class="table-responsive" >
                        <div>
                            <div id="testeSQL">
                            <?php
                            if($_REQUEST['requisicao'] == 'filtrar' || $_REQUEST['proid']){
                                echo $pesquisaProcesso->listar($_REQUEST);
                            }
                            ?>
                            </div>
                        </div>
                    </div>
                    <br>
                    <br>
                </div>
            </div>
            <!-- END Listagem  -->
                        
        </div>
    </div>
    
</div>

<div class="ibox float-e-margins animated modal" id="modal-login-processo" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
		<div class="modal-content">
			<div class="ibox-content" style="text-align: center;">
				<div class="row">
					<div style="text-align: center;">
	        			<h1 class="center" style="font-weight: bold;">Autenticação</h1>
					</div>
        		<div class="ibox-content">
        			<form method="post" name="form-autentica-processo" id="form-autentica-processo" class="form form-horizontal">
        			<div class="row">
						<input type="hidden" name="proid" id="proid" value="">
						<input type="hidden" name="requisicao" id="requisicao" value="">
		        		<div class="form-group ">
		                    <label for="codana" class="col-md-3 control-label">Usuário: </label>
		                    <div class="col-md-7 ">
		                        <input name="ws_usuario" id="ws_usuario" type="text" value="" class="form-control">
		                    </div>
		                </div>
					</div>
					<div class="row">
		                <div class="form-group ">
		                    <label for="codana" class="col-md-3 control-label">Senha: </label>
		                    <div class="col-md-7 ">
		                        <input name="ws_senha" id="ws_senha" type="password" value="" class="form-control">
		                    </div>
		                    <div style="clear:both"></div>
		                </div>
					</div>
        		</form></div>
				</div>
			</div>
	        <div class="ibox-footer" style="text-align: center;">
	                <button type="button" id="btn-solicitar-conta" data-dismiss="modal" class="btn btn-success" ><i class="fa fa-plus"></i> Solicitar / Consultar</button>
	            	<button type="button" id="btn-cancelar" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>
			</div>
		</div>
	</div>
</div>


<div class="ibox float-e-margins animated modal" id="modal_vincula_proceso" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
    <div class="modal-dialog modal-lg" style="width: 80%;">
		<div class="modal-content">
			<div class="ibox-content">
				<div class="row" >
					<div id="conteudo-modal-processo"></div>
				</div>
			</div>
	        <div class="ibox-footer" style="text-align: center;">
	            	<button type="button" id="btn-cancelar-vinculo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>
	                <button type="button" id="btn-vincula-processo" data-dismiss="modal" class="btn btn-success"><i class="fa fa-plus"></i> Vincular Processo</button>
			</div>
		</div>
	</div>
</div>
<script>
$(function(){
    $('[name="tipo_processo"]').focus();
});
jQuery('.municipio_detalhe').click(function(){
	var inuid = jQuery(this).attr('id');

	var url = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid='+inuid;
	window.open(url, '_blank');
});

jQuery('#limpar').click(function(){
	window.location.href = window.location;
});

$('[name=itrid]').change(function(){
    if($(this).val() == 1){
        $('#div-muncod').hide();
        $('#muncod').val('');
        $('#muncod').trigger("chosen:updated");
    }else{
        $('#div-muncod').show();
    }
});

/*$('[name=tipo_processo]').change(function(){
    if($(this).val() == 'O'){
        $('#div-intoid').hide();
        $('#intoid').val('');
        $('#intoid').trigger("chosen:updated");

    }else{
        $('#div-intoid').show();
    }
});*/

jQuery(document).keypress(function(e) {
    if(e.which == 13){
         jQuery('.btn-pesquisar').click();
    }
});

jQuery(function() {
	
	$(window).on('beforeunload',function(){
        $('.loading-dialog-par3').show();
    });
    $(window).on('click','#btn-pesquisar #limpar',function(){
        $('.loading-dialog-par3').show();
    });
    $(window).load(function(){
        $('.loading-dialog-par3').hide();
    });
    //renderizeMunicipio();

    if(jQuery('[name="estuf"]').val() != '') {
        carregarMunicipio(jQuery('[name="estuf"]').val(), <?=$_REQUEST['muncod']?>);
    }

    jQuery("input:radio[name=itrid], select[name=estuf]").change(function() {
        //renderizeMunicipio();
    });

    jQuery('select[name=estuf]').change(function(){
        carregarMunicipio(this.value);
    });

	function renderizeMunicipio() {
	    var filtroMunicipio = jQuery("select[name=muncod]").parents("div.form-group");
	    if (jQuery('input:radio[name=itrid]:checked').val() === '1' || !jQuery('select[name=estuf]').val()) {
	        filtroMunicipio.slideUp();
	    } else {
	        filtroMunicipio.slideDown();
	    }
	}
});

function carregarMunicipio(estuf, muncod) {
	if(estuf != '') {
		var options = jQuery('#muncod');
		options.empty();
		options.append(new Option('', ''));
		jQuery.post('', 'requisicao=carregaMunicipios&estuf='+estuf, function(retorno) {
			options.append(new Option('', ''));
			$.each(JSON.parse(retorno), function() {
				options.append(new Option(this.mundescricao, this.muncod));
			});
			options.focus();
			options.val(muncod);
			options.trigger('chosen:updated');
		});
	}
}

function vincularObra( proid, tipo, inuid, intoid, intaid ){
	var caminho = window.location.href;
    var action  = '&requisicao=carrega-vinculo-processo&proid='+proid+'&tipo='+tipo+'&inuid='+inuid+'&intoid='+intoid+'&intaid='+intaid;
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            jQuery("#conteudo-modal-processo").html(resp);
        }
    });
    $("#modal_vincula_proceso").modal("show");
}

    jQuery("#btn-vincula-processo").click(function () {

        if( jQuery('[name="check_acao[]"]:checked').length == 0 ){
            alert('Selecione um item para o vinculo');
            return false;
        }
    	jQuery('#form-vincula-processo [name="requisicao"]').val('vincula-processo');
        jQuery('[name="form-vincula-processo"]').submit();
    });
        
    jQuery("#btn-solicitar-conta").click(function () {
        var proid = jQuery('[name="proid"]').val();
        var ws_usuario = jQuery('[name="ws_usuario"]').val();
        var ws_senha = jQuery('[name="ws_senha"]').val();

        jQuery('#form-autentica-processo [name="requisicao"]').val('autentica-processo');
        jQuery('[name="form-autentica-processo"]').submit();
    });
    
    jQuery(".btn-pesquisar").click(function () {
    	$btn = jQuery(this).button('loading');
    	jQuery('[name="requisicao"]').val('filtrar');
        jQuery('[name="form-administra-processo"]').submit();
    });
    
    function solicitarConta( proid ) {
    	
       jQuery('[name="proid"]').val(proid);
       jQuery("#modal-login-processo").modal("show");
    }
    
    function removeProcesso( proid, tipo ) {
    	
       window.location.href = 'par3.php?modulo=principal/orcamento/administrarProcesso&acao=A&requisicao=inativaprocesso&proid='+proid+'&tipo='+tipo;
    }

    function desvincularProcesso( ppcid, tipo ) {
    	
        window.location.href = 'par3.php?modulo=principal/orcamento/administrarProcesso&acao=A&requisicao=desvincularProcesso&ppcid='+ppcid+'&tipo='+tipo;
     }
</script>


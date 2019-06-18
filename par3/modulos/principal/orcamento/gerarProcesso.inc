<?php
/**
    * Arquivos que vão popular os campos
    * Totas as consulta devem trazer apenas dados de iniciativas cadastradas
*/
$pesquisaProcesso = new Par3_Controller_Processo();
$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$arrIniciativaAno = Par3_Model_IniciativaPlanejamento::getInicitivaAno();


switch ($_REQUEST['requisicao_processo']){
	case 'gerar-processo-fnde':
		require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
		$obProcesso = new Par3_Controller_Processo();
		echo $obProcesso->gerarProcesoFNDE($_REQUEST);
		die;
	break;
	case 'vincula-processo':
		require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
		$anaid = $_REQUEST['anaid'];
		$obProcesso = new Par3_Controller_Processo();
		$arrRegistro = array();
		parse_str($_REQUEST['form_itens_composicao'], $arrRegistro);
		$_REQUEST['form-item-composicao'] =  $arrRegistro;
	
		$obProcesso->sqlProcessosPorTipoAtendimento($_REQUEST);
		die();
	break;
	case 'vincula-processo-existente':
		require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
		$obProcesso = new Par3_Controller_Processo();
		$obProcesso->vinculaIniciativaProcessoExistente($_REQUEST);
		die();
	break;
	case 'pegaOptionEntidadeExecutora':
		require_once APPRAIZ.'par3/classes/controller/Processo.class.inc';
		$obProcesso = new Par3_Controller_Processo();
		echo $obProcesso->pegaOptionEntidadeExecutora($_REQUEST);
		die();
	break;
}

switch ($_REQUEST['requisicao']){
	case 'carregaMunicipios':
		$municipio = new Territorios_Model_Municipio();
		print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'"))); die;
		break;
    case 'carregarMunicipiosAnalise':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        echo simec_json_encode($municipio->carregar($municipio->pegarSQLSelect($_GET['estuf'])));
        die;
        break;
    case 'carregarTodosMunicipios':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        $arrMun = $municipio->recuperarListaMunicipios();
        echo simec_json_encode($arrMun);
    die;
    break;
}

require APPRAIZ . 'includes/cabecalho.inc';

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
    .itrClass label{
        margin-right: -10px;
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
</style>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Gerar de Processos</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-administra-processo" id="form-administra-processo" class="form-horizontal" method="post">
            	<input type="hidden" name="requisicao" id="requisicao" value=""/>
                <div class="row">
                    <div class="col-lg-6">
                        <?php
                       /* $filtro = simec_htmlentities( $_POST['numeroprocesso'] );
                        $numeroprocesso = $filtro;
                        
                        $config = array();
                        $attribs = array('maxlength' => '255',
                        				'onkeyup' => "this.value=mascaraglobal('#####.######/####-##',this.value)",
                        				'onblur' => "MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);");
                        
                        echo $simec->input('numeroprocesso', 'Número de Processo', $numeroprocesso, $attribs, $config);*/
                        
                        $_REQUEST['tipo_processo'] = (empty($_REQUEST['tipo_processo']) ? 'P' : $_REQUEST['tipo_processo']);
                        
                        $arrOptions = array('O' => 'Obras', 'P' => 'PAR');
                        ?>
                        <?php echo $simec->radio('tipo_processo', 'Tipo de Processo', $_REQUEST['tipo_processo'], $arrOptions, array(), array()); ?>
                        <?php echo $simec->input('iniciativa', 'Código Obra/Iniciativa', $_REQUEST['iniciativa'], array('type' => 'number'),array('input-size' => '9')); ?>
                        <?php echo $simec->select('iniano', 'Ano', $_REQUEST['iniano'], $arrIniciativaAno, array('maxlength' => '255'), array()); ?>

                        </div>
                    <div class="col-lg-6">
                        <div class="itrClass" style="margin-left: 12%;">
                        <?php
                        echo $simec
                            ->radio(
                                'itrid',
                                'Esfera',
                                ($_POST['itrid']?:1),
                                [1 => 'Estadual',2 => 'Municipal'],
                                array('maxlength' => '255'),
                                ['label-size' => '2']
                            );
                        ?>
                        </div>
                        <div class="row">
                            <div class="col-lg-4 anoClass" style="margin-left: 17%;">
                        <?php
                        echo $simec
                            ->select(
                                'estuf',
                                'Estado',
                                $_POST['estuf'],
                                $arrEstado,
                                array('maxlength' => '255','multiple' => 'multiple'),
                                array()
                            );
                        ?>
                        </div>
                        <div class="col-lg-5" id="div-muncod" style="display:<?= $_POST['itrid'] == 1 || empty($_POST['itrid']) ?'none':'';?>;">
                            <?php
                            $arrMunicipio = array();
                            if ($_POST['muncod'] && $_POST['itrid'] != 1) {
                                $_POST['muncod'] = tratarArrayParaMultiSelect($_POST['muncod']);//trata o array
                                $municipio = new Territorios_Model_Municipio();//instancia a classe município
                                //lista todos os municípios relacionados ao Estado(estuf) informado
                                $arrMunicipio = $municipio->pegarSQLSelect($_POST['estuf']);
                            }
                            echo $simec
                                ->select(
                                    'muncod[]',
                                    'Município',
                                    $_POST['muncod'],
                                    $arrMunicipio,
                                    array(
                                        'maxlength' => '255',
                                        'multiple'  => 'multiple',
                                        'onclick'   => 'listarMunicipios()'
                                    ),
                                    array()
                                );
                            ?>
                        </div>
                    </div>
                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                	<button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-info btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button> |
                    <button type="button" id="btn-carrega-login1" data-dismiss="modal" class="btn btn-primary" onclick="btn_carrega_login();" data-loading-text="Gerando, aguarde ..."><i class="fa fa-plus"></i> Gerar Processo</button> |
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
                            if($_REQUEST['requisicao'] == 'filtrar'){
                            	if( $_REQUEST['tipo_processo'] == 'O' ){
                                	echo $pesquisaProcesso->listaIniciativaObrasSemProcesso($_REQUEST);
                            	} else {
                                	echo $pesquisaProcesso->listaIniciativaSemProcesso($_REQUEST);
                            	}
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

<div class="ibox float-e-margins animated modal" id="modal-login-processo" tabdesex="-1" role="dialog" aria-hidden="true">
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
		                        <input name="ws_usuario_p" id="ws_usuario_p" type="text" value="" class="form-control">
		                    </div>
		                </div>
					</div>
					<div class="row">
		                <div class="form-group ">
		                    <label for="codana" class="col-md-3 control-label">Senha: </label>
		                    <div class="col-md-7 ">
		                        <input name="ws_senha_p" id="ws_senha_p" type="password" value="" class="form-control">
		                    </div>
		                    <div style="clear:both"></div>
		                </div>
					</div>
					<div class="row" id="div_main_entidade">
		                <div class="form-group ">
		                    <label for="codana" class="col-md-3 control-label">Entidade Executora: </label>
		                    <div class="col-md-7" id="div_entidade_executora">
		                        
		                    </div>
		                    <label for="codana" class="col-md-3 control-label"> </label>
		                    <div class="col-md-7" id="div_entidade_executora_msg" style="color: red; display: none">
		                        A entidade selecionada será responsável pela execução dos recursos recebidos e prestação de contas do termo gerado para este processo. Certifique-se que a entidade selecionada está correta
		                    </div>
		                    <div style="clear:both"></div>
		                </div>
					</div>
        		</form></div>
				</div>
			</div>
	        <div class="ibox-footer" style="text-align: center;">
	                <!--  <button type="button" id="btn-vincula-processo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Vincular Processo Existente</button>-->
	            	<button type="button" id="btn-cancelar-vinculo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>
	                <button type="button" id="btn-gerar-processo" data-dismiss="modal" class="btn btn-success" onclick="btn_gear_processo_fnde();"><i class="fa fa-plus"></i> Gerar Processo FNDE</button>
			</div>
		</div>
	</div>
</div>
<!--
<div class="ibox float-e-margins animated modal" id="modal_vincula_proceso" tabdesex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
		<div class="modal-content">
			<div style="text-align: center;">
	        			<h1 class="center" style="font-weight: bold;">Autenticação</h1>
			</div>
			<div class="ibox-content" style="text-align: center;">
				<div class="row">
					<div id="conteudo-modal-processo"></div>
				</div>
			</div>
	        <div class="ibox-footer" style="text-align: center;">
	            	<button type="button" id="btn-cancelar-vinculo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>
	                <button type="button" id="btn-carrega-login3" data-dismiss="modal" class="btn btn-success" onclick="btn_carrega_login();"><i class="fa fa-plus"></i> Gerar Processo FNDE</button>
			</div>
		</div>
	</div>
</div>
-->
<script>

/*function vinculaProcessoExistente(proid){

	jQuery('#form-vincula-processo #proid').val(proid);
	jQuery('[name="requisicao_processo"]').val('vincula-processo-existente');
	
	jQuery('#form-vincula-processo').submit();
} */

/*
 jQuery("#btn-gerar-processo").click(function () {

	if( jQuery('.check_acao:checked').length == 0 ){
    	alert('Selecione um item da listagem');
    	return false;
    }
	
	jQuery('#requisicao_processo').val('gerar-processo');
	jQuery('#form-gerar-processo').submit();
});
 */

function btn_gear_processo_fnde(){
 
	jQuery('[name="requisicao_processo"]').val('gerar-processo-fnde');

	if( jQuery('[name="ws_usuario_p"]').val() == '' ){
		alert('Informe o usuário.');
		return false;
	}
	
	if( jQuery('[name="ws_senha_p"]').val() == '' ){
		alert('Informe a senha.');
		return false;
	}

	jQuery('[name="ws_usuario"]').val( jQuery('[name="ws_usuario_p"]').val() );
	jQuery('[name="ws_senha"]').val( jQuery('[name="ws_senha_p"]').val() );
	jQuery('[name="ungid_p"]').val( jQuery('[name="ungid_p"]').val() );
	jQuery('#btn-gerar-processo').attr('disabled', true);
	
	jQuery('#form-gerar-processo').submit();
}

/*jQuery("#btn-vincula-processo").click(function () {
	//jQuery('#form-autentica-processo #requisicao_processo').remove();
	
   var caminho = window.location.href;
    var action  = '&requisicao_processo=vincula-processo&'+jQuery('#form-autentica-processo').serialize();
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            jQuery("#conteudo-modal-processo").html(resp);
        }
    });
    $("#modal-login-processo").modal("hide");
    $("#modal_vincula_proceso").modal("show");
});*/

function btn_carrega_login(){
	if( jQuery('.check_acao:checked').length == 0 ){
		alert('Selecione um item da listagem');
		return false;
	}

	var inuid = jQuery("#inuid").val();
	$('#div_entidade_executora_msg').hide();
	
	var caminho = window.location.href;
    var action  = '&requisicao_processo=pegaOptionEntidadeExecutora&inuid='+inuid;
   	jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            if( resp == 'N' ){
            	jQuery("#div_main_entidade").hide();                
            } else {
            	jQuery("#div_main_entidade").show();
            	jQuery("#div_entidade_executora").html(resp);
            }
        }
    });
	
	$("#modal-login-processo").modal("show");
}

//div_entidade_executora_msg

function carregaMSG(){
	$('#div_entidade_executora_msg').show();	
}

$('[name=itrid]').change(function() {
    if($(this).val() == 1){
        $('#div-muncod').hide();
        $('#muncod').val('');
        $('#muncod').trigger("chosen:updated");
    }else{
        $('#div-muncod').show();
    }
});

$('[name=tipo_processo]').change(function(){
    if($(this).val() == 'O'){
        $('#div-intoid').hide();
        $('#intoid').val('');
        $('#intoid').trigger("chosen:updated");

    }else{
        $('#div-intoid').show();
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
//    renderizeMunicipio();
//
//    if(jQuery('[name="estuf"]').val() != '') {
//        carregarMunicipio(jQuery('[name="estuf"]').val(), <?//=$_REQUEST['muncod']?>//);
//    }
//
//    jQuery("input:radio[name=itrid], select[name=estuf]").change(function() {
//        renderizeMunicipio();
//    });
//
//    jQuery('select[name=estuf]').change(function(){
//        carregarMunicipio(this.value);
//    });
//
//	function renderizeMunicipio() {
//	    var filtroMunicipio = jQuery("select[name=muncod]").parents("div.form-group");
//	    if (jQuery('input:radio[name=itrid]:checked').val() === '1' || !jQuery('select[name=estuf]').val()) {
//	        filtroMunicipio.slideUp();
//	    } else {
//	        filtroMunicipio.slideDown();
//	    }
//	}
//});
//
//	function carregarMunicipio(estuf, muncod) {
//		if(estuf != '') {
//			var options = jQuery('#muncod');
//			options.empty();
//			options.append(new Option('', ''));
//			jQuery.post('', 'requisicao=carregaMunicipios&estuf='+estuf, function(retorno) {
//				options.append(new Option('', ''));
//				$.each(JSON.parse(retorno), function() {
//					options.append(new Option(this.mundescricao, this.muncod));
//				});
//				options.focus();
//				options.val(muncod);
//				options.trigger('chosen:updated');
//			});
//		}
	});

    jQuery("#btn-solicitar-conta").click(function () {
        var proid = jQuery('[name="proid"]').val();
        var ws_usuario = jQuery('[name="ws_usuario"]').val();
        var ws_senha = jQuery('[name="ws_senha"]').val();

        jQuery('#form-autentica-processo [name="requisicao"]').val('autentica-processo');
        jQuery('[name="form-autentica-processo"]').submit();
    	/*var caminho = window.location.href;
        var action  = '&requisicao=autentica-processo&proid='+proid+'&ws_usuario='+ws_usuario+'&ws_senha='+ws_senha;
       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                //jQuery("#debug").html(resp);
                window.location.reload(true);
            }
        });*/
    });
    
    jQuery(".btn-pesquisar").click(function () {
    	$btn = jQuery(this).button('loading');

    	if($('#iniciativa').val() == ""){
            if( jQuery('[name="itrid"]:checked').val() == 1 ) {

                if( jQuery("#estuf").val() === null){
                    alert('Selecione um Estado');
                    $btn.button('reset');
                    return false;
                }
            }

            if( jQuery('[name="itrid"]:checked').val() == 2 ){
                if( jQuery("#estuf").val() === null && jQuery('[name="iniciativa"]').val() == ''){
                    alert('Selecione um Estado!');
                    $btn.button('reset');
                    return false;
                }
                if(jQuery("#muncod").val() === null && jQuery("#estuf").val() !== null) {
                    alert('Selecione um Municipio!');
                    $btn.button('reset');
                    return false;
                }
            }

            if( jQuery('[name="tipo_processo"]:checked').length == 0 ){
                alert('Selecione o Tipo de Processo');
                $btn.button('reset');
                return false;
            }
        }

    	
    	jQuery('[name="requisicao"]').val('filtrar');
        jQuery('[name="form-administra-processo"]').submit();
    });
    
    function solicitarConta( proid ) {
    	
       jQuery('[name="proid"]').val(proid);
       jQuery("#modal-login-processo").modal("show");
    }

    jQuery('#estuf').change(function(){
        if($('[name=itrid]:checked').val() == 1){
            return;
        }else{
            if(!Array.isArray($(this).val())){
                listarMunicipios();
            }else{
                carregarMunicipioAnalise($('#estuf').val(),$('#muncod').val());
            }
        }
    });

    function listarMunicipios()
    {
        console.log('testes');
        var options = jQuery('#muncod');
        options.empty();
        options.trigger('chosen:updated');
        // options.append(new Option('', ''));
        jQuery.ajax({
            type: "GET",
            url: window.location.href,
            data: { requisicao:'carregarTodosMunicipios'},
            success: function(retorno){
                $.each(JSON.parse(retorno), function() {
                    options.append(new Option(this.descricao, this.codigo));
                });
                options.val(muncod);
                options.trigger('chosen:updated');
            }
        });
    }

    function carregarMunicipioAnalise(estuf, muncod) {
        if(estuf != '' && $('[name=itrid]:checked').val() != 1) {
            var options = jQuery('#muncod');
            options.empty();
            options.trigger('chosen:updated');
            if(!Array.isArray(estuf)){
                return;
            }
            $('#loading').hide();
            // options.append(new Option('', ''));
            jQuery.ajax({
                type: "GET",
                url: window.location.href,
                data: { requisicao:'carregarMunicipiosAnalise',estuf : estuf },
                success: function(retorno){
                    // options.append(new Option('', ''));
                    $.each(JSON.parse(retorno), function() {
                        options.append(new Option(this.descricao, this.codigo));
                    });
                    options.focus();
                    options.val(muncod);
                    options.trigger('chosen:updated');
                }
            });
        }
    }

    $('#muncod').one('focus',function() {
        console.log('on focus');
        if(Array.isArray($('#estuf').val())){
            return;
        }
        listarMunicipios();
    });

    $('#div-muncod').one('click',function() {
        console.log('on click');
        if(Array.isArray($('#estuf').val())){
            return;
        }
        listarMunicipios();
    });

    $('[name=itrid]').change(function(){
        if($(this).val() == 1){
            $('#div-muncod').slideUp();
            $('#muncod').empty();
            $('#muncod').trigger("chosen:updated");
        }else{
            $('#div-muncod').slideDown();
        }
    });
</script>
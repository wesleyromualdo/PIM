<?php
$obReformulacao = new Par3_Controller_Reformulacao();
$obDocumento = new Par3_Controller_DocumentoTermo();

switch ($_REQUEST['requisicao']) {
    case 'carregaMun':
        $municipio = new Territorios_Model_Municipio();
        print simec_json_encode($municipio->lista(array(
            'muncod',
            'mundescricao||\' - \'||estuf as mundescricao'
        ), array(
            "estuf = '{$_REQUEST['estuf']}'"
        )));
        die();
        break;
    case 'salvar-termo':
        $_REQUEST['aditivo'] = 'S';
        $_REQUEST['bloqueado'] = 'N';
        $_REQUEST['tirid'] = 2;
        $_REQUEST['aedid'] = 5651;
        $mdoconteudo = $obDocumento->salvar_termo($_REQUEST);
        die();
        break;
    case 'verificaTermoEmGeracao':
        echo  $obReformulacao->verificaTermoEmGeracaoReformulacao($_REQUEST['dotid'], $_REQUEST['proid'], PAR3_REFORMULACAO_ESDID_AGUARDANDO_GERACAO_TERMO);
        die();
        break;
    case 'visualiza_termo':
        ob_clean();
        $mdoconteudo = $obDocumento->pegaTermoCompromissoArquivo($_REQUEST['dotid']);
        $mdoconteudo = str_ireplace('\"', '"', $mdoconteudo);
        echo simec_html_entity_decode($mdoconteudo);
        exit();
        break;
    case 'montar_tela_termo':
        ob_clean();
        $obReformulacao->montarTelaTermoAditivo($_REQUEST['dotid_original'], $_REQUEST['refid'], $_REQUEST['inuid'], $_REQUEST['proid']);
        exit();
        break;
    case 'montar_combo_situacao':
        ob_clean();
        $arrSituacao = $obReformulacao->comboSituacaoReformulacao($_REQUEST['tirid']);
        $arrAttr = array(
            'data-placeholder' => 'Situação',
            'maxlength' => '255'
        );
        echo $simec->select('esdid', 'Situação', $_REQUEST['esdid'], $arrSituacao, $arrAttr, array(
            'input-size' => '8'
        ));
        exit();
        break;
    case 'gerar_termo':
        ob_clean();
    
        if (empty($_REQUEST['dotid'])) {
            $db->executar("DELETE FROM par3.documentotermoarquivo WHERE dotid IN (SELECT dotid FROM par3.documentotermo WHERE proid = {$_REQUEST['proid']} AND dotstatus = 'B' AND dotidpai IS NOT NULL);
                        DELETE FROM par3.termocomposicao WHERE dotid IN (SELECT dotid FROM par3.documentotermo WHERE proid = {$_REQUEST['proid']} AND dotstatus = 'B' AND dotidpai IS NOT NULL);
                        DELETE FROM par3.documentotermo WHERE dotid IN (SELECT dotid FROM par3.documentotermo WHERE proid = {$_REQUEST['proid']} AND dotstatus = 'B' AND dotidpai IS NOT NULL);");
        }

        $_REQUEST['requisicao'] = 'carregar-termo';
        $obDocumento = new Par3_Controller_DocumentoTermo();

        $_REQUEST['bloqueado'] = 'S';
        $_REQUEST['aditivo'] = 'S';
        $_REQUEST['tirid'] = 2;
        $_REQUEST['dotid_ref'] = $_REQUEST['dotid_original'];

        $termoOriginal = $db->pegaLinha("SELECT to_char(dotdatainiciovigencia, 'DD/MM/YYYY') as dotdatainiciovigencia, to_char(dotdatafimvigencia, 'DD/MM/YYYY') as dotdatafimvigencia 
                                        FROM par3.documentotermo 
                                        WHERE proid = {$_REQUEST['proid']} AND dotstatus = 'A'");

        $_REQUEST['dotdatainiciovigencia'] = $termoOriginal['dotdatainiciovigencia'];
        $_REQUEST['dotdatafimvigencia'] = $termoOriginal['dotdatafimvigencia'];

        $dotid = $obDocumento->salvar_termo($_REQUEST);
        
        if( $_REQUEST['refid'] ){
            $contrapartida = $db->pegaUm("SELECT (refvalorentidade + refrafmecfnde) FROM par3.reformulacao_documento WHERE refid = {$_REQUEST['refid']};");
            $_REQUEST['contrapartida_reformulacao'] = $contrapartida;
        }

        $_REQUEST['dotid'] = $dotid;
        $_REQUEST['aditivo'] = 'N';
        $_REQUEST['reformulacao'] = 'T';
        $_REQUEST['dotid_ref'] = '';
        $mdoconteudo = $obDocumento->alteraMacroDocumento($_REQUEST);
        $_REQUEST['mdoconteudo_t'] = $mdoconteudo;
        $obDocumento->salvar_termo($_REQUEST);
        $mdoconteudo = str_replace('"', "'", $mdoconteudo);
        echo simec_json_encode(array(
            'texto' => simec_html_entity_decode($mdoconteudo),
            'dotid' => $dotid
        ));
        exit();
        break;
    case 'detalharReformulacao':
        ob_clean();
        $dotid = $_REQUEST['dados'][0];
        $inuid = $_REQUEST['dados'][1];
        $proid = $_REQUEST['dados'][2];
        $tirid = $_REQUEST['dados'][3];
        echo $obReformulacao->carregaHistoricoReformulacao($dotid, $inuid, $proid, $tirid);
        exit();
        break;
    case 'historicoSituacao':
        ob_clean();
        $controllerObra = new Par3_Controller_Obra();
        echo $controllerObra->listarHistoricoTramitacaoModal($_POST['docid']);
        die();
        break;
    case 'recuperarcomentario':
        ob_clean();
        $comentario = $db->recuperar("SELECT cmddsc FROM workflow.comentariodocumento WHERE hstid = {$_POST['hstid']}");
        echo simec_json_encode($comentario['cmddsc']);
        die();
        break;
}

require APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';

$db->cria_aba($abacod_tela, $url, '');

$sqlIniciatva = "SELECT ini.iniid AS codigo,
                	ini.iniid||' - '||id.inddsc AS descricao
                FROM par3.iniciativa ini
                	INNER JOIN par3.iniciativa_descricao id ON id.indid = ini.indid
                WHERE ini.inistatus = 'A'
                ORDER BY ini.iniid";

$sqlTipoAssitencia = Par3_Model_IniciativaPlanejamento::getTipos_Assistencia();
$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$arrIniciativaAno = "SELECT DISTINCT to_char(refdata, 'YYYY') AS codigo, to_char(refdata, 'YYYY') AS descricao FROM par3.reformulacao_documento";
$arrAreaRelacionada = Par3_Model_IniciativaAreas::getAreaPorIniciativa();
?>
<style>
.termo_detalhe {
	cursor: pointer;
	color: blue;
}

.termo_detalhe:hover {
	cursor: pointer;
	color: #87CEFA;
}

.navbar-listagem {
	margin-top: 0px;
	float: right;
	width: 500px;
}
</style>

<script language="javascript" type="text/javascript"
	src="../includes/tinymce/tiny_mce.js"></script>

<div id="debug"></div>
<div class="ibox">
	<div class="ibox-title">
		<div class="row">
			<div class="col-md-12">
				<h2 class="center">Reprogramação</h2>
			</div>
		</div>
	</div>
	<div class="ibox-content">
		<div class="row">
			<form name="form-pesquisa-reformulacao"
				id="form-pesquisa-reformulacao" class="form-horizontal"
				method="post">
				<input type="hidden" name="requisicao" id="requisicao"
					value="filtrar" />
				<div class="row">
					<div class="col-lg-6">
                        <?php
                        $filtro = simec_htmlentities($_POST['numeroprocesso']);
                        $numeroprocesso = $filtro;
                        $config = array();
                        $attribs = array(
                            'maxlength' => '255',
                            'onkeyup' => "this.value=mascaraglobal('#####.######/####-##',this.value)",
                            'onblur' => "MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);",
                            'autofocus'
                        );
                        echo $simec->input('numeroprocesso', 'Número de Processo', $numeroprocesso, $attribs, array(
                            'input-size' => '8'
                        ));
                        $_REQUEST['tipo_processo'] = (empty($_REQUEST['tipo_processo']) ? 'T' : $_REQUEST['tipo_processo']);
                        ?>
                        <?php echo $simec->input('dotnumero', 'Número do Termo', $_REQUEST['dotnumero'], array(), array('input-size' => '8')); ?>
                        <?php echo $simec->input('proid', 'Código do Processo', $_POST['proid'], array('type' => 'number'), array('input-size' => '8')); ?>
                        <?php echo $simec->input('codigo', 'Código Iniciativa', $_REQUEST['codigo'], array('type' => 'number'), array('input-size' => '8')); ?>
                        <?php 
                        $arrSituacao = $obReformulacao->comboSituacaoReformulacao();
                        $arrAttr = array('data-placeholder' => 'Situação', 'maxlength' => '255');
                        echo $simec->select('esdid', 'Situação', $_REQUEST['esdid'], $arrSituacao, $arrAttr, array('input-size' => '8'));
                        ?>
                        <?php echo $simec->select('tipoassistencia', 'Tipo de Assistência', $_REQUEST['tipoassistencia'], $sqlTipoAssitencia, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        
                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Data da Solicitação:</label>
                            <div class="col-lg-5 col-md-5 col-sm-5">
                                <div class="" id="data_1">
                                    <div class="input-group m-b" id="datepicker">
                                        <span class="input-group-addon" style="padding: 4px 12px !important; background-color: #fff;height:34px;"><i class="fa fa-calendar"></i></span>
                                        <input type="text" class="form-control datemask daterange" autocomplete="off" name="dataini" id="dataini" value="<?= $_REQUEST['dataini']; ?>">
                                        <span class="input-group-addon" style="padding: 4px 12px !important; background-color: #fff;height:34px;">a</span>
                                        <input type="text" class="form-control datemask daterange" autocomplete="off" name="datafim" id="datafim" value="<?= $_REQUEST['datafim'];?>">
                                    </div>
                                </div>
                            </div>
                        </div>
                       </div>
					<div class="col-lg-6">
                        <?php echo $simec->radio('itrid', 'Esfera', ($_REQUEST['itrid']?:0), [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'], array('maxlength' => '255')); ?>
                        <div class="row">
							<div class="col-lg-5" style="margin-left: 55.5px;">
                                <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255'), array('input-size' => '6', 'label-size' => '5')); ?>
                            </div>
							<div class="col-lg-5" id="div-muncod" style="display:<?= $_REQUEST['itrid'] == 1 ?'none':''?>;">
                                <?php
                                $_REQUEST['muncod'] = tratarArrayParaMultiSelect($_REQUEST['muncod']);
                                if ($_REQUEST['estuf']) {
                                    $municipio = new Territorios_Model_Municipio();
                                    $arrMunicipio = simec_preparar_array($municipio->lista(array(
                                        'muncod as codigo',
                                        'estuf ||\'-\'|| mundescricao as descricao'
                                    ), array(
                                        "estuf = '{$_REQUEST['estuf']}'"
                                    )));
                                }
                                echo $simec->select('muncod[]', 'Município', $_REQUEST['muncod'], $arrMunicipio, array(
                                    'maxlength' => '255',
                                    'multiple' => 'multiple'
                                ), array());
                                ?>
                            </div>
						</div>
						<?php echo $simec->select('iniid', 'Iniciativa', $_REQUEST['iniid'], $sqlIniciatva, array('maxlength' => '255', 'multiple' => 'multiple'), array('input-size' => '8')); ?>
                        <?php
                        $arrTipoObjeto = Par3_Model_IniciativaPlanejamento::getTipoObjetoporIniciativa();
                        echo $simec->select('intoid', 'Tipo do Objeto', $_REQUEST['intoid'], $arrTipoObjeto, array(
                            'maxlength' => '255', 'multiple' => 'multiple'
                        ), array(
                            'input-size' => '8'
                        ));
                        ?>
                        <div id="div_situacao">
                        <?php
                        echo $simec->select('iarid', 'Área Relacionada', $_REQUEST['iarid'], $arrAreaRelacionada, array(
                            'maxlength' => '255'
                        ), array(
                            'input-size' => '8'
                        ));
                        ?>
                        </div>
					</div>
				</div>
			</form>
			<div class="ibox-footer">
				<div class="center">
					<button type="button" id="btn-pesquisar" data-dismiss="modal"
						class="btn btn-info btn-pesquisar"
						data-loading-text="Pesquisando, aguarde ...">
						<i class="fa fa-search"></i> Pesquisar
					</button>
					|
					<button type="button" id="btn-excel" class="btn btn-primary"
						name="excel">
						<i class="fa fa-file-excel-o" aria-hidden="true"></i> Gerar Excel
					</button>
					|
					<button type="reset" class="btn btn-default-bright btn-limpar"
						id="limpar">
						<i class="fa fa-eraser"></i>Limpar
					</button>
				</div>
			</div>
			<!-- Listagem  -->
			<div class="float-e-margins" id="listagem-obras">
				<div class="ibox-content" id="div-listagem-obras"
					style="display: block;">
					<div class="table-responsive">
						<div>
							<div id="testeSQL">
                                <?php
                                if ($_REQUEST['requisicao'] == 'filtrar' || $_REQUEST['requisicao'] == 'excel' || $_REQUEST['proid']) {
                                    $_REQUEST['tirid'] = '2';
                                    echo $obReformulacao->listar($_REQUEST);
                                }
                                ?>
                            </div>
						</div>
					</div>
					<br> <br>
				</div>
			</div>
		</div>
	</div>
</div>
<div class="ibox float-e-margins animated modal" id="modal_historico"
	role="dialog" aria-hidden="true">
	<div class="modal-dialog modal-lg">
		<div class="modal-content">
			<div class="ibox-title">
				<button type="button" class="close" data-dismiss="modal"
					aria-label="Close">
					<span aria-hidden="true">&times;</span>
				</button>
				<h3 id="modal_titulo" align="center">Histórico</h3>
			</div>
			<div class="ibox-content" id="conteudo-modal"></div>
			<div class="ibox-footer">
				<div class="row">
					<button type="submit" id="cancelarDescIniciativa"
						data-dismiss="modal" class="btn btn-default">
						<i class="fa fa-times-circle-o"></i> Fechar
					</button>
				</div>
			</div>
		</div>
	</div>
</div>

<div class="ibox float-e-margins animated modal conteudo"
	id="modal-gerar-termo" tabindex="-1" role="dialog" aria-hidden="true">
	<div class="modal-dialog modal-lg" style="width: 80%" id="printable">
		<div class="modal-content">
			<div class="ibox-content">
				<div class="row">
					<div class="ibox-content">
						<div id="termo-modal"></div>
					</div>
				</div>
			</div>
			<div class="ibox-footer notprint" style="text-align: center;">
				<button type="button" id="btn-fechar-modelo" data-dismiss="modal"
					class="btn btn-default">
					<i class="fa fa-times-circle-o"></i> Fechar
				</button>
				<button type="button" id="btn-salvar-termo" data-dismiss="modal"
					class="btn btn-primary btn-salvar-termo"
					data-loading-text="Salvando aguarde ...">
					<i class="fa fa-save"></i> Salvar Termo
				</button>
			</div>
		</div>
	</div>
</div>

<script>
    $('[data-toggle="popover"]').popover();
    $('[data-toggle="popover"]').mouseleave(function(e) {
        e.stopPropagation();
        $(document).find('.popover').remove();
    });
    jQuery(document).keypress(function(e) {
        if(e.which == 13){
             jQuery('.btn-pesquisar').click();
        }
    });

    jQuery(function () {
    	$('#data_1 .input-daterange').datepicker({
            format: "dd/mm/yyyy",
            language: "pt-BR",
        });
        
    	$('body').on('click','#btn-excel', function(){
    		 //$btn = jQuery(this).button('loading');
    	     jQuery('[name="requisicao"]').val('excel');
    	     jQuery('[name="form-pesquisa-reformulacao"]').submit();
        });
        
    	$('body').on('click','.termo_detalhe', function(){
            id = $(this).find('.dotid').attr('value');
            visualizaTermo(id);
        });
        
        $(window).on('beforeunload',function(){
            //$('.loading-dialog-par3').show();
        });
        $(window).on('click','#btn-pesquisar #limpar',function(){
            $('.loading-dialog-par3').show();
        });
        $(window).load(function(){
            $('.loading-dialog-par3').hide();
        });
        //renderizeMunicipio();

        if (jQuery('[name="estuf"]').val() != '') {
            console.log(<?= json_encode($_REQUEST['muncod']);?>);
            carregarMun(jQuery('[name="estuf"]').val(), <?= json_encode($_REQUEST['muncod']);?>);
        }

        jQuery("input:radio[name=itrid], select[name=estuf]").change(function () {
            //renderizeMunicipio();
        });

        jQuery('select[name=estuf]').change(function () {
            carregarMun(this.value);
        });
        
        jQuery('input:radio[name=tirid]').change(function () {
            var tirid = jQuery(this).val();
            
        	var caminho = window.location.href;
            var action = '&requisicao=montar_combo_situacao&tirid='+tirid;
            jQuery.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    jQuery("#div_situacao").html(resp);
                }
            });
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

    $('[name=itrid]').change(function(){
        if($(this).val() == 1){
            $('#div-muncod').hide();
            $('#muncod').val('');
            $('#muncod').trigger("chosen:updated");
        }else{
            $('#div-muncod').show();
        }
    });
    
    function visualizaReformulacao(dotid, inuid, proid, refid, tipo ){
        //window.location.href = "par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid="+dotid+"&inuid="+inuid+"&proid="+proid+"&refid="+refid+"&tipo="+tipo;
        window.open(
        		"par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid="+dotid+"&inuid="+inuid+"&proid="+proid+"&refid="+refid+"&tipo="+tipo,
        		  '_blank' // <- This is what makes it open in a new window.
        		);
    }

    function verificaTermoEmGeracao(dotid, proid){
    	var caminho = window.location.href;
        var action = '&requisicao=verificaTermoEmGeracao&dotid='+dotid+'&proid='+proid;
        var retorno = 'N';
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
            	retorno = resp;
            }
        });
        return retorno;
    }

    function gerarTermo(dotid, refid, inuid, proid, dotid_id) {

    	tinyMCE.execCommand('mceRemoveControl', true, 'mdoconteudo');
        jQuery("#termo-modal").html('');
    	var caminho = window.location.href;
        var action = '&requisicao=montar_tela_termo&dotid_original='+dotid_id+'&refid='+refid+'&inuid='+inuid+'&proid='+proid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery("#termo-modal").html(resp);
            }
        });
        jQuery('#mdoconteudo_tbl').css('width', '100%'); 
        jQuery('#mdoconteudo_tbl').css('height', '546px'); 

        jQuery('#mdoconteudo_ifr').css('width', '100%'); 
        jQuery('#mdoconteudo_ifr').css('height', '477px');
        
        $("#modal-gerar-termo").modal("show");
    }

    function carregarTermoAditivo(){

        var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): <br> <br>';
        var validacao = true;
        
        if( jQuery('#mdoid').val() == '' ){
            mensagem += 'Modelo de documento <br>';
            validacao = false;
        }
        if( !validacao ){
            alert(mensagem);
            return false;
        }else{
        	jQuery('.loading-dialog-par3').show();
        	
            //$btn = jQuery('#btn-salvar-termo').button('loading');
            jQuery('[name="requisicao"]').val('carregar-termo');

            var mdoid = jQuery('[name="form-gerar-termo"]').find('[name="mdoid"]').val();
            var dotid_original = jQuery('[name="form-gerar-termo"]').find('[name="dotid_original"]').val();
            var proid = jQuery('[name="form-gerar-termo"]').find('[name="proid"]').val();
            var inuid = jQuery('[name="form-gerar-termo"]').find('[name="inuid"]').val();
            var refid = jQuery('[name="form-gerar-termo"]').find('[name="refid"]').val();
            var dotid = jQuery('[name="form-gerar-termo"]').find('[name="dotid"]').val();
            
            //jQuery('[name="form-gerar-termo"]').submit();
            var caminho = window.location.href;
            var action = '&requisicao=gerar_termo&dotid_original='+dotid_original+'&refid='+refid+'&inuid='+inuid+'&proid='+proid+'&mdoid='+mdoid+'&dotid='+dotid;
            jQuery.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: true,
                success: function (resp) {
                    //jQuery('#debug').html(resp);
                    
                	var arRetorno = jQuery.parseJSON(resp);
                    //console.log(arRetorno['texto']);
                    
                	jQuery('[name="mdoconteudo_t"]').val( arRetorno['texto'] );
                	jQuery('[name="form-gerar-termo"]').find('[name="dotid"]').val( arRetorno['dotid'] );
                	tinyMCE.get('mdoconteudo').setContent( jQuery('[name="mdoconteudo_t"]').val() );
                	jQuery('.loading-dialog-par3').hide();
                }
            });
        }
    }

    jQuery('#btn-salvar-termo').click(function(){
        var texto = tinyMCE.get('mdoconteudo').getContent();

        jQuery('#mdoconteudo_t').val(texto);

        if( texto != '' ){
        	jQuery('.loading-dialog-par3').show();
            $btn = jQuery(this).button('loading');
            jQuery('[name="requisicao"]').val('salvar-termo');
            jQuery('[name="form-gerar-termo"]').submit();
        } else {
            alert('É necessário carregar o documento antes de salvar.');
            jQuery(this).button('reset')
            return false;
        }
    });
    
    function carregarMun(estuf, muncod) {
        if (estuf != '') {
            var options = jQuery('#muncod');
            options.empty();
            options.append(new Option('', ''));
            jQuery.post('', 'requisicao=carregaMun&estuf=' + estuf, function (retorno) {
                options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function () {
                    options.append(new Option(this.mundescricao, this.muncod));
                });
                options.focus();
                options.val(muncod);
                options.trigger('chosen:updated');
            });
        }
    }

    jQuery(".btn-limpar").click(function () {
        window.location.href = 'par3.php?modulo=principal/planoTrabalho/planejamento/listaReformulacao&acao=A';
    });

    jQuery(".btn-pesquisar").click(function () {
		$('.loading-dialog-par3').show();
        $btn = jQuery(this).button('loading');
        jQuery('[name="requisicao"]').val('filtrar');
        jQuery('[name="form-pesquisa-reformulacao"]').submit();
    });

    $('[data-toggle="popover"]').popover();

    function visualizaTermo(dotid, dotid_ref) {
        var caminho = window.location.href;
        var action = '&requisicao=visualiza_termo&dotid=' + dotid_ref;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery("#div_conteudo_termo").html(resp);
                jQuery('[name="dotid"]').val(dotid_ref);

            }
        });
        $("#modal-visualiza-modelo").modal("show");
    }
    
    $('[data-toggle="popover"]').popover();
    function mostrarHistoricoSituacao(docid)
    {
        var data = {requisicao:'historicoSituacao',docid:docid};
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: data,
            async: false,
            success: function (resp) {
                $('#loading').hide();
                $("#modal_historico").find('#conteudo-modal').html('');
                $("#modal_historico").modal();
                $("#modal_historico").find('#conteudo-modal').html(resp);
            }
        });
    }
    function historicoBootstrapComentario(hstid) {
        var id = '#arow-' + hstid;
        console.log(hstid);
        var parentTr = $(id).closest('tr');

        if ($(id + ' span').hasClass('open')) {
            parentTr.next().remove();
            $(id + ' span').removeClass('open').removeClass('btn-default').addClass('btn-info');
        } else {
            $(id + ' span').addClass('open').removeClass('btn-info').addClass('btn-default');
            ;
            var numCols = $('td', parentTr).length;
            numAcao = parentTr.parents('table').attr('data-qtd-acoes');
            td_acao = '<td colspan="' + numAcao + '">&nbsp;</td>';

            var caminho = window.location.href;
            var action = '&requisicao=recuperarcomentario&hstid=' + hstid;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $('#loading').hide();
                    var comentario = $.parseJSON(resp);
                    console.log(comentario);
                    parentTr.after('<tr>' + td_acao + '<td colspan="' + numCols + '"><blockquote style="text-align:left">' + comentario + '</blockquote></td></tr>');
                }
            });
        }
    }
</script>

<div class="ibox float-e-margins animated modal conteudo"
	id="modal-visualiza-modelo" tabindex="-1" role="dialog"
	aria-hidden="true">
	<div class="modal-dialog modal-lg" style="width: 80%" id="printable">
		<div class="modal-content">
			<div class="ibox-content">
				<div class="row">
					<div class="ibox-content">
                        <?php echo $obDocumento->montaHtmlTermo(); ?>
                    </div>
				</div>
			</div>
			<div class="ibox-footer notprint" style="text-align: center;">
				<button type="button" id="btn-fechar-modelo" data-dismiss="modal"
					class="btn btn-default">
					<i class="fa fa-times-circle-o"></i> Fechar
				</button>
				<button type="button" id="btn-imprimir" data-dismiss="modal"
					class="btn btn-default" onclick="imprimirTermo();">
					<i class="fa fa-times-circle-o"></i> Imprimir
				</button>
			</div>
		</div>
	</div>
</div>
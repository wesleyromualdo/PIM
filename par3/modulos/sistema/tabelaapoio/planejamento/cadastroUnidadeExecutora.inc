<?php
/**
 * Arquivos que vão popular os campos
 * Totas as consulta devem trazer apenas dados de iniciativas cadastradas cadastroUnidadeExecutora
 */
$pesquisaProcesso = new Par3_Controller_Processo();
$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);

require APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela, $url, '');

if ($_REQUEST['carregarDadosUnidadeExecutora'] == 'S') {
    ob_clean();
    $sql = "SELECT ungid, inuid, formata_cpf_cnpj(ungcnpj) as ungcnpj, ungrazao_social, ungdatacadastro, usucpf, ungstatus
            FROM par3.unidade_executora WHERE ungid = {$_REQUEST['ungid']}";
    
    $arDados = $db->pegaLinha($sql);
    
    $arInstrumento = $db->pegaLinha("select i.estuf, i.muncod, i.itrid from par3.instrumentounidade i where inuid = {$arDados['inuid']}");
    
    $arqid = $arDados['arqid'];
    $estuf_s = $arInstrumento['estuf'];
    $muncod_s = $arInstrumento['muncod'];
    $itrid_s = $arInstrumento['itrid'];
    $ungrazao_social = $arDados['ungrazao_social'];
    $ungcnpj = $arDados['ungcnpj'];
    
    $nomeAnexo = '';
    if( $arqid ){
        $file = new FilesSimec();
        $nomeAnexo = $file->getNomeArquivo($arqid);
    }
    $dados = "{\"estuf_s\": \"$estuf_s\", \"muncod_s\" : \"$muncod_s\", \"itrid_s\" : \"$itrid_s\", \"ungrazao_social\" : \"$ungrazao_social\", \"ungcnpj\" : \"$ungcnpj\"}";
    echo $dados;
    die;
} 

switch ($_REQUEST['requisicao']) {  
    case 'getPessoaJuridica':
        ob_clean();
        $return = [];
        require_once APPRAIZ . 'www/includes/webservice/PessoaJuridicaClient.php';
        $pessoaJuridica = new PessoaJuridicaClient("http://ws.mec.gov.br/PessoaJuridica/wsdl");
        
        $pj = str_replace(array('/', '.', '-'), '', $_POST['cnpj']);
        $xml = $pessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($pj);
        $xml = str_replace(array("& "), array("&amp; "), $xml);
        $obj = (array) simplexml_load_string($xml);
        
        if ($obj['PESSOA']->ERRO) {
            $return['sucesso'] = false;
            $return['mensagem'] = 'CNPJ inexistente na base da Receita Federal.';
        } else {
            $empresa  = (array) $obj['PESSOA'];
            $endereco = (array) $obj['PESSOA']->ENDERECOS->ENDERECO;
            
            $return['sucesso'] = true;
            $return['empresa'] = array_merge($empresa, $endereco);
        }
        
        die(simec_json_encode($return));
        break;
    case 'salvar':
        
        if( $_REQUEST['itrid_s'] == '2' ){
            $inuid = $db->pegaUm("SELECT inuid FROM par3.instrumentounidade WHERE estuf = '{$_REQUEST['estuf_s']}' AND muncod = '{$_REQUEST['muncod_s']}' and itrid = '{$_REQUEST['itrid_s']}' AND inustatus = 'A'");
        } else {
            $inuid = $db->pegaUm("SELECT inuid FROM par3.instrumentounidade WHERE estuf = '{$_REQUEST['estuf_s']}' and itrid = '{$_REQUEST['itrid_s']}' AND inustatus = 'A'");
        }
        if( $_REQUEST['ungid_s'] ){
            $db->executar("UPDATE par3.unidade_executora SET ungstatus = 'I' WHERE ungid = {$_REQUEST['ungid_s']}");
        }
        
        $_REQUEST['ungcnpj'] = str_replace(array('/', '.', '-'), '', $_REQUEST['ungcnpj']);
        
        $sql = "INSERT INTO par3.unidade_executora(inuid, ungcnpj, ungrazao_social, usucpf)
                VALUES($inuid, '{$_REQUEST['ungcnpj']}', '{$_REQUEST['ungrazao_social']}', '{$_SESSION['usucpf']}')";
                $db->executar($sql);
        
        $url_pagina = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastroUnidadeExecutora';
        
        if ($db->commit()) {
            simec_redirecionar($url_pagina, 'success');
        } else {
            simec_redirecionar($url_pagina, 'error');
        }
        break;
    case 'delete':
        $boTem = $db->pegaUm("SELECT count(ue.ungid)
                            FROM par3.unidade_executora ue
                            	INNER JOIN par3.processo p ON p.inuid = ue.inuid AND p.procnpj = ue.ungcnpj AND p.prostatus = 'A'
                            WHERE ungstatus = 'A' AND ue.ungid = {$_REQUEST['ungid']}");
        if( (int)$boTem == (int)0 ){
            $db->executar("UPDATE par3.unidade_executora SET ungstatus = 'I' WHERE ungid = {$_REQUEST['ungid']}");        
            $url_pagina = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastroUnidadeExecutora';
            
            if ($db->commit()) {
                simec_redirecionar($url_pagina, 'success');
            } else {
                simec_redirecionar($url_pagina, 'error');
            }
        } else {
            $url_pagina = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastroUnidadeExecutora';
            simec_redirecionar($url_pagina, 'error', 'Não e possivel excluir está entidade executora, já existe vinculo com o processo!');
        }
        break;

}

?>

<div id="debug"></div>
<div class="ibox">
	<div class="ibox-title">
		<div class="row">
			<div class="col-md-12">
				<h2 class="center">Entidade Executora</h2>
			</div>
		</div>
	</div>
	<div class="ibox-content">
		<div class="row">
			<form name="form-pesquisa_unidade" id="form-pesquisa_unidade" class="form-horizontal" method="post">
				<input type="hidden" name="requisicao" id="requisicao" value="" /> 
				
				<div class="row">
					<div class="col-lg-6">
                        <?php echo $simec->radio('itrid', 'Esfera', $_REQUEST['itrid'] ? $_REQUEST['itrid'] : 2, ['9999' => 'Todos', '1' => 'Estadual', '2' => 'Municipal']); ?>
                        <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, ['maxlength' => '255'], []); ?>
                        <?php echo $simec->select('muncod', 'Município', $_REQUEST['muncod'], $arrMunicipio, ['maxlength' => '255'], []); ?>
                        
                        <div class="form-group ">
                            <label for="ungcnpj" class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">CNPJ: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                               <input type="text" id="ungcnpj_p" name="ungcnpj_p" class="form-control" maxlength="18" required
                                               onkeyup="this.value=mascaraglobal('##.###.###/####-##',this.value);"
                                               onblur="this.value=mascaraglobal('##.###.###/####-##',this.value);" value="<?php echo $_REQUEST['ungcnpj_p']?>">
                            </div>
                            <div style="clear:both"></div>
                        </div>
                    </div>					
				</div>
			</form>
			<div class="ibox-footer">
				<div class="center">
					<button type="button" id="btn-pesquisar" class="btn btn-primary btn-pesquisar"><i class="fa fa-search"></i> Pesquisar</button>
					<button type="button" class="btn btn-success" id="btn-incluir" onclick="inserirDocumentoAnexo()"><i class="fa fa-plus"></i> Incluir</button>
				</div>
			</div>
			<!-- Listagem  -->
			<div class="float-e-margins" id="listagem-obras">
				<div class="ibox-content" id="div-listagem-obras"
					style="display: block;">
					<div class="table-responsive">
						<div>
							<div id="debug"></div>
							<div id="testeSQL">
                                <?php
                                $whereD = '';
                                if( $_REQUEST['itrid'] == 1 ) $whereD = " and iu.muncod is null ";
                                if( $_REQUEST['itrid'] == 2 ) $whereD = " and iu.muncod is not null ";
                                if( $_REQUEST['estuf'] ) $whereD = " and iu.estuf = '{$_REQUEST['estuf']}' ";
                                if( $_REQUEST['estuf'] ) $whereD = " and iu.muncod = '{$_REQUEST['muncod']}' ";
                                if( $_REQUEST['ungcnpj_p'] ){
                                    $_REQUEST['ungcnpj_p'] = str_replace(array('/', '.', '-'), '', $_REQUEST['ungcnpj_p']);
                                    $whereD = " and ug.ungcnpj = '{$_REQUEST['ungcnpj_p']}' ";
                                } 
                                
                                $sql = "SELECT ug.ungid, ug.inuid, ungstatus,
                                        	case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                                        	case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
                                        	formata_cpf_cnpj(ug.ungcnpj) AS ungcnpj, ug.ungrazao_social,
                                        	formata_cpf_cnpj(us.usucpf) AS usucpf,
                                        	us.usunome,
                                            to_char(ug.ungdatacadastro, 'DD/MM/YYYY HH24:MI:SS'),
                                            (SELECT count(proid) FROM par3.processo WHERE procnpj = ug.ungcnpj  AND inuid = ug.inuid AND prostatus = 'A') AS tem_processo	
                                        FROM par3.unidade_executora ug
                                        	INNER JOIN par3.instrumentounidade iu ON iu.inuid = ug.inuid
                                        	INNER JOIN seguranca.usuario us ON us.usucpf = ug.usucpf
                                        	LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
                                        	LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                                        WHERE iu.inustatus = 'A' and ug.ungstatus = 'A' $whereD";
                                $listagemSimec = new Simec_Listagem();
                                $arrayCabecalho = array('UF', 'Entidade', 'CNPJ', 'Razão Social', 'CPF', 'Quem Cadastrou', "Quando Cadastrou");
                                $esconderColunas = array('ungid','inuid', 'ungstatus', 'tem_processo');
                                $listagemSimec->setCabecalho($arrayCabecalho);                                
                                $listagemSimec->esconderColunas($esconderColunas);
                                $listagemSimec->setQuery($sql);
                                $listagemSimec->addAcao('edit', 'editarUnidade');
                                $listagemSimec->addAcao('delete', 'deletaUnidade');
                                $listagemSimec->setAcaoComoCondicional(['delete', 'edit'], [
                                    [
                                        'campo' => 'tem_processo',
                                        'valor' => '0',
                                        'op' => 'igual'
                                    ]
                                ]);                              
                                $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                                $listagemSimec->setTamanhoPagina(100);                                
                                $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                                ?>
                            </div>
						</div>
					</div>
					<br> <br>
				</div>
			</div>
			<!-- END Listagem  -->

		</div>
	</div>

</div>

<!--modal-->
<div class="ibox float-e-margins animated modal" id="modal_inclui_unidade_executora" tabindex="-1" role="dialog" aria-hidden="true">
	<div class="modal-dialog">
		<form method="post" name="formsalvar" id="formsalvar" class="form form-horizontal" enctype="multipart/form-data">
			<div class="modal-content">
				<div class="ibox-title">
					<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
					<h3 id="modal_titulo">Entidade Executora - Incluir</h3>
				</div>
				<div class="ibox-content" id="conteudo-modal">
					<input type="hidden" name="ungid_s" id="ungid_s" value=""> 
					<input type="hidden" name="requisicao" id="requisicao" value="salvar">

					<div>
                        <?php echo $simec->radio('itrid_s', 'Esfera', $itrid_s ? $itrid_s : 2, ['1' => 'Estadual', '2' => 'Municipal']); ?>
                        <?php echo $simec->select('estuf_s', 'Estado', $estuf_s, $arrEstado, ['maxlength' => '255'], []); ?>
                        <?php echo $simec->select('muncod_s', 'Município', $muncod_s, $arrMunicipio, ['maxlength' => '255'], []); ?>
                        <div class="form-group ">
                            <label for="ungcnpj" class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">CNPJ *: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                               <input type="text" id="ungcnpj" name="ungcnpj" class="form-control" maxlength="18" required
                                               onkeyup="this.value=mascaraglobal('##.###.###/####-##',this.value);"
                                               onblur="this.value=mascaraglobal('##.###.###/####-##',this.value);">
                            </div>
                            <div style="clear:both"></div>
                        </div>
                        <div class="form-group ">
                            <label for="ungrazao_social" class="col-sm-3 col-md-3 col-lg-3 control-label  control-label">Razão social *: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                               <input type="text" id="ungrazao_social" name="ungrazao_social" class="form-control" maxlength="18">
                            </div>
                            <div style="clear:both"></div>
                        </div>
                    </div>

				</div>
				<div class="ibox-footer">
					<div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
						<button type="submit" id="cancelar" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>
						<button type="button" id="confirmarSalvar" class="btn btn-success"><i class="fa fa-plus-square-o"></i> Salvar</button>
					</div>
				</div>
			</div>
		</form>
	</div>
</div>

<script>

function inserirDocumentoAnexo() {
	jQuery('[name="ungcnpj"]').val('');
	jQuery('[name="ungrazao_social"]').val('');
	jQuery('[name="estuf_s"]').val('');
	jQuery('[name="muncod_s"]').val('');
    jQuery("#modal_inclui_unidade_executora").modal();
}

function deletaUnidade(ungid) {

	window.location.href = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastroUnidadeExecutora&requisicao=delete&ungid='+ungid 
}

//serviço cnpj -- trecho comentado por não haver o serviço de consulta de CNPJ
// jQuery('#ungcnpj').on('change', function () {
//
//     var element = jQuery(this);
//
//     if ("" != jQuery('#ungcnpj').val() && 18 != jQuery('#ungcnpj').length) {
//         var cnpj = str_replace(['.', ',', '/', '-'], [''], jQuery('#ungcnpj').val());
//
//         jQuery.post(window.location.href, {requisicao: 'getPessoaJuridica', cnpj: cnpj}, function (data) {
//             var jsonData = jQuery.parseJSON(data);
//             if (jsonData.sucesso == false) {
//             	jQuery('#ungcnpj').val("");
//                 swal("Alerta", jsonData.mensagem, "warning");
//             } else {
//             	jQuery("#ungrazao_social").val(jsonData.empresa.no_empresarial_rf);
//             	jQuery(element).closest('.form-group').removeClass('has-error');
//             }
//         });
//     }
// });


jQuery(function() {
	renderizeMunicipio();
	
    if(jQuery('[name="estuf_s"]').val() != '') {
        carregarMunicipio(jQuery('[name="estuf_s"]').val(), <?=$muncod_s?$muncod_s:'null'?>, '#muncod_s');
    }

    jQuery("input:radio[name=itrid_s], select[name=estuf_s]").change(function() {
    	renderizeMunicipio();
    });

    jQuery('select[name=estuf_s]').change(function(){
        
        carregarMunicipio(this.value, null, '#muncod_s' );
    });

    jQuery('input[name="itrid_s"]').change(function () {
        if (jQuery(this).val() != 3) {
            // remover distrito federal
        }
    });

    function renderizeMunicipio() {
        
        var filtroMunicipio = jQuery("select[name=muncod_s]").parents("div.form-group");
        
        if (jQuery('input:radio[name=itrid_s]:checked').val() === '1') {
            filtroMunicipio.slideUp();
        }else{
        	filtroMunicipio.slideDown();
        }
    }
});


jQuery(function() {
    renderizeMunicipio();
    
    if(jQuery('[name="estuf"]').val() != '') {
        carregarMunicipio(jQuery('[name="estuf"]').val(), <?=$muncod?>);
    }

    jQuery("input:radio[name=itrid], select[name=estuf]").change(function() {
        renderizeMunicipio();
    });

    jQuery('select[name=estuf]').change(function(){
        carregarMunicipio(this.value);
    });

    jQuery('input[name="itrid"]').change(function () {
        if (jQuery(this).val() != 3) {
            // remover distrito federal
        }
    });

    function renderizeMunicipio() {
        var filtroMunicipio = jQuery("select[name=muncod]").parents("div.form-group");
        if (jQuery('input:radio[name=itrid]:checked').val() === '1') {
            filtroMunicipio.slideUp();
        }else{
        	filtroMunicipio.slideDown();
        }
    }
});


jQuery("#confirmarSalvar").click(function () {
	$btn = jQuery(this).button('loading');

	if(jQuery('[name="itrid_s"]:checked').val() == 1){
		if(jQuery('[name="estuf_s"]').val() == ''){
			alert('Selecione um Estado');
			$btn.button('reset');
			return false;
		}
	}
	
	if(jQuery('[name="itrid_s"]:checked').val() == 2){
		if(jQuery('[name="estuf_s"]').val() == ''){
			alert('Selecione um Estado!');
			$btn.button('reset');
			return false;
		}
		if(jQuery('[name="muncod_s"]').val() == ''){
			alert('Selecione um Municipio!');
			$btn.button('reset');
			return false;
		}
	}

	if(jQuery('[name="ungcnpj"]').val() == ''){
		alert('Informe o CNPJ!');
		$btn.button('reset');
		return false;
	}

	jQuery('#formsalvar').find('[name=requisicao]').val('salvar');
	jQuery('[name="formsalvar"]').submit();
});

jQuery(".btn-pesquisar").click(function () {
    $btn = jQuery(this).button('loading');
    jQuery('[name="requisicao"]').val('pesquisar_unidade');
    jQuery('[name="form-pesquisa_unidade"]').submit();
});

function editarUnidade(ungid){
    
    var caminho = window.location.href;
    
    var action = '&ungid=' + ungid+'&carregarDadosUnidadeExecutora=S';
    
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        dataType: "JSON",
        success: function (obj) {

        	$("#ungid_s").val(ungid);

        	if(obj.itrid_s == "1"){
        		$(".itrid_s_1").addClass("active");
        		$("._2").removeClass("active");
        		
        		$("#itrid_s_1").attr("checked", "checked");
        	}else{
        		$(".itrid_s_1").removeClass("active");
        		$("._2").addClass("active");
        		
        		$("#_2").attr("checked", "checked");
        	}
        	
        	$('[name="estuf_s"] option[value="'+obj.estuf_s+'"]').attr("selected", "selected");
        	
        	$('#estuf_s_chosen a span').text($('[name="estuf_s"] option:selected').text());
        	

        	
        	$('[name="muncod_s"] option[value="'+obj.muncod_s+'"]').attr("selected", "selected");
        	
        	$('#muncod_s_chosen a span').text($('[name="muncod_s"] option:selected').text());

        	if( obj.muncod_s == '' ){
        		var filtroMunicipio = jQuery("select[name=muncod_s]").parents("div.form-group");
                
                if (jQuery('input:radio[name=itrid_s]:checked').val() === '1') {
                    filtroMunicipio.slideUp();
                }else{
                	filtroMunicipio.slideDown();
                }
            }
        	
        	$("#ungcnpj").val(obj.ungcnpj);
        	$("#ungrazao_social").text(obj.ungrazao_social);

        	
            $("#itrid_s [value='"+obj.itrid_s+"']").attr("checked", "checked");        	
            
            $("#modal_inclui_unidade_executora").modal();
            $('#modal_titulo').text('Entidade Executora - Editar');

            $( "#ungcnpj" ).trigger( "change" );
        }
    });
}

</script>
<?php
/**
 * Arquivos que vão popular os campos
 * Totas as consulta devem trazer apenas dados de iniciativas cadastradas
 */
$pesquisaProcesso = new Par3_Controller_Processo();
$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);

$sqlListaPendenciaEntidade = "
    SELECT
      pp.dejid,
      iu.estuf,
      iu.inudescricao,
      to_char(pp.dejdata , 'DD/MM/YYYY HH24:MI:SS' ) as data,
      to_char(pp.dejdatalimite , 'DD/MM/YYYY' ) as dejdatalimite,
      formata_cpf_cnpj(pp.usucpf) as cpf,
      u.usunome,
      pp.arqid,
      CASE
        WHEN (pp.dejdatalimite + 1) <= now()
          THEN 'Vencido'
        ELSE 'Vigente'
      END AS situacao,
      CASE
        WHEN pp.dejstatus = 'A'
          THEN 'Ativo'
        ELSE 'Inativo'
      END AS dejstatus,
      CASE
        WHEN length(pp.dejcomentario) > 30
          THEN LEFT(pp.dejcomentario,30) || '...'
        ELSE pp.dejcomentario
      END as comentario
    FROM
      par3.desbloqueio_juridico pp
      left join par3.instrumentounidade iu on iu.inuid = pp.inuid
      left join seguranca.usuario u on u.usucpf = pp.usucpf
    WHERE
      iu.inustatus = 'A' and pp.dejstatus = 'A'
";

require APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela, $url, '');

if (isset($_REQUEST['metodo']) && $_REQUEST['metodo'] == 'download') {
    $file = new FilesSimec();
    $file->getDownloadArquivo($_REQUEST['arqid']);
}

switch ($_REQUEST['requisicao']) {
    case 'desativaDesbloqueioJudicial':
        $sql = "UPDATE par3.desbloqueio_juridico SET
				dejstatus = 'I'
                WHERE dejid = {$_REQUEST['dejid']}";
        
        $db->executar($sql);
        $url_pagina = "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=desbloqueioJudicial";
        if ($db->commit()) {
            simec_redirecionar($url_pagina, 'success');
        } else {
            simec_redirecionar($url_pagina, 'error');
        }
        break;
    case 'excluirAnexo':
        $sql = "UPDATE par3.desbloqueio_juridico SET
				arqid = null
                WHERE dejid = {$_REQUEST['dejid']}";
        
        $db->executar($sql);
        $url_pagina = $_SERVER['HTTP_REFERER'];
        if ($db->commit()) {
            $file = new FilesSimec();
            $file->excluiArquivoFisico($_REQUEST['arqid']);
            
            simec_redirecionar($url_pagina, 'success');
        } else {
            simec_redirecionar($url_pagina, 'error');
        }
        break;
    case 'salvar':

        $hoje = new DateTime();
        $dataLimite = DateTime::createFromFormat('d/m/Y', $_REQUEST['dejdatalimite_s']);
        
        $diferenca = $hoje->diff($dataLimite);
        $diferencaDias = (integer) $diferenca->format("%R%a");
        
        if ($diferencaDias < 0) {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Por favor, informe uma data futura!');
        }
        
        if ($_REQUEST['itrid_s'] == 9999) {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Selecione a esfera Municipal ou Estadual.');
        }
        
        if ($_REQUEST['itrid_s'] == 1) {
            
            if ($_REQUEST['itrid_s'] == 1 && $_REQUEST['estuf_s'] == 'DF') {
                $_REQUEST['itrid_s'] = 3;
            }
            
            $inuid = $db->pegaUm("select iu.inuid from par3.instrumentounidade iu where iu.itrid = {$_REQUEST['itrid_s']} and iu.estuf = '{$_REQUEST['estuf_s']}'");
        } else {
            
            if ($_REQUEST['itrid_s'] == 2 && $_REQUEST['estuf_s'] == 'DF') {
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Não é possível cadastrar Distrito Federal como municipal.');
            }
            
            $inuid = $db->pegaUm("select iu.inuid from par3.instrumentounidade iu where iu.itrid = {$_REQUEST['itrid_s']} and iu.muncod = '{$_REQUEST['muncod_s']}' and iu.estuf = '{$_REQUEST['estuf_s']}' ");
        }
        
        if (count($_FILES) > 0 && $_FILES['arquivo']["name"] != "") {
            
            if ($_FILES['arquivo']["type"] == "application/pdf") {
                $file = new FilesSimec("desbloqueio_juridico", $campos, 'par3');
                
                $file->setUpload('PAR3 - Desbloqueio Judicial', '', false, 'dejid');
                
                $idArquivo = $file->getIdArquivo();
            } else {
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'O arquivo anexo deve estar no formato PDF.');
            }
        }
        
        $idArquivo = $idArquivo > 0 ? $idArquivo : 'null';
        
        $muncod_s = $_REQUEST['muncod_s'];
        
        if (! empty($_REQUEST['dejid_s'])) {
            $sql = "UPDATE par3.desbloqueio_juridico SET
						inuid = {$inuid},
						dejdatalimite = '" . formata_data_sql($_REQUEST['dejdatalimite_s']) . "',
                        dejcomentario = '" . $_REQUEST['dejcomentario'] . "'";
            
            if ($idArquivo != 'null') {
                $sql .= ",arqid = $idArquivo";
            }
            $sql .= " WHERE dejid = {$_REQUEST['dejid_s']}";
            
            $db->executar($sql);
            $url_pagina = "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=desbloqueioJudicial";
        } else {
            $sqlBuscaoPendenciaAtiva = "
              SELECT
                *
              FROM
                par3.desbloqueio_juridico
              WHERE
                inuid = $inuid
                AND dejstatus = 'A'
                AND dejdatalimite >= CURRENT_DATE
            ";
            
            $pendenciaAtiva = $db->pegaLinha($sqlBuscaoPendenciaAtiva);
            
            if ((int) $pendenciaAtiva > 0) {
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Não é possível cadastrar pois já existe pendência vigente com os dados informados. Considere alterar o registro existente.');
            }
            
            $sql = "INSERT INTO par3.desbloqueio_juridico(inuid, dejdata, usucpf, dejdatalimite, dejcomentario, arqid, dejstatus) VALUES ($inuid, now(), '{$_SESSION['usucpf']}', '" . formata_data_sql($_REQUEST['dejdatalimite_s']) . "', '" . $_REQUEST['dejcomentario'] . "', $idArquivo, 'A')";
            $db->executar($sql);
            $url_pagina = $_SERVER['HTTP_REFERER'];
        }
        
        if ($db->commit()) {
            simec_redirecionar($url_pagina, 'success');
        } else {
            simec_redirecionar($url_pagina, 'error');
        }
        break;
    case 'pesquisarPendenciaEntidade':
        if (array_key_exists('itrid', $_REQUEST) && ! empty($_REQUEST['itrid'])) {
            $itrid = $_REQUEST['itrid'];
            if ($itrid == 9999) {
                $sqlListaPendenciaEntidade .= " AND iu.itrid IN (1,2,3)";
            } else if ($itrid == 1) {
                $sqlListaPendenciaEntidade .= " AND iu.itrid IN (1,3)";
            } else {
                $sqlListaPendenciaEntidade .= " AND iu.itrid = " . $itrid;
            }
        }
        
        if (array_key_exists('estuf', $_REQUEST) && ! empty($_REQUEST['estuf'])) {
            $estuf = $_REQUEST['estuf'];
            $sqlListaPendenciaEntidade .= " AND iu.estuf = '" . $estuf . "'";
        }
        
        if (array_key_exists('muncod', $_REQUEST) && ! empty($_REQUEST['muncod'])) {
            $muncod = $_REQUEST['muncod'];
            $sqlListaPendenciaEntidade .= " AND iu.muncod = '" . $muncod . "'";
        }
        
        if (array_key_exists('dejdatalimite', $_REQUEST) && ! empty($_REQUEST['dejdatalimite'])) {
            $peddatalimite = $_REQUEST['dejdatalimite'];
            $sqlListaPendenciaEntidade .= " AND pp.dejdatalimite = TO_DATE('" . $peddatalimite . "', 'DD/MM/YYYY')";
        }
        break;
}

?>

<div id="debug"></div>
<div class="ibox">
	<div class="ibox-title">
		<div class="row">
			<div class="col-md-12">
				<h2 class="center">Desbloqueio Judicial</h2>
			</div>
		</div>
	</div>
	<div class="ibox-content">
		<div class="row">
			<form name="form-regra-pendecias" id="form-regra-pendecias"
				class="form-horizontal" method="post">
				<input type="hidden" name="requisicao" id="requisicao" value="" /> <input
					type="hidden" name="dejid" id="dejid"
					value="<?php echo $_REQUEST['dejid']; ?>" />
				<div class="row">
					<div class="col-lg-6">
                        <?php echo $simec->radio('itrid', 'Esfera', $itrid ? $itrid : 2, ['9999' => 'Todos', '1' => 'Estadual', '2' => 'Municipal']); ?>
                        <?php echo $simec->select('estuf', 'Estado', $estuf, $arrEstado, ['maxlength' => '255'], []); ?>
                        <?php echo $simec->select('muncod', 'Município', $muncod, $arrMunicipio, ['maxlength' => '255'], []); ?>
                        <?php echo $simec->data('dejdatalimite', 'Data Limite', $peddatalimite ?: $arDados['dejdatalimite'], [], []); ?>
                    </div>
				</div>
			</form>
			<div class="ibox-footer">
				<div class="center">
					<button type="button" id="btn-pesquisar"
						class="btn btn-primary btn-pesquisar">
						<i class="fa fa-search"></i> Pesquisar
					</button>
					<button type="button" class="btn btn-success" id="btn-incluir"
						onclick="inserirDocumentoAnexo()">
						<i class="fa fa-plus"></i> Incluir
					</button>
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
                                
                                $listagemSimec = new Simec_Listagem();
                                $arrayCabecalho = array(
                                    'UF',
                                    'Entidade',
                                    'Data de inclusão',
                                    'Data Limite',
                                    'CPF',
                                    'Nome',
                                    'Situação',
                                    'Status',
                                    'Comentário'
                                );
                                $esconderColunas = array(
                                    '',
                                    'arqid'
                                );
                                $listagemSimec->setCabecalho($arrayCabecalho);
                                
                                $listagemSimec->esconderColunas($esconderColunas);
                                $listagemSimec->setQuery($sqlListaPendenciaEntidade);
                                $listagemSimec->addAcao('edit', 'editarVinculoRegra');
                                $listagemSimec->setAcaoComoCondicional('edit', [
                                    [
                                        'campo' => 'dejstatus',
                                        'valor' => 'Inativo',
                                        'op' => 'diferente'
                                    ]
                                ]);
                                
                                $listagemSimec->addAcao('download', array(
                                    'func' => 'baixarArquivo',
                                    'extra-params' => array(
                                        'arqid'
                                    )
                                ));
                                $listagemSimec->setAcaoComoCondicional('download', [
                                    [
                                        'campo' => 'arqid',
                                        'valor' => '0',
                                        'op' => 'maior'
                                    ]
                                ]);
                                
                                $listagemSimec->addAcao('delete', 'desativaDesbloqueioJudicial');
                                $listagemSimec->setAcaoComoCondicional('delete', [
                                    [
                                        'campo' => 'dejstatus',
                                        'valor' => 'Ativo',
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
<div class="ibox float-e-margins animated modal"
	id="modal_documento_anexo" tabindex="-1" role="dialog"
	aria-hidden="true">
	<div class="modal-dialog">
		<form method="post" name="formsalvar" id="formsalvar"
			class="form form-horizontal" enctype="multipart/form-data">
			<div class="modal-content">
				<div class="ibox-title">
					<button type="button" class="close" data-dismiss="modal"
						aria-label="Close">
						<span aria-hidden="true">&times;</span>
					</button>
					<h3 id="modal_titulo">Documentos Anexos - Incluir</h3>
				</div>
				<div class="ibox-content" id="conteudo-modal">
					<input type="hidden" name="dejid_s" id="dejid_s" value=""> <input
						type="hidden" name="requisicao" id="requisicao" value="salvar"> <input
						type="hidden" name="possuianexo" id="possuianexo" value="nao">

					<div>
                        <?php echo $simec->radio('itrid_s', 'Esfera', $itrid_s ? $itrid_s : 2, ['1' => 'Estadual', '2' => 'Municipal']); ?>
                        <?php echo $simec->select('estuf_s', 'Estado', $estuf_s, $arrEstado, ['maxlength' => '255'], []); ?>
                        <?php echo $simec->select('muncod_s', 'Município', $muncod_s, $arrMunicipio, ['maxlength' => '255'], []); ?>
                        <?php echo $simec->data('dejdatalimite_s', 'Data Limite', $peddatalimite ?: $arDados['dejdatalimite'], [], []); ?>
                    </div>

					<div class="form-group">
						<label
							class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
							Comentário: </label>
						<div class="col-md-8  col-md-8 col-lg-8">
							<textarea name="dejcomentario" id="dejcomentario"
								maxlength="5000" class="form-control" rows="4" cols="60"></textarea>
						</div>
					</div>

					<div class="form-group">
						<label
							class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">
							Arquivo: </label>
						<div class="col-md-8  col-md-8 col-lg-8">
							<input type="file" name="arquivo" id="arquivo"
								class="form-control" style="height: auto !important;" />

							<div id="listaAnexo"></div>

						</div>
					</div>

				</div>
				<div class="ibox-footer">
					<div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
						<button type="submit" id="cancelar" data-dismiss="modal"
							class="btn btn-default" <?php echo $disabled; ?>>
							<i class="fa fa-times-circle-o"></i> Cancelar
						</button>
						<button type="button" id="confirmarSalvar" class="btn btn-success"
							<?php echo $disabled; ?>>
							<i class="fa fa-plus-square-o"></i> Salvar
						</button>
					</div>
				</div>
			</div>
		</form>
	</div>
</div>

<form method="post" name="formexcluirAnexo" id="formexcluirAnexo"
	class="form form-horizontal">
	<input type="hidden" name="dejidexcluir" id="dejidexcluir" value=""> <input
		type="hidden" name="arqid" id="arqid" value=""> <input type="hidden"
		name="requisicao" id="requisicao" value="excluirAnexo">
</form>

<script src="/includes/multifile/jquery.MultiFile.js"></script>

<script>

$('#modal_documento_anexo').on('hidden.bs.modal', function () {
	if($('#dejid_s').val() != ''){
		window.location.href = "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=desbloqueioJudicial";
	}
});

$(function() {
	renderizeMunicipio();
	
    if($('[name="estuf_s"]').val() != '') {
        carregarMunicipio($('[name="estuf_s"]').val(), <?=$muncod_s?$muncod_s:'null'?>, '#muncod_s');
    }

    $("input:radio[name=itrid_s], select[name=estuf_s]").change(function() {
    	renderizeMunicipio();
    });

    $('select[name=estuf_s]').change(function(){
        
        carregarMunicipio(this.value, null, '#muncod_s' );
    });

    $('input[name="itrid_s"]').change(function () {
        if ($(this).val() != 3) {
            // remover distrito federal
        }
    });

    function renderizeMunicipio() {
        
        var filtroMunicipio = $("select[name=muncod_s]").parents("div.form-group");
        
        if ($('input:radio[name=itrid_s]:checked').val() === '1') {
            filtroMunicipio.slideUp();
        }else{
        	filtroMunicipio.slideDown();
        }
    }
});


$(function() {
    renderizeMunicipio();
    
    if($('[name="estuf"]').val() != '') {
        carregarMunicipio($('[name="estuf"]').val(), <?=$muncod?>);
    }

    $("input:radio[name=itrid], select[name=estuf]").change(function() {
        renderizeMunicipio();
    });

    $('select[name=estuf]').change(function(){
        carregarMunicipio(this.value);
    });

    $('input[name="itrid"]').change(function () {
        if ($(this).val() != 3) {
            // remover distrito federal
        }
    });

    function renderizeMunicipio() {
        var filtroMunicipio = $("select[name=muncod]").parents("div.form-group");
        if ($('input:radio[name=itrid]:checked').val() === '1') {
            filtroMunicipio.slideUp();
        }else{
        	filtroMunicipio.slideDown();
        }
    }
});

function inserirDocumentoAnexo() {

    $("#modal_documento_anexo").modal();
}


$("#confirmarSalvar").click(function () {
	$btn = $(this).button('loading');

	if($('[name="itrid_s"]:checked').val() == 1){
		if($('[name="estuf_s"]').val() == ''){
			alert('Selecione um Estado');
			$btn.button('reset');
			return false;
		}
	}
	
	if($('[name="itrid_s"]:checked').val() == 2){
		if($('[name="estuf_s"]').val() == ''){
			alert('Selecione um Estado!');
			$btn.button('reset');
			return false;
		}
		if($('[name="muncod_s"]').val() == ''){
			alert('Selecione um Municipio!');
			$btn.button('reset');
			return false;
		}
	}
	
	if($('[name="dejdatalimite_s"]').val() == ''){
		alert('Selecione uma Data Limite!');
		$btn.button('reset');
		return false;
	}else{

		var str = $('[name="dejdatalimite_s"]').val();
		var partesData = str.split("/");
		var data = new Date(partesData[2], partesData[1] - 1, partesData[0]);

		if(new Date() > data){
			alert('Por favor, informe uma data futura!');
			$btn.button('reset');
			return false;
		}
	}
	
	var extPermitidas = ['pdf'];
	var extArquivo = $('#arquivo').val().split('.').pop();
	
	if(extArquivo != "" && typeof extPermitidas.find(function(ext){ return extArquivo == ext; }) == 'undefined') {
	    alert('O arquivo anexo deve estar no formato PDF.');
	    $btn.button('reset');
		return false;
	}
	
	$('[name="requisicao_s"]').val('salvar');
    $('[name="formsalvar"]').submit();
});

$(".btn-pesquisar").click(function () {
    $btn = $(this).button('loading');
    $('[name="requisicao"]').val('pesquisarPendenciaEntidade');
    $('[name="form-regra-pendecias"]').submit();
});


function desativaDesbloqueioJudicial(dejid){
    var caminho = window.location.href;
    var action = '&requisicao=desativaDesbloqueioJudicial&dejid=' + dejid;
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            swal({
                    title: "",
                    text: "Regras de Pendências desativada com sucesso",
                    type: "success",
                    html: true,
                    confirmButtonText: "OK",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        window.location.href = window.location;
                    }
                    return false;
                }
            );
        }
    });
}


function editarVinculoRegra(dejid){
    
    var caminho = window.location.href;
    
    var action = '&dejid=' + dejid+'&carregarDesbloqueioJudicial=S';
    
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        dataType: "JSON",
        success: function (obj) {

        	$("#dejid_s").val(dejid);

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

        	
        	$("#dejdatalimite_s").val(obj.pepdatalimite_s);
        	$("#dejcomentario").text(obj.comentario);

        	
            $("#itrid_s [value='"+obj.itrid_s+"']").attr("checked", "checked");

            
            if(obj.arqid != ''){
            	$('#listaAnexo').append(
                    	$("<div>").attr('id', 'arquivo_'+obj.arqid).addClass('ibox-title').append
                    	(

                             $("<div>").addClass('MultiFile-label').append
                             (
                            		$('<a>').addClass('MultiFile-remove').attr('href', '#anexo').append
                            		(
										$('<a>').addClass('btn btn-danger btn-xs').attr('aria-label', "Close").attr('title', "Excluir").attr('onclick', "excluirAnexo("+obj.dejid_s +", "+obj.arqid+")").append
                                    	(
                                        	$('<span>').attr('aria-hidden', "true").text('x')
                                        )
                                    ).append
                                    (
                                    	$('<a>').addClass('btn btn-xs').attr('title', "Baixar").attr('href', 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=desbloqueioJudicial&metodo=download&arqid='+obj.arqid).append
                                        (
                                        	$('<i>').addClass('glyphicon glyphicon-cloud-download btn btn-warning btn-xs')
                                        ).append
                                   		(
                                                $('<span>').append
                                                (
                                                	$('<span>').addClass('MultiFile-label').attr('title', 'Selecionado:'+obj.nomeAnexo).append
                                                    (
                                                 		$('<span>').addClass('MultiFile-title').append
                                                        (
                                                        	$('<input>').attr('type', 'hidden').attr('name', 'arquivo-selecionado').attr('id', 'arquivo-selecionado').attr('value', 'anterior')
                                                     	).append
                                                        (
                                                        	" "+obj.nomeAnexo
                                                        )
                                            		)
                                            	)
        									)
                                    )
                             )
                        )     
                );


        		$("#possuianexo").val("sim");
        		
        	}else{
        		$("#possuianexo").val("nao");
           	}
        	
            
            $("#modal_documento_anexo").modal();
            $('#modal_titulo').text('Documentos Anexos - Editar');

        }
    });
}

function excluirAnexo(dejid, arqid) {
    swal({
        title: " Remover Documento Anexo!",
        text: "Tem certeza que deseja remover o Documento Anexo?",
        showCancelButton: true,
        confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
        closeOnConfirm: "on",
        cancelButtonText: "Cancelar",
        html: true
    }, function (isConfirm) {
        if (isConfirm) {
        	
        	$('[name="arqid"]').val(arqid);
        	$('[name="dejidexcluir"]').val(dejid);
        	$('[name="formexcluirAnexo"]').submit();
        }
    });
}

function baixarArquivo(dejid, arqid){
	window.location = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=desbloqueioJudicial&metodo=download&arqid='+arqid;
}

</script>
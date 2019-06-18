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
      pp.pepid, 
      iu.estuf, 
      iu.inudescricao, 
      to_char(pp.pepdata , 'DD/MM/YYYY HH24:MI:SS' ) as data, 
      to_char(pp.peddatalimite , 'DD/MM/YYYY' ) as peddatalimite, 
      formata_cpf_cnpj(pp.usucpf) as cpf, 
      u.usunome, 
      CASE 
        WHEN pp.peddatalimite < now()
          THEN 'Vencido'
        ELSE 'Vigente'
      END AS situacao,
      CASE 
        WHEN pp.pepstatus = 'A'
          THEN 'Ativo'
        ELSE 'Inativo' 
      END AS pepstatus
    FROM 
      par3.pendecia_planejamento pp
      inner join par3.instrumentounidade iu on iu.inuid = pp.inuid
      inner join seguranca.usuario u on u.usucpf = pp.usucpf
    WHERE 
      iu.inustatus = 'A'
";

switch ($_REQUEST['requisicao']){
	case 'salvar':

	    $hoje = new DateTime();
        $dataLimite = DateTime::createFromFormat('d/m/Y', $_REQUEST['peddatalimite']);

        $diferenca = $hoje->diff($dataLimite);
        $diferencaDias = (integer)$diferenca->format("%R%a");

        if ($diferencaDias < 0) {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Por favor, informe uma data futura!');
        }

        if ($_REQUEST['itrid'] == 9999) {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Selecione a esfera Municipal ou Estadual.');
        }

		if ( $_REQUEST['itrid'] == 1)  {

		    if ($_REQUEST['itrid'] == 1 && $_REQUEST['estuf'] == 'DF') {
                $_REQUEST['itrid'] = 3;
            }

		    $inuid = $db->pegaUm("select iu.inuid from par3.instrumentounidade iu where iu.itrid = {$_REQUEST['itrid']} and iu.estuf = '{$_REQUEST['estuf']}'");
        } else {

            if ($_REQUEST['itrid'] == 2 && $_REQUEST['estuf'] == 'DF') {
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Não é possível cadastrar Distrito Federal como municipal.');
            }

		    $inuid = $db->pegaUm("select iu.inuid from par3.instrumentounidade iu where iu.itrid = {$_REQUEST['itrid']} and iu.muncod = '{$_REQUEST['muncod']}' and iu.estuf = '{$_REQUEST['estuf']}' ");
        }

		if( !empty($_REQUEST['pepid']) ){
			$sql = "UPDATE par3.pendecia_planejamento SET 
						inuid = {$inuid}, 
						peddatalimite = '".formata_data_sql($_REQUEST['peddatalimite'])."'
					 WHERE pepid = {$_REQUEST['pepid']}";
			$db->executar($sql);
			$url_pagina = "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=pendeciasEntidade";
		} else {
		    $sqlBuscaoPendenciaAtiva = "
              SELECT
                *
              FROM
                par3.pendecia_planejamento
              WHERE
                inuid = $inuid
                AND pepstatus = 'A'
                AND peddatalimite >= CURRENT_DATE
            ";

			$pendenciaAtiva = $db->pegaLinha($sqlBuscaoPendenciaAtiva);

			if ((int) $pendenciaAtiva > 0) {
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Não é possível cadastrar pois já existe pendência vigente com os dados informados. Considere alterar o registro existente.');
			}

            $sql = "INSERT INTO par3.pendecia_planejamento(inuid, pepdata, usucpf, peddatalimite) VALUES ($inuid, now(), '{$_SESSION['usucpf']}', '".formata_data_sql($_REQUEST['peddatalimite'])."')";
            $db->executar($sql);
			$url_pagina = $_SERVER['HTTP_REFERER'];
		}
		
		if($db->commit()){		
			simec_redirecionar($url_pagina, 'success');
		} else {
			simec_redirecionar($url_pagina, 'error');
		}
	    break;
    case 'pesquisarPendenciaEntidade':
        if (array_key_exists('itrid', $_REQUEST) && !empty($_REQUEST['itrid'])) {
            $itrid = $_REQUEST['itrid'];
            if ($itrid == 9999) {
                $sqlListaPendenciaEntidade .= " AND iu.itrid IN (1,2,3)";
            } else if ($itrid == 1) {
                $sqlListaPendenciaEntidade .= " AND iu.itrid IN (1,3)";
            } else {
                $sqlListaPendenciaEntidade .= " AND iu.itrid = " . $itrid;
            }
        }

        if (array_key_exists('estuf', $_REQUEST) && !empty($_REQUEST['estuf'])) {
            $estuf = $_REQUEST['estuf'];
            $sqlListaPendenciaEntidade .= " AND iu.estuf = '" . $estuf . "'";
        }

        if (array_key_exists('muncod', $_REQUEST) && !empty($_REQUEST['muncod'])) {
            $muncod = $_REQUEST['muncod'];
            $sqlListaPendenciaEntidade .= " AND iu.muncod = '" . $muncod . "'";
        }

        if (array_key_exists('peddatalimite', $_REQUEST) && !empty($_REQUEST['peddatalimite'])) {
            $peddatalimite = $_REQUEST['peddatalimite'];
            $sqlListaPendenciaEntidade .= " AND pp.peddatalimite = TO_DATE('" . $peddatalimite . "', 'DD/MM/YYYY')";
        }
        break;
}

if( $_REQUEST['carregar'] == 'S' ){
	$sql = "SELECT pepid, inuid, pepdata, usucpf, peddatalimite
  			FROM par3.pendecia_planejamento where pepid = {$_REQUEST['pepid']}";
	$arDados = $db->pegaLinha($sql);
	
	$arInstrumento = $db->pegaLinha("select i.estuf, i.muncod, i.itrid from par3.instrumentounidade i where inuid = {$arDados['inuid']}");
	$estuf = $arInstrumento['estuf'];
	$muncod = $arInstrumento['muncod'];
	$itrid = $arInstrumento['itrid'];
}

require APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba( $abacod_tela, $url, '' );
?>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Regras de Pendências</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-regra-pendecias" id="form-regra-pendecias" class="form-horizontal" method="post">
            	<input type="hidden" name="requisicao" id="requisicao" value=""/>
            	<input type="hidden" name="pepid" id="pepid" value="<?php echo $_REQUEST['pepid']; ?>"/>
                <div class="row">
                    <div class="col-lg-6">
                        <?php echo $simec->radio('itrid', 'Esfera', $itrid ? $itrid : 2, ['9999' => 'Todos', '1' => 'Estadual', '2' => 'Municipal']); ?>
                        <?php echo $simec->select('estuf', 'Estado', $estuf, $arrEstado, ['maxlength' => '255'], []); ?>
                        <?php echo $simec->select('muncod', 'Município', $muncod, $arrMunicipio, ['maxlength' => '255'], []); ?>
                        <?php echo $simec->data('peddatalimite', 'Data Limite', $peddatalimite ?: $arDados['peddatalimite'], [], []); ?>
                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                	<button type="button" id="btn-pesquisar" class="btn btn-success btn-pesquisar"><i class="fa fa-search"></i> Pesquisar</button>
                	<button type="button" id="btn-salvar" data-dismiss="modal" class="btn btn-primary btn-salvar" data-loading-text="Salvando, aguarde ..."><i class="fa fa-plus"></i> Salvar</button>
                </div>
            </div>
           <!-- Listagem  -->
            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras" style="display: block;">
                    <div class="table-responsive" >
                        <div>                        	
                            <div id="debug"></div>
                            <div id="testeSQL">
                                <?php

                                    $listagemSimec = new Simec_Listagem();
                                    $arrayCabecalho = array('UF','Descrição', 'Data de inclusão', 'Data Limite', 'CPF', 'Nome', 'Situação', 'Status');
                                    $esconderColunas = array('');
                                    $listagemSimec->setCabecalho($arrayCabecalho);

                                    $listagemSimec->esconderColunas($esconderColunas);
                                    $listagemSimec->setQuery($sqlListaPendenciaEntidade);
                                    $listagemSimec->addAcao('edit', 'editarVinculoRegra' );
                                    $listagemSimec->setAcaoComoCondicional('edit', [['campo' => 'pepstatus', 'valor' => 'Inativo', 'op' => 'diferente']]);
                                    $listagemSimec->addAcao('delete', 'desativaVinculoRegra');
                                    $listagemSimec->setAcaoComoCondicional('delete', [['campo' => 'pepstatus', 'valor' => 'Ativo', 'op' => 'igual']]);

                                    $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                                    $listagemSimec->setTamanhoPagina(100);

                                    $listagemSimec->render();
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

<script>

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
            if ($('input:radio[name=itrid]:checked').val() === '1' || !$('select[name=estuf]').val()) {
                filtroMunicipio.slideUp();
            } else {
                filtroMunicipio.slideDown();
            }
        }
    });
	
	function carregarMunicipio(estuf, muncod) {
		if(estuf != '') {
			var options = $('#muncod');
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
    
    $(".btn-salvar").click(function () {
    	$btn = $(this).button('loading');

    	if($('[name="itrid"]:checked').val() == 1){
    		if($('[name="estuf"]').val() == ''){
    			alert('Selecione um Estado');
    			$btn.button('reset');
    			return false;
    		}
    	}
    	
    	if($('[name="itrid"]:checked').val() == 2){
    		if($('[name="estuf"]').val() == ''){
    			alert('Selecione um Estado!');
    			$btn.button('reset');
    			return false;
    		}
    		if($('[name="muncod"]').val() == ''){
    			alert('Selecione um Municipio!');
    			$btn.button('reset');
    			return false;
    		}
    	}
    	if($('[name="peddatalimite"]').val() == ''){
    		alert('Selecione um Data Limite!');
    		$btn.button('reset');
    		return false;
    	}
    	
    	$('[name="requisicao"]').val('salvar');
        $('[name="form-regra-pendecias"]').submit();
    });

    $(".btn-pesquisar").click(function () {
        $btn = $(this).button('loading');
        $('[name="requisicao"]').val('pesquisarPendenciaEntidade');
        $('[name="form-regra-pendecias"]').submit();
    });

    function desativaVinculoRegra(pepid){
        var caminho = window.location.href;
        var action = '&requisicao=desativaVinculoRegra&pepid=' + pepid;
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

    function ativaVinculoRegra(pepid) {
        var caminho = window.location.href;
        var action = '&requisicao=ativaVinculoRegra&pepid=' + pepid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                swal({
                        title: "",
                        text: "Regras de Pendências ativada com sucesso",
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
    
    function editarVinculoRegra(pepid){
    	var caminho = window.location.href+'&pepid='+pepid+'&carregar=S';
    	window.location.href = caminho;
    }
</script>
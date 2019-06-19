<?php
$obRemanejamento = new Par3_Controller_RemanejamentoPagamento();

switch ($_REQUEST['requisicao']){
    case 'salvarcomposicaoPar':
        ob_clean();
        
        foreach ($_REQUEST['pagid'] as $ppcid => $pagid) {
            
            $ppcvalorpagamento = ($_REQUEST['ppcvalorpagamento'][$ppcid] ? str_replace(array(".",","), array("","."), $_REQUEST['ppcvalorpagamento'][$ppcid]) : 'null');
            
            if( $_REQUEST['tipo'] == 'par' ){
                $sql = "UPDATE par3.pagamentoparcomposicao SET 
                            pagid = $pagid, 
                            epcid = {$_REQUEST['epcid'][$ppcid]},
                            ppcpercentualpag = {$_REQUEST['ppcpercentualpag'][$ppcid]}, 
                            ppcvalorpagamento = {$ppcvalorpagamento}, 
                            ppcstatus = '{$_REQUEST['ppcstatus'][$ppcid]}'
                        WHERE ppcid = {$ppcid}";                
            } else {
                $sql = "UPDATE par3.pagamentoparcomposicao SET
                            pagid = $pagid,
                            eocid = {$_REQUEST['epcid'][$ppcid]},
                            pmcpercentualpag = {$_REQUEST['ppcpercentualpag'][$ppcid]},
                            pmcvalorpagamento = {$ppcvalorpagamento},
                            pmcstatus = '{$_REQUEST['ppcstatus'][$ppcid]}'
                        WHERE pmcid = {$ppcid}";
            }
            $db->executar($sql);
            $db->commit();
        }
        echo '1';
        die;
    break;
    case 'inserircomposicaoPar':
        ob_clean();
        
        $_REQUEST['ppcvalorpagamento'] = ($_REQUEST['ppcvalorpagamento'] ? str_replace(array(".",","), array("","."), $_REQUEST['ppcvalorpagamento']) : 'null');
        
        if( $_REQUEST['tipo'] == 'par' ){
            $sql = "INSERT INTO par3.pagamentoparcomposicao(pagid, epcid, ppcpercentualpag, ppcvalorpagamento, ppcstatus)
                    VALUES({$_REQUEST['pagid']}, {$_REQUEST['epcid']}, {$_REQUEST['ppcpercentualpag']}, {$_REQUEST['ppcvalorpagamento']}, '{$_REQUEST['ppcstatus']}')";
        } else {
            $sql = "INSERT INTO par3.pagamentoobracomposicao(pagid, eocid, pmcpercentualpag, pmcvalorpagamento, pmcstatus)
                    VALUES({$_REQUEST['pagid']}, {$_REQUEST['epcid']}, {$_REQUEST['ppcpercentualpag']}, {$_REQUEST['ppcvalorpagamento']}, '{$_REQUEST['ppcstatus']}')";
        }
        
        $db->executar($sql);
        $db->commit();
        
        echo '1';
        die;
        break;
    case 'excluirComposicaoPagamento':
        ob_clean();
        
        if( $_REQUEST['tipo'] == 'par' ){
            $sql = "UPDATE par3.pagamentoparcomposicao SET ppcstatus = 'I' where ppcid = {$_REQUEST['ppcid']}";
        } else {
            $sql = "UPDATE par3.pagamentoobracomposicao SET pmcstatus='I' WHERE pmcid = {$_REQUEST['ppcid']}";
        }
        $db->executar($sql);
        $db->commit();
        echo '1';
        die;
    break;
    case 'detalharEmpenho':
        ob_clean();
        $empid = $_REQUEST['dados'][0];
        $tipo_processo = $_REQUEST['dados'][1];
        
        $obRemanejamento->detalharEmpenhoComposicao($empid, $tipo_processo);
        die;
    break;
    case 'carregaDadosEmpenho':
        ob_clean();
        if( $_REQUEST['proid'] || $_REQUEST['numeroprocesso'] ){
            echo $obRemanejamento->carregarDadosEmpenho($_REQUEST);
        }
        die;
    break;
    case 'carregaListaItemPagamentoProcesso':
        ob_clean();
        if( $_REQUEST['proid'] || $_REQUEST['numeroprocesso'] ){
            echo $obRemanejamento->carregarDadosParaPagamento( $_REQUEST );
        }
        die;
    break;
    case 'carregarDadosHistoricoPagamento':
        ob_clean();
        
        if( $_REQUEST['proid'] || $_REQUEST['numeroprocesso'] ){
            echo $obRemanejamento->carregarDadosHistoricoPagamento( $_REQUEST );
        }
        die;
    break;
    case 'detalharHistoricoPagamento':
        ob_clean();
        
        $pagid = $_REQUEST['dados'][0];
        $tipo = $_REQUEST['dados'][1];
        $empid = $_REQUEST['dados'][2];
        echo $obRemanejamento->carregarComposicaoPagamento( $pagid, $tipo, $empid );
        die;
    break;
}

/**
    * Arquivos que vão popular os campos
    * Totas as consulta devem trazer apenas dados de iniciativas cadastradas
*/

require APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba( $abacod_tela, $url, '' );

$obPagamento = new Par3_Controller_Pagamentos();
$_REQUEST['origem'] = 'remanejamento'; 
?>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Remanejamento de Pagamento</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-remanejamento" id="form-remanejamento" class="form-horizontal" method="post">
            	<input type="hidden" name="requisicao_form" id="requisicao_form" value=""/>
                <div class="row">
                    <div class="col-lg-12">
                    	<?php echo $simec->input('proid', 'proid', $_REQUEST['proid'], array('type' => 'number'),array('input-size' => '4')); ?>
                    	<?php
                        $attribs = array('maxlength' => '255',
                        				'onkeyup' => "this.value=mascaraglobal('#####.######/####-##',this.value)",
                        				'onblur' => "MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);");
                        
                        echo $simec->input('numeroprocesso', 'Número de Processo', $_REQUEST['numeroprocesso'], $attribs, array('input-size' => '4'));?>
                        <?php
                        $arrOptions = array('obra' => 'Obras', 'par' => 'PAR');
                        echo $simec->radio('tipo', 'Tipo de Processo', $_REQUEST['tipo']?:'par', $arrOptions, array(), array());
                        ?>
                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                	<button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-primary btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button>
                	<button type="reset" class="btn btn-success btn-limpar" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
                </div>
            </div>
           <!-- Listagem  -->
            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras" style="display: block;">
                    <div class="table-responsive" >
<?php           if( $_REQUEST['requisicao_form'] == 'pesquisar' || $_REQUEST['proid'] ){ ?>
					<div class="row">
						<div class="panel panel-success">
                        	<div class="panel-heading">Lista de Empenho(s) do Processo</div>                                    
                            <div class="panel-body" id="lista-composicao">
							</div>
                    	</div>
					</div>
					<div class="row lista-item">
						<div class="panel panel-info">
                            <div class="panel-heading">Lista de Itens de empenho para Pagamento</div>                                    
								<div class="panel-body" id="lista-pagamento">
                            </div>
                            
                            <div class="panel panel-primary">
                             <div class="panel-heading" style="text-align: center;"><h4>Histórico de pagamentos solcitados ao FNDE para o Empenho: <b><?php echo $empnumero;?></b></h4></div>
                                <div class="panel-body" id="lista-historico-pagamento">
        
                                </div>
                             </div>
                            
						</div>
					</div>
					<div class="row lista-item-pagamento">					
					 
                     </div>
<?php           }?>
					</div>
					<div id="debug"></div>
				</div>
                </div>
            </div>
            <!-- END Listagem  -->                        
        </div>
    </div>    
</div>

<script>
    function selecionarEmpenhoPagamento( empid ){
        divCarregando();
        /*var proid = jQuery('[name="proid"]').val();
        var tipo = jQuery('[name="tipo"]').val();
    
        if( jQuery('#chkempenho_'+empid+':checked').length == 0 ){
            window.location.href = window.location.href+"&proid="+proid+"&tipo="+tipo;
        } else {
            window.location.href = window.location.href+"&proid="+proid+"&tipo="+tipo+"&empid="+empid;
        }*/
        carregaListaItemPagamentoProcesso( empid );        
    }

	jQuery(document).ready(function() {
		carregaDadosEmpenho();
    });
    jQuery(".btn-pesquisar").click(function () {
    	$btn = jQuery(this).button('loading');
    	
    	jQuery('[name="requisicao_form"]').val('pesquisar');
        jQuery('[name="form-remanejamento"]').submit();
    });

    function carregaDadosEmpenho(){
    	var form = jQuery('#form-remanejamento').serialize();

        var action  = '&requisicao=carregaDadosEmpenho&'+form;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: false,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            	jQuery('#lista-composicao').html(resp);
            }
        });
        jQuery('.lista-item').hide();
        jQuery('.lista-item-pagamento').hide();
    }
    
    function carregaListaItemPagamentoProcesso( empid ){
    	var form = jQuery('#form-remanejamento').serialize();

        var action  = '&requisicao=carregaListaItemPagamentoProcesso&empid='+empid+'&'+form;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: true,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            	jQuery('#lista-pagamento').html(resp);
        		divCarregado();
            }
        });
        jQuery('.lista-item').show();
        carregarDadosHistoricoPagamento( empid );
    }
    
    function carregarDadosHistoricoPagamento( empid ){
    	var form = jQuery('#form-remanejamento').serialize();

        var action  = '&requisicao=carregarDadosHistoricoPagamento&empid='+empid+'&'+form;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: true,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            	jQuery('#lista-historico-pagamento').html(resp);
        		divCarregado();
            }
        });
        jQuery('.lista-item-pagamento').show();
        jQuery('.navbar-listagem').attr('style', 'z-index: 0');
    }

    function salvarcomposicaoPar( pagid ){
    	var form = jQuery('#form-pagamento-'+pagid).serialize();
    	var formFiltro = jQuery('#form-remanejamento').serialize();
    	var empid = jQuery('[name="empid_pag"]').val();

        var action  = '&requisicao=salvarcomposicaoPar&'+form+'&'+formFiltro;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: true,
            success: function (resp) {
            	jQuery('#debug').html(resp);
            }
        });
        swal("Sucesso!", "Composição do Pagamento Alterada com Sucesso!", "success");
        carregaListaItemPagamentoProcesso(empid);
    }

    function inserirComposicaoPagamento(){
    	var form = jQuery('#form-remanejamento').serialize();
    	var formEmpenho = jQuery('#form-inserir-pagamento').serialize();
    	var empid = jQuery('#form-inserir-pagamento').find('[name="empid_insert"]').val();

        var action  = '&requisicao=inserircomposicaoPar&'+form+'&'+formEmpenho;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: false,
            success: function (resp) {
            	jQuery('#debug').html(resp);
            	jQuery("#modal-inserir-pagamento").modal("hide");
            }
        });
        swal("Sucesso!", "Composição do Pagamento Inserida com Sucesso!", "success");
        carregaListaItemPagamentoProcesso(empid);
    }

    function excluirComposicao( ppcid, empid ){
    	var form = jQuery('#form-remanejamento').serialize();

        var action  = '&requisicao=excluirComposicaoPagamento&ppcid='+ppcid+'&empid='+empid+'&'+form;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: false,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            }
        });
        swal("Sucesso!", "Composição do Pagamento Deletada com Sucesso!", "success");
        carregaListaItemPagamentoProcesso(empid);
    }
    
    function abrirModalPagamento( pagid, tipo, empid ){        
    	jQuery('#form-inserir-pagamento').find('[name="pagid"]').val(pagid);
    	jQuery('#form-inserir-pagamento').find('[name="empid_insert"]').val(empid);
    	jQuery("#modal-inserir-pagamento").modal("show");
    }
    

    jQuery(".btn-limpar").click(function () {
    	window.location.href = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=remanejamentoEmpenho';
    });
</script>

<div class="ibox float-e-margins animated modal" id="modal-inserir-pagamento" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 60%" id="printable">
		<div class="modal-content">
			<div class="ibox-content">
				<div class="row">
					<div>
	        			<h1 class="center" style="font-weight: bold;">Inserir Composição</h1>
					</div>
        			<div class="ibox-content">
        				<form name="form-inserir-pagamento" id="form-inserir-pagamento" class="form-horizontal" method="post">
                			<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="1">
                        	<thead>
                        		<tr>
                        			<th class="">pagid</th>
                        			<th class="">epcid</th>
                        			<th class="">ppcpercentualpag</th>
                        			<th class="">ppcvalorpagamento</th>
                        			<th class="">ppcstatus</th>
                        		</tr>
                        	</thead>
                        	<tbody>
                        	<tr>
                    			<td><input type="hidden" name="empid_insert" value=""> <input name="pagid" id="pagid" type="text" value="" class="form-control" maxlength="255" style="width: 100px;"
                    				onkeyup="this.value=mascaraglobal('[#]',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal('[#]',this.value);"></td>
                    			<td><input name="epcid" id="epcid" type="text" value="" class="form-control" maxlength="255" style="width: 100px;"
                    				onkeyup="this.value=mascaraglobal('[#]',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal('[#]',this.value);"></td>
                    			<td><input name="ppcpercentualpag" id="ppcpercentualpag" type="text" value="100" class="form-control" maxlength="255" style="width: 100px;"
                    				onkeyup="this.value=mascaraglobal('[#]',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal('[#]',this.value);"></td>
                    			<td><input name="ppcvalorpagamento" id="ppcvalorpagamento" type="text" value="" class="form-control" maxlength="255" style="width: 200px;"
                    				onkeyup="this.value=mascaraglobal('[.###],##',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);"></td>
                    			<td><input name="ppcstatus" id="ppcstatus" type="text" value="A" class="form-control" maxlength="255" style="width: 100px;"></td>
                    		</tr>
                        </tbody>
                        </table>
                        </form>        					
        			</div>
				</div>
			</div>
	        <div class="ibox-footer" style="text-align: center;">
				<button type="button" id="btn-salvar-composicao" class="btn btn-success" onclick="inserirComposicaoPagamento();"><i class="fa fa-plus-square-o"></i> Salvar</button>
                <button type="button" id="btn-cancelar-composicao" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>
			</div>
		</div>
	</div>

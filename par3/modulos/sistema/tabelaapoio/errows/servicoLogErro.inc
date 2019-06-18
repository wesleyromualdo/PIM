<?php

/**
    * Arquivos que vão popular os campos
    * Totas as consulta devem trazer apenas dados de iniciativas cadastradas
*/


require APPRAIZ . 'includes/cabecalho.inc';

$sqlServico = "SELECT distinct lgwservico as codigo, lgwservico as descricao FROM par3.logwssigef order by 1";

$db->cria_aba( $abacod_tela, $url, '' );
?>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">LOGS WF SIGEF</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-log-servico" id="form-log-servico" class="form-horizontal" method="post">
            	<input type="hidden" name="requisicao" id="requisicao" value=""/>
                <div class="row">
                    <div class="col-lg-12">
                    	<?php echo $simec->input('dotid', 'id Termo', $_REQUEST['dotid'], array('type' => 'number'),array('input-size' => '4')); ?>
                    	<?php echo $simec->input('proid', 'proid', $_REQUEST['proid'], array('type' => 'number'),array('input-size' => '4')); ?>
                    	<?php
                        $attribs = array('maxlength' => '255',
                        				'onkeyup' => "this.value=mascaraglobal('#####.######/####-##',this.value)",
                        				'onblur' => "MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);");
                        
                        echo $simec->input('numeroprocesso', 'Número de Processo', $_REQUEST['numeroprocesso'], $attribs, array('input-size' => '4'));?>
                        <?php echo $simec->select('lgwservico', 'Serviço', $_REQUEST['lgwservico'], $sqlServico, array('maxlength' => '255'), array('input-size' => '4')); ?>
                        <?php echo $simec->data('lgwdata', 'Data Limite', $_REQUEST['lgwdata'], array(), array( 'input-size' => '4' )); ?>
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
                        <div>
                            <div id="debug"></div>
                            <div id="testeSQL">
                            <?php
                            if( $_REQUEST['requisicao'] == 'pesquisar' ){
                            	
                            	$filtro = '';
                            	if($_REQUEST['lgwdata']) $filtro = " and cast(to_char(lgwdata, 'DD/MM/YYYY') as date) = '".formata_data_sql($_REQUEST['lgwdata'])."' ";
                            	if($_REQUEST['numeroprocesso']){
                            	    $_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
                            	    $_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
                            	    $_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
                            	    $filtro .= " and lw.proid in ( select proid from par3.processo where pronumeroprocesso = '".$_REQUEST['numeroprocesso']."' ) ";
                            	}
                            	
                            	if( $_REQUEST['proid'] ) $filtro .= " and lw.proid = '".$_REQUEST['proid']."' ";
                            	if($_REQUEST['lgwservico']) $filtro .= " and lw.lgwservico = '".$_REQUEST['lgwservico']."' ";
                            	
	                            $sql = "SELECT lw.lgwid, lw.proid, lw.empid, lw.pagid, lw.lgwservico, /*lw.lgwurl, lw.lgwrequest, lw.lgwresponse,*/
										     u.usunome as nome, lw.lgwmsgretorno, to_char(lw.lgwdata, 'DD/MM/YYYY HH24:MI:SS' ) as lgwdata
										FROM par3.logwssigef lw
											inner join seguranca.usuario u on u.usucpf = lw.usucpf
            							WHERE 1=1 $filtro order by lw.lgwid desc";
	                            
	                            $listagemSimec = new Simec_Listagem();
	                            $arrayCabecalho = array('proid','empid', 'pagid', 'serviço', 'nome', 'msg retorno', 'data');
	                            $esconderColunas = array('');
	                            $listagemSimec->setCabecalho($arrayCabecalho);
	                            
	                            $listagemSimec->esconderColunas($esconderColunas);
	                            $listagemSimec->turnOnPesquisator();
	                            $listagemSimec->setQuery($sql);
	                            $listagemSimec->addAcao('view', 'visualizarLog' );
	                            //$listagemSimec->addAcao('delete', 'deleteVinculoRegra' );
	                            $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
	                            $listagemSimec->setTamanhoPagina(10);
	                            $listagemSimec->setCampos($arrayCabecalho);
	                            
	                            $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
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

<script>
    jQuery(".btn-pesquisar").click(function () {
    	$btn = jQuery(this).button('loading');
    	
    	jQuery('[name="requisicao"]').val('pesquisar');
        jQuery('[name="form-log-servico"]').submit();
    });

    function visualizarLog( lgwid ){

    	var caminho = window.location.href;
        var action = '&requisicao=visualizarLog&lgwid=' + lgwid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
				jQuery('#retorno_xml').html(resp);
            }
        });
        $("#modal-login-processo").modal("show");
    }

    jQuery(".btn-limpar").click(function () {
    	window.location.href = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=servicoLogErro';
    });
</script>

<div class="ibox float-e-margins animated modal" id="modal-login-processo" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%" id="printable">
		<div class="modal-content">
			<div class="ibox-content">
				<div class="row">
					<div>
	        			<h1 class="center" style="font-weight: bold;">XML</h1>
					</div>
        			<div class="ibox-content">
        				<div id="retorno_xml"></div>
        			</div>
				</div>
			</div>
	        <div class="ibox-footer" style="text-align: center;">
	            	<button type="button" id="btn-cancelar-vinculo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>
			</div>
		</div>
	</div>
</div>
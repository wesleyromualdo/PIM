<?php
$obRemanejamento = new Par3_Controller_RemanejamentoEmpenho();

switch ($_REQUEST['requisicao']){
    case 'salvarcomposicaoPar':
        ob_clean();
        foreach ($_REQUEST['empid'] as $epcid => $empid) {
            
            $epcvalorempenho = ($_REQUEST['epcvalorempenho'][$epcid] ? str_replace(array(".",","), array("","."), $_REQUEST['epcvalorempenho'][$epcid]) : 'null');
            
            if( $_REQUEST['tipo_processo'] == 'P' ){
                $sql = "UPDATE par3.empenhoparcomposicao SET
                            empid = $empid,
                            ppcid = {$_REQUEST['ppcid'][$epcid]},
                            epcpercentualemp = {$_REQUEST['epcpercentualemp'][$epcid]},
                            epcvalorempenho = {$epcvalorempenho},
                            epcstatus = '{$_REQUEST['epcstatus'][$epcid]}'
                        WHERE epcid = $epcid";
            } else {
                $sql = "UPDATE par3.empenhoobracomposicao SET
                            empid = $empid,
                            pocid = {$_REQUEST['ppcid'][$epcid]},
                            eocpercentualemp = {$_REQUEST['epcpercentualemp'][$epcid]},
                            eocvalorempenho = {$epcvalorempenho},
                            eocstatus = '{$_REQUEST['epcstatus'][$epcid]}'
                        WHERE eocid = $epcid";
            }
            $db->executar($sql);
            $db->commit();
        }
        echo '1';
        die;
    break;
    case 'inserircomposicaoPar':
        ob_clean();
        $proid = $db->pegaUm("SELECT p.proid FROM par3.processo p
                    	INNER JOIN par3.empenho e ON e.empnumeroprocesso = p.pronumeroprocesso
                    WHERE e.empid = {$_REQUEST['empid']}");
        
        $_REQUEST['epcvalorempenho'] = ($_REQUEST['epcvalorempenho'] ? str_replace(array(".",","), array("","."), $_REQUEST['epcvalorempenho']) : 'null');
        
        if( $_REQUEST['tipo_processo'] == 'P' ){
            $sql = "INSERT INTO par3.empenhoparcomposicao(empid, ppcid, epcpercentualemp, epcvalorempenho, epcstatus)
                    VALUES({$_REQUEST['empid']}, {$_REQUEST['ppcid']}, {$_REQUEST['epcpercentualemp']}, {$_REQUEST['epcvalorempenho']}, '{$_REQUEST['epcstatus']}')";
        } else {
            $sql = "INSERT INTO par3.empenhoobracomposicao(empid, pocid, eocpercentualemp, eocvalorempenho, eocstatus)
                    VALUES({$_REQUEST['empid']}, {$_REQUEST['ppcid']}, {$_REQUEST['epcpercentualemp']}, {$_REQUEST['epcvalorempenho']}, '{$_REQUEST['epcstatus']}')";
        }
        
        $db->executar($sql);
        $db->commit();
        
        echo '1';
        die;
        break;
    case 'excluirComposicaoEmpenho':
        ob_clean();
        
        if( $_REQUEST['tipo_processo'] == 'P' ){
            $sql = "update par3.empenhoparcomposicao set epcstatus = 'I' where epcid = {$_REQUEST['epcid']}";
        } else {
            $sql = "update par3.empenhoobracomposicao set eocstatus = 'I' where eocid = {$_REQUEST['epcid']}";
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
    case 'carregaComposicaoProcesso':
        ob_clean();
        if( $_REQUEST['proid'] || $_REQUEST['numeroprocesso'] ){
            echo $obRemanejamento->listaComposicaoProcesso( $_REQUEST );
        }
        die;
    break;
    case 'carregaListaEmpenhoProcesso':
        ob_clean();
        if( $_REQUEST['proid'] || $_REQUEST['numeroprocesso'] ){
            echo $obRemanejamento->listaEmpenhoProcesso( $_REQUEST );
        }
        die;
    break;
    case 'listarItemDisponivel':
        ob_clean();
        if( $_REQUEST['proid'] || $_REQUEST['numeroprocesso'] ){
            echo $obRemanejamento->listaComposicaoItemDisponivel( $_REQUEST );
        }
        die;
    break;
}

/**
    * Arquivos que vão popular os campos
    * Totas as consulta devem trazer apenas dados de iniciativas cadastradas
*/

require APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba( $abacod_tela, $url, '' );
?>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Remanejamento de Empenho</h2>
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
                        $arrOptions = array('O' => 'Obras', 'P' => 'PAR');
                        echo $simec->radio('tipo_processo', 'Tipo de Processo', $_REQUEST['tipo_processo']?:'P', $arrOptions, array(), array());
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
                        	<div class="panel-heading">Composição do Processo</div>                                    
                            <div class="panel-body" id="lista-composicao">
                            	<?php //$obRemanejamento->listaComposicaoProcesso( $_REQUEST );?>
							</div>
                    	</div>
					</div>
					<div class="row">
						<div class="panel panel-info">
                            <div class="panel-heading">Lista de Empenho(s) do Processo</div>                                    
								<div class="panel-body" id="lista-empenho">
								<?php //$obRemanejamento->listaEmpenhoProcesso( $_REQUEST ); ?>
                                </div>
							</div>
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
	jQuery(document).ready(function() {
		carregaComposicaoProcesso();
    });
    jQuery(".btn-pesquisar").click(function () {
    	$btn = jQuery(this).button('loading');
    	
    	jQuery('[name="requisicao_form"]').val('pesquisar');
        jQuery('[name="form-remanejamento"]').submit();
    });

    function excluirComposicao( epcid, empid ){
    	var form = jQuery('#form-remanejamento').serialize();

        var action  = '&requisicao=excluirComposicaoEmpenho&epcid='+epcid+'&'+form;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: false,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            }
        });
        swal("Sucesso!", "Composição do Empenho Deletada com Sucesso!", "success");
        carregaComposicaoProcesso();
    }

    function carregaComposicaoProcesso(){
    	var form = jQuery('#form-remanejamento').serialize();

        var action  = '&requisicao=carregaComposicaoProcesso&'+form;
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

        carregaListaEmpenhoProcesso();
    }
    
    function carregaListaEmpenhoProcesso(){
    	var form = jQuery('#form-remanejamento').serialize();

        var action  = '&requisicao=carregaListaEmpenhoProcesso&'+form;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: false,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            	jQuery('#lista-empenho').html(resp);
            }
        });
    }
    
    function salvarcomposicaoPar( empid ){
    	var form = jQuery('#form-empenho-'+empid).serialize();

        var action  = '&requisicao=salvarcomposicaoPar&'+form;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: false,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            }
        });
        swal("Sucesso!", "Composição do Empenho Alterada com Sucesso!", "success");
        carregaComposicaoProcesso();
    }

    function inserirComposicaoEmpenho( empid ){
    	var form = jQuery('#form-remanejamento').serialize();
    	var formEmpenho = jQuery('#form-inserir-empenho').serialize();

        var action  = '&requisicao=inserircomposicaoPar&'+form+'&'+formEmpenho;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: false,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            	jQuery("#modal-inserir-empenho").modal("hide");
            }
        });
        swal("Sucesso!", "Composição do Empenho Inserida com Sucesso!", "success");
        carregaComposicaoProcesso();
    }
    
    function abrirModalEmpenho( empid, tipo_processo ){        
    	jQuery('#form-inserir-empenho').find('[name="empid"]').val(empid);
    	jQuery("#modal-inserir-empenho").modal("show");

    	carregaListaItemDisponivel();
    }

    function carregaListaItemDisponivel(){
    	var form = jQuery('#form-remanejamento').serialize();
    	//var formEmpenho = jQuery('#form-inserir-empenho').serialize();

        var action  = '&requisicao=listarItemDisponivel&'+form;
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: action,
            async: false,
            success: function (resp) {
            	//jQuery('#debug').html(resp);
            	jQuery("#lista-item-disponivel").html(resp);
            }
        });
    }

    jQuery(".btn-limpar").click(function () {
    	window.location.href = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=remanejamentoEmpenho';
    });

    function calculaTotalEmpenho(epcid, empid){

		var total = 0;
    	jQuery('.class_epcid').each(function(){
            var epcid = jQuery(this).val();
            var epcvalorempenho = retiraPontos(jQuery('[name="epcvalorempenho['+epcid+']"]').val());
            total = parseFloat(total) + parseFloat(epcvalorempenho);
        });

        jQuery('#td_total_empenho').html( number_format(total, 2, ",", ".") );
    }

    function retiraPontos(v){
        if( v != 0 ){
            var valor = v.replace(/\./gi,"");
            valor = valor.replace(/\,/gi,".");
        } else {
            var valor = v;
        }
        
        return valor;
    }
</script>

<div class="ibox float-e-margins animated modal" id="modal-inserir-empenho" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 60%" id="printable">
		<div class="modal-content">
			<div class="ibox-content">
				<div class="row">
					<div>
	        			<h1 class="center" style="font-weight: bold;">Inserir Composição</h1>
					</div>
        			<div class="ibox-content">
        				<div class="row">
        					<div id="lista-item-disponivel"></div>
        				</div>
        			</div>
        			<div class="ibox-content">
        			<div class="row">
        				<form name="form-inserir-empenho" id="form-inserir-empenho" class="form-horizontal" method="post">
                			<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="1">
                        	<thead>
                        		<tr>
                        			<th class="">empid</th>
                        			<th class="">ppcid</th>
                        			<th class="">epcpercentualemp</th>
                        			<th class="">epcvalorempenho</th>
                        			<th class="">epcstatus</th>
                        		</tr>
                        	</thead>
                        	<tbody>
                        	<tr>
                    			<td><input name="empid" id="empid" type="text" value="" class="form-control" maxlength="255" style="width: 100px;"
                    				onkeyup="this.value=mascaraglobal('[#]',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal('[#]',this.value);"></td>
                    			<td><input name="ppcid" id="ppcid" type="text" value="" class="form-control" maxlength="255" style="width: 100px;"
                    				onkeyup="this.value=mascaraglobal('[#]',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal('[#]',this.value);"></td>
                    			<td><input name="epcpercentualemp" id="epcpercentualemp" type="text" value="0" class="form-control" maxlength="255" style="width: 100px;"
                    				onkeyup="this.value=mascaraglobal('[#]',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal('[#]',this.value);"></td>
                    			<td><input name="epcvalorempenho" id="epcvalorempenho" type="text" value="" class="form-control" maxlength="255" style="width: 150px;"
                    				onkeyup="this.value=mascaraglobal('[.###],##',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);"></td>
                    			<td><input name="epcstatus" id="epcstatus" type="text" value="A" class="form-control" maxlength="255" style="width: 100px;"></td>
                    		</tr>
                        </tbody>
                        </table>
                        </form>        					
        			</div>
        			</div>
				</div>
			</div>
	        <div class="ibox-footer" style="text-align: center;">
				<button type="button" id="btn-salvar-composicao" class="btn btn-success" onclick="inserirComposicaoEmpenho();"><i class="fa fa-plus-square-o"></i> Salvar</button>
                <button type="button" id="btn-cancelar-composicao" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>
			</div>
		</div>
	</div>

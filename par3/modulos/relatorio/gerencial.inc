<?
/**
 * @author   Allan Carlos Ramalho <allan.c.ramalho@gmail.com>
 * @license  GNU simec.mec.gov.br
 */

# Classes
$estado = new Territorios_Model_Estado();
$controllerIniciativa = new Par3_Controller_Iniciativa();
if( $_REQUEST['requisicao'] === 'pesquisar' || $_REQUEST['requisicao'] === 'exportar' ) {
    $sqlPrincipal = $controllerIniciativa->getSqlRelatorioGerencial();
    $sqlTotal =  $controllerIniciativa->getSqlTotalizadorRelatorioGerencial( $_REQUEST, $sqlPrincipal );
}

switch ($_REQUEST['requisicao']) {
    case 'exportar':
        ini_set('memory_limit', '-1');
        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
        $list->setCabecalho(array('auto'));
        $list->setQuery( $sqlPrincipal );
        $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        die;
    case 'carregaMunicipios':
        $municipio = new Territorios_Model_Municipio();
        print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
        die;
        break;
}

require APPRAIZ . 'includes/cabecalho.inc';

# SQL para campos carregados
$sqlSituacoesI = "SELECT esdid AS codigo, esdordem||' - '||esddsc AS descricao FROM workflow.estadodocumento WHERE tpdid=".PAR3_WORKFLOW_FLUXO_INICIATIVAS." AND esdstatus='A' ORDER BY esdordem";
$sqlSituacoesO = "SELECT esdid AS codigo, esdordem||' - '||esddsc AS descricao FROM workflow.estadodocumento WHERE tpdid=".PAR3_WORKFLOW_FLUXO_OBRAS." AND esdstatus='A' ORDER BY esdordem";
$sqlTiposObjeto = "SELECT intoid AS codigo, intoid||' - '||intodsc AS descricao FROM par3.iniciativa_tipos_objeto WHERE intostatus='A' AND intosituacao='A' ORDER BY 1";
$sqlDimensoes = "SELECT DISTINCT dimcod AS codigo, dimcod||' - '||dimdsc AS descricao FROM par3.dimensao WHERE dimstatus='A' ORDER BY dimcod";
$sqlCategoriaO = "SELECT octid AS codigo, octid||' - '||octdsc AS descricao FROM par3.obra_categoria WHERE octstatus = 'A' ORDER BY 1";
$sqlLocalO = array('13, 14, 25' => 'Cobertura', '1, 2, 19, 20, 21, 23' => 'Creche', '4, 5, 6, 7, 8, 9, 10, 11, 15, 16, 17, 18, 22' => 'Escola', '12, 24' => 'Quadra');
$sqlAno = 'SELECT DISTINCT obrano AS codigo, obrano AS descricao FROM par3.obra UNION SELECT DISTINCT ipiano AS codigo, ipiano AS descricao FROM par3.iniciativa_planejamento_item_composicao ORDER BY codigo DESC';
$sqlDocumentoTermo = "SELECT dotid AS codigo, dotnumero AS descricao FROM par3.documentotermo WHERE dotstatus = 'A' ORDER BY codigo DESC";
$sqlProcesso = "SELECT proid AS codigo, formata_num_processo( pronumeroprocesso ) AS descricao FROM par3.processo";
$sqlEmpenho = "SELECT proid AS codigo, formata_num_processo( pronumeroprocesso ) AS descricao FROM par3.processo";
$sqlPagamentoSituacao = "SELECT * FROM (VALUES (1, '100% Pago'), (2, 'Pagamento parcial'), (3, 'Sem pagamento')) AS t (codigo,descricao);";
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$obPagamento = new Par3_Controller_Pagamentos();
?>
<link rel="stylesheet" type="text/css" href="css/relatorio.gerencial.css" />
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-10">
               <h3 id="entidade">Relatório Gerencial</h3>
            </div>
        </div>
    </div>
    <form method="post" name="formulario" id="formulario" class="form form-horizontal">
    	<input type="hidden" name="requisicao" value="">
    <div class="ibox-content">
        <div class="row">
        	<div class="col-lg-6">
                <? echo $simec->radio('itrid', 'Esfera', $_POST['itrid'] ? $_POST['itrid'] : 2, array('-1' => 'Todos', '1' => 'Estadual', '2' => 'Municipal')); ?>
                <div class="row">
                    <div class="col-lg-5" style="margin-left:55.5px;">
                        <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $estado->pegarSQLSelect( $_REQUEST ), array('maxlength' => '255'),array('input-size' => '6', 'label-size' => '5')); ?>
                    </div>
                    <div id="div-muncod" style="display:<?= $_REQUEST['itrid'] == 1 ?'none':''?>;margin-left:-30px;" class="col-lg-6">
                        <?php echo $simec->select('muncod', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255'), array('input-size' => '8','label-size' => '3')); ?>
                    </div>
                </div>
                <? echo $simec->select('ano', 'Ano', $_POST['ano'], $sqlAno, null, array('input-size' => '4')); ?>
                <? echo $simec->radio('processogerado', 'Processo gerado', $_POST['processogerado'] ? $_POST['processogerado'] : -1, array('1' => 'Sim', '2' => 'Não', '-1' => 'Todos')); ?>
                <? echo $simec->radio('empenhoefetivado', 'Empenho efetivado', $_POST['empenhoefetivado'] ? $_POST['empenhoefetivado'] : -1, array('1' => 'Sim', '2' => 'Não', '-1' => 'Todos')); ?>
                <? echo $simec->radio('termogerado', 'Termo gerado', $_POST['termogerado'] ? $_POST['termogerado'] : -1, array('1' => 'Sim', '2' => 'Não', '-1' => 'Todos')); ?>
                <?php echo $simec->radio('tipo_processo', 'Tipo de Processo', ($_REQUEST['tipo_processo'] ? $_REQUEST['tipo_processo'] : 'P'), array('O' => 'Obras', 'P' => 'PAR'), array(), array()); ?>
            </div>
            <div class="col-lg-6">
            	<? echo $simec->select('dimcod', 'Dimensão', $_POST['dimcod'], $sqlDimensoes, null, array('input-size' => '7')); ?>
            	<? echo $simec->select('intoid', 'Tipo de objeto', $_POST['intoid'], $sqlTiposObjeto, array('multiple'), array('input-size' => '7')); ?>
                <? echo $simec->select('octid', 'Categoria de obra', $_POST['octid'], $sqlCategoriaO, array('multiple', 'class' => 'soObra'), array('input-size' => '7', 'groupStyle' => 'display: none')); ?>
                <? echo $simec->select('localO', 'Local de obra', $_POST['localO'], $sqlLocalO, array('multiple', 'class' => 'soObra'), array('input-size' => '7', 'groupStyle' => 'display: none')); ?>
                <? echo $simec->select('dotid', 'Termo', $_POST['dotid'], $sqlDocumentoTermo, array('multiple'), array('input-size' => '7')); ?>
                <? echo $simec->input('pronumeroprocesso', 'Processo', $_POST['pronumeroprocesso'],  array('class' => 'inteiro', 'placeholder' => 'Informe o nrº do processo'), array('input-size' => 4) ); ?>
                <?php echo $simec->select('pagamentosituacao', 'Situação do pagamento', $_REQUEST['pagamentosituacao'], $obPagamento->getPagamentoSituacoes(), array('maxlength' => '255'), array('input-size' => '4','label-size' => '3')); ?>
                <? echo $simec->select('esdidI', 'Situação de iniciativa', $_POST['esdidI'], $sqlSituacoesI, array('multiple', 'class' => 'soObraN'), array('input-size' => '7', 'groupStyle' => 'display: none')); ?>
                <? echo $simec->select('esdidO', 'Situação de obra', $_POST['esdidO'], $sqlSituacoesO, array('multiple', 'class' => 'soObra'), array('input-size' => '7', 'groupStyle' => 'display: none')); ?>
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
    </div>
    <div class="ibox-footer">
        <div class="row" style="text-align: center;">
                <button type="button" class="btn btn-success pesquisar" ><i class="fa fa-search"></i> Pesquisar</button>
                <button type="button" class="btn btn-primary export" ><i class="fa fa-list"></i> Exportar XLS</button>
        </div>
    </div>
    </form>
    <div class="ibox-content">
<?php
# Listagem principal
    if($_REQUEST['requisicao'] === 'pesquisar') {
        if( $_REQUEST['listagem']['p'] == 1 || empty($_REQUEST['listagem']['p']) ):
        
        echo '<div class="row">
                    <div class="col-lg-12">';
        // Totalizadores
        $list = new Simec_Listagem();
        $list->setCabecalho('auto');
        $list->setQuery($sqlTotal);
        $list->addCallbackDeCampo(array('Total do Valor Planejado', 'Total do Valor Empenhado', 'Total Aprovado', 'Total Pago'), 'formata_numero_monetario');
        $list->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        endif;
        echo '</div></div>';
        echo '<div class="row">
                    <div class="col-lg-12">';
        // Resultado da busca.
        // Foi solicitada uma versão de lista corrida do relatório, sem paginação. Sendo assim o limite padrão de memória não é suficiente. Logo foi descartado na configuração que segue.
        //ini_set('memory_limit', '-1');
        //$list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $list = new Simec_Listagem();
        $list->setCabecalho('auto');
        $list->setQuery($sqlPrincipal);
        $list->turnOnPesquisator();
        //$list->addCallbackDeCampo('Processo', 'formata_numero_processo');
        $list->addCallbackDeCampo(array('Valor Planejado',  'Valor Aprovado', 'Valor Empenhado', 'Valor Pago'), 'formata_numero_monetario');
        $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $list->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        echo '</div></div>';
    }
?>
	</div>
</div>
<!-- Scripts -->
<script>
    jQuery(function() {
        //tela de loading
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
        renderizeSomenteObras();

        if(jQuery('[name="estuf"]').val() != '') {
            carregarMunicipio(jQuery('[name="estuf"]').val(), <?=$_REQUEST['muncod']?>);
        }

        jQuery("input:radio[name=itrid], select[name=estuf]").change(function() {
            renderizeMunicipio();
        });
        
        jQuery("input:radio[name=tipo_processo]").change(function() {
        	renderizeSomenteObras();
        });

        jQuery('select[name=estuf]').change(function(){
            carregarMunicipio(this.value);
        });

    	function renderizeMunicipio() {
    	    var filtroMunicipio = jQuery("select[name=muncod]").parents("div.form-group");
    	    if (jQuery('input:radio[name=itrid]:checked').val() === '1' /*|| !jQuery('select[name=estuf]').val()*/ ) {
    	        filtroMunicipio.slideUp();
    	    } else {
    	        filtroMunicipio.slideDown();
    	    }
    	}
    });

    jQuery('.pesquisar').click(function(){
        jQuery('[name="requisicao"]').val('pesquisar');
        jQuery('[name="formulario"]').submit();
    });
    
    jQuery('.export').click(function(){
        jQuery('[name="requisicao"]').val('exportar');
        jQuery('[name="formulario"]').submit();
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

    function renderizeSomenteObras() {
        var tipos = jQuery('[name="tipo_processo"]:checked').val();
        
        var flag = tipos && tipos.length == 1 && tipos[0] === 'O';
        if (flag) {
        	jQuery('[name="intoid[]"]').val('1').attr('disabled', true).trigger('chosen:updated');
            jQuery(".soObra").parents("div.form-group").slideDown();
            jQuery(".soObraN").parents("div.form-group").slideUp();
            jQuery('[name="esdidI[]"]').val('').trigger('chosen:updated');
        } else {
        	jQuery('[name="intoid[]"]').val('').attr('disabled', false).trigger('chosen:updated');
        	jQuery('[name="octid[]"]').val('').trigger('chosen:updated');
        	jQuery('[name="localO[]"]').val('').trigger('chosen:updated');
        	jQuery('[name="esdidO[]"]').val('').trigger('chosen:updated');
            jQuery(".soObra").parents("div.form-group").slideUp();
            jQuery(".soObraN").parents("div.form-group").slideDown();
        }
    }
</script>
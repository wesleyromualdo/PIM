<?php
include_once APPRAIZ . "includes/classes/Fnde_WebService_Servico.class.inc";

$obPagamento = new Par3_Controller_Pagamentos();

switch ($_REQUEST['requisicao']) {
    case 'excluir_pagamento':
        echo $obPagamento->excluirPagamento($_REQUEST);
        die;
    break;
    case 'carregaDadosObra':
        echo carregaDadosObraLista($_REQUEST['proid']);
        die;
        break;
    case 'cancelar_pagamento':
        $arrRegistro = array();
        parse_str($_REQUEST['form-autentica-post'], $arrRegistro);
        $_REQUEST['wsusuario'] = $arrRegistro['ws_usuario_aut'];
        $_REQUEST['wssenha'] = $arrRegistro['ws_senha_aut'];
        $obPagamento->cancelarPagamento($_REQUEST);
        die;
    break;
    case 'solicitar':
        $arrRegistro = array();
        parse_str($_REQUEST['form-autentica-post'], $arrRegistro);
        $_REQUEST['wsusuario'] = $arrRegistro['ws_usuario_aut'];
        $_REQUEST['wssenha'] = $arrRegistro['ws_senha_aut'];
        $obPagamento->validaSolicitaPagamento($_REQUEST);
        die;
    break;
    case 'reenviar_pagamento':
        $arrRegistro = array();
        parse_str($_REQUEST['form-autentica-post'], $arrRegistro);
        $_REQUEST['wsusuario'] = $arrRegistro['ws_usuario_aut'];
        $_REQUEST['wssenha'] = $arrRegistro['ws_senha_aut'];
        $obPagamento->solicitarPagamento($_REQUEST);
        die;
    break;
    case 'consultar_pagamento':
        $arrRegistro = array();
        parse_str($_REQUEST['form-autentica-post'], $arrRegistro);
        $_REQUEST['wsusuario'] = $arrRegistro['ws_usuario_aut'];
        $_REQUEST['wssenha'] = $arrRegistro['ws_senha_aut'];
        $obPagamento->consultarPagamento($_REQUEST);
        die;
    break;
    case 'visualiza_xml':
        $obPagamento->solicitarPagamento($_REQUEST);
        die;
    break;
    case 'pegaValorJaPago':
        echo $obPagamento->pegaValorJaPago($_REQUEST['proid'], $_REQUEST['tipo']);
        die;
    break;
    case 'visualiza_historico':
        $obPagamento->historicoPagamento($_REQUEST);
        if ($_REQUEST['tipo'] == 'par') {
            $obPagamento->iniciativasVinculadasPagamento($_REQUEST);
        }
        if ($_REQUEST['tipo'] == 'obra') {
            $obPagamento->obrasVinculadasPagamento($_REQUEST);
        }
        die;
    case 'historicoPagamento':
        ob_clean();
        $empid = $_POST['dados'][0];
        if(!$empid){
            echo '';die;
        }
        $obPagamento->carregarDadosHistoricoPagamento($empid);
        die;
        break;
    break;
}

require APPRAIZ . 'includes/cabecalho.inc';

$url = 'par3.php?modulo=principal/orcamento/solicitarPagamento&acao=A';
echo $simec->tab(criaAbaPagamento(), $url);
//echo $simec->title('Solicitar Pagamentos FNDE', '' );

$obProcesso = new Par3_Controller_Processo();
$arrEntidade = $obProcesso->carregaDadosEntidadeProcesso($_REQUEST['proid']);

//if ($_SERVER['HTTP_HOST'] != "simec-local" && $_SERVER['HTTP_HOST'] != "localhost") {
    $_REQUEST['inuid'] = $arrEntidade['inuid'];
    /*$helperPendencia = new Par3_Helper_PendenciasEntidade();
    $pendenciasImpeditivas = $helperPendencia->controlePendenciasBoxImpeditivas();*/
    
    $pendenciasImpeditivas = 0;
    
    $temPendencia = true;
if ($_REQUEST['inuid']) {
    /*$obPendencia = new Par3_Controller_Pendencia();
    $boHabilitaPorEntidade = $obPendencia->pegaPendenciaInstrumentoUnidade($_REQUEST['inuid']);*/
    //$boHabilitaPorIniciativa = $obPendencia->pegaValidaPendenciaPorInicitiva( $inp->iniid, $_REQUEST['inuid'] );
    
    $obPendenciaBloqueio = new Par3_Controller_Pendencia();
    $temPendencia = $obPendenciaBloqueio->verificaPendenciaBloqueio('pagamento', $_REQUEST['inuid'], '', $_REQUEST['proid'], true);
}
if ($temPendencia == true) {
    $pendenciasImpeditivas = 1;
}

$_REQUEST['pendencias'] = $pendenciasImpeditivas;
//}
?>
<style>
.alert, .thumbnail {
    margin-bottom: 10px;
}

.tabs-container .panel-body {
    padding: 5px;
}

.panel-heading {
    padding: 5px 5px;
}

.ibox .open > .dropdown-menu {
    left: auto;
    right: -200px;
}
</style>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Solicitar Pagamentos FNDE</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-dados-empenho" id="form-dados-empenho" class="form-horizontal" method="post">
                <input type="hidden" name="proid" id="proid" value="<?php echo $_REQUEST['proid'];?>"/>
                <input type="hidden" name="empid" id="empid" value="<?php echo $_REQUEST['empid'];?>"/>
                <input type="hidden" name="pendencias" id="pendencias" value="<?php echo $pendenciasImpeditivas;?>"/>
                <input type="hidden" name="tipo_processo" id="tipo_processo" value="<?php echo $_REQUEST['tipo'];?>"/>
            </form>
                <div class="row">
                    <div class="col-lg-12">
                        <?php
                        $arrErrosWarning = array();
                        
                        $arAcompan = $obPagamento->verificaAcompanhamentoEntidade($_REQUEST['proid']);
                        
                        if (( ($arAcompan['totalpago'] >= $arAcompan['valortotalcontrato'])) && !empty($arAcompan) && $arAcompan['totalpago'] > (int)0) {
                            $link = '<a href="par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=acompanhamento&inuid='.$arrEntidade['inuid'].'&dotid='.$arrEntidade['dotid'].'"><b>Clique aqui!</b></a>';
                            
                            $arrErrosWarning[] = '- O valor pago está maior ou igual que o valor dos contratos informados pela entidade. '.$link;
                        } else {
                            if ((int)$arAcompan['qtcontrato'] < (int)1) {
                                $arrErrosWarning[] = '- Não existe contrato anexado para este processo.';
                            }
                        }
                        $_REQUEST['pendencia_entidade'] = '';
                        $arrErros = $obPagamento->verificaPendenciaPagamento($_REQUEST['proid']);
                        $_REQUEST['boErro'] = 0;
                        
                        if ($temPendencia == true) {
                            if ($pendenciasImpeditivas > 0) {
                                $arrErrosWarning[] = '- A entidade possui pendências no PAR. <a href="par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid='.$arrEntidade['inuid'].'"><b>Clique aqui!</b></a>';
                            }
                        } else {
                            if ($pendenciasImpeditivas > 0) {
                                $_REQUEST['pendencia_entidade'] = 'S';
                                $arrErros[] = '- A entidade possui pendências no PAR. <a href="par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid='.$arrEntidade['inuid'].'"><b>Clique aqui!</b></a>';
                            }
                        }
                        
                        if (!empty($arrErros)) {
                            echo '<div class="alert alert-danger" role="alert">'.implode('<br>', $arrErros).'</div>';
                            $_REQUEST['boErro'] = 1;
                        }
                        
                        if (!empty($arrErrosWarning)) {
                            echo '<div class="alert alert-warning" role="alert">'.implode('<br>', $arrErrosWarning).'</div>';
                        }
                        ?>
                        <div class="panel panel-default">
                            <div class="panel-heading" style="text-align: center; font-size: 18px;"><b>Dados de Processo</b></div>
                            <div class="panel-body">
<?php                           echo $obPagamento->carregaAcompanhamentoEntidade($_REQUEST['proid']);?>
                            </div>
                        </div>
                        <div class="panel panel-primary">
                            <div class="panel-heading" style="text-align: center;"><h2>Lista de Empenho(s) do Processo</h2></div>
                                <div class="panel-body">
<?php echo $obPagamento->carregarDadosEmpenho($_REQUEST); ?>
<?php if ($_REQUEST['empid']) {
        $empnumero = $db->pegaUm("select empnumero from par3.empenho e where empid = {$_REQUEST['empid']}");
    ?>
                                <div class="panel panel-success">
                                    <div class="panel-heading" style="text-align: center;"><h3>Solicitar Pagamento para o Empenho: <b><?php echo $empnumero;?></b></h3></div>
                                    <div class="panel-body">
                                        <form name="form-dados-pagamento" id="form-dados-pagamento" class="form-horizontal" method="post">
                                            <input type="hidden" name="requisicao" id="requisicao" value=""/>
                                            <input type="hidden" name="form-autentica-post" id="form-autentica-post" value=""/>
    <?php      $arRetorno = $obPagamento->carregarDadosParaPagamento($_REQUEST);
           $boPendenciaIniciativa = $arRetorno[1];
           echo $arRetorno[0];
    ?>
                                        <div class="row">
                                            <div class="col-lg-12" style="text-align: right;">
                                            <?php
                                                $sql_mes = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
                                                $sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos";
                                                echo $simec->select('mes', 'Mês', date('m'), $sql_mes, array(), array('input-size' => '4'));
                                                echo $simec->select('ano', 'Ano', date('Y'), $sql_ano, array(), array('input-size' => '4'));?>
                                            </div>
                                        </div>
                                        </form>
                                        <div class="ibox-footer">
                                            <div class="center">
    <?php                                           if ($_REQUEST['boErro'] == 0 && $pendenciasImpeditivas == 0) {    ?>
                                                    <button type="button" id="btn-carrega-login" data-dismiss="modal" class="btn btn-primary" data-loading-text="Solicitando, aguarde ..." style="width: 200px;"><i class="fa fa-plus"></i> Solicitar Pagamento</button>
    <?php                                           }   ?>
                                                <button type="reset" class="btn btn-success btn-visualizar" style="width: 200px;"><i class="fa fa-search"></i> Visualizar</button>
                                                <button type="button" class="btn btn-success btn-voltar" style="width: 200px;"> Voltar</button>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="panel panel-primary">
                                        <div class="panel-heading" style="text-align: center;"><h4>Histórico de pagamentos solcitados ao FNDE para o Empenho: <b><?php echo $empnumero;?></b></h4></div>
                                            <div class="panel-body">
    <?php echo $obPagamento->carregarDadosHistoricoPagamento($_REQUEST['empid']); ?>
                                            </div>
                                        </div>
                                    </div>
                                </div>
<?php }?>
                            </div>
                        </div>
                    </div>
                </div>
        </div>
</div>
<script>

//dropdown da coluna de execução da obra
$('.dropup_deferido').hover(function(e){
    $('.dropup_deferido').removeClass('open'); //fecha qualquer outro drop up
    $(this).addClass('open')}
);

jQuery('.class_dropdown').click(function(e){
 var proid = jQuery(this).attr("proid");

 jQuery('.dados_obras').html('');
    var caminho = window.location.href;
	var action  = '&requisicao=carregaDadosObra&&proid='+proid;
	
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
            jQuery(".dados_obras").html(resp);
        }
    });
}
);

function js_travaPagamento( empid ) {
    jQuery('#chkempenho_'+empid).attr('checked', false);
    jQuery('.chk_acao').attr('disabled', true);
    //jQuery('#btn-carrega-login').attr('disabled', true);
    swal("Ação não Permitida!", "Não é possível realizar o Pagamento. Para Pagar, será necessário resolver as Pendências do PAR", "error");
}

jQuery(document).ready(function(){
    var empid = jQuery('[name="empid"]').val();
    if( empid != '' ){
        jQuery('#chkempenho_'+empid).attr('checked', true);
    }

    if( jQuery('[name="pendencias"]').val() != 0 ){
        js_travaPagamento( empid );
    }
});

function cacularValorPagamento( codigo, campo ){
    var valorItem = jQuery('#valor_item_'+codigo).val();
    var valorTotal = jQuery('#valor_total_'+codigo).val();
    var valorEmpenho = jQuery('#valor_empenho_'+codigo).val();
    var valorPagoEmpenho = jQuery('#valor_pago_empenho_'+codigo).val();

    if( parseFloat(valorEmpenho) > parseFloat(valorItem) ){
        valorEmpenho = valorItem;
    }
    
    if( campo == 'percent' ){
        var valorDigitado = jQuery('#percent_'+codigo).val();
        
        if( valorDigitado > 100 ){
            valorDigitado = 100;
            jQuery('#percent_'+codigo).val(100)
        }
        var valorCalculo = (parseFloat(valorDigitado)/100)* valorEmpenho;

        var totalPago = parseFloat(valorCalculo) + parseFloat(valorPagoEmpenho);
        totalPago = number_format(totalPago, 2, ',', '.');
        totalPago = retiraPontos(totalPago);

        if( parseFloat(totalPago) > parseFloat(valorEmpenho) ){
            alert('"Valor pagamento (R$)" + "Valor pago nesse empenho(R$)" não pode ser maior que o "Valor Empenhado (R$)"');
            valorCalculo = parseFloat(valorEmpenho) - parseFloat(valorPagoEmpenho);
            var percentual = (parseFloat(valorCalculo) * 100) / parseFloat(valorEmpenho);
            
            var vlrSplit = percentual.toFixed(2).split('.');
            if( vlrSplit[1] == '00' ){
                percentual = vlrSplit[0];
                jQuery('#percent_'+codigo).val(percentual);
            } else {
                jQuery('#percent_'+codigo).val(percentual.toFixed(2));
            }
        }
        
        jQuery('#valorpagamento_'+codigo).val(mascaraglobal('[.###],##',valorCalculo.toFixed(2)));
        
    } else if (campo == 'valor') {
        var valorDigitado = retiraPontos(jQuery('#valorpagamento_'+codigo).val());
        if(valorDigitado == '') valorDigitado = 0;
        var valorCalculo = (parseFloat(valorDigitado) * 100) / parseFloat(valorEmpenho);

        var totalPago = parseFloat(valorDigitado) + parseFloat(valorPagoEmpenho);
        totalPago = number_format(totalPago, 2, ',', '.');
        totalPago = retiraPontos(totalPago);
        
        if( parseFloat(totalPago) > parseFloat(valorEmpenho) ){
            alert('"Valor pagamento (R$)" + "Valor pago nesse empenho(R$)" não pode ser maior que o "Valor Empenhado (R$)"');
            var valorCalculoPag = parseFloat(valorEmpenho) - parseFloat(valorPagoEmpenho);
            valorCalculo = (parseFloat(valorCalculoPag) * 100) / parseFloat(valorEmpenho);

            jQuery('#valorpagamento_'+codigo).val(mascaraglobal('[.###],##',valorCalculoPag.toFixed(2)));
        }
         
         if( valorCalculo >= 100 ){
             jQuery('#percent_'+codigo).val(100);
         } else {
            var vlrSplit = valorCalculo.toFixed(2).split('.');
            if( vlrSplit[1] == '00' ){
                valorCalculo = vlrSplit[0];
                jQuery('#percent_'+codigo).val(valorCalculo);
            } else {
                jQuery('#percent_'+codigo).val(valorCalculo.toFixed(2));
            }
         }
    }

    if(campo != 'total') { //calcular percentual obra

        var valorDigitado = retiraPontos(jQuery('#valorpagamento_' + codigo).val());
        if(valorDigitado == '') valorDigitado = 0;
//        var vlr_total_pagamento = jQuery('#vlr_total_pagamento_' + codigo).val();
//        vlr_total_pagamento = parseFloat(vlr_total_pagamento) + parseFloat(valorDigitado);

        var percentualTotal = (parseFloat(valorDigitado) * 100) / parseFloat(valorTotal);

        if (percentualTotal == '') percentualTotal = 0;

        jQuery('#total_pago_' + codigo).val(parseFloat(percentualTotal).toFixed(2));
    }else{ //calcular pela porcentagem total
        var perc_total_digitado = jQuery('#total_pago_' + codigo).val();
        var perc_total_pago = jQuery('#total_pago_' + codigo).attr('data-max');

        if(perc_total_digitado == '') perc_total_digitado = 0;

        var valorPagamento = (parseFloat(valorTotal) * parseFloat(perc_total_digitado)) / 100; //  - parseFloat(vlr_total_pagamento);

        var valorPorcentagem = parseFloat(valorPagamento) * 100 / parseFloat(valorEmpenho);

        if( parseFloat(valorPagamento) + parseFloat(valorPagoEmpenho) > parseFloat(valorEmpenho) ){
            alert('"Valor pagamento (R$)" + "Valor pago nesse empenho(R$)" não pode ser maior que o "Valor Empenhado (R$)"');
            var valorCalculoPag = parseFloat(valorEmpenho) - parseFloat(valorPagoEmpenho);
            valorCalculo = (parseFloat(valorCalculoPag) * 100) / parseFloat(valorEmpenho);
            jQuery('#valorpagamento_'+codigo).val(mascaraglobal('[.###],##',valorCalculoPag.toFixed(2)));

            valorCalculo = parseFloat(valorEmpenho) - parseFloat(valorPagoEmpenho);
            var percentual = (parseFloat(valorCalculo) * 100) / parseFloat(valorEmpenho);

            var vlrSplit = percentual.toFixed(2).split('.');
            if( vlrSplit[1] == '00' ){
                percentual = vlrSplit[0];
                jQuery('#percent_'+codigo).val(percentual);
            } else {
                jQuery('#percent_'+codigo).val(percentual.toFixed(2));
            }

            var percent_total = (valorEmpenho - parseFloat(valorPagoEmpenho)) * 100 / valorTotal;
            percent_total = number_format(percent_total, 2, ',', '.');

            jQuery('#total_pago_'+codigo).val(percent_total);

        }else{
            jQuery('#valorpagamento_' + codigo).val(mascaraglobal('[.###],##',valorPagamento.toFixed(2)));
            jQuery('#percent_' + codigo).val(valorPorcentagem.toFixed(2));
        }

    }

    calculaValorTotal();
}

function alteraValorDesembolso(check, codigo){
        var solicitacoes = $('.chkdef_' + codigo + ':checked');
        var percentTotal = 0;
        solicitacoes.each(function () {
            percentTotal += parseFloat(this.value);
        });

        var percentPago = $('#percent_total_pago_' + codigo).val();

        if(parseInt(percentPago) + percentTotal > 100){
            $(check).prop('checked', false);
            alert("Os desembolsos selecionados para pagamento ultrapassam o valor total!");
            return;
        }

        jQuery('#total_pago_' + codigo).val(percentTotal);

        cacularValorPagamento(codigo, 'total');
}

function calculaValorTotal(){
    var valorTotal = 0;
    jQuery(".valorpagamento").each(function(){
        if( jQuery( this ).val() ){
            valorTotal = parseFloat(valorTotal) + parseFloat(retiraPontos(jQuery( this ).val()));
        }
     });
    jQuery('#valor_total_pagamento').val( number_format(valorTotal, 2, ',', '.') );
}

function pegaValorJaPago(){
	var proid = jQuery('[name="proid"]').val();
	var tipo_processo = jQuery('[name="tipo_processo"]').val();
	
	var caminho = window.location.href;
   	var action  = '&requisicao=pegaValorJaPago&proid='+proid+'&tipo='+tipo_processo;
   	var retorno = 0;
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

jQuery('.btn-voltar').click(function(){
    window.location.href = 'par3.php?modulo=principal/orcamento/listaPagamentos&acao=A';
});

jQuery('.btn-visualizar').click(function(){

    var dados = $('#form-dados-pagamento').serialize();
    
    var caminho = window.location.href;
    var action  = '&requisicao=visualiza_xml&='+dados;
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            jQuery("#div_xml").html(resp);
        }
    });
   
    $("#modal-visualiza-xml").modal("show");
});

jQuery('.chk_acao').click(function(){
    var codigo = this.value;
        
    if( jQuery('#chk_'+codigo+':checked').length > 0 ){
        jQuery('#percent_'+codigo).attr('disabled', false);
        jQuery('#valorpagamento_'+codigo).attr('disabled', false);
        jQuery('#total_pago_'+codigo).attr('disabled', false);
        jQuery('.chkdef_'+codigo).attr('disabled', false);
        jQuery('#percent_'+codigo).focus();

    } else {
        jQuery('#percent_'+codigo).attr('disabled', true);
        jQuery('#valorpagamento_'+codigo).attr('disabled', true);
        jQuery('#total_pago_'+codigo).attr('disabled', true);
        jQuery('.chkdef_'+codigo).attr('disabled', true);
        jQuery('.chkdef_'+codigo).attr('checked', false);
        jQuery('#percent_'+codigo).val('');
        jQuery('#valorpagamento_'+codigo).val('');
        jQuery('#total_pago_'+codigo).val('');
    }
    calculaValorTotal();
});

jQuery('#btn-carrega-login').click(function(){

    if( jQuery('.chk_acao:checked').length == 0 ){
        alert('Selecione um pagamento!');
        return false;
    }

    var text_erro = '';
    var error = false;
    jQuery('.chk_acao:checked').each(function(){
        var codigo = jQuery(this).val();

        if( jQuery('[name="percent['+codigo+']"]').val() == '' ){
        	text_erro = 'Informe o percentual de pagamento!';
        	error = true;
        }
        
        if( jQuery('[name="valorpagamento['+codigo+']"]').val() == '' || jQuery('[name="valorpagamento['+codigo+']"]').val() < 1 ){
        	text_erro = 'Informe o Valor do pagamento!';
        	error = true;
        }
    });

    if( error == true ){
        alert(text_erro);
        return false;
    }
    
    if( jQuery('[name="mes"]').val() == '' ){
        alert('Informe o Mês!');
        return false;
    }
    if( jQuery('[name="ano"]').val() == '' ){
        alert('Informe o Ano!');
        return false;
    }

    var valor_total_ja_pago = pegaValorJaPago();
    var valor_total_informado = jQuery('#valor_total_pagamento').val();    
    var valor_total_processo = jQuery('[name="valor_total_processo"]').val();    
    
    valor_total_ja_pago = retiraPontos(number_format(valor_total_ja_pago, 2, ',', '.'));
    valor_total_informado = retiraPontos(valor_total_informado);

    valor_total_ja_pago = parseFloat(valor_total_ja_pago) + parseFloat(valor_total_informado);

    if( parseFloat(valor_total_processo) < parseFloat(valor_total_ja_pago) ){
    	swal("Ação não Permitida!", "Não é possível realizar o Pagamento. O valor total do processo R$ "+number_format(valor_total_processo, 2, ',', '.')+" não pode ser menor que o valor total a pagar R$ "+number_format(valor_total_ja_pago, 2, ',', '.')+"!", "error");
    	return false;
	} else {    
        jQuery('#btn-carrega-login').button('loading');
        jQuery('[name="acao_login"]').val('solicitar');
        jQuery('#nome_button').html('<i class="fa fa-plus"></i> Solicitar Pagamento');
        jQuery('[name="requisicao"]').val('solicitar');
        //jQuery('[name="form-dados-pagamento"]').submit();
        $("#modal-login-pagamento").modal("show");
	}
});

function selecionarEmpenhoPagamento( empid ){
    divCarregando();
    var proid = jQuery('[name="proid"]').val();
    var tipo = jQuery('[name="tipo_processo"]').val();

    if( jQuery('#chkempenho_'+empid+':checked').length == 0 ){
        window.location.href = "par3.php?modulo=principal/orcamento/solicitarPagamento&acao=A&proid="+proid+"&tipo="+tipo;
    } else {
        window.location.href = "par3.php?modulo=principal/orcamento/solicitarPagamento&acao=A&proid="+proid+"&tipo="+tipo+"&empid="+empid;
    }
    
}

function cancelar_login(){
    jQuery('#btn-carrega-login').button('reset');
}

function carregar_login(){
    if(jQuery('[name="ws_usuario_aut"]').val() == ''){
        alert('Informe o Usuário!');
        jQuery(this).button('reset');
        return false;
    }
    if(jQuery('[name="ws_senha_aut"]').val() == ''){
        alert('Informe a Senha!');
        jQuery(this).button('reset');
        return false;
    }
    divCarregando();
    var $dataForm = jQuery('#form-autentica').serialize();
    
    jQuery('[name="form-autentica-post"]').val( $dataForm );
    jQuery('[name="requisicao"]').val(  jQuery('[name="acao_login"]').val() );

    jQuery('[name="form-dados-pagamento"]').submit();
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

function visualizarDadosPagamento( pagid ){
    
    var caminho = window.location.href;
    var action  = '&requisicao=visualiza_historico&pagid='+pagid;
    var caminho = window.location.href;
    var action  = action;
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            jQuery("#div_historico").html(resp);
        }
    });
   
    $("#modal-visualiza-historico").modal("show");
}

function cancelarPagamento( pagid, pagnumseqob ){
    if( pagnumseqob != '' ){
        jQuery('[name="pagid"]').val( pagid );
        jQuery('[name="pagnumseqob"]').val( pagnumseqob );
        jQuery('[name="acao_login"]').val('cancelar_pagamento');
        jQuery('#nome_button').html('<i class="fa fa-refresh"></i> Cancelar Pagamento');
        $("#modal-login-pagamento").modal("show");
    } else {
        swal({
            title: "Confirmar",
            text: "Deseja excluir este <b>pagamento</b>?",
            type: "success",
            html: true,
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var caminho = window.location.href;
                var action  = '&requisicao=excluir_pagamento&pagid='+pagid;
                var caminho = window.location.href;
                var action  = action;
               jQuery.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        window.location.href = window.location;
                    }
                });
            }
            return false;
        }
        );
    }
}

function concultarPagamento( pagid, pagnumseqob ){
    jQuery('[name="pagid"]').val( pagid );
    jQuery('[name="pagnumseqob"]').val( pagnumseqob );
    jQuery('[name="acao_login"]').val('consultar_pagamento');
    jQuery('#nome_button').html('<i class="fa fa-refresh"></i> Consultar Pagamento');
    $("#modal-login-pagamento").modal("show");
}

function reenviarPagamento( pagid ){
    jQuery('[name="pagid"]').val( pagid );
    jQuery('[name="acao_login"]').val('reenviar_pagamento');
    jQuery('#nome_button').html('<i class="fa fa-plus"></i> Solicitar Pagamento');
    $("#modal-login-pagamento").modal("show");
}
</script>

<div class="ibox float-e-margins animated modal" id="modal-login-pagamento" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-md">
        <div class="ibox-title" style="background-color: #1c84c6; border-color: #1c84c6; color: #ffffff; text-align: center">
            <h1 class="center" style="font-weight: bold;">Autenticação</h1>
        </div>
        <div class="ibox-content" style="text-align: center; border-color: #1c84c6;">
            <div class="row">
                <form method="post" name="form-autentica" id="form-autentica" class="form form-horizontal"  method="post">
                    <input name="acao_login" id="acao_login" type="hidden" value="">
                    <div class="row">
                        <div class="form-group ">
                            <label for="codana" class="col-md-3 control-label">Usuário: </label>
                            <div class="col-md-7 ">
                                <input name="ws_usuario_aut" id="ws_usuario_aut" type="text" value="" class="form-control">
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="form-group ">
                            <label for="codana" class="col-md-3 control-label">Senha: </label>
                            <div class="col-md-7 ">
                                <input name="ws_senha_aut" id="ws_senha_aut" type="password" value="" class="form-control">
                            </div>
                            <div style="clear:both"></div>
                        </div>
                    </div>
                </form>
            </div>
        </div>
        <div class="ibox-footer" style="text-align: center;">
            <button type="button" id="btn-solicitar-pagamento" data-dismiss="modal" class="btn btn-success" onclick="carregar_login();"><div id="nome_button"><i class="fa fa-plus"></i> Solicitar Pagamento</div></button>
            <button type="button" id="btn-cancelar" data-dismiss="modal" class="btn btn-warning" onclick="cancelar_login();"><i class="fa fa-times-circle-o"></i> Cancelar</button>
        </div>
    </div>
</div>

<div class="ibox float-e-margins animated modal" id="modal-visualiza-xml" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
    <div class="modal-dialog modal-lg">
        <div class="ibox-title" style="background-color: #1c84c6; border-color: #1c84c6; color: #ffffff; text-align: center">
            <h1 class="center" style="font-weight: bold;">XML de Envio</h1>
        </div>
        <div class="ibox-content" id="div_xml"></div>
        <div class="ibox-footer" style="text-align: center;">
            <button type="button" id="btn-fechar" data-dismiss="modal" class="btn btn-warning"><i class="fa fa-times-circle-o"></i> Fechar</button>
        </div>
    </div>
</div>

<div class="ibox float-e-margins animated modal" id="modal-visualiza-historico" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
    <div class="modal-dialog modal-lg">
        <div class="ibox-title" style="background-color: #1c84c6; border-color: #1c84c6; color: #ffffff; text-align: center">
            <h1 class="center" style="font-weight: bold;">Historico de Pagamento</h1>
        </div>
        <div class="ibox-content" id="div_historico"></div>
        <div class="ibox-footer" style="text-align: center;">
            <button type="button" id="btn-fechar" data-dismiss="modal" class="btn btn-warning"><i class="fa fa-times-circle-o"></i> Fechar</button>
        </div>
    </div>
</div>

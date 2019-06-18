<?php
$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$arrIniciativaAno = Par3_Model_IniciativaPlanejamento::getInicitivaAno();
$obPagamento = new Par3_Controller_Pagamentos();
$sqlTipoAssitencia = Par3_Model_IniciativaPlanejamento::getTipos_Assistencia();

switch ($_REQUEST['requisicao']) {
    case 'carregaDadosObra':
        echo carregaDadosObraLista($_REQUEST['proid']);
        die;
    break;
    case 'carregarMun':
        $municipio = new Territorios_Model_Municipio();
        print simec_json_encode($municipio->lista(array('muncod', 'mundescricao||\' - \'||estuf as mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
        die;
    break;
    case 'excel':
        ob_clean();
        $sql = $obPagamento->listar($_REQUEST);
        
        $arrayCabecalhoDetalhado = array(
            'Planejamento', 'Ano', 'Termo','Processo','UF', 'Entidade', 'Tipo de Objeto', 'Usuário', 'Data Criação', 'Data Vigência',
            'Valor Processo', 'Valor Termo', 'Nota do empenho', 'Valor Empenho', '% Empenho', 'Valor Pagamento', 'Data Pagamento', 'Parcela', 'Mês', 'Ano Parcela', 'Nº OB', 'Usuário Pagamento', 'Situação Pagamento', 'Assistência', 'Situação última solicitação',
            'Obras', 'CACS', 'PNE', 'SIOPE', 'Habilita', 'Contas', 'ID (PAR3)', 'ID (OBRAS2)', 'Situação PAR3', 'Situação Obras2', 'Validação da OS', '% Físico total', '% Executado Instituição'
        );
        
        //$arrayCabecalho = array('Processo','UF', 'Entidade', 'Tipo de Objeto', 'Valor Processo', 'Valor Empenho', 'Valor Pagamento','% Pago <br>Empenho', '% Pago <br> Processo', 'Assistência', 'Situação', 'Bloqueio PAR');
        $arrayCabecalhoSintetico = array('Processo','UF', 'Entidade', 'Tipo de Objeto', 'Valor Processo', 'Valor Empenho', 'Valor Pagamento', '% Pago <br>Empenho', '% Pago <br> Processo', 'Assistência', 'Situação', 'Obras', 'CACS', 'PNE', 'SIOPE', 'Habilita', 'Contas', 'OS validada');
        //$arrayCabecalhoDetalhado = array('UF','Entidade','Processo','Tipo de Objeto','Assitência','Valor Processo','Valor total pago','Nº Empenho','Valor do Empenho','% Pago <br> Empenho','% Pago <br> Processo','Parcela','Mês','Ano','Seq OB','Nº OB','Valor(R$) Parcela','Data pagamento','Situação pagamento','Usuário','Obras','CACS','PNE','SIOPE','Habilita','Contas', 'OS validada');
        $arrayCabecalho = $_REQUEST['tipodetalhado'] == 'sim' ? $arrayCabecalhoDetalhado : $arrayCabecalhoSintetico;

        $esconderColunas = array('tipo', 'inuid', 'proid', 'id', 'pagvalorparcela', 'pagnumseqob', 'tipo_processo');
        Simec_Listagem_Renderer_Xls::$filename = ($_REQUEST['tipodetalhado'] == 'sim') ? 
            'Lista_de_Pagamentos_Solicitados_Detalhado.xls' : 
            'Lista_de_Pagamentos_Solicitados_Sintético.xls';
        $listagemSimec = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->addCallbackDeCampo(['vlrpagamento','vlrempenho','valor_processo','pagvalorparcela'], 'par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('processo', 'formata_numero_processo');
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        die;
    break;
    case 'detalharPagamento':
        $sql = "select distinct e.empid from par3.processo p
					inner join par3.empenho e on e.empnumeroprocesso = p.pronumeroprocesso and e.empcodigoespecie = '01'
				where p.proid = {$_REQUEST['dados'][0]}
					and e.empstatus = 'A'";
        $arrEmpid = $db->carregarColuna($sql);
        if (is_array($arrEmpid)) {
            $arrEmpid = implode(', ', $arrEmpid);
        }
        echo $obPagamento->carregarDadosHistoricoPagamento($arrEmpid, 'lista');
        die;
    break;
    case 'atualizaPendenciaView':
        
        $sql = "DELETE FROM par3.vm_relatorio_quantitativo_pendencias WHERE inuid = {$_REQUEST['inuid']};
                INSERT INTO par3.vm_relatorio_quantitativo_pendencias(
                    inuid, habilitacao, cae, contas, monitoramento_par, obras_par,
                    cacs, pne, siope, siope_dsc)
                VALUES (
                    {$_REQUEST['inuid']}, '{$_REQUEST['habilita']}', '{$_REQUEST['cae']}', '{$_REQUEST['contas']}', '{$_REQUEST['monitoramento']}', '{$_REQUEST['par']}',
                    '{$_REQUEST['cacs']}', '{$_REQUEST['pne']}', '{$_REQUEST['siope']}', '{$_REQUEST['siope_desc']}');";
                    $db->executar($sql);
                    $db->commit();
        die;
    break;
    case 'atualizaPendencias':
        //include_once APPRAIZ . "par3/classes/helper/PendenciasEntidade.class.inc";
        include_once APPRAIZ . "par/classes/modelo/InstrumentoUnidade.class.inc";
        
        $helperPendencia = new Par3_Helper_PendenciasEntidade();
        
        if( $_REQUEST['tipo'] == 'cae' ){
            $cae = $helperPendencia->carregaPendenciaConselhoCAE($_REQUEST['inuid']) == 1 ? 't' : 'f';
            //retirar valor padrão
            $cae = 'f';
            echo $cae;
        }
        if( $_REQUEST['tipo'] == 'par' ){
            $par = $helperPendencia->existePendenciaPorTipo( 'par', $_REQUEST['inuid'] ) == true ? 't' : 'f';
            echo $par;
        }
        if( $_REQUEST['tipo'] == 'cacs' ){
            $cacs = $helperPendencia->consultaPendenciaCACS($_REQUEST['inuid']) == 1 ? 't' : 'f';
            echo $cacs;
        }
        if( $_REQUEST['tipo'] == 'pne' ){
            $pne = $helperPendencia->consultaArquivoPNE($_REQUEST['inuid']) == 1 ? 't' : 'f';
            echo $pne;
        }
        if( $_REQUEST['tipo'] == 'habilita' ){
            $habilitacao = $helperPendencia->consultaHabilitaEntidade($_REQUEST['inuid']) == 1 ? 't' : 'f';
            echo $habilitacao;
        }
        if( $_REQUEST['tipo'] == 'monitoramento' ){
            $monitoramento = $helperPendencia->consultaPendenciaMonitoramentoPAR($_REQUEST['inuid']) == 1 ? 't' : 'f';
            echo $monitoramento;
        }
        if( $_REQUEST['tipo'] == 'siope' ){
            $oSiope         = new Par3_Model_Siope();
            $resultado      = $oSiope->transmissaoSiope(array('inuid' =>$_REQUEST['inuid']));
            $transmissao    = $resultado['0']['cod_situacao'];
            $siope_dsc      = $resultado['0']['situacao_desc'];
            $siope['cod']   = ($transmissao == 2) ? 't' : 'f';
            $siope['desc']  = $siope_dsc;
//            echo $siope;
            echo simec_json_encode($siope);
        }
        if( $_REQUEST['tipo'] == 'contas' ){
            $prestaoContas = $helperPendencia->consultaPendenciaPrestacaoContas($_REQUEST['inuid']) == 1 ? 't' : 'f';
            echo $prestaoContas;
        }
        die;
    break;
    case 'imprimir-listapagamento':
        echo $obPagamento->listar($_REQUEST);
        die;
}

require APPRAIZ . 'includes/cabecalho.inc';

//echo $simec->title('Lista de Pagamentos Solicitados', '' );

$db->cria_aba($abacod_tela, $url, '');
?>
<style>
    .popover-header {
        padding: .5rem .75rem;
        margin-bottom: 0;
        font-size: 1rem;
        color: inherit;
        background-color: #f7f7f7;
        border-bottom: 1px solid #ebebeb;
        border-top-left-radius: calc(.3rem - 1px);
        border-top-right-radius: calc(.3rem - 1px);
    }
</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Lista de Pagamentos Solicitados</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-pesquisa-pagamento" id="form-pesquisa-pagamento" class="form-horizontal" method="post">
                <input type="hidden" name="tipodetalhado" id="tipodetalhado" value="nao" />
                <input type="hidden" name="requisicao" id="requisicao" value="filtrar"/>
                <div class="row">
                    <div class="col-lg-6">
                        <?php
                        $filtro = simec_htmlentities($_POST['numeroprocesso']);
                        $numeroprocesso = $filtro;

                        $config = array();
                        $attribs = array('maxlength' => '255',
                            'onkeyup' => "this.value=mascaraglobal('#####.######/####-##',this.value)",
                            'onblur' => "MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);", 'autofocus');

                        echo $simec->input('numeroprocesso', 'Número de Processo', $numeroprocesso, $attribs, array('input-size' => '6'));

                        $_REQUEST['tipo_processo'] = (empty($_REQUEST['tipo_processo']) ? 'T' : $_REQUEST['tipo_processo']);

                        $arrOptions = array('O' => 'Obras', 'P' => 'PAR', 'T' => 'Todos');
                        ?>
                        <?php echo $simec->input('dotnumero', 'Número do Termo', $_REQUEST['dotnumero'], array('type' => 'number'), array('input-size' => '6')); ?>
                        <?php echo $simec->input('proid', 'Código do Processo', $_REQUEST['proid'], array('type' => 'number'), array('input-size' => '6')); ?>
                        <?php echo $simec->input('codigo', 'Código Obra/Iniciativa', $_REQUEST['codigo'], array('type' => 'number'), array('input-size' => '6')); ?>



                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termo Validado:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="termo_validado">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termo_validado' id='termo_validado_s' value="S" <?= $_REQUEST['termo_validado'] == 'S'?'checked':'';?>>
                                        <label for="termo_validado_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termo_validado" id="termo_validado_n" value="N"  <?= $_REQUEST['termo_validado'] == 'N'?'checked':'';?>>
                                        <label for="termo_validado_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termo_validado" id="termo_validado_t" value="T"  <?= $_REQUEST['termo_validado'] != 'S' && $_REQUEST['termo_validado'] != 'N'?'checked':'';?>>
                                        <label for="termo_validado_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Termos com contrato:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="pagsituacaopagamento_group_id">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='termocontrato' id='termocontrato_s' value="S" <?= $_REQUEST['termocontrato'] == 'S'?'checked':'';?>>
                                        <label for="termocontrato_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termocontrato" id="termocontrato_n" value="N"  <?= $_REQUEST['termocontrato'] == 'N'?'checked':'';?>>
                                        <label for="termocontrato_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="termocontrato" id="termocontrato_t" value="T"  <?= $_REQUEST['termocontrato'] != 'S' && $_REQUEST['termocontrato'] != 'N'?'checked':'';?>>
                                        <label for="termocontrato_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group" >
                            <label class=" col-sm-3 control-label">Bloqueio do PAR:</label>
                            <div class="col-sm-12 col-md-9 col-lg-4">
                                <div id="bloqueio_par">
                                    <div class="radio radio-default radio-inline ">
                                        <input type='radio' name='bloqueio_par' id='bloqueio_par_s' value="S" <?= $_REQUEST['bloqueio_par'] == 'S'?'checked':'';?>>
                                        <label for="bloqueio_par_s">Sim</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="bloqueio_par" id="bloqueio_par_n" value="N"  <?= $_REQUEST['bloqueio_par'] == 'N'?'checked':'';?>>
                                        <label for="bloqueio_par_n">Não</label>
                                    </div>
                                    <div class="radio radio-default radio-inline ">
                                        <input type="radio" name="bloqueio_par" id="bloqueio_par_t" value="T"  <?= $_REQUEST['bloqueio_par'] != 'S' && $_REQUEST['bloqueio_par'] != 'N'?'checked':'';?>>
                                        <label for="bloqueio_par_t">Todos</label>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <?php
                            $_REQUEST['status_pagamento'] = tratarArrayParaMultiSelect($_REQUEST['status_pagamento']);
                            $arrPagamento = ['P' => 'Parcial','C' => '100% Pago','S' => 'Sem Pagamento','T' => 'Todos'];
                            echo $simec->select(
                                'status_pagamento',
                                'Pagamento',
                                $_REQUEST['status_pagamento'],
                                $arrPagamento,
                                array('maxlength' => '255','multiple','multiple'),
                                array('input-size' => '4','label-size' => '3')
                            );
                        ?>
                    </div>
                    <div class="col-lg-6">
                        <?php echo $simec->radio('itrid', 'Esfera', ($_REQUEST['itrid']?:0), [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'], array('maxlength' => '255')); ?>
                        <div class="row">
                            <div class="col-lg-5" style="margin-left:55.5px;">
                                <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255'), array('input-size' => '6', 'label-size' => '5')); ?>
                            </div>
                            <div class="col-lg-5" id="div-muncod" style="display:<?= $_REQUEST['itrid'] == 1 ?'none':''?>;">
                                <?php
                                $_REQUEST['muncod'] = tratarArrayParaMultiSelect($_REQUEST['muncod']);
                                if ($_REQUEST['estuf']) {
                                    $municipio = new Territorios_Model_Municipio();
                                    $arrMunicipio = simec_preparar_array($municipio->lista(array('muncod as codigo', 'mundescricao||\' - \'||estuf as descricao'), array("estuf = '{$_REQUEST['estuf']}'")));
                                }
                                echo $simec->select('muncod[]', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255','multiple' => 'multiple'), array()); ?>
                            </div>
                        </div>

                        <?php echo $simec->select('iniano', 'Ano', $_REQUEST['iniano'], $arrIniciativaAno, array('maxlength' => '255'), array('input-size' => '4')); ?>
                        <?php echo $simec->radio('tipo_processo', 'Tipo de Processo', $_REQUEST['tipo_processo'], $arrOptions, array(), array()); ?>
                        <div id="div_os_validada" style="display: <?php echo ($_REQUEST['tipo_processo'] == 'O' ? '' : 'none')?>;">
                            <?php echo $simec->radio('os_validada', 'OS Validada', $_REQUEST['os_validada'], ['S' => 'Sim','N' => 'Não','' => 'Todos'], array(), array()); ?>
                        </div>
                        
                        <div id="div-intoid" style="display:<?= $_REQUEST['tipo_processo'] == 'O'?'none;':'';?>">
                            <?php
                                $arrTipoObjeto = Par3_Model_IniciativaPlanejamento::getTipoObjetoporIniciativa(['it.intoid <> 1']);
                                echo $simec->select('intoid', 'Tipo do Objeto', $_REQUEST['intoid'], $arrTipoObjeto, array('maxlength' => '255'), array('input-size' => '4'));
                            ?>
                        </div>
                        <div id="divTiposAssistencia">
                            <?php
                            $controllerIniciativaTiposAssistencia = new Par3_Controller_IniciativaTiposAssistenciaController();
                            $mInta = $controllerIniciativaTiposAssistencia->recuperar();
                            $arrTiposAssistencia = simec_preparar_array($mInta->recuperarTodosFormatoInput('intadsc'));
                            $arrTiposAssistencia[0] = 'Todos';
                            echo $simec->select('intaid', 'Tipos de Assistência', $_REQUEST['intaid'], $arrTiposAssistencia, ['placeHolder' => 'Selecione'], array('input-size' => '4','label-size' => '3'));
                            ?>
                        </div>
                        <?php
                        $_REQUEST['pagsituacaopagamento'] = tratarArrayParaMultiSelect($_REQUEST['pagsituacaopagamento']);
                        $sql = 'select  pagsituacaopagamento as codigo, pagsituacaopagamento as descricao from par3.pagamento group by pagsituacaopagamento';
                        echo $simec->select('pagsituacaopagamento', 'Situação do pagamento', $_REQUEST['pagsituacaopagamento'], $sql, array('maxlength' => '255','multiple','multiple'), array('input-size' => '4','label-size' => '3'));
                        echo $simec->radio('saldo_desembolso', 'Saldo % de deferimento', ($_REQUEST['saldo_desembolso']?:'T'), array('S' => 'Sim', 'N' => 'Não', 'T' => 'Todos'), array(), array());
                        ?>

                        <div class="form-group ">
                            <label for="dataini" class="col-sm-3 col-md-3 col-lg-3 control-label">Data do pagamento: </label>
                            <div class="row">
                                <div class="col-lg-5" style="text-align: center !important;">
                                    <?php
                                    $dotdtinicio = $_POST['pagdatapagamentoini'] ? formata_data_sql($_POST['pagdatapagamentoini']) : '';
                                    echo $simec->data('pagdatapagamentoini', 'De', $dotdtinicio, ['autocomplete' => 'off'], ['label-size' => '1', 'input-size' => '8']);
                                    ?>
                                </div>
                                <div class="col-lg-5" style="text-align: left !important; margin-left: -75.5px">
                                    <?php
                                    $dotdtfim = $_POST['pagdatapagamentofim'] ? formata_data_sql($_POST['pagdatapagamentofim']) : '';
                                    echo $simec->data('pagdatapagamentofim', 'Até', $dotdtfim, ['autocomplete' => 'off'], ['label-size' => '1', 'input-size' => '8']);
                                    ?>
                                </div>
                            </div>
                        </div>

                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-info btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button> |
                    <button type="button" id="btn-excel" data-dismiss="modal" class="btn btn-primary btn-excel" data-loading-text="Gerando, aguarde ..."><i class="fa fa-file-excel-o"></i> Gerar Excel</button> |
                    <button type="button" id="btn-imprimir" class="btn btn-warning" data-dismiss="modal" data-loading-text="Imprimindo, aguarde ..."><i class="fa fa-print" aria-hidden="true"></i> Imprimir</button> |
                    <button type="reset" class="btn btn-default-bright btn-limpar" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
                </div>
            </div>
        </div>
<div id="div_debug"></div>
        <div class="row">
            <!-- Listagem  -->
            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras">
                    <div class="table-responsive" >
                        <?php
                        if ($_REQUEST['requisicao'] == 'filtrar') {
                            echo $obPagamento->listar($_REQUEST);
                        }
                        ?>
                    </div>
                    <br>
                    <br>
                </div>
            </div>
        </div>
        
    </div>
</div>

<div id="div-impressao" style="display: none">
    <div id="div_cabecalho" class="row col-lg-offset-2">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Lista de Pagamentos Solicitados<br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                    Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>
    <div class="clearfix" style="margin-bottom:10px;"></div>
    <div id="impressao-content"></div>
</div>


<div class="ibox float-e-margins animated modal" id="modal-visualiza-pendencia" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
    <div class="modal-dialog modal-lg">
        <div class="ibox-title" style="background-color: #1c84c6; border-color: #1c84c6; color: #ffffff; text-align: center">
            <h1 class="center" style="font-weight: bold;">Pendências</h1>
        </div>
        <div class="ibox-content" id="div_pendencia">
        	<input type="hidden" name="inuid" value="">
        	<input type="hidden" name="cae" value="">
        	<input type="hidden" name="par" value="">
        	<input type="hidden" name="cacs" value="">
        	<input type="hidden" name="pne" value="">
        	<input type="hidden" name="habilita" value="">
        	<input type="hidden" name="monitoramento" value="">
        	<input type="hidden" name="siope" value="">
        	<div class="text-pendencia" style="font-size: 20px; font-weight: bold;">
			</div>
        	<div class="progress">
  				<div class="progress-bar" role="progressbar" aria-valuenow="1" aria-valuemin="0" aria-valuemax="100" style="width: 1%; font-size: 20px; font-weight: bold;">0%</div>
			</div>
        </div>
        <div class="ibox-footer" style="text-align: center;">
            <button type="button" id="btn-fechar" data-dismiss="modal" class="btn btn-warning" onclick="fecharPendencia();"><i class="fa fa-times-circle-o"></i> Fechar</button>
        </div>
    </div>
</div>

<script>
jQuery(document).keypress(function(e) {
    if(e.which == 13){
         jQuery('.btn-pesquisar').click();
    }
});

//dropdown da coluna de execução da obra
jQuery('.dropup_deferido').hover(function(e){
	jQuery('.dropup_deferido').removeClass('open'); //fecha qualquer outro drop up
	jQuery(this).addClass('open')}
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

jQuery(function() {
    //tela de loading
    $(window).on('beforeunload',function(){
        if($('#requisicao').val() != 'excel'){
            $('.loading-dialog-par3').show();
        }
    });
    $(window).on('click','#btn-pesquisar #limpar',function(){
        $('.loading-dialog-par3').show();
    });
    $(window).load(function(){
        $('.loading-dialog-par3').hide();
    });
    //
    //renderizeMunicipio();

    if(jQuery('[name="estuf"]').val() != '') {
        carregarMun(jQuery('[name="estuf"]').val(), <?=$_REQUEST['muncod']?>);
    }

    jQuery("input:radio[name=itrid], select[name=estuf]").change(function() {
        //renderizeMunicipio();
    });

    jQuery('select[name=estuf]').change(function(){
        carregarMun(this.value);
    });

    function renderizeMunicipio() {
        var filtroMunicipio = jQuery("select[name=muncod]").parents("div.form-group");
        if (jQuery('input:radio[name=itrid]:checked').val() === '1' || !jQuery('select[name=estuf]').val()) {
            filtroMunicipio.slideUp();
        } else {
            filtroMunicipio.slideDown();
        }
    }
    jQuery("[data-id-lista='listagemPagamentos']").remove();
});

function carregarMun(estuf, muncod) {
    if(estuf != '') {
        var options = jQuery('#muncod');
        options.empty();
        options.append(new Option('', ''));
        jQuery.post('', 'requisicao=carregarMun&estuf='+estuf, function(retorno) {
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

function solicitarPagamento( proid, tipo ){
    window.location.href = "par3.php?modulo=principal/orcamento/solicitarPagamento&acao=A&proid="+proid+'&tipo='+tipo;
}

function atualizaPendencias( proid, tipo, inuid ){
	$("#modal-visualiza-pendencia").modal("show");

	jQuery('[name="inuid"]').val(inuid);
	jQuery('.text-pendencia').html('Atualizando Pendência - Conselho CAE');
	
	var action  = '&requisicao=atualizaPendencias&tipo=cae&inuid='+inuid;
    var caminho = window.location.href;
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
            //jQuery("#div_debug").html(resp);
            jQuery('[name="cae"]').val(resp);
        	jQuery('.text-pendencia').html('Atualizando Pendência - Obras PAR');
        	jQuery('.progress-bar').html('14%');
        	jQuery('.progress-bar').css('width', '14%');
        	setTimeout(pendenciaPAR,1000);
        }
    });
}

function pendenciaPAR(){
	var caminho = window.location.href;
	var inuid = jQuery('[name="inuid"]').val();
	var action  = '&requisicao=atualizaPendencias&tipo=par&inuid='+inuid;
	
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
            //jQuery("#div_debug").html(resp);
            jQuery('[name="par"]').val(resp);
        	jQuery('.text-pendencia').html('Atualizando Pendência - CACS');
        	jQuery('.progress-bar').html('28%');
        	jQuery('.progress-bar').css('width', '28%');
        	setTimeout(pendenciaCACS,1000);
        }
    });
}

function pendenciaCACS(){
	var caminho = window.location.href;
	var inuid = jQuery('[name="inuid"]').val();
	var action  = '&requisicao=atualizaPendencias&tipo=cacs&inuid='+inuid;
	
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
            //jQuery("#div_debug").html(resp);
            jQuery('[name="cacs"]').val(resp);
        	jQuery('.text-pendencia').html('Atualizando Pendência - PNE');
        	jQuery('.progress-bar').html('42%');
        	jQuery('.progress-bar').css('width', '42%');
        	setTimeout(pendenciaPNE,1000);
        }
    });
}

function pendenciaPNE(){
	var caminho = window.location.href;
	var inuid = jQuery('[name="inuid"]').val();
	var action  = '&requisicao=atualizaPendencias&tipo=pne&inuid='+inuid;
	
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
            //jQuery("#div_debug").html(resp);
            jQuery('[name="pne"]').val(resp);
        	jQuery('.text-pendencia').html('Atualizando Pendência - Habilita');
        	jQuery('.progress-bar').html('56%');
        	jQuery('.progress-bar').css('width', '56%');
        	setTimeout(pendenciaHabilita,1000);
        }
    });
}

function pendenciaHabilita(){
	var caminho = window.location.href;
	var inuid = jQuery('[name="inuid"]').val();
	var action  = '&requisicao=atualizaPendencias&tipo=habilita&inuid='+inuid;
	
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
            //jQuery("#div_debug").html(resp);
            jQuery('[name="habilita"]').val(resp);
        	jQuery('.text-pendencia').html('Atualizando Pendência - Monitoramento PAR');
        	jQuery('.progress-bar').html('70%');
        	jQuery('.progress-bar').css('width', '70%');
   			setTimeout(pendenciaMonitoramento,1000);
        }
    });
}

function pendenciaMonitoramento(){
	var caminho = window.location.href;
	var inuid = jQuery('[name="inuid"]').val();
	var action  = '&requisicao=atualizaPendencias&tipo=monitoramento&inuid='+inuid;
	
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
            //jQuery("#div_debug").html(resp);
            jQuery('[name="monitoramento"]').val(resp);
        	jQuery('.text-pendencia').html('Atualizando Pendência - SIOPE');
        	jQuery('.progress-bar').html('84%');
        	jQuery('.progress-bar').css('width', '84%');
        	setTimeout(pendenciaSiope,1000);
        }
    });
}

function pendenciaSiope(){
	var caminho = window.location.href;
	var inuid = jQuery('[name="inuid"]').val();
	var action  = '&requisicao=atualizaPendencias&tipo=siope&inuid='+inuid;
	
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
            //jQuery("#div_debug").html(resp);
            jQuery('[name="siope"]').val(resp);
            jQuery('.text-pendencia').html('Atualizando View de Pendências');
        	jQuery('.progress-bar').html('100%');
        	jQuery('.progress-bar').css('width', '100%');
        	setTimeout(atualizaPendenciaView,1000);
        }
    });
}

function atualizaPendenciaView(){
	var caminho = window.location.href;
	var inuid = jQuery('[name="inuid"]').val();
	var cae = jQuery('[name="cae"]').val();
	var par = jQuery('[name="par"]').val();
	var cacs = jQuery('[name="cacs"]').val();
	var pne = jQuery('[name="pne"]').val();
	var habilita = jQuery('[name="habilita"]').val();
	var monitoramento = jQuery('[name="monitoramento"]').val();
	var siope = jQuery('[name="siope"]').val();
	
	var action  = '&requisicao=atualizaPendenciaView&tipo=siope&inuid='+inuid+'&cae='+cae+'&par='+par+'&cacs='+cacs+'&pne='+pne+'&habilita='+habilita+'&monitoramento='+monitoramento+'&siope='+siope;
	
   jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: true,
        success: function (resp) {
        	jQuery('.text-pendencia').html('Pendências Atualizadas com Sucesso!');
            //jQuery("#div_debug").html(resp);
        }
    });
}


jQuery(".btn-limpar").click(function () {
    window.location.href = 'par3.php?modulo=principal/orcamento/listaPagamentos&acao=A';
});

function fecharPendencia(){
	window.location.reload();
}

jQuery(".btn-pesquisar").click(function () {
    $btn = jQuery(this).button('loading');
    jQuery('[name="requisicao"]').val('filtrar');
    jQuery('[name="form-pesquisa-pagamento"]').submit();
});

jQuery(".btn-excel").click(function () {
/**/
    swal("Selecione o tipo de relatório:", {
      buttons: {
        cancel: "Cancelar",
        catch: {
          text: "Detalhado",
          value: "detalhado",
        },
        defeat: {
          text: "Sintético",
          value: "sintetico",
        },
      },
    })
    .then((value) => {
        switch(value){
            case 'detalhado':
                $('#tipodetalhado').val('sim');
                break;
            case 'sintetico':
                $('#tipodetalhado').val('nao');
                break;
            default:
                return;
        }
        //$btn = jQuery(this).button('loading');
        jQuery('[name="requisicao"]').val('excel');
        jQuery('[name="form-pesquisa-pagamento"]').submit();
    });    
});

jQuery('#btn-imprimir').click(function ()
{
	jQuery('#requisicao').val('imprimir-listapagamento');
    var action = jQuery('#form-pesquisa-pagamento').serialize();
    jQuery.ajax({
        type		: "POST",
        url			: window.location.href,
        data		: action,
        dataType	: 'html',
        async		: true,
        success: function (resp)
        {
        	jQuery("#impressao-content").html(resp);
            var container = jQuery("#impressao-content");
            var table = container.find('table');
            var th = container.find('th');
            var td = container.find('td');
            table.css('width', '100%')
            table.css('border-collapse', 'collapse');
            container.find(".popover,button,.modal").remove();
            container.find("table,td,th").css('border', '1px solid black');
            container.find('tr:even').css('background-color', 'silver');
            th.css('height', '50px');
            td.css('text-align', 'center');
            var w = window.open();
            w.document.write(jQuery('#div-impressao').html());
            w.document.title = 'Lista de Termos';
            w.print();
            w.close();
            jQuery("#impressao-content").html("");
        }
    });
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

$('[name=tipo_processo]').change(function () {
    let osValidada = $('#div_os_validada');
    if ($(this).val() == 'O') {
        osValidada.show();
    }else{
        osValidada.hide();
    }
});
</script>
<script src='../zimec/public/temas/simec/js/plugins/sweetalert/swal.js'></script>
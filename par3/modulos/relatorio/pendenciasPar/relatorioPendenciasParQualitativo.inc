<?php

switch ($_REQUEST['requisicao']) {
//     case 'buscar':
//         ob_clean();
//         retornaRelatorio($_REQUEST);
//         die();
//     break;
    case 'buscar_xls':
        ob_clean();
        retornaRelatorio($_REQUEST, true);
        die();
    break;
    case 'carregaMunicipios':
        $municipio = new Territorios_Model_Municipio();
        $response = $municipio->lista(['muncod', 'mundescricao'], ["estuf = '{$_REQUEST['estuf']}'"]);
        print simec_json_encode($response);
        die;
    break;
    case 'detalharPendenciasObras':
        $relatorio = new Par3_Model_RelatorioPendenciaPAR();
        $relatorio->listaPendenciasParObras($_POST['dados'][0]);
        die;
    break;
    case 'detalharPendenciasMonitoramento':
        $relatorio = new Par3_Model_RelatorioPendenciaPAR();
        $relatorio->detalharPendenciasMonitoramento($_POST['dados'][0]);
        die;
    break;
    case 'detalhesPendenciasMonitoramento':
        listarPendencias($_REQUEST);
        die;
    break;
    case 'detalharPendencias':
        detalharPendencias($_REQUEST);
        die;
    break;
}
switch ($_REQUEST['req']) {
    case 'detalharPendencia':
        ob_clean(); 
        $helperPendencia = new Par3_Helper_PendenciasEntidade();
        $helperPendencia->detalharPendencia($_POST);
        die;
    break;
}

function listarPendencias($arrPost = array()){
    $dados = explode("-",$arrPost['dados'][0]); 
    //$arrPost = array('inuid' => $dados[0]);
    $_REQUEST['relatorioListado'] = $dados[2];
    $_REQUEST['pendencia'] = $dados[1];
    $_REQUEST['inuid'] = $dados[0];

    $pastaPendencias = 'par3/modulos/principal/planoTrabalho/pendencias/monitoramento/';
    include APPRAIZ.$pastaPendencias.'RelatorioMonitoramento'.'.php';
    //$listarPendencia = new Par3_Controller_ListarPendenciaPorRelatorio();
    //$listarPendencia->setRelatorio($arrPost); 
}

function detalharPendencias($pendencia){ 
    $_REQUEST['pendencia'] = $pendencia['type'];
    $_REQUEST['inuid'] = $pendencia['inuid'];
    $helperPendencia = new Par3_Helper_PendenciasEntidade();
    $helperPendencia->carregarWidgetPrincipal($pendencia['type']); 
}

function retonaRelatorios()
{
    return array(
        'obrasPar' => 'Pendência de Obras do PAR',
        'cacs' => 'CACS',
        'habilitacao' => 'Habilitação',
        'cae' => 'CAE',
        'prestacaoContas' => 'Prestação de Contas',
        'monitoramentoPAR2011' => 'Monitoramento PAR 2011-2014 e Termos de compromisso',
        'pme' => 'PME/PEE',
        'siope' => 'SIOPE',
    );
}

function retornaRelatorio($arrPost = array(), $xls = false)
{ 
    if($arrPost['relatorio'] == 'monitoramentoPAR2011'){
        $relMonitoramento = new Par3_Controller_RelatorioMonitoramento;
         if($xls){
            $arrPost['req'] = 'xls';
            $relMonitoramento->listaMonitoramento($arrPost);
        }
        $relMonitoramento->listaMonitoramento($arrPost);
    }else{

        $relPendenciaPAR = new Par3_Model_RelatorioPendenciaPAR();
        $relPendenciaPAR->setRelatorio($arrPost);
        $arrRelatorios = retonaRelatorios();
    
        if($xls){
            $relPendenciaPAR->buildDataGrid($xls);
            exit;
        }
    }
?>
<div id="div_cabecalho" class="row col-lg-offset-2" style="display:none;">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Relatório <?php echo $arrRelatorios[$relPendenciaPAR->relatorio]?> <br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?=$_SESSION['usunome'];?></b><br/>
                    Hora da Impressão: <?=date( 'd/m/Y - H:i:s' );?><br />
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>
    <div class="clearfix" style="margin-bottom:10px;"></div>
    <?php
        if( $relPendenciaPAR->showGrid ){
            $relPendenciaPAR->buildDataGrid();
        }else{
    ?>
            <div class="alert alert-info" role="alert">
                <p class="text-center">Selecione o relatório que deseja ver.</p>
            </div>
    <?php
        }
}

require APPRAIZ . 'includes/cabecalho.inc';
?>
<style type="text/css">
    .fa-file-excel-o{
        font-size:15px;
        color: #fff;
        cursor: pointer;
    }
    .fa-file-excel-o:hover{
        color: #fff;
        -webkit-box-shadow: 0px 0px 10px 0px rgba(0,0,0,0.5);
        -moz-box-shadow:    0px 0px 10px 0px rgba(0,0,0,0.5);
        box-shadow:         0px 0px 10px 0px rgba(0,0,0,0.5);
    }
    .footer{
        bottom: -38px;
    }

    #wrapper{
       width: 103%;
    }

    @media print{
        .notprint{
            display: none
        }
        .div_rolagem{
            display: none
        }
        .div_rol{
            display: none
        }
    }

    @media screen {
        .notscreen { display: none; }
        .div_rol{display: none;}
    }
    .div_rolagem{
        overflow-x: auto; overflow-y: auto; height: 50px;
    }
    .div_rol{
        overflow-x: auto; overflow-y: auto; height: 50px;
    }
</style>
<form method="post" name="formulario" id="formulario" class="form form-horizontal notprint">
    <input type="hidden" name="requisicao" id="requisicao" value="buscar">
    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-10" >
                    <h3 id="entidade">Relatórios de Pendências do PAR - Qualitativos</h3>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <?php
                $relatorio = simec_htmlentities($_REQUEST['relatorio'] );
                $arrRelatorios = retonaRelatorios();
                $arrAttr = array('data-placeholder' => 'Relatórios', 'required');
                echo $simec->select('relatorio', 'Relatório', $relatorio, $arrRelatorios, $arrAttr);

                $pendencia = simec_htmlentities($_REQUEST['pendencia'] );
                $pendencia = $pendencia ? $pendencia : 'T';
                $options = array('P'=>'Possui', 'N' => 'Não Possui', 'T'=>'Todos');
                $arrAttr = array('data-placeholder' => 'Pendencia', 'required');
                echo $simec->radio('pendencia', 'Pendência', $pendencia, $options, array(), array('style'=>''));
                
                $mEstadoDocumento = new Workflow_Model_EstadoDocumento();
                $rsEsd = $mEstadoDocumento->getArrayEstadoDocumentoPAR3();
                $arrAttr = array (
                    'data-placeholder' => 'Situação'
                );
                echo $simec->select ( 'esdid','Situação', $_REQUEST['esdid'], $rsEsd, $arrAttr );

                $esfera = simec_htmlentities($_REQUEST['esfera']);
                $sql = 'SELECT \'E\' AS codigo, \'Estadual\' AS descricao UNION SELECT \'M\' AS codigo, \'Municipal\' AS descricao';
                $arrAttr = array('data-placeholder' => 'Esfera', 'required');
                echo $simec->select('esfera', 'Esfera', $esfera, $sql, $arrAttr);

                $sql = Territorios_Model_Estado::pegarSQLSelect($_POST);
                $arrAttr = array('data-placeholder' => 'Unidade Federativa');
                echo $simec->select('estuf', 'UF', $_REQUEST['estuf'], $sql, $arrAttr);
            ?>
            <div class="row" id="linha_muncod" style="<?php echo ($esfera=='E'?'display:none':'')?>">
                <div class="col-lg-3 text-right"><label for="muncod">Município:</label></div>
                <div class="col-lg-9">
                    <?php
                        $muncod = simec_htmlentities($_REQUEST['muncod']);
                        $arrAttr = ['data-placeholder' => 'Municipio'];
                        $arrConfig = array('input-size' => '12');
                        echo $simec->select('muncod', null, $muncod, array(), $arrAttr, $arrConfig);
                    ?>
                </div>
            </div>
            <div class="ibox-footer">
                <div class="row">
                    <div class="col-lg-5 col-lg-offset-4">
                        <button type="button" class="btn btn-success buscar" style="width:110px;"><i class="fa fa-search"></i> Filtrar</button>
                        <button type="button" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> XLS</button>
                        <button type="button" class="btn btn-warning" style="width:110px;" onclick="javascript:$('.buscar').click(); $('#div_cabecalho').show(); window.print(); $('#div_cabecalho').hide(); "><i class="glyphicon glyphicon-print"></i> Imprimir </button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</form>
<div class="ibox-content" style="width:98%; height:auto; overflow-x:auto; overflow-y:auto;" id="div_conteudo">
	<?php 
	if($_REQUEST['requisicao'] == 'buscar'){
	    retornaRelatorio($_REQUEST);
	}else{
	?>
    <div id="div_cabecalho" class="row col-lg-offset-2" style="display:none;">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Relatório <?php echo $arrRelatorios[$relatorio]?> <br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?=$_SESSION['usunome'];?></b><br/>
                    Hora da Impressão: <?=date( 'd/m/Y - H:i:s' );?><br />
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>

    <div class="clearfix" style="margin-bottom:10px;"></div>

    <?php
        if( $relatorioNutricionista->showGrid ){
            $relatorioNutricionista->buildDataGrid();
        }else{
    ?>
            <div class="alert alert-info" role="alert">
                <p class="text-center">Selecione o relatório que deseja ver.</p>
            </div>
    <?php
        }
	}
    ?>
</div>
<script type="text/javascript">
    $(function(){

        $(".xls").on("click", function(){
            $("#requisicao").val("buscar_xls");
            window.open(window.location.href+'&'+$("#formulario").serialize());
        });

        $(".buscar").on("click", function(){
            $("#requisicao").val("buscar");
            if($("#formulario").valid()){
            	window.location.href = window.location.href+"&"+$("#formulario").serialize();
//             	jQuery.ajax({
//                     type: "POST",
//                     url: window.location.href,
//                     data: "&"+$("#formulario").serialize(),
//                     async: false,
//                     beforeSend: function () {
//                         $('#loading').show();
//                     },
//                     success: function(result) {
//                         $('#div_conteudo').html(result);
//                     }
//                 });
            }
        });

        var muncod_hid = jQuery('[name="muncod_hid"]').val();

        jQuery('#muncod').append(new Option('', ''));

        if (jQuery('[name="estuf"]').val() != '') {
            carregarMunicipio( jQuery('[name="estuf"]').val(), muncod_hid );
        }

        $('#esfera').change(function(){
            if($(this).val()=='E'){
                $('#linha_muncod').hide();
            }else{
                $('#linha_muncod').show();
            }
        });

        $('[name="estuf"]').change(function(){
            if ( jQuery('[name="muncod"]').val() != 'E' ) {
                var estuf = jQuery(this).val();
                carregarMunicipio( estuf, muncod_hid );
            }
        });

    });

    function carregarMunicipio( estuf, muncod_hid ) {
        if ( estuf != '' ) {
            var options = jQuery('#muncod');
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=carregaMunicipios&estuf="+estuf,
                async: false,
                success: function(result) {
                    options.empty();
                    options.append(new Option('', ''));
                    var result = JSON.parse(result);
                    $.each(result, function() {
                        options.append(new Option(this.mundescricao, this.muncod));
                    });
                    options.focus();
                    if (muncod_hid) {
                        options.val(muncod_hid);
                    }
                    options.trigger('chosen:updated');
                }
            });
        }
    }
</script>
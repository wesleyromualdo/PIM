<?php
checkParamInuid();
$original = false;
$modelUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

if (!$arrPermissao || !$arrPermissao['booVisualizar']) {
    simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
}
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();

$escopoPendencias = $controleUnidade->pegarEscopoPendencias();
$escopoAlertas = $controleUnidade->pegarEscopoAlertas();

function ajusteEstatico($itens, $helper, $tipo = null){
    $deParaId = [
        1 => 'cae',
        2 => 'cacs',
        3 => 'habilitacao',
        4 => 'monitoramento',
        5 => 'par',
        6 => 'pne',
        7 => 'contas',
        8 => 'siope',
    ];
    $deParaMetodo = [
        'par'=> 'existePendenciaPeloTipoPar',
        'habilitacao'=> 'consultaHabilitaEntidade',
        'cacs'=> 'consultaPendenciaCACS',
        'cae'=> 'carregaPendenciaConselhoCAE',
        'contas'=> 'carregaPendenciaContas',
        'monitoramento'=> 'consultaPendenciaMonitoramentoPAR',
        'siope'=> 'carregaPendenciaSiope',
        'pne'=> 'consultaArquivoPNE',
    ];
    $arRes = [
        'ok'=>[],
        'fail'=>[]
    ];
    if($itens){
        foreach($itens as $item){
            if(IS_PRODUCAO && ($item['id'] == 3)){
                continue;
            }
            $pendencia = $deParaId[$item['id']];
            $metodo = $deParaMetodo[$pendencia];
            $res = $helper->controleWidgetsPendencia($pendencia, $helper->{$metodo}($_REQUEST['inuid']));
            if($tipo && $res['boolean']){
                $res['thumb'] = 'fa-times-circle';
            }
            $arRes[$res['boolean'] ? 'fail' : 'ok'][] = $res;
        }
    }
    return $arRes;
}
$colecaoPendencias = ajusteEstatico($escopoPendencias, $helperPendencia, 'pendencia');
$colecaoAlertas = ajusteEstatico($escopoAlertas, $helperPendencia);

$arStatus = $modelUnidade->atualizarPendencia($_REQUEST['inuid']);
if($arStatus && isset($arStatus['dtcarga'])){
    preg_match('/(\d{4})\-(\d{2})\-(\d{2})(.*)\.\d*/', $arStatus['dtcarga'], $m);
    $dataUltimaAtualizacao = "{$m[3]}/{$m[2]}/{$m[1]}{$m[4]}";
    preg_match('/(\d{4})\-(\d{2})\-(\d{2}) (\d+):(\d+):(\d+)\.\d*/', $arStatus['dtcarga'], $p);
    $maisDeDozeHoras = (mktime(date('H'), date('i'), date('s'), date('n'), date('j'), date('Y')) - 
    mktime((integer)$p[4],(integer)$p[5],(integer)$p[6],(integer)$p[2],(integer)$p[3],(integer)$p[1])) > 43200;
}else{
    $dataUltimaAtualizacao = '';
    $maisDeDozeHoras = false;
}
if( $_REQUEST['acao'] == 'download'){

    $helperPendencia->recuperaArquivoPNE( $_REQUEST['inuid'] );

}

switch($_REQUEST['req']){
    case 'detalharPendencia':
        ob_clean();
        $helperPendencia->detalharPendencia($_POST);
        die();
        break;
}

require APPRAIZ.'includes/cabecalho.inc';

if( isset($_REQUEST['pendencia']) ) {
    ob_clean();
    $helperPendencia->carregarWidgetPrincipal($_REQUEST['pendencia']);
    die();
}
?>
<style>
    .processo_detalhe{
        color:blue;
        cursor:pointer;
    }
    .modal-dialog{
        margin-left: 0px;
        margin-right: 0px;
    }
    span.list-group-item.active,
span.list-group-item.active:hover,
span.list-group-item.active:focus {
  /*
  background-color: #1ab394;
  border-color: #1ab394;
  color: #FFFFFF;
  */
  background-color: #E8E8E8;
  border-color: #BEBEBE;
  color: #696969;
  z-index: 2;
}
</style>
<script language="javascript" src="js/documentoLegado.js"></script>
<div class="align-center">
    <div class="ibox float-e-margins animated modal" id="modal-visualiza-pendencia" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
        <div class="modal-dialog modal-lg">
            <div class="ibox-title" style="background-color: #1c84c6; border-color: #1c84c6; color: #ffffff; text-align: center">
                <h1 class="center" style="font-weight: bold;">Pendências</h1>
            </div>
            <div class="ibox-content" id="div_pendencia">
                <div id="div_debug"></div>
                <input type="hidden" name="inuid" value="">
                <input type="hidden" name="cae" value="">
                <input type="hidden" name="par" value="">
                <input type="hidden" name="cacs" value="">
                <input type="hidden" name="pne" value="">
                <input type="hidden" name="habilita" value="">
                <input type="hidden" name="monitoramento" value="">
                <input type="hidden" name="siope" value="">
                <input type="hidden" name="siope_desc" value="">
                <input type="hidden" name="contas" value="">
                <div class="text-pendencia" style="font-size: 20px; font-weight: bold;">
                    wesley
                </div>
                <div class="progress">
                    <div class="progress-bar" role="progressbar" aria-valuenow="1" aria-valuemin="0" aria-valuemax="100" style="width: 1%; font-size: 20px; font-weight: bold;">0%</div>
                </div>
            </div>
            <div class="ibox-footer" style="text-align: center;">
                <button type="button" id="btn-fechar" data-dismiss="modal" class="btn btn-warning" onclick="fecharPendencia();" ><i class="fa fa-times-circle-o"></i> Fechar</button>
            </div>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
            </div>
            <div class="ibox-content">
                <input type="hidden" name="pendencia" id="pendencia" value="<?php echo $_REQUEST['pendencia']?>"/>
                <?php $controleUnidade->cabecalhoUnidade(); ?>
                <?php $arBoxPendencia = [0 => array_merge($colecaoAlertas['ok'] , $colecaoPendencias['ok']), 1=> $colecaoAlertas['fail'], 2 => $colecaoPendencias['fail']]; ?>
                <?php 
                    if($original){
                        $arBoxPendencia = $helperPendencia->controlePendenciasBox(); 
                    }
                ?>
                <div class="ibox-content-round-gray ibox-content">
                    <div class="ibox">
                        <?php if($db->testa_superuser() || $maisDeDozeHoras): ?> 
                            <button class="btn btn-primary js-atualizar-pendencias">Atualizar Pendências</button>
                            <div class="js-ultima-atualizacao">
                                <label>Data da última atualização:</label>
                                <span ><?php echo $dataUltimaAtualizacao; ?></span>
                            </div>
                        <?php endif; ?> 
                        <?php
                        if($original){
                            $arBoxPendencia[1] = ($arBoxPendencia[1] ? $arBoxPendencia[1] : array());
                            foreach( $arBoxPendencia[1] as $id=>$arrPendencias) {
                                if($arrPendencias['type'] != 'cae' && $arrPendencias['type'] != 'contas' && $arrPendencias['type'] != 'monitoramento'){
                                    $arBoxPendencia[1][$id]['thumb'] = 'fa-times-circle';
                                    $arBoxPendencia[2][] = $arBoxPendencia[1][$id];
                                    unset($arBoxPendencia[1][$id]);
                                }
                            }
                            foreach($arBoxPendencia[0] as $idx => $alerta){
                                if($alerta['boolean']){
                                    $arBoxPendencia[1][] = $alerta;
                                    unset($arBoxPendencia[0][$idx]);
                                }
                            }
                        }
                        krsort ($arBoxPendencia);
                        foreach( $arBoxPendencia as $widgetCampo=>$wid) {
                            if (count($wid)>=1){
                        ?>
                            <div class="ibox-title">
                                <?php

                                switch ($widgetCampo){
                                    case 2: $strPendencia = 'Pendências do PAR'; break;
                                    case 1: $strPendencia = 'Alertas!'; break;
                                    case 0: $strPendencia = 'Parabéns!'; break;
                                    default: $strPendencia = 'Não informado';
                                }
                                ?>
                                <h3><?php echo  $strPendencia.' ('.count($wid).')'; ?></h3>
                            </div>
                            <div class="ibox-content">
                                <div class="row">
                                    <?php foreach( $wid as $widget ) { ?>
                                        <?php $activeBox = ""; ?>
                                        <?php if( $widget['type'] == $_REQUEST['pendencia']) $activeBox = "active"; ?>
                                        <div class="col-lg-4">
                                            <div class="box-pendencia <?php echo $activeBox;?> widget style1 yellow-bg" style="background-color: <?php echo $widget['widget'];?>" type="<?php echo $widget['type']?>">
                                                <div class="row vertical-align">
                                                    <div class="col-xs-2">
                                                        <i class="fa <?php echo $widget['icon'];?>  fa-5x"></i>
                                                    </div>
                                                    <div class="col-xs-10 text-right">
                                                        <span><?php echo $widget['description'];?></span>
                                                        <h4><i class="fa <?php echo $widget['thumb'];?> fa-3x"></i></h4>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    <? } ?>
                                </div>
                            </div>
                        <? }
                        } ?>
                        <div class="ibox">
                            <div class="ibox-content">
                                <div class="row">
                                    <div class="col-lg-12 header-pendencias" style="display: none;">
                                        <h3>Detalhes</h3>
                                    </div>
                                    <div class="list-pendencias"></div>
                                </div>
                                <div class="ibox-footer" style="padding-left: 0px; margin-right: -14px">
                                    <div class="row">
                                        <div class="col-lg-6 text-left">
                                            <button type="button" class="btn btn-success previous" >Anterior</button>
                                        </div>
                                        <div class="col-lg-6 text-right">
                                            <button type="button" class="btn btn-success next" >Próximo</button>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div id="modal-form" class="modal fade" aria-hidden="true" style="width:100%">
    <center>
        <div class="modal-dialog">
            <div id="html_modal-form" class="ibox-content"></div>
        </div>
    </center>
</div>
<script>
    $(document).ready(function()
    {
        $('.box-pendencia').click(function() {
            var pendencia = '<?php echo $_REQUEST['pendencia']; ?>';
            var type = $(this).attr('type');

            if( $(this).hasClass('active') ) {
                $(this).removeClass('active');
                $('.'+type).html('');
                return;
            }
            $(this).addClass('active');

            jQuery.ajax({
                type: "POST",
                data: 'pendencia=' +type,
                success: function(resp)
                {
                    
                    var $titulo  = $('.header-pendencias');
                    var $listaPendencias = $('.list-pendencias');
                    $titulo.show();
                    try {
                        var content = $.parseJSON(resp);
                        $listaPendencias.append(tmplWidget(content));

                        if(content.nomeEntidade) {
                            var pendencia = $('.'+type).find('.pendencia-content');
                            pendencia.append("<p><h3>"+content.nomeEntidade+"</h3></p>");
                            pendencia.append(tmplDefault(content));
                            
                        }
                        if(content.type == 'par' && content.municipios) {
                            var list = $('.list-pendencias-content');
                            $.each(content.municipios, function (municipio, pendencias){
                                list.append("<p><h3 class='pendencia-par' style='cursor: pointer;'>"+ municipio +"  <i class='fa fa-plus-circle'></i></h3></p>");

                                $.each(pendencias.pendencias, function (index, pendencia) {
                                    list.append(tmplParPendencias(pendencia));
                                });
                            });
                        }
                    } catch(e) {
                        $listaPendencias.append(resp);
                    }

                }
            });

        });

        $('#pneArquivo').click(function(){
            var pendencia = '<?php echo $_REQUEST['pendencia']; ?>';
            var url	= 'par3.php?modulo=principal/planoTrabalho/pne&acao=A' + '&inuid=<?php echo $_REQUEST['inuid']?>&pendencia='+pendencia+'&acao=download';
            $(location).attr('href',url);
        });

        $('.next').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });

        $('.previous').click(function(){
            window.location.href = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>';
        });

        $(document).on('click', '.pendencia-par', function(event) {
            event.preventDefault();
            var list = $(this).nextUntil('.pendencia-par', '.list-group-item');
            var icon = $(this).children('i');

            if(list.css('display') == 'none') {
                list.show();
                icon.addClass('fa-minus-circle');
            } else {
                list.hide();
                icon.removeClass('fa-minus-circle');
                icon.addClass('fa-plus-circle');
            }
        });
        
       
        $(document).on('click', '.abrir-pendencias', function() {
            var list  = $(this).closest('.par').find('.list-group-item');
            var icons = $('.list-pendencias-content i');

            if(list.css('display') == 'none') {
                list.show();
                $(this).text(' Esconder todas as pendências');
                $(this).addClass('fa-minus-circle');
                icons.addClass('fa-minus-circle');
            } else {
                list.hide();
                $(this).text(' Mostrar todas as pendências');
                $(this).removeClass('fa-minus-circle');
                $(this).addClass('fa-plus-circle');
                icons.removeClass('fa-minus-circle');
                icons.addClass('fa-plus-circle');
            }
        });

        setTimeout(jQueryShowBtnProxReturn, 1000); //desabilita o btn próximo do menu quando entram diretamente no planejamento

    });

    function tmplWidget(content){
        return "<div class='col-lg-12 "+content.type+"'>" +
            "<div class='widget yellow-bg text-left' style='background-color: "+content.widget+";'>" +
            "<div class='slim-scroll'>" +
            "<i class='fa "+content.thumb+" fa-2x'></i> " +
            "<span class='font-bold no-margins' style='font-size: 16pt;'>"+ content.description +" </span>" +
            tmplContainer(content) +
            "</div>" +
            "</div>" +
            "</div>";
    }

    function tmplDefault(content) {

    	var html = "<div class='list-group' id='list_pc'>";
    	
        if(content.pendenciasPrestacaoContas){
            
        	$.each(content.pendenciasPrestacaoContas, function (index, pendencia) {

        		html += "<span class='list-group-item active'>" +
        		"<h4 class='list-group-item-heading'style='color: #f90404;'>Processo " + pendencia.tipo + "</h4>" +
                "<h4 class='list-group-item-heading'>Processo: "+ pendencia.processo +" - N° Documento: "+ pendencia.documento +"</h4>" +
                "<p class='list-group-item-text'>Motivo: "+ pendencia.situacao +"</p>" +
                "</span>";
            });
        }else{
        	html += "<a class='list-group-item active' href='#'>" +
            "<h4 class='list-group-item-heading'>"+content.msg+"</h4>"+
            "</a>";
        }
        html += "</div>";

        return html;
    }

    function tmplParPendencias(pendencia) {
        return "<a class='list-group-item active' style='display: none;' href='#'>" +
            "<h4 class='list-group-item-heading'>ID Obras2: "+ pendencia.obrid +" - "+ pendencia.predescricao +"</h4>" +
            "<p class='list-group-item-text'>"+ pendencia.situacao +"</p>" +
            "</a>";
    }

    function tmplContainer(content) {
        var container = "<div class='pendencia-content'></div>";

        if(content.type == 'par'  && content.municipios){
            container = "<i class='fa fa-plus-circle abrir-pendencias' style='cursor:pointer;'> Exibir todas as pendências</i><div class='list-pendencias-content'></div>";
        }

        return container;
    }
</script>
<script>
    $(document).ready(function(){
        $('.js-atualizar-pendencias').on('click',function(){
            atualizaPendencias(<?php echo $_REQUEST['inuid']; ?>);
        });
    });
    
    
    function atualizaPendencias( inuid ){
        $("#modal-visualiza-pendencia").modal("show");

        jQuery('[name="inuid"]').val(inuid);
        jQuery('.text-pendencia').html('Atualizando Pendência - Conselho CAE');

        var action  = '&requisicao=atualizaPendencias&tipo=cae&inuid='+inuid;
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
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
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
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
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
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
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
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
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
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
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
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
                jQuery('.text-pendencia').html('Atualizando Pendência - Contas');
                jQuery('.progress-bar').html('78%');
                jQuery('.progress-bar').css('width', '78%');
                setTimeout(pendenciaContas,1000);
            }
        });
    }

    function pendenciaContas(){
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=contas&inuid='+inuid;

        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="contas"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - SIOPE');
                jQuery('.progress-bar').html('84%');
                jQuery('.progress-bar').css('width', '84%');
                setTimeout(pendenciaSiope,1000);
            }
        });
    }

    function pendenciaSiope(){
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=siope&inuid='+inuid;

       jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            dataType: "json",
            success: function (resp) {
//                jQuery("#div_debug").html(resp);console.log(resp.desc);return false;
                jQuery('[name="siope_desc"]').val(resp.desc);
                jQuery('[name="siope"]').val(resp.cod);
                jQuery('.text-pendencia').html('Atualizando View de Pendências');
                jQuery('.progress-bar').html('100%');
                jQuery('.progress-bar').css('width', '100%');
                setTimeout(atualizaPendenciaView,1000);
            }
        });
    }

    function atualizaPendenciaView(){
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var cae = jQuery('[name="cae"]').val();
        var par = jQuery('[name="par"]').val();
        var cacs = jQuery('[name="cacs"]').val();
        var pne = jQuery('[name="pne"]').val();
        var habilita = jQuery('[name="habilita"]').val();
        var monitoramento = jQuery('[name="monitoramento"]').val();
        var siope = jQuery('[name="siope"]').val();
        var siope_desc = jQuery('[name="siope_desc"]').val();
        var contas = jQuery('[name="contas"]').val();

        var action  = '&requisicao=atualizaPendenciaView&tipo=siope&inuid='+inuid+'&cae='+cae+'&par='+par+'&cacs='+cacs+'&pne='+pne+'&habilita='+habilita+
            '&monitoramento='+monitoramento+'&siope='+siope+'&siope_desc='+siope_desc+'&contas='+contas;

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
    function fecharPendencia(){
        window.location.reload();
    }
</script>
<style>
.modal {
  text-align: center;
}

@media screen and (min-width: 768px) { 
  .modal:before {
    display: inline-block;
    vertical-align: middle;
    content: " ";
    height: 100%;
  }
}

.modal-dialog {
  display: inline-block;
  text-align: left;
  vertical-align: middle;
}
</style>
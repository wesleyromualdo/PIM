<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 26/08/2015
 * Time: 15:16
 */
global $db;

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
require_once APPRAIZ . "includes/library/simec/Listagem.php";

include APPRAIZ . 'includes/cabecalho.inc';

$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'Monitoramento PNE';


$perfis = arrayPerfil();

if ($_POST['acao']) {
    switch ($_POST['acao']) {
        case 'carrega_legenda':
            ob_clean();
            echo getLegendaAcompanhamentoPne($_POST['subid']);
            exit;
        case 'carrega_tabela':
            ob_clean();
            $selapresentacao = $_POST['selapresentacao'];

            carregaDadosMonitoramentoPNE();
            exit;
        case 'carrega_tabela_xls':
            ob_clean();

            header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
            header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
            header("Pragma: no-cache");
            header("Content-type: application/xls; name=simec_sase_monitoramentopne_" . date("Ymdhis") . ".xls");
            header("Content-Disposition: attachment; filename=simec_sase_monitoramentopne_" . date("Ymdhis") . ".xls");
            header("Content-Description: MID Gera excel");

            $post = $_POST;

            $selapresentacao = $post['selapresentacao'];
            $post['excel'] = true;

            carregaDadosMonitoramentoPNE(true);
            exit;

        case 'carrega_dados_meta':
            ob_clean();
            $metid = $_POST['metid'];
            $sql = "select metchamada, mettitulo from sase.meta where metid = {$metid}";
            $res = $db->carregar($sql);
            echo simec_json_encode($res[0]);
            exit;

        case 'carrega_dados_indicador':
            ob_clean();
            $subid = $_POST['subid'];
            $sql = "select subtitulo from sase.submetadial where subiddial = {$subid} and substatus = 'A'";
            $res = $db->carregar($sql);
            echo $res[0]['subtitulo'];
            exit;
    }
}
// Filtra estados pelo tipo da organização
if ($_GET['filtroTipoOrgaoEstado'] && isset($_GET['tipoOrgao'])) {
      ob_clean();
    ?>
              <select name="estuf[]" id="estuf" class="form-control" multiple="multiple" disabled
                                            onchange="javascript:filtraMunicipios();">
                                        <?php
                                        $cpf = $_SESSION['usucpf'];
                                        if (in_array( PFLCOD_SASE_SUPERVISOR_GERAL ,$perfis) || in_array( PFLCOD_SASE_EXECUTIVO ,$perfis)){
                                            $sql = "SELECT DISTINCT est.estuf, est.estdescricao  
                                                    FROM territorios.estado est
                                                      INNER JOIN sase.usuarioresponsabilidade resp ON
                                                          resp.estuf = est.estuf
                                                      WHERE
                                                      resp.pflcod IN (".PFLCOD_SASE_SUPERVISOR_GERAL.",".PFLCOD_SASE_EXECUTIVO.")
                                                    AND rpustatus = 'A'
                                                    AND usucpf = '{$cpf}'
                                                    ORDER BY est.estdescricao";

                                        }else if (in_array( PFLCOD_SASE_TECNICO ,$perfis)){
                                            $sql = "SELECT DISTINCT est.estuf, est.estdescricao  
                                                    FROM territorios.estado est
                                                    INNER JOIN sase.usuarioresponsabilidade resp ON
                                                      substr(resp.muncod,0,3)  = est.estcod
                                                    WHERE
                                                      resp.pflcod = ".PFLCOD_SASE_TECNICO."
                                                      AND rpustatus = 'A'
                                                      AND usucpf = '{$cpf}' 
                                                      ORDER BY est.estdescricao";

                                        }else if (in_array( PFLCOD_SASE_ADMINISTRADOR ,$perfis) || in_array( PFLCOD_SASE_SUPER_USUARIO ,$perfis)){
                                            $sql = "SELECT e.estuf, estdescricao FROM territorios.estado e
                                                     INNER JOIN sase.orgaoestado oe ON oe.estuf = e.estuf
                                                     WHERE oe.orgid = {$_GET['tipoOrgao']}
                                                     ORDER BY estdescricao ASC";
                                        }
//dbg($sql,1);
                                        $estados = $db->carregar($sql);
                                        foreach ($estados as $chave => $estado) { ?>
                                            <option <?= ($estado['estuf'] == $_POST['estuf']) ? 'selected' : '' ?>
                                                    value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                                        <?php } ?>
                                    </select>

    <?php
    die();
}

// Filtra os municipios
if ($_GET['filtroAjax'] && isset($_GET['estuf'])) {
    ob_clean();
    $cpf = $_SESSION['usucpf'];
    if (in_array( PFLCOD_SASE_SUPERVISOR_GERAL ,$perfis) || in_array( PFLCOD_SASE_EXECUTIVO ,$perfis)){
        $sql = "SELECT  mun.muncod, mundescricao
                FROM territorios.municipio mun 
                    INNER JOIN sase.usuarioresponsabilidade resp
                ON resp.estuf = mun.estuf
                WHERE
                  resp.pflcod IN (".PFLCOD_SASE_SUPERVISOR_GERAL.",".PFLCOD_SASE_EXECUTIVO.")
                AND rpustatus = 'A'
                AND usucpf = '{$cpf}'
                ORDER BY mundescricao";

    }else if (in_array( PFLCOD_SASE_TECNICO ,$perfis)){
        $sql = "SELECT  mun.muncod, mundescricao
                FROM territorios.municipio mun 
                    INNER JOIN sase.usuarioresponsabilidade resp 
                    ON resp.muncod  = mun.muncod
                WHERE resp.pflcod = ".PFLCOD_SASE_TECNICO."
                AND rpustatus = 'A'
                AND usucpf = '{$cpf}' 
                ORDER BY mundescricao";

    }else if (in_array( PFLCOD_SASE_ADMINISTRADOR ,$perfis) || in_array( PFLCOD_SASE_SUPER_USUARIO ,$perfis)){
        if ($_GET['estuf'] && $_GET['estuf'] != '0'){
            $sql = "SELECT DISTINCT mun.muncod, mundescricao 
                FROM territorios.municipio mun 
                WHERE mun.estuf IN ('" . implode("','", $_GET['estuf']) . "')
                ORDER BY mundescricao";
        }
    }

    $municipios = $db->carregar($sql);
    ?>
    <select name="muncod[]" class="form-control" id="muncod" multiple="multiple">
        <?php
        foreach ($municipios as $chave => $municipio) { ?>
            <option value="<?= $municipio['muncod'] ?>"><?= $municipio['mundescricao'] ?></option>
        <?php } ?>
    </select>
    <?php die();
}

// Filtra os indicadores
if ($_GET['filtroAjax'] && $_REQUEST['metid']) {
    ob_clean();
    $indicadores =
    $sql = "SELECT subiddial as codigo, subtitulo as descricao FROM sase.submetadial WHERE substatus = 'A' ";
    if ($_REQUEST['metid']) {
        $sql .= " and metid = '" . $_REQUEST['metid'] . "' ";
    }
    $sql .= " order by subtitulo asc ";
    $indicadores = $db->carregar($sql);
    echo '<select name="subid" class="form-control" id="subid" onchange="carregaDadosIndicador();">
        <option value="">Filtrar indicador:</option>';
    foreach ($indicadores as $chave => $indicador) {
        echo '<option value="' . $indicador[codigo] . '">' . $indicador[descricao] . '</option>';
    }
    echo '</select>';
    die();
}
?>

<script>
    function filtraMunicipios() {
        estuf = '0';
        var est = jQuery('#estuf').val();
        if (Array.isArray(est)) {
            if (est[0] == "") {
                if (est.length > 1) {
                    est.shift();
                    estuf = est;
                }
            } else {
                estuf = est;
            }
        }

        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, estuf: estuf},
            success: function (resposta) {
                jQuery('#divFiltroMunicipio .inputarea').html(resposta);
                // jQuery('#muncod').multiselect('enable');
                jQuery('#muncod').multiselect({
                    numberDisplayed: 1,
                    includeSelectAllOption: true,
                    enableCaseInsensitiveFiltering:true,
                    enableFiltering: true,
                    maxHeight: 300,
                    id: 'muncod'
                });
            }
        });
    }

    function filtraIndicadores(metid) {

        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, metid: metid},
            success: function (resposta) {
                jQuery('#divFiltroIndicador .inputarea').html(resposta);
                jQuery('#todosMuncod').removeAttr('disabled');
            }
        });
    }

    function habilitarEstado() {
        jQuery('#estuf').multiselect('enable');
        jQuery('#estuf').multiselect({
            numberDisplayed: 1,
            enableCaseInsensitiveFiltering:true,
            includeSelectAllOption: true,
            enableFiltering: true,
            maxHeight: 300,
            id: 'estuf'
        });
    }
    
        function habilitarTipoOrganizacao() {
          
             
        if(jQuery('#selapresentacao').val() != ''){
          jQuery('#filtroOrgao').val(0);
          jQuery('#filtroOrgao').attr('disabled',false);

        }else{
          jQuery('#filtroOrgao').attr('disabled',true);
          jQuery('#filtroOrgao').val(0);
         habilitarTipoOrganizacaoEstado();
        }
       
    }
    
    function habilitarTipoOrganizacaoEstado(){
     
           tipoOrgao = jQuery('#filtroOrgao').val();
   
           jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroTipoOrgaoEstado: true, tipoOrgao: tipoOrgao},
            success: function (resposta) {
               // alert(resposta);
                jQuery('#divFiltroEstado .inputarea').html(resposta);
                habilitarEstado();
            }
        });
        
    }

    function carregaLegenda() {
        subid = jQuery('#subid').val();
        jQuery.ajax({
            utl: '',
            type: 'POST',
            data: {acao: 'carrega_legenda', subid: subid},
            success: function (data) {
                jQuery('#divLegenda').html(data);
            }
        });
    }

    function controlaMunicipio() {
        var valor = $('#selapresentacao').val();
        if (valor != 'M') {
            $('#divFiltroMunicipio').hide();
        } else {
            $('#divFiltroMunicipio').show();
        }
    }

    function carregaMapa() {
        var valor = $('#selapresentacao').val();
        var subid = $('#subid').val();
        var tipo = '';
        if (subid != '') {
            switch (valor) {
                case 'M':
                default:
                    var tipo = 'acompanhamento-pne-municipio';
                    break;
                case 'E':
                    var tipo = 'acompanhamento-pne-estado';
                    break;
            }
            Mapas.buscaEstadoForm($("#estuf"), tipo);
        } else {
            alert('Informe o indicador.');
            return;
        }
    }

    function carregarTabela(todos, pagina) {
        
        if(jQuery('#selapresentacao').val() == 'E'){
            jQuery('#muncod').val('');
        }
        var mun = jQuery('#muncod').val();
        var est = jQuery('#estuf').val();
        var selapresentacao = jQuery('#selapresentacao').val();
        var pneciclo = jQuery('#pneciclo').val();
        var filtroOrgao = jQuery('#filtroOrgao').val();
        subid = jQuery('#subid').val();
        selapresentacao = jQuery('#selapresentacao').val();

        todos = typeof todos === 'undefined' ? false : true;
        pagina = typeof pagina === 'undefined' ? false : pagina;
        muncod = '0';
        estuf = '0';

        if (Array.isArray(mun) && mun[0] == "") {
            if (mun.length > 1) {
                mun.shift();
                muncod = mun;
            }
        } else {
            muncod = mun;
        }

        if (est[0] == "") {
            if (est.length > 1) {
                est.shift();
                estuf = est;
            }
        } else {
            estuf = est;
        }

        metid = jQuery('#metid').val();
  
        var dados = {
            acao: 'carrega_tabela',
            subid: subid,
            muncod: muncod,
            estuf: estuf,
            metid: metid,
            pneciclo: pneciclo,
            filtroOrgao:filtroOrgao,
            selapresentacao: selapresentacao
        };

        if (pagina) {
            dados['listagem[p]'] = pagina;
        }

        if (subid == '') {
            alert('Selecione o indicador.');
            return;
        }

        var teste = jQuery.ajax({
            url: '',
            type: 'POST',
            data: dados,
            success: function (dados) {
                var teste2 =  dados;
                jQuery('#divTabela').html(dados);
                jQuery('#tb_render').removeAttr('class');
                resizeTable();
                carregaLegenda();
            }
        });

        return teste;
    }

    function carregaScrollbar() {
        $('#tb_render').find('tbody').css('height', '200px');
        $('#tb_render').find('tbody').scroll();
    }

    function toggleSelect(sel) {
        if (multiselect_selected(sel)) {
            //sel.multiselect('deselect_all');
            multiselect_deselectAll(sel);
//            $btn.text("Selecionar Todos");
        } else {
//            sel.multiselect('selectAll', false);
            multiselect_selectAll(sel);
//            $btn.text("Desselecionar Todos");
        }
    }

    function carretaDadosMeta() {
        metid = jQuery('#metid').val();
        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {
                acao: 'carrega_dados_meta',
                metid: metid
            },
            success: function (data) {
                var d = JSON.parse(data);
                jQuery('#divTituloMeta').html('Meta ' + d['metchamada']);
                jQuery('#divMeta').html(d['mettitulo']);
            }
        });
    }

    function carregaDadosIndicador() {
        subid = jQuery('#subid').val();

        if (subid == "") {
            return false;
        }
        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {
                acao: 'carrega_dados_indicador',
                subid: subid
            },
            success: function (data) {
                jQuery('#divTituloIndicador').html(data);
                jQuery('#divTituloIndicador').show();
            }
        });
    }

    function exportarXls() {
        jQuery('[name=formMapa]').removeAttr('action');
        jQuery('[name=formMapa]').removeAttr('target');


        jQuery('[name=acao]').val('carrega_tabela_xls');
        jQuery('[name=formMapa]').submit();
    }

</script>

<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css">
<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?key=AIzaSyCxb-cN91hx2XcCaBS3v-STxxEUfe0LT9I"></script>
<!--<script type="text/javascript" src="http://maps.google.com/maps/api/js?key=AIzaSyCr5ZzGX-6FUww4KMVMUb4UBM4p5o0eDVU"></script>-->

<!-- http://hpneo.github.io/gmaps/ -->
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/sase/js/Mapas.js"></script>
<script type="text/javascript" src="http://freakyjolly.com/demo/lib/jspdf.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jspdf-autotable/2.3.0/jspdf.plugin.autotable.js"></script>
<!-- http://www.downscripts.com/bootstrap-multiselect_javascript-script.html -->
<!--<script src="/sase/js/jspdf.min.js"></script>-->
<!--<script src="/sase/js/jspdf.plugin.autotable.js"></script>-->
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/functions.js"></script>
<!-- /dependencias -->
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<!--<script src="/sase/js/loader.js"></script>-->

<script type="text/javascript">
    /*google.charts.load("current", {packages:['corechart']});
     google.charts.setOnLoadCallback(drawChart);
     function drawChart() {

     var data = google.visualization.arrayToDataTable([
     ['Element', 'Density', { role: 'style' }],
     ['Copper', 8.94, '#b87333', ],
     ['Silver', 10.49, 'silver'],
     ['Gold', 19.30, 'gold'],
     ['Platinum', 21.45, 'color: #e5e4e2' ]
     ]);

     var options = {
     title: "Density of Precious Metals, in g/cm^3",
     bar: {groupWidth: '95%'},
     legend: 'none',
     };

     var chart_div = document.getElementById('chart_div');
     var chart = new google.visualization.ColumnChart(chart_div);

     // Wait for the chart to finish drawing before calling the getImageURI() method.
     google.visualization.events.addListener(chart, 'ready', function () {
     chart_div.innerHTML = '<img src="' + chart.getImageURI() + '">';
     console.log(chart_div.innerHTML);
     });

     chart.draw(data, options);

     }*/
</script>
<style>
    td, th {
        height: 10px;
        padding: 0px !important;
    }

    td {
        background-color: #FFFFFF;
    }

    #divMapa {
        margin-bottom: 0px !important;
    }

    #map_canvas {
        border: none !important;
        height: 500px !important;
    }

    #legendaMapa {
        padding-top: 0px !important;
    }

    #map_canvastxt {
        float: none;
    }

    #divLegenda {
        margin-top: 10px;
    }

    .txts {
        position: inherit !important;
        float: right !important;
        background-color: transparent !important;
    }

    .btn-todos {
        margin-bottom: 3px;
        padding: 0px 12px !important;
    }

    #local-mapa {
        width: 100% !important;
    }

    /* Tabela */
    #tb_render {
        border-spacing: 0;
        border: 2px solid black;
    }

    #tb_render tbody,
    #tb_render thead {
        display: block;
    }

    #tb_render thead tr th {
        height: 30px;
        line-height: 30px;
    }

    #tb_render tbody {
        height: 100px;
        overflow-y: auto;
        overflow-x: hidden;
    }

    #tb_render tbody {
        border-top: 2px solid black;
    }

    #tb_render tbody td, #tb_render thead th {
        border-right: 1px solid black;
    }

    #tb_render tbody td:last-child, #tb_render thead th:last-child {
        border-right: none;
    }

    #directionsPanel {
        width: 100%;
        margin-bottom: 10px;
    }

    #divTituloMeta {
        margin-bottom: 5px;
        font-size: 19px;
        font-weight: bold;
        color: #1f6b9f;
    }

    #divMeta {
        font-size: 14px;
        font-weight: bold;
    }

    #divTituloIndicador {
        display: none;
        margin-bottom: 0px !important;
        border-bottom-right-radius: 0px !important;
        border-bottom-left-radius: 0px !important;
        padding-top: 10px !important;
        padding-bottom: 10px !important;
        font-weight: bold;
    }

    /*Exemplo*/
    table.scroll {
        width: 100%;
    }

    table.scroll tbody,
    table.scroll thead {
        display: block;
    }

    thead tr th {
        height: 30px;
        line-height: 30px;
    }

    table.scroll tbody {
        height: 200px;
        width: 100%;
        overflow-y: auto;
        overflow-x: hidden;
    }

    table.scroll tbody td {
        min-width: 65px;
    }
    /*#btn-load, #btnxls{
        height: 30px;
    }*/

</style>

<div id="container">
    <div id='chart_div'></div>
    <form class="form-horizontal" name="formMapa" id="formMapa" role="form" method="POST">
        <input type="hidden" name="acao" id="acao"/>
        <input type="hidden" name="imagem" id="imagemGrafico"/>
        <div class="row">
            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <legend>Filtros</legend>

                        <div class="form-inline">
                            <div class="col-md-1">
                                <label>Meta</label>
                                <select name="metid" id="metid" class="form-control"
                                        onchange="javascript:if(this.value!=''){carretaDadosMeta(); filtraIndicadores(this.value);}">
                                    <option value="">Filtrar </option>
                                    <?php
                                    $sql = "SELECT metid as codigo, 'Meta '||metid as descricao FROM sase.meta ORDER BY metid";
                                    $estados = $db->carregar($sql);
                                    foreach ($estados as $chave => $estado) { ?>
                                        <option value="<?= $estado['codigo'] ?>"><?= $estado['descricao'] ?></option>
                                    <?php } ?>
                                </select>
                            </div>

                            <div class="col-md-2" id="divFiltroIndicador">
                                <label for="filtroMunicipio">Indicador</label>
                                <div class="inputarea">
                                    <?php $dis = '';
                                    if (!$_POST['metid']) {
                                        $dis = 'disabled';
                                    } ?>
                                    <select name="subid" <?= $dis ?> class="form-control" id="subid"
                                            onchange="carregaDadosIndicador(); habilitarEstado();">
                                        <option value="">Filtrar indicador</option>
                                    </select>
                                </div>
                            </div>

                            <div class="col-md-2" id="divAno">
                                <label>Ciclo</label>
                                <div class="inputarea">
                                    <select name="pneciclo" id="pneciclo" class="form-control">
                                        <option value="">Selecione</option>
                                        <?php
                                        $sql = "SELECT * FROM sase.pneciclo";
                                        $pneciclo= $db->carregar($sql);
                                        foreach ($pneciclo as $ciclo) { ?>
                                            <option <?= ($ciclo['descricao'] == $_POST['descricao']) ? 'selected' : '' ?>
                                                    value="<?= $ciclo['ciclo']?>"><?= $ciclo['descricao'] ?>
                                            </option>
                                        <?php } ?>
                                    </select>
                                </div>
                            </div>

                            <div class="col-md-2" id="divAno">
                                <label for="filtroMunicipio">Apresentação</label>
                                <div class="inputarea">
                                    <select name="selapresentacao" id="selapresentacao" class="form-control"  onchange="javascript:habilitarTipoOrganizacao();">
                                        <option value="">Selecione</option>
                                        <option value="M">Municipal</option>
                                        <option value="E">Estadual</option>
                                    </select>
                                </div>
                            </div>

                             <div class="col-md-2" id="filtroTipoOrganizacao">
                                <label for="filtroTipoOrganizacao">Tipo de Organização</label>
                                <div class="inputarea">
                                    <select id="filtroOrgao" name="filtroOrgao" class="multiselect form-control " onchange="javascript:habilitarTipoOrganizacaoEstado();" disabled>
                                    <option value="0">Selecione</option>
                                    <?php $sql = "SELECT orgid, orgdsc FROM sase.orgao WHERE orgstatus = 'A' ORDER BY orgid ASC";
                                    $orgaos = $db->carregar($sql);

                                    foreach ($orgaos as $key => $value) {
                                        echo "<option value='" . $value['orgid'] . "'>" . $value['orgdsc'] . "</option>";
                                    } ?>

                                </select>
                                </div>
                            </div>
                            
                            <div class="col-md-2" id="divFiltroEstado">
                                <label for="filtroMunicipio">Estado</label>
                                <div class="inputarea">
                                    <select name="estuf[]" id="estuf" class="form-control" multiple="multiple" disabled
                                            onchange="javascript:filtraMunicipios();">
                                        <?php
                                        $cpf = $_SESSION['usucpf'];
                                        if (in_array( PFLCOD_SASE_SUPERVISOR_GERAL ,$perfis) || in_array( PFLCOD_SASE_EXECUTIVO ,$perfis)){
                                            $sql = "SELECT DISTINCT est.estuf, est.estdescricao  
                                                    FROM territorios.estado est
                                                      INNER JOIN sase.usuarioresponsabilidade resp ON
                                                          resp.estuf = est.estuf
                                                      WHERE
                                                      resp.pflcod IN (".PFLCOD_SASE_SUPERVISOR_GERAL.",".PFLCOD_SASE_EXECUTIVO.")
                                                    AND rpustatus = 'A'
                                                    AND usucpf = '{$cpf}'
                                                    ORDER BY est.estdescricao";

                                        }else if (in_array( PFLCOD_SASE_TECNICO ,$perfis)){
                                            $sql = "SELECT DISTINCT est.estuf, est.estdescricao  
                                                    FROM territorios.estado est
                                                    INNER JOIN sase.usuarioresponsabilidade resp ON
                                                      substr(resp.muncod,0,3)  = est.estcod
                                                    WHERE
                                                      resp.pflcod = ".PFLCOD_SASE_TECNICO."
                                                      AND rpustatus = 'A'
                                                      AND usucpf = '{$cpf}' 
                                                      ORDER BY est.estdescricao";

                                        }else if (in_array( PFLCOD_SASE_ADMINISTRADOR ,$perfis) || in_array( PFLCOD_SASE_SUPER_USUARIO ,$perfis)){
                                            $sql = "SELECT estuf, estdescricao FROM territorios.estado ORDER BY estdescricao ASC";
                                        }

                                        $estados = $db->carregar($sql);
                                        foreach ($estados as $chave => $estado) { ?>
                                            <option <?= ($estado['estuf'] == $_POST['estuf']) ? 'selected' : '' ?>
                                                    value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                                        <?php } ?>
                                    </select>
                                </div>
                            </div>

                            <div class="col-md-2" id="divFiltroMunicipio">
                                <label for="filtroMunicipio">Município</label>
                                <div class="inputarea">
                                    <?php $dis = '';
                                    if (!$_POST['estuf']) {
                                        $dis = 'disabled';
                                    } ?>
                                    <select name="muncod[]" <?= $dis ?> class="form-control" id="muncod" multiple="multiple"></select>
                                </div>
                            </div>
                            <div class="form-inline">
                                <div class="col-lg-1" align="center">
                                    <button type="button" style="margin-top: 15px" class="btn btn-primary" id="btn-load" data-toggle="tooltip" title="Pesquisar informações" data-placement="right">
                                        <i class="fa fa-search"></i>
                                    </button>
                                </div>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-12">
                <div class="well">
                    <fieldset>
                        <legend>
                            <div id="map_canvastxt"><?= $titulo ?></div>
                        </legend>
                        <div class="form-group" id="divMapa">
                            <div class="row">
                                <div id="local-mapa" class="col-lg-12 col-sm-12">
                                    <div class="panel panel-default">
                                        <div class="panel-body">
                                            <div id="directionsPanel">
                                                <div id="divTituloMeta"></div>
                                                <div id="divMeta"></div>
                                            </div>
                                            <div id="divTituloIndicador" class="well"></div>
                                            <div id="map_canvas"></div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-lg-12 col-sm-12">
                                    <div id="divLegenda" class="col-lg-8 col-sm-8 col-lg-offset-2"></div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-lg-12 col-sm-12">
                                    <div id="divTabela" style="overflow:auto;height: 200px ">
                                    </div>
                                </div>
                                <div class="col-lg-10" style="margin-top: 20px">
                                    <button class="btn btn-primary  pull-right " type="button" title="Exportar a tabela para Excel" id="btnxls" data-toggle="tooltip" data-placement="left">
                                        <span class="glyphicon glyphicon-file"></span> Planilha xls
                                    </button>
                                </div>
                                <?php if (in_array( PFLCOD_SASE_SUPER_USUARIO ,$perfis) || in_array( PFLCOD_SASE_TECNICO ,$perfis) || in_array( PFLCOD_SASE_SUPERVISOR ,$perfis) || in_array( PFLCOD_SASE_EXECUTIVO ,$perfis)|| in_array( PFLCOD_SASE_ADMINISTRADOR ,$perfis)):?>
                                    <div class="col-lg-2" style="margin-top: 20px">
                                        <button class="btn btn-primary" type="button" onclick="gerapdf()"
                                                title="Gerar PDF de Análise" data-toggle="tooltip" data-placement="right" id="btnpdf">Gerar PDF de Análise
                                        </button>
                                    </div>
                                <?php endif;?>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>
        </div>

    </form>
</div>
<script>
    jQuery('documento').ready(function () {
        Mapas.inicializar('#map_canvas');
        jQuery('#estuf').multiselect({
            includeSelectAllOption: true,
            enableCaseInsensitiveFiltering:true,
            enableFiltering: true,
            maxHeight: 300,
            id: 'estuf'
        });
        jQuery('#muncod').multiselect({
            numberDisplayed: 1,
            includeSelectAllOption: true,
            enableCaseInsensitiveFiltering:true,
            enableFiltering: true,
            id: 'muncod'
        });

        $('#btnxls').hide();
        $('#btnpdf').hide();
        $('#selapresentacao').change(function () {
            controlaMunicipio();
        });

        $("#btn-load").click(function () {
            if ($('#metid').val() == ''){
                alert('Selecione a meta');
                return;
            }

            if ($('#subid').val() == ''){
                alert('Selecione o Indicador');
                return;
            }

            if ($('#pneciclo').val() == ''){
                alert('Selecione o ciclo');
                return;
            }

            if ($('#estuf').val() == null){
                alert('Selecione o Estado');
                return;
            }

            if ($('#selapresentacao').val() == 'M' && $('#muncod').val() == null){
                alert('Selecione ao menos um município');
                return;
            }

            carregaMapa();
            carregarTabela();
            $('#btnxls').show();
            $('#btnpdf').show();
        });

        $("#btnxls").click(function () {
            if ($("#metid").val() == "") {
                alert("Selecione a meta");
                return;
            }
            if ($("#subid").val() == "") {
                alert("Selecione a submeta");
                return;
            }
            exportarXls();
        });

        // Change the selector if needed
        var $table = $('table.scroll'),
            $bodyCells = $table.find('tbody tr:first').children(),
            colWidth;

// Adjust the width of thead cells when window resizes
        $(window).resize(function () {
            // Get the tbody columns width array
            colWidth = $bodyCells.map(function () {
                return $(this).width();
            }).get();

            // Set the width of thead columns
            $table.find('thead tr').children().each(function (i, v) {
                $(v).width(colWidth[i]);
            });
        }).resize(); // Trigger resize handler
    });

    function resizeTable() {
        var $table = $('table.scroll'),
            $bodyCells = $table.find('tbody tr').children(),
            colWidth;

// Adjust the width of thead cells when window resizes
        $(window).resize(function () {
            // Get the tbody columns width array
            colWidth = $bodyCells.map(function () {
                return $(this).width();
            }).get();
            //console.log(colWidth);

            // Set the width of thead columns
            $table.find('thead tr').children().each(function (i, v) {
                $(v).width(colWidth[i]);
            });
        }).resize();
    }


    function gerapdf() {
        if ($('#subid').val() == ''){
           alert('Selecione o Indicador');
           return;
        }

        var selapresentacao = jQuery('[name=selapresentacao]').val();
        if(selapresentacao == 'E'){
            jQuery('[name=formMapa]').attr('action', 'sase.php?modulo=principal/acompanhamentopne/grafico&acao=A')
            jQuery('[name=formMapa]').attr('target', '_blank')
        }else{
            jQuery('[name=formMapa]').attr('action', 'sase.php?modulo=principal/acompanhamentopne/graficomunicipio&acao=A')
            jQuery('[name=formMapa]').attr('target', '_blank')
        }



        $('#imagemGrafico').val($('#chart_div').children('img').attr('src'));
        jQuery('[name=acao]').val('gera_pdf');
        jQuery('[name=formMapa]').submit();
    }

    delegatePaginacao = function () {
        $('body').on('click', '.container-listing li[class="pgd-item"]:not(".disabled")', function () {
            // -- definindo a nova página
            var novaPagina = $(this).attr('data-pagina');
            // -- Submetendo o formulário
            carregarTabela(true, novaPagina);
        });
    };

    $('[data-toggle="tooltip"]').tooltip();
</script>
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

$pneano = 2010;

$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'Acompanhamento PNE - Estado';

if($_POST['acao']){
    switch($_POST['acao']){
        case 'carrega_legenda':
            ob_clean();
            echo getLegendaAcompanhamentoPne($_POST['subid']);
            exit;

        case 'carrega_tabela':
            ob_clean();
            $cor = '';
//            ver('Teste', d);
            $arrAnos = array(2015, 2025);
            $subid  = $_POST['subid'];
            $metid  = $_POST['metid'];
            $estuf  = $_POST['estuf'];
            $pneano = $_POST['pneano'] ? $_POST['pneano'] : $pneano;

            $sql = <<<DML
                select legcor, legfxa1, legfxa2 from sase.legendaindicadores where subid = {$subid}
DML;
            $cores = $db->carregar($sql);

            $where = array();
            if($estuf != '' && $estuf != '0') { $where[]  = " est.estuf in ('".implode("','", $estuf)."') "; }

            $where = is_array($where) && count($where) > 0 ? 'and '.implode('and', $where) : '';
            $html = "";

            $sql = <<<DML
                with temp_cor as (select legcor, legfxa1, legfxa2 from sase.legendaindicadores where subid = {$subid})

                select
                    est.estuf,
                    est.estdescricao,
                    pneano,
                    pnevalor,
                    pnevalormeta,
                    (select legcor from temp_cor where pnevalor between legfxa1 and legfxa2) as cor,
                    (select legcor from temp_cor where pnevalormeta between legfxa1 and legfxa2) as corMeta
                from sase.pnedial pne
                inner join territorios.estado est on est.estuf = pne.estuf
                left join sase.pneprevdial ppd on ppd.pneiddial = pne.pneiddial
                where subiddial = {$subid}
                and pnetipo = 'E'
                {$where}
                order by est.estdescricao, pneano
DML;



            $res = $db->carregar($sql);
            $dados = array();
            if($res) {

                $dados = array();
                $estado = "";
                $municipio = "";

                $paeano = $db->carregar("select paeano from sase.pneanoexibicaodial where subiddial = {$subid} and paetipo = 'E'");
                $pneano = $paeano[0]['paeano'];

                function retornaCor($v, $k, $i){
                    global $cor;
                    if($i != '') {
                        if ($v['legfxa1'] <= $i && $v['legfxa2'] >= $i) {
                            $cor = $v['legcor'];
                        }
                    } else {
                        $cor = '#FFFFFF';
                    }
                    return;
                };
                $coresEscuras = array(
                    '#3333FF',
                    '#006600'
                );
                $cabecalho = array();
                foreach ($res as $r) {
                    if(!isset($dados[$r['estuf']])){
                        $dados[$r['estuf']] = array(
                            'Estado' => $r['estdescricao']
                        );
                        $a = $arrAnos[0];
                        $dados[$r['estuf']][$pneano] = round($r['pnevalor'], 2);
                        array_walk($cores, 'retornaCor', $r['pnevalor']);
                        while ($a <= $arrAnos[1]) {
                            $dados[$r['estuf']][$a] = '';
                            $a++;
                        }
                    }
                    if(isset($dados[$r['estuf']][$r['pneano']])) {
                        if($r['pneano'] == $pneano){
                            if($r['pnevalor'] != ''){
                                $dados[$r['estuf']][$r['pneano']] = round($r['pnevalor'], 2);
                            } else {
                                $dados[$r['estuf']][$r['pneano']] = '';
                            }
                        } else {
                            if($r['pnevalormeta'] != ''){
                                $dados[$r['estuf']][$r['pneano']] = round($r['pnevalormeta'], 2);
                            } else {
                                $dados[$r['estuf']][$r['pneano']] = '';
                            }
                        }
                        if($r['pneano'] >= 2015){
                            $dados[$r['estuf']]['legcor'] = $r['cormeta'];
                        } else {
                            $dados[$r['estuf']]['legcor'] = $r['cor'];
                        }
                        $color = '';
                        if(in_array($dados[$r['estuf']]['legcor'], $coresEscuras)){
                            $color = 'color: #FFFFFF;';
                        }
                        $dados[$r['estuf']][$r['pneano']] = '<div class="divCor" style="background: '.$dados[$r['estuf']]['legcor'].'; '.$color.'">'.$dados[$r['estuf']][$r['pneano']].'</div>';
                    }
                    if(count($cabecalho) == 0){
                        $cabecalho = array_keys($dados[$r['estuf']]);
                    }
                }

                $c = array(
                    $cabecalho[0],
                    $cabecalho[1]
                );

                array_shift($cabecalho);
                array_shift($cabecalho);
                array_pop($cabecalho);
                $c['Situação'] = $cabecalho;

                $lista = new Simec_Listagem();
                $lista
                    ->setDados($dados)
                    ->setCabecalho($c)
                    ->esconderColunas(array('legcor'))
                    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            }
            exit;
    }
}

// Filtra os municipios
if( $_GET['filtroAjax'] && isset($_GET['estuf']) ){
    ob_clean();
    $sql = "select muncod, mundescricao from territorios.municipio";

    if ($_GET['estuf'] && $_GET['estuf'] != '0'){
        if(is_array($_GET['estuf'])) {
            $sql .= " where estuf in ('" . implode("','", $_GET['estuf']) . "') ";
        }
    }
    $sql .= " order by mundescricao asc ";
    $municipios = $db->carregar($sql);
    ?>
    <select name="muncod" class="form-control" id="muncod" multiple="multiple" onchange="carregarTabela()">
        <?php
        foreach( $municipios as $chave => $municipio ){ ?>
            <option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
        <?php }?>
    </select>
    <?php die();}


// Filtra os indicadores


if( $_GET['filtroAjax'] && $_REQUEST['metid'] ){
    ob_clean(); $indicadores =
    $sql = "select subid as codigo, subtitulo as descricao From sase.submeta";
    if( $_REQUEST['metid'] ) $sql .= " where metid = '" . $_REQUEST['metid'] . "' ";
    $sql .= " order by subtitulo asc ";
    $indicadores = $db->carregar($sql);
    ?>

    <select name="subid" class="form-control" id="subid" onchange="carregaLegenda(); carregaMapa(); carregarTabela();">
        <option value="">Filtrar indicador:</option>
        <?php
        foreach( $indicadores as $chave => $indicador ){ ?>
            <option value="<?=$indicador['codigo']?>"><?=$indicador['descricao']?></option>
        <?php }?>
    </select>
    <?php die();}

?>
<script>
    function filtraIndicadores(metid){
        jQuery.ajax({
            url:'',
            type:'GET',
            data:{filtroAjax:true,metid:metid},
            success: function( resposta ){
                jQuery('#divFiltroIndicador .inputarea').html( resposta );
                jQuery('#estuf').multiselect('enable');
                jQuery('#estuf').multiselect({
                    numberDisplayed: 1,
                    id: 'estuf'
                });
                jQuery('#todosMuncod').removeAttr('disabled');
            }
        });
    }

    function carregaLegenda(){
        subid = jQuery('#subid').val();
        jQuery.ajax({
            utl: '',
            type: 'POST',
            data: {acao: 'carrega_legenda', subid:subid},
            success: function(data){
                jQuery('#legendaMapa').html(data);
                jQuery("#estuf").removeAttr('disabled');
                jQuery('#estuf').multiselect('enable');
                jQuery('#estuf').multiselect({
                    numberDisplayed: 1,
                    id: 'estuf'
                });
                jQuery('#todosEstuf').removeAttr('disabled');
            }
        });
    }

    function carregaMapa(){
        Mapas.buscaEstadoForm( jQuery("#estuf"), 'acompanhamento-pne-estado' );
    }

    function carregarTabela(todos, pagina){
        var est = jQuery('#estuf').val();
        subid  = jQuery('#subid').val();
        todos = typeof todos === 'undefined' ? false : true;
        pagina = typeof pagina === 'undefined' ? false : pagina;
        muncod = '0';
        estuf = '0';
        if(est != null) {
            if (Array.isArray(est)) {

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
                    estuf: estuf,
                    metid: metid
                };

                if (pagina) {
                    dados['listagem[p]'] = pagina;
                }

                jQuery.ajax({
                    url: '',
                    type: 'POST',
                    data: dados,
                    success: function (dados) {
                        jQuery('#divTabela').html(dados);
                    }
                });
            }
        }
    }

    function toggleSelect(sel){
        if (multiselect_selected(sel)) {
            multiselect_deselectAll(sel);
        } else {
            multiselect_selectAll(sel);
        }
    }

</script>

<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?key=AIzaSyCxb-cN91hx2XcCaBS3v-STxxEUfe0LT9I"></script>
<!-- http://hpneo.github.io/gmaps/ -->
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/sase/js/Mapas.js"></script>
<!-- http://www.downscripts.com/bootstrap-multiselect_javascript-script.html -->
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/functions.js"></script>
<!-- /dependencias -->

<style>
    .table th, td{
        padding: 0px !important;
        text-align: center !important;
        vertical-align: middle !important;
    }
    .table th{
        border-top: none !important;
    }
    .table table{
        width: 100% !important;
    }
    #divMapa{
        margin-bottom: 0px !important;
    }
    #map_canvas{
        height: 540px !important;
    }
    #legendaMapa{
        padding-top: 0px !important;
    }
    #map_canvastxt{
        float: none;
    }
    .txts{
        position: inherit !important;
        float: right !important;
        background-color: transparent !important;
    }
    .btn-todos{
        margin-bottom: 3px;
        padding: 0px 12px !important;
    }
    .btn-primary{
        height: 21px;
        width: 51px;
        padding: 0 !important;
    }
</style>

<div id="container">
    <form class="form-horizontal" name="formMapa" id="formMapa" role="form" method="POST">
        <input type="hidden" name="acao" id="acao"/>

        <div class="row">
            <div class="col-lg-2">
                <div class="well">
                    <fieldset>
                        <legend>Filtros</legend>

                        <div class="form-group">
                            <label for="filtroEstado" class="col-lg-12">Ano Previsto:</label>
                            <div class="col-lg-12">
                                <select name="pneano" id="pneano" class="form-control" onchange="carregaMapa()">
                                    <option value="2013">2013</option>
                                    <?php
                                    $anos = array(2015, 2025);
                                    $a = $anos[0];
                                    while($a <= $anos[1]){
                                        $sel = $dadosMun['anoprevisto'] == $a ? 'selected' : '';
                                        echo "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                        $a++;
                                    }?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="filtroEstado" class="col-lg-12">Meta:</label>
                            <div class="col-lg-12">
                                <select name="metid" id="metid" class="form-control" onchange="javascript:if(this.value!=''){filtraIndicadores(this.value);}">
                                    <option value="">Filtrar meta</option>
                                    <?php
                                    $sql = "select metid as codigo, 'Meta '||metid as descricao from sase.meta order by metid";
                                    $estados = $db->carregar($sql);
                                    foreach( $estados as $chave => $estado ){ ?>
                                        <option value="<?=$estado['codigo']?>"><?=$estado['descricao']?></option>
                                    <?php }?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group" id="divFiltroIndicador">
                            <label for="filtroMunicipio" class="col-lg-12">Indicador:</label>
                            <div class="col-lg-12 inputarea">
                                <?php $dis = ''; if(!$_POST['metid']){ $dis = 'disabled'; } ?>
                                <select name="subid" <?= $dis ?> class="form-control" id="subid" onchange="carregaLegenda(); carregaMapa(); carregarTabela();">
                                    <option value="">Filtrar indicador</option>
                                </select>
                            </div>
                        </div>

                        <div class="form-group" id="divFiltroEstado">
                            <label for="filtroEstado" class="col-lg-12">
                                Estado:
                                <input type="button" value="Todos" onclick="toggleSelect(jQuery('#estuf')); carregaMapa(); carregarTabela();" class="btn btn-primary"/>
                            </label>
                            <div class="col-lg-12" style="margin-top: 5px;">
                                <select name="estuf" id="estuf" class="form-control" multiple="multiple" disabled onchange="javascript:carregaMapa(); carregarTabela();">
                                    <?php
                                    $sql = "select estuf, estdescricao from territorios.estado order by estdescricao ASC";
                                    $estados = $db->carregar($sql);
                                    foreach( $estados as $chave => $estado ){ ?>
                                        <option <?=($estado['estuf']==$_POST['estuf'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                                    <?php }?>
                                </select>
                            </div>
                        </div>

                    </fieldset>
                </div>
            </div>

            <div class="col-lg-10">
                <div class="well">
                    <fieldset>
                        <legend><div id="map_canvastxt"><?= $titulo ?></div></legend>
                        <div class="form-group" id="divMapa">
                            <div class="row">
                                <div id="local-mapa" class="col-lg-10 col-sm-10">
                                    <div class="panel panel-default">
                                        <div class="panel-body">
                                            <div id="directionsPanel" style="width: 300px"></div>
                                            <div id="map_canvas"></div>
                                        </div>
                                    </div>
                                </div>
                                <div class="col-lg-2 col-sm-2 panel panel-default">
                                    <fieldset>
                                        <legend>Legenda</legend>
                                        <div id="legendaMapa">

                                        </div>
                                    </fieldset>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-lg-12 col-sm-12">
                                    <?php
                                    $anos = array(2015, 2025);
                                    ?>
                                    <div class="panel panel-default" id="divTabela">
                                    </div>
                                </div>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>
        </div>

    </form>
</div>
<script>
    jQuery('documento').ready(function(){
        Mapas.inicializar( '#map_canvas' );
        jQuery('#estuf').multiselect({
            numberDisplayed: 1,
            id: 'estuf'
        });
    });

    delegatePaginacao = function()
    {
        $('body').on('click', '.container-listing li[class="pgd-item"]:not(".disabled")', function(){
            // -- definindo a nova página
            var novaPagina = $(this).attr('data-pagina');
            // -- Submetendo o formulário
            carregarTabela(true, novaPagina);
        });
    };
</script>
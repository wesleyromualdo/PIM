<script type="text/javascript" src="http://maps.google.com/maps/api/js?key=AIzaSyCxb-cN91hx2XcCaBS3v-STxxEUfe0LT9I"></script>
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/Mapas.js"></script>
<script src="/sase/js/functions.js"></script>
<?php

//municipios
if( $_GET['filtroAjaxMunicipio'] ){

    $ufs = implode(',',$_GET['filtroEstado']);
    $ufs = str_replace(",","','", $ufs);

    $sql = "SELECT mun.muncod, mun.estuf, mun.mundescricao
            FROM territorios.municipio mun
            WHERE mun.estuf IN ('{$ufs}')";


    $municipios = $db->carregar($sql);
    ob_clean(); ?>
    <select name="muncod" id="muncod"   class="form-control"  multiple="multiple">
        <?php
        foreach( $municipios as $chave => $municipio ){ ?>

            <option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
        <?php }?>
    </select>
    <?php die();
}



?>
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
                        <div class="form-group">
                            <label class="col-sm-3 control-label">Tipo de Organização:</label>
                            <div id="divOrgao" class="col-sm-9">
                                <select id="filtroOrgao" name="filtroOrgao" class="multiselect form-control ">
                                    <option value="0">Selecione</option>
                                    <?php $sql = "SELECT orgid, orgdsc FROM sase.orgao WHERE orgstatus = 'A' ORDER BY orgid ASC";
                                    $orgaos = $db->carregar($sql);

                                    foreach ($orgaos as $key => $value) {
                                        echo "<option value='" . $value['orgid'] . "'>" . $value['orgdsc'] . "</option>";
                                    } ?>

                                </select>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-3 control-label">UF:</label>
                            <div class="col-sm-9">
                                <select id="filtroEstado" name="filtroEstado" multiple="multiple" class="multiselect form-control filtroEstado">
                                    <?php $organizacoesTerritoriais->getOptionEstado(); ?>
                                </select>
                            </div>
                        </div>
                    <div style="text-align: right">
                        <button type="button" id="btPesquisarOrgTer" class="btn btn-primary"><i
                                    class="glyphicon glyphicon-search"></i> Pesquisar
                        </button>
                        <button class="btn btn-primary" type="button" onclick="gerapdf()"
                                title="Gerar PDF de Análise" id="btnpdf">Gerar PDF
                        </button>
                    </div>
                </fieldset>
            </div>
        </div>
    </div>

    <hr>

    <div class="row">
        <div class="col-lg-8">
            <div id="map_canvastxt" style="font-size:14px;heigth:90px;"></div>
            <div class="panel panel-default">
                <div class="panel-body">
                    <div id="map_canvas"></div>
                </div>
            </div>
        </div>
        <div class="col-lg-4">
            <div class="panel panel-default">
                <div class="panel-header">
                    <div id="legendaMapa" style="width: 100%;padding-top:5px">
                        <h5>Legenda: </h5>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script>

    function filtraMunicipioss(estuf) {

        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {
                filtroAjaxMunicipio: true,
                filtroEstado: estuf
            },
            success: function (resposta) {

                jQuery('#divFiltroMunicipio').html(resposta);
                jQuery('#muncod').multiselect({
                    numberDisplayed: 1,
                    includeSelectAllOption: true,
                    enableCaseInsensitiveFiltering: true,
                    enableFiltering: true,
                    maxHeight: 300,
                    width:480,

                    id: 'muncod'
                });
            }
        });
    }

    function filtrar() {
        if($('#filtroMunicipio').val()){
            $('#muncod').val();
        }else{
            $('#muncod').val('null');
        }

        if ($('#filtroOrgao').val() != null ) {
            Mapas.buscaEstadoForm($('#filtroEstado'), 'organizacoesterritoriais');
            Mapas.atualizaLegenda($("#filtroEstado"), 'organizacoesterritoriais-legenda');
        }

    }

    function gerapdf() {

        if ($('#filtroOrgao').val() == 0){
                alert('Por favor selecione um Tipo de Organização ');
                return false;
        }
        if ($('#filtroEstado').val() === null){
                alert('Por favor selecione a UF ');
                return false;
        }
        filtrar();
        var filtroOrgao  = jQuery('[name=filtroOrgao]').val();
        var estados = '';
        $('.multiselect-container li.active > a > label > input').each( function(idx,el){
            if(el.value != 'multiselect-all'){
                if(estados == ''){
                    estados = el.value;
                }else{
                    estados += el.value+',';
                }
            }
        } );
        setTimeout(function(){
            jQuery('[name=formMapa]').attr('action', '/sase/mapa.php')
            jQuery('[name=formMapa]').attr('target', '_blank')
            jQuery('#imagemGrafico').val($('#chart_div').children('img').attr('src'));
            jQuery('[name=acao]').val('gera_pdf');
            jQuery('[name=formMapa]').submit();
        }, 5000);
    }
    
    $('documento').ready(function () {
        $('#filtroEstado').multiselect({
            numberDisplayed: 14,
            includeSelectAllOption: true,
            enableCaseInsensitiveFiltering: true,
            id: 'filtroEstado'
        }).on('change', function () {
            var uf = $(this).val();

            filtraMunicipioss(uf);
        });

        $('#mundo').multiselect({
            numberDisplayed: 14,
            includeSelectAllOption: true,
            enableCaseInsensitiveFiltering: true,
            id: 'filtroMunicipio'
        }).on('change', function () {
        });

        $('#btPesquisarOrgTer').on('click', function () {
            if ($('#filtroOrgao').val() == 0){
                alert('Por favor selecione um Tipo de Organização ');
                return false;
            }
            filtrar();
        });
        Mapas.inicializar('#map_canvas');
    });
</script>
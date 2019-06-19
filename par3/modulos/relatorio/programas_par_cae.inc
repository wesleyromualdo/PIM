<?php
$relatorioCAE = new Par3_Model_RelatorioCAE();
if (count($_POST['sfiltro'])) {
    $relatorioCAE->setWhere($_POST['sfiltro']);
}

if ($_POST['requisicao'] == 'xls') {
    $relatorioCAE->buildDataGrid($xlsDownload = true);
    exit;
}

switch ($_REQUEST['requisicao']) {
    case 'carregaMunicipios':
        $municipio = new Territorios_Model_Municipio();
        $response = $municipio->lista(['muncod', 'mundescricao'], ["estuf = '{$_REQUEST['estuf']}'"]);
        print simec_json_encode($response);
        die;
        break;
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
</style>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="muncod_hid" value="<?php echo $_POST['sfiltro']['mundescricao'];?>">
    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-10" >
                    <h3 id="entidade">Programas do PAR - Conselho de Alimentação Escolar</h3>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-3 text-right"><label for="estuf">UF</label></div>
                <div class="col-lg-9 text-center">
                <?php
                    $sql = Territorios_Model_Estado::pegarSQLSelect($_POST);
                    $arrAttr = ['data-placeholder' => 'Unidade Federativa'];
                    echo $simec->select('sfiltro[estuf]', null, $_POST['sfiltro']['estuf'], $sql, $arrAttr);
                ?>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-3 text-right"><label for="mundescricao">Município</label></div>
                <div class="col-lg-9 text-center">
                <?php
                    $mundescricao = simec_htmlentities($_POST['sfiltro']['mundescricao']);
                    $arrAttr = ['placeHolder' => 'Municipio'];
                    echo $simec->select('sfiltro[mundescricao]', null, $mundescricao, $arrAttr);
                ?>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-3 text-right"><label for="itrid">Esfera</label></div>
                <div class="col-lg-9 text-center">
                <?php
                    $itrid = simec_htmlentities($_POST['sfiltro']['itrid']);
                    $sql = 'SELECT 3 AS codigo, \'Estadual\' AS descricao UNION SELECT 2 AS codigo, \'Municipal\' AS descricao';
                    $arrAttr = ['data-placeholder' => 'Esfera'];
                    echo $simec->select('sfiltro[itrid]', null, $itrid, $sql, $arrAttr);
                ?>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-3 text-right">Cargo</div>
                <div class="col-lg-9 text-center">
                    <?php
                    $tenid = simec_htmlentities($_POST['sfiltro']['tenid']);
                    $sql = 'select tenid as codigo, tendsc as descricao from par3.tipoentidade where tenid in (13, 14)';
                    $arrAttr = ['data-placeholder' => 'Cargo'];
                    echo $simec->select('sfiltro[tenid]', null, $tenid, $sql, $arrAttr);
                    ?>
                </div>
            </div>
        </div>
        <div class="ibox-footer">
            <div class="row">
                <div class="col-lg-9 col-lg-offset-3">
                    <button type="submit" class="btn btn-success salvar" ><i class="fa fa-search"></i> Filtrar</button>
                    <button type="submit" class="btn btn-primary xls" ><i class="fa fa-file-excel-o"></i> XLS</button>
                </div>
            </div>
        </div>
    </div>
</form>

<div class="ibox-content" style="overflow: auto;">
    <?php $relatorioCAE->buildDataGrid(); ?>
</div>

<script type="text/javascript">
    $(function(){
        $(".xls").on("click", function(){
            $("#requisicao").val("xls");
            $("#formulario").submit();
        });

        $(".salvar").on("click", function(){
            $("#requisicao").val("");
            $("#formulario").submit();
        });

        var muncod_hid = jQuery('[name="muncod_hid"]').val();

        jQuery('#sfiltro-mundescricao').append(new Option('', ''));

        if (jQuery('[name="sfiltro[estuf]"]').val() != '') {
            carregarMunicipio( jQuery('[name="sfiltro[estuf]"]').val(), muncod_hid );
        }

        jQuery('[name="sfiltro[estuf]"]').change(function(){
            if ( jQuery('[name="sfiltro[mundescricao]"]').val() != 'E' ) {
                var estuf = jQuery(this).val();
                carregarMunicipio( estuf, muncod_hid );
            }
        });
    });

    function carregarMunicipio( estuf, muncod_hid ) {
        if ( estuf != '' ) {
            var options = jQuery('#sfiltro-mundescricao');
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
                        options.append(new Option(this.mundescricao, this.mundescricao));
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
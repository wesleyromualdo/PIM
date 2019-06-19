<?php
$relatorioEscolas = new Par3_Model_RelatorioEscolasPrioritarias();

if (count($_POST['sfiltro']) > 0) {
    $relatorioEscolas->setWhere($_POST['sfiltro']);
}

if ($_POST['requisicao'] == 'xls') {
    $relatorioEscolas->buildDataGrid($xlsDownload = true);
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
    <input type="hidden" name="muncod_hid" value="<?php echo $_POST['sfiltro']['ibge'];?>">
    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-10" >
                    <h3 id="entidade">Programas do PAR - Escolas Prioritárias</h3>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <div class="form-group">
                <div class="col-sm-3 col-md-3 col-lg-3 control-label text-right"><label for="estuf">UF</label></div>
                <div class="col-lg-9 text-center">
                    <?php
                    $sql = Territorios_Model_Estado::pegarSQLSelect($_POST);
                    $arrAttr = ['data-placeholder' => 'Unidade Federativa'];
                    echo $simec->select('sfiltro[estuf]', null, $_POST['sfiltro']['estuf'], $sql, $arrAttr);
                    ?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-3 col-md-3 col-lg-3 control-label text-right"><label for="mundescricao">Município</label></div>
                <div class="col-lg-9 text-center">
                    <?php
                    $mundescricao = simec_htmlentities($_POST['sfiltro']['ibge']);
                    $arrAttr = ['placeHolder' => 'Municipio'];
                    echo $simec->select('sfiltro[ibge]', null, $mundescricao, $arrAttr);
                    ?>
                </div>
            </div>
            <?php
            echo $simec->radio('sfiltro[esfera]', 'Esfera', $_POST['sfiltro']['esfera'] ? $_POST['sfiltro']['esfera'] : 'Estadual', array('Estadual' => 'Estadual', 'Municipal' => 'Municipal'));
            ?>
            <div class="form-group">
                <div class="col-sm-3 col-md-3 col-lg-3 control-label text-right"><label for="itrid">Aceite</label></div>
                <div class="col-lg-9 text-center">
                    <?php
                    $adpresposta = simec_htmlentities($_POST['sfiltro']['aceitouadesao'] );
                    $sql = 'SELECT \'S\' AS codigo, \'SIM\' AS descricao UNION SELECT \'N\' AS codigo, \'Não\' AS descricao UNION SELECT \'Pendente\' AS codigo, \'Pendente\' as descricao';
                    $arrAttr = ['data-placeholder' => 'Selecione'];
                    echo $simec->select('sfiltro[aceitouadesao]', null, $adpresposta, $sql, $arrAttr);
                    ?>
                </div>
            </div>
        </div>
        <div class="ibox-footer">
            <div class="row">
                <div class="col-lg-9 col-lg-offset-3">
                    <button type="submit" class="btn btn-success salvar" ><i class="fa fa-search"></i> Pesquisar</button>
                    <button type="submit" class="btn btn-primary xls" ><i class="fa fa-file-excel-o"></i> Exportar XLS</button>
                </div>
            </div>
        </div>
    </div>
</form>

<div class="ibox-content" style="overflow: auto;">
    <?php $relatorioEscolas->buildDataGrid(); ?>
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

        jQuery('#sfiltro-ibge').append(new Option('', ''));

        if (jQuery('[name="sfiltro[estuf]"]').val() != '') {
            carregarMunicipio( jQuery('[name="sfiltro[estuf]"]').val(), muncod_hid );
        }

        jQuery('[name="sfiltro[estuf]"]').change(function(){
            if ( jQuery('[name="sfiltro[ibge]"]').val() != 'E' ) {
                var estuf = jQuery(this).val();
                carregarMunicipio( estuf, muncod_hid );
            }
        });
    });

    function carregarMunicipio( estuf, muncod_hid ) {
        if ( estuf != '' ) {
            var options = jQuery('#sfiltro-ibge');
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
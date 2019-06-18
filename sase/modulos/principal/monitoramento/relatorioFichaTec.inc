<?php
if($_REQUEST['requisicao']){
    ob_clean();
    $dados = retornaDadosMonitoramento(false, $_REQUEST['muncod']);
    include "fichatecnica.inc";
    die;
}

//municipios
if ($_GET['filtroAjaxMunicipio']) {

//    $estuf = $_GET['filtroEstado'];

    if ($_GET['estuf'] && $_GET['estuf'] != '0'){
        $sql = "SELECT DISTINCT mun.muncod, mundescricao 
                FROM territorios.municipio mun 
                WHERE mun.estuf IN ('" . implode("','", $_GET['estuf']) . "')
                ORDER BY mundescricao";
    }

    $municipios = $db->carregar($sql);

    ob_clean(); ?>
    <select name="muncod[]" id="muncod" class="form-control" multiple="multiple">
        <?php
        foreach ($municipios as $chave => $municipio) { ?>
            <option value="<?= $municipio['muncod'] ?>"><?= $municipio['mundescricao'] ?></option>
        <?php } ?>
    </select>
    <?php die();
}
?>
    <form class="form-horizontal" name="formMonitoramento" id="formMonitoramento" role="form" method="POST"
          action="sase.php?modulo=principal/monitoramento&acao=A&aba=relatorioFichaTec"
          target="_blank"
          enctype="multipart/form-data">    
    
    <fieldset>
        <br>
        <br>
        <div class="form-group">
            <label class="col-sm-4 control-label " for="estauf">Estado: </label>
            <div class="col-sm-3">
                <select class="form-control chosen" multiple="multiple" id="estuf" name="estuf[]" class="multiselect"
                        onchange="javascript:filtraMunicipios();">
                    <?php
                    $sql = "SELECT estuf, estdescricao FROM territorios.estado ORDER BY estdescricao ASC";
                    $estados = $db->carregar($sql);
                    foreach ($estados as $chave => $estado) { ?>
                        <option <?= ($estado['estuf'] == $_POST['estuf']) ? 'selected' : '' ?>
                                value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>

        <div class="form-group">
            <label class="col-sm-4 control-label " for="estauf">Município: </label>
            <div class="col-sm-3" id="divFiltroMunicipio">
                <select id="muncod[]" name="muncod" class="form-control">
                    <option value="">Nenhum Selecionado</option>
                </select>
            </div>
        </div>

        <!-- Button -->
        <div class="form-group">
            <label class="col-sm-4 "></label>
            <div class="col-md-4">
                <button class="btn btn-primary" id="btn_geraXLS" type="button">Gerar Relatório XLS</button>
                <button class="btn btn-primary" id="btn_geraPDF" type="button">Gerar Relatório PDF</button>
            </div>
        </div>
    </fieldset>
    <input type="hidden" id="reqrelfichatec" name="requisicao">
</form>

<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script>
    $(document).ready(function () {
        $('#estuf').multiselect({
            numberDisplayed: 1,
            includeSelectAllOption: true,
            enableCaseInsensitiveFiltering: true,
            enableFiltering: true,
            id: 'estuf'
        })
    });

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
            data: {
                filtroAjaxMunicipio: true,
                estuf: estuf
            },
            success: function (resposta) {
                jQuery('#divFiltroMunicipio').html(resposta);
                jQuery('#muncod').multiselect({
                    numberDisplayed: 1,
                    includeSelectAllOption: true,
                    enableCaseInsensitiveFiltering: true,
                    enableFiltering: true,
                    id: 'muncod'
                });
            }
        });
    }

    $('#btn_geraPDF').click(function (e) {
        erro = false;

        if ($('#estuf').val() == null){
            alert('É necessário selecionar um estado');
            erro = true;
            return;
        }

        if ($('#muncod').val() == null){
            alert('É necessário selecionar ao menos um  Município');
            erro = true;
        }
        
        if(erro == false){
            e.preventDefault(); 
            $('#reqrelfichatec').val('pdf');
            $("#formMonitoramento").submit();        
        }
    });

    $('#btn_geraXLS').click(function (e) {
        erro = false;

        if ($('#estuf').val() == null){
            alert('É necessário selecionar um estado');
            erro = true;
            return;
        }

        if ($('#muncod').val() == null){
            alert('É necessário selecionar ao menos um  Município');
            erro = true;
        }
        
        if(erro == false){
            e.preventDefault(); 
            $('#reqrelfichatec').val('xls');
            $("#formMonitoramento").submit();        
        }
    })
</script>
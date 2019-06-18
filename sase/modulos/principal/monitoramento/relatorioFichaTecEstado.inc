<?php
if($_REQUEST['requisicao']){
    ob_clean();
    $dados = retornaDadosMonitoramento($_REQUEST['estuf']);
    include "fichatecnica.inc";
    die;
}
?>

<div class="container" style="margin-top: 02%">
    <form class="form-horizontal" name="formMonitoramento" id="formMonitoramento" role="form" method="POST"
          action="sase.php?modulo=principal/monitoramentoEstado&acao=A&aba=relatorioFichaTecEstado"
          target="_blank"
          enctype="multipart/form-data">
        <input name="action" type="hidden" value="gerarPdf">
        <br><br>
        <div class="form-group">
            <label class="col-sm-4 control-label " for="estauf">Estado: </label>
            <div class="col-sm-3">
                <select class="form-control chosen" multiple="multiple" id="estuf" name="estuf[]" class="multiselect">

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

        <!-- Button -->
        <div class="form-group">
            <label class="col-sm-4 "></label>
            <div class="col-md-4">
                <a href="javascript:;" class="btn btn-primary" id="btn_geraXLS">Gerar Relatório XLS</a>
                <button class="btn btn-primary" id="btn_geraPDF" type="button">Gerar Relatório PDF</button>
            </div>
        </div>
        <input type="hidden" id="reqrelfichatec" name="requisicao">
    </form>
</div>
<!-- Componentes datatable CSS -->
<script type="text/javascript" src="../library/bootstrap-datatable/js/jquery.dataTables.js"></script>
<script type="text/javascript" src="../library/bootstrap-datatable/js/dataTables.bootstrap.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-filestyle/1.2.3/bootstrap-filestyle.min.js"></script>
<!-- Componentes datetimepicker JS -->
<script type="text/javascript" src="js/bootstrap-datetimepicker/moment.min.js"></script>
<script type="text/javascript" src="js/bootstrap-datetimepicker/moment.pt-br.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/bootstrap-datepicker/1.6.4/js/bootstrap-datepicker.min.js"></script>
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>

<script>
    $(document).ready(function () {
        $('#estuf').multiselect({
            numberDisplayed: 14,
            includeSelectAllOption: true,
            enableCaseInsensitiveFiltering: true,
            enableFiltering: true,
            id: 'estuf'
        })
    });

    $('#btn_geraPDF').click(function (e) {
        if ($('#estuf').val() == null){
            alert('É necessário selecionar um estado');
            return false;
        }else{
            e.preventDefault(); 
            $('#reqrelfichatec').val('pdf');
            $("#formMonitoramento").submit();
        }
    });

    $('#btn_geraXLS').click(function (e) {
        if ($('#estuf').val() == null){
            alert('É necessário selecionar um estado');
            return false;
        }else{
            e.preventDefault(); 
            $('#reqrelfichatec').val('xls');
            $("#formMonitoramento").submit();
        }
    })
</script>

<!--22040000->

<input type="hidden" name="desid" value="<?php echo $mDesdobramento->desid; ?>"/>
<div class="form-group"><label class="col-lg-3 control-label">Código:</label>
    <div class="col-lg-2"><input type="text" disabled="" value="<?php echo $mDesdobramento->desid; ?>"
                                 class="form-control"></div>
</div>
<div class="form-group ">
    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Tipo: </label>
    <div class="col-sm-9 col-md-9 col-lg-9 ">
        <form class="input-group" style="" id="selectDesdobramentoTipo">
            <div id="selectDescTipo">
                <?php echo $simec->select('tipid', '', $mDesdobramento->tipid, $rsTipoDesdobramento,'',array('input-size'=>12)) ; ?>
            </div>
            <span class="input-group-btn" >
                <button type="button" class="btn btn-success" id="btnAcaoTipo" style="margin-top:-10px;" title="">
                         <i id="iconeAcaoTipo" class="fa fa-plus"></i>
                </button>
            </span>
        </form>
    </div>
</div>
<div class="col-lg-offset-3 col-md-offset-3" id="tipidlabel" style="display: none;"></div>
<form style="display: none;" id="formTipoDesdobramento"></form>
<?php echo $simec->input('desdsc', 'Descrição', $mDesdobramento->desdsc, array('maxlength' => '255', 'required', 'title' => 'Insira a descrição do Desdobramento')); ?>
<?php echo $simec->radio('dessituacao', 'Situação', (!($mDesdobramento->dessituacao) ? 'A' : $mDesdobramento->dessituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline')); ?>

<script>
    $(document).on('change','[name=tipid]',function () {
        var selectTipo = $('#selectDesdobramentoTipo').serialize();
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperarDesTipoObj&' + selectTipo;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                if (!isNaN(parseInt(resp)) && isFinite(resp)) {
                    $('#iconeAcaoTipo').attr('class', 'fa fa-pencil');
                    $('#iconeAcaoTipo').attr('title', 'Alterar Tipo de Desdobramento');
                    return false;
                }
                $('#iconeAcaoTipo').attr('class', 'fa fa-plus');
                $('#iconeAcaoTipo').attr('title', 'Novo Tipo de Desdobramento');
                return false;
            }
        });
    }).change();

    $('#btnAcaoTipo').click(function (evt) {
        evt.preventDefault();
        $('#formTipoDesdobramento').fadeIn();
        $('#salvarDesdobramento').attr('disabled', 'disabled');
        var selectTipo = $('#selectDesdobramentoTipo').serialize();
        console.log(selectTipo);
        var caminho = '<?php echo $urlGuia;?>';
        var action = '&req=recuperarDesTipo&' + selectTipo;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#formTipoDesdobramento').html(resp);
            }
        });
    });

    $('.search-choice-close').click(function () {
        $('abbr #iconeAcaoTipo').attr('class', 'fa fa-pencil');
    });
</script>
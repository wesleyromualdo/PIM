
<input type="hidden" name="tipid" value="<?php echo $mDesdobramentoTipo->tipid; ?>">
<div class="form-group">
    <div class="col-lg-offset-3 col-md-9" id="inputgrouptipid">
        <div class="input-group">
            <input type="hidden" id="tipid">
            <input type="text" class="form-control" placeholder="Adicione novo Tipo" id="tipdsc" name="tipdsc"
                   value="<?php echo $mDesdobramentoTipo->tipdsc; ?>">
            <span class="input-group-btn">
                    <button type="button" class="btn btn-primary" id="salvarTipo" title="Salvar Tipo">
                        <i class="fa fa-check"></i>
                    </button>
                    <button type="button" class="btn btn-default" id="cancelarTipo" title="Cancelar">
                        <i class="fa fa-times"></i>
                    </button>
                </span>
        </div>
    </div>
</div>
<div class="col-md-offset-5 col-lg-offset-5">
    <span class="help-block m-b-none" style="display: none" id="msgSalvarTipo"></span>
    <br>
</div>
<script>
    $('#cancelarTipo').click(function () {
        $('#formTipoDesdobramento').fadeOut();
        $('#salvarDesdobramento').removeAttr('disabled');
    });
    $('#salvarTipo').click(function (evt) {
        evt.preventDefault();
        swal({
                title: "Confirmar",
                text: "Deseja <b>Salvar</b> este Tipo de Desdobramento?",
                type: "success",
                html: true,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    salvarDesdobramentoTipo();
                }
                return false;
            }
        );
    });
</script>
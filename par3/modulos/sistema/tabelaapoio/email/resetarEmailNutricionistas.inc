<?php
ini_set("memory_limit", "2048M");
set_time_limit(0);

switch ($_POST['req']) {
    case 'executar':
        ob_clean();
        $mEnt = new Par3_Controller_InstrumentoUnidadeEntidade();
        echo $mEnt->resetarSenhas();
        die;
        break;
}
?>

<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Resetar Senhas Nutricionistas Pendentes</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-offset-5 col-md-3">
                <button class="btn btn-info  dim btn-dim btn-outline" id="resetar-emails-nutri">
                    Resetar e Reenviar senhas
                </button>
            </div>
        </div>
        <div class="row" id="listaNutricionistas" style="overflow-y:scroll;height:  400px;"></div>
    </div>
</div>
<script>
    $('#resetar-emails-nutri').on('click',function(evt) {
        evt.preventDefault();
        swal({
            title: "<b>Resetar</b> Senhas dos nutricionistas pendentes",
            text: "Tem certeza que deseja <b>resetar</b> as senhas dos nutricionistas com pendência?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                $.ajax({
                    type:'POST',
                    dataType: "html",
                    url: window.location.href,
                    data:{'req' : 'executar'},
                    beforeSend: function() {
                        swal.close();
                        $('.loading-dialog-par3').show();
                    },
                    success: function(data) {
                        $('.loading-dialog-par3').hide();
                        console.log(data);
                        // msgSuccess(null,'Configurações salvas.');
                        $('#listaNutricionistas').html(data);
                        // $('#modal_detalhe').hide();
                        // $('.loading-dialog-par3').show();
                        swal.close();
                    }
                });
            }
        });
    });

</script>

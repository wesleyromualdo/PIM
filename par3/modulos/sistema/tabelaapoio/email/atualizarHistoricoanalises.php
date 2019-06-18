<?php
ini_set("memory_limit", "2048M");
set_time_limit(0);

switch ($_POST['req']) {
    case 'executarentidade':
        ob_clean();
        $mEnt = new Par3_Controller_AnaliseEngenhariaObra();
        echo $mEnt->atualizarHistoricoEntidades();
        die;
        break;
}
?>

<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Atualizar histórico das Análises de Obra</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-offset-5 col-md-3">
                <button class="btn btn-info  dim btn-dim btn-outline" id="atualizar-historicos">
                   Atualizar Históricos
                </button>
            </div>
        </div>
        <div class="row" id="listaHistorico" style="overflow-y:scroll;height:  400px;"></div>
    </div>
</div>
<script>
    $('#atualizar-historicos').on('click',function(evt) {
        evt.preventDefault();
        swal({
            title: "<b>Atualizar</b> Histórico das Entidades",
            text: "Tem certeza que deseja <b>Atualizar</b> o histórico das análise das entidades?",
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
                    data:{'req' : 'executarentidade'},
                    beforeSend: function() {
                        swal.close();
                        $('.loading-dialog-par3').show();
                    },
                    success: function(data) {
                        $('.loading-dialog-par3').hide();
                        console.log(data);
                        // msgSuccess(null,'Configurações salvas.');
                        $('#listaHistorico').html(data);
                        // $('#modal_detalhe').hide();
                        // $('.loading-dialog-par3').show();
                        swal.close();
                    }
                });
            }
        });
    });

</script>

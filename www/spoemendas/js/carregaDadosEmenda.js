/**
 * Created by victor on 14/06/18.
 */

function onCargatesouroSubmit() {
    if (!$('#cargatesouro_arquivo').val()) {
        alert('O campo "arquivo" é obrigatório e não pode ser deixado em branco.');
        return false;
    }
    return true;
}

$(document).ready(function(){
    $('input[type=file]').bootstrapFileInput();

    $('#simec_job-btn').on('click',function(){
        $('#divLista').hide();
    });

    $('#cargatesouro').on('submit',function(){
        divCarregando();
    });
});


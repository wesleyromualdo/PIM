<?php
$modelEventoTipo = new Par3_Model_EventoTipo();
$tiposEvento = $modelEventoTipo->recuperarTodos('*', '', 'evtordem');
?>

<form method="post" name="formulario-modal" id="formulario-modal-tipo" class="form form-horizontal">
    <input type="hidden" name="action" value="salvar-tipo"/>
    <button type="button" class="btn btn-primary" style="margin-bottom: 10px;" id="adicionar-tipo"><span class="fa fa-plus"></span> Adicionar Tipo de Evento</button>
    <table class="table table-bordered"  id="tabela_tipo">
        <tr>
            <th width="40%" class="text-center">Título</th>
            <th width="10%" class="text-center">Cor</th>
            <th width="15%" class="text-center">Ordem</th>
            <th width="35%" class="text-center">Preview</th>
        </tr>
        <?php foreach($tiposEvento as $dado) { ?>
            <tr>
                <td>
                    <input type="hidden" name="eventotipo[<?php echo $dado['evtid']; ?>][evtid]" value="<?php echo $dado['evtid']; ?>" />
                    <input class="form-control alterar-preview" data-evtid="<?php echo $dado['evtid']; ?>" type="text" name="eventotipo[<?php echo $dado['evtid']; ?>][evtdsc]" value="<?php echo $dado['evtdsc']; ?>" maxlength="200" />
                </td>
                <td><input class="form-control alterar-preview-cor" data-evtid="<?php echo $dado['evtid']; ?>" type="color" name="eventotipo[<?php echo $dado['evtid']; ?>][evtcor]" value="<?php echo $dado['evtcor']; ?>" /></td>
                <td><input class="form-control" type="text" name="eventotipo[<?php echo $dado['evtid']; ?>][evtordem]" value="<?php echo $dado['evtordem']; ?>" /></td>
                <td><div id="preview_<?php echo $dado['evtid']; ?>" class='external-event btn-danger' style="background-color: <?php echo $dado['evtcor']; ?>" id="tipo_<?php echo $dado['evtid']; ?>" data-evtid="<?php echo $dado['evtid']; ?>"><?php echo $dado['evtdsc']; ?></div></td>
            </tr>
        <?php } ?>
    </table>

</form>

<script type="text/javascript">
    $(function(){
        $('body').on('keyup', '.alterar-preview', function(){
            var evtid = $(this).data('evtid');
            $('#preview_' + evtid).html($(this).val());
        });

        $('body').on('input', '.alterar-preview-cor', function(){
            console.log($(this).val());
            var evtid = $(this).data('evtid');
            $('#preview_' + evtid).css('background-color', $(this).val());
        });

        $('#adicionar-tipo').click(function(){
            var qtd = $('#tabela_tipo tr').length;
            var linha = '<tr>' +
                '<td>' +
                '<input type="hidden" name="eventotipo[novo_' + qtd + '][evtid]" />' +
                '<input class="form-control alterar-preview" data-evtid="novo_' + qtd + '" type="text" name="eventotipo[novo_' + qtd + '][evtdsc]" maxlength="200" />' +
                '</td>' +
                '<td><input class="form-control alterar-preview-cor2" data-evtid="novo_' + qtd + '" type="color" name="eventotipo[novo_' + qtd + '][evtcor]" value="#ED5666" /></td>' +
                '<td><input class="form-control" type="text" name="eventotipo[novo_' + qtd + '][evtordem]" /></td>' +
                '<td></td>';
            $('#tabela_tipo').append(linha);
        });
    });
</script>
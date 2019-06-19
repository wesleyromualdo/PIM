<!-- Modal -->
<div class="modal fade" id="modal-aviso-<?php echo $aviso['avdid']; ?>" tabindex="-1" role="dialog" aria-labelledby="modal-aviso-<?php echo $aviso['avdid']; ?>-label">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="modal-aviso-1-label"><?php echo $aviso['avititulo']; ?></h4>
            </div>
            <div class="modal-body">
                <?php echo $aviso['avidsc']; ?>
            </div>
            <div class="modal-footer">
                <a href="/publico/requisicoes.php?action=sac-ler-aviso&avdid=<?php echo $aviso['avdid']; ?>" class="btn btn-primary btn-ler-aviso" id="">Ciente da Comunicação</a>
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function(){
        $('#modal-aviso-<?php echo $aviso['avdid']; ?>').modal();

        $('.btn-ler-aviso').click(function(){
            var url = $(this).attr('href');
            $.ajax({
                url: url,
                type: 'POST',
                success: function (retorno) {
                    $('#modal-aviso-<?php echo $aviso['avdid']; ?>').modal('hide');
                }
            });
            return false;
        });

    });
</script>
<!-- Tarja -->
<div id="tarja-aviso-<?php echo $aviso['avdid']; ?>" class="alert alert-success" style="margin-top: 5px;">
    <strong><h4><?php echo $aviso['avititulo']; ?></h4></strong>
    <div>
        <?php echo $aviso['avidsc']; ?>
    </div>
    <div class="text-center">
        <a href="/publico/requisicoes.php?action=sac-ler-aviso&avdid=<?php echo $aviso['avdid']; ?>" class="btn btn-primary btn-ler-aviso" id="">Ciente da Comunicação</a>
    </div>
</div>

<script type="text/javascript">
    $(function(){
        $('.btn-ler-aviso').click(function(){
            var url = $(this).attr('href');
            $.ajax({
                url: url,
                type: 'POST',
                success: function (retorno) {
                    $('#tarja-aviso-<?php echo $aviso['avdid']; ?>').remove();
                }
            });
            return false;
        });

    });
</script>
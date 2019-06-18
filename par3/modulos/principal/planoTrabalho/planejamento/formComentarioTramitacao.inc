<div class="row center">
<h3 align="center">Tem certeza que deseja tramitar os planejamentos selecionados da situação <b><?=$estAtual?></b> para a situação <b><?=$proxEst?></b>?</h3>

<?php if($_POST['comentario'] != 0):?>
    <h3 align="center">Comentário:*</h3>
    <style>
        .col-lg-9 {
            width: 100% !important;
        }
    </style>
    <div>
        <?php echo $simec->textarea('docdsc', null, '', array('obrig' => 'S', 'habil' => 'S','cols' => 60, 'rows' => 4, 'width' => null)); ?>
    </div>
<?php endif; ?>
    <div style="float: left;padding: 10px;width: 100%;">
        <hr>
        <button class="btn btn-default" id="btnFecharTramitacao">Cancelar</button>
        <button class="btn btn-success" onclick='ajax_tramitar(<?= json_encode($_POST['arrdocid'])?>,<?=$_POST['aedid']?>)'>Confirmar Tramitação</button>
    </div>

</div>
<script>
    function ajax_tramitar(docid,aedid)
    {
        var docdsc = '';
        if($('#docdsc').length > 0){
            docdsc = $('#docdsc').val();
            if(docdsc == ""){
                alert('Preencha o campo Comentário');
                return false;
            }
        }

        var action = window.location.href;
        var data = {requisicao:'tramitaremlote',arrdocid:docid,aedid:aedid,docdsc:docdsc};
        $.ajax({
            type: "POST",
            url: action,
            data: data,
            async: false,
            success: function (resp) {

                if(resp == 1){
                    swal({
                            title: "Sucesso",
                            html: true,
                            text: 'Tramitação realizada com sucesso.',
                            type: "success",
                            allowEscapeKey: false
                        },
                        function() {
                            $('#modal_tramitar').modal('hide');
                            $(document).find('#formulario-filtro-obras').submit();
                        }
                    );
                }else{
                    swal({
                            title: "Erro",
                            html: true,
                            text: 'Ocorreu um erro ao realizar a tramitação',
                            type: "error"},
                        function() {
                           $('.modal').modal('hide');
                        }
                    );
                }
            }
        });
    }
</script>
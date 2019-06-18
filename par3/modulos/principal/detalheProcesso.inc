<script>
    $(document).ready(function() {
        $('body').on('click','.processo_detalhe', function(){
            var nrprocesso = $(this).html();
            $("#dialog_title_processo").html('Detalhes do Processo ' + nrprocesso);
            $("#dialog_detalhe_processo").load('/obras2/ajax.php?buscasaldoprocesso=' + nrprocesso, function(){
                $("#modal_detalhe_processo").modal();
            });
        });
    })
    $(document).ready(function() {
        $('body').on('click','.executora_detalhe', function(){
            var nrprocesso = $(this).attr('processo_num');
            $("#dialog_title_processo").html('Detalhes do Processo ' + nrprocesso);
            $("#dialog_detalhe_processo").load('/obras2/ajax.php?busca_executora=' + nrprocesso, function(){
                $("#modal_detalhe_processo").modal();
            });
        });
    })
</script>
<style>
    .processo_detalhe{
        cursor:pointer;
        color:blue;
        margin:1px;
    }
    .processo_detalhe:hover{
        cursor:pointer;
        color:#87CEFA;
    }
    .executora_detalhe{
        cursor:pointer;
        color:blue;
        margin:1px;
    }
    .executora_detalhe:hover{
        cursor:pointer;
        color:#87CEFA;
    }

    #modal_detalhe_processo .modal-body {
        max-height: calc(100% - 120px);
        overflow-y: scroll;
    }
</style>
<div class="modal fade" id="modal_detalhe_processo" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" style="display: none;">
    <div class="modal-dialog" role="document" style="width: 70%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel"><div id="dialog_title_processo"></div></h4>
            </div>
            <div class="modal-body">
                <div id="dialog_detalhe_processo"></div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>
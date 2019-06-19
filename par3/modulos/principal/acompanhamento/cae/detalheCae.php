<?php

require_once APPRAIZ . 'includes/workflow.php';

$modelCae = new Par3_Model_CAE();
$controllerCae = new Par3_Controller_CAE();

$sql = $modelCae->getSqlListaCae([], $entid);
$dados = $modelCae->pegaLinha($sql);
$objCAE = (object)$dados;

?>

<div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span>
    </button>
    <h4 class="modal-title" id="myModalLabel">Detalhe</h4>
</div>
<div class="modal-body">
    <div class="ibox">
        <div class="ibox-content">
            <form method="post" name="form-cae" id="form-cae" class="form form-horizontal">

                <div class="row">
                    <div class="col-lg-11">
                        <?php
                        $controllerCae->formConselheiro('disabled', $objCAE);
                        $controllerCae->formConselheiroExterno('disabled', $objCAE);
                        ?>
                    </div>
                    <div class="col-lg-11">
                        <?php wf_desenhaBarraNavegacao($docid, array('caeid' => $objCAE->caeid), ''); ?>
                    </div>
                </div>
            </form>
        </div>
    </div>
</div>

<div class="modal-footer">
    <button type="button" class="btn btn-default" data-dismiss="modal">fechar</button>
</div>

<script>
    $(function () {
        $('.cpf_conselheiro').closest('.form-group').remove()
        $('.nome_conselheiro').closest('.form-group').remove()
        $('#entemail-conselheiro').closest('.form-group').remove()
        $('.excluir_arq').remove();
        $('input#arqid').closest('.form-group').remove()
    })
</script>







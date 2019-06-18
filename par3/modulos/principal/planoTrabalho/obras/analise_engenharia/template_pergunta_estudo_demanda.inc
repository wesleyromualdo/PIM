<?php
global $simec;
?>
<?php if ($permiteEditar): ?>
    <div class="panel panel-info" id="panelpergunta_<?php echo $mAep->aepid; ?>">
        <div class="panel-heading" style="height: 90px;">
            <div class="center">
                <input type="hidden" name="aepid" value="<?php echo $mAep->aepid; ?>">
                <input type="hidden" name="aerid" value="<?php echo $mAer->aerid; ?>">
                <input type="hidden" name="aegid" value="<?php echo $aegid; ?>">
                <input type="hidden" name="oabid" value="<?php echo $mAep->oabid; ?>">
                <input type="hidden" name="aepordem" value="<?php echo $mAep->aepordem; ?>">
                <div class="form-group">
                    <label for="" class="control-label  control-label col-lg-12"><?php echo $mAep->aepdsc; ?></label>
                    <div class="btn-group" data-toggle="buttons" id="btn-pendencia">
                        <label class="btn btn-<?php echo $mAer->aer_pendencia == 'S' ? "danger" : 'primary' ?> <?php echo $mAer->aer_pendencia == 'S' ? "active" : '' ?>">
                            <input name="aer_pendencia_<?php echo $mAep->aepid; ?>" type="checkbox"
                                   value="S" <?php echo $mAer->aer_pendencia == 'S' ? "checked='true'" : '' ?>>Não</label>
                    </div>
                </div>
            </div>
        </div>
        <div class="panel-body" id="bodypendencia_<?php echo $mAep->aepid; ?>" style="<?php echo $mAer->aer_pendencia == 'S' ? "" : 'display: none;' ?>">
            <div class="row">
                <h3 class="center">Descrição da Pendência</h3>
                <div id="txtpendencia_<?php echo $mAep->aepid; ?>" class="col-lg-12" >
                    <textarea class="form-control" name="aer_observacao" id="aer_observacao"><?echo $mAer->aer_observacao; ?></textarea>
                </div>
            </div>
        </div>
        <div class="panel-footer" id="footerpendencia_<?php echo $mAep->aepid; ?>">
            <div class="center">
                <button id="btn_pergunta_<?php echo $mAep->aepid; ?>" class="btn btn-success">Salvar <i class="fa fa-save"></i></button>
            </div>
        </div>
    </div>
    <script>
        $(document).on('change', "[name=aer_pendencia_<?php echo $mAep->aepid; ?>]", function () {
            if ($(this).is(':checked')) {
                $(this).attr('checked', true);
                $(this).parent('label').removeClass('btn-primary').addClass('btn-danger');
                $(this).val('S');
                $(document).find('#bodypendencia_<?php echo $mAep->aepid; ?>').slideDown();
            } else {
                $(this).attr('checked', false);
                $(this).val('N');
                $(this).parent('label').removeClass('btn-danger').addClass('btn-primary');
                $(document).find('#bodypendencia_<?php echo $mAep->aepid; ?>').slideUp();
            }
        }).change();

        $(document).ready(function () {
            $('textarea').removeAttr('disabled'); $('textarea').removeAttr('readonly', 'readonly');
            $('.i-checks').iCheck({
                checkboxClass: 'icheckbox_square-green',
                radioClass: 'iradio_square-green'
            });
        });

        $('#btn_pergunta_<?php echo $mAep->aepid; ?>').click(function (evt) {
            evt.preventDefault();
            var aepid = $('#panelpergunta_<?php echo $mAep->aepid; ?>').find('[name=aepid]').val();
            var aerid = $('#panelpergunta_<?php echo $mAep->aepid; ?>').find('[name=aerid]').val();
            var aegid = $('#panelpergunta_<?php echo $mAep->aepid; ?>').find('[name=aegid]').val();
            var oabid = $('#panelpergunta_<?php echo $mAep->aepid; ?>').find('[name=oabid]').val();
            var aepordem = $('#panelpergunta_<?php echo $mAep->aepid; ?>').find('[name=aepordem]').val();
            var aer_pendencia = $('#panelpergunta_<?php echo $mAep->aepid; ?>').find('[name=aer_pendencia_<?php echo $mAep->aepid; ?>]').val();
            var aer_observacao = $('#panelpergunta_<?php echo $mAep->aepid; ?>').find('[name=aer_observacao]').val();
            var form = '&aepid=' + aepid + '&aegid=' + aegid + '&oabid=' + oabid + '&aepordem=' + aepordem + '&aer_pendencia=' + aer_pendencia + '&aer_observacao=' + aer_observacao + '&aerid=' + aerid;
            var caminho = window.location.href;
            var action = '&requisicao=salvaranalise&' + form;

            swal({
                title: "Confirmar",
                text: "Salvar Análise?",
                type: "info",
                html: true,
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            }, function (isConfirm) {
                if (isConfirm) {
                    $.ajax({
                        type: "POST",
                        url: caminho,
                        data: action,
                        async: false,
                        success: function (resp) {
                            $('#panelpergunta_<?php echo $mAep->aepid; ?>').find('[name=aerid]').val(resp);
                            swal('Análise Salva com sucesso', '', 'success');
                            <?php if($mAep->aepordem == 8):?>
                            msgSuccess(caminho,'Análise Salva com sucesso');
                            <?php endif;?>
                        },
                        complete: function(resp) {
                            checarPendencias();
                        }
                    });
                }
            });
        });
    </script>
<?php else:?>
    <div class="panel panel-danger">
        <div class="panel-heading" align="center">
            <h3><b><?php echo $mAep->aepdsc; ?></b></h3>
        </div>
        <div class="panel-body">
            <p align="center"><b><?php echo  $mAer->aer_observacao?></b></p>
        </div>
    </div>
<?php endif; ?>


<?php
global $simec;
$class = $mOtd->otdanalise_textorico == 'S'?array('class' => 'summernote') : array();
?>
<style>
    .active{
        color: red;
    }
</style>
<div class="panel panel-info" id="panelpergunta_<?php echo $mOtd->otdid; ?>">
    <div class="panel-heading">
        <div class="center">
            <input type="hidden" name="aedid" value="<?php echo $mAed->aedid;?>">
            <input type="hidden" name="aegid" value="<?php echo $aegid;?>">
            <input type="hidden" name="otdid" value="<?php echo $mOtd->otdid; ?>">
            <div class="form-group">
                <div class="col-lg-12">
                    <label for="" class="control-label  control-label">
                        <center><?php echo ($mOtd->otdanalise_engenharia? $mOtd->otdanalise_engenharia  :'O documento informado está de acordo com as especificações exigidas?'); ?></center>
                    </label>
                </div>
                <div class="btn-group" data-toggle="buttons" id="btn-pendencia">
                    <label class="btn btn-<?php echo $mAed->aed_pendencia == 'S'?"danger":'primary'?> <?php echo $mAed->aed_pendencia == 'S'?"active":''?>">
                        <input name="aed_pendencia"  type="checkbox" value="S" <?php echo $mAed->aed_pendencia == 'S'?"checked='true'":''?>>Não</label>
                </div>
            </div>
        </div>
    </div>
    <div class="panel-body" id="bodypendencia" style="<?php echo $mAed->aed_pendencia == 'S'?"":'display: none;'?>">
        <div class="row">
            <div class="center">
                <div id="txtpendencia_<?php echo $mOtd->otdid; ?>">
                    <h3 class="center">Descrição da Pendência</h3>
                    <textarea class="form-control <?php echo $mOtd->otdanalise_textorico == 'S'?'summernote':''?>" name="aed_observacao" id="aed_observacao_<?php echo $mOtd->otdid; ?>"><?php echo $mAed->aed_observacao?simec_stripslashes($mAed->aed_observacao):''; ?></textarea>
                </div>
            </div>
        </div>
    </div>
</div>
<script>

    $(document).ready(function () {
        $('.i-checks').iCheck({
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green'
        });
    });
</script>
<?php global $simec;?>
<form action="" id="form-confs-painel">
    <div class="panel-group" id="painel-iboxes-conf">
        <?php for($i = 0;$i < count($this->quadro);$i++):?>
            <div class="panel panel-default panel-confs" id="panel-conf-<?= $this->quadro[$i]->getId() ?>">
                <div class="panel-heading">
                    <h4 class="panel-title">
                        <a
                            data-toggle="collapse"
                            href="#collapse<?=$this->quadro[$i]->getId()?>"
                        >
                            <?= $this->quadro[$i]->getConteudoTitulo();?>
                        </a>
                        <div class="pull-right move-icon" style="display:none;">
                            <i class="fa fa-sort"></i>
                        </div>
                    </h4>
                </div>
                <div id="collapse<?=$this->quadro[$i]->getId()?>" class="panel-collapse collapse">
                    <input type="hidden" name="ibox[]" value="<?= $this->quadro[$i]->getId() ?>" />
                    <div class="panel-body">
                        <ul class="list-group">
                            <div class="row">
                                <?php
                                    echo $simec->select(
                                        'pflcod',
                                        'Perfil Associado',
                                        [], $this->sqlperfis,
                                        array('multiple'),
                                        array('input-size' => 6, 'label-size' => 2)
                                    );
                                ?>
<!--                                <div class="col-lg-4">-->
<!--                                    <div class="list-group-item-text">-->
<!--                                        <div class="checkbox checkbox-success checkbox-inline">-->
<!--                                            <input-->
<!--                                                type="hidden"-->
<!--                                                name="--><?//= $this->quadro[$i]->getId() ?><!--[visivel]"-->
<!--                                                value="false"-->
<!--                                            >-->
<!--                                            <input type="checkbox" --><?//= ($this->quadro[$i]->isVisivel()?'checked':''); ?>
<!--                                                   class="class-checkbox-todos"-->
<!--                                                   name="--><?//= $this->quadro[$i]->getId() ?><!--[visivel]"-->
<!--                                                   value="true"-->
<!--                                                   id="conf---><?//= $this->quadro[$i]->getId() ?><!--"-->
<!--                                            >-->
<!--                                            <label for="conf---><?//= $this->quadro[$i]->getId() ?><!--">-->
<!--                                                Visível-->
<!--                                            </label>-->
<!--                                        </div>-->
<!--                                    </div>-->
<!--                                </div>-->
                            </div>
                        </ul>
                    </div>
                </div>
            </div>
        <?php endfor;?>
    </div>
    <div class="col-lg-offset-5 col-md-offset-5 col-xs-offset-3">
        <button id="salvar-confs-btn" class="btn btn-success">Salvar
            <i class="fa fa-save"></i>
        </button>
        <button id="resetar-confs-btn" class="btn btn-default">Resetar
            <i class="fa fa-eraser"></i>
        </button>
    </div>
</form>
<script>
    $('.panel-collapse').on('show.bs.collapse','.collapse', function (e) {
        $(this).find('.collapse.in').collapse('hide');
    });
</script>
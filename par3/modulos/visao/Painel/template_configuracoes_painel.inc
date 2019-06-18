<div>
        <button id="ordenar-conf" class="btn btn-default">
            <i class="fa fa-sort">Re-ordenar</i>
        </button>
    </div>


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
            <div id="collapse<?=$this->quadro[$i]->getId()?>" class="panel-collapse collapse sort">
                <input type="hidden" name="ibox[]" value="<?= $this->quadro[$i]->getId() ?>" />
                <div class="panel-body">
                    <ul class="list-group">
                        <div class="row">
                            <div class="col-lg-4">
                                <div class="list-group-item-text">
                                    <div class="checkbox checkbox-success checkbox-inline">
                                        <input
                                                type="hidden"
                                                name="<?= $this->quadro[$i]->getId() ?>[visivel]"
                                                value="false"
                                        >
                                        <input type="checkbox" <?= ($this->quadro[$i]->isVisivel()?'checked':''); ?>
                                               class="class-checkbox-todos"
                                               name="<?= $this->quadro[$i]->getId() ?>[visivel]"
                                               value="true"
                                               id="conf-<?= $this->quadro[$i]->getId() ?>"
                                        >
                                        <label for="conf-<?= $this->quadro[$i]->getId() ?>">
                                            Visível
                                        </label>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </ul>
                <?php foreach($this->quadro[$i]->getListaToolBoxOpcoes() as $toolBox):?>
                    <?php if($toolBox instanceof visao\Helper\ToolBoxIcon\CheckBoxFilterToolBox):?>
                        <?php
                            $values     = array();
                            $valueTodos = '';
                            if(isset($toolBox->getValues()[$toolBox->getName()])) {
                                $values     = $toolBox->getValues()[$toolBox->getName()];
                                $valueTodos = $toolBox->getValues()[$toolBox->getName().'-todos'];
                            }
                        ?>
                        <div class="">
                        <ul class="list-group">
                            <?php if($toolBox->isAgrupar()):?>
                            <div class="panel panel-default">
                                <div class="panel-heading">
                                    <h5 align="center" class="list-group-heading" >
                                        <b><?= $toolBox->getTitle();?></b>
                                    </h5>
                                </div>
                                <div class="panel-body">
                                    <div class="row">
                                        <div class="col-lg-4">
                                            <div class="list-group-item-text">
                                                <div class="checkbox checkbox-success checkbox-inline">
                                                    <input type="checkbox" <?= ($valueTodos == 'S'?'checked':''); ?>
                                                           class="class-checkbox-todos"
                                                           name="<?= $this->quadro[$i]->getId();?>[<?= $toolBox->getName()?>-todos]"
                                                           value="S"
                                                           id="conf-<?= $toolBox->getName(); ?>-todos"
                                                    >
                                                    <label for="conf-<?= $toolBox->getName(); ?>-todos">
                                                        Marcar/Desmarcar Todos
                                                    </label>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <?php foreach($toolBox->getParams() as $key => $grupo):?>
                                        <h5 align="left" class="list-group-heading" ><b><?= $key;?></b></h5>
                                        <div class="row">
                                        <?php foreach($grupo as $key2 => $value):?>
                                        <div class="col-lg-4">
                                        <div class="list-group-item-text">
                                                <div class="checkbox checkbox-success checkbox-inline">
                                                    <input
                                                        type="checkbox"
                                                        name="<?= $this->quadro[$i]->getId() ?>[<?=$toolBox->getName()?>][]"
                                                        value="<?= $key2;?>"
                                                        class="conf-<?= $toolBox->getName()?>-todos"
                                                        <?= (in_array($key2,$values)?'checked':''); ?>
                                                        id="conf-<?=$toolBox->getName().$key2?>"
                                                    >
                                                    <label for="conf-<?=$toolBox->getName().$key2?>">
                                                        <?= $value;?>
                                                    </label>
                                                </div>
                                            </div>
                                        </div>
                                    <?php endforeach;?>
                                        </div>
                                    <?php endforeach;?>
                                </div>
                            </div>
                            <?php else:?>
                            <div class="panel panel-default">
                                <div class="panel-heading">
                                    <h5 align="center" class="list-group-heading" >
                                        <b><?= $toolBox->getTitle();?></b>
                                    </h5>
                                </div>
                                <div class="panel-body">
                                    <div class="row">
                                    <div class="col-lg-4">
                                        <div class="list-group-item-text">
                                            <div class="checkbox checkbox-success checkbox-inline">
                                                <input
                                                    type="checkbox" <?= ($valueTodos == 'S'?'checked':''); ?>
                                                    class="class-checkbox-todos"
                                                    name="<?= $this->quadro[$i]->getId(); ?>[<?= $toolBox->getName()?>-todos]"
                                                    value="S"
                                                    id="conf-<?= $toolBox->getName(); ?>-todos"
                                                >
                                                <label for="conf-<?= $toolBox->getName(); ?>-todos">
                                                    Marcar/Desmarcar Todos
                                                </label>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                <?php foreach($toolBox->getParams() as $key => $value):?>
                                    <div class="col-lg-4">
                                        <div class="list-group-item-text">
                                            <div class="checkbox checkbox-success checkbox-inline">
                                                <input
                                                    type="checkbox"
                                                    name="<?= $this->quadro[$i]->getId() ?>[<?=$toolBox->getName()?>][]"
                                                    value="<?= $key;?>"
                                                    class="conf-<?= $toolBox->getName()?>-todos"
                                                    <?= (in_array($key,$values)?'checked':''); ?>
                                                    id="conf-<?=$toolBox->getName().$key?>">
                                                <label for="conf-<?=$toolBox->getName().$key?>"><?= $value;?></label>
                                            </div>
                                        </div>
                                    </div>
                                <?php endforeach;?>
                                </div>
                                </div>
                            </div>
                            <?php endif;?>
                        </ul>
                        </div>
                    <?php endif;?>
                <?php endforeach;?>
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
    $(function(){
        $('#painel-iboxes-conf').sortable({
            cursor: 'move',
            placeholder: 'ui-state-highlight',
            start: function(e, ui) {
                ui.placeholder.width(ui.item.find('.sort').width());
                ui.placeholder.height(ui.item.find('.sort').height());
                ui.placeholder.addClass(ui.item.attr("class"));
            }
        });


        $("#painel-iboxes-conf").sortable("disable");
        var sortable = false;
        $("#ordenar-conf").on('click',function() {
            let sort = $('.sort');

            if(sort.hasClass('collapse in')) {
                sort.collapse("hide");
            }

            if(!sortable) {
                $(this).find('i').html('Finalizar Reordenar');
                $("#painel-iboxes-conf").sortable("enable");
                $(this).addClass('btn-success');
                $(this).removeClass('btn-default');
                $('.move-icon').show();
                sortable = true;
            }else {
                $(this).find('i').html('Reordenar');
                $("#painel-iboxes-conf").sortable("disable");
                $(this).addClass('btn-default');
                $(this).removeClass('btn-success');
                $('.move-icon').hide();
                sortable = false;
            }
        });
    });


    $('.panel-collapse').on('show.bs.collapse','.collapse', function (e) {
        $(this).find('.collapse.in').collapse('hide');
    });

    $('#salvar-confs-btn').on('click',function(evt) {
        evt.preventDefault();
        var form = $('#form-confs-painel').serialize();

        $.ajax({
            type:'POST',
            dataType: "html",
            url: '/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=salvarconfs&acao=A',
            data: form,
            beforeSend: function() {
                $('.loading-dialog-par3').show();
            },
            success: function(data) {
                $('.loading-dialog-par3').hide();
                msgSuccess('/par3/par3.php?modulo=painel/index&acao=A','Configurações salvas.');
                $('#modal_detalhe').hide();
                $('.loading-dialog-par3').show();

                return false;
            }
        });
    });
    $('#resetar-confs-btn').on('click',function(evt) {
        evt.preventDefault();
        swal({
            title: "<b>Resetar</b> Configurações",
            text: "Tem certeza que deseja <b>resetar</b> as configurações do Painel?",
            html: true,
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                $.ajax({
                    type:'POST',
                    dataType: "html",
                    url: '/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=resetarconfs&acao=A',
                    beforeSend: function() {
                        $('.loading-dialog-par3').show();
                    },
                    success: function(data) {
                        $('.loading-dialog-par3').hide();
                        msgSuccess('/par3/par3.php?modulo=painel/index&acao=A','Configurações salvas.');
                        $('#modal_detalhe').hide();
                        $('.loading-dialog-par3').show();

                        return false;
                    }
                });
            }
        });
    });

    $(".class-checkbox-todos").change(function (e) {
        e.preventDefault();
        var id = $(this).prop('id');
        console.log(id);
        $("#"+id).prop("checked");

        if ( $("#"+id).is(':checked') ) {
            $("."+id).prop("checked", true);
        }else {
            $("."+id).prop("checked", false);
        }
    });
</script>

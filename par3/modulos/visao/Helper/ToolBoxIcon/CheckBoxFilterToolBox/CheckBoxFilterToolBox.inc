<?php
$values     = array();
$valueTodos = '';

if(isset($this->values->{$this->getParentId()}->{$this->name})) {
    $values     = $this->values->{$this->getParentId()}->{$this->name};
    $valueTodos = $this->values->{$this->getParentId()}->{$this->name.'-todos'};
}
$i = 0;
?>
<div class="dropdown" style="display:inline-block;">
<a class="dropdown-toggle" data-toggle="dropdown" href="#" aria-expanded="true" title="<?= $this->title; ?>">
    <i class="fa fa-filter success" title="<?= $this->title; ?>"></i>
</a>
<ul class="dropdown-menu dropdown-user">
    <h3 align="center" class="list-group-heading h3" ><b><?= $this->title;?></b></h3>
    <li class="list-group-item">
        <div class="list-group-item-text">
            <div class="checkbox checkbox-success checkbox-inline">
                <input type="checkbox" <?= ($valueTodos == 'S'?'checked':''); ?>
                       name="<?= $this->getParentId();?>[<?= $this->name?>-todos]" value="S" id="<?= $this->name; ?>-todos">
                <label for="<?= $this->name; ?>-todos">Marcar/Desmarcar Todos</label>
            </div>
        </div>
    </li>
    <?php if($this->agrupar):?>
    <li class="list-group-item" style="max-height: 300px;width:300px;overflow-y: scroll;">
            <?php foreach($this->params as $key => $grupo):?>
                <h4 align="left" class="list-group-heading h4" ><b><?= $key;?></b></h4>
                <?php foreach($grupo as $key2 => $value):?>
                    <div class="list-group-item-text">
                        <div class="checkbox checkbox-success checkbox-inline">
                            <input
                                    type="checkbox"
                                    name="<?= $this->getParentId();?>[<?=$this->name?>][]"
                                    value="<?= $key2;?>"
                                    <?= (in_array($key2,$values)?'checked':''); ?>
                                    id="<?=$this->name.$key2?>">
                            <label for="<?=$this->name.$key2?>"><?= $value;?></label>
                        </div>
                    </div>
                <?php endforeach;?>
                <hr>
            <?php endforeach;?>
    </li>
    <?php else:?>
        <li class="list-group-item" style="max-height: 300px;width:300px;overflow-y: scroll;">
            <?php foreach($this->params as $key => $value):?>
                <div class="list-group-item-text">
                    <div class="checkbox checkbox-success checkbox-inline">
                        <input type="checkbox"
                               name="<?= $this->getParentId();?>[<?=$this->name?>][]"
                               value="<?= $key;?>"
                               <?= (in_array($key,$values)?'checked':''); ?>
                               id="<?=$this->name.$key?>">
                        <label for="<?=$this->name.$key?>"><?= $value;?></label>
                    </div>
                </div>
            <?php endforeach;?>
        </li>
    <?php endif;?>
    <li>
        <button type='button' class='btn btn-success col-lg-12 col-sm-12 col-xs-12 col-md-12' id='btn-filtro-<?=$this->id?>'>Filtrar</button>
    </li>
</ul>
</div>
<script>
    $('.dropdown-menu').click(function(e) {
        e.stopPropagation();
    });

    $("#btn-filtro-<?=$this->id?>").on('click',function () {
        var ibox    = $("#btn-filtro-<?=$this->id?>").parents('.ibox:first');
        var loadDiv = $("#btn-filtro-<?=$this->id?>").parents('.ibox:first').find('.ibox-load-div:first');
        var arr = [];
        $("input:checkbox[name='<?= $this->getParentId();?>[<?=$this->name?>][]']:checked").each(function() {
            arr.push($(this).val());
        }) ;

        <?php
            $cont = 0;
        if($this->aditionalSubmitNames) {
            foreach($this->aditionalSubmitNames as $aditionalSubmitName) {
                echo 'var arr'.$aditionalSubmitName.$cont.' = [];';
                echo "$(\"input:checkbox[name='".$aditionalSubmitName."[]']:checked\").each(function() {return arr".$aditionalSubmitName.$cont.".push($(this).val());});";
                $cont++;
            }
        }
        ?>
        $.ajax({
            dataType: "html",
            url: '<?= $this->url?>',
            data: {
                <?=$this->name?>: arr
                <?php $cont = 0;
                     foreach($this->aditionalSubmitNames as $aditionalSubmitName) {
                        echo ','.$aditionalSubmitName." : arr".$aditionalSubmitName.$cont;
                        $cont++;
                    }
                ?>
            },
            beforeSend : function() {
                $('.dropdown').toggleClass('open',false);
                loadDiv.css('height',ibox.height());
                loadDiv.css('width',ibox.width());
                loadDiv.fadeIn();
            },
            success: function(data) {
                loadDiv.fadeOut();
                ibox.find('.ibox-content:first').html(data);
            }
        });
    });

    $("[name='<?= $this->getParentId();?>[<?= $this->name?>-todos]']").change(function () {
        $("[name='<?= $this->getParentId();?>-<?= $this->getParentId();?>[<?=$this->name?>][]']").prop("checked");
    });

    $("[name='<?= $this->getParentId();?>[<?= $this->name?>-todos]']").click(function() {
        $("[name='<?= $this->getParentId();?>[<?=$this->name?>][]']").prop('checked', this.checked);
    });

    var checkTodos = $("[name='<?= $this->getParentId();?>[<?= $this->name?>-todos]']");
    checkTodos.click(function () {
        if ( $(this).is(':checked') ){
            $("[name='<?= $this->getParentId();?>[<?=$this->name?>][]']").prop("checked", true);
        }else{
            $("[name='<?= $this->getParentId();?>[<?=$this->name?>][]']").prop("checked", false);
        }
    });
    // Fim marcar/desmarcar todos
</script>
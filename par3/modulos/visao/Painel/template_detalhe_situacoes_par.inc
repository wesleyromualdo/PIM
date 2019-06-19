
<?php if (!empty($this->arIniciativa)):?>
<div class="table-responsive">

        <table class="table table-striped table-bordered table-hover table-responsive table-condensed">
            <thead style="">
            <tr>
                <td class="text-center" align="center">
                    Quantidade
                </td>
                <td class="text-center" align="left">
                    Iniciativa
                </td>
            </tr>
            </thead>
            <tbody id="table-pendencia-body">
            <?php foreach ($this->arIniciativa as $iniciativa):?>
                <tr>
                    <td align="center">
                        <span class="label label-primary"><?= $iniciativa['quantidade']; ?></span>
                    </td>
                    <td align="left">
                        <?= $iniciativa['iniciativa']; ?>
                    </td>
                </tr>
            <?php endforeach;?>
            </tbody>
        </table>
</div>
<?php else:?>
    <div class="alert alert-info"><p align="center">Nenhum registro encontrado</p></div>
<?php endif;?>


<?php if (!empty($this->arSituacao)):?>
<div style="height:300px;overflow-x:scroll;">
    <table class="table table-striped table-bordered table-hover table-responsive table-condensed table-head-fixed content">
        <thead style="">
        <tr>
            <td class="text-center">Situação</td>
            <td class="text-center">Tipo</td>
            <td class="text-center">Quantidade</td>
        </tr>
        </thead>
        <tbody id="table-pendencia-body">
        <?php foreach ($this->arSituacao as $situacao):?>
            <tr id="tr_<?= $situacao['inuid']; ?>">
                <td align="center"><?= $situacao['situacao']; ?></td>
                <td align="center"><?= $situacao['tipo']; ?></td>
                <td align="center">
                    <?php if($situacao['tipo'] == 'PAR'):?>
                        <a class="btn btn-primary btn-sm" onclick="carregarDetalheSituacaoPar(<?= $situacao['esdid'];?>)"><?= $situacao['quantidade']; ?> <i class="fa fa-eye"></i></a>
                    <?php else:?>
                        <span class="label label-primary"><?= $situacao['quantidade']; ?></span>
                    <?php endif;?>
                </td>
            </tr>
        <?php endforeach;?>
        </tbody>
    </table>
</div>
<?php else:?>
    <div class="alert alert-info"><p align="center">Nenhum registro encontrado</p></div>
<?php endif;?>

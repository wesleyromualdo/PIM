<?php
$tot_itens = 0;
foreach($arrDadosItensIniciativa as $item){

    $item['ano'] = '2016';
    $tot_item = $item['icovalor']*$item['qtd_itens'];
    $tot_itens += $tot_item;
?>
<div class="panel-group" id="accordion_<?php echo $item['icoid']?>">
    <div class="panel panel-default">
        <div class="panel-heading">
            <h4 class="panel-title">
                <a data-toggle="collapse"
                    data-parent="#accordion_<?php echo $item['icoid']?>"
                    href="#qtdItemIniciativaEscola_<?php echo $item['icoid']?>">
                    <div class="row">
                        <div class="col-lg-1">
                            <i class="fa fa-pencil-square-o" aria-hidden="true" style="font-size: 30px;"></i>
                        </div>
                        <div class="col-lg-5">
                        <?php echo $item['icodescricao']?>
                        </div>
                        <div class="col-lg-3">
                        Valor unitário: R$ <?php echo formata_valor($item['icovalor']);?><br>
                        Quantidade: <?php echo $item['qtd_itens'] > 0 ? $item['qtd_itens'] : 0;?>
                        <input  type="hidden"
                                id="icovalor_<?php echo $item['icoid']?>"
                                value="<?php echo $item['icovalor']?>" />
                        </div>
                        <div class="col-lg-3">
                        Valor total: R$ <?php echo formata_valor($tot_item);?>
                        </div>
                    </div>
                </a>
            </h4>
        </div>
        <div id="qtdItemIniciativaEscola_<?php echo $item['icoid']?>" class="panel-collapse collapse">
            <div class="panel-body">
                <div class="row">
                    <div class="col-lg-12">
                    <?php echo $this->formQtdItensIniciativaEscola($item, $disabled); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<?php
}
?>
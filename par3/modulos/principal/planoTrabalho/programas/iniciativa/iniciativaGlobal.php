<?php
if($dados['habilitado'] == 'true'){
?>
<div class="panel-group" id="accordionPropostaItens">
    <div class="panel panel-default">
        <div class="panel-heading">
            <h4 class="panel-title">
                <a data-toggle="collapse" data-parent="#accordionPropostaItens" href="#propostaItens">Adicionar Itens de Composição</a>
            </h4>
        </div>
        <div id="propostaItens" class="panel-collapse collapse">
            <div class="ibox-content">
                <div class="row">
                    <div class="col-lg-12">
                        <?php $controlIniciativa->formItensPropostaIniciativa($arrDadosIniciativa, $disabled); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<?php
}
?>
<div class="panel-group" id="accordionItemIniciativa">
    <div class="panel panel-default">
        <div class="panel-heading">
            <h4 class="panel-title">
                <a data-toggle="collapse" data-parent="#accordionItemIniciativa" href="#itemIniciativa">Itens de Composição</a>
                <div style="float:right">Valor total da iniciativa: R$ <label id="totalItens"><?php echo formata_valor($arrDadosIniciativa['valor_iniciativa']); ?></label></div>
            </h4>
        </div>
        <div id="itemIniciativa" class="panel-collapse collapse">
            <div class="panel-body">
                <div class="row">
                    <div class="col-lg-12" id="div_ItensIniciativa">
                        <?php $controlIniciativa->formItensIniciativa($arrDadosIniciativa, $disabled); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
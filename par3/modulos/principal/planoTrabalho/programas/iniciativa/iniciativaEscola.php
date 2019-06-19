<?php
if($dados['habilitado'] == 'true'){
?>
<div class="panel-group" id="accordionEscolasMunicipio">
    <div class="panel panel-default">
        <div class="panel-heading">
            <h4 class="panel-title">
                <a data-toggle="collapse" data-parent="#accordionEscolasMunicipio" href="#escolasMunicipio">
                    <i class="fa fa-plus" aria-hidden="true" >Adicionar Escolas</i>
                </a>
            </h4>
        </div>
        <div id="escolasMunicipio" class="panel-collapse collapse">
            <div class="ibox-content">
                <div class="row">
                    <div class="col-lg-12">
                        <?php
                        $sql = $modelMunicipios->pegarSQLSelect($modelUnidade->estuf, $modelUnidade->muncod);
                        echo $simec->select('muncod', 'Município', '', $sql, array('multiple'));
                        ?>
                    </div>
                </div>
                <div class="row">
                    <div class="col-lg-12" id="div_escolasMunicipio" >
                    <?php echo $controlIniciativa->formEscolasMunicipio($arrDadosIniciativa, $disabled); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<?php
}
?>
<div class="panel-group" id="accordionEscolasIniciativa">
    <div class="panel panel-default">
        <div class="panel-heading">
            <h4 class="panel-title">
                <a data-toggle="collapse" data-parent="#accordionEscolasIniciativa" href="#escolasIniciativa">Lista de Escolas</a>
            </h4>
        </div>
        <div id="escolasIniciativa" class="panel-collapse collapse">
            <div class="ibox-content">
                <div class="row">
                    <div class="col-lg-12" id="div_escolasIniciativa" >
                    <?php echo $controlIniciativa->listaEscolasIniciativa($arrDadosIniciativa, $disabled); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<?php
if($dados['habilitado'] == 'true'){
?>
<div class="panel-group" id="accordionPropostaItens">
    <div class="panel panel-default">
        <div class="panel-heading">
            <h4 class="panel-title">
                <a data-toggle="collapse" data-parent="#accordionPropostaItens" href="#propostaItens">
                    <i class="fa fa-plus" aria-hidden="true" > Adicionar Itens de Composição</i>
                </a>
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
<div class="panel-group" id="accordionItemIniciativaEscola">
    <div class="panel panel-default">
        <div class="panel-heading">
            <h4 class="panel-title">
                <a data-toggle="collapse" data-parent="#accordionItemIniciativaEscola" href="#itemIniciativaEscola">Itens de Composição por Escola</a>
                <div style="float:right">Valor total da iniciativa: R$ <label id="totalItens"><?php echo formata_valor($arrDadosIniciativa['valor_iniciativa']); ?></label></div>
            </h4>
        </div>
        <div id="itemIniciativaEscola" class="panel-collapse collapse">
            <div class="panel-body">
                <div class="row">
                    <div class="col-lg-12" id="div_ItensIniciativaEscola">
                        <?php $controlIniciativa->formItensIniciativaEscola($arrDadosIniciativa, $disabled); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<?php
$controllerVn = new Par3_Controller_VinculacaoNutricionista();
?>
<div class="row">
    <div class="col-lg-4">
        <div class="ibox">
            <div class="ibox-title">
                <h5>Tipos de Nutricionistas</h5>
            </div>

            <div class="ibox-content">
                <?php
                    $controllerVn->desenhaGrafico(1);
                ?>
            </div>
            <div class="ibox-footer">
                <button class="btn btn-block" onclick="filtrarChart('tenid')">Filtrar</button>
            </div>
        </div>
    </div>

    <div class="col-lg-4">
        <div class="ibox">
            <div class="ibox-title">
                <h5>Situação dos Nutricionistas</h5>
            </div>

            <div class="ibox-content">
                <?php $controllerVn->desenhaGrafico(2); ?>
            </div>
            <div class="ibox-footer">
                <button class="btn btn-block" onclick="filtrarChart('snid')">Filtrar</button>
            </div>
        </div>
    </div>

    <div class="col-lg-4">
        <div class="ibox">
            <div class="ibox-title">
                <h5>Situação das Entidades</h5>
            </div>

            <div class="ibox-content">
                <?php $controllerVn->desenhaGrafico(3); ?>
            </div>
            <div class="ibox-footer">
                <button class="btn btn-primary btn-block" onclick="gerarRelatorio()"><i class="fa fa-file-excel-o"></i> Gerar Relatório</button>
            </div>
        </div>
    </div>
</div>

<div id="09a"></div>
<div class="row" style="margin-top: 70px;">
    <div class="col-lg-12">
        <div class="panel panel-primary">
            <div class="panel-heading">
                <?php $ide->getTitulo(1, 'Tab09Uf', 'Tab09Mu'); ?>
            </div>
            <div class="panel-body">
                <?php $ide->getTabela(1, 'Tab09Uf', 'Tab09Mu'); ?>
                <p class="text-right">fonte: INEP</p>
            </div>
        </div>
    </div>
</div>

<div id="09b"></div>
<div class="row" style="margin-top: 70px;">
    <div class="col-lg-12">
        <div class="panel panel-success">
            <div class="panel-heading">
                <?php $ide->getTitulo(2, 'Tab09Uf', 'Tab09Mu'); ?>
            </div>
            <div class="panel-body">
                <?php $ide->getTabela(2, 'Tab09Uf', 'Tab09Mu'); ?>
                <p class="text-right">fonte: INEP</p>
            </div>
        </div>
    </div>
</div>

<?php if(empty($ide->muncod)){ ?>
<div id="09c"></div>
<div class="row" style="margin-top: 70px;">
    <div class="col-lg-12">
        <div class="panel panel-success">
            <div class="panel-heading">
                <?php $ide->getTitulo(1, 'Tab09cUf', ''); ?>
            </div>
            <div class="panel-body">
                <?php $ide->getTabela(1, 'Tab09cUf', ''); ?>
                <p class="text-right">fonte: INEP</p>
            </div>
        </div>
    </div>
</div>

<div class="row" style="margin-top: 70px;">
    <div class="col-lg-12">
        <div class="panel panel-success">
            <div class="panel-heading">
                <?php $ide->getTitulo(2, 'Tab09cUf', ''); ?>
            </div>
            <div class="panel-body">
                <?php $ide->getTabela(2, 'Tab09cUf', ''); ?>
                <p class="text-right">fonte: INEP</p>
            </div>
        </div>
    </div>
</div>

<div class="row" style="margin-top: 70px;">
    <div class="col-lg-12">
        <div class="panel panel-success">
            <div class="panel-heading">
                <?php $ide->getTitulo(3, 'Tab09cUf', ''); ?>
            </div>
            <div class="panel-body">
                <?php $ide->getTabela(3, 'Tab09cUf', ''); ?>
                <p class="text-right">fonte: INEP</p>
            </div>
        </div>
    </div>
</div>

<div class="row" style="margin-top: 70px;">
    <div class="col-lg-12">
        <div class="panel panel-success">
            <div class="panel-heading">
                <?php $ide->getTitulo(4, 'Tab09cUf', ''); ?>
            </div>
            <div class="panel-body">
                <?php $ide->getTabela(4, 'Tab09cUf', ''); ?>
                <p class="text-right">fonte: INEP</p>
            </div>
        </div>
    </div>
</div>

<?php } ?>

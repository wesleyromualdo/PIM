<?php
if (in_array('todas', $tables)) {
    $tables = array();
    $tabelas = $modelGerador->getTablesBySchema($_GET['schema']);
    foreach ($tabelas as $tabela) {
        $tables[] = $tabela['table'];
    }
}
?>
<div class="row">
    <div class="col-lg-5">
        <div class="panel panel-default">
            <div class="panel-body">
                <p>Raiz da app: <b><?= $appraiz ?></b></p>

                <h4>Tabelas selecionadas:</h4>
                <ul>
                    <?php foreach ($tables as $table): ?>
                        <li><?= $table; ?></li>
                    <?php endforeach; ?>
                </ul>
            </div>
        </div>
    </div>

    <div class="col-lg-6">
        <div class="panel panel-default">
            <div class="panel-heading">
                <h3>Gerar Model(s) e View(s)</h3>

                <p>Esquema selecionado: <i><?= $schema ?></i></p>
            </div>
            <div class="panel-body">

                <form method="get">
                    <input type="hidden" name="schema" value="<?= $schema; ?>"/>
                    <?php foreach ($tables ? $tables : array() as $table) : ?>
                        <input type="hidden" name="tables[]" value="<?= $table ?>"/>
                    <?php endforeach; ?>

                    <div class="form-group">
                        <label for="path">Caminho dos arquivos:</label>
                        <input readonly type="text" name="path" id="path" class="form-control"
                               value="www\<?= $schema; ?>\classes\"/>
                    </div>

                    <div class="form-group">
                        <label for="extension">Extensão das classes:</label>
                        <select name="extension" id="extension" class="form-control">
                            <option>.class.inc</option>
                            <option>.php</option>
                        </select>
                    </div>

                    <div class="form-group">
                        <label for="prefix">Prefixo da classe:</label>
                        <input type="text" name="prefix" id="prefix" class="form-control"
                               value="<?= ucfirst($schema) ?>_"/>

                        <p class="help-block"><i>prefixo</i>Nomeclasse</p>
                    </div>

                    <div class="form-group">
                        <label for="include">Incluir require para Modelo:</label>
                        <input type="radio" name="include" id="include_s" value="s"/><label for="include_s">Sim</label>
                        <input type="radio" name="include" id="include_n" value="n" checked/><label
                            for="include_n">Não</label>
                    </div>

                    <button class="btn btn-lg btn-success btn-block" type="submit" name="gerar_arquivos" value="sim">
                        Gerar
                    </button>
                </form>

            </div>
        </div>
    </div>
</div>



<div class="container">
    <div class="row vertical-offset-100">
        <div class="col-lg-4 col-lg-offset-4">
            <div class="panel panel-default">
                <div class="panel-heading">
                    <h3>Selecione o Esquema</h3>
                </div>
                <div class="panel-body">
                    <form method="get">

                        <div class="form-group">
                            <label for="schema">Esquema</label>
                            <select name="schema" id="schema" class="form-control" data-placeholder="selecione">
                                <?php
                                $schemas = $modelGerador->getSchemas();

                                foreach ($schemas as $schema) {
                                    echo "<option>{$schema['schema']}</option>";
                                }
                                ?>
                            </select>
                        </div>

                        <div class="form-group">
                            <label for="tables">Tabelas</label>
                            <select class="form-control" name="tables[]" id="sel_tables" data-placeholder="selecione"
                                    multiple>
                                <option value="todas"> Selecione um Esquema</option>
                                <?= $modelGerador->getComboTables($schemas[0]['schema']) ?>
                            </select>
                        </div>

                        <button class="btn btn-lg btn-success btn-block" type="submit">Avançar »</button>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
<script>
    $(function () {
        $('#schema').on('change', function () {
            var idSchema = $(this).val();
            $.post($(location).attr('href'), {schema: idSchema, action: 'getComboTabela'}, function (result) {
                $("#sel_tables").html(result);
                $("#sel_tables").trigger("chosen:updated");
            });
        })
    });
</script>
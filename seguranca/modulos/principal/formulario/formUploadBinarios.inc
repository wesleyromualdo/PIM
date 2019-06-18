<script language="JavaScript" src="../includes/jquery.js"></script>
<style type="text/css">
body {
    overflow: hidden;
}
</style>
<div class="col-xs-12">
    <br>
    <legend>
        <h4>Upload de arquivos binários do SIMEC</h4>
    </legend>

    <!-- resultado -->
    <?php
    echo '<br>'.$fm->getMensagens();

    if($_POST['arqid']):?>
    <div class="row">
        <h4 class="col-sm-offset-2">arqid:
            <span class="text-primary">
                <?php echo "{$_POST['arqid']}" ?>
            </span>
        </h4>

        <h4 class="col-sm-offset-2">Esquema:
            <span class="text-primary">
                <?php echo "{$_POST['esquema']}" ?>
            </span>
        </h4>

        <h4 class="col-sm-offset-2">Nome:
            <span class="text-primary">
                <?php echo "{$_FILES['arqbinario']['name']}" ?>
            </span>
        </h4>

        <h4 class="col-sm-offset-2">Tipo:
            <span class="text-primary">
                <?php echo "{$_FILES['arqbinario']['type']}" ?>
            </span>
        </h4>

        <h4 class="col-sm-offset-2">Size:
            <span class="text-primary">
                <?php echo "{$_FILES['arqbinario']['size']}" ?>
            </span>
        </h4>
    </div>

    <div class="form-group">
        <div class="row">
            <button class="btn btn-primary" onclick="window.location.href=''">
                <i class="glyphicon glyphicon-chevron-left"></i>
                Voltar
            </button>
        </div>
    </div>
    <!-- </div> -->
    <?php endif?>
    <!-- resultado -->

    <!-- formulário -->
    <?php if (empty($_POST['arqid'])):?>
    <form name="fomrManBinarios" id="fomrManBinarios" enctype="multipart/form-data" method="POST" class="form-horizontal" >

        <div class="form-group">
            <label class="col-sm-2 control-label">
                Esquema
            </label>

            <div class="col-md-6">
                <select id="esquema" name="esquema" class="form-control">
                    <option value="">Selecione o esquema</option>
                    <?php foreach (getSchemas() as $schema) {
                        echo "<option>{$schema['schema']}</option>";
                    }?>
                </select>
            </div>
        </div>

        <div class="form-group">
            <label class="col-sm-2 control-label">
                Arquivo
            </label>

            <div class="col-sm-9">
                <input id="file" type="file" title="Carregar Arquivo" name="arqbinario" class="btn btn-primary start" />
            </div>
        </div>

        <div class="form-group">
            <div class=" col-lg-10">
                <button  name="upload" id="upload" value="salvar" class="btn btn-primary" type="submit">
                    <i class="glyphicon glyphicon-upload"></i>
                    Carregar
                </button>
            </div>
        </div>
    </form>
    <?php endif ?>
    <!-- formulário -->
</div>

<script>
    // populate
    $('#esquema').val('<?php echo $_POST['esquema'] ?>');
</script>

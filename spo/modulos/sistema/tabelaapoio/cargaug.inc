<?php

require_once APPRAIZ . "spo/classes/controller/CargaUG.php";
include APPRAIZ . "includes/cabecalho.inc";

$controller = new Spo_Controller_CargaUG();
?>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Carga de Unidade Gestora</li>
    </ol>
    <form method="POST" class="form-horizontal" role="form" name="form-pesquisa" id="form-pesquisa">
        <div class="well">
            <?php $controller->renderForm(); ?>
        </div>

        <div id="div-listagem">
            <?php $controller->renderListagem(); ?>
        </div>
    </form>
</div>

<script src="js/cargaug.js"></script>
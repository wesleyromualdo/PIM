<?php

$controller = new Spoemendas_Controller_CarregaDadosEmenda();

include APPRAIZ . 'includes/cabecalho.inc';
(new Simec_View_Breadcrumb())
    ->add('Carga Emenda')
    ->render();
echo $controller->getFlashMessage();
?>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../spoemendas/js/carregaDadosEmenda.js"></script>
<div class="col-lg-12" id="divForm">
    <?php $controller->renderForm(); ?>
</div>
<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 7/11/2017
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . 'includes/library/simec/view/Helper.php';
initAutoload();
require_once APPRAIZ .'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
*/
?>
<table class="table table-responsive table-bordered">
    <?php $this->montaTabelaComFiltros($arrRequest,$this->ide,$descrMuniOrEstad); ?>
</table>

<?php
require APPRAIZ . 'obras2/includes/principal/listaExecFinanceira/ctrl.php';
require APPRAIZ . 'obras2/includes/principal/listaExecFinanceira/css.php';


?>

 

<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<?php

$execucaoFinanceira = new ExecucaoFinanceira();
$verificarExecucaoFinanceira = $execucaoFinanceira->verificarExecucaoFinanceiraFinalizada($obrid);
$execucaoFinanceiraFinalizada = $verificarExecucaoFinanceira > 0 ? true : false;

echo $execucaoFinanceira->criaSubAba($url, $habilitado, $obrid);

?>

<div class="abaContainer">
    <div class="abaContent">
        <?php if(!$execucaoFinanceiraFinalizada){?>
        <br />
        <button class="btn btn-primary" onclick="incluirMedicao()">Incluir Medição</button>
        <br /><br />
        <?php } ?>
        <div class="construtoraContent">
            <?php
            $arrConstrutoras = $execucaoFinanceira->montarMedicao($obrid);
            ?>
        </div>
    </div>
</div>

<br />

<?php  
    require APPRAIZ . 'obras2/includes/principal/listaExecFinanceira/js.php';
?>

<?php if ($cabecalho_painel==true) : ?>
<?php   include APPRAIZ . 'www/painel/novo/cabecalho_painel.php'; ?>
<?php else: ?>
<?php   if ($painelCabecalho) : ?>
<?php       include APPRAIZ."includes/painel_slide_ajax.inc"; ?>
<?php   else: ?>
    <link href="../library/sliding_panel/css/slide.css" rel="stylesheet" media="screen">
    <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="UTF-8"></script>
    <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
    <script src="../library/chosen-1.0.0/chosen.jquery.js"></script>
    <script language="javascript">$1_11 = $.noConflict();</script>
<?php   endif  ?>
<?php endif  ?>
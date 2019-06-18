<!--<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>-->
<!--<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>-->
<style>
@media print {.notprint { display: none } .div_rolagem{display: none} }
@media screen {.notscreen { display: none; }
.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
</style>
<table id="termo" width="95%" align="left" border="0" cellpadding="3" cellspacing="1">
    <tr>
        <td style="font-size: 12px; font-family:arial;">
            <div>
            <?php echo html_entity_decode($html, ENT_QUOTES, 'ISO-8859-1'); ?>
            </div>
        </td>
    </tr>
</table>
<br>
<br>
<br>
<?php if( $dados['terassinado'] == 't' ){ ?>
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1">
    <tr style="text-align: center;">
        <td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
            <b>Validado <?=$nome ?></b>
        </td>
    </tr>
</table>
<?php } ?>
<script>
    $('#termo .P_2').css('margin-left','0.5in');
    $('#termo p').css('margin-left','0px');
</script>
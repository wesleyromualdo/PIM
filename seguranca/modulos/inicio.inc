<?
foreach ($_REQUEST as $k => $v) ${$k} = $v;
include APPRAIZ . "includes/cabecalho.inc";
?>
    <br>
    <link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
    <script language="JavaScript" src="../includes/funcoes.js"></script>
<?
$db->cria_aba($abacod_tela, $url, '');

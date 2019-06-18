<?php 
require APPRAIZ . 'obras2/includes/principal/monitoramentoEspecial/funcoes.php';

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];



include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";


$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);

echo cabecalhoObra($obrid);

//monta_titulo($titulo_modulo, '');
monta_titulo_obras($titulo_modulo, '');


$sql = getSqlMonitoramentoEspecial($obrid); 

$atividades = $db->carregar($sql);


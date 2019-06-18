<?php 
require APPRAIZ . 'obras2/includes/principal/Desembolso/funcoes.php';



if (empty($_SESSION['obras2']['obrid']) && !empty($_GET['obrid'])) {
    if (!empty($_GET['obrid'])) {
        $_SESSION['obras2']['obrid'] = $_GET['obrid'];
    }
    if (empty($_SESSION['obras2']['empid'])) {
        $o = new Obras();
        $o->carregarPorIdCache($_SESSION['obras2']['obrid']);
        $_SESSION['obras2']['empid'] = $o->empid;
    }
}

// empreendimento || obra || orgao
verificaSessao('obra');

$obrid = $_SESSION['obras2']['obrid'];
$obra = new Obras();
$obra->carregarPorIdCache($obrid);

include_once APPRAIZ . "includes/cabecalho.inc";

// Monta as abas e o título da tela
print "<br/>";
if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
}
echo cabecalhoObra($obrid);
monta_titulo_obras("Solicitação de Desembolso", "");


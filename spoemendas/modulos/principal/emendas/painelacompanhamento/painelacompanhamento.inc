<?php
/**
 * $Id: painelacompanhamento.inc 124331 2017-06-02 14:16:11Z victormachado $
 */
include APPRAIZ . "includes/cabecalho.inc";
/* Funções de Gráficos do Cockpit */
include_once APPRAIZ . 'includes/_funcoes_cockpit.php';
// -- Tratamento de perfis
$perfis = pegaPerfilGeral();
?>
<style>
    rigth{float:right}
    .text-center{text-align:left !important;}
</style>
<ol class="breadcrumb">
    <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
    <li class="active"> Acompanhamento das Emendas </li>
</ol>
<br>
<script type="text/javascript" lang="javascript">
    $(document).ready(function () {
        $(function () {
            $('[data-toggle="tooltip"]').tooltip();
        });
    });
</script>
<?php
$abas = new Simec_Abas('spoemendas.php?modulo=principal/emendas/painelacompanhamento/painelacompanhamento&acao=A');
$perfis = pegaPerfilGeral($_SESSION['usucpf']);
if (!in_array(PFL_PARLAMENTAR, $perfis)) {
    $abas->adicionarAba('vinculacaoiniciativas', "Vinculação de Iniciativas em {$_SESSION['exercicio']}", dirname(__FILE__) . '/vinculacaoiniciativas/vinculacaoiniciativas.inc', 'tags');
    $abas->adicionarAba('impedimentos', "Lançamento dos Programas de Trabalhos / Impedimento {$_SESSION['exercicio']}", dirname(__FILE__) . '/impedimentos/impedimentos.inc', 'th-list');
    $abas->adicionarAba('secretarias', "Avaliação das Secretarias {$_SESSION['exercicio']}", dirname(__FILE__) . '/secretarias/secretarias.inc', 'ok');
    $abas->adicionarAba('limiteempenhado', "Limites Empenhados em {$_SESSION['exercicio']}", dirname(__FILE__) . '/limiteempenhado/limiteempenhado.inc', 'usd');
    $abas->definirAbaDefault('limiteempenhado');
}
$abas->adicionarAba('parlamentar', "Painel por Parlamentar", dirname(__FILE__) . '/parlamentar/parlamentar.inc', 'user');
$abas->definirAbaDefault('parlamentar');
require $abas->render(true);
?>

<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
<link rel="stylesheet" href="css/progorc.css">

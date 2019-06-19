<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include "classes/Gerador.php";
include "classes/ModelGenerator.php";
include "classes/ViewGenerator.php";
include "classes/ControllerGenerator.php";

$modelGerador = new Gerador();
$schema = $_GET['schema'];
$tables = $_GET['tables'];
$gerarArquivos = $_GET['gerar_arquivos'];
$appraiz = APPRAIZ;

$menuEsquema = empty($schema);
$menuTabela = empty($tables) && !empty($schema);
$menuInfo = empty($gerarArquivos) && !empty($tables);
$menuConclusao = $gerarArquivos == 'sim';

$action = $_POST['action'];
if (!empty($action)) {
    switch ($action) {
        case 'getComboTabela':
            echo $modelGerador->getComboTables($_POST['schema']);
            exit();
            break;
    }
}
?>

<html>
<head>
    <link href="/library/chosen-1.0.0/chosen.css" rel="stylesheet" media="screen"></link>
    <link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
    <script src="/library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
    <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>

    <link href="https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css" rel="stylesheet"
          integrity="sha256-k2/8zcNbxVIh5mnQ52A0r3a6jAgMGxFJFE2707UxGCk= sha512-ZV9KawG2Legkwp3nAlxLIVFudTauWuBpC10uEafMHYL0Sarrz5A7G79kXh5+5+woxQ5HM559XX2UZjMJ36Wplg=="
          crossorigin="anonymous">
</head>

<body style="font-family: 'Open Sans', sans-serif;">

<div class="container">

    <h2>Gerador de Código - SIMEC</h2>
    <?php
    if ($menuEsquema): ?>
        <div class="row setup-content" id="step-1">
            <div class="col-lg-12 well text-center">
                <?php include_once('views/formEsquema.php'); ?>
            </div>
        </div>

    <?php elseif ($menuInfo): ?>
        <div class="row setup-content" id="step-3">
            <div class="col-lg-12 well">
                <?php include_once('views/formInfo.php'); ?>
            </div>
        </div>

    <?php elseif ($menuConclusao): ?>
        <div class="row setup-content" id="step-2">
            <div class="col-md-12 well text-center">
                <?php include_once('gerarArquivos.php'); ?>
            </div>
        </div>
    <?php endif; ?>
</div>

<?php include_once('views/rodape.php'); ?>
</body>
</html>

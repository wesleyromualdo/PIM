<?php

//Requisições/'requisicao': contrato.php - controlarRequisicoes -> Controller/ExecucaoContrato.class.inc
//$proid -> contrato.php

$modelExecucaoContrato = new Par3_Model_ExecucaoContrato();

$municipios = $modelExecucaoContrato->retornaSqlMunicipios($proid,$_GET['inuid']);
$itens = $modelExecucaoContrato->retornaSqlItens($proid,$_GET['inuid']);
$localizacao = $modelExecucaoContrato->retornaSqlLocalizacao($proid,$_GET['inuid']);
$totalEscolas = $modelExecucaoContrato->retornaTotalEscolas($proid, $dadosProcesso['ineid']);

global $simec;
?>

<div style="max-width: 600px; margin: 0 auto;">

    <?php
        echo $simec->select('muncod', 'Município', null, $municipios);
        echo $simec->select('ipiid', 'Item', null, $itens);
        echo $simec->select('escno_localizacao', 'Localização', null, $localizacao);
        echo $simec->input('esccodinep', 'Código Inep', null, array(), array());
    ?>

    <div class="center">
        <button class="btn btn-success" type="button" id="filtrarescolas" onclick="atualizarListaEscolasTermo(1)"> Filtrar</button>
        <button class="btn btn-default" type="button" id="filtrarescolas" onclick="limparFiltroEscolasTermo()"> Resetar</button>
        <button class="btn btn-default" type="button" id="filtrarescolas" onclick="mostrarTodasEscolas()"> Mostrar todos (<?= $totalEscolas ?>) </button>
    </div>

</div>
<hr>
<div id="listaEscolas">

</div>


<script>
    $(function() {

        $('a[href="#escolas"]').on('click', function () {
            if (!$(this).parent().hasClass('active')) {
                $('#listaEscolas').html('');
                atualizarListaEscolasTermo(1);
            }
        });

    });

    function gerarExcelEscolasTermo(){
        window.location.href = window.location.href+'&reqdownload=excelescolastermo';
    }

    function atualizarListaEscolasTermo(pagina){
        var muncod = $('#muncod').val();
        var ipiid = $('#ipiid').val();
        var escno_localizacao = $('#escno_localizacao').val();
        var esccodinep = $('#esccodinep').val();

        $.post(window.location.href, {requisicao: "listaEscolasTermo",
            funcao: 'contrato',
            muncod: muncod,
            ipiid: ipiid,
            escno_localizacao: escno_localizacao,
            esccodinep: esccodinep,
            pagina: pagina}, function (resp) {

            $('#listaEscolas').html(resp);

        });
    }

    function mostrarTodasEscolas(){
        $.post(window.location.href, {requisicao: "listaEscolasTermo",
            funcao: 'contrato',
            pagina: 0,
            mostrarTodas: true}, function (resp) {

            $('#listaEscolas').html(resp);

        });
    }

    function limparFiltroEscolasTermo(){
        $('#muncod').val('').trigger("chosen:updated");
        $('#ipiid').val('').trigger("chosen:updated");
        $('#escno_localizacao').val('').trigger("chosen:updated");
        $('#esccodinep').val('');
        atualizarListaEscolasTermo(1);
    }
</script>
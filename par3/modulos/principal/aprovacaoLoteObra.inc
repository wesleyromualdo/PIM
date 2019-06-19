<?php
global $simec;
$arrBtn = array();
?>
<h3 align="center">Situação Atual: <b><?= $arrEsd['esddsc']?></b></h3>
<hr>
<h3>Obras: a serem tramitadas:</h3>
<table class="table table-bordered table-responsive table-hover">
    <tr class="info" align="center">
        <td><b>ID</b></td>
        <td><b>Nome</b></td>
        <td><b>Tipo</b></td>
        <td><b>Município</b></td>
        <td><b>UF</b></td>
        <td><b>Tramitação</b></td>
        <td><b>Analista</b></td>
        <td><b>Valor Obra</b></td>
    </tr>
<?php foreach($arrObras as $obra):?>
<tr align="center">
    <td><?= $obra['obrid'];?></td>
    <td><?= $obra['obrdsc'];?></td>
    <td><?= $obra['otpdsc'];?></td>
    <td><?= ($obra['itrid']== 2 ?$obra['inudescricao']:' - ');?></td>
    <td><?= $obra['estuf'];?></td>
    <td><?= wf_pegarUltimaDataModificacao($obra['docid']);?></td>
    <td><?= wf_pegarUltimoUsuarioModificacao($obra['docid'])['usunome'];?></td>
    <td><?= formata_numero_monetario($obra['obrvalor']);?></td>
</tr>
<?php endforeach;?>
</table>
<h3 align="center">Situações disponíveis:</h3>
<div class="row center">

<?php foreach($arrProximosEstados as $estado):?>
<?php
    if(!wf_acaoPossivel2($docid,$estado['aedid'],$estado)){
        continue;
    }
    switch ($estado['aedicone']) {
        case '1.png':
            $arrBtn =  array(' glyphicon-thumbs-up', "btn-success");break;
        case '2.png':
            $arrBtn = array(' glyphicon-thumbs-down', "btn-danger");break;
        case '3.png':
            $arrBtn = array(' glyphicon-share-alt', "btn-warning");break;
        default:
            $arrBtn = array(' glyphicon-share-alt', "btn-default");break;
    }
?>
<div class="row center">
    <button
        onclick='ajax_informarSituacao(<?= json_encode($arrDocid)?>,<?=$estado['aedid']?>,<?= wf_acaoNecessitaComentario2( $estado['aedid'] )?1:0;?>)'
        class="btn <?= $arrBtn[1]; ?> col-lg-offset-3 col-lg-6">
        <?= $estado['aeddscrealizar'];?> <span class="glyphicon <?= $arrBtn[0]; ?>" align="left"></span>
    </button>
</div>

<?php endforeach;?>
</div>

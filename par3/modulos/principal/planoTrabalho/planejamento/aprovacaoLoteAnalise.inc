<?php
global $simec;
$arrBtn = array();
?>
<h3 align="center">Situação Atual: <b><?= $arrEsd['esddsc']?></b></h3>
<hr>
<!-- <h3>Itens: a serem tramitados:</h3> -->
<!-- <table class="table table-bordered table-responsive table-hover"> -->
<!--     <tr class="info" align="center"> -->
<!--         <td><b>ID</b></td> -->
<!--         <td><b>Nome</b></td> -->
<!--         <td><b>Tipo</b></td> -->
<!--         <td><b>Município</b></td> -->
<!--         <td><b>UF</b></td> -->
<!--         <td><b>Tramitação</b></td> -->
<!--         <td><b>Analista</b></td> -->
<!--         <td><b>Valor Obra</b></td> -->
<!--     </tr> -->
<?php //foreach($arrAnalise as $analise):?>
<!-- <tr align="center"> -->
<!--     <td> //$analise['obrid'];</td> -->
<!--     <td>//$analise['obrdsc'];</td> -->
<!--     <td> //$analise['otpdsc'];</td> -->
<!--     <td> //($analise['itrid']== 2 ?$analise['inudescricao']:' - ');</td> -->
<!--     <td> //$analise['estuf'];</td> -->
<!--     <td> //wf_pegarUltimaDataModificacao($analise['docid']);</td> -->
<!--     <td> //wf_pegarUltimoUsuarioModificacao($analise['docid'])['usunome'];</td> -->
<!--     <td> //formata_numero_monetario($analise['obrvalor']);</td> -->
<!-- </tr> -->
<?php //endforeach;?>
<!-- </table> -->
<h3 align="center">Situações disponíveis:</h3>
<div class="row center">


<?php
    $i = -1;
    foreach($arrProximosEstados as $estado):
    $i++;

    if(!wf_acaoPossivel2($docid,$estado['aedid'],$this->verificarParametrosPosAcao($arrAnaId[$i]))){
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
        onclick='ajax_informarSituacao(<?= simec_json_encode($arrDocid)?>,<?=$estado['aedid']?>,<?= wf_acaoNecessitaComentario2( $estado['aedid'] )?1:0;?>,"<?= $estado['aeddscrealizar'];?>", "<?= $arrEsd['esddsc']?>")'
        class="btn <?= $arrBtn[1]; ?> col-lg-offset-3 col-lg-6">
        <?= $estado['aeddscrealizar'];?> <span class="glyphicon <?= $arrBtn[0]; ?>" align="left"></span>
    </button>
</div>

<?php endforeach;?>
</div>

<?php
$arMeses = array(
    '1' => 'Janeiro',
    '2' => 'Fevereiro',
    '3' => 'Março',
    '4' => 'Abril',
    '5' => 'Maio',
    '6' => 'Junho',
    '7' => 'Julho',
    '8' => 'Agosto',
    '9' => 'Setembro',
    '10' => 'Outubro',
    '11' => 'Novembro',
    '12' => 'Dezembro'
);


$sqlAnos = $mdIniciativa->sqlAnosIniciativaCombo($inp->iniid);

$esdidBloqIniciativa = array(PAR3_ESDID_EM_DILIGENCIA, PAR3_ESDID_EM_ANALISE, PAR3_ESDID_AGUARDANDO_ANALISE);

$disabled = '';
if( $_REQUEST['aba'] == 'analisarPlanejamento' || in_array($mdDocPl->esdid, $esdidBloqIniciativa) ) $disabled = 'disabled';

?>
<div class="row">
    <form name="form-coronograma-iniciativa" id="form-coronograma-iniciativa">
        <div class="form-group">
            <label class="col-sm-1" style="text-align: right;"> Cronograma Físico: </label>
            <? echo $simec->select('inpcronogramamesinicial', null, $inp->inpcronogramamesinicial ,$arMeses, array($disabled), array('input-size' => '2')); ?>
            <label class="col-sm-1" style="text-align: center;"> a </label>
            <? echo $simec->select('inpcronogramamesfinal', null, $inp->inpcronogramamesfinal, $arMeses, array($disabled), array('input-size' => '2')); ?>
            <label class="col-sm-1" style="width: 8%;"> Ano Inicial: </label>
            <? echo $simec->select('inpcronogramaanoinicial', '', $inp->inpcronogramaanoinicial, $sqlAnos, array($disabled), array('input-size' => '2')); ?>
            <label class="col-sm-1" style="text-align: right;"> Ano de término: </label>
            <? echo $simec->select('inpcronogramaano', '', $inp->inpcronogramaano, $sqlAnos, array($disabled), array('input-size' => '2')); ?>
        </div>
    </form>
</div>
<script type="text/javascript">
    $(document).ready(function() {
        <?php if(isset($_REQUEST['inpcronogramamesinicial'])){$selected = $_REQUEST['inpcronogramamesinicial']; ?>
        $('#inpcronogramamesinicial').chosen();
        $('#inpcronogramamesinicial').val(<?php echo $selected; ?>);
        $('#inpcronogramamesinicial').trigger("chosen:updated");

        <?php } ?>

        <?php if(isset($_REQUEST['inpcronogramamesfinal'])){$selected = $_REQUEST['inpcronogramamesfinal']; ?>
        $('#inpcronogramamesfinal').chosen();
        $('#inpcronogramamesfinal').val(<?php echo $selected; ?>);
        $('#inpcronogramamesfinal').trigger("chosen:updated");

        <?php } ?>
        <?php if(isset($_REQUEST['inpcronogramaanoinicial'])){$selected = $_REQUEST['inpcronogramaanoinicial']; ?>
        $('#inpcronogramaanoinicial').chosen();
        $('#inpcronogramaanoinicial').val(<?php echo $selected; ?>);
        $('#inpcronogramaanoinicial').trigger("chosen:updated");

        <?php } ?>
        <?php if(isset($_REQUEST['inpcronogramaano'])){$selected = $_REQUEST['inpcronogramaano']; ?>
        $('#inpcronogramaano').chosen();
        $('#inpcronogramaano').val(<?php echo $selected; ?>);
        $('#inpcronogramaano').trigger("chosen:updated");

        <?php } ?>

    });
</script>
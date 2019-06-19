<?php
$mOed    = new Par3_Model_ObraEstudoDemanda();
$arrOed2 = $mOed->recuperarPorObra($_GET['obrid']);
$mObra   = new Par3_Model_Obra($_GET['orbid']);
$esd     = wf_pegarEstadoAtual($mObra->docid);

$ordem = $arrOed2['omoid'] != 3 ? 2 : 9;
$mpergunta       = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
$disabledAnalise = $mpergunta->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::ESTUDO_DEMANDA,$ordem,$_GET['obrid']);
?>
<div class="construcao">
    <h2>Turnos de Atendimento</h2>


    <?php if($arrOed2['omoid'] != 3) : ?>
        <input type="hidden" id="ordem-2" value="2">
        <div id="carregar-analise-2"></div>
    <?php endif;?>


    <div class="row">
        <?php echo $simec->radio('oedturnos_atendimento', 'Quais turnos de atendimento serão oferecidos para a nova construção no período diurno?',($arrOed2['oedturnos_atendimento']? $arrOed2['oedturnos_atendimento'] : 'I'), array('D' => 'Dois Turnos', 'I' => 'Integral'), array('', 'required',$disabledAnalise), array('type' => 'radio radio-info radio', 'style' => 'inline', 'input-size' => '6', 'label-size' => 5)); ?>
        <?php echo $simec->radio('oednoturno', 'Haverá utilização da escola em periodo noturno?', $arrOed2['oednoturno']?$arrOed2['oednoturno']:'N', array('S' => 'Sim', 'N' => 'Não'), array('', 'required',$disabledAnalise), array('type' => 'radio radio-info radio', 'style' => 'inline', 'input-size' => '6', 'label-size' => 5)); ?>
    </div>
    <div id="div-oednoturno" style="<?php echo ($arrOed2['oednoturno'] == 'S'?'':'display: none;')?>">
        <?php
        $mOem = new Par3_Model_ObraEstudoDemandaModalidade();
        $rsOem = simec_preparar_array($mOem->recuperarTodos('oemid as codigo,oemdsc as descricao'));
        echo $simec->radio('oemid', '',($arrOed2['oemid']? $arrOed2['oemid']: 1 ), $rsOem, array('', 'required',$disabledAnalise), array('type' => 'radio radio-info radio', 'style' => 'inline', 'input-size' => '6', 'label-size' => 5));
        ?>
        <div id="div-outrosoem" style="<?php echo ($arrOed2['oemid'] == 3?'' : 'display: none;')?>">
            <?php echo $simec->textarea('oedmodalidade_noturno_outros', 'Justificativa',$arrOed2['oedmodalidade_noturno_outros'], array('required','maxlength' =>1500,'rows' =>'5',$disabledAnalise), array('label-size' => '2', 'input-size' => '6')); ?>
        </div>
    </div>
</div>

<div class="ampliacao" style="<?php echo ($arrOed2['oedmodalidade_noturno_outros'] == 3?'':'display: none;')?>height:500px;">
    <h2>Informar o Tipo de Ampliação Proposta</h2>

    <?php if($arrOed2['omoid'] == 3): ?>
        <input type="hidden" id="ordem-2" value="9">
        <div id="carregar-analise-9"></div>
    <?php endif;?>

    <div>
        <?php
        $oedquadra = $arrOed2['oedquadra'] && $arrOed2['oedquadra'] != 'N' ? $arrOed2['oedquadra']:'Q';
        echo $simec->radio('oedquadra', '', $oedquadra , array('Q' => 'Quadra', 'C' => 'Cobertura','A' =>'Outras Ampliações'), array('', 'required',$disabledAnalise), array('type' => 'radio radio-info radio', 'style' => 'inline', 'input-size' => '6', 'label-size' => '2')); ?>
    </div>
    <div class="row ampliacaooutros" id="div-ampliacao" style="<?php echo ($arrOed2['oedquadra'] == 'A'?'':'display: none;')?>">
        <?php
        //obra_estudo_demanda_ampliacao
        $mEda  = new Par3_Model_ObraEstudoDemandaAmpliacao();
        $rsEda = simec_preparar_array($mEda->recuperarTodos('edaid as codigo,edadsc as descricao'));
        $arrAmpliacoes = array();
        if($arrOed2['oedid']){
            $mEda = new Par3_Model_ObraEstudoDemandaXampliacao();
            $arrAmpliacoes = $mEda->recuperarAmpliacoes($arrOed2['oedid']);
        }
        echo $simec->select('edaid', '',$arrAmpliacoes, $rsEda, array('maxlength' => '255','multiple'=>'multiple','required',$disabledAnalise), array('input-size' => '12'));
        ?>
    </div>
    <br>
    <div class="row ampliacaooutros"  style="<?php echo ($arrOed2['oedmodalidade_noturno_outros'] == 3?'':'display: none;')?>">
        <?php echo $simec->radio('oedtipoampliacao_outros', 'Outros',($arrOed2['oedtipoampliacao_outros']?$arrOed2['oedtipoampliacao_outros']:'N'), array('S' => 'Sim', 'N' => 'Não'), array('',$disabledAnalise), array('type' => 'radio radio-info radio', 'style' => 'inline', 'input-size' => '2', 'label-size' => '2')); ?>
        <div id="div-oedtipoampliacao_outros" style="<?php echo ($arrOed2['oedtipoampliacao_outros'] == 'S'?'':'display: none;')?>">
            <?php echo $simec->textarea('oedtipoampliacao_outros_dsc', 'Justificativa', $arrOed2['oedtipoampliacao_outros_dsc'], array('required',$disabledAnalise), array('label-size' => '2', 'input-size' => '8')); ?>
        </div>
    </div>
</div>

<script>
    //Caso seja formulário de ampliação
    if($(document).find('#omoid').val() == 3){
        $('.construcao').hide();
        $('.ampliacao').show();
    } else {
        $('.construcao').show();
        $('.ampliacao').hide();
    }
    $(document).on('click', "input[name=oedquadra]", function () {
        console.log('clicou');
        var oedquadra = $(this).val();
        if (oedquadra == 'A') {
            $("#oedquadra-A").prop("checked", true);
            $(".ampliacaooutros").show();
        } else {
            $(".ampliacaooutros").hide();
        }
    });

    $(document).on('click', "input[name=oemid]", function () {
        console.log('clicou');
        var oemid = $(this).val();
        if (oemid == 3) {
            $("#oemid-3").prop("checked", true);
            $("#div-outrosoem").show();
        } else {
            $("#div-outrosoem").hide();
        }
    });

    $(document).on('click', "input[name=oednoturno]", function () {
        console.log('clicou');
        var oednoturno = $(this).val();
        if (oednoturno == 'S') {
            $("#oemid-S").prop("checked", true);
            $("#div-oednoturno").show();
        } else {
            $("#div-oednoturno").hide();
        }
    });

    $(document).on('click', "input[name=oedtipoampliacao_outros]", function () {
        console.log('clicou');
        var oedtipoampliacao_outros = $(this).val();
        if (oedtipoampliacao_outros == 'S') {
            $("#oedtipoampliacao_outros-S").prop("checked", true);
            $("#div-oedtipoampliacao_outros").show();
        } else {
            $("#div-oedtipoampliacao_outros").hide();
        }
    });

    //Mostrar a opções de turno noturno
    $(document).on('click', "input[name=ampliacao_justificativa]", function () {
        var oemid = $(this).val();
        if (oemid == 'S') {
            $("#ampliacao_justificativa-S").prop("checked", true);
            $("#div-ampliacao_justificativa").show();
        } else {
            $("#div-ampliacao_justificativa").hide();
        }
    });
</script>
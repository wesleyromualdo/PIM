<?php
$mOed = new Par3_Model_ObraEstudoDemanda();
$arrOed3 = $mOed->recuperarPorObra($_GET['obrid']);
$qtdestimada = $mOed->calcularQtdAlunosAtendidos($arrOed3['oedid']);

$mObra   = new Par3_Model_Obra($_GET['orbid']);
$esd     = wf_pegarEstadoAtual($mObra->docid);

$ordem = $arrOed2['omoid'] != 3 ? 3 : 5;
$mpergunta       = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
$disabledAnalise = $mpergunta->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::ESTUDO_DEMANDA,$ordem,$_GET['obrid']);
?>
<div class="construcao">
    <h2 id="title-tab-3">Considerando a etapa de ensino, quais anos serão atendidos?</h2>

    <?php if($arrOed2['omoid'] != 3): ?>
        <input type="hidden" id="ordem-3" value="3">
        <div id="carregar-analise-3"></div>
    <?php endif;?>

    <div class="row" id="qtd-series">
        <input type="hidden" name="etaid" value="<?php echo $_GET['etaid'];?>" id="etaid">
        <?php if ($_GET['etaid'] == 1): ?>
            <?php echo $simec->input('oedcreche_1_qtd',   'Sala Creche I   - para atendimento de 0 a 1 ano ',  $arrOed3['oedcreche_1_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '5', 'input-size' => '4')); ?>
            <?php echo $simec->input('oedcreche_2_qtd',   'Sala Creche II  - para atendimento de 1 a 2 anos', $arrOed3['oedcreche_2_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '5', 'input-size' => '4')); ?>
            <?php echo $simec->input('oedcreche_3_qtd',   'Sala Creche III - para atendimento de 3 a 4 anos', $arrOed3['oedcreche_3_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '5', 'input-size' => '4')); ?>
            <?php echo $simec->input('oedpre_escola_qtd', 'Sala Pré-Escola - para atendimento de 4 a 5 anos', $arrOed3['oedpre_escola_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '5', 'input-size' => '4')); ?>

        <?php endif; ?>
        <?php if ($_GET['etaid'] == 2): ?>
            <?php echo $simec->input('oed1_serie_qtd', 'Sala de 1° ano do Ensino fundamental - Anos Iniciais - para atendimento a partir de 6 anos', $arrOed3['oed1_serie_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed2_serie_qtd', 'Sala de 2° ano do Ensino fundamental - Anos Iniciais - para atendimento de 7 anos', $arrOed3['oed2_serie_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed3_serie_qtd', 'Sala de 3° ano do Ensino fundamental - Anos Iniciais - para atendimento de 8 anos', $arrOed3['oed3_serie_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed4_serie_qtd', 'Sala de 4° ano do Ensino fundamental - Anos Iniciais - para atendimento de 9 anos', $arrOed3['oed4_serie_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed5_serie_qtd', 'Sala de 5° ano do Ensino fundamental - Anos Iniciais - para atendimento de 10 anos', $arrOed3['oed5_serie_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed6_serie_qtd', 'Sala de 6° ano do Ensino fundamental - Anos Finais - para atendimento de 11 anos', $arrOed3['oed6_serie_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed7_serie_qtd', 'Sala de 7° ano do Ensino fundamental - Anos Finais - para atendimento de 12 anos', $arrOed3['oed7_serie_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed8_serie_qtd', 'Sala de 8° ano do Ensino fundamental - Anos Finais - para atendimento de 13 anos', $arrOed3['oed8_serie_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed9_serie_qtd', 'Sala de 9° ano do Ensino fundamental - Anos Finais - para atendimento de 14 anos', $arrOed3['oed9_serie_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>

        <?php endif; ?>
        <?php if ($_GET['etaid'] == 3): ?>
            <?php echo $simec->input('oed1_ano_qtd', 'Sala 1º Ano do Ensino Médio', $arrOed3['oed1_ano_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed2_ano_qtd', 'Sala 2º Ano do Ensino Médio', $arrOed3['oed2_ano_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
            <?php echo $simec->input('oed3_ano_qtd', 'Sala 3º Ano do Ensino Médio', $arrOed3['oed3_ano_qtd'], array('required','maxlength' => '255', 'type' => 'number','min' => '0','data-msg-min'=>'Quantidade mínima permitida: 0',$disabledAnalise), array('label-size' => '3', 'input-size' => '4')); ?>
        <?php endif; ?>
        <h3 align="center">Com base na quantidade de salas e na recomendação de atendimento do Ministério da Educação, a quantidade estimada de alunos a serem atendidos é de: <span id="qtd-estimado-alunos"><?php echo $qtdestimada?$qtdestimada:0?></span></h3>
        <input type="hidden" name="qtd-estimado-alunos" />
    </div>
</div>

<div class="ampliacao text-center" style="display: none;">
    <h2>Justifique a Origem da Demanda:</h2>

    <?php if($arrOed2['omoid'] == 3): ?>
        <input type="hidden" id="ordem-3" value="5">
        <div id="carregar-analise-5"></div>
    <?php endif;?>

    <div  id="div-oedapliacao_justificativa" style="margin-top: 120px;">
        <?php echo $simec->textarea('oedapliacao_justificativa', '',$arrOed3['oedapliacao_justificativa'], array('required','rows' => 10,$disabledAnalise), array('label-size' => '2', 'input-size' => '12')); ?>
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
    //**********************************************************************
    // Recupera o valor dos inputs de séries atendidas e realiza o cálculo
    $(document).on('change','#qtd-series input[id^="oed"]',function(e){
        var total       = 0;
        var etaid       = $('#qtd-series #etaid').val();
        var valor       = 0;
        $('#qtd-series input[id^="oed"]').each(function(index,input){
            valor = (input['value'] == '' || isNaN(input['value']) && !isFinite(input['value'])? 0 : parseInt(input['value']));
            //Sala Creche I - para atendimento de 0 a 1 ano
            if(etaid == 1 && index == 0){
                total += valor * 10;
            }
            //Sala Creche II - para atendimento de 1 a 2 anos
            if(etaid == 1 && index == 1){
                total += valor * 16;
            }
            //Sala Creche III - para atendimento de 3 a 4 anos
            if(etaid == 1 && index == 2){
                total += valor * 20;
            }
            //Sala Pré-Escola - para atendimento de 4 a 6 anos
            if(etaid == 1 && index == 3){
                total += valor * 24;
            }
            //Fundamental e Médio
            if(etaid == 2 || etaid == 3){
                total += valor * 36;
            }
        });
        var oedturnosAtendimento = $('[name=oedturnos_atendimento]:checked').val();
        if(oedturnosAtendimento == 'D'){
            total = total * 2;
        }
        $('#qtd-estimado-alunos').html(total);
        $('[name=qtd-estimado-alunos]').val(total);

    });
    //***********************************************************************
</script>
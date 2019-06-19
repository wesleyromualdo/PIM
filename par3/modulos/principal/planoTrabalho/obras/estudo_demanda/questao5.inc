<?php
$mOed = new Par3_Model_ObraEstudoDemanda();
$arrOed5 = $mOed->recuperarPorObra($_GET['obrid']);


$mObra   = new Par3_Model_Obra($_GET['orbid']);
$esd     = wf_pegarEstadoAtual($mObra->docid);

$mpergunta       = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
$disabledAnalise = $mpergunta->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::ESTUDO_DEMANDA,7,$_GET['obrid']);
?>
<div id="questao5">
    <h3>Além do remanejamento de alunos das escolas indicadas no item anterior, o município ainda necessita expandir a vaga devido à demanda reprimida na região?</h3>

    <?php if($arrOed2['omoid'] != 3) : ?>
        <input type="hidden" id="ordem-5" value="7">
        <div id="carregar-analise-7"></div>
    <?php endif;?>

    <div class="text-center">
    <?php
    echo $simec->radio('oedremanejamento_justificativa', '',($arrOed5['oedremanejamento_justificativa']?$arrOed5['oedremanejamento_justificativa']:'N'), array('S' => 'Sim','N' => 'Não'), array('', 'required',$disabledAnalise), array('type' => 'radio radio-info radio', 'style' => 'inline', 'input-size' => '6', 'label-size' => 5));
    ?>
    </div>
    <div id="div-ampliacao_justificativa" class="text-center" style="<?php echo $arrOed5['oedremanejamento_justificativa'] == 'S'?'':'display: none;'?>">
        <?php echo $simec->textarea('oedremanejamento_justificativa_dsc', 'Justificativa',$arrOed5['oedremanejamento_justificativa_dsc'], array('required',$disabledAnalise), array('label-size' => '2', 'input-size' => '9')); ?>
    </div>
</div>
<script>

    $(document).on('click', "input[name=oedremanejamento_justificativa]", function () {
        var oedquadra = $(this).val();
        if (oedquadra == 'S') {
            $("#oedremanejamento_justificativa-S").prop("checked", true);
            $("#div-ampliacao_justificativa").show();
        } else {
            $("#div-ampliacao_justificativa").hide();
        }
    });

    //Caso seja formulário de ampliação
    if($(document).find('#omoid').val() == 3){
        $('.construcao').hide();
        $('.ampliacao').show();
    } else {
        $('.construcao').show();
        $('.ampliacao').hide();
    }
</script>
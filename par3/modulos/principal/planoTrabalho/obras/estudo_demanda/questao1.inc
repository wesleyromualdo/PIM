<?php
?>
<h2 class="text-center"><?php echo ($_GET['octid'] == 1 ? 'Motivo da Construção': 'Motivo da Ampliação'); ?></h2>

<input type="hidden" id="ordem-1" value="3">
<div id="carregar-analise-1">
    <?php
        Par3_Controller_AnaliseEngenhariaObraAbaPerguntas::carregarPerguntaEstudoDemanda(Par3_Model_ObraAba::ESTUDO_DEMANDA,1,$_GET['octid'],$_GET['obrid']);
        $mpergunta       = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
        $disabledAnalise = $mpergunta->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::ESTUDO_DEMANDA,1,$_GET['obrid']);
     ?>
</div>

<div style="margin-top: 40px;height:200px;" >
    <?php
    $mOmo  = new Par3_Model_ObraEstudoDemandaMotivo();
    $where = ($_GET['octid'] == 1 ? array('omoid in(1,2)') : array() );
    $rsOmo = simec_preparar_array($mOmo->recuperarTodos('omoid as codigo, omodsc as descricao',$where));
    echo $simec->select('omoid', '',$arrOed['omoid'], $rsOmo, array('maxlength' => '255', 'required',$disabledAnalise), array('input-size' => '12'));
    ?>
</div>
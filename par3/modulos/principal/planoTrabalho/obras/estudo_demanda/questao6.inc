<h2 class="text-center">Tipo de Obra</h2>

<?php
    Par3_Controller_AnaliseEngenhariaObraAbaPerguntas::carregarPerguntaEstudoDemanda(Par3_Model_ObraAba::ESTUDO_DEMANDA,8,$_GET['octid'],$_GET['obrid']);
    $mpergunta       = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
    $disabledAnalise = $mpergunta->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::ESTUDO_DEMANDA,8,$_GET['obrid']);
?>

<?php if(count($arrOtp) > 0 ):?>
<div class="col-lg-offset-3" id="div-tipo-obra-select">
        <?= $simec->select('otpid', '',$mdObra->otpid, simec_preparar_array($arrOtp), array($disabledAnalise,'class' => ' dadostipoobra'));?>
</div>
<br>
<br>
<br>
<div class="row">
    <div class="col-lg-12" id="detalheTipoObra" style="height:900px;">
        <?php if($mdObra->otpid):// objeto carregado no arquivo estudo de demanda?>
            <div class="alert alert-info">
                <div class="row">
                    <h3 class="text-center">Tipo de Obra Salva:</h3>
                </div>
                <?php
                $ctr = $ctrObraTipo->recuperar($mdObra->otpid);
                require_once 'obratipo_painel.inc';
                ?>
            </div>
        <?php endif;?>
    </div>
</div>
<script>
    $(function(){
//    var oedid   = $(document).find('[name=oedid]').val();
//    var obrid   = $(document).find('[name=obrid]').val();
//    var etaid   = $(document).find('[name=etaid]').val();
        var omoid = $(document).find('#form').find('#omoid').val();
        if(omoid == 3){
            var caminho = window.location.href;
            var action  = 'requisicao=carregartipoobra&'+$(document).find("#form").serialize()+'&octid='+<?php echo $_GET['octid'];?>;
            $.ajax({
                type: "GET",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    $('#loading').hide();
                    $('#div-tipo-obra-select').html(resp);
                }
            });
        }
    });
</script>
<?php else:?>
    <div class="row">
        <div class="col-lg-offset-1 col-lg-10 alert alert-warning text-center">
            <span class="glyphicon glyphicon-warning-sign center"></span>
            <b>Não existe tipo de obra vinculada a esta etapa de ensino.</b>
        </div>
    </div>
<?php endif;?>

<?php
$obrid = (int)$_GET['obrid'];
$inuid = (int)$_GET['inuid'];
if (empty($inuid)) {
    echo 'Não possui inuid! ';
    exit;
}
$itrid = $controleUnidade->pegarItrid($inuid);
$controllerObra     = new Par3_Controller_Obra($inuid, $obrid);
$emenda = new Par3_Controller_EmendasParlamentares();
$totalValoresEmenda = $emenda->retornarTotalEmendaPorObra($obrid);
//ver($totalValoresEmenda);
?>
<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center"> Recursos de Emendas </h2>
                <h3 class="center"><?= (empty($_GET['obrid']) ? 'Cadastrar' : "Pré-Obra ID: {$controllerObra->model->obrid}") ?></h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <form action="" class="">
            <div class="row col-lg-offset-1">
                <div class="form-group " id="">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Valor da Obra:</label>
                    <div class="col-sm-8">
                        <div class="input-group col-sm-6">
                            <input  class="form-control camposvalor"  name=""  disabled="true" value="<?= $totalValoresEmenda['valor_obra'];?>" >
                            <!--                            <span class="input-group-addon"><b>R$</b></span>-->
                        </div>
                    </div>
                </div>
            </div>
            <br>
            <div class="row col-lg-offset-1">
                <div class="form-group " id="">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right"> Valor total da(s) Emenda(s) para a Obra:</label>
                    <div class="col-sm-8">
                        <div class="input-group col-sm-6">
                            <input  class="form-control camposvalor"  name=""  disabled="true" value="<?= $totalValoresEmenda['valor_emenda_obra'];?>" >
<!--                            <span class="input-group-addon"><b>R$</b></span>-->
                        </div>
                    </div>
                </div>
            </div>
            <br>
            <div class="row col-lg-offset-1">
                <div class="form-group " id="">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Valor do FNDE:</label>
                    <div class="col-sm-8">
                        <div class="input-group col-sm-6">
                            <input  class="form-control camposvalor"  name="" value="" >
                            <!--                            <span class="input-group-addon"><b>R$</b></span>-->
                        </div>
                    </div>
                </div>
            </div>
            <br>
            <div class="row col-lg-offset-1">
                <div class="form-group " id="">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Valor de contrapartida:</label>
                    <div class="col-sm-8">
                        <div class="input-group col-sm-6">
                            <input  class="form-control camposvalor"  name=""  disabled="true" value="<?= $totalValoresEmenda['obra_contrapartida'];?>" >
                            <!--                            <span class="input-group-addon"><b>R$</b></span>-->
                        </div>
                    </div>
                </div>
            </div>
            <br>
            <div class="row col-lg-offset-1">
                <div class="form-group " id="">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Valor total empenhado:</label>
                    <div class="col-sm-8">
                        <div class="input-group col-sm-6">
                            <input  class="form-control camposvalor"  name=""  disabled="true" value="<?= $totalValoresEmenda['valor_empenho'];?>" >
<!--                            <span class="input-group-addon"><b>R$</b></span>-->
                        </div>
                    </div>
                </div>
            </div>
        </form>
        <?= $emenda->listarEmendasPorObra($obrid);?>
    </div>
</div>
<script>
    $(function(){
        $(".camposvalor").inputmask('decimal', {
            'alias': 'numeric',
            'groupSeparator': '.',
            'autoGroup': true,
            'digits': 2,
            'min': 0,
            'max': 999999999999,
            'radixPoint': ",",
            'digitsOptional': false,
            'allowMinus': false,
            'prefix': 'R$ ',
            'placeholder': ''
        });
    });
</script>
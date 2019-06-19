<?php


$controller   = new Par3_Controller_CronogramaContrapartida();
$modelObra    = new Par3_Model_Obra($_REQUEST['obrid']);

switch ($_REQUEST['requisicao']) {
    case 'salvarContrapartida':
        $controller->salvar($_REQUEST);
        break;
}

//recuperar dados
$valorTotalObra = $modelObra->obrvalor;
$valorMaximo = $modelObra->obrvalor; //Apenas para validação no javascript. Contrapartida não pode ser maior que o total
$valorEmpenho = $controller->buscaValorEmpenhadoObra($modelObra->obrid);
$valorContrapartida = $modelObra->obrvalor_contrapartida;
$valorFnde = bcsub($valorTotalObra, $valorContrapartida,2);

if($_GET['disabled'] == ''){
    $podeEditar = true;
}
//funcao obras habilitareditacaoporperfilestadodocumento

//tabela de percentual (cronograma de contrapartida)
$execucao20 = par3_mascaraMoeda($controller->calculaValorPorcentagem($valorContrapartida, 20), false, false);
$execucao40 = par3_mascaraMoeda($controller->calculaValorPorcentagem($valorContrapartida, 25), false, false);
$execucao60 = par3_mascaraMoeda($controller->calculaValorPorcentagem($valorContrapartida, 35), false, false);
$execucao80 = par3_mascaraMoeda($controller->calculaValorPorcentagem($valorContrapartida, 20), false, false);
$totalExecucao = par3_mascaraMoeda($controller->calculaValorPorcentagem($valorContrapartida, 100), false, false);

//tabela de valores de empenho
$valorTotalObra = par3_mascaraMoeda($valorTotalObra, true, true);
$valorEmpenho = par3_mascaraMoeda($valorEmpenho, true, true);
$valorContrapartida = par3_mascaraMoeda($valorContrapartida, false, false);
$valorFnde = par3_mascaraMoeda($valorFnde, true, true);

$mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();

?>

<style>

    .panel-primary > .panel-heading{
        border-color: #23c6c8 !important;
        background-color: #23c6c8 !important;
        text-align: center;
    }

    .err{
        border-color: #ed5565 !important;
        -webkit-box-shadow: inset 0 1px 1px rgba(0, 0, 0, 0.075);
        box-shadow: inset 0 1px 1px rgba(0, 0, 0, 0.075);
    }

</style>

<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox">
            <div class="ibox-title">
                <h2 class="center"> Cronograma de Contrapartida </h2>
                <h3 class="center"><?= (empty($_GET['obrid']) ? 'Cadastrar' : "Pré-Obra ID: {$_GET['obrid']}") ?></h3>
            </div>
            <div class="ibox-content">

            <?php
                $mpergunta->carregarPergunta(Par3_Model_ObraAba::CRONOGRAMA_CONTRAPARTIDA,1,$modelObra->obrid);
                $pendencia = $mpergunta->verificarAnalisePendencia(Par3_Model_ObraAba::CRONOGRAMA_CONTRAPARTIDA,1,$modelObra->obrid);

            if($podeEditar){
                $podeEditar = $pendencia ? true : false;
            }
            ?>

                <div id="listaValorContrapartida">
                    <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                        <input type="hidden" name="requisicao" value="salvarContrapartida"/>
                        <input type="hidden" name="valorMaximo" id="valorMaximo" value="<?php echo $valorMaximo; ?>"/>

                        <!-- Tabela de Valores (total, empenho, contrapartida e fnde) -->
                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tabelaPercentualContrapartida">
                            <thead>
                                <tr>
                                    <th>Valor Total da Obra</th>
                                    <th>Valor Empenhado</th>
                                    <th>Valor de Contrapartida</th>
                                    <th>Valor do FNDE</th>
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <td class=""><?php echo $valorTotalObra; ?></td>
                                    <td class=""><?php echo $valorEmpenho; ?></td>
                                    <td class="">
                                        <?php
                                            if($podeEditar){
                                                echo '<div class="center" id="valor_contrapartida">R$ <input id="obrvalor_contrapartida" name="obrvalor_contrapartida" value="' . $valorContrapartida . '" size="12" type="text" maxlength="20" onkeyup="this.value = formataValor(this.value)"></div>';
                                            }else{
                                                echo "R$" . $valorContrapartida;
                                            }
                                        ?>
                                    </td>
                                    <td class=""><?php echo $valorFnde; ?></td>
                                </tr>
                            </tbody>
                        </table>

                        <!-- Tabela de Percentual Cronograma -->

                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tabelaPercentualContrapartida">
                            <tbody>
                                <tr>
                                    <td class="center" colspan="3" style="font-size: 16px;"><b>Cronograma de Contrapartida</b></td>
                                </tr>
                                <tr>
                                    <td><b>Fase da obra</b></td>
                                    <td><b>% do valor da contrapartida a ser depositado</b></td>
                                    <td><b>Valor da Contrapartida (R$)</b></td>
                                </tr>
                                <tr>
                                    <td>Até 20% da execução</td>
                                    <td class="center">20%</td>
                                    <td><?php echo $execucao20; ?></td>
                                </tr>
                                <tr>
                                    <td>Até 40% da execução</td>
                                    <td class="center">25%</td>
                                    <td><?php echo $execucao40; ?></td>
                                </tr>
                                <tr>
                                    <td>Até 60% da execução</td>
                                    <td class="center">35%</td>
                                    <td><?php echo $execucao60; ?></td>
                                </tr>
                                <tr>
                                    <td>Até 80% da execução</td>
                                    <td class="center">20%</td>
                                    <td><?php echo $execucao80; ?></td>
                                </tr>
                                <tr>
                                    <td><b>Total</b></td>
                                    <td class="center"><b>100%</b></td>
                                    <td><b><?php echo $totalExecucao; ?></b></td>
                                </tr>
                            </tbody>
                        </table>

                    </div>

                </div>

                <div class="ibox-footer">
                    <div style="text-align: center;">
                        <button type="submit" class="btn btn-success salvar center"><i class="fa fa-save"></i> Salvar</button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>

<script>

    $(function(){
       $('#btn_pergunta_9').onclick = function(){
           location.reload();
       };

       $('#obrvalor_contrapartida').on('blur', function(){
           var vlrObra = $("#valorMaximo").val();
           var vlrContrapartida = $(this).val();
           vlrContrapartida = vlrContrapartida.replace(/\./gi, '');
           vlrContrapartida = vlrContrapartida.replace(/\,/gi, '.');
           console.log(vlrContrapartida);
           if(vlrObra - vlrContrapartida < 0){
               $(this).val('');
               alert("O valor de contrapartida não pode ser maior que o valor total da obra!");
               return false;
           }
       });

    });

    function formataValor(valor){
        return mascaraglobal('###.###.###.###,##', valor);
    }

</script
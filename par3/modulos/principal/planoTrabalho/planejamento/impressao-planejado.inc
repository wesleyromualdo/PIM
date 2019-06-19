<?php
  	$instrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $oIndicador         = new Par3_Model_Indicador();
        $oIndicadorMeta     = new Par3_Model_IndicadorMeta();
        $mInp               = new Par3_Controller_IniciativaPlanejamento();
        $mItr               = new Par3_Controller_InstrumentoUnidade();
        $nomeItr            = $mItr->pegarNomeEntidade($_REQUEST['inuid']);
        $mInu               = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $mDoc               = new Workflow_Model_Documento($mInu->docid);
        $controllerIniciativaPlanejamentoItemComposicao = new Par3_Controller_IniciativaPlanejamentoItemComposicao();
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
        //$arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($instrumentoUnidade->itrid);
        $arDimensoes = $mInp->getDimensaoIniciativas($_REQUEST['inuid']);
//        ver($mInp, d);
?>

<?php
//require APPRAIZ . 'includes/cabecalho_bootstrap_v3_limpo.inc';

$inu = new Par3_Model_InstrumentoUnidade($_GET['inuid']);

$intrumento = $inu->itrid != 2 ? 'Estado' : 'Município';
?>

<style type="text/css" media="print">
    @page { 
        size: landscape;
    }
</style>

<div class="ibox landscape" style="height: 100%; width:100%;">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-9">
                <h3 class="center">PAR do <?= $intrumento.': '.$inu->inudescricao; ?></h3>
            </div>
            <div class="col-md-3">
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimirSintese"><i
                            class="glyphicon glyphicon-print"></i> Imprimir
                </button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="table-responsive">
                <?php if(is_array($arDimensoes)){ foreach($arDimensoes as $dimensao){ ?>
                <table class="table table-bordered">
                    <thead>
                    <tr>
                        <th>Dimensão</th>
                        <th><?php echo $dimensao['dimcod'] . "." . $dimensao['dimdsc']  ?></th>
                    </tr>
                    </thead>
                    <tbody>
                    <?php $arIniciativas = $mInp->getDimensaoIniciativasPlanejamento($_REQUEST['inuid'], $dimensao['dimid']); ?>
                    <?php if(is_array($arIniciativas)){ foreach($arIniciativas as $iniciativa){ ?>
                    <?php $arrIniciativaDados = $mInp->getIniciativaDados(array('iniid' => $iniciativa['iniid'], 'inuid' => $_REQUEST['inuid']));  ?>
                    <tr>
                        <td style="vertical-align: center; background-color: #e8e8e8;"><b>Iniciativa</b></td>
                        <td>
                            <table class="table table-bordered">
                                <tbody>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Descrição</b></td>
                                    <td><?php echo $iniciativa['iniid'] . "." . $iniciativa['inddsc']; ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Planejamento</b></td>
                                    <td><?php echo $iniciativa['inpid']; ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Ciclo</b></td>
                                    <td><?php echo $arrIniciativaDados['cic']; ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Anos</b></td>
                                    <td><?php echo $arrIniciativaDados['anos']; ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Objeto</b></td>
                                    <td><?php echo trim($arrIniciativaDados['into']); ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Atendimento</b></td>
                                    <td><?php echo $arrIniciativaDados['inta']; ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Esfera</b></td>
                                    <td><?php echo $arrIniciativaDados['esfera']; ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Áreas</b></td>
                                    <td><?php echo $arrIniciativaDados['iar']; ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Programa</b></td>
                                    <td><?php echo $arrIniciativaDados['prg']; ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Projeto</b></td>
                                    <td><?php echo ($arrIniciativaDados['pro']?$arrIniciativaDados['pro']:'&nbsp;'); ?></td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Nível</b></td>
                                    <td>Educação Básica</td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Cronograma Físico e Financeiro</b></td>
                                    <td><?php if ($iniciativa['inpcronogramamesinicial']) { echo $arMeses[$iniciativa['inpcronogramamesinicial']].'/'.$iniciativa['inpcronogramaanoinicial']. ' a ' .$arMeses[$iniciativa['inpcronogramamesfinal']].'/'.$iniciativa['inpcronogramaano']; }  ?></td>
                                </tr>     
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Itens de Composição</b></td>
                                    <td>
                                        <div class="table-responsive">
                                            <?php echo $controllerIniciativaPlanejamentoItemComposicao->getPlanilhaImpressao($iniciativa['inpid'],$_REQUEST['inuid'],0); ?>
                                        </div>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="background-color: #e8e8e8;"><b>Detalhamento</b></td>
                                    <td><?php echo $iniciativa['inpitemcomposicaodetalhamento']; ?></td>
                                </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                    <?php }} // Fim Iniciativa ?>
                    </tbody>
                </table>
                <?php }} // Fim Dimensão ?>
            </div>
        </div>
    </div>
</div>

<script>
    $("#imprimirSintese").on("click", function () {
                getTituloRelatorio(3);
                var caminho = window.location.href;
                var action = '&requisicao=imprimirSintese';
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        $("#impressao-content").html(resp);
                        var container = $("#impressao-content");
                        var table = container.find('table');
                        var th = container.find('th');
                        var td = container.find('td');
                        table.css('width', '100%')
                        table.css('border-collapse', 'collapse');
                        table.css('font-size', 10);
                        container.find(".popover,button,.modal").remove();
                        container.find("table,td,th").css('border', '1px solid black');
                        container.find('tr:even').css('background-color', 'silver');
                        container.css('size', 'landscape');
                        th.css('height', '50px');
                        td.css('text-align', 'left');
                        var w = window.open();
                        w.document.write($('#div-impressao').html());
                        w.document.title = 'Relatório - Planejamentos - <?php echo $nomeItr; ?>';
                        w.print();
                        w.close();
                        $("#impressao-content").html("");
                        return true;
                    }
                });
            });
</script>
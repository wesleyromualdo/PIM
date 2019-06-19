<?php
$controle = new Par3_Controller_ObraPlanilhaOrcamentaria();
switch ($_REQUEST['req']) {
    case 'atualizarValor':
        ob_clean();
        $dados['opoid'] = preg_replace("/[^0-9]/", "", $_REQUEST['nome']);
        $dados['opovalor'] = desformata_valor($_REQUEST['valor']);
        $controle->salvar($dados);

        $dados = array('obrid'=> $_REQUEST['obrid'],'inuid'=>$_REQUEST['inuid']);

        $obraPlanilhaOrcamento = new Par3_Controller_ObraPlanilhaOrcamentaria();
        $obrvalor = $obraPlanilhaOrcamento->valorTotalPlanilha($dados);

        $ctlrObra = new Par3_Controller_Obra();
        $ctlrObra->atualizaValorObra($obrvalor,$_REQUEST['obrid'], $_REQUEST['inpid']);

        die;
        break;
    case 'atualizaQuantidade':
        ob_clean();
        $dados['opoid'] = preg_replace("/[^0-9]/", "", $_REQUEST['nome']);
        $dados['opoqtd'] = $_REQUEST['valor'];
        $controle->salvar($dados);

        $dados = array('obrid'=> $_REQUEST['obrid'],'inuid'=>$_REQUEST['inuid']);

        $obraPlanilhaOrcamento = new Par3_Controller_ObraPlanilhaOrcamentaria();
        $obrvalor = $obraPlanilhaOrcamento->valorTotalPlanilha($dados);

        $ctlrObra = new Par3_Controller_Obra();
        $ctlrObra->atualizaValorObra($obrvalor,$_REQUEST['obrid'], $_REQUEST['inpid']);

        die;
        break;
    default:
        break;
}

$dados['obrid'] = $_REQUEST['obrid'];
$dados['inuid'] = $_REQUEST['inuid'];

$dadosLista = $controle->dadosIniciais($dados);
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <?php
        $mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
        $mpergunta->carregarPergunta(Par3_Model_ObraAba::PLANILHA_ORCAMENTARIA,1,$_GET['obrid']);
        $mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
        $disableDadosBasicos = $mpergunta ->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::PLANILHA_ORCAMENTARIA,1,$_GET['obrid']);        ?>
        <div class="ibox">
            <div class="ibox-title">
                <div class="row">
                    <div class="col-md-12">
                        <h2 class="center"> Planilha Orçamentaria </h2>
                        <h3 class="center"><?= (empty($_GET['obrid']) ? 'Cadastrar' : "Pré-Obra ID: {$_GET['obrid']}") ?></h3>
                    </div>
                </div>
            </div>
            <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive"
                   id="dadosGerais">
                <thead>
                <tr>
                    <th> Descrição do item</th>
                    <th> Valor Unitário</th>
                    <th> Unidade de Medida</th>
                    <th> Quantidade</th>
                    <th> Valor</th>
                    <th> %</th>
                </tr>
                </thead>
                <tbody>
                <?php
                if (is_array($dadosLista)) {
                    foreach ($dadosLista as $linha) {
                        if ($linha['oipid'] == $linha['paigeral']) {
                            $icone = "<i class='mostraesconde fa fa-plus fa-minus' paigeral='{$linha['paigeral']}'></i>";
                            $class = '';
                        } else {
                            $class = "class='mostraesconde{$linha['paigeral']}'";
                            $icone = "&nbsp;&nbsp;&nbsp;&nbsp;";
                        }

                        $valorMontate += $linha['total'];

                        $desc = str_repeat('&nbsp;&nbsp;&nbsp;', $linha['nivel']) . $linha['oipordem'] . " - " . $linha['oipdsc'];

                        if ($linha['oivid']) {
                            $opovalortotal = formata_numero_monetario($linha['opovalor'] * $linha['oivqtd']);
                            $oimedida = $linha['unidsc'];

                            $opovalor = $simec->input('opoid' . $linha['opoid'], NULL, $linha['opoid'], array('type' => 'hidden', 'class' => 'opoid'));
                            if ($linha['oivvalor_editavel'] == 'S') {
                                $opovalor .= $simec->input('opovalor' . $linha['opoid'], NULL, $linha['opovalor'], array('maxlength' => 15, 'class' => 'valor', 'size' => 15));
                            } else {
                                $opovalor .= $simec->input('opovalor' . $linha['opoid'], NULL, $linha['opovalor'], array('disabled', 'maxlength' => 15, 'class' => 'valor', 'size' => 15));
                            }

                            if ($linha['oivqtd_editavel'] == 'S') {
                                $oivqtd = $simec->input('opoqtd' . $linha['opoid'], NULL, $linha['opoqtd'], array('maxlength' => 10, 'class' => 'valor', 'size' => 8));
                            } else {
                                $oivqtd = $simec->input('opoqtd' . $linha['opoid'], NULL, $linha['opoqtd'], array('disabled', 'maxlength' => 10, 'class' => 'valor', 'size' => 8));
                            }
                        } else {
                            $opovalor = $opovalortotal = $oivqtd = $opovalortotal = $oimedida = '';
                        }
                        //<td>{$icone}{$desc}</td>
                        $tabela = " <tr $class >
                                        <td width=50% >{$desc}</td>
                                        <td width=10% align='right'>{$opovalor}</td>
                                        <td width=10% align='center'>{$oimedida}</td>
                                        <td width=13% align='right'>{$oivqtd}</td>
                                        <td width=10% align='right' id='linhaTotal{$linha['opoid']}'></td>
                                        <td width=7% align='right' id='linhaPercentual{$linha['opoid']}'></td>
                                    </tr>";
                        echo $tabela;
                    }
                }
                ?>
                </tbody>
                <tfoot>
                <tr>
                    <td style="text-align:right" colspan="6"><strong>Total <span id="totalgeral"><?php echo formata_numero_monetario($valorMontate);?></span></strong></td>
                </tr>
                </tfoot>
            </table>
        </div>
    </div>
</div>

<script>
<?php
//VERIFICA SE PERMITE EDIÇÃO
if($_GET['disabled']):?>
$(document).find('input').not('[type=search]').prop('disabled','disabled');
<?php endif;?>
    $(document).ready(function () {
        $(".valor").inputmask("currency", {
            radixPoint: ",",
            groupSeparator: ".",
            digits: 2,
            autoGroup: true,
            prefix: ''
        });
        atualizadaTabela();

        var table = $('#dadosGerais').DataTable( {
            "paging":   false,
            "ordering": false,
            "info":     false,
            searchHighlight: true,
            'oLanguage' : {
                'sSearch': "Pesquisar :&nbsp;&nbsp;",
                'sZeroRecords' : "N&atilde;o foram encontrados resultados",
            }
        } );
    });

    $('.mostraesconde').click(function () {
        if ($(this).is(".fa-minus")) {
            $(this).removeClass("fa-minus");
            $('.mostraesconde' + $(this).attr("paigeral")).hide(500);
        } else {
            $(this).addClass("fa-minus");
            $('.mostraesconde' + $(this).attr("paigeral")).show(500);
        }
    });

    $('.valor').change(function () {
        $(this).attr("name")
        var caminho = window.location.href;
        var action = '&req=atualizarValor&nome=' + $(this).attr("name") + '&valor=' + $(this).val();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                atualizadaTabela();
                return true;
            }
        });
    });
    $('.qtd').change(function () {
        $(this).attr("name")
        var caminho = window.location.href;
        var action = '&req=atualizaQuantidade&nome=' + $(this).attr("name") + '&valor=' + $(this).val();
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                atualizadaTabela();
                return true;
            }
        });
    });


    function atualizadaTabela() {
        var calculo = new Calculo();
        var total = valorlinha = valorpercentual = valor = qtd = opoid = 0;
        $(".opoid").each(function () {

            opoid   = $(this).val();
            valor   = $('#opovalor'+opoid).val().replace('.', '').replace('.', '').replace('.', '').replace(',', '.');
            qtd     = $('#opoqtd'+opoid).val().replace('.', '').replace('.', '').replace('.', '').replace(',', '.');

            if(valor && qtd){
                total += parseFloat(valor) * parseFloat(qtd);
            }
        });

        $(".opoid").each(function () {
        	valorlinha = 0;
        	valorpercentual = 0;

            opoid   = $(this).val();
            valor   = $('#opovalor'+opoid).val().replace('.', '').replace('.', '').replace('.', '').replace(',', '.');
            qtd     = $('#opoqtd'+opoid).val().replace('.', '').replace('.', '').replace('.', '').replace(',', '.');

            if(valor && qtd){
                valorlinha = parseFloat(valor) * parseFloat(qtd);

                valorpercentual = parseFloat(valorlinha) * 100 / parseFloat(total);
            }

            $("#linhaTotal"+opoid).text(formatReal(valorlinha.toFixed(2)));
            $("#linhaPercentual"+opoid).text(formatReal(valorpercentual.toFixed(2)) + ' %');
        });

        function formatReal(numero) {
            var tmp = numero + '';
            var neg = false;



            if (tmp.indexOf(".")) {
                tmp = tmp.replace(".", "");
            }

            if (tmp.indexOf("-") == 0) {
                neg = true;
                tmp = tmp.replace("-", "");
            }

            if (tmp.length == 1) tmp = "0" + tmp

            tmp = tmp.replace(/([0-9]{2})$/g, ",$1");
            if (tmp.length > 6)
                tmp = tmp.replace(/([0-9]{3}),([0-9]{2}$)/g, ".$1,$2");

            if (tmp.length > 9)
                tmp = tmp.replace(/([0-9]{3}).([0-9]{3}),([0-9]{2}$)/g, ".$1.$2,$3");

            if (tmp.length = 12)
                tmp = tmp.replace(/([0-9]{3}).([0-9]{3}).([0-9]{3}),([0-9]{2}$)/g, ".$1.$2.$3,$4");

            if (tmp.length > 12)
                tmp = tmp.replace(/([0-9]{3}).([0-9]{3}).([0-9]{3}).([0-9]{3}),([0-9]{2}$)/g, ".$1.$2.$3.$4,$5");

            if (tmp.indexOf(".") == 0) tmp = tmp.replace(".", "");
            if (tmp.indexOf(",") == 0) tmp = tmp.replace(",", "0,");

            return (neg ? '-' + tmp : tmp);
        }
    }
</script>
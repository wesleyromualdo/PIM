<?php

require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraNotas/ctrl.php';
require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraNotas/css.php';

?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<?php

$execucaoFinanceira = new ExecucaoFinanceira();
$verificarExecucaoFinanceira = $execucaoFinanceira->verificarExecucaoFinanceiraFinalizada($obrid);
$execucaoFinanceiraFinalizada = $verificarExecucaoFinanceira > 0 ? true : false;

echo $execucaoFinanceira->criaSubAba($url, $habilitado, $obrid);

?>

<div class="abaContainer">
    <table width="100%">
        <tr>
            <td style="background: #f00; color: #fff; font-weight: bold">
                <img src="/imagens/atencao.png" /> Atenção! As notas fiscais devem conter além do nome do município: a) identificação do órgão concedente (FNDE/MEC);
                b) número do instrumento de pactuação (termo de compromisso ou convênio) e c) nome do Programa (PAR ou Proinfância)
            </td>
        </tr>
    </table>
    <div class="abaContent">
        <?php if(!$execucaoFinanceiraFinalizada){?>
        <br />
        <button class="btn btn-primary" onclick="incluirNotaFiscal()">Incluir Nota Fiscal</button>
        <br /><br />
        <?php }?>
        <div class="construtoraContent">

            <?php

            $arrFornecedores = getFornecedores($obrid);

            if (is_array($arrFornecedores) && !empty($arrFornecedores)) {

                $auxContrato = 0; // Variável auxiliar para verificar se a obra possui mais de um contrato.
                $valorContratoAnterior = null;

                foreach ($arrFornecedores as $arrFornecedor) {

                    $auxContrato++;

                    $arrNotas = carregaDadosNotas($obrid, $arrFornecedor["tipofornecedor"], $arrFornecedor["idfornecedor"]);

                    $execucaoFinanceira = new ExecucaoFinanceira();

                    $crtid = null;
                    $cceid = null;
                    if ($arrFornecedor["tipofornecedor"] === "ent") {
                        $crtid = $arrFornecedor["idcontratofornecedor"];
                    } elseif ($arrFornecedor["tipofornecedor"] === "cex") {
                        $cceid = $arrFornecedor["idcontratofornecedor"];
                    }
                    $valorContratoAnterior = $valorContratoAtual;
                    $valorContratoAtual = $execucaoFinanceira->getValorContratoAtual($crtid, $cceid);
                    $valorSomatorioTodosContratos = $execucaoFinanceira->getSomatorioTodosContratos($obrid, $arrFornecedor["tipofornecedor"]);

                    ?>
                    <table class="table table-bordered">
                        <thead>
                        <tr>
                            <td colspan="8" style="text-align: left;">
                                <?php
                                $cnpjFornecedor = $arrFornecedor["fornecedor"];
                                $nomeFornecedor = $arrFornecedor["nomefornecedor"];
                                $dtAssinaturaContrato = $arrFornecedor["dataassinaturacontrato"];
                                echo "Empresa Contratada: ({$cnpjFornecedor}) {$nomeFornecedor} (Contrato: {$dtAssinaturaContrato})";
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td>Ações</td>
                            <td>Fornecedor</td>
                            <td>Número da Nota Fiscal</td>
                            <td>Data da Nota Fiscal</td>
                            <td>Medicões Físicas</td>
                            <td>% Medição Física</td>
                            <td>% Medição Física Acumulada</td>
                            <td>Valor da Nota Fiscal</td>
                        </tr>
                        </thead>
                        <tbody>
                        <?php
                        if (!empty($arrNotas)) {

                            $valorContratoAnterior = (float)$valorContratoAnterior;

                            if ($valorContratoAnterior > 0) {
                                $ultimoPercAcumulado = ($somatorioValoresNotas / $valorContratoAnterior);
                            } else {
                                $ultimoPercAcumulado = 0;
                            }

                            $somatorioValoresContratosAtuais = 0;
                            $somatorioValoresNotas = 0;
                            $valorTotal = 0;
                            foreach ($arrNotas as $k=>$nota) {

                                $notaTemPendencia = $execucaoFinanceira->verificaNotaFiscalComPendencia($nota["ntfid"]);

                                $somatorioValoresNotas += $nota["valornota"];

                                ?>
                                <tr>
                                    <td>
                                        <?php
                                        if ($notaTemPendencia > 0) {
                                            ?>
                                            <img src='../imagens/atencao.png' title='Pendência de preenchimento'
                                                 width='16'
                                                 style=cursor:pointer;'>&nbsp;
                                            <?php
                                        }
                                        if(!$execucaoFinanceiraFinalizada){
                                        ?>
                                        <img src='../imagens/alterar.gif' title='Editar Nota Fiscal' width='16'
                                             style=cursor:pointer; onclick='editarNotaFiscal(<?= $nota['ntfid'] ?>)'>&nbsp;
                                        <img src='../imagens/excluir.gif' title='Excluir Nota Fiscal' width='16'
                                             style=cursor:pointer; onclick='excluirNotaFiscal(<?= $nota['ntfid'] ?>)'>&nbsp;
                                        <?php }?>
                                        <img src='../imagens/salvar.png' title='Download da Nota Fiscal' width='16'
                                             style=cursor:pointer; onclick='downloadArquivo(<?= $nota['arqid'] ?>)'>&nbsp;
                                    </td>
                                    <td><?= $nota['fornecedor'] ?></td>
                                    <td><?= $nota['numnota'] ?></td>
                                    <td><?= $nota['datanota'] ?></td>
                                    <td>
                                        <?php
                                        $arrMedicoes = buscaMedicoesNota($nota['ntfid']);

                                        if (is_array($arrMedicoes) && !empty($arrMedicoes)) {
                                            $c = count($arrMedicoes);
                                            $aux = 0;
                                            foreach ($arrMedicoes as $arrMed) {
                                                $aux++;
                                                $medid = $arrMed["medid"];
                                                $numMedicao = $arrMed["mednummedicao"];
                                                $separador = $aux == $c ? "" : ",&nbsp";
                                                if ($numMedicao) {
                                                    echo "<img 
                                                        border='0' 
                                                        src='../imagens/icone_lupa.png' 
                                                        width='16' 
                                                        height='16' 
                                                        title='Clique para consultar.'
                                                        onclick='visualizarMedicao({$medid})'
                                                        style='cursor:pointer'
                                                       />" . $numMedicao . $separador;
                                                }
                                            }
                                        }
                                        ?>
                                    </td>
                                    <td>
                                        <?php
                                        if ($nota["crtid"]) {

                                            $percMedicaoFisica = 0;
                                            if ($valorContratoAtual > 0) {
                                                $percMedicaoFisica = $execucaoFinanceira->calcularPercentualMedicao($nota["valornota"], $valorContratoAtual);
                                            }

                                            $percMedicaoFisica = number_format($percMedicaoFisica, 2, ",", ".");
                                            echo $percMedicaoFisica . "%";
                                        } else {
                                            echo "Não se aplica.";
                                        }
                                        ?>
                                    </td>
                                    <td>
                                        <?php
                                        if ($nota["crtid"]) {
                                           
                                            if ($auxContrato > 1) {
                                                if ($k == 0) {
                                                    $percMedicaoAcumulada = $ultimoPercAcumulado + (($nota["valornota"] / $valorContratoAtual) * (1 - $ultimoPercAcumulado));
                                                } else {
                                                    $percMedicaoAcumulada = $percMedicaoAcumulada + (($nota["valornota"] / $valorContratoAtual) * (1 - $ultimoPercAcumulado));
                                                }

                                                $percMedicaoAcumuladaFormatado = $percMedicaoAcumulada * 100;
                                                $percMedicaoAcumuladaFormatado = number_format($percMedicaoAcumuladaFormatado, 2, ",", ".");
                                                echo $percMedicaoAcumuladaFormatado . "%";

                                            } else {
                                                $percMedicaoAcumulada = $execucaoFinanceira->calcularPercentualMedicao($somatorioValoresNotas, $valorContratoAtual);
                                                $percMedicaoAcumulada = number_format($percMedicaoAcumulada, 2, ",", ".");
                                                echo $percMedicaoAcumulada . "%";

                                            }
 
                                        } else {
                                            echo "Não se aplica.";
                                        }
                                        ?>
                                    </td>
                                    <td><?= formata_numero_monetario($nota['valornota']) ?></td>
                                </tr>
                                <?php
                                $valorTotal += $nota['valornota'];
                            }
                            if(count($arrNotas) > 0)
                            {?>
                                <tr>
                                    <td colspan="6"></td>
                                    <td style="background-color: #DCDCDC" ><b>Valor Total:</b></td>
                                    <td style="background-color: #DCDCDC"><b><?php echo  number_format($valorTotal, 2, ",", ".");?></b></td>
                                </tr>
                                <?php
                            }
                        } else {
                            echo "
                                <tr>
                                    <td colspan='8' style='text-align: center'>Não foram encontrados registros</td>
                                </tr>
                            ";
                        }
                        ?>
                        </tbody>
                    </table>
                    <?php

                }

            } else {
                echo "
                    <br>
                    <table class='table table-bordered'>
                        <thead>
                            <tr>
                                <td>Até o momento, não há registro de notas fiscais cadastrado.</td>
                            </tr>
                        </thead>
                    </table>
                ";
            }
            ?>

        </div>
        <br/>
        <div class="divFaixa">
            <p style="font-weight: bold; font-size: 16px;">Construtoras e Contratos Incluídos</p>
        </div>
        <br/>
        <!-- Bloco de codigo lista notas fiscais de construtoras exrtas-->
        <div class="construtoraContent">

            <?php

            $arrFornecedores = getFornecedoresExtras($obrid);
            if (is_array($arrFornecedores) && !empty($arrFornecedores)) {

                foreach ($arrFornecedores as $arrFornecedor) {

                    $arrNotas = carregaDadosNotas($obrid, $arrFornecedor["tipofornecedor"], $arrFornecedor["idfornecedor"]);


                    ?>
                    <table class="table table-bordered">
                        <thead>
                        <tr>
                            <td colspan="8" style="text-align: left;">
                                <?php
                                $cnpjFornecedor = $arrFornecedor["fornecedor"];
                                $nomeFornecedor = $arrFornecedor["nomefornecedor"];
                                $dtAssinaturaContrato = $arrFornecedor["dataassinaturacontrato"];
                                echo "Empresa Contratada: ({$cnpjFornecedor}) {$nomeFornecedor} (Contrato: {$dtAssinaturaContrato})";
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td>Ações</td>
                            <td>Fornecedor</td>
                            <td>Número da Nota Fiscal</td>
                            <td>Data da Nota Fiscal</td>
                            <td>Medicões Físicas</td>
                            <td>% Medição Física</td>
                            <td>% Medição Física Acumulada</td>
                            <td>Valor da Nota Fiscal</td>
                        </tr>
                        </thead>
                        <tbody>
                        <?php
                        if (!empty($arrNotas)) {
                            $valorTotal = 0;
                            foreach ($arrNotas as $k=>$nota) {

                                $notaTemPendencia = $execucaoFinanceira->verificaNotaFiscalComPendencia($nota["ntfid"]);

                                $somatorioValoresNotas += $nota["valornota"];

                                ?>
                                <tr>
                                    <td>
                                        <?php
                                        if ($notaTemPendencia > 0) {
                                            ?>
                                            <img src='../imagens/atencao.png' title='Pendência de preenchimento'
                                                 width='16'
                                                 style=cursor:pointer;'>&nbsp;
                                            <?php
                                        }
                                        if(!$execucaoFinanceiraFinalizada){
                                        ?>
                                        <img src='../imagens/alterar.gif' title='Editar Nota Fiscal' width='16'
                                             style=cursor:pointer; onclick='editarNotaFiscal(<?= $nota['ntfid'] ?>)'>&nbsp;
                                        <img src='../imagens/excluir.gif' title='Excluir Nota Fiscal' width='16'
                                             style=cursor:pointer; onclick='excluirNotaFiscal(<?= $nota['ntfid'] ?>)'>&nbsp;
                                        <?php } ?>
                                        <img src='../imagens/salvar.png' title='Download da Nota Fiscal' width='16'
                                             style=cursor:pointer; onclick='downloadArquivo(<?= $nota['arqid'] ?>)'>&nbsp;
                                    </td>
                                    <td><?= $nota['fornecedor'] ?></td>
                                    <td><?= $nota['numnota'] ?></td>
                                    <td><?= $nota['datanota'] ?></td>
                                    <td>
                                        <?php
                                        $arrMedicoes = buscaMedicoesNota($nota['ntfid']);

                                        if (is_array($arrMedicoes) && !empty($arrMedicoes)) {
                                            $c = count($arrMedicoes);
                                            $aux = 0;
                                            foreach ($arrMedicoes as $arrMed) {
                                                $aux++;
                                                $medid = $arrMed["medid"];
                                                $numMedicao = $arrMed["mednummedicao"];
                                                $separador = $aux == $c ? "" : ",&nbsp";
                                                if ($numMedicao) {
                                                    echo "<img 
                                                        border='0' 
                                                        src='../imagens/icone_lupa.png' 
                                                        width='16' 
                                                        height='16' 
                                                        title='Clique para consultar.'
                                                        onclick='visualizarMedicao({$medid})'
                                                        style='cursor:pointer'
                                                       />" . $numMedicao . $separador;
                                                }
                                            }
                                        }
                                        ?>
                                    </td>
                                    <td>
                                        Não se aplica.
                                    </td>
                                    <td>
                                        Não se aplica.
                                    </td>
                                    <td><?= formata_numero_monetario($nota['valornota']) ?></td>
                                </tr>
                                <?php
                                $valorTotal += $nota['valornota'];
                            }
                            if(count($arrNotas) > 0)
                            {?>
                                <tr>
                                    <td colspan="6"></td>
                                    <td style="background-color: #DCDCDC" ><b>Valor Total:</b></td>
                                    <td style="background-color: #DCDCDC"><b><?php echo  number_format($valorTotal, 2, ",", ".");?></b></td>
                                </tr>
                                <?php
                            }
                        } else {
                            echo "
                                <tr>
                                    <td colspan='8' style='text-align: center'>Não foram encontrados registros</td>
                                </tr>
                            ";
                        }
                        ?>
                        </tbody>
                    </table>
                    <?php

                }


            } else {
                echo "
                    <br>
                    <table class='table table-bordered'>
                        <thead>
                            <tr>
                                <td>Até o momento, não há registro de notas fiscais cadastrado.</td>
                            </tr>
                        </thead>
                    </table>
                ";
            }
            ?>

        </div>
        <br/>
    </div>
</div>

<br />


<?php  
  require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraNotas/js.php';
?>
<?php

require APPRAIZ . "obras2/includes/principal/popUpAcompanhamentoNotaFiscal/ctrl.php";


?>

<body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/obras2.js?v=3"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<?php
require APPRAIZ . "obras2/includes/principal/popUpAcompanhamentoNotaFiscal/css.php";
?>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">

    <input type="hidden" name="ntfid" id="ntfid" value="<?php echo $ntfid; ?>" />
    <input type="hidden" name="medid" id="medid" value="<?php echo $medid; ?>" /> 
    <!-- FORMULÁRIO DE INCLUSÃO DE NOTA FISCAL -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <?php
            $tituloTela = $ntfid ? "Editar Nota Fiscal" : "Incluir Nota FIscal";
            ?>
            <td class="SubTitulocentro" colspan="2"><?php echo $tituloTela; ?></td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Fornecedor:
                        </td>
                        <td>
                            <?php
                            if ($acaoEditar) {
                                $tipoEmpresaContratada = $arrDadosNotaFiscal["tipoempresa"];
                                $idEmpresaContratada = $arrDadosNotaFiscal["idempresa"];
                                $idContrato = $arrDadosNotaFiscal["idcontrato"];
                                $tipoContrato = $arrDadosNotaFiscal["tipocontrato"];
                                $selectedValue = $tipoEmpresaContratada . "_" . $idEmpresaContratada . "_" . $idContrato;
                                /* Criação de campo hidden utilizado para automatizar a seleção da empresa na tela de editar e, consequentemente
                                 * chamar o serviço para a busca da UF. */
                                echo "<input type='hidden' name='idEmpresaContratada' id='idEmpresaContratada' value={$selectedValue} />";
                                echo montaComboEmpresaContratada($obrid, $selectedValue);
                            } else {
                                echo montaComboEmpresaContratada($obrid);
                            }
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            UF Fornecedor:
                        </td>
                        <td class="detalheVisualizar" id="ufFornecedor"></td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Nota Fiscal:
                        </td>
                        <td>
                            <?php
                            if ($acaoEditar) {
                                if (is_array($arquivoNotaFiscal) && !empty($arquivoNotaFiscal)) {
                                    ?>
                                    <div class="divArquivoDownload">
                                        <a class="link"
                                           onclick="excluirArquivo('<?= $arrDadosNotaFiscal['arqid'] ?>');">
                                            <img src="/imagens/excluir.gif"
                                                 title="Excluir arquivo <?= $arquivoNotaFiscal['arqnome'] ?>" />
                                        </a>
                                        &nbsp;
                                        <a class="link"
                                           onclick="downloadArquivo('<?= $arrDadosNotaFiscal['arqid'] ?>');">
                                        <img src="/imagens/salvar.png"
                                             title="Fazer download do arquivo." />
                                        <?= $arquivoNotaFiscal['arqnome'] . '.' . $arquivoNotaFiscal['arqextensao'] ?>
                                        </a>
                                    </div>
                                    <input type="hidden" name="arquivoAtual" id="arquivoAtual" value="<?= $arrDadosNotaFiscal['arqid'] ?>" />
                                    <div class="divArquivoUpload" style="display: none;">
                                        <input type="hidden" name="arqidOld" id="arqidOld" value="" />
                                        <input type="file" name="arquivoNota" id="arquivoNota"/>
                                        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                        <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                    </div>
                                    <?php
                                }
                            } else {
                                ?>
                                <div class="divArquivoUpload">
                                    <input type="file" name="arquivoNota" id="arquivoNota"/>
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                    <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                </div>
                                <?php
                            }
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Número da Nota Fiscal:
                        </td>
                        <td>
                            <?php
                            $numeroNotaFiscal = $acaoEditar ? $arrDadosNotaFiscal["ntfnumnota"] : null;
                            ?>
                            <input type="text" style="text-align:left;" name="numeroNota"
                                   id="numeroNota" size="15" maxlength="10" value="<?php echo $numeroNotaFiscal; ?>"
                                   onKeyUp="this.value=mascaraglobal('##########',this.value);"
                                   onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();"
                                   onmouseout="MouseOut(this);" onblur="MouseBlur(this);"
                                   title="Número da Nota Fiscal." class="obrigatorio normal"
                            />
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." />
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Data da Nota Fiscal:
                        </td>
                        <td>
                            <?php
                            $dataNotaFiscal = $acaoEditar ? $arrDadosNotaFiscal["ntfdtnota"] : null;
                            echo campo_data2("dataNota", "N", "S", "Data da Nota Fiscal.", "", null, "calcularPercentualMedicoes();", $dataNotaFiscal, "calcularPercentualMedicoes();");
                            ?>
                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                        </td>
                    </tr>

                    <tr>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor da Nota Fiscal:
                        </td>
                        <td>
                            <?php
                            /* @todo VERFICIAR O USO DESTA VARIÁVEL. */
                            $valorNotaFiscal = $acaoEditar ? number_format($arrDadosNotaFiscal["ntfvalornota"], 2, ",", ".") : null;
                            ?>
                            <input
                                    type="text"
                                    style="text-align:left;"
                                    name="valorNota"
                                    id="valorNota"
                                    onmouseout="MouseOut(this);"
                                    onfocus="MouseClick(this);this.select();"
                                    onmouseover="MouseOver(this);"
                                    onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                    maxlength="18"
                                    size="17"
                                    class="obrigatorio normal"
                                    value="<?php echo $valorNotaFiscal; ?>"
                            />
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE INCLUSÃO DE NOTA FISCAL. -->

    <!-- FORMULÁRIO DE BOLETIM DE MEDICAO -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTitulocentro" colspan="2">Medições</td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Contrato ou Aditivo de Valor:
                        </td>
                        <td>
                            <?php
                            if ($acaoEditar) {
                                if ($tipoEmpresaContratada === "ent") {
                                    echo montaComboContratosAditivos($obrid, $idContrato, null, $medid);
                                }

                                if ($tipoEmpresaContratada === "cex") {
                                    echo montaComboContratosAditivos($obrid, null, $idContrato, $medid);
                                }
                            } else {
                                ?>
                                <select name="contratos" id="contratos">
                                    <option value="">Selecione...</option>
                                </select>
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                                <?php
                            }
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Boletim de Medição:
                        </td>
                        <td>
                            <select name="medicao" id="medicao" onchange="getDadosMedicao(this)">
                                <option value="">Selecione...</option>
                            </select>
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor da Medição:
                        </td>
                        <td class="detalheVisualizar" id="valorMedicao"></td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor Pago na Nota Fiscal:
                        </td>
                        <td>
                            <input
                                    type="text"
                                    style="text-align:left;"
                                    name="valorPagoNota"
                                    id="valorPagoNota"
                                    onmouseout="MouseOut(this);"
                                    onfocus="MouseClick(this);this.select();"
                                    onmouseover="MouseOver(this);"
                                    onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                    onblur="calcularPercentualMedicoes();"
                                    maxlength="18"
                                    size="17"
                                    class="obrigatorio normal"
                            />
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            % Medição Física na NF:
                        </td>
                        <td class="detalheVisualizar"><span id="medicaoFisica"></span></td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            % Medição Física Acumulada:
                        </td>
                        <td class="detalheVisualizar"><span id="medicaoAcumulada"></span></td>
                    </tr>

                    <tr>
                        <td colspan="2" style="text-align: center">
                            <br/>
                            <input type="button" value="Adicionar Nota Fiscal" id="botaoAtualizarLista"
                                   onclick="atualizarListaContratos()"/>
                        </td>
                    </tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE BOLETIM DE MEDICAO. -->

    <!-- Div para armazenar os dados da medição -->
    <div id="dadosMedicao" style="display: none">
        <input type="hidden" id="medid" value="">
        <input type="hidden" id="dataMedicao" value="">
        <input type="hidden" id="periodo" value="">
        <input type="hidden" id="vlrMedicao" name="vlrMedicao" value="">
        <input type="hidden" id="vlrReferencia" value="">
    </div>
    <!-- Fim Div para armazenar os dados da medição -->

    <br/>

    <!-- INÍCIO DA TABELA DE ACOMPANHAMENTO. -->
    <?php
        $display = $acaoEditar ? "block" : "none";
    ?>
    <div id="acompanhamento" style="display: <?php echo $display; ?>">

        <table width="95%" align="center" border="1" style="border-collapse: collapse" id="tbAcompanhamento">
            <thead>
            <tr>
                <td class="tbAcompanhamentoTitulo">Ações</td>
                <td class="tbAcompanhamentoTitulo">Numero da Medição</td>
                <td class="tbAcompanhamentoTitulo">Data da Mediçao</td>
                <td class="tbAcompanhamentoTitulo">Período de Execução</td>
                <td class="tbAcompanhamentoTitulo">Valor da Medição</td>
                <td class="tbAcompanhamentoTitulo">Valor Pago na Nota Fiscal</td>
                <td class="tbAcompanhamentoTitulo">% Medição Física na NF</td>
                <td class="tbAcompanhamentoTitulo">% Mediçao Fisica Acumulada</td>
            </tr>
            </thead>
            <?php

            if ($acaoEditar) {
                ?>
                <tbody>
                <?php
                $arrDadosNotaMedicao = getDadosNotaMedicao($ntfid);
                $arrM = array();

                $execucaoFinanceira = new ExecucaoFinanceira();

                foreach ($arrDadosNotaMedicao as $k=>$dadosNotaMedicao) {


                    $ntmid = $dadosNotaMedicao["ntmid"];
                    $mecid = $dadosNotaMedicao["mecid"];
                    $dataNotaFiscal = $dadosNotaMedicao["ntfdtnota"];
                    $numeroMedicao = $dadosNotaMedicao["mednummedicao"];
                    $dataMedicao = $dadosNotaMedicao["meddtmedicao"];

                    $periodoExec = (!empty($dadosNotaMedicao["meddtinicioexec"]) && !empty($dadosNotaMedicao["meddtfimexec"])) ? $dadosNotaMedicao["meddtinicioexec"] . " à " . $dadosNotaMedicao["meddtfimexec"] : "";

                    //$periodoExec = $dadosNotaMedicao["meddtinicioexec"] . " à " . $dadosNotaMedicao["meddtfimexec"];
                    $valorMedicao = $dadosNotaMedicao["medvlrmedicao"];
                    $valorMedicao = number_format($valorMedicao, 2, ",", ".");
                    $valorPagoNota = $dadosNotaMedicao["ntmvlrpago"];
                    $valorPagoNotaFormatado = number_format($valorPagoNota, 2, ",", ".");

                    $arrIdContratoNotaMedicao = getIdContratoNotaMedicao($mecid);
                    $crtid = $arrIdContratoNotaMedicao["crtid"];
                    $cceid = $arrIdContratoNotaMedicao["cceid"];

                    $valorContratoOriginal = $execucaoFinanceira->getValorContratoAtual($crtid, $cceid);

                    // Calculando o percentual da medição física.
                    $percentualMedicaoFisica = $execucaoFinanceira->calcularPercentualMedicao($valorPagoNota, $valorContratoOriginal);
                    $percentualMedicaoFisica = number_format($percentualMedicaoFisica, 2, ",", ".");
                    $percentualMedicaoFisica = $percentualMedicaoFisica . "%";

                    // Calculando o percentual da medição acumulada.
                    $tipoEmpresaContratada = $crtid ? "ent" : "cex";

                    if ($tipoEmpresaContratada === "cex") {
                        $percentualMedicaoFisica = ("Não se aplica.");
                        $percentualMedicaoAcumulada = ("Não se aplica.");

                    } else {

                        $valorContratoPrincipal = (float)$execucaoFinanceira->getValorContratoPrincipal($obrid);

                        $arrObras = $execucaoFinanceira->retornaObrids($obrid);
                        if (count($arrObras) > 1) {
                            $somatorioValoresPagosNotaOutrosContratos = (float)$execucaoFinanceira->getSomatorioValoresPagosNota($obrid, true);
                            $x = $valorContratoPrincipal + $somatorioValoresPagosNotaOutrosContratos;
                        } else {
                            $x = $valorContratoPrincipal;
                        }

                        $y = ((float)$execucaoFinanceira->getSomatorioValoresPagosNota($obrid)) + $valorPagoNota;

                        /*$somatorioNotasPagasOutrosContratos = getSomatoriosNotasPagasOutrosContratos($tipoEmpresaContratada, $arrObras, $dataNotaFiscal);
                        $somatorioNotasPagasOutrosContratos += $valorPagoNota;*/

                        //$percentualMedicaoAcumulada = $execucaoFinanceira->calcularPercentualMedicao($somatorioNotasPagasOutrosContratos, $somatorioTodosContratos);
                        $percentualMedicaoAcumulada = $execucaoFinanceira->calcularPercentualMedicao($y, $x);
                        $percentualMedicaoAcumulada = number_format($percentualMedicaoAcumulada, 2, ",", ".");
                        $percentualMedicaoAcumulada = $percentualMedicaoAcumulada . "%";
                    }

                    $arrTmp = array(
                        "id" => $k,
                        "ntmid" => $ntmid,
                        "idContrato" => $tipoContrato . '_' . $idContrato,
                        "idMedicaoContrato" => $mecid,
                        "valorPagoNota" => $dadosNotaMedicao["ntmvlrpago"]
                    );

                    array_push($arrM, $arrTmp);

                    echo "<tr>";
                    echo "<td><img src='/imagens/excluir.gif' alt='Excluir' title='Excluir' onclick='removerNotaTabelaAcompanhamento(this, {$k}, {$mecid}, {$numeroMedicao})' style='cursor:pointer;' />";
                               if($periodoExec){
                                 echo " <span class='acaoTbAcompanhamento' alt='Editar Nota Fiscal' title='Editar Nota Fiscal' 
                                        onclick='editarNotaFiscal(this, {$k}, {$mecid}, {$numeroMedicao}, \"" . $dadosNotaMedicao['contrato'] . "\",  \"" . $dadosNotaMedicao['ntmvlrpago'] . "\",  \"" .$valorMedicao. "\", \"" . $percentualMedicaoFisica . "\" , \"" . $percentualMedicaoAcumulada . "\", ".$dadosNotaMedicao['mecid']." )'>
                                        <img src='/imagens/alterar.gif' alt='Editar Nota Fiscal'/></span>";
    							}
    				echo "</td>";
                    echo "<td>{$numeroMedicao}</td>";
                    echo "<td>{$dataMedicao}</td>";
                    echo "<td>{$periodoExec}</td>";
                    echo "<td>{$valorMedicao}</td>";
                    echo "<td>{$valorPagoNotaFormatado}</td>";
                    echo "<td>{$percentualMedicaoFisica}</td>";
                    echo "<td>{$percentualMedicaoAcumulada}</td>";
                    echo "<tr>";
                }

                $arrM = json_encode($arrM);
                echo "<input type='hidden' name='dadosMedicaoNota' id='dadosMedicaoNota' value='{$arrM}' />";

                ?>
                </tbody>
            <?php } else { ?>
                <tbody></tbody>
            <?php } ?>

        </table>

        <br/>

        <div style="text-align: center">
            <input type="submit" id="botaoSalvar" value="Salvar""/>
            <input type="button" id="botaFechar" value="Fechar" onclick="javascript:window.close();"/>
        </div>
    </div>
    </div>
    <!-- FIM DA TABELA DE ACOMPANHAMENTO. -->

    <input type="hidden" id="requisicao" name="requisicao" value="salvarNotaMedicao"/>
    <input type="hidden" id="dadosNotaFiscalMedicao" name="dadosNotaFiscalMedicao" />
    <input type="hidden" id="notasRemovidas" name="notasRemovidas" />

</form>

<?php
require APPRAIZ . "obras2/includes/principal/popUpAcompanhamentoNotaFiscal/js.php";
?>


</body>
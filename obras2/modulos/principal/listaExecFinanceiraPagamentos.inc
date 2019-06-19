<?php
require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraPagamentos/ctrl.php';
require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraPagamentos/css.php';

?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

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
                <img src="/imagens/atencao.png"/> Todos os valores retirados da conta corrente da obra deverão estar
                discriminados documentados abaixo.
                Valores retirados da conta sem a inserção de documentação comprobatória (Notas Fiscais, Comprovantes de
                Pagamentos e Boletins de medição)
                resultar em abertura de Tomada de Contas Especial.
            </td>
        </tr>
    </table>
    <div class="abaContent">
        <?php if (!$execucaoFinanceiraFinalizada) { ?>
            <br/>
            <button class="btn btn-primary" onclick="incluirPagamentos()">Incluir Pagamentos</button>
            <br/><br/>
            <?php
        }
        $arrConstrutoras = buscaConstrutoras($obrid);
        if (empty($arrConstrutoras)) {
            $arrConstrutoras = array();
        }
        foreach ($arrConstrutoras

                 as $construtora) {
        ?>


        <div class="construtoraContent">
            <table class="table table-bordered">
                <thead>
                <tr>
                    <td colspan="7" style="text-align: left">
                        <p>Empresa Contratada:
                            <?= "(" . formatar_cnpj($construtora['entnumcpfcnpj']) . ")&nbsp;" . $construtora['entnome'] . "&nbsp;(Contrato: " . $construtora['crtdtassinatura'] . ")" ?>
                        </p>
                    </td>
                </tr>
                <tr>
                    <td>Ações</td>
                    <td>Nº da Transação</td>
                    <td>Data da Transação</td>
                    <td>Tipo de Pagamento</td>
                    <td>Valor da Transação</td>
                    <td>Favorecido</td>
                    <td>Notas Fiscais</td>
                </tr>
                </thead>


                <tbody>
                <?php
                $arrLista = getListaPrincipal($obrid, $construtora, 'padrao');

                if (count($arrLista) > 0) {
                    $totalPagamentos = 0;


                    foreach ($arrLista as $k => $v) {
                        $idAtual = $v["id_pagamento"];
                        $totalPagamentos += $v['valor_pagamento'];
                        ?>
                        <tr>
                            <td>
                                <?php
                                if ($v['tipo_de_pagamento'] == "") {
                                    ?>
                                    <img src='../imagens/atencao.png' title='Pendência de preenchimento'
                                         width='16'
                                         style=cursor:pointer;'>&nbsp;
                                    <?php
                                }
                                if (!$execucaoFinanceiraFinalizada) {
                                    ?>
                                    <img src='../imagens/alterar.gif' title='Editar Pagamento' width='16'
                                         style=cursor:pointer;
                                         onclick='editarPagamentos(<?php echo $idAtual; ?>)'>&nbsp;
                                    <img src='../imagens/excluir.gif' title='Excluir Pagamento' width='16'
                                         style=cursor:pointer;
                                         onclick='excluirPagamentos(<?php echo $idAtual; ?>)'>&nbsp;
                                <?php } ?>
                                <img src='../imagens/salvar.png' title='Download do arquivo da transação bancária'
                                     width='16'
                                     style=cursor:pointer; onclick='downloadArquivo(<?php echo $v['id_arquivo']; ?>)'>&nbsp;

                            </td>
                            <td><?php echo $v['numero_transacao']; ?></td>
                            <td><?php echo $v['data_pagamento']; ?></td>
                            <td><?php echo $v['tipo_de_pagamento']; ?></td>
                            <td><?php echo number_format($v['valor_pagamento'], 2, ",", "."); ?></td>
                            <td><?php echo $v['favorecido_pagamento']; ?></td>
                            <td><?php echo $v['notas_fiscais']; ?></td>

                        </tr>
                        <?php
                        $idAtual = "";
                    }
                } else {
                    ?>
                    <td colspan="9" style="color:#cc0000;" align="center">Não foram encontrados Registros.</td>
                    <?php
                }
                if (count($arrLista) > 0) { ?>
                    <tr>
                        <td colspan="3"></td>

                        <td style="background-color: #DCDCDC"><b>Total de Pagamentos:</b></td>
                        <td style="background-color: #DCDCDC"><b><?php echo number_format($totalPagamentos, 2, ",",
                                    "."); ?></b></td>
                        <td colspan="2"></td>


                    </tr>
                    <?php
                }
                ?>

                </tbody>
            </table>
            <?php
            }
            ?>
        </div>
        <br/>
        <div class="divFaixa" align="center">
            <p style="font-weight: bold; font-size: 16px;">Construtoras e Contratos Incluídos</p>
        </div>
        <br/>
        <?php

        $arrConstrutoras = buscaConstrutoras($obrid, 'extra');
        if (empty($arrConstrutoras)) {
            $arrConstrutoras = array();
        }
        foreach ($arrConstrutoras

        as $construtoraEx) {
        ?>


        <div class="construtoraContent">
            <table class="table table-bordered">
                <thead>
                <tr>
                    <td colspan="5">
                        <div class="row">
                            <div class="" style='text-align: left'><b>Empresa Contratada:</b>
                                <?= "(" . formatar_cnpj($construtoraEx['cexnumcnpj']) . ")&nbsp;" . $construtoraEx['cexrazsocialconstrutora'] . "(Contrato: " . formata_data($construtoraEx['ccedataassinatura']) . ")";
                                ?>
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td>Ações</td>
                    <td>Nº da Transação</td>
                    <td>Data da Transação</td>
                    <td>Tipo de Pagamento</td>
                    <td>Valor da Transação</td>
                    <td>Favorecido</td>
                    <td>Notas Fiscais</td>
                </tr>
                </thead>


                <tbody>
                <?php
                $arrLista = getListaPrincipal($obrid, $construtoraEx, 'extra');

                if (count($arrLista) > 0) {
                    $totalPagamentos = 0;


                    foreach ($arrLista as $k => $v) {
                        $idAtual = $v["id_pagamento"];
                        $totalPagamentos += $v['valor_pagamento'];

                        ?>
                        <tr>
                            <td>

                                <?php if (!$execucaoFinanceiraFinalizada) { ?>
                                    <img src='../imagens/alterar.gif' title='Editar Medição' width='16'
                                         style=cursor:pointer;
                                         onclick='editarPagamentos(<?php echo $idAtual; ?>)'>&nbsp;
                                    <img src='../imagens/excluir.gif' title='Excluir Medição' width='16'
                                         style=cursor:pointer;
                                         onclick='excluirPagamentos(<?php echo $idAtual; ?>)'>&nbsp;
                                <?php } ?>
                                <img src='../imagens/salvar.png' title='Download Medição' width='16'
                                     style=cursor:pointer; onclick='downloadArquivo(<?php echo $v['id_arquivo']; ?>)'>&nbsp;

                            </td>
                            <td><?php echo $v['numero_transacao']; ?></td>
                            <td><?php echo $v['data_pagamento']; ?></td>
                            <td><?php echo $v['tipo_de_pagamento']; ?></td>
                            <td><?php echo number_format($v['valor_pagamento'], 2, ",", "."); ?></td>
                            <td><?php echo $v['favorecido_pagamento']; ?></td>
                            <td><?php echo $v['notas_fiscais']; ?></td>

                        </tr>
                        <?php
                        $idAtual = "";
                    }
                } else {
                    ?>
                    <td colspan="9" style="color:#cc0000;" align="center">Não foram encontrados Registros.</td>
                    <?php
                }
                if (count($arrLista) > 0) {
                    ?>
                    <tr>
                        <td colspan="3"></td>

                        <td style="background-color: #DCDCDC"><b>Total de Pagamentos:</b></td>
                        <td style="background-color: #DCDCDC"><b><?php echo number_format($totalPagamentos, 2, ",",
                                    "."); ?></b></td>
                        <td colspan="2"></td>


                    </tr>
                    <?php
                }
                ?>

                </tbody>
            </table>
            <?php
            }
            ?>
        </div>
        <br/>
    </div>

    <br/><br/>

    <div style="text-align: right">
        <a id="mostraDivDadosAntigos" href="#dados_antigos_div">[-] Esconde Dados Antigos</a>
    </div>

    <div id="dados_antigos">
        <a href="dados_antigos_div"><p>&nbsp;</p></a>

        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr style="display: <?php echo $displayEmpenhado ?>;">
                <td class="subtitulocentro" colspan="2">
                    Detalhamento Orçamentário Empenhado
                </td>
            </tr>
            <tr style="display: <?php echo $displayEmpenhado ?>;">
                <td colspan="2">

                    <center>
                        <div class="div_rolagem" style="width: 95%; height: 150px;" id="divDetalhamentoEmpenhado">
                            <?php
                            $param = $_POST;
                            if ($_GET['acao'] == 'A') {
                                $param['empid'] = $_SESSION['obras2']['empid'];
                            } else {
                                $param['obrid'] = $_SESSION['obras2']['obrid'];
                            }
                            $param['eorid'] = $eorid;
                            $param['ioctipodetalhamento'] = TIPO_DETALHAMENTO_EXECUCAO_EMPENHADO;
                            if ($habilitado == false) {
                                $param['block_imgs_acao'] = true;
                            }
                            $itemExecOrcamentaria = new ItensExecucaoOrcamentaria();
                            $sql = $itemExecOrcamentaria->listaSql($param);

                            $cabecalho = array(
                                "Data",
                                "Valor Empenhado (R$)",
                                "% Empenhado",
                                "Arquivo",
                                "Inserido por"
                            );
                            $db->monta_lista($sql, $cabecalho, 100, 5, 'S', 'center', 'S');
                            ?>
                        </div>
                    </center>

                </td>
            </tr>
            <tr style="display: <?php echo $displayLiquidado ?>;">
                <td class="subtitulocentro" colspan="2" style="border-top: 1px solid #000000;">
                    Detalhamento Orçamentário Liquidado
                </td>
            </tr>
            <tr style="display: <?php echo $displayLiquidado ?>;">
                <td colspan="2">

                    <center>
                        <div class="div_rolagem" style="width: 95%; height: 150px;" id="divDetalhamentoLiquidado">
                            <?php
                            $param = $_POST;
                            if ($_GET['acao'] == 'A') {
                                $param['empid'] = $_SESSION['obras2']['empid'];
                            } else {
                                $param['obrid'] = $_SESSION['obras2']['obrid'];
                            }
                            $param['eorid'] = $eorid;
                            $param['ioctipodetalhamento'] = TIPO_DETALHAMENTO_EXECUCAO_LIQUIDADO;
                            if ($habilitado == false) {
                                $param['block_imgs_acao'] = true;
                            }
                            $itemExecOrcamentaria = new ItensExecucaoOrcamentaria();
                            $sql = $itemExecOrcamentaria->listaSql($param);

                            $cabecalho = array(
                                "Data",
                                "Valor Liquidado (R$)",
                                "% Liquidado",
                                "Arquivo",
                                "Inserido por"
                            );
                            $db->monta_lista($sql, $cabecalho, 100, 5, 'S', 'center', 'S');
                            ?>
                        </div>
                    </center>

                </td>
            </tr>
            <tr bgcolor="#c0c0c0">
                <td>&nbsp;</td>
                <td>&nbsp;</td>
            </tr>
        </table>

    </div>
</div>

<br/>

<?php  
    require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraPagamentos/js.php';
?>
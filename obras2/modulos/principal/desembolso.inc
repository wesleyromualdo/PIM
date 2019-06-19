<?php
    require APPRAIZ . 'obras2/includes/principal/desembolso/ctrl.php';

    echo '<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>';

    $s = new SolicitacaoDesembolso();
    $s->verificaTramiteSolicitacoes($obrid);

    if($obra->tooid != 2) {

    $solicitacoes = $db->carregar(getSqlTooIdNotIs2($obrid));
?>
    <table align="center" bgcolor="#f5f5f5" border="0" class="Tabela" cellpadding="3" cellspacing="1">
        <tbody>
        <?
        if ($solicitacoes):
            ?>
            <? foreach ($solicitacoes as $solicitacao): ?>


            <tr>
                <td class="SubTituloCentro" colspan="3">
                    Solicitação Nº <?= $solicitacao['sldid'] ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">
                    Situação
                </td>
                <td width="80%">
                    <img
                        align="absmiddle"
                        src="/imagens/alterar.gif"
                        style="cursor: pointer"
                        onclick="javascript: abreSolicitacao('<?= $solicitacao['sldid'] ?>');"
                        title="Abrir Solicitação">


                    <? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) : ?>
                        <? if ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO_SEM_PAGAMENTO) : ?>
                            <b style="color:#00AA00"><?= $solicitacao['esddsc'] ?></b>
                        <? elseif ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO) : ?>
                            <b style="color:red"><?= $solicitacao['esddsc'] ?></b>
                        <? else: ?>
                            <b><?= $solicitacao['esddsc'] ?></b>
                        <? endif; ?>
                    <? else: ?>
                        <? if ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO_SEM_PAGAMENTO) : ?>
                            <b style="color:#00AA00"><?= $solicitacao['esddsc'] ?></b>
                        <? elseif ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO) : ?>
                            <b style="color:red"><?= $solicitacao['esddsc'] ?></b>
                        <? elseif ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO) : ?>
                            <b><?= $solicitacao['esddsc'] ?></b>
                        <? else: ?>
                            <b style="">Aguardando Análise FNDE</b>
                        <? endif; ?>
                    <? endif; ?>

                </td>

            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">
                    Data da Solicitação
                </td>
                <td width="80%">
                    <?= $solicitacao['slddatainclusao'] ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">
                    Percentual Solicitado
                </td>
                <td width="80%">
                    <?= number_format($solicitacao['sldpercsolicitado'], 2, ',', '.') ?>%
                </td>
            </tr>

            <? if ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO_SEM_PAGAMENTO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO) : ?>
            <tr>
                <td class="SubTituloDireita" width="20%">
                    Análise FNDE
                </td>
                <td>

                    <table >
                            <tr>
                                <td class="SubTituloDireita" width="30%">
                                    Percentual Físico Total
                                </td>
                                <td width="80%">
                                    <?= number_format($solicitacao['sldpercobra'], 2, ',', '.') ?>%
                                </td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita" width="30%">
                                    Percentual a ser Desembolsado
                                </td>
                                <td width="80%">
                                    <?= number_format($solicitacao['sldpercpagamento'], 2, ',', '.') ?>%
                                </td>
                            </tr>


                            <tr>
                                <td class="SubTituloDireita" width="30%">
                                    Analisado Por
                                </td>
                                <td width="80%">
                                    <?= $solicitacao['usunome'] ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita" width="30%">
                                    Observações da Análise
                                </td>
                                <td width="80%">
                                    <?= ($solicitacao['sldobstec']) ? $solicitacao['sldobstec'] : $solicitacao['cmddsc'] ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita" width="20%">
                                    Situação do Pedido 1
                                </td>
                                <td width="80%">

                                    <? if($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO_SEM_PAGAMENTO): ?>
                                        <?= $solicitacao['cmddsc']; ?>
                                    <? else: ?>
                                        <?= ($solicitacao['situacao_pagamento']) ? $solicitacao['situacao_pagamento'] : '-' ?>
                                    <? endif; ?>
                                </td>
                            </tr>
                            
                    </table>
                        
                </td>
                  </tr>
                <?php

                    if( $solicitacao['sldpedidocancelado'] OR $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO_SEM_PAGAMENTO )
                    {
                    ?>
                     <tr>
                        <td class="SubTituloDireita" width="20%">
                            Pedido Finalizado
                        </td>
                        <td width="80%">
                         <table width="100%">
                            <tr>
                                <td class="SubTituloDireita" width="30%">
                                    Usuário
                                </td>
                                <td width="80%">
                                     
                                     <?php
                                         if( $solicitacao['sldcpfcancelamento'] )
                                         {
                                         	
                                         	$sqlUsu = "select usunome from seguranca.usuario  where usucpf = '{$solicitacao['sldcpfcancelamento']}' ";
                                         	$nomeUsuario = $db->pegaUm($sqlUsu);
                                         	$nomeUsuario = ( $nomeUsuario ) ?  $nomeUsuario : 'Usuário não localizado';
                                         	echo "{$solicitacao['sldcpfcancelamento']} - {$nomeUsuario} " . ' - ' ;
                                         	echo ( $solicitacao['data_cancelamento']) ?  $solicitacao['data_cancelamento'] : '-' ;
                                         }
                                     ?>
                                     
                                </td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita" width="30%">
                                    Justificativa
                                </td>
                                <td width="80%">
                                     <?= ( $solicitacao['sldjustificativacancelamento']) ?  $solicitacao['sldjustificativacancelamento'] : '-' ?>
                                </td>
                            </tr>
                           </table>
                            
                        </td>
                    </tr>
                    <?php 
                    }
                    ?>
            <? endif; ?>
        <? endforeach; ?>
        <? else: ?>
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    Nenhuma solicitação feita.
                </td>
            </tr>
        <? endif; ?>
        </tbody>
    </table>

    <script type="text/javascript">
        function abreSolicitacao(sldid) {
            var url = "/obras2/obras2.php?modulo=principal/popupSolicitarDesembolso&acao=A" +
                "&sldid=" + sldid;
            popup1 = window.open(
                url,
                "solicitarDesembolso",
                "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
            );
            return false;
        }
    </script>

<?
} else {
    $sqlSolicitacoes =  getSqlSolicitacoes($obrid, $dadosProcessoConv['pronumeroprocesso']);
    $solicitacoes = $db->carregar($sql);
    ?>
    <table align="center" bgcolor="#f5f5f5" border="0" class="Tabela" cellpadding="3" cellspacing="1">
        <tbody>
        <?
        if ($solicitacoes):
            ?>
            <? foreach ($solicitacoes as $solicitacao): ?>


            <tr>
                <td class="SubTituloCentro" colspan="3">
                    Solicitação Nº <?= $solicitacao['sdcid'] ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">
                    Situação
                </td>
                <td width="80%">
                    <img
                        align="absmiddle"
                        src="/imagens/alterar.gif"
                        style="cursor: pointer"
                        onclick="javascript: abreSolicitacao('<?= $solicitacao['sdcid'] ?>');"
                        title="Abrir Solicitação">


                    <? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) : ?>
                        <? if ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO_SEM_PAGAMENTO) : ?>
                            <b style="color:#00AA00"><?= $solicitacao['esddsc'] ?></b>
                        <? elseif ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO) : ?>
                            <b style="color:red"><?= $solicitacao['esddsc'] ?></b>
                        <? else: ?>
                            <b><?= $solicitacao['esddsc'] ?></b>
                        <? endif; ?>
                    <? else: ?>
                        <? if ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO_SEM_PAGAMENTO) : ?>
                            <b style="color:#00AA00"><?= $solicitacao['esddsc'] ?></b>
                        <? elseif ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO) : ?>
                            <b style="color:red"><?= $solicitacao['esddsc'] ?></b>
                        <? elseif ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO) : ?>
                            <b><?= $solicitacao['esddsc'] ?></b>
                        <? else: ?>
                            <b style="">Aguardando Análise FNDE</b>
                        <? endif; ?>
                    <? endif; ?>

                </td>

            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">
                    Data da Solicitação
                </td>
                <td width="80%">
                    <?= $solicitacao['sdcdatainclusao'] ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">
                    Percentual Solicitado
                </td>
                <td width="80%">
                    <?= number_format($solicitacao['sdcpercsolicitado'], 2, ',', '.') ?>%
                </td>
            </tr>

            <? if ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO_SEM_PAGAMENTO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO) : ?>
                <tr>
                <td class="SubTituloDireita" width="20%">
                    Análise FNDE
                </td>
                <td>

                    <table>
                        <tr>
                            <td class="SubTituloDireita" width="30%">
                                Percentual de execução do convênio validado.
                            </td>
                            <td width="80%">
                                <?= number_format($solicitacao['sdcpercpagamento'], 2, ',', '.') ?>%
                            </td>
                        </tr>

                        <tr>
                            <td class="SubTituloDireita" width="30%">
                                Parcela
                            </td>
                            <td width="80%">
                                <?= $solicitacao['sdcparcela'] ?>
                            </td>
                        </tr>


                        <tr>
                            <td class="SubTituloDireita" width="30%">
                                Analisado Por
                            </td>
                            <td width="80%">
                                <?= $solicitacao['usunome'] ?>
                            </td>
                        </tr>

                        <tr>
                            <td class="SubTituloDireita" width="30%">
                                Observações da Análise
                            </td>
                            <td width="80%">
                                <?= ($solicitacao['sldobstec']) ? $solicitacao['sldobstec'] : $solicitacao['cmddsc'] ?>
                            </td>
                        </tr>

                        <tr>
                            <td class="SubTituloDireita" width="20%">
                                Situação do Pedido 2
                            </td>
                            <td width="80%">
                                <?= ($solicitacao['situacao_pagamento']) ? $solicitacao['situacao_pagamento'] : '-' ?>
                            </td>
                        </tr>
                    </table>
                </td>
            <? endif; ?>

        <? endforeach; ?>
        <? else: ?>
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    Nenhuma solicitação feita.
                </td>
            </tr>
        <? endif; ?>
        </tbody>
    </table>

    <script type="text/javascript">
        function abreSolicitacao(sdcid) {
            var url = "/obras2/obras2.php?modulo=principal/popupSolicitarDesembolsoConvenio&acao=A&obrid=<?=$obrid?>&processo=<?=$dadosProcessoConv['pronumeroprocesso']?>&convenio=<?=$dadosProcessoConv['termo_convenio']?>" +
                "&sdcid=" + sdcid;
            popup1 = window.open(
                url,
                "solicitarDesembolso",
                "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
            );
            return false;
        }
    </script>

<?
}
?>
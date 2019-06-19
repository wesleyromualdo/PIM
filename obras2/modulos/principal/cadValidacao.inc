<?php
    require APPRAIZ . 'obras2/includes/principal/cadValidacao/ctrl.php';
?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    
    <table class="listagem" width="95%" cellSpacing="1" cellPadding="3" align="center">
        <tr> 
            <td style="width: 50%; vertical-align: top; ">
                <div style=" height: 240px; max-height: 240px; overflow: auto;">
                    <table class="listagem" width="95%" cellSpacing="1" cellPadding="3" align="center">
                        <tr>
                            <td class="SubTituloEsquerda" colspan="8">Checklist</td>
                        </tr>
                        <tr>
                            <td class="subtituloesquerda" colspan="8">

                            <span id="cad_chk_adm_sp" style="color: #0000CC; cursor: pointer;">
                                &nbsp; <img alt="Inserir Checklist Administrativo Simplificado"
                                            title="Inserir Checklist Administrativo Simplificado" src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist Administrativo Simplificado
                            </span>

                            <span id="cad_chk_adm_2015" style="color: #0000CC; cursor: pointer;">
                                &nbsp; <img alt="Inserir Checklist Administrativo"
                                            title="Inserir Checklist Administrativo" src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist Administrativo
                            </span> 

                            <span id="cad_chk_tec_2015" style="color: #0000CC; cursor: pointer;">
                            &nbsp; <img alt="Inserir Checklist Técnico" title="Inserir Checklist Técnico"
                                        src="/imagens/gif_inclui.gif">
                            &nbsp; Inserir Checklist Técnico
                            </span>
                            <span id="cad_chk_2p" style="color: #0000CC; cursor: pointer;"> 
                                &nbsp; <img alt="Inserir Checklist da 1° Parcela"
                                            title="Inserir Checklist  da 1° Parcela" src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist da 1° Parcela
                            </span> 
                            <span id="cad_chk_obr_vinc" style="color: #0000CC; cursor: pointer;"> 
                                &nbsp; <img alt="Inserir Checklist de Obra Vinculada"
                                            title="Inserir Checklist de Obra Vinculada" src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist de Obra Vinculada
                            </span>
                                <?php if (obraMi($obrid)) { ?>
                                    <span id="cad_chk_obr_mi" style="color: #0000CC; cursor: pointer;">
                                &nbsp; <img alt="Inserir Checklist de Obra MI" title="Inserir Checklist de Obra MI"
                                            src="/imagens/gif_inclui.gif">
                                &nbsp; Inserir Checklist de Obra MI
                                <?php } ?>
                            </span>

                            </td>
                        </tr>
                        <tr>
                            <td class="subtituloesquerda" style="width: 65px;"> Ação</td>
                            <td class="subtituloesquerda"> ID</td>
                            <td class="subtituloesquerda"> Tipo</td>
                            <td class="subtituloesquerda"> Responsável</td>
                            <td class="subtituloesquerda"> Data de Inclusão</td>
                            <td class="subtituloesquerda"> Situação</td>
                            <td class="subtituloesquerda"> Data da Situação</td>
                            <td class="subtituloesquerda"> Possui Pendências</td>
                        </tr>
                        <?php
                        $chkFnde = new ChecklistFnde();
                        $lista = $chkFnde->getListaCheck('lista');
                        $lista = $chkFnde->montaColunaPendenciasChkLst($lista);
                        if (!empty($lista)) {
                            $c = 0;
                            foreach ($lista as $key => $value) {
                                $c++;
                                switch ($value['queid']) {
                                    case QUEID_QUEST_CHKLST_ADM:
                                        $tipo = 'Administrativo';
                                        $tipo_abrev = 'adm';
                                        break;
                                    case QUEID_QUEST_CHKLST_ADM_SP:
                                        $tipo = 'Administrativo Simplificado';
                                        $tipo_abrev = 'adm_sp';
                                        break;
                                    case QUEID_QUEST_CHKLST_ADM_2015:
                                        $tipo = 'Administrativo';
                                        $tipo_abrev = 'adm_2015';
                                        break;
                                    case QUEID_QUEST_CHKLST_TEC:
                                        $tipo = 'Técnico';
                                        $tipo_abrev = 'tec';
                                        break;
                                    case QUEID_QUEST_CHKLST_TEC_2015:
                                        $tipo = 'Técnico';
                                        $tipo_abrev = 'tec_2015';
                                        break;
                                    case QUEID_QUEST_CHKLST_2P:
                                        $tipo = '1° Parcela';
                                        $tipo_abrev = '2p';
                                        break;
                                    case QUEID_QUEST_CHKLST_OBR_VINC:
                                        $tipo = 'Obra Vinculada';
                                        $tipo_abrev = 'obr_vinc';
                                        break;
                                    case QUEID_QUEST_CHKLST_OBR_MI:
                                        $tipo = 'Obra MI';
                                        $tipo_abrev = 'obr_mi';
                                        break;
                                    case QUEID_QUEST_CHKLST_SOLICITACOES:
                                        $tipo = 'Checklist Analise de Solicitação';
                                        $tipo_abrev = 'solicitacoes';
                                        break;
                                    default:
                                        $tipo = 'Checklist - Modelo Antigo';
                                        $tipo_abrev = 'old';
                                        break;
                                }
                                $data = (!empty($value['htddata'])) ? $value['htddata'] : $value['ckfdatainclusao'];
                                $iconExcluir = '<img src="/imagens/excluir.gif" title="Excluir Questionário"  onclick="javascript: excluirChecklist(\'' . $tipo_abrev . '\', ' . $value['ckfid'] . ')" style="cursor: pointer;" > <!-- Excluir -->';
                                if(possui_perfil(PFLCOD_CALL_CENTER)){
                                    $iconExcluir = '';
                                }
                                echo '<tr>';
                                echo '  <td> 
                                            <img src="/imagens/alterar.gif" title="Editar Questionário"   onclick="javascript: editarChecklist(\'' . $tipo_abrev . '\', ' . $value['ckfid'] . ')" style="cursor: pointer;"  > <!-- Editar -->
                                            <img src="/imagens/send.png"    title="Duplicar Questionário" onclick="javascript: duplicarChecklist(\'' . $tipo_abrev . '\', ' . $value['ckfid'] . ')" style="cursor: pointer;"> <!-- Duplicar -->
                                            '.$iconExcluir.'
                                          </td>';
                                echo '  <td> ' . $value['ckfid'] . ' </td>';
                                echo '  <td> ' . $tipo . ' </td>';
                                echo '  <td> ' . $value['usunome'] . ' </td>';
                                echo '  <td> ' . $value['ckfdatainclusao'] . ' </td>';
                                echo '  <td> ' . $value['esddsc'] . ' </td>';
                                echo '  <td> ' . $data . ' </td>';
                                echo '  <td> ' . $value['pendencias'] . ' </td>';
                                echo '</tr>';
                            }
                            echo '<tr> <td colspan="8" style="color: black; text-align: left; border-top: #000 2px solid"> <strong> Total de Registros: ' . $c . ' </strong> </td> </tr>';
                        } else {
                            echo '<tr> <td colspan="8" style="color: red; text-align: center;"> Não foram encontrados Registros. </td> </tr>';
                        }
                        ?>
                    </table>
                </div>
            </td>
        </tr>
    </table>
    <br>

<?php

$cabecalho = array(
    "Data Inclusão",
    "Nome Arquivo",
    "Tamanho (Mb)",
    "Descrição Arquivo",
    "Responsável");
$arquivoLicitacao = new ArquivoLicitacao();
$sql = $arquivoLicitacao->listaHomologadoSql(array('obrid' => $obrid));

$obrasContrato = new ObrasContrato();
$arqs = $obrasContrato->listaArqidOsPorObra($obrid);

$arqsHomologados = $db->carregar($sql);
$arqsHomologados = ($arqsHomologados ? $arqsHomologados : array());

if (isset($arqs[0])) {
    $arqsHomologados[] = $arqs[0];
}

echo '<table class="listagem" width="95%" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtituloesquerda" style="text-align: center">Anexos da 2ª parcela</td>
        </tr>
      </table>
     ';
$db->monta_lista($arqsHomologados, $cabecalho, 50, 10, 'N', '', '');

/*
 * Regra alterada para habilitar a validação atraves do arquivo da ordem de serviço
 * $habilitaHo = (!empty($arrFases['tflid']) && $habilitado) ? '': 'disabled="disabled"';
 */

$obra = new Obras();
$obra->carregarPorIdCache($obrid);

// Para Obras MI a regra para liberar é a empresa ter aceitado a OS
if ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) {
    $docid = pegaDocidObra($obrid);
    $acaoAceite = wf_acaoFoiExecutada($docid, ESDID_OBJ_AGUARDANDO_ACEITE_OS, ESDID_OBJ_EXECUCAO);
    $arquivoOS = ($acaoAceite) ? true : false;
} else {
    $arquivoOS = $db->pegaUm("SELECT DISTINCT arqidos FROM obras2.obrascontrato WHERE ocrstatus = 'A' AND obrid = $obrid AND arqidos IS NOT NULL");
}

// Calcula o Percentual de Validacao da Parcela com ajustes dos casos com obras vinculadas
$percentual_de_comparacao = $obra->calculaPercentualValidacaoParcela();
$habilitaHo = (!empty($arquivoOS) && $habilitado) ? '' : 'disabled="disabled"';
$habilita25 = ($percentual_de_comparacao >= '25' && $habilitado) ? '' : 'disabled="disabled"';
$habilita50 = ($percentual_de_comparacao >= '50' && $habilitado) ? '' : 'disabled="disabled"';

$label25 = 'Execução Física 25%';
$label50 = 'Execução Física 50%';

if ($obra->tooid == 2) {
    $habilita25 = '';
    $habilita50 = '';
}

// Quando MI, libera sem checar o percentul
if (obraMi($obrid)) {
    $habilitaHo = '';
    $habilita25 = '';
    $habilita50 = '';

    $label25 = 'Execução Física 3º Parcela';
    $label50 = 'Execução Física 4º Parcela ';
}

$docid = pegaDocidObra($obrid);
$doc = wf_pegarDocumento($docid);
if ($doc['esdid'] == ESDID_OBJ_INACABADA) {
    $habilitado = false;
    $habilita = 'N';
    $habilitaHo = 'disabled="disabled"';
    $habilita25 = 'disabled="disabled"';
    $habilita50 = 'disabled="disabled"';
}

?>
    <form name="formulario" id="formulario" method="post">
    <input type="hidden" id="requisicao" name="requisicao" value="">
    <input type="hidden" id="obrpercentultvistoria" name="obrpercentultvistoria"
           value="<?= $arrFases['obrpercentultvistoria']; ?>">
    <input type="hidden" id="vldid" name="vldid" value="<?= $arrFases['vldid']; ?>">

    <table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

        <?php if ($arrFases['vldstatushomologacao'] == 'S') { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Ordem de serviço inserida em:</td>
                <td><?= $arrFases['vlddtinclusaosthomo1']; ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usuhomo']; ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="20%">Observação:</td>
                <td><?= $arrFases['vldobshomologacao']; ?></td>
            </tr>
            <tr>
                <td><input type="hidden" name="vldstatushomologacao" id="vldstatushomologacao"
                           value="<?= $arrFases['vldstatushomologacao']; ?>">
                    <input type="hidden" name="vlddtinclusaosthomo" id="vlddtinclusaosthomo"
                           value="<?= $arrFases['vlddtinclusaosthomo']; ?>">
                    <input type="hidden" name="usucpf_homo" id="usucpf_homo" value="<?= $arrFases['usucpf_homo']; ?>">
                </td>
                <td><input type="button" name="btnCancelaHmo" id="btnCancelaHmo" class="Cancelar" value="Cancelar"></td>
            </tr>
        <?php } else { ?>
            <tr>
                <td class="subtitulodireita" width="20%" valign="top">Documento de Ordem de Serviço Inserido?</td>
                <td>
                    <?php if ($arrFases['vldstatushomologacao'] == 'N'): ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobshomologacao').parent().show();
                            });
                        </script>
                    <?php else: ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobshomologacao').val('').parent().hide();
                            });
                        </script>
                    <?php endif; ?>
                    <input type="radio" name="vldstatushomologacao" id="vldstatushomologacao_s" <?= $habilitaHo; ?>
                           value="S" <?php echo $arrFases['vldstatushomologacao'] == 'S' ? 'checked' : '' ?>>Sim
                    <input type="radio" name="vldstatushomologacao" id="vldstatushomologacao_n" <?= $habilitaHo; ?>
                           value="N" <?php echo $arrFases['vldstatushomologacao'] == 'N' ? 'checked' : '' ?>>Não
                    <?php
                    $var = 'vldobshomologacao';
                    $obrig = 'N';
                    $habil = 'S';
                    $label = '';
                    $cols = 80;
                    $rows = 5;
                    $max = 5000;
                    $funcao = '';
                    $acao = 0;
                    $txtdica = '';
                    $tab = false;
                    $title = NULL;
                    $value = $arrFases['vldobshomologacao'];

                    echo campo_textarea($var, $obrig, $habil, $label, $cols, $rows, $max, $funcao, $acao, $txtdica, $tab, $title, $value);
                    ?>
                </td>
            </tr>
        <?php } ?>
        <tr>
            <td>&nbsp;</td>
        </tr>
    </table>
    <table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtituloesquerda" colspan="2"><?= $label25 ?></td>
        </tr>
        <?php if ($arrFases['vldstatus25exec'] == 'S') { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Valor do percentual executado validado em:</td>
                <td><?= $arrFases['vlddtinclusaost25exec1']; ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usu25']; ?></td>
            </tr>
            <tr>
                <td><input type="hidden" name="vldstatus25exec" id="vldstatus25exec"
                           value="<?= $arrFases['vldstatus25exec']; ?>">
                    <input type="hidden" name="vlddtinclusaost25exec" id="vlddtinclusaost25exec"
                           value="<?= $arrFases['vlddtinclusaost25exec']; ?>">
                    <input type="hidden" name="usucpf_25" id="usucpf_25" value="<?= $arrFases['usucpf_25']; ?>"></td>
                <td><input type="button" name="btnCancela25" id="btnCancela25" class="Cancelar" value="Cancelar"></td>
            </tr>
        <?php } else { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Validar Percentual Executado:</td>
                <td>
                    <input type="radio" name="vldstatus25exec" id="vldstatus25exec_s" <?php echo $habilita25; ?>
                           value="S" <?php echo $arrFases['vldstatus25exec'] == 'S' ? 'checked' : '' ?>>Sim
                    <input type="radio" name="vldstatus25exec" id="vldstatus25exec_n" <?php echo $habilita25; ?>
                           value="N" <?php echo $arrFases['vldstatus25exec'] == 'N' ? 'checked' : '' ?>>Não
                    <?php if ($arrFases['vldstatus25exec'] == 'N'): ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobs25exec').parent().show();
                                jQuery('#responsavel_vldobs25exec').parent().show();
                            });
                        </script>
                    <?php else: ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobs25exec').val('').parent().hide();
                                jQuery('#responsavel_vldobs25exec').val('').parent().hide();
                            });
                        </script>
                    <?php endif; ?>
                    <?php
                    $var = 'vldobs25exec';
                    $obrig = 'N';
                    $habil = 'S';
                    $label = '';
                    $cols = 80;
                    $rows = 5;
                    $max = 5000;
                    $funcao = '';
                    $acao = 0;
                    $txtdica = '';
                    $tab = false;
                    $title = NULL;
                    $value = $arrFases['vldobs25exec'];

                    echo campo_textarea($var, $obrig, $habil, $label, $cols, $rows, $max, $funcao, $acao, $txtdica, $tab, $title, $value);
                    ?>
                </td>
            </tr>
            <tr>
                <td id="responsavel_vldobs25exec" class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usu25']; ?></td>
            </tr>
        <?php } ?>
        <tr>
            <td>&nbsp;</td>
        </tr>
    </table>
    <table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtituloesquerda" colspan="2"><?= $label50 ?></td>
        </tr>
        <?php if ($arrFases['vldstatus50exec'] == 'S') { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Homologação Validade em:</td>
                <td><?= $arrFases['vlddtinclusaost50exec1']; ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usu50']; ?></td>
            </tr>
            <tr>
                <td><input type="hidden" name="vldstatus50exec" id="vldstatus50exec"
                           value="<?= $arrFases['vldstatus50exec']; ?>">
                    <input type="hidden" name="vlddtinclusaost50exec" id="vlddtinclusaost50exec"
                           value="<?= $arrFases['vlddtinclusaost50exec']; ?>">
                    <input type="hidden" name="usucpf_50" id="usucpf_50" value="<?= $arrFases['usucpf_50']; ?>"></td>
                <td><input type="button" name="btnCancela50" id="btnCancela50" class="Cancelar" value="Cancelar"></td>
            </tr>
        <?php } else { ?>
            <tr>
                <td class="subtitulodireita" width="20%">Validar Percentual Executado:</td>
                <td>
                    <input type="radio" name="vldstatus50exec" id="vldstatus50exec_s" <?php echo $habilita50; ?>
                           value="S" <?php echo $arrFases['vldstatus50exec'] == 'S' ? 'checked' : '' ?>>Sim
                    <input type="radio" name="vldstatus50exec" id="vldstatus50exec_n" <?php echo $habilita50; ?>
                           value="N" <?php echo $arrFases['vldstatus50exec'] == 'N' ? 'checked' : '' ?>>Não
                    <?php if ($arrFases['vldstatus50exec'] == 'N'): ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobs50exec').parent().show();
                                jQuery('#responsavel_vldobs50exec').parent().show();
                            });
                        </script>
                    <?php else: ?>
                        <script>
                            jQuery(function () {
                                jQuery('#vldobs50exec').val('').parent().hide();
                                jQuery('#responsavel_vldobs50exec').val('').parent().hide();
                            });
                        </script>
                    <?php endif; ?>
                    <?php
                    $var = 'vldobs50exec';
                    $obrig = 'N';
                    $habil = 'S';
                    $label = '';
                    $cols = 80;
                    $rows = 5;
                    $max = 5000;
                    $funcao = '';
                    $acao = 0;
                    $txtdica = '';
                    $tab = false;
                    $title = NULL;
                    $value = $arrFases['vldobs50exec'];

                    echo campo_textarea($var, $obrig, $habil, $label, $cols, $rows, $max, $funcao, $acao, $txtdica, $tab, $title, $value);
                    ?>
                </td>
            </tr>
            <tr>
                <td id="responsavel_vldobs50exec" class="subtitulodireita" width="20%">Responsável:</td>
                <td><?= $arrFases['usu50']; ?></td>
            </tr>
        <?php } ?>
        <tr bgcolor="#C0C0C0">
            <td class="subtitulocentro" colspan="2">
                <?php if(empty($semPermissao)){
                        if (empty($habilitaHo) || empty($habilita25) || empty($habilita50)) { ?>
                            <input type="button" value="Salvar" onclick="salvarValidacao();">
                <?php   } 
                      }else { ?>
                        <input type="button" value="Salvar" disabled="disabled">
                <?php } ?>
            </td>
        </tr>
    </table>
    </form>


<?php 

    require APPRAIZ . 'obras2/includes/principal/cadValidacao/js.php';

    $objObras = new Obras();
    $objObras->carregarPorIdCache($obrid);
    $blockEdicao = $objObras->verificaObraVinculada();
    if ($blockEdicao) {
        echo '<script type="text/javascript">';
        echo " setTimeout(bloqueiaForm('formulario'), 500);
                       function bloqueiaForm(idForm){
                          jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                          jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                       }
                     ";
        echo '</script>';
}


?>
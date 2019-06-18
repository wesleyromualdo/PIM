<?php

verificaSessao( 'obra' );
require APPRAIZ . 'obras2/includes/principal/monitoramentoEspecial/ctrl.php';

?>

<table align="center" bgcolor="#f5f5f5" border="0" class="Tabela" cellpadding="3" cellspacing="1">
        <tbody>
        <? if ($atividades):?>
            <? foreach ($atividades as $atividade): ?>

            <tr>
                <td class="SubTituloCentro" colspan="3">
                    Atividade Nº <?= $atividade['mesid'] ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">
                    Título
                </td>
                <td width="80%">
                    <img
                        align="absmiddle"
                        src="/imagens/alterar.gif"
                        style="cursor: pointer"
                        onclick="javascript: abreAtividade('<?= $atividade['mesid'] ?>', '<?= $obrid ?>');"
                        title="Abrir Solicitação"> <?= $atividade['mestitulo'] ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="20%">
                    Situação
                </td>
                <td width="80%">
                    <b style=""><?= $atividade['esddscsituacao'] ?></b>
                </td>
            </tr>


            <tr>
                <td class="SubTituloDireita" width="20%">
                    Responsável
                </td>
                <td width="80%">
                    <?= $atividade['esddscresponsavel'] ?>
                </td>

            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">
                    Data Cadastro
                </td>
                <td width="80%">
                    <?= $atividade['mesdtcadastro'] ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">
                    Data de Limite
                </td>
                <td width="80%">
                    <?= $atividade['mesdtlimite'] ?>
                </td>
            </tr>


            <? endforeach; ?>

        <? else: ?>
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    Nenhuma atividade de monitoramento cadastrada.
                </td>
            </tr>
        <? endif; ?>
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <button id="" onclick="javascript: novaAtividade(<?=$obrid?>);"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Atividade">&nbsp;&nbsp;Inserir Atividade</button>
            </td>
        </tr>

    </tbody>
</table>

<?php 
    require APPRAIZ . 'obras2/includes/principal/monitoramentoEspecial/js.php';
?>

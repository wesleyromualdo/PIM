<?php
if (!$bloqueiaSave) {
    ?>
    <form method="post" name="formulario_escolas_excecao" id="formulario_escolas_excecao" class="form form-horizontal">
        <input type="hidden" value="salvar_escolas_excecao" id="salvar_escolas_excecao" name="requisicao"/>
        <input type="hidden" value="<?php echo $arrayParams['inuid']; ?>" id="inuid">
        <input type="hidden" value="<?php echo $arrayParams['adpid']; ?>" id="adpid" name="adpid">
        <input type="hidden" value="<?php echo $dadosEscolasExcecao['eepid']; ?>" id="eepid" name="eepid">
        <center>
            <h3>Escolas do grupo 1</h3>
        </center>
        <?php

        $dados = $listaEscolasG1['arrayEscolas'];

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($cabecalho1);
        $listagem->setDados($dados);
        $listagem->turnOffForm();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        ?>

        <center>
            <h3>Escolas do grupo 2</h3>
        </center>
        <?php

        $dados = $listaEscolasG2['arrayEscolas'];

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($cabecalho2);
        $listagem->setDados($dados);
        $listagem->turnOffForm();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        ?>

        <table id="tabelaJustificativa" cellspacing="2" cellpadding="2" border="0" align="center" width="95%" height="50px;">
            <thead>
            <tr align="center" style=" background: #ccc; color: #000;">
                <td colspan="6" style="font-weight: bold; font-size: 15px;">Justificativa</td>
            </tr>
            </thead>

            <tbody id="tbodyTabela">
            <tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245); height: 40px;" id="linha_1" align="center">
                <td width="5%" style="height: 100px;">&nbsp;</td>
                <td width="95%">
                    <?
                    echo $simec->textarea('justificativa', "Justificativa", $dadosEscolasExcecao['eepjustificativa'], array('cols' => 96, 'rols' => 5));
                    ?>
                </td>
            </tr>

            <tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245); height: 40px;" id="linha_1" align="center">
                <td width="100%" colspan="2" style="height: 100px;">
                    <input type="button" onclick="validaEnvio()" class="btn btn-success salvar" value="Enviar">
                </td>
            </tr>
            </tbody>

        </table>
    </form>
    <?php
}
?>
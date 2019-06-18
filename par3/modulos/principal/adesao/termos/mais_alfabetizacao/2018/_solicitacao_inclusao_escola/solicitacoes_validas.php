<?php
$_REQUEST['validado'] = "IN ('S', 'N')";
$arrRegistros = $objExcecao->retornaListaSolicitacoes($_REQUEST);



if (count($arrRegistros) > 0) {

    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $cabecalho = array("ID da solicitação", "CPF e Nome do Solicitante", "Data", "justificativa", "Validação", "Escolas Selecionadas");
    $listagem->setCabecalho($cabecalho);
    $listagem->setDados($arrRegistros);
    $listagem->turnOffForm();
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS, "*");

    $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
} else {
    ?>
    <div style="" class="text-center " id="tb_render"><b>Nenhum registro encontrado</b></div>
    <?php
}
?>
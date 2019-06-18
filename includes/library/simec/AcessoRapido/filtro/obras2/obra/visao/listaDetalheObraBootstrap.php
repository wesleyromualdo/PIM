<?php
$listagem = new Simec_Listagem();
$listagem->setCabecalho($dados['cabecalho']);
$listagem->setQuery($dados['sqlListaDetalheObra']);
$listagem->addCallbackDeCampo('situacao', 'AcessoRapido\filtro\obras2\obra\controle\Obra::tratarSituacaoObra');
$listagem->addCallbackDeCampo('dtvistoria', 'AcessoRapido\filtro\obras2\obra\controle\Obra::tratarDataUltimaVistoria');
$listagem->setId('listaDetalheDaObra');
$listagem->esconderColunas($dados['campoOculto']);
$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);


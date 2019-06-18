<?php
/**
 * Aqruivo de processamento da carga de financeiros.
 */
$enderecoCarga = 'spoemendas.php?modulo=principal/cargas/segov/carga&acao=A';

$fm = new Simec_Helper_FlashMessage('spoemendas/carga/segov');
$modelSolicitacaoPedido = new Spoemendas_Model_Solicitacaofinanceirapedido();
$modelCargaSegov = new Spoemendas_Model_Cargasegov();

if (!isset($_GET['carga']))
{
    header('Location: spoemendas.php?modulo=principal/cargas/segov/carga&acao=A');
    die;
}

if (isset($_REQUEST['processar']) && $_REQUEST['processar'])
{

    // busca todos os registros da carga pendentes de processamento = aguardando = A
    if ($dadosCarga = $modelCargaSegov->getAll($_GET['carga']))
    {
        $periodo = explode('/', $dadosCarga[0]['casmesdereferencia']);
        $diminuir = false;

        if ($periodo[0] != 12) {
            $diminuir = true;
            $periodo[0]++;
            $prsidNovo = (new Spoemendas_Model_Periodosolicitacao())->retornaPeriodo($periodo[0], $periodo[1]);
        }

        if (!$prsidNovo && $periodo[0] != 12 && $periodo[1] != $_SESSION['exercicio']) {
            $fm->addMensagem("Não existe próximo período cadastrado. Crie um novo período antes de fazer a importação!", Simec_Helper_FlashMessage::ERRO);
            header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/segov/carga&acao=A");
            die;
        }
        $processados = 0;
        $naoProcessados = 0;
        foreach ($dadosCarga as $carga)
        {
            $carga = (object) $carga;
            // busca a linha do pedido referente a linha da carga
            $pedido = $modelSolicitacaoPedido->getPedidoCarga($carga);
            if (!empty($pedido))
            {
                $pedido = (object) $pedido;
                if ($modelSolicitacaoPedido->verificaPeriodoReferenciaNotaEmpenho($carga, $pedido))
                {
                    if ($modelSolicitacaoPedido->atualizaPedido($pedido->sfpid, $carga))
                    {
                        ++$processados;
                    }
                    else
                    {
                        ++$naoProcessados;
                    }
                }else{
                    ++$naoProcessados;
                }
            }else{
                ++$naoProcessados;
            }
        }

        if ($diminuir) {
            $periodo[0]--;
        }

        if ($prsidNovo && $periodo[0] != 12) {
            $modelPeriodoSolicitacao = (new Spoemendas_Model_Periodosolicitacao());

            if (!($prsidAtual = $modelPeriodoSolicitacao->retornaPeriodo($periodo[0], $periodo[1]))) {
                $fm->addMensagem(
                    'Não há uma período de solicitação aberto para executar a carga de retorno da SEGOV.',
                    Simec_Helper_FlashMessage::ERRO
                );
                header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/segov/carga&acao=A");
                die;
            }

            $modelSolicitacaoPedido->criarNovaLinhaNaoSolicitado($prsidAtual, $prsidNovo);
        }
    }

    $fm->addMensagem("Processamento concluido com sucesso, total de {$processados} linha(s) processadas e {$naoProcessados} linha(s) já carregadas!");
    header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/segov/carga&acao=A");
    die;
}

include APPRAIZ . 'includes/cabecalho.inc';
?>
<style type="text/css">
    .processar-carga {
        margin-bottom: 10px
    }
</style>
<script type="text/javascript">
    $(function () {
        $('.processar-carga').click(function () {
            bootbox.confirm('Tem certeza que quer processar essa carga? Ela acarretará modificações na base de dados.', function(ret) {
                if(ret) {
                    window.location.assign(window.location.href + '&processar=1');
                } else {
                    bootbox.hideAll();
                }
            });
        });
    });
</script>
<div class="col-lg-12">
    <?php
    (new Simec_View_Breadcrumb())
        ->add('Carga SEGOV', $enderecoCarga)
        ->add('Processamento SEGOV')
        ->render();
    echo $fm;
    ?>
    <button class="btn btn-success processar-carga"><span class="glyphicon glyphicon-cog"></span> Iniciar processamento</button>
    <?php
    $colunas = [
        'Mês Referência',
        'Código U.O.',
        'Descrição U.O.',
        'Valor Solicitado',
        'Valor Autorizado'
    ];
    (new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO))
        ->setQuery($modelCargaSegov->getAll($_GET['carga'], true))
        ->setCabecalho($colunas)
        ->addCallbackDeCampo([
            'ctgdespempenhadas',
            'ctgdespliquidas',
            'ctgdespliquidadaspagar',
            'ctgdesppagas',
            'ctgrapprocesspagar',
            'ctgrapnprocessliquipagar',
            'ctgrapprocnproc',
            'ctgpagprocnproc',
            'ctgrappagprocnproc',
            'ctgvalliquidopagar',
            'casvlrsolicitado',
            'casvlrautorizado',
        ], 'mascaraMoeda')
        ->turnOnPesquisator()
        ->esconderColunas(['casid', 'emecod', 'casne', 'emeano', 'casdtaautorizacao', 'ptres'])
        ->render();
    ?>
</div>

<?php
/**
 * Aqruivo de processamento da carga de financeiros.
 *
 * @version $Id: processamento.inc 112388 2016-07-22 17:50:38Z werteralmeida $
 */
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/financeiro');
$enderecoCarga = "spoemendas.php?modulo=principal/cargas/financeiro/carga&acao=A";

if (isset($_REQUEST['processar']) && $_REQUEST['processar']) {
    $qtdAtualizacoes = 0;
    $qtdInsercoes = 0;
    $sql = <<<DML
SELECT emeano,
       emecod,
       vlrempenhocancelado,
       vlrsaldoapagar,
       vlrpago
  FROM spoemendas.cargavalores
  WHERE vlrpago <> 0 OR vlrsaldoapagar <> 0 OR vlrempenhocancelado <> 0
DML;

    $update = <<<DML
UPDATE spo.siopexecucao
  SET vlrpago = %f,
      vlrsaldoapagar = %f,
      vlrempenhocancelado = %f,
      dadoscargamanual = true
  WHERE exercicio = '%d'
    AND emecod = '%s'
  RETURNING emecod
DML;
    foreach (is_array($dados = $db->carregar($sql))?$dados:[] as $linha) {
        $stmt = sprintf($update, $linha['vlrpago'], $linha['vlrsaldoapagar'],$linha['vlrempenhocancelado'], $linha['emeano'], $linha['emecod']);
        $emecod = $db->pegaUm($stmt);
        if ($emecod) {
            $qtdAtualizacoes++;
        } else {
            $insert = <<<DML
            INSERT INTO spo.siopexecucao
              (vlrpago, vlrsaldoapagar,vlrempenhocancelado, exercicio, emecod, dadoscargamanual)
            VALUES(%f, %f, %f, '%d', '%s', true )      
DML;
            $stmt = sprintf($insert, $linha['vlrpago'], $linha['vlrsaldoapagar'],$linha['vlrempenhocancelado'], $linha['emeano'], $linha['emecod']);
            $db->executar($stmt);
            $qtdInsercoes++;
        }
    }
    $db->commit();

    $fm->addMensagem("Foram atualizados {$qtdAtualizacoes} registros e inseridos {$qtdInsercoes} registros.");
    header("Location: {$enderecoCarga}");
    die();
}

include APPRAIZ . 'includes/cabecalho.inc';
?>
<style type="text/css">
.processar-carga{margin-bottom:10px}
</style>
<script type="text/javascript">
$(function(){
    $('.processar-carga').click(function(){
        bootbox.confirm('Tem certeza que quer processar essa carga? Ela acarretará modificações na base de dados.', function(){
            window.location.assign(window.location.href + '&processar=1');
        });
    });
});
</script>
<div class="col-lg-12">
<?php
(new Simec_View_Breadcrumb())
    ->add('Rotinas de carga')
    ->add('Financeiro', $enderecoCarga)
    ->add('Processamento')
    ->render();

echo $fm;
?>
    <button class="btn btn-success processar-carga"><span class="glyphicon glyphicon-cog"> Iniciar processamento...</span></button>
<?php
(new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO))
    ->setQuery(
        (new Spoemendas_Model_Cargavalores)
            ->recuperarTodos('emeano, emecod, vlrempenhocancelado, vlrsaldoapagar,vlrpago', [], null, ['query' => true])
    )->setCabecalho(['Exercício', 'Cód. Emenda', 'Cancelado (R$)', 'Saldo a pagar (R$)','Pago (R$)'])
    ->addCallbackDeCampo(['vlrpago', 'vlrsaldoapagar', 'vlrempenhocancelado'], 'mascaraMoeda')
    ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['vlrpago', 'vlrsaldoapagar', 'vlrempenhocancelado'])
    ->turnOnPesquisator()
    ->render();
?>
</div>
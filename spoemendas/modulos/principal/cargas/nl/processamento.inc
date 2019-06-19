<?php
/**
 * Aqruivo de processamento da carga de financeiros.
 *
 * @version $Id: processamento.inc 132159 2017-10-19 13:17:52Z fernandoaires $
 */
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/nl');
$enderecoCarga = "spoemendas.php?modulo=principal/cargas/nl/carga&acao=A";

if (isset($_REQUEST['processar']) && $_REQUEST['processar']) {
    $qtdAtualizacoes = 0;
    $qtdInsercoes = 0;
    $sql = <<<DML
SELECT emeano,
       emecod,
       nlcod,
       MAX(nldata) AS nldata,
       SUM(nlvalor) AS nlvalor
  FROM spoemendas.cargavalores
  GROUP BY
	emeano, emecod, nlcod
DML;

    foreach (is_array($dados = $db->carregar($sql)) ? $dados : [] as $linha) {
        $update = <<<DML
        UPDATE spoemendas.emendanl
          SET nldata = '%s',
              nlvalor = %f
          WHERE exercicio = '%d'
            AND emecod = '%s'
            AND nl = '%s'
          RETURNING emecod
DML;

        $stmt = sprintf($update, $linha['nldata'], $linha['nlvalor'], $linha['emeano'], $linha['emecod'], $linha['nlcod']);
        $emecod = $db->pegaUm($stmt);
        if ($emecod) {
            $qtdAtualizacoes++;
        } else {
            $insert = <<<DML
            INSERT INTO spoemendas.emendanl
              (nldata, nlvalor,exercicio,emecod, nl)
            VALUES('%s', %f, '%d', '%s', '%s' )      
DML;
            $stmt = sprintf($insert, $linha['nldata'], $linha['nlvalor'], $linha['emeano'], $linha['emecod'], $linha['nlcod']);
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
    $(function () {
        $('.processar-carga').click(function () {
            bootbox.confirm('Tem certeza que quer processar essa carga? Ela acarretará modificações na base de dados.', function (result) {
                if(result){
                    window.location.assign(window.location.href + '&processar=1')
                }else{
                    window.location.assign('spoemendas.php?modulo=principal/cargas/nl/carga&acao=A');
                }
            });
        });
    });
</script>
<div class="col-lg-12">
    <?php
    (new Simec_View_Breadcrumb())
            ->add('Rotinas de carga')
            ->add('NL', $enderecoCarga)
            ->add('Processamento')
            ->render();

    echo $fm;
    ?>
    <button class="btn btn-success processar-carga"><span class="glyphicon glyphicon-cog"> Iniciar processamento...</span></button>
    <?php
    (new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO))
            ->setQuery(
                    (new Spoemendas_Model_Cargavalores)
                    ->recuperarTodos("emeano, emecod, nlcod, TO_CHAR(nldata, 'dd/mm/yyyy') AS nldata, nlvalor", [], null, ['query' => true])
            )->setCabecalho(['Exercício', 'Cód. Emenda', 'Cód. NL', 'Data', 'Pago (R$)'])
            ->addCallbackDeCampo('nlvalor', 'mascaraMoeda')
            ->turnOnPesquisator()
            ->render();
    ?>
</div>
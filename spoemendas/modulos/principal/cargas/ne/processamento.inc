<?php
/**
 * Aqruivo de processamento da carga de financeiros.
 *
 * @version $Id: processamento.inc 132160 2017-10-19 13:27:46Z fernandoaires $
 */
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/ne');
$enderecoCarga = "spoemendas.php?modulo=principal/cargas/ne/carga&acao=A";

if (isset($_REQUEST['processar']) && $_REQUEST['processar']) {
    $qtdAtualizacoes = 0;
    $qtdInsercoes = 0;
    $sql = <<<DML
SELECT emeano,
       emecod,
       necod,
       MAX(nedata) AS nedata,
       SUM(nevalor) AS nevalor
  FROM spoemendas.cargavalores
  GROUP BY
	emeano, emecod, necod
DML;

    foreach (is_array($dados = $db->carregar($sql)) ? $dados : [] as $linha) {
        $update = <<<DML
        UPDATE spoemendas.emendane
          SET nedata = '%s',
              nevalor = %f
          WHERE exercicio = '%d'
            AND emecod = '%s'
            AND ne = '%s'
          RETURNING emecod
DML;
        $stmt = sprintf($update, $linha['nedata'], $linha['nevalor'], $linha['emeano'], $linha['emecod'], $linha['necod']);
        $emecod = $db->pegaUm($stmt);
        if ($emecod) {
            $qtdAtualizacoes++;
        } else {
            $insert = <<<DML
            INSERT INTO spoemendas.emendane
              (nedata, nevalor,exercicio,emecod, ne)
            VALUES('%s', %f, '%d', '%s', '%s' )      
DML;
            $stmt = sprintf($insert, $linha['nedata'], $linha['nevalor'], $linha['emeano'], $linha['emecod'], $linha['necod']);
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
        bootbox.confirm('Tem certeza que quer processar essa carga? Ela acarretará modificações na base de dados.', function(result){
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
    ->add('NE', $enderecoCarga)
    ->add('Processamento')
    ->render();

echo $fm;
?>
    <button class="btn btn-success processar-carga"><span class="glyphicon glyphicon-cog"> Iniciar processamento...</span></button>
<?php
(new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO))
    ->setQuery(
        (new Spoemendas_Model_Cargavalores)
            ->recuperarTodos("emeano, emecod, necod, TO_CHAR(nedata, 'DD/MM/YYYY') AS nedata, nevalor", [], null, ['query' => true])
    )->setCabecalho(['Exercício', 'Cód. Emenda', 'Cód. NE', 'Data', 'Pago (R$)'])
    ->addCallbackDeCampo('nevalor', 'mascaraMoeda')
    ->turnOnPesquisator()
    ->render();
?>
</div>
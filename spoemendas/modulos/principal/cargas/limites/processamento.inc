<?php
/**
 * Aqruivo de processamento da carga de financeiros.
 *
 * @version $Id: processamento.inc 110722 2016-05-04 14:27:25Z werteralmeida $
 */
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/limites');
$enderecoCarga = "spoemendas.php?modulo=principal/cargas/limites/carga&acao=A";

if (isset($_REQUEST['processar']) && $_REQUEST['processar']) {
    $qtdAtualizacoes = 0;
    $qtdInsercoes = 0;
    $sql = "SELECT  emeano,
				    emecod,
				    vlrlimiteempenho,
				    vlrempenho,
				    vlrempenhocancelado,
				    datareferencia,
				    ptres        
			FROM    spoemendas.cargavalores";

    $dados = $db->carregar($sql);
    
    foreach (is_array($dados) ? $dados : [] as $linha) {
	    $colunasUpdate = array();
        
	    if(!is_null($linha['vlrlimiteempenho'])){
	    	$colunasUpdate[] = " vlrlimiteempenho = {$linha['vlrlimiteempenho']} ";
	    }
	    
	    if(!is_null($linha['vlrempenho'])){
	    	$colunasUpdate[] = " vlrempenhado = {$linha['vlrempenho']} ";
	    }
	    
	    if(!is_null($linha['vlrempenhocancelado'])){
	    	$colunasUpdate[] = " vlrempenhocancelado = {$linha['vlrempenhocancelado']} ";
	    }
	    	
        $colunasUpdateStr = implode(',', $colunasUpdate);

        $update = "UPDATE 	spo.siopexecucao
          		   SET 		dtatualizacao = '{$linha['datareferencia']}',
              				{$colunasUpdateStr}
          		   WHERE 	exercicio = '{$linha['emeano']}'
            				AND emecod = '{$linha['emecod']}'
          		   RETURNING emecod";
              				
            $emecod = $db->pegaUm($update);
            
            if ($emecod) {
                $qtdAtualizacoes++;
            } else {

                $colunasInsertArr = array();
                if(!is_null($linha['vlrlimiteempenho'])){
                	$colunasInsertArr[] = " vlrlimiteempenho ";
                }
                	
                if(!is_null($linha['vlrempenho'])){
               		$colunasInsertArr[] = " vlrempenhado ";
                }
                
                if(!is_null($linha['vlrempenhocancelado'])){
                	$colunasInsertArr[] = " vlrempenhocancelado ";
                }
                	
                $valoresInsertArr = array();
                if(!is_null($linha['vlrlimiteempenho'])){
               		$valoresInsertArr[] = " {$linha['vlrlimiteempenho']}  ";
                }
                
                if(!is_null($linha['vlrempenho'])){
               		$valoresInsertArr[] = " {$linha['vlrempenho']} ";
               	}
               	
               	if(!is_null($linha['vlrempenhocancelado'])){
               		$valoresInsertArr[] = " {$linha['vlrempenhocancelado']}  ";
               	}
               	
                $colunasInsert = implode(',', $colunasInsertArr);
                $valoresInsert = implode(',', $valoresInsertArr);

                $insert = "INSERT INTO spo.siopexecucao (emecod,exercicio,dadoscargamanual,ptres,{$colunasInsert}) VALUES('{$linha['emecod']}','{$linha['emeano']}',TRUE,'{$linha['ptres']}',{$valoresInsert} )";

                $db->executar($insert);
                $qtdInsercoes++;
            }
    }
    
    $db->commit();

    $fm->addMensagem("Foram atualizados {$qtdAtualizacoes} registros e inseridos {$qtdInsercoes} registros.");
    header("Location: {$enderecoCarga}");
    die();
}

include APPRAIZ . 'includes/cabecalho.inc'; ?>
<style type="text/css">
    .processar-carga{margin-bottom:10px}
</style>
<script type="text/javascript">
    $(function () {
        $('.processar-carga').click(function () {
            bootbox.confirm('Tem certeza que quer processar essa carga? Ela acarretará modificações na base de dados.', function () {
                window.location.assign(window.location.href + '&processar=1');
            });
        });
    });
</script>
<div class="col-lg-12">
<?php
(new Simec_View_Breadcrumb())
        ->add('Rotinas de carga')
        ->add('Limites - Empenhos - Cancelamentos', $enderecoCarga)
        ->add('Processamento')
        ->render();

echo $fm; ?>
    <button class="btn btn-success processar-carga"><span class="glyphicon glyphicon-cog"> Iniciar processamento...</span></button>
    <?php
    (new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO))
            ->setQuery(
                    (new Spoemendas_Model_Cargavalores)
                    ->recuperarTodos(" 
                        emeano,
                        emecod,
                        vlrlimiteempenho,
                        vlrempenho,
                        vlrempenhocancelado,
                        TO_CHAR(datareferencia, 'dd/mm/yyyy') AS datareferencia, ptres ", [], null, ['query' => true])
            )->setCabecalho(['Exercício', 'Cód. Emenda', 'Limite (R$)', 'Empenhado (R$)', 'Cancelado (R$)', 'Data de Referência', 'PTRES'])
            ->addCallbackDeCampo(array('vlrempenho',
                'vlrempenhocancelado',
                'vlrlimiteempenho'), 'mascaraMoeda')
            ->turnOnPesquisator()
            ->render();
    ?>
</div>
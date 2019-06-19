<?php
/**
 * $Id: vinculacaoiniciativas.inc 119037 2017-02-15 18:01:40Z wesleysilva $
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script>
    /**
     * verEmendasParlamentar(){}
     */
    function verEmendasParlamentar(autid) {
        window.open('spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=filtrar&emendas[autid]=' + autid);
    }
</script>
<div class="col-md-12" >
    <div class="col-md-7">
        <?php
        $sql = <<<DML
SELECT
    eme.autid,
    aut.autnome,
    aut.autemail,
    COUNT(0)                         AS emendas,
    COUNT(doc.esdid=1686)            AS finalziado,
    CASE
        WHEN COUNT(0) = COUNT(doc.esdid=1686)
        THEN 'Finalizado'
        ELSE (COUNT(0) - COUNT(doc.esdid=1686))::text
    END AS restantes
FROM
    emenda.emenda eme
JOIN
    emenda.autor aut
USING
    (autid)
LEFT JOIN
    spoemendas.emendainformacoes ein
USING
    (emeid)
LEFT JOIN
    workflow.documento doc
USING
    (docid)
WHERE
    emeano = '{$_SESSION['exercicio']}'
    AND etoid IN(1,3)
    and aut.autnome not ilike '%bancada%'
    and aut.autnome not ilike '%com.%'
    and aut.autnome not ilike '%Comissão%'
AND emeid IN (
    (
        SELECT
            emeid
        FROM
            (
                SELECT
                    emeid,
                    SUM(ede.edevalor) vlr
                FROM
                    emenda.emenda
                JOIN
                    emenda.emendadetalhe emd
                USING
                    (emeid)
                JOIN
                    emenda.emendadetalheentidade ede
                USING
                    (emdid)
    			where emdstatus = 'A' and edestatus = 'A'
                GROUP BY
                    1) foo
        WHERE
            vlr > 0) )       
GROUP BY
    1,2,3
ORDER BY
    2
DML;
        /* Caso não venha o momento poe o SQL como falso */
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho(array(
            //    "Alterar/Excluir",
            'Parlamentar','Email', 'Total de Emendas', 'Finalizadas', 'Restantes'));
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('emendas', 'finalziado', 'restantes'));
        $listagem->addCallbackDeCampo(array('autnome'), 'alinhaParaEsquerda');
        $listagem->turnOnPesquisator();
        $listagem->addAcao('view', 'verEmendasParlamentar');
        $listagem->render();
        ?>

    </div>
    <div class="col-md-5">
        <section class="panel panel-default">
            <section class="panel-heading" style="background-color:white;"><strong>Vinculação Geral</strong></section>
            <section class="panel-body" style="background-color: #f5f5f5;">
                <?php
                $sql = "
SELECT
    'Emendas para Vinculação' as descricao,
    (COUNT(0) - COUNT(doc.esdid=1686)) AS valor
FROM
    emenda.emenda eme
JOIN
    emenda.autor aut
USING
    (autid)
LEFT JOIN
    spoemendas.emendainformacoes ein
USING
    (emeid)
LEFT JOIN
    workflow.documento doc
USING
    (docid)
WHERE
    emeano = '2016'
AND etoid IN(1,3)
AND aut.autnome NOT ilike '%bancada%'
AND aut.autnome NOT ilike '%com.%'
AND aut.autnome NOT ilike '%Comissão%'
AND emeid IN (
    (
        SELECT
            emeid
        FROM
            (
                SELECT
                    emeid,
                    SUM(ede.edevalor) vlr
                FROM
                    emenda.emenda
                JOIN
                    emenda.emendadetalhe emd
                USING
                    (emeid)
                JOIN
                    emenda.emendadetalheentidade ede
                USING
                    (emdid)
 				where emdstatus = 'A' and edestatus = 'A'
                GROUP BY
                    1) foo
        WHERE
            vlr > 0) )
GROUP BY
    1
UNION 
SELECT
    'Emendas Finalizadas' as descricao,
    COUNT(doc.esdid=1686)            AS valor
   
FROM
    emenda.emenda eme
JOIN
    emenda.autor aut
USING
    (autid)
LEFT JOIN
    spoemendas.emendainformacoes ein
USING
    (emeid)
LEFT JOIN
    workflow.documento doc
USING
    (docid)
WHERE
    emeano = '2016'
    AND etoid IN(1,3)
    and aut.autnome not ilike '%bancada%'
    and aut.autnome not ilike '%com.%'
    and aut.autnome not ilike '%Comissão%'
AND emeid IN (
    (
        SELECT
            emeid
        FROM
            (
                SELECT
                    emeid,
                    SUM(ede.edevalor) vlr
                FROM
                    emenda.emenda
                JOIN
                    emenda.emendadetalhe emd
                USING
                    (emeid)
                JOIN
                    emenda.emendadetalheentidade ede
                USING
                    (emdid)
 				where emdstatus = 'A' and edestatus = 'A'
                GROUP BY
                    1) foo
        WHERE
            vlr > 0) )       
GROUP BY
    1

    ";
#ver($sql);
                global $db;
                echo geraGrafico(
                        $db->carregar($sql), $nomeUnico = 'acoes', $titulo = '', $formatoDica = "", $formatoValores = "{point.y} ({point.percentage:.2f} %)", $nomeSerie = "", $mostrapopudetalhes = false, $caminhopopupdetalhes = "", $largurapopupdetalhes = "", $alturapopupdetalhes = "", $mostrarLegenda = true, $aLegendaConfig = false, $legendaClique = false, '#555555'
                );
                ?>
            </section>
        </section>
    </div>
</div>

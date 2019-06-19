<?php
/**
 * $Id: vinculacaoiniciativas.inc 108768 2016-03-02 14:41:50Z werteralmeida $
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script>
    /**
     * verEmendasParlamentar(){}
     */
    function verEmendasUnidade(unicod) {
        window.open('spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=filtrar&emendas[unicod]=' + unicod);
    }
</script>
<div class="col-md-12" >
    <div class="col-md-7">
        <?php
        $sql = <<<DML
SELECT
    uni.unicod,
    vf.unicod ||' - ' ||uni.unidsc AS beneficiario,
    COUNT(
        CASE
            WHEN doc.esdid = 1696
            THEN 1
        END) AS aguardando_secretaria,
    COUNT(
        CASE
            WHEN doc.esdid = 1698
            THEN 1
        END) AS aguardando_limite
FROM
    emenda.emenda eme
INNER JOIN
    emenda.v_funcionalprogramatica vf
ON
    (
        vf.acaid = eme.acaid
    AND vf.acastatus = 'A')
JOIN
    public.unidade uni
ON
    vf.unicod = uni.unicod
LEFT JOIN
    spoemendas.emendainformacoes ein
USING
    (emeid)
LEFT JOIN
    workflow.documento doc
USING
    (docid)
WHERE
    eme.emeano = '{$_SESSION['exercicio']}'
    and etoid in (3,4)
    and uni.unicod <> '26298'
GROUP BY
    1,2
ORDER BY
    1
DML;
        /* Caso não venha o momento poe o SQL como falso */
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho(array('Unidade Orçamentária (UO)','Aguardando Secretaria', 'Aguardando Liberação de Limite'));
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('aguardando_secretaria', 'aguardando_limite'));
        $listagem->addCallbackDeCampo(array('beneficiario'), 'alinhaParaEsquerda');
        $listagem->turnOnPesquisator();
        $listagem->addAcao('view', 'verEmendasUnidade');
        $listagem->render();
        ?>

    </div>
    <div class="col-md-5">
        <section class="panel panel-default">
            <section class="panel-heading" style="background-color:white;"><strong>Avaliação geral das Secretarias</strong></section>
            <section class="panel-body" style="background-color: #f5f5f5;">
                <?php
                $sql = "
SELECT
    'Aguardando Secretaria' as descricao,
    COUNT(
        CASE
            WHEN doc.esdid = 1696
            THEN 1
        END) AS valor
FROM
    emenda.emenda eme
LEFT JOIN
    spoemendas.emendainformacoes ein
USING
    (emeid)
LEFT JOIN
    workflow.documento doc
USING
    (docid)
JOIN
    emenda.v_funcionalprogramatica vf
ON
    (
        vf.acaid = eme.acaid
    AND vf.acastatus = 'A')
    
WHERE
    eme.emeano = '{$_SESSION['exercicio']}'
        and etoid in (3,4)
        and vf.unicod <> '26298'
UNION
SELECT
    'Aguardando Limite' as descricao,
    COUNT(
        CASE
            WHEN doc.esdid = 1698
            THEN 1
        END) AS valor
FROM
    emenda.emenda eme
LEFT JOIN
    spoemendas.emendainformacoes ein
USING
    (emeid)
LEFT JOIN
    workflow.documento doc
USING
    (docid)
JOIN
    emenda.v_funcionalprogramatica vf
ON
    (
        vf.acaid = eme.acaid
    AND vf.acastatus = 'A')
    
WHERE
    eme.emeano = '{$_SESSION['exercicio']}'
        and etoid in (3,4)
        and vf.unicod <> '26298'
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
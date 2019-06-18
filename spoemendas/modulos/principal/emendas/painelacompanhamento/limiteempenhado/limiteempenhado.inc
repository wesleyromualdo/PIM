<?php
/**
 * $Id: limiteempenhado.inc 111370 2016-05-23 18:41:25Z werteralmeida $
 */
include APPRAIZ . "includes/cabecalho.inc";
// -- Tratamento de perfis
$perfis = pegaPerfilGeral();
?>
<style>
    rigth{float:right}
    .text-center{text-align:left !important;}
</style>
<br>
<div class="col-md-12" >
    <section class="col-md-7">
        <?php
        $sql = <<<DML
SELECT
    uni.unicod,
    vf.unicod ||' - ' ||uni.unidsc           AS beneficiario,
    SUM(sex.vlrlimiteempenho)                AS limite,
    SUM(sex.vlrempenhado)                    AS empenhado,
    CEIL(
        CASE
            WHEN SUM(sex.vlrempenhado) > 0
            THEN SUM(sex.vlrempenhado) * 100 / SUM(sex.vlrlimiteempenho)
            ELSE 0
        END) AS limiteempenhado
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
    spo.siopexecucao sex
ON
    sex.emecod::text = eme.emecod::text
AND sex.exercicio::text = eme.emeano::text
AND sex.exercicio = '{$_SESSION['exercicio']}'
AND sex.vlrlimiteempenho > 0
WHERE
eme.emeano = '{$_SESSION['exercicio']}'
GROUP BY
    1,2
ORDER BY
    1
DML;
        /* Caso não venha o momento poe o SQL como falso */
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho(array(
            //    "Alterar/Excluir",
            'Unidade Orçamentária (UO) ', 'Limite de Empenho (R$)', 'Empenhado (R$)', 'Limite Empenhado'));
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('limite', 'empenhado'));
        $listagem->addCallbackDeCampo('limiteempenhado', 'callbackProgressBar');
        $listagem->addCallbackDeCampo(array('limite', 'empenhado'), 'mascaraMoeda');
        $listagem->addCallbackDeCampo(array('beneficiario'), 'alinhaParaEsquerda');
        $listagem->addAcao('view', 'verUnidade');
        ?>
        <?php if (false === $listagem->render()): ?>
            <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
                Nenhum registro encontrado
            </div>
        <?php endif; ?>
    </section>
    <section class="col-md-5">
        <section class="panel panel-primary">
            <section class="panel-heading"><strong>GRÁFICO POR LIMITE TOTAL EMPENHADO</strong></section>
            <section class="panel-body" style="background-color: royalblue;">
                <?php
                $sql = "
SELECT
    'Saldo restante' AS descricao,
    SUM(sex.vlrlimiteempenho) - SUM(sex.vlrempenhado) as valor
FROM
    emenda.emenda e
INNER JOIN
    emenda.v_funcionalprogramatica vf
ON
    (
        vf.acaid = e.acaid
    AND vf.acastatus = 'A')
JOIN
    public.unidade uni
ON
    vf.unicod = uni.unicod
LEFT JOIN
    spo.siopexecucao sex
ON
    sex.emecod::text = e.emecod::text
AND sex.exercicio::text = e.emeano::text
WHERE
    sex.exercicio = '{$_SESSION['exercicio']}'    
UNION
SELECT
    'Total empenhado' AS descricao,
    SUM(sex.vlrempenhado) as valor
FROM
    emenda.emenda e
INNER JOIN
    emenda.v_funcionalprogramatica vf
ON
    (
        vf.acaid = e.acaid
    AND vf.acastatus = 'A')
JOIN
    public.unidade uni
ON
    vf.unicod = uni.unicod
LEFT JOIN
    spo.siopexecucao sex
ON
    sex.emecod::text = e.emecod::text
AND sex.exercicio::text = e.emeano::text
WHERE
    sex.exercicio = '{$_SESSION['exercicio']}'

                    ";
                $dados = $db->carregar($sql);

                if ($dados) {
                    echo geraGrafico(
                            $dados, $nomeUnico = 'acoes', $titulo = '', $formatoDica = "", $formatoValores = "{point.y} ({point.percentage:.2f} %)", $nomeSerie = "", $mostrapopudetalhes = false, $caminhopopupdetalhes = "", $largurapopupdetalhes = "", $alturapopupdetalhes = "", $mostrarLegenda = true, $aLegendaConfig = false, $legendaClique = false, '#FFF'
                    );
                } else {
                    echo "<section class=\"alert alert-info text-center\">Não existem dados a serem apresentados</section>";
                }
                ?>
            </section>
        </section>
    </section>
</div>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
<link rel="stylesheet" href="css/progorc.css">
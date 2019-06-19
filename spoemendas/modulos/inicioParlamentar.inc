<script>
    $(document).ready(function () {
        $('body').css('background-color', '#e4e4e4');
        $('#divAmbiente').parent().css('min-height', 'calc(100% - 15px)');
    });
</script>
<link rel="stylesheet" type="text/css" href="\library\font-awesome\css\font-awesome.min.css" media="all">
<style>
    .box-white {
        background-color: white;
        padding: 15px;
        margin: 10px -10px;
        border-radius: 4px;
    }

    .clear {
        clear: both;
    }

    .tituloCaixa {
        font-size: 18px;
        padding-bottom: 10px;
        text-align: center;
        font-weight: bold;
    }

    .subTituloCaixa {
        font-size: 14px;
        border-top: 1px solid #ddd;
        margin-top: 20px;
        padding-top: 10px;
        text-align: center;
        padding-bottom: 5px;
        font-weight: bold;
    }

    .small-box {
        border-radius: 2px;
        position: relative;
        display: block;
        margin-bottom: 15px;
        box-shadow: 0 1px 1px rgba(0, 0, 0, 0.1);
    }

    .small-box > a > .inner {
        padding: 10px;
    }

    .small-box h3, .small-box p {
        z-index: 5;
    }

    .small-box h3 {
        font-size: 38px;
        font-weight: bold;
        margin: 0 0 10px 0;
        white-space: nowrap;
        padding: 0;
    }

    .small-box .icon {
        -webkit-transition: all .3s linear;
        -o-transition: all .3s linear;
        transition: all .3s linear;
        position: absolute;
        top: -10px;
        right: 10px;
        z-index: 0;
        font-size: 90px;
        color: rgba(0, 0, 0, 0.15);
    }

    .small-box > a > .small-box-footer {
        position: relative;
        text-align: center;
        padding: 3px 0;
        color: #fff;
        color: rgba(255, 255, 255, 0.8);
        display: block;
        z-index: 10;
        background: rgba(0, 0, 0, 0.1);
        text-decoration: none;
    }

    .bg-aqua {
        background-color: #00c0ef !important;
    }

    .bg-green {
        background-color: #00a65a !important;
    }

    .bg-yellow {
        background-color: #f39c12 !important;
    }

    .small-box > a, .small-box > a:hover {
        color: #fff;
        text-decoration: none;
    }
</style>
<?php
global $db;
if (in_array(PFL_PARLAMENTAR, $perfis)) {
    $where = 'WHERE true ';
    $sql = "SELECT autid FROM spoemendas.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' AND rpustatus = 'A'";
    $pfl = $db->carregar($sql);
    if (is_array($pfl)) {
        foreach ($pfl as $key => $value) {
            $usuresp[$key] = $value['autid'];
        }
    }
    if (is_array($usuresp)) {
        $usurespIn = implode(',', $usuresp);
        $whereParlamentar = " AND aut.autid IN ($usurespIn)";
        $where .= $whereParlamentar;
    } else {
        $whereParlamentar = " AND false ";
        $where .= $whereParlamentar;
        $fm->addMensagem('Seu perfil de Parlamentar não está vinculado a nenhum autor. <br/><br/> Solicite a vinculação a ASPAR do MEC (2022-7899/7896/7894)', Simec_Helper_FlashMessage::ERRO);
    }

    $where .= " AND e.emeano = '{$_SESSION['exercicio']}' ";

//	    Emendas Parlamentares
    $sql = <<<DML
            SELECT count(DISTINCT e.emeid) as emendasParlamentates
            FROM emenda.emenda e
            LEFT JOIN emenda.emendadetalhe ed ON (ed.emeid = e.emeid) AND emdstatus = 'A'
            INNER JOIN emenda.autor aut ON (aut.autid = e.autid)
            INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
            JOIN emenda.partido par ON aut.parid = par.parid
            LEFT JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
            JOIN public.unidade uni ON vf.unicod = uni.unicod
            LEFT JOIN public.secretariaunidade secuo ON uni.unicod = secuo.unicod
            LEFT JOIN public.secretaria sec ON sec.secid = secuo.secid
            LEFT JOIN spo.siopexecucao sex ON sex.emecod::text = e.emecod::text AND sex.exercicio::text = e.emeano::text
            LEFT JOIN
              (SELECT DISTINCT emeid,
                               CASE
                                   WHEN emdimpositiva = 6 THEN 'S'
                                   ELSE 'N'
                               END AS impositiva
               FROM emenda.emendadetalhe) imp ON imp.emeid = e.emeid
            LEFT JOIN spoemendas.emendainformacoes ine ON ine.emeid = e.emeid
            LEFT JOIN emenda.entidadebeneficiada enb USING(enbid)
            LEFT JOIN workflow.documento doc USING(docid)
            LEFT JOIN workflow.estadodocumento esd USING(esdid)
            LEFT JOIN emenda.v_funcionalprogramatica vfp ON vfp.acaid = e.acaid
            {$where}
DML;
    $emendasParlamentatesTotal = $db->pegaLinha($sql);

//	    Aguardando preenchimento
    $sql = <<<DML
            SELECT 	count(e.emeid) as emendasParlamentatesAguardando
            FROM emenda.emenda e
            LEFT JOIN emenda.emendadetalhe ed ON (ed.emeid = e.emeid) AND emdstatus = 'A'
            INNER JOIN emenda.autor aut ON (aut.autid = e.autid)
            INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
            JOIN emenda.partido par ON aut.parid = par.parid
            LEFT JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
            JOIN public.unidade uni ON vf.unicod = uni.unicod
            LEFT JOIN public.secretariaunidade secuo ON uni.unicod = secuo.unicod
            LEFT JOIN public.secretaria sec ON sec.secid = secuo.secid
            LEFT JOIN spo.siopexecucao sex ON sex.emecod::text = e.emecod::text AND sex.exercicio::text = e.emeano::text
            LEFT JOIN
                    (SELECT DISTINCT emeid,
                               CASE
                                   WHEN emdimpositiva = 6 THEN 'S'
                                   ELSE 'N'
                               END AS impositiva
               FROM emenda.emendadetalhe) imp ON imp.emeid = e.emeid
            LEFT JOIN spoemendas.emendainformacoes ine ON ine.emeid = e.emeid
            LEFT JOIN emenda.entidadebeneficiada enb USING(enbid)
            LEFT JOIN workflow.documento doc USING(docid)
            LEFT JOIN workflow.estadodocumento esd USING(esdid)
            LEFT JOIN emenda.v_funcionalprogramatica vfp ON vfp.acaid = e.acaid
            {$where} AND doc.esdid IS NULL AND etoid in (1, 3, 5)  --aguardando preenchimento
DML;
    $emendasParlamentatesAguardando = $db->pegaLinha($sql);

//	    Em preenchimento
    $sql = <<<DML
            SELECT 	count(e.emeid) as preenchimento
            FROM emenda.emenda e
            LEFT JOIN emenda.emendadetalhe ed ON (ed.emeid = e.emeid) AND emdstatus = 'A'
            INNER JOIN emenda.autor aut ON (aut.autid = e.autid)
            INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
            JOIN emenda.partido par ON aut.parid = par.parid
            LEFT JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
            JOIN public.unidade uni ON vf.unicod = uni.unicod
            LEFT JOIN public.secretariaunidade secuo ON uni.unicod = secuo.unicod
            LEFT JOIN public.secretaria sec ON sec.secid = secuo.secid
            LEFT JOIN spo.siopexecucao sex ON sex.emecod::text = e.emecod::text AND sex.exercicio::text = e.emeano::text
            LEFT JOIN
                    (SELECT DISTINCT emeid,
                               CASE
                                   WHEN emdimpositiva = 6 THEN 'S'
                                   ELSE 'N'
                               END AS impositiva
               FROM emenda.emendadetalhe) imp ON imp.emeid = e.emeid
            LEFT JOIN spoemendas.emendainformacoes ine ON ine.emeid = e.emeid
            LEFT JOIN emenda.entidadebeneficiada enb USING(enbid)
            LEFT JOIN workflow.documento doc USING(docid)
            LEFT JOIN workflow.estadodocumento esd USING(esdid)
            LEFT JOIN emenda.v_funcionalprogramatica vfp ON vfp.acaid = e.acaid
            {$where} AND doc.esdid = '1685' --em preenchimento
DML;
    $emendasParlamentatesPreenchimento = $db->pegaLinha($sql);

//	    Plano de trabalho
    $sql = <<<DML
            SELECT 	count(e.emeid) as ptaParlamentates
            FROM emenda.emenda e
            LEFT JOIN emenda.emendadetalhe ed ON (ed.emeid = e.emeid) AND emdstatus = 'A'
            INNER JOIN emenda.autor aut ON (aut.autid = e.autid)
            INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
            JOIN emenda.partido par ON aut.parid = par.parid
            LEFT JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
            JOIN public.unidade uni ON vf.unicod = uni.unicod
            LEFT JOIN public.secretariaunidade secuo ON uni.unicod = secuo.unicod
            LEFT JOIN public.secretaria sec ON sec.secid = secuo.secid
            LEFT JOIN spo.siopexecucao sex ON sex.emecod::text = e.emecod::text AND sex.exercicio::text = e.emeano::text
            LEFT JOIN
                    (SELECT DISTINCT emeid,
                               CASE
                                   WHEN emdimpositiva = 6 THEN 'S'
                                   ELSE 'N'
                               END AS impositiva
               FROM emenda.emendadetalhe) imp ON imp.emeid = e.emeid
            LEFT JOIN spoemendas.emendainformacoes ine ON ine.emeid = e.emeid
            LEFT JOIN emenda.entidadebeneficiada enb USING(enbid)
            LEFT JOIN workflow.documento doc USING(docid)
            LEFT JOIN workflow.estadodocumento esd USING(esdid)
            LEFT JOIN emenda.v_funcionalprogramatica vfp ON vfp.acaid = e.acaid
            {$where} AND doc.esdid = '1685' --pta
DML;
    $ptaParlamentates = $db->pegaLinha($sql);

//	    Impedimentos
    $sql = <<<DML
            SELECT count(DISTINCT e.emeid) as impedimento FROM emenda.emenda e
INNER JOIN emenda.emendadetalhe ed ON e.emeid = ed.emeid
INNER JOIN emenda.emendadetalheimpositivo edi ON edi.emdid = ed.emdid
INNER JOIN emenda.autor aut ON aut.autid = e.autid
{$where} AND edi.ediimpositivo != 'NH'   
DML;
    $impedimentoParlamentates = $db->pegaLinha($sql);

//	    Execuçao
    $sql = <<<DML
            SELECT 	count(e.emeid) as execucaoParlamentates
            FROM emenda.emenda e
            LEFT JOIN emenda.emendadetalhe ed ON (ed.emeid = e.emeid) AND emdstatus = 'A'
            INNER JOIN emenda.autor aut ON (aut.autid = e.autid)
            INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
            JOIN emenda.partido par ON aut.parid = par.parid
            LEFT JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
            JOIN public.unidade uni ON vf.unicod = uni.unicod
            LEFT JOIN public.secretariaunidade secuo ON uni.unicod = secuo.unicod
            LEFT JOIN public.secretaria sec ON sec.secid = secuo.secid
            LEFT JOIN spo.siopexecucao sex ON sex.emecod::text = e.emecod::text AND sex.exercicio::text = e.emeano::text
            LEFT JOIN
                    (SELECT DISTINCT emeid,
                               CASE
                                   WHEN emdimpositiva = 6 THEN 'S'
                                   ELSE 'N'
                               END AS impositiva
               FROM emenda.emendadetalhe) imp ON imp.emeid = e.emeid
            LEFT JOIN spoemendas.emendainformacoes ine ON ine.emeid = e.emeid
            LEFT JOIN emenda.entidadebeneficiada enb USING(enbid)
            LEFT JOIN workflow.documento doc USING(docid)
            LEFT JOIN workflow.estadodocumento esd USING(esdid)
            LEFT JOIN emenda.v_funcionalprogramatica vfp ON vfp.acaid = e.acaid
            {$where} AND doc.esdid = '1685' --pta
DML;
    $execucaoParlamentates = $db->pegaLinha($sql);
}

?>
<div class="row">
    <div class="col-sm-9">
        <div class="col-sm-12">
            <div id="divLiberacaoFinanceira" class="box-white">
                <div class="row">
                    <div class="col-sm-4 col-sm-offset-4">

                        <div class="small-box bg-aqua">
                            <a href="spoemendas.php?modulo=principal/emendas/listaremendas&acao=A">
                                <div class="inner">
                                    <h3><?= $emendasParlamentatesTotal['emendasparlamentates'] ?></h3>

                                    <p>Emendas Parlamentares</p>
                                </div>
                                <div class="small-box-footer">&nbsp;</div>
                            </a>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-sm-4 col-sm-offset-2">
                        <div class="small-box bg-green">
                            <a href="spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=filtrar&filtro=none&emendas[etoid][]=1&emendas[etoid][]=3&emendas[etoid][]=5&aguardando=1">
                                <div class="inner">
                                    <h3><?= $emendasParlamentatesAguardando['emendasparlamentatesaguardando'] ?></h3>

                                    <p>Aguardando Preenchimento pelo Parlamentar</p>
                                </div>
                                <div class="small-box-footer">&nbsp;</div>
                            </a>
                        </div>
                    </div>
                    <div class="col-sm-4">
                        <div class="small-box bg-green">
                            <a href="spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=filtrar&filtro=none&emendas[etoid][]=1&emendas[etoid][]=3&emendas[etoid][]=5">
                                <div class="inner">
                                    <h3><?= $emendasParlamentatesPreenchimento['preenchimento'] ?></h3>

                                    <p>Em Preenchimento pelo Parlamentares</p>
                                </div>
                                <div class="small-box-footer">&nbsp;</div>
                            </a>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-sm-4">
                        <div class="small-box bg-yellow">
                            <a href="#">
                                <div class="inner">
                                    <h3><?= $ptaParlamentates['ptaparlamentates'] ?></h3>

                                    <p>Plano de Trabalho</p>
                                </div>
                                <div class="small-box-footer">&nbsp;</div>
                            </a>
                        </div>
                    </div>
                    <div class="col-sm-4">
                        <div class="small-box bg-yellow">
                            <a href="#">
                                <div class="inner">
                                    <h3><?= $impedimentoParlamentates['impedimento'] ?></h3>
                                    <p>Impedimentos</p>
                                </div>

                                <div class="small-box-footer">&nbsp;</div>
                            </a>
                        </div>
                    </div>
                    <div class="col-sm-4">
                        <div class="small-box bg-yellow">
                            <a href="<?= "spoemendas.php?modulo=principal/emendas/painelacompanhamento/painelacompanhamento&acao=A&emendas[exercicio]={$_SESSION['exercicio']}" ?>">
                                <div class="inner">
                                    <h3><?= $emendasParlamentatesTotal['emendasparlamentates'] ?></h3>

                                    <p>Execução</p>
                                </div>
                                <div class="small-box-footer">&nbsp;</div>
                            </a>
                        </div>
                    </div>
                </div>
                <div class="clear"></div>
            </div>
        </div>
    </div>
    <div class="col-sm-3 form-group box-white">
        <?php
        $calendario = (new Simec_Calendario($_SESSION['exercicio'], date('m')))
            ->mostrarQuantosMesesAntes(1)
            ->mostrarQuantosMesesDepois(1)->setLegenda(
                [
                    [
                        'type'      => 'text',
                        'label'     => 'Hoje',
                        'badge'     => '00',
                        'classname' => 'badge-today'
                    ]
                ]
            );

        if (!empty($msgPeriodo)) {
            $cores = [
                'rgba(87, 140, 169, 0.4)',
                'rgba(246, 209, 85, 0.4)',
                'rgba(0, 75, 141, 0.4)',
                'rgba(242, 85, 44, 0.4)',
                'rgba(149, 222, 227, 0.4)',
                'rgba(237, 205, 194, 0.4)',
                'rgba(206, 49, 117, 0.4)',
                'rgba(90, 114, 71, 0.4)',
                'rgba(207, 176, 149, 0.4)',
                'rgba(76, 106, 146, 0.4)',
                'rgba(146, 182, 213, 0.4)'
            ];
            foreach ($msgPeriodo as $index => $item) {
                $calendario->addEventoPeriodo(
                    str_replace('/', '-', $item['ppedatainicio']),
                    str_replace('/', '-', $item['ppedatafim']),
                    false,
                    $item['prfnome'],
                    $item['ppemensagem'],
                    '',
                    $cores[$index],
                    false
                )
                    ->addLegenda('block', $item['prfnome'], null, $cores[$index]);
            }
        }

        $calendario->render();
        ?>
    </div>
</div>
<?php
$sql = <<<DML
SELECT
      e.emecod                     AS categoria
    , 'Valor RCL (R$)'             AS descricao
    , SUM (COALESCE (edevalor, 0)) AS valor
    , aut.autnome
FROM emenda.emenda e
    LEFT JOIN emenda.emendadetalhe ed ON (ed.emeid = e.emeid) AND emdstatus = 'A'
    INNER JOIN emenda.autor aut ON (aut.autid = e.autid)
    INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
    JOIN emenda.partido par ON aut.parid = par.parid
    LEFT JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
    LEFT JOIN spo.siopexecucao sex ON sex.emecod :: TEXT = e.emecod :: TEXT AND sex.exercicio :: TEXT = e.emeano :: TEXT
{$where}
GROUP BY
    e.emecod
    , aut.autnome
UNION
SELECT
      e.emecod                             AS categorias
    , 'Empenhado (R$)'                     AS name
    , SUM (DISTINCT COALESCE (sex.vlrempenhado, 0)) AS empenhado
    , aut.autnome
FROM
    emenda.emenda e
    LEFT JOIN emenda.emendadetalhe ed ON (ed.emeid = e.emeid) AND emdstatus = 'A'
    INNER JOIN emenda.autor aut ON (aut.autid = e.autid)
    INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
    JOIN emenda.partido par ON aut.parid = par.parid
    LEFT JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
    LEFT JOIN
    spo.siopexecucao sex
        ON sex.emecod :: TEXT = e.emecod :: TEXT AND sex.exercicio :: TEXT = e.emeano :: TEXT
{$where}
GROUP BY
    e.emecod
    , aut.autnome
UNION
SELECT
      e.emecod                                 AS categorias
    , 'Pago (R$)'                              AS name
    , SUM (DISTINCT COALESCE (sex.vlrpago, 0)) AS pago
    , aut.autnome
FROM emenda.emenda e
    LEFT JOIN emenda.emendadetalhe ed ON (ed.emeid = e.emeid) AND emdstatus = 'A'
    INNER JOIN emenda.autor aut ON (aut.autid = e.autid)
    INNER JOIN emenda.v_funcionalprogramatica vf ON (vf.acaid = e.acaid AND vf.acastatus = 'A')
    JOIN emenda.partido par ON aut.parid = par.parid
    LEFT JOIN emenda.emendadetalheentidade ede ON (ede.emdid = ed.emdid AND edestatus = 'A')
    LEFT JOIN spo.siopexecucao sex ON sex.emecod :: TEXT = e.emecod :: TEXT AND sex.exercicio :: TEXT = e.emeano :: TEXT
{$where}
GROUP BY
    e.emecod
    ,aut.autnome
ORDER BY
    1, 2
DML;
$dadosGrafico = $db->carregar($sql);

if (!empty($dadosGrafico)) {
    $titleHighCharts = '';
    $categoriesGrafico = [];
    $seriesGrafico = [];
    $seriesDadosGrafico = [];

    foreach ($dadosGrafico as $index => $item) {
        $titleHighCharts = $item['autnome'];
        if (!in_array($item['descricao'], $categoriesGrafico)) {
            $categoriesGrafico[] = $item['categoria'];
        }
        $seriesDadosGrafico[$item['descricao']][] = $item['valor'];
    }

    foreach ($seriesDadosGrafico as $index => $item) {
        $seriesGrafico[] = [
            'name' => $index,
            'data' => $item
        ];
    }

    $categoriesGrafico = json_encode($categoriesGrafico);
    $seriesGrafico = json_encode($seriesGrafico, JSON_NUMERIC_CHECK);

    ?>
    <div class="row">
        <div class="col-sm-12">
            <div class="box-white">
                <div id="containerHC" style="height: <?= 120 + (count($dadosGrafico) * 20) . 'px' ?>"></div>
                <script>
                    <?= <<<JS
Highcharts.setOptions({
    lang: {
        decimalPoint: ',',
        thousandsSep: '.'
    }
}); 
Highcharts.chart('containerHC', {
  chart: {
    type: 'bar'
  },
  title: {
    text: '{$titleHighCharts}'
  },
  xAxis: {
    categories: {$categoriesGrafico},
    title: {
      text: 'Emenda'
    },
    labels: {
      rotation: -45
    }
  },
  yAxis: {
    min: 0,
    title: {
      text: 'Valor (em R$)',
      align: 'high'
    },
    labels: {
      overflow: 'justify',
      rotation: -45
    }
  },
  tooltip: {
    valuePrefix: 'R$ ',
    shared: true
  },
  plotOptions: {
    bar: {
      dataLabels: {
        enabled: true,
        format: 'R$ {point.y:,.2f}'
      }
    }
  },
  legend: {
    layout: 'vertical',
    align: 'right',
    verticalAlign: 'top',
    // x: -40,
    // y: 80,
    // floating: true,
    borderWidth: 1,
    backgroundColor: ((Highcharts.theme && Highcharts.theme.legendBackgroundColor) || '#FFFFFF'),
    shadow: true
  },
  credits: {
    enabled: false
  },
  exporting: {
    enabled: false
  },
  series: {$seriesGrafico}
});
JS;
                    ?>
                </script>
            </div>
        </div>
    </div>
    <?php
}

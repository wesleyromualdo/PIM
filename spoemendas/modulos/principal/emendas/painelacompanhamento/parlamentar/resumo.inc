<?php
/**
 * $Id: vinculacaoiniciativas.inc 108768 2016-03-02 14:41:50Z werteralmeida $
 */
include_once APPRAIZ . 'includes/_funcoes_cockpit.php';
#echo '<script charset="ISO-8895-1" type="text/javascript" src="../library/jquery/jquery-1.10.2.js">';


if (!$autid) {
    $autid = $_REQUEST['autid'];
}
global $db;
$nomeParlamentar = $db->pegaUm("SELECT
                    UPPER(autnome)
                FROM
                    emenda.autor
                WHERE
                    autid = {$autid}");
?>
<div class="col-md-12" >
    <div class="col-md-12">
        <div class="panel panel-info">
            <div class="panel-heading">
                <h4>Parlamentar <b><?php echo $nomeParlamentar ?></b>
                    <button style="float: right; clear: both;" class="btn btn-warning" onClick="javascript:imprimirResumoParlamentar(<?php echo $autid; ?>)" type="button"><span class="glyphicon glyphicon-print"></span> Imprimir </button>
                    <br/>    <br/>
                </h4>
            </div>
        </div>
    </div>
    <div class="col-md-12">
        <section class="panel panel-primary">
            <section class="panel-heading"><strong>Emendas do Parlamentar</strong></section>
            <section class="panel-body">
                <?php
                $exercicios = array('Todos' => '',
                    '2016' => '2016',
                    '2015' => '2015',
                    '2014' => '2014',
                    '2013' => '2013',
                    '2012' => '2012',
                    '2011' => '2011',
                    '2010' => '2010');
                $form = new Simec_View_Form('emendas', $method = Simec_View_Form::POST, $action = '');
                $form->carregarDados($_REQUEST['emendas']);
                $form->addChoices('Exercício', 'exercicio', $exercicios)
                        ->render();
                $whereExercicio = "";
                $exercicio = $_SESSION['exercicio'];
                if (isset($_REQUEST['emendas']['exercicio'])) {
                    $whereExercicio = " AND e.emeano = '{$_REQUEST['emendas']['exercicio']}'";
                    $exercicio = $_REQUEST['emendas']['exercicio'];
                }
                $sql = <<<DML
                    SELECT
                        e.emeid,
                        e.emeano,
                        e.emecod,
                        vf.unicod ||' - ' ||uni.uniabrev ||' - ' ||uni.unidsc AS unidade,
                        e.emevalor                                            AS valoremenda,
                        SUM(ede.edevalor)                                     AS valorrcl,
                        SUM( DISTINCT sex.vlrlimiteempenho)                   AS limite,
                        SUM( DISTINCT sex.vlrempenhado)                       AS empenhado,
                        CASE WHEN e.emeano > 2015 THEN SUM( DISTINCT sex.vlrsaldoapagar) ELSE 0 END AS saldo,
                        CASE WHEN e.emeano > 2015 THEN SUM( DISTINCT sex.vlrpago) ELSE 0 END AS pago
                    FROM
                        emenda.emenda e
                    LEFT JOIN
                        emenda.emendadetalhe ed
                    ON
                        (
                            ed.emeid = e.emeid and emdstatus = 'A' )
                    INNER JOIN
                        emenda.autor aut
                    ON
                        (
                            aut.autid = e.autid)
                    INNER JOIN
                        emenda.v_funcionalprogramatica vf
                    ON
                        (
                            vf.acaid = e.acaid
                        AND vf.acastatus = 'A')
                    JOIN
                        emenda.partido par
                    ON
                        aut.parid = par.parid
                    LEFT JOIN
                        emenda.emendadetalheentidade ede
                    ON
                        (
                            ede.emdid = ed.emdid
                        AND edestatus = 'A')
                    JOIN
                        public.unidade uni
                    ON
                        vf.unicod = uni.unicod
                    LEFT JOIN
                        spo.siopexecucao sex
                    ON
                        sex.emecod:: text = e.emecod::text
                    AND sex.exercicio::text = e.emeano::text
                    LEFT JOIN
                        spoemendas.emendainformacoes ine
                    ON
                        ine.emeid = e.emeid
                    LEFT JOIN
                        emenda.entidadebeneficiada enb
                    USING
                        (enbid)
                    LEFT JOIN
                        workflow.documento doc
                    USING
                        (docid)
                    LEFT JOIN
                        workflow.estadodocumento esd
                    USING
                        (esdid)
                    WHERE
                        true
                    AND aut.autid IN
                        (
                            SELECT
                                autid
                            FROM
                                emenda.autor
                            WHERE
                                UPPER(autnome) =
                                (
                                    SELECT
                                        UPPER(autnome)
                                    FROM
                                        emenda.autor
                                    WHERE
                                        autid = {$autid}) )
                    {$whereExercicio}
                    GROUP BY
                        1,2,3,4,5
                    ORDER BY
                        2 DESC,
                        3 ASC
DML;
                /* Caso não venha o momento poe o SQL como falso */
                $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
                $listagem->setCabecalho(array(
                    'Ano',
                    'Codigo da Emenda',
                    'Unidade Orçamentária (UO)',
                    'Valor (Dot. Atualizada) (a)',
                    'Indicado (b)',
                    'Limite de Empenho (d)',
                    'Empenhado (e)',
                    'Saldo a Pagar (R$)',
                    'Pago (f)'));
                $listagem->setQuery($sql);
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('emendas', 'finalziado', 'restantes'));
                $listagem->turnOnPesquisator();
                $listagem->addAcao('info', 'verEmenda');
                $listagem->addCallbackDeCampo('limiteempenhado', 'callbackProgressBar');
                $listagem->addCallbackDeCampo(array('valoremenda', 'valordetalhado', 'valorrcl', 'limite', 'empenhado', 'saldo', 'pago'), 'mascaraMoeda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('valoremenda', 'valorrcl', 'edevalordisponivel', 'limite', 'empenhado', 'saldo', 'pago'));
                $listagem->addCallbackDeCampo('unidade', 'formatarUnidadeListagem');
                $listagem->render();
                ?>
            </section>
        </section>
    </div>
</div>

<div class="modal fade" id="modal-ver-emenda">
    <div class="modal-dialog-large">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Detalhes da Emenda Parlamentar</h4>
            </div>
            <div class="modal-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>
<script>
    $(document).ready(function () {
        $("input[name='emendas[exercicio]']").change(function () {
            autid = <?php echo $autid; ?>;
            location.href = "/spoemendas/spoemendas.php?modulo=principal/emendas/painelacompanhamento/painelacompanhamento&acao=A&aba=parlamentar&autid=" + autid + '&emendas[exercicio]=' + $("input[name='emendas[exercicio]']:checked").val();
        });
    });
    function verEmenda(emeid) {
        $('#modal-ver-emenda .modal-body').html('');
        $.ajax({
            url: "spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=ver-emenda&emeid=" + emeid,
        }).done(function (html) {
            conteudo = html;
            $('#modal-ver-emenda .modal-body').html(conteudo);
        });

        $('#modal-ver-emenda').modal();

    }
    function imprimirModalEmenda(emeid) {
        window.open('spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=imprimirModalEmenda&emeid=' + emeid);
    }
    function imprimirResumoParlamentar(autid) {
        window.open('spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=imprimirResumoParlamentar&autid=' + autid);
    }
</script>

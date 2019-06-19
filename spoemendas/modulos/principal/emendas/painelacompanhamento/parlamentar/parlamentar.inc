<?php
/**
 * $Id: vinculacaoiniciativas.inc 108768 2016-03-02 14:41:50Z werteralmeida $
 */
include APPRAIZ . "includes/cabecalho.inc";
/*
 * Entra direto no resumo, caso seja parlamentar.
 */
$perfis = pegaPerfilGeral($_SESSION['usucpf']);
if (in_array(PFL_PARLAMENTAR, $perfis)) {
    $sql = "SELECT autid FROM spoemendas.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' AND rpustatus = 'A'";
    global $db;
    $_REQUEST['autid'] = $db->pegaUm($sql);
}

if (isset($_REQUEST['autid'])) {
    $autid = $_REQUEST['autid'];
    include 'resumo.inc';
    die;
}

?>
<script>
    /**
     * verEmendasParlamentar(){}
     */
    function verResumoParlamentar(autid) {
        location.href = '/spoemendas/spoemendas.php?modulo=principal/emendas/painelacompanhamento/painelacompanhamento&acao=A&aba=parlamentar&autid=' + autid;
    }
</script>
<div class="col-md-12" >

        <?php
        $sql = <<<DML
SELECT
    eme.autid,
    aut.autnome,
    par.parsigla || ' ('|| aut.estuf ||')' as partido,
    aut.autemail,
    (SELECT COUNT(0) FROM emenda.emenda WHERE autid = eme.autid )
FROM
    emenda.emenda eme
JOIN
    emenda.autor aut
USING
    (autid)
JOIN emenda.partido par
    using(parid)
WHERE
    emeano = '{$_SESSION['exercicio']}'
GROUP BY
    1,2,3,4
ORDER BY
    2
DML;
        /* Caso não venha o momento poe o SQL como falso */
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho(array('Parlamentar', 'Partido', 'E-mail', 'Emendas (Todos os Exercícios)'));
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('emendas', 'finalziado', 'restantes'));
        $listagem->addCallbackDeCampo(array('autnome'), 'alinhaParaEsquerda');
        $listagem->turnOnPesquisator();
        $listagem->addAcao('view', 'verResumoParlamentar');
        $listagem->render();
        ?>

</div>

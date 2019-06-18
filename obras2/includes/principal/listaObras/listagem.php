<?php

if ((array_key_exists('obrid', $_POST) || array_key_exists('esdid', $_POST) || array_key_exists('estuf', $_POST)) || !possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC, PFLCOD_CALL_CENTER, PFLCOD_ADMINISTRADOR))) {

    $obras = new Obras();
    $filtro = $_POST;

    if ($filtro['req'] == 'excel') {
        ini_set("memory_limit", "1024M");
        $filtro['excel'] = true;
        $cabecalho = getCabecalho(true);
        $sql = listaSql($filtro);
        $db->sql_to_xml_excel($sql, 'relatorioListaObjetosObras', $cabecalho, '');
    } else {
        $cabecalho = getCabecalho();
        $sql = listaSql($filtro);
        $cache = (IS_PRODUCAO) ? 600 : null;

        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'N', "formulario", '', '', $cache);

    }
} else {
    ?>
    <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
        <tr>
            <td style="text-align: justify;">
                Para listar as obras, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for
                escolhido serÃ£o apresentados todos os registros que vocÃª pode acessar.
            </td>
        </tr>
    </table>
    <?php
}

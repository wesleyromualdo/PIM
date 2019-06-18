<?php

ob_start();

if ($_POST['uploadFichaB'] == true){
    include_once APPRAIZ . 'sase/classes/UploadFichaMonitoramento.class.inc';
    $upload = new UploadFichaMonitoramento();
    echo $upload->verificarErroUpload()->uploadFichaB();
    die;

} elseif ($_POST['uploadFichaC'] == true){
    include_once APPRAIZ . 'sase/classes/UploadFichaMonitoramento.class.inc';
    $upload = new UploadFichaMonitoramento();
    echo $upload->verificarErroUpload()->uploadFichaC();
    die;

}else {

    // --------------- Dependências
        include_once '../../sase/classes/Mapa/MetaDados.class.inc';
        include_once '../../sase/classes/Mapa/Poligonos.class.inc';
        include_once '../../sase/classes/Mapa/Mapas.class.inc';
        include_once '../../sase/classes/FichamonitoramentoEstado.class.inc';
        include_once '../../territorios/classes/model/Municipio.class.inc';
        include_once '../../territorios/classes/model/Estado.class.inc';
        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';


        include APPRAIZ . 'includes/cabecalho.inc';
    // --
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);

    // --------------- Decisão da Página
        $pagina = "";
        if (!$_GET['aba'] || $_GET['aba'] == 'listaEstado') $pagina = "listaMonitoramentoEstado";
        if ($_GET['aba'] == 'fichaEstado') $pagina = "fichaMonitoramentoEstado";
        if ($_GET['aba'] == 'mapaEstado') $pagina = "mapaEstado";
        if ($_GET['aba'] == 'relatorioFichaTecEstado') $pagina = "relatorioFichaTecEstado";
    // --

        if (in_array(PFLCOD_SASE_CONSULTA, $perfis)) {
            $pagina = 'mapaEstado';
        }

    // --------------- Cabeçalho
        $cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
        $cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
        $titulo = 'Ficha Técnica do Plano Estadual/Distrital de Educação';

        ?>
        <link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
        <script>
            jQuery("#loading").hide();
        </script>
        <div id='cabecalhoSistema'>
            <?= '<h6><small>' . implode('</small></h6><h6><small>', $cabecalhoSistema) . '</small></h6>' ?>
            <center><h3><?= $titulo ?></h3></center>
        </div>

        <div id="menu_sistema">
            <ul class="nav nav-tabs">
                <?php if ((array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false)) {
                } else { ?>
                    <li <?= ($pagina == 'listaMonitoramentoEstado') ? 'class="active"' : '' ?>><a
                                href="sase.php?modulo=principal/monitoramentoEstado&acao=A&aba=listaEstado">Lista</a></li>
                <?php } ?>

                <?php if ((array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) || (array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis))) {
                } else { ?>
                    <li <?= ($pagina == 'fichaMonitoramentoEstado') ? 'class="active"' : '' ?>><a
                                href="sase.php?modulo=principal/monitoramentoEstado&acao=A&aba=fichaEstado">Ficha Técnica</a></li>
                <?php } ?>

                <?php if ((array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) || (array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis))) {
                } else { ?>
                    <li <?= ($pagina == 'relatorioFichaTecEstado') ? 'class="active"' : '' ?>><a
                                href="sase.php?modulo=principal/monitoramentoEstado&acao=A&aba=relatorioFichaTecEstado">Relatório Ficha Técnica</a></li>
                <?php } ?>

                <?php if (!array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)) { ?>
                    <li <?= ($pagina == 'mapaEstado') ? 'class="active"' : '' ?>><a
                                href="sase.php?modulo=principal/monitoramentoEstado&acao=A&aba=mapaEstado">Mapa</a></li>
                <?php } ?>
            </ul>
        </div><?php
    // --

    // --------------- Inclusão da Página
        include APPRAIZ . "sase/modulos/principal/monitoramento/" . $pagina . ".inc";
    // --

}
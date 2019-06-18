<?php

ob_start();

if ($_POST['uploadFichaB'] == true){
    include_once APPRAIZ . 'sase/classes/UploadFichaMonitoramento.class.inc';
    $upload = new UploadFichaMonitoramento();
    echo $upload->verificarErroUpload()->uploadFichaB();
    die;

}elseif ($_POST['uploadFichaC'] == true){
    include_once APPRAIZ . 'sase/classes/UploadFichaMonitoramento.class.inc';
    $upload = new UploadFichaMonitoramento();
    echo $upload->verificarErroUpload()->uploadFichaC();
    die;

}else {

    // --------------- Dependências
        include_once '../../sase/classes/Mapa/MetaDados.class.inc';
        include_once '../../sase/classes/Mapa/Poligonos.class.inc';
        include_once '../../sase/classes/Mapa/Mapas.class.inc';
        include_once '../../sase/classes/Fichamonitoramento.class.inc';
        include_once '../../territorios/classes/model/Municipio.class.inc';
        include_once '../../territorios/classes/model/Estado.class.inc';
        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

        include APPRAIZ . 'includes/cabecalho.inc';
    // include APPRAIZ . 'www/sase/_funcoes.php';
    // --

        $perfis = pegaPerfilGeral($_SESSION['usucpf']);

    // --------------- Decisão da Página
        $pagina = "";
        /*
        if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "listaMonitoramento";
        if( $_GET['aba'] == 'ficha' ) $pagina = "fichaMonitoramento";
        if( $_GET['aba'] == 'mapa' ) $pagina = "mapa"; */

        if (!$_GET['aba']) $pagina = "listaMonitoramento";

        switch ($_GET['aba']) {
            case 'lista' :
                $pagina = "listaMonitoramento";
                break;
            case 'ficha' :
                $pagina = "fichaMonitoramento";
                break;
            case 'mapa'  :
                $pagina = "mapa";
                break;
            case 'relatorioFichaTec'  :
                $pagina = "relatorioFichaTec";
                break;
            default :
                $pagina = "listaMonitoramento";
                break;
        }
    // --

        if (array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
            && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false
        )
            $pagina = 'mapa';

    // --------------- Cabeçalho
        $cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
        $cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
        $titulo = 'Ficha Técnica do Plano Municipal de Educação ';

        ?>
        <link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

        <div id='cabecalhoSistema'>
            <?= '<h6><small>' . implode('</small></h6><h6><small>', $cabecalhoSistema) . '</small></h6>' ?>
            <center><h3><?= $titulo ?></h3></center>
        </div>

        <div id="menu_sistema">
        <ul class="nav nav-tabs">
            <?php if ((array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
                && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false)
            ) {
            } else { ?>

                <li <?= ($pagina == 'listaMonitoramento') ? 'class="active"' : '' ?>><a
                            href="sase.php?modulo=principal/monitoramento&acao=A&aba=lista">Lista</a></li>

            <?php } ?>

            <?php if ((array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
                    && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) || (array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis))
            ) {
            } else { ?>

                <li <?= ($pagina == 'fichaMonitoramento') ? 'class="active"' : '' ?>><a
                            href="sase.php?modulo=principal/monitoramento&acao=A&aba=ficha">Ficha Técnica</a></li>

            <?php } ?>

            <?php if ((array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
                    && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) || (array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis))
            ) {
            } else { ?>
                <li <?= ($pagina == 'relatorioFichaTec') ? 'class="active"' : '' ?>>
                    <a href="sase.php?modulo=principal/monitoramento&acao=A&aba=relatorioFichaTec">Relatório Ficha Técnica</a>
                </li>
            <?php } ?>

            <?php if (!array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)) { ?>
                <li <?= ($pagina == 'mapa') ? 'class="active"' : '' ?>><a
                            href="sase.php?modulo=principal/monitoramento&acao=A&aba=mapa">Mapa</a></li>
            <?php } ?>

        </ul>
        </div><?php
    // --
    // --------------- Inclusão da Página
        include APPRAIZ . "sase/modulos/principal/monitoramento/" . $pagina . ".inc";
    // --

}
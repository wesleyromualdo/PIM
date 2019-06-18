<?php

include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/Territorio.class.inc';
include_once '../../sase/classes/OrganizacoesTerritoriais.class.inc';

$organizacoesTerritoriais = new OrganizacoesTerritoriais();

global $db;
ob_start();

if (possui_perfil(PFLCOD_SASE_COORDENADOR_UNCME)) {
    $estuf = pegaEstadoVinculado($_SESSION['usucpf'], PFLCOD_SASE_COORDENADOR_UNCME);
} else {
    if (possui_perfil(PFLCOD_SASE_EXECUTIVO)) {
        $estuf = pegaEstadoVinculado($_SESSION['usucpf'], PFLCOD_SASE_EXECUTIVO);
    }
}
/*Os perfis com acesso a essa funcionalidade são:
 * Administrador;
Coordenador Estadual DICOPE;
Coordenador UNCME;
Super Usuário.
 * */

if ($_POST['acao']) {
    switch ($_POST['acao']) {
//        case 'carrega_orgaos':
//            $organizacoesTerritoriais->carregarOrgaos();
//            exit;
        case 'filtraMunicipiosmapa':
            if ($_POST['filtroAjax']) {
                $organizacoesTerritoriais->getOptionMunicipios($_POST['uf']);
            }
            exit;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';


// --------------- Decisão da Página
$pagina = "";
if (!$_GET['aba'] || $_GET['aba'] == 'mapa') $pagina = "mapa";
if ($_GET['aba'] == 'edicao') $pagina = "organizacaoterritorial";
if ($_GET['aba'] == 'lista') $pagina = "lista";
// --

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Organizações Territoriais';

?>
    <link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

    <div id='cabecalhoSistema'>
        <?= '<h6><small>' . implode('</small></h6><h6><small>', $cabecalhoSistema) . '</small></h6>' ?>
        <center><h3><?= $titulo ?></h3></center>
    </div>

    <div id="menu_sistema">
    <ul class="nav nav-tabs">
        <li <?= ($pagina == 'mapa') ? 'class="active"' : '' ?>>
            <a href="sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=mapa">Mapa</a>
        </li>
        <li <?= ($pagina == 'lista') ? 'class="active"' : '' ?>>
            <a href="sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=lista">Lista</a>
        </li>
        <li <?= ($pagina == 'organizacaoterritorial') ? 'class="active"' : '' ?>>
            <a href="sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao">Território</a>
        </li>

    </ul>
    </div><?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/organizacoesterritoriais/" . $pagina . ".inc";
// --


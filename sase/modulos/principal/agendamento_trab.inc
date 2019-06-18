<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include APPRAIZ . 'sase/classes/Model/Etapaagendacomcoord.class.inc';

$lista = new Sase_Model_Etapaagendacomcoord();

if( $_POST['request'] != '' ){
    switch( $_POST['request'] ){
        case 'atualizaComboMunicipio':
            $lista->atualizaComboMunicipio( $_REQUEST );
            die();
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'www/sase/_funcoes.php';
// --

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

// --------------- Decisão da Página
$pagina = "";
switch( $_GET['aba'] ){
    case 'lista':
        $pagina = 'lista_agenda_trab_comissao';
        $_active_lista = 'class="active"';
        break;
    case 'edit':
        $pagina = 'cad_agenda_trab_comissao';
        $_active_cad = 'class="active"';
        break;
    case 'mapa':
        $pagina = 'mapa_agenda_trab_comissao';
        $_active_mapa = 'class="active"';
        break;
    default:
        $pagina = 'lista_agenda_trab_comissao';
        $_active_lista = 'class="active"';
        break;
}
// --

$rMunicipio = $lista->buscaMunicipio($_REQUEST['muncod']);

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'AGENDA DE TRABALHO DA COMISSÃO COORDENADORA - MUNICÍPIO <br/> '.$rMunicipio;

?>
    <link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

    <style>
        #btPainelWorkflow{
            padding:8px 10px;
        }
        #btSimularUsuario{
            padding:8px 10px;
        }
        #btPainelWorkflow{
            padding:8px 10px;
        }
    </style>
    <div id='cabecalhoSistema'>
        <?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
        <center><h3><?=$titulo?></h3></center>
    </div>

    <div id="menu_sistema">
    <ul class="nav nav-tabs">
        <li <?=$_active_lista;?>> <a href="sase.php?modulo=principal/agendamento_trab&acao=A&aba=lista">Lista</a> </li>
        <li <?=$_active_cad;?>> <a href="sase.php?modulo=principal/agendamento_trab&acao=A&aba=edit">Agenda</a> </li>
        <li <?=$_active_mapa;?>> <a href="sase.php?modulo=principal/agendamento_trab&acao=A&aba=mapa">Mapa</a> </li>

    </ul>
    </div><?php
// --
// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/agendamento_trab_comissao/{$pagina}.inc";
// --


<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include APPRAIZ . 'sase/classes/Model/Agendacomissaocoordestado.class.inc';
include_once APPRAIZ . 'sase/classes/Model/Agendacomissaocoordestado.class.inc';

$lista = new Agendacomissaocoordestado();

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'www/sase/_funcoes.php';
// --

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

// --------------- Decisão da Página
$pagina = "";
switch( $_GET['aba'] ){
    case 'lista_estado':
        $pagina = 'lista_agenda_trab_comissao_estado';
        $_active_lista = 'class="active"';
        break;
    case 'edit_estado':
        $pagina = 'cad_agenda_trab_comissao_estado';
        $_active_cad = 'class="active"';
        break;
    case 'mapa_estado':
        $pagina = 'mapa_agenda_trab_comissao_estado';
        $_active_mapa = 'class="active"';
        break;
    default:
        $pagina = 'lista_agenda_trab_comissao_estado';
        $_active_lista = 'class="active"';
        break;
}
// --

$estado = $lista->buscaEstado($_REQUEST['estuf']);

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'AGENDA DE TRABALHO DA COMISSÃO COORDENADORA - ESTADO <br/>'.$estado;

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
        <li <?=$_active_lista;?>> <a href="sase.php?modulo=principal/agendamento_trab_estado&acao=A&aba=lista_estado">Lista</a> </li>
        <li <?=$_active_cad;?>> <a href="sase.php?modulo=principal/agendamento_trab_estado&acao=A&aba=edit_estado">Agenda</a> </li>
        <li <?=$_active_mapa;?>> <a href="sase.php?modulo=principal/agendamento_trab_estado&acao=A&aba=mapa_estado">Mapa</a> </li>

    </ul>
    </div><?php
// --
// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/agendamento_trab_comissao/{$pagina}.inc";
// --


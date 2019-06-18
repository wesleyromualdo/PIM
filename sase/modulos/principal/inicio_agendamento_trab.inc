<?php
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

    $perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

    $pagina = "";
    switch( $_GET['aba'] ){
        case 'lista':
            $pagina = 'lista_agenda_trab_comissao';
            break;
        case 'edit':
            $pagina = 'cad_agenda_trab_comissao';
            break;
        case 'mapa':
            $pagina = 'mapa_agenda_trab_comissao';
            break;
        default:
            $pagina = 'lista_agenda_trab_comissao';
    }

    if( in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) || in_array(PFLCOD_SASE_SUPER_USUARIO, $perfis) || in_array(PFLCOD_SASE_TECNICO, $perfis) ){
        if( $pagina == 'lista_agenda_trab_comissao'){
            $_active_lista = 'class="active"';
        }elseif( $pagina == 'cad_agenda_trab_comissao' ){
            $_active_cad = 'class="active"';
        }elseif( $pagina == 'mapa_agenda_trab_comissao' ){
            $_active_mapa = 'class="active"';
        }else{
            $_active_lista = 'class="active"';
        }
    }
?>
    <link href="/sase/css/estilo.css" type="text/css" media='screen'/>
    <link href="../library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">

    <script src="/includes/funcoes.js" type="text/javascript"></script>
    <script src="../library/bootstrap-switch/js/bootstrap-switch.min.js"></script>

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
    
    <div class="row">
        <div class="row col-md-12">
            <div class="panel panel-default">
                <div class="panel-heading">
                    <h3 class="panel-title text-center">
                        <b>AGENDA DE TRABALHO DA COMISSÃO COORDENADORA</b>
                    </h3>
                </div>
            </div>

            <div id="menu_agenda">
                <ul class="nav nav-tabs">
                    <li <?=$_active_lista;?>> <a href="sase.php?modulo=principal/inicio_agendamento_trab&acao=A&aba=lista">Listagem de Municípios</a> </li>
                    <li <?=$_active_cad;?>> <a href="sase.php?modulo=principal/inicio_agendamento_trab&acao=A&aba=edit">Angenda de Trabalho</a> </li>
                    <li <?=$_active_mapa;?>> <a href="sase.php?modulo=principal/inicio_agendamento_trab&acao=A&aba=mapa">Mapas</a> </li>
                </ul>
            </div>

            <?php
                include APPRAIZ . "sase/modulos/principal/agendamento_trab_comissao/{$pagina}.inc";
            ?>

        </div>
    </div>
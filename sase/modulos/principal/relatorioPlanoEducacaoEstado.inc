<?php
$cpf = $_SESSION['usucpf'];
$perfis = pegaPerfilGeral($cpf);
$lista =  listaEstadoVinculacao($cpf,$perfis);

ob_start();

    if($_POST['combomunicipio'] == true){
        comboMunicipio ("'".$_POST['estuf']."'", null);
        die;
    }elseif ($_REQUEST['pagina']) {
        switch ($_REQUEST['pagina']) {
            case 'relatorioAvaliacaoEstado';
                $pagina = 'relatorioAvaliacaoEstado';
                $titulo = 'Relatório de Avaliação dos Planos de Educação';
                break;
            case 'relatorioMonitoramentoEstado';
                $pagina = 'relatorioMonitoramentoEstado';
                $titulo = 'Relatório de Monitoramento dos Planos de Educação';
                break;
        }
    }

// --------------- Dependências


include APPRAIZ . 'includes/cabecalho.inc';

$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";

?>
    <link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
    <link rel="stylesheet" href="https://cdn.datatables.net/1.10.15/css/dataTables.bootstrap.min.css">
    <script>
        jQuery("#loading").hide();
    </script>
    <div id='cabecalhoSistema'>
        <?= '<h6><small>' . implode('</small></h6><h6><small>', $cabecalhoSistema) . '</small></h6>' ?>
        <center><h3><?= $titulo ?></h3></center>
    </div>

<?php
include APPRAIZ . "sase/modulos/principal/monitoramento/" . $pagina . ".inc";
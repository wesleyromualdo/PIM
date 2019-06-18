<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 07/06/2016
 * Time: 11:02
 */
ob_start();

include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
include_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';
include_once '../../sase/classes/Model/Espacoparticipacao.class.inc';
include_once '../../sase/classes/Model/Espacoparticipacaoestado.class.inc';

$fm = new Simec_Helper_FlashMessage('sase/espacoparticipacao');
if($_POST['acao']){
    switch($_POST['acao']){
        case 'salvar':
            switch($_REQUEST['tipo']){
                case 'municipio':
                    $esp = new Sase_Model_Espacoparticipacao();
                    $esp->carregarPorId($_POST['espid']);
                    if(empty($esp->usucpf)) { $esp->usucpf = $_SESSION['usucpf']; }
                    $esp->salvaArquivos();
                    break;

                case 'estado':
                    $epe = new Sase_Model_Espacoparticipacaoestado();
                    $epe->carregarPorId($_POST['epeid']);
                    if(empty($epe->usucpf)) { $epe->usucpf = $_SESSION['usucpf']; }
                    $epe->salvaArquivos();
                    break;
            }
            break;

        case 'download_arquivo':
            if (empty($_REQUEST['arqid'])) {
                throw new Exception('Não foi informado nenhum arquivo para download.');
            }

            $file = new FilesSimec();
            $file->getDownloadArquivo($_REQUEST['arqid']);
            die();

        case 'apaga_arquivo':
            if (empty($_REQUEST['arqid'])) {
                throw new Exception('Não foi informado nenhum arquivo para download.');
            } else {
                $esp = new Sase_Model_Espacoparticipacao($_REQUEST['espid']);
                $esp->$_REQUEST['campofile'] = null;
                if (!$esp->salvar(true, true, array($_REQUEST['campofile']))) {
                    throw new Exception('Não foi possível excluir os arquivos solicitados.');
                }
                $esp->commit();

                $file = new FilesSimec('espacoparticipacao', $_REQUEST['campofile'], 'sase');
                $file->setPulaTableEschema(true);
                $file->setRemoveUpload($_REQUEST['arqid']);
                unset($file);
                $fm->addMensagem('Arquivo removido com sucesso.');
            }

            break;
    }
}

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

$tipo = "municipio";
if($_GET['tipo']) {$tipo = $_GET['tipo'];}

if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "lista";
if( $_GET['aba'] == 'edicao' ) $pagina = "cadastro";
if( $_GET['aba'] == 'mapa' ) $pagina = "mapa";

$model = new Sase_Model_Espacoparticipacao($_REQUEST['espid']);

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$tpTitulo = 'Município';
if($tipo == 'estado'){
    $tpTitulo = 'Estado';
}
$titulo = 'Espaço de Participação - '.$tpTitulo;

include APPRAIZ . 'includes/cabecalho.inc';
?>

<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<div id='cabecalhoSistema'>
    <?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
    <center><h3><?=$titulo?></h3></center>
</div>
<?=$fm?>
<div id="menu_sistema">
    <ul class="nav nav-tabs">
        <li <?=($pagina=='lista')?'class="active"':''?>><a href="sase.php?modulo=principal/espacodeparticipacao&tipo=<?= $tipo ?>&acao=A&aba=lista">Lista</a></li>
        <li <?=($pagina=='cadastro')?'class="active"':''?>><a href="sase.php?modulo=principal/espacodeparticipacao&tipo=<?= $tipo ?>&acao=A&aba=edicao">Cadastro</a></li>
        <li <?=($pagina=='mapa')?'class="active"':''?>><a href="sase.php?modulo=principal/espacodeparticipacao&tipo=<?= $tipo ?>&acao=A&aba=mapa">Mapa</a></li>
    </ul>
</div>
<?php

include APPRAIZ . "sase/modulos/principal/espacodeparticipacao/{$tipo}/" . $pagina . ".inc";
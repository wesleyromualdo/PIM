<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 10:10
 */
ob_start();

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/PlanodecarreiraEstado.class.inc';
include_once '../../sase/classes/Planocarreiraprofessorestado.class.inc';
//include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
include_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';
$planodecarreira = new PlanodecarreiraEstado();
$fm = new Simec_Helper_FlashMessage('sase/planocarreiraestado');

$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );

$pagina = "";
if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "lista";
if( $_GET['aba'] == 'edicao' ) $pagina = "formulario";
if( $_GET['aba'] == 'mapa' ) $pagina = "mapa";

if( array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false ){
    $pagina = 'mapa';
}

// exporta xls
if(isset($_POST['funcao'])){
    switch ($_POST['funcao']){
        case 'xls':
//            ver('entrou',d);
            $planodecarreira->montaListaXls( $filtros );
            exit();
    }
}
if (isset($_POST['acao'])) {
    $pular = false;

    try {
        switch ($_POST['acao'])
        {
            case 'salva_arquivo':

                if (UPLOAD_ERR_OK == $_FILES['filepcr']['error']) {
                    $arquivo = new FilesSimec("plano de carreira estadual - pcr - pceid: {$_POST['pceid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['filepcr']['name'], 'filepcr', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $model = new Sase_Model_Planocarreiraprofessorestado();
                        $model->popularDadosObjeto(['pceid' => $_POST['pceid'], 'pcepcr' => $idArquivo]);
                        if (!$model->salvar()) {
                            throw new Exception('Não foi possível salvar os novos arquivos.');
                        }
                        $model->commit();
                        $fm->addMensagem('Arquivo PCR salvo com sucesso.');
                    }
                    unset($arquivo, $model);
                }

                if (UPLOAD_ERR_OK == $_FILES['fileadesao']['error']) {
                    $arquivo = new FilesSimec("plano de carreira municipal - adesao - pceid: {$_POST['pceid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['fileadesao']['name'], 'fileadesao', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $model = new Sase_Model_Planocarreiraprofessorestado();
                        $model->popularDadosObjeto(['pceid' => $_POST['pceid'], 'pceadesao' => $idArquivo]);
                        if (!$model->salvar()) {
                            throw new Exception('Não foi possível salvar o Termo de Adesão.');
                        }
                        $model->commit();
                        $fm->addMensagem('Termo de adesão salvo com sucesso.');
                    }
                    unset($arquivo, $model);
                }

                if (UPLOAD_ERR_OK == $_FILES['fileato']['error']) {
                    $arquivo = new FilesSimec("plano de carreira municipal - pceato - pceid: {$_POST['pceid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['fileato']['name'], 'fileato', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $model = new Sase_Model_Planocarreiraprofessorestado();
                        $model->popularDadosObjeto(['pceid' => $_POST['pceid'], 'pceato' => $idArquivo]);

                        if (!$model->salvar()) {
                            throw new Exception('Não foi possível salvar o Ato Legal da Comissão');
                        }
                        $model->commit();
                        $fm->addMensagem('Ato Legal da Comissão salvo com sucesso.');
                    }
                    unset($arquivo, $model);
                }

                if (UPLOAD_ERR_OK == $_FILES['filempl']['error']) {
                    $arquivo = new FilesSimec("plano de carreira estadual - pcempl - pceid: {$_POST['pceid']}", null, 'sase');
                    $nomeArquivo = $arquivo->setUpload($_FILES['filempl']['name'], 'filempl', false);
                    $idArquivo = $arquivo->getIdArquivo();

                    if (!empty($idArquivo)) {

                        $model = new Sase_Model_Planocarreiraprofessorestado();
                        $model->popularDadosObjeto(['pceid' => $_POST['pceid'], 'pcempl' => $idArquivo]);
                        if (!$model->salvar()) {
                            throw new Exception('Não foi possível salvar os novos arquivos.');
                        }
                        $model->commit();
                        $fm->addMensagem('Arquivo Minuta do PL salvo com sucesso.');
                    }
                    unset($arquivo, $model);
                }

                break;
            case 'download_arquivo':
                if (empty($_REQUEST['arqidpcr']) && empty($_REQUEST['arqidadesao']) && empty($_REQUEST['arqidato']) && empty($_REQUEST['arqidmpl'])) {
                    throw new Exception('Não foi informado nenhum arquivo para download.');
                }

                $file = new FilesSimec();
                if(!empty($_REQUEST['arqidpcr'])){
                    $file->getDownloadArquivo($_REQUEST['arqidpcr']);
                }
                if(!empty($_REQUEST['arqidadesao'])){
                    $file->getDownloadArquivo($_REQUEST['arqidadesao']);
                }
                if(!empty($_REQUEST['arqidato'])){
                    $file->getDownloadArquivo($_REQUEST['arqidato']);
                }
                if(!empty($_REQUEST['arqidmpl'])){
                    $file->getDownloadArquivo($_REQUEST['arqidmpl']);
                }
                die();
            case 'apaga_arquivo':
                if (empty($_REQUEST['arqidpcr']) && empty($_REQUEST['arqidadesao']) && empty($_REQUEST['arqidato']) && empty($_REQUEST['arqidmpl'])) {
                    throw new Exception('Não foi informado nenhum arquivo para exclusão.');
                }

                $model = new Sase_Model_Planocarreiraprofessorestado($_REQUEST['pceid']);
                // -- Atualizando o banco de dados
                if (!empty($_REQUEST['arqidpcr'])) {
                    $model->pcepcr = null;
                    if (!$model->salvar(true, true, ['pcepcr'])) {
                        throw new Exception('Não foi possível excluir os arquivos solicitados.');
                    }
                    $model->commit();

                    $file = new FilesSimec('planocarreiraprofessorestado', 'pcepcr', 'sase');
                    $file->setPulaTableEschema(true);
                    $file->setRemoveUpload($_REQUEST['arqidpcr']);
                    unset($file);
                    $fm->addMensagem('Arquivo PCR removido com sucesso.');

                }
                if (!empty($_REQUEST['arqidadesao'])) {
                    $model->pceadesao = null;
                    if (!$model->salvar(true, true, ['pceadesao'])) {
                        throw new Exception('Não foi possível excluir o Termo de Adesão.');
                    }
                    $model->commit();

                    $file = new FilesSimec('planocarreiraprofessorestado', 'pceadesao', 'sase');
                    $file->setPulaTableEschema(true);
                    $file->setRemoveUpload($_REQUEST['arqidadesao']);
                    unset($file);

                    $fm->addMensagem('Termo de adesão removido com sucesso.');
                }
                if (!empty($_REQUEST['arqidato'])) {
                    $model->pceato = null;
                    if (!$model->salvar(true, true, ['pceato'])) {
                        throw new Exception('Não foi possível excluir o Ato Legal da Comissão.');
                    }
                    $model->commit();

                    $file = new FilesSimec('planocarreiraprofessorestado', 'pceato', 'sase');
                    $file->setPulaTableEschema(true);
                    $file->setRemoveUpload($_REQUEST['arqidato']);
                    unset($file);

                    $fm->addMensagem('Ato Legal da Comissão  removido com sucesso.');
                }
                if (!empty($_REQUEST['arqidmpl'])) {
                    $model->pcempl = null;
                    if (!$model->salvar(true, true, ['pcempl'])) {
                        throw new Exception('Não foi possível excluir os arquivos solicitados.');
                    }
                    $model->commit();

                    $file = new FilesSimec('planocarreiraprofessorestado', 'pcempl', 'sase');
                    $file->setPulaTableEschema(true);
                    $file->setRemoveUpload($_REQUEST['arqidmpl']);
                    unset($file);
                    $fm->addMensagem('Arquivo Minuta do PL removido com sucesso.');

                }


                break;
            case 'pesquisar':
                $pular = true;

                break;
            default:
//                ver($_POST, d);
        }
    } catch (Exception $ex) {
        $fm->addMensagem($ex->getMessage(), Simec_Helper_FlashMessage::ERRO);
    }

    if (!$pular) {
        header("Location: {$_SERVER['REQUEST_URI']}");
        die();
    }
}

$model = new Sase_Model_Planocarreiraprofessorestado($_REQUEST['pceid']);
$arquivopcr = $model->getInfoArquivo('pcepcr');
$arquivoadesao = $model->getInfoArquivo('pceadesao');
$arquivoato = $model->getInfoArquivo('pceato');
$arquivompl = $model->getInfoArquivo('pcempl');
// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'Plano de Carreira de Profissionais de Ensino <br/> Estado';
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
        <?php if( (array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
            && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) ){ }else{ ?>

            <li <?=($pagina=='lista')?'class="active"':''?>><a href="sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=lista">Lista</a></li>

        <?php } ?>

        <?php if( (array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
                && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false) || (array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)) ){ }else{ ?>

            <li <?=($pagina=='formulario')?'class="active"':''?>><a href="sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=edicao">Plano de Carreira</a></li>

        <?php } ?>

        <?php if (!array_search(PFLCOD_SASE_CONSULTA_LEI_PEE_PME, $perfis)){ ?>
            <li <?=($pagina=='mapa')?'class="active"':''?>><a href="sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=mapa">Mapa</a></li>
        <?php } ?>
    </ul>
    </div><?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/planodecarreira/estado/" . $pagina . ".inc";
// --


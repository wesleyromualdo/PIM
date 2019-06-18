<?php
require_once APPRAIZ . 'includes/sac/classes/model/Area.class.inc';
require_once APPRAIZ . 'includes/sac/classes/model/Aviso.class.inc';
require_once APPRAIZ . 'includes/sac/classes/model/AvisoArquivo.class.inc';
require_once APPRAIZ . 'includes/sac/classes/model/AvisoDestinatario.class.inc';
require_once APPRAIZ . 'includes/sac/classes/model/AvisoFiltro.class.inc';
require_once APPRAIZ . 'includes/sac/classes/model/Categoria.class.inc';
require_once APPRAIZ . 'includes/sac/classes/model/Chamado.class.inc';
require_once APPRAIZ . 'includes/sac/classes/model/ChamadoHistorico.class.inc';
require_once APPRAIZ . 'includes/sac/classes/model/PerfilTipo.class.inc';

/**
 * Classe de controle do  aviso
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-03-2016
 * @link     no link
 */

/**
 * Sac_Controller_Aviso
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-03-2016
 * @link     no link
 */
class Sac_Controller_Aviso
{    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($dados,$arrFilesOld)
    {
        global $url;

        $modelAviso = new Sac_Model_Aviso($dados['aviid']);
        $modelAviso->popularDadosObjeto($dados);

        try{
            if(!$modelAviso->aviid){
                $modelAviso->avidatainclusao = date('Y-m-d H:i:s');
                $modelAviso->usucpfinclusao = $_SESSION['usucpforigem'];
                $modelAviso->avistatus = Sac_Model_Aviso::K_STATUS_ATIVO;
            }

            $modelAviso->avidatafim = !empty($dados['avidatafim']) ? $dados['avidatafim'] . ' 23:59:59' : null;

            $modelAviso->salvar(null, null, array('avititulo', 'avidatainicio', 'avidatafim'));

            // Não é executado na alteração
            if(!$dados['aviid'] && $modelAviso->aviid){
                $this->salvarFiltros($modelAviso->aviid, $dados);
                $this->salvarDestinatarios($modelAviso->aviid, $dados);
            }
            if (!empty($arrFilesOld['arqid_old'])){
                $this->salvarArquivos($modelAviso->aviid, $arrFilesOld);
            }
            
            $modelAviso->commit();

            simec_redirecionar($url . '&aviid=' . $modelAviso->aviid, 'success');
        } catch (Exception $e){
            $modelAviso->rollback();
            simec_redirecionar($url, 'error');
        }
    }//end salvar()

    public function inativar($aviid)
    {
        global $url;
        $modelAviso = new Sac_Model_Aviso($aviid);
        try{
            $modelAviso->avistatus = Sac_Model_Aviso::K_STATUS_INATIVO;

            $modelAviso->salvar();
            $modelAviso->commit();

            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $modelAviso->rollback();
            simec_redirecionar($url, 'error');
        }
    }//end salvar()

    public function salvarFiltros($aviid, $dados)
    {
        if($aviid){
            $modelAvisoFiltro = new Sac_Model_AvisoFiltro();
            $modelAvisoFiltro->aviid = $aviid;

            $modelAvisoFiltro->excluirVarios(" aviid = $aviid ");

            /*
             * Salvando perfis
             */
            if (isset($dados['pflcod']) && is_array($dados['pflcod'])) {
                foreach ($dados['pflcod'] as $pflcod) {
                    $modelAvisoFiltro->avfid = null;
                    $modelAvisoFiltro->pflcod = $pflcod;
                    $modelAvisoFiltro->salvar();
                }
            }

            $modelAvisoFiltro->pflcod = null;

            /*
             * Se tiver município, não salva uf nem região
             * Se tiver uf, não salva região
             */
            if (isset($dados['muncod']) && is_array($dados['muncod'])) {
                foreach ($dados['muncod'] as $muncod) {
                    $modelAvisoFiltro->avfid = null;
                    $modelAvisoFiltro->muncod = $muncod;
                    $modelAvisoFiltro->salvar();
                }
            } elseif(isset($dados['estuf']) && is_array($dados['estuf'])) {
                foreach ($dados['estuf'] as $estuf) {
                    $modelAvisoFiltro->avfid = null;
                    $modelAvisoFiltro->estuf = $estuf;
                    $modelAvisoFiltro->salvar();
                }
            } elseif(isset($dados['regcod']) && is_array($dados['regcod'])) {
                foreach ($dados['regcod'] as $regcod) {
                    $modelAvisoFiltro->avfid = null;
                    $modelAvisoFiltro->regcod = $regcod;
                    $modelAvisoFiltro->salvar();
                }
            }
        }
    }//end salvarFiltros()

    public function salvarDestinatarios($aviid, $dados)
    {

        if($aviid){
            $modelAvisoDestinatario = new Sac_Model_AvisoDestinatario();
            $usuarios = $modelAvisoDestinatario->recuperarUsuarios($dados);
            
            $modelAvisoDestinatario->aviid = $aviid;
            $modelAvisoDestinatario->avdstatus = Sac_Model_AvisoDestinatario::K_STATUS_NAO_LIDO;

            /*
             * Salvando destinatários
             */
            foreach ($usuarios as $usuario) {
                $modelAvisoDestinatario->avdid = null;
                $modelAvisoDestinatario->usucpf = $usuario['usucpf'];
                $modelAvisoDestinatario->salvar();
            }
        }
    }//end salvarDestinatarios()

    public function recuperarFiltros($aviid)
    {
        $modelAvisoFiltro = new Sac_Model_AvisoFiltro();
        return $modelAvisoFiltro->recuperarFiltros($aviid);
    }

    public function listar($sisid = null, $filtros = array())
    {
        $sisid = $sisid ? $sisid : $_SESSION['sisid'];

        $modelAviso = new Sac_Model_Aviso();
        $sql = $modelAviso->listar($sisid, $filtros);

        $cabecalho = array('Código', 'Título', 'Dt. Criação', 'Dt. Início', 'Dt. Fim', 'Tipo');

        if(!empty($filtros['gerar-excel'])) {
            ob_clean();
            $modelAviso->gerarExcel($sql, $cabecalho, 'Avisos');
            exit;
        }

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
//        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

        $listagem->setQuery($sql);
        $listagem->addAcao('edit', 'exibirAviso');
        $listagem->addAcao('view', 'exibirDestinatarios');
        $listagem->addAcao('delete', 'inativarAviso');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function visualizar($sisid = null, $filtros = array())
    {
        $sisid = $sisid ? $sisid : $_SESSION['sisid'];

        $modelAviso = new Sac_Model_Aviso();
        $sql = $modelAviso->listar($sisid, $filtros);

        $cabecalho = array('Código', 'Título', 'Dt. Criação', 'Dt. Início', 'Dt. Fim', 'Tipo');

        if(!empty($filtros['gerar-excel'])) {
            ob_clean();
            $modelAviso->gerarExcel($sql, $cabecalho, 'Avisos');
            exit;
        }

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
//        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

        $listagem->setQuery($sql);
        $listagem->addAcao('view', 'exibirDestinatarios');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function listarDestinatarios($aviid)
    {
        $modelAvisoDestinatario = new Sac_Model_AvisoDestinatario();
        $sql = $modelAvisoDestinatario->listar($aviid);
        $cabecalho = array('CPF', 'Nome', 'Status', 'Dt. Leitura');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
//        $listagem->turnOnPesquisator();

        $listagem->setQuery($sql);
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function verificarMensagens($usucpf = null)
    {
        $usucpf = $usucpf ? $usucpf : $_SESSION['usucpf'];

        $modelAviso = new Sac_Model_Aviso();
        $existeAviso = $modelAviso->existePorSistema($_SESSION['sisid']);

        if($existeAviso){
            $avisos = $modelAviso->recuperarAvisos($_SESSION['sisid'], $usucpf);

            foreach($avisos as $aviso){
                switch($aviso['avitipoexibicao']){
                    case Sac_Model_Aviso::K_EXIBIR_MODAL:
                        include APPRAIZ .'includes/sac/modulos/principal/aviso/modal.inc';
                        break;
                    case Sac_Model_Aviso::K_EXIBIR_TARJA:
                        include APPRAIZ .'includes/sac/modulos/principal/aviso/tarja.inc';
                        break;
                }
            }
        }
    }
    public function salvarArquivos($aviid,$arrFilesOld){
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        for ($i=0;$i<count($arrFilesOld['arqid_old']['name']);$i++){
            $arqid = 'arqid' . rand();

            $arrFiles = array();
            $arrFiles['name']     = $arrFilesOld['arqid_old']['name'][$i];
            $arrFiles['type']     = $arrFilesOld['arqid_old']['type'][$i];
            $arrFiles['tmp_name'] = $arrFilesOld['arqid_old']['tmp_name'][$i];
            $arrFiles['error']    = $arrFilesOld['arqid_old']['error'][$i];
            $arrFiles['size']     = $arrFilesOld['arqid_old']['size'][$i];
            $arrFiles['files']     = $arrFilesOld['arqid_old']['files'];

            $arquivosparts = explode(".", $arrFiles['name']);
            $avisoArquivo = new Sac_Model_AvisoArquivo();
            
            $_FILES[$arqid] = $arrFiles;
            
            $file = new FilesSimec("sac", null, "par3");
            $file->setUpload('Comunicação sac', $arqid, false);
            $arrDados['arqid'] = $file->getIdArquivo();
            if (!empty($arrDados['arqid'])){
                $avisoArquivo->arqid     = $arrDados['arqid'];
                $avisoArquivo->aviid     = $aviid;
                $avisoArquivo->avadsc    = $arquivosparts[0];
                $avisoArquivo->avastatus = "A";
                $avisoArquivo->salvar();
            }            
            
        }
        
    }
    /**
     * - downloadArquivo
     *
     * @return void.
     *
     */
    public function downloadArquivo($arqid) {
    	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    	$file = new FilesSimec("sac", null, "par3");
    	$file->getDownloadArquivo($arqid);
    }//end downloadArquivo()
}//end Class
?>
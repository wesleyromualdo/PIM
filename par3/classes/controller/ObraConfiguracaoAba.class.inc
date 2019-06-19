<?php
/**
 * Classe de controle do  obra_configuracao_aba
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-07-2017
 * @link     no link
 */



/**
 * Par3_Controller_ObraConfiguracaoAba
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-07-2017
 * @link     no link
 */
class Par3_Controller_ObraConfiguracaoAba
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ObraConfiguracaoAba($_REQUEST['ocaid']);
        $this->modelHistorico = new Par3_Model_ObraConfiguracaoAbaHistorico();
    }

    public function pegarSelectComboTipoFotos($otpid = null)
    {
        return $this->model->pegarSelectComboTipoFotos($otpid);
    }

    public function salvarConfFotos($arrPost)
    {
        $acao = Par3_Model_ObraConfiguracaoAbaHistorico::UPDATE;
        $arraySalvar['ocaid']               = $arrPost['ocaid'];
        $arraySalvar['ocastatus']           = 'A';
        $arraySalvar['ocaqtd_minimo']       = $arrPost['ocaqtd_minimo'] ? $arrPost['ocaqtd_minimo'] : 3;
        $arraySalvar['ocaqtd_maximo']       = $arrPost['ocaqtd_maximo'] ? $arrPost['ocaqtd_maximo'] : 30;
        $arraySalvar['ocatamanho_maximo']   = $arrPost['ocatamanho_maximo'] ? $arrPost['ocatamanho_maximo'] : 5;
        $arrPost['atfid'][0]                = $arrPost['atfid'][0] ? $arrPost['atfid'][0] : 'T';
        $arraySalvar['atfid']               = $arrPost['atfid'];
        //Valida campos
        $erro = $this->model->validarInput($arraySalvar);
        if($erro) return $erro;
        unset($arraySalvar['atfid']);
        try {
            $this->model->popularDadosObjeto($arraySalvar);
            $ocaid =  $this->model->salvar();
            $this->model->commit();

            $this->model->excluirTiposFoto($ocaid);
            if($arrPost['atfid'][0] == 'T'){
                $this->model->incluirTodosTiposFoto($ocaid);
            }else{
                foreach($arrPost['atfid'] as $aftid){
                    if($aftid != ''){
                        $this->model->incluirTipoFoto($ocaid, $aftid);
                    }
                }
            }

            $model = new Par3_Model_ObraConfiguracaoAba($ocaid);
            $this->modelHistorico->gravarHistorico($model,$acao);
            $this->modelHistorico->commit();

        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($ocaid) {
            return 'sucesso';
        }
        return 'erro';
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_ObraConfiguracaoAbaHistorico::CREATE;
        if($arrPost['ocaid']){
            $arraySalvar['ocaid'] = $arrPost['ocaid'];
            $acao = Par3_Model_ObraConfiguracaoAbaHistorico::UPDATE;
        }
        $arraySalvar['ocastatus']      = $arrPost['ocastatus'] ? $arrPost['ocastatus'] : 'A';
        $arraySalvar['oabid']          = $arrPost['oabid'];
        $arraySalvar['otpid']          = $arrPost['otpid'];
        //Valida campos
        try {
            $this->model->popularDadosObjeto($arraySalvar);
            $arraySalvar['ocaid'] =  $this->model->salvar();
            $this->model->commit();

            $model = new Par3_Model_ObraConfiguracaoAba($arraySalvar['ocaid']);
            $this->modelHistorico->gravarHistorico($model,$acao);
            $this->modelHistorico->commit();

            if(!$this->model->ocaqtd_minimo){
                $this->salvarConfFotos($arraySalvar);
            }
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($arraySalvar['ocaid']) {
            return $arraySalvar['ocaid'];
        }
        return 'erro';
    }

    public function formConfiguracao($arrPost)
    {
        global $disabled, $simec;
        $ctrObraTipo = new Par3_Controller_ObraTipo($this->model->otpid);
        switch($this->model->oabid){
        	case '4':
        	    $arrTipoFoto   = $this->pegarSelectComboTipoFotos($_REQUEST['otpid']);
        	    $arrTiposFoto  = $this->model->recuperaTiposFotoVinculados($_REQUEST['ocaid']);
                include APPRAIZ . 'par3/modulos/sistema/tabelaapoio/obras/configurar_aba/confFotos.inc';
        	    break;
        	case '7':
                include APPRAIZ . 'par3/modulos/sistema/tabelaapoio/obras/configurar_aba/confDocumentosAnexos.inc';
        	    break;
        }
    }

    public function listarDocumentos()
    {
        global $simec;

        $arrDocumentos = $this->model->retornarDocumentos($_REQUEST);

        include APPRAIZ . 'par3/modulos/sistema/tabelaapoio/obras/configurar_aba/listaConfDocumentosAnexos.inc';
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['id'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&id={$id}";
        $obra_configuracao_aba = new Par3_Model_ObraConfiguracaoAba($id);
        try{
             $obra_configuracao_aba->Xstatus = 'I';
             $obra_configuracao_aba->Xdtinativacao = date('Y-m-d H:i:s');
             $obra_configuracao_aba->Xusucpfinativacao = $_SESSION['usucpf'];

             $obra_configuracao_aba->salvar();
             $obra_configuracao_aba->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>
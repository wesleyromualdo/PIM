<?php
/**
 * Classe de controle do  acessoperfilabas
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   JEFERSON CARLOS ARAÚJO <jeferson.araujo@castgroup.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-06-2017
 * @link     no link
 */



/**
 * Spoemendas_Controller_Acessoperfilabas
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-06-2017
 * @link     no link
 */
class Spoemendas_Controller_Acessoperfilabas
{
    private $model;
    private $dados;
    private $apaid;
    private $flashMensagem;
    public function __construct($dados = null)
    {
        $this->flashMensagem = new Simec_Helper_FlashMessage('spoemendas/periodoreferencia');
        $this->dados = $dados;
        $this->model = new Spoemendas_Model_Acessoperfilabas($_GET['apaid']);
        
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
        $this->validaCampos($this->dados);
        $this->verificaExisteBanco($this->dados);
        $this->verificaPossuiAcessTodos($this->dados);
        $this->dados['apaativo'] = 'TRUE';
        $this->model->popularDadosObjeto($this->dados);
        $url .= '&apaid=' . $this->model->apaid;

        try{
            $apaid = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
            return $apaid;
    }

    /**
     * Verifica se os dados que estao vindo do formulario ja existe no banco de dados
     * @param $dados
     */
    public function verificaExisteBanco($dados)
    {
        if (!$dados['apaid']) {
            $result = $this->model->verificaExisteBancoModel($dados);
            if ($result) {
                $this->flashMensagem->addMensagem('Já existe um registro para esse perfil cadastrado!', 'danger');
                header("Location: spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Abas&novo=true");
                die;
            }
        }
    }

    /**
     * Verifica se possui acesso a todas formas de execucao
     * @param $dados
     */
    public function verificaPossuiAcessTodos($dados)
    {
        $result = $this->model->verificaPossuiAcessTodosModel($dados);
        if ($result && !$dados['apaid']) {
            foreach ($result as $dado) {
                if ($dado['etoid'] == EDIT_TODAS_FORMA_EXEC) {
                    $this->flashMensagem->addMensagem('Perfil ja possui acesso a todas as formas de execução!', 'danger');
                    header("Location: spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Abas&novo=true");
                    die;
                }
            }
        }
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($apaid)
    {
        try {
            $this->model->invativaAcessoAbasModel($apaid);
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            echo $e;
        }
    }

    /**
     * @return array|mixed|NULL
     */
    public function getPerfis()
    {
        return $this->model->getPerfisModel();
    }

    /**
     * @return array|mixed|NULL
     */
    public function getFormaExecucao()
    {
        return $this->model->getFormaExecucaoModel();
    }

    /**
     * @param $apaid
     * @return array|mixed|NULL
     */
    public function getPerfilAbasAcesso($apaid)
    {
        $dados = $this->model->getPerfilAbasAcessoModel($apaid);
        $abasById['apaid'] = $dados[0]['apaid'];
        $abasById['pflcod'] = $dados[0]['pflcod'];
        $abasById['etoid'] = $dados[0]['etoid'];
        if($dados) {
            foreach ($dados as $key => $dado) {
                $abasById[$dado['aacaba']] = $dado['aacaba'];
            }
            return $abasById;
        }
    }

    /**
     * retorna os dados filtrado de acordo com os dados passado na view
     * @return string
     */
    public function getPerfilAbasList($filtro){
        $where = '';
        if($filtro['pflcod']){
            $where .= "AND pflcod = {$filtro['pflcod']}";
        }
        if($filtro['etoid']){
            $where .= "AND etoid = {$filtro['etoid']}";
        }
       return $this->model->getPerfilAbasListModel($where);
    }

    /**
     * Retorna as permissoes que o perfil pode acessar na edição da emenda
     * @param $etoid
     * @param $formaExec
     * @return array
     */
    public function getPermissaoAbas($formaExec)
    {
        $perfis = pegaPerfilGeral();
        $dados = $this->model->getPermissaoAbasModel(implode(',', $perfis));
        if (empty($dados) || $dados == 'NULL') {
            return false;
        } else {
            foreach ($dados as $key => $dado) {
                if ($dado['etoid'] == $formaExec || $dado['etoid'] == EDIT_TODAS_FORMA_EXEC) {
                    $perfisReplace = str_replace(['{', '}'], '', $dados[$key]['perfis']);
                    $Arrays = explode(',', $perfisReplace);
                    return $Arrays;
                }
            }
            return false;
        }
    }
    /**
     * Retorna as origens editaveis que o perfil pode editar
     * @return array
     */
    public function getPermissaoFormasExecucao()
    {
        //Pega os perfis que o usuario possui
        $perfis = pegaPerfilGeral();
        //Busca as formas de execucao de acordo com o perfil
        $dados = $this->model->getPermissaoFormaExecucaoModel(implode(',',$perfis));
        $formasExec = str_replace(['{','}'],'',$dados[0]['formaexec']);
        $formaExecuArray = explode(',',$formasExec);
        //Verifica se usuario possui acesso a todos os tipos de execucao
        $formaTodos = array_search(EDIT_TODAS_FORMA_EXEC,$formaExecuArray);
        return ($formaTodos == EDIT_TODAS_FORMA_EXEC)?  EDIT_TODAS_FORMA_EXEC :  $formasExec;

    }

    /**
     * Valida os campos que são obrigatorios
     * @param $dados
     */
    private function validaCampos($dados)
    {
        $msg = null;
        if (empty($dados['pflcod'])) {
            $msg .= 'Perfil<br>';
        }
        if (empty($dados['etoid'])) {
            $msg .= 'Forma de Execução<br>';
        }
        if (empty($dados['iniciativa']
            || $dados['beneficiarios']
            || $dados['iniciativa-beneficiario']
            || $dados['subacoes']
            || $dados['impedimentos']
            || $dados['anexos-oficio'])) {
            $msg .= 'Abas.<br>';
        }
        if (!empty($msg) || !empty($msgAbas)) {
            $this->flashMensagem->addMensagem('Os seguintes campos são Obrigatórios: <br>' . $msg, 'danger');
            if ($dados['apaid']) {
                $url = '&apaid=' . $dados['apaid'];
            }
            header("Location: spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Abas&novo=true" . $url);
            die;
        }
    }

    /**
     * Retorna os perfis que existe cadastro com forma de execucao
     * @return array|mixed|NULL
     */
    public function getPerfisFormasExecExist($perfil)
    {
        $dados = $this->model->getPerfisFormasExecExistModel($perfil);
        return simec_json_encode($dados);
    }

    /**
     * Retorna os perfis que o usuario logado tem no perfil
     * @return string
     */
    public function getPerfisLogado()
    {
        $perfis = pegaPerfilGeral();
        return implode(',', $perfis);
    }

    /**
     * Permissão para editar emenda
     * @param $formaEx
     * @return bool
     */
    public function permissaoEditEmenda($formaEx)
    {
        $perfis = pegaPerfilGeral();
        $dados = $this->model->EditEmendaFormaExecModel(implode(',', $perfis));
        $formasExec = str_replace(['{', '}'], '', $dados[0]['etoid']);
        foreach (explode(',', $formasExec) as $etoid) {
            if ($etoid == $formaEx || $etoid == EDIT_TODAS_FORMA_EXEC) {
                    return true;
            }
        }
        return false;
    }

}

<?php
/**
 * Classe de controle do  periodopreenchimentoemenda
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   JEFERSON CARLOS ARAÚJO <jeferson.araujo@castgroup.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-06-2017
 * @link     no link
 */



/**
 * Spoemendas_Controller_Periodopreenchimentoemenda
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-06-2017
 * @link     no link
 */
class Spoemendas_Controller_Periodopreenchimentoemenda
{
    private $model;
    private $flashMensagem;

    public function __construct()
    {
        $this->flashMensagem = new Simec_Helper_FlashMessage('spoemendas/periodoreferencia');
        $this->model = new Spoemendas_Model_Periodopreenchimentoemenda($_GET['ppeid']);
        
    }

    /**
     * Salvar os dados na tabela
     * @param $dados
     * @return bool|int
     */
    public function salvar($dados)
    {

        $this->validaCamposObrigatorios($dados);
        $this->verificaPossuiBanco($dados);
        $dados['ppeativo'] = TRUE;
        $this->model->popularDadosObjeto($dados);
        try {
            $sucesso = $this->model->salvar(true,true,['ppemensagem']);
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar('', 'error');
        }
        $this->flashMensagem->addMensagem('Período cadastrado com sucesso!', 'success');
        return $sucesso;

    }

    /**
     * Verifica se os dados para cadastro ja existe no banco de dados
     * @param $dados
     */
    public function verificaPossuiBanco($dados)
    {
        if (empty($dados['ppeid'])) {
            $result = $this->model->ExistePeriodoCadastradoModel($dados);
            if ($result) {
                $this->flashMensagem->addMensagem('Já existe um registro cadastrado com essas informações', 'danger');
                header("Location: spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Periodo&novo=true" . $url);
                die;
            }
        }
    }

    /**
     * Inativa o registro do periodo de preenchimento
     * @param $ppeid
     */
    public function inativar($ppeid)
    {
           $result = $this->model->inativarPeriodoModel($ppeid);
            $this->model->commit();
            if($result) {
                $this->flashMensagem->addMensagem('Período removido com sucesso!', 'success');
            }
    }

    /**
     * Retornar as opcoes de resultado primario para formulario
     * @return array
     */
    public function getResultadoPrimario()
    {
        return [
            [
                'codigo' => '2',
                'descricao' => '2',
            ], [
                'codigo' => '6',
                'descricao' => '6',
            ],
            [
                'codigo' => '7',
                'descricao' => '7'
            ]
        ];
    }

    /**
     * Retorna os periodos cadastrados para a view os dados buscado da model
     * @param $ppeid
     * @return array|mixed|NULL
     */
    public function getPeriodoPreenchimento($ppeid)
    {
        if ($ppeid) {
            return $this->model->getPeriodoPreenchimentoModel($ppeid);
        }
    }

    private function validaCamposObrigatorios($dados)
    {
        $msg = null;
        if (empty($dados['pflcod'])) {
            $msg .= 'Perfil<br>';
        }
        if (empty($dados['etoid'])) {
            $msg .= 'Forma de Execução<br>';
        }
        if (empty($dados['pperesultprimario'])) {
            $msg .= 'Resultado Prímario<br>';
        }
        if (empty($dados['ppedatainicio'])) {
            $msg .= 'Período  <br>';
        }
        if (empty($dados['ppedatafim'])) {
            $msg .= 'Até <br>';
        }
        if(!empty($msg)){
            $this->flashMensagem->addMensagem('Os seguintes campos são Obrigatórios: <br>'.$msg, 'danger');
            if($dados['ppeid']){
                $url = '&ppeid='.$dados['ppeid'];
            }
            header("Location: spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Periodo&novo=true".$url);
            die;
        }
    }

    public function getPeriodosPreenchimento($filtro)
    {
        $where = '';
        if ($filtro['pflcod']) {
            $where .= "AND pflcod = {$filtro['pflcod']}";
        }
        if ($filtro['etoid']) {
            $where .= "AND etoid = {$filtro['etoid']}";
        }
        if ($filtro['pperesultprimario']) {
            $where .= "AND pperesultprimario = {$filtro['pperesultprimario']}";
        }
        return $this->model->getPeriodosPreenchimentoModel($where);
    }

    /**
     * retorna os dados de permissao de preenchimento para a view
     * @return array|mixed|NULL
     */
    public function getPermissaoPreenchimento()
    {
        $perfis = pegaPerfilGeral();
        $perfis = implode(",", $perfis);
        $dados = $this->model->getPermissaoPreenchimentoModel($perfis);
        return $this->geraPermissaoPeriodo($dados);
    }

    /**
     * Verifica se esta dentro do periodo de preenchimento e retorna quem pode ser editado
     * @param $dados
     * @return mixed
     */
    private function geraPermissaoPeriodo($dados)
    {
        $etoid = [];
        $pperesultprimario = [];
        $exercicio = [];
        if ($dados) {
            foreach ($dados as $dado) {
                $etoid[] = $dado['etoid'];
                $pperesultprimario[] = $dado['pperesultprimario'];
                $exercicio[] = $dado['prsano'];

            }
        }
        if (in_array(EDIT_TODAS_FORMA_EXEC, $etoid)) {
            $permissao['etoid'] = EDIT_TODAS_FORMA_EXEC;
        } else {
            $permissao['etoid'] = ($etoid) ? implode(",", $etoid) : '0';
        }
        $permissao['pperesultprimario'] = ($pperesultprimario) ? implode(",", $pperesultprimario) : '0';
        $permissao['prsano'] = ($exercicio) ? implode(",", $exercicio) : '0';


        return $permissao;
    }

    /**
     * Busca as mensagens que foram cadastradas no periodo
     * @return array|mixed|NULL
     */
    public function getMensagensPeriodo(){
        $perfis = pegaPerfilGeral();
        $perfis = implode(",", $perfis);
        return $this->model->getMensagensPeriodoModel($perfis);
    }

}
?>
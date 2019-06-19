<?php
/**
 * Classe de controle do  solicitacaofinanceirapedido
 *
 * @category Class
 * @package  A1
 * @version  $Id$
 * @author   SAULO ARAÚJO CORREIA <saulo.correia@castgroup.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 11-09-2017
 * @link     no link
 */


/**
 * Spoemendas_Controller_Solicitacaofinanceirapedido
 *
 * @category  Class
 * @package   A1
 * @author    <>
 * @license   GNU simec.mec.gov.br
 * @version   Release: 11-09-2017
 * @link      no link
 */
class Spoemendas_Controller_Solicitacaofinanceirapedido
{
    private $model;

    public function __construct ()
    {
        $this->model = new Spoemendas_Model_Solicitacaofinanceirapedido($_GET['sfpid']);
    }

    /**
     * Função gravar
     * - grava os dados
     *
     * @param                                     $sfnid
     * @param Spoemendas_Model_Periodosolicitacao $periodoAtual
     * @param                                     $dados
     */
    public function salvar ($sfnid, $periodoAtual, $dados)
    {
        if (empty($sfnid))
        {
            $this->model->sfpvalorpedido = $dados['sfpvalorpedido'];
        }
        else
        {
            $this->model->popularDadosObjeto($dados);
            $this->model->sfnid = $sfnid;
            $this->model->usucpf = $_SESSION['usucpf'];
            $this->model->prsid = $periodoAtual['prsid'];
        }

        $this->model->sfpvalorpedido = removeMascaraMoeda($this->model->sfpvalorpedido);

        if (empty($this->model->sfpvalorpedido))
        {
            $this->model->sfpvalorpedido = 0;
        }

        if ($this->model->sfpvalorpedido > 0)
        {
            $this->model->sfsid = Spoemendas_Model_Solicitacaofinanceirasituacao::PENDENTE_DE_AUTORIZACAO;
        }
        else
        {
            $this->model->sfsid = Spoemendas_Model_Solicitacaofinanceirasituacao::NAO_SOLICITADO;
        }

        $sucesso = $this->model->salvar();
        $this->model->commit();
    }

    public function inativarTodos ($sfnid)
    {
        $this->model->inativarTodos($sfnid);
        $this->model->commit();
    }
}

<?php

/**
 * Controle da funcionalidade categoriadespesa
 *
 * @author DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license GNU simec.mec.gov.br
 * @version $Id$
 */
class Par3_Controller_Categoriadespesa extends Controle
{
    /**
     * Variável para instância do modelo.
     *
     * @var object
     */
    protected $model;
    protected $modelHistorico;

    /**
     * Variável para guardar a url da funcionalidade.
     *
     * @var string
     */
    protected $_url;

    public function __construct()
    {
        $this->model = new Model_Categoriadespesa($_GET['ctdid']);
    }

    /**
     * Função responsável por salvar
     *
     * @author DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
     * @filesource
     * @throws Exception Mensagem de erro
     * @param array $dados Dados de assunto
     * @return bool Resultado da transação
     */
    public function salvar(array $dados)
    {

    }

    public function recuperaPorId($param)
    {
        try {
            $this->_modCategoriadespesa->carregarPorId($param);
            return (object) $this->_modCategoriadespesa->getDados();
        } catch (Exception $e) {
            simec_redirecionar($this->_url, 'error');
        }
    }

    public function lista($dados = null)
    {

    }
}


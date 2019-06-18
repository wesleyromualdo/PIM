<?php
/**
 * Implementação da classe base de services.
 * $Id: Abstract.php 96742 2015-04-29 17:43:52Z maykelbraz $
 */

/**
 * Classe de abstração de services do simec.
 * Implementa a base para as demais classes de service.
 *
 * @abstract
 */
abstract class Simec_Service_Abstract
{
    /**
     * Conjunto de dados que serão trabalhado na service.
     *
     * @var array
     */
    protected $dados;

    /**
     * Armazena as mensagens de retorno para o usuário.
     *
     * @var Simec_Helper_FlashMessage.
     */
    protected $flashMessage;

    /**
     * Seta os dados que serão usados na service.
     *
     * @param array $dados Dados tratados pela service.
     * @return \Simec_Service_Abstract
     */
    public function setDados(array $dados)
    {
        $this->dados = $dados;
        return $this;
    }

    /**
     * Retorna os dados usados na service.
     *
     * @return array
     */
    public function getDados()
    {
        return $this->dados;
    }

    public function setFlashMessage(Simec_Helper_FlashMessage $fm)
    {
        $this->flashMessage = $fm;
    }

    /**
     * Valida os dados carregados na service.
     * Para validações mais detalhadas, sobreescrever o método.
     *
     * @throws Exception Lançada qdo os dados estão vazios.
     */
    protected function validaDados()
    {
        if (empty($this->dados)) {
            throw new Exception('Não existem dados carregados.');
        }
        return $this;
    }
}

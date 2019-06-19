<?php
/**
 * Implementação de uma classe abstrata de fonte de dados para a Simec_Listagem.
 *
 * @version $Id: Datasource.php 109562 2016-03-30 17:46:57Z fellipesantos $
 * @filesource
 */

/**
 * Classe abstrata de fonte de dados.
 *
 * @package Simec\View\Listagem
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
abstract class Simec_Listagem_Datasource
{
    /**
     * @var mixed Fonte de dados.
     */
    protected $source = null;

    /**
     * @var int Número total de registros do datasource.
     */
    protected $totalRegistros = null;

    /**
     * @var int Número da página que deverá ser exibida.
     */
    protected $paginaAtual = 1;

    /**
     * @var int Número total de páginas contidas no datasource.
     */
    protected $totalPaginas;

    /**
     * @var bool Indica se os dados devem ser paginados.
     */
    protected $paginarDados = true;

    /**
     * @var Simec_Listagem_Config Configurações da listagem.
     */
    protected $config;

    /**
     * Constrói um novo datasource indicando o se há paginação.
     *
     * @param bool $paginarDados Indica se os dados do datasource devem sem páginados.
     */
    public function __construct($paginarDados = true)
    {
        $this->paginarDados = $paginarDados;
    }

    /**
     * Retorna a fonte de dados completa.
     *
     * @return string
     */
    public function getSource()
    {
        return $this->source;
    }

    /**
     * Define a página atualmente selecionada pelo usuário.
     *
     * @param int|string $pagina O número da página atual, ou 'all'.
     * @return \Simec_Listagem_Datasource
     */
    public function setPaginaAtual($pagina)
    {
        if (empty($pagina)) {
            $pagina = 1;
        }

        $this->paginaAtual = $pagina;
        return $this;
    }

    /**
     * Retorna a página atualmente selecionada pelo usuário.
     *
     * @return int|string
     */
    public function getPaginaAtual()
    {
        return $this->paginaAtual;
    }

    /**
     * Computa, se necessário, e retorna a quantidade de registros da fonte de dados.
     * @return int
     */
    public function getTotalRegistros()
    {
        if (is_null($this->totalRegistros)) {
            $this->totalRegistros = $this->contaRegistros();
        }

        return $this->totalRegistros;
    }

    /**
     * Computa, se necessário, e retorna a quantidade de páginas da fonte de dados.
     * @return int
     */
    public function getTotalPaginas()
    {
        if (is_null($this->totalPaginas)) {
            $this->totalPaginas = ceil($this->getTotalRegistros() / $this->config->getTamanhoPagina());
        }

        return $this->totalPaginas;
    }

    public function setConfig(Simec_Listagem_Config $config)
    {
        $this->config = $config;
        return $this;
    }

    /**
     * Verifica se a fonte de dados tem algum registro.
     *
     * @return bool
     */
    public function estaVazio()
    {
        return 0 === (int)$this->getTotalRegistros();
    }

    /**
     * Verifica se existe mais de uma página de dados.
     *
     * @return bool
     */
    public function paginar()
    {
        return $this->getTotalRegistros() > $this->config->getTamanhoPagina();
    }

    /**
     * Calcula e retorna o offset da página selecionada.
     *
     * @return int
     */
    protected function offset()
    {
        if($this->paginaAtual == "all")
            return 0;

        return (($this->paginaAtual - 1) * $this->config->getTamanhoPagina());
    }

    /**
     * Retorna o número do primeiro e do último registro exibido quando há paginação.
     * @return type
     */
    public function registrosExibidos()
    {
        // -- Considerando a contagem começando do 1 e não do 0.
        $regInicial = $this->offset() + 1;
        $regFinal = $this->offset() + $this->config->getTamanhoPagina();

        // -- Verificação para páginas finais incompletas
        $regFinal = ($regFinal > $this->totalRegistros)?$this->totalRegistros:$regFinal;

        return array($regInicial, $regFinal);
    }

    /**
     * Retorna a condição de paginação dos dados.
     *
     * @return bool
     * @uses Simec_Listagem_Datasource::$paginarDados
     */
    public function paginarDados()
    {
        return $this->paginarDados;
    }

    /**
     * Indica que os dados não devem ser paginados.
     */
    public function naoPaginar()
    {
        $this->paginarDados = false;
    }

    protected function validaCallbackOrdenacao()
    {
        return true;
    }

    protected function invocaCallbackOrdenacao()
    {
        return $this->source;
    }

    /**
     * Implemente a definição de uma fonte de dados e suas opções.
     *
     * @return \Simec_Listagem_Datasource
     */
    public abstract function setSource($source, array $opcoes = array());

    /**
     * Implementa a forma de retornar a query que originou o conjunto de dados.
     *
     * @returns string
     */
    public abstract function getQuery();

    /**
     * Esta é a função de retorno de dados da página atualmente selecionada.
     * Com base no offset e número de registros a serem retornados, a implmentação esta função deve
     * retornar apenas o conjunto de dados a serem exibidos na lista atual.
     *
     * @return mixed[] Lista de dados para exibição
     */
    public abstract function getDados();

    /**
     * Descobre a quantidade total de registros.
     *
     * @return int
     */
    protected abstract function contaRegistros();

    /**
     * Função de ordenação dos registros.
     */
    protected abstract function ordenarRegistros();
}

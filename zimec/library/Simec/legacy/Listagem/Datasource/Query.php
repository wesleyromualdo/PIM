<?php
/**
 * Arquivo de implementação do datasource da listagem do tipo Query.
 *
 * $Id: Query.php 111845 2016-06-16 18:18:17Z lucasgomes $
 * @filesource
 */

/**
 * Implementa uma fonte de dados do tipo query.
 *
 * @package Simec\View\Listagem\Datasource
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
class Simec_Listagem_Datasource_Query extends Simec_Listagem_Datasource
{
    /**
     * Valor máximo para o timeout da query no memcache.
     */
    const MAX_QUERY_TIMEOUT = 2592000;

    /**
     * @var \cls_banco Instância do banco de dados.
     */
    protected $db;

    /**
     * @var integer Define tempo de armazenamento em cache da query.
     * @see \cls_banco::carregar()
     */
    protected $queryTimeout = null;

    /**
     * Inicializa a base de dados.
     *
     * @global cls_banco $db Conexão com a base de dados.
     * @param bool $paginarDados Indica se os dados devem ser paginados.
     */
    public function __construct($paginarDados = true)
    {
        global $db;

        parent::__construct($paginarDados);
        $this->db = $db;
    }

    /**
     * Limpar a string da query.
     *
     * @param string $query Query utilizada para carregar os dados.
     */
    protected function clean(&$query)
    {
        $query = trim($query);
        // -- Removendo ';' no final da query para evitar problemas com o count
        if (';' == substr($query, -1)) {
            $query = substr($query, 0, -1);
        }
    }

    /**
     * Carrega a fonte de dados.
     *
     * Opções disponíveis:
     * timout: Timeout de cache da query.
     *
     * @param string $source Query de consulta dos dados.
     * @param array $opcoes Opções de configuração extra da fonte de dados.
     * @throws Exception
     */
    public function setSource($source, array $opcoes = array())
    {
        $this->clean($source);

        if (empty($source)) {
            throw new Exception('A query da lista não deve ser vazia.');
        }

        $this->source = $source;

        if (array_key_exists('timeout', $opcoes)) {
            if (self::MAX_QUERY_TIMEOUT < $opcoes['timeout']) {
                throw new Exception('O tempo de cache da query não deve exceder 2592000.');
            }
            $this->queryTimeout = $opcoes['timeout'];
        }
    }

    /**
     * Retorna a query que originou as linhas do array ($source).
     *
     * @return string
     */
    public function getQuery()
    {
        return $this->getSource();
    }

    /**
     * Retorna o conjunto de registros da página (ou todos os registros).
     *
     * @return array
     */
    public function getDados()
    {
        $query = $this->ordenarRegistros();

        // -- Todos os dados, sem restrições
        if (('all' == $this->getPaginaAtual()) || !$this->paginarDados()) {
            $query = $this->source;
        } else { // -- Apenas da página atual
            $offset = $this->offset();
            $limit = $this->config->getTamanhoPagina();
            $query = "{$query} OFFSET {$offset} LIMIT {$limit}";
        }
        $resultado = $this->db->carregar($query);
        //$resultado = $this->db->carregar($query, null, $this->queryTimeout);

        return $resultado?$resultado:array();
    }

    /**
     * Realiza a contagem dos registros atuais.
     *
     * @return int
     * @see Simec_Listagem_Datasource::getTotalRegistros()
     */
    protected function contaRegistros()
    {
        if (is_null($this->db)) {
            throw new Exception('Não foi informada a conexão com a base de dados para execução da query.');
        }

        $resultado = $this->db->pegaUm("SELECT COUNT(1) AS total FROM({$this->source}) contagem");
        return $resultado?$resultado:0;
    }

    protected function ordenarRegistros()
    {
        $infoOrdenacao = $this->config->ordenar();

        $infoOrdenacao = true;

        // -- Lista sem ordenação
        if (!$infoOrdenacao) {
            return $this->source;
        }

        // -- Lista com a ordenação padrão
        if (true === $infoOrdenacao) {
            list($campo, $sentido) = $this->config->getCampoOrdenado();

            // -- Se o sentido não for indicado, quer dizer que voltou à ordenação normal
            if (empty($campo)) {
                return $this->source;
            }

            return "SELECT * FROM ({$this->source}) tbfull ORDER BY {$campo} {$sentido}";
        }

        if (!$this->validaCallbackOrdenacao()) {
            throw new Exception('Função callback de ordenação');
        }

        return $this->invocaCallbackOrdenacao();
    }
}

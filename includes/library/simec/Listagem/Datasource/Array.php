<?php
/**
 * Arquivo de implementação do datasource da listagem do tipo Array.
 *
 * @version $Id: Array.php 112863 2016-08-18 12:48:41Z maykelbraz $
 * @filesource
 */

/**
 * Implementa uma fonte de dados do tipo array.
 *
 * Da mesma forma que antes da sua implementação, pode ser fornecido
 * um array com as linhas da lista, no entanto, agora é possível passar
 * um array de configuração, que permite, inclusive, emular a paginação.
 *
 * @package Simec\View\Listagem\Datasource
 * @see Simec\Listagem
 * @see Simec_Listagem_Datasource_Array::setSource()
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
class Simec_Listagem_Datasource_Array extends Simec_Listagem_Datasource
{
    /**
     * @var string Query que originou o array de dados.
     */
    protected $query;

    /**
     * @var bool Indica se o $source deve ser cortado, ou se ele tem apenas a qtd necessária de dados.
     */
    protected $slice = true;

    /**
     * Carrega a fonte de dados.
     *
     * O array $source pode ser tanto uma lista simples com as linhas do relatório, quanto o seguinte
     * conjunto de configurações:
     * dados: Lista de dados;
     * query: query que originou os dados - utilizado externamento;
     * registros: quantidade total de registros (ao informar, permite simular uma paginação);
     * pagina: a pagina solicitada pelo usuário - utilizado externamente.
     *
     * @param array $source Array de configuração ou de dados da listagem.
     * @param array $opcoes Opções extras de configuração do source.
     * @return \Simec_Listagem_Datasource_Array
     */
    public function setSource($source, array $opcoes = array())
    {
        if (!is_array($source)) {
            $this->source = array();
        } elseif (array_key_exists('dados', $source)) {
            $this->source = $source['dados']?$source['dados']:array();

            if (array_key_exists('query', $source)) {
                $this->query = $source['query'];
            }

            if (array_key_exists('registros', $source)) {
                $this->totalRegistros = $source['registros'];
                // -- Apenas uma página de registros foi retornada
                $this->slice = false;
            }

            if (array_key_exists('pagina', $source)) {
                $this->paginaAtual = $source['pagina'];
            }
        } else {
            $this->source = $source;
        }
        return $this;
    }

    /**
     * Retorna o conjunto de registros da página (ou todos os registros).
     *
     * @return array
     */
    public function getDados()
    {
        if (('all' == $this->getPaginaAtual())
            || (!$this->slice)
            || (!$this->paginarDados) // -- datasource array
        ) {
            return $this->source;
        }

        return array_slice($this->source, $this->offset(), $this->config->getTamanhoPagina());
    }

    /**
     * Retorna a query que originou as linhas do array ($source).
     *
     * @return string
     */
    public function getQuery()
    {
        if (empty($this->query)) {
            return 'QUERY NÃO DISPONÍVEL PARA ESTE CONJUNTO DE DADOS.';
        }

        return $this->query;
    }

    /**
     * Realiza a contagem dos registros atuais.
     *
     * @return int
     * @see Simec_Listagem_Datasource::getTotalRegistros()
     */
    protected function contaRegistros()
    {
        return count($this->source);
    }

    protected function ordenarRegistros()
    {

    }
}

<?php

/**
 * @version $Id$
 * Date: 20/11/2017
 * Time: 11:26
 */
class Simec_Job_Model extends Modelo
{
    /**
     * @var string
     */
    private $table;

    /**
     * @var array
     */
    private $dados;

    /**
     * @return mixed
     */
    public function getTable()
    {
        return $this->table;
    }

    /**
     * @param mixed $table
     *
     * @return Simec_Job_Model
     */
    public function setTable($table)
    {
        $this->table = $table;

        return $this;
    }

    /**
     * @return mixed
     */
    public function getDados()
    {
        return $this->dados;
    }

    /**
     * @param array $dados
     *
     * @return Simec_Job_Model
     */
    public function setDados($dados)
    {
        $this->dados = $dados;

        return $this;
    }

    /**
     * @param null $dados parametro opcional para passagem de dados
     * @param bool $forcarVazio
     *
     * @return int
     * @throws Exception
     */
    protected function jobInsert($dados = null, $forcarVazio = false)
    {
        try {
            $dados = (is_null($dados)) ? $this->getDados() : $dados;
            if (!is_array($dados) || empty($dados)) {
                return 0;
            }

            $campos = array_keys(get_object_vars($dados[0]));
            $colunas = implode(', ', $campos);
            $qtdColunas = count($campos);

            $inserts = "INSERT INTO {$this->getTable()} ({$colunas}) VALUES ";

            $count = 0;
            foreach ($dados as $dado) {
                $dado = get_object_vars($dado);

                if ($qtdColunas != count($dado)) {
                    if ($forcarVazio) {
                        // TODO : ARRUMAR PARA PREENCHER VAZIO A COLUNA QUE FALTA, NÃO A ULTIMA
                        while ($qtdColunas != count($dado)) {
                            $dado[] = '';
                        }
                    } else {
                        $print_dados = print_r($dado, true);

                        throw new Exception(
                            "Quantidade de atributos diferente da quantidade de colunas.<br>
                        Colunas esperadas
                        <pre>{$colunas}</pre>
                        Colunas recebidas
                        <pre>{$print_dados}</pre>"
                        );
                    }
                }

                $valores = array_map(
                    function ($val) {
                        if ($val === false) {
                            $val = 'f';
                        }

                        return addslashes($val);
                    },
                    $dado
                );


                $inserts .= "('" . implode("', '", $valores) . "'),";

                $count++;
            }

            $this->executar(rtrim($inserts, ','));

            return $count;
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param array $where
     * @param bool  $deleteAll
     *
     * @throws Exception
     * @example ["id = 1","status = 'A'"]
     */
    protected function jobDelete($where, $deleteAll = false)
    {
        if (!$deleteAll && empty($where)) {
            throw new Exception('Tentando deletar dados sem clausula where');
        }

        try {
            if (!empty($where)) {
                $where = " WHERE " . implode(' AND ', $where);
            } else {
                $where = '';
            }

            $this->executar("DELETE FROM {$this->getTable()} {$where}");
        } catch (Exception $e) {
            throw $e;
        }
    }


    /**
     * @param string $esquema
     * @param string $prefixo
     * @param string $tabela
     *
     * @return string
     */
    protected function nomeTabela($esquema, $prefixo = '', $tabela)
    {
        $tabela = strtolower($tabela);
        $tabela = "{$esquema}.{$prefixo}{$tabela}";

        $this->setTable($tabela);

        return $tabela;
    }

    protected function getTypecolumn($column)
    {
        $table = explode('.', $this->getTable());

        return $this->carregar(<<<SQL
                SELECT
                    data_type
                FROM
                    information_schema.columns
                WHERE
                    table_schema = '{$table[0]}'
                    AND table_name = '{$table[1]}'
                    AND column_name = '{$column}'
SQL
        );
    }
}
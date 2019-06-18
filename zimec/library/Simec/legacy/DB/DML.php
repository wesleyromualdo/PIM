<?php
/**
 * Implementa a classe Simec_DB_DML responsável por criar uma string
 * parametrizada e com os tratamentos necessários para execução no
 * objeto $db.
 * $Id: DML.php 88336 2014-10-13 18:29:00Z maykelbraz $
 */

/**
 * @todo Verificação de tipos no banco de dados
 * @todo Validação da estrutura da query
 * @todo Tratamento de null no meio do WHERE
 * @todo Tratar parametro repetido na query
 */
class Simec_DB_DML
{
    protected $dmlString = '';
    protected $parsedDmlString = '';
    protected $dmlParams = array();
    protected $params = array();
    protected $paramsForcedAsString = array();

    public function __construct($dml = null)
    {
        if (!is_null($dml) && is_string($dml)) {
            $this->setString($dml);
        }
    }

    public function setString($dml)
    {
        $this->checkEmptyDMLString($dml);
        $this->dmlString = $dml;
        $this->parsedDmlString = '';
        $this->params = array();
        $this->findDMLStringParams();
        return $this;
    }

    /**
     * Adiciona o valor de um parâmetro para a string DML carregada na classe.
     *
     * @param string $param Nome do parâmetro que receberá o valor.
     * @param mixed|scalar $value Valor para subistituição ns string.
     * @return \Simec_DB_DML
     */
    public function addParam($param, $value, $forceString = false)
    {
        $this->params[$param] = $value;

        // -- Independente do tipo, estes parametros devem ser tratados como string.
        if ($forceString) {
            $this->forceParamAsString($param);
        }

        $this->parsedDmlString = '';
        return $this;
    }

    /**
     *
     * @param array $params
     * @return \Simec_DB_DML
     * @throws Exception
     * @todo Adicionar um parametro para ignorar parametros que não estão na query
     */
    public function addParams(array $params)
    {
        foreach ($params as $paramName => $paramValue) {
            $paramNameType = gettype($paramName);
            if ('string' != $paramNameType) {
                throw new Exception("Identificador de parâmetro deve ser do tipo string, {$paramNameType} encontrado.");
            }
            $this->addParam($paramName, $paramValue);
        }
        return $this;
    }

    public function __toString()
    {
        $this->parseDMLString();
        return $this->parsedDmlString;
    }

    public function getParsedString()
    {
        return $this->parseDMLString();
    }

    protected function checkEmptyDMLString($dml)
    {
        if (empty($dml)) {
            throw new Exception('A string da query não pode ser vazia.');
        }
    }

    protected function checkDMLSTring()
    {
        // -- Verificando se algum dos parâmetros da query não tem um valor definido
        if ($missingParams = array_diff($this->dmlParams, array_keys($this->params))) {
            trigger_error(
                'Nem todos os parâmetros da string DML tiveram parâmetros definidos. Verifique os seguintes parâmetros: '
                . implode(',', $missingParams) . '.',
                E_USER_ERROR
            );
        }
    }

    /**
     * @todo tratamento de datas
     * @todo tratamento de null
     * @todo tramento de null em uma condição
     * @return type
     */
    protected function parseDMLString()
    {
        if (!empty($this->parsedDmlString)) {
            return $this->parsedDmlString;
        }

        // -- Verificando se a string pode ser parseada
        $this->checkDMLSTring();

        $dml = $this->dmlString;
        // -- Substituição dos placeholders
        foreach ($this->dmlParams as $param) {
            $valor = $this->params[$param];
            // -- Números com mascara monetária ou formatação de milhares

            // -- Antes de fazer as substituições na query, faz o escape dos '%' para eviar problemas com sprintf.
            $dml = str_replace('%', '%%', $dml);
            $operator = 'scalar';
            if (is_array($valor)) {
                list($valor, $flag, $operator) = $this->parseList($valor);
            } elseif (in_array($param, $this->paramsForcedAsString)) {
                $valor = $this->escapeString($valor);
                $flag = "'%s'";
            } elseif ($this->isFormatedNumericString($valor)) {
                list($valor, $flag) = $this->removeNumericMask($valor);
            } elseif (is_int($valor)) {
                $flag = '%d';
            } elseif (is_float($valor)) {
                $flag = '%f';
            } elseif (is_null($valor) || '' === $valor) {
                $flag = '%s';
                $valor = 'NULL';
            } else { // -- Considera como string se não achar um tipo correspondente antes
                $valor = str_replace("'", "''", $valor);
                $flag = "'%s'";
            }

            // -- substituíndo o nome do parametro por uma flag de tipo
            if ('scalar' == $operator) {
                $dml = preg_replace("/\:{$param}(?![a-z0-9])/", $flag, $dml, -1, $numSubstituicoes);
            } else {
                $dml = $this->replaceOperator($dml, $operator, $param);
                $dml = preg_replace("/\:{$param}(?![a-z0-9])/", $flag, $dml, -1, $numSubstituicoes);
            }
            // -- Substituíndo o valor na string DML
            $dml = vsprintf($dml, array_fill(0, $numSubstituicoes, $valor));
        }
        $this->parsedDmlString = $dml;
    }

    function removeDuplicatas($novoParam)
    {
        if (!in_array($novoParam, $this->dmlParams) && $novoParam != 'SS' && $novoParam != 'MI') {
            $this->dmlParams[] = $novoParam;
        }
    }

    protected function findDMLStringParams()
    {
        $this->dmlParams = array();

        // -- Sequencia de caracteres alfanumericos (incluíndo '_' e '-') precedidos por ':' e não precedidos por '::'.
        preg_match_all('|[^:]:([A-z0-9_-]+)|', $this->dmlString, $dmlParams);
        list(, $dmlParams) = $dmlParams;
        // -- Removendo duplicadas e inserindo em $this->dmlParams
        array_filter($dmlParams, array($this, 'removeDuplicatas'));
    }

    public function forceParamAsString($param)
    {
        if (!is_array($param)) {
            $this->paramsForcedAsString[] = $param;
        } else {
            foreach ($param as $par) {
                $this->paramsForcedAsString[] = $par;
            }
        }
        return $this;
    }

    /**
     * Identifica um valor como um número formatado e, portanto, uma string. Reconhece tanto formatação<br />
     * de moeda ('.'s e ','s). como formatação de número (apenas '.'s).
     * @param mixed $valor O valor para identificação do tipo.
     */
    protected function isFormatedNumericString($valor)
    {
        return (is_string($valor)
                && (1 == preg_match_all('/^(\d){1,3}(\.\d{3})*(,\d{2}){0,1}/', $valor, $_))
                && (current($_[0]) == $valor)); // -- O match foi identico ao que eu estava procurando
    }

    /**
     * Escapa as aspas em um valor do tipo string para evitar problemas de construção da DML.
     * @param string $valor
     * @return string
     */
    protected function escapeString($valor)
    {
        return str_replace(array("'", '\\'), array("''", ''), $valor);
    }

    /**
     * Converter uma string numérica para seu correspondente numérico (int ou double).
     * @param string $valor O valor para remover a máscara de real e converter para inteiro ou double.
     * @return array Valor convertido e flag de substituição.
     */
    protected function removeNumericMask($valor)
    {
        // -- Convertendo string numérica para float
        $valor = str_replace(array('.', ','), array('', '.'), $valor);
        if ((int)$valor == $valor) {
            $flag = '%d';
            $valor = (int)$valor;
        } else {
            $flag = '%f';
            $valor = (double)$valor;
        }
        return array($valor, $flag);
    }

    protected function parseList($valor)
    {
        $flag = "'%s'";
        $operator = 'scalar';
        if (empty($valor)) {
            $valor = '';
        } elseif (1 == count($valor)) {
            $valor = current($valor);
        } else {
            $valor = "'" . implode("', '", $valor) . "'";
            $flag = "%s";
            $operator = 'matriz';
        }

        return array($valor, $flag, $operator);
    }

    protected function replaceOperator($dml, $operator, $param)
    {
        // -- "/[\s]*[<>|!=|=]+[\s]*:{$param}/"
        switch ($operator) {
            case 'matriz':
                $dml = preg_replace(
                    array("/[\s]*(<>|!=)[\s]*:{$param}/", "/[\s]*=[\s]*:{$param}/"),
                    array(" NOT IN (:{$param}) ", " IN (:{$param}) "),
                    $dml
                );
                break;
        }
        return $dml;
    }

    public static function arrayToUpdate($array)
    {

    }

    public static function arrayToInsert($array)
    {

    }
}

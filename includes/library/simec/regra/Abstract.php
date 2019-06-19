<?php
/**
 * Classe cria funcionalidades basicas para validacao de premissa
 *
 * Descricao de uso da classe
 *
 * classe abstrata deve ser estendida por uma sub classe
 *
 * A sub classe deve implementar os metodos obrogatorios (metodos abstratos e interface)
 *
 * Todas as classe de validacao filhas de Simec_Regra_Abstract devem ser usadas para compor um agregador de regras
 *
 * Mas tambem podem ser usadas de forma individual como no exemplo que segue
 *
 * @see Simec_Regra_Agregador
 *
 * @package Simec\regra
 * @example
 * <code>
 * // -- Parametros de validacao para ser injetado dentro da classe
 * $params = array(
 *     'sisid' => '194',
 *     'usucpf' => ''
 * );
 *
 * // -- Objeto criado a partir da classe filha de Simec_Regra_Abstract
 * $objRegra = new Ted_Regra_Validacao();
 * // -- Inseri valores externos para serem validados dentro da classe
 * $objRegra->populaParams($params);
 *
 * // -- metodo premissa e implementado na classe filha a cargo do programador, ele deve ter o retorno booleano
 * if ($objRegra->premissa()) :
 *     echo 'Retornou verdade - regra validada';
 * else :
 *     echo 'Regra de validacao nao foi atendida';
 * endif;
 *
 * </code>
 *
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
abstract class Simec_Regra_Abstract implements Simec_Regra_Interface
{
    /**
     * Parametros para validacao
     * @var array
     */
    protected $params = [];

    /*
     * Regra de validacao
     */
    abstract public function premissa();

    /**
     * Retorna parametros de validacao
     */
    public function getParamList()
    {
        return $this->params;
    }

    /**
     * Verifica a validacao dos parametros da classe
     */
    public function validaParam()
    {}

    /**
     * @param $name
     * @return mixed
     * @throws Simec_Regra_Exception
     */
    public function __get($name)
    {
        if (key_exists($name, $this->params))
            return $this->params[$name];
        else
            throw new Simec_Regra_Exception("O parametro '{$name}' não foi definido na classe.");
    }

    /**
     * @param $name
     * @param $value
     * @throws Simec_Regra_Exception
     */
    public function __set($name, $value)
    {
        if (key_exists($name, $this->params)) {
            $this->params[$name] = $value;
        } else {
            throw new Simec_Regra_Exception("O parametro '{$name}' não foi definido na classe.");
        }
        return $this;
    }

    /**
     * @param $name
     * @return bool
     */
    public function __isset($name)
    {
        return isset($this->params[$name]);
    }

    /**
     * @param array $params
     * @return $this
     */
    public function populaParams(array $request)
    {
        foreach ($this->getParamList() as $chave => $valor) {
            try {
                if (key_exists($chave, $request)) {
                    $this->$chave = $request[$chave];
                }
            } catch (Simec_Regra_Exception $e) {
                continue;
            }
        }
        return $this;
    }
}
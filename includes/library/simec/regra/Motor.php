<?php
/**
 * Parse das classes de regras
 *
 * @package Simec\regra
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 * @example
 * <code>
 *
 * // -- Parametros para validacao
 * $params = ['usucpf' => '', '' => 'sisid' => 194];
 *
 * $agregador = new Ted_Regra_Agregador('*'); // -- verificar Simec_Regra_Agregador
 *
 * // -- Validando um conjunto de regras de um agregador de regras
 * if (Simec_Regra_Motor::check($agregador, $params)) {
 *     echo 'Validacao concluida sem pendencias';
 * } else {
 *     echo 'Validacao possui premissa falsas';
 * }
 *
 * </code>
 */
class Simec_Regra_Motor
{
    /**
     * @param Simec_Regra_Agregador $agregadorRegras
     * @param array $extraParams
     * @return bool
     */
    public static function check(Simec_Regra_Agregador $agregadorRegras, array $extraParams = [])
    {
        try {
            foreach ($agregadorRegras as $regras) {
                if (is_array($regras)) {
                    // -- Condição OR
                    $retorno = false;
                    foreach ($regras as $regra) {
                        if (Simec_Regra_Motor::validaRegra($regra, $extraParams)) {
                            $retorno = true;
                        }
                    }
                    if (!$retorno){
                        return false;
                    }
                } else {
                    // -- Condição AND
                    if (!Simec_Regra_Motor::validaRegra($regras, $extraParams)) {
                        return false;
                    }
                }
            }

            return true;
        } catch (Simec_Regra_Exception $e) {
            throw new Simec_Regra_Exception($e->getMessage(), $e->getCode());
        }
    }

    /**
     * @param $regra
     * @param $extraParams
     * @return bool
     */
    private final static function validaRegra($regra, $extraParams) {
        $rg = (new $regra);

        if (!empty($extraParams)) {
            $rg->populaParams($extraParams);
        }

        if (!$rg->premissa())
            return false;
        else
            return true;
    }
}
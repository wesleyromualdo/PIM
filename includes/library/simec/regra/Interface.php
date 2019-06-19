<?php
/**
 * Interface para implementacao de regras
 */
interface Simec_Regra_Interface
{
    /**
     * Retorna parametros de validacao
     * @return array
     */
    public function getParamList();

    /**
     * Verifica a validacao dos parametros da classe
     * @return bool
     */
    public function validaParam();

    /**
     * Carrega uma lista de parametro a ser validado
     * @return this
     */
    public function populaParams(array $request);
}
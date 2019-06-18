<?php
/**
 * Classe para tratar Exception das Regras
 */
class Simec_Regra_Exception extends Exception
{
    public function __construct($message = "", $code = 0, Throwable $previous = null)
    {
        parent::__construct($message, $code, $previous);
    }
}
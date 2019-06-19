<?php
namespace MecCoder;

/**
 * Classe definidora da sessão
 */
class Session
{

    public function __construct()
    {
        if (session_status() == PHP_SESSION_NONE) {
            session_start();
        }
    }

    /**
     * Retorna da sessão ou armazena dados na sessão
     * @param string $var nome da variável
     * @param mixed $val valor para armazenamento
     * @return mixed valor armazenado
     */
    public function session($var, $val = null)
    {
        if ($var) {
            if ($val) {
                return $_SESSION[$var] = $val;
            }
            if (isset($_SESSION[$var])) {
                return $_SESSION[$var];
            }
        }
    }
}

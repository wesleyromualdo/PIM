<?php
namespace App;

class Session extends \Illuminate\Support\Facades\Session
{

    /**
     * Integra a sessao do SIMEC com o laravel
     * @return array
     */
    static function integrar($bForce = false)
    {
        if (static::isStarted()) {
            return static::all();
        }
        if (!session_id()) {
            session_start();
        }
        $aSessao = $_SESSION;
        if (empty($aSessao['usucpf'])) {
            return array();
        }
        static::registrarLegado($aSessao);

        return static::all();
    }

    /**
     * Registra na sessao as variaveis em comum com o legado
     * @param array $aSessao
     */
    static function registrarLegado(array $aSessao)
    {
        foreach($_SESSION AS $key => $value) {
            static::put($key, $value);            
        }
    }
}

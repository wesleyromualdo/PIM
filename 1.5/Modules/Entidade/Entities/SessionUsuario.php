<?php
namespace Modules\Seguranca\Entities;

class SessionUsuario
{
    static $usucpf;

    static function getUsuCpf()
    {
        if (!static::$usucpf) {
            $aUser = static::parseLegado();
            if (!empty($aUser['usucpf'])) {
                static::$usucpf = $aUser['usucpf'];
            }
            $aUser = null;
        }
        return static::$usucpf;
    }

    /**
     * Efetua o parse da sessão do legado para a estrutura atual
     * @return array[]
     */
    static function parseLegado()
    {
        if (!session_id()) {
            session_start();
        }

        return $_SESSION;
    }
}

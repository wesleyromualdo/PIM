<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\PerfilBase;

class Perfil extends PerfilBase {




    public function scopeAtivo($query) {
        return $query->where('pflstatus', 'A');
    }

    public function pegaPerfis($usucpf, $sisid = null) {

        $sisid = empty($sisid) ? $_SESSION['sisid'] : $sisid;

        return $this->select('pfl.pflcod')
            ->from('seguranca.perfil as pfl')
            ->join('seguranca.perfilusuario as pfu', 'pfu.pflcod', '=', 'pfl.pflcod')
            ->join('seguranca.usuario as usu', 'usu.usucpf', '=', 'pfu.usucpf')
            ->where('pfl.sisid', '=', $sisid)
            ->where('usu.usucpf', '=', $usucpf)
            ->get();
    }

    public static function isExecutaQuerySimulacaoPerfil()
    {
        if (!empty($_SESSION['usucpf']) && $_SESSION['usucpf'] === $_SESSION['usucpforigem'])
        {
            return true;
        }
        return false;

    }
}
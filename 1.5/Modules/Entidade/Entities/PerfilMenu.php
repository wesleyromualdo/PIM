<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\PerfilMenuBase;

class PerfilMenu extends PerfilMenuBase {
  
    public function scopeAtivo($query) {
        return $query->where('pflstatus', 'A');
    }
    
    public function perfilSistemaByID($id) {
        return $query->where('mnuid', $id);
    }
}
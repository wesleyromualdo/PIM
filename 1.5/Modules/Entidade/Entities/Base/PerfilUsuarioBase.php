<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class PerfilUsuarioBase extends Model {

    protected $fillable = ['usucpf', 'pflcod'];
    protected $guarded = [];
    protected $table = 'seguranca.perfilusuario';
    protected $primaryKey = ['usucpf', 'pflcod'];
    public $timestamps = false;

    public function perfil() {
        return $this->hasOne('Modules\Seguranca\Entities\Perfil', 'pflcod', 'pflcod');
    }    
}

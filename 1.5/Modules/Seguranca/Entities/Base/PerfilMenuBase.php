<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class PerfilMenuBase extends Model {

    protected $fillable = [        
        'pflcod',
        'pmnstatus',
        'mnuid'
    ];    
    
    protected $guarded = ['pflcod'];
    protected $table = 'seguranca.perfilmenu';
    protected $primaryKey = 'pflcod';
    public $timestamps = false;
    
    public function menu() {
        return $this->belongsToMany('Modules\Seguranca\Entities\Menu');
    }
    
    public function perfil() {
        return $this->belongsToMany('Modules\Seguranca\Entities\Perfil');
    }
}
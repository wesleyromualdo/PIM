<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class TipoResponsabilidadePerfilBase extends Model {

    protected $fillable = [
        'trpcod',
        'tprcod',
        'pflcod',
        'sisid',
        'status'
    ];    

    protected $guarded = ['trpcod'];
    protected $table = 'seguranca.tiporesponsabilidade_perfil_geral';
    protected $primaryKey = 'trpcod';
     
    public function sistema() {
        return $this->belongsToMany('Modules\Seguranca\Entities\Sistema');
    }    
    
    public function tiporesponsabilidade() {
        return $this->belongsToMany('Modules\Seguranca\Entities\TipoResponsabilidade');
    }
    
    public function perfil() {
        return $this->belongsToMany('Modules\Seguranca\Entities\Perfil');
    }
}
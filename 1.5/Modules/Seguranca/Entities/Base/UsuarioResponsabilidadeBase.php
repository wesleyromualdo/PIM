<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class UsuarioResponsabilidadeBase extends Model {

    protected $fillable = [        
        'urcod',
        'usucpf',
        'trpcod',
        'status',
        'ur_data_inclusao',
        'responsabilidadecod'
    ];    

    protected $guarded = ['urcod'];
    protected $table = 'seguranca.usuarioresponsabilidade_geral';
    protected $primaryKey = 'urcod';
    //public $timestamps = false;
  
    public function usuario() {
        return $this->belongsToMany('Modules\Seguranca\Entities\Usuario');
    }
    
    public function tiporesponsabilidadeperfil() {
        return $this->belongsToMany('Modules\Seguranca\Entities\TipoResponsabilidadePerfil');
    }
}
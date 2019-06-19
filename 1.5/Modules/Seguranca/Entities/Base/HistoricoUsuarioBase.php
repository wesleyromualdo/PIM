<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class HistoricoUsuarioBase extends Model {   

    protected $fillable = [
        'htuid',
        'htudsc',
        'htudata',        
        'usucpf',
        'sisid',
        'suscod',
        'usucpfadm',
    ];
    protected $guarded = [];
    protected $table = 'seguranca.historicousuario';
    protected $primaryKey = 'htuid';
    public $timestamps = false;
    
    public $rules = [
        'sisid' => 'required',
        'usucpf' => 'required',
    ];
}
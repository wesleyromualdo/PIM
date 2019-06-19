<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class UsuarioSistemaBase extends Model {   

    protected $fillable = [
        'usucpf',
        'sisid',
        'susstatus',
        'pflcod',
        'susdataultacesso',
        'suscod',
    ];
    protected $guarded = [];
    protected $table = 'seguranca.usuario_sistema';
    protected $primaryKey = ['sisid', 'usucpf'];
    public $timestamps = false;
    public $autoincrement = false;
    public $incrementing = false;
    
    public $rules = [
        'suscod' => 'required',
        'pflcod' => 'required'
    ];
    
    public function messages()
    {
        return [
            'suscod.required' => 'O campo status é obrigatório.',
            'pflcod.required'  => 'O campo perfil é obrigatório',
        ];
    }
}
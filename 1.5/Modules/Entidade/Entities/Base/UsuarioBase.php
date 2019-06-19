<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class UsuarioBase extends Model {

    protected $fillable = ['usucpf',
        'regcod',
        'usunome',
        'usuemail',
        'usustatus',
        'usufoneddd',
        'usufonenum',
        'ususenha',
        'usudataultacesso',
        'usunivel',
        'usufuncao',
        'ususexo',
        'orgcod',
        'unicod',
        'usuchaveativacao',
        'usutentativas',
        'usuprgproposto',
        'usuacaproposto',
        'usuobs',
        'ungcod',
        'usudatainc',
        'usuconectado',
        'pflcod',
        'suscod',
        'usunomeguerra',
        'orgao',
        'muncod',
        'usudatanascimento',
        'usudataatualizacao',
        'entid',
        'tpocod',
        'carid',
        'arqidfoto'];
   
    protected $guarded = [];
    protected $primaryKey = 'usucpf';
    protected $table = 'seguranca.usuario';
    public $incrementing = false;
    public $timestamps = false;

    public $rules = [
        'usucpf' => 'required',
        'usunome' => 'required'
    ];

}
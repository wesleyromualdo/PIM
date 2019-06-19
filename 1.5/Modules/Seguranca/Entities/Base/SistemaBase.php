<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class SistemaBase extends Model {

    protected $fillable = [
        'sisid',
        'sisdsc',
        'sisurl',
        'sisabrev',
        'sisdiretorio',
        'sisfinalidade',
        'sisrelacionado',
        'sispublico',
        'sisstatus',
        'sisexercicio',
        'sismostra',
        'sisemail',
        'paginainicial',
        'sisarquivo',
        'sistel',
        'sisfax',
        'sisdtalteradados',
        'sissnalertaajuda',
        'usucpfanalista',
        'usucpfgestor',
        'sislayoutbootstrap',
        'sisdtcriacao',
        'sisordemmenu',
    ];
    
    protected $guarded = ['sisid', 'sisdtcriacao'];
    protected $table = 'seguranca.sistema';
    protected $primaryKey = 'sisid';
    public $timestamps = false;

    public $rules = [
        'sisid' => 'required',
    ];
}

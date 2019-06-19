<?php

namespace Modules\Publico\Entities;

use App\Entities\Model;

class UnidadeFederativa extends Model {

    protected $fillable = [
        'iduf',
        'idpais',
        'descricaouf',
        'regcod',
        'idufcapital',
        'codigoibgeuf'
    ];
    
    protected $guarded = [];
    protected $primaryKey = ['iduf'];
    protected $table = 'public.uf';
    public $incrementing = false;
    public $timestamps = false;

    public $rules = [];
    
}
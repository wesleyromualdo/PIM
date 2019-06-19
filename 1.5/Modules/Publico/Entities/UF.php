<?php

namespace App\Entities;

namespace Modules\Publico\Entities;

class UF extends Model {

    protected $fillable = ['iduf',
        'idpais',
        'descricaouf',
        'regcod',
        'idufcapital',
        'codigoibgeuf'];
    protected $guarded = [];
    protected $primaryKey = 'iduf';
    protected $table = 'public.municipio';
    public $timestamps = false;

    public function scopeAtivo($query) {
        return $query->where(with(new UF())->getTable() . '.st_registro_ativo', true);
    }

    public $rules = [
    ];

}

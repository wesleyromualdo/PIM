<?php

namespace Modules\Territorio\Entities;

use App\Entities\Model;

class Municipio extends Model {

    protected $fillable = [
        'muncod',
        'estuf',
        'miccod',
        'mescod',
        'mundescricao',
        'munprocesso',
        'muncodcompleto',
        'munmedlat',
        'munmedlog',
        'munhemis',
        'munaltitude',
        'munmedarea',
        'muncepmenor',
        'muncepmaior',
        'munmedraio',
        'munmerid',
        'muncodsiafi',
        'munpopulacao',
        'munpopulacaohomem',
        'munpopulacaomulher',
        'munpopulacaourbana',
        'munpopulacaorural'
    ];
    
    protected $guarded = [];
    protected $primaryKey = ['muncod', 'estuf'];
    protected $table = 'territorios.municipio';
    public $incrementing = false;
    public $timestamps = false;

    public $rules = [];
    
    public function estado() {
        return $this->belongsTo('\Modules\Territorio\Entities\Estado', 'estuf', 'estuf')->orderBy('estuf');
    }
    
    public function instrumentoUnidadeMun() {
        return $this->belongsTo('\Modules\Territorio\Entities\InstrumentoUnidade', 'muncod', 'muncod')->where('itrid', '=', 2)->orderBy('estuf');
    }

    public function scopeAtivo($query) {
        return $query->where(with(new Municipio())->getTable() . '.st_registro_ativo', true);
    }
}

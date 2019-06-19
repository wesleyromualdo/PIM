<?php

namespace Modules\Territorio\Entities;

use App\Entities\Model;

class Estado extends Model {

    protected $fillable = [
        'estuf',
        'muncodcapital',
        'regcod',
        'estcod',
        'estdescricao'
    ];
    
    protected $guarded = ['estuf'];
    protected $primaryKey = ['estuf'];
    protected $table = 'territorios.estado';
    public $incrementing = false;
    public $timestamps = false;

    public $rules = [
        
    ];
    
    public function municipios() {
        return $this->hasMany('\Modules\Territorio\Entities\Municipio', 'estuf', 'estuf')->orderBy('estuf');
    }
    
    public function instrumentoUnidadeUf() {
        return $this->belongsTo('\Modules\Territorio\Entities\InstrumentoUnidade', 'estuf', 'estuf')->where('itrid', '<>', 2)->orderBy('estuf');
    }
  
}

<?php

namespace Modules\Territorio\Entities;

use App\Entities\Model;

class InstrumentoUnidade extends Model {

    protected $fillable = [
        'inuid',
        'itrid',
        'estuf',
        'muncod',
        'docid',
        'inudescricao',
        'inustatus',
        'inusemconselhoeducacao',
        'inusiop'
    ];
    
    protected $guarded = ['inuid', 'muncod', 'estuf'];
    protected $primaryKey = ['inuid'];
    protected $table = 'territorios.instrumentounidade';
    public $incrementing = false;
    public $timestamps = false;

    public $rules = [
        
    ];
    
    public function scopeAtivo($query) {
        return $query->where('inustatus', 'A');
    }
    
    public function instrumento() {
        return $this->belongsTo('\Modules\Territorio\Entities\Instrumento', 'itrid', 'itrid');
    }
    
    public function estado() {
        return $this->hasOne('\Modules\Territorio\Entities\Estado', 'estuf', 'estuf')->orderBy('estuf');
    }
    
    public function municipio() {
        return $this->hasOne('\Modules\Territorio\Entities\Municipio', 'muncod', 'muncod')->orderBy('estuf');
    }
  
}

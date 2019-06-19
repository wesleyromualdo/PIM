<?php

namespace Modules\Territorio\Entities;

use App\Entities\Model;

class Instrumento extends Model {

    protected $fillable = [
        'itrid',
        'itrdsc'
    ];
    
    protected $guarded = ['itrid'];
    protected $primaryKey = ['itrid'];
    protected $table = 'territorios.instrumento';
    public $incrementing = false;
    public $timestamps = false;

    public $rules = [
        
    ];
    
    public function instrumentoUnidade() {
        return $this->hasMany('\Modules\Territorio\Entities\InstrumentoUnidade', 'itrid', 'itrid');
    }
  
}

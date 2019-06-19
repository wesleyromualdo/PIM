<?php

namespace Modules\Territorio\Entities;

use App\Entities\Model;

class Regiao extends Model {
    
    protected $fillable = [
        'regcod',
        'regdsc',
        'regdscregiaouf',
        'regcodgeo',
        'regstatus', 
        'regtipo'       
    ];
    
    protected $guarded = ['regcod'];
    protected $primaryKey = ['regcod'];
    protected $table = 'territorios.regiao';
    public $incrementing = false;
    public $timestamps = false;

    public $rules = [
        
    ];
  
}
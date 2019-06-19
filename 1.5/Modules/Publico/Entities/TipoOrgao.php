<?php

namespace Modules\Publico\Entities;

use App\Entities\Model;

class TipoOrgao extends Model {

    protected $fillable = [        
       'tpocod', 'tpodsc', 'tpostatus'
    ];
   
    protected $guarded = [];
    protected $primaryKey = ['tpocod'];
    public $incrementing = false;
    protected $table = 'public.tipoorgao';
    public $timestamps = false;

}

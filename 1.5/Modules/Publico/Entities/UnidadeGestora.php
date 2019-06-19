<?php

namespace Modules\Publico\Entities;

use App\Entities\Model;

class UnidadeGestora extends Model {

    protected $fillable = [        
       
       'ungcod',
       'ungdsc',
       'ungstatus',
       'unicod',
       'unitpocod', 
       'uniid', 
       'ungabrev', 
       'ungcodsetorialorcamentaria',
       'ungcodsetorialauditoria',
       'ungcodsetorialcontabil', 
       'ungcodsetorialfinanceira',
       'ungcodpolo',
       'ungdescentralfinancsit', 
       'orgcod',
       'ungcnpj',
       'ungendereco',
       'ungfone',
       'muncod',
       'ungemail', 
       'ungbairro', 
       'ungcep', 
       'ungnumddfone', 
       'ungnumddfax', 
       'ungfonefax', 
       'gescod', 
       'podelancarcredito', 
       'podeserugconcedente'
        
        
    ];
   
    protected $guarded = [];
    protected $primaryKey = ['ungcod'];
    //public $incrementing = false;
    protected $table = 'public.unidadegestora';
    public $timestamps = false;

}

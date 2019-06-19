<?php

namespace Modules\Publico\Entities;

use App\Entities\Model;

class UnidadeOrcamentaria extends Model {

    protected $fillable = [        
       'unicod',
       'unitpocod',
       'orgcod',
       'organo',
       'tpocod',
       'uniano',
       'unidsc',
       'unistatus',
       'uniid',
       'uniabrev',
       'gunid',
       'ungcodresponsavel',
       'gstcod',
       'orgcodsupervisor',
       'uniddd',
       'unitelefone',
       'uniemail',
       'unidataatualiza'
    ];
   
    protected $guarded = [];
    protected $primaryKey = ['unicod', 'unitpocod'];
    public $incrementing = false;
    protected $table = 'public.unidade';
    public $timestamps = false;

}

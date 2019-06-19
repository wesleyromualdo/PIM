<?php

namespace Modules\Publico\Entities;

use App\Entities\Model;

class Orgao extends Model {

    protected $fillable = [
       'orgcod',
       'organo',
       'tpocod',
       'orgdsc',
       'orgsigla',
       'orgstatus',
       'orgid',
       'ungcodsetorialfinanceira', 
       'ungcodsetorialorcamentaria',
       'ungc'
    ];
   
    protected $guarded = [];
    protected $primaryKey = ['orgcod'];
    public $incrementing = false;
    protected $table = 'public.orgao';
    public $timestamps = false;

}

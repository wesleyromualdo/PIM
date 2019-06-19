<?php

namespace Modules\Publico\Entities;

use App\Entities\Model;

class Cargo extends Model {

    protected $fillable = [
       'carid', 'cardsc'
    ];
    
    protected $guarded = [];
    protected $primaryKey = ['carid'];
    protected $table = 'public.cargo';    
    public $timestamps = false;

    public $rules = [];
    
}
<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class MenuBase extends Model {

    protected $fillable = [ 
        'mnucod',
        'mnucodpai', 
        'mnudsc', 
        'mnustatus', 
        'mnulink', 
        'mnutipo', 
        'mnustile', 
        'mnuhtml',
        'mnusnsubmenu',
        'mnutransacao', 
        'mnushow', 
        'abacod', 
        'mnuhelp',
        'sisid',
        'mnuid', 
        'mnuidpai', 
        'mnuordem', 
        'mnupaginainicial', 
        'mnuimagem',
        'constantevirtual',
    ];
    
    protected $guarded = ['mnuid'];
    protected $table = 'seguranca.menu';
    protected $primaryKey = 'mnuid';
    public $timestamps = false;
    
    
    public $rules = [
        'mnucod' => 'required',
        'mnudsc' => 'required'
    ]; 
}

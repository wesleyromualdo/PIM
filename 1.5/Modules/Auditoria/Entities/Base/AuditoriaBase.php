<?php

namespace Modules\Auditoria\Entities\Base;

use App\Entities\Model;

class AuditoriaBase extends Model {

    protected $fillable = [
        'usucpf', 'mnuid', 'audsql', 'audtabela', 'audtipo', 'audip', 'auddata', 'audmsg', 'sisid', 'audscript'
    ];

    protected $table = 'auditoria.auditoria_2019_01';
    protected $primaryKey = 'mnuid';
    public $timestamps = false;    
  
}

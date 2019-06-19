<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class TipoResponsabilidadeBase extends Model {

    protected $fillable = [        
        'tprcod',
        'tprdsc',
        'sisid',
        'tpr_join_schema',
        'tpr_join_table',
        'tpr_join_column',
        'tpr_join_table_column_dsc',
        'tpr_join_table_column_status',
        'status'
    ];    
    
    protected $guarded = ['tprcod'];
    protected $table = 'seguranca.tiporesponsabilidade_geral';
    protected $primaryKey = 'tprcod';


    public function sistema() {
        return $this->belongsToMany('Modules\Seguranca\Entities\Sistema');
    }    
//    public function menu() {
//        return $this->belongsToMany('Modules\Seguranca\Entities\Menu');
//    }
//    
//    public function perfil() {
//        return $this->belongsToMany('Modules\Seguranca\Entities\Perfil');
//    }
}
<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

class PerfilBase extends Model {

    protected $fillable = [
        'pflcod',
        'pfldsc',
        'pfldatainicio',
        'pfldatafim',
        'pflstatus',
        'pflresponsabilidade',
        'pflsncumulativo',
        'pflfinalidade',
        'pflnivel',
        'pfldescricao',
        'sisid',
        'pflsuperuser',
        'pflsuporte',
        'pflinddelegar',
        'pflpadrao',
        'modid',
        'constantevirtual'
    ];
    protected $guarded = ['pflcod'];
    protected $table = 'seguranca.perfil';
    protected $primaryKey = 'pflcod';
    public $timestamps = false;

    public $rules = [
        'pfldsc' => 'required|min:3|max:100',
        'pflnivel'=>'integer'
    ];

    public function sistema() {
        return $this->belongsToMany('Modules\Seguranca\Entities\Sistema');
    }

}

<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

abstract class RotaPerfilBase extends Model
{
    protected $table = 'seguranca.tb_rota_perfil';
    protected $primaryKey = ['pflcod','co_rota'];
    public $incrementing = false;
    protected $fillable = [
		'st_exclusao',
		'dt_cadastro',
		'dt_exclusao',
		'st_habilitado',
		'pflcod',
		'co_rota',
	];

    public $rules = [
		'pflcod' => 'required|integer',
		'co_rota' => 'required',
	];

    public $labels = [
		'st_exclusao' => '',
		'dt_cadastro' => '',
		'dt_exclusao' => '',
		'st_habilitado' => '',
		'pflcod' => '',
		'co_rota' => '',
	];

    public function rota() {
        return $this->belongsToMany('Modules\Seguranca\Entities\Rota');
    }

    public function perfil() {
        return $this->belongsToMany('Modules\Seguranca\Entities\Perfil');
    }
}
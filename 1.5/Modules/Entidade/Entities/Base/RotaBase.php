<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

abstract class RotaBase extends Model
{
    protected $table = 'seguranca.tb_rota';
    protected $primaryKey = 'co_rota';
    protected $fillable = [
		'sisid',
		'st_exclusao',
		'dt_exclusao',
		'dt_cadastro',
		'st_habilitado',
		'ds_rota_descricao',
		'ds_rota',
		'co_rota',
	];

    public $rules = [
		'sisid' => 'integer',
		'ds_rota_descricao' => 'required|string|max:255',
		'ds_rota' => 'required|string|max:255',
	];

    public $labels = [
		'sisid' => '',
		'st_exclusao' => '',
		'dt_exclusao' => '',
		'dt_cadastro' => '',
		'st_habilitado' => '',
		'ds_rota_descricao' => '',
		'ds_rota' => '',
	];
}
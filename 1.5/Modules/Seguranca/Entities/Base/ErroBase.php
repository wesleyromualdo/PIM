<?php

namespace Modules\Seguranca\Entities\Base;

use App\Entities\Model;

abstract class ErroBase extends Model
{
    protected $table = 'seguranca.erro';
    protected $primaryKey = 'errid';
    protected $fillable = [
		'errdescricaocompleta',
		'usucpf',
		'errlinha',
		'errarquivo',
		'errdescricao',
		'errtipo',
		'errdata',
		'sisid',
		'errid',
	];
    public $rules = [
		'errdescricaocompleta' => 'string',
		'usucpf' => 'string|max:15',
		'errlinha' => 'integer',
		'errarquivo' => 'string|max:300',
		'errdescricao' => 'string|max:4000',
		'errtipo' => 'required|string|max:2',
		'errdata' => 'required',
		'sisid' => 'integer',
	];
    public $labels = [
		'errdescricaocompleta' => '',
		'usucpf' => 'Usuário que gerou o erro',
		'errlinha' => 'Linha do arquivo que gerou o erro',
		'errarquivo' => 'Nome do arquivo que gerou o erro',
		'errdescricao' => 'Descrição do Erro',
		'errtipo' => 'Tipo do erro:
DB - Erro de Banco de dados (sintaxe, etc)
PR - Erro de Programação (expected array, etc)
QB - Queda no banco
WS - WebService
EN - Encoding no banco
PD - Erro na Conexão (possivelmente pdeinterativo)
DC - Diretório Cheio
AI - Arquivo inexistente
DV - Diversos',
		'errdata' => 'Data e hora do erro',
		'sisid' => 'Sistema que gerou o erro',
	];
}
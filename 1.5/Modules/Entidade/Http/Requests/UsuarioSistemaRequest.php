<?php

namespace Modules\Seguranca\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class UsuarioSistemaRequest extends FormRequest
{
    
    /* Regras de Validação */
    public function rules()
    {
        return [
            //'usucpf'=> 'required',
            'suscod' => 'required',
            //'htudsc' => 'required_unless:suscod,null',
            'pflcod' => 'required'
        ];
    }

    /* Personalização das mensagens de validação. */
    public function messages()
    {
        return [
            'suscod.required' => 'O campo status é obrigatório.',
            'htudsc.required_unless' => 'O campo justificativa é obrigatório.',
            'pflcod.required'  => 'O campo perfil é obrigatório',
        ];
    }


    public function authorize()
    {
        return true;
    }
}
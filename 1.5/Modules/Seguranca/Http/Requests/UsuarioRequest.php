<?php

namespace Modules\Seguranca\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class UsuarioRequest extends FormRequest
{
    
    /* Regras de Validação */
    public function rules()
    {
        return [
            'usucpf'=> 'required',
            'usunome'=> 'required',
            'usuemail'=> 'required',
        ];
    }

    /* Personalização das mensagens de validação. */
    public function messages()
    {
        return [
            'usucpf.required' => 'O campo Nome é obrigatório.',
            'usunome.required' => 'O campo Nome é obrigatório.',
            'usuemail.required'  => 'O campo E-mail é obrigatório',
        ];
    }


    public function authorize()
    {
        return true;
    }
}
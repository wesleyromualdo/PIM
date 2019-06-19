<?php

namespace Modules\Seguranca\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class PerfilRotaRequest extends FormRequest
{
    
    /* Regras de Validação */
    public function rules()
    {
        return [
            'pfldsc'=> 'required',
            'pflfinalidade'=> 'required',
            'pflnivel'=>'integer'
        ];
    }

    /* Personalização das mensagens de validação. */
    public function messages()
    {
        return [
            'pfldsc.required' => 'O campo Nome é obrigatório.',
            'pflfinalidade.required'  => 'O campo Descrição é obrigatório',
            'pflnivel.integer'  => 'O campo Nível precisa ser numérico',
        ];
    }


    public function authorize()
    {
        return true;
    }
}
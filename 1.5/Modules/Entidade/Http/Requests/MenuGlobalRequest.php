<?php

namespace Modules\Seguranca\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class MenuGlobalRequest extends FormRequest
{
    
    /* Regras de Validação */
    public function rules()
    {
        return [
            'mnucod'=> 'required',
            'mnutipo'=> 'required',
            'mnuidpai' => 'required_unless:mnutipo,0,1',
            'mnulink'=> 'required_unless:mnusnsubmenu,1',
            'mnudsc'=> 'required'
        ];
    }

    /* Personalização das mensagens de validação. */
    public function messages()
    {
        return [
            'mnucod.required' => 'O campo Código é obrigatório.',
            'mnutipo.required'  => 'O campo Tipo é obrigatório',
            'mnuidpai.required_unless'  => 'É necessário selecionar um menu pai',
            'mnulink.required_unless'  => 'O campo Link é obrigatório',
            'mnudsc.required'  => 'O campo Descrição é obrigatório',

        ];
    }


    public function authorize()
    {
        return true;
    }
}
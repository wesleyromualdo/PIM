<?php

namespace Modules\Seguranca\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class SistemaRequest extends FormRequest
{

    /* Regras de Validação */
    public function rules()
    {
        return [
            'sisdsc'=> 'required',
            'sisabrev'=>'required',
            'paginainicial'=>'required',
            'sisarquivo'=>'required',
            'sispublico'=>'required',
            'sisrelacionado'=>'required',
            'sisfinalidade'=>'required',
            'sisdiretorio'=>'required',
            'sisabrev'=>'required'
        ];
    }

    /* Personalização das mensagens de validação. */
    public function messages()
    {
        return [
            'sisdsc.required' => 'O campo Descrição é obrigatório.',
            'sisurl.required'  => 'O campo URL do Sistema é obrigatório',
            'sisabrev.integer'  => 'O campo Abreviação é obrigatório',
            'paginainicial.integer'  => 'O campo Página Inicial é obrigatório',
            'sisarquivo.integer'  => 'O campo Arquivo é obrigatório',
            'sispublico.integer'  => 'O campo Público Alvo é obrigatório',
            'sisrelacionado.integer'  => 'O campo Sistemas Relacionados é obrigatório',
            'sisfinalidade.integer'  => 'O campo Finalidade é obrigatório',
            'sisdiretorio.integer'  => 'O campo Diretório é obrigatório',
        ];
    }


    public function authorize()
    {
        return true;
    }
}
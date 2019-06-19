<?php

class Demanda_Controller_entidade extends Controle
{

    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função formPessoaJuridica
     * - monta o formulario de dados de pessoa juridica
     *
     * @return string escreve a lista.
     *
     */
    public function formPessoaJuridica($disabled = 'disabled', $objPessoaJuridica = null, $form = null, $display = array())
    {
        global $simec;
        
        $disabled = array(
            'enjcnpj'              => 'disabled'
        );

        if(!is_array($disabled)){
            $disabled = array(
            	'enjcnpj'              => $disabled,
            	'enjdsc'              => $disabled,
            	'enjrazaosocial'       => $disabled,
            	'enjinscricaoestadual' => $disabled,
            	'enjemail'             => $disabled,
            	'entsigla'             => $disabled,
            	'enjtelefone'          => $disabled,
            );
        }
        
        echo '<input type="hidden" name="enjid" value="'.$objPessoaJuridica->enjid.'"/>';
        echo '<input type="hidden" id="form" value="'.$form.'"/>';
        echo '<input type="hidden" name="enfid" value="1"/>';
        echo '<input type="hidden" name="enjcnpj_old" value="'.formatar_cnpj($objPessoaJuridica->enjcnpj).'"/>';
        if($display['enjcnpj'] != 'N') echo $simec->cnpj('enjcnpj', 'CNPJ', $objPessoaJuridica->enjcnpj, array('class' => 'cnpj', $disabled['enjcnpj'], 'required'));
        if($display['enjdsc'] != 'N') echo $simec->input('enjdsc', 'Nome', $objPessoaJuridica->enjdsc, array('maxlength' => '255', $disabled['enjdsc'], 'required'));
        if($display['enjrazaosocial'] != 'N') echo $simec->input('enjrazaosocial', 'Razão Social', $objPessoaJuridica->enjrazaosocial, array('maxlength' => '255', $disabled['enjrazaosocial'], 'required'));
        if($display['enjinscricaoestadual'] != 'N') echo $simec->input('enjinscricaoestadual', 'Inscrição Estadual', $objPessoaJuridica->enjinscricaoestadual, array('maxlength' => '255', $disabled['enjinscricaoestadual']));
        if($display['enjemail'] != 'N') echo $simec->email('enjemail', 'E-mail', $objPessoaJuridica->enjemail, array('class' => 'email', $disabled['enjemail'], 'required'));
        if($display['enjtelefone'] != 'N') echo $simec->telefone('enjtelefone', 'Telefone Comercial', $objPessoaJuridica->enjtelefone, array('class' => 'telefone', $disabled['enjtelefone'], 'required'));
    }//end formPessoaJuridica()


    /**
     * Função formSecretaria
     * - monta o formulario de dados da secretaria de educação
     *
     * @return string escreve a lista.
     *
     */
    public function formSecretaria($disabled = 'disabled', $objPessoaJuridica = null, $form = null)
    {
    	global $simec;

    	if(!is_array($disabled)){
    	    $disabled = array(
    	        'entcnpj'              => $disabled,
    	        'entnome'              => $disabled,
    	        'entemail'             => $disabled,
    	        'entsigla'             => $disabled,
    	        'enttelefonecomercial' => $disabled,
    	    );
    	}

    	echo '<input type="hidden" name="enjid" value="'.$objPessoaJuridica->enjid.'" />';
        echo '<input type="hidden" id="form" value="'.$form.'" />';
        echo '<input type="hidden" name="entcnpj_old" value="'.formatar_cnpj($objPessoaJuridica->entcnpj).'"/>';
        echo $simec->cnpj('entcnpj', 'CNPJ', $objPessoaJuridica->entcnpj, array('class' => 'cnpj', $disabled['entcnpj']));
    	echo $simec->input('entnome', 'Nome', $objPessoaJuridica->entnome, array('maxlength' => '255', $disabled['entnome'], 'required'));
    	echo $simec->email('entemail', 'E-mail', $objPessoaJuridica->entemail, array('class' => 'email', $disabled['entemail'], 'required'));
    	echo $simec->telefone('enttelefonecomercial', 'Telefone Comercial', $objPessoaJuridica->enttelefonecomercial, array('class' => 'telefone', 'required', $disabled['enttelefonecomercial']));
    }//end formSecretaria()


    /**
     * Função formPessoaFisica
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formPessoaFisica($disabled = 'disabled', $objPessoaFisica = null, $form = null, $display = array() )
    {
        global $simec;

        if(!is_array($disabled)){
            $disabled = array(
            	'enscpf' => $disabled,
            	'ensdsc' => $disabled,
            	'enssexo' => $disabled,
            	'ensemail' => $disabled,
            	'ensrg' => $disabled,
            	'ensorgexpedidor' => $disabled,
            	'ensdtnascimento' => $disabled,
            	'enstelefonecomercial' => $disabled,
            	'enstelefone' => $disabled,
            	'enstelefonefax' => $disabled,
            );
        }

        if($form == 'dirigente'){
            $required = 'required';
        }
        
        $radioSexo = array('f' => 'Feminino', 'm' => 'Masculino');

        echo '<input type="hidden" name="enjid" value="'.$objPessoaFisica->enjid.'"/>';
        echo '<input type="hidden" id="form" value="'.$form.'"/>';
        echo '<input type="hidden" name="enscpf_old" value="'.formatar_cpf($objPessoaFisica->enscpf).'"/>';
        if($display['enscpf'] != 'N') echo $disabled['enscpf'] != '' ? '<input type="hidden" name="enscpf" value="'.formatar_cpf($objPessoaFisica->enscpf).'"/>' : '';
        if($display['enscpf'] != 'N') echo $simec->cpf('enscpf', 'CPF', $objPessoaFisica->enscpf, array('class' => 'cpf', $disabled['enscpf'], 'data-pessoa' => true, ($objPessoaFisica->enscpf != '' ? 'disabled' : ''), 'data-pessoa-campos' => '{"#entnome": "no_pessoa_rf"}'));
        if($display['ensdsc'] != 'N') echo $simec->input('ensdsc', 'Nome', $objPessoaFisica->ensdsc, array('maxlength' => '255', $disabled['ensdsc'], 'required', 'readonly' => 'readonly'));
        if($display['enssexo'] != 'N') echo $simec->radio('enssexo', 'Sexo', $objPessoaFisica->enssexo ? strtolower($objPessoaFisica->enssexo) : '', $radioSexo, array($disabled['enssexo'], $required), array('type' => 'radio radio-info radio', 'style' => 'inline'));
        if($display['ensemail'] != 'N') echo $simec->email('ensemail', 'E-mail', $objPessoaFisica->ensemail, array('class' => 'email', $disabled['ensemail'], 'required'));
        if($display['ensrg'] != 'N') echo $simec->input('ensrg', 'Registro Geral (RG)', $objPessoaFisica->ensrg, array($disabled['ensrg'], 'required', 'maxlength' => '20'));
        if($display['ensorgexpedidor'] != 'N') echo $simec->input('ensorgexpedidor', 'Orgão Expedidor', $objPessoaFisica->ensorgexpedidor, array($disabled['ensorgexpedidor'], 'maxlength' => '20', 'required'));
        if($display['ensdtnascimento'] != 'N') echo $simec->data('ensdtnascimento', 'Data Nascimento', $objPessoaFisica->ensdtnascimento, array($disabled['ensdtnascimento'], $required));
        if($display['enstelefone'] != 'N') echo $simec->telefone('enstelefone', 'Telefone Comercial', $objPessoaFisica->enstelefone, array('class' => 'telefone', $disabled['enstelefone'], $required));
        echo '<script>';
        echo "$('[name=ensdtnascimento]').val('".formata_data($objPessoaFisica->ensdtnascimento)."');";
        echo '</script>';
    }//end formPessoaFisica()
    
    
    /**
     * Função formEnderecoEntidade
     * - monta o formulario de endereço de pessoa juridica ou fisica
     *
     * @return string escreve o formulario.
     *
     */
    public function formEnderecoEntidade($disabled = 'disabled', $objEndereco = null, $objmunicipio = [] ,$form = null)
    {
        global $simec;
        
        echo '<input type="hidden" name="enjid" value="'.$objEndereco->enjid.'"/>';
        echo $simec->cep('enjendcep', 'CEP', $objEndereco->enjendcep, array('class' => 'cep', 'onblur' => 'getEnderecoByCEP(this.value,\'1\');', $disabled, 'required'));
        echo $simec->input('enjendlogradouro', 'Logradouro', str_to_upper($objEndereco->enjendlogradouro), array('maxlength' => '255', $disabled, 'required'));
        echo $simec->input('enjendcomplemento', 'Complemento', str_to_upper($objEndereco->enjendcomplemento), array('maxlength' => '255', $disabled));
        echo $simec->input('enjendnumero', 'Número', $objEndereco->enjendnumero, array('maxlength' => '20', $disabled));
        echo $simec->input('enjendbairro', 'Bairro', str_to_upper($objEndereco->enjendbairro), array('maxlength' => '255', $disabled, 'required'));
                
        //echo $simec->input('muncod', 'Município', $objmunicipio['muncod'], array('maxlength' => '255', $disabled, 'required'));
    }


    /**
     * Função formPessoaFisica
     * - monta o formulario de dados de pessoa fisica
     *
     * @param string $disabled
     * @param string $form
     *
     * @return string escreve a lista.
     *
     */
    public function formPessoaFisicaSimples($disabled = 'disabled', $objPessoaFisica = null, $form = null)
    {
        global $simec;

        $radioSexo = array('f' => 'Feminino', 'm' => 'Masculino');

        echo '<input type="hidden" name="enjid" value="'.$objPessoaFisica->enjid.'"/>';
        echo '<input type="hidden" id="form" value="'.$form.'"/>';
        echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>';
        echo $simec->cpf('entcpf', 'CPF', $objPessoaFisica->entcpf, array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{".'.$form.'_ensdsc" : "no_pessoa_rf"}'));
        echo $simec->input('entnome', 'Nome', $objPessoaFisica->entnome, array('class' => $form.'_entnome', 'maxlength' => '255', $disabled, 'required', 'readonly' => 'readonly'));
        echo $simec->email('entemail', 'E-mail', $objPessoaFisica->entemail, array('class' => 'email', $disabled, 'required'));
    }//end formPessoaFisica()


    /**
     * Função formDirigente
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formDirigente($disabled = 'disabled', $objPessoaFisica = null, $arrEsconde = array())
    {
        global $simec;

        /*
         * @todo Verificar tabela que possui estas informações
         */
        $niveisEnsino = array(
            '1' => 'Ensino fundamental',
            '2' => 'Ensino médio',
            '3' => 'Ensino superior incompleto',
            '4' => 'Ensino superior completo',
            '5' => 'Especialização',
            '6' => 'Mestrado',
            '7' => 'Doutorado'
        );

        $radioBoolean = array('t' => 'Sim', 'f' => 'Não');
        if(!$arrEsconde['entdt_inicio_mandato']) echo $simec->data('entdt_inicio_mandato', 'Data de Nomeação','', array($disabled, 'required'));
        if(!$arrEsconde['entnivelensino']) echo $simec->select('entnivelensino', 'Escolaridade', $objPessoaFisica->entnivelensino, $niveisEnsino, array('maxlength' => '255', $disabled, 'required', 'display'=>'block'));
        if(!$arrEsconde['entempoatuacao']) echo $simec->input('entempoatuacao', 'Tempo de atuação', $objPessoaFisica->entempoatuacao, array('data-inputmask' => "'alias': 'integer','rightAlign':false", 'maxlength' => 5, $disabled, 'help' => 'Quanto tempo de atuação como dirigente de educação?', 'required'));
        if(!$arrEsconde['entcursomec']) echo $simec->radio('entcursomec', 'Curso de formação MEC', $objPessoaFisica->entcursomec, $radioBoolean, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline', 'required'));
        if(!$arrEsconde['entcursomecdescricao']) echo $simec->input('entcursomecdescricao', 'Qual curso?', $objPessoaFisica->entcursomecdescricao, array('maxlength' => '255', $disabled, 'required' => ($objPessoaFisica->entcursomec == 't' ? 'required' : '')), array('visible' => $objPessoaFisica->entcursomec == 't'));
        echo '<script>';
        echo "$('[name=entdt_inicio_mandato]').val('".formata_data($objPessoaFisica->entdt_inicio_mandato)."');";
        echo '</script>';
    }//end formDirigente()

    /**
     * Função formDirigente
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formSecretario($disabled = 'disabled', $objPessoaFisica = null, $arrEsconde = array())
    {
        global $simec;

        /*
         * @todo Verificar tabela que possui estas informações
         */
        $niveisEnsino = array(
            '1' => 'Ensino fundamental',
            '2' => 'Ensino médio',
            '3' => 'Ensino superior incompleto',
            '4' => 'Ensino superior completo',
            '5' => 'Especialização',
            '6' => 'Mestrado',
            '7' => 'Doutorado'
        );

        $radioBoolean = array('t' => 'Sim', 'f' => 'Não');

        if(!$arrEsconde['entnivelensino']) echo $simec->select('entnivelensino', 'Escolaridade', $objPessoaFisica->entnivelensino, $niveisEnsino, array('maxlength' => '255', $disabled, 'required', 'display'=>'block'));
        if(!$arrEsconde['entempoatuacao']) echo $simec->input('entempoatuacao', 'Tempo de atuação', $objPessoaFisica->entempoatuacao, array('maxlength' => '255', $disabled, 'help' => 'Quanto tempo de atuação como dirigente de educação?', 'required'));
        if(!$arrEsconde['entcursomec']) echo $simec->radio('entcursomec', 'Curso de formação MEC', $objPessoaFisica->entcursomec, $radioBoolean, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline', 'required'));
        if(!$arrEsconde['entcursomecdescricao']) echo $simec->input('entcursomecdescricao', 'Qual curso?', $objPessoaFisica->entcursomecdescricao, array('maxlength' => '255', $disabled, 'required' => ($objPessoaFisica->entcursomec == 't' ? 'required' : '')), array('visible' => $objPessoaFisica->entcursomec == 't'));

    }//end formDirigente()

    /**
     * Função formEnderecoenjidade
     * - monta o formulario de endereço de pessoa juridica ou fisica
     *
     * @return string escreve o formulario.
     *
     */
    public function formEnderecoenjidade($disabled = 'disabled', $objEndereco = null, $objmunicipio = [] ,$form = null)
    {
        global $simec;

        echo '<input type="hidden" name="endid" value="'.$objEndereco->endid.'"/>';
        echo $simec->cep('endcep', 'CEP', $objEndereco->endcep, array('class' => 'cep', 'onblur' => 'getEnderecoByCEP(this.value,\'1\');', $disabled, 'required'));
        echo $simec->input('enjendlogradouro', 'Logradouro', str_to_upper($objEndereco->enjendlogradouro), array('maxlength' => '255', $disabled, 'required'));
        echo $simec->input('endcomplemento', 'Complemento', str_to_upper($objEndereco->endcomplemento), array('maxlength' => '255', $disabled));
       	echo $simec->input('endnumero', 'Número', $objEndereco->endnumero, array('maxlength' => '20', $disabled));
        echo $simec->input('endbairro', 'Bairro', str_to_upper($objEndereco->endbairro), array('maxlength' => '255', $disabled, 'required'));

        $mundsc = null;

        if (!is_null($objmunicipio)) {
            if (is_array($objmunicipio) && array_key_exists('mundsc', $objmunicipio) && !empty($objmunicipio['mundsc'])) {
                $mundsc = $objmunicipio['mundsc'];
            }
        }

        echo $simec->input('endmunicipio', 'Município', $mundsc, array('maxlength' => '255', $disabled, 'required'));
    }

    /**
     * TODO
     *     - Solução temporaria para resolver o problema dos ID duplicados na mesma página para campos
     *     - que buscam endereço baseado no CEP e usam a função js getEnderecoByCEP
     */
    public function formEnderecoenjidadePresidente($disabled = 'disabled', $objEndereco = null, $form = null)
    {
        global $simec;

        echo '<input type="hidden" name="endid" value="'.$objEndereco->endid.'"/>';
        echo $simec->cep('endcep', 'CEP', $objEndereco->endcep, array('id' => 'endcep1', 'class' => 'cep', 'onblur' => 'getEnderecoByCEP1(this.value,\'1\');', $disabled, 'required'));
        echo $simec->input('enjendlogradouro', 'Logradouro', $objEndereco->enjendlogradouro, array('id' => 'enjendlogradouro1', 'maxlength' => '255', $disabled, 'required'));
        echo $simec->input('endcomplemento', 'Complemento', $objEndereco->endcomplemento, array('id' => 'endcomplemento1', 'maxlength' => '255', $disabled));
        echo $simec->input('endnumero', 'Número', $objEndereco->endnumero, array('id' => 'endnumero1', 'maxlength' => '20', $disabled, 'required'));
        echo $simec->input('endbairro', 'Bairro', $objEndereco->endbairro, array('id' => 'endbairro1', 'maxlength' => '255', $disabled, 'required'));
    }//end formEnderecoenjidade()


    /**
     * Função formConselho
     * - monta o formulario de dados do conselho
     *
     * @return string escreve a lista.
     *
     */
    public function formConselho($disabled = 'disabled', $objPessoaJuridica = null, $form = null)
    {
    	global $simec;

        echo '<input type="hidden" name="enjid" id="enjid" value="'.$objPessoaFisica->enjid.'"/>';
        echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>';
        echo '<input type="hidden" id="form" value="'.$form.'" />';
        echo $simec->cpf('entcpf', 'CPF', $objPessoaFisica->entcpf, array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{"#entnome": "no_pessoa_rf"}'), array('label-size' => 3, 'input-size' => 9));
        echo $simec->input('entnome', 'Nome', $objPessoaFisica->entnome, array('maxlength' => '255', $disabled, 'required', 'readonly' => 'readonly'), array('label-size' => 3, 'input-size' => 9));
        echo $simec->email('entemail', 'E-mail', $objPessoaFisica->entemail, array('class' => 'email', $disabled, 'required'), array('label-size' => 3, 'input-size' => 9));
        echo $simec->input('entatuacao', 'Atuação', $objPessoaFisica->entatuacao, array($disabled), array('label-size' => 3, 'input-size' => 9));
        echo $simec->input('entcargo', 'Cargo', $objPessoaFisica->entcargo, array($disabled), array('label-size' => 3, 'input-size' => 9));
    }//end formConselho()


}//end class

?>

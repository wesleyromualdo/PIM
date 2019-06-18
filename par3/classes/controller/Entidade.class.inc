<?php

/**
 * Classe de controle da entidade
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */

/**
 * Par_Controle_Entidade
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
class Par3_Controller_Entidade extends Controle
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

        if(!is_array($disabled)){
            $disabled = array(
            	'entcnpj'              => $disabled,
            	'entnome'              => $disabled,
            	'entrazaosocial'       => $disabled,
            	'entinscricaoestadual' => $disabled,
            	'entemail'             => $disabled,
            	'entsigla'             => $disabled,
            	'enttelefonecomercial' => $disabled,
            );
        }

        echo '<input type="hidden" name="entid" value="'.$objPessoaJuridica->entid.'"/>';
        echo '<input type="hidden" id="form" value="'.$form.'"/>';
        echo '<input type="hidden" name="entcnpj_old" value="'.formatar_cnpj($objPessoaJuridica->entcnpj).'"/>';
        if($display['entcnpj'] != 'N') echo $simec->cnpj('entcnpj', 'CNPJ', $objPessoaJuridica->entcnpj, array('class' => 'cnpj', $disabled['entcnpj'], 'required'));
        if($display['entnome'] != 'N') echo $simec->input('entnome', 'Nome', $objPessoaJuridica->entnome, array('maxlength' => '255', $disabled['entnome'], 'required'));
        if($display['entrazaosocial'] != 'N') echo $simec->input('entrazaosocial', 'Razão Social', $objPessoaJuridica->entrazaosocial, array('maxlength' => '255', $disabled['entrazaosocial'], 'required'));
        if($display['entinscricaoestadual'] != 'N') echo $simec->input('entinscricaoestadual', 'Inscrição Estadual', $objPessoaJuridica->entinscricaoestadual, array('maxlength' => '255', $disabled['entinscricaoestadual']));
        if($display['entemail'] != 'N') echo $simec->email('entemail', 'E-mail', $objPessoaJuridica->entemail, array('class' => 'email', $disabled['entemail'], 'required'));
        if($display['entsigla'] != 'N') echo $simec->input('entsigla', 'Sigla', $objPessoaJuridica->entsigla, array('maxlength' => '15', $disabled['entsigla']));
        if($display['enttelefonecomercial'] != 'N') echo $simec->telefone('enttelefonecomercial', 'Telefone Comercial', $objPessoaJuridica->enttelefonecomercial, array('class' => 'telefone', $disabled['enttelefonecomercial'], 'required'));
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

    	echo '<input type="hidden" name="entid" value="'.$objPessoaJuridica->entid.'" />';
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
            	'entcpf' => $disabled,
            	'entnome' => $disabled,
            	'entsexo' => $disabled,
            	'entemail' => $disabled,
            	'entrg' => $disabled,
            	'entorgexpedidor' => $disabled,
            	'entdtnascimento' => $disabled,
            	'enttelefonecomercial' => $disabled,
            	'enttelefonecelular' => $disabled,
            	'enttelefonefax' => $disabled,
            );
        }

        if($form == 'dirigente'){
            $required = 'required';
        }

        $radioSexo = array('f' => 'Feminino', 'm' => 'Masculino');

        echo '<input type="hidden" name="entid" value="'.$objPessoaFisica->entid.'"/>';
        echo '<input type="hidden" id="form" value="'.$form.'"/>';
        echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>';
        if($display['entcpf'] != 'N') echo $disabled['entcpf'] != '' ? '<input type="hidden" name="entcpf" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>' : '';
        if($display['entcpf'] != 'N') echo $simec->cpf('entcpf', 'CPF', $objPessoaFisica->entcpf, array('class' => 'cpf', $disabled['entcpf'], 'data-pessoa' => true, ($objPessoaFisica->entcpf != '' ? 'disabled' : ''), 'data-pessoa-campos' => '{"#entnome": "no_pessoa_rf"}'));
        if($display['entnome'] != 'N') echo $simec->input('entnome', 'Nome', $objPessoaFisica->entnome, array('maxlength' => '255', $disabled['entnome'], 'required', 'readonly' => 'readonly'));
        if($display['entsexo'] != 'N') echo $simec->radio('entsexo', 'Sexo', $objPessoaFisica->entsexo ? strtolower($objPessoaFisica->entsexo) : '', $radioSexo, array($disabled['entsexo'], $required), array('type' => 'radio radio-info radio', 'style' => 'inline'));
        if($display['entemail'] != 'N') echo $simec->email('entemail', 'E-mail', $objPessoaFisica->entemail, array('class' => 'email', $disabled['entemail'], 'required'));
        if($display['entrg'] != 'N') echo $simec->input('entrg', 'Registro Geral (RG)', $objPessoaFisica->entrg, array($disabled['entrg'], 'required', 'maxlength' => '20'));
        if($display['entorgexpedidor'] != 'N') echo $simec->input('entorgexpedidor', 'Orgão Expedidor', $objPessoaFisica->entorgexpedidor, array($disabled['entorgexpedidor'], 'maxlength' => '20', 'required'));
        if($display['entdtnascimento'] != 'N') echo $simec->data('entdtnascimento', 'Data Nascimento', $objPessoaFisica->entdtnascimento, array($disabled['entdtnascimento'], $required));
        if($display['enttelefonecomercial'] != 'N') echo $simec->telefone('enttelefonecomercial', 'Telefone Comercial', $objPessoaFisica->enttelefonecomercial, array('class' => 'telefone', 'required', $disabled['enttelefonecomercial']));
        if($display['enttelefonecelular'] != 'N') echo $simec->telefone('enttelefonecelular', 'Telefone Celular', $objPessoaFisica->enttelefonecelular, array('class' => 'telefone', $disabled['enttelefonecelular'], $required));
        if($display['enttelefonefax'] != 'N') echo $simec->telefone('enttelefonefax', 'Telefone Fax', $objPessoaFisica->enttelefonefax, array('class' => 'telefone', $disabled['enttelefonefax']));
        echo '<script>';
        echo "$('[name=entdtnascimento]').val('".formata_data($objPessoaFisica->entdtnascimento)."');";
        echo '</script>';
    }//end formPessoaFisica()


    /**
     * Função formPessoaFisica
     * - monta o formulario de dados de pessoa fisica
     *
     * @param string $disabled
     * @param obj $objPessoaFisica
     * @param string $form
     *
     * @return string escreve a lista.
     *
     */
    public function formPessoaFisicaSimples($disabled = 'disabled', $objPessoaFisica = null, $form = null)
    {
        global $simec;

        $radioSexo = array('f' => 'Feminino', 'm' => 'Masculino');

        echo '<input type="hidden" name="entid" value="'.$objPessoaFisica->entid.'"/>';
        echo '<input type="hidden" id="form" value="'.$form.'"/>';
        echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>';
        echo $simec->cpf('entcpf', 'CPF', $objPessoaFisica->entcpf, array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{".'.$form.'_entnome" : "no_pessoa_rf"}'));
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
     * Função formEnderecoEntidade
     * - monta o formulario de endereço de pessoa juridica ou fisica
     *
     * @return string escreve o formulario.
     *
     */
    public function formEnderecoEntidade($disabled = 'disabled', $objEndereco = null, $objmunicipio = [] ,$form = null)
    {
        global $simec;

        echo '<input type="hidden" name="endid" value="'.$objEndereco->endid.'"/>';
        echo $simec->cep('endcep', 'CEP', $objEndereco->endcep, array('class' => 'cep', 'onblur' => 'getEnderecoByCEP(this.value,\'1\');', $disabled, 'required'));
        echo $simec->input('endlogradouro', 'Logradouro', str_to_upper($objEndereco->endlogradouro), array('maxlength' => '255', $disabled, 'required'));
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
    public function formEnderecoEntidadePresidente($disabled = 'disabled', $objEndereco = null, $form = null)
    {
        global $simec;

        echo '<input type="hidden" name="endid" value="'.$objEndereco->endid.'"/>';
        echo $simec->cep('endcep', 'CEP', $objEndereco->endcep, array('id' => 'endcep1', 'class' => 'cep', 'onblur' => 'getEnderecoByCEP1(this.value,\'1\');', $disabled, 'required'));
        echo $simec->input('endlogradouro', 'Logradouro', $objEndereco->endlogradouro, array('id' => 'endlogradouro1', 'maxlength' => '255', $disabled, 'required'));
        echo $simec->input('endcomplemento', 'Complemento', $objEndereco->endcomplemento, array('id' => 'endcomplemento1', 'maxlength' => '255', $disabled));
        echo $simec->input('endnumero', 'Número', $objEndereco->endnumero, array('id' => 'endnumero1', 'maxlength' => '20', $disabled, 'required'));
        echo $simec->input('endbairro', 'Bairro', $objEndereco->endbairro, array('id' => 'endbairro1', 'maxlength' => '255', $disabled, 'required'));
    }//end formEnderecoEntidade()


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

        echo '<input type="hidden" name="entid" id="entid" value="'.$objPessoaFisica->entid.'"/>';
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

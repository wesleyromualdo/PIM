<?php 
global $simec;

?>
<form method="post" name="formulario-pessoa-fisica" id="formulario-pessoa-fisica" class="form form-horizontal">
    <input type="text" name="ddtid"   id="ddtid"  value="<?php echo $this->ddtid; ?>"/>
    <input type="text" name="ensid"   id="ensid"  value="<?php echo $this->ensid; ?>"/>
    <input type="text" name="enfid"   id="enfid"  value="<?php echo $this->enfid; ?>"/>
    <input type="text" name="ejfid"   id="ejfid"  value="<?php echo $this->ejfid; ?>"/>
    <input type="text" name="enrgestor"   id="enrgestor"  value="<?php echo $this->enrgestor; ?>"/>
    <input type="text" name="enrtipo"   id="enrtipo"  value="<?php echo $this->enrtipo; ?>"/>
    
    <input type="hidden" name="requisicao" value=""/>
    <div class="ibox-title">
    	<h3 id="entidade">Dados do(a) <?php echo $this->entidades;?></h3>
    </div>
    <div class="ibox-content"><?php
    $radioSexo = array('f' => 'Feminino', 'm' => 'Masculino');
    
    echo $disabled['enscpf'] != '' ? '<input type="hidden" name="enscpf" value="'.formatar_cpf($this->pessoa_fisica['enscpf']).'"/>' : '';
    echo $simec->cpf('enscpf', 'CPF', $this->pessoa_fisica['enscpf'], array('class' => 'cpf', $disabled['enscpf'], 'data-pessoa' => true, ($this->pessoa_fisica['enscpf'] != '' ? 'disabled' : ''), 'data-pessoa-campos' => '{"#ensdsc": "no_pessoa_rf"}'));
    echo $simec->input('ensdsc', 'Nome', $this->pessoa_fisica['ensdsc'], array('maxlength' => '255', $disabled['ensdsc'], 'required'));
    echo $simec->radio('enssexo', 'Sexo', $this->pessoa_fisica['enssexo'] ? strtolower($this->pessoa_fisica['enssexo']) : '', $radioSexo, array($disabled['enssexo'], $required), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    echo $simec->email('ensemail', 'E-mail', $this->pessoa_fisica['ensemail'], array('class' => 'email', $disabled['ensemail'], 'required'));
    echo $simec->input('ensrg', 'Registro Geral (RG)', $this->pessoa_fisica['ensrg'], array($disabled['ensrg'], 'required', 'maxlength' => '20'));
    echo $simec->input('ensorgexpedidor', 'Orgão Expedidor', $this->pessoa_fisica['ensorgexpedidor'], array($disabled['ensorgexpedidor'], 'maxlength' => '20', 'required'));
    echo $simec->data('ensdtnascimento', 'Data Nascimento', $this->pessoa_fisica['ensdtnascimento'], array($disabled['ensdtnascimento'], $required));
    echo $simec->telefone('enstelefone', 'Telefone Comercial', $this->pessoa_fisica['enstelefone'], array('class' => 'telefone', $disabled['enstelefone'], $required));
    ?></div>
    <div class="ibox-title">
    	<h3>Endereço da <?php echo $this->entidades;?></h3>
    </div>
    <div class="ibox-content"><?php 
    echo $simec->cep('enjendcep', 'CEP', $this->pessoa_fisica['enjendcep'], array('class' => 'cep', 'onblur' => 'getEnderecoByCEP(this.value,\'1\');', '', 'required'));
    echo $simec->input('enjendlogradouro', 'Logradouro', str_to_upper($this->pessoa_fisica['enjendlogradouro']), array('maxlength' => '255', '', 'required'));
    echo $simec->input('enjendcomplemento', 'Complemento', str_to_upper($this->pessoa_fisica['enjendcomplemento']), array('maxlength' => '255', ''));
    echo $simec->input('enjendnumero', 'Número', $this->pessoa_fisica['enjendnumero'], array('maxlength' => '20', ''));
    echo $simec->input('enjendbairro', 'Bairro', str_to_upper($this->pessoa_fisica['enjendbairro']), array('maxlength' => '255', '', 'required'));    
    ?></div>
</form>
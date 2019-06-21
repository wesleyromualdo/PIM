<?php 
global $simec;

?>
<form method="post" name="formulario-pessoa-juridica" id="formulario-pessoa-juridica" class="form form-horizontal" data-toggle="validator" role="form">
    <input type="hidden" name="ddtid"   id="ddtid"  value="<?php echo $this->ddtid; ?>"/>
    <input type="hidden" name="enjid"   id="enjid"  value="<?php echo $this->enjid; ?>"/>
    <input type="hidden" name="enfid"   id="enfid"  value="<?php echo $this->enfid; ?>"/>
    <input type="hidden" name="ejfid"   id="ejfid"  value="<?php echo $this->ejfid; ?>"/>
    <input type="hidden" name="ejfid_pai"   id="ejfid_pai"  value="<?php echo $this->demandante['ejfid']; ?>"/>
    
    <input type="hidden" name="requisicao" value=""/>
    <div class="ibox-title">
    	<h3 id="entidade">Dados do(a) <?php echo $this->entidades;?></h3>
    </div>
    <div class="ibox-content"><?php
    echo '<input type="hidden" name="enjcnpj_old" value="'.formatar_cnpj($this->pessoa_juridica['enjcnpj']).'"/>';
    echo $simec->cnpj('enjcnpj', 'CNPJ', formatar_cnpj($this->pessoa_juridica['enjcnpj']), array('class' => 'cnpj', ($this->pessoa_juridica['enjcnpj'] != '' ? 'disabled' : ''), 'required'));
    echo $simec->input('enjdsc', 'Nome', $this->pessoa_juridica['enjdsc'], array('maxlength' => '255', '', 'required'));
    echo $simec->input('enjrazaosocial', 'Razão Social', $this->pessoa_juridica['enjrazaosocial'], array('maxlength' => '255', '', 'required'));
    echo $simec->input('enjinscricaoestadual', 'Inscrição Estadual', $this->pessoa_juridica['enjinscricaoestadual'], array('maxlength' => '255', ''));
    echo $simec->email('enjemail', 'E-mail', $this->pessoa_juridica['enjemail'], array('class' => 'email', '', 'required'));
    echo $simec->telefone('enjtelefone', 'Telefone Comercial', $this->pessoa_juridica['enjtelefone'], array('class' => 'telefone', '', 'required'));
    ?></div>
    <div class="ibox-title">
    	<h3>Endereço do(a) <?php echo $this->entidades;?></h3>
    </div>
    <div class="ibox-content"><?php 
    echo $simec->cep('enjendcep', 'CEP', $this->pessoa_juridica['enjendcep'], array('class' => 'cep', 'onblur' => 'getEnderecoByCEP(this.value,\'1\');', '', 'required'));
    echo $simec->input('enjendlogradouro', 'Logradouro', str_to_upper($this->pessoa_juridica['enjendlogradouro']), array('maxlength' => '255', '', 'required'));
    echo $simec->input('enjendcomplemento', 'Complemento', str_to_upper($this->pessoa_juridica['enjendcomplemento']), array('maxlength' => '255', ''));
    echo $simec->input('enjendnumero', 'Número', $this->pessoa_juridica['enjendnumero'], array('maxlength' => '20', ''));
    echo $simec->input('enjendbairro', 'Bairro', str_to_upper($this->pessoa_juridica['enjendbairro']), array('maxlength' => '255', '', 'required'));    
    ?></div>
</form>
<script>
jQuery('#formulario-pessoa-juridica').validator();
</script>
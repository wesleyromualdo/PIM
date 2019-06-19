<?php
  global $simec;

?>

<div class="ibox">


    <div class="ibox-title">
          <h3 class="center">Cadastrar Programa</h3>
    </div>
    
    <div class="tabs-container">
      <ul class="nav nav-tabs nav-custom">
        <li>
          <a class="nav-link" href="demandas.php?modulo=programa/listar">Pesquisar</a>
        </li>
        <li class="active">
          <a class="nav-link" href="demandas.php?modulo=programa/cadastrar"><?php echo ($this->co_programa) ? 'Editar' : 'Cadastrar'; ?></a>
        </li>
      </ul>
    </div>


    <div class="ibox-content">
  
      <div class="row">
        <div class="col-lg-12">
          <form method="post" name="formulario-programa" id="formulario-programa">
            <input type="hidden" name="requisicao" value="salvarPrograma">
            <div class="row">
                <div class="col-lg-6">
                  <?php
                  
                  $arrAttrCod2 = array(
                    'readonly' => 'readonly',
                    'maxlength' => '20'
                  );
                  $arrAttrCod = array(
                    'placeholder' => 'Código do Serviço',
                    'maxlength' => '20'
                  );
                  $arrAttrSigla = array(
                    'placeholder' => 'Sigla',
                    'maxlength' => '10'
                  );
                  $arrAttrPrograma = array(
                    'placeholder' => 'Descrição',
                    'maxlength' => '255'
                  );
                  $arrAttrLink = array(
                    'placeholder' => 'Link',
                    'maxlength' => '255'
                  );
                  $arrAttrObjetivo = array(
                    'placeholder' => 'Objetivo',
                    'maxlength' => '255'
                  );
                  $arrAttrCombo = array(
                    'id' => 'enjid',
                    'multiple' => 'multiple'
                  );
                  if(isset($this->co_programa))
                    echo $simec->input( 'co_programa', 'ID do programa', $this->co_programa, $arrAttrCod2, array('label-size' => '4') );
                  
                  echo $simec->input( 'nu_codigo_programa', 'Código do Programa', $this->nu_codigo_programa, $arrAttrCod, array('label-size' => '4') );
                  echo $simec->input( 'ds_sigla', 'Sigla', $this->ds_sigla, $arrAttrSigla, array('label-size' => '4') );
                  echo $simec->input( 'ds_programa', 'Descrição do Programa', $this->ds_programa, $arrAttrPrograma, array('label-size' => '4') );
                  echo $simec->input( 'ds_link', 'Link', $this->ds_link, $arrAttrLink, array('label-size' => '4') );
                  echo $simec->textArea( 'ds_objetivo', 'Objetivo', $this->ds_objetivo, $arrAttrObjetivo, array('label-size' => '4') );
                                    
                  echo $simec->select('enjid[]', 'Secretaria', $this->secretarias, $this->sql_secretarias, $attrCombo, array('label-size' => '4', 'input-size' => '8'));
                  ?>
                </div>
            </div>
          </form>
        </div>
    </div>
	</div>
	
	<div class="ibox-footer">
    <div class="text-center">
      <button type="button" class="btn btn-success btn-voltar"><i class="fa fa-arrow-left"></i> Voltar</button>
      <button type="reset" class="btn btn-primary btn-salvar"><i class="fa fa-save"></i> Salvar</button>
    </div>
  </div>
</div>

<script>
$(document).ready(function() {
  $('.btn-salvar').click(function() {

    var ds_sigla = $('#ds_sigla').val();
    var ds_programa = $('#ds_programa').val();
    var nu_codigo_programa = $('#nu_codigo_programa').val();

    if(ds_sigla == '' || ds_programa == '' || nu_codigo_programa == '')
      return;

    $('#formulario-programa').submit();
  });


  jQuery('.btn-voltar').click(function(){
      window.location='<?=$_SERVER["HTTP_REFERER"]?>'
  }); 

});
</script>
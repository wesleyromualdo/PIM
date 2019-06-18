<?php
  global $simec;

?>

<div class="ibox">

    <div class="ibox-title">
          <h3 class="center">Unidade de Medida</h3>
    </div>

    <div class="tabs-container">
      <ul class="nav nav-tabs nav-custom">
        <li class="active">
          <a class="nav-link" href="demandas.php?modulo=unidadeMedida/cadastrar"><?php echo ($this->co_unidade_medida) ? 'Editar' : 'Cadastrar'; ?></a>
        </li>
      </ul>
    </div>
    
    <div class="ibox-content">
      <div class="row">
        <div class="col-lg-12">
          <form method="post" name="formulario-unidade-medida" id="formulario-unidade-medida">
            <input type="hidden" name="requisicao" value="salvarUnidadeMedida">
            <div class="row">
                <div class="col-lg-6">
                  <?php
                  
                  $arrAttrSigla = array(
                    'placeholder' => 'Sigla da Unidade de Medida',
                    'maxlength' => '20'
                  );
                  $arrAttrDesc = array(
                    'placeholder' => 'Descrição da Unidade de Medida',
                    'maxlength' => '200'
                  );

                  echo $simec->input( 'ds_sigla', 'Sigla', $this->ds_sigla, $arrAttrSigla, array('label-size' => '4') );
                  echo $simec->input( 'ds_unidade_medida', 'Descrição', $this->ds_unidade_medida, $arrAttrDesc, array('label-size' => '4') );
                  ?>
                </div>
            </div>
          </form>
        </div>
    </div>
	</div>
	
	<div class="ibox-footer">
    <div class="text-center">
      <button type="button" onclick="window.location='demandas.php?modulo=unidadeMedida/listar';" class="btn btn-primary"><i class="fa fa-arrow-left"></i> Voltar</button>
      <button type="button" id="salvar" class="btn btn-success"><i class="fa fa-save text-white"></i> Salvar</button>
    </div>
  </div>
</div>

<script>
$(document).ready(function() {
  $('#salvar').click(function() {
    $('#formulario-unidade-medida').submit();
  });
});
</script>
<?php
  global $simec;

?>

<div class="ibox">


    <div class="ibox-title">
          <h3 class="center">Cadastrar Serviço</h3>
    </div>
    
    <div class="tabs-container">
      <ul class="nav nav-tabs nav-custom">
        <li>
          <a class="nav-link" href="demandas.php?modulo=servico/listar">Pesquisar</a>
        </li>
        <li class="active">
          <a class="nav-link" href="demandas.php?modulo=servico/cadastrar"><?php echo ($this->co_servico) ? 'Editar' : 'Cadastrar'; ?></a>
        </li>
      </ul>
    </div>


    <div class="ibox-content">
  
      <div class="row">
        <div class="col-lg-12">
          <form method="post" name="formulario-servico" id="formulario-servico">
            <input type="hidden" name="requisicao" value="salvarServico">
            <div class="row">
                <div class="col-lg-6">
                  <?php
                  
                    $arrAttrCod = array(
                      'placeholder' => 'Código do Serviço',
                      'maxlength' => '20'
                    );
                    $arrAttrDesc = array(
                      'placeholder' => 'Descrição do Serviço',
                      'maxlength' => '255'
                    );
                    $arrAttrCombo = array(
                      'id' => 'co_unidade_medida'
                    );
                    $arrAttrCombo2 = array(
                      'id' => 'co_grupo_servico'
                    );

                    echo $simec->input( 'nu_codigo_servico', 'Código do Serviço', $this->nu_codigo_servico, $arrAttrCod, array('label-size' => '4') );
                    echo $simec->input( 'ds_servico', 'Descrição do Serviço', $this->ds_servico, $arrAttrDesc, array('label-size' => '4') );
                    echo $simec->select('co_unidade_medida', 'Unidade de Medida', $this->co_unidade_medida, $this->sql_unidade_medida, $arrAttrCombo, array('label-size' => '4', 'input-size' => '8'));
                    echo $simec->select('co_grupo_servico', 'Grupo do Serviço', $this->co_grupo_servico, $this->sql_grupo_servico, $arrAttrCombo2, array('label-size' => '4', 'input-size' => '8'));

                    if (!empty($this->co_servico)) {
                        echo $simec->radio("co_status", "Situação", $this->co_status, array('1' => 'Ativo', '0' => 'Inativo'), array('id' => 'co_status'), array('label-size' => '4', 'input-size' => '8'));
                    }
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
    var um = $('#co_unidade_medida :selected').val();
    if(um == '')
      return;
    var es = $('#co_grupo_servico :selected').val();
    if(es == '')
      return;
    $('#formulario-servico').submit();
  });


  jQuery('.btn-voltar').click(function(){
      window.location='<?=$_SERVER["HTTP_REFERER"]?>'
  }); 

});
</script>
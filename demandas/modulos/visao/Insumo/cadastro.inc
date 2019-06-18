<?php
  global $simec;
?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Grupo Insumo</h3>
  </div>
  <div class="ibox-content">
    <ul class="nav nav-tabs nav-custom">
      <li>
        <a href="demandas.php?modulo=insumo/listar">Lista</a>
      </li>
      <li class="active">
        <a href="demandas.php?modulo=insumo/cadastrar"><?php echo ($this->co_grupo_insumo) ? 'Editar' : 'Cadastro'; ?></a>
      </li>
    </ul>
    <br/>
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-insumo" id="formulario-insumo">
          <input type="hidden" name="requisicao" value="salvarInsumo">
          <div class="row">
            <div class="col-lg-6">
                <?php
                $display_piso_insumo = 'none';

                $arrAttr = array (
                    'placeHolder' => 'Código do Insumo',
                    'maxlength' => '60',
                    'autocomplete' => 'off'
                );

                $arrAttrDsInsumo = array (
                    'placeHolder' => 'Nome do Insumo',
                    'maxlength' => '255',
                    'autocomplete' => 'off'
                );

                echo $simec->input ( 'nu_codigo_insumo', 'Código do Insumo', $this->nu_codigo_insumo, $arrAttr, array('label-size' => '4') );
                echo $simec->input ( 'ds_insumo', 'Insumo', $this->ds_insumo, $arrAttrDsInsumo, array('label-size' => '4') );

                echo $simec->select('co_unidade_medida', 'Unidade de medida', $this->co_unidade_medida, $this->sql_unidade_medida, $attrCombo, array('label-size' => '4', 'input-size' => '8'));
                echo $simec->select('co_tipo_insumo', 'Tipo Insumo', $this->co_tipo_insumo, $this->sql_tipo_insumo, array('id' => 'co_tipo_insumo'), array('label-size' => '4', 'input-size' => '8'));

                if($this->co_tipo_insumo && ($this->co_tipo_insumo == 1)) {
                  $display_piso_insumo = 'show';
                }
                ?>
                <div id="div_piso_insumo"  style="display: <?php echo $display_piso_insumo ?>">
                  <?php
                    echo $simec->select('co_piso_insumo', 'Piso Insumo', $this->co_piso_insumo, $this->sql_piso_insumo, array('id' => 'co_piso_insumo'), array('label-size' => '4', 'input-size' => '8'));
                  ?>
                </div>
                <?php
                  echo $simec->select('co_grupo_insumo', 'Grpo de Insumo', $this->co_grupo_insumo, $this->sql_grupo_insumo, $attrCombo, array('label-size' => '4', 'input-size' => '8'));
                ?>
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>
  <div class="ibox-footer">
    <div class="text-center">
      <button type="button" onclick="window.location='demandas.php?modulo=insumo/listar';" class="btn btn-success"><i class="fa fa-arrow-left"></i> Voltar</button>
      <button type="button" class="btn btn-primary btn-salvar"><i class="fa fa-save"></i> Salvar</button>
    </div>
  </div>
</div>

<script type="text/javascript">

    jQuery('.btn-salvar').click(function(){
        jQuery('[name="formulario-insumo"]').submit();
    });

    $('#co_tipo_insumo').on("change", function() {
        //console.log($(this).val());
        if ($(this).val() == 1){
            $('#div_piso_insumo').show();
            $('#co_piso_insumo').val('').trigger('chosen:updated');
        } else {
            $('#div_piso_insumo').hide();
            $('#co_piso_insumo').val('').trigger('chosen:updated');
        }
    });

</script>
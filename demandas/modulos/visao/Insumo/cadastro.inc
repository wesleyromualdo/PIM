<?php
  global $simec;
?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Insumo</h3>
  </div>
  <div class="tabs-container">
    <ul class="nav nav-tabs nav-custom">
      <li>
        <a href="demandas.php?modulo=insumo/listar">Pesquisar</a>
      </li>
      <li class="active">
        <a href="demandas.php?modulo=insumo/cadastrar"><?php echo ($this->co_insumo) ? 'Editar' : 'Cadastrar'; ?></a>
      </li>
    </ul>
  </div>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-insumo" id="formulario-insumo">
          <input type="hidden" name="requisicao" value="salvarInsumo">
          <div class="row">
            <div class="col-lg-6">
                <?php
                $display_piso_insumo = 'none';

                $arrAttr = array (
                    'id' => 'nu_codigo_insumo',
                    'placeHolder' => 'Código do Insumo',
                    'maxlength' => '60',
                    'autocomplete' => 'off',
                    'class' => 'nu_codigo_insumo',
                    'onKeyUp' => 'validaCodigoInsumoExiste(this.value)',
                    'required' => true
                );

                $arrAttrDsInsumo = array (
                    'id' => 'ds_insumo',
                    'placeHolder' => 'Nome do Insumo',
                    'maxlength' => '255',
                    'autocomplete' => 'off',
                    'required' => true
                );

                echo $simec->select('co_grupo_insumo', 'Subgrupo responsável', $this->co_grupo_insumo, $this->sql_grupo_insumo_filho, array('required' => true, 'id' => 'co_grupo_insumo'), array('label-size' => '4', 'input-size' => '8'));
                echo $simec->input( 'nu_codigo_insumo', 'Código', $this->nu_codigo_insumo, $arrAttr, array('label-size' => '4') );
                echo $simec->input( 'ds_insumo', 'Insumo', $this->ds_insumo, $arrAttrDsInsumo, array('label-size' => '4') );
                echo $simec->select('co_unidade_medida', 'Unidade de medida', $this->co_unidade_medida, $this->sql_unidade_medida, array('required' => true, 'id' => 'co_unidade_medida'), array('label-size' => '4', 'input-size' => '8'));
                echo $simec->select('co_tipo_insumo', 'Tipo', $this->co_tipo_insumo, $this->sql_tipo_insumo, array('required' => true, 'id' => 'co_tipo_insumo'), array('label-size' => '4', 'input-size' => '8'));

                if($this->co_tipo_insumo && ($this->co_tipo_insumo == 1)) {
                  $display_piso_insumo = 'show';
                }
                ?>
                <div id="div_piso_insumo"  style="display: <?php echo $display_piso_insumo ?>">
                  <?php
                    echo $simec->select('co_piso_insumo', 'Piso', $this->co_piso_insumo, $this->sql_piso_insumo, array('id' => 'co_piso_insumo'), array('label-size' => '4', 'input-size' => '8'));
                  ?>
                </div>
                <?php
                  if (!empty($this->co_insumo)) {
                      echo $simec->radio("co_status", "Situação", $this->co_status, array('1' => 'Ativo', '0' => 'Inativo'), array('id' => 'co_status'), array('label-size' => '4', 'input-size' => '8'));
                      echo $simec->input( 'co_insumo', null, $this->co_insumo, null, array('visible' => false) );
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
      <button type="button" onclick="window.location='demandas.php?modulo=insumo/listar';" class="btn btn-success"><i class="fa fa-arrow-left"></i> Voltar</button>
      <button type="button" class="btn btn-primary btn-salvar"><i class="fa fa-save"></i> Salvar</button>
    </div>
  </div>
</div>

<script type="text/javascript">


    $('.btn-salvar').click(function(){
        if ($('#co_grupo_insumo').val() === '') {
            swal('Atenção', 'Subgrupo responsável é obrigatório!', 'warning');
        } else if ($('#nu_codigo_insumo').val() === '') {
            swal('Atenção', 'Código do insumo é obrigatório!', 'warning');
        } else if ($('#ds_insumo').val() === '') {
            swal('Atenção', 'Nome do insumo é obrigatório!', 'warning');
        } else if ($('#co_unidade_medida').val() === '') {
            swal('Atenção', 'Unidade de medida é obrigatório!', 'warning');
        } else if ($('#co_tipo_insumo').val() === '') {
            swal('Atenção', 'Tipo insumo é obrigatório!', 'warning');
        } else if ($('#co_tipo_insumo').val() === '1' && $('#co_piso_insumo').val() === '') {
            swal('Atenção', 'Piso insumo é obrigatório!', 'warning');
        }  else {
            $("#formulario-insumo").submit();
        }
    });

    $('#co_tipo_insumo').on("change", function() {
        //console.log($(this).val());
        if ($(this).val() == 1){
            $('#div_piso_insumo').show();
            $('#co_piso_insumo').attr('required', true);
            $('#co_piso_insumo').val('').trigger('chosen:updated');
        } else {
            $('#div_piso_insumo').hide();
            $('#co_piso_insumo').attr('required', false);
            $('#co_piso_insumo').val('').trigger('chosen:updated');
        }
    });

    $(function(){
        //$('#nu_codigo_insumo').focus();
        $('.nu_codigo_insumo').mask('999', { placeholder: "" });
    });

    $('#co_grupo_insumo').on('change', function () {
        $('#nu_codigo_insumo').val('');
    });

    function validaCodigoInsumoExiste(nu_codigo_insumo){
        //console.log(nu_codigo_insumo);
        var co_insumo = $('#co_insumo').val();
        var co_grupo_insumo = $('#co_grupo_insumo').val();

        if (nu_codigo_insumo.length > 2) {
            //console.log('Digitou 3 números, então chama a função. ', nu_codigo_insumo.length);
            //alert('valida codigo do insumo');
            var url = 'demandas.php?modulo=insumo/listar';
            $.ajax({
                type: "POST",
                url: url,
                data: {'requisicao': 'validarCodigoInsumo', 'co_insumo': co_insumo, 'nu_codigo_insumo': nu_codigo_insumo, 'co_grupo_insumo': co_grupo_insumo},
                success: function (resp) {
                    //console.log(resp);
                    if (resp == 'false'){
                        alert('O código de insumo inserido já foi utilizado, tente novamente!');
                        $('#nu_codigo_insumo').parent().parent().addClass('has-error');
                        $('#nu_codigo_insumo').val('');
                        $('.nu_codigo_insumo').mask('999', { placeholder: "" });
                        return false;
                    } else {
                        $('#ds_insumo').focus();
                        $('#nu_codigo_insumo').parent().parent().removeClass('has-error');
                    }
                }
            });
        }
    }
</script>
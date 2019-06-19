<?php
global $simec;


?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Grupo Insumo</h3>
  </div>
  <div class="tabs-container">
    <ul class="nav nav-tabs nav-custom">
      <li>
        <a href="demandas.php?modulo=grupoInsumo/listar">Pesquisar</a>
      </li>
      <li class="active">
        <a href="demandas.php?modulo=grupoInsumo/cadastrar<?php echo ($this->co_grupo_insumo) ? "&co_grupo_insumo={$this->co_grupo_insumo}" : ""; ?>"><?php echo ($this->co_grupo_insumo) ? 'Editar' : 'Cadastrar'; ?></a>
      </li>
    </ul>
  </div>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-grupo-insumo" id="formulario-grupo-insumo" class="">
          <input type="hidden" name="requisicao" value="salvarGrupoInsumo">
          <div class="row">
            <div class="col-lg-6">
                <?php

                $arrAttr = array (
                    'id' => 'nu_codigo_grupo',
                    'placeHolder' => 'Código do Grupo de Insumo',
                    'maxlength' => '3',
                    'autocomplete' => 'off',
                    'class' => 'nu_codigo_grupo',
                    'onKeyUp' => 'validaCodigoGrupoExiste(this.value)',
                    'required' => true
                );

                $arrAttrDsGrupo = array (
                    'id' => 'ds_grupo_insumo',
                    'placeHolder' => 'Nome do Grupo de Insumo',
                    'maxlength' => '200',
                    'autocomplete' => 'off',
                    'required' => true
                );

                $attrCombo = array (
                    'id' => 'co_grupo_insumo_pai'
                );

                if (empty($this->co_grupo_insumo_pai) && ( is_array($this->sql_grupo_insumo_filho) && count($this->sql_grupo_insumo_filho) > 0 ) ) {
                    echo "<div class='form-group'><div class='alert alert-info col-sm-8 col-md-8 col-lg-8'>Este grupo já possui <b>(".count($this->sql_grupo_insumo_filho).")</b> subgrupo(s) vinculado(s).</div></div>";
                } else {
                    echo $simec->select('co_grupo_insumo_pai', 'Grupo responsável', $this->co_grupo_insumo_pai, $this->sql_grupo_insumo_pai, $attrCombo, array('label-size' => '4', 'input-size' => '8'));
                }
                echo $simec->input( 'co_grupo_insumo', null, $this->co_grupo_insumo, null, array('visible' => false) );
                echo $simec->input( 'nu_codigo_grupo', 'Código', $this->nu_codigo_grupo, $arrAttr, array('required', 'label-size' => '4', 'errorValidate' => 'Erro de código!') );
                echo $simec->input( 'ds_grupo_insumo', 'Descrição do Grupo', $this->ds_grupo_insumo, $arrAttrDsGrupo, array('required', 'label-size' => '4') );

                if (!empty($this->co_grupo_insumo)) {
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
      <button type="button" onclick="window.location='demandas.php?modulo=grupoInsumo/listar';" class="btn btn-success"><i class="fa fa-arrow-left"></i> Voltar</button>
      <button type="button" class="btn btn-primary btn-salvar"><i class="fa fa-save"></i> Salvar</button>
    </div>
  </div>
</div>

<script type="text/javascript">

    $('.btn-salvar').click(function(){
        if ($('#nu_codigo_grupo').val() === '') {
            swal('Atenção', 'Código do grupo é obrigatório!', 'warning');
        } else if ($('#ds_grupo_insumo').val() === '') {
            swal('Atenção', 'Nome do grupo de insumo é obrigatório!', 'warning');
        } else {
            $('[name="formulario-grupo-insumo"]').submit();
        }
    });

    $(function(){
        //$('#nu_codigo_grupo').focus();
        $('.nu_codigo_grupo').mask('999', { placeholder: "" });
    });

    $('#co_grupo_insumo_pai').on('change', function () {
        $('#nu_codigo_grupo').val('');
    });

    function validaCodigoGrupoExiste(nu_codigo_grupo){
        //console.log(nu_codigo_grupo);
        var co_grupo_insumo = $('#co_grupo_insumo').val();
        var co_grupo_insumo_pai = $('#co_grupo_insumo_pai').val();

        if (nu_codigo_grupo.length > 2) {
            //console.log('Digitou 3 números, então chama a função. ', valor.length);
            //alert('valida codigo do grupo');
            var url = 'demandas.php?modulo=grupoInsumo/listar';
            $.ajax({
                type: "POST",
                url: url,
                data: {'requisicao': 'validarCodigoGrupoInsumo', 'co_grupo_insumo': co_grupo_insumo, 'nu_codigo_grupo': nu_codigo_grupo, 'co_grupo_insumo_pai': co_grupo_insumo_pai},
                success: function (resp) {
                    //console.log(resp);
                    if (resp == 'false'){
                        alert('O código do grupo inserido já foi utilizado, tente novamente!');
                        $('#nu_codigo_grupo').parent().parent().addClass('has-error');
                        $('#nu_codigo_grupo').val('');
                        $('.nu_codigo_grupo').mask('999', { placeholder: "" });
                        return false;
                    } else {
                        $('#ds_grupo_insumo').focus();
                        $('#nu_codigo_grupo').parent().parent().removeClass('has-error');
                    }
                }
            });
        }
    }

</script>
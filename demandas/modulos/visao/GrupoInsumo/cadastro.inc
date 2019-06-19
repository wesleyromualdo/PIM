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
        <a href="demandas.php?modulo=grupoInsumo/listar">Lista</a>
      </li>
      <li class="active">
        <a href="demandas.php?modulo=grupoInsumo/cadastrar"><?php echo ($this->co_grupo_insumo) ? 'Editar' : 'Cadastro'; ?></a>
      </li>
    </ul>
    <br/>
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-grupo-insumo" id="formulario-grupo-insumo">
          <input type="hidden" name="requisicao" value="salvarGrupoInsumo">
          <div class="row">
            <div class="col-lg-6">
                <?php

                $arrAttr = array (
                    'placeHolder' => 'Código do Grupo de Insumo',
                    'maxlength' => '60',
                    'autocomplete' => 'off'
                );

                $arrAttrDsGrupo = array (
                    'placeHolder' => 'Nome do Grupo de Insumo',
                    'maxlength' => '200',
                    'autocomplete' => 'off'
                );
                echo $simec->input ( 'nu_codigo_grupo', 'Código do Grupo', $this->nu_codigo_grupo, $arrAttr, array('label-size' => '4') );
                echo $simec->input ( 'ds_grupo_insumo', 'Grupo Insumo', $this->ds_grupo_insumo, $arrAttrDsGrupo, array('label-size' => '4') );

                if (empty($this->co_grupo_insumo_pai) && ( is_array($this->sql_grupo_insumo_filho) && count($this->sql_grupo_insumo_filho) > 0 ) ) {
                    //echo 'Este grupo é pai e possui um ou mais filhos vinculados';
                } else {
                    echo $simec->select('co_grupo_insumo_pai', 'Grupo de Insumo Vinculado', $this->co_grupo_insumo_pai, $this->sql_grupo_insumo_pai, $attrCombo, array('label-size' => '4', 'input-size' => '8'));
                }
                ?>
            </div>
            <?php


            ?>
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

    jQuery('.btn-salvar').click(function(){
        jQuery('[name="formulario-grupo-insumo"]').submit();
    });
    //
    // function abrirCardapio(dctid){
    //  var url = 'demandas.php?modulo=demandantes/verDadosDemandante&acao=A&dctid='+dctid;
    //     $(location).attr('href', url);
    // }

</script>
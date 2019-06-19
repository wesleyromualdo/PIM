<?php
global $simec;


?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Grupo Serviço</h3>
  </div>

  <div class="tabs-container">
    <ul class="nav nav-tabs nav-custom">
      <li>
        <a href="demandas.php?modulo=grupoServico/listar">Pesquisar</a>
      </li>
      <li class="active">
        <a href="demandas.php?modulo=grupoServico/cadastrar"><?php echo ($this->co_grupo_servico) ? 'Editar' : 'Cadastrar'; ?></a>
      </li>
    </ul>
  </div>

  <div class="ibox-content">

    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-grupo-insumo" id="formulario-grupo-insumo">
          <input type="hidden" name="requisicao" value="salvarGrupoServico">
          <div class="row">
            <div class="col-lg-6">
                <?php

                    $arrAttr = array (
                        'placeHolder' => 'Código do Grupo do Serviço',
                        'maxlength' => '60'
                    );

                    $arrAttrDsGrupo = array (
                        'placeHolder' => 'Descrição do Grupo do Serviço',
                        'maxlength' => '200'
                    );
                    echo $simec->input ( 'nu_codigo_grupo_servico', 'Código do Grupo', $this->nu_codigo_grupo_servico, $arrAttr, array('label-size' => '4') );
                    echo $simec->input ( 'ds_grupo_servico', 'Nome do Grupo', $this->ds_grupo_servico, $arrAttrDsGrupo, array('label-size' => '4') );
     
                    if (empty($this->co_grupo_servico_pai) && ( is_array($this->sql_grupo_servico_filho) && count($this->sql_grupo_servico_filho) > 0 ) ) {
                        //echo 'Este grupo é pai e possui um ou mais filhos vinculados';
                    } else {
                        echo $simec->select('co_grupo_servico_pai', 'Grupo Responsável', $this->co_grupo_servico_pai, $this->sql_grupo_servico_pai, $attrCombo, array('label-size' => '4', 'input-size' => '8'));
                    }

                    if (!empty($this->co_grupo_servico)) {
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

<script type="text/javascript">

$(document).ready(function(){

    var validacaoAutomatica = new ValidacaoAutomatica();

    validacaoAutomatica.addCampo('name', 'nu_codigo_grupo_servico', 'Código da Grupo do Serviço')
             .addValidaObrigatorio();

    validacaoAutomatica.addCampo('name', 'ds_grupo_servico', 'Descrição Grupo Serviço')
             .addValidaObrigatorio();


    jQuery('.btn-voltar').click(function(){
        window.location='demandas.php?modulo=grupoServico/listar'
    });

    jQuery('.btn-salvar').click(function (){ 

      validacaoAutomatica.enviarFormulario('formulario-grupo-insumo', 'requisicao', 'salvarAvisoConteudo');

    });
})

</script>
<?php global $simec; ?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Grupo Serviço</h3>
  </div>


    <div class="tabs-container">
      <ul class="nav nav-tabs nav-custom">
        <li class="active">
          <a href="demandas.php?modulo=GrupoServico/listar">Lista</a>
        </li>
        <li >
          <a href="demandas.php?modulo=GrupoServico/cadastrar"><?php echo ($this->co_grupo_servico) ? 'Editar' : 'Cadastro'; ?></a>
        </li>
      </ul>
    </div>


  <div class="ibox-content"> 
    
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-etapa-servico" id="formulario-etapa-servico">
          <input type="hidden" name="pesquisar" value="1">
          <div class="row">
            <div class="col-lg-6">
                <?php

                $nu_codigo_grupo_servico = simec_htmlentities ( $_POST['nu_codigo_grupo_servico'] );
                $ds_grupo_servico = simec_htmlentities ( $_POST['ds_grupo_servico'] );

                $arrAttr = array (
                    'placeHolder' => '',
                    'maxlength' => '60'
                );

                $arrAttrDsGrupo = array (
                    'placeHolder' => '',
                    'maxlength' => '200'
                );

                echo $simec->input( 'nu_codigo_grupo_servico', 'Código do Grupo', $nu_codigo_grupo_servico, $arrAttr, array('label-size' => '4') );
                echo $simec->input( 'ds_grupo_servico','Descrição do Grupo', $ds_grupo_servico, $arrAttrDsGrupo, array('label-size' => '4') );

                echo $simec->radio("co_status", "Situação", '1', array('1' => 'Ativo', '0' => 'Inativo', '-1' => 'Todos'), null, array('label-size' => '4', 'input-size' => '8'));

                ?>
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>

  <div class="ibox-footer">
    <div class="text-center">
      <button type="reset" class="btn btn-primary btn-pesquisar"><i class="fa fa-search"></i> Pesquisar</button>
      <button type="button" class="btn btn-success btn-incluir"><i class="fa fa-plus"></i> Incluir</button>
    </div>
  </div>
</div>


<div class="ibox">
    <div class="ibox-content">
        <?php 

          $this->listagem->render();
        
        ?>
    </div>
</div>

<script type="text/javascript">

    jQuery('.btn-pesquisar').click(function(){
      jQuery('[name="formulario-etapa-servico"]').submit();
    });

    jQuery('.btn-incluir').click(function(){
        window.location='demandas.php?modulo=GrupoServico/cadastrar'
    }); 


    function abrirGrupoServico(co_grupo_servico){
      var url = 'demandas.php?modulo=GrupoServico/cadastrar&co_grupo_servico='+co_grupo_servico;
      $(location).attr('href', url);
    }


    function mostrarFilhos(co_grupo_servico){
      var url = 'demandas.php?modulo=GrupoServico/listar&co_grupo_servico='+co_grupo_servico;
      //$(location).attr('href', url);
    }


    function deletarGrupoServico(co_grupo_servico){
 

        swal({
            title: "Deletar?",
            text: "Deseja realmente remover",
            type: "warning",
            showCancelButton: true,
            confirmButtonClass: "btn-success",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        },
        function(){
            window.location='demandas.php?modulo=GrupoServico/listar&requisicao=deletarGrupoServico&co_grupo_servico='+co_grupo_servico;
        });

    }




</script>
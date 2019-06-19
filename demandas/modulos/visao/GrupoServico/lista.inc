<?php global $simec; ?>
 

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Grupo Serviço </h3>
  </div>


    <div class="tabs-container">
      <ul class="nav nav-tabs nav-custom">
        <li class="active">
          <a href="demandas.php?modulo=grupoServico/listar">Pesquisar</a>
        </li>
        <!---
          <li >
          <a href="demandas.php?modulo=grupoServico/cadastrar"><?php echo ($this->co_grupo_servico) ? 'Editar' : 'Cadastro'; ?></a>
        </li>
          --->
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
                $co_status = ( ($_POST['co_status'] == null)  ) ? "1" : $_POST['co_status'];

                if(trim($_POST['co_status']) =='')
                  $co_status = 1; 

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

                echo $simec->radio("co_status", "Situação", $co_status, array('1' => 'Ativo', '0' => 'Inativo', '-1' => 'Todos'), null, array('label-size' => '4', 'input-size' => '8'));

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


<?php if ($this->pesquisar) { ?>
  <div class="ibox">
    <div class="ibox-content">
      <h2 class="text-left"> Grupo de Serviços </h2>
        <?php $this->listagem->render(); ?>
    </div>
  </div>
<?php } ?>

<script type="text/javascript">

    jQuery('.btn-pesquisar').click(function(){
      jQuery('[name="formulario-etapa-servico"]').submit();
    });

    jQuery('.btn-incluir').click(function(){
        window.location='demandas.php?modulo=grupoServico/cadastrar'
    }); 


    function abrirGrupoServico(co_grupo_servico){
      var url = 'demandas.php?modulo=grupoServico/cadastrar&co_grupo_servico='+co_grupo_servico;
      $(location).attr('href', url);
    }

    function abrirServico(co_servico){
      var url = 'demandas.php?modulo=servico/cadastrar&co_servico='+co_servico;
      $(location).attr('href', url);
    }

    function deletarServico(co_servico){
        swal({
            title: "Exclusão",
            text: "Deseja remover este registro?",
            type: "warning",
            showCancelButton: true,
            confirmButtonClass: "btn-success",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        },
        function(){
            window.location='demandas.php?modulo=servico/listar&requisicao=deletarServico&co_servico='+co_servico;
        });
    }

    function deletarGrupoServico(co_grupo_servico){
        swal({
            title: "Exclusão",
            text: "Deseja remover este registro?",
            type: "warning",
            showCancelButton: true,
            confirmButtonClass: "btn-success",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        },
        function(){
            window.location='demandas.php?modulo=grupoServico/listar&requisicao=deletarGrupoServico&co_grupo_servico='+co_grupo_servico;
        });
    }

</script>
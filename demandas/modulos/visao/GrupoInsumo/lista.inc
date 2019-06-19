<?php global $simec; ?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Grupo Insumo</h3>
  </div>

  <div class="ibox-content">
    <ul class="nav nav-tabs nav-custom">
      <li class="active">
        <a href="demandas.php?modulo=grupoInsumo/listar">Lista</a>
      </li>
    </ul>
    <br/>
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-grupo-insumo" id="formulario-grupo-insumo">
          <input type="hidden" name="pesquisar" value="1">
          <div class="row">
            <div class="col-lg-6">
                <?php
                //ver($_SESSION['usucpf']);
                $nu_codigo_grupo = simec_htmlentities ( $_POST['nu_codigo_grupo'] );
                $ds_grupo_insumo = simec_htmlentities ( $_POST['ds_grupo_insumo'] );

                $arrAttr = array (
                    'placeHolder' => 'Código do Grupo de Insumo',
                    'maxlength' => '60'
                );

                $arrAttrDsGrupo = array (
                    'placeHolder' => 'Nome do Grupo de Insumo',
                    'maxlength' => '200'
                );

                echo $simec->input( 'nu_codigo_grupo', 'Código do Grupo', $nu_codigo_grupo, $arrAttr, array('label-size' => '4') );
                echo $simec->input( 'ds_grupo_insumo', 'Grupo de Insumo', $ds_grupo_insumo, $arrAttrDsGrupo, array('label-size' => '4') );
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
      <button type="button" onclick="window.location='demandas.php?modulo=grupoInsumo/cadastrar';" class="btn btn-success"><i class="fa fa-plus"></i> Incluir</button>
    </div>
  </div>
</div>

<div class="ibox">
  <div class="ibox-content">
      <?php

      $this->listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);

      ?>
  </div>
</div>

<script type="text/javascript">

    jQuery('.btn-pesquisar').click(function(){
      jQuery('[name="formulario-grupo-insumo"]').submit();
    });

    function abrirGrupoInsumo(co_grupo_insumo){
      var url = 'demandas.php?modulo=grupoInsumo/cadastrar&co_grupo_insumo='+co_grupo_insumo;
      $(location).attr('href', url);
    }

    function deletarGrupoInsumo(co_grupo_insumo){

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
            window.location='demandas.php?modulo=grupoInsumo/listar&requisicao=deletarGrupoInsumo&co_grupo_insumo='+co_grupo_insumo;
        });

    }

</script>
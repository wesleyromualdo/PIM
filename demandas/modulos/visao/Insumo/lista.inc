<?php global $simec; ?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Insumo</h3>
  </div>
  <div class="tabs-container">
    <ul class="nav nav-tabs nav-custom">
      <li class="active">
        <a href="demandas.php?modulo=insumo/listar">Pesquisar</a>
      </li>
    </ul>
  </div>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-insumo" id="formulario-insumo">
          <input type="hidden" name="pesquisar" value="1">
          <div class="row">
            <div class="col-lg-6">
              <?php
                $nu_codigo_insumo = simec_htmlentities ( $_POST['nu_codigo_insumo'] );
                $ds_insumo = simec_htmlentities ( $_POST['ds_insumo'] );
                $co_status = ( ($_POST['co_status'] == null)  ) ? "1" : $_POST['co_status'];

                $arrAttr = array (
                    'id' => 'nu_codigo_insumo',
                    'placeHolder' => 'Código do Insumo',
                    'maxlength' => '60',
                    'autocomplete' => 'off'
                );

                $arrAttrDsGrupo = array (
                    'id' => 'ds_insumo',
                    'placeHolder' => 'Nome do Insumo',
                    'maxlength' => '255',
                    'autocomplete' => 'off'
                );

                echo $simec->input('nu_codigo_insumo', 'Código do Insumo', $nu_codigo_insumo, $arrAttr, array('label-size' => '4') );
                echo $simec->input('ds_insumo', 'Insumo', $ds_insumo, $arrAttrDsGrupo, array('label-size' => '4') );
                echo $simec->radio('co_status', 'Situação', $co_status, array('1' => 'Ativo', '0' => 'Inativo', '-1' => 'Todos'), null, array('label-size' => '4', 'input-size' => '8'));
              ?>
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>

  <div class="ibox-footer">
    <div class="text-center">
      <button type="button" class="btn btn-primary btn-pesquisar"><i class="fa fa-search"></i> Pesquisar</button>
      <button type="button" onclick="window.location='demandas.php?modulo=insumo/cadastrar';" class="btn btn-success"><i class="fa fa-plus"></i> Incluir</button>
    </div>
  </div>
</div>
<?php if ($this->pesquisar) { ?>
  <div class="ibox">
    <div class="ibox-content">
        <?php
          $this->listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        ?>
    </div>
  </div>
<?php } ?>

<script type="text/javascript">
  
    $(document).ready(function() {
        $('.navbar-listagem').remove();
    });

    jQuery('.btn-pesquisar').click(function(){
      jQuery('[name="formulario-insumo"]').submit();
    });

    function abrirInsumo(co_insumo){
      var url = 'demandas.php?modulo=insumo/cadastrar&co_insumo='+co_insumo;
      $(location).attr('href', url);
    }

    function deletarInsumo(co_insumo){
      swal({
          title: "Exclusão",
          text: "Deseja remover este registro?",
          type: "warning",
          showCancelButton: true,
          cancelButtonText: "Não",
          confirmButtonClass: "btn-success",
          confirmButtonText: "Sim",
          closeOnConfirm: true
      },
      function(){
          window.location='demandas.php?modulo=insumo/listar&requisicao=deletarInsumo&co_insumo='+co_insumo;
      });
      return false;
    }

</script>
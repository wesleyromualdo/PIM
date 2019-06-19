<?php global $simec; ?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Grupo Insumo</h3>
  </div>
  <div class="tabs-container">
    <ul class="nav nav-tabs nav-custom">
      <li class="active">
        <a href="demandas.php?modulo=grupoInsumo/listar">Pesquisar</a>
      </li>
    </ul>
  </div>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-grupo-insumo" id="formulario-grupo-insumo">
          <input type="hidden" name="pesquisar" value="1">
          <div class="row">
            <div class="col-lg-6">
                <?php
                $nu_codigo_grupo = simec_htmlentities ( $_POST['nu_codigo_grupo'] );
                $ds_grupo_insumo = simec_htmlentities ( $_POST['ds_grupo_insumo'] );
                $co_status = ( ($_POST['co_status'] == null)  ) ? "1" : $_POST['co_status'];

                $arrAttr = array (
                    'placeHolder' => 'Código do Grupo de Insumo',
                    'maxlength' => '3'
                );

                $arrAttrDsGrupo = array (
                    'placeHolder' => 'Nome do Grupo de Insumo',
                    'maxlength' => '200'
                );

                echo $simec->input('nu_codigo_grupo', 'Código do Grupo', $nu_codigo_grupo, $arrAttr, array('label-size' => '4') );
                echo $simec->input('ds_grupo_insumo', 'Grupo de Insumo', $ds_grupo_insumo, $arrAttrDsGrupo, array('label-size' => '4') );
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
      <button type="button" class="btn btn-primary btn-pesquisar"><i class="fa fa-search"></i> Pesquisar</button>
      <button type="button" onclick="window.location='demandas.php?modulo=grupoInsumo/cadastrar';" class="btn btn-success"><i class="fa fa-plus"></i> Incluir</button>
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

    $('.btn-pesquisar').click(function(){
      $('[name="formulario-grupo-insumo"]').submit();
    });

    function abrirGrupoInsumo(co_grupo_insumo){
      var url = 'demandas.php?modulo=grupoInsumo/cadastrar&co_grupo_insumo='+co_grupo_insumo;
      $(location).attr('href', url);
    }

    function abrirInsumo(co_insumo){
        var url = 'demandas.php?modulo=insumo/cadastrar&co_insumo='+co_insumo;
        $(location).attr('href', url);
    }

    function deletarGrupoInsumo(co_grupo_insumo){

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
        function(isConfirm){
            if(isConfirm){
                var url = 'demandas.php?modulo=grupoInsumo/listar';

                $.ajax({
                    type: "POST",
                    url: url,
                    data: {'requisicao': 'validarExclusaoGrupoInsumo', 'co_grupo_insumo': co_grupo_insumo},
                    success: function (resp) {
                        //console.log(resp);
                        if (resp == 'false'){
                            swal('Atenção', 'Não é possivel excluir esse registro pois já possui subgrupos vinculados!', 'error');
                            return false;
                        } else {
                            window.location='demandas.php?modulo=grupoInsumo/listar&requisicao=deletarGrupoInsumo&co_grupo_insumo='+co_grupo_insumo;
                        }
                    }
                });
            }
            return false;
        });
    }

    function deletarSubgrupoInsumo(co_grupo_insumo){
        swal({
            title: "Exclusão",
            text: "Deseja realmente remover este subgrupo?",
            type: "warning",
            showCancelButton: true,
            cancelButtonText: "Não",
            confirmButtonClass: "btn-success",
            confirmButtonText: "Sim",
            closeOnConfirm: true
        },
        function(isConfirm){
            if(isConfirm){
                var url = 'demandas.php?modulo=grupoInsumo/listar';

                $.ajax({
                    type: "POST",
                    url: url,
                    data: {'requisicao': 'validarExclusaoSubgrupoInsumo', 'co_grupo_insumo': co_grupo_insumo},
                    success: function (resp) {
                        //console.log(resp);
                        if (resp == 'false'){
                            swal('Atenção', 'Não é possivel excluir esse registro pois já possui insumos vinculados!', 'error');
                            return false;
                        } else {
                            window.location='demandas.php?modulo=grupoInsumo/listar&requisicao=deletarGrupoInsumo&co_grupo_insumo='+co_grupo_insumo;
                        }
                    }
                });
            }
            return false;
        });
    }

</script>
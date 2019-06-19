<?php global $simec; ?>

<div class="ibox">

  <div class="ibox-title">
    <h3 class="center">Unidade Medida</h3>
  </div>

  <div class="tabs-container">
    <ul class="nav nav-tabs nav-custom">
      <li class="active">
        <a class="nav-link" href="demandas.php?modulo=unidadeMedida/listar">Pesquisar</a>
      </li>
    </ul>
  </div>

  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-unidade-medida" id="formulario-unidade-medida">
          <input type="hidden" name="pesquisar" value="1">
          <div class="row">
            <div class="col-lg-6">
                <?php

                $ds_sigla = simec_htmlentities ( $_POST ['ds_sigla'] );
                $ds_unidade_medida = simec_htmlentities ( $_POST ['ds_unidade_medida'] );
                $co_status = ( ($_POST['co_status'] == null)  ) ? "1" : $_POST['co_status'];

                $arrAttr = array (
                    'maxlength' => '300'
                );
                $arrAttrCombo = array(
                    'id' => 'co_sigla'
                );
                echo $simec->input( 'ds_unidade_medida', 'Descrição Unidade Medida', $ds_unidade_medida, $arrAttr, array('label-size' => '4') );
                //echo $simec->input( 'ds_sigla', 'Sigla', $ds_sigla, $arrAttr, array('label-size' => '4') );
                echo $simec->select('co_unidade_medida', 'Sigla', $this->co_unidade_medida, $this->sql_co_sigla, $attrCombo, array('label-size' => '4', 'input-size' => '8'));

                // radio($name, $label = null, $value = null, $options = null, $attribs = null, $config = array())
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
      <button type="reset" class="btn btn-primary btn-pesquisar text-white"><i class="fa fa-search"></i> Pesquisar</button>
      <button type="button" onclick="window.location='demandas.php?modulo=unidadeMedida/cadastrar';" class="btn btn-success"><i class="fa fa-plus text-white"></i> Incluir</button>
    </div>
  </div>
</div>

<?php if ($this->pesquisar) { ?>
  <div class="ibox">
      <div class="ibox-content">
        <?php

          $listagemSimec = new Simec_Listagem();
          $arrayCabecalho = array('Sigla', 'Descrição Unidade Medida');
          $esconderColunas = array('co_unidade_medida');
          $listagemSimec->setCabecalho($arrayCabecalho);
          $listagemSimec->esconderColunas($esconderColunas);
          $listagemSimec->turnOffForm();
          $listagemSimec->setQuery($this->sql_unidade_medida);
          $listagemSimec->addAcao('edit', array('func' => 'abrirUnidadeMedida', 'extra-params' => array('co_unidade_medida')));
          $listagemSimec->addAcao('delete', array('func' => 'deletarUnidadeMedida', 'extra-params' => array('co_unidade_medida')));
          $listagemSimec->setFormFiltros('formulario-unidade-medida');
          $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
          $listagemSimec->setTamanhoPagina(50);
          $listagemSimec->setCampos($arrayCabecalho);

          $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);

          ?>
      </div>
  </div>
<?php } ?>

<script type="text/javascript">

    jQuery('.btn-pesquisar').click(function(){
        jQuery('[name="formulario-unidade-medida"]').submit();
    });

    function abrirUnidadeMedida(co_unidade_medida){
        var url = 'demandas.php?modulo=unidadeMedida/cadastrar&co_unidade_medida='+co_unidade_medida;
        $(location).attr('href', url);
    }

    function deletarUnidadeMedida(co_unidade_medida){

        swal({
                title: "Exclusão",
                text: "Deseja remover este registro?",
                type: "warning",
                showCancelButton: true,
                cancelButtonText: "Não",
                confirmButtonClass: "btn-success",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            },
            function(){
                window.location='demandas.php?modulo=unidadeMedida/listar&requisicao=deletarUnidadeMedida&co_unidade_medida='+co_unidade_medida;
            });

    }

</script>
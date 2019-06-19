<?php global $simec; ?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Serviço</h3>
  </div>
  <div class="tabs-container">
    <ul class="nav nav-tabs nav-custom">
      <li class="active">
        <a class="nav-link" href="demandas.php?modulo=servico/listar">Pesquisar</a>
      </li>
    </ul>
  </div>

  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-servico" id="formulario-servico">
          <input type="hidden" name="pesquisar" value="1">
          <div class="row">
            <div class="col-lg-6">
                <?php

                $nu_codigo_servico = simec_htmlentities ( $_POST ['nu_codigo_servico'] );
                $ds_servico = simec_htmlentities ( $_POST ['ds_servico'] );
                $co_grupo_servico = simec_htmlentities ( $_POST ['co_grupo_servico'] );
                $co_unidade_medida = simec_htmlentities ( $_POST ['co_unidade_medida'] );
                $co_status = ( ($_POST['co_status'] == null)  ) ? "1" : $_POST['co_status'];

                $arrAttr = array (
                    'maxlength' => '300'
                );
                $arrAttrCombo = array(
                    'id' => 'co_unidade_medida'
                );
                $arrAttrCombo2 = array(
                    'id' => 'co_grupo_servico'
                );

                echo $simec->input( 'nu_codigo_servico', 'Código do Serviço', $nu_codigo_servico, $arrAttr, array('label-size' => '4') );

                echo $simec->input( 'ds_servico', 'Descrição Serviço', $ds_servico, $arrAttr, array('label-size' => '4') );

                echo $simec->select('co_unidade_medida', 'Unidade de Medida', $this->co_unidade_medida, $this->sql_unidade_medida, $attrCombo, array('label-size' => '4', 'input-size' => '8'));

                echo $simec->select('co_grupo_servico', 'Grupo do Serviço', $this->co_grupo_servico, $this->sql_grupo_servico, $attrCombo2, array('label-size' => '4', 'input-size' => '8'));

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
      <button type="button" onclick="window.location='demandas.php?modulo=servico/cadastrar';" class="btn btn-success"><i class="fa fa-plus text-white"></i> Incluir</button>
    </div>
  </div>
</div>

<?php if ($this->pesquisar) { ?>
<div class="ibox">
    <div class="ibox-content">
    	<?php
    	
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Código', 'Descrição Serviço', 'Unidade de Medida', 'Grupo Serviço');
        $esconderColunas = array('co_servico');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOffForm();
        $listagemSimec->setQuery($this->sql_servico);
        $listagemSimec->addAcao('edit', array('func' => 'abrirServico', 'extra-params' => array('co_servico')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarServico', 'extra-params' => array('co_servico')));
        $listagemSimec->setFormFiltros('formulario-servico');
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
        jQuery('[name="formulario-servico"]').submit();
    });

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

</script>
<?php global $simec; ?>

<div class="ibox">
    <div class="ibox-title">
          <h3 class="center">Programa</h3>
    </div>

    <div class="tabs-container">
        <ul class="nav nav-tabs nav-custom">
          <li class="active">
            <a class="nav-link" href="demandas.php?modulo=programa/listar">Pesquisar</a>
          </li>
          <li>
            <a class="nav-link" href="demandas.php?modulo=programa/cadastrar">Cadastrar</a>
          </li>
        </ul>
    </div> 

    <div class="ibox-content">


    
      <div class="row">
        <div class="col-lg-12">
          <form method="post" name="formulario-programa" id="formulario-programa">
            <input type="hidden" name="pesquisar" value="1">
            <div class="row">
                <div class="col-lg-6">
                  <?php

                  $nu_codigo_programa = simec_htmlentities ( $_POST ['nu_codigo_programa'] );
                  $ds_sigla = simec_htmlentities ( $_POST ['ds_sigla'] );

                  $arrAttr = array (
                    'maxlength' => '300'
                  );
                  $arrAttrCombo = array(
                    'id' => 'co_secretaria'
                  );
                  
                  echo $simec->input( 'nu_codigo_programa', 'Código do Programa', $nu_codigo_programa, $arrAttr, array('label-size' => '4') );
                  
                  echo $simec->input( 'ds_sigla', 'Sigla', $ds_sigla, $arrAttr, array('label-size' => '4') );
                  
                  echo $simec->select('co_secretaria', 'Secretaria', $this->co_secretaria, $this->sql_secretarias, $attrCombo, array('label-size' => '4', 'input-size' => '8'));
                  
                  //echo $simec->select('co_grupo_servico', 'Grupo do Serviço', $this->co_grupo_servico, $this->sql_grupo_servico, $attrCombo2, array('label-size' => '4', 'input-size' => '8'));

                  echo $simec->radio("co_situacao", "Situação", '1', array('1' => 'Ativo', '0' => 'Inativo', '-1' => 'Todos'), $this->co_situacao, array('label-size' => '4', 'input-size' => '8'));

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
      <button type="button" onclick="window.location='demandas.php?modulo=programa/cadastrar';" class="btn btn-success"><i class="fa fa-plus text-white"></i> Incluir</button>
    </div>
  </div>
</div>

<div class="ibox">
    <div class="ibox-content">
    	<?php
    	
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Código', 'Descrição Programa', 'Sigla', 'Secretaria');
        $esconderColunas = array('co_programa', 'co_programa_secretaria', 'ds_objetivo');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_programa);
        $listagemSimec->addAcao('edit', array('func' => 'abrirPrograma', 'extra-params' => array('co_programa')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarPrograma', 'extra-params' => array('co_programa')));
        $listagemSimec->setFormFiltros('formulario-programa');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        
        ?>
    </div>
</div>

<script type="text/javascript">

jQuery('.btn-pesquisar').click(function(){
	jQuery('[name="formulario-programa"]').submit();
});

function abrirPrograma(co_programa){
  var url = 'demandas.php?modulo=programa/cadastrar&co_programa='+co_programa;
  $(location).attr('href', url);
}

function deletarPrograma(co_programa){

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
    window.location='demandas.php?modulo=programa/listar&requisicao=deletarPrograma&co_programa='+co_programa;
});

}

</script>
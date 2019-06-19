<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Portfólio</h3>
  </div>
    <?php criaAbaTabelaApoio(); ?>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-portfolio" id="formulario-portfolio">
          <input type="hidden" name="pesquisar" value="1">
          <div class="row">
            <div class="col-lg-12">
                <?php
                $dcdid = $_POST ['dcdid'];
                $dctid = $_POST ['dctid'];
                $dddid = $_POST ['dddid'];
                $dcostatus = ( ($_POST['dcostatus'] == null)  ) ? "1" : $_POST['dcostatus'];

                echo $simec->select('dcdid', 'Descrição', $dcdid, $this->sql_combo_portfolio_descricao, null, array('label-size' => '4'));
                echo $simec->select('dctid', 'Tipo Portfólio', $dctid, $this->sql_combo_portfolio_tipo, null, array('label-size' => '4'));
                echo $simec->select('dddid', 'Ofertante', $dddid, $this->sql_combo_demandados, null, array('label-size' => '4'));
                echo $simec->radio('dcostatus', 'Situação', $dcostatus, array('1' => 'Ativo', '0' => 'Inativo', '-1' => 'Todos'), null, array('label-size' => '4', 'input-size' => '8'));
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
      <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolio/cadastrar';" class="btn btn-success"><i class="fa fa-plus"></i> Incluir</button>
    </div>
  </div>
</div>

<?php if ($this->pesquisar) { ?>
  <div class="ibox">
    <div class="ibox-content">
        <?php

        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Portfólio', 'Tipo Portfólio', 'Ofertante');
        $esconderColunas = array('dcoid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_portfolio);
        $listagemSimec->addAcao('edit', array('func' => 'abrirPortfolio', 'extra-params' => array('dcoid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarPortfolio', 'extra-params' => array('dcoid')));
        $listagemSimec->setFormFiltros('formulario-portfolio');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);

        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);

        ?>
    </div>
  </div>
<?php } ?>

<script type="text/javascript">

    $('.btn-pesquisar').click(function(){
        $('[name="formulario-portfolio"]').submit();
    });

    function abrirPortfolio(dcoid){
        var url = 'demandas.php?modulo=demandasPortfolio/cadastrar&dcoid='+dcoid;
        $(location).attr('href', url);
    }

    function deletarPortfolio(dcoid){

        swal({
                title: "Exclusão",
                text: "Deseja remover este registro?",
                type: "warning",
                showCancelButton: true,
                confirmButtonClass: "btn-success",
                confirmButtonText: "Sim",
                closeOnConfirm: false
            },
            function(isConfirm){
                if(isConfirm) {
                    window.location='demandas.php?modulo=demandasPortfolio/listar&requisicao=deletarPortfolio&dcoid='+dcoid;
                }
            });

    }

</script>
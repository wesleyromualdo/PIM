<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Portfólio Tipo</h3>
  </div>
    <?php criaAbaTabelaApoio(); ?>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-portfolio-tipo" id="formulario-portfolio-tipo">
          <input type="hidden" name="pesquisar" value="1">
          <div class="row">
            <div class="col-lg-6">
                <?php
                $dctdsc = simec_htmlentities ( $_POST ['dctdsc'] );
                $dctstatus = ( ($_POST['dctstatus'] == null)  ) ? "1" : $_POST['dctstatus'];

                echo $simec->input ( 'dctdsc', 'Tipo Portfólio', $dctdsc, null, array('label-size' => '4') );
                echo $simec->radio('dctstatus', 'Situação', $dctstatus, array('1' => 'Ativo', '0' => 'Inativo', '-1' => 'Todos'), null, array('label-size' => '4', 'input-size' => '8'));
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
      <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolioTipo/cadastrar';" class="btn btn-success"><i class="fa fa-plus"></i> Incluir</button>
    </div>
  </div>
</div>

<?php if ($this->pesquisar) { ?>
  <div class="ibox">
    <div class="ibox-content">
        <?php
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Portfólio Tipo');
        $esconderColunas = array('dctid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOffForm();
        $listagemSimec->setQuery($this->sql_portfolio_tipo);
        $listagemSimec->addAcao('edit', array('func' => 'abrirPortfolioTipo', 'extra-params' => array('dctid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarPortfolioTipo', 'extra-params' => array('dctid')));
        $listagemSimec->setFormFiltros('formulario-portfolio-tipo');
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
        $('[name="formulario-portfolio-tipo"]').submit();
    });

    function abrirPortfolioTipo(dctid){
        var url = 'demandas.php?modulo=demandasPortfolioTipo/cadastrar&dctid='+dctid;
        $(location).attr('href', url);
    }

    function deletarPortfolioTipo(dctid){
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
                window.location='demandas.php?modulo=demandasPortfolioTipo/listar&requisicao=deletarPortfolioTipo&dctid='+dctid;
            });
    }

</script>
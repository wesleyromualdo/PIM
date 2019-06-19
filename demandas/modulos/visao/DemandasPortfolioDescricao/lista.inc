<?php global $simec; ?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Portfólio Descrição</h3>
  </div>
    <?php criaAbaTabelaApoio(); ?>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-portfolio-descricao" id="formulario-portfolio-descricao">
          <input type="hidden" name="pesquisar" value="1">
          <div class="row">
            <div class="col-lg-6">
                <?php
                $dcddsc = simec_htmlentities ( $_POST ['dcddsc'] );
                $dcdstatus = ( ($_POST['dcdstatus'] == null)  ) ? "1" : $_POST['dcdstatus'];

                echo $simec->input ( 'dcddsc', 'Descrição Portfólio', $dcddsc, null, array('label-size' => '4') );
                echo $simec->radio('dcdstatus', 'Situação', $dcdstatus, array('1' => 'Ativo', '0' => 'Inativo', '-1' => 'Todos'), null, array('label-size' => '4', 'input-size' => '8'));
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
      <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolioDescricao/cadastrar';" class="btn btn-success"><i class="fa fa-plus"></i> Incluir</button>
    </div>
  </div>
</div>

<?php if ($this->pesquisar) { ?>
  <div class="ibox">
    <div class="ibox-content">
        <?php

        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Portfólio Descrição');
        $esconderColunas = array('dctid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOffForm();
        $listagemSimec->setQuery($this->sql_portfolio_descricao);
        $listagemSimec->addAcao('edit', array('func' => 'abrirPortfolioDescricao', 'extra-params' => array('dcdid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarPortfolioDescricao', 'extra-params' => array('dcdid')));
        $listagemSimec->setFormFiltros('formulario-cardapio-descricao');
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
        $('[name="formulario-portfolio-descricao"]').submit();
    });

    function abrirPortfolioDescricao(dcdid){
        var url = 'demandas.php?modulo=demandasPortfolioDescricao/cadastrar&dcdid='+dcdid;
        $(location).attr('href', url);
    }

    function deletarPortfolioDescricao(dcdid){

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
                window.location='demandas.php?modulo=demandasPortfolioDescricao/listar&requisicao=deletarPortfolioDescricao&dcdid='+dcdid;
            });

    }


</script>
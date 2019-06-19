<?php global $simec; ?>

<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Itens Portfólio Descrição</h3>
  </div>
    <?php criaAbaTabelaApoio(); ?>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-portfolio-itens-descricao" id="formulario-portfolio-itens-descricao">
          <input type="hidden" name="pesquisar" value="1">
            <?php
            $ciddsc = simec_htmlentities ( $_POST ['ciddsc'] );
            $cidstatus = ( ($_POST['cidstatus'] == null)  ) ? "1" : $_POST['cidstatus'];

            echo $simec->input ( 'ciddsc', 'Descrição Itens Portfólio', $ciddsc, null, array('label-size' => '4') );
            echo $simec->radio('cidstatus', 'Situação', $cidstatus, array('1' => 'Ativo', '0' => 'Inativo', '-1' => 'Todos'), null, array('label-size' => '4', 'input-size' => '8'));
            ?>
        </form>
      </div>
    </div>
  </div>
  <div class="ibox-footer">
    <div class="text-center">
      <button type="button" class="btn btn-primary btn-pesquisar"><i class="fa fa-search"></i> Pesquisar</button>
      <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolioItensDescricao/cadastrar';" class="btn btn-success"><i class="fa fa-plus"></i> Incluir</button>
    </div>
  </div>
</div>

<?php if ($this->pesquisar) { ?>
  <div class="ibox">
    <div class="ibox-content">
        <?php

        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Itens Portfólio Descrição');
        $esconderColunas = array('cidid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($this->sql_portfolio_itens_descricao);
        $listagemSimec->addAcao('edit', array('func' => 'abrirPortfolioItensDescricao', 'extra-params' => array('cidid')));
        $listagemSimec->addAcao('delete', array('func' => 'deletarPortfolioItensDescricao', 'extra-params' => array('cidid')));
        $listagemSimec->setFormFiltros('formulario-portfolio-itens-descricao');
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
        jQuery('[name="formulario-portfolio-itens-descricao"]').submit();
    });

    function abrirPortfolioItensDescricao(cidid){
        var url = 'demandas.php?modulo=demandasPortfolioItensDescricao/cadastrar&cidid='+cidid;
        $(location).attr('href', url);
    }

    function deletarPortfolioItensDescricao(cidid){

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
                window.location='demandas.php?modulo=demandasPortfolioItensDescricao/listar&requisicao=deletarPortfolioItemDescricao&cidid='+cidid;
            });

    }


</script>
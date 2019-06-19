<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Portfólio</h3>
  </div>
    <?php criaAbaTabelaApoio(); ?>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-portfolio" id="formulario-portfolio">
            <?php if($this->dcoid) : ?>
              <input type="hidden" name="requisicao" value="adicionarPortfolioItem">
            <?php else : ?>
              <input type="hidden" name="requisicao" value="salvarPortfolio">
            <?php endif; ?>
          <div class="row">
            <div class="col-lg-6">
                <?php
                $_att = array();

                if ($this->dcoid) {
                    $_att = array('disabled');
                }

                echo $simec->select('dcdid', 'Descrição', $this->dcdid, $this->sql_combo_portfolio_descricao, $_att, array('label-size' => '4'));
                echo $simec->select('dctid', 'Tipo Cardápio', $this->dctid, $this->sql_combo_portfolio_tipo, $_att, array('label-size' => '4'));
                echo $simec->select('dddid', 'Ofertante', $this->dddid, $this->sql_combo_demandados, $_att, array('label-size' => '4'));

                if (!empty($this->dcoid)) {
                    echo $simec->radio("dcdstatus", "Situação", $this->dcostatus, array('1' => 'Ativo', '0' => 'Inativo'), array('id' => 'co_status'), array('label-size' => '4', 'input-size' => '8'));
                }
                ?>
            </div>
          </div>
          <div class="row">
            <div class="col-lg-12">
                <?php if($this->dcoid) : ?>
                  <div class="panel panel-default">
                    <div class="panel-heading">
                      <strong>Itens</strong>
                    </div>
                    <div class="panel-body">
                        <?php
                        echo $simec->select('cidid', 'Item descrição', '', $this->sql_combo_item_descricao, array(), array());
                        echo $simec->input( 'dcivalor', 'Valor', $this->dctdsc, array ('maxlength' => '300'), array() );

                        $sql = "( SELECT 'TRUE' AS codigo, 'SIM' AS descricao ) UNION ALL ( SELECT 'FALSE' AS codigo, 'NÃO' AS descricao )";
                        echo $simec->select('dcivaloreditavel', 'Item editável para demandante', '', $sql, array(''), array());

                        ?>
                      <div class="ibox-footer">
                        <div class="text-center">
                          <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolio/listar';" class="btn btn-success"><i class="fa fa-arrow-left"></i> Voltar</button>
                          <button type="submit" class="btn btn-primary"><i class="fa fa-plus"></i> Adicionar</button>
                        </div>
                      </div>
                        <?php if($this->portfolioItens) : ?>
                            <?php

                            $listagemSimec = new Simec_Listagem();
                            $arrayCabecalho = array('Item', 'Valor', 'Item editável para demandante');
                            $esconderColunas = array('dciid');
                            $listagemSimec->setCabecalho($arrayCabecalho);
                            $listagemSimec->esconderColunas($esconderColunas);
                            $listagemSimec->turnOffForm();
                            $listagemSimec->setDados($this->portfolioItens);
                            $listagemSimec->addAcao('delete', array('func' => 'deletarCardapioItem'));
                            $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                            $listagemSimec->setTamanhoPagina(500);
                            $listagemSimec->setCampos($arrayCabecalho);

                            $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);

                            ?>
                        <?php endif; ?>
                    </div>
                  </div>
                <?php endif; ?>
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>
    <?php if(!$this->dcoid) : ?>
      <div class="ibox-footer">
        <div class="text-center">
          <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolio/listar';" class="btn btn-success"><i class="fa fa-arrow-left"></i> Voltar</button>
          <button type="button" class="btn btn-primary btn-salvar"><i class="fa fa-save"></i> Salvar</button>
        </div>
      </div>
    <?php endif; ?>
</div>

<script type="text/javascript">

    $('.btn-salvar').click(function(){
        $('[name="formulario-portfolio"]').submit();
    });

    $('#inserirItem').click(function(){
        $.ajax({
            type: "GET",
            url: window.location.href,
            data: 'requisicao=inserirCardapioItem&',
            success: function(retorno){
                options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function() {
                    options.append(new Option(this.descricao, this.codigo));
                });
                options.val(dcoid);
                options.trigger('chosen:updated');
            }
        });
    });

    <?php if($this->dcoid) : ?>
      function deletarCardapioItem(dciid) {
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
              function(isConfirm){
                  if(isConfirm){
                      window.location='demandas.php?modulo=demandasCardapio/cadastrar&acao=A&dcoid=<?=$this->dcoid?>&requisicao=deletarCardapioItem&dciid='+dciid;
                  }
              });
          return false;
      }
    <?php endif; ?>

</script>
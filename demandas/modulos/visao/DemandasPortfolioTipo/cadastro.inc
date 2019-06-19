<div class="ibox">
  <div class="ibox-title">
    <h3 class="center">Portfólio Tipo</h3>
  </div>
  <?php criaAbaTabelaApoio(); ?>
  <div class="ibox-content">
    <div class="row">
      <div class="col-lg-12">
        <form method="post" name="formulario-portfolio-tipo" id="formulario-portfolio-tipo">
          <input type="hidden" name="requisicao" value="salvarPortfolioTipo">
          <div class="row">
            <div class="col-lg-6">
                <?php
                  $dctdsc = simec_htmlentities ( $_POST ['dctdsc'] );

                  echo $simec->input( 'dctdsc', 'Tipo Portfólio', $this->dctdsc, null, array('label-size' => '4') );
                  if (!empty($this->dctid)) {
                      echo $simec->radio("dctstatus", "Situação", $this->dctstatus, array('1' => 'Ativo', '0' => 'Inativo'), array('id' => 'co_status'), array('label-size' => '4', 'input-size' => '8'));
                  }
                ?>
            </div>
          </div>
        </form>
      </div>
    </div>
  </div>
  <div class="ibox-footer">
    <div class="text-center">
      <button type="button" onclick="window.location='demandas.php?modulo=demandasPortfolioTipo/listar';" class="btn btn-success"><i class="fa fa-arrow-left"></i> Voltar</button>
      <button type="button" class="btn btn-primary btn-salvar"><i class="fa fa-save"></i> Salvar</button>
    </div>
  </div>
</div>

<script type="text/javascript">

    $('.btn-salvar').click(function(){
        $('[name="formulario-portfolio-tipo"]').submit();
    });

</script>
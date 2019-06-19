<div class="modal fade" id="modal-representante" tabindex="-1" role="dialog" aria-hidden="true">
  <div class="modal-dialog modal-lg" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title">Formulário de representante</h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body">
      <form name="form-modal-representante" id="form-modal-representante" action="" method="post">
		<input type="hidden" name="requisicao" id="requisicao" value="">
      <?php 
        echo $simec->select('enfidfilho', 'Representante', $this->edicao['enfidfilho'], $this->arComboRepresentanteSql, [], ['input-size'=>'8', 'label-size'=>'4']);		
        echo $simec->radio('enrtipo', 'Abrangência do representante', $this->edicao['enrtipo'], $this->arTipoRepresentante, [], ['input-size'=>'8', 'label-size'=>'4']);
        echo $simec->radio('enrgestor', 'Gestor', $this->edicao['enrgestor'], $this->arRadioGestor, [], ['input-size'=>'8', 'label-size'=>'4']);
        echo $simec->input ('enrqtdcadastrados', 'Quantidade de representantes', $this->edicao['enrqtdcadastrados'], ['type'=>'number', 'maxlength'=>3], ['input-size'=>'3', 'label-size'=>'4']);
      ?>
      </form>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary" data-dismiss="modal">
        	<i class="fas fa-times-circle"></i> Fechar
        </button>
        <button type="button" class="btn btn-primary js-btn-salvar">
        	<i class="fa fa-save"></i> Salvar
        </button>
      </div>
    </div>
  </div>
</div>


<script type="text/javascript">
var $validacao = new ValidacaoAutomatica();
$validacao.addCampo('name', 'enfidfilho', 'Representante')
		  .addValidaObrigatorio();
$validacao.addCampo('name', 'enrtipo', 'Abrangência do representante')
		  .addValidaObrigatorio();
$validacao.addCampo('name', 'enrgestor', 'Gestor')
		  .addValidaObrigatorio();
$validacao.addCampo('name', 'enrqtdcadastrados', 'Quantidade de representantes')
		  .addValidaObrigatorio();
		  
jQuery('.js-btn-salvar').click(function ()
{
	jQuery('#enfidfilho').prop('disabled', false);
	$validacao.enviarFormulario('form-modal-representante', 'requisicao', 'salvar-representante');
	jQuery('#enfidfilho').prop('disabled', true);
});

function limparDadosFormModal()
{
	jQuery('#enfidfilho').prop('disabled', false);
	jQuery('#enfidfilho').val('').trigger("chosen:updated");

	if (jQuery('[name=enrtipo]:checked').val()) {
		jQuery('[name=enrtipo]:checked').parents('.btn-group:first').find('label').removeClass('active');
    	jQuery('[name=enrtipo]:checked').prop('checked', false);
	}
	
	if (jQuery('[name=enrgestor]:checked').val()) {
		jQuery('[name=enrgestor]:checked').parents('.btn-group:first').find('label').removeClass('active');
    	jQuery('[name=enrgestor]:checked').prop('checked', false);
	}
	
	jQuery('#enrqtdcadastrados').val('');
}
</script>


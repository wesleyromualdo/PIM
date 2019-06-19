<?php
include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
include_once APPRAIZ.'includes/classes/modelo/questionario/QQuestionario.class.inc';

if($_POST['atualizar']) {
	
	$atualiza = updateDataVigencia($_POST);
	
}

$questionario = new QQuestionario(133);
?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Tabela de apoio - Vigência do questionario banda larga</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    	
    	<?php if($atualiza) :?>
	    	<?php if($atualiza['sucesso']) : ?>
	    		<div class="alert alert-success" id="sucesso">
	    			<b><?=$atualiza['sucesso']?></b>
	    		</div>
	    	<?php else : ?>
	    		<div class="alert alert-danger" id="erro">
	    			<b><?=$atualiza['erro']?></b>
	    		</div>
	    	<?php endif; ?>
    	<?php endif;?>
    
        <form method="post" name="formulario-vigencia-questionario">
            <div class="row">
                <div class="col-lg-6 col-md-6 center">
                	<?php echo $simec->select('Tipo de Configuração', 'DE', $questionario->quedtinicio, array("required" => "required")); ?>
                </div>
                <div class="col-md-6">
                    <button type="submit" class="btn btn-info" name="atualizar" value="atualizar">Atualizar</button>
                </div>
            </div>
        </form>
    </div>
</div>

<?php if(!empty($atualiza)) : ?>
	<script>
		var time = setTimeout(function(){
		    $("#erro").fadeOut();
		    $("#sucesso").fadeOut();
		}, 3500); 
	</script>
<?php endif; ?>

<?php 
function updateDataVigencia($dados) {

	$questionario = new QQuestionario(133);
	
	# VERIFICA SE DATA É VÁLIDA
	$dtinicio = explode("/", $dados['quedtinicio']);
	$dtfim	  = explode("/", $dados['quedtfim']);
	
	if(!checkdate($dtinicio[1],$dtinicio[0],$dtinicio[2]) || !checkdate($dtfim[1],$dtfim[0],$dtfim[2])) {
		
		return array("erro" => "Favor inserir datas válidas.");
		
	}
	
	# SETA NOVAS DATAS AO OBJETO
	$questionario->quedtinicio = formata_data_sql($dados['quedtinicio']);
	$questionario->quedtfim = formata_data_sql($dados['quedtfim']);
	
	# SALVAR
	if($queid = $questionario->salvar()){
		
		$questionario->commit();
		
		return array("sucesso" => "Operação realizada com sucesso.");
		
	} else {
		
		# ROLBACK EM CASO DE FALHA
		$questionario->rollback();
		
		return array("erro" => "Não foi possível atualizar este registro, tente novamente");
	}
	
}

?>
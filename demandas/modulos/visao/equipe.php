<?php
$renderEntidade = new Demanda_Controller_Entidade();

?>

<input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid ?>"/>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Equipe Local - Integrantes</h3>
            </div>
            <div class="col-md-6">
                <button class="btn btn-success novo  pull-left" data-toggle="modal" data-target="#modal">
                    <i class="fa fa-plus-square-o"></i> Inserir Integrante
                </button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <?php
            $_REQUEST['elostatus'] = 'A';
            //$controllerEquipeLocal->listaEquipe($_REQUEST);
        ?>
    </div>
    <div class="ibox-footer">
        <div class="row">
            <div class="col-lg-6 text-left">
                <button type="button" class="btn btn-success previous" >Anterior</button>
            </div>
            <div class="col-lg-6 text-right">
                <button type="button" class="btn btn-success next" >Próximo</button>
            </div>
        </div>
    </div>
</div>
<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
    	<form method="post" name="formEquipe" id="formEquipe" class="form form-horizontal">
	        <div class="modal-content animated flipInY">
	            <div class="ibox-title" tipo="integrantes">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
	                <h3>Novo Integrante </h3>
	            </div>
	            <div class="ibox-content">
					<input type="hidden" name="req" value="salvar"/>
	               	<?php //echo $controllerEquipeLocal->formNovoEquipeLocal($_REQUEST); ?>
	            </div>
	            <div class="ibox-footer">
	            	<div class="col-sm-offset-2 col-md-offset-2 col-lg-offset-2">
	                	<button type="submit" class="btn btn-success" <?php echo $disabled; ?>>
	                		<i class="fa fa-plus-square-o"></i> Salvar
	                	</button>
	                </div>
	            </div>
	        </div>
        </form>
    </div>
</div>
<div class="ibox collapsed">
    <div class="ibox-title">
        <h5>Equipe Local - Histórico Modificações</h5>
        <div class="ibox-tools">
            <a class="collapse-link">
                <i class="fa fa-chevron-down"></i>
            </a>
        </div>
    </div>
    <?php //$controllerEntidade->formHistorico($arrParamHistorico); ?>
</div>

<script>


</script>
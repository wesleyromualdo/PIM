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
	               	<?php echo $controllerEquipeLocal->formNovoEquipeLocal($_REQUEST); ?>
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

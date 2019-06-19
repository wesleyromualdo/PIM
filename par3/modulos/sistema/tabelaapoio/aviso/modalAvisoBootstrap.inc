<div class="ibox float-e-margins animated modal in" id="<?php echo ($actid ? 'modal_conteudo_aviso' : 'modal_conteudo_aviso_exemplo'); ?>" tabdesex="-1" role="dialog" aria-hidden="true">
	<div class="modal-dialog">
		<div class="modal-content" style="width: auto;">
			<div class="ibox-title">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close">
					<span aria-hidden="true">×</span>
				</button>
				<h2 class="center">Aviso</h2>
				<input type="hidden" name="actid-aviso" id="actid-aviso" value="<?php echo $actid ?>">
			</div>
			<div class="ibox-content" id="conteudo-modal-aviso" style="overflow: auto; max-height: 520px;">
			<?php
			echo $conteudoAviso;
			?>
			</div>
			<div class="ibox-footer">
				<div class="center">
					<button type="button" id="bnt-fechar" data-dismiss="modal" class="btn btn-default">
						<i class="fa fa-times-circle-o"></i> Fechar
					</button>
					<button type="button" id="bnt-ciente" class="btn btn-primary <?php echo (($actpermiteciente == 't' && $actid) ? '' : 'hidden') ?>" 
					data-toggle="popover" data-html="true" data-trigger="hover" data-placement="top" 
					data-content="Clique para <b>não aparecer</b> mais este aviso!"> 
						<i class="fa fa-check-square"></i> Ciente
					</button>
				</div>
			</div>
		</div>
	</div>
</div>

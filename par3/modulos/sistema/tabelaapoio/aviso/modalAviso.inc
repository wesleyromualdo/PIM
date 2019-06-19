<div class="" id="<?php echo ($actid ? 'modal_conteudo_aviso' : 'modal_conteudo_aviso_exemplo'); ?>" style="display: none;">
	<input type="hidden" name="actid-aviso" id="actid-aviso" value="<?php echo $actid ?>">
	<input type="hidden" name="actpermiteciente-aviso" id="actpermiteciente-aviso" value="<?php echo $actpermiteciente ?>">
	<div class="" id="conteudo-modal-aviso" style="overflow: auto; max-height: 650px;">
	<?php
	echo $conteudoAviso;
	?>
	</div>
</div>

<style>
.ui-widget-content.dialog-modal-aviso{
    border: 1px solid #CECECE;
}

.dialog-modal-aviso .ui-widget-header{
    background: #E9E9E9;
    border: 1px solid #CECECE;
    color: #000000;
}

.dialog-modal-aviso .ui-dialog-titlebar .ui-dialog-titlebar-close{
    background: #FFFFFF;
    border: 1px solid #CECECE;
    color: #000000;
}

.dialog-modal-aviso .ui-dialog-buttonset button{
    background:#F5F5F5;
    border: 1px solid #E9E9E9;
}

.dialog-modal-aviso .ui-dialog-buttonset button:hover{
    background:#E9E9E9;
    border: 1px solid #E9E9E9;
}

/*     background:#1AB394; */
.dialog-modal-aviso .ui-dialog-buttonset #btn_ciente{
    background:#1FD1AA;
    color:#FFFFFF;
    border: 1px solid #1AB394;
}

.dialog-modal-aviso .ui-dialog-buttonset #btn_ciente:hover{
    background  : #1AB394;
    color       : #FFFFFF;
    border      : 1px solid #1AB394;
}
</style>


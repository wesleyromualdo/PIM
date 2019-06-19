<?php
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();

$pendenciaBox = $_REQUEST['pendencia'];
$boExists = $helperPendencia->consultaHabilitaEntidade( $_REQUEST['inuid'] );
$arAuxHelperPendencia = $helperPendencia->controleWidgetsPendenciaPrincipal($pendenciaBox, $boExists); ?>


<div class="col-lg-12 habilitacao">
	<div class=" widget  yellow-bg text-left" style="background-color: <?php echo $arAuxHelperPendencia['widget']; ?>;">
		<div class="slim-scroll">
			<i class="fa <?php echo $arAuxHelperPendencia['thumb']; ?> fa-2x"></i>

			<span class="font-bold no-margins" style="font-size: 16pt;"><?php echo $arAuxHelperPendencia['description']; ?> </span>

			<p>
			<h3><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h3>
			</p>

			<div class="list-group">
				<a class="list-group-item active" href="#">
					<h4 class="list-group-item-heading"><?php echo $arAuxHelperPendencia['msg']; ?></h4>
				</a>
			</div>
		</div>
	</div>
</div>
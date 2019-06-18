<?php
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$titulo_modulo = 'Central de Pendências nas Diligências';
monta_titulo($titulo_modulo, '');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" id="formulario" name="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td>
			<center>
				
<!--				<input name="btn_abrir" value="" type="button" onclick="criarDiligenciasEmLote();">-->
<!--                --><?php //if($habilitada_insercao && $habilitado && !possui_perfil( array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_GESTOR))): ?>
                <?php if(true): ?>
                    <input type="button"
                           value="Criar Diligências em Lote"
                           style="padding: 1px; padding-left: 20px; padding-right: 20px; margin-top:3px; margin-bottom:3px;"
                           onclick="janela = window.open('obras2.php?modulo=principal/cadDiligenciaLote&acao=O', 'inserirdiligencia', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); janela.focus();"/>
                <?php endif; ?>


				<input name="btn_abrir" value="Diligências" type="button" onclick="abrirPopPainelDiligencia();">
			</center>
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">


function abrirPopPainelDiligencia(){
	document.formulario.action = '../obras2/popUpPainelDiligencia.php';

	var janela = window.open( '', 'modopop', 'status=1,menubar=0,toolbar=0,scrollbars=1,resizable=1' );
	janela.focus();
	document.formulario.target = 'modopop';
	document.formulario.submit();
}

</script>

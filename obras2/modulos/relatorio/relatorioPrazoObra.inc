<?php
	include APPRAIZ . 'includes/cabecalho.inc';

	print '<br/>';
	monta_titulo( 'Relatório - Prazo de Obra', 'Relatório Validações - Monitoramento de Obras' );

	//usa a referência para listar o campo situação
	require APPRAIZ . 'obras2/includes/principal/listaRestricao/ctrl.php';
?>

	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
	<style>
	    .chosen-select{
	        width: 400px;
	    }
	</style>
	<form action="obras2.php?modulo=relatorio/popupRelatorioPrazoObras&acao=A" method="post" name="filtro" id="filtro">
	<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">ID Obras</td>
			<td>
				<?php echo campo_texto( 'obrid', 'N', 'S', '', 58, 60, '', '', 'left', '', 0, ''); ?>
			</td>
		</tr>
		<tr>
            <td class="SubTituloDireita">Programa:</td>
            <td class="tdInputForm">
                <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['prfid']) && in_array($key['codigo'], $_POST['prfid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
		</tr>
	
        <tr>
            <td class="SubTituloDireita">Fonte:</td>
            <td class="tdInputForm">
                <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                       ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tooid']) && in_array($key['codigo'], $_POST['tooid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
   

		<tr>
            <td class="SubTituloDireita">Situação da Obra:</td>
            <td>
                <?php
                $sql_obr = "";

                $dados = $db->carregar(getSqlEstadoDoc(TPDID_OBJETO));
                $dados = $dados ? $dados : array();
                ?>
                <select name="esdid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['esdid']) && in_array($key['codigo'], $_POST['esdid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>

		<tr>
			<td class="SubTituloDireita">Município:</td>
            <td class="td_municipio ">
                <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                    <?php
                    $municipio = new Municipio();
                    foreach ($municipio->listaComboMulti() as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['muncod']) && in_array($key['codigo'], $_POST['muncod'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>				
		</tr>

		<tr>
			<td class="SubTituloDireita">Paralisação:</td>
			<td>
				<?php 	
		            $array_paralisacao = array(array("descricao" => "Sim","codigo" => "Sim"),array("descricao" => "Não","codigo" => "Não"));                        
					$db->monta_combo( "paralisacao", $array_paralisacao, "S", "Selecione", "", "", "", "", "N", "paralisacao" );			
				?>
			</td>
		</tr>



		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				<input type="button" value="Visualizar XLS" onclick="obras_exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
			</td>
		</tr>
</table>
<script type="text/javascript">
var formulario = document.getElementById('filtro');

function obras_exibeRelatorioGeral(tipo){
			 
	/*if ( !document.getElementsByName('orgid[]').item(0).checked &&
		 !document.getElementsByName('orgid[]').item(1).checked &&
		 !document.getElementsByName('orgid[]').item(2).checked ){
		alert( 'Favor selecionar ao menos um tipo de ensino!' );
		return false;
	}


	if(document.getElementById('prfid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'prfid' ) );
	}
	
	if(document.getElementById('tooid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'tooid' ) );
	}
		if(document.getElementById('tooid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'esdid' ) );
	}
	*/

	formulario.target = 'RelatorioValidacoes';
	var janela = window.open( 'obras2.php?modulo=relatorio/popupRelatorioPrazoObras&acao=A', 'RelatorioValidacoes', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes' );
	janela.focus();
	
	// Tipo de relatorio
	document.getElementById('pesquisa').value= tipo ;
	formulario.submit();		
}

/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
</script>
</form>

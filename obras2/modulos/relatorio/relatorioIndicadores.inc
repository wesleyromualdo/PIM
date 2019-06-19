<?php
switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=utf-8');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;		
}
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório - Indicadores fnde', 'Relatório Validações' );

?>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<style>
    .chosen-select{
        width: 400px;
    }
</style>
<form action="obras2.php?modulo=relatorio/popupRelatorioIndicadores&acao=A" method="post" name="filtro" id="filtro">
<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

    <tr>
        <td class="SubTituloDireita" style="width: 490px;">Nome da Obra/ID</td>
        <td>
            <?php echo campo_texto( 'obrid', 'N', 'S', '', 58, 60, '', '', 'left', '', 0, ''); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="width: 190px;">ID Pré-Obra:</td>
        <td>
            <?php echo campo_texto( 'preid', 'N', 'S', '', 58, 60, '', '', 'left', '', 0, ''); ?>
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

        <tr id="idMunicipio">
            <td class="SubTituloDireita ">Municípios(s):</td>
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
            </td>
        </tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorio('exibir');" style="cursor: pointer;"/>
				<input type="button" value="Visualizar XLS" onclick="exibeRelatorio('xls');" style="cursor: pointer;"/>
			</td>
		</tr>
</table>
<script type="text/javascript">

function carregarMunicipio( estuf ){
	var td	= $('#td_municipio');
	if ( estuf != '' ){
		var url = location.href;
		$.ajax({
			  url  		 : url,
			  type 		 : 'post',
			  data 		 : {ajax  : 'municipio', 
			  		  	    estuf : estuf},
			  dataType   : "html",
			  async		 : false,
			  beforeSend : function (){
			  	divCarregando();
				td.find('select option:first').attr('selected', true);
			  },
			  error 	 : function (){
			  	divCarregado();
			  },
			  success	 : function ( data ){
          console.info(data);
			  	td.html( data );
			  	divCarregado();
			  }
		});	
	}else{
		td.find('select option:first').attr('selected', true);
		td.find('select').attr('selected', true)
						 .attr('disabled', true);
	}			
}

formulario = document.getElementById('filtro');
function exibeRelatorio(tipo){
	
    if(document.getElementById('prfid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'prfid' ) );
	}
	if(document.getElementById('tooid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'tooid' ) );
	}
    
	formulario.target = 'RelatorioIndicadores';
	var janela = window.open( 'obras2.php?modulo=relatorio/popupRelatorioIndicadores&acao=A', 'RelatorioIndicadores', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes' );
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
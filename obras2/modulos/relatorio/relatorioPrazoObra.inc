<?php
	include APPRAIZ . 'includes/cabecalho.inc';

	print '<br/>';


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
	<?php 
		monta_titulo_relatorios( 'Relatório - Prazo de Obra', 'Relatório Validações - Monitoramento de Obras' );
	?>


    <div class="form-filters form-design">
        <div class="row">
            <div class="col-lg-6">
                <div class="row">
                    <div class="col-md-4">Obra/ID:</div>
                    <div class="col-lg-8">
                        <?php echo campo_texto('obrid', 'N', 'S', '', 58, 60, '', '', 'left', '', 0, ''); ?>    
                    </div> 
                </div>
                <div class="row">
                    <div class="col-md-4">Municípios</div>
                    <div class="col-lg-8">
                        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                            <?php
                            $municipio = new Municipio();
                            foreach ($municipio->listaComboMulti() as $key) {
                                ?>
                                <option
                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['muncod']) && in_array($key['codigo'], $_POST['muncod'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>  
                    </div> 
                </div>
                <div class="row">
                    <div class="col-md-4">Fonte</div>
                    <div class="col-lg-8">
                        <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                            <?php  $tipoOrigemObra = new TipoOrigemObra();
                            $param = array();
                            foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                                ?>
                                <option
                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tooid']) && in_array($key['codigo'], $_POST['tooid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>  
                    </div> 
                </div>
            </div>

            <!--Segunda Col-->
            <div class="col-lg-6">
                <div class="row"> 
                    <div class="col-md-4">Situação da Obra:</div>
                    <div class="col-lg-8">
                        <?php
                        $sql_obr = "";

                        $dados = $db->carregar(getSqlEstadoDoc(TPDID_OBJETO));
                        $dados = $dados ? $dados : array();
                        ?>
                        <select name="esdid_obr[]" class="chosen-select" multiple data-placeholder="Selecione">
                            <?php
                            foreach ($dados as $key) {
                                ?>
                                <option
                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['esdid_obr']) && in_array($key['codigo'], $_POST['esdid_obr'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>
                    </div>                    
                </div>  
                <div class="row"> 
                    <div class="col-md-4">Programa</div>
                    <div class="col-lg-8">
                        <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                            <?php  $programa = new ProgramaFonte();
                            $param = array("orgid" => $_SESSION['obras2']['orgid']);
                            foreach ($programa->listacombo($param, false) as $key) {
                                ?>
                                <option
                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['prfid']) && in_array($key['codigo'], $_POST['prfid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>
                    </div>                    
                </div>  
                <div class="row"> 
                    <div class="col-md-4">Paralisação</div>
                    <div class="col-lg-8">
						<?php 	
				            $array_paralisacao = array(array("descricao" => "Sim","codigo" => "Sim"),array("descricao" => "Não","codigo" => "Não"));                        
							$db->monta_combo( "paralisacao", $array_paralisacao, "S", "Selecione", "", "", "", "", "N", "paralisacao" );			
						?>
		            </div>                    
                </div>                                 
            </div>
        </div>       
    </div>
    <div class="row form-filters text-center">
        <div class="col">

            <button type="button" class="btn btn-success" value="Pesquisar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"><span class="glyphicon glyphicon-list-alt"></span> Pesquisar </button>
            <button type="button" class="btn btn-success" value="Gerar Excel" onclick="obras_exibeRelatorioGeralXLS();"style="cursor: pointer;" ><span class="glyphicon glyphicon-download-alt"></span> Gerar Excel</button>

        </div>
    </div> 

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

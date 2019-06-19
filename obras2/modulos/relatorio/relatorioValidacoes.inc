<?php
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';


$dir_relatorios = APPRAIZ . 'arquivos/obras2/relatorio/validacao';
if($_GET['download']){
    if(file_exists($dir_relatorios . '/' . $_GET['download'])) {
        ob_clean();
        header("Content-Type: application/vnd.ms-excel; charset=ISO-8859-1");
        header("Content-Disposition: inline; filename=\"" . $_GET['download'] . "\"");
        echo file_get_contents($dir_relatorios . '/' . $_GET['download']);
    }
    exit;
}


?>

<?php
	//lista e exibe a situação da obra
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
<script type="text/javascript">


    function exibeRelatorioGeral(tipo) {
        var formulario = document.formulario;
        selectAllOptions(document.getElementById('prfid'));
        selectAllOptions(document.getElementById('tooid'));
        selectAllOptions(document.getElementById('estuf'));
        selectAllOptions(document.getElementById('muncod'));
        selectAllOptions(document.getElementById('esdid_obr'));

        formulario.action = 'obras2.php?modulo=relatorio/relatorioObrasVinculadas&acao=A&tiporelatorio=' + tipo;
        window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        formulario.target = 'relatorio';
        formulario.submit();
    }


    /* Função para substituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }

    /**
     * Alterar visibilidade de um bloco.
     * 
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco(bloco)
    {
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
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
    function onOffCampo(campo)
    {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
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

//-->
</script>
<form action="obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A" method="post" name="filtro" id="filtro">
<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>


	<?php 
		monta_titulo_relatorios( 'Relatório - Monitoramento de Obras: Validações ', 'Relatório Validações' );
	?>

	<div class="form-filters form-design">
        <div class="row">
            <div class="col-lg-2">ID Obras </div>
            <div class="col-lg-10">
				<?php echo campo_texto( 'obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
            </div>
        </div> 

        <div class="row">
            <div class="col-lg-2">ID Pré-Obras </div>
            <div class="col-lg-10">
				<?php echo campo_texto( 'preid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
            </div>
        </div> 
        <div class="row">
            <div class="col-lg-2">Tipo de Ensino:</div>
            <div class="col-lg-10">
				<?php						
					$orgaos = $db->carregar("SELECT orgid, orgdesc FROM obras2.orgao where orgid in (1, 2, 3)");					
					$count = count($orgaos);
					for($i = 0; $i < $count; $i++){						
						echo '<input type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[$i]['orgid'] . '"/> ' . $orgaos[$i]["orgdesc"] . '</label>&nbsp;';						
					}
				?>
            </div>
        </div> 
        <div class="row">
            <div class="col-lg-2">Programa</div>
            <div class="col-lg-10">
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
            <div class="col-lg-2">Tipologia</div>
            <div class="col-lg-10">
	            <select name="tpoid[]" id="tpoid" class="chosen-select" multiple data-placeholder="Selecione">
	                <?php   $tipologiaObra = new TipologiaObra();
	                $param = array("orgid" => $_SESSION['obras2']['orgid']);
	                $dados = $tipologiaObra->listaCombo($param, false);
	                    foreach ($dados as $key) {
	                ?>
	                    <option value="<?php echo $key['codigo'] ?>" <?php if ($tpoid && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
	                <?php } ?>
	            </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">Unidade</div>
            <div class="col-lg-10">
			<?php 			
				$sql = "SELECT ee.entid as codigo,upper(ee.entnome) as descricao 
	                    FROM entidade.entidade ee
						INNER JOIN 
	                        obras2.empreendimento oi ON oi.entidunidade = ee.entid 
	                    WHERE empstatus = 'A'
						GROUP BY 
	                        ee.entnome, 
	                        ee.entid 
	                    ORDER BY 
	                        ee.entnome";
                $dados = $db->carregar($sql);
			?>
            <select name="entidunidade[]" id="entidunidade" class="chosen-select" multiple data-placeholder="Selecione">
                <?php

                foreach ($dados as $key) {
                    ?>
                    <option value="<?php echo $key['codigo'] ?>" <?php if ($entidunidade && in_array($key['codigo'], $tentidunidade)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
            </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">Situação da Obra </div>
            <div class="col-lg-10">
	            <?php
	                $sql_obr = "";
	                $dados = $db->carregar(getSqlEstadoDoc(TPDID_OBJETO));
	                $dados = $dados ? $dados : array();
	            ?>
	        	<select name="esdid_obr[]" class="chosen-select" multiple data-placeholder="Selecione">
	                <?php
	                    foreach ($dados as $key) {
	                ?>
	                <option value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['esdid_obr']) && in_array($key['codigo'], $_POST['esdid_obr'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
	                    <?php } ?>
	             </select>
            </div>
        </div> 
       <div class="row">
            <div class="col-lg-2">Recebeu o primeiro repasse? </div>
            <div class="col-lg-10">
                <input type="radio" name="repasse" id="" value="S" <?= ( $_POST["repasse"] == "S" ? "checked='checked'" : "" ) ?>/> Sim&nbsp;
                <input type="radio" name="repasse" id="" value="N" <?= ( $_POST["repasse"] == "N" ? "checked='checked'" : "" ) ?>/> Não&nbsp;
                <input type="radio" name="repasse" id="" value="T"  <?= ( $_POST["repasse"] == "T" || $_POST["repasse"] == "" ? "checked='checked'" : "" ) ?> /> Todas
            </div>
        </div> 
       <div class="row">
            <div class="col-lg-2">2ª Parcela Validada:</div>
            <div class="col-lg-10">
				<input type="radio" value="S" id="homologacao" name="homologacao" <?php if($_REQUEST["homologacao"] == "S") { echo "checked"; } ?> /> Sim&nbsp;
				<input type="radio" value="N" id="homologacao" name="homologacao" <?php if($_REQUEST["homologacao"] == "N") { echo "checked"; } ?> /> Não Validados&nbsp;
				<input type="radio" value="P" id="homologacao" name="homologacao" <?php if($_REQUEST["homologacao"] == "P") { echo "checked"; } ?> /> Não Analisados&nbsp;
				<input type="radio" value="" id="homologacao" name="homologacao"  <?php if($_REQUEST["homologacao"] == "") { echo "checked"; } ?> /> Todos
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">Data 2ª Parcela Validada:</div>
            <div class="col-lg-10">
	        	de&nbsp;
	        	<input type="text" id="dthomologacao_de" name="dthomologacao_de" value="<?php echo $dthomologacao_de;?>" size="15" maxlength="10" class="normal calendar" >
	       		 &nbsp;
	        	até&nbsp;
	        	<input type="text" id="dthomologacao_ate" name="dthomologacao_ate" value="<?php echo $dthomologacao_ate;?>" size="15" maxlength="10" class="normal calendar">
            </div>
        </div> 
        <div class="row">
            <div class="col-lg-2">Execução 25% Validada:</div>
            <div class="col-lg-10">
				<input type="radio" value="S" id="execucao25" name="execucao25" <?php if($_REQUEST["execucao25"] == "S") { echo "checked"; } ?> /> Sim&nbsp;
				<input type="radio" value="N" id="execucao25" name="execucao25" <?php if($_REQUEST["execucao25"] == "N") { echo "checked"; } ?> /> Não&nbsp; Validados&nbsp;
				<input type="radio" value="P" id="execucao25" name="execucao25" <?php if($_REQUEST["execucao25"] == "P") { echo "checked"; } ?> /> Não&nbsp; Analisados
				<input type="radio" value="" id="execucao25" name="execucao25"  <?php if($_REQUEST["execucao25"] == "") { echo "checked"; } ?> /> Todos
            </div>
        </div> 
        <div class="row">
            <div class="col-lg-2">Data 2ª Execução 25% Validada:</div>
            <div class="col-lg-10">
		    	de&nbsp;
		    	<input type="text" id="dtexecucao25_de" name="dtexecucao25_de" value="<?php echo $dtexecucao25_de;?>" size="15" maxlength="10" class="normal calendar" >
		        &nbsp;
		        até&nbsp;
		        <input type="text" id="dtexecucao25_ate" name="dtexecucao25_ate" value="<?php echo $dtexecucao25_ate;?>" size="15" maxlength="10" class="normal calendar">
            </div>
        </div> 
        <div class="row">
            <div class="col-lg-2">Execução 50% Validada:</div>
            <div class="col-lg-10">
				<input type="radio" value="S" id="execucao50" name="execucao50" <?php if($_REQUEST["execucao50"] == "S") { echo "checked"; } ?> /> Sim&nbsp;
				<input type="radio" value="N" id="execucao50" name="execucao50" <?php if($_REQUEST["execucao50"] == "N") { echo "checked"; } ?> /> Não&nbsp; Validados&nbsp;
				<input type="radio" value="P" id="execucao50" name="execucao50" <?php if($_REQUEST["execucao50"] == "P") { echo "checked"; } ?> /> Não Analisados&nbsp;
				<input type="radio" value="" id="execucao50" name="execucao50"  <?php if($_REQUEST["execucao50"] == "") { echo "checked"; } ?> /> Todos
            </div>
        </div> 
        <div class="row">
            <div class="col-lg-2">Possui anexo da homologação da licitação:</div>
            <div class="col-lg-10">
				<input type="radio" value="S" id="arqhanexosim" name="arqhanexo"  <?php if($_REQUEST["arqhanexo"] == "S") { echo "checked"; } ?> /> Sim&nbsp;
				<input type="radio" value="N" id="arqhanexonao" name="arqhanexo"  <?php if($_REQUEST["arqhanexo"] == "N") { echo "checked"; } ?> /> Não&nbsp;	
				<input type="radio" value="" id="arqahnexotodos" name="arqhanexo" <?php if($_REQUEST["arqhanexo"] == "") { echo "checked"; } ?> /> Todos&nbsp;
            </div>
        </div> 
        <div class="row">
            <div class="col-lg-2">Possui anexo da ordem de serviço:</div>
            <div class="col-lg-10">
				<input type="radio" value="S" id="arqanexosim" name="arqanexo"  <?php if($_REQUEST["arqanexo"] == "S") { echo "checked"; } ?> /> Sim&nbsp;
				<input type="radio" value="N" id="arqanexonao" name="arqanexo"  <?php if($_REQUEST["arqanexo"] == "N") { echo "checked"; } ?> /> Não&nbsp;	
				<input type="radio" value="" id="arqanexotodos" name="arqanexo" <?php if($_REQUEST["arqanexo"] == "") { echo "checked"; } ?> /> Todos
            </div>
        </div> 

	</div>

    <div class="row form-filters text-center">
        <div class="col">

            <button type="button" class="btn btn-success" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"><span class="glyphicon glyphicon-list-alt"></span> Visualizar </button>
	            <button type="button" class="btn btn-success" value="Visualizar XLS" onclick="obras_exibeRelatorioGeralXLS();"style="cursor: pointer;" ><span class="glyphicon glyphicon-download-alt"></span> Gerar Excel</button>
	        <?
	        $dir_relatorios = APPRAIZ . 'arquivos/obras2/relatorio/validacao';

	        if(file_exists($dir_relatorios)) {
	            $scanned_directory = array_diff(scandir($dir_relatorios), array('..', '.', '.svn'));

	            $file = array_pop($scanned_directory);
	            if ($file):
	                ?>
	                <button type="button" value="Donwload Relatório Compilado"
	                       onclick="downloadUltimoArquivo('<?= $file ?>');" style="cursor: pointer;"/></button>
	            <? endif; ?>
	        <?
	        }
	        ?>
        </div>
    </div> 




<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">


<tr>
	<td bgcolor="#CCCCCC"></td>
	<td bgcolor="#CCCCCC">
		<input type="button" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" />
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
	}*/
	
	// if(document.getElementById('prfid_campo_flag').value == "1"){
	// 	selectAllOptions( document.getElementById( 'prfid' ) );
	// }
	// if(document.getElementById('tooid_campo_flag').value == "1"){
	// 	selectAllOptions( document.getElementById( 'tooid' ) );
	// }
	
	formulario.target = 'RelatorioValidacoes';
	var janela = window.open( 'obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A', 'RelatorioValidacoes', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes' );
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
	// function onOffBloco( bloco )
	// {
	// 	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	// 	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	// 	var img = document.getElementById( bloco + '_img' );
	// 	var input = document.getElementById( bloco + '_flag' );
	// 	if ( div_on.style.display == 'none' )
	// 	{
	// 		div_on.style.display = 'block';
	// 		div_off.style.display = 'none';
	// 		input.value = '0';
	// 		img.src = '/imagens/menos.gif';
	// 	}
	// 	else
	// 	{
	// 		div_on.style.display = 'none';
	// 		div_off.style.display = 'block';
	// 		input.value = '1';
	// 		img.src = '/imagens/mais.gif';
	// 	}
	// }
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	// function onOffCampo( campo )
	// {
	// 	var div_on = document.getElementById( campo + '_campo_on' );
	// 	var div_off = document.getElementById( campo + '_campo_off' );
	// 	var input = document.getElementById( campo + '_campo_flag' );
	// 	if ( div_on.style.display == 'none' )
	// 	{
	// 		div_on.style.display = 'block';
	// 		div_off.style.display = 'none';
	// 		input.value = '1';
	// 	}
	// 	else
	// 	{
	// 		div_on.style.display = 'none';
	// 		div_off.style.display = 'block';
	// 		input.value = '0';
	// 	}
	// }
</script>
</form>

<script type="text/javascript">

    $(function(){

        jQuery(function($){
            $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro','Fevereiro','Mar&ccedil;o','Abril','Maio','Junho','Julho','Agosto','Setembro','Outubro','Novembro','Dezembro'],
                monthNamesShort: ['Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set','Out','Nov','Dez'],
                dayNames: ['Domingo','Segunda-feira','Ter&ccedil;a-feira','Quarta-feira','Quinta-feira','Sexta-feira','S&aacute;bado'],
                dayNamesShort: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                dayNamesMin: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''};
            $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
        });


        jQuery('.calendar').mask('99/99/9999');
        jQuery('.calendar').datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim:'drop'
        });
    });
</script>

<script type="text/javascript">
    function downloadUltimoArquivo(file){
        window.open( window.location + '&download=' + file );
    }
</script>
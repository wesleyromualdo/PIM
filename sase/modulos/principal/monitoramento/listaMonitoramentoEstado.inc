<?php

global $db;
$monitoramentoEstado = new FichamonitoramentoEstado();

// tratamento de filtros -------------------------

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
        switch ($chave) {		
            case 'filtroEstado':
                $filtros['a.estuf'] = $valor;
                break;
            case 'filtroPreenchido':
                if($valor == 1){
                    $filtroSemPadrao['preenchido'] = " usucpf is not null 
                                                    AND fmedatainclusao is not null 
                                                    AND fmepme is not null
                                                    AND (fmeperavalanual = 't' OR fmeperavalbianual = 't' OR fmeperavaltrianual = 't' OR fmeperavalquadrienal = 't' OR fmeperavalquinquenal = 't' OR fmeperavalnaoprevisto = 't' )
                                                    AND fmeperavalano1 is not null
                                                    AND fmecomcoordenadora is not null
                                                    AND fmecomnumanoatolegal is not null
                                                    AND fmeequipetecnica is not null
                                                    AND fmeequipeatolegal is not null
                                                    AND (fmeequipetelefone is not null OR fmeequipeemail is not null) ";
                }
                if($valor == 2){
                    $filtroSemPadrao['preenchido'] = "usucpf is null AND fmedatainclusao is null ";
                }
                break;
        }
// ver($filtros);
// para lista /
if(isset($_POST['funcao'])){
	switch ($_POST['funcao']){
		case 'xls':
			$monitoramentoEstado->montaListaXls( $filtros, $filtroSemPadrao, $order = "a.estuf ASC" );
			exit();
	}
}
		
		
// para exclusao
if( $_GET['excluir'] )
	if( $monitoramentoEstado->excluirAssessoramentoEstado( $_GET['fmeid'] ) ){
		echo "
			<script>
				alert('Ficha Técnica excluída com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/monitoramentoEstado&acao=A&aba=listaEstado';
			</script>";
		exit;
	}
// para exclusao /

// tratamento de filtros ------------------------- /

?>

<!-- ------------------------------- FORM ------------------------------- -->
<script>
	function geraRelatorioXls(){
		$('#funcao').val('xls');
		$('#formPesquisaLista').submit();
	}
</script>
<div id="formularioFiltros">
	<form class="form-horizontal" name="formPesquisaLista" id="formPesquisaLista" role="form" method="POST" action="">
        <input type="hidden" name="acao" id="acao"/>
		<input name="formulario" value="1" type="hidden"/>
		<input type="hidden" name="funcao" id="funcao" value=""/>
		<div class="form-group" id="divFiltroEstado">
			<label for="filtroEstado" class="col-sm-1 control-label">Estado:</label>
			<div class="col-sm-10 inputarea">
				<select name="filtroEstado" id="filtroEstado" onchange="javascript:if(this.value!=''){filtraMunicipios(this.value);}">
					<option value="">Filtrar estado</option>
					<?php 
					foreach( $estados as $chave => $estado ){ ?>
						<option <?=($estado['estuf']==$_POST['filtroEstado'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
					<?php }?>
				</select>
			</div>
		</div>
                <div class="form-group" id="divFiltroPreenchido">
                    <label for="filtroPreenchido" class="col-sm-1 control-label">Preenchido:</label>
                    <div class="col-sm-10 inputarea">
                        <select name="filtroPreenchido" id="filtroPreenchido">
                            <option value="">Todos</option>
                            <option value="1" <?= $_POST['filtroPreenchido'] == '1' ? 'selected' : '' ?>>Preenchido</option>
                            <option value="2" <?= $_POST['filtroPreenchido'] == '2' ? 'selected' : '' ?>>Não preenchido</option>
                        </select>
                    </div>
                </div>
		<div class="form-group">
			<label class="col-sm-1 control-label">&nbsp;</label>
			<div class="col-sm-10">
                            <input type="button" class="btn btn-primary" onclick="jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar" />
                            &nbsp;&nbsp;
                            <input type="button" class="btn btn-primary" onclick="window.location.href='sase.php?modulo=principal/monitoramento&acao=A&aba=listaEstado'" value="Listar Todos" />
                            &nbsp;&nbsp;
                            <input type="button" class="btn btn-primary" onclick="geraRelatorioXls()" value="Exportar XLS" />
                            </div>
		</div>
		<hr/>
	</form>
</div>

<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação","Estado","Preenchido");
$alinhamento = array('left','left','left');
//$larguras = array('40%','40%','20%');
$sql = $monitoramentoEstado->montaListaQuery( $filtros, $filtroSemPadrao, $order = "", $_SESSION['usucpf'] );
$sql = $monitoramentoEstado->trataRegrasPerfis( $_SESSION['usucpf'], $sql, PFLCOD_SASE_TECNICO );
$db->monta_lista($sql,$cabecalho,30,5,'N','','N','listaMonitoramentoEstado',$larguras,$alinhamento); ?>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>

	/**
	 * Manda para edicao de monitoramento
	 */
	function editarMonitoramentoEstado( fmeid ){
		window.location.href = '/sase/sase.php?modulo=principal/monitoramentoEstado&acao=A&aba=fichaEstado&fmeid=' + fmeid;
	}

	/**
	 * Excluir logicamente monitoramento
	 */
	 function excluirMonitoramentoEstado( fmeid ){
	 	if( confirm("Deseja realmente inativar essa Ficha?") ){
	 		window.location.href = '/sase/sase.php?modulo=principal/monitoramentoEstado&acao=A&aba=listaEstado&excluirEstado=1&fmeid=' + fmeid;
	 	}
	}


</script>
<!-- script js / -->
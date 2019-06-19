<?php
global $db;

$monitoramento = new Fichamonitoramento();

$filtros = array();

$pfls = arrayPerfil();

// tratamento de filtros -------------------------

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /

// municipio
$sql = "select * from territorios.municipio";
if( $_REQUEST['filtroEstado'] ) $sql .= " where estuf = '" . $_REQUEST['filtroEstado'] . "' ";
$sql .= " order by mundescricao asc ";
$municipios = $db->carregar($sql);

if( $_GET['filtroAjax'] ){ 
	ob_clean(); ?>
	<select name="filtroMunicipio" id="filtroMunicipio">
		<option value="">Filtrar município:</option>
		<?php 
		foreach( $municipios as $chave => $municipio ){ ?>
			<option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
		<?php }?>
	</select>
<?php die();}
// municipio /


// para lista
if( $_POST['formulario'] ){
	foreach( $_POST as $chave => $valor )
		if( $valor != '' )
            switch ($chave) {
                case 'filtroMunicipio':
                    $filtros['a.muncod'] = $valor;
                    break;
                case 'filtroEstado':
                    $filtros['a.estuf'] = $valor;
                    break;
                case 'filtroPreenchido':
                    if($valor == 1){
                        $filtroSemPadrao['preenchido'] = " usucpf is not null 
                        AND fmtdatainclusao is not null 
                        AND fmtpme is not null
                        AND (fmtperavalanual  = 't' OR fmtperavalbianual = 't' OR fmtperavaltrianual = 't' OR fmtperavalquadrienal = 't' OR fmtperavalquinquenal = 't' OR fmtperavalnaoprevisto = 't')
                        AND fmtperavalano1 is not null
                        AND fmtcomcoordenadora is not null
                        AND fmtcomnumanoatolegal is not null
                        AND fmtequipetecnica is not null
                        AND fmtequipeatolegal is not null
                        AND ( fmtequipetelefone  is not null OR fmtequipeemail  is not null ) ";
                    }
                    if($valor == 2){
                        $filtroSemPadrao['preenchido'] = "usucpf is null AND fmtdatainclusao is null ";
                    }
                    break;
            }
	$_SESSION['sase']['filtros'] = $filtros;
}
// para lista

// tratamento de filtros  ------------------------- /

// exporta xls
if(isset($_POST['funcao'])){
    switch ($_POST['funcao']){
        case 'xls':
            $monitoramento->montaListaXls( $filtros, $filtroSemPadrao, $order = "a.estuf, a.muncod ASC" );
        exit();
    }
}
// exporta xls /

// para exclusao
if( $_GET['excluir'] )
	if( $monitoramento->excluirLogicamenteMonitoramento( $_GET['fmtid'] ) ){
		echo "
			<script>
				alert('Ficha Técnica excluída com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/monitoramento&acao=A&aba=lista';
			</script>";
		exit;
	}
// para exclusao /



?>

<!-- ------------------------------- FORM ------------------------------- -->
<script>
	function geraRelatorioXls(){
        $('#formulario').val('1');
		$('#funcao').val('xls');
		$('#formPesquisaLista').submit();
	}
</script>
<div id="formularioFiltros">
	<form class="form-horizontal" name="formPesquisaLista" id="formPesquisaLista" role="form" method="POST">
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

            <div class="form-group" id="divFiltroMunicipio">
                <label for="filtroMunicipio" class="col-sm-1 control-label">Município:</label>
                <div class="col-sm-10 inputarea">
                    <?php if(!$_POST['filtroEstado']){ echo "Selecione um Estado"; }else{ ?>
                    <select name="filtroMunicipio" id="filtroMunicipio">
                        <option value="">Filtrar município</option>
                        <?php 
                        foreach( $municipios as $chave => $municipio ){ ?>
                            <option <?=($municipio['muncod']==$_POST['filtroMunicipio'])?'selected':''?> value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
                        <?php }?>
                    </select>
                    <?php } ?>
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
                        <input type="button" class="btn btn-primary" onclick="jQuery('[name=formulario]').val('1'); jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar" />
                        &nbsp;&nbsp;
                        <input type="button" class="btn btn-primary" onclick="jQuery('[name=formulario]').val('1'); window.location.href='sase.php?modulo=principal/monitoramento&acao=A&aba=lista'" value="Listar Todos" />
                        &nbsp;&nbsp;
                        <input type="button" class="btn btn-primary" onclick="geraRelatorioXls()" value="Exportar XLS" />
                    </div>
            </div>
            <hr/>
	</form>
</div>

<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação","Estado","Município","Preenchido");
$alinhamento = array('left','left','left','left');
$larguras = array('10%','30%','30%','30%');
$sql = $monitoramento->montaListaQuery( $filtros, $filtroSemPadrao, $order = "" );
$sql = $monitoramento->trataRegrasPerfis( $_SESSION['usucpf'], $sql, array(PFLCOD_SASE_TECNICO, PFLCOD_SASE_EXECUTIVO, PFLCOD_SASE_SUPERVISOR_GERAL) );
$db->monta_lista($sql,$cabecalho,30,5,'N','','N','listaMonitoramento',$larguras,$alinhamento); ?>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
    /**
     * Filtro de municipios com a mudanca do estado
     */
    function filtraMunicipios( estuf ){
            jQuery.ajax({
                    url:'',
                    type:'GET',
                    data:{filtroAjax:true,filtroEstado:estuf},
                    success: function( resposta ){
                            jQuery('#divFiltroMunicipio .inputarea').html( resposta );
                    }
            });
    }

    /**
     * Manda para edicao de monitoramento
     */
    function editarMonitoramento( fmtid ){
            window.location.href = '/sase/sase.php?modulo=principal/monitoramento&acao=A&aba=ficha&fmtid=' + fmtid;
    }

    /**
     * Excluir logicamente monitoramento
     */
     function excluirMonitoramento( fmtid ){
            if( confirm("Deseja realmente inativar essa Ficha Técnica?") ){
                    window.location.href = '/sase/sase.php?modulo=principal/monitoramento&acao=A&aba=lista&excluir=1&fmtid=' + fmtid;
            }
    }

</script>
<!-- script js / -->
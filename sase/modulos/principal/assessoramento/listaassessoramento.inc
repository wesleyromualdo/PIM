<?php
global $db;
$Assessoramento = new Assessoramento();

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
// situacao
    $sql = "select * from sase.situacaoassessoramento where stastatus = 'A' order by stacod ASC ";
$situacoes = $db->carregar( $sql );
// situacao /
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
                    case 'filtroSituacao':
                        $filtros['s.stacod'] = $valor;
                        break;
                    case 'filtroAssstatus':
                        $filtros['a.assstatus'] = $valor;
                        break;
                    case 'filtroLeiPne':
                        $filtros['a.assleipne'] = $valor;
                        break;
                    case 'filtroTermoAdesao':
                        $filtros['a.assadesao'] = $valor;
                }
        $_SESSION['sase']['filtros'] = $filtros;
    }
// para lista /

// exporta xls
if(isset($_POST['funcao'])){
	switch ($_POST['funcao']){
            case 'xls':
			$Assessoramento->montaListaXls( $filtros );
//			ver($Assessoramento,d);
                exit();
        }
    }
// exporta xls /

// para exclusao
if( $_GET['excluir'] )
	if( $Assessoramento->excluirLogicamenteAssessoramento( $_GET['assid'] ) ){
            echo "
			<script>
				alert('Assessoramento excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/assessoramento&acao=A&aba=lista';
			</script>";
            exit;
        }
// para exclusao /

// tratamento de filtros  ------------------------- /

    ?>

<!-- ------------------------------- FORM ------------------------------- -->
<script>
    function geraRelatorioXls() {
        $('#formulario').val('1');
        $('#funcao').val('xls');
        $('#formPesquisaLista').submit();
    }
</script>
<div id="formularioFiltros">
    <form class="form-horizontal" name="formPesquisaLista" id="formPesquisaLista" role="form" method="POST">
        <input type="hidden" name="arqid" id="arqid" >
        <input type="hidden" name="arqidpne" id="arqidpne" value="" >
        <input type="hidden" name="arqidadesao" id="arqidadesao" value="">
        <input type="hidden" name="acao" id="acao"/>
        <input name="formulario" value="1" type="hidden"/>
        <input type="hidden" name="funcao" id="funcao" value=""/>
        <div class="form-group" id="divFiltroEstado">
            <label for="filtroEstado" class="col-sm-1 control-label">Estado:</label>
            <div class="col-sm-10 inputarea">
                <select name="filtroEstado" id="filtroEstado" onchange="javascript:if (this.value != '') {
                            filtraMunicipios(this.value);
                        }">
                    <option value="">Filtrar estado</option>
<?php foreach ($estados as $chave => $estado) { ?>
                        <option <?= ($estado['estuf'] == $_POST['filtroEstado']) ? 'selected' : '' ?> value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
<?php } ?>
                </select>
            </div>
        </div>

        <div class="form-group" id="divFiltroMunicipio">
            <label for="filtroMunicipio" class="col-sm-1 control-label">Município:</label>
            <div class="col-sm-10 inputarea">
<?php if (!$_POST['filtroEstado']) {
    echo "Selecione um Estado";
} else { ?>
                    <select name="filtroMunicipio" id="filtroMunicipio">
                        <option value="">Filtrar município</option>
                    <?php foreach ($municipios as $chave => $municipio) { ?>
                            <option <?= ($municipio['muncod'] == $_POST['filtroMunicipio']) ? 'selected' : '' ?> value="<?= $municipio['muncod'] ?>"><?= $municipio['mundescricao'] ?></option>
                        <?php } ?>
                    </select>
                    <?php } ?>
            </div>
        </div>

        <div class="form-group" id="divFiltroSituacao">
            <label for="filtroSituacao" class="col-sm-1 control-label">Situação:</label>
            <div class="col-sm-10 inputarea">
                <select name="filtroSituacao" id="filtroSituacao">
                    <option value="">Filtrar situação</option>
<?php foreach ($situacoes as $chave => $situacao) { ?>
                        <option <?= ($situacao['stacod'] == $_POST['filtroSituacao']) ? 'selected' : '' ?> value="<?= $situacao['stacod'] ?>"><?= $situacao['stadsc'] ?></option>
<?php } ?>
                </select>
            </div>
        </div>

        <div class="form-group" id="divFiltroStatus">
            <label for="filtroLeiPne" class="col-sm-1 control-label">Lei PNE:</label>
            <div class="col-sm-10 inputarea">
                <select name="filtroLeiPne" id="filtroLeiPne">
                    <option value="">Todos</option>
                    <option value="1" <?= $_POST['filtroLeiPne'] == '1' ? 'selected' : '' ?>>Com lei</option>
                    <option value="2" <?= $_POST['filtroLeiPne'] == '2' ? 'selected' : '' ?>>Sem lei</option>
                </select>
            </div>
        </div>
        <div class="form-group" id="divFiltroStatus">
            <label for="filtroTermoAdesao" class="col-sm-1 control-label">Termo Adesão:</label>
            <div class="col-sm-10 inputarea">
                <select name="filtroTermoAdesao" id="filtroTermoAdesao">
                    <option value="">Todos</option>
                    <option value="1" <?= $_POST['filtroTermoAdesao'] == '1' ? 'selected' : '' ?>>Com termo</option>
                    <option value="2" <?= $_POST['filtroTermoAdesao'] == '2' ? 'selected' : '' ?>>Sem termo</option>
                </select>
            </div>
        </div>

        <!--		<div class="form-group" id="divFiltroStatus">-->
        <!--			<label for="filtroAssstatus" class="col-sm-1 control-label">Status:</label>-->
        <!--			<div class="col-sm-10 inputarea">-->
        <!--				<select name="filtroAssstatus" id="filtroAssstatus">-->
        <!--					<option value="">Status</option>-->
        <!--					<option --><?php // echo($_POST['filtroAssstatus']=='A')?'selected':'' ?><!-- value="A">Ativo</option>-->
        <!--					<option --><?php //echo($_POST['filtroAssstatus']=='I')?'selected':'' ?><!-- value="I">Inativo</option>-->
        <!--				</select>-->
        <!--			</div>-->
        <!--		</div>-->

        <div class="form-group">
            <label class="col-sm-1 control-label">&nbsp;</label>
            <div class="col-sm-10">
                <input type="button" class="btn btn-primary" onclick="jQuery('[name=formulario]').val('1');
                        jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar" />
                &nbsp;&nbsp;
                <input type="button" class="btn btn-primary" onclick="jQuery('[name=formulario]').val('1');
                        window.location.href = 'sase.php?modulo=principal/assessoramento&acao=A&aba=lista'" value="Listar Todos" />
                &nbsp;&nbsp;
                <input type="button" class="btn btn-primary" onclick="geraRelatorioXls()" value="Exportar XLS" />
            </div>
        </div>
        <hr/>
    </form>
</div>

<!-- / ------------------------------- FORM ------------------------------- -->

<?php
$cabecalho = array("Ação", "Estado", "Município", "Situação", "Lei PNE", "Adesão", "Status");
$alinhamento = array('left', 'left', 'left', 'left', 'center', 'left', 'left');
$larguras = array('10%', '20%', '20%', '20%', '5%', '5%', '10%');
$sql = $Assessoramento->montaListaQuery($filtros);
// ver($sql,$filtros,d);
$sql = $Assessoramento->trataRegrasPerfis($_SESSION['usucpf'], $sql);
$db->monta_lista($sql, $cabecalho, 30, 5, 'N', '', 'N', 'listaAssessoramento', $larguras, $alinhamento);
?>

<style>
    .modal.modal-wide .modal-dialog {
        width: 80%;
    }
    .modal-history {
        position: fixed;
        transform: translate(0,0);
        width: auto; left: 0; right: 0;
        height: auto; top: 0; bottom: 0;
        z-index: 990; /* display above everything else */
        padding: 20px; 
        overflow-y: scroll;
    }
</style>
<div class="container">
    <div class="fade modal" id="modalHistorico" tabindex="-1" role="dialog">
        <div class="modal-history modal-lg" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h4 class="modal-title">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Histórico dos arquivos Lei PNE e Termo de Adesão</h4>
                </div>
                <div class="modal-header">
                    <h5 class="modal-title">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Município:
                        <b id="titulo_tabela"></b>
                    </h5>
                </div>
                <br>
                <div class="modal-body painel_po_body" id="tabela_historico" > </div>
                <br>
                <div class="col-lg-12">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                    <button type="button" class="btn btn-default" data-dismiss="modal">Voltar</button>
                </div>
                <br>
            </div><!-- /.modal-content -->
        </div><!-- /.modal-history -->
    </div><!-- /.fade modal-->
</div>
<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
    /**
     * Filtro de municipios com a mudanca do estado
     */
    function filtraMunicipios(estuf) {
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, filtroEstado: estuf},
            success: function(resposta) {
                jQuery('#divFiltroMunicipio .inputarea').html(resposta);
            }
        });
    }

    /**
     * Manda para edicao de assessoramento
     */
    function editarAssessoramento(assid) {
        window.location.href = '/sase/sase.php?modulo=principal/assessoramento&acao=A&aba=edicao&assid=' + assid;
    }

    /**
     * Excluir logicamente assessoramento
     */
    function excluirAssessoramento(assid) {
        if (confirm("Deseja realmente inativar esse Assessoramento?")) {
            window.location.href = '/sase/sase.php?modulo=principal/assessoramento&acao=A&aba=lista&excluir=1&assid=' + assid;
        }
    }

    function downloadArquivo(tipo, arqid){
        jQuery('[name=acao]').val('download_arquivo');
        jQuery('[name=formulario]').val("");
        if(tipo == 'assleipne'){
            jQuery('[name=arqidpne]').val(arqid);
        }
        if(tipo == 'assadesao'){
            jQuery('[name=arqidadesao]').val(arqid);
        }
            //jQuery('[name=arqid]').val(arqid);
            jQuery('[name=formPesquisaLista]').submit();
        }

    function downloadAllArquivo(){
        jQuery('[name=acao]').val('download_zip');
        jQuery('[name=formulario]').val("");
        jQuery('[name=formPesquisaLista]').submit();
    }
    
    function downloadAnexo (tipo,arqid){
        var url = window.location.href;

        if (url.charAt(url.length-1) == '#'){
            url = url.substring(0, url.length-1) + '&tipo='+ tipo +'&acao_download=downloadAnexo&arqid=' + arqid;
        }else{
            url = window.location.href+'&tipo='+ tipo +'&acao_download=downloadAnexo&arqid=' + arqid;
        }
        window.location=url;
        return;
    }
    
    
    /**
     * Manda para historico arquivos assessoramento
     */
    function historicoAssessoramento(assid, estuf, muncod, mundescricao) {
        jQuery.ajax({
            url: '',
            type: "POST",
            data: {
                acao: 'historico', assid: assid, estuf: estuf, muncod: muncod
            },
            success: function(resposta) {
                jQuery("#titulo_tabela").html(estuf+' - '+trim(mundescricao));
                jQuery("#tabela_historico").html(resposta);
                jQuery('#modalHistorico').modal('show');
            }
        });
    }

</script>
<!-- script js / -->
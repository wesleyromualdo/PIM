<?php

global $db;
$Assessoramento = new AssessoramentoEstado();

// tratamento de filtros -------------------------

// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /


// situacao
$sql = "select * from sase.situacaoassessoramento where stastatus = 'A' order by stacod ASC ";
$situacoes = $db->carregar( $sql );
// situacao /

// para lista
if( $_POST['formulario'] )
foreach( $_POST as $chave => $valor )
	if( $valor != '' )
        switch ($chave) {		
            case 'filtroEstado':
                    $filtros['a.estuf'] = $valor;
                    break;
            case 'filtroSituacao':
                    $filtros['a.stacod'] = $valor;
                    break;
            case 'filtroAssstatus':
                    $filtros['a.asestatus'] = $valor;
                    break;
            case 'filtroLeiPne':
                    $filtros['a.aseleipne'] = $valor;
                break;
            case 'filtroTermoAdesao':
                    $filtros['a.aseadesao'] = $valor;
        }
// ver($filtros);
// para lista /
if(isset($_POST['funcao'])){
	switch ($_POST['funcao']){
		case 'xls':
			$Assessoramento->montaListaXls( $filtros );
			exit();
	}
}
		
		
// para exclusao
if( $_GET['excluir'] )
	if( $Assessoramento->excluirAssessoramentoEstado( $_GET['aseid'] ) ){
		echo "
			<script>
				alert('Assessoramento excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/assessoramento&acao=A&aba=lista';
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
        <input type="hidden" name="arqid" id="arqid" >
        <input type="hidden" name="arqidpne" id="arqidpne" value="" >
        <input type="hidden" name="arqidadesao" id="arqidadesao" value="">
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

		<div class="form-group" id="divFiltroSituacao">
			<label for="filtroSituacao" class="col-sm-1 control-label">Situação:</label>
			<div class="col-sm-10 inputarea">
				<select name="filtroSituacao" id="filtroSituacao">
					<option value="">Filtrar situação</option>
					<?php 
					foreach( $situacoes as $chave => $situacao ){ ?>
						<option <?=($situacao['stacod']==$_POST['filtroSituacao'])?'selected':''?> value="<?=$situacao['stacod']?>"><?=$situacao['stadsc']?></option>
					<?php }?>
				</select>
			</div>
		</div>

        <div class="form-group" id="divFiltroStatus">
            <label for="filtroSituacao" class="col-sm-1 control-label">Lei PNE:</label>
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
<!--					<option --><?php //echo($_POST['filtroAssstatus']=='A')?'selected':''?><!-- value="A">Ativo</option>-->
<!--					<option --><?php //echo($_POST['filtroAssstatus']=='I')?'selected':''?><!-- value="I">Inativo</option>-->
<!--				</select>-->
<!--			</div>-->
<!--		</div>-->

		<div class="form-group">
			<label class="col-sm-1 control-label">&nbsp;</label>
			<div class="col-sm-10">
				<input type="button" class="btn btn-primary" onclick="jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar" />
				&nbsp;&nbsp;
				<input type="button" class="btn btn-primary" onclick="window.location.href='sase.php?modulo=principal/assessoramento&acao=A&aba=lista'" value="Listar Todos" />
				&nbsp;&nbsp;
				<input type="button" class="btn btn-primary" onclick="geraRelatorioXls()" value="Exportar XLS" />
				</div>
		</div>
		<hr/>
	</form>
</div>

<!-- / ------------------------------- FORM ------------------------------- -->

<?php 
$cabecalho = array("Ação","Estado","Situação", "Lei PNE","Adesão","Status");
$alinhamento = array('left','left','left', 'center','left','left');
$larguras = array('20%','20%','30%','10%','10%','10%');
// ver($Assessoramento->montaListaQuery( $filtros ),d);
$db->monta_lista($Assessoramento->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaAssessoramento',$larguras,$alinhamento); ?>

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
                    <h5 class="modal-title">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Estado:
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
     * Manda para edicao de assessoramento
     */
    function editarAssessoramentoEstado( aseid ){
            window.location.href = '/sase/sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=edicao&aseid=' + aseid;
    }

    /**
     * Excluir logicamente assessoramento
     */
     function excluirAssessoramentoEstado( aseid ){
            if( confirm("Deseja realmente inativar esse Assessoramento?") ){
                    window.location.href = '/sase/sase.php?modulo=principal/assessoramentoEstado&acao=A&aba=lista&excluir=1&aseid=' + aseid;
            }
    }

    function downloadArquivo(tipo,arqid){
        jQuery('[name=acao]').val('download_arquivo');
        jQuery('[name=formulario]').val("");
        if(tipo == 'aseleipne'){
            jQuery('[name=arqidadesao]').val('');
            jQuery('[name=arqidpne]').val(arqid);
            
        }
        if(tipo == 'aseadesao'){
            jQuery('[name=arqidpne]').val('');
            jQuery('[name=arqidadesao]').val(arqid);
        }
        jQuery('[name=formPesquisaLista]').submit();
    }
    
     /**
     * Manda para historico arquivos assessoramento
     */
    function historicoAssessoramento(aseid, estuf, estdescricao) {
        jQuery.ajax({
            url: '',
            type: "POST",
            data: {
                acao: 'historico', aseid: aseid, estuf: estuf
            },
            success: function(resposta) {
                jQuery("#titulo_tabela").html(trim(estdescricao));
                jQuery("#tabela_historico").html(resposta);
                jQuery('#modalHistorico').modal('show');
            }
        });
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

</script>
<!-- script js / -->
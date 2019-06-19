<?php


$url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=listar_regra_termo';

if( $_REQUEST['requisicao'] == 'delete' ){
    ob_clean();
    $db->executar("UPDATE par3.configuracao_termo SET cotstatus = 'I' WHERE cotid = {$_REQUEST['cotid']}");
    $db->commit();
    
    $urlM = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=listar_regra_termo';
    simec_redirecionar($urlM, 'success', 'Dados excluído com sucesso!');
    exit();
}

$sqlObjeto = "SELECT intoid AS codigo, intodsc AS descricao FROM par3.iniciativa_tipos_objeto WHERE intostatus = 'A' ORDER BY intodsc";
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Regras de Termos</h2>
            </div>
        </div>
    </div>
    <?php echo $simec->tab ( criaAbaRegrasTermos(), $url );?>
	<div class="ibox-content">
		<div class="row">
    		<form name="formDocumento" id="formDocumento" class="form form-horizontal" method="post">
    			<input type="hidden" name="requisicao" id="requisicao" value="">
    			<?php echo $simec->select('intoid', 'Tipo de Objeto', $arrDados['intoid'], $sqlObjeto, array('maxlength' => '255'), array('input-size' => '7')); ?>
			</form>
		</div>
	</div>
	<div class="ibox-footer">
		<div class="center">
    		<button type="button" class="btn btn-primary" id="pesquisar-regras"><i class="fa fa-search"></i> Pesquisar</button>
    		<button type="reset" class="btn btn-success btn-limpar" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
		</div>
	</div>
	<div class="ibox-content">
        <div id="documentoListagem">
            <?php
            if( $_REQUEST['intoid'] ){
                $where = " and io.intoid = {$_REQUEST['intoid']} ";
            }
            $sql = "SELECT ct.cotid, io.intoid||' - '||io.intodsc as objetos, ct.cotdiasinisolic, ct.cotdiasfimsolic, u.usunome, to_char(ct.cotdata, 'DD/MM/YYYY') AS datas 
                    FROM par3.configuracao_termo ct 
                    	INNER JOIN par3.iniciativa_tipos_objeto io ON io.intoid = ct.intoid
                    	INNER JOIN seguranca.usuario u ON u.usucpf = ct.usucpf
                    WHERE ct.cotstatus = 'A' $where";
            
            $listagemSimec = new Simec_Listagem();
            $arrayCabecalho = array('Objeto', 'Dias Inicial', 'Dias Final', 'Usúario', 'Data');
            $esconderColunas = array('cotid');
            $listagemSimec->setCabecalho($arrayCabecalho);
            
            $listagemSimec->esconderColunas($esconderColunas);
            $listagemSimec->turnOnPesquisator();
            $listagemSimec->setQuery($sql);
            $listagemSimec->addAcao('delete', 'deletarRegraTermo');
            $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagemSimec->setTamanhoPagina(100);
            $listagemSimec->setCampos($arrayCabecalho);
            
            $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
            ?>
        </div>
    </div>
</div>
<script type="text/javascript">
jQuery(".btn-limpar").click(function () {
	window.location.href = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=listar_regra_termo';
});

$("#pesquisar-regras").click(function () {
    jQuery('[name="requisicao"]').val('pesquisar');
    jQuery('[name="formDocumento"]').submit();
});

function deletarRegraTermo(cotid){
	window.location.href = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=listar_regra_termo&cotid='+cotid+'&requisicao=delete'
}
</script>



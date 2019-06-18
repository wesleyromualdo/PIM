<script type="text/javascript">

function excluirLicitacao(lieid) {
    if(confirm('Tem certeza que deseja remover esta Licitação?')){
        var caminho = window.location.href;
        var action = '&requisicao=excluirLicitacao&lieid=' + lieid;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (data) {
                alert(data);
                window.location.href = window.location;
            }
        });
    }
}
function editarLicitacao(lieid) {
	window.open("obras2.php?modulo=principal/popUpLicitacaoExtraExecFinanceira&acao=O&edit=true&lieid="+lieid, "popUpCadastroConstrutora", "width=780,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    return false;
}
function visualizarLicitacao(lieid) {
	window.open("obras2.php?modulo=principal/popUpLicitacaoExtraExecFinanceira&acao=O&visualizar=true&lieid="+lieid, "popUpCadastroConstrutora", "width=780,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    return false;
}

function incluirLicitacaoPropria() {
    window.open("obras2.php?modulo=principal/popUpLicitacaoExtraExecFinanceira&acao=O", "popUpCadastroConstrutora", "width=780,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
}

function abreDeclaracao(){
	return windowOpen( '?modulo=principal/cadDeclaracaoConformidade&acao=A','blank','height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function visualizaFase(flcid){
	return windowOpen( '?modulo=principal/vincFaseLicitacao&acao=V&flcid='+flcid,'blank','height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

DownloadArquivo = function(arqid){
	window.location = '?modulo=principal/exibeLicitacao&acao=A&requisicao=download&arqid='+arqid;
}
</script>

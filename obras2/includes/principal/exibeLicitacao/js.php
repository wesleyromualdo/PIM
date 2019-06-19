
<script type="text/javascript">
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


<script type="text/javascript">

    function incluirNotaFiscal() {
        window.open("obras2.php?modulo=principal/popUpAcompanhamentoNotaFiscal&acao=O", "popUpAcompanhamentoNotaFiscal", "width=900,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }

    function downloadArquivo( arqid ){
        window.location='?modulo=principal/listaExecFinanceira&acao=O&requisicao=download&arqid=' + arqid
    }

    function excluirNotaFiscal(ntfid) {
        if(confirm('Tem certeza que deseja remover esta Nota Fiscal?')){
            var caminho = window.location.href;
            var action = '&requisicao=excluirNotaFiscal&ntfid=' + ntfid;
            $.ajax({
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

    function editarNotaFiscal(ntfid) {
        window.open("obras2.php?modulo=principal/popUpAcompanhamentoNotaFiscal&acao=O&ntfid=" + ntfid + "&tipo=E", "popUpAcompanhamentoNotaFiscal", "width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }

    function visualizarMedicao(medid) {
        window.open("obras2.php?modulo=principal/popUpVisualizarMedicao&acao=O&medid="+medid, "popUpVisualizarMedicao", "width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }

</script>


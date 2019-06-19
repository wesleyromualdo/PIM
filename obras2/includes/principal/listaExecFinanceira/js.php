
<script type="text/javascript">

    function incluirMedicao() {
        window.open("obras2.php?modulo=principal/popUpAcompanhamentoMedicao&acao=O", "popUpAcompanhamentoMedicao", "width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }

    function incluirContrato() {
        window.open("obras2.php?modulo=principal/popUpConstrutoraExtraExecFinanceira&acao=O", "popUpCadastroConstrutora", "width=780,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }

    function excluirConstrutora(cexid) {
        if(confirm('Tem certeza que deseja remover esta Construtora?')){
            var caminho = window.location.href;
            var action = '&requisicao=excluirConstrutora&cexid=' + cexid;
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

    function downloadArquivo( arqid ){
        window.location='?modulo=principal/listaExecFinanceira&acao=O&requisicao=download&arqid=' + arqid
    }

    function excluirMedicao(medid) {
        if(confirm('Tem certeza que deseja remover esta Medição?')){
            var caminho = window.location.href;
            var action = '&requisicao=excluirMedicao&medid=' + medid;
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

    function editarMedicao(medid) {
        window.open("obras2.php?modulo=principal/popUpAcompanhamentoMedicao&acao=O&medid=" + medid, "popUpAcompanhamentoMedicao", "width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }

    function editarConstrutora(cexid) {
        window.open("obras2.php?modulo=principal/popUpConstrutoraExtraExecFinanceira&acao=O&cexid=" + cexid, "popUpCadastroConstrutora", "width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }
</script>


<script type="text/javascript">

    $(document).ready(function () {
        $("#mostraDivDadosAntigos").click(function () {
            $("#dados_antigos").toggle("fast", function () {
                if ($("#dados_antigos").is(':visible')) {
                    $("#mostraDivDadosAntigos").text('[-] Esconde Dados Antigos');
                } else {
                    $("#mostraDivDadosAntigos").text('[+] Mostra Dados Antigos');
                }
            });
        });

        $('.download').click(function () {
            var id = $(this).attr('id');
            downloadArquivo(id);
        });
    });

    function incluirPagamentos() {
        window.open("obras2.php?modulo=principal/popUpPagamentoExecFinanceira&acao=O", "popUpPagamentoExecFinanceira", "width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }


    function downloadArquivo(arqid) {

        window.location = '?modulo=principal/listaExecFinanceira&acao=O&requisicao=download&arqid=' + arqid
    }

    function excluirPagamentos(pgtid) {

        if (confirm('Tem certeza que deseja remover este registro de Pagamentos?')) {
            var caminho = window.location.href;
            var action = '&requisicao=excluirPagamento&pgtid=' + pgtid;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (data) {
                    window.location.href = "obras2.php?modulo=principal/listaExecFinanceiraPagamentos&acao=O";
                }
            });
        }
    }

    function visualizanota(ntfid) {
        window.open("obras2.php?modulo=principal/popUpVisualizaNotaFiscal&acao=O&ntfid=" + ntfid, "popUpAcompanhamentoMedicao", "width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }


    function editarPagamentos(pgtid) {

        window.open("obras2.php?modulo=principal/popUpPagamentoExecFinanceira&acao=O&pgtid=" + pgtid, "popUpAcompanhamentoMedicao", "width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
        return false;
    }


</script>

<script type="text/javascript">
    function incluirContrato() {
        window.open("obras2.php?modulo=principal/popUpConstrutoraExtraExecFinanceira&acao=O", "popUpCadastroConstrutora", "width=780,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }

    function editarConstrutora(cexid) {
        window.open("obras2.php?modulo=principal/popUpConstrutoraExtraExecFinanceira&acao=O&cexid=" + cexid, "popUpCadastroConstrutora", "width=780,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }

    function inserirAditivoExtra(obrid, entid) {
        window.open("obras2.php?modulo=principal/popUpAditivoExtra&acao=O&obrid=" + obrid + "&entid=" + entid, "popUpAditivoExtra", "width=780,height=500,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
    }

    function editarAditivoExtra(obrid, entid, crtid) {
        window.open("obras2.php?modulo=principal/popUpAditivoExtra&acao=O&crtid="+ crtid +"&obrid=" + obrid + "&entid=" + entid, "popUpAditivoExtra", "width=780,height=500,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left=" + (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 600) / 2);
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
        window.location='?modulo=principal/listaExecFinanceiraContratacao&acao=O&requisicao=download&arqid=' + arqid
    }

    function excluirAditivoExtra(crtid){
        if(confirm('Tem certeza que deseja remover este Aditivo?')){
            var caminho = window.location.href;
            var action = '&requisicao=excluirAditivo&crtid=' + crtid;
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


    function inserirEmpresa(entidempresa, tipo,obrid) {
        if (entidempresa) {
            return windowOpen('?modulo=principal/inserir_empresa&acao=A&busca=entnumcpfcnpj&tipo=' + tipo + '&entid=' + entidempresa + '&obrid=' + obrid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        } else {
            return windowOpen('?modulo=principal/inserir_empresa&acao=A&tipo=' + tipo + '&obrid=' + obrid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

</script>

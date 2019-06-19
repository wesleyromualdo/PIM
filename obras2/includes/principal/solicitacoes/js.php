
<script type="text/javascript">
    function aprovaSolicitacao(slcid){
        if(confirm('Você deseja prosseguir com a aprovação?')){
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=aprova_solicitacao&slcid=" + slcid,
                async: false,
                dataType: 'json',
                success: function(res) {
                    alert(res.mensagem);
                    if(res.retorno){
                        window.location.href = 'obras2.php?modulo=principal/solicitacoes&acao=A';
                    }
                },
                error: function (jqXHR, textStatus, errorThrown){
                }
            });
        }

    }

    function abreSolicitacao(obrid, tipo, slcid){
        var url = "/obras2/obras2.php?modulo=principal/solicitacao&acao=A" +
            "&obrid=" + obrid +
            "&slcid="+ slcid +
            "&tslid[]=" + tipo;
        popup1 = window.open(
            url,
            "Solicitação",
            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
        );

        return false;
    }

    function printQuestionario(obrid,qrpid,slcid,queid) {
        var url = "/obras2/obras2.php?modulo=principal/questionarioImpressaoSolicitacoes&acao=A&qrpid="+qrpid+"&obrid="+obrid+"&slcid="+slcid+"&queid="+queid;
        popup2 = window.open(
            url,
            "_blank",
            "width=1000,height=650,scrollbars=yes,scrolling=no,resizebled=no"
        );
    }
</script>


<script type="text/javascript">
    function DownloadArquivo(arqid){
        $('#requisicao').val('download');
        $('#arqid').val(arqid);
        $('#form-projeto').unbind('submit');
        $('#form-projeto').submit();
    }

    function alteraProjeto(item){
        if(item.is(":checked")){;
            $('#'+item.attr('name')).hide();
            $('#'+item.attr('name')+' input[type="file"]').attr('disabled', 'disabled');
            $('#'+item.attr('name')+' input[type="file"]').val('');
            item.parent().parent().find('input[type=file]').show();
            item.parent().parent().find('a').show();
        } else {
            $('#'+item.attr('name')).show();
            $('#'+item.attr('name')+' input[type="file"]').removeAttr('disabled');
            item.parent().parent().find('input[type="file"]').hide();
            item.parent().parent().find('a').hide();
        }
    }

    jQuery(function () {
        $('[data-toggle="tooltip"]').tooltip();
        $('[data-toggle="popover"]').popover({html:true});

        $('#form-projeto').submit(function(){
            for(x=0; x < $('img.obrig').prev('input[type=text]').length; x++){
                if($('img.obrig').prev('input').eq(x).val() == ''){
                    alert('Preencha todos os campos obrigatórios.');
                    return false;
                }
            }

            for(x=0; x < $('img.obrig').prev('input[type=file]').length; x++){
                if($('img.obrig').prev('input[type=file]').eq(x).val() == '' && $('img.obrig').prev('input[type=file]').eq(x).next().next().length == 0) {
                    alert('Preencha todos os campos obrigatórios.');
                    return false;
                }
            }

        });
    })

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


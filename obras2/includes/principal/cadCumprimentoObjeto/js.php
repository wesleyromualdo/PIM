<script>
    $(document).ready(function () {
        $('#janela').on('click', function () {
            var preid = '<?=$obra->preid?>';
            var obrid_par3 = '<?=$obra->obrid_par3?>';
            if (preid != '') {
                janela('/par/par.php?modulo=principal/programas/proinfancia/visualizarPreObra&acao=A&preid=' + preid + '&div=planilha', 800, 600, 'preobra');
            } else if (obrid_par3 != '') {
                janela('/par3/par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=planilha_orcamentaria&inuid=3205&inpid=35872&obrid=3047871&toReturn=formListaObra&indCabechalho=0', 800, 600, 'preobra');
            } else {
                alert('Esta obra não possui pré-obra cadastrada.');
            }
        });

        $('.esclarecimento').click(function () {
            var qstId = $(this).attr('qstId');
            var qcodesclarecimento = $('#qcodesclarecimento_' + qstId).val();
            $('#textoModalEsclarecimento').val(qcodesclarecimento);
            $('#modalEsclarecimento').show();
            $("#modalEsclarecimento").dialog({
                resizable: true,
                width: 800,
                modal: true,
                show: {effect: 'drop', direction: "down"},
                buttons: {
                    "OK": function () {
                        $('#qcodesclarecimento_' + qstId).val($('#textoModalEsclarecimento').val());
                        $(this).dialog("close");
                    }
                }
            });
        });
    });

    function excluirArquivo(arqid) {
        if (confirm('Você deseja realmente excluir este arquivo?')) {
            window.location = '?modulo=principal/cadCumprimentoObjeto&acao=A&excluir=S&arqid=' + arqid;
        }
    }
    
    $(function () {
        $('#liberaCOO').click(function (e) {
            location.href = location.href + '&liberacoo=true';
        })
    });



    $(document).ready(function () {
        $('#div_dialog_workflow').next().css('width', '100%');
    });


    function addConstrutora() {

        var originalFormNovaConstrutora = $("#formNovaConstrutora").html();

        $("#dialog").dialog({
            resizable: false,
            height: 260,
            width: 520,
            modal: true,
            buttons: {
                "Salvar": function () {

                    if ($("#razaosocial").val() == "") {
                        $("#msgNovaConstrutora").show();
                        $("#msgNovaConstrutora").html("Informe a razão social.");
                        $("#razaosocial").focus();
                        return false;
                    }

                    if ($("#cnpj").val() == "") {
                        $("#msgNovaConstrutora").show();
                        $("#msgNovaConstrutora").html("Informe o CNPJ.");
                        $("#cnpj").focus();
                        return false;
                    } else if ($("#cnpj").val().length < 18) {
                        $("#msgNovaConstrutora").show();
                        $("#msgNovaConstrutora").html("O CNPJ informado possui formato inválido.");
                        $("#cnpj").focus();
                        return false;
                    }

                    $.ajax({
                        type: "post",
                        url: window.location.href,
                        data: {
                            "req": "novaConstrutora",
                            "razaosocial": $("#razaosocial").val(),
                            "cnpj": $("#cnpj").val()
                        },
                        async: false,
                        success: function (data) {
                            var retorno = (data == "true") || false;
                            if (!retorno) {
                                $("#msgNovaConstrutora").show();
                                $("#msgNovaConstrutora").html("Houve um problema ao cadastrar a construtora. Tente novamemente mais tarde.");
                                return false;
                            }

                            $("#dialog").dialog("close");
                            alert("Construtora inserida com sucesso.");
                            window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';
                        }
                    });
                },
                "Cancelar": function () {
                    $("#formNovaConstrutora").html(originalFormNovaConstrutora);
                    $(this).dialog("close");
                }
            }
        });
    }

</script>
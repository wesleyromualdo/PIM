<script type="text/javascript">

        jQuery.noConflict();

        function DownloadArquivoFase(arqid) {
            window.location = '?modulo=principal/cadValidacao&acao=A&requisicao=download&arqid=' + arqid;
        }

        function salvarValidacao() {
            var form = document.getElementById('formulario');
            var msg = true;
            for (i = 0; i < form.length; i++) {
                if (formulario.elements[i].type == 'radio') {
                    if (formulario.elements[i].checked == true) {
                        msg = false;
                    }
                }
            }

            if (msg) {
                alert('Selecione uma validação!');
                return false;
            }
            if (jQuery('#vldstatushomologacao_n').attr('checked') && jQuery('#vldobshomologacao').val() == '') {
                alert('É obrigatório a justificativa da validação da homologação!');
                jQuery('#vldobshomologacao').focus();
                return false;
            }
            if (jQuery('#vldstatus25exec_n').attr('checked') && jQuery('#vldobs25exec').val() == '') {
                alert('É obrigatório a justificativa da <?=$label25?>!');
                jQuery('#vldobs25exec').focus();
                return false;
            }
            if (jQuery('#vldstatus50exec_n').attr('checked') && jQuery('#vldobs50exec').val() == '') {
                alert('É obrigatório a justificativa da <?=$label50?>!');
                jQuery('#vldobs50exec').focus();
                return false;
            }

            document.getElementById('requisicao').value = 'salvar';
            document.getElementById('formulario').submit();
        }

        function popupChecklist(tipo, ckfid, acao) {
            var url = "/obras2/obras2.php?modulo=principal/cadValidacao&acao=A";
            var requisicao = 'checklist';
            var obrid = <?php echo $_SESSION['obras2']['obrid']?>;
            if (acao.trim() === '') {
                acao = 'cadastro';
            }

            url = url + '&requisicao=' + requisicao + '&tipo=' + tipo + '&obrid=' + obrid + '&ckfid=' + ckfid + '&acao=' + acao;

            popup1 = window.open(
                url,
                "_blank",
                "width=1200,height=750,scrollbars=yes,scrolling=no,resizebled=no"
            );
            
            popup1.opener.notclose = popup1;
        }


        function editarChecklist(tipo, ckfid) {
            var acao = 'editar';
            popupChecklist(tipo, ckfid, acao);
        }

        function duplicarChecklist(tipo, ckfid) {
            var acao = 'duplicar';
            popupChecklist(tipo, ckfid, acao);
        }

        function excluirChecklist(tipo, ckfid) {
            var acao = 'excluir';
            var r = window.confirm('Você realmente deseja excluir o Checklist ' + ckfid + ' ?');
            if (r) {
                popupChecklist(tipo, ckfid, acao);
                reload();
            }
        }

        function reload() {
            window.location.reload();
        }

        jQuery(function () {

            jQuery('#vldobshomologacao, #vldobs25exec, #vldobs50exec').parent().hide();

            jQuery('#vldstatushomologacao_n, #vldstatushomologacao_s').click(function () {
                jQuery('#vldobshomologacao').parent().show();
            });

            jQuery('#vldstatus25exec_n').click(function () {
                jQuery('#vldobs25exec').parent().show();
            });
            jQuery('#vldstatus25exec_s').click(function () {
                jQuery('#vldobs25exec').val('').parent().hide();
            });

            jQuery('#vldstatus50exec_n').click(function () {
                jQuery('#vldobs50exec').parent().show();
            });
            jQuery('#vldstatus50exec_s').click(function () {
                jQuery('#vldobs50exec').val('').parent().hide();
            });

            jQuery('.Cancelar').click(function () {
                id = this.id.replace('btnCancela', '');
                validacao = id == 'Hmo' ? 'Homologação' : id == '25' ? '<?=$label25?>' : '<?=$label50?>';
                if (confirm('Deseja cancelar a validação da ' + validacao + '?')) {
                    jQuery.ajax({
                        url: '?modulo=principal/cadValidacao&acao=A',
                        type: 'post',
                        data: 'requisicao=' + this.id + '&vldid=' + jQuery('input[name=vldid]').val(),
                        success: function (e) {
                            if (e == 'true')
                                document.location.href = '?modulo=principal/cadValidacao&acao=A';
                        }
                    });
                }
            });

            jQuery('#cad_chk_adm').click(function () {
                popupChecklist('adm', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_adm_sp').click(function () {
                popupChecklist('adm_sp', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_adm_2015').click(function () {
                popupChecklist('adm_2015', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_tec').click(function () {
                popupChecklist('tec', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_tec_2015').click(function () {
                popupChecklist('tec_2015', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_2p').click(function () {
                popupChecklist('2p', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_obr_vinc').click(function () {
                popupChecklist('obr_vinc', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_obr_mi').click(function () {
                popupChecklist('obr_mi', 0, acao = 'cadastro');
            });

            jQuery('#cad_chk_solicitacoes').click(function () {
                popupChecklist('solicitacoes', 0, acao = 'cadastro');
            });

        });

    </script>
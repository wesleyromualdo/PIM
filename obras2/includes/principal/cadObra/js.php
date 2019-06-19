<script type="text/javascript">

    function inserirEntidade(entid, orgid) {
        if (entid) {
            if (confirm('Esta ação apagará todos os responsáveis da Obra. \n Deseja continuar?')) {
                jQuery('[id^="rpuid_"]').each(function () {
                    jQuery(this).remove();
                });
                return windowOpen('?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&orgid=' + orgid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
            }
        } else {
            return windowOpen('?modulo=principal/inserir_entidade&acao=A&orgid=' + orgid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

    function abreEdicaoEndereco() {
        windowOpen('?modulo=principal/editaEndereco&acao=O', 'blank', 'height=500,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function visualizarEntidade(entid, orgid) {
        if (entid) {
            return windowOpen('?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&bloq=disabled="disabled"&entid=' + entid + '&orgid=' + orgid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

    function inserirContato(entid, tipo) {
        if (entid) {
            return windowOpen('?modulo=principal/selecionaContatoObra&acao=A&busca=entnumcpfcnpj&tipo=' + tipo + '&entid=' + entid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        } else {
            return windowOpen('?modulo=principal/selecionaContatoObra&acao=A&tipo=' + tipo, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

    function inserirResponsavel(entid, tipo) {
        if (entid) {
            return windowOpen('?modulo=principal/selecionaResponsavelObra&acao=A&busca=entnumcpfcnpj&tipo=' + tipo + '&entid=' + entid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        } else {
            return windowOpen('?modulo=principal/selecionaResponsavelObra&acao=A&tipo=' + tipo, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

    function visualizaContato(entid) {
        var index = window.document.getElementById('linha_' + entid).rowIndex;
        return windowOpen('?modulo=principal/selecionaContatoObra&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&tr=' + index + '&habilita=N', 'blank', 'height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function atualizaContato(entid) {
        var index = window.document.getElementById('linha_' + entid).rowIndex;
        return windowOpen('?modulo=principal/selecionaContatoObra&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&tr=' + index, 'blank', 'height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function RemoveLinha(idContato) {
        if (confirm('Deseja realmente excluir este contato?')) {
            var index = window.document.getElementById('linha_' + idContato).rowIndex;
            table = window.document.getElementById("contatos");
            table.deleteRow(index);
        }
    }

    function excluirResponsavel(rpuid) {
        if (confirm('Deseja realmente excluir este responsÃ¡vel?')) {
            var index = window.document.getElementById('rpuid_' + rpuid).rowIndex;
            table = window.document.getElementById("responsaveisobra");
            table.deleteRow(index);
        }
    }

    function atualizaPrograma(tpoid) {
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: "req=litaPrograma&tpoid=" + tpoid,
            async: false,
            success: function (msg) {
                jQuery('#td_programa').html(msg);
            }
        });
    }

    jQuery(document).ready(function () {

        jQuery('[type="text"]').keyup();
        jQuery('.salvar').click(function () {
            var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
            var stop = false;

            if (jQuery('#entid').val() == "") {
                mensagem += 'Unidade Implantadora \n';
                stop = true;
            }

            if (jQuery('#entidsecretaria').val() == "") {
                mensagem += 'Secretaria \n';
                stop = true;
            }

            if (jQuery('#obrnumprocessoconv').val() == "") {
                mensagem += 'Processo \n';
                stop = true;
            }

            if (jQuery('#numconvenio').val() == "") {
                mensagem += 'Convênio / Termo \n';
                stop = true;
            }

            if (jQuery('#obranoconvenio').val() == "") {
                mensagem += 'Ano \n';
                stop = true;
            }

            if (jQuery('#obrnome').val() == "") {
                mensagem += 'Nome da Obra \n';
                stop = true;
            }

            if (jQuery('#tpoid').val() == "") {
                mensagem += 'Tipologia da Obra \n';
                stop = true;
            }

            if (jQuery('#tobid').val() == "") {
                mensagem += 'Tipo da Obra \n';
                stop = true;
            }

            if (jQuery('#cloid').val() == "") {
                mensagem += 'Classificação da Obra \n';
                stop = true;
            }

            if (jQuery('[id*=obra[obrdsc]]').val() == "") {
                mensagem += 'Descrição / Composição da Obra \n';
                stop = true;
            }

            if (jQuery('#obrvalorprevisto').val() == "") {
                mensagem += 'Valor Previsto (R$) \n';
                stop = true;
            }

            <?php
            if ($obridVinculado):
            ?>

            if (jQuery('#obrperccontratoanterior').val() == "") {
                mensagem += 'Percentual executado aproveitÃ¡vel do contrato anterior (%)\n';
                stop = true;
            }

            <?php
            endif;
            ?>

            if (jQuery('#moeid').val() == "") {
                mensagem += 'Modalidade de Ensino \n';
                stop = true;
            }

            if (jQuery('#endcep').val() == "") {
                mensagem += 'CEP \n';
                stop = true;
            }

            if (jQuery('#muncod').val() == "") {
                mensagem += 'Município \n';
                stop = true;
            }

            if (stop) {
                alert(mensagem);
                return false;
            }

//      jQuery('.obrigatorio').each(function(){
//          if( jQuery(this).val() == '' ){
//              stop = true;
//              alert('Campo obrigatÃ³rio.');
//              jQuery(this).focus();
//              return false;
//          }
//      });
            if (stop) {
                return false;
            }
            jQuery(this).hide();
            jQuery('#formObra').submit();
        });

        jQuery('#imgExcluir').click(function () {
            //obj = jQuery(this);
            if (confirm('Deseja excluir a foto?')) {
                jQuery.ajax({
                    url: '?modulo=principal/cadObra&acao=A',
                    type: 'post',
                    data: 'req=removeAnexo&obrid=' + jQuery('#obrid').val() + '&arqid=' + jQuery('#arqid').val(),
                    success: function (e) {
                        if (e == 'true') {
                            jQuery('#div_imagem').remove();
                            jQuery('#arqid').val('');
                        } else {
                            alert('NÃ£o foi possÃ­vel deletar a foto!');
                        }
                    }
                });

            }
        });

    });

    function abrirContatosPar(obrid) {
        var horizontal = 1000;
        var vertical = 600;

        var res_ver = screen.height;
        var res_hor = screen.width;

        var pos_ver_fin = (res_ver - vertical) / 2;
        var pos_hor_fin = (res_hor - horizontal) / 2;

        window.open('obras2.php?modulo=principal/popupContatosPar&acao=A&obrid=' + obrid, "Contatos PAR", "width=" + horizontal + ",height=" + vertical + ",top=" + pos_ver_fin + ",left=" + pos_hor_fin + ",status=yes");
    }

    function desbloqueioObras(obrid) {
        var horizontal = 1000;
        var vertical = 700;

        var res_ver = screen.height;
        var res_hor = screen.width;

        var pos_ver_fin = (res_ver - vertical) / 2;
        var pos_hor_fin = (res_hor - horizontal) / 2;

        window.open('obras2.php?modulo=principal/pedidoDesbloqueio&acao=A&obrid=' + obrid, "Desbloqueio", "width=" + horizontal + ",height=" + vertical + ",top=" + pos_ver_fin + ",left=" + pos_hor_fin + ",status=yes");
    }
</script>




<script type="text/javascript">
    jQuery('#map-canvas').click(function (e) {
        e.preventDefault();

        var horizontal = 500;
        var vertical = 500;

        var res_ver = screen.height;
        var res_hor = screen.width;

        var pos_ver_fin = (res_ver - vertical) / 2;
        var pos_hor_fin = (res_hor - horizontal) / 2;

        window.open('/obras2/geral/popupMapaObras.php?obrid=<?php echo $_SESSION['obras2']['obrid'] ?>', "LocalizaÃ§Ã£o da Obra", "width=" + horizontal + "px ,height=" + vertical + "px,top=" + pos_ver_fin + ",left=" + pos_hor_fin + ",status=yes");

    });
    

    function displayMessage(url) {

        var horizontal = 600;
        var vertical = 500;

        var res_ver = screen.height;
        var res_hor = screen.width;

        var pos_ver_fin = (res_ver - vertical) / 2;
        var pos_hor_fin = (res_hor - horizontal) / 2;


        window.open(url, "Localizaçãoo da Obra", "width=" + horizontal + ",height=" + vertical + ",top=" + pos_ver_fin + ",left=" + pos_hor_fin + ",status=yes");

    }

</script>

<script type="text/javascript">

    $(document).ready(function(){

        $('[type="text"]').keyup();
        $('.incluir_art').click(function(){
            var stop = false;
            $('.obrigatorio').each(function(){
                if( $(this).val() == '' ){
                    stop = true;
                    alert('Campo obrigatório.');
                    $(this).focus();
                    return false;
                }
            });
            if( stop ){
                return false;
            }
            $('#req').val('salvarArquivosArt');
            $('#formulario_art').submit();
        });
        $('.download').click(function(){
            $('#req').val('downloadArquivosArt');
            $('#arqid').val( $(this).attr('id') );
            $('#formulario_art').submit();
        });
        $('.excluir').click(function(){
            if ( confirm('Deseja apagar o documento?') ){
                $('#req').val('excluirArquivosArt');
                $('#oarid').val( $(this).attr('id') );
                $('#formulario_art').submit();
            }
        });
    });

</script>






<script>
    function imprimirLaudo( sueid ){
        return windowOpen( '?modulo=principal/popupImpressaoLaudo&acao=A&sueid=' + sueid,'blank',
            'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }
    function excluirSupervisao( sueid ){
        if(confirm("Deseja realmente excluir essa supervisão?")){
            $('#sueid').val( sueid );
            $('#operacao').val( 'apagarSupervisaoEmpresa' );
            $('#formularioEmpresa').submit();
        }
    }
    function editarSupervisao( sueid ){
        $('#formularioEmpresa > #sueid').val( sueid );
        $('#formularioEmpresa > #operacao').val('editarSupervisaoEmpresa');
        $('#formularioEmpresa').submit();
    }
</script>


<script>
    function alterarSupFNDE( sfndeid ){
        $('#formSupervisaoFnde #operacao').val('alterarSupervisaoFNDE');
        $('#formSupervisaoFnde #sfndeid').val(sfndeid);
        $('#formSupervisaoFnde').submit();
    }

    function excluirSupFNDE( sfndeid ){
        if ( confirm('Deseja apagar esta supervisão?') ){
            $('#operacao').val('apagarSupervisaoFNDE');
            $('#sfndeid').val(sfndeid);
            $('#formSupervisaoFnde').submit();
        }
    }
</script>

<script type="text/javascript">
    $(document).ready(function(){

        editarEvolucaoMi = function(emiid){
            window.location.href = 'obras2.php?modulo=principal/cadEvolucaoMi&acao=E&emiid='+emiid;
        }

        imprimirEvolucaoMi = function(emiid){
            var url = 'obras2.php?modulo=principal/listaEvolucaoMi&acao=I&emiid='+emiid;
            var popup = window.open(
                url,
                "Dados Evolução MI",
                "width=1300,height=750,scrollbars=yes,scrolling=no,resizebled=no"
            );
        }

    });
</script>



<script type="text/javascript">

    function ExcluirVistoria(url, supid){
        if(confirm("Deseja realmente excluir essa vistoria?")){
            window.location = url + '&subacao=VerificaVistoria&supid=' + supid;
        }
    }

    function AtualizarVistoria(url,supid){
        window.location = url + '&supid='+supid;
    }

    function atualizar(){
        document.getElementById('requisicao').value = 'atualizarRegistros';
        document.getElementById('formulario').submit();
    }

    function toggleParaObraVinculada(obj_tabela){
        var linha = ' <tr id="tr_obra_vinculada"> ' +
            '    <td class="SubTituloDireita" width="20%">Percentual da última vistoria da última obra vinculada antes da atual:</td> ' +
            '    <td> ' +
            '        <span style="margin-left:5px"> ' +
            '            <?php echo number_format($dados_obra_vinculada['obrpercentultvistoria_vinculado'],2,',','.').'%';?> ' +
            '        </span> ' +
            '    </td> ' +
            '</tr>';
        if( jQuery( '#chamada_obra_vinculada' ).is(':checked') ){
            jQuery(obj_tabela).append( linha );
        }else{
            jQuery('#tr_obra_vinculada').remove();
        }
    }

    function toggleParaResponsabilidades( id_tabela ){
        if( jQuery( '#chamada_'+id_tabela ).is(':checked') ){
            jQuery('#'+id_tabela).css('display','');
        }else{
            jQuery('#'+id_tabela).css('display','none');
        }
    }

    function imprimirQuestionarioRespondido(sueid, sosid){
        return windowOpen (
            '?modulo=principal/cadVistoriaEmpresaImpressaoPreenchido&acao=A&sueid='+sueid+'&sosid='+sosid,'blank'
            , 'height=700,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'
        );
    }

    function imprimirQuestionarioRespondidoFNDE(sfndeid){
        return windowOpen (
            '?modulo=principal/cadVistoriaFNDEImpressaoPreenchido&acao=A&sfndeid='+sfndeid,'blank'
            , 'height=700,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'
        );
    }

    function alterarSupMI (smiid) {
        $('#op').val('alterar');
        $('#smiid').val(smiid);
        $('#formSupervisaoMi').submit();
    }

    function excluirSupMI(smiid) {
        if (confirm('Deseja apagar esta supervisão?')) {
            $('#op').val('apagar');
            $('#smiid').val(smiid);
            $('#formSupervisaoMi').submit();
        }
    }

    function criaCopiaSupEmpresaMI(smiid){
        var url = 'obras2.php?modulo=principal/inserir_vistoria&acao=A&smiid='+smiid;
        window.location = url;
    }

    function abreTelaImpressao(smiid) {
        return windowOpen(
            '?modulo=principal/impressaoSupervisaoMI&acao=A&smiid='+smiid,'blank'
            , 'height=700,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'
        );
    }

</script>



<script>
$(function(){
    $('#capacidade').keyup();
    $('#investimentototal').keyup();
    $('#alunosmatriculados').keyup();
    $('#quantidadehabitantes').keyup();
    $('#distancia').keyup();
    
    $('#total_file_obra_funcionamento').val();
    $('#total_file_obra_inauguracao').val();
    
    $('#adicionar_anexo_obra_concluida').click(function (e) {
        if($('#total_file_obra_funcionamento').val() < 9){
            $('#total_file_obra_funcionamento').val(parseInt($('#total_file_obra_funcionamento').val()) + 1);
            $('#table_anexos_obra_concluida').append($('<tr class="anexos anexos-base-obr-concluida"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_obra_concluida" href="">Excluir</a></span></td></td><td class="SubTituloEsquerda"><td class="SubTituloEsquerda"><input onchange="validarEscolhaArquivo(this)" <?=!$habilitado ? 'disabled' : ''?> type="file" name="arquivo_obra_concluida[]" id="arquivo_obr_concluida"/></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> maxlength="255" size="" type="text" name="arquivo_descricao_obra_concluida[]" id="arquivo_descricao_obr_concluida"/></td></tr>').removeClass('anexos-base-obr-concluida'));
            e.preventDefault();
        } else {
            alert('Limite de 10 arquivos simultâneos!');
            return false;
        }
    });
    
    $('#adicionar_anexo_inauguracao').click(function (e) {
        if($('#total_file_obra_inauguracao').val() < 9){
            $('#total_file_obra_inauguracao').val(parseInt($('#total_file_obra_inauguracao').val()) + 1);
            $('#table_anexos_inauguracao').append($('<tr class="anexos anexos-base-inauguracao"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo_inauguracao" href="">Excluir</a></span></td><td class="SubTituloEsquerda"></td><td class="SubTituloEsquerda"><input onchange="validarEscolhaArquivo(this)" <?=!$habilitado ? 'disabled' : ''?> type="file" name="arquivo_inauguracao[]" id="arquivo_inauguracao"/></td><td class="SubTituloEsquerda"><input <?=!$habilitado ? 'disabled' : ''?> maxlength="255" size="" type="text" name="arquivo_descricao_inauguracao[]" id="arquivo_descricao_inauguracao"/></td></tr>').removeClass('anexos-base-inauguracao'));
            e.preventDefault();
        } else {
            alert('Limite de 10 arquivos simultâneos!');
            return false;
        }
    });
    
    $('.excluir_anexo_obra_concluida').live('click',function (e) {
        $('#total_file_obra_funcionamento').val(( parseInt( $('#total_file_obra_funcionamento').val() ) - 1));
        $(this).parents('tr.anexos').remove();
        e.preventDefault();
    });
    
    $('.excluir_anexo_inauguracao').live('click',function (e) {
        $('#total_file_obra_inauguracao').val(( parseInt( $('#total_file_obra_inauguracao').val() ) - 1));
        $(this).parents('tr.anexos').remove();
        e.preventDefault();
    });
});

function validarFormatoArquivo(arquivo) {
    if ( /\.(jpe?g|png|gif|bmp)$/i.test(arquivo.files[0].name) === false ) { 
        alert("Somente imagens são permitidas!"); 
        arquivo.value = null;
        return false;
    }
}

function validarEscolhaArquivo(arquivo){
    
    validarFormatoArquivo(arquivo);
    
    if(arquivo.files[0] && arquivo.files[0].size > 4194304){     
        alert('O tamanho máximo do arquivo deve ser de 4 MB.');
        arquivo.value = null;
        return false;
    }
}
</script>


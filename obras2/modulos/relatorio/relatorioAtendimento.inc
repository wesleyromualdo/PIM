<?php
if ( $_REQUEST['pesquisa'] == 'xls' ) {
    include APPRAIZ . "obras2/modulos/relatorio/relatorioAtendimento.inc";
}

extract( $_POST );

switch ($_REQUEST['ajax']){
    case 'municipio':
        header('content-type: text/html; charset=utf-8');
        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
monta_titulo('Relatório para Atendimento', '&nbsp;');

?>


<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js">
</script>
<script type="text/javascript">
    
function carregarMunicipio(estuf) {
    var td = $('#td_municipio');
    if (estuf != '') {
        var url = location.href;
        $.ajax({
            url: url,
            type: 'post',
            data: {ajax: 'municipio',
                estuf: estuf},
            dataType: "html",
            async: false,
            beforeSend: function() {
                divCarregando();
                td.find('select option:first').attr('selected', true);
            },
            error: function() {
                divCarregado();
            },
            success: function(data) {
                td.html(data);
                divCarregado();
            }
        });
    } else {
        td.find('select option:first').attr('selected', true);
        td.find('select').attr('selected', true)
                .attr('disabled', true);
    }
}

$(document).ready(function() {

    $('.pesquisar').click(function() {
        $('#formListaObra').submit();
    });

    $('.exportarxls').click(function() {
        $('#xls').val('1');
        $('#formListaObra').submit();
    });

});

function gerarRelatorio(tipo) {
    formulario = document.getElementById('formListaObra');

    if (tipo != 'xls') {
        formulario.target = 'RelatorioAtendimento';
        var janela = window.open('obras2.php?modulo=relatorio/popupRelatorioAtendimento&acao=A', 'RelatorioAtendimento', 'width=1150,height=700,status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes');
        janela.focus();

        // Tipo de relatorio
    } else {
        formulario.target = null;
    }
    document.getElementById('pesquisa').value = tipo;
    formulario.submit();
}
$(function() {
    $('.pos_sim').change(function() {
        if ($(this).val() == 'nao') {
            $('.f_supervisao').hide();
        } else {
            $('.f_supervisao').show();
        }
    });
});
</script>
<style>
    .chosen-select{
        width: 400px;
    }
</style>
<form method="post" name="formListaObra" id="formListaObra" action="obras2.php?modulo=relatorio/popupRelatorioAtendimento&acao=A">
    <input type="hidden" name="pesquisa" id="pesquisa" value="" />
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="15%">ID da Obra:</td>
            <td>
                <?=campo_texto('obrid','N','S','',58,100,'','', '', '', '', 'id="obrid"', '', $_REQUEST['obrid']);?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="15%">ID da Pré-Obra:</td>
            <td>
                <?=campo_texto('preid','N','S','',58,100,'','', '', '', '', 'id="preid"', '', $_REQUEST['preid']);?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Processo:</td>
            <td>
                <input type="text" style="text-align:right;" name="processo" size="59" maxlength="20" value="" onkeyup="this.value=mascaraglobal('#####.######/####-##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('#####.######/####-##',this.value);" title="" class=" normal">
        </tr>
        <tr>
            <td class="SubTituloDireita">Termo/Convênio:</td>
            <td>
                Número:&nbsp;
                <input type="text" style="text-align:right;" name="convenio" size="31" maxlength="20" value="" onkeyup="this.value=mascaraglobal('####################',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('####################',this.value);" title="" class=" normal">                            Ano:&nbsp;
                <input type="text" style="text-align:right;" name="ano_convenio" size="7" maxlength="4" value="" onkeyup="this.value=mascaraglobal('####',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('####',this.value);" title="" class=" normal">                        </td>
        </tr>
        <tr id="idMunicipio">
            <td class="SubTituloDireita ">Municípios(s):</td>
            <td class="td_municipio ">
                <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                    <?php
                    $municipio = new Municipio();
                    foreach ($municipio->listaComboMulti() as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['muncod']) && in_array($key['codigo'], $_POST['muncod'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
            </td>
        </tr>
        <tr>
            <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
                <input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('visual');" /> 
                <input type="button" name="Gerar Relatório" value="Visualizar XLS" onclick="javascript:gerarRelatorio('xls');" />
            </td>
        </tr>
    </table>
</form>

<script type="text/javascript">
    $(function(){
        $('.filtro_diff').change(function(){
            m = $('input[name=diff_m]').attr('checked');
            n = $('input[name=diff_n]').attr('checked');
            t = $('input[name=diff_t]').attr('checked');

            if(!m && !n && !t){
                $('.porcentagem').hide();
            } else if(!m || !n || !t) {
                $('.porcentagem').show();
            } else {
                $('.porcentagem').hide();
            }
        });
    });
</script>
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
    <?php 
        monta_titulo_relatorios('Relatório para Atendimento', '&nbsp;');
    ?>
    <div class="form-filters form-design">

        <div class="row">
            <div class="col-lg-6">
                <div class="row">
                    <div class="col-md-4">Id da Obra</div>
                    <div class="col-lg-8">
                         <?php echo campo_texto('obrid','N','S','',58,100,'','', '', '', '', 'id="obrid"', '', $_POST['obrid']);?>    
                    </div> 
                   
                </div>
                <div class="row">
                    <div class="col-md-4">Municípios(s)</div>
                    <div class="col-lg-8">
                        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                            <?php
                            $municipio = new Municipio();
                            foreach ($municipio->listaComboMulti() as $key) {
                                ?>
                                <option
                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['muncod']) && in_array($key['codigo'], $_POST['muncod'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>   
                    </div> 
                </div> 

                   
            </div>
            <div class="col-lg-6">
                <div class="row">
                    <div class="col-md-4">ID da Pré-Obra</div>
                    <div class="col-lg-8">
                        <?php echo campo_texto('preid','N','S','',58,100,'','', '', '', '', 'id="preid"', '', $_POST['preid']);?>  
                    </div> 
                </div> 
                 <div class="row">
                    <div class="col-md-4">Termo/Convênio: </div>
                    <div class="col-lg-8">
                        Número:&nbsp;
                        <input type="text" style="text-align:right;" name="convenio" size="28" maxlength="20" value="" onkeyup="this.value=mascaraglobal('####################',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('####################',this.value);" title="" class=" normal">                            
                        Ano:&nbsp;
                        <input type="text" style="text-align:right;" name="ano_convenio" size="7" maxlength="4" value="" onkeyup="this.value=mascaraglobal('####',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('####',this.value);" title="" class=" normal">       
                    </div> 
                </div> 
                     
            </div>
        </div>
    </div>

    <div class="row form-filters text-center">
        <div class="col">

            <button type="button" class="btn btn-success" value="Pesquisar" name="Gerar Relatório" onclick="javascript:gerarRelatorio('visual');">
            <span class="glyphicon glyphicon-list-alt"></span> Pesquisar</button>
            <button type="button" class="btn btn-success" value="Visualizar XLS" name="Gerar Relatório" onclick="javascript:gerarRelatorio('xls');">
            <span class="glyphicon glyphicon-download-alt"></span> Gerar Excel</button>

        </div>
    </div> 
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
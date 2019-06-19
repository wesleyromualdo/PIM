<?php
if ( $_REQUEST['pesquisa'] == 'xls' ) {
    include APPRAIZ . "obras2/modulos/relatorio/relatorioFisicoFinanceiro.inc";
}

extract( $_POST );


switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=utf-8');
        $estuf = $_REQUEST['estuf'];
        ?>

        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="muncod[]"]').chosen();

            });
        </script>

        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
            <?php   $municipio = new Municipio();
            foreach ($municipio->listaComboMulti($estuf) as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php
        exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

//usa a referência para listar o campo situação
require APPRAIZ . 'obras2/includes/principal/listaRestricao/ctrl.php';

echo "<br>";
monta_titulo('Relatório Físico-Financeiro', '&nbsp;');

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
        formulario.target = 'RelatorioFisicoFinanceiro';
        var janela = window.open('obras2.php?modulo=relatorio/popupRelatorioFisicoFinanceiro&acao=A', 'RelatorioFisicoFinanceiro', 'width=1150,height=700,status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes');
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
<form method="post" name="formListaObra" id="formListaObra" action="obras2.php?modulo=relatorio/popupRelatorioFisicoFinanceiro&acao=A">
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
            <td class="SubTituloDireita">
                Situação da Obra
            </td>
            <td>
                <select name="esdid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
                    $dados = $db->carregar($sql);
                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($esdid) && in_array($key['codigo'], $esdid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>

        <tr>

            <td class="SubTituloDireita">
                Tipologia:
            </td>
            <td>
                <select name="tpoid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php   $tipologiaObra = new TipologiaObra();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    $dados = $tipologiaObra->listaCombo($param, false);


                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($tpoid) && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">
                Programa:
            </td>
            <td>
                <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($prfid) && in_array($key['codigo'], $prfid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Fonte:
            </td>
            <td>
                <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($tooid) && in_array($key['codigo'], $tooid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
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
                <input type="text" style="text-align:right;" name="convenio" size="21" maxlength="20" value="" onkeyup="this.value=mascaraglobal('####################',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('####################',this.value);" title="" class=" normal">                            Ano:&nbsp;
                <input type="text" style="text-align:right;" name="ano_convenio" size="18" maxlength="4" value="" onkeyup="this.value=mascaraglobal('####',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);this.value=mascaraglobal('####',this.value);" title="" class=" normal">                        </td>
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
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Situação do equilíbrio físico-financeiro:</td>
            <td>
                <input type="checkbox" name="diff_m" class="filtro_diff" id="" value="true" checked="checked"> diferença a maior
                <input type="checkbox" name="diff_n" class="filtro_diff" id="" value="true" checked="checked"> diferença a menor
                <input type="checkbox" name="diff_t" class="filtro_diff" id="" value="true" checked="checked"> sem diferença
            </td>
        </tr>
        <tr class="porcentagem" style="display: none">
            <td class="SubTituloDireita">Diferença de:</td>
            <td>
                <?php
                for ($i = 0; $i <= 100; $i++) {
                    $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                }
                $percentualinicial = $_POST['percentualinicial'];
                $percentualfinal = $_POST['percentualfinal'];
                $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                ?>
                <?$db->monta_combo("diff_de", $arPercentual, 'S', '', '', '', '', '', 'N', 'diff_de');?> Até:&nbsp;
                <?$db->monta_combo("diff_ate", $arPercentual, 'S', '', '', '', '', '', 'N', 'diff_ate', false, $percfinal);?>
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



<style>
    .chosen-container-multi {
        width: 400px !important;
    }

    .chosen-container-multi .chosen-choices {
        width: 400px !important;
    }

    label.btn.active {
        background-image: none;
        outline: 0;
        -webkit-box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        color: #ffffff;
        background-color: #3276b1 !important;
        border-color: #285e8e;
    }
</style>


<script>

    function carregarMunicipio(estuf) {


        var td = $1_11('.td_municipio');
        if (estuf != '') {
            var url = location.href;
            $1_11.ajax({
                url: url,
                type: 'post',
                data: {
                    ajax: 'municipio',
                    estuf: values
                },
                dataType: "html",
                async: false,
                beforeSend: function () {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function () {
                    divCarregado();
                    alert(2);
                },
                success: function (data) {
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

    $1_11(document).ready(function () {
        $1_11('select[name="prfid[]"]').chosen();
        $1_11('select[name="tooid[]"]').chosen();
        $1_11('select[name="estuf[]"]').chosen();
        $1_11('select[name="muncod[]"]').chosen();
        $1_11('select[name="tpoid[]"]').chosen();
        $1_11('select[name="esdid[]"]').chosen();


        $1_11(".estados").chosen().change(function (e, params) {
            values = $1_11(".estados").chosen().val();
            carregarMunicipio(values);
        });

    });

</script>
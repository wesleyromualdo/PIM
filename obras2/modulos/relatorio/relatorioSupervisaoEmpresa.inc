<?php
switch ($_REQUEST['ajax']){
	case 'municipio':
        header('content-type: text/html; charset=utf-8');

        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;

}
include APPRAIZ . 'includes/cabecalho.inc';

//usa a referência para listar o campo situação
require APPRAIZ . 'obras2/includes/principal/listaRestricao/ctrl.php';

print '<br/>';
monta_titulo( 'Relatório - Supervisão Empresa', 'Relatório - Supervisão Empresa' );

?>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<style>
    .chosen-select{
        width: 400px;
    }
</style>

<form action="obras2.php?modulo=relatorio/popupRelatorioSupervisaoEmpresa&acao=A" method="post" name="filtro" id="filtro">
<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <tr>
        <td class="SubTituloDireita" width="40%">Nome da Obra / ID:</td>
        <td>
            <?=campo_texto('empbuscatexto','N','S','',58,100,'','', '', '', '', 'id="empbuscatexto"', '', $_REQUEST['empbuscatexto']);?>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="40%">ID Empreendimento:</td>
        <td>
            <?=campo_texto('empid','N','S','',58,100,'','', '', '', '', 'id="empid"', '', $_REQUEST['empid']);?>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Número da OS:</td>
        <td>
            <?php
            echo campo_texto( 'sosnum', 'N', 'S', '', 11, 30, '############', '', 'right', '', 0, '');
            ?>
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita">Número da Nota Fiscal:</td>
        <td>
            <?php
            echo campo_texto( 'pagnrnotafiscal', 'N', 'S', '', 11, 30, '############', '', 'right', '', 0, '');
            ?>
        </td>
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
    <td class="SubTituloDireita">Empresa responsável</td>
    <td>
        <?php
        $supervisao_Grupo_Empresa = new Supervisao_Grupo_Empresa();
        $empresas = $supervisao_Grupo_Empresa->listaComboEntidades();
        echo $db->monta_combo("entid", $empresas, 'S','Selecione...','', '', '',200,'N','sgeid');
        ?>
    </td>
        <tr>
            <td class="SubTituloDireita">Situação da Obra:</td>
            <td>
                <?php
                $sql_obr = "";

                $dados = $db->carregar(getSqlEstadoDoc(TPDID_OBJETO));
                $dados = $dados ? $dados : array();
                ?>
                <select name="esdid_obr[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['esdid_obr']) && in_array($key['codigo'], $_POST['esdid_obr'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>    

<tr>
    <td class="SubTituloDireita">Supervisão homologada com atraso</td>
    <td>
        <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "sim" ? "checked='checked'" : "" ?> value='sim' />Sim
        <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "nao" ? "checked='checked'" : "" ?> value='nao' />Não
        <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "todas" ? "checked='checked'" : "" ?> value='todas' />Todas
    </td>
</tr>
<tr>
    <td class="subtitulodireita" width="190px">Data de Homologação:</td>
    <td>
        de: <input type="text" id="dthomol_de" name="dthomol_de" value="" size="15" maxlength="10" class="normal" >
        &nbsp;
        até: <input type="text" id="dthomol_ate" name="dthomol_ate" value="" size="15" maxlength="10" class="normal">
    </td>
    <td>&nbsp;</td>
</tr>
<tr>
	<td bgcolor="#CCCCCC"></td>
	<td bgcolor="#CCCCCC">
		<input type="button" value="Visualizar" onclick="exibeRelatorio('exibir');" style="cursor: pointer;"/>
		<input type="button" value="Visualizar XLS" onclick="exibeRelatorio('xls');" style="cursor: pointer;"/>
	</td>
</tr>
</table>
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
                beforeSend: function () {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function () {
                    divCarregado();
                },
                success: function (data) {
                    console.info(data);
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

    formulario = document.getElementById('filtro');
    function exibeRelatorio(tipo){
        document.getElementById('pesquisa').value = tipo;

        formulario.target = 'RelatorioSupervisaoEmpresa';
        var janela = window.open('obras2.php?modulo=relatorio/popupRelatorioSupervisaoEmpresa&acao=A', 'RelatorioSupervisaoEmpresa', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=yes,width=780,height=460');

        janela.focus();

        formulario.submit();
    }

</script>
</form>


<script lang="javascript">

    jQuery(function($) {
        $.datepicker.regional['pt-BR'] = {
            closeText: 'Fechar',
            prevText: '&#x3c;Anterior',
            nextText: 'Pr&oacute;ximo&#x3e;',
            currentText: 'Hoje',
            monthNames: ['Janeiro', 'Fevereiro', 'Mar&ccedil;o', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
            monthNamesShort: ['Jan', 'Fev', 'Mar', 'Abr', 'Mai', 'Jun', 'Jul', 'Ago', 'Set', 'Out', 'Nov', 'Dez'],
            dayNames: ['Domingo', 'Segunda-feira', 'Ter&ccedil;a-feira', 'Quarta-feira', 'Quinta-feira', 'Sexta-feira', 'S&aacute;bado'],
            dayNamesShort: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
            dayNamesMin: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
            weekHeader: 'Sm',
            dateFormat: 'dd/mm/yy',
            firstDay: 0,
            isRTL: false,
            showMonthAfterYear: false,
            yearSuffix: ''};
        $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
    });

    setTimeout(function() {

        jQuery('#dthomol_de').mask('99/99/9999');
        jQuery('#dthomol_ate').mask('99/99/9999');

        jQuery("#dthomol_de").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });
        jQuery("#dthomol_ate").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });


    }, 500);

</script>
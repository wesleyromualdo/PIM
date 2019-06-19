<?php

/*switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=utf-8');
        $estuf = $_REQUEST['estuf'];
        ?>

        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="muncod[]"]').chosen();

            });
        </script>

        <select name="muncod[]" class="chosen-select municipios" id="chosenForm" multiple data-placeholder="Selecione">
            <?php   $municipio = new Municipio();
            foreach ($municipio->listaComboMulti($estuf) as $key) {
                ?>
                <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php
        exit;
    }*/
?>
        
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        
<?php

include APPRAIZ . 'includes/cabecalho.inc';

//usa a referência para listar o campo situação
require APPRAIZ . 'obras2/includes/principal/listaRestricao/ctrl.php';

print '<br/>';
monta_titulo( 'Relatório - Restrições/Inconformidades', 'Relatório - Restrições/Inconformidades' );

?>
    
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>    
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript">

/*function getEstados(){
        var estados = '';

        var elemento = document.getElementsByName('slEstado[]')[0];

        for (var i = 0; i < elemento.options.length; i++) {
            if (elemento.options[i].value != '')
            {
                estados += "'" + elemento.options[i].value + "',";
            }
        }

        return estados;
    }

    function ajaxEstado(){
        jQuery.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=filtrarMunicipio&estados=" + getEstados(),
            success: function(retorno) {
                jQuery('#idMunicipio').html(retorno);
            }});
    }*/
 
    function onOffCampo(campo){
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }

    /**
     * Alterar visibilidade de um bloco.     
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco(bloco){
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }
    
    jQuery(function($){
        $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro','Fevereiro','Mar&ccedil;o','Abril','Maio','Junho','Julho','Agosto','Setembro','Outubro','Novembro','Dezembro'],
                monthNamesShort: ['Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set','Out','Nov','Dez'],
                dayNames: ['Domingo','Segunda-feira','Ter&ccedil;a-feira','Quarta-feira','Quinta-feira','Sexta-feira','S&aacute;bado'],
                dayNamesShort: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                dayNamesMin: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''};
        $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
    });


</script>
<style>
    .chosen-select{
        width: 400px;
    }
</style>

<form action="obras2.php?modulo=relatorio/popupRelatorioRestricoesInconformidades&acao=A" method="post" name="filtro" id="filtro">
<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

    <tr>
        <td class="SubTituloDireita">Nome da Obra/ID:</td>
        <td>
            <?php echo campo_texto( 'obrid', 'N', 'S', '', 60, 80, '', '', 'left', '', 0, ''); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Tipologia:</td>
        <td>
            <select name="tpoid[]" id="tpoid" class="chosen-select" multiple data-placeholder="Selecione">
                <?php   $tipologiaObra = new TipologiaObra();
                $param = array("orgid" => $_SESSION['obras2']['orgid']);
                $dados = $tipologiaObra->listaCombo($param, false);
                    foreach ($dados as $key) {
                ?>
                    <option value="<?php echo $key['codigo'] ?>" <?php if ($tpoid && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
            </select>
        </td>
    </tr>
    <tr>
        <td class="subtituloDireita">Item:</td>
        <td>
            <?php
                $select_r = '';
                $select_i = '';
                $select_t = '';
                switch ($_POST['item_restrict']) {
                    case 'R':
                        $select_r = 'checked="checked"';
                        break;
                    case 'I':
                        $select_i = 'checked="checked"';
                        break;
                    case 'T':
                        $select_t = 'checked="checked"';
                        break;
                }
            ?>
            <input type="radio" name="item_restrict" id="item_restrict_r" value="R" <?php echo $select_r; ?> > Restrição
            <input type="radio" name="item_restrict" id="item_restrict_i" value="I" <?php echo $select_i; ?> > Inconformidade
            <input type="radio" name="item_restrict" id="item_restrict_t" value="T" <?php echo $select_t; ?> > Todas
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Tipo de Restrição:</td>
        <td>
            <select name="tprid[]" class="chosen-select" multiple data-placeholder="Selecione">
                <?php
                    $tipoRetricao = new TipoRestricao();
                    foreach ($tipoRetricao->listaCombo() as $key) {
                        ?>
                    <option value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tprid']) && in_array($key['codigo'], $_POST['tprid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
            </select>
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
            <td class="SubTituloDireita">Programa:</td>
            <td class="tdInputForm">
                <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['prfid']) && in_array($key['codigo'], $_POST['prfid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Fonte:</td>
            <td class="tdInputForm">
                <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tooid']) && in_array($key['codigo'], $_POST['tooid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
    
        
        <tr>
            <td class="subtitulodireita" width="190px">Data de Cadastro:</td>
            <td>
                <?php 
                    $data_de  = '';
                    $data_ate = '';
                ?>
                de: <input type="text" id="rstdtinclusao_de" name="rstdtinclusao_de" value="<?php echo $data_de;?>" size="15" maxlength="10" class="normal"> 
                &nbsp;
                até: <input type="text" id="rstdtinclusao_ate" name="rstdtinclusao_ate" value="<?php echo $data_ate;?>" size="15" maxlength="10" class="normal">
            </td>
            <td>&nbsp;</td>                
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Data de Superação:</td>
            <td>
                <?php 
                    $data_de  = '';
                    $data_ate = '';
                ?>
                de: <input type="text" id="rstdtsuperacao_de" name="rstdtsuperacao_de" value="<?php echo $data_de;?>" size="15" maxlength="10" class="normal"> 
                &nbsp;
                até: <input type="text" id="rstdtsuperacao_ate" name="rstdtsuperacao_ate" value="<?php echo $data_ate;?>" size="15" maxlength="10" class="normal">
            </td>
            <td>&nbsp;</td>                
        </tr>
        
        <tr>
            <td class="subtituloDireita">Existe Ressalva ?</td>
            <td>
                <?php
                    $select_s = '';
                    $select_n = '';
                    $select_t = '';
                    switch ($_POST['rstflressalva']) {
                        case 'S':
                            $select_s = 'checked="checked"';
                            break;
                        case 'N':
                            $select_n = 'checked="checked"';
                            break;
                        case 'T':
//                        default:
                            $select_t = 'checked="checked"';
                            break;
                    }
                ?>
                <input type="radio" name="rstflressalva" id="rstflressalva_s" value="S" <?php echo $select_s; ?> > Sim
                <input type="radio" name="rstflressalva" id="rstflressalva_n" value="N" <?php echo $select_n; ?> > Não
                <input type="radio" name="rstflressalva" id="rstflressalva_t" value="T" <?php echo $select_t; ?> > Todas
            </td>
        </tr>
        
<tr>
    <td bgcolor="#CCCCCC"></td>
    <td bgcolor="#CCCCCC">
        <input type="button" value="Visualizar" onclick="exibeRelatorio('exibir');" style="cursor: pointer;"/>
        <input type="button" value="Visualizar XLS" onclick="exibeRelatorio('xls');" style="cursor: pointer;"/>
    </td>
</tr>
</table>

<script lang="javascript">
            
setTimeout(function(){
    
    jQuery('#rstdtinclusao_de').mask('99/99/9999');
    jQuery('#rstdtsuperacao_de').mask('99/99/9999');
    jQuery('#rstdtinclusao_ate').mask('99/99/9999');
    jQuery('#rstdtsuperacao_ate').mask('99/99/9999');
    
    var d = new Date();
    var data_hoje = d.toUTCString();
    
    jQuery("#rstdtinclusao_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        showAnim:'drop'
    });
    jQuery("#rstdtinclusao_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        showAnim:'drop'
    });
    jQuery("#rstdtsuperacao_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });
    jQuery("#rstdtsuperacao_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });
               
}, 500);

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

        /*if(document.getElementById('prfid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'prfid' ) );
        }
        if(document.getElementById('tooid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'tooid' ) );
        }
        
        if(document.getElementById('esdid_ri_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'esdid_ri' ) );
        }
        
        if(document.getElementById('esdid_obr_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'esdid_obr' ) );
        }
        
        if(document.getElementById('estuf_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'estuf' ) );
        }
        
        if(document.getElementById('muncod_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'muncod' ) );
        }*/

        document.getElementById('pesquisa').value = tipo;

        formulario.target = 'RelatorioRestricoesInconformidades';
        var janela = window.open('obras2.php?modulo=relatorio/popupRelatorioRestricoesInconformidades&acao=A', 'RelatorioRestricoesInconformidades', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=yes,width=780,height=460');

        janela.focus();

        formulario.submit();
    }

    /**
     * Alterar visibilidade de um bloco.
     * 
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco( bloco )
    {
        var div_on = document.getElementById( bloco + '_div_filtros_on' );
        var div_off = document.getElementById( bloco + '_div_filtros_off' );
        var img = document.getElementById( bloco + '_img' );
        var input = document.getElementById( bloco + '_flag' );
        if ( div_on.style.display == 'none' )
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }
    
    /**
     * Alterar visibilidade de um campo.
     * 
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo( campo )
    {
        var div_on = document.getElementById( campo + '_campo_on' );
        var div_off = document.getElementById( campo + '_campo_off' );
        var input = document.getElementById( campo + '_campo_flag' );
        if ( div_on.style.display == 'none' )
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
    $(function () {
        $('#esdid').change(function () {
            var val = $('#esdid').val();
            if (val == '<?= ESDID_OBJ_PARALISADO ?>' || val == '<?= ESDID_OBJ_EXECUCAO ?>') {
                $('#nivelpreenchimento').removeAttr("disabled");
            } else {
                $('#nivelpreenchimento').attr("disabled", "disabled");
                $('#nivelpreenchimento').val(0);
            }
        });
    });
  
</script>
</form>

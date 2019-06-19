<?php
$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];

if ($_REQUEST['form'] == '1' && $_REQUEST['tipo_relatorio'] == 'xls') {
    montaLista($_REQUEST['tipo_relatorio']);
    exit;
}
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<?php
include APPRAIZ . "includes/cabecalho.inc";
print '<br/>';
$titulo_modulo = 'Lista de Checklist FNDE';
$subtitulo_modulo = 'Pesquisa dos registros de todas as Obras';
monta_titulo_obras($titulo_modulo, $subtitulo_modulo);

//Fluxo de Restrição/Inconformidade
$tpdid = TPDID_RESTRICAO_INCONFORMIDADE;
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="UTF-8"></script>

<script type="text/javascript">

    function getEstados() {
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

    function ajaxEstado() {
        jQuery.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=filtrarMunicipio&estados=" + getEstados(),
            success: function(retorno) {
                jQuery('#idMunicipio').html(retorno);
            }});
    }

    function onOffCampo(campo) {
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


    function onOffBloco(bloco) {
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

    function getLista(tipo) {
        var formulario = document.formulario;
        var tipo_relatorio = tipo;
        prepara_formulario();
        document.getElementById('tipo_relatorio').value = tipo_relatorio;
        formulario.submit();
        document.getElementById('tipo_relatorio').value = '';
    }

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

</script>

<form name="formulario" id="formulario" action="" method="post">

    <input type="hidden" name="form" value="1" />
    <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" />

    <div class="form-filters form-design">
        <div class="row">
            <div class="col-lg-2">
                Nome da Obra / ID:
            </div>
            <div class="col-lg-10">
            <?php
                $val = (!empty($_POST['obrbuscatexto'])) ? $_POST['obrbuscatexto'] : '';
                echo campo_texto('obrbuscatexto', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="obrbuscatexto"', '', $val);
            ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Tipologia da Obra:
            </div>
            <div class="col-lg-10">
                <select name="tpoid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php   $tipologiaObra = new TipologiaObra();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    $dados = $tipologiaObra->listaCombo($param, false);
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tpoid']) && in_array($key['codigo'], $_POST['tpoid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Tipo do Checklist:
            </div>
            <div class="col-lg-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" name="item_checklist" id="item_checklist_2p" value="2P" <?= ($_POST["item_checklist"] == "2P" ? "checked" : "") ?>> 2ª Parcela
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="item_checklist" id="item_checklist_ad" value="AD" <?= ($_POST["item_checklist"] == "AD" ? "checked" : "") ?>> Administrativo
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="item_checklist" id="item_checklist_tc" value="TC" <?= ($_POST["item_checklist"] == "TC" ? "checked" : "") ?>> Técnico
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="item_checklist" id="item_checklist_tc" value="OV" <?= ($_POST["item_checklist"] == "OV" ? "checked" : "") ?>> Obra Vinculada
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="item_checklist" id="item_checklist_tt" value="TT" <?= ($_POST["item_checklist"] == "TT" ? "checked" : "") ?>> Todas
                    </label>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação do Checklist:
            </div>
            <div class="col-lg-10">
                <?php
                $sql = "SELECT esdid as codigo, esddsc as descricao
                     FROM workflow.estadodocumento
                     WHERE  tpdid = " . TPID_CHECKLIST_VALIDACAO . "
                     AND esdstatus = 'A'
                     ORDER BY esdid";

                $dados = $db->carregar($sql);
                $dados = $dados ? $dados : array();
                ?>
                <select name="esdid_ck[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['esdid_ck']) && in_array($key['codigo'], $_POST['esdid_ck'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Responsável pela análise:
            </div>
            <div class="col-lg-10">
                <?php
                $sql = "SELECT DISTINCT usuario.usucpf as codigo, usuario.usunome as descricao
                     FROM seguranca.usuario usuario
                     LEFT JOIN seguranca.perfilusuario perfilusuario     ON usuario.usucpf = perfilusuario.usucpf
                     LEFT JOIN seguranca.perfil perfil                   ON perfil.pflcod  = perfilusuario.pflcod
                     INNER JOIN seguranca.usuario_sistema usuariosistema ON usuario.usucpf = usuariosistema.usucpf
                     WHERE usuario.usustatus = 'A'
                       AND usuariosistema.suscod = 'A'
                       AND usuariosistema.sisid = '".$_SESSION['sisid']."'
                     ORDER BY usuario.usunome";

                $dados = $db->carregar($sql);
                $dados = $dados ? $dados : array();
                ?>
                <select name="usucpf[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['usucpf']) && in_array($key['codigo'], $_POST['usucpf'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação da Obra:
            </div>
            <div class="col-lg-10">
                <?php
                $sql = "SELECT esdid as codigo, esddsc as descricao
                                        FROM workflow.estadodocumento
                                        WHERE tpdid='" . TPDID_OBJETO . "'
                                          AND esdstatus='A'
                                        ORDER BY esdordem";

                $dados = $db->carregar($sql);
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
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Municípios:
            </div>
            <div class="col-lg-10">
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
        <div class="row">
            <div class="col-lg-2">
                Programa:
            </div>
            <div class="col-lg-10">
                <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['prfid']) && in_array($key['codigo'], $_POST['prfid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Fonte:
            </div>
            <div class="col-lg-10">
                <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tooid']) && in_array($key['codigo'], $_POST['tooid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Data de Cadastro:
            </div>
            <div class="col-lg-10">
                <?php
                    $data_de = (empty($_POST['ckfdatainclusao_de'])) ? '' : $_POST['ckfdatainclusao_de'];
                    $data_ate = (empty($_POST['ckfdatainclusao_ate'])) ? '' : $_POST['ckfdatainclusao_ate'];
                ?>
                de: <input type="text" id="ckfdatainclusao_de" name="ckfdatainclusao_de" value="<?php echo $data_de; ?>" size="15" maxlength="10" class="normal" >
                &nbsp;
                até: <input type="text" id="ckfdatainclusao_ate" name="ckfdatainclusao_ate" value="<?php echo $data_ate; ?>" size="15" maxlength="10" class="normal">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Data da Situação:
            </div>
            <div class="col-lg-10">
                <?php
                    $data_de = (empty($_POST['htddata_de'])) ? '' : $_POST['htddata_de'];
                    $data_ate = (empty($_POST['htddata_ate'])) ? '' : $_POST['htddata_ate'];
                ?>
                de: <input type="text" id="htddata_de" name="htddata_de" value="<?php echo $data_de; ?>" size="15" maxlength="10" class="normal">
                &nbsp;
                até: <input type="text" id="htddata_ate" name="htddata_ate" value="<?php echo $data_ate; ?>" size="15" maxlength="10" class="normal">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Possui pendência:
            </div>
            <div class="col-lg-10">
            <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" name="possui_pendencia" id="possui_pendencia_s" value="S" <?= ($_POST["item_checklist"] == "S" ? "checked" : "") ?>> Sim
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="possui_pendencia" id="possui_pendencia_n" value="N" <?= ($_POST["item_checklist"] == "N" ? "checked" : "") ?>> Não
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="possui_pendencia" id="possui_pendencia_t" value="T" <?= ($_POST["item_checklist"] == "T" ? "checked" : "") ?>> Todos
                    </label>
                </div>
            </div>
        </div>
    </div>

    <div class="row form-filters text-center">
        <div class="col">
            <button type="button" name="btn_filtrar" id="btn_filtrar" class="btn btn-success" onclick="javascript:getLista('visual');">
                <span class="glyphicon glyphicon-search"></span> Pesquisar
            </button>
            <button type="button" name="btn_excel" id="btn_excel" class="btn btn-primary" onclick="javascript:getLista('xls');">
                <span class="glyphicon glyphicon-download-alt"></span> Gerar Excel
            </button>
        </div>
    </div>
        
</form>

<?php
if ($_REQUEST['form'] == '1') {
    unset($_SESSION['obras2']['empid']);
    unset($_SESSION['obras2']['obrid']);
    montaLista($_REQUEST['tipo_relatorio']);
}

function montaLista($tipo) {
    global $db;
    $restricao = new ChecklistFnde();
    $arr = $restricao->getListaChecklistGeral('array');
    //Para o método $db->monta_lista, as chaves do array tem q ser crescente de 0 a X. Se algum número não seguir a ordem, dá erro.
    $arrFinal = array();
    foreach ($arr as $key => $value) {
        $arrFinal[] = $value;

    }

    $cabecalho = array("ID da Obra", "Nome da Obra", "UF", "Município", "Situação da Obra", "% Executado", /*"Ação",*/ "ID do Checklist", "Tipo", "Responsável",
                       "Data de Inclusão", "Situação", "Data da Situação", "Possui Pendências");
    if ($tipo == 'xls') {
        $cabecalho = array("ID da Obra", "Nome da Obra", "UF", "Município", "Situação da Obra", "% Executado", /*"Ação",*/ "ID do Checklist", "Tipo", "Responsável",
                           "Data de Inclusão", "Situação", "Data da Situação", "Possui Pendências");
        ob_clean();
        ini_set("memory_limit", "512M");
        header("Content-type: application/excel; name=Lista de Checklists FNDE.xls");
        header("Content-Disposition: attachment; filename=Lista de Checklists FNDE.xls");
        $db->sql_to_xml_excel($arr, 'ListadeChecklistsFNDE', $cabecalho);
    } else {
        $db->monta_lista($arr, $cabecalho, 30, 10, 'N', 'center', 'N', 'N');
    }
}
?>


<script>
    $1_11(document).ready(function () {
        $1_11('select[name="esdid_ck[]"]').chosen();
        $1_11('select[name="usucpf[]"]').chosen();
        $1_11('select[name="esdid_obr[]"]').chosen();
        $1_11('select[name="muncod[]"]').chosen();
        $1_11('select[name="prfid[]"]').chosen();
        $1_11('select[name="tooid[]"]').chosen();
        $1_11('select[name="tpoid[]"]').chosen();
    });


</script>

<script lang="javascript">
    setTimeout(function() {

        jQuery('#ckfdatainclusao_de').mask('99/99/9999');
        jQuery('#htddata_de').mask('99/99/9999');
        jQuery('#ckfdatainclusao_ate').mask('99/99/9999');
        jQuery('#htddata_ate').mask('99/99/9999');

        jQuery("#ckfdatainclusao_de").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });
        jQuery("#ckfdatainclusao_ate").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });
        jQuery("#htddata_de").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            'showAnim': 'drop'
        });
        jQuery("#htddata_ate").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            'showAnim': 'drop'
        });

    }, 500);
</script>

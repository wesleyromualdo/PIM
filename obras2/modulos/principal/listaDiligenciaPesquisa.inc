<?php

    if($_GET['novoDoc'] == true){
       atualizaDocidNullRestricao($_GET['rstid']);
    }

if($_REQUEST['form'] == '1' && $_REQUEST['tipo_relatorio'] == 'xls'){
    ini_set("memory_limit", "7000M");
    montaLista($_REQUEST['tipo_relatorio']);
    exit;
}
    ?>

    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

    <?php

    include  APPRAIZ."includes/cabecalho.inc";
    print '<br/>';
    $titulo_modulo    = 'Lista de Diligências';
    $subtitulo_modulo = 'Pesquisa dos registros de todas as Obras';
    monta_titulo_obras($titulo_modulo, $subtitulo_modulo);

    //Fluxo de Diligência
    $tpdid = TPDID_DILIGENCIA;

    ?>

    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

    <script type="text/javascript">

    function getEstados(){
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
    }

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

    function getLista(tipo){
        var formulario = document.formulario;
        var tipo_relatorio = tipo;
        prepara_formulario();
        document.getElementById('tipo_relatorio').value = tipo_relatorio;
        formulario.submit();
        document.getElementById('tipo_relatorio').value = '';
    }

    function alterarDiligencia(dlgid, obrid, empid){
        var url = '?modulo=principal/cadDiligencia&acao=O&dlgid='+dlgid+'&obrid='+obrid+'&empid='+empid;
        janela = window.open(url, 'inserirDiligencia', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' );
        janela.focus();
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


<form name="formulario" id="formulario" action="" method="post">

    <input type="hidden" name="form" value="1" />
    <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" />

    <div class="form-filters">
        <div class="row">
            <div class="col-lg-2">
                Nome da Obra/ID:
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
                ID da Diligência:
            </div>
            <div class="col-lg-10">
                <?php
                $val = (!empty($_POST['dlgid'])) ? $_POST['dlgid'] : '';
                echo campo_texto('dlgid', 'N', 'S', '', 70, 80, '', '', '', '', '', 'id="rstid"', '', $val);
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
                Tipo de Diligência:
            </div>
            <div class="col-lg-10">
                <select name="tpdid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php   $tipoDiligencia = new TipoDiligencia();
                    $dados = $tipoDiligencia->listaCombo();
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tpdid']) && in_array($key['codigo'], $_POST['tpdid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação da Diligência:
            </div>
            <div class="col-lg-10">
                <?php
                    $sql = '
                            SELECT esdid as codigo, esddsc as descricao
                            FROM workflow.estadodocumento
                            WHERE tpdid = '.TPDID_DILIGENCIA;

                    $dados = $db->carregar($sql);
                    $dados = $dados ? $dados : array();
                ?>
                <select name="esdid_ri[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['esdid_ri']) && in_array($key['codigo'], $_POST['esdid_ri'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
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
                $sql_obr = "SELECT esdid as codigo, esddsc as descricao
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
                    $data_de  = '';
                    $data_ate = '';
                ?>
                de: <input type="text" id="dlgdtinclusao_de" name="dlgdtinclusao_de" value="<?php echo $data_de;?>" size="15" maxlength="10" class="normal" >
                &nbsp;
                até: <input type="text" id="dlgdtinclusao_ate" name="dlgdtinclusao_ate" value="<?php echo $data_ate;?>" size="15" maxlength="10" class="normal">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Data de Superação:
            </div>
            <div class="col-lg-10">
                <?php
                    $data_de  = '';
                    $data_ate = '';
                ?>
                de: <input type="text" id="dlgdtsuperacao_de" name="dlgdtsuperacao_de" value="<?php echo $data_de;?>" size="15" maxlength="10" class="normal">
                &nbsp;
                até: <input type="text" id="dlgdtsuperacao_ate" name="dlgdtsuperacao_ate" value="<?php echo $data_ate;?>" size="15" maxlength="10" class="normal">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Data do Último Tramite:
            </div>
            <div class="col-lg-10">
                <?php
                    $ultimotramite_de  = $_REQUEST['ultimotramite_de'];
                    $ultimotramite_ate = $_REQUEST['ultimotramite_ate'];
                ?>
                de: <input type="text" id="ultimotramite_de" name="ultimotramite_de" value="<?php echo $ultimotramite_de;?>" size="15" maxlength="10" class="normal">
                &nbsp;
                até: <input type="text" id="ultimotramite_ate" name="ultimotramite_ate" value="<?php echo $ultimotramite_ate;?>" size="15" maxlength="10" class="normal">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Existe ressalva:
            </div>
            <div class="col-lg-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" name="dlgflressalva" id="dlgflressalva_s" value="S" <?= ($_POST['dlgflressalva'] == "S" ? "checked" : "") ?>> Sim
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="dlgflressalva" id="dlgflressalva_n" value="N" <?= ($_POST['dlgflressalva'] == "N" ? "checked" : "") ?>> Não
                    </label>
                    <label class="btn btn-default <?= ($_POST['dlgflressalva'] == "" || $_POST['dlgflressalva'] == "T" ? "active" : "") ?>">
                        <input type="radio" name="dlgflressalva" id="dlgflressalva_t" value="T" <?= ($_POST['dlgflressalva'] == "T" ? "checked" : "") ?>> Todas
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

<div class="row form-filters">
    <div class="col-lg-12">
        <b>Providência: </b>
        <span><img style=" vertical-align:middle" src="/imagens/0_alerta.png" title="Aguardando Providência ou Aguardando Correção" /> Aguardando Providência ou Aguardando Correção | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_inativo.png" title="Aguardando Correção ou Aguardando Correção há mais de 15 dias" /> Aguardando Providência ou Aguardando Correção há mais de 15 dias | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_ativo.png" title="Aguardando Análise FNDE"/> Aguardando Análise FNDE | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_concluido.png" title="Superada" /> Superada | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_concluido_2.png" title="Justificada" /> Justificada | </span>
        <span><img style=" vertical-align:middle" src="/imagens/0_inexistente.png" title="Cancelada" /> Cancelada </span>
    </div>
</div>

<script lang="javascript">

setTimeout(function(){

    jQuery('#dlgdtinclusao_de').mask('99/99/9999');
    jQuery('#dlgdtsuperacao_de').mask('99/99/9999');
    jQuery('#dlgdtinclusao_ate').mask('99/99/9999');
    jQuery('#dlgdtsuperacao_ate').mask('99/99/9999');
    jQuery('#ultimotramite_de').mask('99/99/9999');
    jQuery('#ultimotramite_ate').mask('99/99/9999');

    var d = new Date();
    var data_hoje = d.toUTCString();

    jQuery("#dlgdtinclusao_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        showAnim:'drop'
    });
    jQuery("#dlgdtinclusao_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        showAnim:'drop'
    });
    jQuery("#dlgdtsuperacao_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });
    jQuery("#dlgdtsuperacao_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });

    jQuery("#ultimotramite_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });jQuery("#ultimotramite_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });

}, 500);

</script>

<?php
if($_REQUEST['form'] == '1' || ( ($_GET['acao'] == 'L') AND possui_perfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE)) ) ){
    montaLista($_REQUEST['tipo_relatorio']);
}


function montaLista($tipo){
    global $db;
    $diligencia = new Diligencia();
    $sql       = $diligencia->getDadosListaDiligencia('sql', $tipo);

    $cabecalho = array( "Ação", "ID Obra", "ID Item", "Providência", "Situação", "Estado", "Município", "Esfera", "Fase", "Tipo",
        "Nome da Obra", "Situação Obra", "Data Cadastro", "Criado por", "Previsão da Providência", "Superação", "Superado por", "Último Tramite", "Data do Último Tramite");

    //ini xls
    if($tipo == 'xls'){

        $dados = $db->carregar($sql);

        $dados = (!$dados) ? array() : $dados;

        foreach ($dados as $key => $value){
            $historico = explode(';', $value['historico']);
            $dados[$key] = $dados[$key] + $historico;
            unset($dados[$key]['historico']);

            $contador[] = count($historico);

        }
        //ver(d,$contador);
        $i = 1;
        if($contador) {
            while ($i <= (max($contador) / 3 + 1)) {
                $cabecalho_extra[] = "Data {$i}";
                $cabecalho_extra[] = "Ação {$i}";
                $cabecalho_extra[] = "Usuário {$i}";
                $i++;
            }

        }

        $cabecalho = array( "ID Obra", "ID Item", "Situação", "Estado", "Município", "Esfera", "Fase", "Tipo", "Tipologia",
            "Nome da Obra", "Situação","Data Cadastro", "Criado por", "Previsão da Providência", "Superação", "Superado por", "Usuário da Operação", "Data da Última Tramitação","Descrição","Providência","Divisão");
        ini_set("memory_limit", "7000M");

        if(!empty($cabecalho_extra)){

            $novo_cabecalho = array_merge($cabecalho,$cabecalho_extra);
        }
        else{

            $novo_cabecalho = $cabecalho;
        }

        $db->sql_to_xml_excel ($dados, 'Listadeicia', $novo_cabecalho);
    }else{
//        ver($sql,d);
        $db->monta_lista($sql, $cabecalho, 30, 10,'N','center', 'N', 'N');
    }//end xls
}

?>
<script>
    $1_11(document).ready(function () {

        $1_11('select[name="tooid[]"]').chosen();
        $1_11('select[name="prfid[]"]').chosen();
        $1_11('select[name="muncod[]"]').chosen();
        $1_11('select[name="esdid_obr[]"]').chosen();
        $1_11('select[name="esdid_ri[]"]').chosen();
        $1_11('select[name="tpdid[]"]').chosen();
        $1_11('select[name="tpoid[]"]').chosen();

    });


</script>

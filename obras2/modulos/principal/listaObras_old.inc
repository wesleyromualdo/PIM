<?php 
    

    require APPRAIZ . 'obras2/includes/principal/listaObras/ctrl.php';

    

    //Chamada de programa
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";

    $arAba = getArAba('listaorgao');
    echo montarAbasArray($arAba, "?modulo=principal/listaObras&acao=A&orgid=" . $orgid);

    monta_titulo_obras('Lista de Obras', 'Filtre as Obras.');

?> 


<link rel="stylesheet" type="text/css" href="/includes/superTitle.css"/>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="/obras2/js/obras2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<?php
    auto_require_refactor_obras2($modulo = $_REQUEST['modulo'],  $camada = 'css');
?>


<br>
<form method="post" name="formListaObra" id="formListaObra">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="obrid" id="obrid" value="">
    <input type="hidden" name="empid" id="empid" value="">
    <input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">
    <table align="center" style='background-color: #FFFFff' border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="divLabelForm" width="15%">Nome da Obra / ID:</td>
            <td>
                <?= campo_texto('obrbuscatexto', 'N', 'S', '', 70, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <a href="javascript:exibeBuscaAvancada( true );" id="labelBuscaAvancada">[Busca avançada]</a>
            </td>
        </tr>
        <tr>
            <td id="tr_busca_avancada" colspan="2" style="display: none;">


                <!-- Inicio tablela novo layout               -->
            <table align="center" style='background-color: #FFFFff' border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <th colspan="4" style='background-color: #dcdcdc'>
                        Busca Avançada
                        <a style="float:right; " onclick="exibeBuscaAvancada(false);">[Fechar]</a>
                    </th>
                </tr>
                <tr>
<!--                    Coluna 1-->
                    <td width="50%">
                        <table width="100%">
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">Processo:</td>
                                <td class="tdInputForm">
                                    <?php
                                    echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, '');
                                    ?>
                                    Ano:&nbsp;
                                    <?php
                                    echo campo_texto('ano_processo', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">Convênio/Termo:</td>
                                <td class="tdInputForm">
                                    <?php
                                    echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, '');
                                    ?>
                                    Ano:&nbsp;
                                    <?php
                                    echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Situação:
                                </td>
                                <td class="tdInputForm">
                                    <select name="strid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php $situacaoRegistro = new SituacaoRegistro();
                                        foreach ($situacaoRegistro->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($strid) && in_array($key['codigo'], $strid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Tipo de Obra:
                                </td>
                                <td class="tdInputForm">
                                    <select name="tobid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php  $tipoObra = new TipoObra();
                                        foreach ($tipoObra->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($tobid) && in_array($key['codigo'], $tobid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Classificação da Obra:
                                </td>
                                <td class="tdInputForm">
                                    <select name="cloid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php  $classificacaoObra = new ClassificacaoObra();
                                        foreach ($classificacaoObra->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($cloid) && in_array($key['codigo'], $cloid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Tipologia:
                                </td>
                                <td class="tdInputForm">
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
                                <td class="SubTituloDireita tdLabelForm">
                                    Programa:
                                </td>
                                <td class="tdInputForm">
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
                                <td class="SubTituloDireita tdLabelForm">
                                    Fonte:
                                </td>
                                <td class="tdInputForm">
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
                                <td class="SubTituloDireita tdLabelForm">
                                    Modalidade de Ensino:
                                </td>
                                <td class="tdInputForm">
                                    <select name="moeid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php  $modalidade = new ModalidadeEnsino();
                                        foreach ($modalidade->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($moeid) && in_array($key['codigo'], $moeid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Última atualização:</td>
                                <td class="tdInputForm">
                                    <?php
                                    $ultatualizacao = $_POST["ultatualizacao"];
                                    $arSel = array(
                                        array("codigo" => 1, "descricao" => "Em até 25 dias"),
                                        array("codigo" => 2, "descricao" => "Entre 25 e 30 dias"),
                                        array("codigo" => 3, "descricao" => "Mais de 30 dias"),
                                    );
                                    $db->monta_combo("ultatualizacao", $arSel, "S", "Todos", "", "", "", "200", "N", "ultatualizacao");
                                    ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">
                                    Tipo Vistoria (Realizado por):
                                </td>
                                <td class="tdInputForm">
                                    <select name="rsuid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                        <?php
                                        $realizacaosupervisao = new RealizacaoSupervisao();
                                        foreach ($realizacaosupervisao->listaCombo() as $key) {
                                            ?>
                                            <option
                                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($rsuid) && in_array($key['codigo'], $rsuid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                        <?php } ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Valor da Obra:</td>
                                <td class="tdInputForm">
                                    De:&nbsp;
                                    <?php
                                    echo campo_texto('obrvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                    ?>
                                    Até:&nbsp;
                                    <?php
                                    echo campo_texto('obrvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                    ?>
                                </td>
                            </tr>
                        </table>
                    </td>

<!--                    Coluna 2-->
                    <td width="50%">
                        <table width="100%">
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">Municípios(s):</td>
                                <td class="td_municipio tdInputForm">
                                        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                                            <?php   $municipio = new Municipio();
                                            foreach ($municipio->listaComboMulti() as $key) {
                                                ?>
                                                <option
                                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                            <?php } ?>
                                        </select>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm">Unidade:</td>
                                <td id="td_unidade" class="tdInputForm">
                                    <?php if(!empty($muncod)){
                                        $entidade = new Entidade();
                                        $a = $entidade->listaComboMulti($muncod);
                                        ?>
                                        <select name="entid[]" class="chosen-select" multiple data-placeholder="Selecione">
                                            <?php

                                            foreach ($entidade->listaComboMulti($muncod) as $key) {
                                                ?>
                                                <option
                                                        value="<?php echo $key['codigo'] ?>" <?php if (isset($entid) && in_array($key['codigo'], $entid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                                            <?php } ?>
                                        </select>
                                    <?php } ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui foto:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="totalfoto" id=""
                                           value="S" <?= ($_POST["totalfoto"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="totalfoto" id=""
                                           value="N" <?= ($_POST["totalfoto"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="totalfoto" id=""
                                           value=""  <?= ($_POST["totalfoto"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui vistoria:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="possui_vistoria" id=""
                                           value="S" <?= ($_POST["possui_vistoria"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="possui_vistoria" id=""
                                           value="N" <?= ($_POST["possui_vistoria"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="possui_vistoria" id=""
                                           value=""  <?= ($_POST["possui_vistoria"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui restrição:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="res_estado" id=""
                                           value="S" <?= ($_POST["res_estado"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="res_estado" id=""
                                           value="N" <?= ($_POST["res_estado"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="res_estado" id=""
                                           value=""  <?= ($_POST["res_estado"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Restrição superada:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="res_superada" id=""
                                           value="S" <?= ($_POST["res_superada"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="res_superada" id=""
                                           value="N" <?= ($_POST["res_superada"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="res_superada" id=""
                                           value=""  <?= ($_POST["res_superada"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui Aditivo:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="ocraditivado" id=""
                                           value="S" <?= ($_POST["ocraditivado"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="ocraditivado" id=""
                                           value="N" <?= ($_POST["ocraditivado"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="ocraditivado" id=""
                                           value=""  <?= ($_POST["ocraditivado"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui Supervisão por O.S:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="responsavel" id=""
                                           value="S" <?= ($_POST["responsavel"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="responsavel" id=""
                                           value="N" <?= ($_POST["responsavel"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="responsavel" id=""
                                           value=""  <?= ($_POST["responsavel"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Listar as Obras Vinculantes?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="obrvinculada" id=""
                                           value="S" <?= ($_POST["obrvinculada"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="obrvinculada" id=""
                                           value="N" <?= ($_POST["obrvinculada"] == "N" || $_POST["obrvinculada"] == "" ? "checked='checked'" : "") ?>/>
                                    Não
                                    <input type="radio" name="obrvinculada" id=""
                                           value="T"  <?= ($_POST["obrvinculada"] == "T" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Recebeu o primeiro repasse?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="repasse" id=""
                                           value="S" <?= ($_POST["repasse"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="repasse" id=""
                                           value="N" <?= ($_POST["repasse"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="repasse" id=""
                                           value="T"  <?= ($_POST["repasse"] == "T" || $_POST["repasse"] == "" ? "checked='checked'" : "") ?> />
                                    Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui checklist administrativo?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="chk_adm" id=""
                                           value="S" <?= ($_POST["chk_adm"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="chk_adm" id=""
                                           value="N" <?= ($_POST["chk_adm"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="chk_adm" id=""
                                           value="T"  <?= ($_POST["chk_adm"] == "T" || $_POST["chk_adm"] == "" ? "checked='checked'" : "") ?> />
                                    Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui checklist 2° Parcela?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="chk_2pc" id=""
                                           value="S" <?= ($_POST["chk_2pc"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="chk_2pc" id=""
                                           value="N" <?= ($_POST["chk_2pc"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="chk_2pc" id=""
                                           value="T"  <?= ($_POST["chk_2pc"] == "T" || $_POST["chk_2pc"] == "" ? "checked='checked'" : "") ?> />
                                    Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">% Executado da Obra:</td>
                                <td class="tdInputForm">
                                    <div style="margin: 5px;">
                                        <label id="txtPercentualinicial"></label>
                                        &nbsp;&nbsp;
                                        <label id="txtPercentualfinal"></label>
                                        <input type="hidden" id="percentualfinal" name="percentualfinal" readonly>
                                        <input type="hidden" id="percentualinicial" name="percentualinicial" readonly>
                                        <div style="width: 200px;">
                                        <div id="slider-range"></div>
                                        </div>
                                    </div>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">% Executado da Obra:</td>
                                <td class="tdInputForm">
                                    <table>
                                        <tr>
                                            <th>Mínimo</th>
                                            <th>Máximo</th>
                                        </tr>
                                        <tr>
                                            <?php
                                            for ($i = 0; $i <= 100; $i++) {
                                                $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                                            }
                                            $percentualinicial = $_POST['percentualinicial'];
                                            $percentualfinal = $_POST['percentualfinal'];
                                            $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                                            print '<td>';
                                            $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                                            print '</td><td>';
                                            $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percfinal);
                                            print '</td>';
                                            ?>
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Possui Marcador de Órgão de Controle?</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="chk_org_controle" id=""
                                           value="S" <?= ($_POST["chk_org_controle"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="chk_org_controle" id=""
                                           value="N" <?= ($_POST["chk_org_controle"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="chk_org_controle" id=""
                                           value="T"  <?= ($_POST["chk_org_controle"] == "T" || $_POST["chk_org_controle"] == "" ? "checked='checked'" : "") ?> />
                                    Todas
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Funcionamento:</td>
                                <td class="tdInputForm">
                                    <table>
                                        <tr>
                                            <td>
                                                <input type="radio" <?= ($_POST['funcionamentoflag'] == 1) ? 'checked' : ''; ?>
                                                       name="funcionamentoflag" id="moefuncionamentoflag_s" value="1"/>
                                                Em Funcionamento:
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <input type="radio" <?= ($_POST['funcionamentoflag'] == 2) ? 'checked' : ''; ?>
                                                       name="funcionamentoflag" id="moefuncionamentoflag_pi" value="2"/>
                                                Previsão de Inauguração:
                                                <?php echo campo_data2('moedtprevinauguracao_i', 'S', true, '', null, '', ' ', '', ''); ?>
                                                &nbsp;até&nbsp;
                                                <?php echo campo_data2('moedtprevinauguracao_f', 'S', true, '', null, '', ' ', '', ''); ?>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <input type="radio" <?= ($_POST['funcionamentoflag'] == 3) ? 'checked' : ''; ?>
                                                       name="funcionamentoflag" id="moefuncionamentoflag_if" value="3"/>
                                                Previsão de Início de Funcionamento:
                                                <?php echo campo_data2('moedtpreviniciofunc_i', 'S', true, '', null, '', ' ', '', ''); ?>
                                                &nbsp;até&nbsp;
                                                <?php echo campo_data2('moedtpreviniciofunc_f', 'S', true, '', null, '', ' ', '', ''); ?>
                                            </td>
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita tdLabelForm" style="width: 190px;">Mobiliário Empenhado:</td>
                                <td class="tdInputForm">
                                    <input type="radio" name="moeempenhadoflag" id=""
                                           value="S" <?= ($_POST["moeempenhadoflag"] == "S" ? "checked='checked'" : "") ?>/> Sim
                                    <input type="radio" name="moeempenhadoflag" id=""
                                           value="N" <?= ($_POST["moeempenhadoflag"] == "N" ? "checked='checked'" : "") ?>/> Não
                                    <input type="radio" name="moeempenhadoflag" id=""
                                           value=""  <?= ($_POST["moeempenhadoflag"] == "" ? "checked='checked'" : "") ?> /> Todas
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
                <br><br><br>


<!-- Fim tablela novo layout               -->



            </td>
        </tr>
        <tr>
            <td class="divTituloPadrao" width="15%" colspan="2" align="center">
                <?php
                if (possui_perfil(PFLCOD_SUPER_USUARIO)):
                    ?>
                    <div style="float:left" class="novaObra">
                        <a style="cursor: pointer;">
                            <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Nova obra"> Nova Obra
                        </a>
                    </div>
                <?php endif; ?>
                <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                <input type="button" name="btnEexcel" class="btnEexcel" value="Gerar Excel"/>
            </td>
        </tr>
    </table>
</form>


<?php  

    require APPRAIZ . 'obras2/includes/principal/listaObras/listagem.php';
    require APPRAIZ . 'obras2/includes/principal/listaObras/js.php'; 
?>
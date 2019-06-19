<?php
ini_set("memory_limit", "512M");
$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
	$_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid 						 = $_SESSION['obras2']['orgid'];

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';


#Requisicao Limpar
if ($_POST['req'] == 'limpar') {
    unset($_SESSION['obras2']['solicitacaodesembolso']['filtros']);

    echo "<script>window.location.href = window.location.href;</script>";
    exit();
}


if ($_POST['req'] == 'apagar' && !empty($_POST['sldid'])) {
    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
        $solicitacaoDesembolso = new SolicitacaoDesembolso($_POST['sldid']);
        $solicitacaoDesembolso->sldstatus = 'I';
        $solicitacaoDesembolso->salvar();
        $solicitacaoDesembolso->commit();
        die('<script type="text/javascript">
            alert(\'Operação realizada com sucesso!\');
            location.href=\'?modulo=principal/listaObrasSolicitacaoDesembolso&acao=A\';
         </script>');
    }
}


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
echo "<br>";


monta_titulo_obras('Lista de Solicitações de Desembolso', 'Filtre as Solicitações');

if(empty($_POST) && !empty($_SESSION['obras2']['solicitacaodesembolso']['filtros'])) {
    unset($_SESSION['obras2']['solicitacaodesembolso']['filtros']['req']);
    $_POST = $_SESSION['obras2']['solicitacaodesembolso']['filtros'];
} else
    $_SESSION['obras2']['solicitacaodesembolso']['filtros'] = $_POST;

//ver($_SESSION['obras2'], d);

extract( $_POST );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form method="post" name="formListaObraSolicitacaoDesembolso" id="formListaObraSolicitacaoDesembolso">
	<input type="hidden" name="req" id="req" value="">
	<input type="hidden" name="obrid" id="obrid" value="">
	<input type="hidden" name="empid" id="empid" value="">
	<input type="hidden" name="sldid" id="sldid" value="">

    <div class="form-filters">
        <div class="row">
            <div class="col-lg-2">
                Nome da Obra / ID:
            </div>
            <div class="col-lg-10">
                <?=campo_texto('obrbuscatexto','N','S','',70,100,'','', '', '', '', 'id="obrbuscatexto"');?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                ID da Solicitação:
            </div>
            <div class="col-lg-10">
                <?=campo_texto('sldid','N','S','',70,100,'##########','', '', '', '', 'id="sldid"');?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação da Obra:
            </div>
            <div class="col-lg-10">
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
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Tipologia:
            </div>
            <div class="col-lg-10">
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
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($prfid) && in_array($key['codigo'], $prfid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
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
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($tooid) && in_array($key['codigo'], $tooid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <? if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))): ?>
        <div class="row">
            <div class="col-lg-2">
                Municípios:
            </div>
            <div class="col-lg-10">
                <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                    <?php   $municipio = new Municipio();
                    foreach ($municipio->listaComboMulti() as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação:
            </div>
            <div class="col-lg-10">
                <?php
                $dado = $db->carregar('
                                        SELECT
                                            \'999\' codigo,
                                            \'Aguardando Análise FNDE\' descricao
                                        UNION
                                        SELECT
                                            esdid codigo,
                                            esddsc descricao
                                        FROM workflow.estadodocumento
                                        WHERE
                                            tpdid = '.TPDID_SOLICITACAO_DESEMBOLSO.'
                                        ');

                ?>

                <select name="esdidsituaco[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($esdidsituaco) && in_array($key['codigo'], $esdidsituaco)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Convênio/Termo:
            </div>
            <div class="col-lg-10">
                Número: 
                <?php
                echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, '');
                ?>
                Ano: 
                <?php
                echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Processo:
            </div>
            <div class="col-lg-10">
                Número:
                <?php
                echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, '');
                ?>
                Ano:
                <?php
                echo campo_texto('ano_processo', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                % Executado da Obra:
            </div>
            <div class="col-lg-10">
                <?php
                    for ($i = 0; $i <= 100; $i++) {
                        $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                    }
                    $percentualinicial = $_POST['percentualinicial'];
                    $percentualfinal = $_POST['percentualfinal'];
                    $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                    print 'Mínimo: ';
                    $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                    print ' Máximo: ';
                    $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percfinal);
                ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                % Pago:
            </div>
            <div class="col-lg-10">
                <?php
                    $percentualpagoPagoinicial = $_POST['percentualpagoinicial'];
                    $percentualpagofinal = $_POST['percentualpagofinal'];
                    $percfinal = $percentualpagofinal == '' ? 100 : $percentualpagofinal;
                    print 'Mínimo: ';
                    $db->monta_combo("percentualpagoinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualpagoinicial');
                    print ' Máximo: ';
                    $db->monta_combo("percentualpagofinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualpagofinal', false, $percfinal);
                ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                % Solicitado:
            </div>
            <div class="col-lg-10">
                <?php
                    $percentualsolicitadoinicial = $_POST['percentualsolicitadoinicial'];
                    $percentualsolicitadofinal = $_POST['percentualsolicitadofinal'];
                    $percfinal = $percentualsolicitadofinal == '' ? 100 : $percentualsolicitadofinal;
                    print 'Mínimo: ';
                    $db->monta_combo("percentualsolicitadoinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualsolicitadoinicial');
                    print ' Máximo: ';
                    $db->monta_combo("percentualsolicitadofinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualsolicitadofinal', false, $percfinal);
                ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                % Aprovado:
            </div>
            <div class="col-lg-10">
                <?php
                    $percentualaprovadoinicial = $_POST['percentualaprovadoinicial'];
                    $percentualaprovadofinal = $_POST['percentualaprovadofinal'];
                    $percfinal = $percentualaprovadofinal == '' ? 100 : $percentualaprovadofinal;
                    print 'Mínimo: ';
                    $db->monta_combo("percentualaprovadoinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualaprovadoinicial');
                    print ' Máximo: ';
                    $db->monta_combo("percentualaprovadofinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualaprovadofinal', false, $percfinal);
                ?>
            </div>
        </div>
        <? else: ?>
        <div class="row">
            <div class="col-lg-2">
                Situação: 
            </div>
            <div class="col-lg-10">
                <?php
                    $dado = $db->carregar('
                                            SELECT
                                              \'999\' codigo,
                                              \'Aguardando Análise FNDE\' descricao
                                            UNION
                                            SELECT
                                              esdid codigo,
                                              esddsc descricao
                                            FROM workflow.estadodocumento
                                            WHERE
                                              tpdid = '.TPDID_SOLICITACAO_DESEMBOLSO.'
                                              AND esdid NOT IN (' . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA . ', ' . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI . ', ' . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL . ')');

                ?>

                <select name="esdidsituaco[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($esdidsituaco) && in_array($key['codigo'], $esdidsituaco)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <? endif; ?>
        <div class="row">
            <div class="col-lg-2">
                Situação do Pedido:
            </div>
            <div class="col-lg-10">
                <?php

                $dado = Array(Array('codigo'=>'Solicitação Aprovada', 'descricao'=>'Solicitação Aprovada'),
                             Array('codigo'=>'agamento Solicitado', 'descricao'=>'Pagamento Solicitado'),
                             Array('codigo'=>'Efetivado', 'descricao'=>'Efetivado'));
                ?>
                <select name="sitpedido[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($sitpedido) && in_array($key['codigo'], $sitpedido)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <? if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))): ?>
        <div class="row">
            <div class="col-lg-2">
                Solicitações inseridas por carga:
            </div>
            <div class="col-lg-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" name="carga" value="1" <?= ($carga == 1 ? "checked" : "") ?>> Sim
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="carga" value="2" <?= ($carga == 2 ? "checked" : "") ?>> Não
                    </label>
                    <label class="btn btn-default <?= ($carga == 0 ? "active" : "") ?>">
                        <input type="radio" name="carga" value="0" <?= ($carga == 0 ? "checked" : "") ?>> Todas
                    </label>
                </div>
            </div>
        </div>
        <? endif; ?>
        <div class="row">
            <div class="col-lg-2">
                Data de Inserção:
            </div>
            <div class="col-lg-10">
                De: 
                <input type="text" class="date" id="data_de" name="data_de">
                 até 
                <input type="text" class="date" id="data_ate" name="data_de">
            </div>
        </div>
    </div>

    <div class="row form-filters text-center">
        <div class="col">
            <button type="button" name="pesquisar" class="btn btn-success pesquisar">
                <span class="glyphicon glyphicon-search"></span> Pesquisar
            </button>
            <button type="button" name="btnEexcel" class="btn btn-primary btnEexcel">
                <span class="glyphicon glyphicon-download-alt"></span> Gerar Excel
            </button>
            <? if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))): ?>
            <button type="button" name="btnEstatistica" class="btn btn-warning btnEstatistica">
                <span class="glyphicon glyphicon-flash"></span> Gerar Estatística
            </button>
            <?endif;?>
            <button type="button" id="button_limpar" class="btn btn-danger">
                <span class="glyphicon glyphicon-remove"></span> Limpar Filtros
            </button>
        </div>
    </div>
</form>

<?php

if ( $obrbuscatexto ){
    $obrbuscatexto = removeAcentos($obrbuscatexto);
    $obrid = limpaObridSec(trim($obrbuscatexto));
	$where[] = " ( UPPER(public.removeacento(o.obrnome)) ILIKE ('%" . $obrbuscatexto . "%') OR
				   public.removeacento(o.obrid::CHARACTER VARYING) ILIKE ('%" . $obrid . "%')OR
                        o.entidsecretaria::CHARACTER VARYING ILIKE ('%" . $obrid . "%') ) ";
}

if( $empesfera ){
	$empesfera = (array) $empesfera;
	$where[] = "emp.empesfera IN('" . implode("', '", $empesfera) . "')";
}


if ($_POST['sldid'])
    $where['sldid'] = "sv.sldid::text IN ('{$_POST['sldid']}')";

if ($_POST['sitpedido'] && $_POST['sitpedido'][0] != '')
    $where['sitpedido'] = "ps.situacao_pagamento  ILIKE ANY(ARRAY['%" . implode("%', '%", $_POST['sitpedido']) . "%'])  ";
//if ($_POST['estuf'] && $_POST['estuf'][0] != '')
    $where['estuf'] = "m.estuf IN ('SP')";
if ($_POST['muncod'] && $_POST['muncod'][0] != '')
    $where['muncod'] = "m.muncod IN ('" . implode("', '", $_POST['muncod']) . "')";
if ($_POST['processo'])
    $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$_POST['processo']}','.',''),'/',''),'-','')";
if ($_POST['ano_processo'])
    $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$_POST['ano_processo']}'";
if ($_POST['convenio'])
    $where['convenio'] = "p_conv.termo_convenio = '{$_POST['convenio']}'";
if ($_POST['ano_convenio'])
    $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$_POST['ano_convenio']}'";
if ($_POST['esdid'] && $_POST['esdid'][0] != '')
    $where['esdid'] = "dc.esdid IN (" . implode(', ', $_POST['esdid']) . ")";
if ($_POST['tpoid'] && $_POST['tpoid'][0] != '')
    $where['tpoid'] = "o.tpoid IN ('" . implode("', '", $_POST['tpoid']) . "')";
if ($_POST['prfid'] && $_POST['prfid'][0] != '')
    $where['prfid'] = "emp.prfid IN(" . implode(',', $_POST['prfid']) . ")";
if ($_POST['tooid'] && $_POST['tooid'][0] != '')
    $where['tooid'] = "o.tooid IN(" . implode(',', $_POST['tooid']) . ")";
if ($_POST['carga'] && $_POST['carga'] == '1' || $_POST['carga'] == '2')
    $where['carga'] = $_POST['carga'] == '1' ? "sv.usucpf = '00000000191'" : "sv.usucpf != '00000000191'";
if ($_POST['data_de'] && $_POST['data_ate'])
    $where['data'] = 'sv.slddatainclusao BETWEEN \'' . ajusta_data($_POST['data_de']) . '\' AND \'' . ajusta_data($_POST['data_ate']) . '\'';

if ($_POST['percentualinicial'] != '')
    $where['percentualinicial'] = "COALESCE(obrpercentultvistoria, 0) >= " . $_POST['percentualinicial'];
if ($_POST['percentualfinal'] != '')
    if ($_POST['percentualfinal'] < 100)
        $where['percentualfinal'] = "COALESCE(obrpercentultvistoria, 0) <= " . $_POST['percentualfinal'];

if ($_POST['percentualpagoinicial'] != '')
    $where['percentualpagoinicial'] = "COALESCE(((p.totalpago * 100) / p.vlrobra), 0) >= " . $_POST['percentualpagoinicial'];
if ($_POST['percentualpagofinal'] != '')
    if ($_POST['percentualpagofinal'] < 100)
        $where['percentualpagofinal'] = "COALESCE(((p.totalpago * 100) / p.vlrobra), 0) <= " . $_POST['percentualpagofinal'];

if ($_POST['percentualsolicitadoinicial'] != '')
    $where['percentualsolicitadoinicial'] = "COALESCE(sv.sldpercsolicitado, 0) >= " . $_POST['percentualsolicitadoinicial'];
if ($_POST['percentualsolicitadofinal'] != '')
    if ($_POST['percentualsolicitadofinal'] < 100)
        $where['percentualsolicitadofinal'] = "COALESCE(sv.sldpercsolicitado, 0) <= " . $_POST['percentualsolicitadofinal'];

if ($_POST['percentualaprovadoinicial'] != '')
    $where['percentualaprovadoinicial'] = "COALESCE(sv.sldpercpagamento, 0) >= " . $_POST['percentualaprovadoinicial'];
if ($_POST['percentualaprovadofinal'] != '')
    if ($_POST['percentualaprovadofinal'] < 100)
        $where['percentualaprovadofinal'] = "COALESCE(sv.sldpercpagamento, 0) <= " . $_POST['percentualaprovadofinal'];

if ($esdidsituaco){
    if (!possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))) {
        if(in_array(999, $esdidsituaco) ) {
            $esdidsituaco[] = ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI;
            $esdidsituaco[] = ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL;
            $esdidsituaco[] = ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA;
            $where[] = " d.esdid IN (" . implode(',', $esdidsituaco) . ") ";
        } else {
            $where[] = "  d.esdid IN (" . implode(',', $esdidsituaco) . ") ";
        }

    } else {
        $where[] = "  d.esdid IN (" . implode(',', $esdidsituaco) . ") ";
    }
}

if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

    $arrObr = array(
        PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
        PFLCOD_EMPRESA_VISTORIADORA_GESTOR
    );

    $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

    $arrOrg = Array(PFLCOD_ADMINISTRADOR,
        PFLCOD_CADASTRADOR_INSTITUCIONAL,
        PFLCOD_CONSULTA_TIPO_DE_ENSINO,
        PFLCOD_SUPERVISOR_MEC,
        PFLCOD_GESTOR_MEC);

    $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

    $resp = array();
    $arPflcod = array();
    $orWhere = array();

    if (possui_perfil($arrEst)) {
        $arPflcod = array_merge($arPflcod, $arrEst);
        $orWhere['estuf'] = "m.estuf IN ( SELECT estuf FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = ed.estuf)";
    }

    if (possui_perfil($arrObr)) {
        $arPflcod = array_merge($arPflcod, $arrObr);
        $orWhere['empid'] = "emp.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
    }

    if (possui_perfil($arrOrg)) {
        $arPflcod = array_merge($arPflcod, $arrOrg);
        $orWhere['orgid'] = "emp.orgid IN ( SELECT urs.orgid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = emp.orgid)";
    }

    if (possui_perfil($arrUni)) {
        $arPflcod = array_merge($arPflcod, $arrUni);
        $orWhere['entidunidade'] = "emp.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = emp.entidunidade)";
    }

    if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
        $usuarioResp = new UsuarioResponsabilidade();
        $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
        $arEmpid = ($arEmpid ? $arEmpid : array(0));

        $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
        $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

        $orWhere['sup'] = "emp.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = emp.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
    }

}

$esddsc = (possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))) ? "e.esddsc" : "CASE WHEN e.esdid IN (".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA.", ".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL." , ".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI.") THEN 'Aguardando Análise FNDE' ELSE e.esddsc END AS estado_documento" ;

$acao = '';
$with = '';
$estatistica = '';
if ($_POST['req'] != 'excel') {

    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
        $excluir = "<img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluirSolicitacao(' ||sv.sldid || ');\"
                        title=\"Excluir\">";
    }

    $acao = "'<center style=\"width:60px\">
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/icone_lupa.png\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: abreSolicitacao(' || sv.sldid  || ');\"
                        title=\"Alterar Solicitação\">
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: alterarObr(' || o.obrid || ');\"
                        title=\"Alterar Obra\">
                    $excluir
                 </center>' AS acao,";
}

if($_POST['req'] == 'estatistica' || $_POST['req'] == 'excel'){
    if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))){
        $with = "

            WITH estatistica_processo AS (

                SELECT
                    f.sldid, f.esdid, f.processo, SUM(f.dias) as dias
                FROM (

                    SELECT
                        s.sldid,
                        eo.esdid,
                        CASE
                            WHEN eo.esdid = 1575 THEN  'Administrativo'
                            WHEN eo.esdid = 1597 THEN  'Executivas'
                            WHEN eo.esdid = 1581 THEN  'Técnica'
                            WHEN eo.esdid = 1598 THEN  'Município'
                        END processo,
                        eo.esddsc as de,
                        ed.esddsc as para,
                        COALESCE ( (SELECT htddata FROM workflow.historicodocumento WHERE docid = d.docid AND htddata < h.htddata ORDER BY htddata DESC LIMIT 1),  d.docdatainclusao) as entrou,
                        h.htddata saiu,
                        DATE_PART( 'days',
                            (h.htddata - COALESCE ( (SELECT htddata FROM workflow.historicodocumento WHERE docid = d.docid AND htddata < h.htddata ORDER BY htddata DESC LIMIT 1),  d.docdatainclusao))
                            ) dias,
                        u.usunome as por,
                        (SELECT COUNT(*) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc  WHERE esdiddestino = 1598) ) as correcao

                    FROM obras2.obras o
                    JOIN obras2.solicitacao_desembolso s ON s.obrid = o.obrid AND s.sldstatus = 'A'
                    JOIN workflow.documento d ON d.docid = s.docid
                    JOIN workflow.estadodocumento e ON e.esdid = d.esdid AND e.esdid = 1576
                    JOIN workflow.historicodocumento h ON h.docid = d.docid
                    JOIN seguranca.usuario u ON u.usucpf = h.usucpf
                    JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
                    JOIN workflow.estadodocumento ed ON ed.esdid = a.esdiddestino
                    JOIN workflow.estadodocumento eo ON eo.esdid = a.esdidorigem
                    WHERE
                        o.obridpai IS NULL AND
                        o.obrstatus = 'A' AND
                        eo.esdid IN (1575, 1597, 1581, 1598)
                    ORDER BY s.sldid, h.htddata ASC

                ) AS f
                GROUP BY f.sldid, f.processo, f.esdid
                ORDER BY f.processo

            )

        ";

        $estatistica = '
            ,
            COALESCE((SELECT dias FROM estatistica_processo WHERE esdid = 1575 AND sldid = sv.sldid), 0) as administrativo,
            COALESCE((SELECT dias FROM estatistica_processo WHERE esdid = 1597 AND sldid = sv.sldid), 0) as executivas,
            COALESCE((SELECT dias FROM estatistica_processo WHERE esdid = 1581 AND sldid = sv.sldid), 0) as tecnica,
            COALESCE((SELECT dias FROM estatistica_processo WHERE esdid = 1598 AND sldid = sv.sldid), 0) as municipio,
            COALESCE((SELECT SUM (dias) FROM estatistica_processo WHERE sldid = sv.sldid),0) as total

        ';

    }
}

//string que monta a concatenação do campo obrid + idsecretaria para ser montada na query
$campoObridSec = montaCampoObridSec('o');

if(!is_array($orWhere))
	$orWhere = array();

$sql = "
    $with
    SELECT * FROM (


            SELECT DISTINCT ON (sv.sldid)
                $acao
                sv.sldid,
                {$campoObridSec} as obrid,
                o.obrnome,
                p_conv.pronumeroprocesso,
                p_conv.termo_convenio,
                p_conv.ano_termo_convenio,
                eo.esddsc as esddsc1,
                ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) as percentual_execucao,
                (p.totalpago * 100) / p.vlrobra as totalpago,
                sv.sldpercsolicitado,
                sv.sldpercpagamento,
                sv.sldpercobra,
                m.estuf,
                m.mundescricao,
                sv.sldjustificativa,
                u.usunome usunome1,
                TO_CHAR(sv.slddatainclusao, 'DD/MM/YYYY') slddatainclusao,
                $esddsc,
                ud.usunome as usunome,
                TO_CHAR(h.htddata, 'DD/MM/YYYY') as htddata,
                c.cmddsc as cmddsc,
                ps.situacao_pagamento
                $estatistica
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')

            LEFT JOIN workflow.documento dc ON dc.docid = o.docid
            LEFT JOIN workflow.estadodocumento eo ON eo.esdid = dc.esdid
            LEFT JOIN par.v_pagamento_total_por_obra p ON p.preid = o.preid
            LEFT JOIN (SELECT * FROM obras2.vm_termo_convenio_obras WHERE termo_convenio IS NOT NULL) as p_conv ON p_conv.obrid = o.obrid

            LEFT JOIN (

              SELECT f.sldid,  array_to_string(array_agg(f.situacao), '<br />') as situacao_pagamento FROM (
                    SELECT f.sldid, f.percentualpag || '% - ' || f.situacao_pagamento as situacao FROM (
                        SELECT sldid, situacao_pagamento, sum(percentualpag) as percentualpag FROM obras2.v_solicitacao_pagamento GROUP BY 1,2
                    ) as f
                ) as f
                GROUP BY f.sldid

            ) ps ON ps.sldid = sv.sldid

            JOIN obras2.empreendimento emp ON emp.empid = o.empid
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN territorios.municipio m ON m.muncod = ed.muncod
            JOIN seguranca.usuario u ON u.usucpf = sv.usucpf
            JOIN workflow.documento d ON d.docid = sv.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid
            LEFT JOIN workflow.historicodocumento h ON d.hstid = h.hstid
            LEFT JOIN workflow.comentariodocumento  c ON h.hstid = c.hstid AND c.cmdstatus = 'A'
            LEFT JOIN seguranca.usuario ud ON h.usucpf = ud.usucpf
            WHERE sv.sldstatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ',$where) : "") . "
            " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "

            ) as f

            ORDER BY 2
            ";


if ($_POST['req'] == 'excel') {
    $cabecalho = array('ID Solicitação', 'ID Obra', 'Obra', 'Nº Processo', 'Nº Termo/Convênio', 'Ano Termo/Convênio', 'Situação', '% Execução', '% Pago', '% Solicitado', '% Aprovado', '% Execução Validado', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Data de Cadastro',
        'Situação do Deferimento', 'Última tramitação', 'Data da Resposta', 'Observação da Resposta', 'Situação do Pedido');

    if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))){
        $cabecalho[] = 'Administrativo (dias)';
        $cabecalho[] = 'Executivas (dias)';
        $cabecalho[] = 'Técnica (dias)';
        $cabecalho[] = 'Município (dias)';
        $cabecalho[] = 'Total (dias)';
    }

    $db->sql_to_xml_excel($sql, 'relatorioListaObjetosObras', $cabecalho, '');
} else if ($_POST['req'] == 'estatistica') {
    $cabecalho = array('Ação', 'ID Solicitação', 'ID Obra', 'Administrativo (dias)', 'Executivas (dias)', 'Técnica (dias)', 'Município (dias)', 'Total (dias)') ;
    $sql = "
        SELECT
            f.acao,
            f.sldid,
            f.obrid,
            COALESCE(SUM(administrativo), 0) as administrativo,
            COALESCE(SUM(executivas), 0) as executivas,
            COALESCE(SUM(tecnica), 0) as tecnica,
            COALESCE(SUM(municipio), 0) as municipio,
            COALESCE(SUM(total), 0) as total
        FROM ( $sql ) as f
        GROUP BY f.sldid, f.obrid, f.acao
        ORDER BY 2
    ";
    $db->monta_lista($sql, $cabecalho, 100, 5, array('', '', '', 'S', 'S', 'S', 'S', 'S'), 'center', 'N', "formulario");
}else {
    $cabecalho = array('Ação', 'ID Solicitação', 'ID Obra', 'Obra', 'Nº Processo', 'Nº Termo/Convênio', 'Ano Termo/Convênio', 'Situação', '% Execução', '% Pago', '% Solicitado', '% Aprovado', '% Execução Validado', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Data de Cadastro',
        'Situação do Deferimento', 'Última tramitação', 'Data da Resposta', 'Observação da Resposta', 'Situação do Pedido');

    if(!possui_perfil( array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO) )){
        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', null, "formulario");
    } else if (!empty($_POST)){
        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', null, "formulario");
    }
}


?>
<script type="text/javascript">

$(document).ready(function (){
	$('.pesquisar').click(function (){
		$('#req').val('');
		$('#formListaObraSolicitacaoDesembolso').submit();
	});

    $('#button_limpar').click(function() {
        $('#req').val('limpar');
        $('#formListaObraSolicitacaoDesembolso').submit();
    });
});

function excluirSolicitacao(sldid) {
    if (confirm('Deseja apagar esta solicitação?')) {
        $('#sldid').val(sldid);
        $('#req').val('apagar');
        $('#formListaObraSolicitacaoDesembolso').submit();
    }
}

function alterarObr(obrid) {
    location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function abreSolicitacao(sldid){
	windowOpen('?modulo=principal/popupSolicitarDesembolso&acao=A&sldid=' + sldid,'telaSolicitacaoDesembolso','height=700,width=1200,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

$('.btnEexcel').click(function () {
    $('#req').val('excel');
    $('#formListaObraSolicitacaoDesembolso').submit();
});

$('.btnEstatistica').click(function () {
    $('#req').val('estatistica');
    $('#formListaObraSolicitacaoDesembolso').submit();
});

function carregarMunicipio( estuf ){
	var td	= $('#td_municipio');
	if ( estuf != '' ){
		var url = location.href;
		$.ajax({
			  url  		 : url,
			  type 		 : 'post',
			  data 		 : {ajax  : 'municipio',
			  		  	    estuf : estuf},
			  dataType   : "html",
			  async		 : false,
			  beforeSend : function (){
			  	divCarregando();
				td.find('select option:first').attr('selected', true);
			  },
			  error 	 : function (){
			  	divCarregado();
			  },
			  success	 : function ( data ){
			  	td.html( data );
			  	divCarregado();
			  }
		});
	}else{
		td.find('select option:first').attr('selected', true);
		td.find('select').attr('selected', true)
						 .attr('disabled', true);
	}
}



function carregarMunicipio(estuf) {

    var td = $('.td_municipio');
    if (estuf != '') {
        var url = location.href;
        $.ajax({
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
    $1_11(document).ready(function () {
        $1_11('select[name="tobid[]"]').chosen();
        $1_11('select[name="prfid[]"]').chosen();
        $1_11('select[name="tooid[]"]').chosen();
        $1_11('select[name="estuf[]"]').chosen();
        $1_11('select[name="muncod[]"]').chosen();
        $1_11('select[name="esdid[]"]').chosen();
        $1_11('select[name="tpoid[]"]').chosen();
        $1_11('select[name="esdidsituaco[]"]').chosen();
        $1_11('select[name="sitpedido[]"]').chosen();

        $1_11(".estados").chosen().change(function (e, params) {
            values = $1_11(".estados").chosen().val();
            carregarMunicipio(values);
        });
    });
</script>

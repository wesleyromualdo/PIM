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
    unset($_SESSION['obras2']['solicitacao']['filtros']);

    echo "<script>window.location.href = window.location.href;</script>";
    exit();
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

$arAba = getArAba('listaorgaodesbloqueio');
echo montarAbasArray($arAba, "?modulo=principal/listaObrasDesbloqueio&acao=A&orgid=" . $orgid);

monta_titulo_obras( 'Lista de Obras Solicitação', 'Filtre as Obras');

if(empty($_POST) && !empty($_SESSION['obras2']['solicitacao']['filtros'])) {
    unset($_SESSION['obras2']['solicitacao']['filtros']['req']);
    $_POST = $_SESSION['obras2']['solicitacao']['filtros'];
} else
    $_SESSION['obras2']['solicitacao']['filtros'] = $_POST;
extract( $_POST );

$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
$permissaoFiltro = false;
if (possui_perfil($pflcods)) {
    $permissaoFiltro = true;
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<form method="post" name="formListaObraDesbloqueio" id="formListaObraDesbloqueio">
	<input type="hidden" name="req" id="req" value="">
	<input type="hidden" name="obrid" id="obrid" value="">
	<input type="hidden" name="empid" id="empid" value="">

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
                <input type="text" name="slcid" size="71" maxlength="100" value="<?=$slcid?>" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="sldid" title="" class=" normal">
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação da Obra:
            </div>
            <div class="col-lg-10">
                <select name="esdid[]" id="esdid" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
                    $dados = $db->carregar($sql);
                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($esdid && in_array($key['codigo'], $esdid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Tipologia:
            </div>
            <div class="col-lg-10">
                <select name="tpoid[]" id="tpoid" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php   $tipologiaObra = new TipologiaObra();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    $dados = $tipologiaObra->listaCombo($param, false);
                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($tpoid && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Programa:
            </div>
            <div class="col-lg-10">
                <select name="prfid[]" id="prfid" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($prfid && in_array($key['codigo'], $prfid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Fonte:
            </div>
            <div class="col-lg-10">
                <select name="tooid[]" id="tooid" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($tooid && in_array($key['codigo'], $tooid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Municípios:
            </div>
            <div class="col-lg-10">
                <select name="muncod[]" id="muncod" class="chosen-select municipios" multiple data-placeholder="Selecione">
                    <?php   $municipio = new Municipio();
                    foreach ($municipio->listaComboMulti($estuf) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($muncod && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
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
                $dado = $db->carregar('SELECT esdid codigo, esddsc descricao FROM workflow.estadodocumento WHERE tpdid = '.TPDID_SOLICITACOES.' ');
                ?>
                <select name="esdidsituaco[]" id="esdidsituaco" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($esdidsituaco && in_array($key['codigo'], $esdidsituaco)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
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
                echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, 'id="convenio"');
                ?>
                 Ano: 
                <?php
                echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, 'id="ano_convenio"');
                ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                % Execução:
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
                Tipo de Solicitação:
            </div>
            <div class="col-lg-10">
                <?php
                $dado = $db->carregar('SELECT tslid as codigo, tslnome as descricao FROM obras2.tiposolicitacao WHERE tslstatus = \'A\' ');
                ?>
                <select name="tslid[]" id="tslid" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($tslid && in_array($key['codigo'], $tslid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <? if($permissaoFiltro): ?>
        <div class="row">
            <div class="col-lg-2">
                Estado:
            </div>
            <div class="col-lg-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" name="solicitacao" value="S" <?= ($solicitacao == "S" ? "checked" : "") ?>> Aprovado
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="solicitacao" value="N" <?= ($solicitacao == "N" ? "checked" : "") ?>> Não Aprovado
                    </label>
                    <label class="btn btn-default <?= (!isset($solicitacao) ? "active" : "") ?>">
                        <input type="radio" name="solicitacao" value="" <?= (!isset($solicitacao) ? "checked" : "") ?>> Todas
                    </label>
                </div>
            </div>
        </div>
        <? endif; ?>
    </div>

    <div class="row form-filters text-center">
        <div class="col">
            <button type="button" name="pesquisar" class="btn btn-success pesquisar" onclick="validaFiltro();">
                <span class="glyphicon glyphicon-search"></span> Pesquisar
            </button>  
            <button type="button" name="btnEexcel" class="btn btn-primary btnEexcel">
                <span class="glyphicon glyphicon-download-alt"></span> Gerar Excel
            </button>
            <button type="button" id="button_limpar" class="btn btn-danger">
                <span class="glyphicon glyphicon-remove"></span> Limpar Filtros
            </button>
        </div>
    </div>
</form>
<?php
$where = array("obrstatus = 'A'");
$where[] = "emp.orgid = " . $orgid;
if ( $obrbuscatexto ){
    $obrbuscatexto = removeAcentos($obrbuscatexto);
    $obrid = limpaObridSec(trim($obrbuscatexto));
    $where[] = " ( UPPER(public.removeacento(o.obrnome)) ILIKE ('%" . $obrbuscatexto . "%') OR
				   public.removeacento(o.obrid::CHARACTER VARYING) ILIKE ('%" . $obrid . "%')OR
                        o.entidsecretaria::CHARACTER VARYING ILIKE ('%" . $obrid . "%') ) ";
}



if( $slcid  && is_numeric($slcid)){
	$where[] = "p.slcid = " .$slcid;
}

if($esdid) {
    $esdid = (array) $esdid;
    $where[] = "dc.esdid IN('" . implode("', '", $esdid) . "')";
}

if($tpoid) {
    $tpoid = (array) $tpoid;
    $where[] = "o.tpoid IN('" . implode("', '", $tpoid) . "')";
}

if($prfid) {
    $prfid = (array) $prfid;
    $where[] = "emp.prfid IN('" . implode("', '", $prfid) . "')";
}

if($tooid) {
    $tooid = (array) $tooid;
    $where[] = "emp.tooid IN('" . implode("', '", $tooid) . "')";
}

if( $empesfera ){
	$empesfera = (array) $empesfera;
	$where[] = "emp.empesfera IN('" . implode("', '", $empesfera) . "')";
}

//if( $estuf ){
//	$estuf = (array) $estuf;
	$where[] = "ende.estuf IN('SP')";
//}

if( $muncod ){
	$muncod = (array) $muncod;
	$where[] = "ende.muncod IN('" . implode("', '", $muncod) . "')";
}

if($esdidsituaco){
	$esdidsituaco = (array) $esdidsituaco;
    $where[] = "p.esdid IN('" . implode("', '", $esdidsituaco) . "')";
}

if ($convenio) {
    $where[] = "p_conv.termo_convenio = '".$convenio."'";
}

if ($ano_convenio) {
    $where[] = "p_conv.ano_termo_convenio = '".$ano_convenio."'";
}

if ($percentualinicial != '') {
    $where[] = "COALESCE(o.obrpercentultvistoria, 0) >= " . $percentualinicial;
}

if ($percentualfinal != '') {
    if ($percentualfinal < 100) {
        $where[] = "COALESCE(o.obrpercentultvistoria, 0) <= " . $percentualfinal;
    }
}

if($tslid) {
    $tslid = (array) $tslid;
    $where[] = "p.slcid IN (SELECT slcid FROM obras2.tiposolicitacao_solicitacao WHERE tslid IN (".implode(", ", $tslid).")) ";
}

if($solicitacao){
    $where[] = "p.aprovado = '$solicitacao'";
    if($solicitacao == 'N'){
        $where[] = "p.esdid IN(".ESDID_SOLICITACOES_DEFERIDO.",".ESDID_SOLICITACOES_INDEFERIDO.",".ESDID_SOLICITACOES_DILIGENCIA.")";
    }
}

$cabecalho = array('Ação', 'ID Solicitação', 'ID Obra', 'Obra', 'UF', 'Município', "Vigência Termo/Convênio","Tipo Solicitação", 'Justificativa', 'Inserido Por', 'Data de Cadastro',
				   'Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Observação', 'Estado');
if( ($tslid == 13 && $_SESISON['usucpf'] == '') || $tslid != 13 ){
    $aprovado = ", CASE WHEN p.aprovado = 'S' THEN '<b style=\"color:green;\">APROVADO</b>' WHEN p.reprovado = 'S' THEN '<b style=\"color:red;\">REPROVADO</b>' WHEN (p.aprovado = 'N' AND p.esdid IN(".ESDID_SOLICITACOES_DEFERIDO.",".ESDID_SOLICITACOES_INDEFERIDO.")) THEN '
    <button type=\"button\" onclick=\"aprovaSolicitacao(this,'||p.slcid||')\">APROVAR</button>
    <button type=\"button\" style=\"background-color:#EF3939;border-color:#F00;margin-top:10px;\" onclick=\"reprovaSolicitacao(this,'||p.slcid||')\">REPROVAR</button>' ELSE '' END AS aprovado";
}else{
    $aprovado = ", '' AS aprovado";
}
$situacao = "p.esddsc";

if ($_POST['req'] == 'excel') {
    $cabecalho = array('ID Solicitação', 'ID Obra', 'Obra', 'UF', 'Município', "Vigência Termo/Convênio", "Tipo Solicitação", 'Justificativa', 'Inserido Por', 'Data de Cadastro',
        'Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Observação', 'Estado');
    $aprovado = ", CASE WHEN p.aprovado = 'S' THEN 'APROVADO' WHEN p.reprovado = 'S' THEN 'REPROVADO' WHEN (p.aprovado = 'N' AND p.esdid IN(".ESDID_SOLICITACOES_DEFERIDO.",".ESDID_SOLICITACOES_INDEFERIDO.")) THEN '' ELSE '' END AS aprovado";
}

if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
    $cabecalho = array('Ação', 'ID Solicitação', 'ID Obra', 'Obra', 'UF', 'Município',"Vigência Termo/Convênio", "Tipo Solicitação", 'Justificativa', 'Inserido Por', 'Data de Cadastro','Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Observação');
    $aprovado = "";
    $situacao = "CASE WHEN p.aprovado = 'N' THEN (CASE WHEN p.esdid = ".ESDID_SOLICITACOES_CADASTRAMENTO." THEN '<b>Em Cadastramento</b>' WHEN p.esdid = ".ESDID_SOLICITACOES_DILIGENCIA." THEN '<b>Diligência</b>' WHEN p.esdid = ".ESDID_SOLICITACOES_CANCELADO." THEN '<b>Cancelado</b>' ELSE '<b>Aguardando Análise</b>' END) ELSE '<b style=\"color:green;\">'||p.esddsc||'</b>' END AS esddsc";
    if ($_POST['req'] == 'excel') {
        $cabecalho = array('ID Solicitação', 'ID Obra', 'Obra', 'UF', 'Município',"Vigência Termo/Convênio", "Tipo Solicitação", 'Justificativa', 'Inserido Por', 'Data de Cadastro','Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Observação');
        $situacao = "CASE WHEN p.aprovado = 'N' THEN (CASE WHEN p.esdid = ".ESDID_SOLICITACOES_CADASTRAMENTO." THEN 'Em Cadastramento' WHEN p.esdid = ".ESDID_SOLICITACOES_CANCELADO." THEN 'Cancelado' WHEN p.esdid = ".ESDID_SOLICITACOES_DILIGENCIA." THEN 'Diligência' ELSE 'Aguardando Análise' END) ELSE p.esddsc END AS esddsc";
    }


    $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

    $resp = array();
    $arPflcod = array();
    $orWhere = array();
    if (possui_perfil($arrUni)) {
        $arPflcod = array_merge($arPflcod, $arrUni);
        $orWhere['entidunidade'] = "emp.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = emp.entidunidade)";
    }

    if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE))) {
        $usuarioResp = new UsuarioResponsabilidade();
        $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
        $arEmpid = ($arEmpid ? $arEmpid : array(0));

        $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
        $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

        $orWhere['sup'] = "emp.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
    }

}

if(isset($_POST['req']) && !empty($_POST['req'])) {

		if(!is_array($orWhere))
			$orWhere = array();

    //string que monta a concatenação do campo obrid + idsecretaria para ser montada na query
    $campoObridSec = montaCampoObridSec('o');

    $sql = "
        WITH tmp_obras2_solicitacao AS (
                    SELECT
                        slcid,
                        obrid,
                        slcjustificativa,
                        u.usunome,
                        slcdatainclusao,
                        ed.esddsc,
                        ed.esdid,
                        p.aprovado,
                        cd.cmddsc,
                        p.reprovado,
                        u2.usunome AS usunomedesbloqueio,
                        htd.htddata
                    FROM obras2.solicitacao p
                    LEFT JOIN workflow.documento d ON d.docid = p.docid
                    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                    LEFT JOIN workflow.comentariodocumento cd ON (d.hstid = cd.hstid AND cd.cmdstatus = 'A')
                    LEFT JOIN workflow.historicodocumento htd ON (d.hstid = htd.hstid)
                    LEFT JOIN seguranca.usuario u2 ON (htd.usucpf = u2.usucpf)
                    JOIN seguranca.usuario u ON u.usucpf = p.usucpf
                    WHERE slcstatus = 'A'
            )";
    if ($_POST['req'] == 'excel') {
        $columns = "
            p.slcid,
	 		{$campoObridSec} as obrid,
			'(' || {$campoObridSec} || ') ' || o.obrnome as dslcricao,
			ende.estuf,
			mun.mundescricao,
			CASE
                    WHEN TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text IS NULL AND o.tooid = 2 THEN TO_CHAR(dco.dcodatafim,'DD/MM/YYYY')::text
                    ELSE TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text
            END as dt_fim_vigencia_termo,
			--'teste',
			(SELECT array_to_string(array_agg(tslnome), ',') FROM obras2.tiposolicitacao_solicitacao ts
                JOIN obras2.tiposolicitacao t ON t.tslid = ts.tslid
                WHERE ts.slcid = p.slcid),
			p.slcjustificativa,
			p.usunome AS usunomepedido,
			TO_CHAR(p.slcdatainclusao, 'dd/mm/YYYY') AS slcdatainclusao,
			$situacao,
			p.usunomedesbloqueio,
			TO_CHAR(p.htddata, 'dd/mm/YYYY') AS dsldata,
			p.cmddsc as observacao
            $aprovado";
    } else {
        $columns = "'
            <center>
                <div style=\"width:100%;\">
                    <img align=\"absmiddle\" src=\"/imagens/icone_lupa.png\" style=\"cursor: pointer\" onclick=\"javascript: desbloqueioObr(' || o.obrid || ',' || p.slcid  || ');\"
                        title=\"Visualizar Solicitação\">
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: acessaSolicitacao(' || o.obrid || ');\"
                        title=\"Acessar Solicitações\">
                </div>
            </center>' AS acao,
            p.slcid,
	 		{$campoObridSec} as obrid,
			'<a href=\"javascript: alterarObr(' || o.obrid || ');\">(' || {$campoObridSec} || ') ' || o.obrnome || '</a>' as dslcricao,
			ende.estuf,
			mun.mundescricao,
			CASE
                    WHEN TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text IS NULL AND o.tooid = 2 THEN TO_CHAR(dco.dcodatafim,'DD/MM/YYYY')::text
                    ELSE TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text
            END as dt_fim_vigencia_termo,
			(SELECT array_to_string(array_agg(tslnome), ',') FROM obras2.tiposolicitacao_solicitacao ts
                JOIN obras2.tiposolicitacao t ON t.tslid = ts.tslid
                WHERE ts.slcid = p.slcid),
			p.slcjustificativa,
			p.usunome AS usunomepedido,
			TO_CHAR(p.slcdatainclusao, 'dd/mm/YYYY') AS slcdatainclusao,
			$situacao,
			p.usunomedesbloqueio,
			TO_CHAR(p.htddata, 'dd/mm/YYYY') AS dsldata,
			p.cmddsc as observacao
            $aprovado";

    }
    $sql .= "
        SELECT DISTINCT ON (p.slcid)
            $columns
		FROM obras2.obras o
        LEFT JOIN (SELECT * FROM obras2.vm_termo_convenio_obras WHERE termo_convenio IS NOT NULL) as p_conv ON p_conv.obrid = o.obrid
        LEFT JOIN workflow.documento dc ON (o.docid = dc.docid)
		INNER JOIN obras2.empreendimento emp on emp.empid = o.empid
		LEFT JOIN entidade.endereco ende ON ende.endid = o.endid
            AND ende.endstatus = 'A'
            AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
		LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod
		JOIN tmp_obras2_solicitacao p ON p.obrid = o.obrid

		LEFT JOIN painel.dadosconvenios dco on dco.dcoprocesso = Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')
		LEFT JOIN obras2.vm_vigencia_obra_2016  ter ON ter.obrid = o.obrid

		WHERE
            " . (count($where) ? implode(' AND ', $where) : "") . "
		    " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
		";

    $tamanho = Array('4%', '', '', '', '', '', '', '', '', '', '');
    if ($_POST['req'] == 'excel') {
        $db->sql_to_xml_excel($sql, 'relatorioListaObraSolicitacao', $cabecalho, '');
    } else {
        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', $par2, "formulario", $tamanho, '', '', array('ordena' => false));
    }

		//echo("<pre>");
		//echo("$sql");
		//echo("</pre>");
}
?>
<script type="text/javascript">


    function validaFiltro(){
        var obrbuscatexto = document.getElementById('obrbuscatexto').value;
        var sldid = document.getElementById('sldid').value;
        var esdid = document.getElementById('esdid').value;
        var tpoid = document.getElementById('tpoid').value;
        var prfid = document.getElementById('prfid').value;
        var tooid = document.getElementById('tooid').value;
        // var empesfera = document.getElementById('empesfera').value;
        // var estuf = document.getElementById('estuf').value;
        var esdidsituaco = document.getElementById('esdidsituaco').value;
        var convenio = document.getElementById('convenio').value;
        var ano_convenio = document.getElementById('ano_convenio').value;

        // var filtros = [obrbuscatexto,sldid,esdid,tpoid,prfid,tooid,estuf,empesfera,esdidsituaco,convenio,ano_convenio];
        var filtros = [obrbuscatexto,sldid,esdid,tpoid,prfid,tooid,esdidsituaco,convenio,ano_convenio];

        var filtrosQuatidade = filtros.length;

        var ocorrenciasFiltro = 0;
        for(i=0;i<filtrosQuatidade;i++ ){

            if(filtros[i] != ''){
                ocorrenciasFiltro ++;
            }
        }
        if(ocorrenciasFiltro > 0){
            $('#req').val('pesquisar');
            document.formListaObraDesbloqueio.submit();
        }else{
            alert('Para realizar a pesquisa deve ser preenchido ao menos um filtro.');
            return false;
        }

    }
$(document).ready(function (){


	$( "#sldid" ).keyup(function() {
		  //alert( "Handler for .keyup() called." );
		  var er = /[^0-9.]/;
          er.lastIndex = 0;
          var campo = this;
          if (er.test(campo.value)) {
          alert('O campo ID da Solicitação deve ser númerico ');
          campo.value = "";
        }
		});

	 function somenteNumeros(num) {
	        var er = /[^0-9.]/;
	        er.lastIndex = 0;
	        var campo = num;
	        if (er.test(campo.value)) {
	          campo.value = "";
	        }
	    }

    $('#button_limpar').click(function() {
        $('#req').val('limpar');
        $('#formListaObraDesbloqueio').submit();
    });
    $('.btnEexcel').click(function () {
        $('#req').val('excel');
        $('#formListaObraDesbloqueio').submit();
    });
});

$1_11(document).ready(function () {
    $1_11('select[name="prfid[]"]').chosen();
    $1_11('select[name="tooid[]"]').chosen();
    $1_11('select[name="estuf[]"]').chosen();
    $1_11('select[name="muncod[]"]').chosen();
    $1_11('select[name="esdid[]"]').chosen();
    $1_11('select[name="tpoid[]"]').chosen();
    $1_11('select[name="esdidsituaco[]"]').chosen();
    $1_11('select[name="tslid[]"]').chosen();

    $1_11(".estados").chosen().change(function (e, params) {
        values = $1_11(".estados").chosen().val();
        carregarMunicipio(values);
    });
});

function desbloqueioObr(obrid, slcid){
	windowOpen('?modulo=principal/solicitacao&acao=A&obrid=' + obrid + '&slcid=' + slcid,'telaDesbloqueio','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function acessaSolicitacao(obrid){
	location.href = '?modulo=principal/solicitacoes&acao=A&obrid=' + obrid;
}

function alterarObr( obrid ){
	location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function aprovaSolicitacao(objeto,slcid){
    if(confirm('Você deseja prosseguir com a aprovação?')){
        jQuery.ajax({
            type: "POST",
            url: 'obras2.php?modulo=principal/solicitacoes&acao=A',
            data: "requisicao=aprova_solicitacao&slcid=" + slcid,
            async: false,
            dataType : 'json',
            success: function(res) {
                alert(res.mensagem);
                if(res.retorno){
                    $(objeto).parent().html('<b style="color:green;">APROVADO</b>');
                }

            }
        });
    }
}

function reprovaSolicitacao(objeto,slcid){
    if(confirm('Você deseja prosseguir com a reprovação?')){
        jQuery.ajax({
            type: "POST",
            url: 'obras2.php?modulo=principal/solicitacoes&acao=A',
            data: "requisicao=reprova_solicitacao&slcid=" + slcid,
            async: false,
            dataType : 'json',
            success: function(res) {
                alert(res.mensagem);
                location.reload();
            }
        });
    }
}

function carregarMunicipio( estuf ){
	var td	= $('#td_municipio');
	if ( estuf != '' ){
		var url = location.href;
		$.ajax({
            url  		 : url,
            type 		 : 'post',
            data 		 : {ajax  : 'municipio', estuf : estuf},
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
		td.find('select').attr('selected', true).attr('disabled', true);
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

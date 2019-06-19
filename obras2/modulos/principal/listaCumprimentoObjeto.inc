<?php
$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if (!in_array($_SESSION['obras2']['orgid'], $arOrgid)) {
    $_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current($arOrgid));
$orgid = $_SESSION['obras2']['orgid'];

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

#Requisicao Limpar
if ($_POST['req'] == 'limpar') {
    unset($_SESSION['obras2']['solicitacao']['filtros']);

    echo "<script>window.location.href = window.location.href;</script>";
    exit();
}

//tratando filtro de leitura da notificação de cumprimento de objeto
if ($_REQUEST['leitura'] == 'sim') {
    //traz apenas as notificações lidas
    $lido = " AND leitor.usunome IS NOT NUll ";
} elseif ($_REQUEST['leitura'] == 'nao') {
    //traz apenas as notificações não lidas
    $lido = " AND leitor.usunome IS NUll ";
} else {
    //qualquer outro valor (incluindo nulo) não adiciona condição, entao traz todos
    $lido = '';
}




if ($_POST['req'] == 'liberado_excel') {
    $cabecalho = array('ID Cumprimento Objeto',
        'ID',
        'Obra',
        'UF',
        'Município',
        'Situação da Obra',
        'Programa',
        'Fonte',
        'Tipologia',
        'Numero do Processo',
        'Obras do Processo',
        'Situação do Cumprimento do Objeto',
        'Data de Liberação',
        'Leitura Notificação de Cumprimento',
        'Data Leitura',
        'Lida por');
    $sql = "

        WITH alerta_obra AS (SELECT max(aoid) AS aoid, obrid, usucpf, dtacao
                       FROM obras2.alerta_obra
                         INNER JOIN (SELECT max(aolid),
                                            aoid,
                                            (SELECT usucpf
                                               FROM obras2.alerta_obra_lido aol2
                                               WHERE aol2.aolid = MAX(aol.aolid)) as usucpf,
                                            (SELECT dtacao
                                               FROM obras2.alerta_obra_lido aol2
                                               WHERE aol2.aolid = MAX(aol.aolid)) as dtacao
                                       FROM obras2.alerta_obra_lido aol
                                       WHERE aid = 1
                                         AND acao = 'L'
                                      GROUP BY aoid) ultimo_alerta USING(aoid)
                       WHERE aid = 1
                       GROUP BY obrid, usucpf, dtacao
                       limit 10

                       )

        SELECT
          co.coid AS id_cumprimento_objeto,
          obr.obrid AS obrid,
          obr.obrnome AS nome_obra,
          mun.estuf AS estado_da_obra,
          mun.mundescricao AS municipio,
          esd.esddsc AS situacao_obra,
          pf.prfdesc programa,
          too.toodescricao fonte,
          tpo.tpodsc,
          p_conv.pronumeroprocesso AS numero_processo,
          COUNT( DISTINCT v_processo.obrid),
          --esd2.esddsc AS situacao_cumprimento_objeto,
          CASE
              WHEN esd2.esddsc  IS NULL THEN 'Em Cadastramento'
              ELSE esd2.esddsc
           END   situacao_cumprimento_objeto,
          TO_CHAR(cco.dtcriacao,'DD/MM/YYYY') AS data_liberacao,
          CASE
                WHEN leitor.usunome IS NOT NUll THEN 'Sim'
                ELSE 'Não'
            END AS lido,
          TO_CHAR(alerta_obra.dtacao, 'DD/MM/YYYY') dt_lido,
          leitor.usunome
        FROM obras2.controle_cumprimento_objeto cco
        LEFT JOIN obras2.cumprimento_objeto co ON (cco.obrid = co.obrid)
        LEFT JOIN obras2.obras obr ON (cco.obrid = obr.obrid)
        JOIN obras2.empreendimento emp ON emp.empid = obr.empid
        LEFT JOIN entidade.endereco edr ON edr.endid = obr.endid
        LEFT JOIN territorios.municipio mun ON mun.muncod = edr.muncod
        LEFT JOIN workflow.documento doc ON (obr.docid = doc.docid)
        LEFT JOIN workflow.estadodocumento esd ON (doc.esdid = esd.esdid)
        LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = obr.obrid
        LEFT JOIN workflow.documento doc2 ON (co.docid = doc2.docid)
        LEFT JOIN workflow.estadodocumento esd2 ON (doc2.esdid = esd2.esdid)
        LEFT JOIN obras2.programafonte pf                    ON pf.prfid = emp.prfid
        LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = obr.tpoid
        LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = obr.tooid
        LEFT JOIN obras2.vm_termo_convenio_obras AS v_processo ON (p_conv.pronumeroprocesso = v_processo.pronumeroprocesso)
        LEFT JOIN alerta_obra ON (alerta_obra.obrid = obr.obrid)
        LEFT JOIN seguranca.usuario AS leitor ON (alerta_obra.usucpf = leitor.usucpf )
        WHERE cco.ccostatus = 'A' {$lido}
        GROUP BY cco.obrid, co.coid, obr.obrid, obr.obrnome, mun.estuf, mun.mundescricao, esd.esddsc, pf.prfdesc,
         too.toodescricao, tpo.tpodsc, esd2.esddsc, cco.dtcriacao, p_conv.pronumeroprocesso, leitor.usunome,
         alerta_obra.dtacao
        ORDER BY cco.obrid limit 10
    ";
    global $db;

    $db->sql_to_xml_excel($sql, 'relatorioListaCumprimentoObjetoLiberado', $cabecalho, '');
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
            <?php $municipio = new Municipio();
            foreach ($municipio->listaComboMulti($estuf) as $key) {
                ?>
                <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?><?php echo "selected='selected'"; ?><?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php
        exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";


$arAba = getArAba('listaorgaodesbloqueio');
echo montarAbasArray($arAba, "?modulo=principal/listaObrasDesbloqueio&acao=A&orgid=" . $orgid);

monta_titulo_obras('Lista de Cumprimento do Objeto', '');
if (empty($_POST) && !empty($_SESSION['obras2']['solicitacao']['filtros'])) {
    unset($_SESSION['obras2']['solicitacao']['filtros']['req']);
    $_POST = $_SESSION['obras2']['solicitacao']['filtros'];
} else
    $_SESSION['obras2']['solicitacao']['filtros'] = $_POST;
extract($_POST);

$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
$permissaoFiltro = false;
if (possui_perfil($pflcods)) {
    $permissaoFiltro = true;
}

$display = "style='display: none'";
if ($_POST['novaPactuacao'] == 14 && $_POST['novaPactuacao'] != "" ){
    $display = "style='display: show'";
}else{

    unset($_POST['esdidsituaco']);
    unset($_REQUEST['esdidsituaco']);
}


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<form method="post" name="formListaObraCumprimento" id="formListaObraCumprimento">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="obrid" id="obrid" value="">
    <input type="hidden" name="empid" id="empid" value="">

    <div class="form-filters">
        <div class="row">
            <div class="col-lg-2">
                Nome da Obra / ID:
            </div>
            <div class="col-lg-10">
                <?= campo_texto('obrbuscatexto', 'N', 'S', '', 70, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação:
            </div>
            <div class="col-lg-10">
                <select name="strid[]" id="strid" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php $situacaoRegistro = new SituacaoRegistro();
                    foreach ($situacaoRegistro->listaCombo() as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($strid) && in_array($key['codigo'], $strid)) { ?><?php echo "selected='selected'"; ?><?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Situação do Cumprimento:
            </div>
            <div class="col-lg-10">
                <select name="stcoid[]" id="stcoid" class="chosen-select" multiple data-placeholder="Selecione" >
                    <?php
                    $sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 200 and esdstatus = 'A' order by esdordem";
                    $dados = $db->carregar($sql);

                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($stcoid) && in_array($key['codigo'], $stcoid)) { ?><?php echo "selected='selected'"; ?><?php } ?>><?php echo $key["descricao"] ?></option>
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
                    <?php $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($prfid) && in_array($key['codigo'], $prfid)) { ?><?php echo "selected='selected'"; ?><?php } ?>><?php echo $key["descricao"] ?></option>
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
                    <?php $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($tooid) && in_array($key['codigo'], $tooid)) { ?><?php echo "selected='selected'"; ?><?php } ?>><?php echo $key["descricao"] ?></option>
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
                    <?php $municipio = new Municipio();
                    foreach ($municipio->listaComboMulti($estuf) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?><?php echo "selected='selected'"; ?><?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Convênio/Termo:
            </div>
            <div class="col-lg-10">
                Número:&nbsp;
                <?php
                echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, 'id="convenio"');
                ?>
                Ano:&nbsp;
                <?php
                echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, 'id="ano_convenio"');
                ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Processo:
            </div>
            <div class="col-lg-10">
                Número:&nbsp;
                <?php
                echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, 'id="processo"');
                ?>
                Ano:&nbsp;
                <?php
                echo campo_texto('ano_processo', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, 'id="ano_processo"');
                ?>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Leitura de Notificação de Cumprimento de Objeto:
            </div>
            <div class="col-lg-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" name="leitura" value="sim" <?= ($_POST['leitura'] == "sim" ? "checked" : "") ?>> Sim
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="leitura" value="nao" <?= ($_POST['leitura'] == "nao" ? "checked" : "") ?>> Não
                    </label>
                    <label class="btn btn-default <?= ($_POST['leitura'] == "" || $_POST['leitura'] == "todos" ? "active" : "") ?>">
                        <input type="radio" name="leitura" value="todos" <?= ($_POST['leitura'] == "todos" || $_POST['leitura'] == "" ? "checked" : "") ?>> Todos
                    </label>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-2">
                Nova Pactuação:
            </div>
            <div class="col-lg-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" name="novaPactuacao" value="14" <?= ($_POST['novaPactuacao'] == "14" ? "checked" : "") ?>> Sim
                    </label>
                    <label class="btn btn-default">
                        <input type="radio" name="novaPactuacao" value="1" <?= ($_POST['novaPactuacao'] == "1" ? "checked" : "") ?>> Não
                    </label>
                    <label class="btn btn-default <?= ($_POST['novaPactuacao'] == "" || $_POST['novaPactuacao'] == "" ? "active" : "") ?>">
                        <input type="radio" name="novaPactuacao" value="" <?= ($_POST['novaPactuacao'] == "" ? "checked" : "") ?>> Todos
                    </label>
                </div>
            </div>
        </div>
        <div class="row" <?= $display ?>>
            <div class="col-lg-2">
                Situação da nova pactuação:
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
    </div>

    <div class="row form-filters text-center">
        <div class="col">
            <button type="button" name="pesquisar" id="pesquisar" class="btn btn-success pesquisar" onclick="validaFiltro(this);">
                <span class="glyphicon glyphicon-search"></span> Pesquisar
            </button>
            <button type="button" name="btnEexcel" id="btnEexcel" class="btn btn-primary btnEexcel" onclick="validaFiltro(this);">
                <span class="glyphicon glyphicon-download-alt"></span> Gerar Excel
            </button>
            <button type="button" id="button_limpar" class="btn btn-danger">
                <span class="glyphicon glyphicon-remove"></span> Limpar Filtros
            </button>
            <?
            if ($permissaoFiltro): ?>
                <button type="button" id="button_liberado" class="btn btn-info">
                    <span class="glyphicon glyphicon-ok"></span> Cumprimento do Objeto Liberado
                </button>
            <? endif; ?>
        </div>
    </div>
</form>
<?php
$coluns = array();
$join = array();
$where = array();
if ($_REQUEST['obrbuscatexto']) {
    $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($_REQUEST['obrbuscatexto']))));
    $obrbuscatextoTemp = trim($obrbuscatextoTemp);

    if (!strpos($obrbuscatextoTemp, ',')) {
        $_REQUEST['obrbuscatexto'] = limpaObridSec(trim($_REQUEST['obrbuscatexto']));
        $where['obrnome'] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        obr.obrid::CHARACTER VARYING ILIKE ('%" . $_REQUEST['obrbuscatexto'] . "%') OR
                        obr.entidsecretaria::CHARACTER VARYING ILIKE ('%" . $param['obrbuscatexto'] . "%') ) ";
    } else {
        $campos = explode(',', $obrbuscatextoTemp);
        $w = array();
        foreach ($campos as $c) {
            $c = trim($c);
            $d = limpaObridSec($c);
            $w[] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        obr.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') OR
                        obr.entidsecretaria::CHARACTER VARYING ILIKE ('%" . $d . "%')) ";
        }

        $w = '(' . implode('OR', $w) . ')';
        $where['obrnome'] = $w;
    }

}

if ($_REQUEST['strid'] && $_REQUEST['strid'][0] != '')
    $where['strid'] = "srd.strid IN(" . implode(',', $_REQUEST['strid']) . ")";
if ($_REQUEST['stcoid'] && $_REQUEST['stcoid'][0] != '')
    $where['stcoid'] = "doc.esdid IN(" . implode(',', $_REQUEST['stcoid']) . ")";
if ($_REQUEST['prfid'] && $_REQUEST['prfid'][0] != '')
    $where['prfid'] = "emp.prfid IN(" . implode(',', $_REQUEST['prfid']) . ")";
if ($_REQUEST['tooid'] && $_REQUEST['tooid'][0] != '')
    $where['tooid'] = "obr.tooid IN(" . implode(',', $_REQUEST['tooid']) . ")";
//if ($_REQUEST['estuf'] && $_REQUEST['estuf'][0] != '')
    $where['estuf'] = "mun.estuf IN ('SP')";
if ($_REQUEST['muncod'] && $_REQUEST['muncod'][0] != '')
    $where['muncod'] = "mun.muncod IN ('" . implode("', '", $_REQUEST['muncod']) . "')";
//if ($_REQUEST['empesfera'])
    $where['empesfera'] = "emp.empesfera IN('M')";
if ($_REQUEST['convenio'])
    $where['convenio'] = "p_conv.termo_convenio = '{$_REQUEST['convenio']}'";
if ($_REQUEST['ano_convenio'])
    $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$_REQUEST['ano_convenio']}'";
if ($_REQUEST['processo'])
    $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$_REQUEST['processo']}','.',''),'/',''),'-','')";
if ($_REQUEST['ano_processo'])
    $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$_REQUEST['ano_processo']}'";
if ($_POST['esdidsituaco'] && $_POST['esdidsituaco'][0] != '' && $_POST['novaPactuacao'] == 14)
    $where['esdidsituaco'] = " p.esdid  IN ('" . implode("', '", $_POST['esdidsituaco']) . "')";

if ($_POST['novaPactuacao'] == 1){
    $where['novaPactuacao'] = " obr.obrid NOT IN (SELECT p.obrid FROM tmp_obras2_solicitacao p )";
}


if (isset($_POST['req']) && !empty($_POST['req'])) {

    $arrFiltros = array(
        $obrbuscatexto = $_POST['obrbuscatexto'],
        $strid = $_POST['strid'],
        $stcoid = $_POST['stcoid'],
        $prfid = $_POST['prfid'],
        $tooid = $_POST['tooid'],
        $estuf = $_POST['estuf'],
        $empesfera = $_POST['empesfera'],
        $convenio = $_POST['convenio'],
        $ano_convenio = $_POST['ano_convenio'],
        $ano_processo = $_POST['ano_processo']
    );

    $ocorrenciasFiltro = 0;
    $filtrosQuatidade = count($arrFiltros);

    for ($i = 0; $i < $filtrosQuatidade; $i++) {

        if ($arrFiltros[$i] != '') {
            $ocorrenciasFiltro++;
        }
    }

    if ($ocorrenciasFiltro > 0) {

        if($_POST['novaPactuacao'] == 14 || $_POST['novaPactuacao'] == 1){
            if($_POST['novaPactuacao'] == 14){
                $colunaEstadoDocumento = 'Situação Nova Pactuação';
                $estadoDocumentoJoin =  "JOIN tmp_obras2_solicitacao p ON p.obrid = obr.obrid";
            }


            $colunaEstadoDocSql = " p.esddsc,";
            $colunaEstadoDocumentoWith= ",tmp_obras2_solicitacao AS (
                                                            SELECT
                                                                p.slcid,
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
                                                            LEFT JOIN obras2.tiposolicitacao_solicitacao tps ON p.slcid = tps.slcid
                                                            LEFT JOIN workflow.documento d ON d.docid = p.docid
                                                            LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                                                            LEFT JOIN workflow.comentariodocumento cd ON (d.hstid = cd.hstid AND cd.cmdstatus = 'A')
                                                            LEFT JOIN workflow.historicodocumento htd ON (d.hstid = htd.hstid)
                                                            LEFT JOIN seguranca.usuario u2 ON (htd.usucpf = u2.usucpf)
                                                            JOIN seguranca.usuario u ON u.usucpf = p.usucpf
                                                            WHERE slcstatus = 'A' AND tps.tslid = 14
                                                    )";

        }else{
            $colunaEstadoDocumento= '';
            $colunaEstadoDocSql= '';
            $colunaEstadoDocumentoWith = '';
            $estadoDocumentoJoin = '';
        }


        if(!is_array($orWhere))
          $orWhere = array();


        if ($_POST['req'] == 'excel') {

            if($_POST['novaPactuacao'] == 14){
                $colunaEstadoDocSql = " p.esddsc,";
            }else{
                $colunaEstadoDocSql = '';
            }


            $cabecalho = array('ID Cumprimento Objeto', $colunaEstadoDocumento,'ID Obra', 'Obra', 'UF',
                                'Município', "Estado da Obra", 'Numero do Processo', 'Obras do Processo', 'Situação do Cumprimento do Objeto',
                                'Data de Cadastro', 'Dt. Ult. Tramitação', 'Dt. Vencimento Termo', 'Leitura Notificação de Cumprimento', 'Data Leitura',
                                'Lida por',);

            $sql = "

        WITH alerta_obra AS (SELECT max(aoid) AS aoid, obrid, usucpf, dtacao
                       FROM obras2.alerta_obra
                         INNER JOIN (SELECT max(aolid),
                                            aoid,
                                            (SELECT usucpf
                                               FROM obras2.alerta_obra_lido aol2
                                               WHERE aol2.aolid = MAX(aol.aolid)) as usucpf,
                                            (SELECT dtacao
                                               FROM obras2.alerta_obra_lido aol2
                                               WHERE aol2.aolid = MAX(aol.aolid)) as dtacao
                                       FROM obras2.alerta_obra_lido aol
                                       WHERE aid = 1
                                         AND acao = 'L'
                                      GROUP BY aoid) ultimo_alerta USING(aoid)
                       WHERE aid = 1
                       GROUP BY obrid, usucpf, dtacao)
                       {$colunaEstadoDocumentoWith}
        select
            DISTINCT
            co.coid,
             {$colunaEstadoDocSql}
            obr.obrid,
            '(' || obr.obrid || ') ' || obr.obrnome as dslcricao,
            ende.estuf,
            mun.mundescricao,
            esd2.esddsc as estado_obra,
            p_conv.pronumeroprocesso as processo,
            COUNT( DISTINCT v_processo.obrid),
            case when esd.esddsc is null then 'Em Cadastramento' else esd.esddsc end as estado_cumprimento,
            TO_CHAR(doc.docdatainclusao,'DD/MM/YYYY') as data_inclusao,
            TO_CHAR(h.htddata, 'DD/MM/YYYY') htddata,
            TO_CHAR(dt_fim_vigencia_termo, 'DD/MM/YYYY') dt_fim_vigencia_termo,
            CASE
                WHEN leitor.usunome IS NOT NUll THEN 'Sim'
                ELSE 'Não'
            END AS lido,
            TO_CHAR(alerta_obra.dtacao, 'DD/MM/YYYY') dt_lido,
            leitor.usunome,
            doc.docid

        from obras2.controle_cumprimento_objeto cco
        inner join obras2.obras obr ON (cco.obrid = obr.obrid)
        inner join obras2.empreendimento emp on emp.empid = obr.empid
        LEFT JOIN obras2.cumprimento_objeto co ON (cco.obrid = co.obrid) and co.status = 'A'
        left join obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid)
        left join workflow.documento doc ON(co.docid = doc.docid)
        left join workflow.estadodocumento esd ON(doc.esdid = esd.esdid)
        left join workflow.documento doc2 ON(obr.docid = doc2.docid)
        left join workflow.estadodocumento esd2 ON(doc2.esdid = esd2.esdid)
        left join entidade.endereco ende ON ende.endid = obr.endid
            and ende.endstatus = 'A'
            AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
        left join territorios.municipio mun ON mun.muncod = ende.muncod
        LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = doc2.esdid
        LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = obr.obrid

        LEFT JOIN (select obrid, MAX(dt_fim_vigencia_termo) dt_fim_vigencia_termo from obras2.vm_vigencia_obra_2016 GROUP BY obrid) v ON v.obrid = obr.obrid
        LEFT JOIN workflow.historicodocumento h ON h.hstid = doc.hstid

        LEFT JOIN alerta_obra ON (alerta_obra.obrid = obr.obrid)

        LEFT JOIN obras2.vm_termo_convenio_obras AS v_processo ON (p_conv.pronumeroprocesso = v_processo.pronumeroprocesso)

        LEFT JOIN seguranca.usuario AS leitor ON (alerta_obra.usucpf = leitor.usucpf )

        {$estadoDocumentoJoin}

        WHERE obr.obrstatus = 'A' {$lido}
            " . (count($where) ? (' AND ' . implode(' AND ', $where)) : "") . "
		    " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "";

            $tamanho = Array('4%', '', '', '', '', '', '', '', '', '', '');

            //adicionando GROUP BY
            $sql .= " GROUP BY co.coid,  {$colunaEstadoDocSql} obr.obrid, ende.estuf, mun.mundescricao, esd2.esddsc, p_conv.pronumeroprocesso, "
                . " esd.esddsc, doc.docdatainclusao, h.htddata, dt_fim_vigencia_termo, doc.docid, leitor.usunome,"
                . " alerta_obra.dtacao ";


            //fazendo a query
            $resultados = $db->carregar($sql);

            //adicionando primeira coluna do historico
            //(como numero é indefinido as proximas colunas de historico ficams em cabeçalho)
            $cabecalho[] = 'historico';

            //buscando o histórico do workflow  do cumprimento do objeto

            if($resultados != ""){
                foreach ($resultados as $j => $resultado) {
                    if ($resultado['docid']) {
                        $query = "SELECT acao.aeddscrealizada, usu.usunome, log.usucpf, TO_CHAR(log.htddata, 'DD/MM/YYYY') AS data
			            FROM workflow.historicodocumento AS log
				            JOIN workflow.acaoestadodoc AS acao ON (log.aedid = acao.aedid)
				            JOIN seguranca.usuario AS usu ON (log.usucpf = usu.usucpf)
			            WHERE log.docid = " . $resultado['docid'] . "
			            ORDER BY log.htddata DESC ";

                        $historico = $db->carregar($query);

                        //removendo ultimo elemento do array (docid)
                        array_pop($resultados[$j]);

                        //var_dump($historico);die();
                        if (empty($historico)) {
                            continue;
                        }

                        //adicionando colunas de histórico
                        foreach ($historico as $k => $item) {
                            $resultados[$j][] .=
                                $item['aeddscrealizada'] . " por " . $item['usunome'] .
                                "(" . $item['usucpf'] . ") em " . $item['data'];
                        }
                    }
                }

                $db->sql_to_xml_excel($resultados, 'relatorioListaCumprimentoObjeto', $cabecalho, '');
            }else{
                echo '<script type="text/javascript">alert("Não foram encontrados Registros."); </script>';
            }

        } else {

            if($_POST['novaPactuacao'] == 14){
                $colunaEstadoDocSql = " ,p.esddsc";
            }else{
                $colunaEstadoDocSql = '';
            }
            $cabecalho = array('Ação','ID Cumprimento Objeto', 'ID Obra', 'Obra', 'UF',
                                'Município', "Estado da Obra", 'Numero do Processo', 'Obras do Processo', 'Situação do Cumprimento do Objeto',
                                'Data de Cadastro', 'Dt. Ult. Tramitação', 'Dt. Vencimento Termo', 'Leitura Notificação de Cumprimento', 'Data Leitura',
                                'Lida por',$colunaEstadoDocumento);


            if ($_POST['req'] == 'pesquisar') {


                $sql = "
WITH alerta_obra AS (SELECT max(aoid) AS aoid, obrid, usucpf, dtacao
                       FROM obras2.alerta_obra
                         INNER JOIN (SELECT max(aolid),
                                            aoid,
                                            (SELECT usucpf
                                               FROM obras2.alerta_obra_lido aol2
                                               WHERE aol2.aolid = MAX(aol.aolid)) as usucpf,
                                            (SELECT dtacao
                                               FROM obras2.alerta_obra_lido aol2
                                               WHERE aol2.aolid = MAX(aol.aolid)) as dtacao
                                       FROM obras2.alerta_obra_lido aol
                                       WHERE aid = 1
                                         AND acao = 'L'
                                      GROUP BY aoid) ultimo_alerta USING(aoid)
                       WHERE aid = 1
                       GROUP BY obrid, usucpf, dtacao)
                       {$colunaEstadoDocumentoWith}
        select
            DISTINCT
            '<center>
                <div style=\"width:100%;\">
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: acessaCumprimento(' || obr.obrid || ');\"
                        title=\"Acessar Cumprimento do Objeto\">
                </div>
            </center>' AS acao,
            co.coid,
            obr.obrid,
            '<a href=\"javascript: alterarObr(' || obr.obrid || ' );\">(' || obr.obrid || ') ' || obr.obrnome || '</a>' as dslcricao,
            ende.estuf,
            mun.mundescricao,
            esd2.esddsc as estado_obra,
            p_conv.pronumeroprocesso as processo,
            '<a href=\"javascript:void(0);\" data-processo=\"' || p_conv.pronumeroprocesso || '\" class=\"processo\" >' || COUNT( DISTINCT v_processo.obrid) || '</a>',
            case when esd.esddsc is null then 'Em Cadastramento' else esd.esddsc end as estado_cumprimento,
            TO_CHAR(doc.docdatainclusao,'DD/MM/YYYY') as data_inclusao,
            TO_CHAR(h.htddata, 'DD/MM/YYYY') htddata,
            TO_CHAR(dt_fim_vigencia_termo, 'DD/MM/YYYY') dt_fim_vigencia_termo,
            CASE
                WHEN leitor.usunome IS NOT NUll THEN 'Sim'
                ELSE 'Não'
            END AS lido,
            TO_CHAR(alerta_obra.dtacao, 'DD/MM/YYYY') dt_lido,
            leitor.usunome
            {$colunaEstadoDocSql}


        from obras2.controle_cumprimento_objeto cco
        inner join obras2.obras obr ON (cco.obrid = obr.obrid)
        inner join obras2.empreendimento emp on emp.empid = obr.empid
        LEFT JOIN obras2.cumprimento_objeto co ON (cco.obrid = co.obrid) and co.status = 'A'
        left join obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid)
        left join workflow.documento doc ON(co.docid = doc.docid)
        left join workflow.estadodocumento esd ON(doc.esdid = esd.esdid)
        left join workflow.documento doc2 ON(obr.docid = doc2.docid)
        left join workflow.estadodocumento esd2 ON(doc2.esdid = esd2.esdid)
        left join entidade.endereco ende ON ende.endid = obr.endid
            and ende.endstatus = 'A'
            AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
        left join territorios.municipio mun ON mun.muncod = ende.muncod
        LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = doc2.esdid
        LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = obr.obrid

        LEFT JOIN obras2.vm_termo_convenio_obras AS v_processo ON (p_conv.pronumeroprocesso = v_processo.pronumeroprocesso)

        LEFT JOIN (select obrid, MAX(dt_fim_vigencia_termo) dt_fim_vigencia_termo from obras2.vm_vigencia_obra_2016 GROUP BY obrid) v ON v.obrid = obr.obrid
        LEFT JOIN workflow.historicodocumento h ON h.hstid = doc.hstid
        LEFT JOIN alerta_obra ON (alerta_obra.obrid = obr.obrid)

        LEFT JOIN seguranca.usuario AS leitor ON (alerta_obra.usucpf = leitor.usucpf )

        {$estadoDocumentoJoin}

        WHERE obr.obrstatus = 'A' {$lido}
            " . (count($where) ? (' AND ' . implode(' AND ', $where)) : "") . "
		    " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "";

                $sql .= "GROUP BY  obr.obrid,   co.coid, ende.estuf, mun.mundescricao, esd2.esddsc, alerta_obra.dtacao, "
                    . "p_conv.pronumeroprocesso, esd.esddsc, doc.docdatainclusao, h.htddata, dt_fim_vigencia_termo, leitor.usunome {$colunaEstadoDocSql}";

                $tamanho = Array('4%', '', '', '', '', '', '', '', '', '', '');
                $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', $par2, "formulario", $tamanho);

            }
        }
    } else {
        echo '<script>alert("Para realizar a pesquisa deve ser preenchido ao menos um filtro");</script>';
    }
}

?>
<script type="text/javascript">

    $("*[name='novaPactuacao']").change(function(){
        valor = ($(this).attr('value'));
       if(valor == 14){
           $( "#situacaoNovaPac" ).show();
       }else{
           $( "#situacaoNovaPac" ).hide();
       }
    });



    function validaFiltro(botao) {

        var obrbuscatexto = document.getElementById('obrbuscatexto').value;
        var strid = document.getElementById('strid').value;
        var stcoid = document.getElementById('stcoid').value;
        var prfid = document.getElementById('prfid').value;
        var tooid = document.getElementById('tooid').value;
        var muncod = document.getElementById('muncod').value;
        // var estuf = document.getElementById('estuf').value;
        // var empesfera = document.getElementById('empesfera').value;
        var convenio = document.getElementById('convenio').value;
        var ano_convenio = document.getElementById('ano_convenio').value;
        var ano_processo = document.getElementById('ano_processo').value;
        var botao = botao.id;


        var filtros =
                [obrbuscatexto,
                strid,
                stcoid,
                prfid,
                tooid,
                muncod,
                convenio,
                ano_convenio,
                ano_processo];

        var filtrosQuatidade = filtros.length;

        var ocorrenciasFiltro = 0;
        for (i = 0; i < filtrosQuatidade; i++) {

            if (filtros[i] != '') {
                ocorrenciasFiltro++;
            }
        }
        if (ocorrenciasFiltro > 0) {

            if(botao == 'pesquisar'){
                document.getElementById('req').value = 'pesquisar';
                document.formListaObraCumprimento.submit();
            }else if(botao == 'excel'){
                document.getElementById('req').value = 'excel';
                document.formListaObraCumprimento.submit();
            }

        } else {
            alert('Para realizar a pesquisa deve ser preenchido ao menos um filtro.');
            return false;
        }

    }

    $(document).ready(function () {
        // $('.pesquisar').click(function (){
        // 	$('#req').val('');
        // 	$('#formListaObraCumprimento').submit();
        // });
        $('#button_limpar').click(function () {
            $('#req').val('limpar');
            $('#formListaObraCumprimento').submit();
        });

        $('#button_liberado').click(function () {
            $('#req').val('liberado_excel');
            $('#formListaObraCumprimento').submit();
        });

        $('.processo').click(function () {
            var processo = $(this).attr('data-processo');
            //alert(processo);
            window.open('/obras2/obrasPorProcesso.php?processo=' + processo, '', 'width=600,height=600');
        });
    });

    $1_11(document).ready(function () {
        $1_11('select[name="strid[]"]').chosen();
        $1_11('select[name="stcoid[]"]').chosen();
        $1_11('select[name="prfid[]"]').chosen();
        $1_11('select[name="tooid[]"]').chosen();
        // $1_11('select[name="estuf[]"]').chosen();
        $1_11('select[name="muncod[]"]').chosen();
        $1_11('select[name="esdid[]"]').chosen();
        $1_11('select[name="tpoid[]"]').chosen();
        $1_11('select[name="esdidsituaco[]"]').chosen();
        $1_11('select[name="tslid[]"]').chosen();
        $1_11('select[name="esdidsituaco[]"]').chosen();
        $1_11(".estados").chosen().change(function (e, params) {
            values = $1_11(".estados").chosen().val();
            carregarMunicipio(values);
        });
    });

    function acessaCumprimento(obrid) {
        location.href = '?modulo=principal/cadCumprimentoObjeto&acao=A&obrid=' + obrid;
    }

    function alterarObr(obrid) {
        location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
    }

    // function carregarMunicipio(estuf) {
    //     var td = $('#td_municipio');
    //     if (estuf != '') {
    //         var url = location.href;
    //         $.ajax({
    //             url: url,
    //             type: 'post',
    //             data: {ajax: 'municipio', estuf: estuf},
    //             dataType: "html",
    //             async: false,
    //             beforeSend: function () {
    //                 divCarregando();
    //                 td.find('select option:first').attr('selected', true);
    //             },
    //             error: function () {
    //                 divCarregado();
    //             },
    //             success: function (data) {
    //                 td.html(data);
    //                 divCarregado();
    //             }
    //         });
    //     } else {
    //         td.find('select option:first').attr('selected', true);
    //         td.find('select').attr('selected', true).attr('disabled', true);
    //     }
    // }
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

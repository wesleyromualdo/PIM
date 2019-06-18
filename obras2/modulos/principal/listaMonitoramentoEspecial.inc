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
        $monitoramento = new MonitoramentoEspecial($_POST['mesid']);
        $monitoramento->messtatus = 'I';
        $monitoramento->salvar();
        $monitoramento->commit();
        die('<script type="text/javascript">
            alert(\'Operação realizada com sucesso!\');
            location.href=\'?modulo=principal/listaMonitoramentoEspecial&acao=A\';
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


monta_titulo( 'Lista de Monitoramento Especial', '');

if(empty($_POST) && !empty($_SESSION['obras2']['monitoramentoespecial']['filtros'])) {
    unset($_SESSION['obras2']['monitoramentoespecial']['filtros']['req']);
    $_POST = $_SESSION['obras2']['monitoramentoespecial']['filtros'];
} else
    $_SESSION['obras2']['monitoramentoespecial']['filtros'] = $_POST;

//ver($_SESSION['obras2'], d);

extract( $_POST );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form method="post" name="formListaMonitoramentoEspecial" id="formListaMonitoramentoEspecial">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="obrid" id="obrid" value="">
    <input type="hidden" name="empid" id="empid" value="">
    <input type="hidden" name="sldid" id="sldid" value="">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
            <td>
                <?=campo_texto('obrbuscatexto','N','S','',70,100,'','', '', '', '', 'id="obrbuscatexto"');?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="15%">ID do Monitoramento:</td>
            <td>
                <?=campo_texto('mesid','N','S','',70,100,'','', '', '', '', 'id="sldid"');?>
            </td>
        </tr>


        <tr>
            <td class="SubTituloDireita">
                Situação do Obra:
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
            <td class="SubTituloDireita">Esfera:</td>
            <td>
                <?php
                $sql = Array(Array('codigo'=>'E', 'descricao'=>'Estadual'),
                    Array('codigo'=>'M', 'descricao'=>'Municipal'));
                $db->monta_combo('empesfera',$sql, 'S','Selecione...','','','',200,'N', 'empesfera');
                ?>
            </td>
        </tr>
        <tr>

            <td class="SubTituloDireita">UF(s):</td>
            <td>

                <select name="estuf[]" class="chosen-select estados" multiple data-placeholder="Selecione">
                    <?php  $uf = new Estado();
                    foreach ($uf->listaCombo() as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($estuf) && in_array($key['codigo'], $estuf)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>


            </td>


        </tr>
        <tr>
            <td class="SubTituloDireita">Municípios(s):</td>
            <td class="td_municipio">
                <?php if (!empty($estuf)) { ?>

                    <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                        <?php   $municipio = new Municipio();
                        foreach ($municipio->listaComboMulti($estuf) as $key) {
                            ?>
                            <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                        <?php } ?>
                    </select>
                <?php } ?>
            </td>
        </tr>


        <tr>
            <td class="SubTituloDireita">Convênio/Termo:</td>
            <td>
                Número:&nbsp;
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
            <td class="SubTituloDireita">Processo:</td>
            <td>
                Número:&nbsp;
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
            <td class="SubTituloDireita">
                Situação do Monitoramento:
            </td>
            <td>
                <select name="esdidsituacao[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_SITUACAO_MONITORAMENTO . "' AND esdstatus='A' ORDER BY esdordem";
                    $dados = $db->carregar($sql);
                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($esdidsituacao) && in_array($key['codigo'], $esdidsituacao)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">
                Orgão Responsável
            </td>
            <td>
                <select name="esdidmonitoramento[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_RESPONSAVEL_MONITORAMENTO . "' AND esdstatus='A' ORDER BY esdordem";
                    $dados = $db->carregar($sql);
                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($esdidmonitoramento) && in_array($key['codigo'], $esdidmonitoramento)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">
                Origem
            </td>
            <td>
                <select name="meoid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $sql = "SELECT meoid as codigo, meonome as descricao FROM obras2.monitoramento_especial_origem WHERE meostatus='A' ORDER BY meonome";
                    $dados = $db->carregar($sql);
                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($meoid) && in_array($key['codigo'], $meoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>


        <tr>
            <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
                <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                <input type="button" name="btnEexcel" class="btnEexcel" value="Gerar Excel"/>
                <? if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))): ?>
                    <input type="button" name="btnEstatistica" class="btnEstatistica" value="Gerar Estatística"/>
                <?endif;?>
                <input id="button_limpar" type="button" value="Limpar Filtros"/>
            </td>
        </tr>
    </table>
</form>

<?php

if ( $obrbuscatexto ){
    $where[] = " ( UPPER(public.removeacento(o.obrnome)) ILIKE ('%" . removeAcentos($obrbuscatexto) . "%') OR
				   public.removeacento(o.obrid::CHARACTER VARYING) ILIKE ('%" . removeAcentos($obrbuscatexto) . "%') ) ";
}

if( $empesfera ){
    $empesfera = (array) $empesfera;
    $where[] = "e.empesfera IN('" . implode("', '", $empesfera) . "')";
}


if ($_POST['mesid'])
    $where['mesid'] = "m.mesid IN ({$_POST['mesid']})";

if ($_POST['estuf'] && $_POST['estuf'][0] != '')
    $where['estuf'] = "mun.estuf IN ('" . implode("', '", $_POST['estuf']) . "')";
if ($_POST['muncod'] && $_POST['muncod'][0] != '')
    $where['muncod'] = "mun.muncod IN ('" . implode("', '", $_POST['muncod']) . "')";
if ($_POST['processo'])
    $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$_POST['processo']}','.',''),'/',''),'-','')";
if ($_POST['ano_processo'])
    $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$_POST['ano_processo']}'";
if ($_POST['convenio'])
    $where['convenio'] = "p_conv.termo_convenio = '{$_POST['convenio']}'";
if ($_POST['ano_convenio'])
    $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$_POST['ano_convenio']}'";
if ($_POST['esdid'] && $_POST['esdid'][0] != '')
    $where['esdid'] = "doc.esdid IN (" . implode(', ', $_POST['esdid']) . ")";
if ($_POST['tpoid'] && $_POST['tpoid'][0] != '')
    $where['tpoid'] = "o.tpoid IN ('" . implode("', '", $_POST['tpoid']) . "')";
if ($_POST['prfid'] && $_POST['prfid'][0] != '')
    $where['prfid'] = "emp.prfid IN(" . implode(',', $_POST['prfid']) . ")";
if ($_POST['tooid'] && $_POST['tooid'][0] != '')
    $where['tooid'] = "o.tooid IN(" . implode(',', $_POST['tooid']) . ")";

if ($_POST['esdidsituacao'] && $_POST['esdidsituacao'][0] != '')
    $where['esdidsituacao'] = "es.esdid IN (" . implode(', ', $_POST['esdidsituacao']) . ")";

if ($_POST['esdidresponsavel'] && $_POST['esdidresponsavel'][0] != '')
    $where['esdidresponsavel'] = "er.esdid IN (" . implode(', ', $_POST['esdidresponsavel']) . ")";


$acao = '';
$with = '';
$estatistica = '';
if ($_POST['req'] != 'excel') {

    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
        $excluir = "<img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluirMonitoramento(' ||m.mesid || ');\"
                        title=\"Excluir\">";
    }

    $acao = "'<center style=\"width:60px\">
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/icone_lupa.png\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: abreMonitoramento(' || m.mesid  || ');\"
                        title=\"Alterar\">
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: alterarObr(' || o.obrid || ');\"
                        title=\"Alterar Obra\">
                    $excluir
                 </center>' AS acao,";
}


$sql = "
    $with
        SELECT * FROM (
                SELECT
                    $acao
                    m.mesid,
                    o.obrid,
                    o.obrnome,
                    mun.mundescricao,
                    mun.estuf,
                    es.esddsc situacao,
                    er.esddsc responsavel,
                    m.mesdtinclusao,
                    mo.meonome,
                    u.usunome,
                    m.mesdtlimite,
                    m.mestitulo

                FROM obras2.monitoramento_especial m
                JOIN obras2.obras o ON o.obrid = m.obrid
                JOIN workflow.documento ds ON ds.docid = m.docidsituacao
                JOIN workflow.estadodocumento es ON es.esdid = ds.esdid
                JOIN workflow.documento dr ON dr.docid = m.docidresponsavel
                JOIN workflow.estadodocumento er ON er.esdid = dr.esdid
                JOIN seguranca.usuario u ON u.usucpf = m.usucpfinclusao
                JOIN entidade.endereco edr ON edr.endid = o.endid
                JOIN territorios.municipio mun ON mun.muncod = edr.muncod
                JOIN obras2.monitoramento_especial_origem mo ON mo.meoid = m.meoid


                JOIN workflow.documento as doc ON doc.docid = o.docid
                JOIN workflow.estadodocumento eo ON eo.esdid = doc.esdid
                LEFT JOIN obras2.empreendimento e ON e.empid = o.empid
                LEFT JOIN obras2.programafonte pf ON pf.prfid = e.prfid
                LEFT JOIN obras2.tipologiaobra tpo ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.tipoorigemobra too ON too.tooid = o.tooid

                LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid
                WHERE
                    m.messtatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ',$where) : "") . "
                " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "

                ) as f

                ORDER BY 2
            ";


if ($_POST['req'] == 'excel') {
    $cabecalho = array('ID', 'ID Obra', 'Obra', 'Município', 'UF', 'Situação', 'Responsável', 'Data de Cadastro', 'Orgão', 'Usuário', 'Data Limite', 'Título');

    $db->sql_to_xml_excel($sql, 'relatorioListaObjetosObras', $cabecalho, '');
} else {
    $cabecalho = array('Ação', 'ID', 'ID Obra', 'Obra', 'Município', 'UF', 'Situação', 'Responsável', 'Data de Cadastro', 'Orgão', 'Usuário', 'Data Limite', 'Título');

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
            $('#formListaMonitoramentoEspecial').submit();
        });

        $('#button_limpar').click(function() {
            $('#req').val('limpar');
            $('#formListaMonitoramentoEspecial').submit();
        });
    });

    function excluirMonitoramento(mesid) {
        if (confirm('Deseja apagar este monitoramento?')) {
            $('#mesid').val(sldid);
            $('#req').val('apagar');
            $('#formListaMonitoramentoEspecial').submit();
        }
    }

    function alterarObr(obrid) {
        location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
    }

    function abreMonitoramento(mesid){
        windowOpen('?modulo=principal/cadastroAtividadeMonitoramentoEspecial&acao=A&mesid=' + mesid,'telaSolicitacaoDesembolso','height=700,width=1200,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }

    $('.btnEexcel').click(function () {
        $('#req').val('excel');
        $('#formListaMonitoramentoEspecial').submit();
    });

    $('.btnEstatistica').click(function () {
        $('#req').val('estatistica');
        $('#formListaMonitoramentoEspecial').submit();
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
    //-->


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
        $1_11('select[name="esdidsituacao[]"]').chosen();
        $1_11('select[name="esdidmonitoramento[]"]').chosen();
        $1_11('select[name="meoid[]"]').chosen();

        $1_11(".estados").chosen().change(function (e, params) {
            values = $1_11(".estados").chosen().val();
            carregarMunicipio(values);
        });
    });
</script>
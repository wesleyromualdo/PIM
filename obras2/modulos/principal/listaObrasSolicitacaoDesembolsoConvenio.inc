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
    unset($_SESSION['obras2']['solicitacaodesembolsoconvenio']['filtros']);

    echo "<script>window.location.href = window.location.href;</script>";
    exit();
}


if ($_POST['req'] == 'apagar' && !empty($_POST['sdcid'])) {
    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
        $solicitacaoDesembolso = new SolicitacaoDesembolsoConvenio($_POST['sdcid']);
        $solicitacaoDesembolso->sdcstatus = 'I';
        $solicitacaoDesembolso->salvar();
        $solicitacaoDesembolso->commit();
        die('<script type="text/javascript">
            alert(\'Operação realizada com sucesso!\');
            location.href=\'?modulo=principal/listaObrasSolicitacaoDesembolsoConvenio&acao=A\';
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


monta_titulo( 'Lista de Solicitações de Desembolso Convênio', '');

if(empty($_POST) && !empty($_SESSION['obras2']['solicitacaodesembolsoconvenio']['filtros'])) {
    unset($_SESSION['obras2']['solicitacaodesembolsoconvenio']['filtros']['req']);
    $_POST = $_SESSION['obras2']['solicitacaodesembolsoconvenio']['filtros'];
} else
    $_SESSION['obras2']['solicitacaodesembolsoconvenio']['filtros'] = $_POST;

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
    
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">

        <tr>
            <td class="SubTituloDireita" width="15%">ID da Solicitação:</td>
            <td>
                <?=campo_texto('sdcid','N','S','',70,100,'#########','', '', '', '', 'id="sdcid"');?>
            </td>
        </tr>

        <? if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))): ?>

 
            <tr>
                <td class="SubTituloDireita">Municípios(s):</td>
                <td class="td_municipio">
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
			<td class="SubTituloDireita" style="width: 190px;">Situação:</td>
			<td>
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

                <select name="esdidsituaco[]" id="esdidsituaco" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($esdidsituaco) && in_array($key['codigo'], $esdidsituaco)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
			</td>
		</tr>

        <tr>
            <td class="SubTituloDireita">Convênio/Termo:</td>
            <td>
                Número:&nbsp;
                <?php
                echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, 'id="convenio"');
                ?>
                Ano:&nbsp;
                <?php
                echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, 'id="ano_convenio"');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Processo:</td>
            <td>
                Número:&nbsp;
                <?php
                echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, 'id="processo"');
                ?>
                Ano:&nbsp;
                <?php
                echo campo_texto('ano_processo', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, 'id="ano_processo"');
                ?>
            </td>
        </tr>


        <tr>
            <td class="SubTituloDireita" style="width: 190px;">% Solicitado:</td>
            <td>
                <table>
                    <tr>
                        <th>Mínimo</th>
                        <th>Máximo</th>
                    </tr>
                    <tr>
                        <?php

                        $arPercentual[]  = array( 'codigo' =>  0 , 'descricao' => '0 %' );
                        $arPercentual[]  = array( 'codigo' =>  5 , 'descricao' => '5 %' );
                        $arPercentual[]  = array( 'codigo' => 10 , 'descricao' => '10 %' );
                        $arPercentual[]  = array( 'codigo' => 15 , 'descricao' => '15 %' );
                        $arPercentual[]  = array( 'codigo' => 20 , 'descricao' => '20 %' );
                        $arPercentual[]  = array( 'codigo' => 25 , 'descricao' => '25 %' );
                        $arPercentual[]  = array( 'codigo' => 30 , 'descricao' => '30 %' );
                        $arPercentual[]  = array( 'codigo' => 35 , 'descricao' => '35 %' );
                        $arPercentual[]  = array( 'codigo' => 40 , 'descricao' => '40 %' );
                        $arPercentual[]  = array( 'codigo' => 45 , 'descricao' => '45 %' );
                        $arPercentual[]  = array( 'codigo' => 50 , 'descricao' => '50 %' );
                        $arPercentual[]  = array( 'codigo' => 55 , 'descricao' => '55 %' );
                        $arPercentual[]  = array( 'codigo' => 60 , 'descricao' => '60 %' );
                        $arPercentual[]  = array( 'codigo' => 65 , 'descricao' => '65 %' );
                        $arPercentual[]  = array( 'codigo' => 70 , 'descricao' => '70 %' );
                        $arPercentual[]  = array( 'codigo' => 75 , 'descricao' => '75 %' );
                        $arPercentual[]  = array( 'codigo' => 80 , 'descricao' => '80 %' );
                        $arPercentual[]  = array( 'codigo' => 85 , 'descricao' => '85 %' );
                        $arPercentual[]  = array( 'codigo' => 90 , 'descricao' => '90 %' );
                        $arPercentual[]  = array( 'codigo' => 95 , 'descricao' => '95 %' );
                        $arPercentual[]  = array( 'codigo' => 100 , 'descricao' => '100 %' );

                        $percentualsolicitadoinicial = $_POST['percentualsolicitadoinicial'];
                        $percentualsolicitadofinal = $_POST['percentualsolicitadofinal'];
                        $percfinal = $percentualsolicitadofinal == '' ? 100 : $percentualsolicitadofinal;
                        print '<td>';
                        $db->monta_combo("percentualsolicitadoinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualsolicitadoinicial');
                        print '</td><td>';
                        $db->monta_combo("percentualsolicitadofinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualsolicitadofinal', false, $percfinal);
                        print '</td>';
                        ?>
                    </tr>
                </table>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">% Aprovado:</td>
            <td>
                <table>
                    <tr>
                        <th>Mínimo</th>
                        <th>Máximo</th>
                    </tr>
                    <tr>
                        <?php
                        $percentualaprovadoinicial = $_POST['percentualaprovadoinicial'];
                        $percentualaprovadofinal = $_POST['percentualaprovadofinal'];
                        $percfinal = $percentualaprovadofinal == '' ? 100 : $percentualaprovadofinal;
                        print '<td>';
                        $db->monta_combo("percentualaprovadoinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualaprovadoinicial');
                        print '</td><td>';
                        $db->monta_combo("percentualaprovadofinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualaprovadofinal', false, $percfinal);
                        print '</td>';
                        ?>
                    </tr>
                </table>
            </td>
        </tr>
        <? else: ?>
            <tr>
                <td class="SubTituloDireita" style="width: 190px;">Situação:</td>
                <td>
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

                    <select name="esdidsituaco[]" id="esdidsituaco"  class="chosen-select" multiple data-placeholder="Selecione">
                        <?php
                        foreach ($dado as $key) {
                            ?>
                            <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($esdidsituaco) && in_array($key['codigo'], $esdidsituaco)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                        <?php } ?>
                    </select>

                </td>
            </tr>
        <? endif; ?>

        <? if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))): ?>
        <tr>
            <td class="SubTituloDireita">Solicitações inseridas por Carga:</td>
            <td>
                <label><input type="radio" name="carga" value="1" <?= $carga == 1 ? 'checked=checked' : ''?>> Sim</label>
                <label><input type="radio" name="carga" value="2" <?= $carga == 2 ? 'checked=checked' : ''?>> Não</label>
                <label><input type="radio" name="carga" value="0" <?= $carga == 0 ? 'checked=checked' : ''?>> Todos</label>
            </td>
        </tr>
        <? endif; ?>
        <tr>
            <td class="SubTituloDireita">Data de Inserção:</td>
            <td>
                De:
                <?php echo campo_data2('data_de', 'S', true, '', null, '', ' ', '', 'id="data_de"'); ?>
                &nbsp;até&nbsp;
                <?php echo campo_data2('data_ate', 'S', true, '', null, '', ' ', '', 'id="data_ate"'); ?>
            </td>
        </tr>

        <tr>
			<td class="divTituloPadrao" width="15%" colspan="2" align="center">
                <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar" id="pesquisar" onclick="validaFiltro(this);" />
                <input type="button" name="btnEexcel" class="btnEexcel" value="Gerar Excel" id="excel" onclick="validaFiltro(this);"/>
                <input id="button_limpar" type="button" value="Limpar Filtros"/>
			</td>
		</tr>
	</table>
</form>

<?php


if ($_POST['sdcid'])
    $where['sdcid'] = "sv.sdcid::text IN ('{$_POST['sdcid']}')";

if ($_POST['sitpedido'] && $_POST['sitpedido'][0] != '')
    $where['sitpedido'] = "ps.situacao_pagamento  ILIKE ANY(ARRAY['%" . implode("%', '%", $_POST['sitpedido']) . "%'])  ";
//if ($_POST['estuf'] && $_POST['estuf'][0] != '')
    $where['estuf'] = "m.estuf IN ('SP')";
if ($_POST['muncod'] && $_POST['muncod'][0] != '')
    $where['muncod'] = "m.muncod IN ('" . implode("', '", $_POST['muncod']) . "')";
if ($_POST['processo'])
    $where['processo'] = "Replace(Replace(Replace( TRIM(processo),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$_POST['processo']}','.',''),'/',''),'-','')";
if ($_POST['ano_processo'])
    $where['ano_processo'] = "substring(Replace(Replace(Replace( processo,'.',''),'/',''),'-','') from 12 for 4) = '{$_POST['ano_processo']}'";
if ($_POST['convenio'])
    $where['convenio'] = "convenio = '{$_POST['convenio']}'";
//if ($_POST['ano_convenio'])
//    $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$_POST['ano_convenio']}'";
if ($_POST['esdid'] && $_POST['esdid'][0] != '')
    $where['esdid'] = "dc.esdid IN (" . implode(', ', $_POST['esdid']) . ")";
if ($_POST['carga'] && $_POST['carga'] == '1' || $_POST['carga'] == '2')
    $where['carga'] = $_POST['carga'] == '1' ? "sv.usucpf = '00000000191'" : "sv.usucpf != '00000000191'";
if ($_POST['data_de'] && $_POST['data_ate'])
    $where['data'] = 'sv.sdcdatainclusao BETWEEN \'' . ajusta_data($_POST['data_de']) . '\' AND \'' . ajusta_data($_POST['data_ate']) . '\'';

if ($_POST['percentualinicial'] != '')
    $where['percentualinicial'] = "COALESCE(obrpercentultvistoria, 0) >= " . $_POST['percentualinicial'];
if ($_POST['percentualfinal'] != '')
    if ($_POST['percentualfinal'] < 100)
        $where['percentualfinal'] = "COALESCE(obrpercentultvistoria, 0) <= " . $_POST['percentualfinal'];

if ($_POST['percentualsolicitadoinicial'] != '')
    $where['percentualsolicitadoinicial'] = "COALESCE(sv.sdcpercsolicitado, 0) >= " . $_POST['percentualsolicitadoinicial'];
if ($_POST['percentualsolicitadofinal'] != '')
    if ($_POST['percentualsolicitadofinal'] < 100)
        $where['percentualsolicitadofinal'] = "COALESCE(sv.sdcpercsolicitado, 0) <= " . $_POST['percentualsolicitadofinal'];

if ($_POST['percentualaprovadoinicial'] != '')
    $where['percentualaprovadoinicial'] = "COALESCE(sv.sdcpercpagamento, 0) >= " . $_POST['percentualaprovadoinicial'];
if ($_POST['percentualaprovadofinal'] != '')
    if ($_POST['percentualaprovadofinal'] < 100)
        $where['percentualaprovadofinal'] = "COALESCE(sv.sdcpercpagamento, 0) <= " . $_POST['percentualaprovadofinal'];

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


$resp = array();
$arPflcod = array();
$orWhere = array();


// Gestor
if (possui_perfil(array(PFLCOD_GESTOR_UNIDADE))) {
    $arPflcod = array(PFLCOD_GESTOR_UNIDADE);
    $orWhere['entidunidade'] = "sv.processo IN (
                                                        SELECT
                                                            pronumeroprocesso
                                                        FROM obras2.usuarioresponsabilidade urs
                                                        JOIN obras2.obras o ON o.obridpai IS NULL AND o.obrstatus = 'A' AND o.entid = urs.entid
                                                        JOIN obras2.vm_termo_convenio_obras c ON c.obrid = o.obrid
                                                        WHERE
                                                            urs.rpustatus = 'A' AND
                                                            urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                            urs.pflcod IN (" . implode(', ', $arPflcod) . ")
                                                                )";
}

// Fiscal
if (possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE))) {
    $usuarioResp = new UsuarioResponsabilidade();
    $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
    $arEmpid = ($arEmpid ? $arEmpid : array(0));

    $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;

    $orWhere['sup'] = "sv.processo IN (
                                        SELECT
                                            pronumeroprocesso
                                        FROM obras2.usuarioresponsabilidade urs
                                        JOIN obras2.empreendimento ep ON ep.empid = urs.empid
                                        JOIN obras2.obras o ON o.obridpai IS NULL AND o.obrstatus = 'A' AND o.empid = urs.empid
                                        JOIN obras2.vm_termo_convenio_obras c ON c.obrid = o.obrid
                                        WHERE
                                            urs.rpustatus = 'A' AND
                                            urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                            urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                            urs.empid IN('" . implode("', '", $arEmpid) . "'))";
}


$esddsc = (possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))) ? "e.esddsc" : "CASE WHEN e.esdid IN (".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA.", ".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL." , ".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI.") THEN 'Aguardando Análise FNDE' ELSE e.esddsc END AS estado_documento" ;

$acao = '';
if ($_POST['req'] != 'excel') {

    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
        $excluir = "<img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluirSolicitacao(' ||sv.sdcid || ');\"
                        title=\"Excluir\">";
    }

    $acao = "'<center style=\"width:60px\">
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/icone_lupa.png\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: abreSolicitacao(' || sv.sdcid  || ',' || sv.processo  || ');\"
                        title=\"Alterar Solicitação\">
                    $excluir
                 </center>' AS acao,";
}

if(isset($_POST['req']) && !empty($_POST['req'])) {
    $arrFiltros = array(
        $sdcid = $_POST['sdcid'],
        $estuf = $_POST['estuf'],
        $esdidsituaco = $_POST['esdidsituaco'],
        $convenio = $_POST['convenio '],
        $ano_convenio = $_POST['ano_convenio'],
        $processo = $_POST['processo'],
        $ano_processo = $_POST['ano_processo'],
        $esdidsituaco = $_POST['esdidsituaco'],
        $data_de = $_POST['data_de'],
        $data_ate = $_POST['data_ate']
    );

    $ocorrenciasFiltro = 0;
    $filtrosQuatidade = count($arrFiltros);

    for ($i = 0; $i < $filtrosQuatidade; $i++) {

        if ($arrFiltros[$i] != '' ) {
            $ocorrenciasFiltro++;
        }
    }

    if ($ocorrenciasFiltro > 0) {
        $sql = "
    SELECT * FROM (


            SELECT DISTINCT ON (sv.sdcid)
                $acao
                sv.sdcid,
                sv.processo,
                sv.convenio,
                sv.sdcpercsolicitado,
                sv.sdcpercpagamento,
                m.estuf,
                m.mundescricao,
                sv.sdcjustificativa,
                u.usunome usunome1,
                TO_CHAR(sv.sdcdatainclusao, 'DD/MM/YYYY') sdcdatainclusao,
                $esddsc,
                ud.usunome as usunome,
                TO_CHAR(h.htddata, 'DD/MM/YYYY') as htddata,
                c.cmddsc as cmddsc
            FROM obras2.solicitacao_desembolso_convenio sv

            JOIN painel.dadosconvenios dc ON dc.dcoprocesso = sv.processo

            JOIN territorios.municipio m ON m.muncod = dc.muncod
            JOIN seguranca.usuario u ON u.usucpf = sv.usucpf
            JOIN workflow.documento d ON d.docid = sv.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid
            LEFT JOIN workflow.historicodocumento h ON d.hstid = h.hstid
            LEFT JOIN workflow.comentariodocumento  c ON h.hstid = c.hstid AND c.cmdstatus = 'A'
            LEFT JOIN seguranca.usuario ud ON h.usucpf = ud.usucpf
            WHERE sv.sdcstatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
            " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "

            ) as f

            ORDER BY 1
            ";

        if ($_POST['req'] == 'excel') {
            $cabecalho = array('ID Solicitação', 'Nº Processo', 'Nº Convênio', '% Solicitado', '% Execução Validado', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Data de Cadastro',
                'Situação do Deferimento', 'Última tramitação', 'Data da Resposta', 'Observação da Resposta');
            $db->sql_to_xml_excel($sql, 'relatorioListaObjetosObras', $cabecalho, '');
        } else {
            $cabecalho = array('Ação', 'ID Solicitação', 'Nº Processo', 'Nº Convênio', '% Solicitado', '% Execução Validado', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Data de Cadastro',
                'Situação do Deferimento', 'Última tramitação', 'Data da Resposta', 'Observação da Resposta');
            $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', null, "formulario");
        }

    }else{
        echo '<script>alert("Para realizar a pesquisa deve ser preenchido ao menos um filtro");</script>';
    }
}

?>
<script type="text/javascript">
    function validaFiltro(botao){

        var sdcid = document.getElementById('sdcid').value;
        var estuf = 'SP'//document.getElementById('estuf').value;
        var esdidsituaco = document.getElementById('esdidsituaco').value;
        var convenio = document.getElementById('convenio').value;
        var ano_convenio = document.getElementById('ano_convenio').value;
        var processo = document.getElementById('processo').value;
        var ano_processo = document.getElementById('ano_processo').value;
        var esdidsituaco = document.getElementById('esdidsituaco').value;
        var data_de = document.getElementById('data_de').value;
        var data_ate = document.getElementById('data_ate').value;

        var botao = botao.id;

        var filtros =
                [sdcid,
                estuf,
                esdidsituaco,
                convenio,
                ano_convenio,
                processo,
                ano_processo,
                esdidsituaco,
                data_de,
                data_ate];

        var filtrosQuatidade = filtros.length;

        var ocorrenciasFiltro = 0;
        for(i=0;i<filtrosQuatidade;i++ ){

            if(filtros[i] != ''){
                ocorrenciasFiltro ++;
            }
        }
        if(ocorrenciasFiltro > 0){

            if(botao == 'pesquisar'){
                document.getElementById('req').value = 'pesquisar';
                document.formListaObraSolicitacaoDesembolso.submit();
            }else if(botao == 'excel'){
                document.getElementById('req').value = 'excel';
                document.formListaObraSolicitacaoDesembolso.submit();
            }

        }else{
            alert('Para realizar a pesquisa deve ser preenchido ao menos um filtro.');
            return false;
        }

    }
$(document).ready(function (){

    $('#button_limpar').click(function() {
        $('#req').val('limpar');
        $('#formListaObraSolicitacaoDesembolso').submit();
    });
});

function excluirSolicitacao(sdcid) {
    if (confirm('Deseja apagar esta solicitação?')) {
        $('#sdcid').val(sdcid);
        $('#req').val('apagar');
        $('#formListaObraSolicitacaoDesembolso').submit();
    }
}

function alterarObr(obrid) {
    location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function abreSolicitacao(sdcid, processo){
	windowOpen('?modulo=principal/popupSolicitarDesembolsoConvenio&acao=A&processo='+processo+'&sdcid=' + sdcid,'telaSolicitacaoDesembolsoConvenio','height=700,width=1200,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

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

        $1_11(".estados").chosen().change(function (e, params) {
            values = $1_11(".estados").chosen().val();
            carregarMunicipio(values);
        });
    });
</script>
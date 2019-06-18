<?php
/**
 * Simec
 */
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

extract($_POST);
$where = array();

if ($_REQUEST['obrbuscatexto']) {
    $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($_REQUEST['obrbuscatexto']))));
    $obrbuscatextoTemp = trim($obrbuscatextoTemp);

    if (!strpos($obrbuscatextoTemp, ',')) {
        $where['obrnome'] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                    obr.obrid::CHARACTER VARYING ILIKE ('%" . trim($_REQUEST['obrbuscatexto']) . "%') ) ";
    } else {
        $campos = explode(',', $obrbuscatextoTemp);
        $w = array();
        foreach ($campos as $c) {
            $c = trim($c);
            $w[] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                    obr.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') ) ";
        }

        $w = '(' . implode('OR', $w) . ')';
        $where['obrnome'] = $w;
    }
}

if ($esdid && $esdid[0] != '') {
    $where['esdid'] = "doc.esdid IN(" . implode(',', $esdid) . ")";
}
if ($prfid && $prfid[0] != '') {
    $where['prfid'] = "emp.prfid IN(" . implode(',', $prfid) . ")";
}
if ($tooid && $tooid[0] != '') {
    $where['tooid'] = "obr.tooid IN(" . implode(',', $tooid) . ")";
}
if ($estuf && $estuf[0] != '') {
    $where['estuf'] = "mun.estuf IN ('" . implode("', '", $_REQUEST['estuf']) . "')";
}
if ($muncod && $muncod[0] != '') {
    $where['muncod'] = "mun.muncod IN ('" . implode("', '", $muncod) . "')";
}
if ($empesfera) {
    $where['empesfera'] = "emp.empesfera IN('" . $empesfera . "')";
}
if ($mescod) {
    $where['mescod'] = "mes.mescod = '{$mescod}'";
}
if ($convenio) {
    $where['convenio'] = "p_conv.termo_convenio = '{$convenio}'";
}
if ($ano_convenio) {
    $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$ano_convenio}'";
}
if ($processo) {
    $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$processo}','.',''),'/',''),'-','')";
}
if ($ano_processo) {
    $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$ano_processo}'";
}

if ($dtiniciofuncionamento && $dtfinalfuncionamento) {
    $where[] = "igo.dtiniciofuncionamento BETWEEN TO_DATE('{$dtiniciofuncionamento}', 'DD/MM/YYYY') AND TO_DATE('{$dtfinalfuncionamento} 23:59:59', 'DD/MM/YYYY HH24:MI:SS')";
} elseif ($dtiniciofuncionamento) {
    $where[] = "igo.dtiniciofuncionamento >= TO_DATE('{$dtiniciofuncionamento}', 'DD/MM/YYYY')";
} elseif ($dtfinalfuncionamento) {
    $where[] = "igo.dtiniciofuncionamento <= TO_DATE('{$dtfinalfuncionamento} 23:59:59', 'DD/MM/YYYY HH24:MI:SS')";
}

if ($dtinicioprevisao && $dtfinalprevisao) {
    $where[] = "igo.dtiniciofuncionamento BETWEEN TO_DATE('{$dtinicioprevisao}', 'DD/MM/YYYY') AND TO_DATE('{$dtfinalprevisao} 23:59:59', 'DD/MM/YYYY HH24:MI:SS')";
} elseif ($dtinicioprevisao) {
    $where[] = "igo.dtiniciofuncionamento >= TO_DATE('{$dtinicioprevisao}', 'DD/MM/YYYY')";
} elseif ($dtfinalprevisao) {
    $where[] = "igo.dtiniciofuncionamento <= TO_DATE('{$dtfinalprevisao} 23:59:59', 'DD/MM/YYYY HH24:MI:SS')";
}

if ($distanciainicial && $distanciafinal) {
    $where[] = "igo.distancia BETWEEN {$distanciainicial} AND {$distanciafinal}";
} elseif ($distanciainicial) {
    $where[] = "igo.distancia >= {$distanciainicial}";
} elseif ($distanciafinal) {
    $where[] = "igo.distancia <= {$distanciafinal}";
}

if ($aeroportos) {
    $where[] = "igo.aeroportos ILIKE ('%{$aeroportos}%')";
}

$where[] = "obr.obridpai IS NULL";
$where[] = "obr.obrstatus = 'A'";

if (!empty($where)) {
    $where = " WHERE " . implode(' AND ', $where);
} else {
    $where = '';
}

$cabecalho = [
    'Obrid',
    'Nome da obra',
    'UF',
    'Município',
    'Tipologia',
    '% de execução',
    'Situação',
    'Última vistoria',
    'Capacidade',
    'Investimento total',
    'Início do funcionamento',
    'Previsão de inauguração',
    'Alunos matriculados',
    'Possui contrato',
    'Qtd. hab. do município',
    'Dist. da capital',
    'Aeroporto próximo',
    'Outras informações'
];

$sqlRelatorio = <<<DML
SELECT obr.obrid,
       obr.obrnome,
       mun.estuf,
       mun.mundescricao,
       tpo.tpodsc,
       obras2.calcula_percentual_execucao_obra(obr.obrid) AS percentual_execucao,
       esd.esddsc,
       TO_CHAR(obr.obrdtultvistoria, 'DD/MM/YYYY') AS dt_ultima_vistoria,
       igo.capacidade,
       igo.investimentototal,
       TO_CHAR(igo.dtiniciofuncionamento, 'DD/MM/YYYY') AS dtiniciofuncionamento,
       TO_CHAR(igo.dtprevisaoinauguracao, 'DD/MM/YYYY') AS dtprevisaoinauguracao,
       igo.alunosmatriculados,
       igo.statuspc,
       igo.quantidadehabitantes,
       igo.distancia AS distancia_da_capital,
       igo.aeroportos,
       igo.outras
  FROM obras2.obras obr
    INNER JOIN entidade.endereco edr USING(endid)
    INNER JOIN territorios.municipio mun USING(muncod)
    INNER JOIN territorios.mesoregiao mes USING(mescod)
    INNER JOIN obras2.tipologiaobra tpo USING(tpoid)
    INNER JOIN obras2.inauguracao_obra igo USING(obrid)
    INNER JOIN workflow.documento doc USING(docid)
    INNER JOIN workflow.estadodocumento esd USING(esdid)
    INNER JOIN obras2.empreendimento emp USING(empid)
    INNER JOIN obras2.programafonte prf USING(prfid)
  {$where}
DML;

if ('excel' == $req) {
    $db->sql_to_xml_excel($sqlRelatorio, 'relatorioListaObjetosObras', $cabecalho, '');
    die();
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
monta_titulo('Relatório de Funcionamento da Obra', '&nbsp;');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<form method="post" name="formFuncionamentoObra" id="formFuncionamentoObra">
	<input type="hidden" name="req" id="req" value="">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
            <td>
                <?=campo_texto('obrbuscatexto', 'N', 'S', '', 70, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Situação:
            </td>
            <td>
                <select name="esdid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php $situacaoRegistro = $db->carregar("SELECT esdid AS codigo, esddsc AS descricao FROM workflow.estadodocumento WHERE tpdid = 105");
                    foreach (is_array($situacaoRegistro)?$situacaoRegistro:[] as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($esdid) && in_array($key['codigo'], $esdid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
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
            <td class="td_municipio" id="td_municipio">
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
            <td class="SubTituloDireita">Esfera:</td>
            <td>
                <?php
                $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
                    Array('codigo' => 'M', 'descricao' => 'Municipal'));
                $db->monta_combo('empesfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'empesfera');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Mesoregião:</td>
            <td>
                <?php
                $sql = <<<DML
SELECT mescod AS codigo,
       mesdsc AS descricao
  FROM territorios.mesoregiao
  ORDER BY mesdsc
DML;
                $db->monta_combo('mescod', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'mescod');
                ?>
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
            <td class="subtitulodireita" width="250px">Funcionamento:</td>
            <td>
                <?php
                $dtiniciofuncionamento = !empty($dtiniciofuncionamento)
                    ?DateTime::createFromFormat('d/m/Y', $dtiniciofuncionamento)->format('Y-m-d')
                    :null;
                $dtfinalfuncionamento = !empty($dtfinalfuncionamento)
                    ?DateTime::createFromFormat('d/m/Y', $dtfinalfuncionamento)->format('Y-m-d')
                    :null;
                ?>
                Entre <?=campo_data2('dtiniciofuncionamento', 'N', 'S', '', 'S', '', '', $dtiniciofuncionamento, ''); ?>
                e
                <?=campo_data2('dtfinalfuncionamento', 'N', 'S', '', 'S', '', '', $dtfinalfuncionamento, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Previsão de Inauguração:</td>
            <td>
                <?php
                $dtinicioprevisao = !empty($dtinicioprevisao)
                    ?DateTime::createFromFormat('d/m/Y', $dtinicioprevisao)->format('Y-m-d')
                    :null;
                $dtfinalprevisao = !empty($dtfinalprevisao)
                    ?DateTime::createFromFormat('d/m/Y', $dtfinalprevisao)->format('Y-m-d')
                    :null;
                ?>
                Entre <?=campo_data2('dtinicioprevisao', 'N', 'S', '', 'S', '', ' ', $dtinicioprevisao, ''); ?>
                e
                <?=campo_data2('dtfinalprevisao', 'N', 'S', '', 'S', '', ' ', $dtfinalprevisao, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Proximidade da capital:</td>
            <td>
                entre <?=campo_texto('distanciainicial', 'N', 'S', '', 20, 20, '[.###]', '', 'right', '', 0, 'id="distanciainicial"','', (int)$distanciainicial, '', array('width' => '10%')); ?>
                e <?=campo_texto('distanciafinal', 'N', 'S', '', 20, 20, '[.###]', '', 'right', '', 0, 'id="distanciafinal"','', (int)$distanciafinal, '', array('width' => '10%')); ?> KM
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="250px">Aeroportos mais próximos:</td>
            <td>
                <?=campo_texto('aeroportos', 'N', 'S', '', 10, 250, '', '', 'left', '', 0, 'id="aeroportos"','',$aeroportos, '', array('width' => '40%')); ?>
            </td>
        </tr>
        <tr>
            <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
                <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar" />
                <input type="button" name="btnEexcel" class="btnEexcel" value="Gerar Excel" />
                <input id="button_limpar" type="button" value="Limpar Filtros" />
            </td>
        </tr>
    </table>
</form>
<script type="text/javascript">
$1_11(function(){
    $1_11('#formFuncionamentoObra select').chosen();

    $1_11(".estados").chosen().change(function (e, params) {
        values = $1_11(".estados").chosen().val();
        carregarMunicipio(values);
    });

	$1_11('.pesquisar').click(function (){
		$1_11('#req').val('');
		$1_11('#formFuncionamentoObra').submit();
	});
    $1_11('#button_limpar').click(function() {
        $1_11('#req').val('limpar');
        $1_11('#formFuncionamentoObra').submit();
    });
    $1_11('.btnEexcel').click(function () {
        $1_11('#req').val('excel');
        $1_11('#formFuncionamentoObra').submit();
    });
});

function carregarMunicipio(estuf)
{
	var td	= $1_11('#td_municipio');
	if (estuf != '') {
		var url = location.href;
		$1_11.ajax({
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
	} else {
		td.find('select option:first').attr('selected', true);
		td.find('select').attr('selected', true).attr('disabled', true);
	}
}
</script>
<?php
$db->monta_lista($sqlRelatorio, $cabecalho, 100, 5, 'N', 'center', 'N');
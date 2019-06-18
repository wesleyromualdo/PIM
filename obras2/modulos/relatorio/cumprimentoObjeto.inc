<?php
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

if ($_REQUEST['esdid'] && $_REQUEST['esdid'][0] != '') {
    $where['esdid'] = "doc.esdid IN(" . implode(',', $_REQUEST['esdid']) . ")";
}
if ($_REQUEST['stcoid'] && $_REQUEST['stcoid'][0] != '') {
    $where['stcoid'] = "docc.esdid IN(" . implode(',', $_REQUEST['stcoid']) . ")";
}
if ($_REQUEST['prfid'] && $_REQUEST['prfid'][0] != '') {
    $where['prfid'] = "emp.prfid IN(" . implode(',', $_REQUEST['prfid']) . ")";
}
if ($_REQUEST['tooid'] && $_REQUEST['tooid'][0] != '') {
    $where['tooid'] = "obr.tooid IN(" . implode(',', $_REQUEST['tooid']) . ")";
}
if ($_REQUEST['estuf'] && $_REQUEST['estuf'][0] != '') {
    $where['estuf'] = "mun.estuf IN ('" . implode("', '", $_REQUEST['estuf']) . "')";
}
if ($_REQUEST['muncod'] && $_REQUEST['muncod'][0] != '') {
    $where['muncod'] = "mun.muncod IN ('" . implode("', '", $_REQUEST['muncod']) . "')";
}
if ($_REQUEST['empesfera']) {
    $where['empesfera'] = "emp.empesfera IN('" . $_REQUEST['empesfera'] . "')";
}
if ($_REQUEST['convenio']) {
    $where['convenio'] = "p_conv.termo_convenio = '{$_REQUEST['convenio']}'";
}
if ($_REQUEST['ano_convenio']) {
    $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$_REQUEST['ano_convenio']}'";
}
if ($_REQUEST['processo']) {
    $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$_REQUEST['processo']}','.',''),'/',''),'-','')";
}
if ($_REQUEST['ano_processo']) {
    $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$_REQUEST['ano_processo']}'";
}

if (!empty($where)) {
    $where = " AND " . implode(' AND ', $where);
} else {
    $where = '';
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
monta_titulo('Relatório de Cumprimento do Objeto', '&nbsp;');

$cabecalho = [
    'ID Cumprimento Objeto',
    'ID Obra',
    'Obra',
    'UF',
    'Município',
    'Situação da Obra',
    'Número do Processo',
    'Situação do Cumprimento do Objeto',
    'Dt. Ult. Tramitação',
    'Dt da Liberação',
    'Dt de Leitura',
    'Usuário'
];

$relatorio = <<<DML
SELECT cob.coid AS id_cumprimento_objeto,
       obr.obrid AS id_obra,
       obr.obrnome AS obra,
       mun.estuf,
       mun.mundescricao,
       esd.esddsc AS situacao_obra,
       p_conv.pronumeroprocesso AS numero_processo,
       esdc.esddsc AS situacao_cumprimento_objeto,
       TO_CHAR(COALESCE(hst.htddata, docc.docdatainclusao), 'DD/MM/YYYY') AS data_ultima_tramitacao,
       TO_CHAR(cco.dtcriacao, 'DD/MM/YYYY') AS data_liberacao,
       TO_CHAR(alt.dtacao, 'DD/MM/YYYY') AS data_leitura,
       usunome AS usuario
  FROM obras2.obras obr
    JOIN obras2.controle_cumprimento_objeto cco ON (cco.ccostatus = 'A' AND cco.obrid = obr.obrid)
    JOIN obras2.alerta_obra ao ON (ao.obrid = obr.obrid AND aid = 1)
    LEFT JOIN (SELECT MAX(dtacao) dtacao,
                      MAX(aolid) aolid,
                      aoid
                 FROM obras2.alerta_obra_lido
                 WHERE acao = 'C'
                   AND aid = 1
                 GROUP BY aoid) alt ON (alt.aoid = ao.aoid)
    LEFT JOIN obras2.alerta_obra_lido ol ON (ol.aolid = alt.aolid)
    LEFT JOIN seguranca.usuario u ON (u.usucpf = ol.usucpf)
    INNER JOIN obras2.cumprimento_objeto cob ON (obr.obrid = cob.obrid)
    INNER JOIN entidade.endereco edr USING(endid)
    INNER JOIN territorios.municipio mun ON (edr.muncod = mun.muncod)
    INNER JOIN workflow.documento doc ON (obr.docid = doc.docid)
    INNER JOIN workflow.estadodocumento esd USING(esdid)
    LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = obr.obrid
    INNER JOIN workflow.documento docc ON (cob.docid = docc.docid)
    INNER JOIN workflow.estadodocumento esdc ON (docc.esdid = esdc.esdid)
    LEFT JOIN workflow.historicodocumento hst ON (docc.hstid = hst.hstid)
    INNER JOIN obras2.empreendimento emp USING(empid)
  WHERE obr.obridpai IS NULL
    AND obr.obrstatus = 'A'
    {$where}
DML;
?>
<form method="post" name="formListaObraCumprimento" id="formListaObraCumprimento">
	<input type="hidden" name="req" id="req" value="">
	<input type="hidden" name="obrid" id="obrid" value="">
	<input type="hidden" name="empid" id="empid" value="">

        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
                <td>
                    <?= campo_texto('obrbuscatexto', 'N', 'S', '', 70, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
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
                    Situação do Cumprimento:
                </td>
                <td>
                    <select name="stcoid[]" class="chosen-select" multiple data-placeholder="Selecione">
                        <?php
                        $sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 200 and esdstatus = 'A' order by esdordem";
                        $dados = $db->carregar($sql);

                        foreach ($dados as $key) {
                            ?>
                            <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($stcoid) && in_array($key['codigo'], $stcoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
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
			<td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
				<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                <input id="button_limpar" type="button" value="Limpar Filtros"/>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
$1_11(document).ready(function () {
    $1_11('select[name="strid[]"], #empesfera').chosen();
    $1_11('select[name="stcoid[]"]').chosen();
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
        console.log(values);
        carregarMunicipio(values);
    });

	$1_11('.pesquisar').click(function (){
		$1_11('#req').val('');
		$1_11('#formListaObraCumprimento').submit();
	});
    $1_11('#button_limpar').click(function() {
        $1_11('#req').val('limpar');
        $1_11('#formListaObraCumprimento').submit();
    });
});

function carregarMunicipio( estuf ){
	var td	= $1_11('#td_municipio');
	if ( estuf != '' ){
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
	}else{
		td.find('select option:first').attr('selected', true);
		td.find('select').attr('selected', true).attr('disabled', true);
	}
}
</script>
<?php
$db->monta_lista($relatorio, $cabecalho, 100, 5, 'N', 'center', 'N');

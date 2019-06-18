<?php
// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once "_constantes.php";
include_once "_funcoes.php";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();
echo monta_cabecalho_relatorio('100');


$coluna = 'h.htddata';
if($_REQUEST['dt_analise'] != 'fnde'){
    $coluna = 'dt_fim_vigencia_termo';
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>

<script src="../library/multiple-select/jquery.multiple.select.js" type="text/javascript"></script>
<link href="../library/multiple-select/multiple-select.css" rel="stylesheet"/>

<style>
	.subtituloDireita{
		padding-right: 5px;
	}

	.tabela{
		width: 100% !important;
		margin-top: 10px !important;
	}

</style>

<script type="text/javascript">
    $(function(){
        $('.item_situacao').change(function(){
            $('#filtro').val($('.item_situacao:checked').val());
        });
    });
	function pegaItem(val) {
		$('#rstitem').val(val);
		$('#formPainel').submit();
	}

	function pegaTipo(val) {
		$('#tprid').val(val);
		$('#formPainel').submit();
	}

	function pegaSituacao(val) {
		$('#filtro').val(val);
		$('#formPainel').submit();
	}

	function pegaUf(val) {
		$('#estuf').val(val);
		$('#formPainel').submit();
	}

	function pegaTipologia(val) {
		$('#tpoid').val(val);
		$('#formPainel').submit();
	}
</script>

<form id="formPainel" name="formPainel" method="GET">
    <input type="hidden" name="gerar_xls" id="gerar_xls"/>
	<table border="0" cellpadding="0" cellspacing="0" class="tabela" >
        <tr>

            <td class="subtituloDireita">UF:</td>
            <td style="padding-left: 5px;">
                <?php
                $val = (!empty($_REQUEST['estuf'])) ? $_REQUEST['estuf'] : '';

                $sql = " SELECT estuf AS codigo, estdescricao AS descricao FROM territorios.estado ORDER BY estuf";
                $db->monta_combo("estuf", $sql, "S", "Todos", "", "", "", "150", "N", "estuf", false, $val);
                ?>
            </td>

            <td class="subtituloDireita">Situação:</td>
            <td>
                <?php
                $item_situacao_verm = '';
                $item_situacao_amar = '';
                $item_situacao_verd = '';
                $item_sitaucao_preto = '';
                $item_situacao_todas = '';
                switch ($_REQUEST['filtro']){
                    case 'R':
                        $item_situacao_verm = 'checked="checked"';
                        break;
                    case 'A':
                        $item_situacao_amar = 'checked="checked"';
                        break;
                    case 'V':
                        $item_situacao_verd = 'checked="checked"';
                        break;
                    case 'T':
                        $item_situacao_todas = 'checked="checked"';
                        break;
                    case 'P':
                        $item_sitaucao_preto = 'checked="checked"';
                        break;
                    default:
                        $item_situacao_todas = 'checked="checked"';
                        break;
                }
                ?>
                <input type="hidden" name="filtro" id="filtro" value="<?php echo (!empty($_REQUEST['filtro'])) ? $_REQUEST['filtro'] : ($_REQUEST['item_situacao'] ? $_REQUEST['item_situacao'] : 'T') ?>" />
                <input type="radio" name="item_situacao" class="item_situacao" id="item_situacao_verm" value="R" <?php echo $item_situacao_verm; ?> > Vermelha
                <input type="radio" name="item_situacao" class="item_situacao" id="item_situacao_amar" value="A" <?php echo $item_situacao_amar; ?> > Amarela
                <input type="radio" name="item_situacao" class="item_situacao" id="item_situacao_verd" value="V" <?php echo $item_situacao_verd; ?> > Verde
                <input type="radio" name="item_sitaucao" class="item_sitaucao" id="item_sitaucao_preto" value="P" <?php echo $item_sitaucao_preto; ?> > Preto
                <input type="radio" name="item_situacao" class="item_situacao" id="item_situacao_todas" value="T" <?php echo $item_situacao_todas; ?> > Todas
            </td>

            <td class="subtituloDireita">Tipologia da Obra:</td>
            <td style="padding-left: 5px;">
            <?php
                $sql = "SELECT tpoid AS codigo, tpodsc AS descricao FROM obras2.tipologiaobra WHERE tpostatus = 'A' AND orgid = 3 ORDER BY tpodsc";
                $db->monta_combo_multiplo("tpoid", $sql, "S",'','','', '', 1, '150',array(), "tpoid", false, null);
                ?>
                <script type="text/javascript">
                    var t = '<?php echo !empty($_REQUEST['tpoid']) ? is_array($_REQUEST['tpoid']) ? implode(",", $_REQUEST['tpoid']) : $_REQUEST['tpoid'] : ''; ?>';
                    var a = t.split(",");

                    $('#tpoid').multipleSelect({
                        filter: true,
                        placeholder: 'Todos',
                        selectAllText: 'Selecione todos',
                        allSelected: 'Todos selecionados',
                        minimumCountSelected: 2,
                        countSelected: 'Selecionados, # de %'
                    });

                    $('#tpoid').removeAttr('class');
                    $('.ms-parent').attr('class', 'ms-parent');
                    $('#tpoid').multipleSelect('setSelects', a);
                </script>
            </td>

            
        </tr>

	</table>
</form>
	<?php
	$db = new cls_banco();

	if ($_REQUEST['estuf'] != ''){
		$estado = " AND mun.estuf = '{$_REQUEST['estuf']}'";
	}

	if ($_REQUEST['tpoid'] && $_REQUEST['tpoid'] != '' && $_REQUEST['tpoid'] != 'null'){
		if ( is_array($_REQUEST['tpoid']) ){
			$tipologia = " AND o.tpoid in (".implode(",", $_REQUEST['tpoid']).")";
		} else {
			$tipologia = " AND o.tpoid in (".$_REQUEST['tpoid'].")";
		}
	}

	if ($_REQUEST['filtro']) {
		switch ($_REQUEST['filtro']) {
			case 'R':
				$filtro = "AND (DATE_PART('days', NOW() - $coluna) > 60 AND DATE_PART('days', NOW() - $coluna) <= 90) ";
				break;
			case 'A':
				$filtro = "AND (DATE_PART('days', NOW() - $coluna) >= 30 AND DATE_PART('days', NOW() - $coluna) <= 60) ";
				break;
			case 'V':
				$filtro = "AND DATE_PART('days', NOW() - $coluna ) < 30 ";
				break;
            case 'P':
                $filtro = "AND DATE_PART('days', NOW() - $coluna ) > 90 ";
                break;
		}
	}
    $acao = "'<center>
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: alterarObra('|| o.obrid ||');\"
                        title=\"Alterar Obra\">
                 </center>' AS acao,";
    $acao = ($_REQUEST['gerar_xls'] == 'xls') ? '' : $acao;

    $sql = "
        SELECT * FROM (
            SELECT
                $acao
                o.obrid,
                o.obrnome,
                mun.estuf,
                mun.mundescricao,
                pf.prfdesc programa,
                too.toodescricao fonte,
                tpo.tpodsc,
                TO_CHAR(h.htddata, 'DD/MM/YYYY') htddata,
                TO_CHAR(dt_fim_vigencia_termo, 'DD/MM/YYYY') dt_fim_vigencia_termo

            FROM obras2.cumprimento_objeto co
            JOIN obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid)
            JOIN obras2.obras o ON (co.obrid = o.obrid) AND o.obridpai IS NULL AND o.obrstatus = 'A'
            JOIN obras2.empreendimento emp ON emp.empid = o.empid
            JOIN workflow.documento d ON (co.docid = d.docid AND d.esdid IN(1268))
            JOIN workflow.estadodocumento esd ON(d.esdid = esd.esdid)
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN territorios.municipio mun ON mun.muncod = ed.muncod
            LEFT JOIN obras2.programafonte pf                    ON pf.prfid = emp.prfid
            LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
            LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
            LEFT JOIN (select obrid, MAX(dt_fim_vigencia_termo) dt_fim_vigencia_termo from obras2.vm_vigencia_obra_2016 GROUP BY obrid) v ON v.obrid = o.obrid
            LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid AND h.aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = d.esdid AND aedstatus = 'A')
            RIGHT JOIN territorios.estado est ON est.estuf = mun.estuf
            WHERE co.status = 'A'

            $estado
            $tipologia
            $filtro

            ) as f ORDER BY 1
        ";

    $cabecalho = array('Ação', 'ID Obra', 'Obra', 'UF', 'Município','Programa', 'Fonte', 'Tipologia', 'Dt Envio FNDE', 'Dt fim vigenêcia termo');

    if($_REQUEST['gerar_xls'] == 'xls'){
        unset($cabecalho[0]);
        $db->sql_to_xml_excel($sql, 'relatorio', $cabecalho, '');
    } else {
        $db->monta_lista($sql, $cabecalho, 100, 10, 'N','center', 'N');
    }
	?>

<script>
    function abreSolicitacao(slcid, obrid, tslid){
        windowOpen('/obras2/obras2.php?modulo=principal/solicitacao&acao=A&slcid=' + slcid + '&obrid=' + obrid + '&tslid[]=' + tslid,'telaSolicitacaoDesembolso','height=700,width=1200,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }

    function alterarObra(obrid, empid){
        var url = 'obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid;
        janela = window.open(url, 'inserirObra', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' );
        janela.focus();
    }


</script>


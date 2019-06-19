<?php
// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once "_constantes.php";
include_once "_funcoes.php";

$pagamento = $_GET['pagamento'];

// abre conexão com o servidor de banco de dados
$db = new cls_banco();
echo monta_cabecalho_relatorio('100');
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
    <? if($pagamento)?>
    <input type="hidden" name="pagamento" id="pagamento" value="true"/>
    <? if($pagamento)?>
	<table border="0" cellpadding="0" cellspacing="0" class="tabela" >
        <tr>
			<td class="subtituloDireita">Situação da Solicitação</td>
			<td style="padding-left: 5px;">
                <?php
                $val = (!empty($_REQUEST['esdid'])) ? $_REQUEST['esdid'] : '';

                $sqlPagamento = ($pagamento) ? ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO : ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA;

                $sql = " SELECT esdid as codigo, esddsc as descricao
                    FROM workflow.estadodocumento
                    WHERE esdid IN ($sqlPagamento)";
                $db->monta_combo("esdid", $sql, "S", "Todos", "", "", "", "200", "N", "esdid", false, $val, '', '', 'chosen');
                ?>
			</td>


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
			
			<td><input type="submit" name="pesquisar" class="pesquisar" value="Pesquisar"/> <input type="submit" name="xls" class="xls" value="Gerar Excel"/></td>

            <script type="text/javascript">
                $(function(){
                    $('input[name=xls]').click(function(e){
                        e.preventDefault();
                        $('#gerar_xls').val('xls');
                        $('#formPainel').submit();
                    });
                });
            </script>

        </tr>
	</table>
</form>
	<?php
	$db = new cls_banco();
    if ($_REQUEST['esdid']) {
        $item = " AND d.esdid = '{$_REQUEST['esdid']}' ";
    }

	if ($_REQUEST['estuf'] != '') {
		$estado = " AND m.estuf = '{$_REQUEST['estuf']}'";
	}

	if ($_REQUEST['tpoid'] && $_REQUEST['tpoid'] != '' && $_REQUEST['tpoid'] != 'null') {
		if ( is_array($_REQUEST['tpoid']) ) {
			$tipologia = " AND o.tpoid in (".implode(",", $_REQUEST['tpoid']).")";
		} else {
			$tipologia = " AND o.tpoid in (".$_REQUEST['tpoid'].")";
		}
	}
	
	if ($_REQUEST['filtro']) {
		switch ($_REQUEST['filtro']) {
			case 'R':
				$filtro = " AND (DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao)) > 10 AND DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao)) <= 30)";
				break;
			case 'A':
				$filtro = " AND (DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao)) >= 5 AND DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao)) <= 10) ";
				break;
			case 'V':
				$filtro = " AND DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao) ) < 5 ";
				break;
            case 'P':
                $filtro = " AND DATE_PART('days', NOW() - COALESCE(h.htddata, d.docdatainclusao) ) > 30 ";
                break;
		}
	}
    $sqlPagamentoFeito = ($pagamento) ? 'AND sv.sldid NOT IN (SELECT sldid FROM obras2.v_solicitacao_pagamento)' : '';

    $acao = "'<center>
                <img
                    align=\"absmiddle\"
                    src=\"/imagens/consultar.gif\"
                    style=\"cursor: pointer\"
                    onclick=\"javascript: abreSolicitacao(' || sv.sldid  || ');\"
                    title=\"Alterar Solicitação\">
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
            sv.sldid,
            o.obrid,
            o.obrnome,
            m.estuf,
            m.mundescricao,
            sv.sldjustificativa,
            u.usunome usunome1,
            TO_CHAR(sv.slddatainclusao, 'DD/MM/YYYY') slddatainclusao,
            e.esddsc,
            (
                SELECT ud.usunome FROM workflow.historicodocumento h
                LEFT JOIN seguranca.usuario ud ON ud.usucpf = h.usucpf
                WHERE h.docid = d.docid ORDER BY h.htddata DESC LIMIT 1
            ) as usunome,
            (
                SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento h WHERE h.docid = d.docid ORDER BY h.htddata DESC LIMIT 1
            ) as htddata,
            (
                SELECT c.cmddsc FROM workflow.historicodocumento h
				LEFT JOIN workflow.comentariodocumento  c ON c.hstid = h.hstid 
				WHERE h.hstid = (SELECT hstid FROM workflow.historicodocumento h WHERE h.docid = d.docid ORDER BY h.htddata DESC LIMIT 1)
				ORDER BY h.htddata DESC LIMIT 1
            ) as cmddsc
        FROM obras2.solicitacao_desembolso sv
        JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
        JOIN obras2.empreendimento emp ON emp.empid = o.empid
        JOIN entidade.endereco ed ON ed.endid = o.endid
        JOIN territorios.municipio m ON m.muncod = ed.muncod
        JOIN seguranca.usuario u ON u.usucpf = sv.usucpf
        JOIN workflow.documento d ON d.docid = sv.docid AND d.esdid IN ($sqlPagamento)
        JOIN workflow.estadodocumento e ON e.esdid = d.esdid
        LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid AND h.aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = d.esdid AND aedstatus = 'A')
        WHERE sv.sldstatus = 'A'
            $estado
            $tipologia
            $filtro
            $item
            $sqlPagamentoFeito
        ) as f ORDER BY 1
        ";

    $cabecalho = array('Ação', 'ID Solicitação', 'ID Obra', 'Obra', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Data de Cadastro',
        'Situação do Deferimento', 'Inserido Por', 'Data da Resposta', 'Observação da Resposta');

    if($_REQUEST['gerar_xls'] == 'xls'){
        unset($cabecalho[0]);
        $db->sql_to_xml_excel($sql, 'relatorio', $cabecalho, '');
    } else {
        $db->monta_lista($sql, $cabecalho, 100, 10, 'N','center', 'N');
    }
	?>

<script>
    function abreSolicitacao(sldid){
        windowOpen('/obras2/obras2.php?modulo=principal/popupSolicitarDesembolso&acao=A&sldid=' + sldid,'telaSolicitacaoDesembolso','height=700,width=1200,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }

    function alterarObra(obrid, empid){
        var url = 'obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid;
        janela = window.open(url, 'inserirObra', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' );
        janela.focus();
    }


</script>


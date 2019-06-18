<?php

// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once "_constantes.php";
include_once "_funcoes.php";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//verificaSessao( 'obra' );

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

			
			<td class="subtituloDireita">Tipo de Diligência:</td>
			<td style="padding-left: 5px;">
	                <?php
	                $val = (!empty($_REQUEST['tpdid'])) ? $_REQUEST['tpdid'] : '';
	
	                $sql = " SELECT tpdid as codigo, tpddsc as descricao 
	                         FROM obras2.tipodiligencia
	                         WHERE tpdstatus = 'A' 
	                         ORDER BY tpdid";
	                $db->monta_combo("tpdid", $sql, "S", "Todos", "", "", "", "200", "N", "tpdid", false, $val);
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
					$item_sitaucao_verm = '';
					$item_sitaucao_amar = '';
					$item_sitaucao_verd = '';
					$item_sitaucao_preto = '';
					$item_sitaucao_todas = '';
					switch ($_REQUEST['filtro']){
						case 'R':
							$item_sitaucao_verm = 'checked="checked"';
							break;
						case 'A':
							$item_sitaucao_amar = 'checked="checked"';
							break;
						case 'V':
							$item_sitaucao_verd = 'checked="checked"';
							break;
						case 'T':
							$item_sitaucao_todas = 'checked="checked"';
							break;
                        case 'P':
							$item_sitaucao_preto = 'checked="checked"';
							break;
						default:
							$item_sitaucao_todas = 'checked="checked"';
							break;
					}
				?>
	                <input type="hidden" name="filtro" id="filtro" value="<?php echo (!empty($_REQUEST['situacao'])) ? $_REQUEST['situacao'] : 'T' ?>" />
	                <input type="radio" name="item_sitaucao" class="item_sitaucao" id="item_sitaucao_verm" value="R" <?php echo $item_sitaucao_verm; ?> > Vermelho
	                <input type="radio" name="item_sitaucao" class="item_sitaucao" id="item_sitaucao_amar" value="A" <?php echo $item_sitaucao_amar; ?> > Amarelo
	                <input type="radio" name="item_sitaucao" class="item_sitaucao" id="item_sitaucao_verd" value="V" <?php echo $item_sitaucao_verd; ?> > Verde
	                <input type="radio" name="item_sitaucao" class="item_sitaucao" id="item_sitaucao_verd" value="P" <?php echo $item_sitaucao_preto; ?> > Preto
	                <input type="radio" name="item_sitaucao" class="item_sitaucao" id="item_sitaucao_todas" value="T" <?php echo $item_sitaucao_todas; ?> > Todos
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
	
	$cabecalho = array(
			'Ação',
			'ID Obra',
			'ID Item',
			'Andamento',
			'Estado',
			'Município',
			'Esfera',
			'Fase',
			'Tipo',
			'Nome da Obra',
			'Situação da Obra',
			'Data de Cadastro',
			'Criado por',
			'Previsão da Providência',
			'Superação',
			'Superado por',
			'Último Tramite',
			'Data do Último Tramite',
			'Responsável'
	);
	
	if ($_REQUEST['estuf'] != ''){
		$estado = " AND uf.estuf = '{$_REQUEST['estuf']}'";
	}
	
	if ($_REQUEST['tpdid'] && $_REQUEST['tpdid'] != ''){
		$tipo = " AND td.tpdid = {$_REQUEST['tpdid']} ";
	}
	if ($_REQUEST['tpoid'] && $_REQUEST['tpoid'] != '' && $_REQUEST['tpoid'] != 'null'){
		if ( is_array($_REQUEST['tpoid']) ){
			$tipologia = " AND obr.tpoid in (".implode(",", $_REQUEST['tpoid']).")";
		} else {
			$tipologia = " AND obr.tpoid in (".$_REQUEST['tpoid'].")";
		}
	}
	
	if ($_REQUEST['filtro']) {
		switch ($_REQUEST['filtro']) {
			case 'R':
				$filtro = " AND (DATE_PART('days', NOW() - htd_ri.htddata) > 10 AND DATE_PART('days', NOW() - htd_ri.htddata) <= 30)";
				break;
			case 'A':
				$filtro = " AND (DATE_PART('days', NOW() - htd_ri.htddata) >= 5 AND DATE_PART('days', NOW() - htd_ri.htddata) <= 10)";
				break;
			case 'V':
				$filtro = " AND DATE_PART('days', NOW() - htd_ri.htddata) < 5";
				break;
            case 'P':
				$filtro = " AND DATE_PART('days', NOW() - htd_ri.htddata) > 30 ";
				break;
		}
	}
	
	$acao = "'<center><div style=\"width:50px;\">
	                            <img
	                                    align=\"absmiddle\"
	                                    src=\"/imagens/alterar.gif\"
	                                    style=\"cursor: pointer\"
	                                    onclick=\"javascript: alterarRestricao(' || dlg.dlgid || ', '|| obr.obrid ||', '|| obr.empid ||' );\"
	                                    title=\"Alterar\">
	
	                            <img
	                                    align=\"absmiddle\"
	                                    src=\"/imagens/consultar.gif\"
	                                    style=\"cursor: pointer\"
	                                    onclick=\"javascript: alterarObra(' || dlg.dlgid || ', '|| obr.obrid ||', '|| obr.empid ||');\"
	                                    title=\"Alterar\">
	                     </center></div>'  as acao,";

    if(($_REQUEST['gerar_xls'] != 'xls')) {
        $andamento = "CASE
                                    WHEN dlg.docid IS NULL
                                    THEN '<img
                                            align=\"absmiddle\"
                                            src=\"/imagens/obras/atencao.png\"
                                            style=\"cursor: pointer; width:15px;\"
                                            onclick=\"cadastroNovoDocid('|| dlg.dlgid ||')\"
                                            title=\"Restrição/Inconformidade sem Fluxo cadastrado. Clique aqui para cadastrar.\">&nbsp;&nbsp;'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA . "
                                    THEN '<img
                                            align=\"absmiddle\"
                                            src=\"/imagens/0_alerta.png\"
                                            style=\"cursor: pointer\"
                                            title=\"Aguardando Providência\">&nbsp;&nbsp;'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_ANALISE_FNDE . "
                                    THEN '<img
                                            align=\"absmiddle\"
                                            src=\"/imagens/0_ativo.png\"
                                            style=\"cursor: pointer\"
                                            title=\"Aguardando Análise FNDE\">&nbsp;&nbsp;'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_SUPERADA . "
                                    THEN '<img
                                            align=\"absmiddle\"
                                            src=\"/imagens/0_concluido.png\"
                                            style=\"cursor: pointer\"
                                            title=\"Superada\">&nbsp;&nbsp;'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_CANCELADA . "
                                    THEN '<img
                                            align=\"absmiddle\"
                                            src=\"/imagens/0_inexistente.png\"
                                            style=\"cursor: pointer\"
                                            title=\"Cancelada\">&nbsp;&nbsp;'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . "
                                    THEN '<img
                                            align=\"absmiddle\"
                                            src=\"/imagens/0_inativo.png\"
                                            style=\"cursor: pointer\"
                                            title=\"Aguardando Correção\">&nbsp;&nbsp;'
                                    ELSE
                                         '<img
                                            align=\"absmiddle\"
                                            src=\"/imagens/0_inexistente.png\"
                                            style=\"cursor: pointer\"
                                            title=\"Cancelada\">&nbsp;&nbsp;'

                                END
                                ||
                                CASE
                                    WHEN dlg.dlgdtprevisaoregularizacao < NOW() AND dlg.dlgdtsuperacao IS NULL
                                    THEN '<img
                                            align=\"absmiddle\"
                                            src=\"/imagens/0_inativo.png\"
                                            style=\"cursor: pointer\"
                                            title=\"Data prevista ultrapassada \">'
                                    ELSE ''

                                END  AS andamento,  ";
    } else {
        $andamento = "CASE
                                    WHEN dlg.docid IS NULL
                                    THEN 'diligência sem Fluxo cadastrado'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA . "
                                    THEN 'Aguardando Providência'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_ANALISE_FNDE . "
                                    THEN 'Aguardando Análise FNDE'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_SUPERADA . "
                                    THEN 'Superada'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_CANCELADA . "
                                    THEN 'Cancelada'
                                    WHEN esd_ri.esdid = " . ESDID_DILIGENCIA_AGUARDANDO_CORRECAO . "
                                    THEN 'Aguardando Correção'
                                    ELSE
                                         'Cancelada'
                                END
                                AS andamento,  ";
    }
	$acao = ($_REQUEST['gerar_xls'] == 'xls') ? '' : $acao;

	$sql = "SELECT
				{$acao}
				obr.obrid AS id_obra,
				dlg.dlgid AS id_item,
			    {$andamento}
				uf.estuf AS estado,
				mun.mundescricao AS municipio,
				CASE
					WHEN ep.empesfera = 'M' THEN 'Municipal'
					WHEN ep.empesfera = 'E' THEN 'Estadual'
					WHEN ep.empesfera = 'F' THEN 'Federal'
					ELSE ''
				END as esfera,
				CASE 
					WHEN dlg.dlgid IS NOT NULL THEN fd.fsddsc 
					ELSE 'Não Informada' 
				END AS fase,
				td.tpddsc AS tipo2,
				'(' || obr.obrid || ') ' || obr.obrnome  as nome_obra,
				esd_obr.esddsc,
				TO_CHAR(dlg.dlgdtinclusao, 'DD/MM/YYYY') as data_cadastro,
				usu.usunome as usucriacao,
				TO_CHAR(dlg.dlgdtprevisaoregularizacao, 'DD/MM/YYYY') AS previsao_providencia_dt,
				CASE 
					WHEN dlg.dlgsituacao = TRUE THEN TO_CHAR(dlg.dlgdtsuperacao, 'DD/MM/YYYY') 
					ELSE 'Não' 
				END AS superacao,
				sup.usunome as ususuperacao,
				usuh.usunome,
				TO_CHAR(h.htddata, 'DD/MM/YYYY') as htddata,
				array_to_string(array_agg(usufr.usunome), ', ') AS responsavel
			FROM obras2.diligencia dlg 
			LEFT JOIN obras2.obras obr ON obr.obrid = dlg.obrid AND obr.obridpai IS NULL AND obr.obrstatus = 'A'
			INNER JOIN obras2.empreendimento ep ON ep.empid = obr.empid
			LEFT JOIN workflow.documento doc_ri ON doc_ri.docid = dlg.docid
			LEFT JOIN workflow.estadodocumento esd_ri ON esd_ri.esdid = doc_ri.esdid

			LEFT JOIN workflow.historicodocumento htd_ri
				ON htd_ri.docid = doc_ri.docid
				AND htd_ri.aedid IN (SELECT aedid FROM workflow.acaoestadodoc  WHERE esdiddestino = esd_ri.esdid AND aedstatus = 'A')
				AND htd_ri.htddata = (SELECT MAX(h.htddata) FROM workflow.historicodocumento h WHERE h.docid = doc_ri.docid AND h.aedid IN (SELECT aedid FROM workflow.acaoestadodoc  WHERE esdiddestino = esd_ri.esdid AND aedstatus = 'A'))

			LEFT JOIN workflow.documento doc_obr ON doc_obr.docid = obr.docid
			LEFT JOIN workflow.estadodocumento esd_obr ON esd_obr.esdid = doc_obr.esdid 
			LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid 
			LEFT JOIN obras2.tipoorigemobra too ON too.tooid = obr.tooid 
			LEFT JOIN obras2.tipodiligencia td ON td.tpdid = dlg.tpdid AND td.tpdstatus = 'A' 
			LEFT JOIN obras2.fasediligencia fd ON fd.fsdid = dlg.fsdid AND fd.fsdstatus = 'A' 
			LEFT JOIN obras2.tipologiaobra tpo ON tpo.tpoid = obr.tpoid 
			LEFT JOIN entidade.endereco ende ON ende.endid = obr.endid AND ende.endstatus = 'A'
			LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod 
			LEFT JOIN territorios.estado uf ON mun.estuf = uf.estuf 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = dlg.usucpf 
			LEFT JOIN seguranca.usuario sup ON sup.usucpf = dlg.usucpfsuperacao 
			LEFT JOIN workflow.historicodocumento h ON doc_ri.hstid = h.hstid 
			LEFT JOIN seguranca.usuario usuh ON usuh.usucpf = h.usucpf 
			LEFT JOIN obras2.usuariofndediligencia ufd ON ufd.estuf = uf.estuf
			LEFT JOIN seguranca.usuario usufr ON usufr.usucpf = ufd.usucpf
			WHERE dlg.dlgstatus = 'A' 
			AND esd_ri.esdid  = ".ESDID_DILIGENCIA_AGUARDANDO_ANALISE_FNDE." 
			{$estado}
			{$item} 
			{$tipo}
			{$filtro}
			{$tipologia}
			GROUP BY obr.empid, obr.obrid, dlg.dlgid, td.tpddsc, uf.estuf, mun.mundescricao, dlg.fsdid, fd.fsddsc, 
			 dlg.docid, esd_ri.esdid, dlg.dlgdtprevisaoregularizacao, dlg.dlgdtsuperacao, ep.empesfera,
			 obr.obrnome, esd_obr.esddsc, dlg.dlgdtinclusao, usu.usunome, dlg.dlgsituacao, sup.usunome, usuh.usunome, h.htddata";

    if($_REQUEST['gerar_xls'] == 'xls'){
        unset($cabecalho[0]);
        $db->sql_to_xml_excel($sql, 'relatorio', $cabecalho, '');
    } else {
        $db->monta_lista($sql, $cabecalho, 30, 10, 'N','center', 'N');
    }

	?>

<script>
function alterarRestricao(dlgid, obrid, empid){
    var url = 'obras2.php?modulo=principal/cadDiligencia&acao=O&dlgid='+dlgid+'&obrid='+obrid+'&empid='+empid;
    janela = window.open(url, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
    janela.focus();
}

function alterarObra(dlgid, obrid, empid){

    var url = 'obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid;
    janela = window.open(url, 'inserirObra', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
    janela.focus();
}

</script>


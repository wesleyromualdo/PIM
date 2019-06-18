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
		function alterarObr(obrid) {
	        var url = 'obras2.php?modulo=principal/cadObra&acao=A&obrid=' + obrid;
	        janela = window.open(url, 'inserirObra', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' );
		}

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
			<td class="subtituloDireita">Situação Obra</td>
			<td style="padding-left: 5px;">
                <?php
                
				$val = (!empty($_REQUEST['strid'])) ? $_REQUEST['strid'] : '';

                $sqlPagamento = ($pagamento) ? ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO : ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA;

                $sql = " SELECT 
							str.strid as codigo,
							str.strdsc as descricao
						FROM
							obras2.situacao_registro str
						where str.strid IN ( 4,5 ) ";
				$db->monta_combo("strid", $sql, "S", "Todos", "", "", "", "200", "N", "strid", false, $val, '', '', 'chosen');
					                
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

			<td class="subtituloDireita">Situação Painel:</td>
			<td>
				<?php 
					$item_situacao_verm = '';
					$item_situacao_amar = '';
					$item_situacao_verd = '';
                
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
						default:
							$item_situacao_todas = 'checked="checked"';
							break;
					}
				?>
                <input type="hidden" name="filtro" id="filtro" value="<?php echo (!empty($_REQUEST['filtro'])) ? $_REQUEST['filtro'] : ($_REQUEST['item_situacao'] ? $_REQUEST['item_situacao'] : 'T') ?>" />
                <input type="radio" name="item_situacao" class="item_situacao" id="item_situacao_verm" value="R" <?php echo $item_situacao_verm; ?> > Vermelha
                <input type="radio" name="item_situacao" class="item_situacao" id="item_situacao_amar" value="A" <?php echo $item_situacao_amar; ?> > Amarela
                <input type="radio" name="item_situacao" class="item_situacao" id="item_situacao_verd" value="V" <?php echo $item_situacao_verd; ?> > Verde
                <input type="radio" name="item_situacao" class="item_situacao" id="item_situacao_todas" value="T" <?php echo $item_situacao_todas; ?> > Todas
	        </td>
	        <td class="subtituloDireita">ID da Obra:</td>
			<td>
				 <?php echo campo_texto('obrid', 'N', 'S', 'Obrid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['obrid']); ?>
	        </td>
	     
			
			<td><input type="submit" name="pesquisar" class="pesquisar" value="Pesquisar"/> <input type="submit" name="xls" class="xls" value="Gerar Planilha"/></td>

            <script type="text/javascript">
                $(function(){
                    $('input[name=xls]').click(function(e){
                        e.preventDefault();
                        $('#gerar_xls').val('xls');
                        $('#formPainel').submit();
                    });
                    $('input[name=pesquisar]').click(function(e){
                        e.preventDefault();
                        $('#gerar_xls').val('');
                        $('#formPainel').submit();
                    });
                });
            </script>

        </tr>
	</table>
</form>
	<?php
	$db = new cls_banco();
	if ($_REQUEST['strid']!= '')
	{
		$strid = $_REQUEST['strid'];
	}
	else
	{
		$strid = '4, 5';
	}
	if ($_REQUEST['estuf'] != '') {
		$estado = " AND mun.estuf IN ('{$_REQUEST['estuf']}')";
	}
	if ($_REQUEST['obrid'] != '') {
		$obrid = " AND o.obrid = {$_REQUEST['obrid']}";
	}
	else
	{
		$obrid = "";
	}

	
	
	if ($_REQUEST['filtro']) {
		switch ($_REQUEST['filtro']) {
			case 'R': /*Vermelho*/
				$filtro = " 
					AND (CASE WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
		                        DATE_PART('days', NOW() - obrdtultvistoria)
		                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
		                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
		                        DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))
		                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
		                        DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))
		                    ELSE
		                        DATE_PART('days', NOW() - obrdtultvistoria)
		                    END ) > 30";
				break;
			case 'A': /*Amarelo*/
				$filtro = "AND (CASE WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
		                        DATE_PART('days', NOW() - obrdtultvistoria)
		                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
		                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
		                        DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))
		                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
		                        DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))
		                    ELSE
		                        DATE_PART('days', NOW() - obrdtultvistoria)
		                    END ) BETWEEN 26 AND 30";
				break;
			case 'V': /*Verde*/
				$filtro = "AND (CASE WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
		                        DATE_PART('days', NOW() - obrdtultvistoria)
		                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
		                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
		                        DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))
		                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
		                        DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))
		                    ELSE
		                        DATE_PART('days', NOW() - obrdtultvistoria)
		                    END ) < 26 ";
				break;
            default:
            	$filtro = "AND (CASE WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
		                        DATE_PART('days', NOW() - obrdtultvistoria)
		                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
		                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
		                        DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))
		                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
		                        DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))
		                    ELSE
		                        DATE_PART('days', NOW() - obrdtultvistoria)
		                    END ) is not null";
            	break;
		}
	}

    $acao = "'<center><div style=\"width:100px\">
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarObr(' || o.obrid || ');\"
                            title=\"Alterar Obra\" />
              </div></center>' AS acao, ";
	
    $ultimaAtualizacao = "-- Obra concluida aplicar cor azul
                    CASE WHEN ed.esdid IN (693) THEN
                        '<span style=\"color:#3276B1\">' || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</span>'
                    -- Obra em execução ou paralisada com vistoria
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) <= 25 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 25 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 30 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '<span>'
                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) <= 25 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 25 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) >= 30 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )' || '<span>'
                    -- Obra convencional em execução ou paralisada sem vistoria usar a data do inicio da execução
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) <= 25 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 25 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) >= 30 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char((SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))||' dia(s) )' || '<span>'
                    ELSE
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) <= 25 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada em até 25 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 30 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '<span>'
                    END as ultima_atualizacao,";
    
    $nomeObra = "CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                            '<b style=\"color:green\">(EP) </b>'
                        ELSE
                            ''
                        END || '<a href=\"javascript: alterarObr(' || o.obrid || ');\">(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome,";
    
    if($_REQUEST['gerar_xls'] == 'xls'){
    
    	$ultimaAtualizacao = "-- Obra concluida aplicar cor azul
                    CASE WHEN ed.esdid IN (693) THEN
                        '' || to_char(obrdtultvistoria, 'DD/MM/YYYY')||''
                    -- Obra em execução ou paralisada com vistoria
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        to_char(obrdtultvistoria, 'DD/MM/YYYY')||' ('||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || ''
                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
                        to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||' ('||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )' || '<span>'
                    -- Obra convencional em execução ou paralisada sem vistoria usar a data do inicio da execução
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        to_char((SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)), 'DD/MM/YYYY')||' ('||DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))||' dia(s) )' || '<span>'
                    ELSE
                        to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )'
                    END as ultima_atualizacao,";
    	
    	$nomeObra = " '(' || o.obrid || ') ' || o.obrnome as obrnome,";
    	
    }
    
    $acao = ($_REQUEST['gerar_xls'] == 'xls') ? '' : $acao;

    $sql = "

        SELECT * FROM (
                SELECT DISTINCT 
                    	{$acao}
                        o.obrid, 
                        o.preid, 
                        p_conv.pronumeroprocesso as processo,
                        p_conv.termo_convenio,
                        p_conv.ano_termo_convenio, 
                        {$nomeObra}
                        uni.entnome, 
                        mun.mundescricao, 
                        mun.estuf, 
                        CASE WHEN o.tpoid IN (104, 105) 
                        THEN 
				(SELECT TO_CHAR(osmdtinicio, 'dd/mm/YYYY') FROM obras2.ordemservicomi WHERE obrid = o.obrid AND osmstatus = 'A' AND tomid = 1 ORDER BY osmid DESC LIMIT 1)
			ELSE 
				TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') 
			END AS inicio,
			CASE WHEN 
				o.tpoid IN (104, 105) 
			THEN 
				(SELECT TO_CHAR(osmdttermino, 'dd/mm/YYYY') FROM obras2.ordemservicomi WHERE obrid = o.obrid AND osmstatus = 'A' AND tomid = 1 ORDER BY osmid DESC LIMIT 1) ELSE TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') 
			END AS termino,
                    CASE
                        WHEN str.strid = 6 THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || str.strdsc || '</font>'
                        ELSE str.strdsc
                    END AS situacao
                    , CASE WHEN
			((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0)) > '99.95' 
			THEN
			ROUND(	((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) ) || '%'
			ELSE((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%'
			
			END AS percentual_execucao, 
					
			{$ultimaAtualizacao} CASE WHEN
			o.obrpercentultvistoria > '99.95' 
		THEN
		ROUND(	o.obrpercentultvistoria )  || '%'
		ELSE o.obrpercentultvistoria || '%'
		
		END AS obrpercentultvistoria, 
		to_char(e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa,
		 e.emppercentultvistoriaempresa || '%' as emppercentultvistoriaempresa, tpo.tpodsc
                FROM obras2.obras o
                LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade

                LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid

                LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = d.esdid
                LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid

                -- Dados do Processo e Termo
                LEFT JOIN obras2.v_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                
                WHERE
                    o.obridpai IS NULL
                    AND str.strid IN($strid) {$estado} {$obrid}  AND  o.obrstatus = 'A'  AND COALESCE(obrpercentultvistoria, 0) >= 0
                    {$filtro}  
                    
            ) as f
            ORDER BY f.obrid ASC
            
            ";

    $cabecalho = array('Ação', 'ID', 'ID Pré-Obra', 'Nº Processo', 'Nº Termo/Convênio', 'Ano Termo/Convênio', 'Obra', 'Unidade Implantadora', 'Município',
        'UF', 'Data de Início da Execução', 'Data de Término da Execução', 'Situação da Obra', '% Executado Instituição Acumulado', 'Última Vistoria Instituição', '% Executado Instituição',
    		'Última Vistoria Empresa', '% Executado Empresa', 'Tipologia');

    if($_REQUEST['gerar_xls'] == 'xls'){
        unset($cabecalho[0]);
        $db->sql_to_xml_excel($sql, 'relatorio', $cabecalho, '');
    } else {
        $db->monta_lista($sql, $cabecalho, 100, 10, 'N','center', 'N');
    }
	?>

<script>


</script>


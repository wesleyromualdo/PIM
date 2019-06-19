<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'includes/workflow.php';

include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'obras2/classes/modelo/Obras.class.inc';
include_once APPRAIZ . 'obras2/classes/modelo/Supervisao.class.inc';
include_once APPRAIZ . 'obras2/classes/modelo/SolicitacaoDesembolso.class.inc';
include_once APPRAIZ . 'obras2/classes/modelo/Questao.class.inc';
include_once APPRAIZ . 'obras2/classes/modelo/Validador.class.inc';
include_once APPRAIZ . 'obras2/classes/modelo/Contrato.class.inc';
include_once APPRAIZ . 'obras2/classes/modelo/ObrasContrato.class.inc';
include_once APPRAIZ . 'obras2/classes/modelo/Empreendimento.class.inc';
include_once APPRAIZ . 'obras2/classes/modelo/QuestaoSolicitacaoDesembolso.class.inc';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

$sql = "SELECT cd.obrid, cd.restricao, cd.processo, cd.vigencia, cd.ano_processo, cd.num_convenio, 
		    cd.ano_convenio, cd.obra, cd.mesoregiao, cd.municipio, cd.uf, cd.situacao, cd.perc_instituicao, 
		    cast(replace(trim(cd.perc_pago), ',', '.') as numeric(20,2)) as perc_pago, cd.diferenca, cd.tipo_paralisacao, cd.ultims_vistoria, cd.ultima_analise_tec, 
		    cd.diferenca_dias, cd.ultima_vist_empresa, cd.perc_exe_empresa, cd.programa, 
			cd.fonte, cd.esfera, cd.tipologia
		FROM carga.obra_desembolso cd
			inner join obras2.obras o on o.obrid = cast(cd.obrid as integer)
		WHERE 
			o.obrstatus = 'A'
			and o.obridpai IS NULL
			and cd.entrou_carga = 'N'";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$arrEntrou = array();
$arrNaoEntrou = array();

foreach ($arrDados as $v) {
	$obrid = $v['obrid'];
	
	$boProvidencias = verificaProvidencias( $v['obrid'] );
	
	if( $boProvidencias == 'N' ){
		$supervisao = new Supervisao();
		$supid = $supervisao->pegaUltSupidByObra( $v['obrid'] );
		
		$solicitacaoDesembolso = new SolicitacaoDesembolso();
		$sldpercsolicitado = $solicitacaoDesembolso->pegaPercentualSolicitacao($v['obrid'], $supid);
		
		$questao = new Questao();
		$sldidS = ($sldid) ? $sldid : "NULL";
		$questionario = $questao->pegaTodaEstruturaSolicitacaoDesembolso($sldidS);
		
		$divisao = '';
		$c = 0;
		$obra = new Obras($obrid);
		$doc = wf_pegarDocumento($obra->docid);
		$ops = array('1.1', '1.2', '1.3', '1.4', '1.5', '1.6', '2.1');
		
		if(is_array($questionario)){
			foreach($questionario as $key => $questao){
			    $validador = new Validador();
			    $check = $validador->check($obra, $questao['vdrid']);
			
			    if(empty($questionario[$key]['qsvresposta']))
			        $questionario[$key]['qsvresposta'] = ($check) ? 't' : 'f';
			
			    if(!$check && $validador->vdrobrigatorio == 't'){
			
			        if($doc['esdid'] == 693 && in_array($questao['qstnumero'], $ops )){
			            continue;
			        }
			    }
			}
		}
		
		//ver($sldpercsolicitado, $obrid, $questionario, $v, d);
		$solicitacaoDesembolso = new SolicitacaoDesembolso();
		    
	    $sql = "
	            SELECT
	                COUNT(*)
	            FROM obras2.solicitacao_desembolso sv
	            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
	            JOIN workflow.documento d ON d.docid = sv.docid
	            WHERE d.esdid NOT IN(".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO.", ".ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO.") AND sv.sldstatus = 'A' AND o.obrid = $obrid";
	    
	    if ($db->pegaUm($sql) == 0) {
	    	$solicitacaoDesembolso->docid = wf_cadastrarDocumento(TPDID_SOLICITACAO_DESEMBOLSO, 'Fluxo da Solicitação de Desembolso');
	    	$solicitacaoDesembolso->usucpf = $_SESSION['usucpf'];
	    	$solicitacaoDesembolso->commit();
	    }
	
	    $solicitacaoDesembolso->obrid = $obrid;
	    $solicitacaoDesembolso->sldjustificativa = 'O percentual de execução maior que o percentual de recursos financeiros repassados';
	    //$solicitacaoDesembolso->sldobs = $dados['sldobs'];
	
	    $solicitacaoDesembolso->sldpercsolicitado = simec_number_format($sldpercsolicitado, 2, '.');
	    $solicitacaoDesembolso->sldpercpagamento = simec_number_format($v['perc_pago'], 2, '.');
	    //$solicitacaoDesembolso->sldpercobra = desformata_valor($dados['sldpercobra']);
	    //$solicitacaoDesembolso->sldobstec = $dados['sldobstec'];
	    $solicitacaoDesembolso->supid = $supid;
	
	    $solicitacaoDesembolso->salvar();
	    $solicitacaoDesembolso->commit();
	
		if(is_array($questionario)){
		    foreach ($questionario as $key => $value) {
		        $questao = new QuestaoSolicitacaoDesembolso();
		        if (!$questao->carregaPorQstideSldid($value['qstid'], $solicitacaoDesembolso->sldid)) {
		            $questao->sldid = $solicitacaoDesembolso->sldid;
		            $questao->qstid = $value['qstid'];
		        }
		        $questao->qsdresposta = $value['qsvresposta'];
		        $questao->salvar();
		    }
		}
	
	    $solicitacaoDesembolso->commit();
	    
	    $sql = "UPDATE carga.obra_desembolso SET entrou_carga = 'S' WHERE obrid = $obrid";
	    $db->executar($sql);
	    $db->commit();
	
	    //ver($sldpercsolicitado, $obrid, $questionario, $v, d);
	    /* if ($_POST['tramitacao'] == 1){
	        wf_alterarEstado($solicitacaoDesembolso->docid, AEDID_SOLICITACAO_DESEMBOLSO_CORRECAO_PARA_ANALISE, '', array('obrid' => $solicitacaoDesembolso->obrid, 'sldid' => $solicitacaoDesembolso->sldid));
	    } */
	
	    /* if(empty($dados['sldid'])) {
	        // Quando o percentual da obra é 20% menor que o percentual previsto no cronograma, cria uma inconformidade
	        $solicitacaoDesembolso->controlaInconformidadeCronogramaDesatualizado($obrid, $criar = true, $superar = false);
	    } */
	}
	
	/* $sql = "UPDATE carga.obra_desembolso SET entrou_carga = 'N' WHERE obrid = $obrid";
	$db->executar($sql);
	$db->commit(); */
}

/*
$sql = "SELECT distinct cd.restricao, cd.obrid, cd.processo, cd.ano, cd.municipio, cd.uf, cd.situacao, cd.executado_instituicao, 
		     cd.programa, cd.fonte, cd.esfera, cd.tipilogia, cd.perc_pago, cd.diferenca, cd.vigencia_instrumento, 
		     cd.valor_obra, cd.valor_parcela, o.tooid
		FROM carga.cronograma_desembolso cd
			inner join obras2.obras o on o.obrid = cast(cd.obrid as integer)
		WHERE 
			o.obrstatus = 'A'
		    and o.obridpai IS NULL";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $v) {
	
	$sql = "SELECT
                COUNT(*)
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid NOT IN(1576, 1577) AND sv.sldstatus = 'A' AND o.obrid = {$v['obrid']}";
	
	$strTotal = $db->pegaUm($sql);
	
	if( (int)$strTotal < 1 ) {
		$sql = "insert into workflow.documento( tpdid, esdid, docdsc ) values ( 236, 1575, 'Fluxo da Solicitação de Desembolso' )returning docid";
		$docid = $db->pegaUm( $sql );
		
		$v['diferenca'] = str_replace(",",".", $v['diferenca']);
		$supid = $db->pegaUm("SELECT supid FROM obras2.supervisao s            
				            WHERE s.supstatus = 'A'  AND s.usucpf IS NOT NULL AND s.obrid = {$v['obrid']}
				            ORDER BY s.supdata DESC, s.supid DESC
				            LIMIT 1");
		
		if( empty($supid) ){
			$sql = "select obridvinculado from obras2.obras 
					where obrid = {$v['obrid']}
						and obrstatus = 'A' 
					    and obridpai IS NULL";
			$obridvinculado = $db->pegaUm($sql);
			
			$supid = $db->pegaUm("SELECT supid FROM obras2.supervisao s
								WHERE s.supstatus = 'A'  AND s.usucpf IS NOT NULL AND s.obrid = {$obridvinculado}
								ORDER BY s.supdata DESC, s.supid DESC
								LIMIT 1");
		}
		$sql = "INSERT INTO obras2.solicitacao_desembolso(
			            obrid, docid, sldjustificativa, sldobs, sldstatus, usucpf,
			            slddatainclusao, sldpercsolicitado, supid, sldpercpagamento,
			            sldobstec, arqid, sldpercobra)
			    VALUES ({$v['obrid']}, $docid, 'O percentual de execução maior que o percentual de recursos financeiros repassados', null, 'A', '00000000191',
			            now(), {$v['diferenca']}, $supid, {$v['perc_pago']},
			            null, null, null)";
		$db->executar($sql);
		$db->commit();
	}
}
*/

function verificaProvidencias( $obrid ){
	global $db;
	
	$msgB = array();
	if($obrid) {
		$obra = new Obras($obrid);
		$sql = "
            SELECT
                COUNT(*)
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid NOT IN(".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO.", ".ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO.", ".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO_SEM_PAGAMENTO.") AND sv.sldstatus = 'A' AND o.obrid = $obrid";
	
		if ($db->pegaUm($sql) > 0) {
			$msgB[] = 'Esta obra possui uma solicitação em análise, aguarde a conclusão da solicitação para solicitar novamente.';
		} else {
	
			// Verifica se possui uma solicitação DEFERIDA e se possui uma vistoria inserida após a soolicitação
			$sql = "
            SELECT
                sv.supid
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid IN(".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO.") AND sv.sldstatus = 'A' AND o.obrid = $obrid
	            ORDER BY sv.supid DESC";
			$ultSupidSolicitacao = $db->pegaUm($sql);
			$supervisao = new Supervisao();
			$ultSupid = $supervisao->pegaUltSupidByObra($obrid);
			
			if($ultSupidSolicitacao == $ultSupid){
				$msgB[] = 'Já existe uma solicitação referente a última vistoria. Para fazer uma nova solicitação é necessário inserir uma nova vistoria.';
			}
	
	
			// Verifica se cronograma da obra possui etapas vencidas
			$sql = "
			SELECT
                COUNT(*)
            FROM obras2.obras o
            JOIN workflow.documento d ON d.docid = o.docid
            JOIN obras2.cronograma c ON c.obrid = o.obrid AND c.crostatus = 'A'
            JOIN obras2.itenscomposicaoobra ic ON ic.obrid = o.obrid AND ic.croid = c.croid AND ic.icostatus = 'A' AND ic.relativoedificacao = 'D'
            JOIN obras2.itenscomposicao i ON i.itcid = ic.itcid AND i.itcstatus = 'A'
            WHERE o.obrid = $obrid AND (ic.icodterminoitem < now()
            AND (SELECT
                        si.spivlrinfsupervisor
                        FROM obras2.supervisaoitem si
                        WHERE
                            si.supid = (
                                            SELECT s.supid
                                            FROM obras2.supervisao s
                                            JOIN seguranca.usuario u ON u.usucpf = s.usucpf
                                            WHERE s.emsid IS NULL  AND s.smiid IS NULL  AND s.supstatus = 'A' AND validadaPeloSupervisorUnidade = 'S' AND s.obrid = $obrid
                                            ORDER BY s.supdata DESC
                                            LIMIT 1
                                        )
                        AND si.icoid = ic.icoid
                 ) < 100) AND d.esdid NOT IN (".ESDID_OBJ_CONCLUIDO.")
	
        ";
	
	
			if ($db->pegaUm($sql) > 0) {
				//$msgB[] = 'Para fazer a solicitação é necessário atualizar as datas de início e término de cada etapa do cronograma, conforme previsão de execução física da obra.';
			}
	
			// Verificar se o percentual da solicitação é valido
			$solicitacaoDesembolso = new SolicitacaoDesembolso();
			if( $ultSupid ) $percSolicitado = $solicitacaoDesembolso->pegaPercentualSolicitacao($obrid, null);
	
			if( ($percSolicitado <= 3 && $percSolicitado > 0) && $obra->obrpercentultvistoria < 97){
				$msgB[] = 'Para fazer a solicitação é necessário um percentual mínimo de 3%.';
			}
			
			// Verifica se a obra esta acima de 90 e possui os dados preenchidos na aba de inauguração
			$sql = "
			SELECT
                obrid
            FROM obras2.inauguracao_obra
            WHERE obrid = $obrid
                AND iobstatus = 'A'
                AND dtprevisaoinauguracao IS NOT NULL
                AND distancia IS NOT NULL
                AND quantidadehabitantes IS NOT NULL
                AND aeroportos IS NOT NULL
                AND iobid = (SELECT MIN (iobid) FROM obras2.inauguracao_obra WHERE obrid = $obrid AND iobstatus = 'A')
            LIMIT 1
	
			";
	
			if ($db->pegaUm($sql) == false && $obra->obrpercentultvistoria > 90) {
				//$msgB[] = 'Para fazer a solicitação é necessário preencher os seguintes campos da aba "Funcionamento da Obra": "Previsão de Inauguração", "Distância entre capital e município", "Quantidade de habitantes no município", "Aeroportos mais próximos".';
			}
		}
	}	
	
	$s = new SolicitacaoDesembolso();
	
	$questao = new Questao();
	$sldidS = ($sldid) ? $sldid : "NULL";
	$questionario = $questao->pegaTodaEstruturaSolicitacaoDesembolso($sldidS);
	$divisao = '';
	$c = 0;
	$validacaoMsg = array();
	$obra = new Obras($obrid);
	$doc = wf_pegarDocumento($obra->docid);
	$ops = array('1.1', '1.2', '1.3', '1.4', '1.5', '1.6', '2.1');
	
	if(is_array($questionario)){
		foreach($questionario as $key => $questao){
			$validador = new Validador();
			$check = $validador->check($obra, $questao['vdrid']);
	
			if(empty($questionario[$key]['qsvresposta']))
				$questionario[$key]['qsvresposta'] = ($check) ? 't' : 'f';
	
				if(!$check && $validador->vdrobrigatorio == 't'){
	
					if($doc['esdid'] == 693 && in_array($questao['qstnumero'], $ops )){
						continue;
					}
					
					$validacaoMsg[$questao['qstnumero']] = str_replace('||obrid||', $obrid, trim($validador->getMessage()));
				}
		}
	}
	

        $obrasVermelho = $s->verificaObrasEmVermelho($obrid);
        if($obrasVermelho):
            //$msgB[] = 'O seu município possui obras desatualizadas. É necessário que todas as obras estejam atualizadas para fazer a solicitação de desembolso.';
        endif;

        $obraVermelho = $s->verificaObraEmVermelho($obrid);
        if($obraVermelho):
            $msgB[] = 'Esta obra está desatualizada. É necessário que a obra esteja atualizada para fazer a solicitação de desembolso.';
        endif;
		        
	if( is_array($validacaoMsg) && empty($validacaoMsg) && empty($msgB) ){
		return 'N';
	} else {
		if( is_array($validacaoMsg) ){
			foreach ($validacaoMsg as $value) {
				
				$sql = "INSERT INTO carga.obra_desembolso_mensagem_erro(obrid, mensagem)
						VALUES ($obrid, '".trim($value)."')";
				$db->executar($sql);
			}
		}
		if( is_array($msgB) ){
			foreach ($msgB as $value) {
				$sql = "INSERT INTO carga.obra_desembolso_mensagem_erro(obrid, mensagem)
						VALUES ($obrid, '".trim($value)."')";
				$db->executar($sql);
			}
		}
    	$db->commit();			
		return 'S';
	}
}
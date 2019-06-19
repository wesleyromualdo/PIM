<?php
function detalharPagamento($dados)
{
    $processo = $dados['dados'][2];
    $tipo     = $dados['dados'][1];

    $sql = "(
			SELECT
        
				--v.empid,
			    COALESCE(v.empenho_original,'-') as numeroempenho,
				'<small>'||u.usunome||' - '||to_char(v.empdata,'dd/mm/YYYY HH24:MI')||'</small>' as usuario,
				formata_cpf_cnpj(v.entcnpj) as cnpj,
				COALESCE(v.empprotocolo,'-') as numeroprotocolo,
				SUM(v.vlrempenho) as valorempenho,
				SUM(v.vlrcancelado) as valorcancelado,
				SUM(v.saldo) as valorsaldo,
                v.empsituacao as situacaoempenho
        
			FROM par3.v_saldo_empenho_composicao_iniciativa v
			LEFT JOIN seguranca.usuario u ON u.usucpf = v.usucpf
			WHERE v.processo='".$processo."'
			GROUP BY v.empid, v.empenho_original, v.empsituacao, u.usunome, v.empdata, v.entcnpj, v.empprotocolo
			    
			) UNION ALL (
			    
			SELECT
			    
				--v.empid,
			    COALESCE(v.empenho_original,'-') as numeroempenho,
				'<small>'||u.usunome||' - '||to_char(v.empdata,'dd/mm/YYYY HH24:MI')||'</small>' as usuario,
				formata_cpf_cnpj(v.entcnpj) as cnpj,
				COALESCE(v.empprotocolo,'-') as numeroprotocolo,
				SUM(v.vlrempenho) as valorempenho,
				SUM(v.vlrcancelado) as valorcancelado,
				SUM(v.saldo) as valorsaldo,
                v.empsituacao as situacaoempenho
			    
			FROM par3.v_saldo_empenho_composicao_obra v
			LEFT JOIN seguranca.usuario u ON u.usucpf = v.usucpf
			WHERE v.processo='".$processo."'
			GROUP BY v.empid, v.empenho_original, v.empsituacao, u.usunome, v.empdata, v.entcnpj, v.empprotocolo
			    
			)";

    ob_clean();
    $cabecalho = array("Nota do empenho","Usuário","CNPJ","Número do Protocolo","Valor Solicitado","Valor Cancelado","Valor Empenhado","Situação do Empenho");
    $listagem = new Simec_Listagem();
    //    $listagem->esconderColunas('');
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    //    $listagem->setFormFiltros('formulario');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->setTamanhoPagina(50);
    $listagem->addCallbackDeCampo('valorempenho', 'par3_mascaraMoeda');
    $listagem->addCallbackDeCampo('valorcancelado', 'par3_mascaraMoeda');
    $listagem->addCallbackDeCampo('valorsaldo', 'par3_mascaraMoeda');
    $listagem->setCampos($cabecalho);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    die;
}

function relatorioEmpenhoPDF($sql)
{
    $cabecalho = array(
        "<small>Código</small>",
        "<small>Número do Processo</small>",
        "<small>UF</small>",
        "<small>Entidade</small>",
        "<small>Tipo Objeto</small>",
        "<small>Valor do Processo</small>",
        "<small>Valor Empenhado</small>",
        "<small>% Empenhado</small>",
        "<small>Valor Pagamento</small>",
        "<small>Assistência</small>",
        "<small>Situação</small>",
        "<small>Bloqueio PAR</small>"
    );
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->esconderColunas('itrid', 'obr_inp_id', 'muncod', 'intoid');
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    $listagem->setFormFiltros('formulario');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->setCampos($cabecalho);
    $listagem->addCallbackDeCampo('processo', 'formata_numero_processo_sem_html');
    $listagem->addCallbackDeCampo('valor_processo', 'par3_mascaraMoeda');
    $listagem->addCallbackDeCampo(['vlrempenho', 'vlrpago'], 'par3_mascaraMoeda');
    $listagem->addCallbackDeCampo(['inuid_pendencia'], 'popOverPendenciarsPar');
    $listagem->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
}

function relatorioEmpenhoExcel($sql, $cabecalho = false, $camposNumericos = false)
{
    ob_clean();
    /* configurações */
    ini_set("memory_limit", "2048M");
    set_time_limit(0);
    /* FIM configurações */
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=rel_pagamentos_".date("Ymdhis").".xls");
    header("Content-Disposition: attachment; filename=rel_empenho_".date("Ymdhis").".xls");
    header("Content-Description: MID Gera excel");
    $cabecalho = $cabecalho ? $cabecalho : array("Código", "Processo","UF","Entidade", "Tipo Objeto", "Valor Processo","Valor Empenho","% Empenho", "Valor Pagamento", "Assistência","Situação", "Nº Empenho");
    $camposNumericos = $camposNumericos ? $camposNumericos : ['valor_processo', 'vlrempenho', 'vlrpago'];
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    $listagem->esconderColunas('itrid', 'proid', 'inuid', 'obr_inp_id', 'muncod', 'intoid', 'inuid_pendencia', 'tipo_processo');
    $listagem->setCabecalho($cabecalho);
    $listagem->addCallbackDeCampo('processo', 'formata_numero_processo_sem_html');
    $listagem->addCallbackDeCampo($camposNumericos, 'formataNumeroMonetarioSemSimbolo');
    $listagem->setQuery($sql);
    $listagem->render();
    header("Content-type: application/xls; name=rel_pagamentos_".date("Ymdhis").".xls", true);
    header("Content-Disposition: attachment; filename=rel_empenho_".date("Ymdhis").".xls", true);
}

function sqlListarExcelDetalhadoPlanejamento( $where, $wherePlan )
{
    $sql = "WITH empenho_planejamento AS(
            	SELECT
            		pp.inpid,
            		pp.ppcano,
            	    e.empid,
            	    e.empnumero,
            	    e.empnumeroprocesso as processo,
            	    sum(ep.epcvalorempenho) AS vlr_empenhado,
            	    sum(coalesce(ec.vlr_cancelado,0)) AS vlr_cancelado,
            	    (sum(ep.epcvalorempenho) - sum(coalesce(ec.vlr_cancelado,0))) AS saldo,
            	    e.empsituacao,
            	    usunome  as usuario,
            	    to_char(e.empdata,' dd/mm/YYYY HH24:MI') as empdata,
            	    case when e.empsituacao <> '2 - EFETIVADO' then (sum(ep.epcvalorempenho) - sum(coalesce(ec.vlr_cancelado,0))) else 0 end AS vlr_solicitado,
            	    case when e.empsituacao = '2 - EFETIVADO' then (sum(ep.epcvalorempenho) - sum(coalesce(ec.vlr_cancelado,0))) else 0 end AS vlr_empenho
            	FROM par3.empenho e
            		INNER JOIN par3.empenhoparcomposicao ep ON ep.empid = e.empid AND ep.epcstatus = 'A'
            		INNER JOIN par3.processoparcomposicao pp ON pp.ppcid = ep.ppcid AND pp.ppcstatus = 'A'
            		LEFT JOIN (
            		    SELECT empidpai, ppc.inpid, ppc.ppcano, sum(empvalorempenho) AS vlr_cancelado
            		    FROM par3.empenho ec
            		    	INNER JOIN par3.empenhoparcomposicao epc ON epc.empid = ec.empid AND epc.epcstatus = 'A'
            		    	INNER JOIN par3.processoparcomposicao ppc ON ppc.ppcid = epc.ppcid AND ppc.ppcstatus = 'A'
            		    WHERE ec.empstatus = 'A'
            		    AND ec.empcodigoespecie IN ('03', '04', '13')
            		    GROUP BY empidpai, ppc.inpid, ppc.ppcano
            		) ec ON ec.empidpai = e.empid AND ec.inpid = pp.inpid AND ec.ppcano = pp.ppcano
            		LEFT JOIN seguranca.usuario u on (u.usucpf = e.usucpf)
            	WHERE e.empcodigoespecie = '01'
            		AND e.empstatus = 'A'
            	GROUP BY pp.inpid, pp.ppcano, e.empid, e.empnumero, e.empnumeroprocesso,
            	    e.empsituacao, usunome, e.empdata
            )
            SELECT DISTINCT pro.proid,
            	po.inpid,
            	po.ppcano,
            	pro.inuid,
                (dt.dotnumero || '-'::text) || po.intoid AS dotnumero,
                pro.pronumeroprocesso as processo,
            	case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
                obj.intodsc AS tipo_obrjeto,
                emp.usuario,
                emp.empdata as data_criacao,
                to_char(dt.dotdatafimvigencia, 'DD/MM/YYYY') as data_vigencia,
                vip.vlriniciativa::NUMERIC(20,2) AS valor_processo,
                dt.dotvalortermo,
                emp.empnumero,
                COALESCE(emp.vlr_empenho,0) AS vlrempenho,
                CASE WHEN (vip.vlriniciativa::NUMERIC(20,2)) > 0 THEN
            		((coalesce(emp.vlr_empenho,0) / (vip.vlriniciativa::NUMERIC(20,2))) * 100)::numeric(20,2)||'%'
            	ELSE 0||'%' END AS percent,
                sum(coalesce(pag.vlr_efetivado, 0)) AS vlrpago,
                ta.intadsc,
                CASE WHEN emps.empsituacao is null then 'Sem empenho' else emps.empsituacao end as empsituacao,
                CASE WHEN vm.obras_par = 't' THEN 'Sim' ELSE 'Não' END AS obras_par,
            	CASE WHEN vm.cacs = 't' THEN 'Sim' ELSE 'Não' END AS cacs,
            	CASE WHEN vm.pne = 't' THEN 'Sim' ELSE 'Não' END AS pne,
            	CASE WHEN vm.siope = 't' THEN 'Sim' ELSE 'Não' END AS siope,
            	CASE WHEN vm.habilitacao = 't' THEN 'Sim' ELSE 'Não' END AS habilitacao,
            	CASE WHEN vm.contas = 't' THEN 'Sim' ELSE 'Não' END AS contas,
                '-' AS id_par,
                '-' AS id_obra2,
                '-' AS situacao_par,
                '-' AS situacao_obra2,
                '-' AS validacao,
                '-' AS percet_fisico,
                '-' AS percent_obra,
                vip.tipo_processo
            FROM par3.processo pro
            	INNER JOIN par3.processoparcomposicao po ON po.proid = pro.proid AND po.ppcstatus = 'A'
            	INNER JOIN(
            		SELECT pp.proid, pp.inpid, pp.ppcano, sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa, 'P'::text as tipo_processo
            	    FROM par3.iniciativa_planejamento_item_composicao ipi
            	    	INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = ipi.inpid AND ip.inpstatus = 'A'
            			INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
            			INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
            			INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid AND pp.ppcstatus = 'A'
            		WHERE ipi.ipistatus = 'A' $wherePlan
            		GROUP BY pp.proid, pp.inpid, pp.ppcano
            	) vip ON vip.proid = pro.proid AND vip.inpid = po.inpid AND vip.ppcano = po.ppcano
            	INNER JOIN empenho_planejamento emp ON emp.inpid = po.inpid AND emp.ppcano = po.ppcano
            	INNER JOIN par3.iniciativa_planejamento pp ON pp.inpid = po.inpid AND pp.inpstatus = 'A'
            	INNER JOIN par3.iniciativa ini on ini.iniid = pp.iniid
            	INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = po.intoid
            	INNER JOIN par3.instrumentounidade iu on iu.inuid = pro.inuid and iu.inustatus = 'A'
            	INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = pro.intaid
            	LEFT JOIN par3.vm_situacao_empenho_pagamento_processo emps ON emps.empnumeroprocesso = pro.pronumeroprocesso
            	LEFT JOIN par3.documentotermo dt ON dt.proid = po.proid AND dt.dotstatus = 'A'
            	LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
            	LEFT JOIN territorios.estado est on est.estuf = iu.estuf
            	LEFT JOIN (
            		SELECT
            			inpid, ppcano, pagparcela, pagmes, paganoparcela, pagnumeroob,
            			sum(foo.vlr_efetivado) AS vlr_efetivado,
            		    sum(foo.vlr_solicitado) AS vlr_solicitado,
            		    sum(foo.vlrpago) AS vlrpago
            		FROM (
            			SELECT
            				po.inpid, po.ppcano, pag.pagparcela, pag.pagmes, pag.paganoparcela, pag.pagnumeroob,
            				CASE WHEN pag.pagsituacaopagamento ILIKE '%efetivado%' THEN sum(pac.ppcvalorpagamento) ELSE 0 END AS vlr_efetivado,
            				CASE WHEN pag.pagsituacaopagamento NOT ILIKE '%efetivado%' THEN sum(pac.ppcvalorpagamento) ELSE 0 END AS vlr_solicitado,
            				sum(pac.ppcvalorpagamento) AS vlrpago
            			FROM par3.pagamento pag
            				INNER JOIN par3.pagamentoparcomposicao pac ON pac.pagid = pag.pagid AND pac.ppcstatus = 'A'
            				INNER JOIN par3.empenhoparcomposicao epc ON epc.epcid = pac.epcid AND epc.epcstatus = 'A'
            				INNER JOIN par3.processoparcomposicao po ON po.ppcid = epc.ppcid AND po.ppcstatus = 'A'
            			WHERE pag.pagstatus = 'A'
            				AND pag.pagsituacaopagamento NOT ILIKE '%CANCELADO%' AND pag.pagsituacaopagamento NOT ILIKE '%vala%'
            				AND pag.pagsituacaopagamento NOT ILIKE '%devolvido%' AND pag.pagsituacaopagamento NOT ILIKE '%VALA CENTRO DE GESTÃO%'
            			GROUP BY po.inpid, po.ppcano, po.anaid, pag.pagsituacaopagamento, pag.pagparcela, pag.pagmes, pag.paganoparcela, pag.pagnumeroob
            		) foo
            		GROUP BY inpid, ppcano, pagparcela, pagmes, paganoparcela, pagnumeroob
            	) pag ON pag.inpid = po.inpid AND pag.ppcano = po.ppcano
            	left join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = pro.inuid
            WHERE pro.prostatus = 'A'
                ".(($where)?" and ".implode(" AND ", $where):"")."
            GROUP BY pro.proid, po.inpid, po.ppcano, pro.inuid, dt.dotnumero, po.intoid, pro.pronumeroprocesso, iu.muncod, est.estuf, mun.estuf, est.estdescricao, mun.mundescricao,
                obj.intodsc, emp.usuario, emp.empdata, dt.dotdatafimvigencia, vip.vlriniciativa, dt.dotvalortermo, emp.empnumero, emp.vlr_empenho, ta.intadsc,
                vm.obras_par, vm.cacs, vm.pne, vm.siope, vm.habilitacao, vm.contas, emps.empsituacao, vip.tipo_processo
            ORDER BY 7,8";

    return $sql;
}

function listarExcelDetalhadoObra( $where, $whObra )
{
    $sql = "WITH empenho_obra AS(
            	SELECT
            		po.obrid,
            	    e.empid,
            	    e.empnumero,
            	    e.empnumeroprocesso as processo,
            	    sum(eo.eocvalorempenho) AS vlr_empenhado,
            	    sum(coalesce(ec.vlr_cancelado,0)) AS vlr_cancelado,
            	    (sum(eo.eocvalorempenho) - sum(coalesce(ec.vlr_cancelado,0))) AS saldo,
            	    e.empsituacao,
            	    u.usunome  as usuario,
            	    to_char(e.empdata,' dd/mm/YYYY HH24:MI') as empdata,
            	    case when e.empsituacao <> '2 - EFETIVADO' then (sum(eo.eocvalorempenho) - sum(coalesce(ec.vlr_cancelado,0))) else 0 end AS vlr_solicitado,
            	    case when e.empsituacao = '2 - EFETIVADO' then (sum(eo.eocvalorempenho) - sum(coalesce(ec.vlr_cancelado,0))) else 0 end AS vlr_empenho
            	FROM par3.empenho e
            		INNER JOIN par3.empenhoobracomposicao eo ON eo.empid = e.empid AND eo.eocstatus = 'A'
                	INNER JOIN par3.processoobracomposicao po ON po.pocid = eo.pocid AND po.pocstatus = 'A'
            		LEFT JOIN (
            		    SELECT empidpai, ppc.obrid, sum(epc.eocvalorempenho) AS vlr_cancelado
            		    FROM par3.empenho ec
            		    	INNER JOIN par3.empenhoobracomposicao epc ON epc.empid = ec.empid AND epc.eocstatus = 'A'
            		    	INNER JOIN par3.processoobracomposicao ppc ON ppc.pocid = epc.pocid AND ppc.pocstatus = 'A'
            		    WHERE ec.empstatus = 'A'
            		    AND ec.empcodigoespecie IN ('03', '04', '13')
            		    GROUP BY empidpai, ppc.obrid
            		) ec ON ec.empidpai = e.empid AND ec.obrid = po.obrid
            		LEFT JOIN seguranca.usuario u on u.usucpf = e.usucpf
            	WHERE e.empcodigoespecie = '01'
            		AND e.empstatus = 'A'
            	GROUP BY po.obrid, e.empid, e.empnumero, e.empnumeroprocesso, e.empsituacao, usunome, e.empdata
            )
                SELECT DISTINCT pro.proid,
                    o.inpid,
                	o.obrano,
                	pro.inuid,
                    (dt.dotnumero || '-'::text) || pp.intoid AS dotnumero,
                    pro.pronumeroprocesso AS processo,
                	case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                    case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
                    obj.intodsc AS tipo_obrjeto,
                    emp.usuario,
	                emp.empdata as data_criacao,
	                to_char(dt.dotdatafimvigencia, 'DD/MM/YYYY') as data_vigencia,
	                (o.obrvalor - coalesce(o.obrvalor_contrapartida,0))::NUMERIC(20,2) AS valor_processo,
	                dt.dotvalortermo,
	                emp.empnumero,
	                COALESCE(emp.vlr_empenho,0) AS vlrempenho,
	                CASE WHEN (o.obrvalor - coalesce(o.obrvalor_contrapartida,0))::NUMERIC(20,2) > 0 THEN
	            		((coalesce(emp.vlr_empenho,0) / (o.obrvalor - coalesce(o.obrvalor_contrapartida,0))::NUMERIC(20,2)) * 100)::numeric(20,2)||'%'
	            	ELSE 0||'%' END AS percent,
	                coalesce(pag.vlr_efetivado, 0) AS vlrpago,
                    ta.intadsc,
                    CASE WHEN emps.empsituacao is null then 'Sem empenho' else emps.empsituacao end as empsituacao,
                    CASE WHEN vm.obras_par = 't' THEN 'Sim' ELSE 'Não' END AS obras_par,
                	CASE WHEN vm.cacs = 't' THEN 'Sim' ELSE 'Não' END AS cacs,
                	CASE WHEN vm.pne = 't' THEN 'Sim' ELSE 'Não' END AS pne,
                	CASE WHEN vm.siope = 't' THEN 'Sim' ELSE 'Não' END AS siope,
                	CASE WHEN vm.habilitacao = 't' THEN 'Sim' ELSE 'Não' END AS habilitacao,
                	CASE WHEN vm.contas = 't' THEN 'Sim' ELSE 'Não' END AS contas,
                    o.obrid::text AS id_par,
                    coalesce(obr.id_obra2::text,'-') AS id_obra2,
                    es.esddsc AS situacao_par,
                    coalesce(obr.situacao_obra2,'-') AS situacao_obra2,
                    coalesce(obr.validacao, '-') AS validacao,
                    coalesce(obr.percet_fisico,0)::text AS percet_fisico,
                    coalesce(obr.percent_obra,0)::text AS percent_obra,
                    'O' AS tipo_processo
                FROM par3.obra o
                	INNER JOIN par3.processoobracomposicao pp ON pp.obrid = o.obrid AND pp.pocstatus = 'A'
                	INNER JOIN par3.processo pro ON pro.proid = pp.proid AND pro.prostatus = 'A'
                	INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = pp.intoid
                	INNER JOIN par3.instrumentounidade iu on iu.inuid = o.inuid and iu.inustatus = 'A'
                	INNER JOIN par3.iniciativa_planejamento ip on ip.inpid = o.inpid
                	INNER JOIN par3.iniciativa ini on ini.iniid = ip.iniid
                	INNER JOIN workflow.documento d ON d.docid = o.docid
                	INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                	INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = pro.intaid
                    INNER JOIN empenho_obra emp ON emp.obrid = o.obrid
                    LEFT JOIN par3.vm_situacao_empenho_pagamento_processo emps ON emps.empnumeroprocesso = pro.pronumeroprocesso
                	LEFT JOIN par3.documentotermo dt ON dt.proid = pro.proid AND dt.dotstatus = 'A'
                	LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
                	LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                    LEFT JOIN seguranca.usuario usu ON usu.usucpf = dt.usucpfinclusao
                	LEFT JOIN(
                		SELECT
                			o.obrid_par3,
                			o.obrid id_obra2,
                			es.esddsc AS situacao_obra2,
                			CASE WHEN val.vldstatushomologacao = 'S' THEN 'Sim' ELSE 'Não' END AS validacao,
                			sde.sldpercobra AS percet_fisico,
                			((100::numeric - COALESCE(o.obrperccontratoanterior, 0::numeric)) * COALESCE(o.obrpercentultvistoria, 0::numeric) / 100::numeric + COALESCE(o.obrperccontratoanterior, 0::numeric))::numeric(20,2) AS percent_obra
                		FROM obras2.obras o
                			INNER JOIN workflow.documento d ON d.docid = o.docid
                			INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                			LEFT JOIN obras2.validacao val ON val.obrid = o.obrid
                			LEFT JOIN(
                				SELECT sd.sldid, sd.obrid, sd.sldpercobra
                				FROM obras2.solicitacao_desembolso sd
                				WHERE sd.slddatainclusao = (SELECT max(slddatainclusao)
                												FROM obras2.solicitacao_desembolso de
                													INNER JOIN workflow.documento dc ON dc.docid = de.docid
                												WHERE de.sldstatus = 'A' AND dc.esdid IN (1576, 2150) AND de.obrid = sd.obrid)
                			) sde ON sde.obrid = o.obrid
                		WHERE o.obrstatus = 'A'
                			AND o.obridpai IS NULL
                			AND o.obrid_par3 IS NOT NULL
                	) obr ON obr.obrid_par3 = o.obrid
                	LEFT JOIN (
                		SELECT
                			obrid,
                			sum(foo.vlr_efetivado) AS vlr_efetivado,
                		    sum(foo.vlr_solicitado) AS vlr_solicitado,
                		    sum(foo.vlrpago) AS vlrpago
                		FROM (
                			SELECT
                				po.obrid,
                				CASE WHEN pag.pagsituacaopagamento ILIKE '%efetivado%' THEN sum(pac.pmcvalorpagamento) ELSE 0 END AS vlr_efetivado,
                				CASE WHEN pag.pagsituacaopagamento NOT ILIKE '%efetivado%' THEN sum(pac.pmcvalorpagamento) ELSE 0 END AS vlr_solicitado,
                				sum(pac.pmcvalorpagamento) AS vlrpago
                			FROM par3.pagamento pag
                				INNER JOIN par3.pagamentoobracomposicao pac ON pac.pagid = pag.pagid AND pac.pmcstatus = 'A'
                				INNER JOIN par3.empenhoobracomposicao epc ON epc.eocid = pac.eocid AND epc.eocstatus = 'A'
                				INNER JOIN par3.processoobracomposicao po ON po.pocid = epc.pocid AND po.pocstatus = 'A'
                			WHERE pag.pagstatus = 'A'
                				AND pag.pagsituacaopagamento NOT ILIKE '%CANCELADO%' AND pag.pagsituacaopagamento NOT ILIKE '%vala%'
                				AND pag.pagsituacaopagamento NOT ILIKE '%devolvido%' AND pag.pagsituacaopagamento NOT ILIKE '%VALA CENTRO DE GESTÃO%'
                			GROUP BY po.obrid, pag.pagsituacaopagamento
                		) foo
                		GROUP BY foo.obrid
                	) pag ON pag.obrid = o.obrid
                	left join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = pro.inuid
                WHERE o.obrstatus = 'A' $whObra
                ".(($where)?" and ".implode(" AND ", $where):"")."
            ORDER BY 7,8";

    return $sql;
}

function carregarPlanoInterno($dados)
{
    global $db, $simec;

    $planointerno = "SELECT DISTINCT p.inplintdsc as codigo, p.inplintdsc as descricao
					 FROM par3.iniciativa_plano_interno p
					 INNER JOIN par3.programa pp ON pp.prgid = p.inplinprog
					 WHERE p.inplintstatus='A' AND
						   p.inplintsituacao='A' AND
						   ".(($dados['inplintptres'])?"p.inplintptres='".$dados['inplintptres']."'":"1=2")." AND
						   (p.inplinprog IN( SELECT prgid FROM par3.iniciativa_iniciativas_programas prg
										  INNER join par3.iniciativa_planejamento inp on inp.iniid = prg.iniid AND inp.inpstatus = 'A'
										  INNER join par3.processoparcomposicao ppc on ppc.inpid = inp.inpid AND ppc.ppcstatus = 'A'
										  WHERE ppc.proid='".$dados['proid']."' ) OR
						    p.inplinprog IN( SELECT prgid FROM par3.iniciativa_iniciativas_programas prg
										  INNER join par3.iniciativa_planejamento inp on inp.iniid = prg.iniid AND inp.inpstatus = 'A'
										  INNER JOIN par3.obra obr ON obr.inpid = inp.inpid
										  INNER join par3.processoobracomposicao poc on poc.obrid = obr.obrid AND poc.pocstatus = 'A'
										  WHERE poc.proid='".$dados['proid']."' ))";

    echo $simec->select('planointerno', 'Plano Interno', "", $planointerno, array('required', 'onchange'=>'carregarDadosPlanoInterno(this.value,\''.$dados['inplintptres'].'\');'), array());
}

function carregarFonteRecurso($dados)
{
    global $db, $simec;

    if ($dados['inplintdsc'] && $dados['inplintptres']) {
        $dados['inplintid'] = $db->pegaUm("SELECT inplintid FROM par3.iniciativa_plano_interno
										   WHERE inplintdsc='".$dados['inplintdsc']."' AND inplintptres='".$dados['inplintptres']."' AND inplintstatus='A' AND inplintsituacao='A'");
    }

    $fonterecurso = "SELECT ifrcodigofonte as codigo, ifrano||' / '||ifrcodigofonte||' - '||ifrdescicaofonte as descricao
					 FROM par3.iniciativa_fonte_recurso
					 WHERE ifrstatus='A' AND ifrsituacao='A' AND ".(($dados['inplintid'])?"inplintid='".$dados['inplintid']."'":"1=2");

    echo $simec->select('fonterecurso', 'Fonte de Recurso', "", $fonterecurso, array('required'));
}

function confirmarEmpenho($dados, $co_especie_empenho = '01')
{
    global $db;

    include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

    if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
        $urlWS                     = 'https://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
        $nu_processo                = $dados['pronumeroprocesso'];
    } else {
        $urlWS                     = 'https://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
        $nu_processo                = $dados['pronumeroprocesso'];
    }

    // informações fixas
    $data_created                  = date("c");
    $nu_sistema                    = "7";
    $co_programa_fnde              = "CM";
    $an_convenio                   = null;
    $nu_convenio                   = null;
    $co_tipo_empenho               = "3";
    $nu_empenho_original           = null;
    $an_exercicio_original         = null;
    //$co_especie_empenho              = "01";
    $co_esfera_orcamentaria_solic  = "1";
    $co_observacao                 = "2";
    $co_descricao_empenho          = "0010";
    $co_gestao_emitente            = "15253";
    $co_unidade_gestora_emitente   = "153173";
    $co_unidade_orcamentaria_solic = "26298";
    $nu_proposta_siconv            = null;


    // informações variaveis
    $usuario                       = $dados['wsusuario'];
    $senha                         = $dados['wssenha'];
    $co_fonte_recurso_solic        = $dados['fonterecurso'];
    $co_plano_interno_solic        = $dados['planointerno'];
    $co_ptres_solic                = $dados['ptres'];
    $co_centro_gestao_solic        = $dados['centrogestao'];
    $nu_cnpj_favorecido            = str_replace(array(".","/","-"), array("","",""), $dados['entcnpj']);
    $vl_empenho                    = str_replace(array(".",","), array("","."), $dados['valortotalempenho']);

    if ($dados['categoriadespesa']=='C') {
        $co_natureza_despesa_solic     = $db->pegaUm("SELECT CASE WHEN i.itrid IN(1,3) THEN '33304100' ELSE '33404100' END as natureza_despesa
													FROM par3.processo p
												  	INNER JOIN par3.instrumentounidade i ON i.inuid = p.inuid
												  WHERE p.pronumeroprocesso='".$dados['pronumeroprocesso']."'");
    } elseif ($dados['categoriadespesa']=='A') {
        $co_natureza_despesa_solic     = $db->pegaUm("SELECT CASE WHEN i.itrid IN(1,3) THEN '44304200' ELSE '44404200' END as natureza_despesa
													FROM par3.processo p
												  	INNER JOIN par3.instrumentounidade i ON i.inuid = p.inuid
												  WHERE p.pronumeroprocesso='".$dados['pronumeroprocesso']."'");
    }

    $proid                         = $db->pegaUm("SELECT proid FROM par3.processo WHERE pronumeroprocesso='".$dados['pronumeroprocesso']."'");

    include_once APPRAIZ.'includes/classes/HistoricoSolicitacaoEmpenho.class.inc';
    $obRequest = new HistoricoSolicitacaoEmpenho();
    $obRequest->hsesistema = $nu_sistema;
    if ($dados['visualizarxmlsigef']) {
        $request_id = $obRequest->pegaRequest_ID();
    } else {
        $request_id = $obRequest->gravarRequest_ID();
    }

    //$request_id                    = $db->pegaUm("SELECT (COALESCE(MAX(empid),0)+113000001) as r FROM par3.empenho");

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$request_id</request_id>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

    if ($dados['visualizarxmlsigef']) {
        echo '<pre><small>';
        echo simec_htmlentities($arqXml);
        echo '</small></pre>';
        exit;
    }

    $xml = Fnde_Webservice_Client::CreateRequest()
        ->setURL($urlWS)
        ->setParams(array('xml' => $arqXml, 'method' => 'solicitar'))
        ->execute();

    $xmlarr = simplexml_load_string($xml, "SimpleXMLElement", LIBXML_NOCDATA);

    $lgwid = $db->pegaUm("INSERT INTO par3.logwssigef(proid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		  VALUES ('".$proid."', 'solicitar', 'solicitarEmpenho', '".$urlWS."', '".addslashes($arqXml)."', '".addslashes($xml)."',
								  ".((!trim($xmlarr->status->result))?"TRUE":"FALSE").", '".$_SESSION['usucpf']."',
								  '".addslashes($xmlarr->status->error->message->text)."', NOW()) RETURNING lgwid");

    $db->commit();
    $obRequest->lgwid = $lgwid;

    if (!trim($xmlarr->status->result)) {
        $_SESSION['mensagem_carregar'] = array('type'=>'error', 'text'=>'WS FNDE :'.addslashes($xmlarr->status->error->message->text));
    } else {
        $_SESSION['mensagem_carregar'] = array('type'=>'success', 'text'=>'Empenho enviado com sucesso');

        $sql = "INSERT INTO par3.empenho(empcnpj,
										 empnumerooriginal,
										 empanooriginal,
										 empnumeroprocesso,
										 empcodigoespecie,
										 empcodigopi,
										 empcodigoesfera,
										 empcodigoptres,
										 empfonterecurso,
										 empcodigonatdespesa,
										 empcentrogestaosolic,
										 empanoconvenio,
										 empnumeroconvenio,
										 empcodigoobs,
										 empcodigotipo,
										 empdescricao,
										 empgestaoeminente,
										 empunidgestoraeminente,
										 empprogramafnde,
										 empnumerosistema,
										 empsituacao, usucpf, empprotocolo, empnumero, empvalorempenho,
										 ds_problema,
										 valor_total_empenhado,
										 valor_saldo_pagamento,
										 empdata,
										 tp_especializacao,
										 co_diretoria,
										 empidpai,
										 teeid,
										 empcarga,
										 empstatus,
										 empdataatualizacao)
            
		VALUES ('".str_replace(array(".","/","-"), array("","",""), $dados['entcnpj'])."',
				".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
				".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
				'".$dados['pronumeroprocesso']."',
				'".$co_especie_empenho."',
				'".$co_plano_interno_solic."',
				".$co_esfera_orcamentaria_solic.",
				'".$co_ptres_solic."',
				'".$co_fonte_recurso_solic."',
				".$co_natureza_despesa_solic.",
				'".$co_centro_gestao_solic."',
				".(($an_convenio)?"'".$an_convenio."'":"NULL").",
				".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
				'".$co_observacao."',
				'".$co_tipo_empenho."',
				'".$co_descricao_empenho."',
				'".$co_gestao_emitente."',
				'".$co_unidade_gestora_emitente."',
				'".$co_programa_fnde."',
				'".$nu_sistema."',
				'8 - SOLICITAÇÃO APROVADA',
				'".$_SESSION['usucpf']."',
				'".$xmlarr->body->nu_seq_ne."',
				null,
				'".$vl_empenho."',
				null,
				null,
				null,
				NOW(),
				null,
				null,
				null,
				null,
				'N',
				'A',
				NOW()) RETURNING empid;";

        $empid = $db->pegaUm($sql);

        $obRequest->empid = $empid;
        $obRequest->alterar($request_id);

        $db->executar("UPDATE par3.logwssigef SET empid='".$empid."', lgwmsgretorno = '".addslashes($xmlarr->status->message->text)."' WHERE lgwid='".$lgwid."'");

        if ($dados['ppcid']) {
            foreach ($dados['ppcid'] as $ppcid) {
                $sql = "INSERT INTO par3.empenhoparcomposicao(empid, ppcid, epcpercentualemp, epcvalorempenho, epcstatus)
	    				VALUES ('".$empid."', '".$ppcid."', '".$dados['porc_empenhar'][$ppcid]."', '".str_replace(array(".",","), array("","."), $dados['valor_empenhar'][$ppcid])."', 'A');";

                $db->executar($sql);
            }
        }

        if ($dados['pocid']) {
            foreach ($dados['pocid'] as $pocid) {
                $sql = "INSERT INTO par3.empenhoobracomposicao(empid, pocid, eocpercentualemp, eocvalorempenho, eocstatus)
	    				VALUES ('".$empid."', '".$pocid."', '".$dados['porc_empenhar'][$pocid]."', '".str_replace(array(".",","), array("","."), $dados['valor_empenhar'][$pocid])."', 'A');";

                $db->executar($sql);
            }
        }
    }

    $db->commit();

    echo '<form action="par3.php?modulo=principal/orcamento/empenhoPar&acao=A" method="post" name="frmredirect">';
    echo '<input type="hidden" name="tipovisualizacao" value="html">';
    echo '<input type="hidden" name="numeroprocesso" value="'.$dados['pronumeroprocesso'].'">';
    echo '<input type="hidden" name="tipo" value="'.str_to_upper($dados['tipoobjeto']).'">';
    echo '</form>';
    echo '<script type="text/javascript">document.frmredirect.submit();</script>';
}


function exibirComposicaoIniciativaPar($dados)
{
    global $db;

    echo '<small>';

    $sql = "SELECT distinct
                CASE WHEN iig.itcid IS NOT NULL THEN itc.itcdsc ELSE igr.igrnome END as item, ppc.ppcano,
                ipi.ipivalorreferencia as valoritem, ini.intaid,
                --CASE WHEN ini.intaid = 1 THEN ipi.ipiquantidade ELSE /*esc.quantidade*/ 0 END as qtdsolicitada,
                ipi.ipiquantidade as qtdsolicitada,
                ai.aicqtdaprovado as qtdaprovada
			FROM par3.processoparcomposicao ppc
			INNER JOIN par3.iniciativa_planejamento inp ON ppc.inpid = inp.inpid
			INNER JOIN par3.iniciativa ini on ini.iniid = inp.iniid AND ini.inistatus = 'A'
			INNER JOIN par3.iniciativa_itenscomposicao_grupo iig on iig.iniid = inp.iniid --AND iig.iigsituacao = 'A'
			LEFT JOIN par3.itenscomposicao itc on itc.itcid = iig.itcid AND itc.itcstatus = 'A'
			LEFT JOIN par3.itenscomposicao_grupos igr ON igr.igrid = iig.igrid
			INNER JOIN par3.iniciativa_planejamento_item_composicao ipi on ipi.iigid = iig.iigid AND ipi.ipistatus = 'A' AND ipi.ipiano = ppc.ppcano AND ipi.inpid = inp.inpid
			INNER JOIN par3.analise a ON a.inpid = inp.inpid AND a.anastatus = 'A' AND a.anaano = ipi.ipiano
			INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'
			--LEFT JOIN ( select ipe.ipiid, sum(ipe.ipequantidade) as quantidade from par3.iniciativa_planejamento_item_composicao_escola ipe where ipe.ipestatus = 'A' group by ipe.ipiid ) esc on esc.ipiid = ipi.ipiid
			WHERE ppc.ppcid = '".$dados['ppcid']."'";

    $composicaoiniciativa = $db->carregar($sql);

    if ($composicaoiniciativa[0]) {
        echo '<table class="table table-bordered">';
        echo '<thead>';
        echo '<tr>';
        echo '<th>Item</th>';
        echo '<th>Ano</th>';
        echo '<th>Valor do Item</th>';
        echo '<th>Qtd. Solicitada</th>';
        echo '<th>Qtd. Aprovada</th>';
        echo '<th>Valor Aprovado</th>';
        echo '</tr>';
        echo '</thead>';
        echo '<tbody>';

        foreach ($composicaoiniciativa as $cpc) {
            echo '<tr>';
            echo '<td>'.$cpc['item'].'</td>';
            echo '<td>'.$cpc['ppcano'].'</td>';
            echo '<td align="right">'.number_format($cpc['valoritem'], 2, ",", ".").'</td>';
            echo '<td>'.$cpc['qtdsolicitada'].'</td>';
            echo '<td>'.$cpc['qtdaprovada'].'</td>';
            echo '<td align="right">'.number_format($cpc['valoritem']*$cpc['qtdaprovada'], 2, ",", ".").'</td>';
            echo '</tr>';

            $total += ($cpc['valoritem']*$cpc['qtdaprovada']);
        }

        echo '</tbody>';

        echo '<tfooter>';
        echo '<tr>';
        echo '<td align="right" colspan="5"><b>Total:</b></td>';
        echo '<td align="right">'.number_format($total, 2, ",", ".").'</td>';
        echo '</tr>';
        echo '</tfooter>';

        echo '</table>';

        echo '</small>';
    }
}


function exibirComposicaoIniciativaObra($dados)
{
    global $db;

    echo '<small>';

    $sql = "SELECT
				obr.obrdsc as item,
				ot.otpdsc as tipoobra,
				obr.obrvalor as valoritem,
				obr.obrvalor_contrapartida as valorcontrapartida,
				e.esddsc as situacao
			FROM par3.processoobracomposicao poc
			INNER JOIN par3.obra obr ON obr.obrid = poc.obrid
			LEFT JOIN par3.obra_tipo ot ON ot.otpid = obr.otpid
			INNER JOIN par3.iniciativa_planejamento inp ON obr.inpid = inp.inpid
			INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid AND ini.inistatus = 'A'
            LEFT JOIN workflow.documento d ON d.docid = obr.docid
            LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
			WHERE poc.pocid = '".$dados['pocid']."'";


    $composicaoiniciativa = $db->carregar($sql);

    if ($composicaoiniciativa[0]) {
        echo '<table class="table table-bordered">';
        echo '<thead>';
        echo '<tr>';
        echo '<th>Obra</th>';
        echo '<th>Tipo</th>';
        echo '<th>Valor</th>';
        echo '<th>Contrapartida</th>';
        echo '<th>Valor Aprovado</th>';
        echo '<th>Situação</th>';
        echo '</tr>';
        echo '</thead>';
        echo '<tbody>';

        foreach ($composicaoiniciativa as $cpc) {
            echo '<tr>';
            echo '<td>'.$cpc['item'].'</td>';
            echo '<td>'.$cpc['tipoobra'].'</td>';
            echo '<td align="right">'.number_format($cpc['valoritem'], 2, ",", ".").'</td>';
            echo '<td align="right">'.number_format($cpc['valorcontrapartida'], 2, ",", ".").'</td>';
            echo '<td align="right">'.number_format($cpc['valoritem']-$cpc['valorcontrapartida'], 2, ",", ".").'</td>';
            echo '<td>'.$cpc['situacao'].'</td>';
            echo '</tr>';

            $total += ($cpc['valoritem']-$cpc['valorcontrapartida']);
        }

        echo '</tbody>';

        echo '<tfooter>';
        echo '<tr>';
        echo '<td align="right" colspan="4"><b>Total:</b></td>';
        echo '<td align="right">'.number_format($total, 2, ",", ".").'</td>';
        echo '<td>&nbsp;</td>';
        echo '</tr>';
        echo '</tfooter>';

        echo '</table>';

        echo '</small>';
    }
}


function consultarEmpenho($dados)
{
    global $db;

    include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

    try {
        $data_created = date("c");

        $dadosemp = $db->pegaLinha("SELECT * FROM par3.empenho WHERE empid='".$dados['empid']."'");
        $proid = $db->pegaUm("SELECT proid FROM par3.processo WHERE pronumeroprocesso='".$dadosemp['empnumeroprocesso']."'");

        $usuario   = $dados['wsusuario'];
        $senha     = $dados['wssenha'];
        $nu_seq_ne = $dadosemp['empprotocolo'];

        $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

        /*      <?xml version="1.0" encoding="iso-8859-1"?>
         <response>
         <header>
         <app>SIGEF</app>
         <version>20.04.2018#fdec48</version>
         <created>2018-04-27T16:44:04</created>
         </header>
         <status>
         <result>1</result>
         <message>
         <code>1</code>
         <text>Consulta realizada com sucesso.</text>
         </message>
         </status>
         <body>
         <row>
         <nu_seq_ne>385012</nu_seq_ne>
         <status>0 - NE não existe.</status>
         </row>
         </body>
         </response>*/


        $urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';

        $xml = Fnde_Webservice_Client::CreateRequest()
            ->setURL($urlWS)
            ->setParams(array('xml' => $arqXml, 'method' => 'consultar'))
            ->execute();

        $xmlarr = simplexml_load_string($xml, "SimpleXMLElement", LIBXML_NOCDATA);

        $lgwid = $db->pegaUm("INSERT INTO par3.logwssigef(proid, empid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
					 		  VALUES ('".$proid."', '".$dadosemp['empid']."', 'consultar', 'consultarEmpenho', '".$urlWS."', '".addslashes($arqXml)."', '".addslashes($xml)."',
								  ".((!trim($xmlarr->status->result))?"TRUE":"FALSE").", '".$_SESSION['usucpf']."',
								  '".addslashes($xmlarr->status->error->message->text)."', NOW()) RETURNING lgwid");
        $db->commit();
        $status = (string)$xmlarr->body->row->status;
        $co_status          = substr($status, 0, 1);


        if (!trim((string)$xmlarr->status->result)) {
            $retorno = $xmlarr->status->error->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xmlarr->status->error->message->text);
            $retorno = str_ireplace("'", "", $retorno);
            $db->executar("UPDATE par3.logwssigef SET lgwmsgretorno = '{$retorno}' WHERE lgwid = $lgwid");
            echo $retorno;
        } elseif ((int)$co_status == (int)0) {
            $retorno = $xmlarr->body->row->status;
            $db->executar("UPDATE par3.logwssigef SET lgwmsgretorno = '{$retorno}' WHERE lgwid = $lgwid");
            $db->commit();
            echo "*** Detalhes da consulta ***\n";
            echo "* Protocolo : ".(($xmlarr->body->row->nu_seq_ne)?$xmlarr->body->row->nu_seq_ne:"-")."\n";
            echo "* Situação : ".$status."\n";
        } else {
            $retorno = iconv("UTF-8", "ISO-8859-1", $xmlarr->status->message->text);
            $db->executar("UPDATE par3.logwssigef SET lgwmsgretorno = '{$retorno}' WHERE lgwid = $lgwid");

            $situacaoEmpenho = iconv("UTF-8", "ISO-8859-1", $xmlarr->body->row->situacao_documento);

            if ($xmlarr->body->row->nu_cnpj) {
                $set[] = "empcnpj = '".$xmlarr->body->row->nu_cnpj."'";
            }
            if ($xmlarr->body->row->processo) {
                $set[] = "empnumeroprocesso = '".$xmlarr->body->row->processo."'";
            }
            if ($xmlarr->body->row->nu_seq_ne) {
                $set[] = "empprotocolo = '".$xmlarr->body->row->nu_seq_ne."'";
            }
            if ($xmlarr->body->row->co_especie_empenho) {
                $set[] = "empcodigoespecie = '".$xmlarr->body->row->co_especie_empenho."'";
            }
            if ($xmlarr->body->row->situacao_documento) {
                $set[] = "empsituacao = '".$situacaoEmpenho."'";
            }
            if (trim($xmlarr->body->row->numero_documento)) {
                $empnumerooriginal  = substr($xmlarr->body->row->numero_documento, 6);
                $empanooriginal     = substr($xmlarr->body->row->numero_documento, 0, 4);

                $set[] = "empnumero = '".$xmlarr->body->row->numero_documento."'";
                $set[] = "empnumerooriginal = ".($empnumerooriginal ? "'".$empnumerooriginal."'" : 'Null');
                $set[] = "empanooriginal = ".($empanooriginal ? "'".$empanooriginal."'" : 'Null');
            }
            if ($xmlarr->body->row->valor_ne) {
                $set[] = "empvalorempenho = '".$xmlarr->body->row->valor_ne."'";
            }
            if ($xmlarr->body->row->ds_problema) {
                $set[] = "ds_problema = '".$xmlarr->body->row->ds_problema."'";
            }
            if ($xmlarr->body->row->valor_total_empenhado) {
                $set[] = "valor_total_empenhado = '".$xmlarr->body->row->valor_total_empenhado."'";
            }
            if ($xmlarr->body->row->valor_saldo_pagamento) {
                $set[] = "valor_saldo_pagamento = '".$xmlarr->body->row->valor_saldo_pagamento."'";
            }
            if ($xmlarr->body->row->data_documento) {
                $set[] = "empdata = '".$xmlarr->body->row->data_documento."'";
            }
            if ($xmlarr->body->row->unidade_gestora_responsavel) {
                $set[] = "empunidgestoraeminente = '".$xmlarr->body->row->unidade_gestora_responsavel."'";
            }

            if ($set) {
                $sql = "UPDATE par3.empenho SET ".(($set)?implode(",", $set):"")." WHERE empid='".$dados['empid']."'";
                $db->executar($sql);
            }

            $db->commit();

            echo "*** Detalhes da consulta ***\n";
            echo "* Nº processo : ".(($xmlarr->body->row->processo)?$xmlarr->body->row->processo:"-")."\n";
            echo "* CNPJ : ".$xmlarr->body->row->nu_cnpj."\n";
            echo "* Valor(R$) : ".number_format((string)$xmlarr->body->row->valor_ne, 2, ",", ".")."\n";
            echo "* Data : ".$xmlarr->body->row->data_documento."\n";
            echo "* Nº documento : ".((strlen($xml->body->row->numero_documento))?$xmlarr->body->row->numero_documento:"-")."\n";
            echo "* Valor empenhado(R$) : ".((strlen($xmlarr->body->row->valor_total_empenhado))?$xmlarr->body->row->valor_total_empenhado:"-")."\n";
            echo "* Saldo pagamento(R$) : ".((strlen($xmlarr->body->row->valor_saldo_pagamento))?$xmlarr->body->row->valor_saldo_pagamento:"-")."\n";
            echo "* Situação : ".$situacaoEmpenho."\n";
            echo "* Espécie : ".$$xmlarrxml->body->row->co_especie_empenho.' - '.$situacaoEmpenho."\n\n";
        }
    } catch (Exception $e) {
        # Erro 404 página not found
        if ($e->getCode() == 404) {
            echo "Erro-Serviço Consulta empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
        }
        $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
        $erroMSG = str_replace("'", '"', $erroMSG);

        echo "Erro-WS Consultar empenho no SIGEF: $erroMSG";
    }
}


function cancelarEmpenho($dados)
{
    global $db;

    include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

    try {
        $data_created = date("c");
        $usuario = $dados['wsusuario'];
        $senha   = $dados['wssenha'];

        $dadosemp = $db->pegaLinha("SELECT e.*, op.proid, op.protipoexecucao
			 							FROM par3.empenho e
											inner join par3.processo op on e.empnumeroprocesso = op.pronumeroprocesso and op.prostatus = 'A' and empstatus = 'A'
										WHERE e.empid = '".$dados['empid']."'");

        $dadosemp['empvalorempenho'] = str_replace(array(".",","), array("","."), $dados['valor_cancelado']);

        $nu_seq_ne = $dadosemp['empprotocolo'];
        $errodecancelamento = false;

        $nu_cnpj_favorecido = $dadosemp['empcnpj'];

        if ($dadosemp['empnumero']) {
            $arrNumero = explode("NE", $dadosemp['empnumero']);
            $nu_empenho_original=$arrNumero[1];
            $an_exercicio_original=$arrNumero[0];

            #A pedido da Sâmara via e-mail dia 06/11/2013, que alterasse a espécie empenho de 03 para 13
            if ($an_exercicio_original == date('Y')) {
                $co_especie_empenho="03";
                $co_descricao_empenho = "0011";
            } else {
                $co_especie_empenho="13";
                $co_descricao_empenho = "0022";
            }
        } else {
            $nu_empenho_original=null;
            $an_exercicio_original=null;
            $co_especie_empenho="13";
            $co_descricao_empenho = "0022";
        }

        $vl_empenho                 = $dadosemp['empvalorempenho'];
        $nu_proposta_siconv         = null;
        $nu_processo                = $dadosemp['empnumeroprocesso'];
        $co_programa_fnde           = $dadosemp['empprogramafnde'];
        $nu_sistema                 = $dadosemp['empnumerosistema'];
        //$co_unidade_gestora_emitente = $dadosemp['empunidgestoraeminente'];
        $co_plano_interno_solic     = $dadosemp['empcodigopi'];
        //$co_esfera_orcamentaria_solic = $dadosemp['empcodigoesfera'];
        $co_ptres_solic             = $dadosemp['empcodigoptres'];
        $co_fonte_recurso_solic     = $dadosemp['empfonterecurso'];
        $co_natureza_despesa_solic  = $dadosemp['empcodigonatdespesa'];
        $co_centro_gestao_solic     = $dadosemp['empcentrogestaosolic'];
        $an_convenio                = $dadosemp['empanoconvenio'];
        $nu_convenio                = $dadosemp['empnumeroconvenio'];
        //$co_observacao                = $dadosemp['empcodigoobs'];
        $co_tipo_empenho            = $dadosemp['empcodigotipo'];
        //$co_descricao_empenho         = $dadosemp['empdescricao'];
        //$co_gestao_emitente       = $dadosemp['empgestaoeminente'];
        $nu_empenho_original        = $dadosemp['empnumerooriginal'];
        $an_exercicio_original      = $dadosemp['empanooriginal'];

        $co_esfera_orcamentaria_solic   = "1";
        $co_observacao                  = "2";
        $co_gestao_emitente             = "15253";
        $co_unidade_gestora_emitente    = "153173";
        $co_unidade_orcamentaria_solic  = "26298";

        //$request_id = $db->pegaUm("SELECT (COALESCE(MAX(empid),0)+113000001) as r FROM par3.empenho");

        // SE EXISTE NE
        //------------------- CANCELAR EMPENHO ----------------------//
        if ($nu_seq_ne) {
            $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;
            if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
                $urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
            } else {
                $urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
            }

            $xml = Fnde_Webservice_Client::CreateRequest()
                ->setURL($urlWS)
                ->setParams(array('xml' => $arqXml, 'method' => 'cancelar'))
                ->execute();

            $xmlRetorno = $xml;
            $xmlarr = $xml;
            $xmlarr = simplexml_load_string($xml);

            $result = (integer) $xmlarr->status->result;

            $lgwid = $db->pegaUm("INSERT INTO par3.logwssigef(proid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		         VALUES ('".$dados['proid']."', 'cancelar', 'cancelarEmpenho', '".$urlWS."', '".addslashes($arqXml)."', '".addslashes($xml)."',
								  ".((!trim($xmlarr->status->result))?"TRUE":"FALSE").", '".$_SESSION['usucpf']."',
								  '".addslashes($xmlarr->status->error->message->text)."', NOW()) RETURNING lgwid");
            $db->commit();

            if ($result) { // Se sucesso
                $errodecancelamento = false;

                $sql = "INSERT INTO par3.empenho(empcnpj,
									 empnumerooriginal,
									 empanooriginal,
									 empnumeroprocesso,
									 empcodigoespecie,
									 empcodigopi,
									 empcodigoesfera,
									 empcodigoptres,
									 empfonterecurso,
									 empcodigonatdespesa,
									 empcentrogestaosolic,
									 empanoconvenio,
									 empnumeroconvenio,
									 empcodigoobs,
									 empcodigotipo,
									 empdescricao,
									 empgestaoeminente,
									 empunidgestoraeminente,
									 empprogramafnde,
									 empnumerosistema,
									 empsituacao, usucpf, empprotocolo, empnumero, empvalorempenho,
									 ds_problema,
									 valor_total_empenhado,
									 valor_saldo_pagamento,
									 empdata,
									 tp_especializacao,
									 co_diretoria,
									 empidpai,
									 teeid,
									 empcarga,
									 empstatus,
									 empdataatualizacao)
                    
							VALUES ('".$dadosemp['empcnpj']."',
									".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
									".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
									'".$dadosemp['empnumeroprocesso']."',
									'".$co_especie_empenho."',
									'".$co_plano_interno_solic."',
									".$co_esfera_orcamentaria_solic.",
									'".$co_ptres_solic."',
									'".$co_fonte_recurso_solic."',
									".$co_natureza_despesa_solic.",
									'".$co_centro_gestao_solic."',
									".(($an_convenio)?"'".$an_convenio."'":"NULL").",
									".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
									'".$co_observacao."',
									'".$co_tipo_empenho."',
									'".$co_descricao_empenho."',
									'".$co_gestao_emitente."',
									'".$co_unidade_gestora_emitente."',
									'".$co_programa_fnde."',
									'".$nu_sistema."',
									'8 - SOLICITAÇÃO APROVADA',
									'".$_SESSION['usucpf']."',
									'".$xmlarr->body->nu_seq_ne."',
									null,
									'".$vl_empenho."',
									null,
									null,
									null,
									NOW(),
									null,
									null,
									".$dados['empid'].",
									null,
									'N',
									'A',
									NOW()) RETURNING empid;";

                $empid = $db->pegaUm($sql);

                $db->executar("UPDATE par3.logwssigef SET empid='".$empid."', lgwmsgretorno = '".addslashes($xmlarr->status->message->text)."' WHERE lgwid='".$lgwid."'");

                $sql = "SELECT 'PAR' as tipo FROM par3.empenhoparcomposicao WHERE empid = {$dados['empid']} and epcstatus = 'A'
					    union all
					    SELECT 'OBRA' as tipo FROM par3.empenhoobracomposicao WHERE empid = {$dados['empid']} and eocstatus = 'A'";
                $strTipoEmpenho = $db->pegaUm($sql);

                if ($strTipoEmpenho) {
                    foreach ($dados['codigo'] as $codigo) {
                        $porc_cancelar = str_replace(array(".",","), array("","."), $dados['porc_cancelar'][$codigo]);
                        $valor_cancelar = str_replace(array(".",","), array("","."), $dados['valor_cancelar'][$codigo]);

                        $sql = "INSERT INTO par3.empenhoparcomposicao(empid, ppcid, epcpercentualemp, epcvalorempenho, epcstatus)
						        VALUES ($empid, $codigo, {$porc_cancelar}, {$valor_cancelar}, 'A')";
                        $db->executar($sql);
                    }
                } else {
                    foreach ($dados['codigo'] as $codigo) {
                        $porc_cancelar = str_replace(array(".",","), array("","."), $dados['porc_cancelar'][$codigo]);
                        $valor_cancelar = str_replace(array(".",","), array("","."), $dados['valor_cancelar'][$codigo]);

                        $sql = "INSERT INTO par3.empenhoobracomposicao(empid, pocid, eocpercentualemp, eocvalorempenho, eocstatus)
					            VALUES ($empid, $codigo, {$porc_cancelar}, {$valor_cancelar}, 'A')";
                        $db->executar($sql);
                    }
                }

                $db->commit();

                echo "Empenho Cancelado.";
            } else {
                //echo $xmlarr->status->error->message->text;
                $errodecancelamento = true;
            }
        } //FIM IF SE EXISTE NE

        if ($errodecancelamento) {
            include_once APPRAIZ.'includes/classes/HistoricoSolicitacaoEmpenho.class.inc';
            $obRequest = new HistoricoSolicitacaoEmpenho();
            $obRequest->hsesistema = $nu_sistema;
            $request_id = $obRequest->gravarRequest_ID();

            $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$request_id</request_id>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;
            /*if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
             $urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
             } else {*/
            $urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
            //}
            //ver(simec_htmlentities($arqXml),d);
            $xml = Fnde_Webservice_Client::CreateRequest()
                ->setURL($urlWS)
                ->setParams(array('xml' => $arqXml, 'method' => 'solicitar'))
                ->execute();

            $xmlarr = simplexml_load_string($xml, "SimpleXMLElement", LIBXML_NOCDATA);

            $lgwid = $db->pegaUm("INSERT INTO par3.logwssigef(proid, empid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		  VALUES ('".$dados['proid']."', ".$dados['empid'].", 'solicitar', 'cancelarEmpenhoAnulacao', '".$urlWS."', '".addslashes($arqXml)."', '".addslashes($xml)."',
								  ".((!trim($xmlarr->status->result))?"TRUE":"FALSE").", '".$_SESSION['usucpf']."',
								  '".addslashes($xmlarr->status->error->message->text)."', NOW()) RETURNING lgwid");
            $db->commit();

            $obRequest->lgwid = $lgwid;
            $obRequest->alterar($request_id);

            if (!trim($xmlarr->status->result)) {
                echo $xmlarr->status->error->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xmlarr->status->error->message->text)."\n\n";
                //$_SESSION['mensagem_carregar'] = array('type'=>'error', 'text'=>'WS FNDE :'.addslashes($xmlarr->status->error->message->text));
            } else {
                //$_SESSION['mensagem_carregar'] = array('type'=>'success', 'text'=>'Empenho enviado com sucesso');

                $sql = "INSERT INTO par3.empenho(empcnpj,
										 empnumerooriginal,
										 empanooriginal,
										 empnumeroprocesso,
										 empcodigoespecie,
										 empcodigopi,
										 empcodigoesfera,
										 empcodigoptres,
										 empfonterecurso,
										 empcodigonatdespesa,
										 empcentrogestaosolic,
										 empanoconvenio,
										 empnumeroconvenio,
										 empcodigoobs,
										 empcodigotipo,
										 empdescricao,
										 empgestaoeminente,
										 empunidgestoraeminente,
										 empprogramafnde,
										 empnumerosistema,
										 empsituacao, usucpf, empprotocolo, empnumero, empvalorempenho,
										 ds_problema,
										 valor_total_empenhado,
										 valor_saldo_pagamento,
										 empdata,
										 tp_especializacao,
										 co_diretoria,
										 empidpai,
										 teeid,
										 empcarga,
										 empstatus,
										 empdataatualizacao)
							VALUES ('".$dadosemp['empcnpj']."',
										".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
										".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
										'".$dadosemp['empnumeroprocesso']."',
										'".$co_especie_empenho."',
										'".$co_plano_interno_solic."',
										".$co_esfera_orcamentaria_solic.",
										'".$co_ptres_solic."',
										'".$co_fonte_recurso_solic."',
										".$co_natureza_despesa_solic.",
										'".$co_centro_gestao_solic."',
										".(($an_convenio)?"'".$an_convenio."'":"NULL").",
										".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
										'".$co_observacao."',
										'".$co_tipo_empenho."',
										'".$co_descricao_empenho."',
										'".$co_gestao_emitente."',
										'".$co_unidade_gestora_emitente."',
										'".$co_programa_fnde."',
										'".$nu_sistema."',
										'8 - SOLICITAÇÃO APROVADA',
										'".$_SESSION['usucpf']."',
										'".$xmlarr->body->nu_seq_ne."',
										null,
										'".$vl_empenho."',
										null,
										null,
										null,
										NOW(),
										null,
										null,
										".$dados['empid'].",
										null,
										'N',
										'A',
										NOW()) RETURNING empid;";

                $empid = $db->pegaUm($sql);

                $obRequest->empid = $empid;
                $obRequest->alterar($request_id);

                $db->executar("UPDATE par3.logwssigef SET empid='".$empid."', lgwmsgretorno = '".addslashes($xmlarr->status->message->text)."' WHERE lgwid='".$lgwid."'");

                $sql = "SELECT 'PAR' as tipo FROM par3.empenhoparcomposicao WHERE empid = {$dados['empid']} and epcstatus = 'A'
						union all
						SELECT 'OBRA' as tipo FROM par3.empenhoobracomposicao WHERE empid = {$dados['empid']} and eocstatus = 'A'";
                $strTipoEmpenho = $db->pegaUm($sql);


                if ($strTipoEmpenho == 'PAR') {
                    foreach ($dados['codigo'] as $codigo) {
                        $porc_cancelar = str_replace(array(".",","), array("","."), $dados['porc_cancelar'][$codigo]);
                        $valor_cancelar = str_replace(array(".",","), array("","."), $dados['valor_cancelar'][$codigo]);

                        $sql = "INSERT INTO par3.empenhoparcomposicao(empid, ppcid, epcpercentualemp, epcvalorempenho, epcstatus)
							    VALUES ($empid, $codigo, {$porc_cancelar}, {$valor_cancelar}, 'A')";
                        $db->executar($sql);
                    }
                } else {
                    foreach ($dados['codigo'] as $codigo) {
                        $porc_cancelar = str_replace(array(".",","), array("","."), $dados['porc_cancelar'][$codigo]);
                        $valor_cancelar = str_replace(array(".",","), array("","."), $dados['valor_cancelar'][$codigo]);

                        $sql = "INSERT INTO par3.empenhoobracomposicao(empid, pocid, eocpercentualemp, eocvalorempenho, eocstatus)
						        VALUES ($empid, $codigo, {$porc_cancelar}, {$valor_cancelar}, 'A')";
                        $db->executar($sql);
                    }
                }
                $db->commit();

                echo "Empenho Cancelado com sucesso.";
            }
        }
    } catch (Exception $e) {
        # Erro 404 página not found
        if ($e->getCode() == 404) {
            echo "Erro-Serviço Cancelar Empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
        }
        $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
        $erroMSG = str_replace("'", '"', $erroMSG);

        echo "Erro-WS Cancelar Empenho no SIGEF: $erroMSG";
    }
}


function exibirComposicaoEmpenho($dados)
{
    global $db;

    $empenho = $db->pegaLinha("SELECT empprotocolo, empnumero, empcodigopi, empcodigoptres, empfonterecurso, empcentrogestaosolic FROM par3.empenho WHERE empid='".$dados['empid']."'");

    echo '<small>';
    echo '<table class="table">';
    echo '<tbody>';

    echo '<tr>';
    echo '<td align="right" width="15%"><b>Nº empenho:</b></td>';
    echo '<td width="35%">'.$empenho['empnumero'].'</td>';
    echo '<td align="right" width="15%"><b>Protocolo:</b></td>';
    echo '<td width="35%">'.$empenho['empprotocolo'].'</td>';
    echo '</tr>';

    echo '<tr>';
    echo '<td align="right" width="15%"><b>Plano Interno:</b></td>';
    echo '<td width="35%">'.$empenho['empcodigopi'].'</td>';
    echo '<td align="right" width="15%"><b>Fonte de Recurso:</b></td>';
    echo '<td width="35%">'.$empenho['empfonterecurso'].'</td>';
    echo '</tr>';
    echo '<tr>';
    echo '<td align="right" width="15%"><b>Centro de Gestão:</b></td>';
    echo '<td width="35%">'.$empenho['empcentrogestaosolic'].'</td>';
    echo '<td align="right" width="15%"><b>PTRes:</b></td>';
    echo '<td width="35%">'.$empenho['empcodigoptres'].'</td>';
    echo '</tr>';
    echo '</tbody>';
    echo '</table>';

    $sql = "SELECT ppc.inpid, ppc.ppcano as anoitem,
					ind.inddsc as descricaoitem,
					ana.vlr_item,
					CASE WHEN ana.vlr_item > 0 THEN ((epc.epcvalorempenho/ana.vlr_item)*100) ELSE 0 END as porc_empenhado,
					epc.epcvalorempenho as vlr_empenhado
			FROM par3.empenhoparcomposicao epc
			INNER join par3.processoparcomposicao ppc on ppc.ppcid = epc.ppcid and ppc.ppcstatus = 'A'
			INNER join par3.iniciativa_planejamento inp on inp.inpid = ppc.inpid AND inp.inpstatus = 'A'
			INNER JOIN(
				SELECT inpid, anaid, ipiano, sum(vlr_item) AS vlr_item FROM(
					SELECT DISTINCT ipi.inpid, ipi.ipiid, ai.anaid, ipi.ipiano, (ai.aicqtdaprovado * ai.aicvaloraprovado) AS vlr_item FROM par3.iniciativa_planejamento_item_composicao ipi
						INNER JOIN par3.analise_itemcomposicao ai ON ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'
					WHERE ipi.ipistatus = 'A'
				) AS foo
				GROUP BY inpid, anaid, ipiano
			) ana ON ana.inpid = inp.inpid AND ana.anaid = ppc.anaid AND ana.ipiano = ppc.ppcano
			INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid AND ini.inistatus = 'A'
			INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid AND ind.indstatus='A'
			WHERE epc.empid = {$_REQUEST['empid']}
            union all
            SELECT ppc.obrid AS inpid, o.obrano as anoitem,
            		o.obrdsc as descricaoitem,
            		(o.obrvalor - coalesce(o.obrvalor_contrapartida,0)) AS vlr_item,
            		CASE WHEN (o.obrvalor - coalesce(o.obrvalor_contrapartida,0)) > 0 THEN ((epc.eocvalorempenho/(o.obrvalor - coalesce(o.obrvalor_contrapartida,0)))*100) ELSE 0 END as porc_empenhado,
            		epc.eocvalorempenho as vlr_empenhado
            FROM par3.empenhoobracomposicao epc
            	INNER join par3.processoobracomposicao ppc on ppc.pocid = epc.pocid AND ppc.pocstatus = 'A'
            	INNER join par3.obra o ON o.obrid = ppc.obrid AND o.obrstatus = 'A'
            WHERE epc.empid = {$_REQUEST['empid']}";

    $empenhoparcomposicao = $db->carregar($sql);

    if ($empenhoparcomposicao[0]) {
        echo '<table class="table table-bordered">';
        echo '<thead>';
        echo '<tr>';
        echo '<th>Planejamento</th>';
        echo '<th>Ano</th>';
        echo '<th>Descrição</th>';
        echo '<th>Valor Aprovado</th>';
        echo '<th>% Empenhado</th>';
        echo '<th>Valor Empenhado</th>';
        echo '</tr>';
        echo '</thead>';
        echo '<tbody>';

        foreach ($empenhoparcomposicao as $epc) {
            echo '<tr>';
            echo '<td>'.$epc['inpid'].'</td>';
            echo '<td>'.$epc['anoitem'].'</td>';
            echo '<td>'.$epc['descricaoitem'].'</td>';
            echo '<td align="right">'.number_format($epc['vlr_item'], 2, ",", ".").'</td>';
            echo '<td align="right">'.number_format_par3($epc['porc_empenhado']).'%</td>';
            echo '<td align="right">'.number_format($epc['vlr_empenhado'], 2, ",", ".").'</td>';
            echo '</tr>';
        }

        echo '</tbody>';
        echo '</table>';
    }

    $sql = "SELECT e.empnumero,
				   e.empvalorempenho,
				   u.usunome||' - '||to_char(e.empdata,'dd/mm/YYYY HH24:MI') as usuario,
				   e.empprotocolo
			FROM par3.empenho e
			INNER JOIN seguranca.usuario u ON u.usucpf = e.usucpf
			WHERE empidpai='".$_REQUEST['empid']."' and empstatus = 'A'";

    $empenhocancelado = $db->carregar($sql);

    if ($empenhocancelado[0]) {
        echo '<span class="label label-danger">Cancelamentos</span><br><br>';

        echo '<table class="table table-bordered">';
        echo '<thead>';
        echo '<tr>';
        echo '<th>Número do Cancelamento</th>';
        echo '<th>Valor Cancelado</th>';
        echo '<th>Usuário</th>';
        echo '<th>Número protocolo</th>';
        echo '</tr>';
        echo '</thead>';
        echo '<tbody>';

        foreach ($empenhocancelado as $emc) {
            echo '<tr>';
            echo '<td>'.$emc['empnumero'].'</td>';
            echo '<td align="right">'.number_format($emc['empvalorempenho'], 2, ",", ".").'</td>';
            echo '<td>'.$emc['usuario'].'</td>';
            echo '<td>'.$emc['empprotocolo'].'</td>';

            echo '</tr>';
        }

        echo '</tbody>';
        echo '</table>';
    }


    echo '</small>';
}

function solicitarEmpenhoPar($dados)
{
    global $db, $simec;

    include_once APPRAIZ.'/par3/modulos/principal/orcamento/solicitacaoEmpenhoPar.inc';
}



/////////////////////////////// funções pagamentos


function listaComposicaoEmpenho($dados)
{
    global $db, $simec;

    $empenho = $db->pegaLinha("SELECT empid, empprotocolo, empnumero, empcodigopi, empcodigoptres, empfonterecurso, empcentrogestaosolic, empnumeroprocesso FROM par3.empenho WHERE empid='".$dados['empid']."'");

    echo '<div class="panel panel-success">';
    echo '<div class="panel-heading">Dados do Empenho</div>';

    echo '<div class="panel-body">';

    echo '<table class="table">';
    echo '<tbody>';

    echo '<tr>';
    echo '<td align="right" width="15%"><b>Nº empenho:</b></td>';
    echo '<td width="35%">'.$empenho['empnumero'].'</td>';
    echo '<td align="right" width="15%"><b>Protocolo:</b></td>';
    echo '<td width="35%">'.$empenho['empprotocolo'].'</td>';
    echo '</tr>';

    echo '<tr>';
    echo '<td align="right" width="15%"><b>Plano Interno:</b></td>';
    echo '<td width="35%">'.$empenho['empcodigopi'].'</td>';
    echo '<td align="right" width="15%"><b>Fonte de Recurso:</b></td>';
    echo '<td width="35%">'.$empenho['empfonterecurso'].'</td>';
    echo '</tr>';
    echo '<tr>';
    echo '<td align="right" width="15%"><b>Centro de Gestão:</b></td>';
    echo '<td width="35%">'.$empenho['empcentrogestaosolic'].'</td>';
    echo '<td align="right" width="15%"><b>PTRes:</b></td>';
    echo '<td width="35%">'.$empenho['empcodigoptres'].'</td>';
    echo '</tr>';
    echo '</tbody>';
    echo '</table>';

    $sql = "SELECT
				   p.pagid,
				   p.pagparcela as parcela,
				   COALESCE(p.pagnumeroob,'-') as numeropagamento,
				   p.pagsituacaopagamento as situacaopagamento,
				   '<small>'||u.usunome||' - '||to_char(p.pagdatapagamento,'dd/mm/YYYY HH24:MI')||'</small>' as usuario,
				   p.pagvalorparcela as valorpagamento
				FROM par3.pagamento p
				LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf
				WHERE p.empid='".$dados['empid']."'";



    $pagamentos = $db->carregar($sql);

    if ($pagamentos[0]) {
        echo '<div class="panel panel-default">';
        echo '<div class="panel-heading">Lista de Empenho(s) do Processo</div>';

        echo '<div class="panel-body">';
        echo '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">';
        echo '<thead>';
        echo '<tr>';
        echo '<th>&nbsp;</th>';
        echo '<th>Número do Pagamento</th>';
        echo '<th>Situação do Pagamento</th>';
        echo '<th>Usuário</th>';
        echo '<th>Valor Pagamento</th>';
        echo '</tr>';
        echo '</thead>';
        echo '<tbody>';

        foreach ($pagamentos as $pag) {
            echo '<tr>';
            echo '<td><span class="btn btn-primary btn-sm glyphicon glyphicon-search" style="cursor:pointer;" onclick="exibirComposicaoEmpenho('.$emp['empid'].');"></span></td>';
            echo '<td>'.$pag['numeroempenho'].'</td>';
            echo '<td>'.$pag['situacaoempenho'].'</td>';
            echo '<td>'.$pag['usuario'].'</td>';
            echo '<td>'.number_format($emp['valorpagamento'], 2, ",", ".").'</td>';
            echo '</tr>';
        }


        echo '</tbody>';
        echo '</table>';
        echo '</div>';
        echo '</div>';
    }

    echo '<div class="panel panel-default">';
    echo '<div class="panel-heading">Iniciativas vinculadas ao Empenho</div>';

    echo '<div class="panel-body">';

    $sql = "SELECT  epc.epcid as id,
					CASE WHEN epc.epcvalorempenho>= SUM(ppc.ppcvalorpagamento) THEN '<span class=\"btn btn-primary btn-sm glyphicon glyphicon-ok\"></span>'
				    ELSE '<input type=\"checkbox\" name=\"epcid[]\" onchange=\"selecionarSubacaoPagamento(' || epc.epcid || ');\" class=\"js-switch\" id=\"chk_'||epc.epcid||'\" value=\"'||epc.epcid||'\" >' END as btn,
					ppa.ppcano as anoitem,
					ind.inddsc as descricaoitem,
					epc.epcvalorempenho as vlr_empenhado,
					SUM(COALESCE(ppc.ppcpercentualpag,0)) as ppcpercentualpag,
					SUM(COALESCE(ppc.ppcvalorpagamento,0)) as ppcvalorpagamento
			FROM par3.empenhoparcomposicao epc
			INNER join par3.processoparcomposicao ppa on ppa.ppcid = epc.ppcid
			INNER join par3.iniciativa_planejamento inp on inp.inpid = ppa.inpid AND inp.inpstatus = 'A'
			INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid AND ini.inistatus = 'A'
			INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid AND ind.indstatus='A'
			LEFT JOIN par3.pagamentoparcomposicao ppc ON ppc.epcid = epc.epcid AND ppc.ppcstatus='A'
			WHERE epc.empid = '".$dados['empid']."'
			GROUP BY epc.epcid, epc.epcvalorempenho, ppa.ppcano, ind.inddsc, epc.epcvalorempenho";

    $pagamentoparcomposicao = $db->carregar($sql);

    if ($pagamentoparcomposicao[0]) {
        echo '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">';
        echo '<thead>';
        echo '<tr>';
        echo '<th>&nbsp;</th>';
        echo '<th>Ano</th>';
        echo '<th>Descrição</th>';
        echo '<th>Valor Empenhado</th>';
        echo '<th>% Pago</th>';
        echo '<th>Valor Pago</th>';
        echo '<th>% a Pagar</th>';
        echo '<th>Valor a Pagar</th>';
        echo '</tr>';
        echo '</thead>';
        echo '<tbody>';

        foreach ($pagamentoparcomposicao as $ppc) {
            echo '<tr>';
            echo '<td>'.$ppc['btn'].'</td>';
            echo '<td>'.$ppc['anoitem'].'</td>';
            echo '<td>'.$ppc['descricaoitem'].'</td>';
            echo '<td align="right">'.number_format($ppc['vlr_empenhado'], 2, ",", ".").'<input type="hidden" id="totalitem_'.$ppc['id'].'" value="'.$ppc['vlr_empenhado'].'"></td>';
            echo '<td>'.floor(($ppc['ppcpercentualpag']/$ppc['vlr_empenhado'])*100).'%<input type="hidden" id="porcpago_'.$ppc['id'].'" value="'.floor(($ppc['ppcpercentualpag']/$ppc['vlr_empenhado'])*100).'"></td>';
            echo '<td align="right">'.number_format($ppc['ppcvalorpagamento'], 2, ",", ".").'</td>';
            echo '<td><input name="porc_pagar['.$ppc['id'].']" id="porc_pagar_'.$ppc['id'].'" readonly="readonly" type="text" value="" class="form-control" maxlength="255" style="text-align: right; width: 100px;" oninput="this.value = mascaraglobal(\'###\', this.value); calcularIniciativas(\''.$ppc['id'].'\', \'porcentagem\')"></td>';
            echo '<td><input name="valor_pagar['.$ppc['id'].']" id="valor_pagar_'.$ppc['id'].'" readonly="readonly" type="text" value="" class="form-control" maxlength="255" required="required" style="text-align: right; width: 200px;" oninput="this.value = mascaraglobal(\'###.###.###.###,##\', this.value); calcularIniciativas(\''.$ppc['id'].'\', \'valor\');"></td>';
            echo '</tr>';
        }

        echo '</tbody>';
        echo '</table>';
    }

    echo '</div>';
    echo '</div>';

    echo '<div class="ibox-footer">';
    echo '<div class="form-actions col-md-offset-5">';
    echo '<button type="button" class="btn btn-success" onclick="solicitarPagamento();"><i class="fa fa-plus-square-o"></i> Solicitar Pagamento</button> ';
    echo '<button type="button" class="btn btn-info" onclick="listaComposicaoEmpenho(\'S\');">Sair</button>';
    echo '</div>';
    echo '</div>';



    echo '</div>';

    echo '</div>';
    echo '</div>';


    echo '<div class="ibox float-e-margins animated modal" id="dvconfirmarpagamento" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">';
    echo '<div class="modal-dialog modal-lg">';
    echo '<div class="modal-content">';
    echo '<div class="ibox-title">';
    echo '<h5><i class="fa fa-angle-double-right"></i> Confirmar Pagamento</h5>';
    echo '</div>';

    echo '<div class="ibox-content">';
    echo '<input type="hidden" name="requisicao" id="requisicao" value="confirmarPagamento">';
    echo '<input type="hidden" name="empid" id="empid" value="'.$empenho['empid'].'">';
    echo $simec->input('pronumeroprocesso', 'Número do processo', $empenho['empnumeroprocesso'], array('maxlength' => '255', 'required', 'readonly'));
    echo $simec->input('pagnumeroempenho', 'Número do empenho', $empenho['empnumero'], array('maxlength' => '255', 'required', 'readonly'));
    echo $simec->input('valortotalpagamento', 'Valor Total do Pagamento', "", array('maxlength' => '255', 'required', 'readonly'));

    $parcela = $db->pegaUm("SELECT (COALESCE(MAX(pagparcela),0)+1) as parcela FROM par3.pagamento WHERE empid = ".$dados['empid']." AND pagstatus='A'");
    echo $simec->input('pagparcela', 'Parcela', $parcela, array('maxlength' => '255', 'required', 'readonly'));

    $sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos ORDER BY ano";
    echo $simec->select('ano', 'Ano', date("Y"), $sql_ano, array('required'));

    $sql_mes = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
    echo $simec->select('mescod', 'Mês', date("m"), $sql_mes, array('required'));

    echo '<div class="panel panel-default">';
    echo '<div class="panel-heading">Autenticação SIGEF</div>';
    echo '<div class="panel-body">';
    echo $simec->input('wsusuario', 'Usuário', "", array('maxlength' => '255', 'required'));
    echo '<div class="form-group ">';
    echo '<label for="wssenha" class="col-sm-3 col-md-3 col-lg-3 control-label">Senha: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>';
    echo '<div class="col-sm-9 col-md-9 col-lg-9 ">';
    echo '<input name="wssenha" id="wssenha" type="password" value="" class="form-control" maxlength="255" required="required">';
    echo '</div>';
    echo '<div style="clear:both"></div>';
    echo '</div>';
    echo '</div>';
    echo '</div>';
    echo '</div>';
    echo '<div class="ibox-footer">';
    echo '<div class="form-actions col-md-offset-2">';
    echo '<button type="button" class="btn btn-success" onclick="confirmarPagamento();"> <i class="fa fa-plus-square-o"></i> Pagar</button> ';
    echo '<button type="button" class="btn btn-danger" onclick="visualizarXMLSigef();"> <i class="fa fa-eye" aria-hidden="true"></i> Visualizar XML SIGEF</button> ';
    echo '<button type="button" class="btn btn-white" onclick="$(\'#dvconfirmarpagamento\').modal(\'toggle\');$(\'#modal-form-large\').modal(\'show\');">Fechar</button> ';
    echo '</div>';
    echo '</div>';
    echo '</div>';
    echo '</div>';
    echo '</div>';
}


function relatorioPagamentoExcel($dados)
{
    global $db;

    if ($dados['numeroprocesso']) {
        $wh[] = "pro.pronumeroprocesso='".$dados['numeroprocesso']."'";
    }
    if ($dados['anoprocesso']) {
        $wh[] = "substring(pro.pronumeroprocesso,12,4)='".$dados['anoprocesso']."'";
    }
    if ($dados['estuf']) {
        $wh[] = "m.estuf='".$dados['estuf']."'";
    }
    if ($dados['municipio']) {
        $wh[] = "m.mundescricao ilike removeacento('%".$dados['municipio']."%')";
    }
    if ($dados['programa']) {
        $wh[] = "pro.proid IN(SELECT ppc.proid FROM par3.processoparcomposicao ppc
			  									INNER join par3.iniciativa_planejamento inp ON ppc.inpid = inp.inpid AND inp.inpstatus = 'A'
			  									INNER JOIN par3.iniciativa_iniciativas_programas prg ON inp.iniid = prg.iniid
			  									WHERE ppc.ppcstatus = 'A' AND prg.prgid='".$dados['programa']."')";
    }
    if ($_POST['esfera']) {
        if ($dados['esfera']=='E') {
            $wh[] = "inu.estuf IS NOT NULL";
        }
        if ($dados['esfera']=='M') {
            $wh[] = "inu.muncod IS NOT NULL";
        }
    }


    $sql = "(
				SELECT pro.pronumeroprocesso,
					   'PAR' as tipo,
					   m.estuf,
					   m.mundescricao,
					   ind.inddsc,
					   ppc.ppcano as ano,
					   COALESCE(emp.empnumero,'-') as empnumero,
					   COALESCE(ec.epcpercentualemp,0) as porcen,
					   COALESCE(ec.epcvalorempenho,0) as valoremp
        
				FROM
				                par3.processo pro
				INNER join par3.processoparcomposicao ppc on ppc.proid = pro.proid
				INNER join par3.iniciativa_planejamento inp on inp.inpid = ppc.inpid AND inp.inuid = pro.inuid AND inp.inpstatus = 'A'
				INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid AND ini.inistatus = 'A'
				INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid AND ind.indstatus='A'
				LEFT JOIN par3.empenho emp ON emp.empnumeroprocesso = pro.pronumeroprocesso
				LEFT JOIN par3.empenhoparcomposicao ec ON ec.empid = emp.empid
				LEFT JOIN territorios.municipio m ON m.muncod = pro.muncod
				".(($wh)?"WHERE ".implode(" AND ", $wh):"")."
				    
				    
        		) UNION ALL (
				    
				SELECT pro.pronumeroprocesso,
					   'OBRA' as tipo,
					   m.estuf,
					   m.mundescricao,
					   ind.inddsc,
					   obr.obrano as ano,
					   COALESCE(emp.empnumero,'-') as empnumero,
					   COALESCE(ec.eocpercentualemp,0) as porcen,
					   COALESCE(ec.eocvalorempenho,0) as valoremp
				FROM
				                par3.obra obr
				INNER JOIN par3.processoobracomposicao poc ON poc.obrid = obr.obrid AND pocstatus = 'A'
				INNER JOIN par3.processo pro ON pro.proid = poc.proid AND pro.prostatus = 'A'
				INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = obr.inpid AND inpstatus = 'A'
        		INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid AND ini.inistatus = 'A'
        		INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid AND ind.indstatus='A'
				LEFT JOIN par3.empenho emp ON emp.empnumeroprocesso = pro.pronumeroprocesso
				LEFT JOIN par3.empenhoobracomposicao ec ON ec.empid = emp.empid
				LEFT JOIN territorios.municipio m ON m.muncod = pro.muncod
				".(($wh)?"WHERE ".implode(" AND ", $wh):"")."
				    
				)";


    ob_clean();
    /* configurações */
    ini_set("memory_limit", "2048M");
    set_time_limit(0);
    /* FIM configurações */
    header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=rel_pagamentos_".date("Ymdhis").".xls");
    header("Content-Disposition: attachment; filename=rel_empenho_".date("Ymdhis").".xls");
    header("Content-Description: MID Gera excel");
    $cabecalho = array("Processo","Tipo","UF","Município","Iniciativa","Ano","Nota do Empenho","% Empenho","Valor Empenhado");
    $db->monta_lista_tabulado($sql, $cabecalho, 100000000, 5, 'N', '100%', '');
}


function confirmarPagamento($dados)
{
    global $db;

    include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

    // informações fixas
    $data_created               = date("c");

    $nu_cpf_favorecido          = null;
    $nu_convenio_siafi          = null;
    $nu_proposta_siconv         = null;
    $termo_aditivo_original     = null;
    $apostilamento_original     = null;
    $darf                       = null;
    $tp_avaliador               = null;
    $id_solicitante             = null;
    $sub_tipo_documento         = "01";
    $unidade_gestora            = "153173";
    $gestao                     = "15253";
    $co_programa_fnde           = "CM";

    $dadosse = $db->pegaLinha("SELECT emp.empcnpj, pro.proseqconta, pro.seq_conta_corrente,
									  emp.empnumeroprocesso, emp.empprogramafnde,
									  emp.empnumerosistema, emp.empanooriginal,
									  emp.empnumero, pro.protipoexecucao, emp.empnumeroconvenio,
									  emp.empanoconvenio, pro.pronumeroconveniosiafi,
									  probanco, proagencia, nu_conta_corrente, empcodigonatdespesa
							   FROM par3.empenho emp
							   INNER JOIN par3.processo pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and empstatus = 'A'
							   WHERE empid='".$dados['empid']."'");

    $usuario                       = $dados['wsusuario'];
    $senha                         = $dados['wssenha'];
    $nu_processo                   = $dadosse['empnumeroprocesso'];
    $nu_documento_siafi_ne         = substr($dadosse['empnumero'], strpos($dadosse['empnumero'], 'NE')+2);
    $nu_cgc_favorecido             = $dadosse['empcnpj'];
    $nu_seq_conta_corrente_favorec = $dadosse['proseqconta'];
    $empnumero                     = $dadosse['empnumero'];


    $nu_banco                      = $dadosse['probanco'];
    $nu_agencia                    = $dadosse['proagencia'];
    $nu_conta_corrente             = $dadosse['nu_conta_corrente'];
    $an_convenio_original          = $dadosse['empanoconvenio'];
    $nu_convenio_original          = $dadosse['empnumeroconvenio'];
    $an_convenio                   = $dadosse['empanoconvenio'];
    $nu_sistema                    = $dadosse['empnumerosistema'];

    $nu_mes                        = sprintf("%02d", $dados['mescod']);
    $an_parcela                    = $dados['ano'];
    $an_exercicio                  = date("Y");


    // Custeio
    if ($dadosse['empcodigonatdespesa'] == '33304100' || $dadosse['empcodigonatdespesa'] == '33404100') {
        $vl_custeio = str_replace(array(".",","), array("","."), $dados['valortotalpagamento']);
        $vl_capital = "0";
    } else { // Capital
        $vl_custeio = "0";
        $vl_capital = str_replace(array(".",","), array("","."), $dados['valortotalpagamento']);
    }

    $parcela = $dados['pagparcela'];

    $valor = str_replace(array(".",","), array("","."), $dados['valortotalpagamento']);
    $numeroconvenioSIAFI = $dadosse['pronumeroconveniosiafi'];
    $request_id = $db->pegaUm("SELECT (COALESCE(MAX(pagid),0)+1) as r FROM par3.pagamento");

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<request_id>$request_id</request_id>
			<nu_cgc_favorecido>$nu_cgc_favorecido</nu_cgc_favorecido>
			<nu_seq_conta_corrente_favorec>$nu_seq_conta_corrente_favorec</nu_seq_conta_corrente_favorec>
			<nu_processo>$nu_processo</nu_processo>
			<vl_custeio>$vl_custeio</vl_custeio>
			<vl_capital>$vl_capital</vl_capital>
			<an_referencia>$an_referencia</an_referencia>
			<sub_tipo_documento>$sub_tipo_documento</sub_tipo_documento>
			<nu_sistema>$nu_sistema</nu_sistema>
			<unidade_gestora>$unidade_gestora</unidade_gestora>
			<gestao>$gestao</gestao>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<detalhamento_pagamento>
			<item>
				<nu_parcela>$parcela</nu_parcela>
				<an_exercicio>$an_exercicio</an_exercicio>
				<vl_parcela>$valor</vl_parcela>
				<an_parcela>$an_parcela</an_parcela>
				<nu_mes>$nu_mes</nu_mes>
				<nu_documento_siafi_ne>$nu_documento_siafi_ne</nu_documento_siafi_ne>
			</item>
			</detalhamento_pagamento>
		</params>
	</body>
</request>
XML;

    if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
        $urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
    } else {
        $urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
    }

    try {
        $xml = Fnde_Webservice_Client::CreateRequest()
            ->setURL($urlWS)
            ->setParams(array('xml' => $arqXml, 'method' => 'solicitar'))
            ->execute();

        $xmlarr = simplexml_load_string($xml, "SimpleXMLElement", LIBXML_NOCDATA);

        $lgwid = $db->pegaUm("INSERT INTO par3.logwssigef(empid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		  	  VALUES ('".$dados['empid']."', 'solicitar', 'solicitarPagamento', '".$urlWS."', '".addslashes($arqXml)."', '".addslashes($xml)."',
								  ".((!trim($xmlarr->status->result))?"TRUE":"FALSE").", '".$_SESSION['usucpf']."',
								  '".addslashes($xmlarr->status->error->message->text)."', NOW()) RETURNING lgwid");

        if (!trim($xmlarr->status->result)) {
            ob_clean();

            if (count($xmlarr->status->error->message)) {
                foreach ($xmlarr->status->error->message as $er) {
                    $erros[] = addslashes($er->text);
                }
                echo implode(' | ', $erros);
            } else {
                echo $xmlarr->status->error->message->text;
            }
        } else {
            $sql = "INSERT INTO par3.pagamento(
					pagparcela, paganoexercicio, pagvalorparcela, paganoparcela,
					pagmes, pagnumeroempenho, empid, usucpf, pagsituacaopagamento,
					pagdatapagamento, pagnumseqob, pagstatus)
					VALUES ('".$parcela."', '".$an_exercicio."', '".$valor."', '".$an_parcela."',
							'".$nu_mes."', '".$empnumero."', '".$dados['empid']."', '".$_SESSION['usucpf']."', 'Enviado ao SIGEF',
							NOW(), ".(($xmlarr->body->nu_registro_ob)?"'".$xmlarr->body->nu_registro_ob."'":"NULL").", 'A') RETURNING pagid;";

            $pagid = $db->pegaUm($sql);

            $db->executar("UPDATE par3.logwssigef SET pagid='".$pagid."' WHERE lgwid='".$lgwid."'");

            if ($dados['epcid']) {
                foreach ($dados['epcid'] as $epcid) {
                    $db->executar("INSERT INTO par3.pagamentoparcomposicao(pagid, epcid, ppcpercentualpag, ppcvalorpagamento, ppcstatus)
    							   VALUES ('".$pagid."', '".$epcid."', '".$dados['porc_pagar'][$epcid]."', '".str_replace(array(".",","), array("","."), $dados['valor_pagar'][$epcid])."', 'A');");
                }
            }
            ob_clean();
            echo 'ok';
        }

        $db->commit();
    } catch (Exception $e) {
        # Erro 404 página not found
        if ($e->getCode() == 404) {
            echo "Erro-Serviço Solicitar Pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
        }
        $erroMSG = str_replace(array(chr(13),chr(10)), ' ', $e->getMessage());
        $erroMSG = str_replace("'", '"', $erroMSG);

        $db->executar("INSERT INTO par3.logwssigef(empid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 	   VALUES ('".$dados['empid']."', 'solicitar', 'solicitarPagamento', '".$urlWS."', '".addslashes($arqXml)."', '".addslashes($xml)."',
								  TRUE, '".$_SESSION['usucpf']."', 'Erro-WS: ".$erroMSG."', NOW()) RETURNING lgwid");

        $db->commit();
    }
}


function solicitarPagamentoPar($dados)
{
    global $db, $simec;

    include_once APPRAIZ.'/par3/modulos/principal/orcamento/solicitacaoPagamentoPar.inc';
}

function valida_empenho_cancela($dados)
{
    global $db;

    $sql = "SELECT count(dt.dotid) FROM par3.processo pp
            	INNER JOIN par3.documentotermo dt ON dt.proid = pp.proid AND dt.dotstatus = 'A'
            	INNER JOIN par3.empenho e ON e.empnumeroprocesso = pp.pronumeroprocesso
            WHERE pp.prostatus = 'A' AND e.empid = {$dados['empid']}";

    $arrPerfil = pegaPerfils($_SESSION['usucpf']);

    if( in_array( PAR3_PERFIL_ADMINISTRADOR, $arrPerfil) || in_array( PAR3_PERFIL_SUPER_USUARIO, $arrPerfil) || in_array( PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDO, $arrPerfil) ){
        $boTem = 0;
    } else {
        $boTem = $db->pegaUm($sql);
    }

    if ((int)$boTem > (int)0) {
        echo 'S';
    } else {
        echo 'N';
    }
}
function lista_empenho_cancela($dados)
{
    global $db;

    $empid = $dados['empid'];
    $sql = "SELECT distinct processo, empprotocolo, empenho_original, saldo, vlrpago FROM par3.v_saldo_empenho_por_iniciativa WHERE empid = {$empid}
            UNION ALL
            SELECT distinct processo, empprotocolo, empenho_original, saldo, vlrpago FROM par3.v_saldo_empenho_por_obra WHERE empid = {$empid}";

    $list = new Simec_Listagem();
    $list->setCabecalho(array('Processo', 'Protocolo', 'Nota do Empenho', 'Valor Empenhado', 'Valor Pago'));
    $list->setQuery($sql);
    $list->addCallbackDeCampo(array('saldo', 'vlrpago'), 'formata_numero_monetario');
    $list->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    $sql = "SELECT
                pp.ppcid as id,
                inp.iniciativa as descricaoitem,
                pp.ppcano as anoitem,
                inp.inpid as codigo,
                sum(ec.epcvalorempenho) AS vlrempenho,
                sum(COALESCE(epc.vlrcancelado, 0::numeric)) AS vlrcancelado,
                sum(ec.epcvalorempenho - COALESCE(epc.vlrcancelado, 0::numeric)) AS valorempenhado,
                coalesce(sum(vlrpago),0) AS vlr_pago
               FROM par3.processo p
                 JOIN par3.processoparcomposicao pp ON pp.proid = p.proid
                 JOIN par3.v_valor_iniciativa_por_processo inp ON inp.inpid = pp.inpid AND inp.ipiano = pp.ppcano AND inp.proid = pp.proid
                 INNER JOIN par3.empenho e ON p.pronumeroprocesso::text = e.empnumeroprocesso::text AND e.empstatus = 'A'::bpchar AND e.empcodigoespecie = '01'::bpchar
                 INNER JOIN par3.empenhoparcomposicao ec ON ec.empid = e.empid AND ec.ppcid = pp.ppcid AND ec.epcstatus = 'A'::bpchar
                 LEFT JOIN ( SELECT
                        epc_1.ppcid,
                        sum(epc_1.epcvalorempenho) AS vlrcancelado,
                        e_1.empidpai
                       FROM par3.empenho e_1
                         JOIN par3.empenhoparcomposicao epc_1 ON epc_1.empid = e_1.empid AND epc_1.epcstatus = 'A'::bpchar
                      WHERE e_1.empstatus = 'A'::bpchar AND (e_1.empcodigoespecie = ANY (ARRAY['03'::bpchar, '13'::bpchar]))
                      GROUP BY epc_1.ppcid, e_1.empidpai) epc ON epc.empidpai = e.empid AND epc.ppcid = ec.ppcid
                LEFT JOIN(
                 	SELECT
					    sum(pac.ppcvalorpagamento) AS vlrpago,
					    pac.epcid,
						pag_1.empid
					FROM par3.pagamento pag_1
						INNER JOIN par3.pagamentoparcomposicao pac ON pac.pagid = pag_1.pagid AND pac.ppcstatus = 'A'::bpchar
					WHERE pag_1.pagstatus = 'A'::bpchar AND pag_1.pagsituacaopagamento::text !~~* '%CANCELADO%'::text AND pag_1.pagsituacaopagamento::text !~~* '%vala%'::text AND pag_1.pagsituacaopagamento::text !~~* '%devolvido%'::text AND pag_1.pagsituacaopagamento::text !~~* '%VALA CENTRO DE GESTÃO%'::text
					GROUP BY pac.epcid, pag_1.empid
                 ) pg ON pg.empid = e.empid AND pg.epcid = ec.epcid
            WHERE e.empid = $empid
            GROUP BY pp.ppcid, inp.iniciativa, pp.ppcano, inp.inpid
            UNION ALL
            SELECT
            	v.pocid as id,
            	v.iniciativa as descricaoitem,
            	v.obrano as anoitem,
            	v.obrid as codigo,
            	sum(v.vlrempenho) AS vlrempenho,
            	sum(v.vlrcancelado) AS vlrcancelado,
            	sum(v.saldo) as valorempenhado,
                coalesce(sum(v.vlrpago),0) AS vlr_pago
            FROM par3.v_obras_planejamento_processo_empenho v WHERE v.empid = $empid
            GROUP BY v.pocid, v.valorobra, v.iniciativa, v.obrano, v.obrid, v.obrvalor_contrapartida";
    $iniciativas = $db->carregar($sql);

    //echo getToobarListagem($sql, false);
    ?>
    <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">
    <thead>
    <tr>
        <th>&nbsp;</th>
        <th>Código</th>
        <th>Ano</th>
        <th style="width: 250px">Descrição</th>
        <th>Valor Empenhado</th>
        <th>Valor Cancelado</th>
        <th>Valor Pago</th>
        <th>% a Cancelar</th>
        <th>Valor a Cancelar</th>
    </tr>
    </thead>
    <tbody>
    <?php
    if ($iniciativas[0]) {
        foreach ($iniciativas as $ini) {
            $disabled = '';
            if ($ini['valorempenhado'] == $ini['vlr_pago']) {
                $disabled = ' disabled ';
            }
            echo '<tr>';
            echo '<td><div class="checkbox checkbox-success">
            			<input type="checkbox" name="codigo[]" id="chk_'.$ini['id'].'" value="'.$ini['id'].'" '.$disabled.' onchange="selecionarTipoCancelamento(\''.$ini['id'].'\');">
            			<label for="chk_'.$ini['id'].'">&nbsp;</label>
            		</div></td>';
            echo '<td>'.$ini['codigo'].'</td>';
            echo '<td>'.$ini['anoitem'].'</td>';
            echo '<td style="width: 250px">'.$ini['descricaoitem'].'</td>';
            echo '<td>'.number_format($ini['vlrempenho'], 2, ",", ".").'<input type="hidden" id="vlrempenho_'.$ini['id'].'" value="'.$ini['vlrempenho'].'"></td>';
            echo '<td>'.number_format($ini['vlrcancelado'], 2, ",", ".").'<input type="hidden" id="vlrcancelado_'.$ini['id'].'" value="'.$ini['vlrcancelado'].'"></td>';
            echo '<td>'.number_format($ini['vlr_pago'], 2, ",", ".").'<input type="hidden" id="vlr_pago_'.$ini['id'].'" value="'.$ini['vlr_pago'].'">
                        <input type="hidden" id="valorempenhado_cancel_'.$ini['id'].'" value="'.$ini['valorempenhado'].'">
                    </td>';
            echo '<td><input name="porc_cancelar['.$ini['id'].']" id="porc_cancelar_'.$ini['id'].'" readonly="readonly" type="text" value="" class="form-control" maxlength="10" style="text-align: right; width: 80px;"
                            	oninput="this.value = mascaraglobal(\'###\', this.value); calcularValorCancelar(\'porcentagem\', '.$ini['id'].')"></td>';
            echo '<td><input name="valor_cancelar['.$ini['id'].']" id="valor_cancelar_'.$ini['id'].'" readonly="readonly" type="text" value="" class="form-control" maxlength="30" required="required" style="text-align: right; width: 150px;"
			    oninput="this.value = mascaraglobal(\'###.###.###.###,##\', this.value); calcularValorCancelar(\'valor\', '.$ini['id'].');"></td>';
            echo '</tr>';
        }
        echo '<tr>';
        echo '<td colspan="8" style="text-align: right;"><b>Valor Cancelamento:</b></td>';
        echo '<td style="text-align: left;"><input name="valor_cancelado" id="valor_cancelado" readonly="readonly" type="text" value="" class="form-control" maxlength="255" required="required" style="text-align: right; width: 150px;"></td>';
        echo '</tr>';
    }
    ?>
    </tbody>
    </table><?php
}

function carregaListaEmpenhoProcesso( $processo, $empidPai = '' ){
    global $db;

    if( $empidPai ){
        $filtro = " e.empcodigoespecie in ('03', '13') and e.empidpai = $empidPai AND e.empstatus = 'A'";
    } else {
        $filtro = " e.empcodigoespecie = '01' AND e.empstatus = 'A'
            	AND e.empnumeroprocesso = '".$processo."'";
    }

    $sql = "SELECT e.empid, COALESCE(e.empnumero,'-') as numeroempenho, e.empsituacao AS situacaoempenho, ''||u.usunome||' - '||to_char(e.empdata,'dd/mm/YYYY HH24:MI')||'' as usuario,
            	formata_cpf_cnpj(e.empcnpj) as cnpj, COALESCE(e.empprotocolo,'-') as numeroprotocolo,
            	e.empvalorempenho AS vlr_solicitado,
            	coalesce(ec.vlr_cancelado,0) AS vlr_cancelado, (e.empvalorempenho - coalesce(ec.vlr_cancelado,0))::numeric(20,2) as valorsaldo,
            	coalesce(pg.vlr_pagamento,0) AS vlr_pagamento
            FROM par3.empenho e
            	LEFT JOIN seguranca.usuario u ON u.usucpf = e.usucpf
            	LEFT JOIN(
            		SELECT empidpai, sum(empvalorempenho) AS vlr_cancelado
            		FROM par3.empenho e
            		WHERE e.empcodigoespecie in ('03', '13', '04') AND e.empstatus = 'A'
            		GROUP BY empidpai
            	) ec ON ec.empidpai = e.empid
            	LEFT JOIN (
            		SELECT empid, sum(pagvalorparcela) AS vlr_pagamento FROM par3.pagamento
            		WHERE pagstatus = 'A'
            			and pagsituacaopagamento not ilike '%CANCELADO%'
            			and pagsituacaopagamento not ilike '%vala%'
            			and pagsituacaopagamento not ilike '%devolvido%'
                        and pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'
            		GROUP BY empid
            	) pg ON pg.empid = e.empid
            WHERE $filtro";

    $empenhos = $db->carregar($sql);
    $empenhos = $empenhos ? $empenhos : array();

    return $empenhos;
}

function carregaEmpenhoCanceladoPorEmpid( $empid ){
    global $db;

    $empenhos = carregaListaEmpenhoProcesso('' , $empid);

    $html = '
    <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">
    <thead>
    <tr>
        <th>Atualizar</th>
        <th>Visualizar</th>
        <th>Nota do Empenho</th>
        <th>Situação do Empenho</th>
        <th>Usuário</th>
        <th>CNPJ</th>
        <th>Número do Protocolo</th>
        <th>Valor Cancelado</th>
    </tr>
    </thead>
    <tbody>';

    foreach ($empenhos as $emp) {
        $html .= '<tr>';
        $html .= '<td><span class="btn btn-success btn-sm glyphicon glyphicon-refresh" onclick="exibirConsultarEmpenho('.$emp['empid'].');"></span></td>';
        $html .= '<td><span class="btn btn-primary btn-sm glyphicon glyphicon-search" style="cursor:pointer;" onclick="exibirComposicaoEmpenho('.$emp['empid'].');"></span></td>';
        $html .= '<td>'.$emp['numeroempenho'].'</td>';
        $html .= '<td>'.$emp['situacaoempenho'].'</td>';
        $html .= '<td>'.$emp['usuario'].'</td>';
        $html .= '<td>'.$emp['cnpj'].'</td>';
        $html .= '<td>'.$emp['numeroprotocolo'].'</td>';
        $html .= '<td id="td_cancelamento_valor">'.simec_number_format($emp['valorsaldo'], 2, ",", ".").'</td>';
        $html .= '</tr>';
    }
    $html .= '
        </tbody>
        </table>';
    echo $html;
}

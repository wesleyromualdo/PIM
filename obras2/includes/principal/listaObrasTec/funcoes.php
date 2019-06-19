<?php

function listaSql(Array $param = array())
{
    $coluns = array();
    $join = array();
    $where = array();

    if ($param['obrbuscatexto']) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-"," ",(trim($param['obrbuscatexto']) )));

        if(!strpos ($obrbuscatextoTemp, ',')){
            $param['obrbuscatexto'] = limpaObridSec(trim($param['obrbuscatexto']));
            $where['obrnome'] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $param['obrbuscatexto'] . "%') OR
                        o.entidsecretaria::CHARACTER VARYING ILIKE ('%" . $param['obrbuscatexto'] . "%') ) ";
        } else {
            $campos = explode(',', $obrbuscatextoTemp);
            $w = array();
            foreach ($campos as $c){
                $c = trim($c);
                $d = limpaObridSec(trim($c));
                $w[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') OR 
                        o.entidsecretaria::CHARACTER VARYING ILIKE ('%" . $d . "%')) ";
            }

            $w = '(' . implode('OR', $w) . ')';
            $where['obrnome'] = $w;
        }

    }

    if ($param['reformulada'] == 'S')
        $where['reformulada'] =  "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridpai = o.obrid) > 0 THEN true ELSE false END";
    if ($param['reformulada'] == 'N')
        $where['reformulada'] =  "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridpai = o.obrid) > 0 THEN false ELSE true END";

    if ($param['vinc'] == 'S')
        $where['vinc'] =  "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridvinculado = o.obrid) > 0 THEN true ELSE false END";
    if ($param['vinc'] == 'N')
        $where['vinc'] =  "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridvinculado = o.obrid) > 0 THEN false ELSE true END";

    if ($param['eq_fis_fin'] == 'S')
        $where['eq_fis_fin'] =  "CASE WHEN ff.diff_number >= -5 AND ff.diff_number <= 5 THEN true ELSE false END";
    if ($param['eq_fis_fin'] == 'N')
        $where['eq_fis_fin'] =  "CASE WHEN ff.diff_number >= -5 AND ff.diff_number <= 5 THEN false ELSE true END";

    if ($param['conta_bancaria_bloqueada'] == 'S')
        $where1['conta_bancaria_bloqueada'] =  "conta_bancaria_bloqueada = 'SIM' ";
    if ($param['conta_bancaria_bloqueada'] == 'N')
        $where1['conta_bancaria_bloqueada'] =  "conta_bancaria_bloqueada = 'NÃO'";

    if ($param['bloqueio'] == 'S')
        $where['bloqueio'] =  "(CASE WHEN emp.empesfera = 'E' THEN
                        CASE WHEN (SELECT COUNT(estuf) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.estuf = estuf GROUP BY estuf) > 0 THEN 'SIM' ELSE 'NÃO' END
                    ELSE
                        CASE WHEN (SELECT COUNT(muncod) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.muncod = muncod GROUP BY muncod) > 0 THEN 'SIM' ELSE 'NÃO' END
                    END) = 'SIM' ";
    if ($param['bloqueio'] == 'N')
        $where['bloqueio'] =  "(CASE WHEN emp.empesfera = 'E' THEN
                        CASE WHEN (SELECT COUNT(estuf) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.estuf = estuf GROUP BY estuf) > 0 THEN 'SIM' ELSE 'NÃO' END
                    ELSE
                        CASE WHEN (SELECT COUNT(muncod) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.muncod = muncod GROUP BY muncod) > 0 THEN 'SIM' ELSE 'NÃO' END
                    END) = 'NAO'";



    if ($param['empid'])
        $where['empid'] =  "o.empid IN(" . $param['empid'] . ")";
    if ($param['strid'] && $param['strid'][0] != '')
        $where['strid'] = "str.strid IN(" . implode(',', $param['strid']) . ")";
    if ($param['tobid'] && $param['tobid'][0] != '')
        $where['tobid'] = "o.tobid IN(" . implode(',', $param['tobid']) . ")";
    if ($param['tooid'] && $param['tooid'][0] != '')
        $where['tooid'] = "o.tooid IN(" . implode(',', $param['tooid']) . ")";
    if ($param['tpoid'] && $param['tpoid'][0] != '')
        $where['tpoid'] = "o.tpoid IN ('" . implode("', '", $param['tpoid']) . "')";

    if ($param['prfid'] && $param['prfid'][0] != '')
        $where['prfid'] = "emp.prfid IN(" . implode(',', $param['prfid']) . ")";

    if ($param['entid'])
        $where['entid'] = "o.entid IN(" . implode(',', $param['entid']) . ")";

        $where['estuf'] = "m.estuf IN('SP')";
    if ($param['muncod'] && $param['muncod'][0] != '')
        $where['muncod'] = "m.muncod IN ('" . implode("', '", $param['muncod']) . "')";
    if ($param['empesfera'])
        $where['empesfera'] = "emp.empesfera IN('" . $param['empesfera'] . "')";
    if ($param['processo'])
        $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')";
    if ($param['ano_processo'])
        $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$param['ano_processo']}'";
    if ($param['convenio'])
        $where['convenio'] = "p_conv.termo_convenio = '{$param['convenio']}'";
    if ($param['ano_convenio'])
        $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$param['ano_convenio']}'";

    if ($param['obrami'] == 'S')
        $where['obrami'] = "o.tpoid IN (104, 105)";
    elseif ($param['obrami'] == 'N')
        $where['obrami'] = "o.tpoid NOT IN (104, 105)";
 
    $ck_sql = "SELECT
                        ckf.obrid
                    FROM obras2.checklistfnde                    ckf
                    INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                    INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                    WHERE ckf.ckfstatus = 'A' AND que.queid = %s";
    if ($param['chk_adm'] == 'S')
        $where['chk_adm'] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM).")";
    elseif ($param['chk_adm'] == 'N')
        $where['chk_adm'] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM).")";
    if ($param['chk_2pc'] == 'S')
        $where['chk_2pc'] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_2P).")";
    elseif ($param['chk_2pc'] == 'N')
        $where['chk_2pc'] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_2P).")";
    if ($param['chk_tec'] == 'S')
        $where['chk_tec'] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC).")";
    elseif ($param['chk_tec'] == 'N')
        $where['chk_tec'] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC).")";
    if ($param['chk_vinc'] == 'S')
        $where['chk_vinc'] = " o.obrid IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC).")";
    elseif ($param['chk_vinc'] == 'N')
        $where['chk_vinc'] = " o.obrid NOT IN (".sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC).")";
 
    if ($param['esdid'])
        $where['esdid'] = "d.esdid IN(" . $param['esdid'] . ")";
 
    if ($param['percentualinicial'] != '')
        $where['percentualinicial'] = "COALESCE(obrpercentultvistoria, 0) >= " .  $param['percentualinicial'];
    if ($param['percentualfinal'] != '')
        if ($param['percentualfinal'] < 100)
            $where['percentualfinal'] = "COALESCE(obrpercentultvistoria, 0) <= " . $param['percentualfinal'];
    if ($param['atraso'] == '1')
        $where['atraso'] = "numdias <= 0";
    if ($param['atraso'] == '2')
        $where['atraso'] = "numdias > 0 AND numdias < 15";
    if ($param['atraso'] == '3')
        $where['atraso'] = "numdias >= 15";


    $coluns['acao'] = "'
            <center><div>
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarObr(' || o.obrid || ');\"
                            title=\"Alterar Obra\" />
            </div></center>' AS acao";


    
    $campoObridSec = montaCampoObridSec('o');

    $coluns['obrid'] = $campoObridSec . "as obrid";
    $coluns['preid'] = "o.preid";
    $coluns['mi'] = "CASE WHEN o.tpoid IN (104, 105) THEN 'SIM' ELSE 'NÃO' END as mi";
    $coluns['vinculada'] = "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridvinculado = o.obrid) > 0 THEN 'SIM' ELSE 'NÃO' END AS vinculada";
    $coluns['mundescricao'] = "m.mundescricao";
    $coluns['estuf'] = "m.estuf";
    $coluns['obrnome'] = "'<a href=\"javascript: alterarObr(' || o.obrid || ');\">(' || {$campoObridSec} || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['tipologia'] = "tpo.tpodsc";
    $coluns['programa'] = "pf.prfdesc";
    $coluns['fonte'] = "too.toodescricao";
    $coluns['esfera'] = "CASE WHEN emp.empesfera = 'M' THEN 'Municipal' ELSE 'Estadual' END empesfera";
    $coluns['valor'] = "coalesce(TRUNC (pre.prevalorobra, 2), 0)";
    $coluns['situacao'] = "et.esddsc";
    $coluns['excel_obrnome'] = "'(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome";

    $coluns['vig_ter'] = "
                            CASE
                                WHEN TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text IS NULL AND o.tooid = 2 THEN TO_CHAR(dc.dcodatafim,'DD/MM/YYYY')::text
                                ELSE
                                    CASE WHEN ter.fonte = 'PAR' THEN
                                        '<a target=\"_blank\" href=\"/par/par.php?modulo=principal/documentoParObras&acao=A\">' || TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text || '</a>'
                                    WHEN ter.fonte = 'PAC' THEN
                                        '<a target=\"_blank\" href=\"/par/par.php?modulo=principal/termoPac&acao=A\">' || TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text || '</a>'
                                    END
                            END as dt_fim_vigencia_termo";
    $coluns['osmi'] = "CASE WHEN osm2.tomid = 2 AND osmd2.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem cadastrada para essa obra\">'
				 			 WHEN osm2.tomid = 2 AND (osmd2.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem aguardando aceite para essa obra\">'
                             WHEN osm2.tomid = 2 AND (osmd2.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Sondagem concluída para essa obra\">'
                             WHEN osm2.tomid = 2 AND osmd2.esdid = 910 THEN
                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Sondagem recusada.\">'
				 			 ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Sondagem cadastrada para essa obra\">'
                             END
                             ||
                             CASE WHEN osm3.tomid = 3 AND osmd3.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação cadastrada para essa obra\">'
				 			 WHEN osm3.tomid = 3 AND (osmd3.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação aguaradando aceite para essa obra\">'
                             WHEN osm3.tomid = 3 AND (osmd3.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Implantação concluída para essa obra\">'
                             WHEN osm3.tomid = 3 AND osmd3.esdid = 910 THEN
                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Implantação recusada.\">'
				 			 ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Implantação cadastrada para essa obra\">'
                             END
						     ||
						     CASE WHEN osm1.tomid = 1 AND osmd1.esdid IN(905, 906, 907) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_ativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução cadastrada para essa obra\">'
				 			 WHEN osm1.tomid = 1 AND (osmd1.esdid = 904) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_alerta.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução aguardando aceite para essa obra\">'
                             WHEN osm1.tomid = 1 AND (osmd1.esdid = 908) THEN
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_concluido.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Existe OS Execução concluida para essa obra\">'
                             WHEN osm1.tomid = 1 AND osmd1.esdid = 910 THEN
                                 '<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inativo.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"OS de Execução recusada.\">'
				 			 ELSE
								'<img
				 					align=\"absmiddle\"
				 					src=\"/imagens/0_inexistente.png\"
				 					style=\"cursor: pointer; width:15px;\"
				 					title=\"Não existe OS Execução cadastrada para essa obra\">'
                             END as OS";
    $coluns['ckfadm'] = "(SELECT TO_CHAR(MAX(ckf.ckfdatainclusao), 'DD/MM/YYYY')
                FROM obras2.checklistfnde          ckf
                INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                WHERE que.queid = 96 AND ckf.obrid = o.obrid
                GROUP BY ckf.obrid) as ckfadm";
    $coluns['ckftec'] = "(SELECT TO_CHAR(MAX(ckf.ckfdatainclusao), 'DD/MM/YYYY')
                FROM obras2.checklistfnde          ckf
                INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                WHERE que.queid = 98 AND ckf.obrid = o.obrid
                GROUP BY ckf.obrid) as ckftec";
    $coluns['obrdtultvistoria'] = "TO_CHAR(o.obrdtultvistoria, 'DD/MM/YYYY') as obrdtultvistoria";
    $coluns['percentual_execucao'] = "((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2)  as percentual_execucao";
    $coluns['cronograma_atraso'] = "crono.flag";
    $coluns['cronograma_atraso_excel'] = "crono.flag_xls";
    $coluns['empdtultvistoriaempresa'] = "(


                        SELECT  TO_CHAR(suedtsupervisao, 'DD/MM/YYYY')
                        FROM obras2.supervisaoempresa  se
                        LEFT JOIN obras2.supervisao s ON s.sueid = se.sueid AND s.supstatus = 'A'
                        WHERE se.suestatus = 'A' AND se.empid = emp.empid
                        ORDER BY suedtsupervisao DESC
                        LIMIT 1

                    ) as empdtultvistoriaempresa";
    $coluns['emppercentultvistoriaempresa'] = "(

                        SELECT ( SELECT CASE WHEN SUM(icovlritem) > 0 THEN ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2) ELSE 0 END AS total
                                FROM
                                obras2.itenscomposicaoobra i
                                JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A', 'H') AND s.croid = cro.croid
                                LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                                WHERE
                                i.icostatus = 'A' AND
                                i.relativoedificacao = 'D' AND
                                cro.obrid = s.obrid AND i.obrid = cro.obrid
                            ) as percentual
                        FROM obras2.supervisaoempresa  se
                        LEFT JOIN obras2.supervisao s ON s.sueid = se.sueid AND s.supstatus = 'A'
                        WHERE se.suestatus = 'A' AND se.empid = emp.empid
                        ORDER BY suedtsupervisao DESC
                        LIMIT 1

                    ) as emppercentultvistoriaempresa";

    $coluns['perc_pago'] = "(
                                SELECT (

                                    ( SELECT case when pe.prevalorobra > 0 then
                                        ROUND(SUM( po.pobvalorpagamento * 100 ) / pe.prevalorobra)
                                        else 0 end
                                        FROM par.pagamento p
                                        INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
                                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                                        WHERE p.pagstatus = 'A'
                                            AND p.pagsituacaopagamento ILIKE '%EFETIVADO%'
                                            AND po.preid = pre.preid
                                        GROUP BY pe.prevalorobra
                                    )
                                    UNION
                                    ( SELECT
                                        CASE WHEN pe.prevalorobra > 0 then
                                        ROUND(SUM( po.popvalorpagamento * 100 ) / pe.prevalorobra)
                                        ELSE 0 END
                                        FROM par.pagamento p
                                        INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid
                                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                                        WHERE p.pagstatus = 'A'
                                        AND p.pagsituacaopagamento ilike '%EFETIVADO%'
                                        AND po.preid = pre.preid
                                        GROUP BY pe.prevalorobra
                                    )
                                )
                            )
                            ";
    $coluns['execucao_fisica'] = "

        (1+
        ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatushomologacao = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
        ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus25exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
        ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus50exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END ))||' / 4' as exec_fisica

    ";

    $coluns['equilibrio'] = "CASE WHEN ff.diff_number >= -5 AND ff.diff_number <= 5 THEN 'SIM' ELSE 'NÃO' END AS equilibrio";
    $coluns['qtdobras'] = "ff.qtdobras";
    $coluns['bloqueio'] = "CASE WHEN emp.empesfera = 'E' THEN
                        CASE WHEN (SELECT COUNT(estuf) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.estuf = estuf GROUP BY estuf) > 0 THEN 'SIM' ELSE 'NÃO' END
                    ELSE
                        CASE WHEN (SELECT COUNT(muncod) FROM obras2.vm_pendencia_obras WHERE empesfera = emp.empesfera AND m.muncod = muncod GROUP BY muncod) > 0 THEN 'SIM' ELSE 'NÃO' END
                    END AS bloqueio";
    $coluns['reformulacao'] = "CASE WHEN (SELECT COUNT(obrid) FROM obras2.obras WHERE obridpai = o.obrid) > 0 THEN 'SIM' ELSE 'NÃO' END AS reformulacao";

    $coluns['ca_e'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN 'VERMELHO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN 'AMARELO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN 'VERDE'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN 'AZUL'
            ELSE 'CINZA' END as CA

    ";
    $coluns['cv_e'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN 'VERMELHO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN 'AMARELO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN 'VERDE'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN 'AZUL'
            ELSE 'CINZA' END as CV

    ";
    $coluns['ct_e'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN 'VERMELHO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN 'AMARELO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN 'VERDE'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN 'AZUL'
            ELSE 'CINZA' END as CT

    ";
    $coluns['co_e'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN 'VERMELHO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN 'AMARELO'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN 'VERDE'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN 'AZUL'
            ELSE 'CINZA' END as Outros

    ";

    $coluns['ca'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as CA

    ";
    $coluns['cv'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (22) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as CV

    ";
    $coluns['ct'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid IN (20) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as CT

    ";
    $coluns['co'] = "

            CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1144
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_inativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1140
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_alerta.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1141
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_ativo.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid = 1142
                WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.tprid NOT IN (20, 22, 19) AND r.obrid = o.obrid) > 0 THEN '<img align=\"absmiddle\" src=\"/imagens/0_concluido.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;'
            ELSE '<img align=\"absmiddle\" src=\"/imagens/0_inexistente.png\" style=\"cursor: pointer\" title=\"Aguardando Correção\">&nbsp;&nbsp;' END as Outros

    ";

    $coluns['pronumeroprocesso'] = "p_conv.pronumeroprocesso";
    $coluns['anoprocesso'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) anoprocesso";
    $coluns['termo_convenio'] = "p_conv.termo_convenio";
    $coluns['ano_termo_convenio'] = "p_conv.ano_termo_convenio";

    $coluns['conta_bancaria_bloqueada'] = "(
    SELECT CASE WHEN prosituacaoconta IS NULL THEN '-' WHEN prosituacaoconta IN (25,24,14) THEN 'SIM' ELSE 'NÃO' END prosituacaoconta FROM (

    SELECT prosituacaoconta, pronumeroprocesso FROM par.processoobraspar
                    UNION
                    SELECT prosituacaoconta, pronumeroprocesso FROM par.processoobra

                ) as p
                WHERE pronumeroprocesso = vo.processo ) as conta_bancaria_bloqueada";
    if ($param['excel']) {
        unset($coluns['cv']);
        unset($coluns['ct']);
        unset($coluns['ca']);
        unset($coluns['co']);
        unset($coluns['acao']);
        unset($coluns['cronograma_atraso']);

        $coluns['vig_ter'] = "
                            CASE
                                WHEN TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text IS NULL AND o.tooid = 2 THEN TO_CHAR(dc.dcodatafim,'DD/MM/YYYY')::text
                                ELSE
                                    CASE WHEN ter.fonte = 'PAR' THEN
                                        '<a target=\"_blank\" href=\"/par/par.php?modulo=principal/documentoParObras&acao=A\">' || TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text || '</a>'
                                    WHEN ter.fonte = 'PAC' THEN
                                        '<a target=\"_blank\" href=\"/par/par.php?modulo=principal/termoPac&acao=A\">' || TO_CHAR(ter.dt_fim_vigencia_termo,'DD/MM/YYYY')::text || '</a>'
                                    END
                            END as dt_fim_vigencia_termo";

        $coluns['osmi'] = "CASE WHEN osm2.tomid = 2 AND osmd2.esdid IN(905, 906, 907) THEN
								'Existe OS Sondagem cadastrada para essa obra'
				 			 WHEN osm2.tomid = 2 AND (osmd2.esdid = 904) THEN
								'Existe OS Sondagem aguardando aceite para essa obra'
                             WHEN osm2.tomid = 2 AND (osmd2.esdid = 908) THEN
								'Existe OS Sondagem concluída para essa obra'
                             WHEN osm2.tomid = 2 AND osmd2.esdid = 910 THEN
                                 'OS de Sondagem recusada.'
				 			 ELSE
								'Não existe OS Sondagem cadastrada para essa obra'
                             END
                             ||
                             CASE WHEN osm3.tomid = 3 AND osmd3.esdid IN(905, 906, 907) THEN
								'Existe OS Implantação cadastrada para essa obra'
				 			 WHEN osm3.tomid = 3 AND (osmd3.esdid = 904) THEN
								'Existe OS Implantação aguaradando aceite para essa obra'
                             WHEN osm3.tomid = 3 AND (osmd3.esdid = 908) THEN
								'Existe OS Implantação concluída para essa obra'
                             WHEN osm3.tomid = 3 AND osmd3.esdid = 910 THEN
                                 'OS de Implantação recusada.'
				 			 ELSE
								'Não existe OS Implantação cadastrada para essa obra'
                             END
						     ||
						     CASE WHEN osm1.tomid = 1 AND osmd1.esdid IN(905, 906, 907) THEN
								'Existe OS Execução cadastrada para essa obra'
				 			 WHEN osm1.tomid = 1 AND (osmd1.esdid = 904) THEN
								'Existe OS Execução aguardando aceite para essa obra'
                             WHEN osm1.tomid = 1 AND (osmd1.esdid = 908) THEN
								'Existe OS Execução concluida para essa obra'
                             WHEN osm1.tomid = 1 AND osmd1.esdid = 910 THEN
                                 'OS de Execução recusada'
				 			 ELSE
								'Não existe OS Execução cadastrada para essa obra'
                             END as OS";

    } else {
        unset($coluns['cronograma_atraso_excel']);
        unset($coluns['excel_obrnome']);
        unset($coluns['cv_e']);
        unset($coluns['ct_e']);
        unset($coluns['ca_e']);
        unset($coluns['co_e']);
    }


    $sqlBase = "
        SELECT * FROM (
                SELECT DISTINCT ON (o.obrid)
                    " . (count($coluns) ? implode(", ", $coluns) : "") . "
                FROM obras2.obras o
                LEFT JOIN painel.dadosconvenios dc on dc.dcoprocesso = Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')
                LEFT JOIN obras.preobra pre                          ON pre.preid = o.preid
                LEFT JOIN obras2.empreendimento emp                  ON emp.empid = o.empid
                LEFT JOIN entidade.endereco ed                       ON ed.endid = o.endid
                LEFT JOIN territorios.municipio m                    ON m.muncod = ed.muncod
                LEFT JOIN territorios.estado est                     ON est.estuf = m.estuf
                LEFT JOIN obras2.programafonte pf                    ON pf.prfid = emp.prfid
                LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                LEFT JOIN workflow.estadodocumento et                ON et.esdid   = d.esdid

                LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = d.esdid
                LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid

                LEFT JOIN obras2.vm_vigencia_obra_2016  ter                  ON ter.obrid = o.obrid
                LEFT JOIN par.vm_saldo_empenho_por_obra vo ON vo.obrid = o.obrid

                -- Join com a ultima OS Execucao
                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 1 GROUP BY osmt.tomid, osmt.obrid ) AS osm1 ON osm1.obrid = o.obrid
                LEFT JOIN workflow.documento osmd1 ON osm1.docid = osmd1.docid

                -- Join com a ultima OS Sondagem
                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 2 GROUP BY osmt.tomid, osmt.obrid ) AS osm2 ON osm2.obrid = o.obrid
                LEFT JOIN workflow.documento osmd2 ON osm2.docid = osmd2.docid

                -- Join com a ultima OS Inplantação
                LEFT JOIN ( SELECT MAX(osmt.osmid) as osmid, MAX(osmt.docid) as docid, osmt.obrid, osmt.tomid FROM obras2.ordemservicomi osmt WHERE osmt.osmstatus = 'A' AND osmt.tomid = 3 GROUP BY osmt.tomid, osmt.obrid ) AS osm3 ON osm3.obrid = o.obrid
                LEFT JOIN workflow.documento osmd3 ON osm3.docid = osmd3.docid

                -- Dados do Processo e Termo
                LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                LEFT JOIN (
                        SELECT
                            *,
                            CASE WHEN diff = 'N/A' OR diff IS NULL THEN '0 ' ELSE formataMonetario(trunc(diff::numeric, 2))  END as diff_formatado,
                            CASE WHEN diff = 'N/A' OR diff IS NULL THEN 0 ELSE trunc(diff::numeric, 2)  END as diff_number
                        FROM obras2.vm_fisico_financeiro_processo) ff ON ff.pronumeroprocesso = p_conv.pronumeroprocesso
                
                LEFT JOIN (
                    SELECT subcrono.obrid, subcrono.numdias,
                        CASE WHEN subcrono.numdias >= 15 THEN
                            '<center><img src=\"/imagens/icone_critico.png\" style=\"width:15px;\" title=\"Atraso crítico no cronograma\"></center>'
                        WHEN subcrono.numdias <= 0 THEN
                            '<center><img src=\"/imagens/icone_ok.png\" style=\"width:15px;\" title=\"Cronograma OK\"></center>'
                        ELSE
                            '<center><img src=\"/imagens/icone_atencao.png\" style=\"width:15px;\" title=\"Atraso no cronograma\"></center>'
                        END AS flag,
                        CASE WHEN subcrono.numdias >= 15 THEN
                            'Atraso crítico no cronograma'
                        WHEN subcrono.numdias <= 0 THEN
                            'Cronograma OK'
                        ELSE
                            'Atraso no cronograma'
                        END AS flag_xls
                    FROM (
                        SELECT o.obrid, EXTRACT(DAY FROM now() - MIN(i.icodterminoitem)) as numdias
                        FROM obras2.obras o
                        JOIN workflow.documento d ON d.docid = o.docid AND d.esdid = 690
                        JOIN obras2.cronograma cro ON cro.obrid = o.obrid 
                        JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid AND i.obrid = o.obrid AND i.icopercexecutado < 100
                        WHERE cro.crostatus = 'A' AND i.icostatus = 'A'::bpchar
                        group by o.obrid 
                    ) subcrono
                ) crono ON crono.obrid = o.obrid

                " . (count($join) ? implode(" ", $join) : "") . "
                WHERE
                    o.obridpai IS NULL AND o.obrstatus = 'A'
                    " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
            ) as f ";

    if($param['conta_bancaria_bloqueada'] == 'S')
    {
        $sqlBase .= "WHERE conta_bancaria_bloqueada = 'SIM'";
    }
    if($param['conta_bancaria_bloqueada'] == 'N')
    {
        $sqlBase .= "WHERE conta_bancaria_bloqueada = 'NÃO'";
    }
    $sqlBase .="
            ORDER BY f.obrid ASC
    ";
    return $sqlBase;
}

function getCabecalho($xls = false)
{
    if (!$xls)
        $cabecalho[] = 'Ação';
    $cabecalho[] = 'ID';
    $cabecalho[] = 'ID Pré-Obra';
    $cabecalho[] = 'MI';
    $cabecalho[] = 'Vinculada';
    $cabecalho[] = 'Município';
    $cabecalho[] = 'UF';
    $cabecalho[] = 'Nome';
    $cabecalho[] = 'Tipologia';
    $cabecalho[] = 'Programa';
    $cabecalho[] = 'Fonte';
    $cabecalho[] = 'Esfera';
    $cabecalho[] = 'Valor Pactuado';
    $cabecalho[] = 'Situaçao';
    $cabecalho[] = 'Vigência Termo';
    $cabecalho[] = 'OS MI';
    $cabecalho[] = 'Checklist Adminsitrativo';
    $cabecalho[] = 'Checklist Técnico';
    $cabecalho[] = 'Dt Últ Vistoria';
    $cabecalho[] = '% Últ Vistoria';
    $cabecalho[] = 'Atraso Cronograma';
    $cabecalho[] = 'Dt Últ Supervisão';
    $cabecalho[] = '% Últ Supervisão';

    $cabecalho[] = '% Valor Pago';
    $cabecalho[] = 'Validação';

    $cabecalho[] = 'Equilíbrio Fis-Fin';
    $cabecalho[] = 'Qtd Obras';
    $cabecalho[] = 'Bloqueio PAR';
    $cabecalho[] = 'Reformulada';
    $cabecalho[] = 'CA';
    $cabecalho[] = 'CV';
    $cabecalho[] = 'CT';
    $cabecalho[] = 'CO';
    $cabecalho[] = 'Nº Processo';
    $cabecalho[] = 'Ano Processo';
    $cabecalho[] = 'Nº Termo/Convênio';
    $cabecalho[] = 'Ano Termo/Convênio';
    $cabecalho[] = 'Conta Bancária Bloqueada';


    return $cabecalho;
}
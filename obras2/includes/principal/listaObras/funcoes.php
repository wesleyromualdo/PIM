<?php 

function listaSql(Array $param = array())
{
    $coluns = array();
    $join = array();
    $where = array();

    if ($param['obrbuscatexto']) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($param['obrbuscatexto']))));
        $obrbuscatextoTemp = trim($obrbuscatextoTemp);

        if (!strpos($obrbuscatextoTemp, ',')) {
            $where['obrnome'] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . trim($param['obrbuscatexto']) . "%') ) ";
        } else {
            $campos = explode(',', $obrbuscatextoTemp);
            $w = array();
            foreach ($campos as $c) {
                $c = trim($c);
                $w[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        o.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') ) ";
            }

            $w = '(' . implode('OR', $w) . ')';
            $where['obrnome'] = $w;
        }

    }

    if ($param['empid'])
        $where['empid'] = "o.empid IN(" . $param['empid'] . ")";


    if ($param['strid'] && $param['strid'][0] != '')
        $where['strid'] = "str.strid IN(" . implode(',', $param['strid']) . ")";
    if ($param['tobid'] && $param['tobid'][0] != '')
        $where['tobid'] = "o.tobid IN(" . implode(',', $param['tobid']) . ")";
    if ($param['tooid'] && $param['tooid'][0] != '')
        $where['tooid'] = "o.tooid IN(" . implode(',', $param['tooid']) . ")";
    if ($param['cloid'] && $param['cloid'][0] != '')
        $where['cloid'] = "o.cloid IN(" . implode(',', $param['cloid']) . ")";
    if ($param['prfid'] && $param['prfid'][0] != '')
        $where['prfid'] = "e.prfid IN(" . implode(',', $param['prfid']) . ")";
    if ($param['moeid'] && $param['moeid'][0] != '')
        $where['moeid'] = "e.moeid IN(" . implode(',', $param['moeid']) . ")";
    if ($param['entid'])
        $where['entid'] = "o.entid IN(" . implode(',', $param['entid']) . ")";
    if ($param['ultatualizacao']) {
        switch ($param['ultatualizacao']) {
            case 1:
                $where['ultatualizacao'] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) <= 25";
                break;
            case 2:
                $where['ultatualizacao'] = "( DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) > 25 AND
                                  DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) <= 30 )";
                break;
            case 3:
                $where['ultatualizacao'] = "DATE_PART('days', NOW() - coalesce(obrdtultvistoria,obrdtinclusao) ) > 30";
                break;
        }
    }

    if ($param['tpoid'] && $param['tpoid'][0] != '')
        $where['tpoid'] = "o.tpoid IN ('" . implode("', '", $param['tpoid']) . "')";
    if ($param['estuf'] && $param['estuf'][0] != '')
        $where['estuf'] = "mun.estuf IN ('" . implode("', '", $param['estuf']) . "')";
    if ($param['muncod'] && $param['muncod'][0] != '')
        $where['muncod'] = "mun.muncod IN ('" . implode("', '", $param['muncod']) . "')";
    if ($param['empesfera'])
        $where['empesfera'] = "e.empesfera IN('" . $param['empesfera'] . "')";
    if ($param['processo'])
        $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')";
    if ($param['ano_processo'])
        $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$param['ano_processo']}'";
    if ($param['convenio'])
        $where['convenio'] = "p_conv.termo_convenio = '{$param['convenio']}'";
    if ($param['ano_convenio'])
        $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$param['ano_convenio']}'";
    if ($param['totalfoto'] == 'S')
        $where['totalfoto'] = "obrsnfotos = true";
    elseif ($param['totalfoto'] == 'N')
        $where['totalfoto'] = "obrsnfotos = false OR obrsnfotos IS NULL";
    if ($param['obrami'] == 'S')
        $where['obrami'] = "o.tpoid IN (104, 105)";
    elseif ($param['obrami'] == 'N')
        $where['obrami'] = "o.tpoid NOT IN (104, 105)";
    if ($param['possui_vistoria'] == 'S')
        $where['possui_vistoria'] = "obrdtultvistoria IS NOT NULL";
    elseif ($param['possui_vistoria'] == 'N')
        $where['possui_vistoria'] = "o.obrdtultvistoria IS NULL";
    if ($param['res_estado'] == 'S')
        $where['res_estado'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0";
    elseif ($param['res_estado'] == 'N')
        $where['res_estado'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) = 0";
    if ($param['res_superada'] == 'S')
        $where['res_superada'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0";
    elseif ($param['res_superada'] == 'N')
        $where['res_superad'] = "(SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) = 0";
    if ($param['ocraditivado'] == 'S')
        $where['ocraditivado'] = "c.ttaid > 0";
    elseif ($param['ocraditivado'] == 'N')
        $where['ocraditivado'] = " (c.ttaid IS NULL OR c.ttaid=0) ";
    if ($param['responsavel'] == 'S')
        $where['responsavel'] = "e.empdtultvistoriaempresa IS NOT NULL";
    elseif ($param['responsavel'] == 'N')
        $where['responsavel'] = "e.empdtultvistoriaempresa IS NULL";
    if ($param['evomi'] == 'S') {
        $where['evmi'] = "evmi.emidtinclusao IS NOT NULL";
        $join['evmi'] = "LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'";
    } elseif ($param['evomi'] == 'N') {
        $where['evmi'] = "evmi.emidtinclusao IS NULL";
        $join['evmi'] = "LEFT JOIN obras2.evolucaomi      evmi ON evmi.obrid  = o.obrid AND evmi.emistatus = 'A'";
    }
    switch ($param['obrvinculada']) {
        case 'S':
            $where['obrvinculada'] = " o.obrstatus = 'P' ";
            break;
        case 'N':
            $where['obrvinculada'] = " o.obrstatus = 'A' ";
            break;
        case 'T':
            $where['obrvinculada'] = " o.obrstatus IN ('A', 'P') ";
            break;
        default:
            $where['obrvinculada'] = " o.obrstatus = 'A' ";
            break;
    }
    if ($param['repasse'] == 'S') {
        $where['repasse'] = "(o.obrid IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par3.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')
                                        OR
                                        EXISTS(
                                        select 1 from par3.processoobracomposicao p
                                        JOIN par3.empenhoobracomposicao e ON p.pocid = e.pocid AND eocstatus = 'A'
                                        JOIN par3.pagamentoobracomposicao c ON c.eocid = e.eocid AND pmcstatus = 'A'
                                        WHERE p.pocstatus = 'A' AND p.obrid = o.obrid_par3))";
    } elseif ($param['repasse'] == 'N') {
        $where['repasse'] = "(o.obrid NOT IN (SELECT DISTINCT o.obrid FROM obras2.obras o
                                        JOIN par.pagamentoobra po ON po.preid = o.preid
                                        JOIN par3.pagamento p ON p.pagid = po.pagid AND p.pagstatus = 'A'::bpchar AND btrim(p.pagsituacaopagamento::text) = '2 - EFETIVADO'::text
                                        WHERE o.obridpai IS NULL AND o.obrstatus = 'A')
                                        OR
                                        NOT EXISTS(
                                        select 1 from par3.processoobracomposicao p
                                        JOIN par3.empenhoobracomposicao e ON p.pocid = e.pocid AND eocstatus = 'A'
                                        JOIN par3.pagamentoobracomposicao c ON c.eocid = e.eocid AND pmcstatus = 'A'
                                        WHERE p.pocstatus = 'A' AND p.obrid = o.obrid_par3))";
    }
    if ($param['medidasexcecao'] == 'S')
        $where['medidasexcecao'] = "medidasexcecao = TRUE ";
    elseif ($param['medidasexcecao'] == 'N')
        $where['medidasexcecao'] = "medidasexcecao = FALSE ";
    if ($param['fiscal_ativo'] == 'S')
        $where['fiscal_ativo'] = "e.empid IN (select
                                ur.empid
                            from obras2.usuarioresponsabilidade ur
                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                            where
                            ur.rpustatus = 'A' AND
                            u.suscod = 'A' AND
                            us.suscod = 'A' AND ur.empid IS NOT NULL
                            GROUP BY ur.empid
                            HAVING COUNT(*) > 1)";
    elseif ($param['fiscal_ativo'] == 'N')
        $where['fiscal_ativo'] = " e.empid NOT IN (select
                                ur.empid
                            from obras2.usuarioresponsabilidade ur
                            inner join seguranca.usuario u on u.usucpf = ur.usucpf
                            inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                            where
                            ur.rpustatus = 'A' AND
                            u.suscod = 'A' AND
                            us.suscod = 'A' AND ur.empid IS NOT NULL
                            GROUP BY ur.empid
                            HAVING COUNT(*) > 1)";


    if ($param['posvigencia'] == 'S')
        $where['posvigencia'] = "o.obrid IN (select distinct
                                obrid
                                from obras2.supervisao sup where posvigencia = 't' and supstatus = 'A')";
    elseif ($param['posvigencia'] == 'N')
        $where['posvigencia'] = "o.obrid IN (select  distinct
                                sup.obrid
                                from obras2.supervisao sup where posvigencia = 'f' and supstatus = 'A')";

    if ($param['mesid'] == 'S')
        $where['mesid'] = "o.obrid IN (SELECT DISTINCT m.obrid FROM obras2.monitoramento_especial m WHERE m.messtatus = 'A')";
    elseif ($param['mesid'] == 'N')
        $where['mesid'] = "o.obrid IN (SELECT DISTINCT m.obrid FROM obras2.monitoramento_especial m WHERE m.messtatus = 'A')";


    $ck_sql = "SELECT
                        ckf.obrid
                    FROM obras2.checklistfnde                    ckf
                    INNER JOIN questionario.questionarioresposta qrp ON ckf.qrpid  = qrp.qrpid
                    INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                    WHERE ckf.ckfstatus = 'A' AND que.queid = %s";
    if ($param['chk_adm'] == 'S')
        $where['chk_adm'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM) . ")";
    elseif ($param['chk_adm'] == 'N')
        $where['chk_adm'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_ADM) . ")";
    if ($param['chk_2pc'] == 'S')
        $where['chk_2pc'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_2P) . ")";
    elseif ($param['chk_2pc'] == 'N')
        $where['chk_2pc'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_2P) . ")";
    if ($param['chk_tec'] == 'S')
        $where['chk_tec'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC) . ")";
    elseif ($param['chk_tec'] == 'N')
        $where['chk_tec'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_TEC) . ")";
    if ($param['chk_vinc'] == 'S')
        $where['chk_vinc'] = " o.obrid IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC) . ")";
    elseif ($param['chk_vinc'] == 'N')
        $where['chk_vinc'] = " o.obrid NOT IN (" . sprintf($ck_sql, QUEID_QUEST_CHKLST_OBR_VINC) . ")";

    if ($param['chk_org_controle'] == 'S')
        $where['chk_org_controle'] = " o.obrorgcontrole = 't' ";
    elseif ($param['chk_org_controle'] == 'N')
        $where['chk_org_controle'] = " o.obrorgcontrole = 'f'";

    if ($param['chk_conta_bloqueada'] == 'S')
        $where['chk_conta_bloqueada'] = " o.obrcontabloqueada = 't'";
    elseif ($param['chk_conta_bloqueada'] == 'N')
        $where['chk_conta_bloqueada'] = " o.obrcontabloqueada = 'f'";

    if ($param['chk_obrprocessoanterior'] == 'S')
        $where['chk_obrprocessoanterior'] = " o.obrprocessoanterior = 't'";
    elseif ($param['chk_obrprocessoanterior'] == 'N')
        $where['chk_cobrprocessoanterior'] = " o.obrprocessoanterior = 'f'";

    if ($param['stiid'] && $param['stiid'][0] != '')
        $where['stiid'] = "o.stiid IN (" . implode(', ', $param['stiid']) . ")";

    if ($param['esdid'] && $param['esdid'][0] != '')
        $where['esdid'] = "d.esdid IN (" . implode(', ', $param['esdid']) . ")";

    if ($param['rsuid'] && $param['rsuid'][0] != '')
        $where['rsuid'] = "rsuid IN (" . implode(', ', $param['rsuid']) . ")";

    if ($param['obrvalorprevisto_menor'])
        $where['obrvalorprevisto_menor'] = "o.obrvalorprevisto < " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_menor']);
    if ($param['obrvalorprevisto_maior'])
        $where['obrvalorprevisto_maior'] = "o.obrvalorprevisto > " . str_replace(array(".", ","), array("", "."), $param['obrvalorprevisto_maior']);
    switch ($param['nivelpreenchimento']) {
        /* 1 - Verde    -> x < 25              */
        /* 2 - Amarelo  -> 25 > x <= 30         */
        /* 3 - Vermelho -> x > 30              */
        case 1 : //Verde
            $where['nivelpreenchimento'] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) <= 25 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( DATE_PART('days', NOW() - sup.supdata  ) <= 25 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                END  ";
            break;
        case 2 :
            $where['nivelpreenchimento'] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( (DATE_PART('days', NOW() - o.obrdtultvistoria ) > 25) AND DATE_PART('days', NOW() - o.obrdtultvistoria ) <= 30 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( (DATE_PART('days', NOW() - sup.supdata  ) > 25) AND DATE_PART('days', NOW() - sup.supdata ) <= 30  )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                END";
            break;
        case 3 :
            $where['nivelpreenchimento'] = " CASE
                                WHEN o.obrdtultvistoria IS NOT NULL THEN
                                    ( DATE_PART('days', NOW() - o.obrdtultvistoria ) > 30 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                ELSE
                                    ( DATE_PART('days', NOW() - sup.supdata  ) > 30 )
                                     AND ( ed.esdid = " . ESDID_OBJ_PARALISADO . " OR ed.esdid = " . ESDID_OBJ_EXECUCAO . ") = true
                                END  ";
            break;
    }
    if ($param['nivelpreenchimento'] || $param['rsuid'])
        $join['vistoria'] = "LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid";

    if ($param['percentualinicial'] != '')
        $where['percentualinicial'] = "COALESCE(o.obrpercentultvistoria, 0) >= " . $param['percentualinicial'];
    if ($param['percentualfinal'] != '')
        if ($param['percentualfinal'] < 100)
            $where['percentualfinal'] = "COALESCE(o.obrpercentultvistoria, 0) <= " . $param['percentualfinal'];
    if ($param['funcionamentoflag']) {
        switch ($param['funcionamentoflag']) {
            case 1: // em funcionamento
                $queryComplement = 'AND moedtiniciofunc < NOW()';
                break;
            case 2: // previsão de inauguração
                $queryComplement = 'AND moedtprevinauguracao BETWEEN \'' . ajusta_data($param['moedtprevinauguracao_i']) . '\' AND \'' . ajusta_data($param['moedtprevinauguracao_f']) . '\'';
                break;
            case 3: //previsão inicio de funcionamento
                $queryComplement = 'AND moedtpreviniciofunc BETWEEN \'' . ajusta_data($param['moedtpreviniciofunc_i']) . '\' AND \'' . ajusta_data($param['moedtpreviniciofunc_f']) . '\'';
                break;
        }

        $where['funcionamentoflag'] = sprintf("CASE WHEN (SELECT
                            CASE WHEN moeid IS NOT NULL THEN TRUE ELSE FALSE END
                            FROM obras2.mobiliarioempenhado
                            WHERE obrid = o.obrid %s
                            ORDER BY moeid DESC
                            LIMIT 1) THEN true
                         ELSE false END", $queryComplement);
    }

    $sqlCondEmp = "
                    SELECT
                           sov.obrid as obra
                           FROM par.subacaoobravinculacao sov
                                   INNER JOIN par.v_saldo_empenho_por_subacao es on es.sbaid = sov.sbaid and es.eobano = sov.sovano
                                  INNER JOIN par.subacao sba ON sba.sbaid = sov.sbaid
                                   INNER JOIN par3.propostatiposubacao pts ON pts.ptsid = sba.ptsid
                           WHERE
                           pts.ptsdescricao IN ('EQUIPAMENTOS', 'EQUIPAMENTOS (BNDES)', 'MOBILIÁRIO', 'MOBILIÁRIO (BNDES)')
                           AND es.saldo > 0
                    UNION ALL
                    -- Carga de Mobiliário Empenhado por Convênio Ativo
                    SELECT DISTINCT moe.obrid as obra
                        FROM obras2.mobiliarioempenhado moe
                        WHERE moe.moestatus = 'A'
                        AND moe.moeempenhadoflag = 'S'
                        
                    UNION ALL
                    SELECT inp.obrid FROM par3.iniciativa_planejamento inp
                                  INNER JOIN par3.analise ana ON ana.inpid = inp.inpid
                                 INNER JOIN par3.processoparcomposicao ppc ON ppc.inpid = inp.inpid AND ppc.anaid = ana.anaid
                                 INNER JOIN par3.processo pro ON pro.proid = ppc.proid
                                 INNER JOIN par3.empenho emp ON emp.empnumeroprocesso = pro.pronumeroprocesso
                                 WHERE empstatus = 'A'
                                 AND inp.obrid IS NOT NULL
                    -- Empenho pelo par 3
                    
    ";

    if ($param['moeempenhadoflag'] == 'S') {
        $where['moeempenhadoflag'] = "o.obrid IN ($sqlCondEmp)";
    } elseif ($param['moeempenhadoflag'] == 'N') {
        $where['moeempenhadoflag'] = "o.obrid NOT IN ($sqlCondEmp)";
    }
    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR))) {

        $arrObr = array(
            PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
            PFLCOD_EMPRESA_VISTORIADORA_GESTOR
        );

        $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

        $arrOrg = Array(PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_TIPO_DE_ENSINO,
            PFLCOD_SUPERVISOR_MEC,
            PFLCOD_GESTOR_MEC);

        $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

        $resp = array();
        $arPflcod = array();
        $orWhere = array();

        if (possui_perfil($arrEst)) {
            $arPflcod = array_merge($arPflcod, $arrEst);
            $orWhere['estuf'] = "mun.estuf IN ( SELECT estuf FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = edr.estuf)";
        }

        if (possui_perfil($arrObr)) {
            $arPflcod = array_merge($arPflcod, $arrObr);
            $orWhere['empid'] = "e.empid IN ( SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
        }

        if (possui_perfil($arrOrg)) {
            $arPflcod = array_merge($arPflcod, $arrOrg);
            $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = e.orgid)";
        }

        if (possui_perfil($arrUni)) {
            $arPflcod = array_merge($arPflcod, $arrUni);
            $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = e.entidunidade)";
        }

        if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
            $usuarioResp = new UsuarioResponsabilidade();
            $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
            $arEmpid = ($arEmpid ? $arEmpid : array(0));

            $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
            $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

            $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
        }

    }

    $imgNovoObj = '';
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC))) {
        $imgNovoObj = "' ||
                                    CASE WHEN ed.esdid = " . ESDID_OBJ_PARALISADO . " AND o.obrstatus != 'P' THEN
                                        '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/editar_nome.gif\"
                                                style=\"cursor: pointer\"
                                                onclick=\"javascript: novaObrVinculada(' || o.obrid || ');\"
                                                title=\"Cadastrar nova obra vinculada\">'
                                    ELSE
                                            '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                                    END || '";
    }

    $coluns['acao'] = "'<center><div style=\"width:100px\">
                    {$imgNovoObj}
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/edit_on.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: abreListaSupervisaoFnde(' || o.obrid || ',' || o.empid || ');\"
                            title=\"Supervisão FNDE\" />
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/seriehistorica_ativa.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: abreEvolucaoFinan(' || o.obrid || ');\"
                            title=\"Evolução Financeira\" />
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarObr(' || o.obrid || ');\"
                            title=\"Alterar Obra\" />
              </div></center>' AS acao";

    $coluns['r'] = "
                        CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0 THEN
                            '<img title=\"Restrições pendentes\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_vermelho.png\">'
                        ELSE '<img title=\"Restrições superadas\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_verde.png\">' END as r ";
    $coluns['i'] = "
                         CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'I' AND r.obrid = o.obrid) > 0 THEN
                            '<img title=\"Restrições pendentes\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_vermelho.png\">'
                        ELSE '<img title=\"Restrições superadas\"
                                style=\"cursor:pointer; width:15px;\"
                                onclick=\"abreRestricao(' || o.obrid || ');\"
                                src=\"/imagens/obras/atencao_verde.png\">' END as i";
    $coluns['vinculada'] = "
                        CASE WHEN o.obridvinculado IS NOT NULL THEN
                            '<img title=\"Com obra vinculada\" src=\"/imagens/check_p.gif\"/>'
                        ELSE
                            ''
                        END AS vinculada";


    $coluns['excel_r'] = "CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid) > 0 THEN 'SIM' ELSE 'NÃO' END as excel_r";

    $coluns['excel_i'] = "CASE WHEN (SELECT COUNT(*) FROM obras2.restricao r
                                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN (" . ESDID_SUPERADA . ", " . ESDID_JUSTIFICADA . ")
                                    WHERE r.rststatus = 'A' AND r.rstitem = 'I' AND r.obrid = o.obrid) > 0 THEN 'SIM' ELSE 'NÃO' END as excel_i";

    $coluns['excel_vinculada'] = "
                        CASE WHEN o.obridvinculado IS NOT NULL THEN
                            'SIM'
                        ELSE
                            'NÃO'
                        END AS vinculada";


    $coluns['obrid'] = "o.obrid";
    $coluns['preid'] = "o.preid as preid";
    $coluns['pronumeroprocesso'] = (verificaTecnico()) ? 'p_conv.pronumeroprocesso' : "'<span class=\"processo_detalhe\">'||p_conv.pronumeroprocesso||'</span>'";
    $coluns['excel_pronumeroprocesso'] = "p_conv.pronumeroprocesso";
    $coluns['anoprocesso'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) anoprocesso";
    $coluns['termo_convenio'] = "p_conv.termo_convenio";
    $coluns['ano_termo_convenio'] = "p_conv.ano_termo_convenio";
    $coluns['obrnome'] = "
                        CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                            '<b style=\"color:green\">(EP) </b>'
                        ELSE
                            ''
                        END || '<a href=\"javascript: alterarObr(' || o.obrid || ');\">(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['excel_obrnome'] = "'(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['ocrdtordemservico'] = "TO_CHAR(oc.ocrdtordemservico, 'dd/mm/YYYY') AS ocrdtordemservico";
    $coluns['mesdsc'] = "meso.mesdsc";
    $coluns['entnome'] = "uni.entnome";
    $coluns['mundescricao'] = "mun.mundescricao";
    $coluns['estuf'] = "mun.estuf";
    $coluns['excel_endereco'] = "trim(edr.endlog) ||', '||  trim(edr.endbai) ||', '|| trim(mun.mundescricao) ||' - '|| trim(mun.estuf) ||' CEP: '|| trim(edr.endcep) as endereco";
    $coluns['inicio'] = "CASE WHEN o.tpoid IN (104, 105) THEN (SELECT TO_CHAR(osmdtinicio, 'dd/mm/YYYY') FROM obras2.ordemservicomi WHERE obrid = o.obrid AND osmstatus = 'A' AND tomid = 1 ORDER BY osmid DESC LIMIT 1) ELSE TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') END AS inicio";
    $coluns['termino'] = "CASE WHEN o.tpoid IN (104, 105) THEN (SELECT TO_CHAR(osmdttermino, 'dd/mm/YYYY') FROM obras2.ordemservicomi WHERE obrid = o.obrid AND osmstatus = 'A' AND tomid = 1 ORDER BY osmid DESC LIMIT 1) ELSE TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') END AS termino";
    $coluns['strdsc'] = "
                    CASE
                        WHEN str.strid = " . STRID_OBJ_CONCLUIDO . " THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || str.strdsc || '</font>'
                        ELSE str.strdsc
                    END AS situacao
                    ";
    $coluns['excel_strdsc'] = "str.strdsc AS situacao";
    $coluns['percentual_execucao'] = "CASE WHEN
            ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0)) > '99.95' 
        THEN
        ROUND(  ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) ) || '%'
        ELSE((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%'
        
        END AS percentual_execucao";
    $coluns['tpldsc'] = "sup.tpldsc";
    $coluns['ultima_atualizacao'] = "
                    -- Obra concluida aplicar cor azul
                    CASE WHEN ed.esdid IN (693) THEN
                        '<span style=\"color:#3276B1\">' || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</span>'
                    -- Obra em execução ou paralisada com vistoria
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) <= 25 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 25 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 30 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '</span>'
                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) <= 25 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 25 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) > 30 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )' || '</span>'
                    -- Obra convencional em execução ou paralisada sem vistoria usar a data do inicio da execução
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) <= 25 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 25 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) > 30 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char((SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))||' dia(s) )' || '</span>'
                    ELSE
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) <= 25 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada em até 25 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 25 AND DATE_PART('days', NOW() - obrdtultvistoria) <= 30 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada entre 25 e 30 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) > 30 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada a mais de 30 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '</span>'
                    END as ultima_atualizacao";
    $coluns['excel_ultima_atualizacao'] = "to_char(obrdtultvistoria, 'DD/MM/YYYY') as ultima_atualizacao";
    $coluns['qtddias'] = " DATE_PART( 'days', NOW() - o.obrdtultvistoria )::text AS qtddias";
    $coluns['obrpercentultvistoria'] = "CASE WHEN
            o.obrpercentultvistoria > '99.95' 
        THEN
        ROUND(  o.obrpercentultvistoria )  || '%'
        ELSE o.obrpercentultvistoria || '%'
        
        END AS obrpercentultvistoria";
    $coluns['empdtultvistoriaempresa'] = "to_char(e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa";
    $coluns['emppercentultvistoriaempresa'] = "e.emppercentultvistoriaempresa || '%' as emppercentultvistoriaempresa";
    $coluns['programa'] = "pf.prfdesc programa";
    $coluns['fonte'] = "too.toodescricao fonte";
    $coluns['esfera'] = "
                    CASE
                        WHEN e.empesfera = 'M' THEN 'Municipal'
                        WHEN e.empesfera = 'E' THEN 'Estadual'
                        WHEN e.empesfera = 'F' THEN 'Federal'
                    ELSE ''
                    END as esfera";
    $coluns['tpodsc'] = "tpo.tpodsc";
    $coluns['ocrvalorexecucao'] = "ocrvalorexecucao";
    $coluns['obrvalorprevisto'] = "o.obrvalorprevisto";


    $coluns['excel_valor_pactuado'] = "v_fim_obr.valor_pactuado";
    $coluns['excel_valor_empenhado'] = "
                                         CASE WHEN peo.valorempenho NOTNULL THEN 
                                               peo.valorempenho
                                             ELSE
                                               p3eo.vlrempenho
                                             END as valor_empenhado ";
    $coluns['excel_valor_receber'] = "(valor_pactuado - valor_repassado) as valor_receber";
    $coluns['excel_saldo_conta'] = "v_fim_obr.saldo";
    $coluns['excel_nr_inep'] = "uni.entcodent";




    if ($param['excel']) {
        $join['mesoregiao'] = "LEFT JOIN territorios.mesoregiao meso ON meso.mescod = mun.mescod";
        $join['vistoria'] = "LEFT JOIN obras2.v_vistoria_obra_instituicao as sup ON sup.obrid = o.obrid";
        $join['v_financeiro_obras'] = "LEFT JOIN obras2.v_financeiro_obras v_fim_obr  ON v_fim_obr.obrid = o.obrid";
        $join['par_v_saldo_empenho_por_obra'] = "LEFT JOIN par3.v_saldo_empenho_por_obra as peo ON peo.obrid = o.obrid";
        $join['par3_v_saldo_empenho_por_obra'] = "LEFT JOIN par3.v_saldo_empenho_por_obra as p3eo ON p3eo.obrid = o.obrid_par3";


        unset($coluns['r']);
        unset($coluns['i']);
        unset($coluns['vinculada']);
        unset($coluns['acao']);
        unset($coluns['obrnome']);
        unset($coluns['ultima_atualizacao']);
        unset($coluns['strdsc']);
        unset($coluns['pronumeroprocesso']);
    } else {
        unset($coluns['ocrdtordemservico']);
        unset($coluns['tipoparalisacao']);
        unset($coluns['mesoregiao']);
        unset($coluns['programa']);
        unset($coluns['fonte']);
        unset($coluns['esfera']);
        unset($coluns['termo']);
        unset($coluns['situacao']);
        unset($coluns['qtddias']);
        unset($coluns['processoobra']);
        unset($coluns['anoprocesso']);
        unset($coluns['qtddias']);
        unset($coluns['mesdsc']);
        unset($coluns['tpldsc']);
        unset($coluns['obrvalorprevisto']);
        unset($coluns['excel_r']);
        unset($coluns['excel_pronumeroprocesso']);
        unset($coluns['excel_i']);
        unset($coluns['excel_vinculada']);
        unset($coluns['excel_obrnome']);
        unset($coluns['excel_ultima_atualizacao']);
        unset($coluns['excel_strdsc']);
        unset($coluns['excel_endereco']);
        unset($coluns['excel_valor_pactuado']);
        unset($coluns['excel_valor_empenhado']);
        unset($coluns['excel_valor_receber']);
        unset($coluns['excel_saldo_conta']);
        unset($coluns['excel_nr_inep']);

    }


    if (verificaTecnico()) {
        unset($coluns['percentual_execucao']);
    }

    if(empty($orWhere))
        $orWhere = array();

    $sqlBase = "
        WITH  usuarioresponsabilidade AS (
            SELECT entid, pflcod, estuf, orgid, empid FROM obras2.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}'
        )
        SELECT * FROM (
                SELECT DISTINCT ON (o.obrid)
                    " . (count($coluns) ? implode(", ", $coluns) : "") . "
                FROM obras2.obras o
                LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
                LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                LEFT JOIN obras2.contrato c                          ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
                LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade

                LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid

                LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = d.esdid
                LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid

                -- Dados do Processo e Termo
                LEFT JOIN obras2.v_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                " . (count($join) ? implode(" ", $join) : "") . "
                WHERE
                    o.obridpai IS NULL
                    " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "
                    " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
            ) as f
            ORDER BY f.obrid ASC
    ";

    return $sqlBase;
}

function getCabecalho($xls = false)
{
    $cabecalho = array('Restrições', 'Inconformidades', 'Vinculada', 'ID', 'Obra Número', 'Nº Processo', 'Ano Processo', 'Termo/Nº Convênio', 'Ano Termo/Convênio', 'Obra', 'Data da Ordem de Serviço', 'Mesoregião', 'Unidade Implantadora', 'Município', 'UF','Endereço da Obra' ,'Data de Início da Execução', 'Data Prevista de Término de Execução', 'Situação da Obra', '% Executado Instituição Acumulado', 'Tipo de Paralisação', 'Última Vistoria Instituição', 'Qtd Dias Última Vistoria', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Programa', 'Fonte', 'Esfera', 'Tipologia', 'Valor Contrato', 'Valor Previsto','Valores Pactuados','Valores Empenhados','Valores a Receber','Saldo em Conta','Nº do INEP');

    if (!$xls)
        $cabecalho = array('Ação', 'R', 'I', 'V', 'ID', 'Obra Número', 'Nº Processo', 'Nº Termo/Convênio', 'Ano Termo/Convênio', 'Obra', 'Unidade Implantadora', 'Município', 'UF', 'Data de Início da Execução', 'Data Prevista de Término de Execução', 'Situação da Obra', '% Executado Instituição Acumulado', 'Última Vistoria Instituição', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Tipologia', 'Valor Contrato');

    if (verificaTecnico()) {
        $cabecalho = array('Restrições', 'Inconformidades', 'Vinculada', 'ID', 'Obra Número', 'Nº Processo', 'Ano Processo', 'Termo/Nº Convênio', 'Ano Termo/Convênio', 'Obra', 'Data da Ordem de Serviço', 'Mesoregião', 'Unidade Implantadora', 'Município', 'UF', 'Data de Início da Execução', 'Data Prevista de Término de Execução', 'Situação da Obra', 'Tipo de Paralisação', 'Última Vistoria Instituição', 'Qtd Dias Última Vistoria', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Programa', 'Fonte', 'Esfera', 'Tipologia', 'Valor Contrato', 'Valor Previsto');

        if (!$xls)
            $cabecalho = array('Ação', 'R', 'I', 'V', 'ID', 'Obra Número', 'Nº Processo', 'Nº Termo/Convênio', 'Ano Termo/Convênio', 'Obra', 'Unidade Implantadora', 'Município', 'UF', 'Data de Início da Execução', 'Data Prevista de Término de Execução', 'Situação da Obra', 'Última Vistoria Instituição', '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Tipologia', 'Valor Contrato');

    }

    return $cabecalho;
}

function verificaTecnico()
{
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_CGIMP_GESTOR, PFLCOD_CGIMP_TECNICO, PFLCOD_CALL_CENTER, PFLCOD_ADMINISTRADOR))) {
        return false;
    } else {
        return true;
    }
}


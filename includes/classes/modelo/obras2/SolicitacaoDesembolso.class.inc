<?php

class SolicitacaoDesembolso extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.solicitacao_desembolso";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("sldid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'docid' => null,
        'sldid' => null,
        'obrid' => null,
        'usucpf' => null,
        'sldjustificativa' => null,
        'sldobs' => null,
        'sldstatus' => null,
        'slddatainclusao' => null,
        'sldpercsolicitado' => null,
        'supid' => null,
        'sldpercpagamento' => null,
        'sldpercobra' => null,
        'sldobstec' => null,
        'arqid' => null,

    );


    public function verificaTramiteSolicitacoes($obrid)
    {
        if(!$obrid)
            return true;

        $obras = new Obras($obrid);

        if($obras->tooid != 2) {

            $sql = "
               SELECT
                    sv.*,
                    (SELECT htddata FROM workflow.historicodocumento WHERE docid = d.docid ORDER BY hstid DESC LIMIT 1),
                    (
                        SELECT
                        c.cmddsc
                        FROM workflow.historicodocumento h
                        JOIN workflow.comentariodocumento  c ON c.hstid = h.hstid
                        WHERE h.docid = d.docid ORDER BY h.hstid DESC LIMIT 1
                    )
                FROM obras2.solicitacao_desembolso sv
                JOIN workflow.documento d ON d.docid = sv.docid
                WHERE d.esdid IN(" . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA . ") AND sv.sldstatus = 'A' AND sv.obrid = $obrid
            ";

            $solicitacoesEmAnalise = $this->pegaLinha($sql);

            if (!empty($solicitacoesEmAnalise)) {
                // Verifica se possui Restrições
                // Indeferido
                $validador = new Validador();
                $obra = new Obras($obrid);
                if (!$validador->restricoesNaoSuperadas($obra)) {
                    $doc = wf_pegarDocumento($solicitacoesEmAnalise['docid']);

                    if ($doc['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI)
                        wf_alterarEstado($solicitacoesEmAnalise['docid'], AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_REI_PARA_INDEFERIDO, 'A obra apresenta restrições impeditivas de liberação de pagamento. Favor consultar aba "Restrições e Inconformidades" para regularização de pendências.', array('obrid' => $solicitacoesEmAnalise['obrid'], 'sldid' => $solicitacoesEmAnalise['sldid']));

                    else if ($doc['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL)
                        wf_alterarEstado($solicitacoesEmAnalise['docid'], AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_DOCUMENTAL_PARA_INDEFERIDO, 'A obra apresenta restrições impeditivas de liberação de pagamento. Favor consultar aba "Restrições e Inconformidades" para regularização de pendências.', array('obrid' => $solicitacoesEmAnalise['obrid'], 'sldid' => $solicitacoesEmAnalise['sldid']));

                    else if ($doc['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA)
                        wf_alterarEstado($solicitacoesEmAnalise['docid'], AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO, 'A obra apresenta restrições impeditivas de liberação de pagamento. Favor consultar aba "Restrições e Inconformidades" para regularização de pendências.', array('obrid' => $solicitacoesEmAnalise['obrid'], 'sldid' => $solicitacoesEmAnalise['sldid']));
                }

                $sql = "UPDATE workflow.historicodocumento SET usucpf = '00000000191' WHERE docid = {$solicitacoesEmAnalise['docid']} AND aedid IN (" . AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_REI_PARA_INDEFERIDO . ", " . AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_DOCUMENTAL_PARA_INDEFERIDO . ", " . AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO . ")";
                $this->executar($sql);
                $this->commit();
            }
        } else {


            $sql = "
               SELECT
                    sv.*,
                    (SELECT htddata FROM workflow.historicodocumento WHERE docid = d.docid ORDER BY hstid DESC LIMIT 1),
                    (
                        SELECT
                        c.cmddsc
                        FROM workflow.historicodocumento h
                        JOIN workflow.comentariodocumento  c ON c.hstid = h.hstid
                        WHERE h.docid = d.docid ORDER BY h.hstid DESC LIMIT 1
                    )
                FROM obras2.solicitacao_desembolso_convenio sv
                JOIN workflow.documento d ON d.docid = sv.docid
                WHERE d.esdid IN(" . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL . ", " . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA . ") AND sv.sdcstatus = 'A' AND sv.processo IN (SELECT DISTINCT pronumeroprocesso FROM obras2.vm_termo_convenio_obras WHERE obrid = $obrid)
            ";

            $solicitacoesEmAnalise = $this->pegaLinha($sql);

            if (!empty($solicitacoesEmAnalise)) {

                $obras = "SELECT obrid FROM obras2.vm_termo_convenio_obras WHERE pronumeroprocesso = '{$solicitacoesEmAnalise['processo']}'";
                $obras = $this->carregarColuna($obras);

                foreach($obras as $obrid) {
                    $obra = new Obras($obrid);
                    $validador = new Validador();
                    if (!$validador->restricoesNaoSuperadas($obra)) {
                        $doc = wf_pegarDocumento($solicitacoesEmAnalise['docid']);

                        if ($doc['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI)
                            wf_alterarEstado($solicitacoesEmAnalise['docid'], AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_REI_PARA_INDEFERIDO, 'O processo possui restrições impeditivas de liberação de pagamento. Favor consultar aba "Restrições e Inconformidades" para regularização de pendências.', array('sldid' => null, 'obrid' => null, 'processo' => $solicitacoesEmAnalise['processo'], 'sdcid' => $solicitacoesEmAnalise['sdcid']));

                        else if ($doc['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL)
                            wf_alterarEstado($solicitacoesEmAnalise['docid'], AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_DOCUMENTAL_PARA_INDEFERIDO, 'O processo possui restrições impeditivas de liberação de pagamento. Favor consultar aba "Restrições e Inconformidades" para regularização de pendências.', array('sldid' => null, 'obrid' => null, 'processo' => $solicitacoesEmAnalise['processo'], 'sdcid' => $solicitacoesEmAnalise['sdcid']));

                        else if ($doc['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA)
                            wf_alterarEstado($solicitacoesEmAnalise['docid'], AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO, 'O processo possui restrições impeditivas de liberação de pagamento. Favor consultar aba "Restrições e Inconformidades" para regularização de pendências.', array('sldid' => null, 'obrid' => null, 'processo' => $solicitacoesEmAnalise['processo'], 'sdcid' => $solicitacoesEmAnalise['sdcid']));

                        break;
                    }

                    $sql = "UPDATE workflow.historicodocumento SET usucpf = '00000000191' WHERE docid = {$solicitacoesEmAnalise['docid']} AND aedid IN (" . AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_REI_PARA_INDEFERIDO . ", " . AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_DOCUMENTAL_PARA_INDEFERIDO . ", " . AEDID_SOLICITACAO_DESEMBOLSO_ANALISE_TECNICA_PARA_INDEFERIDO . ")";
                    $this->executar($sql);
                    $this->commit();
                }
            }


        }

        return true;
    }

    public function pegaPercentualUltSolicitacaoAtiva($obrid)
    {
        $sql = "
            SELECT
                sv.sldpercpagamento
            FROM obras2.solicitacao_desembolso sv
            JOIN workflow.documento d ON d.docid = sv.docid AND d.esdid = 1576
            LEFT JOIN (
                    SELECT
                    sldid,
                    CASE WHEN situacao_pagamento IN ('8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA') THEN 'Solicitação Aprovada'
                    WHEN situacao_pagamento IN ('Enviado ao SIGEF', '6 - VALA SIAFI', '0 - AUTORIZADO') THEN 'Pagamento Solicitado'
                    WHEN situacao_pagamento IN ('2 - EFETIVADO') THEN 'Efetivado'
                    ELSE ''
                    END as situacao_pagamento
                    FROM (

                    select pdo.sldid, pg.pagsituacaopagamento as situacao_pagamento
                        from par.pagamentodesembolsoobras pdo
                        inner join par.pagamentoobra po on po.pobid = pdo.pobid
                        inner join par.pagamento pg on pg.pagid = po.pagid
                        inner join obras2.solicitacao_desembolso sld on sld.sldid = pdo.sldid AND sld.sldstatus = 'A'
                    where pdo.pdostatus = 'A'
                    UNION ALL
                    select pdo.sldid, pg.pagsituacaopagamento as situacao_pagamento
                        from par.pagamentodesembolsoobras pdo
                        inner join par.pagamentoobrapar pop on pop.popid = pdo.popid
                        inner join par.pagamento pg on pg.pagid = pop.pagid
                        inner join obras2.solicitacao_desembolso sld on sld.sldid = pdo.sldid AND sld.sldstatus = 'A'
                        where pdo.pdostatus = 'A'
                    ) as p
                ) ps ON ps.sldid = sv.sldid
            WHERE sv.sldstatus = 'A' AND sv.obrid = $obrid AND ps.situacao_pagamento IS NULL
            ORDER BY sv.sldid DESC LIMIT 1
        ";
        $result = $this->pegaUm($sql);
        return ($result) ? $result : 0;
    }

    public function pegaPercentualPago($obrid)
    {
        $sql = "SELECT (totalpago*100)/vlrobra AS percentualpgo FROM par.v_pagamento_total_por_obra f WHERE f.obrid = $obrid";

        return $this->pegaUm($sql);
    }
	
	public function pegaPercentualSaldo($obrid){

        $sql = "SELECT 
                    SUM(sv.sldpercpagamento) AS sldpercpagamento
                FROM obras2.solicitacao_desembolso sv
                INNER JOIN obras2.obras o ON o.obrid = sv.obrid
                INNER JOIN obras2.v_solicitacao_pagamento sp ON sp.sldid = sv.sldid
                INNER JOIN workflow.documento d ON d.docid = sv.docid
                INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                WHERE  
                  o.obrid = {$obrid}  AND
                  e.esdid = 1576 AND
				  sv.sldstatus = 'A' AND
                  (sp.situacao_pagamento NOT ILIKE '%Efetivado%' OR sp.situacao_pagamento IS NULL)";

        return $this->pegaUm($sql);
    }

    public function pegaPercentualSolicitacao($obrid, $supid = null)
    {
        $supervisao = new Supervisao();
        $supid = ($supid) ? $supid : $supervisao->pegaUltSupidByObra($obrid);

        $sql = "SELECT 
                  *, 
                  (totalpago * 100) / COALESCE(vc.valor_fnde, vc.vlrobra, f.vlrobra) AS percentualpgo 
                  FROM par.v_pagamento_total_por_obra f
                  JOIN par.vm_valor_obra_contrapartida vc ON vc.preid = f.preid
                  WHERE f.obrid = $obrid";

        $pgoObra = $this->pegaLinha($sql);
        $pgoSolicitado = $this->pegaPercentualUltSolicitacaoAtiva($obrid);

        $obras = new Obras($obrid);
        $percentual = $obras->pegaPercentualExecucaoPactuadoSupervisao($obrid, $supid);

        return $percentual - ($pgoObra['percentualpgo'] + $pgoSolicitado);
    }

    public function verificaObrasEmVermelho($obrid)
    {
        $sql = "
            SELECT DISTINCT o.*
            FROM obras2.obras o
            LEFT JOIN workflow.documento 		 d ON d.docid = o.docid AND tpdid = 105
            LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = d.esdid
            LEFT JOIN obras2.empreendimento 	 e ON e.empid = o.empid AND e.empstatus = 'A'
            JOIN entidade.endereco edr ON edr.endid = o.endid
            WHERE
                o.obrstatus = 'A' AND o.obridpai IS NULL AND e.empesfera = 'M' AND d.esdid IN(691,690) AND e.orgid IN(3) AND
                CASE
                    WHEN
                    (obrdtultvistoria IS NOT NULL AND
                    ed.esdid IN (690, 691) AND
                    DATE_PART('days', NOW() - obrdtultvistoria) > 30 AND
                    obrpercentultvistoria < 100.00) THEN
                        true
                    ELSE
                        false
                END
                AND
                edr.muncod IN (

                    SELECT
                        m.muncod
                    FROM
                        obras2.obras o
                    JOIN obras2.empreendimento 	 ep ON ep.empid = o.empid AND ep.empstatus = 'A' AND ep.empesfera = 'M'
                    JOIN entidade.endereco ed ON ed.endid = o.endid
                    JOIN territorios.municipio m ON m.muncod = ed.muncod
                    WHERE o.obridpai IS NULL AND o.obrstatus = 'A' AND o.obrid = $obrid

                )";

        $result = $this->pegaUm($sql);

        return (!empty($result)) ? true : false;
    }


    public function verificaObraEmVermelho($obrid)
    {
        $sql = "
            SELECT DISTINCT o.*
            FROM obras2.obras o
            LEFT JOIN workflow.documento 		 d ON d.docid = o.docid AND tpdid = 105
            LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = d.esdid
            LEFT JOIN obras2.empreendimento 	 e ON e.empid = o.empid AND e.empstatus = 'A'
            JOIN entidade.endereco edr ON edr.endid = o.endid
            WHERE
                o.obrstatus = 'A' AND o.obridpai IS NULL AND e.empesfera = 'E' AND d.esdid IN(691,690) AND e.orgid IN(3) AND
                CASE
                    WHEN
                    (obrdtultvistoria IS NOT NULL AND
                    ed.esdid IN (690, 691) AND
                    DATE_PART('days', NOW() - obrdtultvistoria) > 30 AND
                    obrpercentultvistoria < 100.00) THEN
                        true
                    ELSE
                        false
                END
                AND o.obrid = $obrid ";

        $result = $this->pegaUm($sql);

        return (!empty($result)) ? true : false;
    }



    public function percentualCronogramaExecutado($obrid)
    {

        $sql = "
            SELECT
                SUM(ic.icovlritem) valorobra
            FROM obras2.obras o
            JOIN obras2.cronograma c ON c.obrid = o.obrid AND c.crostatus = 'A'
            JOIN obras2.itenscomposicaoobra ic ON ic.obrid = o.obrid AND ic.croid = c.croid AND ic.icostatus = 'A' AND ic.relativoedificacao = 'D'
            JOIN obras2.itenscomposicao i ON i.itcid = ic.itcid AND i.itcstatus = 'A'
            WHERE
                o.obrid = $obrid";
        $valorObra = $this->pegaUm($sql);

        $sql = "
        SELECT
            ic.icopercsobreestrutura,
            ic.icovlritem,
            ic.icodtinicioitem,
            ic.icodterminoitem,
            ic.icopercexecutado,

            CASE WHEN ic.icodterminoitem <= NOW() THEN
              DATE_PART ('day', (ic.icodterminoitem - ic.icodtinicioitem))
            ELSE
              DATE_PART ('day', (NOW() - ic.icodtinicioitem))
            END diasexecutados,

			DATE_PART ('day', (ic.icodterminoitem - ic.icodtinicioitem)) totaldiasexecucao
        FROM obras2.obras o
        JOIN obras2.cronograma c ON c.obrid = o.obrid AND c.crostatus = 'A'
        JOIN obras2.itenscomposicaoobra ic ON ic.obrid = o.obrid AND ic.croid = c.croid AND ic.icostatus = 'A' AND ic.relativoedificacao = 'D'
        JOIN obras2.itenscomposicao i ON i.itcid = ic.itcid AND i.itcstatus = 'A'
        WHERE
            o.obrid = $obrid
            AND (ic.icodtinicioitem <= NOW() )

        ";

        $itensExecucao = $this->carregar($sql);

        $percentualTotalPrevisto = 0;

        if(!empty($itensExecucao)) {
            foreach ($itensExecucao as $key => $item) {
                $percentualExecucao = ($item['totaldiasexecucao'] == 0) ? 100 : ($item['diasexecutados'] * 100 / $item['totaldiasexecucao']);
                $valorPrevisto = ($item['icovlritem'] * $percentualExecucao) / 100;
                $percentualPrevisto = ($valorPrevisto * 100) / $valorObra;

                $itensExecucao[$key]['percentualprevisto'] = $percentualExecucao;
                $itensExecucao[$key]['percentualprevistoobra'] = $percentualPrevisto;
                $itensExecucao[$key]['valorprevisto'] = $valorPrevisto;

                $percentualTotalPrevisto += $percentualPrevisto;
            }
        }

        // Quando obra vinculada, faz o cálculas para estimativa com o percentual aproveitado
        $obras = new Obras($obrid);
        $dadosPercentual         = $obras->getPercentExecutadoAproveitadoObraAnterior($this->obrid);
        $percentual_aproveitavel = ($dadosPercentual) ? $dadosPercentual['obrperccontratoanterior'] : 0;
        $percentual_do_todo      = 100 - $percentual_aproveitavel;
        $percentual_final        = round((($percentualTotalPrevisto/100)*$percentual_do_todo), 2)+$percentual_aproveitavel;

        return $percentual_final;
    }

    public function controlaInconformidadeCronogramaDesatualizado($obrid, $criar = true, $superar = true)
    {
        $obra = new Obras($obrid);

        $sql = "SELECT
                    r.rstid,
                    r.docid,
                    CASE
                      WHEN e.esdid = 1141 THEN 3867
                      WHEN e.esdid = 1144 THEN 3868
                      WHEN e.esdid = 1140 THEN 3866
                    END aedid
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid
                JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                WHERE
                    r.rstdsc = 'A execução física da obra está divergente do cronograma preenchido.'
                    AND r.rststatus = 'A'
                    AND e.esdid IN (1141 /*Aguardando análise FNDE*/, 1144/*Aguardando correção*/, 1140/*Aguardando Providência*/)
                    AND r.obrid = $obrid";
        $restricao = $this->pegaLinha($sql);

        if(empty($restricao) && $criar) {
            // Quando o percentual da obra é 20% menor que o percentual previsto no cronograma, cria uma inconformidade
            $percentualTotalPrevisto = $this->percentualCronogramaExecutado($obrid);
            $percentTotal            = $obra->pegaPercentualExecucao($obrid);
            if (($percentTotal + 20) <= $percentualTotalPrevisto) {
                $date = new DateTime(date('Y-m-d'));
                $date->modify('+30 day');

                $restricao = new Restricao();
                $arDado    = array(
                    'tprid' => 20,
                    'fsrid' => 1,
                    'empid' => NULL,
                    'obrid' => $obrid,
                    'usucpf' => '00000000191',
                    'rstdsc' => 'A execução física da obra está divergente do cronograma preenchido.',
                    'rstdtprevisaoregularizacao' => $date->format('Y-m-d'),
                    'rstdscprovidencia' => 'O cronograma da obra deve ser atualizado, conforme, execução física. Utilize a opção "Editar prazos do cronograma" existente na  lista de opções da obra.',
                    'rstitem' => 'I'
                );

                $restricao->popularDadosObjeto($arDado)->salvar(true, true, $arCamposNulo);
                $restricao->atualizaDocidNullRetricao($restricao->rstid);
                $restricao->clearDados();
            }
        } else if (!empty($restricao) && $superar) {
            $percentualTotalPrevisto = $this->percentualCronogramaExecutado($obrid);
            $percentTotal            = $obra->pegaPercentualExecucao($obrid);

            if (($percentTotal + 20) >= $percentualTotalPrevisto) {
                $r = wf_alterarEstado($restricao['docid'], $restricao['aedid'], 'Cronograma atualizado.', array('obrid' => $os->obrid));

                $sql = "UPDATE workflow.historicodocumento SET usucpf = '00000000191' WHERE docid = {$restricao['docid']} AND aedid IN ({$restricao['aedid']})";
                $this->executar($sql);
                $this->commit();
            }
        }
    }

}
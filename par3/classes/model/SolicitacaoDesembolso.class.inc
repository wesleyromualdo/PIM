<?php

/**
 * Created by PhpStorm.
 * User: danielmarconi
 * Date: 04/05/2018
 * Time: 17:46
 */
class Par3_Model_SolicitacaoDesembolso extends Modelo
{
    protected $stNomeTabela = 'par3.solicitacao_desembolso_par';

    protected $arChavePrimaria = array(
        'sdpid',
    );

    protected $arChaveEstrangeira = array(
        'ecoid' => array('tabela' => 'par3.execucao_contrato', 'pk' => 'ecoid'),
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'proid' => array('tabela' => 'par3.processo', 'pk' => 'proid'),
    );

    protected $arAtributos = array(
        'sdpid' => null,
        'ecoid' => null,
        'edlid' => null,
        'docid' => null,
        'proid' => null,
        'sdpvalor' => null,
        'sdpobservacao' => null,
        'sdpsaldotermo' => null,
        'usucpf' => null,
        'sdpdtinclusao' => null,
        'sdpstatus' => null,
    );

    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui
        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function recuperarDadosSolicitacaoDesembolso($ecoid = null, $edlid = null){
        $sql = '';

        if($edlid > 0){
            $sql = $this->sqlDesembolsoDocumento($edlid);
        }else if($ecoid > 0){
            $sql = $this->sqlDesembolsoContrato($ecoid);
        }

        return $this->carregar($sql);
    }

    public function sqlDesembolsoContrato($ecoid){
        $sql = "SELECT 
                  sdpid,
                  eco.ecoid,
                  sdpobservacao,
                  sdpvalor,
                  usu.usunome,
                  to_char(sdpdtinclusao, 'dd/mm/YYYY') as datacriacao,
                  esd.esddsc,
                  esd.esdid,
                  sdp.docid,
                  ecocnpj as cnpjfornecedor,
                  ecorazaosocialfornecedor as razaosocial,
                  econumero as contrato,
                  to_char(ecodtainclusao,'dd/mm/YYYY') as datacriacaodocumento,
                  concat(dot.dotnumero,'-',dot.intoid)  as codigotermo,
                  pronumeroprocesso,
                  to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' - (' || (dot.dotdatafimvigencia::date - CURRENT_DATE) || ' dias)' as dotdatafimvigencia,
                  ecovalor,
                  dotvalortermo as vlrtermo,
                  pro.proid,
                  ecostatus,
                  coalesce(vsaldo.vlrpago,0) as totalpago,
                  v.valorsolicitacoes,
                  pro.proid,
                  sdpsaldotermo,
                  eco.arqid,
                  arqnome
                FROM  par3.processo pro
                INNER JOIN par3.execucao_contrato eco ON eco.proid = pro.proid
                LEFT JOIN par3.solicitacao_desembolso_par sdp ON eco.ecoid = sdp.ecoid AND sdpstatus = 'A' 
                LEFT JOIN workflow.documento doc ON doc.docid = sdp.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                LEFT JOIN seguranca.usuario usu ON usu.usucpf = sdp.usucpf
                INNER JOIN par3.documentotermo dot ON dot.proid = pro.proid
                INNER JOIN par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.processo = pro.pronumeroprocesso
                INNER JOIN arquivo arq ON arq.arqid = eco.arqid
                LEFT JOIN LATERAL (SELECT sum(sdpvalor) as valorsolicitacoes
                          FROM par3.solicitacao_desembolso_par a 
                          INNER JOIN workflow.documento c ON c.docid = a.docid
                          WHERE a.proid = pro.proid AND c.esdid = " . PAR3_DESEMBOLSO_APROVADO . "
                          ) v on true
                WHERE eco.ecoid = {$ecoid} AND dotstatus = 'A'
                ORDER BY sdpid DESC
                ";
        return $sql;
    }

    public function sqlDesembolsoDocumento($edlid){
        $sql = "SELECT 
                  sdpid,
                  edl.edlid,
                  sdpobservacao,
                  sdpvalor,
                  usu.usunome,
                  to_char(sdpdtinclusao, 'dd/mm/YYYY') as datacriacao,
                  esd.esddsc,
                  esd.esdid,
                  sdp.docid,
                  edlcnpjfornecedor as cnpjfornecedor,
                  edldsc,
                  edlvalordocumento,
                  doadescricao,
                  edldtainclusao as datacriacaodocumento,
                  concat(dot.dotnumero,'-',dot.intoid)  as codigotermo,
                  pronumeroprocesso,
                  to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' - (' || (dot.dotdatafimvigencia::date - CURRENT_DATE) || ' dias)' as dotdatafimvigencia,                
                  dotvalortermo as vlrtermo,
                  pro.proid,
                  edlstatus,
                  coalesce(vsaldo.vlrpago,0) as totalpago,
                  v.valorsolicitacoes,
                  pro.proid,
                  sdpsaldotermo,
                  arq.arqid,
                  arq.arqnome
                FROM  par3.processo pro
                INNER JOIN par3.execucao_documentolicitacao edl ON edl.proid = pro.proid
                LEFT JOIN par3.solicitacao_desembolso_par sdp ON sdp.edlid = edl.edlid AND sdpstatus = 'A' 
                LEFT JOIN par3.tipo_documento_anexo tda ON tda.doaid = edl.doaid
                LEFT JOIN workflow.documento doc ON doc.docid = sdp.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                LEFT JOIN seguranca.usuario usu ON usu.usucpf = sdp.usucpf
                INNER JOIN par3.documentotermo dot ON dot.proid = pro.proid
                INNER JOIN par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.processo = pro.pronumeroprocesso
                LEFT JOIN LATERAL (SELECT sum(sdpvalor) as valorsolicitacoes
                INNER JOIN arquivo arq ON arq.arqid = eco.arqid
                          FROM par3.solicitacao_desembolso_par a 
                          INNER JOIN workflow.documento c ON c.docid = a.docid
                          WHERE a.proid = pro.proid AND c.esdid = " . PAR3_DESEMBOLSO_APROVADO . "
                          ) v on true
                WHERE edl.edlid= {$edlid} AND dotstatus = 'A'
                ORDER BY sdpid DESC
                ";
        return $sql;
    }

    public function montaSqlListaSolicitacoes($filtros){
        $where  = '';

        if(strlen( $filtros['dotnumero']) > 9){
            $filtros['dotnumero'] = substr($filtros['dotnumero'], 0, 9);
        }

        $where .= $filtros['inuid'] ? ' AND inu.inuid = ' . $filtros['inuid'] : '';
        $where .= $filtros['dotnumero'] ? " AND dotnumero::TEXT like '%{$filtros['dotnumero']}%'" : '';
        $where .= $filtros['muncod'] ? " AND inu.muncod like '%{$filtros['muncod']}%'" : '';
        $where .= $filtros['esdid_analise'] ? " AND esd.esdid = {$filtros['esdid_analise']} " : '';
        $where .= $filtros['econumero'] ? " AND econumero::TEXT like '%{$filtros['econumero']}%' " : '';
        $where .= $filtros['sdpcod'] ? " AND (sdpid = {$filtros['sdpcod']} OR sldid = {$filtros['sdpcod']})" : '';
        $where .= $filtros['obrid'] ? " AND (o.obrid = {$filtros['obrid']} OR o.obrid_par3 = {$filtros['obrid']})" : '';
        $where .= $filtros['anoprocesso'] ? " AND substring(pronumeroprocesso,12,4)::int = {$filtros['anoprocesso']} " : '';

        if($filtros['dotnumero'] == '' && $filtros['econumero'] == '' && $filtros['sdpcod']  == ''){
            $where .= $filtros['tipo'] == 'P' ? " AND dot.intoid <> 1 " : '';
            $where .= $filtros['tipo'] == 'O' ? " AND dot.intoid = 1 " : '';
        }

        if($filtros['sdpdtinicio'] && !$filtros['sdpdtfim']){
            $where .= " AND sdpdtinclusao >= '" . formata_data_sql($filtros['sdpdtinicio']) . "'";
        }else if(!$filtros['sdpdtinicio'] && $filtros['sdpdtfim']){
            $where .= " AND sdpdtinclusao <= '" . formata_data_sql($filtros['sdpdtfim']) . "'";
        }else if($filtros['sdpdtinicio'] && $filtros['sdpdtfim']){
            $where .= " AND sdpdtinclusao BETWEEN '" . formata_data_sql($filtros['sdpdtinicio']) . "' AND '" . formata_data_sql($filtros['sdpdtfim']) . "'";
        }

        $dropupObra = "
    <div class=\'btn-group dropup\' >
      <span type=\'button\' class=\'btn btn-secondary dropdown-toggle\' data-toggle=\'dropdown\' aria-haspopup=\'true\' aria-expanded=\'false\'>
        Obra: ' || o.obrid || ' <br><i style=\'color: #64a0e8;\' class=\'fa fa-info-circle\'></i>
      </span>
      <div class=\'dropdown-menu\' style=\'padding: 8px\'>
        Pré-ID: ' || o.obrid_par3 || '
      </div>
    </div>";

        $sql = "SELECT 
                  CASE WHEN eco.ecoid IS NOT NULL 
                    THEN eco.ecoid 
                    ELSE -1 
                  END AS ecoid,
                  edl.edlid,
                  sdpid,
                  --CASE WHEN sldid IS NOT NULL THEN sldid ELSE sdpid END as solicitacao,
                  pronumeroprocesso,
                  dotnumero || '-' || dot.intoid as termoCompromisso,
                  to_char(dotdatainclusao, 'YYYY') as ano,
                  to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' <br> (' || (dot.dotdatafimvigencia::date - CURRENT_DATE) || ' dias)' as dotdatafimvigencia, -- Vigência
                  CASE WHEN eco.ecoid IS NOT NULL THEN 'Contrato' 
                       WHEN sldid IS NOT NULL THEN '{$dropupObra}' 
                       ELSE doadescricao 
                  END as documento,
                  CASE WHEN eco.ecoid IS NOT NULL
                    THEN ecocnpj
                    ELSE edlcnpjfornecedor
                  END as cnpj,
                  CASE WHEN eco.ecoid IS NOT NULL 
                    THEN ecovalor
                    ELSE edlvalordocumento
                  END as valordocumento,
                  CASE WHEN sdpid > 0 
                    THEN round(sdpvalor,2)
                    ELSE round((sldpercpagamento*obrvalor)/100,2)
                  END as valorsolicitacao,
                  coalesce(ecovalor - totalSolicitacao, ecovalor) as saldo,
                  dotvalortermo,
                  CASE WHEN sdpid > 0
                    THEN coalesce(round((sdpvalor / dotvalortermo) * 100,2), 0) || '%'
                    ELSE coalesce(sldpercpagamento, 0) || '%'
                  END as per_sol,
                  --(case when  dotvalortermo > 0 then ((sdpvalor / dotvalortermo) * 100) ELSE 0 end)::NUMERIC(20,2) ||'%' as per_sol,
                  vlrpago as vlr_pago,
                  vlrempenho as vlr_emp,
                  (case when  vlrempenho > 0 then ((vlrpago / vlrempenho) * 100) ELSE 0 end)::NUMERIC(20,2) ||'%' as per_emp,
                  vlriniciativa as vlr_processo,
                  (case when  vlriniciativa > 0 then ((vlrpago / vlriniciativa) * 100) ELSE 0 end)::NUMERIC(20,2) ||'%' as per_processo,
                  CASE WHEN sldid IS NOT NULL THEN to_char(slddatainclusao, 'dd/mm/YYYY')
                       ELSE to_char(sdpdtinclusao, 'dd/mm/YYYY') 
                  END as datasolicitacao, 
                  esddsc,
                  CASE WHEN eco.ecoid IS NOT NULL 
                    THEN eco.arqid
                    ELSE edl.arqid
                  END as arqid,
                  CASE WHEN poc.obrid > 0 THEN 'obra' ELSE 'par' END AS tipo,
                  pro.proid,
                  esd.esdid
                 FROM par3.processo pro
                INNER JOIN par3.documentotermo dot ON dot.proid = pro.proid
                LEFT JOIN par3.solicitacao_desembolso_par sdp ON sdp.proid = pro.proid and sdpstatus = 'A'
                LEFT JOIN par3.execucao_contrato eco ON eco.ecoid = sdp.ecoid AND ecostatus = 'A'
                LEFT JOIN par3.execucao_documentolicitacao edl ON edl.edlid = sdp.edlid AND edlstatus = 'A'
                LEFT JOIN par3.instrumentounidade inu ON inu.inuid = pro.inuid
                LEFT JOIN par3.tipo_documento_anexo tda ON tda.doaid = edl.doaid
                LEFT JOIN par3.processoobracomposicao poc ON poc.proid = dot.proid and poc.pocstatus = 'A'
                LEFT JOIN par3.obra ob ON ob.obrid = poc.obrid AND ob.obrstatus = 'A'::bpchar
                LEFT JOIN obras2.obras o ON o.obrid_par3 = poc.obrid
                LEFT JOIN obras2.solicitacao_desembolso sdv ON sdv.obrid = o.obrid and sdv.sldstatus = 'A'
                INNER JOIN workflow.documento doc ON doc.docid = sdp.docid or doc.docid = sdv.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                LEFT JOIN LATERAL (SELECT sum(sdpvalor) totalSolicitacao FROM par3.solicitacao_desembolso_par sdp
                                     INNER JOIN workflow.documento doc ON sdp.docid = doc.docid
                                     WHERE sdp.ecoid = eco.ecoid 
                                     AND sdpstatus = 'A' AND doc.esdid = ".PAR3_DESEMBOLSO_APROVADO.") v on true
                LEFT JOIN LATERAL (SELECT sum(t.aicqtdaprovado* t.aicvaloraprovado) as valordoc, sum(aicqtdaprovado) as qtd
                                      FROM par3.analise_itemcomposicao t 
                                      INNER JOIN par3.execucao_documentolicitacao_itens t2 ON t2.aicid = t.aicid
                                      WHERE t2.edlid = edl.edlid
                                      ) v2 on true
                INNER JOIN(
                        	SELECT pp.proid, sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa, 'P' as tipo_processo
                		    FROM par3.iniciativa_planejamento_item_composicao ipi
                				INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
                				INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
                				INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid
                			WHERE ipi.ipistatus = 'A' AND pp.ppcstatus = 'A' 
                			GROUP BY pp.proid
                			UNION ALL
                			SELECT po.proid, sum(o.obrvalor) AS vlriniciativa, 'O' as tipo_processo
                            FROM par3.obra o
                         		INNER JOIN par3.processoobracomposicao po ON po.obrid = o.obrid AND po.pocstatus = 'A'
                         	WHERE o.obrstatus = 'A' GROUP BY po.proid
                        )pp ON pp.proid = pro.proid
                left join par3.v_saldo_empenho_do_processo e on e.processo = pro.pronumeroprocesso
                WHERE dotstatus = 'A'
                {$where}
                ";
//ver($sql);
        return $sql;
    }

    public function pegaSqlSituacaoAnalise(){
        $sql = "SELECT codigo, descricao FROM(
				SELECT DISTINCT 
						e.esdid AS codigo,
					    e.esddsc AS descricao,
					    e.esdordem
					FROM workflow.estadodocumento e 
					WHERE e.tpdid = " . PAR3_FLUXO_SOLICITACAO_DESEMBOLSO . "
					ORDER BY e.esddsc ASC
				) AS foo
				ORDER BY descricao ASC";
        return $sql;
    }

}
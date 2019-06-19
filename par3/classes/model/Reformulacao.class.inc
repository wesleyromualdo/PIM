<?php
class Par3_Model_Reformulacao extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.processo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('proid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                        'pronumeroprocesso'     => null,
                                        'muncod'                => null,
                                        'probanco'              => null,
                                        'proagencia'            => null,
                                        'prodatainclusao'       => null,
                                        'usucpf'                => null,
                                        'proseqconta'           => null,
                                        'protipo'               => null,
                                        'seq_conta_corrente'    => null,
                                        'nu_conta_corrente'     => null,
                                        'inuid'                 => null,
                                        'protipoexecucao'       => null,
                                        'pronumeroconveniofnde' => null,
                                        'proanoconveniofnde'    => null,
                                        'pronumeroconveniosiafi'=> null,
                                        'prodocumenta'          => null,
                                        'procnpj'               => null,
                                        'prostatus'             => null,
                                        'dt_movimento'          => null,
                                        'fase_solicitacao'      => null,
                                        'co_situacao_conta'     => null,
                                        'situacao_conta'        => null,
                                        'nu_identificador'      => null,
                                        'ds_razao_social'       => null,
                                        'arqidanexodoc'         => null,
                                        'prostatusmotivo'       => null,
                                        'prodataconsultasigef'  => null,
                                        'profinalizado'         => null,
                                        'prodatapagamentosigef' => null,
                                        'intaid'                => null,
                                        'proid'                 => null,
                                      );
                                  
    function __construct($id = null)
    {
        parent::__construct($id);
    }
    
    public function recuperar($arrPost)
    {
        return $this->model;
    }
    
    public function sqlListaReformulacaoIniciativa( $where, $arrPost = '' )
    {
        $sql = "SELECT DISTINCT dotid, proid, inuid, dotnumero, pronumeroprocesso, uf, entidade,
                    tipo_obrjeto, data_vigencia, dotvalortermo, vlrempenho, vlrpago, ta.intadsc, tirid
                FROM(
                    SELECT distinct p.proid, p.inuid,
                        dt.dotid,
                        dt.dotnumero,
                	    p.pronumeroprocesso,
                	    case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                        ''||case when iu.muncod is null then est.estdescricao else mun.mundescricao end||'' as entidade,
                	    array_to_string(array(SELECT DISTINCT t.intodsc FROM par3.iniciativa_tipos_objeto t
                	    						INNER JOIN par3.processoparcomposicao pp ON pp.intoid = t.intoid AND pp.ppcstatus = 'A'
                	    						WHERE t.intostatus = 'A'
                	    						AND pp.proid = p.proid), ', ') AS tipo_obrjeto,
                	    to_char(dt.data_vigencia, 'DD/MM/YYYY') as data_vigencia,
                	    dt.dotvalortermo,
                	    e.vlrempenho as vlrempenho,
                	    coalesce(e.vlr_pg_efetivado,0) as vlrpago,
                        p.intaid,
                        dt.esdid,
                        dt.refid,
                        '".$arrPost['tirid']."' as tirid
                	FROM par3.processo p
                		INNER JOIN par3.processoparcomposicao pp ON pp.proid = p.proid AND pp.ppcstatus = 'A'
                	    inner join par3.v_saldo_empenho_do_processo e on e.processo = p.pronumeroprocesso
                	    inner join par3.iniciativa_planejamento ip on ip.inpid = pp.inpid
                	    inner join par3.instrumentounidade iu on iu.inuid = ip.inuid and iu.inustatus = 'A'
                	    inner join par3.iniciativa ini on ini.iniid = ip.iniid
                	    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
                	    LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
                	    LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                	    INNER JOIN (
                			SELECT d.dotid, d.proid, (d.dotnumero || '-'::text) || d.intoid AS dotnumero, d.dotdatafimvigencia AS data_vigencia, sum(d.dotvalortermo) AS dotvalortermo,
                	    		md.mdonome||'_'||ito.intodsc as termo, rd.refid, rt.tirdescricao, rq.vlr_solicitado, rq.vlr_aprovado, esd.esddsc, to_char(rd.refdata, 'YYYY') as ano, rd.refdata::date, rd.tirid, esd.esdid, rd.docid
                			FROM par3.documentotermo d
                				inner join par3.modelodocumento md on md.mdoid = d.mdoid and md.mdostatus = 'A'
                	           	inner join par3.iniciativa_tipos_objeto ito on ito.intoid = d.intoid
                	           	INNER JOIN par3.reformulacao_documento rd ON rd.dotid = d.dotid AND rd.refstatus = 'A' AND rd.refidpai IS NOT NULL
                                INNER JOIN workflow.documento doc ON doc.docid = rd.docid
                	           	INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                	           	left JOIN(
                	           		SELECT ri.refid, sum(ri.reiquantidade * ri.reivalor) AS vlr_solicitado, sum(ra.raiqtdaprovado * ra.raivaloraprovado) AS vlr_aprovado
                	           		FROM par3.reformulacao_itemcomposicao ri
                	           			INNER JOIN par3.reformulacao_analise_itemcomposicao ra ON ra.reiid = ri.reiid AND ra.raistatus = 'A'
                	           		WHERE ri.reistatus = 'A' AND ri.refidpai IS NOT NULL
                	           		GROUP BY ri.refid
                	           	) rq ON rq.refid = rd.refid
                	           	INNER JOIN par3.reformulacao_tipos rt ON rt.tirid = rd.tirid
                	        WHERE d.dotstatus IN ('A', 'B') /*descomentei porque estava duplicando a lista de reformulação, tinha 3 termos para o processo, um com status A e outro B e outro com I*/
                	        GROUP BY d.dotid, d.proid, d.dotnumero, d.intoid, d.dotdatafimvigencia, md.mdonome, ito.intodsc, rd.refid, rt.tirdescricao, rq.vlr_solicitado, rq.vlr_aprovado, esd.esddsc, esd.esdid, rd.docid
                	    ) dt ON dt.proid = p.proid
                	WHERE
                	    ip.inpstatus = 'A'
                        and e.vlrempenho > 0
                        $where
                	GROUP BY dt.dotid, p.proid, p.inuid, dt.dotnumero, p.pronumeroprocesso, iu.muncod, est.estuf, mun.estuf, est.estdescricao, mun.mundescricao,
                	    dt.data_vigencia, dt.dotvalortermo, e.vlrempenho, e.vlr_pg_efetivado, p.intaid, dt.termo, dt.refid, dt.docid, dt.esdid
                ) as foo
                INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = foo.intaid
                order by uf, entidade";
        return $sql;
    }
    
    public function sqlListaReprogramacaoPrazo( $where )
    {
        $sql = "SELECT dotid, proid, inuid, refid, uf, entidade, pronumeroprocesso, dotnumero, tipo_obrjeto, data_vigencia, data_solicitacao, data_requisitada, 
                    data_aprovada, data_validacao, ta.intadsc, esdid, situacao, tirid, refjustificativa, justificativa_completa, tipo
                FROM(
                    SELECT distinct p.proid, p.inuid,
                        dt.dotid,
                        dt.dotnumero,
                	    p.pronumeroprocesso,
                	    case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                        ''||case when iu.muncod is null then est.estdescricao else mun.mundescricao end||'' as entidade,
                	     array_to_string(array(SELECT DISTINCT t.intodsc FROM par3.iniciativa_tipos_objeto t
                	    						INNER JOIN (
                	    							SELECT pp.proid, pp.intoid FROM par3.processoparcomposicao pp WHERE pp.ppcstatus = 'A'
					                			  		UNION ALL 
					                			 	SELECT pp.proid, pp.intoid FROM par3.processoobracomposicao pp WHERE pp.pocstatus = 'A') pp1 ON pp1.intoid = t.intoid
                	    						WHERE t.intostatus = 'A'
                	    						AND pp1.proid = p.proid), ', ') AS tipo_obrjeto,
                	    to_char(dt.data_vigencia, 'DD/MM/YYYY') as data_vigencia,
                	    to_char(dt.data_requisitada, 'DD/MM/YYYY') as data_requisitada,
                	    to_char(dt.data_aprovada, 'DD/MM/YYYY') as data_aprovada,
                	    to_char(dt.data_solicitacao, 'DD/MM/YYYY HH24:MI:SS') as data_solicitacao,
                	    dt.esddsc as situacao,
                	    dt.refid,
                        substring(dt.refjustificativa,1,50)||'...' AS refjustificativa,
                        dt.refjustificativa as justificativa_completa, 
                        dt.data_validacao,
                        p.intaid,
                        dt.esdid,
                        '1' as tirid,
                        pp.tipo
                	FROM par3.processo p
                		INNER JOIN(                		
                			SELECT pp.proid, pp.inpid, 'P' as tipo FROM par3.processoparcomposicao pp WHERE pp.ppcstatus = 'A'
                			UNION ALL 
                			SELECT pp.proid, o.inpid, 'O' as tipo FROM par3.processoobracomposicao pp 
                				INNER JOIN par3.obra o ON o.obrid = pp.obrid AND o.obrstatus = 'A'
                			WHERE pp.pocstatus = 'A'                			
                		) pp ON pp.proid = p.proid
                	    inner join par3.v_saldo_empenho_do_processo e on e.processo = p.pronumeroprocesso
                	    inner join par3.iniciativa_planejamento ip on ip.inpid = pp.inpid
                	    inner join par3.instrumentounidade iu on iu.inuid = ip.inuid and iu.inustatus = 'A'
                	    inner join par3.iniciativa ini on ini.iniid = ip.iniid
                	    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
                	    LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
                	    LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                	    INNER JOIN (                	    
                	    	SELECT rd.refid, d.dotid, d.proid, (d.dotnumero || '-'::text) || d.intoid AS dotnumero, d.dotdatafimvigencia AS data_vigencia,
                	    		md.mdonome||'_'||ito.intodsc as termo, rt.tirdescricao, esd.esddsc, to_char(rd.refdata, 'YYYY') as ano, 
                	    		rd.refdata::date, rd.tirid, esd.esdid, rd.docid, rd.refdata_solicitada AS data_requisitada, rd.refdata_aprovada AS data_aprovada,
                                CASE WHEN esd.esdid = 2462 THEN to_char(dtv.data_validacao, 'DD/MM/YYYY HH24:MI:SS') ELSE '-' END AS data_validacao,
                	    		(SELECT max(htddata) FROM workflow.historicodocumento WHERE docid = rd.docid AND aedid = 5772) AS data_solicitacao, rd.refjustificativa
                			FROM par3.documentotermo d
                				INNER JOIN par3.modelodocumento md on md.mdoid = d.mdoid and md.mdostatus = 'A'
                	           	INNER JOIN par3.iniciativa_tipos_objeto ito on ito.intoid = d.intoid
                	           	INNER JOIN par3.reformulacao_documento rd ON rd.dotid = d.dotid AND rd.refstatus = 'A' AND rd.refidpai IS NOT NULL
                                INNER JOIN workflow.documento doc ON doc.docid = rd.docid
                	           	INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                                LEFT JOIN(                	           	
                	           		SELECT max(dtvdatavalidacao) AS data_validacao, dotid FROM par3.documentotermovalidacao WHERE dtvstatus = 'A' GROUP BY dotid                	           		
                	           	) dtv ON dtv.dotid = d.dotid
                	           	LEFT JOIN(
                	           		SELECT ri.refid, sum(ri.reiquantidade * ri.reivalor) AS vlr_solicitado, sum(ra.raiqtdaprovado * ra.raivaloraprovado) AS vlr_aprovado
                	           		FROM par3.reformulacao_itemcomposicao ri
                	           			INNER JOIN par3.reformulacao_analise_itemcomposicao ra ON ra.reiid = ri.reiid AND ra.raistatus = 'A'
                	           		WHERE ri.reistatus = 'A' AND ri.refidpai IS NOT NULL
                	           		GROUP BY ri.refid
                	           	) rq ON rq.refid = rd.refid
                	           	INNER JOIN par3.reformulacao_tipos rt ON rt.tirid = rd.tirid
                	        WHERE rd.tirid = 1 AND rd.refid = (SELECT max(refid) FROM par3.reformulacao_documento rd 
										                	    	INNER JOIN workflow.documento dc ON dc.docid = rd.docid
										                	    WHERE rd.proid = d.proid AND rd.dotid = d.dotid 
										                	    	AND rd.refidpai IS NOT NULL AND rd.tirid = 1 
										                	    	AND dc.esdid NOT IN (".PAR3_REFORMULACAO_PRAZO_ESDID_EM_REPROVADA.", ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_FINALIZADA_REPROVADA."))										                	    	
                	    ) dt ON dt.proid = p.proid
                	WHERE
                	    ip.inpstatus = 'A'
                        and e.vlrempenho > 0
                        $where
                	GROUP BY dt.dotid, p.proid, p.inuid, dt.dotnumero, p.pronumeroprocesso, iu.muncod, est.estuf, mun.estuf, est.estdescricao, mun.mundescricao, dt.data_vigencia, p.intaid, dt.termo, 
                    dt.refid, dt.docid, dt.esdid, dt.data_requisitada, dt.data_aprovada, dt.esddsc, dt.data_solicitacao, dt.refjustificativa, pp.tipo, dt.data_validacao
                ) as foo
                INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = foo.intaid
                order by uf, entidade";
        return $sql;
    }
}

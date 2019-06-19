<?php

class Par3_Model_DocumentoTermo extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.documentotermo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('dotid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('dotid', 'tirid', 'proid', 'mdoid', 'docid', 'dotidpai', 'dotnumero', 'dotstatus', 'dotdatainclusao', 'usucpfinclusao', 'dotdataalteracao',
        'usucpfalteracao', 'dotdatainiciovigencia', 'dotdatafimvigencia', 'dotvalortermo', 'intoid', 'dotvalorcontrapartida', 'refid', 'dotjustificativa_de_oficio', 'dotdata_de_oficio');

    protected $arAtributos = array(
        'proid' => null,
        'mdoid' => null,
        'docid' => null,
        'dotidpai' => null,
        'usucpf' => null,
        'dotnumero' => null,
        'dotstatus' => null,
        'dotdatainclusao' => null,
        'usucpfinclusao' => null,
        'dotdataalteracao' => null,
        'usucpfalteracao' => null,
        'dotdatainiciovigencia' => null,
        'dotdatafimvigencia' => null,
        'dotvalortermo' => null,
        'dotvalorcontrapartida' => null,
        'intoid' => null,
        'refid' => null,
        'dotjustificativa_de_oficio' => null,
        'dotdata_de_oficio' => null
    );

    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listarDocPAR($where, $whereObra, $wherePlan)
    {
        $joinUsuario    = '';
        $colunaUsu      = '';
        $colunaUsuOuter = '';
        $colunaUsuInner = '';
        $colunaGroup    = '';
        if ($_REQUEST['requisicao'] == 'excel') {
            $colunaExcel = ",CASE WHEN obras_par = 't' THEN 'Sim' ELSE 'Não' END AS obras_par,
                        	CASE WHEN cacs = 't' THEN 'Sim' ELSE 'Não' END AS cacs,
                        	CASE WHEN pne = 't' THEN 'Sim' ELSE 'Não' END AS pne,
                        	CASE WHEN siope = 't' THEN 'Sim' ELSE 'Não' END AS siope,
                        	CASE WHEN habilitacao = 't' THEN 'Sim' ELSE 'Não' END AS habilitacao,
                        	CASE WHEN contas = 't' THEN 'Sim' ELSE 'Não' END AS contas";
            $joinExcel      = "inner join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = foo.inuid ";
            $joinUsuario    = "inner join seguranca.usuario usu on usu.usucpf = d.usucpfinclusao";
            $colunaUsu      = ",usu.usunome";
            $colunaUsuOuter = ",usunome";
            $colunaUsuInner = ",(usunome ||' - '||dt.usucpfinclusao) as usunome";
            $colunaGroup    = ", dt.usunome,dt.usucpfinclusao ";
        }
        
        $sql = "SELECT
                    proid,
                    id,
                    tipo,
                    foo.inuid,
                    dotnumero,
                    pronumeroprocesso,
                    uf,
                    entidade,
                    tipo_obrjeto
                    {$colunaUsuOuter},
                    dotdatainclusao,
                    data_vigencia,
                    dtvdatavalidacao,
                    saldo_processo,
                    valor_processo,
                    dotvalortermo,
                    vlrempenho,
                    vlrpago,
                    ta.intadsc,
                    foo.inuid as inuid_pendencia
                    {$colunaExcel},
                    dotdatainiciovigencia,
                    dotdadafimvigencia,
                    (case mdoex_oficio 
                        when 'S' then 'Sim'
                        when 'N' then 'Não'
                        else ' - ' 
                    end) as mdoex_oficio
                FROM(
                    SELECT distinct
                        p.proid,
                        p.proid as id,
                        case when vip.tipo_processo = 'P' then 'par' else 'obra' end as tipo,
                        p.inuid,
                        dt.dotnumero,
    				    p.pronumeroprocesso,
    				    case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                        ''||case when iu.muncod is null then est.estdescricao else mun.mundescricao end||'' as entidade,
    				    array_to_string(array(SELECT DISTINCT t.intodsc FROM par3.iniciativa_tipos_objeto t
    				    						INNER JOIN(
        				    						SELECT intoid, proid FROM par3.processoparcomposicao WHERE ppcstatus = 'A'
        				    						UNION ALL
        				    						SELECT intoid, proid FROM par3.processoobracomposicao WHERE pocstatus = 'A'
    				    						) pp ON pp.intoid = t.intoid
    				    						WHERE t.intostatus = 'A'
    				    						AND pp.proid = p.proid), ', ') AS tipo_obrjeto,
    				    to_char(dt.dotdatainiciovigencia, 'DD/MM/YYYY') as dotdatainiciovigencia,
    				    to_char(dt.dotdatafimvigencia, 'DD/MM/YYYY') as dotdadafimvigencia,
    				    to_char(dt.data_vigencia, 'DD/MM/YYYY') as data_vigencia,
                        (par.retornasaldoprocesso(p.pronumeroprocesso)) as saldo_processo,
    				    sum(vip.vlriniciativa::NUMERIC(20,2)) AS valor_processo,
    				    dt.dotvalortermo,
    				    e.vlrempenho as vlrempenho,
    				    coalesce(e.vlr_pg_efetivado,0) as vlrpago,
                        (
                            select
                                to_char(dtv.dtvdatavalidacao, 'DD/MM/YYYY') as dtvdatavalidacao
                            from par3.documentotermo dot2
                            inner join par3.documentotermovalidacao dtv on dtv.dotid = dot2.dotid
                            inner join par3.modelodocumento mdo on mdo.mdoid = dot2.mdoid
                            where dot2.dotnumero = dt.dotnum
                            and   (mdo.mdoex_oficio = 'N' or null)
                            order by dtvdatavalidacao desc limit 1
                        ) as dtvdatavalidacao,
                        p.intaid,
                        dt.mdoex_oficio,
                        to_char(dotdatainclusao, 'DD/MM/YYYY') as dotdatainclusao
                        {$colunaUsuInner}
    				FROM par3.processo p
    					INNER JOIN(
    					    SELECT pp.proid, sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa, a.inpid, 'P' as tipo_processo
    					    FROM par3.iniciativa_planejamento_item_composicao ipi
    	     					INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
    	     					INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
    	     					INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid AND pp.ppcstatus = 'A'
    	     				WHERE ipi.ipistatus = 'A' $wherePlan
    	     				GROUP BY pp.proid, a.inpid
    	     				UNION ALL
							SELECT po.proid, sum(o.obrvalor) AS vlriniciativa, o.inpid, 'O' as tipo_processo
							FROM par3.obra o
								INNER JOIN par3.processoobracomposicao po ON po.obrid = o.obrid AND po.pocstatus = 'A'
							WHERE o.obrstatus = 'A' $whereObra GROUP BY po.proid, o.inpid
    				     )vip ON vip.proid = p.proid
    				    inner join par3.v_saldo_empenho_do_processo e on e.processo = p.pronumeroprocesso
    				    inner join par3.iniciativa_planejamento ip on ip.inpid = vip.inpid
    				    inner join par3.instrumentounidade iu on iu.inuid = ip.inuid and iu.inustatus = 'A'
    				    inner join par3.iniciativa ini on ini.iniid = ip.iniid
    				    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
    				    LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
    				    LEFT JOIN territorios.estado est on est.estuf = iu.estuf
    				    LEFT JOIN (
    				    	select
    				    	    d.proid,
    				    	    d.dotid,
    				    	    (d.dotnumero || '-'::text) || d.intoid AS dotnumero,
    				    	    d.dotnumero as dotnum,
    				    	    d.dotdatainclusao,
    				    	    d.dotdatafimvigencia AS data_vigencia,
    				    	    d.dotdatafimvigencia,
    				    	    d.dotdatainiciovigencia,
    				    		sum(d.dotvalortermo) AS dotvalortermo,
    				    		d.mdoid,
    				    		md.mdoex_oficio,
    				    		d.usucpfinclusao
    				    	    {$colunaUsu}
							FROM par3.documentotermo d
								inner join par3.modelodocumento md on md.mdoid = d.mdoid
								{$joinUsuario}
							WHERE d.dotstatus IN ('A', 'B') 
								and d.dotid not in (select d1.dotid
													FROM par3.documentotermo d1
														inner join par3.modelodocumento md on md.mdoid = d1.mdoid
														inner join workflow.documento dc on dc.docid = d1.docid
													WHERE d1.dotstatus = 'B' and (md.mdoex_oficio = 'S' OR md.tpdcod = 21) and dc.esdid not in (2447) and d1.proid = d.proid)
    				        GROUP BY d.proid,d.dotid, d.dotnumero, d.intoid, d.dotdatafimvigencia,d.dotdatainiciovigencia,d.mdoid,d.dotdatainclusao,md.mdoex_oficio {$colunaUsu}
    				    ) dt ON dt.proid = p.proid
    				WHERE
    				    ip.inpstatus = 'A'
                        and e.vlrempenho > 0
                        $where
					GROUP BY p.proid,dt.dotid, p.inuid, dt.dotnumero, p.pronumeroprocesso, iu.muncod, est.estuf, mun.estuf, est.estdescricao, mun.mundescricao, vip.tipo_processo,
    				    dt.data_vigencia, dt.dotvalortermo, e.vlrempenho, e.vlr_pg_efetivado, p.intaid,dt.dotdatainiciovigencia,
    				    dt.dotdatafimvigencia,dt.mdoex_oficio,dt.dotdatainclusao,dt.dotnum {$colunaGroup}
                ) as foo
                INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = foo.intaid
                {$joinExcel}
                order by uf, entidade";

        return $sql;
    }
    
    public function listarExcelDetalhadoObra($where, $whereObra)
    {
        $sql = "WITH empenho_obra AS(
                	SELECT po.proid, po.obrid, (sum(ec.eocvalorempenho) - coalesce(sum(epc.vlrcancelado), 0)) AS vlrempenho, sum(epc.vlrcancelado) AS vlrcancelado 
                	FROM par3.empenho e
                		INNER JOIN par3.empenhoobracomposicao ec ON ec.empid = e.empid AND ec.eocstatus = 'A'
                		INNER JOIN par3.processoobracomposicao po ON po.pocid = ec.pocid AND po.pocstatus = 'A'
                		LEFT JOIN (SELECT po.obrid,
                	            		sum(epc.eocvalorempenho) AS vlrcancelado,
                	            		e.empidpai
                					FROM par3.empenho e
                	             		INNER JOIN par3.empenhoobracomposicao epc ON epc.empid = e.empid AND epc.eocstatus = 'A'::bpchar
                	             		INNER JOIN par3.processoobracomposicao po ON po.pocid = epc.pocid AND po.pocstatus = 'A'
                					WHERE e.empstatus = 'A'AND e.empcodigoespecie IN ('03', '13')
                					GROUP BY po.obrid, e.empidpai) epc ON epc.empidpai = e.empid AND epc.obrid = po.obrid
                	WHERE e.empcodigoespecie IN ('01')
                		AND e.empstatus = 'A'
                	GROUP BY po.obrid, po.proid
                )
                SELECT DISTINCT p.proid, 
                	'obra' as tipo,
                    o.inpid,
                	o.obrano,
                	p.inuid,
                    (dt.dotnumero || '-'::text) || po.intoid AS dotnumero,
                    p.pronumeroprocesso,
                	case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                    case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
                    obj.intodsc AS tipo_obrjeto,
                    (usu.usunome ||' - '||dt.usucpfinclusao) as usunome,
                    to_char(dt.dotdatainclusao, 'DD/MM/YYYY') as data_criacao,
                    to_char(dt.dotdatafimvigencia, 'DD/MM/YYYY') as data_vigencia,
                    (
                        select
                            to_char(dtv.dtvdatavalidacao, 'DD/MM/YYYY') as dtvdatavalidacao
                        from par3.documentotermo dot2
                        inner join par3.documentotermovalidacao dtv on dtv.dotid = dot2.dotid
                        inner join par3.modelodocumento mdo on mdo.mdoid = dot2.mdoid
                        where dot2.dotnumero = dt.dotnumero
                        and   (mdo.mdoex_oficio = 'N' or null)
                        order by dtvdatavalidacao desc limit 1
                    ) as dtvdatavalidacao,
                    (par.retornasaldoprocesso(p.pronumeroprocesso)) as saldo_processo,
                    o.obrvalor::NUMERIC(20,2) AS valor_processo,
                    dt.dotvalortermo,
                    COALESCE(e.vlrempenho,0) AS vlrempenho,
                    coalesce(pag.vlr_efetivado, 0) AS vlrpago,
                    ta.intadsc,
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
                    (case mdoex_oficio 
                        when 'S' then 'Sim'
                        when 'N' then 'Não'
                        else ' - ' 
                    end) as mdoex_oficio
                FROM par3.obra o
                	INNER JOIN par3.processoobracomposicao po ON po.obrid = o.obrid AND po.pocstatus = 'A'
                	INNER JOIN par3.processo p ON p.proid = po.proid AND p.prostatus = 'A'                	
                	INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = po.intoid
                	INNER JOIN par3.instrumentounidade iu on iu.inuid = o.inuid and iu.inustatus = 'A'
                	INNER JOIN par3.iniciativa_planejamento ip on ip.inpid = o.inpid
                	INNER JOIN par3.iniciativa ini on ini.iniid = ip.iniid
                	INNER JOIN workflow.documento d ON d.docid = o.docid
                	INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                	INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = p.intaid
                    INNER JOIN empenho_obra e ON e.obrid = o.obrid AND e.proid = po.proid
                	LEFT JOIN par3.documentotermo dt ON dt.proid = p.proid AND dt.dotstatus = 'A'	
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
                	left join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = p.inuid
                	left join par3.modelodocumento mdo ON mdo.mdoid = dt.mdoid
                WHERE o.obrstatus = 'A' $where $whereObra";

        return $sql;
    }
    
    public function listarExcelDetalhadoPlanejamento($where, $wherePlan)
    {
        $sql = "WITH empenho_planejamento AS(	
                	SELECT po.proid,po.inpid, po.ppcano, (sum(ec.epcvalorempenho) - coalesce(sum(epc.vlrcancelado), 0)) AS vlrempenho, sum(epc.vlrcancelado) as vlrcancelado 
                	FROM par3.empenho e
                		INNER JOIN par3.empenhoparcomposicao ec ON ec.empid = e.empid AND ec.epcstatus = 'A'
                		INNER JOIN par3.processoparcomposicao po ON po.ppcid = ec.ppcid AND po.ppcstatus = 'A'
                		LEFT JOIN (SELECT po.inpid, po.ppcano,
                	            		sum(epc.epcvalorempenho) AS vlrcancelado,
                	            		e.empidpai
                					FROM par3.empenho e
                	             		INNER JOIN par3.empenhoparcomposicao epc ON epc.empid = e.empid AND epc.epcstatus = 'A'::bpchar
                	             		INNER JOIN par3.processoparcomposicao po ON po.ppcid = epc.ppcid AND po.ppcstatus = 'A'
                					WHERE e.empstatus = 'A'AND e.empcodigoespecie IN ('03', '13')
                					GROUP BY po.inpid, po.ppcano, e.empidpai) epc ON epc.empidpai = e.empid AND epc.inpid = po.inpid AND epc.ppcano = po.ppcano
                    WHERE e.empcodigoespecie IN ('01')
						AND e.empstatus = 'A'
                    GROUP BY po.proid,po.inpid, po.ppcano
                )
                SELECT DISTINCT p.proid, 
                	'par' as tipo,
                	po.inpid,
                	po.ppcano,
                	p.inuid,
                    (dt.dotnumero || '-'::text) || po.intoid AS dotnumero,
                    p.pronumeroprocesso,
                	case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                    case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
                    obj.intodsc AS tipo_obrjeto,
                    (usu.usunome ||' - '||dt.usucpfinclusao) as usunome,
                    to_char(dt.dotdatainclusao, 'DD/MM/YYYY') as data_criacao,
                    to_char(dt.dotdatafimvigencia, 'DD/MM/YYYY') as data_vigencia,
                    (
                        select
                            to_char(dtv.dtvdatavalidacao, 'DD/MM/YYYY') as dtvdatavalidacao
                        from par3.documentotermo dot2
                        inner join par3.documentotermovalidacao dtv on dtv.dotid = dot2.dotid
                        inner join par3.modelodocumento mdo on mdo.mdoid = dot2.mdoid
                        where dot2.dotnumero = dt.dotnumero
                        and   (mdo.mdoex_oficio = 'N' or null)
                        order by dtvdatavalidacao desc limit 1
                    ) as dtvdatavalidacao,
                    (par.retornasaldoprocesso(p.pronumeroprocesso)) as saldo_processo,
                    vp.vlriniciativa::NUMERIC(20,2) AS valor_processo,
                    dt.dotvalortermo,
                    COALESCE(e.vlrempenho,0) AS vlrempenho,
                    coalesce(pag.vlr_efetivado, 0) AS vlrpago,
                    ta.intadsc,
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
                    (case mdoex_oficio 
                        when 'S' then 'Sim'
                        when 'N' then 'Não'
                        else ' - ' 
                    end) as mdoex_oficio
                FROM par3.processo p
                	INNER JOIN par3.processoparcomposicao po ON po.proid = p.proid AND po.ppcstatus = 'A'
                	--INNER JOIN par3.analise a ON a.anaid = po.anaid AND a.anastatus = 'A'
                	INNER JOIN(	
                		SELECT pp.proid, sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa
                	    FROM par3.iniciativa_planejamento_item_composicao ipi
                	    	INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = ipi.inpid AND ip.inpstatus = 'A'
                			INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
                			INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
                			INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid AND pp.ppcstatus = 'A'
                		WHERE ipi.ipistatus = 'A' 
                		GROUP BY pp.proid	
                	) vp ON vp.proid = p.proid
                	INNER JOIN empenho_planejamento e ON e.inpid = po.inpid AND e.ppcano = po.ppcano and e.proid = po.proid
                	INNER JOIN par3.iniciativa_planejamento pp ON pp.inpid = po.inpid AND pp.inpstatus = 'A'
                	INNER JOIN par3.iniciativa ini on ini.iniid = pp.iniid
                	INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = po.intoid
                	INNER JOIN par3.instrumentounidade iu on iu.inuid = p.inuid and iu.inustatus = 'A'
                	INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = p.intaid
                	LEFT JOIN par3.documentotermo dt ON dt.proid = po.proid AND dt.dotstatus = 'A'
                	LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
                	LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                	LEFT JOIN seguranca.usuario usu ON usu.usucpf = dt.usucpfinclusao
                	LEFT JOIN (	
                		SELECT 
                			inpid, ppcano,
                			sum(foo.vlr_efetivado) AS vlr_efetivado,
                		    sum(foo.vlr_solicitado) AS vlr_solicitado,
                		    sum(foo.vlrpago) AS vlrpago
                		FROM (		
                			SELECT
                				po.inpid, po.ppcano,
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
                			GROUP BY po.inpid, po.ppcano, po.anaid, pag.pagsituacaopagamento			
                		) foo
                		GROUP BY inpid, ppcano		
                	) pag ON pag.inpid = po.inpid AND pag.ppcano = po.ppcano
                	left join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = p.inuid
                	left join par3.modelodocumento mdo ON mdo.mdoid = dt.mdoid
                    left join par3.documentotermovalidacao dtv on dtv.dotid = dt.dotid
                WHERE p.prostatus = 'A' $where $wherePlan";

        return $sql;
    }

    /*public function listarDocObras($where)
    {
        $sql = "SELECT proid, tipo, inuid, dotnumero, pronumeroprocesso, uf, entidade, tipo_obrjeto, data_vigencia, valor_processo, dotvalortermo, vlrempenho, vlrpago, ta.intadsc FROM(
                    SELECT distinct p.proid, 'Obra' as tipo, p.inuid,
                        dt.dotnumero,
    				    p.pronumeroprocesso,
    				    case when iu.muncod is null then est.estuf else mun.estuf end as uf,
    				    '<span class=\"municipio_detalhe\" id=\"'||p.inuid||'\" data-toggle=\"popover\" data-html=\"false\" title=\"\" data-trigger=\"hover\" data-placement=\"top\" data-content=\"Clique para acessar a Execução e acompanhamento da entidade\">'||case when iu.muncod is null then est.estdescricao else mun.mundescricao end||'</span>' as entidade,
    				    'Obra' AS tipo_obrjeto,
    				    to_char(dt.data_vigencia, 'DD/MM/YYYY') as data_vigencia,
    				    vip.vlriniciativa::NUMERIC(20,2) AS valor_processo,
    				    dt.dotvalortermo,
    				    e.vlrempenho as vlrempenho,
    				    coalesce(e.vlr_pg_efetivado,0) as vlrpago,
    				    p.intaid
    				FROM par3.processo p
    				    inner join par3.processoobracomposicao pp on pp.proid = p.proid and pp.pocstatus = 'A'
    				    inner join par3.v_saldo_empenho_do_processo e on e.processo = p.pronumeroprocesso
    				    inner join par3.obra o on o.obrid = pp.obrid and o.obrstatus = 'A'
    				    inner join par3.iniciativa_planejamento ip on ip.inpid = o.inpid
    				    inner join par3.instrumentounidade iu on iu.inuid = ip.inuid and iu.inustatus = 'A'
    				    inner join par3.iniciativa ini on ini.iniid = ip.iniid
    				    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
    				    INNER JOIN(
    				     	SELECT po.proid, sum(o.obrvalor) AS vlriniciativa, sum(COALESCE(o.obrvalor_contrapartida,0)) as vlr_contrapartida FROM par3.obra o
    				     		INNER JOIN par3.processoobracomposicao po ON po.obrid = o.obrid AND po.pocstatus = 'A'
    				     	WHERE o.obrstatus = 'A' GROUP BY po.proid
    				     )vip ON vip.proid = p.proid
    				    LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
    				    LEFT JOIN territorios.estado est on est.estuf = iu.estuf
    				    LEFT JOIN (
    				    	SELECT d.proid, (d.dotnumero || '-'::text) || d.intoid AS dotnumero, d.dotdatafimvigencia AS data_vigencia, sum(d.dotvalortermo) AS dotvalortermo
    				           FROM par3.documentotermo d WHERE d.dotstatus IN ('A', 'B')
    				          GROUP BY d.proid, d.dotnumero, d.intoid, d.dotdatafimvigencia
    				    ) dt ON dt.proid = p.proid
    				WHERE
    				    ip.inpstatus = 'A'
                        and e.vlrempenho > 0
    					$where
                ) as foo
                INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = foo.intaid
                order by uf, entidade";

        return $sql;
    }*/

    public function listarIniciativasProcesso($proid)
    {
        $sql = "SELECT
					/*(case when (select count(t.tecid) from par3.termocomposicao t where t.inpid = ip.inpid) > 0 then
                    	'<input type=\"checkbox\" class=\"check_acao\" name=\"check_acao[]\" id=\"check_acao_'||ip.inpid||'_'||iia.iniano||'_'||p.inuid||'_'||obj.intoid||'\" value=\"'||ip.inpid||'_'||iia.iniano||'_'||p.inuid||'_'||obj.intoid||'\" checked>'
                    else
						'<input type=\"checkbox\" class=\"check_acao\" name=\"check_acao[]\" id=\"check_acao_'||ip.inpid||'_'||iia.iniano||'_'||p.inuid||'_'||obj.intoid||'\" value=\"'||ip.inpid||'_'||iia.iniano||'_'||p.inuid||'_'||obj.intoid||'\">'
                    end) as acao,*/
					p.proid,
					ip.inpid,
					'par' as tipo,
					p.inuid,
					ini.iniid,
					ini.iniid||' - '||de.inddsc as iniciativa,
					iia.iniano,
					(SELECT array_to_string(array(
								          SELECT DISTINCT
								          prg.prgdsc
								          FROM par3.iniciativa_iniciativas_programas iiprg
								          INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
								          WHERE iniid = ini.iniid
								      ),',')
								      ) as programas,
				      obj.intodsc as tipoobjeto,
	                  --(CASE WHEN vip.vlr_processo > sum(emp.vlr_empenho) THEN coalesce((vip.vlr_processo - sum(emp.vlr_empenho)),0) ELSE 0 END) as inecontrapartida,
                      CASE WHEN coalesce(con.inecontrapartida,0) >= 0 THEN coalesce(con.inecontrapartida,0) ELSE
	                  	(CASE WHEN vip.vlr_processo > sum(emp.vlr_empenho) THEN coalesce((vip.vlr_processo - sum(emp.vlr_empenho)),0) ELSE 0 END) END AS inecontrapartida,
	                  sum(emp.vlr_empenho) AS vlrempenho
				FROM par3.processo p
					inner join par3.processoparcomposicao pp on pp.proid = p.proid and pp.ppcstatus = 'A'
					inner join par3.iniciativa_planejamento ip on ip.inpid = pp.inpid
					inner join par3.instrumentounidade iu on iu.inuid = ip.inuid and iu.inustatus = 'A'
				    inner join par3.iniciativa ini on ini.iniid = ip.iniid
				    INNER JOIN(
				     	SELECT proid, sum(vlriniciativa) AS vlr_processo, inpid,  sum(coalesce(inecontrapartida,0)) as vlr_contrapartida FROM par3.v_valor_iniciativa_por_processo GROUP BY proid, inpid
				     )vip ON vip.proid = p.proid AND vip.inpid = pp.inpid
				    INNER JOIN(
				    	WITH empenho_cancelado AS (
							SELECT pp.inpid, e.empid, e.empidpai, pp.ppcano, e.empnumeroprocesso, pp.proid, sum(ec.epcvalorempenho) AS vlr_cancelado
							FROM par3.empenho e
								INNER JOIN par3.empenhoparcomposicao ec ON ec.empid = e.empid AND ec.epcstatus = 'A'
								INNER JOIN par3.processoparcomposicao pp ON pp.ppcid = ec.ppcid
							WHERE e.empcodigoespecie IN ('03', '13', '04')
								AND e.empstatus = 'A'
							GROUP BY e.empid, e.empidpai, e.empnumeroprocesso, pp.proid, pp.inpid, pp.ppcano
						)
						SELECT pp.inpid, e.empid, e.empidpai, e.empnumeroprocesso, pp.ppcano, pp.proid, (sum(ec.epcvalorempenho) - coalesce(emc.vlr_cancelado,0)) AS vlr_empenho, emc.vlr_cancelado
						FROM par3.empenho e
							INNER JOIN par3.empenhoparcomposicao ec ON ec.empid = e.empid AND ec.epcstatus = 'A'
							INNER JOIN par3.processoparcomposicao pp ON pp.ppcid = ec.ppcid
							LEFT JOIN empenho_cancelado emc ON emc.empidpai = e.empid AND emc.inpid = pp.inpid
						WHERE e.empcodigoespecie = '01'
							AND e.empstatus = 'A'
						GROUP BY e.empid, e.empidpai, e.empnumeroprocesso, pp.proid, pp.inpid, emc.vlr_cancelado, pp.ppcano
				    ) emp ON emp.inpid = ip.inpid AND emp.ppcano = pp.ppcano
				    inner join par3.iniciativa_iniciativas_anos iia on iia.iniid = ip.iniid and iia.iniano = pp.ppcano
				    inner join par3.iniciativa_descricao de on de.indid = ini.indid
				    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
					INNER JOIN par3.iniciativa_tipos_atendimento inta ON inta.intaid = ini.intaid
					INNER JOIN par3.ensino_etapa eta ON eta.etaid = ip.etaid
				    LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
					LEFT JOIN territorios.estado est on est.estuf = iu.estuf
					LEFT JOIN ( SELECT ie.inpid,
					            e.emeano,
					            pp_1.proid,
					            sum(ie.inecontrapartida)::numeric(20,2) AS inecontrapartida
					           FROM par3.iniciativa_emenda ie
					             JOIN emenda.emenda e ON e.emeid = ie.emeid
					             JOIN par3.analise a1 ON a1.edeid = ie.edeid AND a1.inpid = ie.inpid AND a1.anaano = e.emeano AND a1.anastatus = 'A'::bpchar
					             JOIN par3.processoparcomposicao pp_1 ON pp_1.anaid = a1.anaid AND pp_1.inpid = a1.inpid AND pp_1.ppcstatus::text = 'A'::text
					          WHERE ie.inestatus = 'A'::bpchar
					          GROUP BY ie.inpid, e.emeano, pp_1.proid) con ON con.inpid = ip.inpid AND con.emeano = pp.ppcano AND con.proid = pp.proid
				WHERE
					ip.inpstatus = 'A'
					and p.proid = $proid
				GROUP BY p.proid, ip.inpid, p.inuid, ini.iniid, de.inddsc, iia.iniano, obj.intodsc, vip.vlr_processo, con.inecontrapartida";
        return $sql;
    }

    public function listarIniciativasProcessoObras($proid)
    {
        $sql = "SELECT distinct
				    /*(case when (select count(t.tecid) from par3.termocomposicao t where t.inpid = ip.inpid) > 0 then
                    	'<input type=\"checkbox\" class=\"check_acao\" name=\"check_acao[]\" id=\"check_acao_'||o.obrid||'_'||o.obrano||'_'||p.inuid||'_'||obj.intoid||'\" value=\"'||o.obrid||'_'||o.obrano||'_'||p.inuid||'_'||obj.intoid||'\" checked>'
                    else
						'<input type=\"checkbox\" class=\"check_acao\" name=\"check_acao[]\" id=\"check_acao_'||o.obrid||'_'||o.obrano||'_'||p.inuid||'_'||obj.intoid||'\" value=\"'||o.obrid||'_'||o.obrano||'_'||p.inuid||'_'||obj.intoid||'\">'
                    end) as acao,*/
					p.proid,
					'par' as tipo,
					p.inuid,
					o.obrid,
					ini.iniid,
					ini.iniid||' - '||de.inddsc as iniciativa,
					o.obrid||' - '||o.obrdsc as obra,
					o.obrano,
				    obj.intodsc as tipoobjeto,
				    eta.etadsc   as etapa,
				    (SELECT array_to_string(array(
												          SELECT DISTINCT
												          prg.prgdsc
												          FROM par3.iniciativa_iniciativas_programas iiprg
												          INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
												          WHERE iniid = ini.iniid
												      ),',')
												      ) as programas,
                    coalesce(o.obrvalor_contrapartida,0) AS inecontrapartida,
				    e.vlrempenho
				FROM par3.processo p
				    inner join par3.processoobracomposicao pp on pp.proid = p.proid and pp.pocstatus = 'A'
				    inner join par3.obra o on o.obrid = pp.obrid and o.obrstatus = 'A'
				    inner join par3.iniciativa_planejamento ip on ip.inpid = o.inpid
				    inner join par3.iniciativa ini on ini.iniid = ip.iniid
				    inner join par3.iniciativa_descricao de on de.indid = ini.indid
				    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
				    INNER JOIN par3.ensino_etapa eta ON eta.etaid = ip.etaid
                    LEFT JOIN(
				    	select sum(saldo) AS vlrempenho, obrid from par3.v_saldo_empenho_por_obra GROUP BY obrid
				    ) e ON e.obrid = o.obrid
				WHERE
				    ip.inpstatus = 'A'
					and p.proid = $proid";
        return $sql;
    }

    public function getDadosProcesso($proid)
    {
        $sql = "select distinct iu.itrid, iu.inuid, p.pronumeroprocesso,
												case when iu.estuf is null then iu.muncod else iu.estuf end as estuf
											from	par3.processo p
											    inner join par3.instrumentounidade iu on iu.inuid = p.inuid
											where p.proid = $proid and p.prostatus = 'A'";
        return $this->pegaLinha($sql);
    }

    public function getDadosCronograma($proid)
    {
        $sql = "SELECT  distinct
						to_char(now(), 'DD/MM/YYYY') AS cronogramainicial,
						to_char( (now() + INTERVAL '365 DAY'), 'DD/MM/YYYY') AS cronogramafinal
				FROM par3.processo p
					INNER JOIN par3.empenho e ON e.empnumeroprocesso =  p.pronumeroprocesso and empstatus <> 'I'
				WHERE p.prostatus = 'A' and p.proid = $proid";

        return $this->pegaLinha($sql);
    }

    public function getDadosPrefeitoPrefeitura($inuid, $prefeitura, $prefeito)
    {
        $sql = "SELECT ent.entid, ent2.entid, ent2.entnome, ent2.entcnpj, ed.endlogradouro, ed.endcep, ed.endnumero, ed.endbairro,
					case when iu.itrid = 2 then mun.mundescricao else est.estdescricao end as mundescricao,
					case when iu.itrid = 2 then mun.estuf else est.estuf end as estuf, mun.muncod,
				    ent.entcpf as cpfprefeito, ent.entnome as prefeito, ent.entemail
				FROM  par3.instrumentounidade_entidade ent
					INNER JOIN par3.instrumentounidade_entidade ent2 ON ent.inuid = ent2.inuid AND ent2.tenid = $prefeitura
                	inner join par3.instrumentounidade iu on iu.inuid = ent2.inuid
                	left join par3.endereco ed on ed.endid = ent2.endid
					left join territorios.municipio mun on mun.muncod = iu.muncod
					left join territorios.estado est on est.estuf = iu.estuf
				where
					ent.tenid = $prefeito AND
					ent.entstatus='A' AND
					ent2.entstatus='A' AND
					ent2.entnome is not null and
					ent.inuid = " . $inuid;

        $arrDados = $this->pegaLinha($sql);
        return $arrDados;
    }
    
    public function getDadosEntidadeExecutora($proid, $inuid)
    {
        $sql = "SELECT ue.ungcnpj, ungrazao_social, ed.endlogradouro, ed.endcep, ed.endnumero, ed.endbairro,
                	case when iu.itrid = 2 then mun.mundescricao else est.estdescricao end as mundescricao,
                	case when iu.itrid = 2 then mun.estuf else est.estuf end as estuf, mun.muncod,
                	us.usucpf, us.usunome
                FROM par3.unidade_executora ue
                	INNER JOIN par3.processo p ON p.inuid = ue.inuid AND p.procnpj = ue.ungcnpj AND p.prostatus = 'A'
                	INNER JOIN par3.usuarioresponsabilidade ur ON ur.ungid = ue.ungid AND ur.rpustatus = 'A'
                	INNER JOIN seguranca.usuario us ON us.usucpf = ur.usucpf
                	LEFT JOIN par3.instrumentounidade_entidade ie ON ie.entcnpj = ue.ungcnpj AND ie.entstatus = 'A'
                	LEFT JOIN par3.instrumentounidade iu on iu.inuid = ie.inuid AND iu.inustatus = 'A'
                	LEFT JOIN par3.endereco ed on ed.endid = ie.endid
                	LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
                	LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                WHERE ungstatus = 'A' AND ue.inuid = $inuid AND p.proid = $proid";

        $arrDados = $this->pegaLinha($sql);
        return $arrDados;
    }

    public function getDadosPrefeito($muncod)
    {
        $sql = "SELECT DISTINCT
					u.usunome as prefeito, u.usucpf as cpf, u.usuemail
				FROM
					par3.usuarioresponsabilidade ur
				    inner join seguranca.usuario u on u.usucpf = ur.usucpf
				    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
				WHERE
					ur.pflcod = " . PAR3_PERFIL_PREFEITO . "
	                and us.sisid = 231
				    and ur.muncod = '{$muncod}'
				    and ur.rpustatus = 'A'
				    and us.suscod = 'A'";
        return $this->pegaLinha($sql);
    }

    public function getDadosEmpenho($proid)
    {
        $sql = "select DISTINCT
                    vi.iniciativa,
				    vi.empenho_original as empnumero,
				    (vi.saldo) as valor
				from par3.v_dados_processo p
                	inner join par3.v_saldo_empenho_por_iniciativa vi on vi.proid = p.proid
				WHERE p.proid = $proid
				and vi.saldo > 0";

        return $this->carregar($sql);
    }

    public function getDadosObrasEmpenho($proid)
    {
        $sql = "select DISTINCT
                    vi.obras,
				    vi.empenho_original as empnumero,
				    (vi.saldo) as valor
				from par3.v_dados_processo p
                	inner join par3.v_saldo_empenho_por_obra vi on vi.proid = p.proid
				WHERE p.proid = $proid
				and vi.saldo > 0";

        return $this->carregar($sql);
    }

    public function getDadosItemComposicao($arrPost = array())
    {

        $sql = "select *, (valorreferencia * ipiquantidadeaprovada) as total from(
				    SELECT distinct ipc.ipiid, i.iniid, ind.inddsc, eta.etadsc, iicg.iigid, iicg.itcid, iicg.igrid, iia.iniano, igrnome as itcdsc, '-' as unidsc, ipc.ipiaprovado, ipc.ipiquantidadeaprovada, ipc.ipivalorreferencia as valorreferencia
				    FROM par3.iniciativa_planejamento ip
				    inner join par3.iniciativa i USING (iniid)
				    inner join par3.iniciativa_descricao ind on ind.indid = i.indid
				    inner join par3.iniciativa_tipos_objeto obj  ON obj.intoid = i.intoid
				    inner join par3.iniciativa_iniciativas_anos iia USING (iniid)
				    inner join par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
				    inner join par3.itenscomposicao_grupos icg USING (igrid)
				    inner join par3.itenscomposicao_grupos_itens icgi USING (igrid)
				    inner join par3.ensino_etapa eta ON eta.etaid = ip.etaid
				    inner join par3.processoparcomposicao pp on pp.inpid = ip.inpid and pp.ppcano = iia.iniano AND pp.ppcstatus = 'A'
				    inner join par3.analise  a on a.inpid = ip.inpid AND a.anaid = pp.anaid AND a.anaano = pp.ppcano AND a.anastatus = 'A'
				    inner join par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = pp.ppcano AND ipc.anaid = a.anaid
				    WHERE iia.iianostatus = 'A'
				    	and pp.proid = {$arrPost['proid']}
                        AND ipc.ipiquantidadeaprovada > 0
				    GROUP BY ipc.ipiid, i.iniid, ind.inddsc, iicg.iigid, eta.etadsc, iia.iniano, igrnome, ipc.ipivalorreferencia, ipc.ipiaprovado, ipc.ipiquantidadeaprovada,ip.inpid, ip.inuid, obj.intoid
				    union all
				    SELECT distinct ipc.ipiid, i.iniid, ind.inddsc, eta.etadsc, iicg.iigid, iicg.itcid, iicg.iigid, iia.iniano, itcdsc, unidsc, ipc.ipiaprovado,  ipc.ipiquantidadeaprovada, ipc.ipivalorreferencia as valorreferencia
				    FROM par3.iniciativa_planejamento ip
				    inner join par3.iniciativa i USING (iniid)
				    inner join par3.iniciativa_descricao ind on ind.indid = i.indid
				    inner join par3.iniciativa_tipos_objeto obj  ON obj.intoid = i.intoid
				    inner join par3.iniciativa_iniciativas_anos iia USING (iniid)
				    inner join par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
				    inner join par3.ensino_etapa eta ON eta.etaid = ip.etaid
				    inner join par3.processoparcomposicao pp on pp.inpid = ip.inpid and pp.ppcano = iia.iniano AND pp.ppcstatus = 'A'
				    inner join par3.analise  a on a.inpid = ip.inpid AND a.anaid = pp.anaid AND a.anaano = pp.ppcano AND a.anastatus = 'A'
				    inner join par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = pp.ppcano AND ipc.anaid = a.anaid
				    inner join par3.itenscomposicao ic USING (itcid)
				    inner join par3.unidade_medida um USING (uniid)
				    WHERE  iia.iianostatus = 'A'
				    	and pp.proid = {$arrPost['proid']}
				        AND ipc.ipiquantidadeaprovada > 0
				) as foo
				order by itcdsc, iniid, inddsc";

        $arDados = $this->carregar($sql);
        $arDados = $arDados ? $arDados : array();


        return $arDados;
    }

    public function getDadosItemComposicaoGrupoIniciativa($arrPost = array())
    {

        $sql = "select iniid, inddsc, etadsc, sum(ipiquantidadeaprovada) as ipiquantidadeaprovada, sum(valorreferencia) as valorreferencia, sum((valorreferencia * ipiquantidadeaprovada)) as total from(
				    SELECT distinct i.iniid, ind.inddsc, eta.etadsc, iicg.iigid, iicg.itcid, iicg.igrid, iia.iniano, igrnome as itcdsc, '-' as unidsc, ipc.ipiaprovado, ipc.ipiquantidadeaprovada,
				                ipc.ipivalorreferencia as valorreferencia
				    FROM par3.iniciativa_planejamento ip
				    inner join par3.iniciativa i USING (iniid)
                    inner join par3.iniciativa_descricao ind on ind.indid = i.indid
				    inner join par3.iniciativa_tipos_objeto obj  ON obj.intoid = i.intoid
				    inner join par3.iniciativa_iniciativas_anos iia USING (iniid)
				    inner join par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
				    inner join par3.itenscomposicao_grupos icg USING (igrid)
				    inner join par3.itenscomposicao_grupos_itens icgi USING (igrid)
				    inner join par3.ensino_etapa eta ON eta.etaid = ip.etaid
                    inner join par3.analise a on a.inpid = ip.inpid AND a.anastatus = 'A'
				    inner join par3.processoparcomposicao pp on pp.inpid = ip.inpid and pp.ppcano = a.anaano AND pp.ppcstatus = 'A'
                    inner join par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = pp.ppcano AND ipc.anaid = a.anaid
				    WHERE iia.iianostatus = 'A'
				    	and pp.proid = {$arrPost['proid']}
                        AND ipc.ipiquantidadeaprovada > 0
				    GROUP BY i.iniid, ind.inddsc, iicg.iigid, eta.etadsc, iia.iniano, igrnome, ipc.ipivalorreferencia, ipc.ipiaprovado, ipc.ipiquantidadeaprovada,ip.inpid, ip.inuid, obj.intoid
				    union all
				    SELECT distinct i.iniid, ind.inddsc, eta.etadsc, iicg.iigid, iicg.itcid, iicg.iigid, iia.iniano, itcdsc, unidsc, ipc.ipiaprovado,  ipc.ipiquantidadeaprovada,
				        ipc.ipivalorreferencia as valorreferencia
				    FROM par3.iniciativa_planejamento ip
				    inner join par3.iniciativa i USING (iniid)
                    inner join par3.iniciativa_descricao ind on ind.indid = i.indid
				    inner join par3.iniciativa_tipos_objeto obj  ON obj.intoid = i.intoid
				    inner join par3.iniciativa_iniciativas_anos iia USING (iniid)
				    inner join par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
				    inner join par3.ensino_etapa eta ON eta.etaid = ip.etaid
				    inner join par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = iia.iniano
				    inner join par3.analise a on a.inpid = ip.inpid AND a.anastatus = 'A'
				    inner join par3.processoparcomposicao pp on pp.inpid = ip.inpid and pp.ppcano = a.anaano AND pp.ppcstatus = 'A'
				    inner join par3.itenscomposicao ic USING (itcid)
				    inner join par3.unidade_medida um USING (uniid)
				    WHERE  iia.iianostatus = 'A'
				    	and pp.proid = {$arrPost['proid']}
                        AND ipc.ipiquantidadeaprovada > 0
				) as foo
				group by iniid, inddsc, etadsc";

        $arDados = $this->carregar($sql);
        $arDados = $arDados ? $arDados : array();


        return $arDados;
    }

    public function getDadosObrasComposicao($arrPost = array())
    {

        $sql = "SELECT distinct
				    o.obrid,
					o.obrid||' - '||o.obrdsc as obra,
                    (o.obrvalor - coalesce(o.obrvalor_contrapartida,0)) as vlrobra,
					o.obrano,
                    o.obrlogradouro,
                    o.obrcep,
                    o.obrbairro,
                    mun.mundescricao,
                    o.estuf,
                    o.muncod,
                    eta.etadsc,
                    ot.otpdsc,
				    obj.intodsc as tipoobjeto,
				    (select sum(saldo) from par3.v_saldo_empenho_por_obra e where e.processo = p.pronumeroprocesso and e.obrid = o.obrid) as vlrempenho
				FROM par3.processo p
				    inner join par3.processoobracomposicao pp on pp.proid = p.proid and pp.pocstatus = 'A'
				    inner join par3.obra o on o.obrid = pp.obrid and o.obrstatus = 'A'
				    inner join par3.instrumentounidade as inu on inu.inuid = o.inuid
				    inner join par3.iniciativa_planejamento ip on ip.inpid = o.inpid
				    inner join par3.iniciativa ini on ini.iniid = ip.iniid
				    inner join par3.iniciativa_descricao de on de.indid = ini.indid
				    inner join par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
                    left join par3.ensino_etapa eta ON eta.etaid = ip.etaid
                    left join par3.obra_tipo ot on ot.otpid = o.otpid
					left join territorios.municipio mun on mun.muncod = inu.muncod
					left join territorios.estado est on est.estuf = inu.estuf
				WHERE
				    ip.inpstatus = 'A'
				    and p.proid = '{$arrPost['proid']}'";

        $arDados = $this->carregar($sql);
        $arDados = $arDados ? $arDados : array();

        return $arDados;
    }

    public function carregarTermoPorProcesso($proid)
    {
        $sql = "WITH reformulacao AS (
                	SELECT refid, rd.dotid, rd.proid, rd.tirid, rt.tirdescricao, es.esdid, es.esddsc
                	FROM par3.reformulacao_documento rd
                		INNER JOIN workflow.documento d ON d.docid = rd.docid
                		INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                		INNER JOIN par3.reformulacao_tipos rt ON rt.tirid = rd.tirid
                	WHERE refstatus = 'A' AND refidpai IS NOT NULL
                		AND refid in (SELECT max(refid) FROM par3.reformulacao_documento WHERE refstatus = 'A' AND dotid = rd.dotid GROUP BY tirid)
                )
                SELECT
					dt.dotid,
					dt.proid,
                    dt.intoid,
                    p.inuid,
					dt.dotnumero||'-'||dt.intoid as dotnumero,
					md.mdonome||' - '||ito.intodsc as termo,
                    (dt.dotdatafimvigencia::date - now()::date) as dias,
				    to_char(dt.dotdatafimvigencia, 'DD/MM/YYYY') as dotdatafimvigencia,
				    dt.dotvalortermo,
				    case when md.mdoex_oficio = 'S' then '-'
				    else 
					    CASE WHEN md.mdoqtdvalidacao <= (SELECT COUNT(dv.dtvid) FROM par3.documentotermovalidacao dv WHERE dv.dotid = dt.dotid AND dv.dtvstatus = 'A') THEN 'Documento Validado' ELSE 'Documento não Validado' end
					end as situacao,
				    us.usunome as inclusao,
				    usu2.usunome as alterou,
				    dt.dotstatus,
                    dt.dotrejeitado,
                    dt.dotdata_de_oficio,
                    edt.esdid as esdid_doc,
                    md.mdoex_oficio,
                    CASE WHEN dt.dotidpai IS NOT NULL THEN rt.tirdescricao ELSE
                    	(case when dt.dotdataexclusao is not null then 'Excluído' else '-' end)
                    end as status,
                    CASE WHEN vp.vlrpago > 0 THEN 'S' ELSE 'N' END AS vlrpago,
                    CASE WHEN dt.dotidpai IS NULL THEN '-' ELSE 
                    	CASE WHEN re.esddsc IS NOT NULL THEN coalesce(re.esddsc, '-') ELSE edt.esddsc END
                    END AS reformulacao   
				FROM par3.documentotermo dt
                    inner join par3.processo p on p.proid = dt.proid and p.prostatus = 'A'
					inner join par3.modelodocumento md on md.mdoid = dt.mdoid and md.mdostatus = 'A'
					inner join par3.iniciativa_tipos_objeto ito on ito.intoid = dt.intoid
					inner join seguranca.usuario us ON us.usucpf = dt.usucpfinclusao
					LEFT JOIN seguranca.usuario usu2 ON usu2.usucpf = dt.usucpfalteracao
                    LEFT JOIN reformulacao re ON re.refid = dt.refid
					LEFT JOIN par3.reformulacao_tipos rt ON rt.tirid = dt.tirid
                    LEFT JOIN(
						SELECT proid, vlrempenho, vlrpago  FROM par3.v_saldo_empenho_do_processo
					) vp ON vp.proid = dt.proid
                    left join(					
						select t.dotid, es.esdid, es.esddsc from par3.documentotermo t 
							inner join workflow.documento d on d.docid = t.docid
							inner join workflow.estadodocumento es on es.esdid = d.esdid
						--where t.dotstatus in ('A', 'B')					
					) edt on edt.dotid = dt.dotid
				WHERE dt.proid = $proid
				    AND dt.dotid NOT IN (SELECT dt1.dotid FROM par3.documentotermo dt1
											INNER JOIN par3.reformulacao_documento rd ON rd.refid = dt1.refid
											INNER JOIN workflow.documento d ON d.docid = rd.docid
										WHERE dt1.proid = dt.proid AND d.esdid in (".PAR3_REFORMULACAO_ESDID_AGUARDANDO_GERACAO_TERMO.", ".PAR3_REFORMULACAO_ESDID_AGUARDANDO_VALIDACAO_GESTOR.") AND rd.refstatus = 'A' AND dt1.dotstatus in ('A', 'B') )
				order by dt.dotid desc, (dt.dotstatus = 'A') desc, 11 asc";
        return $sql;
    }

    public function sqlVisualizaDadosSigarp($dotid, $boSigarp = false, $boItensAdesao = false)
    {

        if ($boSigarp) $filtro = " and itc.itcidsigarp is null ";
        if ($boItensAdesao) $filtro = " and ids.iasid is null";

        $sql = "SELECT procnpj, pronumeroprocesso, dotnumero, dotdatafimvigencia,
					inddsc, itcid, itcdsc, seq_sigarp, sum(ipiquantidadeaprovada) AS ipiquantidadeaprovada, ipivalorreferencia, ipiano, adesao
                FROM(
                    select distinct p.procnpj, p.pronumeroprocesso, d.dotnumero, d.dotdatafimvigencia,
    					i.iniid||' - '||ind.inddsc as inddsc, itc.itcid, itc.itcid||' - '||itc.itcdsc as itcdsc, itc.itcidsigarp as seq_sigarp,
                        it.ipiquantidadeaprovada, it.ipivalorreferencia, it.ipiano, case when ids.iasid is null then 'Não' else ids.nu_seq_solicitacao_adesao::varchar end as adesao, it.anaid
    				from par3.documentotermo d
    					inner join par3.processo p on p.proid = d.proid
    					INNER JOIN par3.processoparcomposicao pp ON pp.proid = p.proid AND pp.ppcstatus = 'A'
    					inner join par3.termocomposicao t on t.dotid = d.dotid and t.tecstatus = 'A'
    					inner join par3.v_analise_planejamento_item_composicao it on it.inpid = t.inpid and it.ipiano = t.tecano and it.ipistatus = 'A' AND it.anaid = pp.anaid
    					inner join par3.iniciativa_planejamento ip on ip.inpid = t.inpid and ip.inpstatus = 'A'
    					inner join par3.iniciativa i on i.iniid = ip.iniid
    					inner join par3.iniciativa_descricao ind on ind.indid = i.indid
    					inner join par3.analise a on a.inpid = ip.inpid and a.anaano = t.tecano AND a.anastatus = 'A'
    				    left join (
    				    	select distinct g.iniid, g.iigid, ic.itcid, ic.itcdsc, ic.itcidsigarp
    				        from par3.iniciativa_itenscomposicao_grupo g
    				    		inner join par3.itenscomposicao ic on ic.itcid = g.itcid
    				        union all
    				        select distinct g.iniid, g.iigid, g.itcid, icg.igrnome as itcdsc, icg.igridsigarp as itcidsigarp
    				        from par3.iniciativa_itenscomposicao_grupo g
    				            inner join par3.itenscomposicao_grupos icg on icg.igrid = g.igrid
    				    ) itc on itc.iniid = i.iniid and it.iigid = itc.iigid
                        left join par3.logwssigarp l on l.dotid = d.dotid
                        						and to_char(lgwdata, 'YYYY-MM-DD HH24:MI') = (select max(to_char(lgwdata, 'YYYY-MM-DD HH24:MI')) from par3.logwssigarp where dotid = l.dotid)
                        						AND l.itcid = itc.itcid
                        left join par3.itemadesaosigarp ids on ids.item = itc.itcidsigarp and ids.dotid = d.dotid and ids.proid = p.proid and ids.adestatus = 'A'
    				where t.dotid = $dotid and ipiaprovado in ('S', 'CS') AND it.ipiquantidadeaprovada > 0 $filtro
    			) AS foo
				GROUP BY procnpj, pronumeroprocesso, dotnumero, dotdatafimvigencia, inddsc, itcid, itcdsc, seq_sigarp, ipivalorreferencia, ipiano, adesao
    		    order by 7";
        return $sql;
    }

    public function sqlVisualizaLogSigarp($dotid)
    {
        $sql = "select l.lgwservico, u.usunome, l.lgwerro, l.lgwmsgretorno, to_char(l.lgwdata, 'DD/MM/YYYY HH24:MI:SS') as lgwdata
				from par3.logwssigarp l
					inner join seguranca.usuario u on u.usucpf = l.usucpf
				where l.dotid = $dotid
					and l.lgwid = (select max(lgwid) from par3.logwssigarp where dotid = l.dotid )
                ";
        return $sql;
    }

    /**
     * Função montarSQLPAR
     * - monta a SQL da lista de documentos PAR e Obras
     *
     * @return string. escreve o sql .
     *
     */
    public function montarSQLPAR3($arrPost)
    {
        $sqlPendente = '';
        if ($arrPost['pendente']) {
            $sqlPendente = "AND  dot.dotid IN ( SELECT coalesce(par3.retornar_termo_pendente_validacao(dot.dotid)::integer,0) )";
        } else {
            $sqlPendente = "AND  dot.dotid NOT IN ( SELECT coalesce(par3.retornar_termo_pendente_validacao(dot.dotid)::integer,0) )";
        }

        $sql = "SELECT
                      dot.dotid,
                      pro.proid,
                      pro.pronumeroprocesso,
                      dot.dotnumero||'-'||dot.intoid as dotnumero,
                      '<span class=\"dotid\" value=' || dot.dotid || '></span>' || concat(mdo.mdonome,' - ',ito.intodsc) as termo,
                      case when mdo.mdoex_oficio = 'S' then
				    	'Documento Validado'
				    else 
					    CASE WHEN mdo.mdoqtdvalidacao <= (SELECT COUNT(dv.dtvid) FROM par3.documentotermovalidacao dv WHERE dv.dotid = dot.dotid AND dv.dtvstatus = 'A')
		                	THEN 'Documento Validado'
						ELSE 'Documento não Validado' end
					end as situacao,
                      CASE WHEN ( (date_trunc('MONTH', dotdatafimvigencia::date) + INTERVAL '1 MONTH - 1 day')::date - current_date) < 60
                	          AND ( (date_trunc('MONTH', dotdatafimvigencia::date) + INTERVAL '1 MONTH - 1 day')::date - current_date) > 0
                	        THEN
                		      '<span style=\"color:red\">' || to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' (' || (dotdatafimvigencia::date - CURRENT_DATE) || ' dias)'  || '</span>'
                		 ELSE to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' (' || (dotdatafimvigencia::date - CURRENT_DATE) || ' dias)'
                	  END as vigencia,
                      coalesce(dot.dotvalortermo,0) as dotvalortermo,
                      coalesce(vlrempenho,0) as valor_empenhado,
                      coalesce(vlr_pg_efetivado, 0)  as pagamento_efetivado,
                      ( SELECT sum(p_1.dfisaldoconta + p_1.dfisaldofundo + p_1.dfisaldopoupanca + p_1.dfisaldordbcdb) AS saldo
                        FROM painel.dadosfinanceirosconvenios p_1
                          JOIN par3.processo p_2 ON p_2.pronumeroprocesso::text = p_1.dfiprocesso::text
                        WHERE p_1.dfiprocesso::text = pro.pronumeroprocesso::text AND to_char(p_1.dfidatasaldo, 'YYYY-MM-DD'::text) = (( SELECT to_char(max(df1.dfidatasaldo), 'YYYY-MM-DD'::text) AS to_char
                                                                                                                                         FROM painel.dadosfinanceirosconvenios df1
                                                                                                                                           JOIN par3.processo p_3 ON p_3.pronumeroprocesso::text = df1.dfiprocesso::text
                                                                                                                                         WHERE df1.dfiprocesso::text = pro.pronumeroprocesso::text))) AS saldo_bancario,
                      dot.intoid,
                      v.obrid,
                      totalobrid,
                      dfc.dfibanco,
                      dfc.dfiagencia,
                      dfc.dficonta
                    FROM
                      par3.documentotermo dot
                      INNER JOIN par3.modelodocumento mdo ON dot.mdoid = mdo.mdoid
                      INNER JOIN par3.processo pro ON pro.proid = dot.proid and pro.prostatus = 'A'
                      INNER JOIN par3.iniciativa_tipos_objeto ito ON ito.intoid = dot.intoid
                      LEFT JOIN par3.processoobracomposicao poc ON poc.proid = dot.proid and poc.pocstatus = 'A'
                      LEFT JOIN obras2.obras o ON o.obrid_par3 = poc.obrid
                      LEFT JOIN LATERAL (
                            SELECT   dfibanco,
                                     dficonta,
                                     dfiagencia
                            FROM     painel.dadosfinanceirosconvenios dfc
                            WHERE    dfc.dfiprocesso = pro.pronumeroprocesso
                            ORDER BY dfc.dfidatasaldo ASC limit 1 ) dfc ON true
                      LEFT JOIN LATERAL (SELECT obrid, obrid_par3, COUNT(obrid) as totalobrid FROM obras2.obras obr WHERE obr.obrid_par3 = poc.obrid GROUP BY obrid, obr.obrid_par3) v on true
                      LEFT JOIN LATERAL (SELECT sum(vlrempenho) as vlrempenho, sum(vlr_pg_efetivado) as vlr_pg_efetivado
                                         FROM par3.v_saldo_empenho_do_processo vsaldo
                                         WHERE vsaldo.proid = dot.proid ) v2 on true
                      left join workflow.documento d on d.docid = dot.docid --and d.esdid = 2447
                    WHERE
                      /*dotidpai IS NULL
                      AND*/ dotstatus = 'A'
                      {$sqlPendente}
                      AND pro.inuid = {$arrPost['inuid']}
                      and (case when dot.dotdata_de_oficio is not null then d.esdid = 2447 else 1=1 end)
                        group by
  	v.obrid,
  	dot.dotid,
  	pro.proid,
  	mdo.mdonome,
  	ito.intodsc,
  	mdo.mdoqtdvalidacao,
  	v2.vlrempenho,
  	v2.vlr_pg_efetivado,
  	v.totalobrid,
    dfc.dfibanco,
    dfc.dfiagencia,
    dfc.dficonta, mdo.mdoex_oficio
                    order by dotdatafimvigencia";
//        ver($sql);
        return $sql;

    }

    /**
     * Verifica se existem documentos pendentes para esta unidade
     * @return mixed
     */
    public function montarSqlDocumentosPendentes($inuid){
        /*$sql = "SELECT dotid FROM par3.documentotermo dot
                INNER JOIN par3.processo pro ON pro.proid = dot.proid
                LEFT JOIN par3.processoobracomposicao poc ON poc.proid = dot.proid
                WHERE  pro.inuid = {$inuid}
                  AND dotstatus = 'A'
                  --AND dotidpai IS NULL
                  AND NOT EXISTS (SELECT dtvid FROM par3.documentotermovalidacao WHERE dotid = dot.dotid AND dtvstatus = 'A')
                ";*/

        $sql = "SELECT dotid FROM(
                    SELECT dotid, (SELECT count(ue.ungcnpj)
                    		FROM par3.unidade_executora ue
                    			INNER JOIN par3.processo po ON po.inuid = ue.inuid AND po.procnpj = ue.ungcnpj AND po.prostatus = 'A'
                    		WHERE ue.inuid = pro.inuid AND po.proid = pro.proid AND ue.ungcnpj = pro.procnpj) AS qtd_validar_executora,
                    		case when dot.dotdata_de_oficio is not null then 1 else 
                    		(select count(d.dtvid) from par3.documentotermovalidacao d 
                    			where d.dotid = dot.dotid and d.dtvstatus = 'A' AND dtvcpf NOT IN (SELECT usucpf FROM par3.usuarioresponsabilidade WHERE rpustatus = 'A' AND ungid IS NOT NULL AND pflcod = 1862)
                    		) end as qtd_validado,
                    		(SELECT count(ur.rpuid) FROM par3.documentotermovalidacao dt
            					INNER JOIN par3.usuarioresponsabilidade ur ON ur.usucpf = dt.dtvcpf AND ur.rpustatus = 'A'
            					INNER JOIN par3.unidade_executora ue ON ue.ungid = ur.ungid AND ue.ungstatus = 'A'
            				WHERE dt.dotid = dot.dotid AND dtvstatus = 'A') AS qtd_validado_executora
                    FROM par3.documentotermo dot
                                    INNER JOIN par3.processo pro ON pro.proid = dot.proid
                                    LEFT JOIN par3.processoobracomposicao poc ON poc.proid = dot.proid
                                    WHERE  pro.inuid = $inuid
                                      AND dotstatus = 'A'
                    ) AS foo
                    	WHERE (qtd_validado = 0 or (qtd_validar_executora > 0 AND qtd_validado_executora = 0))";
        return $sql;
    }


    public function montarSQLHistoricoPAR3($arrPost)
    {
        $dotid = $arrPost['dados'][0];
        $proid = $arrPost['dados'][1];
        
        $sql = "SELECT	DISTINCT
    				dot.dotid as id,
                    ROW_NUMBER () OVER (ORDER BY dot.dotid) as row,
    			    dot.dotnumero||'-'||dot.intoid as dotnumero,
    				mdo.mdonome||' - '||ito.intodsc as termo,
    				CASE WHEN ( (date_trunc('MONTH', dot.dotdatafimvigencia::date) + INTERVAL '1 MONTH - 1 day')::date - current_date) < 60
                	          AND ( (date_trunc('MONTH', dot.dotdatafimvigencia::date) + INTERVAL '1 MONTH - 1 day')::date - current_date) > 0
                	        THEN
                		      '<span style=\"color:red\">' || to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' (' || (dot.dotdatafimvigencia::date - CURRENT_DATE) || ' dias)'  || '</span>'
                		 ELSE to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' (' || (dot.dotdatafimvigencia::date - CURRENT_DATE) || ' dias)'
                	  END as vigencia,
                	  dot.dotvalortermo,
                    CASE WHEN dot.dotidpai IS NOT NULL THEN rt.tirdescricao ELSE 'Original' END AS tipos,
                    CASE WHEN dot.dotidpai IS NULL THEN '-' ELSE 
                    	CASE WHEN rf.situacao IS NOT NULL THEN coalesce(rf.situacao, '-') ELSE esd.esddsc END
                    END AS situacao
				FROM par3.documentotermo dot
			    INNER JOIN par3.modelodocumento  mdo ON dot.mdoid = mdo.mdoid
			    INNER JOIN par3.processo         pro ON dot.proid = pro.proid
			    INNER JOIN par3.iniciativa_tipos_objeto ito on ito.intoid = dot.intoid
                LEFT JOIN par3.reformulacao_tipos rt ON rt.tirid = dot.tirid
                LEFT JOIN workflow.documento dc on dc.docid = dot.docid
				LEFT JOIN workflow.estadodocumento esd on esd.esdid = dc.esdid
			    LEFT JOIN(			    
			    	SELECT rd.refid, rd.dotid, rd.proid, rt.tirdescricao, es.esddsc AS situacao 
			    	FROM par3.reformulacao_documento rd 
		    			INNER JOIN par3.reformulacao_tipos rt ON rt.tirid = rd.tirid
						INNER JOIN workflow.documento d on d.docid = rd.docid
						INNER JOIN workflow.estadodocumento es on es.esdid = d.esdid
					WHERE rd.refstatus = 'A' AND refidpai IS NOT NULL			    
			    ) rf ON rf.refid = dot.refid
			    WHERE dot.proid = $proid AND dot.dotstatus NOT IN ('B')
			    ORDER BY dot.dotid DESC";

        return $sql;
    }

    /**
     * Visualizar o HTML do termo
     *
     * @param $arrPost
     * @return text/html
     */
    public function formVisualizarDocumento($arrPost)
    {

        ob_clean();

        $sql = "SELECT dtanomearquivo
                 FROM par3.documentotermoarquivo
                 WHERE dotid = {$arrPost['dotid']}
                  AND dtastatus = 'A'";

        $nomeArquivo = $this->pegaUm($sql);

        $caminho = APPRAIZ . 'arquivos/par3/documentoTermo/' . $nomeArquivo;

        if (is_file($caminho)) {
            echo html_entity_decode(file_get_contents($caminho));
        } else {
            echo "Desculpe, nenhum arquivo foi encontrado.";
        }

    }

    public function listarDocumentosProcesso($arrParams)
    {
        $sql = "Select * from par3.documentotermo where proid = '{$arrParams['proid']}'";
        return $sql;
    }

    public function detalharDocumentoTermoGrafico($arrPost = null,$relatorioCompleto = false)
    {
        $descricao = '';
        switch ($arrPost['descricao']) {
            case 'Documento Validado':                $descricao = 'validado';break;
            case 'Bloqueado':                         $descricao = 'bloqueado';break;
            case 'Documento não Validado':            $descricao = 'nao_validado';break;
            case 'Documento Rejeitado Contrapartida': $descricao = 'rejeitado';break;
            default: $descricao;break;
        }

        if($relatorioCompleto == false) {
            $limitVal  = $arrPost['limit']?" LIMIT   {$arrPost['limit']} ":'';
            $offsetVal = $arrPost['offset']?" OFFSET {$arrPost['offset']} ":'';
        }

        $where  = $arrPost['descricao'] ? " WHERE {$descricao} = 1 ": '';
        $where .= $arrPost['inudescricao'] ? " AND inudescricao ilike '%{$arrPost['inudescricao']}%' ":"";
        $where .= $arrPost['itrid'] && $arrPost['itrid'] != 0 ?($arrPost['itrid'] == 1 ? "AND itrid in(1,3)" :"AND itrid = {$arrPost['itrid']}"): "";
        $where .= $arrPost['inuid'] ? "AND inuid = {$arrPost['inuid']} ":'';
        $where .= $arrPost['ano']? " AND date_part('year', dotdatainclusao)::numeric =  {$arrPost['ano']}":'';
        if($arrPost['muncod'] && count($arrPost['muncod']) != 0 ) {
            $arrMuncod = tratarArrayParaMultiSelect($arrPost['muncod']);
            if(count($arrMuncod) != 0) {
                $where .= " and muncod::integer in (".implode(',',$arrMuncod).")";
            }
        }

        if($arrPost['estuf'] && count($arrPost['estuf']) != 0 ) {
            $arrEstuf = implode(',',array_map(function($estuf){return (string)"'".$estuf."'";},$arrPost['estuf']));
            $where .= $arrPost['estuf'] ? " and estuf in ({$arrEstuf})":'';
        }

        if($arrPost['intaidtermo']) {
            if (is_array($arrPost['intaidtermo'])) {
                $arInta = implode(',',$arrPost['intaidtermo']);
                $where .= " AND intaid in ({$arInta}) ";
            }else{
                switch ($arrPost['intaidtermo'] ){
                    case 1:
                        $where .= " and intaid = 1 ";
                        break;
                    case 2:
                        $where .= " and p.intaid = 2 ";
                        break;
                    default:
                        $where .='';
                        break;
                }
            }

        }

        $sql = "
        SELECT
        pronumeroprocesso,
	    (
		case when validado = 1 then 'Documento Validado' else
			case when bloqueado = 1 then 'Bloqueado ' else
				case when nao_validado = 1 then 'Documento não Validado' else
					case when rejeitado = 1 then 'Documento Rejeitado Contrapartida' else ''
		end end end end) as situacao,
        documento,
        entidade,
        count(*) OVER() AS full_count,
        inudescricao,
        itrid,
        (case when intaid = 1 then 'PAR' else 'Emendas ' end) as atendimento,
        estuf,
        muncod,
        intaid
        FROM(
            SELECT distinct
            p.pronumeroprocesso,
            dt.dotnumero||'-'||dt.intoid as documento,
            CASE WHEN (SELECT count(dtvid) FROM par3.documentotermovalidacao WHERE dtvstatus = 'A' AND dotid = dt.dotid) > 0 AND dotstatus = 'A' THEN count(dt.dotid) ELSE 0 END AS validado,
            CASE WHEN (SELECT count(dtvid) FROM par3.documentotermovalidacao WHERE dtvstatus = 'A' AND dotid = dt.dotid) < 1 AND dotstatus = 'A' THEN count(dt.dotid) ELSE 0 END AS nao_validado,
            CASE WHEN dt.dotrejeitado = 'S' AND (SELECT count(dotid) FROM par3.documentotermo WHERE dotstatus IN ('A', 'B') AND proid = dt.proid) < 1 THEN count(dt.dotid) ELSE 0 END AS rejeitado,
            CASE WHEN dt.dotstatus = 'B' THEN count(dt.dotid) ELSE 0 END AS bloqueado,
            inu.inudescricao||' - '||inu.estuf as entidade,
            inu.inudescricao,
            inu.itrid,
            inu.muncod,
            inu.estuf,
            intaid,
            inu.inuid,
            dt.dotdatainclusao
            FROM par3.processo p
            INNER JOIN par3.documentotermo dt ON dt.proid = p.proid
            inner join par3.instrumentounidade inu on inu.inuid = p.inuid
            GROUP BY p.pronumeroprocesso, p.proid, dt.dotid, dt.dotrejeitado, dt.dotstatus,entidade,itrid,inudescricao,inu.estuf,inu.muncod,inu.inuid,dt.dotdatainclusao
        ) AS termo
        {$where}
        ";
        $sql = $sql.$offsetVal.$limitVal;
        $res = $this->carregar($sql);
        if (count($res) == 0) {
            $sql = $sql.$limitVal;
            $res = $this->carregar($sql);
        }
        $retorno['resultset'] = $res;
        $retorno['sql']       = $sql;
        return $retorno;
    }

    public function sqlGrafico($arrParams = null)
    {
        $where  = ' where 1=1 ';
        $where .= $arrParams['inuid']?" AND p.inuid = {$arrParams['inuid']}":'';
        $where .= $arrParams['ano']? " AND date_part('year', dt.dotdatainclusao)::numeric =  {$arrParams['ano']}":'';
        if($arrParams) {
            if (is_array($arrParams['intaidtermo'])) {
                $arInta = implode(',',$arrParams['intaidtermo']);
                $where .= " AND p.intaid in ({$arInta}) ";
            }
        }
        $sql = "
        SELECT
        sum(validado) AS validado,
        sum(valor_validado) as valor_validado,
        
        sum(nao_validado) AS nao_validado,
        sum(valor_nao_validado) AS valor_nao_validado,
        
        sum(rejeitado) AS rejeitado,
        sum(valor_rejeitado) AS valor_rejeitado,
        
        sum(bloqueado) AS bloqueado,
        sum(valor_bloqueado) AS valor_bloqueado

        FROM (
            SELECT DISTINCT p.pronumeroprocesso, p.proid,
                CASE WHEN (SELECT count(dtvid) FROM par3.documentotermovalidacao WHERE dtvstatus = 'A' AND dotid = dt.dotid) > 0 AND dotstatus = 'A' THEN count(dt.dotid) ELSE 0 END AS validado,
                CASE WHEN (SELECT count(dtvid) FROM par3.documentotermovalidacao WHERE dtvstatus = 'A' AND dotid = dt.dotid) > 0 AND dotstatus = 'A' THEN sum(dt.dotvalortermo) ELSE 0 END AS valor_validado,

                CASE WHEN (SELECT count(dtvid) FROM par3.documentotermovalidacao WHERE dtvstatus = 'A' AND dotid = dt.dotid) < 1 AND dotstatus = 'A' THEN count(dt.dotid) ELSE 0 END AS nao_validado,
                CASE WHEN (SELECT count(dtvid) FROM par3.documentotermovalidacao WHERE dtvstatus = 'A' AND dotid = dt.dotid) < 1 AND dotstatus = 'A' THEN sum(dt.dotvalortermo) ELSE 0 END AS valor_nao_validado,
                
                CASE WHEN dt.dotrejeitado = 'S' AND (SELECT count(dotid) FROM par3.documentotermo WHERE dotstatus IN ('A', 'B') AND proid = dt.proid) < 1 THEN count(dt.dotid) ELSE 0 END AS rejeitado,
                CASE WHEN dt.dotrejeitado = 'S' AND (SELECT count(dotid) FROM par3.documentotermo WHERE dotstatus IN ('A', 'B') AND proid = dt.proid) < 1 THEN sum(dt.dotvalortermo) ELSE 0 END AS valor_rejeitado,
                
                CASE WHEN dt.dotstatus = 'B' THEN count(dt.dotid) ELSE 0 END AS bloqueado,
                CASE WHEN dt.dotstatus = 'B' THEN sum(dt.dotvalortermo) ELSE 0 END AS valor_bloqueado

                
            FROM par3.processo p
                INNER JOIN par3.documentotermo dt ON dt.proid = p.proid
            {$where}
            GROUP BY p.pronumeroprocesso, p.proid, dt.dotid, dt.dotrejeitado, dt.dotstatus
        ) AS termo
        ";
        return $sql;
    }

    public function sqlPainelPeriodoVigencia($detalhar = false,$tipoVigencia = null,$arrDados = null)
    {
        $sqlWhereTipoVigencia = '';
        $where = $arrDados['inuid'] ? " AND pro.inuid = {$arrDados['inuid']}":'';
        $where .= $arrDados['ano'] ?   " AND date_part('year',dot.dotdatainclusao)::numeric = {$arrDados['ano']}":'';
        $inudesc   = '';
        $offsetVal = $arrDados['offset']?" OFFSET {$arrDados['offset']} ":'';
        $orderBy   = $arrDados['order'] !== null ? " ORDER BY ".($arrDados['order'] + 1)." {$arrDados['dir']} ":" ORDER BY 1 desc ";
        $sqlGroup  = 'group by descricao,id';
        $sqlWhereTipoVigencia .= $tipoVigencia? " WHERE dot.id_data_vigencia = {$tipoVigencia} ":'';
        $limitVal  = $arrDados['limit']?" LIMIT   {$arrDados['limit']} ":'';
        $innerJoin = '';
        $sqlCampos =<<<SELECTCAMPOS
           (case dot.id_data_vigencia
            when 1 then '0 e 30 dias'
            when 2 then '31 e 45 dias'
            when 3 then '46 e 60 dias'
            when 4 then '90 e 61 dias'
            when 5 then 'Mais de 90 dias'
          end) as descricao,
          COUNT(*) as valor,
          dot.id_data_vigencia as id
SELECTCAMPOS;

        if($detalhar) {
            $inudesc    = $arrDados['inudescricao'] !== ''?"AND inudescricao ilike '%{$arrDados['inudescricao']}%'":'';
            $innerJoin  = 'inner join par3.instrumentounidade inu on inu.inuid = dot.inuid';
            $sqlGroup   = 'group by dot.id_data_vigencia,dot.inuid,dot.pronumeroprocesso,dot.vigencia,dot.valor_empenho,dot.valor_pago,inudescricao,proid,documento';
            $sqlCampos  = 'inudescricao,';
            $sqlCampos .= 'pronumeroprocesso,documento,';
            $sqlCampos .= 'valor_empenho,';
            $sqlCampos .= <<<SELECTUSUEMPENHO
            (
                SELECT 
                    emp.usunome||' ('||to_char(emp.empdata, 'dd/MM/yyyy HH:MM:SS')||')'
                from
                (
                    select
                    usu.usunome,
                    e.empdata
                    FROM par3.empenho e
                        INNER JOIN par3.processo p ON p.pronumeroprocesso = e.empnumeroprocesso
                        inner join seguranca.usuario usu on usu.usucpf = e.usucpf
                    WHERE empstatus = 'A' 
                        AND empcodigoespecie = '01'
                        AND e.empdata = (
                        SELECT max(empdata) FROM par3.empenho
                            WHERE empstatus = 'A' 
                            AND empcodigoespecie = '01'
                            AND empnumeroprocesso = e.empnumeroprocesso
                        )
                        and p.proid = dot.proid
                        order by e.empdata desc
                        limit 1
                ) as emp
            ) AS usuempenho,
SELECTUSUEMPENHO;
            $sqlCampos .= 'valor_pago,';
            $sqlCampos .= <<<SELECTUSUPAGAMENTO
            (
                SELECT 
                    pag.usunome||' ('||to_char(pag.pagdatapagamento, 'dd/MM/yyyy HH:MM:SS')||')'
                    from
                    (
                    select 
                        usu.usunome,
                        pg.pagdatapagamento
                        FROM par3.empenho e
                        INNER JOIN par3.processo p ON p.pronumeroprocesso = e.empnumeroprocesso
                        INNER JOIN par3.pagamento pg ON pg.empid = e.empid AND pg.pagstatus = 'A'
                        inner join seguranca.usuario usu on usu.usucpf = pg.usucpf
                        WHERE empstatus = 'A' 
                        AND empcodigoespecie = '01'
                        AND pg.pagdatapagamento = (
                        SELECT max(p1.pagdatapagamento)
                        FROM par3.pagamento p1
                        INNER JOIN par3.empenho e1 ON e1.empid = p1.empid
                            WHERE p1.pagstatus = 'A'
                            AND e1.empnumeroprocesso = e.empnumeroprocesso
                    )
                        and p.proid = dot.proid
                        order by pg.pagdatapagamento desc
                        limit 1
                    ) as pag
            ) AS usupagamento,
SELECTUSUPAGAMENTO;

            $sqlCampos .= 'vigencia,count(*) OVER() AS full_count';
        }
        $sql =<<<SQL
        select 
        {$sqlCampos}
        from (
            select
            pro.inuid,
            pro.proid,
            pro.pronumeroprocesso,
            dot.dotnumero||'-'||dot.intoid as documento,
             (CASE 
              WHEN (dotdatafimvigencia::date - CURRENT_DATE) <= 30  AND (dotdatafimvigencia::date - CURRENT_DATE) >= 0  then 1
              WHEN (dotdatafimvigencia::date - CURRENT_DATE) <= 45  AND (dotdatafimvigencia::date - CURRENT_DATE) >= 31 then 2
              WHEN (dotdatafimvigencia::date - CURRENT_DATE) <= 60  AND (dotdatafimvigencia::date - CURRENT_DATE) >  46 then 3 	      
              WHEN (dotdatafimvigencia::date - CURRENT_DATE) <= 90  AND (dotdatafimvigencia::date - CURRENT_DATE) >  61 then 4
              WHEN (dotdatafimvigencia::date - CURRENT_DATE) > 90   then 5
              end) as id_data_vigencia,
              COALESCE (sum(v_empenho.vlrempenho),0) as valor_empenho,
              COALESCE (sum(v_empenho.vlrpago),0) as valor_pago,
              to_char(dot.dotdatafimvigencia, 'DD/MM/YYYY') || ' (' || (dotdatafimvigencia::date - CURRENT_DATE) || ' dias)' as vigencia
            from par3.documentotermo dot
              INNER JOIN par3.processo pro ON pro.proid = dot.proid and pro.prostatus = 'A'
              inner join (
                    select 
                        proid,
                        vlrempenho,
                        processo,
                        vlrpago
                    FROM par3.v_painel_empenho_composicao_iniciativa WHERE empsituacao ILIKE '%efetivado%'
                UNION ALL        
                    select
                        proid,
                        vlrempenho,
                        processo,
                        vlrpago
                    FROM par3.v_painel_empenho_composicao_obra WHERE empsituacao ILIKE '%efetivado%'
              ) as v_empenho on v_empenho.processo = pro.pronumeroprocesso
            where dot.dotstatus = 'A'
            AND EXISTS (SELECT dtvid FROM par3.documentotermovalidacao WHERE dotid = dot.dotid AND dtvstatus = 'A')
            {$where}
            group by 
            id_data_vigencia,pro.inuid,pro.pronumeroprocesso,dot.dotdatafimvigencia,pro.proid,documento
        ) as dot
        {$innerJoin}
        {$sqlWhereTipoVigencia}
        {$inudesc}
        {$sqlGroup}
        
SQL;
        if ($detalhar) {
            $res = $this->carregar($sql.$orderBy.$offsetVal.$limitVal);
            if (count($res) == 0) {
                $res = $this->carregar($sql.$orderBy.$limitVal);
            }
            return $res;
        }
        return $sql;
    }
}
<?php
class Par3_Model_RemanejamentoPagamento extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.pagamento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array();
                                  
    function __construct($id = null)
    {
        parent::__construct($id);
    }
    
    public function recuperar($arrPost)
    {
        return $this->model;
    }
    
    public function carregarDadosParaPagamentoPAR($empid)
    {
        
        $sql = "SELECT distinct
                    vp.iniid as id,
				    vp.iniciativa as descricaoitem,
				    vp.epcid as codigo,
				    vp.vlriniciativa,
				    dt.dotvalortermo as valortotal,
				    vp.saldo,
				    vdp.desembolso,
				    vdp.deferimento,
				    vdp.saldo as saldo_desembolso,
				    coalesce(vp.vlrpago,0) as vlrpagamento,
				    CASE WHEN vp.vlriniciativa > 0 THEN ((coalesce(vp.saldo,0) / vp.vlriniciativa) * 100)::numeric(20,2) ELSE 0 END AS percent_empenho,
				    CASE WHEN vp.vlriniciativa > 0 THEN
					    CASE WHEN vp.vlriniciativa < coalesce(vp.saldo,0) THEN ((coalesce(vp.vlrpago,0) / vp.vlriniciativa) * 100)::numeric(20,2)
					    	ELSE CASE WHEN vp.saldo > 0 THEN ((coalesce(vp.vlrpago,0) / vp.saldo) * 100)::numeric(20,2) ELSE 0 END END
				    ELSE 0 END AS percent_pago,
					pp.ppcano as ano,
					coalesce(CASE WHEN vp.vlriniciativa > 0 THEN ((coalesce(vt.vlr_total_pagamento,0) / vp.vlriniciativa) * 100)::numeric(20,2) ELSE 0 END,0) AS percent_total_pago,
                    coalesce(vt.vlr_total_pagamento,0) as vlr_total_pagamento
				FROM par3.processo p
				    inner join par3.processoparcomposicao pp on pp.proid = p.proid
				    inner join par3.v_saldo_empenho_composicao_iniciativa vp on vp.ppcid = pp.ppcid
				    INNER JOIN par3.documentotermo dt ON dt.proid = p.proid	AND dt.dotstatus = 'A'
				    LEFT JOIN(
				    	SELECT inpid, sum(vlr_total_pagamento)::numeric(20,2) AS vlr_total_pagamento FROM(
                            SELECT DISTINCT pc.inpid, (po.ppcvalorpagamento) AS vlr_total_pagamento
                            FROM par3.pagamentoparcomposicao po
                            	INNER JOIN par3.pagamento pg ON pg.pagid = po.pagid AND pg.pagstatus = 'A'
                            	INNER JOIN par3.empenhoparcomposicao ec ON ec.epcid = po.epcid AND ec.epcstatus = 'A'
                            	INNER JOIN par3.processoparcomposicao pc ON pc.ppcid = ec.ppcid
                            WHERE po.ppcstatus = 'A'
                            	AND pg.pagsituacaopagamento not ilike '%CANCELADO%'
                            	and pg.pagsituacaopagamento not ilike '%vala%'
                            	and pg.pagsituacaopagamento not ilike '%devolvido%'
                            	and pg.pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'
                        ) AS foo
                        GROUP BY inpid
				    ) vt ON vt.inpid = vp.inpid
				    LEFT JOIN par3.v_desembolso_por_processo vdp ON vdp.proid = p.proid
				    
				WHERE
				    p.prostatus = 'A'
				    and vp.empid = $empid";
        return $sql;
    }
    
    public function carregarDadosParaPagamentoOBRA($empid)
    {
        
        $sql = "SELECT
                    emp.obrid as id,
                	emp.obrid||' - '||emp.obrdsc as descricaoitem,
                    emp.eocid as codigo,
                    emp.valorobra as vlriniciativa,
                    emp.valorobra as valortotal,
                    percent_obra,
                    emp.saldo,
                    coalesce(emp.vlrpago,0) as vlrpagamento,
                    CASE WHEN emp.valorobra > 0 THEN ((coalesce(emp.saldo,0) / emp.valorobra) * 100)::numeric(20,2) ELSE 0 END AS percent_empenho,
                    vdp.desembolso,
                    vdp.deferimento,
                    vdp.saldo as saldo_desembolso,
                    CASE WHEN emp.valorobra > 0 THEN
					    CASE WHEN emp.valorobra < coalesce(emp.saldo,0) THEN ((coalesce(emp.vlrpago,0) / emp.valorobra) * 100)::numeric(20,2)
					    	ELSE CASE WHEN emp.saldo > 0 THEN ((coalesce(emp.vlrpago,0) / emp.saldo) * 100)::numeric(20,2) ELSE 0 END END
				    ELSE 0 END AS percent_pago,
                	coalesce(CASE WHEN emp.valorobra > 0 THEN ((coalesce(vt.vlr_total_pagamento,0) / emp.valorobra) * 100)::numeric(20,2) ELSE 0 END,0) AS percent_total_pago,
                    coalesce(vt.vlr_total_pagamento,0) as vlr_total_pagamento
                FROM par3.processo p
                	inner join par3.processoobracomposicao pp on pp.proid = p.proid and pp.pocstatus = 'A'
                    inner join par3.v_saldo_empenho_composicao_obra emp on emp.pocid = pp.pocid
                    LEFT JOIN(
                    	SELECT pc.obrid, sum(po.pmcvalorpagamento)::numeric(20,2) AS vlr_total_pagamento
                		FROM par3.pagamentoobracomposicao po
                            INNER JOIN par3.pagamento pg ON pg.pagid = po.pagid AND pg.pagstatus = 'A'
                			INNER JOIN par3.empenhoobracomposicao ec ON ec.eocid = po.eocid AND ec.eocstatus = 'A'
                			INNER JOIN par3.processoobracomposicao pc ON pc.pocid = ec.pocid
                		WHERE po.pmcstatus = 'A'
                            AND pg.pagsituacaopagamento not ilike '%CANCELADO%'
                        	and pg.pagsituacaopagamento not ilike '%vala%'
                        	and pg.pagsituacaopagamento not ilike '%devolvido%'
                        	and pg.pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'
                		GROUP BY pc.obrid
                    ) vt ON vt.obrid = emp.obrid
                    LEFT JOIN par3.v_desembolso_por_processo vdp ON vdp.proid = p.proid
                WHERE
                	p.prostatus = 'A'
				    and emp.empid = $empid";
        return $sql;
    }
}

<?php
class Par3_Model_EmendasParlamentares extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    public $stNomeTabela = "";
    
    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    public $arChavePrimaria = array('emeid');
    
    /**
     * Atributos
     * @var array
     * @access protected
     */
    public $arAtributos     = array();
    
    function __construct()
    {
        parent::__construct();
    }
    
    public function recuperar($arrPost)
    {
        return $this->model;
    }
    
    public function montaSQLLista($arrPost = array())
    {
        
        
        $where =  " and v.emeano = '2018'" ;
        if ($arrPost['emecod']) {
            $where .= " and v.emecod = '{$arrPost['emecod']}'";
        }
        
        if ($arrPost['estuf']) {
            $where .= " and iu.estuf = '{$arrPost['estuf']}'";
        }
        if ($arrPost['esdid']) {
            $where .= " and ie.esdid = '{$arrPost['esdid']}'";
        }
        if ($arrPost['itrid']) {
            $where .= " and iu.itrid = {$arrPost['itrid']}";
        }
        
        $arrPost['muncod'] = (array) ($arrPost['muncod'] ? $arrPost['muncod'] : null);
        if ($arrPost['muncod'][0]) {
            $where .= " and iu.muncod IN ('". implode("', '", $arrPost['muncod']) ."')";
            $where .= " and iu.itrid = 2 ";
        }
        
        if (is_array($arrPost['autid']) && !empty($arrPost['autid'][0])) {
            $where .= " and v.autid in (".implode(',', $arrPost['autid']).")";
        }
        
        if ($arrPost['autid_f']) {
            $where .= " and v.autid = {$arrPost['autid_f']}";
        }
        if ($arrPost['iniid']) {
            $where .= " and ie.iniid = '{$arrPost['iniid']}'";
        }
        
        $sql = "SELECT DISTINCT
                	v.emeid,
                    v.edeid,
                	v.emecod,
                	v.emeano,
                    a.autcod||' - '||a.autnome AS autor,
                	v.enbnome,
                    CASE WHEN v.estuf IS NOT NULL THEN v.estuf ELSE iu.estuf END AS uf,
                	ie.esddsc AS situacao,
                	v.emevalor,
                	v.edevalor,
                	coalesce(sex.vlr_empenho,0) as valor_empenho,
                	coalesce(sex.vlr_pagamento,0) as valor_pago
                FROM emenda.v_emendadetalheentidade v
                    LEFT JOIN(
                    	SELECT DISTINCT ie.edeid, ed.esdid, ed.esddsc, p.iniid
                    	FROM
                    		par3.iniciativa_emenda ie
                    	inner join par3.iniciativa_planejamento p on p.inpid = ie.inpid and p.inpstatus='A'
						INNER JOIN workflow.documento d on d.docid = ie.docid AND d.tpdid = 317
						INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid AND ed.tpdid = 317
						WHERE
							ie.inestatus = 'A'
                    ) ie ON ie.edeid = v.edeid
                    inner JOIN(
                		SELECT DISTINCT  entcnpj, inuid FROM par3.instrumentounidade_entidade WHERE entstatus = 'A'
                	) iue on iue.entcnpj = v.enbcnpj
                    INNER JOIN par3.instrumentounidade iu ON iu.inuid = iue.inuid AND iu.inustatus = 'A'
                    INNER JOIN emenda.autor a ON a.autid = v.autid
                	LEFT JOIN(
	                	SELECT DISTINCT ie.edeid, sum(vc.saldo) AS vlr_empenho, sum(vc.vlr_efetivado) AS vlr_pagamento
                    	FROM par3.iniciativa_emenda ie
                        	INNER JOIN par3.analise a ON a.inpid = ie.inpid AND a.edeid = ie.edeid AND a.anastatus = 'A'
                        	INNER JOIN par3.v_empenho_composicao_iniciativa vc ON vc.inpid = a.inpid AND vc.anaid = a.anaid
                        WHERE ie.inestatus = 'A'
                        GROUP BY ie.edeid
                        UNION ALL
                        SELECT DISTINCT ie.edeid, sum(vc.saldo) AS vlr_empenho, sum(vc.vlr_efetivado) AS vlr_pagamento
                    	FROM par3.iniciativa_emenda ie
                        	INNER JOIN par3.iniciativa_emenda_obra eo ON eo.ineid = ie.ineid AND eo.ieostatus = 'A'
                        	INNER JOIN par3.v_empenho_composicao_obra vc ON vc.obrid = eo.obrid
                        WHERE ie.inestatus = 'A'
                        GROUP BY ie.edeid
                	) sex on sex.edeid = v.edeid
                WHERE v.edestatus = 'A'
                    and v.etoid = 1
                	$where
                ORDER BY v.emecod ";
        return $sql;
    }
    
    public function montaSQLListaVisaoMunicipio($arrPost = array())
    {
        
        $where =  ( empty($arrPost['emeano']) ? " and v.emeano = '".(date('Y'))."'" : " and v.emeano = '{$arrPost['emeano']}'" );
        
        if ($arrPost['inuid']) {
            $itrid = $this->pegaUm("SELECT itrid FROM par3.instrumentounidade WHERE inuid = {$arrPost['inuid']}");
            $where .= " and iu.itrid = $itrid ";
        }
        
        if ($arrPost['emecod']) {
            $where .= " and v.emecod = '{$arrPost['emecod']}'";
        }
        if ($arrPost['estuf']) {
            $where .= " and iu.estuf = '{$arrPost['estuf']}'";
        }
        if ($arrPost['esdid']) {
            $where .= " and ie.esdid = '{$arrPost['esdid']}'";
        }
        if ($arrPost['itrid']) {
            $where .= " and iu.itrid = {$arrPost['itrid']}";
        }
            
        
        if ($arrPost['muncod']) {
            $arrPost['muncod'] = (array) $arrPost['muncod'];
            $where .= " and iu.muncod IN ('". implode("', '", $arrPost['muncod']) ."')";
        }
        
        if ($arrPost['autid_f']) {
            $where .= " and v.autid = {$arrPost['autid_f']}";
        }
        if (is_array($arrPost['autid']) && !empty($arrPost['autid'][0])) {
            $where .= " and v.autid in (".implode(',', $arrPost['autid']).")";
        }
        if ($arrPost['iniid']) {
            $where .= " and ie.iniid = '{$arrPost['iniid']}'";
        }
        
        $sql = "SELECT DISTINCT
        			v.emeid,
                    v.edeid,
                	v.emecod,
                	v.emeano,
                    a.autcod||' - '||a.autnome AS autor,
                	v.enbnome,
                	CASE WHEN v.estuf IS NOT NULL THEN v.estuf ELSE iu.estuf END AS uf,
                	ie.esddsc AS situacao,
                	v.emevalor,
                	v.edevalor,
                	coalesce(sex.vlr_empenho,0) as valor_empenho,
                	coalesce(sex.vlr_pagamento,0) as valor_pago
                FROM emenda.v_emendadetalheentidade v
                    LEFT JOIN(
                    	SELECT
                    		DISTINCT ie.edeid, ed.esdid, ed.esddsc, p.iniid
                    	FROM
                    		par3.iniciativa_emenda ie
                    		inner join par3.iniciativa_planejamento p on p.inpid = ie.inpid and p.inpstatus='A'
							INNER JOIN workflow.documento d on d.docid = ie.docid AND d.tpdid = 317
							INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid AND ed.tpdid = 317
						WHERE ie.inestatus = 'A'
                    ) ie ON ie.edeid = v.edeid
                	INNER JOIN par3.instrumentounidade_entidade iue ON iue.entcnpj = v.enbcnpj AND iue.entstatus = 'A'
                    INNER JOIN par3.instrumentounidade iu ON iu.inuid = iue.inuid AND iu.inustatus = 'A'
                    INNER JOIN emenda.autor a ON a.autid = v.autid
                	LEFT JOIN(
	                	SELECT DISTINCT ie.edeid, sum(vc.saldo) AS vlr_empenho, sum(vc.vlr_efetivado) AS vlr_pagamento
                    	FROM par3.iniciativa_emenda ie
                        	INNER JOIN par3.analise a ON a.inpid = ie.inpid AND a.edeid = ie.edeid AND a.anastatus = 'A'
                        	INNER JOIN par3.v_empenho_composicao_iniciativa vc ON vc.inpid = a.inpid AND vc.anaid = a.anaid
                        WHERE ie.inestatus = 'A'
                        GROUP BY ie.edeid
                        UNION ALL
                        SELECT DISTINCT ie.edeid, sum(vc.saldo) AS vlr_empenho, sum(vc.vlr_efetivado) AS vlr_pagamento
                    	FROM par3.iniciativa_emenda ie
                        	INNER JOIN par3.iniciativa_emenda_obra eo ON eo.ineid = ie.ineid AND eo.ieostatus = 'A'
                        	INNER JOIN par3.v_empenho_composicao_obra vc ON vc.obrid = eo.obrid
                        WHERE ie.inestatus = 'A'
                        GROUP BY ie.edeid
                	) sex on sex.edeid = v.edeid
                WHERE
                	v.edestatus = 'A'
                    and v.etoid = 1
                	$where
                ORDER BY v.emecod";
        
        return $sql;
    }
    
    public function sqlDetalheEmenda($emeid)
    {
        $sql = "SELECT distinct
                	ed.gndcod||' - '||g.gnddsc as gnd,
                	ed.mapcod||' - '||m.mapdsc as modalidade,
                	ed.foncod||' - '||f.fondsc as fonte,
                	ed.emdvalor
                FROM emenda.emenda e
                	inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
                	inner join public.gnd g on g.gndcod = ed.gndcod
                	inner join public.modalidadeaplicacao m on m.mapcod = ed.mapcod
                	inner join public.fonterecurso f on f.foncod = ed.foncod
                WHERE e.emeid = $emeid
                	and ed.emdstatus = 'A'
                ORDER BY 1";
        
        return $sql;
    }
    
    public function sqlBeneficiarioEmenda($emeid)
    {
        $sql = "SELECT distinct
                	ve.edeid,
                    ve.emeid,
                    iue.inuid,
                	formata_cpf_cnpj(ve.enbcnpj) as cnpj,
                	ve.enbnome,
                	ve.gndcod,
                	ve.mapcod,
                	ve.edevalor,
                    ed.esddsc as status
                FROM emenda.v_emendadetalheentidade ve
                	inner join public.gnd g on g.gndcod = ve.gndcod
                	inner join public.modalidadeaplicacao m on m.mapcod = ve.mapcod
                	inner join public.fonterecurso f on f.foncod = ve.foncod
                    left join par3.instrumentounidade_entidade iue on iue.entcnpj = ve.enbcnpj and iue.entstatus = 'A'
                    left join par3.iniciativa_emenda e
                    	inner join workflow.documento d on d.docid = e.docid
                    	inner join workflow.estadodocumento ed on ed.esdid = d.esdid
                    on e.edeid = ve.edeid AND e.inestatus = 'A'
                WHERE ve.emeid = $emeid
                	and ve.edestatus = 'A'
                ORDER BY 1 ";
        
        return $sql;
    }
    
    public function sqlBeneficiarioSelecionado($edeid, $inpid = '', $inuid = '')
    {
        
        if ($edeid) {
            $filtro = " ve.edeid = $edeid ";
        } elseif ($inpid) {
            $coluna = " iu.inuid, ie.inpid, a.anaid, au.autcod||' - '||au.autnome AS parlamentar, ";
            $join = " INNER JOIN par3.iniciativa_emenda ie ON ie.edeid = ve.edeid AND inestatus = 'A'
                      INNER JOIN par3.analise a ON a.edeid = ve.edeid AND a.inpid = ie.inpid AND a.anastatus = 'A'
                      INNER JOIN emenda.autor au ON au.autid = ve.autid ";
            $filtro = " ie.inpid = $inpid AND ve.emeano = '{$_REQUEST['iniano']}'  ";
            
            $filtroInpid = " and ie.inpid = $inpid ";
        }
        
        if (!empty($inuid)) {
            $filtro .= " and iu.inuid = $inuid ";
        }
        
        $sql = "SELECT distinct
                	ve.emecod as id,
                    ve.emecod,
                    $coluna
                	ve.emeano,
                	ve.gndcod,
                	ve.mapcod,
                	formata_cpf_cnpj(ve.enbcnpj) as cnpj,
                	'<input type=\"hidden\" name=valor_designado value='||ve.edevalor||'>'||ve.enbnome as enbnome,
                    CASE WHEN ve.estuf IS NOT NULL THEN ve.estuf ELSE iu.estuf END AS uf,
                	array_to_string(array(SELECT DISTINCT ed.esddsc
					                        FROM par3.iniciativa_emenda e
					                        	INNER JOIN workflow.documento d ON d.docid = e.docid
					                        	INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
					                        WHERE e.inestatus = 'A' and e.edeid = ve.edeid), ', <br>') as status,
                	ve.emevalor,
                	ve.edevalor,
                    coalesce(iei.ieiquantidade_total,0) as valor_total_indicado_municipio,
                    coalesce(iei.total_aprovado,0) as total_aprovado
                FROM emenda.v_emendadetalheentidade ve
                	inner join public.gnd g on g.gndcod = ve.gndcod
                	inner join public.modalidadeaplicacao m on m.mapcod = ve.mapcod
                	inner join public.fonterecurso f on f.foncod = ve.foncod
                    INNER JOIN par3.instrumentounidade_entidade iue ON iue.entcnpj = ve.enbcnpj AND iue.entstatus = 'A'
                    INNER JOIN par3.instrumentounidade iu ON iu.inuid = iue.inuid AND iu.inustatus = 'A'
                    $join
                    LEFT JOIN (
                        SELECT edeid, sum(ieiquantidade_total) AS ieiquantidade_total, sum(total_aprovado) AS total_aprovado FROM(
                    	SELECT edeid, sum(ieiquantidade_total) AS ieiquantidade_total, sum(total_aprovado) AS total_aprovado FROM (
	                	  	SELECT iec.edeid, (iec.ieiquantidade * ipi.ipivalorreferencia) as ieiquantidade_total, ana.total_aprovado, ana.anaano
							FROM par3.iniciativa_planejamento_item_composicao ipi
							INNER JOIN(
								SELECT DISTINCT ie.edeid, ie.inpid, iei.ipiid, iei.ieiquantidade
								FROM par3.iniciativa_emenda ie
									INNER JOIN par3.iniciativa_emenda_item_composicao iei ON iei.ineid = ie.ineid AND iei.ieistatus = 'A'
								WHERE ie.inestatus = 'A' $filtroInpid
							) iec ON iec.inpid = ipi.inpid AND iec.ipiid = ipi.ipiid --AND iec.edeid = ipi.edeid
							LEFT JOIN (
								SELECT ie.edeid, ai.ipiid, ie.inpid, ie.anaano, (ai.aicqtdaprovado * ai.aicvaloraprovado) AS total_aprovado FROM par3.analise ie
								INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = ie.anaid AND ai.aicstatus = 'A'
								WHERE ie.anastatus = 'A' AND ie.edeid IS NOT NULL	AND ai.aicqtdaprovado > 0 $filtroInpid
							) ana ON ana.ipiid = ipi.ipiid AND ana.inpid = ipi.inpid
                            WHERE ipi.ipistatus = 'A'
                    	) AS foo
                    	GROUP BY edeid
	                	UNION ALL
	                	SELECT ie.edeid, sum(eo.ieovalor) AS ieiquantidade_total, 0 AS total_aprovado FROM par3.iniciativa_emenda_obra eo
	                		INNER JOIN par3.iniciativa_emenda ie ON ie.ineid = eo.ineid AND ie.inestatus = 'A'
	                	WHERE eo.ieostatus = 'A' $filtroInpid GROUP BY ie.edeid
	                	) AS foo GROUP BY edeid
	                ) iei on iei.edeid = ve.edeid
                WHERE $filtro
                	and ve.edestatus = 'A'
                order by ve.emecod";
        
        return $sql;
    }
    
    public function sqlIniciativaBeneficiario($edeid)
    {
        $sql = "SELECT distinct
                    ve.edeid,
                	ip.inpid,
                    dm.dimcod,
                	et.etadsc,
                	mo.moddsc,
                	ini.iniid||' - '||id.inddsc as inciativa,
                    ito.intodsc,
                    (SELECT ctddsc FROM(
                    	SELECT count(iigid), ctddsc FROM par3.v_item_composicao_planejamento WHERE iniid = ip.iniid and inpid = ip.inpid AND ctdid IS NOT NULL GROUP BY ctddsc ORDER BY 1 LIMIT 1
                    ) AS foo) as categoria,
                    sum(ipi.valor) AS valor,
                    ipi.ipiano
                FROM emenda.v_emendadetalheentidade ve
                    inner JOIN(
                		SELECT DISTINCT  entcnpj, inuid FROM par3.instrumentounidade_entidade WHERE entstatus = 'A'
                	) iu on iu.entcnpj = ve.enbcnpj
                	inner join par3.iniciativa_planejamento ip on ip.inuid = iu.inuid and ip.inpstatus = 'A'
                    INNER JOIN(
                    	SELECT ipi.inpid, ipi.ipiano, sum(ipi.ipiquantidade * ipi.ipivalorreferencia) AS valor FROM par3.iniciativa_planejamento_item_composicao ipi WHERE ipi.ipistatus = 'A' GROUP BY ipi.inpid, ipi.ipiano
                    	UNION ALL
                    	SELECT inpid, obrano AS ipiano, sum(obrvalor) AS valor FROM par3.obra WHERE obrstatus = 'A' GROUP BY inpid, obrano
                    ) ipi ON ipi.inpid = ip.inpid AND ipi.ipiano = ve.emeano
                	inner join par3.iniciativa ini on ini.iniid = ip.iniid
                	inner join par3.iniciativa_descricao id on id.indid = ini.indid
                	inner join workflow.documento d on d.docid = ip.docid
                	inner join par3.ensino_etapa et on et.etaid = ip.etaid
                	inner join par3.modalidade mo on mo.modid = ip.modid --and mo.nivid = et.nivid
                	inner join par3.dimensao dm on dm.dimid = ip.dimid
                    inner join par3.iniciativa_tipos_objeto ito ON ito.intoid = ini.intoid
                WHERE ve.edeid = $edeid
                	and ve.edestatus = 'A'
                GROUP BY ve.emeid, ve.edeid, ip.inpid, dm.dimcod, et.etadsc, mo.moddsc, ini.iniid, id.inddsc, ito.intodsc, ipi.ipiano
                order by dm.dimcod, ito.intodsc, 6";
        
        return $sql;
    }
    
    public function sqlIniciativaBeneficiarioComVinculoParlamentar($edeid)
    {

        $sql = "select
                        *
                    from
                        (
                            with valor_total_indicado as(
                                select
                                    iei.ineid,	sum( iei.ieiquantidade * ipi.ipivalorreferencia ) as ieiquantidade_total
                                from
                                    par3.iniciativa_emenda_item_composicao iei
                                join par3.iniciativa_planejamento_item_composicao ipi on
                                    ipi.ipiid = iei.ipiid
                                where
                                    iei.ieistatus = 'A'
                                group by
                                    iei.ineid
                            union all select
                                    eo.ineid,
                                    sum( eo.ieovalor ) as ieiquantidade_total
                                from
                                    par3.iniciativa_emenda_obra eo
                                where
                                    eo.ieostatus = 'A'
                                group by
                                    eo.ineid
                            ) select
                                distinct ie.ineid,
                                ip.inuid,
                                ip.inpid,
                                dm.dimcod as dimensao,
                                ini.iniid,
                                ini.iniid || ' - ' || id.inddsc as iniciativa,
                                ito.intodsc as objeto,
                                ini.intoid,
                                ed.esddsc as situacao,
                                coalesce(
                                    ip.inpvalor_planejado_total,
                                    0
                                ) as valor_planejado,
                                --coalesce(ie.inevalor,0) as valor_vinculado_parlamentar,
                                '<input name=\"valor_vinculado['||ip.inpid||']\" id=\"valor_vinculado_'||ip.inpid||'\" inpid='||ip.inpid||' type=\"text\" maxlength=\"20\" value=\"'||coalesce(ie.inevalor,0)||'\" size=\"15\" class=\"form-control valor_vinculado\"          onkeyup=\"this.value=mascaraglobal(''[.###],##'',this.value);\" onblur=\"calculaTotalParlamentar('||ip.inpid||', '||ie.ineid||');\" style=\"text-align: right;\">    <input type=\"hidden\" id=\"valorcontrapartida_'|| ie.ineid ||'\" value=\"' || coalesce(ie.inecontrapartida, 0) || '\">' as valor_vinculado_parlamentar,   coalesce(ie_t.inevalor_total,0) as valor_total_vinculado_parlamentar,   coalesce(iei.ieiquantidade_total,0) as valor_total_indicado_municipio   FROM emenda.v_emendadetalheentidade ve    inner JOIN(   SELECT DISTINCT entcnpj, inuid FROM par3.instrumentounidade_entidade WHERE entstatus = 'A'   ) iu on iu.entcnpj = ve.enbcnpj   inner join par3.iniciativa_planejamento ip on ip.inuid = iu.inuid and ip.inpstatus = 'A'
                                --   inner join par3.iniciativa_planejamento_item_composicao ipi on ipi.inpid = ip.inpid and ipi.ipistatus = 'A'
                                inner join par3.iniciativa ini on ini.iniid = ip.iniid
                                inner join par3.iniciativa_descricao id on id.indid = ini.indid
                                --   inner join workflow.documento d on d.docid = ip.docid
                                inner join par3.ensino_etapa et on et.etaid = ip.etaid
                                inner join par3.modalidade mo on mo.modid = ip.modid
                                --and mo.nivid = et.nivid
                                   inner join par3.dimensao dm on dm.dimid = ip.dimid
                                   inner join par3.iniciativa_tipos_objeto ito ON ito.intoid = ini.intoid
                                   inner join par3.iniciativa_emenda ie ON ie.edeid = ve.edeid and
                                   ie.inpid = ip.inpid and
                                   ie.inestatus = 'A'
                                   inner join workflow.documento d on d.docid = ie.docid
                                   inner join workflow.estadodocumento ed on ed.esdid = d.esdid
                                   inner join ( select sum(inevalor) as inevalor_total, inpid FROM par3.iniciativa_emenda WHERE inestatus = 'A' GROUP BY inpid ) ie_t ON ie_t.inpid = ip.inpid
                                   left join valor_total_indicado iei on iei.ineid = ie.ineid
                                   WHERE ve.edeid = $edeid   and
                                   ve.edestatus = 'A'
                                   order by dm.dimcod, iniciativa) contagem";
//         dbg($sql);
        return $sql;
    }
    
    public function sqlItemComposicaoIniciativaBeneficiarioComVinculoParlamentar($inpid, $ineid, $anoEmenda )
    {
        $arWhere = array();
        if ($inpid) {
            $inpid = (array) $inpid;
            $arWhere[] = "vicp.inpid IN (".implode(", ", $inpid).")";
        }
        
        $sql = "select
					vicp.*,
    				ieiquantidade,
    				(iei_t.qtd_total_indicado * valorreferencia) as vlr_total_indicado
				from
					par3.v_item_composicao_planejamento vicp
    			left join par3.iniciativa_emenda_item_composicao iei on iei.ipiid = vicp.ipiid and
    																	iei.ieistatus = 'A' AND
    																	iei.ineid = {$ineid}
    			left join (
							select
								ipiid,
								SUM(ieiquantidade) as qtd_total_indicado
							from
								par3.iniciativa_emenda ie
							join par3.iniciativa_emenda_item_composicao iec on iec.ineid = ie.ineid and
							 												   ieistatus = 'A'
							where
								ie.inestatus = 'A'
							group by
								ipiid) iei_t on iei_t.ipiid = vicp.ipiid
                WHERE vicp.iniano = '".($anoEmenda ? $anoEmenda : date('Y'))."'
    			" . ($arWhere ? " and " . implode(" and ", $arWhere) : "");

        return $sql;
    }
    
    public function sqlCarregarIniciativaEmenda()
    {
        $sql = "SELECT DISTINCT ini.iniid as codigo, (ini.iniid||' - '||ind.inddsc) as descricao
                FROM par3.iniciativa ini
                	INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
                	INNER JOIN par3.iniciativa_planejamento ip ON ip.iniid = ini.iniid AND ip.inpstatus = 'A'
                	INNER JOIN par3.iniciativa_emenda ie ON ie.inpid = ip.inpid AND ie.inestatus = 'A'
                WHERE ini.inistatus = 'A'
                ORDER BY ini.iniid";
        return $sql;
    }
    
    public function sqlVerificaSituacaoAtual($edeid, $esdid)
    {
        
        if ($esdid) {
            $where = " and ed.esdid = $esdid ";
        }
        
        $sql = "SELECT DISTINCT ed.esdid, ed.esddsc
                FROM par3.iniciativa_emenda e
                	INNER JOIN workflow.documento d ON d.docid = e.docid
                	INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                WHERE e.inestatus = 'A' and e.edeid = $edeid $where";
        
        return $sql;
    }
    
    public function sqlPegaParlamenatar($emeid)
    {
        
        if ($emeid && !$this->testa_superuser()) {
            $where = " AND e.emeid = $emeid ";
        }
        
        $sql = "SELECT a.autid as codigo, a.autcod||' - '||a.autnome as descricao FROM emenda.emenda e
                	INNER JOIN emenda.autor a ON a.autid = e.autid
                WHERE e.emeano = '2018' AND e.etoid = 1
                	$where ORDER  BY a.autnome ASC";
        
        return $sql;
    }
    
    public function sqlPegaAcaoWorkFlow()
    {
        
        $sql = "SELECT DISTINCT ed.esdid AS codigo,
                	ed.esdid||' - '||ed.esddsc AS descricao
                FROM workflow.estadodocumento ed
                WHERE ed.tpdid = 317
                ORDER BY ed.esdid";
        
        return $sql;
    }
    
    public function sqlObrasBeneficiarioComVinculoParlamentar($inpid, $emeid, $edeid)
    {
        $sql = "SELECT DISTINCT
                	ie.ineid,
                	ip.inpvalor_planejado_total,
                	ip.inpid,
                	o.obrid,
                	o.obrdsc,
                    o.obrano,
                	ot.otpdsc,
                	o.obrvalor,
                    o.obrvalor_contrapartida,
                	esd.esddsc,
                	ieo.ieovalor,
                	(SELECT sum(ieovalor) FROM par3.iniciativa_emenda_obra WHERE obrid = o.obrid AND ieostatus = 'A') AS vlr_total_indicado,
                	vpp.vlrempenho,
                	vpp.vlrpago
                FROM par3.iniciativa_emenda ie
                	INNER JOIN par3.iniciativa_planejamento ip on ip.inpid = ie.inpid and ip.inpstatus = 'A'
                	INNER JOIN(
                		SELECT DISTINCT  entcnpj, inuid FROM par3.instrumentounidade_entidade WHERE entstatus = 'A'
                	) iu on iu.inuid = ip.inuid
                	INNER JOIN par3.obra o ON o.inpid = ip.inpid AND o.obrstatus = 'A'
                	INNER JOIN par3.obra_tipo as ot on ot.otpid = o.otpid
                	INNER JOIN workflow.documento as doc on doc.docid = o.docid
                	INNER JOIN workflow.estadodocumento as esd on esd.esdid = doc.esdid
                	LEFT JOIN par3.iniciativa_emenda_obra ieo ON ieo.obrid = o.obrid AND ieo.inpid = ip.inpid and ieo.ineid = ie.ineid AND ieo.ieostatus = 'A'
                	LEFT JOIN (
                    	SELECT ve.proid, ve.obrid, sum(ve.saldo) AS vlrempenho, sum(ve.vlrpago) AS vlrpago FROM par3.v_saldo_empenho_por_obra ve GROUP BY ve.obrid, ve.proid
                    ) as vpp on vpp.obrid = o.obrid
                WHERE ie.inestatus = 'A'
                    AND ip.inpid = $inpid and ie.emeid = $emeid AND ie.edeid = $edeid";
        
        return $sql;
    }
    
    public function sqlDadosIniciativaEmenda($inpid, $edeid)
    {
        $sql = "SELECT e.ineid, e.edeid, e.inpid, e.inevalor, ed.esdid, ed.esddsc as situacao, d.docid,
                	coalesce(a.tem_analise,0) AS tem_analise
                FROM par3.iniciativa_emenda e
                	INNER JOIN workflow.documento d ON d.docid = e.docid
                	INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                	LEFT JOIN(
                		SELECT count(a.anaid) AS tem_analise, a.edeid, a.inpid FROM par3.analise a
                		INNER JOIN workflow.documento d ON d.docid = a.docid
                		WHERE d.esdid IN (".PAR3_ESDID_ANALISE_PLANEJAMENTO_APROVADO.", ".PAR3_ESDID_EM_VALIDACAO_COORDENADOR.") AND a.anastatus = 'A'
                		GROUP BY a.edeid, a.inpid
                	) a ON a.edeid = e.edeid AND a.inpid = e.inpid
                WHERE e.inestatus = 'A' and e.edeid = $edeid and e.inpid = $inpid";
        
        return $sql;
    }

    public function selectEmendasPorObra($obrid, $campos = '*', $clausulas = null, $groupby = null)
    {
        if (is_array($campos)) {
            $campos = implode(',', $campos);
        }
        if (is_array($clausulas)) {
            $clausulas = ' AND '.implode(' AND ', $clausulas);
        }
        if (is_array($groupby)) {
            $groupby = 'GROUP BY '.implode(',', $groupby);
        }
         $sql ="
        SELECT {$campos}
        FROM par3.iniciativa_emenda_obra ine
        inner join par3.iniciativa_emenda ieo using(ineid)
        inner join par3.obra obr using (obrid)
        inner join emenda.emenda eme using(emeid)
        INNER JOIN emenda.autor aut using(autid)
        left  join par3.processoobracomposicao poc using(obrid)
        left  join par3.empenhoobracomposicao  emp using(pocid)
        where obrid = {$obrid}
        {$clausulas}
        {$groupby}
      ";
        return $sql;
    }
    
    public function sqlPegaEstadoAtualEmenda($edeid)
    {
        $sql = "SELECT DISTINCT ie.edeid, ed.esdid, ed.esddsc, d.docid
                FROM par3.iniciativa_emenda ie
                	INNER JOIN workflow.documento d on d.docid = ie.docid AND d.tpdid = 317
                	INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid AND ed.tpdid = 317
                WHERE ie.inestatus = 'A' AND edeid = $edeid";
        
        return $sql;
    }
    
    public function sqlTramitePlanejamento($edeid, $esdid)
    {
        $sql = "SELECT DISTINCT COALESCE(eo.tot_obra,0) AS tot_obra, e.inpid, ip.docid, d1.esdid, ae.aedid, eic.qtd
                FROM par3.iniciativa_emenda e
                	INNER JOIN workflow.documento d ON d.docid = e.docid
                	INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = e.inpid AND ip.inpstatus = 'A'
                	INNER JOIN workflow.documento d1 ON d1.docid = ip.docid
                	left JOIN workflow.acaoestadodoc ae ON ae.esdidorigem = d1.esdid AND ae.esdiddestino = ".PAR3_ESDID_AGUARDANDO_ANALISE."
                	LEFT JOIN (
                		SELECT count(eo.ieoid) AS tot_obra, eo.inpid FROM par3.iniciativa_emenda_obra eo
                		INNER JOIN par3.obra o ON o.obrid = eo.obrid
                		WHERE o.obrstatus = 'A' AND eo.ieostatus = 'A'
                		GROUP BY eo.inpid
                	) eo ON eo.inpid = e.inpid
                    INNER JOIN(
                		SELECT ineid, sum(qtd) AS qtd FROM(
							SELECT ic.ineid, ic.ieiquantidade AS qtd FROM par3.iniciativa_emenda_item_composicao ic WHERE ic.ieistatus = 'A'
							UNION ALL
							SELECT eo.ineid, eo.ieovalor AS qtd FROM par3.iniciativa_emenda_obra eo WHERE eo.ieostatus = 'A'
						) AS foo GROUP BY ineid
					) eic ON eic.ineid = e.ineid
                WHERE e.inestatus = 'A' AND e.edeid = $edeid and d.esdid = $esdid AND eic.qtd > 0";
        
        return $sql;
    }
    
    public function sqlPartido()
    {
        $sql = "SELECT
                	p.parid AS codigo,
                	p.parsigla||' - '||p.parnome AS descricao
                FROM emenda.partido p
                ORDER BY 2";
        
        return $sql;
    }
    
    public function sqlAutor()
    {
        $sql = "SELECT codigo, autcod||' - '||descricao AS descricao FROM(
                	SELECT
                		a.autid AS codigo,
                		a.autnome AS descricao,
                		a.autcod
                	FROM emenda.autor a
                	ORDER BY 2
                ) AS foo";
        
        return $sql;
    }
    
    public function sqlAnoExercicio()
    {
        $sql = "SELECT
                	p.prsano AS codigo,
                	p.prsano AS descricao
                FROM emenda.programacaoexercicio p
                WHERE p.prsano >= '2018'
                ORDER BY p.prsano";
        
        return $sql;
    }
    
    public function sqlCarregaRelatorioParlamentar($arrPost = array())
    {
        $empNumero = '';
        $proEmpNumero = '';
        
        $arrPost['prsano'] = (empty($arrPost['prsano']) ? '2018' : $arrPost['prsano']);

        $where = '';
        if ($arrPost['estuf']) {
            $where .= " and iue.estuf = '{$arrPost['estuf']}' ";
        }
        if ($arrPost['muncod']) {
            $where .= " and iue.muncod = '{$arrPost['muncod']}' ";
        }
        if ($arrPost['emeid']) {
            $where .= " and ve.emecod = '{$arrPost['emeid']}' ";
        }
        if ($arrPost['parid']) {
            $where .= " and pa.parid = '{$arrPost['parid']}' ";
        }
        if ($arrPost['autid']) {
            $where .= " and au.autid = '{$arrPost['autid']}' ";
        }
        if ($arrPost['prsano']) {
            $where .= " and ve.emeano = '{$arrPost['prsano']}' ";
        }
        if ($arrPost['esdid']) {
            $where .= " and dap.esdid = '{$arrPost['esdid']}' ";
        }
        if ($arrPost['intoid']) {
            $where .= " and tob.intoid = '{$arrPost['intoid']}' ";
        }
        if( $arrPost['iniciativa_entidade'] == 'S' ){
            $where .= " and ie.valor_indicado > 0";
        }
        if( $arrPost['iniciativa_entidade'] == 'N' ){
            $where .= " and ie.valor_indicado IS NULL";
        }
        if($arrPost['requisicao'] == 'exportar'){
            $empNumero = 'empnumero,';
        }
        
        $sql = "WITH iniciativa_emenda AS (
                 	SELECT ie.inpid, ie.edeid, ie.emeid, sum(ie.inevalor) AS inevalor, sum(itc.valor_indicado) AS valor_indicado, sum(itc.contrapartida) AS contrapartida, itc.tipo
                	FROM par3.iniciativa_emenda ie
                		LEFT JOIN(
                			--Item de Composição
                			SELECT ineid, sum(contrapartida) AS contrapartida, sum(valor_indicado) AS valor_indicado, 'Item' AS tipo FROM(
                				SELECT eic.ineid, e.edeid, e.emeid, ipi.inpid, eic.ipiid, e.inecontrapartida AS contrapartida, (eic.ieiquantidade * ipi.ipivalorreferencia) AS valor_indicado
                				FROM par3.iniciativa_emenda e
                					INNER JOIN par3.iniciativa_emenda_item_composicao eic ON eic.ineid = e.ineid
                					INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.ipiid = eic.ipiid AND ipi.ipistatus = 'A'
                				WHERE eic.ieistatus = 'A' AND e.inestatus = 'A'
                			) AS foo GROUP BY ineid
                			UNION ALL
                			--Obras
                			SELECT eo.ineid, sum(o.obrvalor_contrapartida) AS contrapartida, sum(eo.ieovalor) AS valor_indicado, 'Obra' AS tipo FROM par3.iniciativa_emenda e
                				INNER JOIN par3.iniciativa_emenda_obra eo ON eo.ineid = e.ineid AND eo.ieostatus = 'A'
                				INNER JOIN par3.obra o ON o.obrid = eo.obrid AND o.obrstatus = 'A'
                			WHERE e.inestatus = 'A' GROUP BY eo.ineid
                		) itc ON itc.ineid = ie.ineid
                	WHERE ie.inestatus = 'A' GROUP BY ie.inpid, ie.edeid, ie.emeid, itc.tipo
                 ),
                 dados_analise_processo AS (
                 	SELECT ie.inpid, ie.emeid, ie.edeid, sum(ai.aicqtdaprovado * ai.aicvaloraprovado) AS vlr_processo, es.esdid, es.esddsc AS situacao_analise, ese.esddsc AS situacao_emenda,
                		p.pronumeroprocesso, dt.dotnumero, $empNumero coalesce(emp.vlr_empenho,0) AS vlrempenho, coalesce(emp.vlr_pagamento,0) AS vlr_pagamento
                	FROM par3.iniciativa_emenda ie
                		--INNER JOIN par3.iniciativa_emenda_item_composicao eic ON eic.ineid = ie.ineid AND eic.ieistatus = 'A'
                		--INNER JOIN par3.analise_itemcomposicao ai ON ai.ipiid = eic.ipiid AND ai.aicstatus = 'A'
                		--INNER JOIN par3.analise a ON a.anaid = ai.anaid AND a.edeid = ie.edeid AND a.inpid = ie.inpid AND a.anastatus = 'A'
                		INNER JOIN par3.analise a ON a.inpid = ie.inpid AND a.edeid = ie.edeid AND a.anastatus = 'A'
                        INNER JOIN workflow.documento d ON d.docid = a.docid
                        INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                        INNER JOIN workflow.documento de ON de.docid = ie.docid
                        INNER JOIN workflow.estadodocumento ese ON ese.esdid = de.esdid
                        INNER JOIN par3.iniciativa_emenda_item_composicao eic ON eic.ineid = ie.ineid AND eic.ieistatus = 'A'
                        LEFT JOIN par3.analise_itemcomposicao ai ON ai.ipiid = eic.ipiid AND ai.aicstatus = 'A'
                		LEFT JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = a.inpid AND pp.ppcstatus = 'A'
                		LEFT JOIN par3.processo p ON p.proid = pp.proid AND p.prostatus = 'A'
                		LEFT JOIN par3.documentotermo dt ON dt.proid = p.proid AND dt.dotstatus = 'A'
                		LEFT JOIN(
                            SELECT empnumeroprocesso, inpid, edeid, $empNumero coalesce(sum(vlr_empenho),0) AS vlr_empenho, coalesce(sum(vlr_pagamento),0) AS vlr_pagamento FROM(
                				SELECT e.empcodigoespecie, e.empid, e.empnumeroprocesso, e.empnumero, pc.inpid, a.edeid,
                		    		(sum(epc.epcvalorempenho) - COALESCE(sum(ec.vlr_cancelado),0)) AS vlr_empenho, sum(pg.vlr_pagamento) AS vlr_pagamento
                		    	FROM par3.empenho e
                		    		INNER JOIN par3.empenhoparcomposicao epc ON epc.empid = e.empid AND epc.epcstatus = 'A'
                			        LEFT JOIN(
                			        	SELECT e.empcodigoespecie, e.empidpai, epc.ppcid, sum(epc.epcvalorempenho) AS vlr_cancelado
                			        	FROM par3.empenho e
                			        		INNER JOIN par3.empenhoparcomposicao epc ON epc.empid = e.empid AND epc.epcstatus = 'A'
                			        	WHERE e.empstatus = 'A' AND e.empcodigoespecie in ('03', '13', '04') GROUP BY e.empidpai, e.empcodigoespecie, epc.ppcid
                					) ec ON ec.empidpai = e.empid AND ec.ppcid = epc.ppcid
                					LEFT JOIN(
                						SELECT p.empid, epc.ppcid, sum(ppc.ppcvalorpagamento) AS vlr_pagamento FROM par3.pagamento p
                							INNER JOIN par3.pagamentoparcomposicao ppc ON ppc.pagid = p.pagid
                							INNER JOIN par3.empenhoparcomposicao epc ON epc.epcid = ppc.epcid AND epc.epcstatus = 'A'
                						WHERE p.pagstatus = 'A' and p.pagsituacaopagamento ilike '%efetivado%' GROUP BY p.empid, epc.ppcid
                					) pg ON pg.empid = e.empid AND pg.ppcid = epc.ppcid
                					INNER JOIN par3.processoparcomposicao pc ON pc.ppcid = epc.ppcid AND pc.ppcstatus = 'A'
                                    INNER JOIN par3.analise a ON a.anaid = pc.anaid AND a.anastatus = 'A'
					                INNER JOIN par3.iniciativa_emenda ie ON ie.edeid = a.edeid AND ie.inpid = pc.inpid AND ie.inestatus = 'A'
                				WHERE e.empstatus = 'A' AND e.empcodigoespecie = '01' GROUP BY e.empid, pc.inpid, e.empnumeroprocesso, a.edeid
                			) AS foo GROUP BY $empNumero empnumeroprocesso, inpid, edeid
                	       ) emp ON emp.empnumeroprocesso = p.pronumeroprocesso AND emp.inpid = ie.inpid AND emp.edeid = ie.edeid
                	WHERE ie.inestatus = 'A'
                	GROUP BY ie.inpid, ie.emeid, ie.edeid, es.esdid, es.esddsc, ese.esddsc, p.pronumeroprocesso, dt.dotnumero, emp.vlr_empenho, $empNumero emp.vlr_pagamento
                    union all
                    SELECT ie.inpid, ie.emeid, ie.edeid, sum(eic.ieovalor) AS vlr_processo , es.esdid, es.esddsc AS situacao_analise, ese.esddsc AS situacao_emenda,
                		p.pronumeroprocesso, dt.dotnumero, $empNumero coalesce(emp.vlr_empenho,0) AS vlrempenho, coalesce(emp.vlr_pagamento,0) AS vlr_pagamento
                	FROM par3.iniciativa_emenda ie
                		INNER JOIN par3.iniciativa_emenda_obra eic ON eic.ineid = ie.ineid AND eic.ieostatus = 'A'
                		INNER JOIN par3.obra o ON o.obrid = eic.obrid AND o.obrstatus = 'A'
                		INNER JOIN workflow.documento d ON d.docid = o.docid
                		INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                		INNER JOIN workflow.documento de ON de.docid = ie.docid
                		INNER JOIN workflow.estadodocumento ese ON ese.esdid = de.esdid
                		LEFT JOIN par3.processoobracomposicao pp ON pp.obrid = o.obrid AND pp.pocstatus = 'A'
                		LEFT JOIN par3.processo p ON p.proid = pp.proid AND p.prostatus = 'A'
                		LEFT JOIN par3.documentotermo dt ON dt.proid = p.proid AND dt.dotstatus = 'A'
                        LEFT JOIN(
                			SELECT empnumeroprocesso, inpid, edeid, $empNumero coalesce(sum(vlr_empenho),0) AS vlr_empenho, coalesce(sum(vlr_pagamento),0) AS vlr_pagamento FROM(
                				SELECT e.empcodigoespecie, e.empid, e.empnumeroprocesso, e.empnumero, o.inpid, ie.edeid,
                		    		(sum(epc.eocvalorempenho) - COALESCE(sum(ec.vlr_cancelado),0)) AS vlr_empenho, sum(pg.vlr_pagamento) AS vlr_pagamento
                		    	FROM par3.empenho e
                		    		INNER JOIN par3.empenhoobracomposicao epc ON epc.empid = e.empid AND epc.eocstatus = 'A'
                			        LEFT JOIN(
                			        	SELECT e.empcodigoespecie, e.empidpai, epc.pocid, sum(epc.eocvalorempenho) AS vlr_cancelado
                			        	FROM par3.empenho e
                			        		INNER JOIN par3.empenhoobracomposicao epc ON epc.empid = e.empid AND epc.eocstatus = 'A'
                			        	WHERE e.empstatus = 'A' AND e.empcodigoespecie in ('03', '13', '04') GROUP BY e.empidpai, e.empcodigoespecie, epc.pocid
                					) ec ON ec.empidpai = e.empid AND ec.pocid = epc.pocid
                					LEFT JOIN(
                						SELECT p.empid, epc.pocid, sum(ppc.pmcvalorpagamento) AS vlr_pagamento FROM par3.pagamento p
                							INNER JOIN par3.pagamentoobracomposicao ppc ON ppc.pagid = p.pagid
                							INNER JOIN par3.empenhoobracomposicao epc ON epc.eocid = ppc.eocid AND epc.eocstatus = 'A'
                						WHERE p.pagstatus = 'A' and p.pagsituacaopagamento ilike '%efetivado%' GROUP BY p.empid, epc.pocid
                					) pg ON pg.empid = e.empid AND pg.pocid = epc.pocid
                					INNER JOIN par3.processoobracomposicao pc ON pc.pocid = epc.pocid AND pc.pocstatus = 'A'
                					INNER JOIN par3.obra o ON o.obrid = pc.obrid AND o.obrstatus = 'A'
                                    INNER JOIN par3.iniciativa_emenda_obra io ON io.obrid = o.obrid AND io.ieostatus = 'A'
					                INNER JOIN par3.iniciativa_emenda ie ON ie.ineid = io.ineid AND ie.inestatus = 'A'
                				WHERE e.empstatus = 'A' AND e.empcodigoespecie = '01' GROUP BY e.empid, o.inpid, e.empnumeroprocesso, ie.edeid
                			) AS foo GROUP BY $empNumero empnumeroprocesso, inpid, edeid
                	    ) emp ON emp.empnumeroprocesso = p.pronumeroprocesso AND emp.inpid = ie.inpid AND emp.edeid = ie.edeid
                	WHERE ie.inestatus = 'A'
                	GROUP BY ie.inpid, ie.emeid, ie.edeid, es.esdid, es.esddsc, ese.esddsc, p.pronumeroprocesso, dt.dotnumero, emp.vlr_empenho, $empNumero emp.vlr_pagamento
                 )
                 SELECT DISTINCT
                    ve.emecod,
                    ve.emeano,
                    pa.parsigla partido,
                    au.autcod||' - '||au.autnome AS paralmentar,
                    formata_cpf_cnpj(ve.enbcnpj) as cnpj,
                    ve.enbnome,
                    iue.estuf,
                    tob.objeto as objeto,
                    ie.inpid,
                    ve.emevalor,
                    ve.edevalor AS vlrdesignado,
                    coalesce(ie.valor_indicado,0) AS valor_indicado,
                    formata_num_processo(dap.pronumeroprocesso) as processo,
                    dap.dotnumero||'-'||tob.intoid as termo,
                    case when dap.pronumeroprocesso is not null then coalesce(dap.vlr_processo,0) else 0 end AS vlr_processo,
                    coalesce(ie.contrapartida,0) AS vlr_contrapartida,
                    coalesce(dap.vlrempenho,0) AS vlrempenho,
                    $empNumero
                    coalesce(dap.vlr_pagamento,0) AS vlrpagamento,
                    dap.situacao_analise
                FROM emenda.v_emendadetalheentidade ve
                    INNER JOIN emenda.autor au ON au.autid = ve.autid
                    INNER JOIN emenda.partido pa ON pa.parid = au.parid
                    INNER JOIN(
                        SELECT DISTINCT entcnpj, inuid FROM par3.instrumentounidade_entidade WHERE entstatus = 'A' AND entcnpj IS NOT NULL
                    ) iu on iu.entcnpj = ve.enbcnpj
                    INNER JOIN par3.instrumentounidade iue ON iue.inuid = iu.inuid AND iue.inustatus = 'A'
                    LEFT JOIN iniciativa_emenda ie ON ie.emeid = ve.emeid AND ie.edeid = ve.edeid
                    LEFT JOIN dados_analise_processo dap ON dap.emeid = ve.emeid AND dap.edeid = ve.edeid AND ie.inpid = dap.inpid
                    LEFT JOIN(
                    	SELECT DISTINCT ip.inpid, it.intoid, it.intodsc AS objeto FROM par3.iniciativa_planejamento ip
                			INNER JOIN par3.iniciativa i ON i.iniid = ip.iniid
                			INNER JOIN par3.iniciativa_tipos_objeto it ON it.intoid = i.intoid
                		WHERE ip.inpstatus = 'A'
                    ) tob ON tob.inpid = ie.inpid
                WHERE ve.edestatus = 'A' and ve.etoid = 1 $where
                ORDER BY enbnome";
        
        return $sql;
    }

    public function sqlSituacao()
    {
        $sql = "SELECT distinct esd1.esddsc as descricao,  doc1.esdid as codigo FROM par3.analise a
                INNER JOIN workflow.documento as doc1 on doc1.docid = a.docid
                INNER JOIN workflow.estadodocumento as esd1 on esd1.esdid = doc1.esdid
                WHERE a.anastatus = 'A'";
        return $sql;
    }

    public function sqlObjeto()
    {
        $sql = "select
                intoid as codigo,
                intodsc as descricao
                FROM par3.iniciativa_tipos_objeto WHERE intostatus = 'A'";
        return $sql;
    }
}

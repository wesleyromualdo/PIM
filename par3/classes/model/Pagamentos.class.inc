<?php
class Par3_Model_Pagamentos extends Modelo
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
    protected $arChavePrimaria = array('dotid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela     = array('pagid', 'pagparcela', 'paganoexercicio', 'pagvalorparcela', 'paganoparcela', 'pagmes', 'pagnumeroempenho', 'empid', 'usucpf', 'pagsituacaopagamento',
                                                'pagdatapagamento', 'pagnumseqob', 'pagstatus', 'pagdatapagamentosiafi', 'pagnumeroob', 'pagusucpfexclusao', 'pagdataexclusao', 'pagjustificaitiva');
    
    protected $arAtributos     = array(
            'pagid' => null,
            'pagparcela' => null,
            'paganoexercicio' => null,
            'pagvalorparcela' => null,
            'paganoparcela' => null,
            'pagmes' => null,
            'pagnumeroempenho' => null,
            'empid' => null,
            'usucpf' => null,
            'pagsituacaopagamento',
            'pagdatapagamento' => null,
            'pagnumseqob' => null,
            'pagstatus' => null,
            'pagdatapagamentosiafi' => null,
            'pagnumeroob' => null,
            'pagusucpfexclusao' => null,
            'pagdataexclusao' => null,
            'pagjustificaitiva'  => null
                                      );
    
                                  
    function __construct()
    {
        parent::__construct();
    }
    
    public function recuperar($arrPost)
    {
        return $this->model;
    }
    
    public function pegaFiltrosLista($arrPost = array())
    {
        $where = '';
        if ($arrPost['muncod'] && count($arrPost['muncod']) != 0) {
            $arrMuncod = array_filter($arrPost['muncod'], function ($val) {
                return !empty($val);
            });
            if (count($arrMuncod) != 0) {
                $where  .= " AND iu.muncod::integer in (".implode(',', $arrMuncod).")";
            }
        }
        $where  .= $arrPost['estuf'] ? " AND iu.estuf = '{$arrPost['estuf']}'":'';
        $where  .= $arrPost['proid'] ? " AND p.proid = '{$arrPost['proid']}'":'';
        $where  .= $arrPost['itrid'] && $arrPost['itrid'] != 0 ?($arrPost['itrid'] == 1 ? "AND iu.itrid in(1,3)" :"AND iu.itrid = {$arrPost['itrid']}"): "";
        
        if ($arrPost['termo_validado'] == 'S') {
            $where .= " and p.proid in(select
                                            dt.proid
                                       from par3.documentotermo dt
                                       inner join par3.documentotermovalidacao dtv ON dtv.dotid = dt.dotid
                                       where dotstatus = 'A' ) ";
        }
        if ($arrPost['termo_validado'] == 'N') {
            $where .= " and p.proid not in(select
                                            dt.proid
                                       from par3.documentotermo dt
                                       inner join par3.documentotermovalidacao dtv ON dtv.dotid = dt.dotid
                                       where dotstatus = 'A' ) ";
        }
        
        if ($arrPost['termonota'] == 'S') {
            $where .= " and p.proid in(select proid from par3.documentotermo inner join par3.execucao_notafiscal using (proid) where dotstatus = 'A' ) ";
        }
        
        if ($arrPost['termonota'] == 'N') {
            $where .= " and p.proid not in(select proid from par3.documentotermo inner join par3.execucao_notafiscal using (proid) where dotstatus = 'A' ) ";
        }
        
        if ($arrPost['bloqueio_par'] == 'S') {
            $where .= " AND p.inuid IN (Select inuid from par3.vm_relatorio_quantitativo_pendencias
									WHERE (obras_par = true
										or cacs = true or pne = true
										or siope = true or habilitacao = true) ) ";
        }
        if ($arrPost['bloqueio_par'] == 'N') {
            $where .= " AND p.inuid IN (Select inuid from par3.vm_relatorio_quantitativo_pendencias
									WHERE obras_par = FALSE
										AND cacs = FALSE AND pne = FALSE
										AND siope = FALSE AND habilitacao = FALSE) ";
        }
//        if ($arrPost['status_pagamento'] == 'S') {
//            $where .= " AND p.pronumeroprocesso IN (SELECT processo FROM par3.v_saldo_empenho_do_processo WHERE vlrempenho > (vlrpago)) ";
//        }
//        if ($arrPost['status_pagamento'] == 'N') {
//            $where .= " AND p.pronumeroprocesso not IN ( SELECT processo FROM par3.v_saldo_empenho_do_processo WHERE vlrempenho > (vlrpago) ) ";
//        }

        $status_pagamento = !empty($arrPost['status_pagamento']) ? $arrPost['status_pagamento'] : array();
        $status_pagamento =  array_filter($status_pagamento);

        if($status_pagamento) {
            $wherePagamento = [];
            $wherePagamento[] = in_array('S',$status_pagamento) ? ' (vlrpago = 0 or vlrpago is null) ' : '';//Sem pagamento
            $wherePagamento[] = in_array('C',$status_pagamento) ? ' (vlrpago = vlrempenho ) ' : '';//100% pagamento
            $wherePagamento[] = in_array('P',$status_pagamento) ? ' (vlrpago > 0 AND vlrempenho > vlrpago )' : '';//Parcial

            if(!in_array('T',$status_pagamento)) {
                $wherePagamento = array_filter($wherePagamento);
                if($wherePagamento) {
                    $arrImplode = implode(' OR ',$wherePagamento);
                    $where .= "
                AND p.pronumeroprocesso IN ( SELECT processo FROM par3.v_saldo_empenho_do_processo WHERE ({$arrImplode}) ) ";
                }
            }

        }
        
        if ($arrPost['pagsituacaopagamento'] && count($arrPost['pagsituacaopagamento']) != 0) {
            $arrPagSituacao = array_filter($arrPost['pagsituacaopagamento'], function ($val) {
                return !empty($val);
            });
            if (count($arrPagSituacao) != 0) {
                if (count($arrPost['pagsituacaopagamento']) > 1 || (count($arrPost['pagsituacaopagamento']) == 1 && $arrPost['pagsituacaopagamento'][0] != 'sem_pagamento')) {
                    $arrPagSituacao = $arrPost['pagsituacaopagamento'];
                    $pagsituacao  = " AND (";
                    $pagsituacao .= ($arrPagSituacao[0] == 'sem_pagamento'? 'pag.pagsituacaopagamento IS NULL ' :" pag.pagsituacaopagamento ilike '%{$arrPagSituacao[0]}%' ");
                    unset($arrPagSituacao[0]);
                    if (count($arrPagSituacao) == 1) {
                        $pagsituacao .= ($arrPagSituacao[1] == 'sem_pagamento'? 'OR pag.pagsituacaopagamento IS NULL' :" OR pag.pagsituacaopagamento ilike '%{$arrPagSituacao[1]}%' ");
                    } else {
                        $pagsituacao .= implode(' ', array_map(function ($val) {
                            return $val == 'sem_pagamento'? 'OR pag.pagsituacaopagamento IS NULL ':"OR pag.pagsituacaopagamento ilike '%{$val}%' ";
                        }, $arrPagSituacao));
                    }
                } else {
                    $pagsituacao =' AND( pag.pagsituacaopagamento IS NULL ';
                }
                $pagsituacao .= ")";
                $where .= $pagsituacao;
            }
        }
        
        if ($arrPost['iniano']) {
            $where  .= " AND substring(p.pronumeroprocesso, 12,4) = '{$arrPost['iniano']}'";
        }
        
        if ($_REQUEST['numeroprocesso']) {
            $_REQUEST['numeroprocesso'] = str_replace(".", "", $_REQUEST['numeroprocesso']);
            $_REQUEST['numeroprocesso'] = str_replace("/", "", $_REQUEST['numeroprocesso']);
            $_REQUEST['numeroprocesso'] = str_replace("-", "", $_REQUEST['numeroprocesso']);
            $where .= " and p.pronumeroprocesso = '".$_REQUEST['numeroprocesso']."'";
        }
        
        if ($arrPost['intaid']) {
            switch ($arrPost['intaid']) {
                case 1:
                    $where .= " and p.intaid = 1 ";
                    break;
                case 2:
                    $where .= " and p.intaid = 2 ";
                    break;
                default:
                    $where .='';
                    break;
            }
        }

        if($arrPost['saldo_desembolso'] == 'S'){
            $where .= ' AND vdp.saldo > 0 ';
        }else if($arrPost['saldo_desembolso'] == 'N'){
            $where .= ' AND (vdp.saldo IS NULL OR vdp.saldo <= 0) ';
        }

        if($arrPost['pagdatapagamentoini'] && !$arrPost['pagdatapagamentofim']){
            $where .= " AND pa.pagdatapagamento >= '" . formata_data_sql($arrPost['pagdatapagamentoini']) . "'";
        }else if(!$arrPost['pagdatapagamentoini'] && $arrPost['pagdatapagamentofim']){
            $where .= " AND pa.pagdatapagamento <= '" . formata_data_sql($arrPost['pagdatapagamentofim']) . "'";
        }else if($arrPost['pagdatapagamentoini'] && $arrPost['pagdatapagamentofim']){
            $where .= " AND pa.pagdatapagamento BETWEEN '" . formata_data_sql($arrPost['pagdatapagamentoini']) . "' AND '" . formata_data_sql($arrPost['pagdatapagamentofim']) . "'";
        }

        return $where;
    }
    
    public function listarDadosPagamento($arrPost)
    {
        
        $where = $this->pegaFiltrosLista($arrPost);
        if( $arrPost['codigo'] ){
            $whereObra = " AND po.proid in (SELECT proid FROM par3.processoobracomposicao WHERE obrid = '{$arrPost['codigo']}' AND pocstatus = 'A') ";
            $wherePlan = " AND pp.proid in (SELECT proid FROM par3.processoparcomposicao WHERE inpid = '{$arrPost['codigo']}' AND ppcstatus = 'A') ";
        }else{
            if($arrPost['tipo_processo'] != 'T') {
                if ($arrPost['tipo_processo'] == 'O') $where .= " AND obj.intoid = 1";
                if ($arrPost['tipo_processo'] == 'P') $where .= " AND obj.intoid not in (1)";
            }
        }
        
        $where  .= $arrPost['intoid'] ? " AND obj.intoid = {$arrPost['intoid']}":'';
        
        if ($arrPost['tipo_processo'] == 'O'){
            if( $arrPost['os_validada'] == 'S' ){
                $whereObra .= "AND po.obrid IN (SELECT obrid_par3 FROM obras2.obras obr
					    							INNER JOIN obras2.validacao val ON val.obrid = obr.obrid
					    						WHERE obr.obrstatus = 'A' AND obr.obridpai IS NULL AND obrid_par3 IS NOT NULL
					    							AND val.vldstatushomologacao = 'S')";
            }            
            if( $arrPost['os_validada'] == 'N' ){
                $whereObra .= "AND po.obrid IN (SELECT obrid_par3 FROM obras2.obras obr
					    							INNER JOIN obras2.validacao val ON val.obrid = obr.obrid
					    						WHERE obr.obrstatus = 'A' AND obr.obridpai IS NULL AND obrid_par3 IS NOT NULL
					    							AND val.vldstatushomologacao = 'N')";
            }
        }
        
        $innerTermo = '';
        if ($arrPost['termocontrato'] == 'S' || $arrPost['termocontrato'] == 'N' || $arrPost['dotnumero']) {
            $termoWhere = '';
            switch ($arrPost['termocontrato']) {
                case 'S':
                    $termoWhere = 'and qtdcontrato.qtd > 0';
                    $termoWhere = 'and qtdcontrato.qtd > 0';
                    break;
                case 'N':
                    $termoWhere = 'and qtdcontrato.qtd = 0';
                    break;
                default:
                    break;
            }
            
            $teWhere = $arrPost['dotnumero'] ? " AND doc.dotnumero = {$arrPost['dotnumero']}":'';
            
            $innerTermo .= "
	                inner JOIN (
	                    select
	                    count(ec.ecoid) as qtd,pro.proid
	                    FROM par3.processo pro
	                    inner join par3.documentotermo doc ON doc.proid = pro.proid AND doc.dotstatus = 'A' $teWhere
	                    inner join par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.processo = pro.pronumeroprocesso
	                    left join par3.execucao_contrato ec ON ec.proid = pro.proid
	                    group by pro.proid
	                ) as qtdcontrato on qtdcontrato.proid = p.proid {$termoWhere}
            ";
        }
        
        $join = '';
        $orderBy = '';
        $orderByGeral = '5,3,4';
        $camposDetalhados = '';
        $groupByGeral = '';
        $coluna = <<<coluna
                        p.proid,
                        p.proid as id,
                        (case pp.tipo_processo
                          when 'O' then 'obra'
                          when 'P' then 'par'
                          else pp.tipo_processo
                          end
                        ) as tipo,
                        p.inuid,
    				    (p.pronumeroprocesso) as processo,
                        case when iu.muncod is null then est.estuf else mun.estuf end as uf,
    				    case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
    				    array_to_string(array(SELECT DISTINCT t.intodsc FROM par3.iniciativa_tipos_objeto t
        				    						INNER JOIN(
            				    						SELECT intoid, proid FROM par3.processoparcomposicao WHERE ppcstatus = 'A'
            				    						UNION ALL
            				    						SELECT intoid, proid FROM par3.processoobracomposicao WHERE pocstatus = 'A'
        				    						) pp ON pp.intoid = t.intoid
        				    						WHERE t.intostatus = 'A'
        				    						AND pp.proid = p.proid), ', ') AS tipo_obrjeto,
    				    sum(pp.vlriniciativa::NUMERIC(20,2)) AS valor_processo,
    				    e.vlrempenho as vlrempenho,
    				    coalesce(e.vlrpago,0) as vlrpagamento,
                        (case when e.vlrempenho > 0 then ((e.vlrpago / e.vlrempenho) * 100) ELSE 0 end)::NUMERIC(20,2) ||'%' as porcent_pago,
                        (case when sum(pp.vlriniciativa::NUMERIC(20,2)) > 0 then ((e.vlrpago / sum(pp.vlriniciativa::NUMERIC(20,2))) * 100) ELSE 0 end)::NUMERIC(20,2) ||'%' as porcent_pago_processo,
                        ta.intadsc,
coluna;
        if ($_REQUEST['requisicao'] == 'excel') {
            if($_REQUEST['tipodetalhado'] == 'nao'){
                
                $join = " left join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = p.inuid ";
                $orderBy = " ,vm.obras_par, vm.cacs, vm.pne, vm.siope, vm.habilitacao, pp.intoid ";
                $coluna.= " CASE WHEN pag.pagsituacaopagamento IS NOT NULL THEN pag.pagsituacaopagamento ELSE 'Sem Pagamento' END AS situacaopagamento, 
                        CASE WHEN obras_par = 't' THEN 'Sim' ELSE 'Não' END AS obras_par,
                    	CASE WHEN cacs = 't' THEN 'Sim' ELSE 'Não' END AS cacs,
                    	CASE WHEN pne = 't' THEN 'Sim' ELSE 'Não' END AS pne,
                    	CASE WHEN siope = 't' THEN 'Sim' ELSE 'Não' END AS siope,
                    	CASE WHEN habilitacao = 't' THEN 'Sim' ELSE 'Não' END AS habilitacao,
                    	CASE WHEN contas = 't' THEN 'Sim' ELSE 'Não' END AS contas,
                        case when pp.intoid = 1 then 
                            (SELECT 
                            	CASE WHEN val.vldstatushomologacao = 'S' THEN 'Sim' ELSE 'Não' END 
                            FROM obras2.obras obr
                            	INNER JOIN obras2.validacao val ON val.obrid = obr.obrid
                            	INNER JOIN par3.processoobracomposicao pp ON pp.obrid = obr.obrid_par3 AND pp.pocstatus = 'A'
                            WHERE obr.obrstatus = 'A' AND obr.obridpai IS NULL AND pp.proid = p.proid limit 1) else '-' end as os_validada
                        ";
                $groupByGeral = ', contas';
            }

        } else {
            $coluna.= "CASE WHEN pag.pagsituacaopagamento IS NOT NULL THEN pag.pagsituacaopagamento ELSE 'Sem Pagamento' END AS situacaopagamento, 
                       vdp.saldo as saldo_desembolso,
                       vdp.deferimento,
                       p.inuid as inuid_pendencia ";
        }
        
        if($arrPost['pagdatapagamentoini'] || $arrPost['pagdatapagamentofim']){
            $join .= " INNER JOIN par3.empenho ep ON ep.empnumeroprocesso = p.pronumeroprocesso
                         left JOIN par3.pagamento pa ON pa.empid = ep.empid ";
        }
        
        if( $_REQUEST['requisicao'] == 'excel'){
            if( $_REQUEST['tipodetalhado'] == 'sim' ){
                
                $sqlO = $this->listarExcelDetalhadoObra($where, $whereObra, $join, $innerTermo);
                $sqlP = $this->listarExcelDetalhadoPlanejamento($where, $wherePlan, $join, $innerTermo);
                
                if ($arrPost['tipo_processo'] == 'O'){
                    $sql = $sqlO;
                } elseif ($arrPost['tipo_processo'] == 'P'){
                    $sql = $sqlP;
                } else {
                    $sql = '('.$sqlO.') union all ('.$sqlP.')';
                }
                return $sql;
            }
        }
        
            $sql = "SELECT
                         $coluna
    				FROM par3.processo p
    					INNER JOIN(
    						SELECT pp.proid, sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa, 'P' as tipo_processo, pp.intoid
    						FROM par3.iniciativa_planejamento_item_composicao ipi
    							INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
    							INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
    							INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid
    						WHERE ipi.ipistatus = 'A' AND pp.ppcstatus = 'A' $wherePlan GROUP BY pp.proid, pp.intoid
    						UNION ALL
    						SELECT po.proid, sum(o.obrvalor) AS vlriniciativa, 'O' as tipo_processo, po.intoid
    						FROM par3.obra o
    							INNER JOIN par3.processoobracomposicao po ON po.obrid = o.obrid AND po.pocstatus = 'A'
    						WHERE o.obrstatus = 'A' $whereObra GROUP BY po.proid, po.intoid
    					)pp ON pp.proid = p.proid
    				    inner join par3.v_saldo_empenho_do_processo e on e.processo = p.pronumeroprocesso
    				    --inner join par3.iniciativa_planejamento ip on ip.inpid = p.inpid and ip.inpstatus = 'A'
    				    inner join par3.instrumentounidade iu on iu.inuid = p.inuid and iu.inustatus = 'A'
    				    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = pp.intoid
                        INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = p.intaid
                        INNER JOIN par3.vm_situacao_empenho_pagamento_processo pag ON pag.empnumeroprocesso = p.pronumeroprocesso
                        $innerTermo
    				    LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
    				    LEFT JOIN territorios.estado est on est.estuf = iu.estuf
    				    LEFT JOIN par3.v_desembolso_por_processo vdp ON vdp.proid = p.proid
                        $join
    				WHERE
    				    e.vlrempenho > 0
                        $where
                    GROUP BY  p.proid, p.inuid, p.pronumeroprocesso, iu.muncod, est.estuf, mun.estuf, est.estdescricao, mun.mundescricao, e.vlrempenho, e.vlrpago, ta.intadsc, 
                           pag.pagsituacaopagamento, p.inuid, pp.tipo_processo, vdp.saldo, vdp.deferimento $groupByGeral
                        $orderBy
    				order by ".$orderByGeral;

        return $sql;
    }
    /*public function listarDocObras($arrPost)
    {
        $where = $this->pegaFiltrosLista($arrPost);
        $where  .= $arrPost['codigo'] ? " AND o.obrid = '{$arrPost['codigo']}'":'';

        $innerTermo = '';
        if ($arrPost['termocontrato'] == 'S' || $arrPost['termocontrato'] == 'N' || $arrPost['dotnumero']) {
            $termoWhere = '';
            switch ($arrPost['termocontrato']) {
                case 'S':
                    $termoWhere = 'and qtdcontrato.qtd > 0';
                    break;
                case 'N':
                    $termoWhere = 'and qtdcontrato.qtd = 0';
                    break;
                default:
                    break;
            }
            
            $teWhere = $arrPost['dotnumero'] ? " AND doc.dotnumero = {$arrPost['dotnumero']}":'';
            
            $innerTermo .= "
	                inner JOIN (
	                    select
	                    count(ec.ecoid) as qtd,pro.proid
	                    FROM par3.processo pro
	                    inner join par3.documentotermo doc ON doc.proid = pro.proid AND doc.dotstatus = 'A' $teWhere
	                    inner join par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.processo = pro.pronumeroprocesso
	                    left join par3.execucao_contrato ec ON ec.dotid = doc.dotid
	                    group by pro.proid
	                ) as qtdcontrato on qtdcontrato.proid = p.proid {$termoWhere}
            ";
        }
        
        $join = '';
        if ($_REQUEST['requisicao'] == 'excel') {
            $coluna = "CASE WHEN obras_par = 't' THEN 'Sim' ELSE 'Não' END AS obras_par,
                    	CASE WHEN cacs = 't' THEN 'Sim' ELSE 'Não' END AS cacs,
                    	CASE WHEN pne = 't' THEN 'Sim' ELSE 'Não' END AS pne,
                    	CASE WHEN siope = 't' THEN 'Sim' ELSE 'Não' END AS siope,
                    	CASE WHEN habilitacao = 't' THEN 'Sim' ELSE 'Não' END AS habilitacao";
            $join = " left join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = p.inuid";
        } else {
            $coluna = " p.inuid as inuid_pendencia ";
        }
        
        $sql = "SELECT distinct p.proid, 'obra' as tipo, p.inuid,
				    (p.pronumeroprocesso) as processo,
				    case when iu.muncod is null then est.estuf else mun.estuf end as uf,
				    case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
				    'Obra' AS tipo_obrjeto,
					vip.vlriniciativa::NUMERIC(20,2) AS valor_processo,
				    e.vlrempenho::NUMERIC(20,2) as vlrempenho,
				    e.vlrpago::NUMERIC(20,2) as vlrpagamento,
                    (case when e.vlrempenho > 0 then ((vlrpago / vlrempenho) * 100) ELSE 0 end)::NUMERIC(20,2) ||'%' as porcentagem,
                    ta.intadsc,
				    CASE WHEN pag.pagsituacaopagamento IS NOT NULL THEN pag.pagsituacaopagamento ELSE 'Sem Pagamento' END AS situacaopagamento,
                    $coluna
                    FROM par3.processo p
				    inner join par3.processoobracomposicao pp on pp.proid = p.proid and pp.pocstatus = 'A'
                    inner join par3.obra o on o.obrid = pp.obrid and o.obrstatus = 'A'
				    inner join par3.v_saldo_empenho_do_processo e on e.processo = p.pronumeroprocesso
				    inner join par3.iniciativa_planejamento ip on ip.inpid = o.inpid
                    inner join par3.iniciativa ini on ini.iniid = ip.iniid
				    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
				    inner join par3.instrumentounidade iu on iu.inuid = ip.inuid and iu.inustatus = 'A'
				    INNER JOIN(
				     	SELECT po.proid, sum(o.obrvalor) - sum(COALESCE(o.obrvalor_contrapartida,0)) AS vlriniciativa FROM par3.obra o
				     		INNER JOIN par3.processoobracomposicao po ON po.obrid = o.obrid AND po.pocstatus = 'A'
				     	WHERE o.obrstatus = 'A' GROUP BY po.proid
				     )vip ON vip.proid = p.proid
                    INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = p.intaid
                    INNER JOIN par3.vm_situacao_empenho_pagamento_processo pag ON pag.empnumeroprocesso = p.pronumeroprocesso
				    {$innerTermo}
				    LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
				    LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                    $join
				WHERE
				    ip.inpstatus = 'A'
                    and e.vlrempenho > 0
					$where
				order by 5,6";
        return $sql;
    }*/
    
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
                    coalesce(vt.vlr_total_pagamento,0) as vlr_total_pagamento,
                    coalesce(CASE WHEN vse.vlr_termo> 0 THEN ((coalesce(vse.vlrpago,0) / vse.vlr_termo) * 100)::numeric(20,2) ELSE 0 END,0) AS percent_total_pago_processo
				FROM par3.processo p
				    inner join par3.processoparcomposicao pp on pp.proid = p.proid
				    inner join par3.v_saldo_empenho_composicao_iniciativa vp on vp.ppcid = pp.ppcid
				    INNER JOIN par3.documentotermo dt ON dt.proid = p.proid	AND dt.dotstatus = 'A'	
                    LEFT JOIN par3.v_saldo_empenho_do_processo vse ON vse.proid = p.proid		
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
                    (SELECT sd.sldpercobra
	                   FROM obras2.solicitacao_desembolso sd
	                   	INNER JOIN obras2.obras o ON o.obrid = sd.obrid AND o.obrstatus = 'A' AND o.obridpai IS NULL
	                    INNER JOIN workflow.documento dc ON dc.docid = sd.docid
	                  WHERE o.obrid_par3 = emp.obrid AND (dc.esdid = ANY (ARRAY[1576, 2150]))
	                  ORDER BY sd.sldid DESC LIMIT 1) AS percent_obra,
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
                    coalesce(vt.vlr_total_pagamento,0) as vlr_total_pagamento,
                    coalesce(CASE WHEN vse.vlr_termo> 0 THEN ((coalesce(vse.vlrpago,0) / vse.vlr_termo) * 100)::numeric(20,2) ELSE 0 END,0) AS percent_total_pago_processo
                FROM par3.processo p
                	inner join par3.processoobracomposicao pp on pp.proid = p.proid and pp.pocstatus = 'A'
                    inner join par3.v_saldo_empenho_composicao_obra emp on emp.pocid = pp.pocid
                    LEFT JOIN par3.v_saldo_empenho_do_processo vse ON vse.proid = p.proid
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
    
    public function sqlPegaAcompanhamentoEntidade($proid)
    {
        
        $sql = "SELECT formata_num_processo(pro.pronumeroprocesso) as pronumeroprocesso, iue.inuid, iue.estuf, iue.entidade,
                	(doc.dotnumero||'-'|| doc.intoid)  as numtermo,
                	coalesce(count(ec.ecoid),0) as qtcontrato,
                	coalesce(sum(ec.ecoqtd),0) as qttotalitens,
                	coalesce(sum(ec.ecovalor),0) as valortotalcontrato,
                	coalesce(vsaldo.vlr_termo,0) as valorprocesso,
                	coalesce(vsaldo.vlrempenho,0) as totalempenhado,
                	coalesce(vsaldo.vlrpago,0) as totalpago,
                	coalesce(CASE WHEN vsaldo.vlr_termo > 0 THEN ((coalesce(vsaldo.vlrpago,0) / vsaldo.vlr_termo) * 100)::numeric(20,2) ELSE 0 END,0) AS percent_total_pago
                FROM par3.processo pro
                	INNER JOIN(
                		SELECT iu.inuid, m.estuf, m.mundescricao AS entidade
                		FROM par3.instrumentounidade iu
				    		INNER JOIN territorios.municipio m on m.muncod = iu.muncod
				    	WHERE iu.inustatus = 'A' AND iu.muncod IS NOT NULL
				    	UNION ALL
				    	SELECT distinct iu.inuid, e.estuf, e.estdescricao AS entidade
                		FROM par3.instrumentounidade iu
				    		INNER JOIN territorios.estado e on e.estuf = iu.estuf
				    	WHERE iu.inustatus = 'A' AND iu.muncod IS NULL
                	) iue ON iue.inuid = pro.inuid                	
                	inner join par3.documentotermo doc ON doc.proid = pro.proid AND doc.dotstatus = 'A'
                	inner join par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.processo = pro.pronumeroprocesso
                	left join par3.execucao_contrato ec ON ec.proid = doc.proid AND ec.ecostatus = 'A'
                WHERE pro.proid = $proid
                GROUP BY pro.pronumeroprocesso, doc.dotnumero, doc.intoid, vsaldo.vlrempenho , vsaldo.vlrpago, iue.inuid, iue.estuf, iue.entidade, vsaldo.vlr_termo";

        return $sql;
    }

    /**
     * @todo acrescentar mais situações mapeadas
     */
    public function getPagamentoSituacoes()
    {
        return [
            'sem_pagamento' => 'Sem pagamento',
            'Enviado ao SIGEF' => 'Pendente',
            'SOLICITAÇÃO APROVADA' => '8 - SOLICITAÇÃO APROVADA',
            'EFETIVADO' => '2 - EFETIVADO',
            'Enviado ao SIGEF' => 'Pendente',
            'Cancelado' => '9 - CANCELADO'
        ];
    }

    public function vincularPagamentoDesembolso($arrSld, $arrPagSld, $pagid, $perc_total, $tipo){

        $id = $tipo == 'par' ? 'sdpid' : 'sldid';

        $sql = "INSERT INTO par3.pagamento_desembolso ({$id}, pagid, percent) VALUES ";

        foreach ($arrSld as $id => $percent) {

            $perc_pago = $arrPagSld[$id] / 100 * $percent;

            $perc_total += $perc_pago;

            //ver($percent, $perc_total, $perc_pago);

            if($perc_total >= $percent){
                $perc_total -= $percent;
                $perc_pagamento = 100.00 - $arrPagSld[$id];
            }else{
                $perc_pagamento = $perc_total * 100 / $percent - $arrPagSld[$id];
                $perc_total= 0;
            }

            $perc_pagamento = number_format((float)$perc_pagamento, 2, '.', '');

            $sql .= '(' . $id . ',' . $pagid . ', ' . $perc_pagamento . '),';

            if($perc_total == 0) break;

        }

        $sql = rtrim($sql, ',');
//        ver($sql,d);
        $this->executar($sql);
        $this->commit();
    }
    
    public function listarExcelDetalhadoObra($where, $whereObra, $join, $innerTermo)
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
                SELECT DISTINCT p.proid,
                    o.inpid,
                	o.obrano,
                	p.inuid,
                    (dt.dotnumero || '-'::text) || pp.intoid AS dotnumero,
                    p.pronumeroprocesso AS processo,
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
                    pag.pagdatapagamento,
                    pag.pagparcela,
                    pag.pagmes,
                    pag.paganoparcela,
                    pag.pagnumseqob,
                    pag.pagnumeroob,
                    pag.pagvalorparcela,
                    pag.usucpf || ' - ' || pag.usunome AS usuario_pag,
                    pag.pagsituacaopagamento,
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
                	INNER JOIN par3.processo p ON p.proid = pp.proid AND p.prostatus = 'A'
                	INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = pp.intoid
                	INNER JOIN par3.instrumentounidade iu on iu.inuid = o.inuid and iu.inustatus = 'A'
                	INNER JOIN par3.iniciativa_planejamento ip on ip.inpid = o.inpid
                	INNER JOIN par3.iniciativa ini on ini.iniid = ip.iniid
                	INNER JOIN workflow.documento d ON d.docid = o.docid
                	INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                	INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = p.intaid
                    INNER JOIN empenho_obra emp ON emp.obrid = o.obrid
                    LEFT JOIN par3.vm_situacao_empenho_pagamento_processo emps ON emps.empnumeroprocesso = p.pronumeroprocesso
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
                			obrid, proid, pagparcela, pagmes, paganoparcela, pagnumeroob, pagnumseqob, pagvalorparcela,
                			usucpf, usunome, pagdatapagamento, pagsituacaopagamento,
                			sum(foo.vlr_efetivado) AS vlr_efetivado,
                		    sum(foo.vlr_solicitado) AS vlr_solicitado,
                		    sum(foo.vlrpago) AS vlrpago
                		FROM (		
                			SELECT
                				po.obrid, po.proid, pag.pagparcela, pag.pagmes, pag.paganoparcela, pag.pagnumeroob, pag.pagnumseqob, pag.pagvalorparcela,
                                    pag.usucpf,
                                    u.usunome,
                                    to_char(pag.pagdatapagamento,'dd/mm/YYYY HH24:MI:SS') AS pagdatapagamento,
                                    pag.pagsituacaopagamento,
                				CASE WHEN pag.pagsituacaopagamento ILIKE '%efetivado%' THEN sum(pac.pmcvalorpagamento) ELSE 0 END AS vlr_efetivado,
                				CASE WHEN pag.pagsituacaopagamento NOT ILIKE '%efetivado%' THEN sum(pac.pmcvalorpagamento) ELSE 0 END AS vlr_solicitado,
                				sum(pac.pmcvalorpagamento) AS vlrpago
                			FROM par3.pagamento pag
                				INNER JOIN par3.pagamentoobracomposicao pac ON pac.pagid = pag.pagid AND pac.pmcstatus = 'A'
                				INNER JOIN par3.empenhoobracomposicao epc ON epc.eocid = pac.eocid AND epc.eocstatus = 'A'
                				INNER JOIN par3.processoobracomposicao po ON po.pocid = epc.pocid AND po.pocstatus = 'A'
                				left join seguranca.usuario u on (pag.usucpf = u.usucpf)
                			WHERE pag.pagstatus = 'A'
                				AND pag.pagsituacaopagamento NOT ILIKE '%CANCELADO%' AND pag.pagsituacaopagamento NOT ILIKE '%vala%'
                				AND pag.pagsituacaopagamento NOT ILIKE '%devolvido%' AND pag.pagsituacaopagamento NOT ILIKE '%VALA CENTRO DE GESTÃO%'
                			GROUP BY po.obrid, po.proid, pag.pagsituacaopagamento, pag.pagparcela, pag.pagmes, pag.paganoparcela, pag.pagnumeroob,
                				pag.pagnumseqob, pag.pagvalorparcela, pag.usucpf, u.usunome, pag.pagdatapagamento				
                		) foo
                		GROUP BY obrid, proid, pagparcela, pagmes, paganoparcela, pagnumeroob, pagnumseqob, pagvalorparcela, usucpf, usunome, pagdatapagamento, pagsituacaopagamento		
                	) pag ON pag.obrid = pp.obrid AND pag.proid = pp.proid
                	left join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = p.inuid
                    LEFT JOIN par3.v_desembolso_por_processo vdp ON vdp.proid = p.proid      
                    $innerTermo
                    $join          
                WHERE p.prostatus = 'A' $where $whereObra
                ORDER BY 7,8";
        
        return $sql;
    }
    
    public function listarExcelDetalhadoPlanejamento($where, $wherePlan, $join, $innerTermo)
    {
        $sql = "WITH empenho_planejamento AS(
            	SELECT
            		pp.inpid,
            		pp.ppcano,
                    pp.proid,
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
            	GROUP BY pp.inpid, pp.ppcano, pp.proid, e.empid, e.empnumero, e.empnumeroprocesso,
            	    e.empsituacao, usunome, e.empdata
            )
            SELECT DISTINCT p.proid,
            	po.inpid,
            	po.ppcano,
            	p.inuid,
                (dt.dotnumero || '-'::text) || po.intoid AS dotnumero,
                p.pronumeroprocesso as processo,
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
                coalesce(pag.vlr_efetivado, 0) AS vlrpago,
                pag.pagdatapagamento,
                pag.pagparcela,
                pag.pagmes,
                pag.paganoparcela,
                pag.pagnumseqob,
                pag.pagnumeroob,
                pag.pagvalorparcela,
                pag.usucpf || ' - ' || pag.usunome AS usuario_pag,
                pag.pagsituacaopagamento,
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
            FROM par3.processo p
            	INNER JOIN par3.processoparcomposicao po ON po.proid = p.proid AND po.ppcstatus = 'A'
            	INNER JOIN(
            		SELECT pp.proid, pp.inpid, pp.ppcano, sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa, 'P'::text as tipo_processo
            	    FROM par3.iniciativa_planejamento_item_composicao ipi
            	    	INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = ipi.inpid AND ip.inpstatus = 'A'
            			INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
            			INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
            			INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid AND pp.ppcstatus = 'A'
            		WHERE ipi.ipistatus = 'A' $wherePlan
            		GROUP BY pp.proid, pp.inpid, pp.ppcano
            	) vip ON vip.proid = p.proid AND vip.inpid = po.inpid AND vip.ppcano = po.ppcano
            	INNER JOIN empenho_planejamento emp ON emp.inpid = po.inpid AND emp.ppcano = po.ppcano and emp.proid = po.proid
            	INNER JOIN par3.iniciativa_planejamento pp ON pp.inpid = po.inpid AND pp.inpstatus = 'A'
            	INNER JOIN par3.iniciativa ini on ini.iniid = pp.iniid
            	INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = po.intoid
            	INNER JOIN par3.instrumentounidade iu on iu.inuid = p.inuid and iu.inustatus = 'A'
            	INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = p.intaid
            	LEFT JOIN par3.vm_situacao_empenho_pagamento_processo emps ON emps.empnumeroprocesso = p.pronumeroprocesso
            	LEFT JOIN par3.documentotermo dt ON dt.proid = po.proid AND dt.dotstatus = 'A'
            	LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
            	LEFT JOIN territorios.estado est on est.estuf = iu.estuf
            	LEFT JOIN (
            		SELECT
            			inpid, ppcano, proid, pagparcela, pagmes, paganoparcela, pagnumeroob, pagnumseqob, pagvalorparcela,
            			usucpf, usunome, pagdatapagamento, pagsituacaopagamento,
            			sum(foo.vlr_efetivado) AS vlr_efetivado,
            		    sum(foo.vlr_solicitado) AS vlr_solicitado,
            		    sum(foo.vlrpago) AS vlrpago
            		FROM (
            			SELECT
            				po.inpid, po.proid, po.ppcano, pag.pagparcela, pag.pagmes, pag.paganoparcela, pag.pagnumeroob, pag.pagnumseqob, pag.pagvalorparcela,
                                pag.usucpf,
                                u.usunome,
                                to_char(pag.pagdatapagamento,'dd/mm/YYYY HH24:MI:SS') AS pagdatapagamento,
                                pag.pagsituacaopagamento,
            				CASE WHEN pag.pagsituacaopagamento ILIKE '%efetivado%' THEN sum(pac.ppcvalorpagamento) ELSE 0 END AS vlr_efetivado,
            				CASE WHEN pag.pagsituacaopagamento NOT ILIKE '%efetivado%' THEN sum(pac.ppcvalorpagamento) ELSE 0 END AS vlr_solicitado,
            				sum(pac.ppcvalorpagamento) AS vlrpago
            			FROM par3.pagamento pag
            				INNER JOIN par3.pagamentoparcomposicao pac ON pac.pagid = pag.pagid AND pac.ppcstatus = 'A'
            				INNER JOIN par3.empenhoparcomposicao epc ON epc.epcid = pac.epcid AND epc.epcstatus = 'A'
            				INNER JOIN par3.processoparcomposicao po ON po.ppcid = epc.ppcid AND po.ppcstatus = 'A'
            				left join seguranca.usuario u on (pag.usucpf = u.usucpf)
            			WHERE pag.pagstatus = 'A'
            				AND pag.pagsituacaopagamento NOT ILIKE '%CANCELADO%' AND pag.pagsituacaopagamento NOT ILIKE '%vala%'
            				AND pag.pagsituacaopagamento NOT ILIKE '%devolvido%' AND pag.pagsituacaopagamento NOT ILIKE '%VALA CENTRO DE GESTÃO%'
            			GROUP BY po.inpid, po.proid, po.ppcano, pag.pagsituacaopagamento, pag.pagparcela, pag.pagmes, pag.paganoparcela, pag.pagnumeroob,
            				pag.pagnumseqob, pag.pagvalorparcela, pag.usucpf, u.usunome, pag.pagdatapagamento
            		) foo
            		GROUP BY inpid, ppcano, proid, pagparcela, pagmes, paganoparcela, pagnumeroob, pagnumseqob, pagvalorparcela, usucpf, usunome, pagdatapagamento, pagsituacaopagamento
            	) pag ON pag.inpid = po.inpid AND pag.ppcano = po.ppcano and pag.proid = po.proid
            	left join par3.vm_relatorio_quantitativo_pendencias vm on vm.inuid = p.inuid
                LEFT JOIN par3.v_desembolso_por_processo vdp ON vdp.proid = p.proid
                $innerTermo
                $join
            WHERE p.prostatus = 'A' $where
            ORDER BY 7,8";
        
        return $sql;
    }

}

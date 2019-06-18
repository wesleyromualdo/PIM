<?php
class Par3_Model_Processo extends Modelo
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
    
    public function sqlProcessosPorTipoObjeto($intoid, $inuid, $intaid)
    {
        $sql = "SELECT proid, processo, proagencia, probanco, procnpj, intodsc, intadsc, sum(vlr_processo) AS vlr_processo FROM(
                	SELECT DISTINCT p.proid, formata_num_processo(p.pronumeroprocesso) as processo, p.proagencia, p.probanco, formata_cpf_cnpj(p.procnpj) as procnpj, ta.intodsc, ita.intadsc, (vv.vlriniciativa) AS vlr_processo
                	FROM par3.processo p
                	  	inner join par3.processoparcomposicao pp on pp.proid = p.proid --and pp.ppcstatus = 'A'
                	    inner join par3.iniciativa_planejamento ip on ip.inpid = pp.inpid
                	    inner join par3.iniciativa i on i.iniid = ip.iniid
                	    inner join par3.iniciativa_descricao des on des.indid = i.indid
                	    inner join par3.iniciativa_tipos_objeto ta on ta.intoid = i.intoid
                	    INNER JOIN par3.iniciativa_tipos_assistencia ita ON ita.intaid = p.intaid
                	    left JOIN par3.v_valor_iniciativa_por_processo vv ON vv.proid = pp.proid
                	WHERE i.intoid in ($intoid)
                	   and p.intaid = $intaid
                	   and p.inuid = $inuid
                	   AND p.proid NOT IN (SELECT proid FROM par3.documentotermo WHERE dotstatus = 'A' )
                ) AS foo
                GROUP BY proid, processo, proagencia, probanco, procnpj, intodsc, intadsc";
        
        return $sql;
    }
    
    public function vinculaIniciativaProcessoExistente($arrPost = array())
    {
        $sql = "INSERT INTO par3.processoparcomposicao(proid, inpid, ppcano, anaid, intoid, ppcstatus)
				VALUES ({$arrPost['proid']}, {$arrPost['inpid']}, {$arrPost['iniano']}, {$arrPost['anaid']}, {$arrPost['intoid']}, 'A')";
        return $sql;
    }
    
    public function montarSQLProcesso($arrPost = array())
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
        $where  .= $arrPost['intoid'] ? " AND obj.intoid = {$arrPost['intoid']}":'';
        $where  .= $arrPost['dotnumero'] ? " AND dt.codigo = {$arrPost['dotnumero']}":'';
        
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
        
        if ($arrPost['iniano']) {
            $where  .= " AND substring(p.pronumeroprocesso, 12,4) = '{$arrPost['iniano']}'";
        }
        
        if ($_REQUEST['numeroprocesso']) {
            $_REQUEST['numeroprocesso'] = str_replace(".", "", $_REQUEST['numeroprocesso']);
            $_REQUEST['numeroprocesso'] = str_replace("/", "", $_REQUEST['numeroprocesso']);
            $_REQUEST['numeroprocesso'] = str_replace("-", "", $_REQUEST['numeroprocesso']);
            $where .= " and p.pronumeroprocesso = '".$_REQUEST['numeroprocesso']."'";
        }else{
            if($arrPost['tipo_processo'] != 'T') {
                if($arrPost['tipo_processo'] == 'P'){
                    $where .= " AND pp.tipo_processo = 'P' ";
                }

                if($arrPost['tipo_processo'] == 'O'){
                    $where .= " AND pp.tipo_processo = 'O' ";
                }
            }
        }
        
        $innerTermo = '';
        if ($arrPost['termocontrato'] == 'S' || $arrPost['termocontrato'] == 'N') {
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
            
            $innerTermo .= "
	                inner JOIN (
	                    select
	                    count(ec.ecoid) as qtd,pro.proid
	                    FROM par3.processo pro
	                    inner join par3.documentotermo doc ON doc.proid = pro.proid AND doc.dotstatus = 'A'
	                    inner join par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.processo = pro.pronumeroprocesso
	                    left join par3.execucao_contrato ec ON ec.proid = pro.proid
	                    group by pro.proid
	                ) as qtdcontrato on qtdcontrato.proid = p.proid {$termoWhere}
            ";
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
        
        if( $arrPost['codigo'] ){
            $whereObra = " AND o.obrid = '{$arrPost['codigo']}' ";
            $wherePlan = " AND pp.inpid = '{$arrPost['codigo']}' ";
        }

        $sql = "SELECT 
                    proid,
                    (case tipo_processo 
                      when 'O' then 'obra'
                      when 'P' then 'par' 
                      else tipo_processo
                      end
                    ) as tipo,
                    inuid,
                    intoid,
                    pronumeroprocesso,
                    uf,
                    entidade,
                    tipo_obrjeto,
                    data_vigencia,
                    valor_processo,
                    valor_termo,
                    vlrempenho,
                    vlrpago,
                    (case when  valor_termo > 0 then ((vlrpago / valor_termo) * 100) ELSE 0 end)::NUMERIC(20,2) ||'%' as porcent_pago,
                    ta.intadsc,
                    ta.intaid
                FROM(
                     SELECT distinct p.proid, 'par' as tipo, p.inuid,
                        p.pronumeroprocesso,
                        case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                        ''||case when iu.muncod is null then est.estdescricao else mun.mundescricao end||'' as entidade,
                        array_to_string(ARRAY_AGG(obj.intodsc), ', ', NULL) AS tipo_obrjeto,
                    	array_to_string(ARRAY_AGG(obj.intoid), ', ', NULL) AS intoid,
                        to_char(dt.data_vigencia, 'DD/MM/YYYY') as data_vigencia,
                        sum(pp.vlriniciativa::NUMERIC(20,2)) AS valor_processo,
                        dt.dotvalortermo AS valor_termo,
                        coalesce(e.vlrempenho,0) as vlrempenho,
                        coalesce(e.vlrpago,0) as vlrpago,
                        (case when  dt.dotvalortermo > 0 then ((e.vlrpago /  dt.dotvalortermo) * 100) ELSE 0 end)::NUMERIC(20,2) ||'%' as porcent_pago,
                        p.intaid,
                        pp.tipo_processo
                    FROM par3.processo p
                    	INNER JOIN(
                        	SELECT pp.proid, sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa, a.inpid, 'P' as tipo_processo
                		    FROM par3.iniciativa_planejamento_item_composicao ipi
                				INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
                				INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
                				INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid
                			WHERE ipi.ipistatus = 'A' AND pp.ppcstatus = 'A' $wherePlan
                			GROUP BY pp.proid, a.inpid
                			UNION ALL
                			SELECT po.proid, sum(o.obrvalor) AS vlriniciativa, o.inpid, 'O' as tipo_processo
                            FROM par3.obra o
                         		INNER JOIN par3.processoobracomposicao po ON po.obrid = o.obrid AND po.pocstatus = 'A'
                         	WHERE o.obrstatus = 'A' $whereObra GROUP BY po.proid, o.inpid
                        )pp ON pp.proid = p.proid
                        inner join par3.iniciativa_planejamento ip on ip.inpid = pp.inpid
                        inner join par3.instrumentounidade iu on iu.inuid = ip.inuid and iu.inustatus = 'A'
                        inner join par3.iniciativa ini on ini.iniid = ip.iniid
                        INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
                        LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
                        LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                        LEFT JOIN (
                        	SELECT d.proid, d.dotnumero as codigo, (d.dotnumero || '-'::text) || d.intoid AS dotnumero, d.dotdatafimvigencia AS data_vigencia, sum(d.dotvalortermo) AS dotvalortermo
                               FROM par3.documentotermo d WHERE d.dotstatus = 'A'::bpchar
                              GROUP BY d.proid, d.dotnumero, d.intoid, d.dotdatafimvigencia
                        ) dt ON dt.proid = p.proid
                        left join par3.v_saldo_empenho_do_processo e on e.processo = p.pronumeroprocesso
                    WHERE
                        ip.inpstatus = 'A'
                        $where
                    group by p.proid, iu.muncod,est.estuf,mun.estuf,mun.mundescricao,dt.data_vigencia,dt.dotvalortermo,e.vlrempenho,e.vlrpago,pp.tipo_processo
                ) as foo
                INNER JOIN par3.iniciativa_tipos_assistencia ta ON ta.intaid = foo.intaid";

        return $sql;
    }


    /**
     * Função que lista os processos de acordo com os filtros recebidos
     * @param array $arrFiltros
     * @return string
     */
    public function listarProcessos($arrFiltros = [])
    {
        $where = $this->montarWhere($arrFiltros);

        $sql = "SELECT proid, pronumeroprocesso, muncod, inuid, proagencia, proseqconta, prostatus, intaid
                  from par3.processo where proid in (
                    SELECT p.proid
                      FROM par3.processo p
                      LEFT JOIN par3.processoparcomposicao pp ON pp.proid = p.proid
                      LEFT JOIN par3.processoobracomposicao po ON po.proid = p.proid
                      WHERE $where GROUP BY p.proid)";

        return $sql;
    }


    public function listarComposicaoPar($arrParams)
    {
        $sql = "select * from par3.processoparcomposicao where proid = '{$arrParams['proid']}'";
        return $sql;
    }

    public function listarComposicaoObra($arrParams)
    {
        $sql = "select * from par3.processoobracomposicao where proid = '{$arrParams['proid']}'";
        return $sql;
    }

    //Listar Analise - Model Analise
    public function listarAnalisePar($arrParams)
    {
        $sql = "select an.anaid, an.inpid, an.anaano, an.anaparecer, an.anastatus, an.intaid, an.edeid
                from par3.analise an
                inner join par3.processoparcomposicao ppc ON ppc.anaid = an.anaid
                where ppc.proid = '{$arrParams['proid']}'";
        return $sql;
    }

    public function listarAnaliseObra($arrParams)
    {
        $sql = "select obr.obrid, obr.otpid, obr.ovpid, obr.tplid, obr.inuid, obr.muncod, obr.obrano, obr.obrstatus
                from par3.obra obr
                inner join par3.processoobracomposicao poc ON poc.obrid = obr.obrid
                where poc.proid = '{$arrParams['proid']}'";

        return $sql;
    }

    //Listar Empenho
    public function listarEmpenho($numeroProcesso)
    {
        $sql = "select empid, empcnpj, empidpai, empnumero, empvalorempenho, empstatus, empcodigoespecie, empsituacao from par3.empenho
                where empnumeroprocesso = '{$numeroProcesso}'";
        return $sql;
    }
    //Listar Empenho
    public function listarEmpenhoPar($numeroProcesso)
    {
        $sql = "select epc.* from par3.empenhoparcomposicao epc
                inner join par3.empenho emp ON emp.empid = epc.empid
                where emp.empnumeroprocesso = '{$numeroProcesso}'";
        return $sql;
    }

    public function listarEmpenhoObra($numeroProcesso)
    {
        $sql = "select epc.* from par3.empenhoobracomposicao epc
                inner join par3.empenho emp ON emp.empid = epc.empid
                where emp.empnumeroprocesso = '{$numeroProcesso}'";

        return $sql;
    }
    public function listarPagamento($numeroProcesso)
    {
        $sql = "Select pag.pagid, pag.pagparcela, pag.paganoexercicio, pag.empid, pag.pagstatus from par3.pagamento pag
        inner join par3.empenho emp ON emp.empid = pag.empid
        where emp.empnumeroprocesso = '{$numeroProcesso}'";

        return $sql;
    }

    public function listarPagamentoParComposicao($numeroProcesso)
    {
        $sql = "select poc.* from par3.pagamentoparcomposicao poc
        inner join par3.pagamento pag ON pag.pagid = poc.pagid
        inner join par3.empenho emp ON emp.empid = pag.empid
        where emp.empnumeroprocesso = '{$numeroProcesso}'";

        return $sql;
    }

    public function listarPagamentoObraComposicao($numeroProcesso)
    {
        $sql = "select poc.*  from par3.pagamentoobracomposicao poc
        inner join par3.pagamento pag ON pag.pagid = poc.pagid
        inner join par3.empenho emp ON emp.empid = pag.empid
        where emp.empnumeroprocesso = '{$numeroProcesso}'";

        return $sql;
    }

    /**
     * Função que monta o sql para edição do par3.processo
     * @param $arrParams
     * @return string
     */
    public function editarProcesso($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.processo
                SET  $setUpdate
                where proid = '{$arrParams['proid']}'";
        return $sql;
    }

    /**
     * Função que monta o sql para edição da processoparcomposicao
     * @param $arrParams
     * @return string
     */
    public function editarComposicaoPar($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.processoparcomposicao
                SET  $setUpdate
                where ppcid = '{$arrParams['ppcid']}'";
        return $sql;
    }

    /**
     * Função que monta o sql para edição da processoobracomposicao
     * @param $arrParams
     * @return string
     */
    public function editarComposicaoObra($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.processoobracomposicao
                SET  $setUpdate
                where proid = '{$arrParams['proid']}'";

        return $sql;
    }

    /**
     * Função que monta o sql para edição da par3.analise
     * @param $arrParams
     * @return string
     */
    public function editarAnalisePar($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.analise
                SET $setUpdate
                where anaid = '{$arrParams['anaid']}'";

        return $sql;
    }

    /**
     * Função que monta o sql para edição da par3.obra
     * @param $arrParams
     * @return string
     */
    public function editarAnaliseObra($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.obra
                SET $setUpdate
                where obrid = '{$arrParams['obrid']}'";

        return $sql;
    }

    /**
     * Função que monta o sql para edição do par3.documentotermo
     * @param $arrParams
     * @return string
     */
    public function editarDocumento($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.documentotermo
                SET $setUpdate
                where dotid = {$arrParams['dotid']}";

        return $sql;
    }

    /**
     * Função que monta o sql para edição do par3.empenho
     * @param $arrParams
     * @return string
     */
    public function editarEmpenho($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.empenho
                SET $setUpdate
                where empid = '{$arrParams['empid']}'";

        return $sql;
    }

    /**
     * Função que monta o sql para edição do par3.empenhoparcomposicao
     * @param $arrParams
     * @return string
     */
    public function editarEmpenhoPar($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.empenhoparcomposicao
                SET $setUpdate
                where epcid = '{$arrParams['epcid']}'";

        return $sql;
    }

    /**
     * Função que monta o sql para edição do par3.empenhoobracomposicao
     * @param $arrParams
     * @return string
     */
    public function editarEmpenhoObra($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.empenhoobracomposicao
                SET $setUpdate
                where eocid = '{$arrParams['eocid']}'";

        return $sql;
    }

    /**
     * Função que monta o sql para edição do par3.pagamento
     * @param $arrParams
     * @return string
     */

    public function editarPagamento($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.pagamento
                SET $setUpdate
                where pagid = '{$arrParams['pagid']}'";

        return $sql;
    }

    /**
     * Função que monta o sql para edição do par3.pagamentoparcomposicao
     * @param $arrParams
     * @return string
     */
    public function editarPagamentoPar($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.pagamentoparcomposicao
                SET $setUpdate
                where ppcid = '{$arrParams['ppcid']}'";

        return $sql;
    }

    /**
     * Função que monta o sql para edição do par3.pagamentoobracomposicao
     * @param $arrParams
     * @return string
     */
    public function editarPagamentoObra($arrParams)
    {
        //Monta o set do update
        $setUpdate = $this->montaUpdate($arrParams);

        $sql = "UPDATE par3.pagamentoobracomposicao
                SET $setUpdate
                where pmcid = '{$arrParams['pmcid']}'";

        return $sql;
    }


    private function montarWhere($arrFiltros)
    {
        $where = [];
        if ($arrFiltros['proid']) {
            $where[] = "p.proid='{$arrFiltros['proid']}'";
        }

        if ($arrFiltros['pronumeroprocesso']) {
            $arrFiltros['pronumeroprocesso'] = preg_replace("/[^0-9\s]/", "", $arrFiltros['pronumeroprocesso']);
            $arrFiltros['pronumeroprocesso'] = trim($arrFiltros['pronumeroprocesso']);
            $where[] = "pronumeroprocesso ='{$arrFiltros['pronumeroprocesso']}'";
        }

        if ($arrFiltros['inpid']) {
            $where[] = "inpid='{$arrFiltros['inpid']}'";
        }

        if($arrFiltros['tipo'] == 'P'){
            $where[] = "protipo='O'";
        }else{
            $where[] = "protipo='P'";
        }

        return implode(' AND ', $where);
    }

    /**
     * Função que monta os campos para da update
     * @param $arrData
     * @return string
     */
    private function montaUpdate($arrData)
    {
        unset($arrData['oper']);
        //Filtrando o array, tirando elemento com valor vazio.
        $arrData = array_filter($arrData);
        $setUpdate = [];
        //Varrendo o $arrData e motando o array com cada parametro e o valor
        foreach ($arrData as $parametro => $valor) {
            $setUpdate[] = $parametro .'='. "'$valor'";
        }

        return implode(' , ', $setUpdate);
    }
}

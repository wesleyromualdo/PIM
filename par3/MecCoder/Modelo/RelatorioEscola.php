<?php
namespace Simec\Par3\Modelo;

/**
 * Description of RelatorioEscolaModel
 *
 * @author calixto
 */
class RelatorioEscola extends \Simec\AbstractModelo
{

    /**
     * Monta o comando SQL para realizar a pesquisa de escolas
     * @param array $params
     * @return array
     */
    protected function montarSqlDadosPesquisa($params)
    {
        $params['pagina'] = isset($params['pagina']) ? (integer) $params['pagina'] : 1;

        $whereInpid = '';
        $planejamento = '';
        $joinPlanejamento = '';
        if (!empty($params['planejamento'])) {
            if (filter_var($params['inpid'],FILTER_VALIDATE_INT)) {
                $whereInpid =  "and ipi.inpid = {$params['inpid']} and ipi.ipistatus = 'A'";
            }

        }

        $JoinInp = 'left';
        if($params['planejamento'] == 'possui') {
            $JoinInp = 'inner';
        }

        $joinPlanejamento = <<<JOINPLANEJAMENTO
                {$JoinInp} join par3.iniciativa_planejamento_item_composicao_escola  ipe 
                    on (ipe.escid = esc.escid AND ipestatus = 'A') and (ipequantidade > 0)
                {$JoinInp} join par3.iniciativa_planejamento_item_composicao ipi on ipi.ipiid = ipe.ipiid and (ipi.ipiquantidade > 0 or ipi.ipiquantidadeaprovada > 0) and ipi.ipistatus = 'A'
                LEFT JOIN par3.execucao_monitoramento_escola eme ON eme.ipeid = ipe.ipeid AND eme.emestatus = 'A' and emeqtd > 0
JOINPLANEJAMENTO;

        $filtros = [
            'itrid' => "and escno_esfera = '%s' ",
            'estuf' => "and iu.estuf in (%s) ",
            'mundescricao' => "and iu.muncod in (%s) ",
            'esdid' => "and escno_situacao_funcionamento in (%s) ",
            'escno_localizacao' => "and escno_localizacao = '%s' ",
            'inep'  => "and esccodinep = '%s' ",
        ];

        $where = "where 1=1 ";
        $isFiltered = false;
        foreach ($params as $param => $valor) {
            if (isset($params[$param]) && !empty($params[$param])) {
                //$valor = $this->antiInjection($valor);
                $where .= sprintf($filtros[$param], $valor);
                $isFiltered = true;
            }
        }
        switch ($params['itrid']) {
            case 'Municipal':
                $joinInstrumentoUnidade = "inner join par3.instrumentounidade iu on iu.muncod = esc.muncod or iu.estuf = esc.estuf";
                break;
            case 'Estadual':
                $joinInstrumentoUnidade = "inner join par3.instrumentounidade iu on iu.estuf = esc.estuf";
                break;
            default:
                $joinInstrumentoUnidade = <<<join
                inner join par3.instrumentounidade iu 
                    on (iu.muncod = esc.muncod)
join;
                break;
        }
        switch ($params['planejamento'] ) {
            case 'possui':
                 $where .= $whereInpid;
                break;
            // Not int
            case 'nao possui':
                $where .= " and ipe.ipeid is null ";
                break;
            default:
                break;
        }

        $pronumeroprocesso = trim(str_replace(['/','-','.'],['','',''],$params['pronumeroprocesso']));
        $dotnumero = preg_match('/\d-{1}\d/',$params['dotnumero'])?$params['dotnumero']:'';
        $joinProcesso = '';
        $campoPronumeroprocesso = '';

        $whereAnd = '';
        if(
            !empty($pronumeroprocesso) && filter_var($pronumeroprocesso,FILTER_VALIDATE_INT) ||
            !empty($dotnumero)
        ) {
            $joinPlanejamentoProcesso = $joinPlanejamento;
            unset($joinPlanejamento);
            $joinProcesso = <<<JOINPROCESSO
            {$joinPlanejamentoProcesso}
            INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano AND a.anastatus = 'A'
            INNER JOIN par3.processoparcomposicao ppc ON ppc.anaid = a.anaid AND ppcstatus = 'A'
            INNER JOIN par3.processo pro ON pro.proid = ppc.proid AND pro.prostatus = 'A'
JOINPROCESSO;
            $whereAnd =<<<WHEREAND
               AND (
                    ipequantidade > 0
                    OR eme.emeid IS NOT NULL
                    or eme.emeqtd > 0
               )
WHEREAND;

            if(!empty($pronumeroprocesso) && filter_var($pronumeroprocesso,FILTER_VALIDATE_INT)) {
                $where .= " and pro.pronumeroprocesso = '{$pronumeroprocesso}'";
            }

            if(!empty($dotnumero)) {
                $joinProcesso .= <<<JOINTERMO
                LEFT JOIN (
                    SELECT 
                        d.proid,
                        (d.dotnumero || '-'::text) || d.intoid AS dotnumero,
                        d.dotdatafimvigencia AS data_vigencia,
                        sum(d.dotvalortermo) AS dotvalortermo
                    FROM par3.documentotermo d WHERE d.dotstatus IN ('A', 'B')
                    GROUP BY d.proid, d.dotnumero, d.intoid, d.dotdatafimvigencia
                ) dt ON dt.proid = pro.proid
JOINTERMO;
                $where .= " and dt.dotnumero = '{$dotnumero}'";
            }
        }

        if (isset($params['estuf']) && preg_match('/DF/', $params['estuf'])) {
            $joinInstrumentoUnidade = str_replace(
                'iu.muncod = esc.muncod',
                'iu.estuf = esc.estuf',
                $joinInstrumentoUnidade
            );
        }
        if (!empty($params['nome'])) {
            $where .= "and escnome ilike '%{$params['nome']}%' ";
        }

        $sql = <<<sql
             SELECT distinct
                 esccodinep as "inep",  
                 iu.inudescricao as "municipio",
                 iu.estuf as "uf",
                 iu.muncod as "ibge",
                 escno_esfera as "esfera",
                 escnome as "nome",
                 escno_situacao_funcionamento as "situacao",
                 escno_localizacao as "localizacao",
                 escqtd_salas_utilizadas as "salasUtilizadas",
                 escqtd_alunos as "totalAlunos",
                 escno_situacao_imovel as "imovel",
                 escendereco as "endereco",
                 escendereco_cep as "cep",
                 esccoordenadas_latitude || ' / '|| esccoordenadas_longitude as "coordenadas",
                 escqtd_salas as "totalDeSalas",
                 escqtd_alunos_infantil as "alunosInfantil",
                 escqtd_alunos_fundamental as "alunosFundamental",
                 escqtd_alunos_medio as "alunosMedio",
                 esc.escid as "escid"
             FROM 
                par3.escola esc
                inner join par3.instrumentounidade iu on iu.muncod = esc.muncod
                {$joinPlanejamento}
                {$joinProcesso}
            {$where}
            {$whereAnd}
            ORDER BY
                uf, municipio, inep
sql;
        return ['sql' => $sql, 'isFiltered' => $isFiltered];
    }

    /**
     * Realiza a execução do comando de pesquisa de escolas no banco
     * @param array $params
     */
    public function executarLeituraTodasEscolas($params)
    {
        $this->executar($this->montarSqlDadosPesquisa($params)['sql']);
    }

    /**
     * Realiza a retirada da próxima tupla de escola da pesquisa executada
     * @return array
     */
    public function pegarProximaEscola()
    {
        return $this->proximo();
    }

    /**
     * Realiza a pesquisa das escolas e retorna os dados paginados
     * @param array $params
     * @return array
     */
    public function lerPaginadoEscolas($params)
    {
        $arSql = $this->montarSqlDadosPesquisa($params);
        $result = [];
        $tamanho = 50;
        $fim = (($params['pagina'] - 1) * $tamanho);
        $sqlPaginado = <<<sql
                    select * from ({$arSql['sql']}) selecao limit {$tamanho} offset {$fim}
sql;
        if ($params['pagina'] == 1) {
            $total = $this->pegaUmObjeto(<<<sql
                        select count(1) quantidade from ({$arSql['sql']}) selecao
sql
            );
            $result['totalRegistros'] = $total->quantidade;
            $result['tamanhoPagina'] = $tamanho;
        }
        $result['sql'] = $sqlPaginado;
        $result['dados'] = $this->carregar($sqlPaginado);
        return $result;
    }

    /**
     * Realiza a seleção das situações existentes nas escolas
     * @return array
     */
    public function situacaoEscola()
    {
        $res = $this->carregar(<<<sql
            select distinct escno_situacao_funcionamento from par3.escola esc
sql
        );
        $itens = [];
        foreach ($res as $item) {
            $itens[] = [
                'valor' => $item['escno_situacao_funcionamento'],
                'descricao' => $item['escno_situacao_funcionamento'],
            ];
        }
        return $itens;
    }

    /**
     * Realiza a seleção dos itens destinados à uma escola pelo identificador
     * @param integer $escid
     * @return array
     */
    public function lerItensDestinados($escid)
    {
        $escid = (integer) $escid;
        $sql = <<<sql
 SELECT  
     inp.inpid AS planejamento, 
     ini.iniid || ' - ' || inid.inddsc as "iniciativa",
     ipi.ipiano "ano",
     COALESCE (itc.itcdsc,
     icg.igrnome) AS item,
     ipequantidade AS "qtdPlanejada",
     emeqtd AS "qtdMonitorada",
     aicqtdaprovado,
     quantidade_emenda,
	 COALESCE (aic.aicvaloraprovado, ipi.ipivalorreferencia) as "valor",
     COALESCE (aic.aicvaloraprovado, ipi.ipivalorreferencia)*ipequantidade as "valorTotal",
	 pro.pronumeroprocesso AS "processo",
	 dot.dotnumero||'-'||dot.intoid AS "termo",
     COALESCE (eda.esddsc, '-') AS "situacaoPlanejamento",
    CASE WHEN a.intaid = 1
        THEN 'PAR'
    ELSE
        CASE WHEN a.intaid = 2
        THEN'Emendas'
        ELSE '-' END
    END as tipoAssistencia,
    a.anaid
FROM par3.iniciativa_planejamento_item_composicao_escola ipe
    INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.ipiid = ipe.ipiid
    INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = ipi.inpid
    INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
    left JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
    INNER JOIN PAR3.iniciativa ini ON ini.iniid = inp.iniid
    INNER JOIN PAR3.iniciativa_descricao inid ON inid.indid = ini.indid 
    INNER JOIN par3.escola esc ON esc.escid = ipe.escid
    INNER JOIN par3.instrumentounidade iu ON iu.muncod = esc.muncod
    LEFT JOIN par3.analise a ON a.inpid = inp.inpid AND a.anaano = ipi.ipiano AND a.anastatus = 'A' --> ver
    LEFT JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid AND aic.anaid = a.anaid and aicstatus = 'A'
    LEFT JOIN par3.itenscomposicao_grupos icg USING (igrid)
    LEFT JOIN par3.itenscomposicao_grupos_itens icgi USING (igrid)
    LEFT JOIN workflow.documento doca ON doca.docid = a.docid
    LEFT JOIN workflow.estadodocumento eda ON doca.esdid = eda.esdid 
    LEFT JOIN par3.processoparcomposicao ppc ON ppc.anaid = a.anaid AND ppcstatus = 'A'
    LEFT JOIN par3.processo pro ON pro.proid = ppc.proid AND pro.prostatus = 'A'
    LEFT JOIN par3.documentotermo dot ON dot.proid = pro.proid AND dot.dotstatus = 'A' 
    LEFT JOIN par3.execucao_contratoitens eci ON eci.aicid = aic.aicid
    LEFT JOIN par3.execucao_monitoramento_escola eme ON eme.ipeid = ipe.ipeid AND eme.emestatus = 'A'
    LEFT JOIN(
        select
            inpid,
            ipeid,
            sum(ieequantidade) AS quantidade_emenda,
            anaid
        FROM (
            SELECT distinct
                ie.inpid,
                eic.ipeid,
                eic.ieequantidade,
                ana.anaid
            FROM par3.iniciativa_emenda ie
                INNER JOIN par3.iniciativa_emenda_item_composicao_escola eic ON eic.ineid = ie.ineid
                INNER JOIN par3.analise ana ON ana.edeid = ie.edeid AND ana.anastatus = 'A'
            WHERE ie.inestatus = 'A'
            AND eic.ieestatus = 'A'
        ) AS foo
        GROUP BY inpid, ipeid,anaid
    ) iec ON iec.inpid = inp.inpid AND ipe.ipeid = iec.ipeid and iec.anaid = a.anaid
	WHERE 
  		ipe.escid = {$escid}
  		AND inpstatus = 'A'
      	AND ipistatus = 'A'
      	AND ipe.ipestatus = 'A'
       	AND (	
        	ipequantidade > 0
        	OR eme.emeid IS NOT NULL
        	or eme.emeqtd > 0
       )
union all
	SELECT DISTINCT
	     inp.inpid AS planejamento, 
	     ini.iniid || ' - ' || inid.inddsc as "iniciativa",
	     ipi.ipiano ano,
	     COALESCE (itc.itcdsc,
	   	 icg.igrnome) AS item,
	     0 AS "qtdPlanejada",
	     emeqtd AS "qtdMonitorada",
         aicqtdaprovado,
         quantidade_emenda,
	     COALESCE (aic.aicvaloraprovado, ipi.ipivalorreferencia) as "valor",
	     0 as "valorTotal",
	     pro.pronumeroprocesso AS "processo",
	     dot.dotnumero||'-'||dot.intoid AS "termo",
		COALESCE (eda.esddsc, '-') AS "situacaoPlanejamento",
	    CASE WHEN a.intaid = 1
	        THEN 'PAR'
	    ELSE
	        CASE WHEN a.intaid = 2
	        THEN'Emendas'
	        ELSE '-' END
	    END as tipoAssistencia,
        a.anaid
	FROM par3.execucao_contratoitens eci
	INNER JOIN par3.execucao_monitoramento_escola eme ON eci.eciid = eme.eciid
	INNER JOIN par3.execucao_contrato eco ON eco.ecoid = eci.ecoid
	INNER JOIN par3.analise_itemcomposicao aic ON aic.aicid = eci.aicid AND aicstatus = 'A'
	INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.ipiid = aic.ipiid
	INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
	LEFT JOIN par3.itenscomposicao_grupos icg USING (igrid)
	LEFT JOIN par3.itenscomposicao_grupos_itens icgi USING (igrid)
	INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
	INNER JOIN par3.escola esc ON esc.escid = eme.escid
	LEFT JOIN par3.processoparcomposicao ppc ON ppc.anaid = aic.anaid AND ppcstatus = 'A'
	LEFT JOIN par3.analise a ON aic.anaid = a.anaid AND a.anastatus = 'A' --> ver
	LEFT JOIN workflow.documento doca ON doca.docid = a.docid
	LEFT JOIN workflow.estadodocumento eda ON doca.esdid = eda.esdid 
	INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = a.inpid
	INNER JOIN PAR3.iniciativa ini ON ini.iniid = inp.iniid
	INNER JOIN PAR3.iniciativa_descricao inid ON inid.indid = ini.indid 
	LEFT JOIN par3.processo pro ON pro.proid = ppc.proid AND pro.prostatus = 'A'
	LEFT JOIN par3.documentotermo dot ON dot.proid = pro.proid AND dot.dotstatus = 'A'
    LEFT JOIN(
        select
            inpid,
            ipeid,
            sum(ieequantidade) AS quantidade_emenda,
            anaid
        FROM (
            SELECT distinct
                ie.inpid,
                eic.ipeid,
                eic.ieequantidade,
                ana.anaid
            FROM par3.iniciativa_emenda ie
                INNER JOIN par3.iniciativa_emenda_item_composicao_escola eic ON eic.ineid = ie.ineid
                INNER JOIN par3.analise ana ON ana.edeid = ie.edeid AND ana.anastatus = 'A'
            WHERE ie.inestatus = 'A'
            AND eic.ieestatus = 'A'
        ) AS foo
        GROUP BY inpid, ipeid,anaid
    ) iec ON iec.inpid = inp.inpid AND eme.ipeid = iec.ipeid and iec.anaid = a.anaid
	where esc.escid = {$escid}
	    ORDER BY 
	        processo,termo,item
sql;
//        ver($sql,d);
        $res = [
            'sql' => $sql,
            'dados' => []
        ];
        $this->executar($sql);
        while (($tupla = $this->proximo())) {
            $tupla['processo'] = \Simec\Par3\Dado\Processo::formatar($tupla['processo']);
            $res['dados'][] = $tupla;
        }
        return $res;
    }

    public function opcoesEsfera()
    {
        return [
            ['valor' => 1, 'descricao' => 'Estadual'],
            ['valor' => 2, 'descricao' => 'Municipal'],
            ['valor' => 3, 'descricao' => 'Todos'],
        ];
    }

    public function opcoesPlanejamento()
    {
        return [
            ['valor' => 1, 'descricao' => 'Sim'],
            ['valor' => 2, 'descricao' => 'Não'],
            ['valor' => 3, 'descricao' => 'Todos'],
        ];
    }

    public function opcoesMonitoramento()
    {
        return [
            ['valor' => 1, 'descricao' => 'Sim'],
            ['valor' => 2, 'descricao' => 'Não'],
            ['valor' => 3, 'descricao' => 'Todos'],
        ];
    }

    public function opcoesVisiveis()
    {
        return [
            ['valor' => 1, 'descricao' => 'Imóvel'],
            ['valor' => 2, 'descricao' => 'Endereço'],
            ['valor' => 3, 'descricao' => 'CEP'],
            ['valor' => 4, 'descricao' => 'Coordenadas'],
            ['valor' => 5, 'descricao' => 'Total de Salas'],
            ['valor' => 6, 'descricao' => 'Alunos Infantil'],
            ['valor' => 7, 'descricao' => 'Alunos Fundamental'],
            ['valor' => 8, 'descricao' => 'Alunos Médio'],
        ];
    }
}

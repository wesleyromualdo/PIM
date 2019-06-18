<?php
/**
 * Classe de mapeamento da entidade par3.pagamento.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_Pagamento: sem descricao
 *
 * @package Model\Pagamento
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Pagamento($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Pagamento($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $pagid  - default: nextval('par3.pagamento_pagid_seq'::regclass)
 * @property numeric $pagparcela 
 * @property numeric $paganoexercicio 
 * @property numeric $pagvalorparcela 
 * @property numeric $paganoparcela 
 * @property numeric $pagmes 
 * @property string $pagnumeroempenho 
 * @property int $empid 
 * @property string $usucpf 
 * @property string $pagsituacaopagamento 
 * @property \Datetime(Y-m-d H:i:s) $pagdatapagamento 
 * @property int $pagnumseqob 
 * @property string $pagstatus  - default: 'A'::bpchar
 * @property \Datetime(Y-m-d H:i:s) $pagdatapagamentosiafi 
 * @property string $pagnumeroob 
 * @property string $pagcarga  - default: 'N'::bpchar
 * @property string $pagverificacarga  - default: 'A'::bpchar
 * @property string $pagusucpfexclusao 
 * @property \Datetime(Y-m-d H:i:s) $pagdataexclusao 
 * @property \Datetime(Y-m-d H:i:s) $pagdataatualizacao 
 * @property string $pagjustificaitiva 
 */
class Par3_Model_Pagamento extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.pagamento';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'pagid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'empid' => array('tabela' => 'par3.empenho', 'pk' => 'empid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'pagid' => null,
        'pagparcela' => null,
        'paganoexercicio' => null,
        'pagvalorparcela' => null,
        'paganoparcela' => null,
        'pagmes' => null,
        'pagnumeroempenho' => null,
        'empid' => null,
        'usucpf' => null,
        'pagsituacaopagamento' => null,
        'pagdatapagamento' => null,
        'pagnumseqob' => null,
        'pagstatus' => null,
        'pagdatapagamentosiafi' => null,
        'pagnumeroob' => null,
        'pagcarga' => null,
        'pagverificacarga' => null,
        'pagusucpfexclusao' => null,
        'pagdataexclusao' => null,
        'pagdataatualizacao' => null,
        'pagjustificaitiva' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'pagid' => array('Digits'),
            'pagparcela' => array(),
            'paganoexercicio' => array(),
            'pagvalorparcela' => array(),
            'paganoparcela' => array(),
            'pagmes' => array(),
            'pagnumeroempenho' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'empid' => array('Digits'),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'pagsituacaopagamento' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 40))),
            'pagdatapagamento' => array(),
            'pagnumseqob' => array('allowEmpty' => true,'Digits'),
            'pagstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'pagdatapagamentosiafi' => array('allowEmpty' => true),
            'pagnumeroob' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 12))),
            'pagcarga' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'pagverificacarga' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'pagusucpfexclusao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'pagdataexclusao' => array('allowEmpty' => true),
            'pagdataatualizacao' => array('allowEmpty' => true),
            'pagjustificaitiva' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function sqlSituacaoGrafico($arrParams = null)
    {
        $where = '';
        if($arrParams) {
            if (is_array($arrParams['intaidpagamento'])) {
                $arInta = implode(',',$arrParams['intaidpagamento']);
                $where .= " AND p.intaid in ({$arInta}) ";
            }
        }

        $sql = <<<SQL
        SELECT        
            CASE WHEN pag.pagsituacaopagamento IS NOT NULL THEN pag.pagsituacaopagamento ELSE 'Sem Pagamento' END AS descricao,
            count(*) as valor,
            sum(coalesce(e.vlrpago,0))::NUMERIC(20,2) as  valor_monetario
        FROM par3.processo p
            INNER JOIN(
                SELECT pp.proid, sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa, a.inpid,'P' as tipo_processo
                FROM par3.iniciativa_planejamento_item_composicao ipi
                    INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
                    INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
                    INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid
                WHERE ipi.ipistatus = 'A' AND pp.ppcstatus = 'A'  GROUP BY pp.proid, a.inpid
                UNION ALL
                SELECT po.proid, sum(o.obrvalor) AS vlriniciativa, o.inpid, 'O' as tipo_processo
                FROM par3.obra o
                    INNER JOIN par3.processoobracomposicao po ON po.obrid = o.obrid AND po.pocstatus = 'A'
                WHERE o.obrstatus = 'A'  GROUP BY po.proid, o.inpid
            )pp ON pp.proid = p.proid
            inner join par3.v_painel_saldo_empenho_pagamento e on e.processo = p.pronumeroprocesso
            inner join par3.iniciativa_planejamento ip on ip.inpid = pp.inpid
            INNER JOIN par3.vm_situacao_empenho_pagamento_processo pag ON pag.empnumeroprocesso = p.pronumeroprocesso
        WHERE
            ip.inpstatus = 'A'
        {$where}
        GROUP BY  descricao
SQL;
        return $sql;
    }

    public function sqlEvolucaoPagamentos($tipo,$arParams = null)
    {
        $group = '';
        $where  = " WHERE 1=1";
        $column = '';
        switch ($tipo) {
            case 1: $group = " ,foo1.tipo,foo1.tipo_label,mes_ano_termo";$column = ",tipo,tipo_label,mes_ano_termo";break;
            case 2: $group = '';$column = "";$where .= " AND tipo_label = 'empenho_efetivado' ";break;
            case 3: $group = '';$column = "";$where .= " AND tipo_label = 'pagamento_efetivado' ";break;
        }

        if($arParams['data-inicio'] || $arParams['data-fim']) {
            if( $arParams['data-inicio'] && $arParams['data-fim']) {
                $where .= " AND mes_ano_termo between '01-{$arParams['data-inicio']}' AND '01-{$arParams['data-fim']}' ";
            }
        }

        $whereAno = '';
        if($arParams['ano']) {
            $whereAno .= "date_part('year', dtcarga) = '{$arParams['ano']}' ";
        }

        $limit = $arParams['limit'] ? " limit {$arParams['limit']} " : '';
        $sql = "
            select
               mes,
               ano,
               sum(qtd) as qtd,
               sum(valor) as valor
               {$column}
            FROM(
                select
                mes,
                ano,
                count(proid) AS qtd,
                sum(valor) AS valor,
                '1 - Termos gerados' as tipo,
                'termo_gerado' as tipo_label,
                to_date('0'||mes||'/'||ano, '	MM/YYYY') as mes_ano_termo
                FROM(
                    select
                    proid, 
                    (dotvalortermo)::numeric(20,2) AS valor,
                    date_part('month', dotdatainclusao)::integer AS mes,
                    date_part('year', dotdatainclusao)::numeric AS ano
                    FROM par3.documentotermo
                    WHERE dotstatus = 'A' or dotstatus = 'B'
                ) AS foo
                GROUP BY mes, ano
                UNION ALL
                select
                    mes,
                    ano,
                    count(proid) AS qtd,
                    sum(valor) AS valor,
                    '2 - Termos validados' as tipo,
                    'termo_validado' as tipo_label,
                    to_date('0'||mes||'/'||ano, '	MM/YYYY') as mes_ano_termo
                FROM(
                    select
                    proid,
                    (dotvalortermo)::numeric(20,2) AS valor,
                    date_part('month', dotdatainclusao)::integer AS mes,
                    date_part('year', dotdatainclusao)::numeric AS ano
                    FROM par3.documentotermo
                    WHERE dotstatus = 'A'
                    AND dotid IN (SELECT dotid FROM par3.documentotermovalidacao WHERE dtvstatus = 'A')
                ) AS foo
                GROUP BY mes, ano
            UNION ALL        
                select
                    mes,
                    ano,
                    count(proid) AS qtd,
                    sum(valor) AS valor,
                    '3 - Empenhos efetivados' as tipo,
                    'empenho_efetivado' as tipo_label,
                    to_date('0'||mes||'/'||ano, '	MM/YYYY') as mes_ano_termo
                FROM(
                        SELECT 
                            proid,
                            (vlrempenho) AS valor,
                            date_part('month', empdata)::integer AS mes,
                            date_part('year', empdata)::numeric AS ano
                        FROM par3.v_painel_empenho_composicao_iniciativa WHERE empsituacao ILIKE '%efetivado%'
                    UNION ALL        
                        select
                            proid,
                            (vlrempenho) AS valor,
                            date_part('month', empdata)::integer AS mes,
                            date_part('year', empdata)::numeric AS ano
                        FROM par3.v_painel_empenho_composicao_obra WHERE empsituacao ILIKE '%efetivado%'
                ) AS foo
                GROUP BY mes, ano
                UNION ALL
                select
                    mes,
                    ano,
                    count(processo) AS qtd,
                    sum(valor) AS valor,
                    '4 - Pagamentos efetivados' AS tipo,
                    'pagamento_efetivado' as tipo_label,
                    to_date('0'||mes||'/'||ano, '	MM/YYYY') as mes_ano_termo
                FROM(
                    select
                        e.empnumeroprocesso AS processo,
                        (p.pagvalorparcela) AS valor,
                        date_part('month', p.pagdatapagamento)::integer AS mes,
                        date_part('year', p.pagdatapagamento)::numeric AS ano
                    FROM par3.empenho e
                    INNER JOIN par3.pagamento p ON p.empid = e.empid
                    WHERE e.empstatus = 'A'
                    AND p.pagsituacaopagamento ILIKE '%efetivado%'
                ) AS foo
                GROUP BY mes, ano,mes_ano_termo
            ) AS foo1
            {$where}
            group by 
                foo1.ano,
                foo1.mes
                {$group}
            ORDER BY  
                foo1.ano,
                foo1.mes
                {$limit}
        ";
        return $sql;
    }

    public function     sqlPagamentoPorRegiao($arrFiltros = null,$tipoFiltro = null)
    {
//        $date = $arrFiltros['data'] ? "'{$arrFiltros['data']}'" : "date_part('year', now())";
//        $dateWhere = " where date_part('year', data_pagamento) = {$date} ";
        $arGroup  = [];
        $selectEstado = [
            'est.estuf',
            'reg.regcod',
            'reg.regdescricao',
            'est.estdescricao'
        ];
        $withValorTotal = <<<SQLVALORTOTAL
        with valor_total as (
            SELECT
                sum(vlr_pg_efetivado) AS total_pagamento,
                sum(vlrempenho) as total_empenho
            FROM par3.v_painel_saldo_empenho_pagamento vpag
            INNER JOIN par3.processo pro ON pro.pronumeroprocesso = vpag.processo
            INNER JOIN par3.instrumentounidade inu ON inu.inuid = pro.inuid
            INNER JOIN territorios.estado est ON est.estuf = inu.estuf
            INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod
--             where date_part('year', data_pagamento) = date_part('year', now()) 
        ) 
SQLVALORTOTAL;

        $selectRegiao = ['reg.regcod','reg.regdescricao'];
        switch($tipoFiltro) {
            case 1 : $arSelect = $selectEstado;break;
            case 2 : $arSelect = $selectRegiao;break;
            default: $arSelect = $selectRegiao;break;
        }

        $group  = implode(',',$arSelect);
        $select = implode(',',$arSelect);
        $sql =<<<SQL
        {$withValorTotal} 
        SELECT
            $select,
            count(pro.proid) AS valor,
            sum(vlr_pg_efetivado) AS vlr_pagamento,
            sum(vlrempenho) as vlr_empenho,
            ((sum(vlr_pg_efetivado)/(select total_pagamento from valor_total))*100)::numeric(20,2) as percent_pagamento,
            ((sum(vlrempenho)/(select total_empenho from valor_total))*100)::numeric(20,2) as percent_empenho
        FROM par3.v_painel_saldo_empenho_pagamento vpag
        INNER JOIN par3.processo pro ON pro.pronumeroprocesso = vpag.processo
        INNER JOIN par3.instrumentounidade inu ON inu.inuid = pro.inuid
        INNER JOIN territorios.estado est ON est.estuf = inu.estuf
        INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod
        group by $group
SQL;
        return $sql;
    }
}

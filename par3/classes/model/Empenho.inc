<?php
/**
 * Classe de mapeamento da entidade par3.empenho.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_Empenho: Tabela de empenhos do PAR
 *
 * @package Model\Empenho
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Empenho($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Empenho($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $empid Chave primaria da tabela par3.empenho - default: nextval('par3.empenho_empid_seq'::regclass)
 * @property string $empcnpj CNPJ do favorecido
 * @property numeric $empnumerooriginal Numero do empenho original
 * @property numeric $empanooriginal Ano do empenho original
 * @property string $empnumeroprocesso Número do processo do empenho
 * @property string $empcodigoespecie clasificação do empenho
 * @property string $empcodigopi Código do PI
 * @property numeric $empcodigoesfera Código da esfera orçamentaris 1- Fiscal , 2- Seguridade
 * @property string $empcodigoptres codigo PTRES
 * @property string $empfonterecurso código da Fonte de recurso
 * @property numeric $empcodigonatdespesa Código da natureza de despesa
 * @property string $empcentrogestaosolic Código de centro de gestão solicitante
 * @property numeric $empanoconvenio Ano do convênio
 * @property numeric $empnumeroconvenio número do convênio
 * @property string $empcodigoobs Codigo da observação do convênio
 * @property string $empcodigotipo tipo de empenho
 * @property string $empdescricao Descrição da empenho
 * @property string $empgestaoeminente Gestão Eminente
 * @property string $empunidgestoraeminente Codgio da unidade gestora
 * @property string $empprogramafnde Programa FNDE
 * @property string $empnumerosistema Codigo do Sistema
 * @property string $empsituacao Descrição da situação
 * @property string $usucpf Chave estrangeira da tabela seguranca.usuario
 * @property string $empprotocolo nr do protocolo
 * @property string $empnumero numero do empenho
 * @property numeric $empvalorempenho Valor do empenho
 * @property string $ds_problema Descrição do problema
 * @property numeric $valor_total_empenhado Valor empenhado
 * @property numeric $valor_saldo_pagamento Saldo do pagamento
 * @property \Datetime(Y-m-d H:i:s) $empdata Data do Empenho - default: now()
 * @property string $tp_especializacao Tipo de especialização
 * @property string $co_diretoria Codigo da diretoria
 * @property int $empidpai Liga a chave primária para vincular um empenho do tipo cancelamento, reforço, anulação e empenho parcial ao empenho original.
 * @property int $teeid cheve estrangeira da tabela execucaofinanceira.tipoespecieempenho
 * @property string $empcarga Indica se o empenho foi feito por carga - default: 'N'::bpchar
 * @property string $empstatus coluna utilizada para cancelar processo quando não estão no SIGEF - default: 'A'::bpchar
 * @property \Datetime(Y-m-d H:i:s) $empdataatualizacao Data de atualização do empenho pela rotina noturna
 */
class Par3_Model_Empenho extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.empenho';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'empid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'empid' => null,
        'empcnpj' => null,
        'empnumerooriginal' => null,
        'empanooriginal' => null,
        'empnumeroprocesso' => null,
        'empcodigoespecie' => null,
        'empcodigopi' => null,
        'empcodigoesfera' => null,
        'empcodigoptres' => null,
        'empfonterecurso' => null,
        'empcodigonatdespesa' => null,
        'empcentrogestaosolic' => null,
        'empanoconvenio' => null,
        'empnumeroconvenio' => null,
        'empcodigoobs' => null,
        'empcodigotipo' => null,
        'empdescricao' => null,
        'empgestaoeminente' => null,
        'empunidgestoraeminente' => null,
        'empprogramafnde' => null,
        'empnumerosistema' => null,
        'empsituacao' => null,
        'usucpf' => null,
        'empprotocolo' => null,
        'empnumero' => null,
        'empvalorempenho' => null,
        'ds_problema' => null,
        'valor_total_empenhado' => null,
        'valor_saldo_pagamento' => null,
        'empdata' => null,
        'tp_especializacao' => null,
        'co_diretoria' => null,
        'empidpai' => null,
        'teeid' => null,
        'empcarga' => null,
        'empstatus' => null,
        'empdataatualizacao' => null,
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
            'empid' => array('Digits'),
            'empcnpj' => array(new Zend_Validate_StringLength(array('max' => 15))),
            'empnumerooriginal' => array('allowEmpty' => true),
            'empanooriginal' => array('allowEmpty' => true),
            'empnumeroprocesso' => array(new Zend_Validate_StringLength(array('max' => 21))),
            'empcodigoespecie' => array(new Zend_Validate_StringLength(array('max' => 2))),
            'empcodigopi' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'empcodigoesfera' => array(),
            'empcodigoptres' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'empfonterecurso' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'empcodigonatdespesa' => array(),
            'empcentrogestaosolic' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'empanoconvenio' => array('allowEmpty' => true),
            'empnumeroconvenio' => array('allowEmpty' => true),
            'empcodigoobs' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'empcodigotipo' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'empdescricao' => array(new Zend_Validate_StringLength(array('max' => 4))),
            'empgestaoeminente' => array(new Zend_Validate_StringLength(array('max' => 6))),
            'empunidgestoraeminente' => array(new Zend_Validate_StringLength(array('max' => 10))),
            'empprogramafnde' => array(new Zend_Validate_StringLength(array('max' => 2))),
            'empnumerosistema' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'empsituacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 40))),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'empprotocolo' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'empnumero' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 20))),
            'empvalorempenho' => array('allowEmpty' => true),
            'ds_problema' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'valor_total_empenhado' => array('allowEmpty' => true),
            'valor_saldo_pagamento' => array('allowEmpty' => true),
            'empdata' => array('allowEmpty' => true),
            'tp_especializacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'co_diretoria' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 20))),
            'empidpai' => array('allowEmpty' => true,'Digits'),
            'teeid' => array('allowEmpty' => true,'Digits'),
            'empcarga' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'empstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'empdataatualizacao' => array('allowEmpty' => true),
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

    public function sqlGrafico($arrParams = null)
    {
        $where = '';
        if($arrParams) {
            if (is_array($arrParams['intaidempenho'])) {
                $arInta = implode(',',$arrParams['intaidempenho']);
                $where .= " AND pro.intaid in ({$arInta}) ";
            }
        }
        $sql = "
//        SELECT DISTINCT 
//        (
//            CASE WHEN emps.empsituacao is null 
//            then 'Sem empenho' else emps.empsituacao end
//        ) as descricao,
//        count(*) as valor,
//        sum(vlr_empenho) as valor_monetario
//        FROM par3.processo pro
//        left JOIN par3.vm_situacao_empenho_pagamento_processo emps ON emps.empnumeroprocesso = pro.pronumeroprocesso
//        left join par3.v_dados_pagamento vpag on pro.pronumeroprocesso = vpag.processo
//        {$where}
//        group by descricao
        ";
        $sql =<<<SQL
         SELECT DISTINCT 
            	CASE WHEN emps.empsituacao is null then 'Sem empenho' else emps.empsituacao end as descricao,
            	count(*) as valor,
            	sum(coalesce(vs.vlrempenho,0))::numeric(20,2) as valor_monetario
            FROM par3.processo pro
                INNER JOIN(
                  	SELECT 
                  	  pp.proid,
                  	  sum(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado) AS vlriniciativa,
                  	  pp.intoid,
                      'P' as tipo_processo
            	    FROM par3.iniciativa_planejamento_item_composicao ipi
            			INNER JOIN par3.analise a ON a.inpid = ipi.inpid AND a.anaano = ipi.ipiano
            			INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.ipiid = ipi.ipiid AND ai.aicstatus = 'A'::bpchar
            			INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = ipi.inpid and pp.ppcstatus = 'A'
            		WHERE ipi.ipistatus = 'A' 
            		GROUP BY pp.proid, pp.intoid
                  	UNION ALL
                     SELECT 
                        proid,
                        sum(vlr_obra) AS vlriniciativa,
                        intoid,
                        'O' as tipo_processo
                    FROM(
                     	SELECT pp.proid, (o.obrvalor - coalesce(o.obrvalor_contrapartida,0) ) AS vlr_obra, pp.intoid FROM par3.obra o
                     		INNER JOIN par3.processoobracomposicao pp ON pp.obrid = o.obrid AND pp.pocstatus = 'A'
                     	WHERE o.obrstatus = 'A' 
                     ) AS foo
                     GROUP BY proid, intoid
                 ) vip ON vip.proid = pro.proid   
                 left JOIN par3.vm_situacao_empenho_pagamento_processo emps ON emps.empnumeroprocesso = pro.pronumeroprocesso
                 LEFT JOIN par3.v_painel_saldo_empenho_pagamento vs ON vs.processo = pro.pronumeroprocesso
              WHERE pro.prostatus::text = 'A'
                 {$where}
                 group by descricao
SQL;

        return $sql;
    }
}

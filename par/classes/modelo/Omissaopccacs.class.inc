<?php
/**
 * Classe de mapeamento da entidade par.omissaopccacs.
 *
 * @version $Id$
 * @since 2019.01.17
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par_Model_Omissaopccacs: Tabela responsável por armazenar os dados da omissão do envio da Prestação de Contas por parte do cacs
 *
 * @package Par\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par_Model_Omissaopccacs($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par_Model_Omissaopccacs($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $prpid FK do processo par.processopar
 * @property int $inuid 
 * @property string $occcpfciencia CPF de quem declarou ciência
 * @property \Datetime(Y-m-d H:i:s) $occdataciencia Data da declaração de ciência
 * @property int $proid_par 
 * @property int $ropid 
 * @property int $occid Primary key da tabela - default: nextval('par.omissaopccacs_occid_seq'::regclass)
 * @property \Datetime(Y-m-d H:i:s) $occdataomissao Data de registro da Omissão - default: now()
 * @property bool $occciencia Declaração de ciência por parte do cacs - default: false
 * @property string $occstatus  - default: 'A'::bpchar
 */
class Par_Model_Omissaopccacs extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.omissaopccacs';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'occid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'prpid' => array('tabela' => 'par.processopar', 'pk' => 'prpid'),
        'proid_par' => array('tabela' => 'par.processoobraspar', 'pk' => 'proid'),
        'ropid' => array('tabela' => 'par.responsavelomissaopccacs', 'pk' => 'ropid'),
        'inuid' => array('tabela' => 'par.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'prpid' => null,
        'inuid' => null,
        'occcpfciencia' => null,
        'occdataciencia' => null,
        'proid_par' => null,
        'ropid' => null,
        'occid' => null,
        'occdataomissao' => null,
        'occciencia' => null,
        'occstatus' => null,
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
            'prpid' => array('allowEmpty' => true,'Digits'),
            'inuid' => array('allowEmpty' => true,'Digits'),
            'occcpfciencia' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'occdataciencia' => array('allowEmpty' => true),
            'proid_par' => array('allowEmpty' => true,'Digits'),
            'ropid' => array('allowEmpty' => true,'Digits'),
            'occid' => array('Digits'),
            'occdataomissao' => array('allowEmpty' => true),
            'occciencia' => array('allowEmpty' => true),
            'occstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
        );
    }
    
    /**
     * 
     * @param string $inuid
     * @param string $ropid: 1 = Gestor Unidade, 2 = Presidente CACS
     * @return boolean
     */
    public function verificaExistenciaOmissaoNaoAssinada($inuid = '', $ropid = '')
    {
        $sql = "SELECT
        occid
        FROM
        par.omissaopccacs opc
        WHERE
        inuid = {$inuid}
        AND ropid = {$ropid}
        AND occstatus = 'A'
        AND occciencia = FALSE
        ";
    
        $omissao = $this->pegaUm($sql);
        $omissao = ($omissao) ? TRUE : FALSE;
        return $omissao;
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
    
    /**
     */
    public function retornaListaProcessosComOmissao($inuid)
    {
        
        if ( DPCID_OMISSAO_CACS  ) {
           
            
            $datas = $this->pegaLinha("
                SELECT
                	dpcdatainiciocontagem1, dpcdatainiciocontagem2, dpcquantidadedias
                FROM
                	par.datasprestacaocontas
                WHERE
                	dpcid = ".DPCID_OMISSAO_CACS." ");
            
            
            $dpcdatainiciocontagem1 = $datas['dpcdatainiciocontagem1'];
            $dpcquantidadedias = $datas['dpcquantidadedias'];
        }
        
        
        if (($inuid != '') && ($dpcdatainiciocontagem1 != '') && ($dpcquantidadedias != '') ) {
           
            
            $sql = "
    
              SELECT DISTINCT
                    CASE WHEN prp.prpid IS NOT NULL THEN
        		'PAR'
                    WHEN pro_par.proid IS NOT NULL THEN
        		'OBRAS PAR'
                    END,
                    CASE WHEN prp.prpid IS NOT NULL THEN
        		    substring(prp.prpnumeroprocesso from 1 for 5)||'.'||
        		    substring(prp.prpnumeroprocesso from 6 for 6)||'/'||
        		    substring(prp.prpnumeroprocesso from 12 for 4)||'-'||
        		    substring(prp.prpnumeroprocesso from 16 for 2) || ''
                    WHEN pro_par.proid IS NOT NULL THEN
        		    substring(pro_par.pronumeroprocesso from 1 for 5)||'.'||
        		    substring(pro_par.pronumeroprocesso from 6 for 6)||'/'||
        		    substring(pro_par.pronumeroprocesso from 12 for 4)||'-'||
        		    substring(pro_par.pronumeroprocesso from 16 for 2) || ''            
                    END as processo,
                    CASE WHEN prp.prpid IS NOT NULL THEN
        		    substring(prpcnpj, 1, 2) || '.' || substring(prpcnpj, 3, 3) || '.' ||
        		    substring(prpcnpj, 6, 3) || '/' || substring(prpcnpj, 9, 4) || '-' ||
        		    substring(prpcnpj, 13)
                    WHEN pro_par.proid IS NOT NULL THEN
            
        		    substring(pro_par.procnpj, 1, 2) || '.' || substring(pro_par.procnpj, 3, 3) || '.' ||
        		    substring(pro_par.procnpj, 6, 3) || '/' || substring(pro_par.procnpj, 9, 4) || '-' ||
        		    substring(pro_par.procnpj, 13)
                    END as cnpj_entidade,
                    (SELECT ent.entnome FROM par.entidade ent WHERE ent.entstatus = 'A' AND ent.inuid = opc.inuid AND ent.dutid in( 9,6) limit 1) as nome_entidade,
            
                    CASE WHEN prp.prpid IS NOT NULL THEN
        		dop.dopnumerodocumento
                    WHEN pro_par.proid IS NOT NULL THEN
        		dop2.dopnumerodocumento
                    END as numero_termo,
            
                    CASE WHEN prp.prpid IS NOT NULL THEN
        		    coalesce( (
        		    select foo1.valor_pagamento FROM
        		    (
        		    Select
        		    coalesce(sum(pg.pagvalorparcela),0.00) as valor_pagamento
        		    From par.pagamento pg
        		    join par.empenho em on em.empid = pg.empid and empstatus = 'A' AND pg.pagstatus = 'A'
        		    where trim(pg.pagsituacaopagamento) ilike '%EFETIVADO%'
        		    AND em.empnumeroprocesso = prp.prpnumeroprocesso
        		    group by empnumeroprocesso, pagstatus
        		     
        		    )  	AS foo1
        		    ),0)
                    WHEN pro_par.proid IS NOT NULL THEN
        		    coalesce(  (
        		    select foo1.valor_pagamento FROM
        		    (SELECT
        		    SUM(pag1.pagvalorparcela) as valor_pagamento
        		    from
        		    par.processoobraspar prp1
        		    inner join par.empenho emp1 on emp1.empnumeroprocesso = prp1.pronumeroprocesso and emp1.empcodigoespecie not in ('03', '13', '02', '04') and emp1.empstatus = 'A'
        		    inner join par.pagamento pag1 on pag1.empid = emp1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento = '2 - EFETIVADO'
        		    where prp1.proid = pro_par.proid ) as foo1
        		    ),0)
                    END
                    as valor_repassado,
                    
                    
                    CASE WHEN prp.prpid IS NOT NULL THEN

                	CASE WHEN ((select htddata  from par.documentoparprestacaodecontas dpp
                			inner join workflow.documento doc ON doc.docid = dpp.docid
                			inner join workflow.historicodocumento hst ON hst.docid = doc.docid 
                			WHERE hst.aedid in( 4939 ) AND doc.tpdid in (303 ) AND dpp.prpid = prp.prpid LIMIT 1))::date <= '{$dpcdatainiciocontagem1}' 
                	THEN
                		-- CONTO A PARTIR DO DIA 19-11-2018 
                		to_char(('{$dpcdatainiciocontagem1}'::date  + {$dpcquantidadedias} ) , 'DD/MM/YYYY')   
                		-- OU ENTAO SE TIVER SIDO ENVIADO APÓS O DIA 19-11-2018
                	ELSE
                		to_char(((select htddata  from par.documentoparprestacaodecontas dpp
                		inner join workflow.documento doc ON doc.docid = dpp.docid
                		inner join workflow.historicodocumento hst ON hst.docid = doc.docid 
                		WHERE hst.aedid in( 4939 ) AND doc.tpdid in (303 ) AND dpp.prpid =  prp.prpid LIMIT 1)::date + {$dpcquantidadedias} ) , 'DD/MM/YYYY')
                	END	
                
                WHEN pro_par.proid IS NOT NULL THEN
                	CASE WHEN ((select htddata  from obras2.execucaofinanceira exec
                		inner join workflow.documento doc ON doc.docid =exec.docid
                		inner join workflow.historicodocumento hst ON hst.docid = doc.docid 
                		WHERE hst.aedid in( 5416 ) AND doc.tpdid in (328 ) AND exec.proid_par = pro_par.proid LIMIT 1))::date <= '{$dpcdatainiciocontagem1}' 
                	THEN
                		-- CONTO A PARTIR DO DIA 19-11-2018 
                		to_char(('{$dpcdatainiciocontagem1}'::date +  {$dpcquantidadedias} ) , 'DD/MM/YYYY') 
                		-- OU ENTAO SE TIVER SIDO ENVIADO APÓS O DIA 19-11-2018
                	ELSE
                		to_char(((select htddata  from obras2.execucaofinanceira exec
                		inner join workflow.documento doc ON doc.docid =exec.docid
                		inner join workflow.historicodocumento hst ON hst.docid = doc.docid 
                		WHERE hst.aedid in( 5416 ) AND doc.tpdid in (328 ) AND exec.proid_par = pro_par.proid LIMIT 1)::date + {$dpcquantidadedias} ) , 'DD/MM/YYYY')
                	END
                END as prazo_prestar_contas,
                            
                     
                    CASE WHEN prp.prpid IS NOT NULL THEN
        		dop.dopdatainiciovigencia || ' - '|| dop.dopdatafimvigencia
                    WHEN pro_par.proid IS NOT NULL THEN
        		dop2.dopdatainiciovigencia ||' - '|| par.retornavigenciaobrapar(pro_par.proid)
                    END as prazo_vigencia,
            
                    CASE WHEN prp.prpid IS NOT NULL THEN
        		par.retornasaldoprocesso(prpnumeroprocesso)
                    WHEN pro_par.proid IS NOT NULL THEN
        		par.retornasaldoprocesso(pro_par.pronumeroprocesso)
                    END as saldo,
                     
                    CASE WHEN prp.prpid IS NOT NULL THEN
        		(SELECT mdonome FROM par.modelosdocumentos  where mdoid = dop.mdoid)
                    WHEN pro_par.proid IS NOT NULL THEN
        		(SELECT mdonome FROM par.modelosdocumentos  where mdoid = dop2.mdoid)
                    END as programa
                     
                     
                    FROM
                    par.omissaopccacs opc
                    LEFT JOIN par.processopar prp ON prp.prpid = opc.prpid
                    LEFT JOIN par.documentopar dop ON dop.prpid = prp.prpid AND dop.dopstatus = 'A'
                    LEFT JOIN par.processoobraspar   pro_par ON pro_par.proid = opc.proid_par
                    LEFT JOIN par.documentopar dop2 ON dop2.proid = pro_par.proid AND dop2.dopstatus = 'A'
                    
                    WHERE
                    opc.inuid =  {$inuid}
                    AND occstatus = 'A'
                    AND occciencia = FALSE    
            ";
            
        } else {
            return Array();
        }
        
        $ListaOmissao = $this->carregar($sql);
        $ListaOmissao = ($ListaOmissao) ? $ListaOmissao : Array();
    
        return $ListaOmissao;
    }
    
    /**
     */
    public function insereCienciaOmissao($arrParams)
    {
        $cpf    = ($arrParams['usucpf']) ? $arrParams['usucpf'] :'';
        $inuid  = ($arrParams['inuid']) ? $arrParams['inuid'] : '';
        $ropid  = ($arrParams['ropid']) ? $arrParams['ropid'] : '';
        
        if (($ropid != '') &&($cpf != '') && ($ropid != '')) {  
            $sql = "
                UPDATE
                    par.omissaopccacs
                SET
                occciencia = TRUE,
                occcpfciencia = '{$cpf}',
                occdataciencia = 'now()'
                WHERE inuid = {$inuid}
                AND
                ropid = {$ropid} 
            ";
            $executa = $this->executar($sql);
        
            if ($executa) {
                return $this->commit();
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

<?php
/**
 * Classe de mapeamento da entidade par.documentoparprestacaodecontas.
 *
 * @version $Id$
 * @since 2018.07.04
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par_Model_Documentoparprestacaodecontas: sem descricao
 *
 * @package Par\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *        
 * @example <code>
 *          // -- Consultando registros
 *          $model = new Par_Model_Documentoparprestacaodecontas($valorID);
 *          var_dump($model->getDados());
 *         
 *          // -- Alterando registros
 *          $valores = ['campo' => 'valor'];
 *          $model = new Par_Model_Documentoparprestacaodecontas($valorID);
 *          $model->popularDadosObjeto($valores);
 *          $model->salvar(); // -- retorna true ou false
 *          $model->commit();
 *          </code>
 *         
 * @property int $dppid Id da tabela PK - default: nextval('par.documentoparprestacaodecontas_dppid_seq'::regclass)
 * @property int $dopid Coluna relacional com id da par.documentopar
 * @property int $docid Coluna relacional com id da par.processopar
 * @property string $dppstatus Status do registro A = ativo I = inavito (exclusão lógica) - default: 'A'::character varying
 * @property bool $dpporientacao coluna informa leitura da orientacao da analise do cacs - default: false
 * @property bool $dppquestionario coluna informa se o questionario do cacs foi totalmente preenchido - default: false
 * @property int $prpid
 */
class Par_Model_Documentoparprestacaodecontas extends Modelo
{

    /**
     *
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.documentoparprestacaodecontas';

    /**
     *
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'dppid'
    );

    /**
     *
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'docid' => array(
            'tabela' => 'workflow.documento',
            'pk' => 'docid'
        ),
        'dopid' => array(
            'tabela' => 'par.documentopar',
            'pk' => 'dopid'
        )
    );

    /**
     *
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'dppid' => null,
        'dopid' => null,
        'docid' => null,
        'dppstatus' => null,
        'dpporientacao' => null,
        'dppquestionario' => null,
        'prpid' => null
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
            'dppid' => array(
                'Digits'
            ),
            'dopid' => array(
                'allowEmpty' => true,
                'Digits'
            ),
            'docid' => array(
                'allowEmpty' => true,
                'Digits'
            ),
            'dppstatus' => array(
                'allowEmpty' => true,
                new Zend_Validate_StringLength(array(
                    'max' => 1
                ))
            ),
            'dpporientacao' => array(
                'allowEmpty' => true
            ),
            'dppquestionario' => array(
                'allowEmpty' => true
            ),
            'prpid' => array(
                'allowEmpty' => true,
                'Digits'
            )
        );
    }

    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams            
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function retornaProcessosNotificacaoOmissaoEnte($arrParams)
    {
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'includes/classes_simec.inc';
        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        $offset = ($arrParams['offset'] != '') ? "OFFSET {$arrParams['offset']} " : "";
        
        $datas = $this->pegaLinha("SELECT dpcdatainiciocontagem1, dpcdatainiciocontagem2, dpcdatalote1, dpcdatalote2, dpcquantidadedias from par.datasprestacaocontas WHERE dpcid = 1");
        
        $dpcdatainiciocontagem1 = $datas['dpcdatainiciocontagem1'];
        $dpcdatalote1 = $datas['dpcdatalote1'];
        $dpcquantidadedias = $datas['dpcquantidadedias'];
       
        
        if (($dpcdatainiciocontagem1 != '') && ($dpcdatalote1 != '') && ($dpcquantidadedias != '')) {
            $sqlProcessos = "
                 WITH wf_documento_e_estado AS (
                	SELECT esd.esddsc, docid, doc.esdid FROM workflow.documento doc
                	INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                	where doc.tpdid in (303 , 335)
        		)
                    SELECT 
                    	prp.prpid,
                    	dopdatafimvigencia,
                        (date_trunc('MONTH', to_date(dopdatafimvigencia,'MM/YYYY')) + INTERVAL '1 MONTH - 1 day')::date as data,
                        doc.esdid as esdid_fase,
                        doc2.esdid as esdid_situacao,
                		doc.docid as docid_fase_pc,
                		doc2.docid as docid_situ_pc,
                		prp.inuid
                    FROM 
                    	par.documentopar dop
                    INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid AND mdostatus = 'A' and tpdcod in ( 21, 102, 103) AND mdo.mdoid <> 3 /*@todo pendente de validação, aqui estou excluindo os casos de convenio*/
                    INNER JOIN par.processopar prp ON prp.prpid = dop.prpid AND prpstatus = 'A'
                    INNER JOIN par.documentoparprestacaodecontas  dpp ON dpp.prpid = dop.prpid AND dppstatus = 'A'
                	INNER JOIN wf_documento_e_estado doc ON doc.docid = dpp.docid AND esdid = 2005
                	INNER JOIN par.situacaoprestacaocontas  spc ON spc.prpid = dop.prpid AND spcstatus = 'A'
                	INNER JOIN wf_documento_e_estado doc2 ON doc2.docid = spc.docid AND  doc2.esdid = 2236
                    WHERE 
                    	dop.prpid IS NOT NULL 
                    
                     AND
                    	(-- SE JÁ ESTIVER VENCIDO ANTES DE 02/07/2018 
            			CASE WHEN (date_trunc('MONTH', to_date(dopdatafimvigencia,'MM/YYYY')) + INTERVAL ' 1 MONTH - 1 day') < '{$dpcdatainiciocontagem1}' THEN
            				-- ENTÃO CONTA OS DIAS A PARTIR DE 02/07/2018
            				/* ((( date_trunc('MONTH', to_date('02/05/2018','DD/MM/YYYY'))  + INTERVAL ' 1 day')::date - current_date) * -1) > {$dpcquantidadedias} -- testes*/ 
            				((( date_trunc('MONTH', to_date('{$dpcdatalote1}','DD/MM/YYYY'))  + INTERVAL ' 1 day')::date - current_date) * -1) > {$dpcquantidadedias} 
            			-- CASO NÃO ESTEJA VENCIDO
            			WHEN ((date_trunc('MONTH', to_date(dopdatafimvigencia,'MM/YYYY')) + INTERVAL ' 1 MONTH - 1 day')::date - current_date) > 0  THEN  
            				-- RETORNA FALSO E ESTÁ OK
            				FALSE
            			-- CASO ESTEJA VENCIDO E NÃO ENTROU NA CONDIÇÃO DO 02/07/2018 
            			WHEN ((date_trunc('MONTH', to_date(dopdatafimvigencia,'MM/YYYY')) + INTERVAL ' 1 MONTH - 1 day')::date - current_date) < 0 THEN  
            				-- CONTA 60 DIAS A PARTIR DO DIA DO VENCIMENTO 
            				(((date_trunc('MONTH', to_date(dopdatafimvigencia,'MM/YYYY')) + INTERVAL ' 1 MONTH - 1 day')::date - current_date) * -1) > {$dpcquantidadedias} 
            			ELSE
            				 FALSE
            			-- FIM
            			END)
                    AND
                    	   dop.prpid not in (select distinct prpid from par.termossigpcpar WHERE prpid IS NOT NULL)
                     AND
                    	   dop.prpid not in ( select distinct prpid from par.omissaopcente  where opcstatus = 'A' AND prpid is not null)
                    AND
                    	dopstatus = 'A'
                {$offset}
                {$limite}
                ";
            
            $arrProcessos = $this->carregar($sqlProcessos);
        }
        
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou = (is_array($arrProcessos)) ? TRUE : FALSE;
        $registros = (is_array($arrProcessos)) ? $arrProcessos : Array();
        $arrReturn = Array(
            'existeRegistro' => $retornou,
            'arrProcessos' => $registros
        );
        return $arrReturn;
    }

    /**
     * Retorna processos com omissão do cacs
     *
     * @param Array $arrParams
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function retornaProcessosNotificacaoOmissaoCACS($arrParams)
    {
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'includes/classes_simec.inc';
        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        $offset = ($arrParams['offset'] != '') ? "OFFSET {$arrParams['offset']} " : "";
    
        $datas = $this->pegaLinha("
            SELECT        
            	dpcdatainiciocontagem1, dpcdatainiciocontagem2, dpcdatalote1, dpcdatalote2, dpcquantidadedias
            FROM 
            	par.datasprestacaocontas 
            WHERE 
            	dpcid = ".DPCID_OMISSAO_CACS." ");
        
        
        $dpcdatainiciocontagem1 = $datas['dpcdatainiciocontagem1'];
        $dpcdatalote1 = $datas['dpcdatalote1'];
        $dpcquantidadedias = $datas['dpcquantidadedias'];

    
        if (($dpcdatainiciocontagem1 != '') && ($dpcdatalote1 != '') && ($dpcquantidadedias != '')) {
            
            $sqlProcessos = "
             WITH wf_documento_e_estado AS (
        		SELECT esd.esddsc,(SELECT htddata FROM workflow.historicodocumento  where aedid = ".AEDID_ENVIAR_ANALISE_CONSELHO." and docid = doc.docid ORDER BY hstid desc LIMIT 1) as data_tramitacao, docid, doc.esdid FROM workflow.documento doc
        		INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid   
        		where doc.tpdid in (".TPDID_FLUXO_PRESTACAO_CONTA." , ".TPDID_FLUXO_SITUACAO_PRESTACAO_CONTAS.") AND doc.esdid in ( ".ESDID_AGUARDANDO_ANALISE_CONSELHO.", ".ESDID_SPC_ENVIADA_AO_CONTROLE_SOCIAL.")
                    )
                    SELECT
                    DISTINCT 
                    prp.prpid, 
                    dopdatafimvigencia,
                    doc.esdid as esdid_fase,
                    doc2.esdid as esdid_situacao,
                    doc.docid as docid_fase_pc,
                    doc2.docid as docid_situ_pc,
                    prp.inuid
                    FROM
                    par.documentopar dop
                    INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid AND mdostatus = 'A' and tpdcod in ( 21, 102, 103) AND mdo.mdoid <> 3 
                    INNER JOIN par.processopar prp ON prp.prpid = dop.prpid AND prpstatus = 'A'
                    INNER JOIN par.documentoparprestacaodecontas  dpp ON dpp.prpid = dop.prpid AND dppstatus = 'A'
                    INNER JOIN wf_documento_e_estado doc ON doc.docid = dpp.docid 
                    INNER JOIN par.situacaoprestacaocontas  spc ON spc.prpid = dop.prpid AND spcstatus = 'A'
                    INNER JOIN wf_documento_e_estado doc2 ON doc2.docid = spc.docid AND doc2.esdid = ".ESDID_SPC_ENVIADA_AO_CONTROLE_SOCIAL." /*Para trazer apenas as que ainda não foram notificadas*/ 
                    WHERE
                    dop.prpid IS NOT NULL
                    AND dop.dopstatus = 'A'
        		AND
        		(
        			-- SE JÁ TIVER SIDO ENVIADO ANTES DE 19-11-2018
        			CASE WHEN (doc.data_tramitacao)::date <= '{$dpcdatainiciocontagem1}' THEN
        				-- CONTO A PARTIR DO DIA 19-11-2018 
        				CASE WHEN (('{$dpcdatainiciocontagem1}'::date - current_date)*-1) > {$dpcquantidadedias} THEN
        					TRUE
        				ELSE
        					FALSE
        				END
        			-- OU ENTAO SE TIVER SIDO ENVIADO APÓS O DIA 19-11-2018
        			WHEN (doc.data_tramitacao)::date > '{$dpcdatainiciocontagem1}' THEN
        				-- COMPARO COM HOJE E VEJO SE JÁ PASSOU DE 60 DIAS
        				CASE WHEN (((doc.data_tramitacao)::date - current_date)*-1) > {$dpcquantidadedias} THEN 
        					TRUE
        				ELSE
        					FALSE
        				END
        			-- SENÃO  
        			ELSE
        				--FALSO
        				FALSE
        			END	
        		)
        		
            {$offset}
            {$limite}
            ";
            $arrProcessos = $this->carregar($sqlProcessos);
            
        }
    
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou = (is_array($arrProcessos)) ? TRUE : FALSE;
        $registros = (is_array($arrProcessos)) ? $arrProcessos : Array();
            $arrReturn = Array(
            'existeRegistro' => $retornou,
            'arrProcessos' => $registros
            );
            
            return $arrReturn;
    }
    
    /**
     */
    public function verificaFormacao($prpid = '')
    {
        if ($prpid != '') {
            $sql = "
            SELECT DISTINCT 
            	dop.prpid
            FROM
            	par.subacaodetalhe sd 
            	
            
            INNER JOIN par.subacao s ON s.sbaid =sd.sbaid
            INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
            INNER JOIN par.documentopar dop ON dop.dopid = tc.dopid AND dopstatus = 'A'
            
            WHERE
            	s.prgid in (select DISTINCT prgid from par.programaformacao WHERE frmstatus = 'A')
            AND dop.prpid = {$prpid} 
            ";
            $resultado = $this->pegaUm($sql);
            return ($resultado) ? TRUE : FALSE;
        } else {
            return false;
        }
    }
    
    /**
     */
    public function verificaExistenciaPagamento($prpid = '')
    {
        if ($prpid != '') {
            $sql = "           
                select coalesce( (
                   Select 
                	  coalesce(sum(pg.pagvalorparcela),0.00) as valor_pagamento
                   From par.pagamento pg
                   join par.empenho em on em.empid = pg.empid and empstatus = 'A' AND pg.pagstatus = 'A' 
                   where trim(pg.pagsituacaopagamento) ilike '%EFETIVADO%'
                   AND em.empnumeroprocesso = (SELECT prpnumeroprocesso from par.processopar where prpid = {$prpid})
                   group by empnumeroprocesso, pagstatus
                   
                ),0) 
            ";
            
            $resultado = $this->pegaUm($sql);
            return  ($resultado > 0) ? TRUE : FALSE;
            
        } else {
            return false;
        }
    }
    
    /**
     * Caso tenha sido aprovado um retorno para monitoramento nos últimos 3 dias, não iremos notificar a entidade 
     * @param string $prpid
     * @return boolean
     */
    public function verificaRetornoUltimosTresDias($prpid = '')
    {
        
        $dpcquantidadedias = $this->pegaUm("SELECT dpcquantidadedias from par.datasprestacaocontas WHERE dpcid = 4");
        
        if (($prpid != '') && ($dpcquantidadedias!='')) {
            $sql = "
                SELECT 
                    hstid 
                FROM 
                    par.documentoparprestacaodecontas dpc
                INNER JOIN workflow.historicodocumento hst ON hst.docid = dpc.docid
                WHERE 
                    ((htddata::date - current_date )*-1) <= {$dpcquantidadedias} AND aedid = ".AEDID_PC_RETORNO_MONITORAMENTO_APROVADO."  AND prpid = {$prpid}
            ";
            
            $resultado = $this->pegaUm($sql);
            return ($resultado) ? TRUE : FALSE;
             
        } else {
            return false;
        }
    }
    

    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams            
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function insereRegistrosInexistente($arrRegistros)
    {
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'includes/classes_simec.inc';
        
        $arrValores = Array();
        foreach ($arrRegistros as $k => $v) {
            $docidFluxo = '';
            $prpid = '';
            
            $docidFluxo = wf_cadastrarDocumento(TPDID_FLUXO_PRESTACAO_CONTA, "Cadastro de documento do Fluxo de prestação de contas - Via acompanhamento", ESDID_AGUARDANDO_ENVIO_PROPONENTE);
            $prpid = $v['prpid'];
            if (($prpid != '') && ($docidFluxo != '')) {
                $arrValores[] = "({$prpid}, {$docidFluxo})";
            }
        }
        
        if (count($arrValores) > 0) {
            $sql = "
                INSERT INTO par.documentoparprestacaodecontas
                    (prpid, docid)
                VALUES
                ";
            $sql .= implode(",
                            ", $arrValores);
            $this->executar($sql);
            return $this->commit();
        }
        
        return FALSE;
    }

    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams            
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function retornaProcessosSemFluxo($arrParams)
    {
        // Recupera parametros
        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        $offset = ($arrParams['offset'] != '') ? "OFFSET {$arrParams['offset']} " : "";
        
        // 'par.documentoparprestacaodecontas';
        /*
         * RN = Seleciona todos prpids que não possuam registro na tabela par.documentoparprestacaodecontas
         * RN2 = Que não estajam contemplados pelo SIGPC
         */
        $sql = "
    	   SELECT DISTINCT 
	           prpid 
            FROM 
	           par.processopar 
            WHERE 
	           prpstatus = 'A' 
            AND
	           prpid not in (select distinct prpid from par.documentoparprestacaodecontas)
            AND
	           prpid not in (select distinct prpid from par.termossigpcpar WHERE prpid is not null)
            {$offset}
            {$limite}
        ";
        
        $arrProcessos = $this->carregar($sql);
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou = (is_array($arrProcessos)) ? TRUE : FALSE;
        $registros = (is_array($arrProcessos)) ? $arrProcessos : Array();
        $arrReturn = Array(
            'existeRegistro' => $retornou,
            'cadastroRegistro' => $registros
        );
        return $arrReturn;
    }
    
    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function retornaProcessosSemSaldo($arrParams)
    {
        
        // Recupera parametros
        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        $offset = ($arrParams['offset'] != '') ? "OFFSET {$arrParams['offset']} " : "";
    
        // 'par.documentoparprestacaodecontas';
        /*
         * RN = Seleciona todos prpids que não possuam registro na tabela par.documentoparprestacaodecontas
         * RN2 = Que não estajam contemplados pelo SIGPC
         */
        $sql = "
                    WITH wf_documento_e_estado AS (
                            	SELECT esd.esddsc, docid, doc.esdid FROM workflow.documento doc
                            	INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                            	where doc.tpdid in (303 , 335, 336)
                    		)
                    SELECT 
                        prp.prpid,
                    	doc.docid as docid_fase_pc,
                        doc.esdid as esdid_fase,
                        doc2.docid as docid_situ_pc,
                        doc2.esdid as esdid_situacao,
            			doc3.docid as docid_opc,
            			doc3.esdid as esdid_opc,
            			prp.inuid 
                        FROM 
                        	par.documentopar dop
                        INNER JOIN par.modelosdocumentos mdo ON mdo.mdoid = dop.mdoid AND mdostatus = 'A'  
                        INNER JOIN par.processopar prp ON prp.prpid = dop.prpid AND prpstatus = 'A'
                        LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dop.dopid
                        INNER JOIN par.documentoparprestacaodecontas  dpp ON dpp.prpid = dop.prpid AND dppstatus = 'A'
                        INNER JOIN wf_documento_e_estado doc ON doc.docid = dpp.docid 
                        INNER JOIN par.situacaoprestacaocontas  spc ON spc.prpid = dop.prpid AND spcstatus = 'A'
                        INNER JOIN wf_documento_e_estado doc2 ON doc2.docid = spc.docid
                        INNER JOIN par.situacaoopc sitopc ON sitopc.prpid = prp.prpid 
                        INNER JOIN wf_documento_e_estado doc3 ON doc3.docid = sitopc.docid
                        WHERE 
                        
                        	/*Tenha vigência*/
                        	dopdatafimvigencia IS NOT NULL 
                        AND
                        	/*Esteja vencido*/
                        	(date_trunc('MONTH', to_date(dopdatafimvigencia,'MM/YYYY')) + INTERVAL ' 1 MONTH - 1 day')::date < current_date 
                        AND
                        	/*termoativo*/
                        	dop.dopstatus = 'A'
                        AND 
                        	/*Não recebeu recurso */
                        	coalesce( (
                        		   Select 
                        			  coalesce(sum(pg.pagvalorparcela),0.00) as valor_pagamento
                        		   From par.pagamento pg
                        		   join par.empenho em on em.empid = pg.empid and empstatus = 'A' AND pg.pagstatus = 'A' 
                        		   where trim(pg.pagsituacaopagamento) ilike '%EFETIVADO%'
                        		   AND em.empnumeroprocesso = (SELECT prpnumeroprocesso from par.processopar where prpid = prp.prpid)
                        		   group by empnumeroprocesso, pagstatus
                        
                        	),0) <= 0
                    	AND    
                            /*Não tenha sido tramitada ainda pela rotina*/
                            ((doc.esdid <> ".ESDID_AGUARDANDO_ENVIO_PROPONENTE.") OR (doc2.esdid <> ".ESDID_SITU_PC_SEM_VALOR_A_COMPROVAR.") OR (doc3.esdid <> ".ESDID_OPC_CONCLUIDA."))
                        AND
            				CASE WHEN (
            					SELECT
            					    count(d.dopid)
            					FROM par.documentopar d
            					WHERE d.mdoid IN (
            					    SELECT
            						   md.mdoid
            					    FROM   par.modelosdocumentos md
            					    WHERE (mdodocumentoex = TRUE OR mdonome ILIKE '%_EX') AND mdostatus = 'A'
            					) AND d.dopid = dop.dopid
            				) > 0 THEN
            					TRUE
            				ELSE
            					(
            						mdo.tpdcod in ( 21, 102, 103) AND mdo.mdoid <> 3 
            						AND 
            						dpv.dpvstatus = 'A'
            					)
            				END
            			AND 
		                    prp.prpid not in (select distinct prpid from par.termossigpcpar  where  prpid is not null)
                    {$offset}
                    {$limite}
        ";
                    
        $arrProcessos = $this->carregar($sql);
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou = (is_array($arrProcessos)) ? TRUE : FALSE;
        $registros = (is_array($arrProcessos)) ? $arrProcessos : Array();
        $arrReturn = Array(
            'existeRegistro' => $retornou,
            'cadastroRegistro' => $registros
        );
        return $arrReturn;
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
     * Função responsável por retornar as Omissões ativas em que o ex gestor não foi notificado
     *
     * @param $offset
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'omissoesRegistros' => retorna array com os registros encontrados)
     *
     */
    public function recuperaOmissoes($offset){
        
        $sql = "select 
                    opcid,
                    case when opc.prpid is not null
                              then opc.prpid
                        when opc.proid_par is not null
                            then opc.proid_par
                        else opc.proid_pac
                    end as id,
                    case when opc.prpid is not null
                              then 'par'
                        when opc.proid_par is not null
                            then 'obras'
                        else 'pac'
                    end as tipo
                from (
                    -- PAR
                    select opc.*
                    from par.omissaopcente opc
                           INNER JOIN par.documentoparprestacaodecontas dpp ON opc.prpid = dpp.prpid AND dpp.dppstatus = 'A'
                           INNER JOIN workflow.documento d ON dpp.docid = d.docid
                    WHERE d.esdid = 2005
                      AND opc.opcstatus = 'A'
                    union all
                    -- OBRAS PAR
                    select opc.*
                    from par.omissaopcente opc
                           INNER JOIN obras2.execucaofinanceira exe ON opc.proid_par = exe.proid_par AND exe.exestatus = 'A'
                           INNER JOIN workflow.documento d ON exe.docid = d.docid
                    WHERE d.esdid = 2174
                      AND opc.opcstatus = 'A'
                    union all
                    -- OBRAS PAC
                    select opc.*
                    from par.omissaopcente opc
                           INNER JOIN obras2.execucaofinanceira exe ON opc.proid_pac = exe.proid_pac AND exe.exestatus = 'A'
                           INNER JOIN workflow.documento d ON exe.docid = d.docid
                    WHERE d.esdid = 2174
                      AND opc.opcstatus = 'A'
                    ) as opc where opc.opcstatus = 'A' and opc.opcid not in (select neg.opcid from par.notificaexgestorespc neg where neg.negstatus = 'A' and neg.sngid <> 3) 
                  offset 
                    {$offset}  ROWS FETCH NEXT 500 ROWS ONLY";
        
        $arrOmissao = $this->carregar($sql);
        // Caso retorne alguma omissão  retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou = (is_array($arrOmissao)) ? TRUE : FALSE;
        $registros = (is_array($arrOmissao)) ? $arrOmissao : Array();
        $arrReturn = Array(
            'existeRegistro' => $retornou,
            'omissoesRegistros' => $registros
            );
        return $arrReturn;
        
    }
    
    /**
     * Função responsável por retornar os ex-gestores ainda não notificados
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'exGestoresRegistro' => retorna array com os registros encontrados)
     *
     */
    public function recuperaGestoresNaoNotificados($omissao){
        
        
        $sql = "select egn.usucpf from par.exgestorpcnotificado egn
                inner join seguranca.usuario usu on usu.usucpf = egn.usucpf
                left join par.notificaexgestorespc neg on egn.negid = neg.negid where neg.opcid = {$omissao['opcid']}";
        
        $notificaexgestorespc = $this->carregar($sql);
        
        $exgestores = array();//buscar todos ex-gestores
        
        $registros = array();
        if(is_array($notificaexgestorespc) && is_array($exgestores) && count($notificaexgestorespc) < count($exgestores)){
            
            $registros = array_diff($exgestores, $notificaexgestorespc);
            
        }else{
            $registros = $exgestores;
        }
        
        $registros = (is_array($registros)) ? $registros : Array();
        $arrReturn = Array(
            'existeRegistro' => (is_array($registros)) ? TRUE : FALSE,
            'exGestores' => $registros
            );
        
        return $arrReturn;
        
    }
    
    
    /**
     * Função responsável por retornar os ex-gestores ainda não notificados
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'exGestoresRegistro' => retorna array com os registros encontrados)
     *
     */
    public function verificaNotificaExGestores($omissao, $situacao){
        
        $sql = "select * from par.notificaexgestorespc neg where neg.opcid = {$omissao['opcid']} and neg.negstatus = 'A'";
        $notificaexgestorespc = $this->pegaLinha($sql);
        
        //Caso não exista registro de notificação para essa omissão, criamos o registro como iniciado.
        if(!is_array($notificaexgestorespc)){
            
            $insert = "insert into par.notificaexgestorespc (sngid, opcid) values ({$situacao}, {$omissao['opcid']})";
            
            if($this->executar($insert)){
                $this->commit();
            }
            
            $notificaexgestorespc = $this->pegaLinha($sql);
        }

        return (is_array($notificaexgestorespc)) ? $notificaexgestorespc : Array();
    }
    
    /**
     * Função responsável por retornar os ex-gestores ainda não notificados
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'exGestoresRegistro' => retorna array com os registros encontrados)
     *
     */
    public function verificaExGestores($notificaexgestorespc, $omissao){
        
        include_once APPRAIZ . 'www/par/_funcoes_acompanhamento.php';

        $sql = "select * from par.exgestorpcnotificado egn where egn.negid = {$notificaexgestorespc['negid']} and egn.egnstatus = 'A'";
        $registros = $this->carregar($sql);
      
        //buscamos o municipio ou estado de acordo com o tipo do processo
        switch ($omissao['tipo']) {
            case 'par':
                
                $sql = "SELECT inu.itrid, inu.estuf, inu.muncod
                            FROM
                                par.processopar prp
                            INNER JOIN
                                par.instrumentounidade inu ON prp.inuid = inu.inuid
                            WHERE prp.prpid = ".$omissao['id'];
                break;
            case 'obras':
                
                $sql = "SELECT inu.itrid, inu.estuf, inu.muncod
                            FROM
                              par.processoobraspar pro
                            INNER JOIN
                              par.instrumentounidade inu ON pro.inuid = inu.inuid
                            WHERE pro.proid = ".$omissao['id'];
                break;
            case 'pac':
                $sql = "SELECT
                        inu.itrid, inu.estuf, inu.muncod
                        FROM
                          par.processoobra pro
                        INNER JOIN par.termocompromissopac tc ON pro.proid = tc.proid AND tc.terstatus = 'A'
                        INNER JOIN par.omissaopcente opc ON opc.proid_pac = pro.proid
                        INNER JOIN par.instrumentounidade inu ON opc.inuid = inu.inuid
                            WHERE pro.proid = ".$omissao['id'];
                break;
        }
        
        $municipio = $this->pegaLinha($sql);

        //pelo código do municipio ou estado identificamos os gestores
        if ($municipio['itrid'] == 1) {
            $sqlResp = "SELECT
                  usucpf             AS cpf,
                  rrsnomesecretario  AS nome,
                  rrsemail           AS email,
                  rrsfuncao          AS funcao,
                  rrsdtiniciomandato AS inicio,
                  rrsdtfimmandato    AS fim
                FROM par.rollresponsaveissecretarios
                WHERE rrsufmandato = '{$municipio['estuf']}' ORDER BY rrsdtfimmandato DESC";
        } else {
            $sqlResp = "SELECT
                  usucpf as cpf,
                  rrpnomeprefeito as nome,
                  rrpemail as email,
                  rrpfuncao as funcao,
                  rrpdtiniciomandato as inicio,
                  rrpdtfimmandato as fim
                FROM par.rollresponsaveisprefeitos WHERE muncod = '{$municipio['muncod']}' ORDER BY rrpdtfimmandato DESC";
        }
        
        $arrResponsaveis = $this->carregar($sqlResp);

        $dadosTermo = $this->buscaDadosTermoUnidade($omissao['tipo'], $omissao['id']);

        $entrou = false;
        
        //se não encontrarmos responsáveis, retornamos o array
        if(!is_array($arrResponsaveis)){
            return (is_array($registros)) ? $registros : Array();
        }
        foreach ($arrResponsaveis as $responsavelInserir){
            
            //verificamos se o gestor é ex-gestor
            $interesse = buscaNaturezaInteresse($responsavelInserir, $dadosTermo);
            
            $cpfAtualGestor = '';
            $atual = strpos( $interesse, 'Atual' );
            $exDirigente = strpos( $interesse, 'Ex-Dirigente' );
            

            if($atual === false && $exDirigente === false){

                if(!is_array($registros)){
                    if( $entrou === false){
                        $insert = "insert into par.exgestorpcnotificado (usucpf, negid) values ('".$responsavelInserir['cpf']."', '".$notificaexgestorespc['negid']."')";

                        $entrou = true;
                    }
                    
                }else{
                    $notificado = false;
                    
                    //se o cpf não está cadastrado, como notificado (pelo token)
                    foreach ($registros as $registro){

                        if($registro['usucpf'] == $responsavelInserir['cpf'] && $responsavelInserir['cpf'] != $cpfAtualGestor){
                            $notificado = true;
                        }
                    }

                    if(!$notificado){
                        $insert = "insert into par.exgestorpcnotificado (usucpf, negid) values ('".$responsavelInserir['cpf']."', '".$notificaexgestorespc['negid']."')";
                    }
                }

                if($insert != '' && $this->executar($insert)){
                    $this->commit();
                    $insert = '';
                    
                    $sql = "select * from par.exgestorpcnotificado egn where egn.negid = {$notificaexgestorespc['negid']} and egn.egnstatus = 'A'";
                    $registros = $this->carregar($sql);
                }
               
            }else{
                $cpfAtualGestor = $responsavelInserir['cpf'];
            }
        }

        
        return (is_array($registros)) ? $registros : Array();
    }
    
    
    public function atualizarNotificaExGestores($situacao, $opcid){
        $update = "update par.notificaexgestorespc set sngid = {$situacao} where opcid = {$opcid};";
        
        if($this->executar($update)){
            $this->commit();
        }
        
    }
    
    public function contadorTermosOmissao(){
        $sql = "select COUNT(*)
                from (
                    -- PAR
                    select opc.*
                    from par.omissaopcente opc
                           INNER JOIN par.documentoparprestacaodecontas dpp ON opc.prpid = dpp.prpid AND dpp.dppstatus = 'A'
                           INNER JOIN workflow.documento d ON dpp.docid = d.docid
                    WHERE d.esdid = 2005
                      AND opc.opcstatus = 'A'
                    union all
                    -- OBRAS PAR
                    select opc.*
                    from par.omissaopcente opc
                           INNER JOIN obras2.execucaofinanceira exe ON opc.proid_par = exe.proid_par AND exe.exestatus = 'A'
                           INNER JOIN workflow.documento d ON exe.docid = d.docid
                    WHERE d.esdid = 2174
                      AND opc.opcstatus = 'A'
                    union all
                    -- OBRAS PAC
                    select opc.*
                    from par.omissaopcente opc
                           INNER JOIN obras2.execucaofinanceira exe ON opc.proid_pac = exe.proid_pac AND exe.exestatus = 'A'
                           INNER JOIN workflow.documento d ON exe.docid = d.docid
                    WHERE d.esdid = 2174
                      AND opc.opcstatus = 'A'
                    ) as opc where opc.opcstatus = 'A' and opc.opcid not in (select neg.opcid from par.notificaexgestorespc neg where neg.negstatus = 'A' and neg.sngid <> 3)";
        
        $result = $this->pegaUm($sql);

        return $result;
    }
    
    public function buscaDadosTermoUnidade($tipoPc, $id){
        global $db;
        
        switch ($tipoPc) {
            case 'par':
                $sql = "SELECT
                              inu.inuid,
                              inu.itrid,
                              inu.estuf,
                              inu.muncod,
                              dop.dopid as id,
                              'PAR' as tipo,
                              ((date_trunc('MONTH', to_date(dop.dopdatainiciovigencia, 'MM/YYYY'))) :: DATE)                           AS iniciovigencia,
                              ((date_trunc('MONTH', to_date(dop.dopdatafimvigencia, 'MM/YYYY')) + INTERVAL '1 MONTH - 1 day') :: DATE) AS fimvigencia
                            FROM
                              par.documentopar dop
                            INNER JOIN
                              par.processopar prp ON prp.prpid = dop.prpid
                            INNER JOIN
                              par.instrumentounidade inu ON prp.inuid = inu.inuid
                            WHERE prp.prpid = {$id}";
                
                break;
            case 'obras':
                $sql = "SELECT
                              inu.inuid,
                              inu.itrid,
                              inu.estuf,
                              inu.muncod,
                              dop.dopid as id,
                              'OBRAS' as tipo,
                              ((date_trunc('MONTH', to_date(dop.dopdatainiciovigencia, 'MM/YYYY'))) :: DATE)                           AS iniciovigencia,
                              ((date_trunc('MONTH', to_date(dop.dopdatafimvigencia, 'MM/YYYY')) + INTERVAL '1 MONTH - 1 day') :: DATE)
                            FROM
                              par.documentopar dop
                            INNER JOIN
                              par.processoobraspar pro ON pro.proid = dop.proid
                            INNER JOIN
                              par.instrumentounidade inu ON pro.inuid = inu.inuid
                            WHERE pro.proid = {$id}";
                
                break;
            case 'pac':
                $sql = "SELECT
                              inu.inuid,
                              inu.itrid,
                              inu.estuf,
                              inu.muncod,
                              tc.terid as id,
                              'PAC' as tipo,
                              (
                                SELECT t.terdataassinatura
                                 FROM par.termocompromissopac t
                                 WHERE t.proid = tc.proid ORDER BY t.terid ASC LIMIT 1
                              ) AS iniciovigencia,
                              tc.terdatafimvigencia :: DATE AS fimvigencia
                            FROM
                              par.termocompromissopac tc
                            INNER JOIN
                              par.processoobra pro ON pro.proid = tc.proid
                            INNER JOIN
                              par.instrumentounidade inu ON (tc.muncod = inu.muncod OR tc.estuf = inu.estuf)
                            WHERE pro.proid = {$id} AND tc.terstatus = 'A'";
                break;
        }
        
        if($id && $sql != ''){
            return $db->pegaLinha($sql);
        } else {
            return false;
        }
    }
       
    
    /**
     * Função responsável por retornar os ex-gestores ainda não notificados
     *
     * @return $arrReturn Array ( 'existeRegistro' = caso exista registros que foram cadastrados retorna true, 'exGestoresRegistro' => retorna array com os registros encontrados)
     *
     */
    public function notificarExGestores($exGestores){
        
        $servidor = $_SERVER['HTTP_HOST'];
        $tituloServidor = "Omissão no dever legal de prestar contas ";
        $assuntoMensagemPadrao = "=?ISO-8859-1?B?" . base64_encode($tituloServidor) . "?=";
        
        $remetente = array(
            "nome" => "=?ISO-8859-1?B?" . base64_encode("PAR - Plano de Ações Articuladas") . "?=",
            "email" => "simecpar@mec.gov.br"
        );

        $notificouTodos = true;
        
        $arrCPF = array();
        
        foreach ($exGestores as $exGestor){
            
            if( is_null($exGestor['egntoken'])){
                
                $sql = "SELECT rrpemail as email, rrpnomeprefeito as nome FROM par.rollresponsaveisprefeitos where usucpf = '".$exGestor['usucpf']."'";
                
                $arrResponsaveis = $this->pegaLinha($sql);
                if(!$arrResponsaveis){
                    $sql = "SELECT rrsemail as email, rrsnomesecretario as nome FROM par.rollresponsaveissecretarios where usucpf = '".$exGestor['usucpf']."'";
                    $arrResponsaveis = $this->pegaLinha($sql);
                }
                
                $hash = md5($exGestor['usucpf']);
                

                $mensagem = "Prezado Sr.(a) ". ucwords(strtolower($arrResponsaveis['nome'])).", 
                    <br>
                    <br>
                    Informamos que não consta no Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC o envio de dados de prestação de contas de algum(s) Termo(s) de Compromisso pactuado(s) com o Fundo Nacional de Desenvolvimento da Educação/FNDE acerca do(s) qual(is) apurou-se responsabilidade de Vossa Senhoria.
                    <br> 
                    Alertamos que está disponível a possibilidade de envio da prestação de contas pelo SIMEC. Através do link (<a href='http://simec.mec.gov.br/par/notificacao_prestacao_contas_ex_gestores.php?token={$hash}'>http://simec.mec.gov.br/par/notificacao_prestacao_contas_ex_gestores.php?token={$hash}</a>), basta preencher os dados de cadastro solicitado e fazer o upload do arquivo.";
                

                $arrEmails = array(
                    $arrResponsaveis['email']
                );
                
                enviar_email($remetente, $arrEmails, $assuntoMensagemPadrao, $mensagem);
                
                $update = "update par.exgestorpcnotificado set egntoken = '".$hash."' where egnid = ".$exGestor['egnid'];
                
                if($this->executar($update)){
                    $this->commit();
                }else{
                    $notificouTodos = false;
                }
            }

        }
        
        return $notificouTodos;
    }
    
    
    public function buscaDadosPorToken($token){
        
        
        $datas = $this->pegaLinha("SELECT dpcdatainiciocontagem1, dpcdatainiciocontagem2, dpcdatalote1, dpcdatalote2, dpcquantidadedias from par.datasprestacaocontas WHERE dpcid = 1");
        
        $dpcdatainiciocontagem1 = $datas['dpcdatainiciocontagem1'];
        $dpcdatalote1 = $datas['dpcdatalote1'];
        $dpcquantidadedias = $datas['dpcquantidadedias'];
        $dpcdatalote2 = $datas['dpcdatalote2'];
        $dpcdatainiciocontagem2 = $datas['dpcdatainiciocontagem2'];
        
        $dpcquantidadediasMenusUm = $dpcquantidadedias - 1;
        
        
        $sql = "select * from par.exgestorpcnotificado egn
                inner join par.notificaexgestorespc neg on egn.negid = neg.negid and neg.negstatus = 'A'
                where egn.egnstatus = 'A' and egn.egntoken = '".$token."'";
        
        $arrEgn = $this->pegaLinha($sql);
        
        if($arrEgn){
            
            $sqlTermo = "select opc.*, egn.egnid, egn.egnstatus, egn.egntoken, egn.usucpf, egn.egnciencia, TO_CHAR(egn.egndataciencia, 'DD/MM/YYYY') AS egndataciencia, TO_CHAR(egn.egndataenviopc, 'DD/MM/YYYY') AS egndataenviopc, egn.negid, egn.arqid,
                    CASE WHEN rrp.rrprazaosocial IS NOT NULL
                      THEN rrp.rrprazaosocial
                      ELSE rrs.rrsrazaosocial
                    END as razaosocial,
                    CASE WHEN rrp.rrpufmandato IS NOT NULL
                      THEN rrp.rrpufmandato
                      ELSE rrs.rrsufmandato
                    END as ufmandato,
                    CASE WHEN rrp.rrpmunmandato IS NOT NULL
                      THEN rrp.rrpmunmandato
                      ELSE rrs.rrsmunmandato
                    END as munmandato,
                    case when opc.prpid is not null
                            then dop_par.dopid
                        when opc.proid_par is not null
                            then dop_obras.dopid
                        else ter.terid
                    end as id,
                    case when opc.prpid is not null
                              then 'PAR'
                        when opc.proid_par is not null
                            then 'OBRAS'
                        else 'PAC'
                    end as tipo,
                    case when opc.prpid is not null
                            then TO_CHAR(to_date(dop_par.dopdatafimvigencia, 'MM/YYYY'), 'MM/YYYY')
                        when opc.proid_par is not null
                            then TO_CHAR(to_date(dop_obras.dopdatafimvigencia, 'MM/YYYY'), 'MM/YYYY')
                        else TO_CHAR(ter.terdatafimvigencia,'DD/MM/YYYY')
                    end as vigencia,
                    case when opc.prpid is not null
                            then  CASE WHEN to_date(dop_par.dopdatafimvigencia, 'MM/YYYY') > to_date('{$dpcdatalote1}', 'DD/MM/YYYY')  
                				    THEN TO_CHAR(date_trunc('month', to_date(dop_par.dopdatafimvigencia, 'MM/YYYY'))+ '1 month' + '{$dpcquantidadediasMenusUm} day':: INTERVAL , 'DD/MM/YYYY') 
                					ELSE TO_CHAR(to_date('{$dpcdatalote1}', 'DD/MM/YYYY') + '{$dpcquantidadedias} day':: INTERVAL, 'DD/MM/YYYY') 
                				END
                        when  opc.proid_par is not null
                            then  CASE WHEN to_date(dop_obras.dopdatafimvigencia, 'MM/YYYY') > to_date('{$dpcdatalote1}', 'DD/MM/YYYY')  
                				    THEN TO_CHAR(date_trunc('month', to_date(dop_obras.dopdatafimvigencia, 'MM/YYYY'))+ '1 month' + '{$dpcquantidadediasMenusUm} day':: INTERVAL , 'DD/MM/YYYY') 
                					ELSE TO_CHAR(to_date('{$dpcdatalote1}', 'DD/MM/YYYY') + '{$dpcquantidadedias} day':: INTERVAL, 'DD/MM/YYYY') 
                				END
                        else CASE WHEN to_date(TO_CHAR(ter.terdatafimvigencia,'DD/MM/YYYY'), 'DD/MM/YYYY') > to_date('{$dpcdatalote2}', 'DD/MM/YYYY')
                                THEN TO_CHAR(to_date(TO_CHAR(ter.terdatafimvigencia,'DD/MM/YYYY'), 'DD/MM/YYYY') + INTERVAL '{$dpcquantidadedias} day', 'DD/MM/YYYY')
                                  ELSE TO_CHAR(to_date('{$dpcdatalote2}', 'DD/MM/YYYY') + '{$dpcquantidadedias} day':: INTERVAL, 'DD/MM/YYYY')
                              END
                    end as prazo, esd.esddsc,doc.docid
                     from par.omissaopcente opc
                    INNER JOIN par.notificaexgestorespc neg on opc.opcid = neg.opcid
                    INNER JOIN par.exgestorpcnotificado egn ON neg.negid = egn.negid
                    LEFT JOIN (select distinct on (rrp1.usucpf) * from par.rollresponsaveisprefeitos rrp1 ) rrp ON egn.usucpf = rrp.usucpf
                    LEFT JOIN (select distinct on (rrs1.usucpf) * from par.rollresponsaveissecretarios rrs1 ) rrs ON egn.usucpf = rrs.usucpf
                    LEFT JOIN par.documentopar dop_par ON opc.prpid = dop_par.prpid and dop_par.dopstatus = 'A'
                    LEFT JOIN par.documentopar dop_obras ON opc.proid_par = dop_obras.proid and dop_obras.dopstatus = 'A'
                    LEFT JOIN par.termocompromissopac ter ON opc.proid_pac = ter.proid and ter.terstatus = 'A'
                    LEFT JOIN workflow.documento doc ON egn.docid = doc.docid
                    LEFT JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid

                    where egn.usucpf = '".$arrEgn['usucpf']."' ORDER BY opc.opcid";
            
            $sqlUsuario = "SELECT DISTINCT ON (rrp.usucpf) rrp.usucpf AS cpf, rrp.rrpnomeprefeito AS nome, rrp.rrpemail AS email FROM par.rollresponsaveisprefeitos rrp
                            WHERE rrp.usucpf = '".$arrEgn['usucpf']."'
                            UNION ALL
                            SELECT DISTINCT ON (rrs.usucpf) rrs.usucpf AS cpf, rrs.rrsnomesecretario AS nome, rrs.rrsemail AS email FROM par.rollresponsaveissecretarios rrs
                            WHERE rrs.usucpf = '".$arrEgn['usucpf']."'";
//             ver($sqlTermo);
            $dadosTermo = $this->carregar($sqlTermo);
            $dadosUsuario = $this->pegaLinha($sqlUsuario);
            
        }
        
        $tokenValido = (is_array($dadosUsuario) && is_array($dadosTermo) && $arrEgn);
        
        $dadosUsuario = (is_array($dadosUsuario)) ? $dadosUsuario : Array();
        $dadosTermo = (is_array($dadosTermo)) ? $dadosTermo : Array();


        return array("tokenValido" => $tokenValido, "dadosUsuario" => $dadosUsuario, "dadosTermo" => $dadosTermo);
    }
    
    
    public function confirmarCiencia($egnid){
        
        //Cria um novo DOCID
        $sql = "INSERT INTO workflow.documento (tpdid, esdid, docdsc, docdatainclusao)
											VALUES (".TPDID_FLUXO_PC_EX_GESTOR.", ".ESDID_PC_NAO_ENVIADA.", 'Ex-gestor Prestação de contas - egnid: {$egnid}', now()) returning docid ";
        
        $docid = $this->pegaUm($sql);

//         Atualiza tabela aplicando a ciêcia
        $update = "update par.exgestorpcnotificado set egnciencia = true, egndataciencia = now(), docid = {$docid} where egnid = {$egnid}";
        
        if($this->executar($update)){
            $this->commit();
        }
        return $docid;
        
    }
    
    public function salvarUploadPC($arqid, $egnid, $descricao = ''){
        $update = "update par.exgestorpcnotificado set arqid = {$arqid}, egndataenviopc = now(), egnarquivodescricao = '{$descricao}' where egnid = {$egnid}";
        
        if($this->executar($update)){
            $this->commit();
        }
    }
    
    
    public function salvarDadosCpfReceita($pessoa){

        $update = "INSERT INTO par.exgestorentepc (usucpf, eepnome, eepnomemae, eepdatanascimento) values('{$pessoa['nu_cpf_rf']}', '{$pessoa['no_pessoa_rf']}', '{$pessoa['no_mae_rf']}', '{$pessoa['dt_nascimento_rf']}')";
        
        if($this->executar($update)){
            $this->commit();
        }
    }
    
}
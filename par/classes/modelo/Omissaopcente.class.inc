<?php
/**
 * Classe de mapeamento da entidade par.omissaopcente.
 *
 * @version $Id$
 * @since 2018.07.24
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par_Model_Omissaopcente: Tabela responsável por armazenar os dados da omissão do envio da Prestação de Contas por parte do Ente
 *
 * @package Par\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *        
 * @example <code>
 *          // -- Consultando registros
 *          $model = new Par_Model_Omissaopcente($valorID);
 *          var_dump($model->getDados());
 *         
 *          // -- Alterando registros
 *          $valores = ['campo' => 'valor'];
 *          $model = new Par_Model_Omissaopcente($valorID);
 *          $model->popularDadosObjeto($valores);
 *          $model->salvar(); // -- retorna true ou false
 *          $model->commit();
 *          </code>
 *         
 * @property int $opcid Primary key da tabela - default: nextval('par.omissaopcente_opcid_seq'::regclass)
 * @property \Datetime(Y-m-d H:i:s) $opcdataomissao Data de registro da Omissão
 * @property int $prpid FK do processo par.processopar
 * @property int $inuid
 * @property bool $opcciencia Declaração de ciência por parte do ente - default: false
 * @property string $opccpfciencia CPF de quem declarou ciência
 * @property \Datetime(Y-m-d H:i:s) $opcdataciencia Data da declaração de ciência
 * @property string $opcstatus - default: 'A'::bpchar
 */
class Par_Model_Omissaopcente extends Modelo
{

    /**
     *
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.omissaopcente';

    /**
     *
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'opcid'
    );

    /**
     *
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array(
            'tabela' => 'par.instrumentounidade',
            'pk' => 'inuid'
        ),
        'prpid' => array(
            'tabela' => 'par.processopar',
            'pk' => 'prpid'
        )
    );

    /**
     *
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'opcid' => null,
        'opcdataomissao' => null,
        'prpid' => null,
        'inuid' => null,
        'opcciencia' => null,
        'opccpfciencia' => null,
        'opcdataciencia' => null,
        'opcstatus' => null
    );

    /**
     */
    public function insereCienciaOmissao($arrParams)
    {
        $cpf = $arrParams['usucpf'];
        $inuid = $arrParams['inuid'];
        
        $sql = "
            UPDATE 
            	par.omissaopcente 
            SET
            	opcciencia = TRUE,
            	opccpfciencia = '{$cpf}',
            	opcdataciencia = 'now()'
            WHERE inuid = {$inuid}";
        
        $executa = $this->executar($sql);
        
        if ($executa) {
            return $this->commit();
        } else {
            return false;
        }
    }

    /**
     */
    public function isPrefeitoPar($usucpf)
    {
        if ($usucpf != '') {
            
            include_once APPRAIZ . "www/par/_constantes.php";
            $sql = "select usucpf from par.usuarioresponsabilidade where usucpf = '$usucpf' and rpustatus = 'A' AND pflcod in (".PAR_PERFIL_PREFEITO." ,".PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO.")";
            $executa = $this->pegaUm($sql);
            return ($executa) ? TRUE : FALSE;
        } else {
            return false;
        }
        
        if ($executa) {
            return $this->commit();
        } else {
            return false;
        }
    }
    
    /**
     */
    public function getListaPrincipalATramitar($arrParams)
    {
        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        
        /*Usado para testes, para simular data mais próxima*/
        /*if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost"  || $_SERVER['HTTP_HOST'] == "dsv-simec" || $_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br")
        {
            //@TODO TESTE!
            $dpcidRotina = 3;
            	
        }
        else{
            //@TODO PRODUÇÃO!
            $dpcidRotina = 2;
        }*/
        $dpcidRotina = 2;
        /*Pega quantidade de dias*/
        $dpcquantidadedias = $this->pegaUm("SELECT dpcquantidadedias from par.datasprestacaocontas WHERE dpcid = $dpcidRotina");
        /*Caso retorne verifica processos passíveis de serem colocados como inadimplentes*/
        
        if($dpcquantidadedias != '' )
        {
            $sql = "
               WITH wf_documento_e_estado AS (
                	SELECT docid, doc.esdid FROM workflow.documento doc
                	
                	where doc.tpdid in (336, 345, 335, 344, 303,328) 
        		)

                SELECT DISTINCT
                    CASE WHEN opc.prpid IS NOT NULL THEN
                    	'PAR'
                    WHEN opc.proid_par IS NOT NULL THEN
                    	'OBRAS_PAR'
                    WHEN opc.proid_pac IS NOT NULL THEN
                    	'PAC'	
                    END as tipo,
                    
                    CASE WHEN opc.prpid IS NOT NULL THEN
                    	opc.prpid
                    WHEN opc.proid_par IS NOT NULL THEN
                    	opc.proid_par
                    WHEN opc.proid_pac IS NOT NULL THEN
                    	opc.proid_pac
                    END as id,
                    
                    CASE WHEN opc.prpid IS NOT NULL THEN
                    	doc1.esdid 
                    WHEN opc.proid_par IS NOT NULL THEN
                    	doc2.esdid 
                    WHEN opc.proid_pac IS NOT NULL THEN
                    	doc3.esdid 
                    END	 as situacao_opc,
                    
                    CASE WHEN opc.prpid IS NOT NULL THEN
                    doc11.esdid 
                    WHEN opc.proid_par IS NOT NULL THEN
                    	doc22.esdid 
                    WHEN opc.proid_pac IS NOT NULL THEN
                    	doc33.esdid 
                    END	 as situacao_pc,
                    
                    CASE WHEN opc.prpid IS NOT NULL THEN
                    	doc1.docid 
                    WHEN opc.proid_par IS NOT NULL THEN
                    	doc2.docid 
                    WHEN opc.proid_pac IS NOT NULL THEN
                    	doc3.docid 
                    END	 as docid
                    
                FROM 
                    par.omissaopcente opc
                
                LEFT JOIN par.situacaoopc sop ON sop.prpid = opc.prpid AND sop.sopstatus = 'A'
                LEFT JOIN wf_documento_e_estado doc1 ON doc1.docid = sop.docid
                LEFT JOIN par.situacaoprestacaocontas sit1 ON sit1.prpid = opc.prpid AND sit1.spcstatus = 'A'
                LEFT JOIN wf_documento_e_estado doc11 ON doc11.docid = sit1.docid
                LEFT JOIN par.documentoparprestacaodecontas dpc ON dpc.prpid = opc.prpid AND dpc.dppstatus = 'A'
                LEFT JOIN wf_documento_e_estado doc111 ON doc111.docid = dpc.docid
                
                LEFT JOIN obras2.situacaoopcobras sop2 ON sop2.proid_par = opc.proid_par  AND sop2.sopstatus = 'A'
                LEFT JOIN wf_documento_e_estado doc2 ON doc2.docid = sop2.docid
                LEFT JOIN obras2.situacaoprestacaocontasobras  sit2 ON sit2.proid_par = opc.proid_par AND sit2.spostatus = 'A'
                LEFT JOIN wf_documento_e_estado doc22 ON doc22.docid = sit2.docid
                LEFT JOIN obras2.execucaofinanceira exec ON exec.proid_par = opc.proid_par AND exec.exestatus = 'A'
                LEFT JOIN wf_documento_e_estado doc222 ON doc222.docid = exec.docid
                
                LEFT JOIN obras2.situacaoopcobras sop3 ON sop3.proid_pac = opc.proid_pac AND sop3.sopstatus = 'A'
                LEFT JOIN wf_documento_e_estado doc3 ON doc3.docid = sop3.docid
                LEFT JOIN obras2.situacaoprestacaocontasobras  sit3 ON sit3.proid_pac = opc.proid_pac AND sit3.spostatus = 'A'
                LEFT JOIN wf_documento_e_estado doc33 ON doc33.docid = sit3.docid
                LEFT JOIN obras2.execucaofinanceira exec2 ON exec2.proid_pac = opc.proid_pac AND exec2.exestatus = 'A'
                LEFT JOIN wf_documento_e_estado doc333 ON doc333.docid = exec2.docid
                
                WHERE
                    CASE WHEN opc.prpid IS NOT NULL THEN
                    	doc1.esdid = ".ESDID_OPC_ADIMPLENTE."
                    WHEN opc.proid_par IS NOT NULL THEN
                    	doc2.esdid = ".ESDID_SITUACAO_PC_ADIMPLENTE."
                    WHEN opc.proid_pac IS NOT NULL THEN
                    	doc3.esdid = ".ESDID_SITUACAO_PC_ADIMPLENTE."
                    ELSE
                    	false
                    END
                
                AND
                    CASE WHEN opc.prpid IS NOT NULL THEN
                    	doc11.esdid = ".ESDID_NOTIFICADA_POR_OMISSAO."
                    WHEN opc.proid_par IS NOT NULL THEN
                    	doc22.esdid = ".ESDID_OBRAS_NOTIFICADA_POR_OMISSAO."
                    WHEN opc.proid_pac IS NOT NULL THEN
                    	doc33.esdid = ".ESDID_OBRAS_NOTIFICADA_POR_OMISSAO."
                    ELSE
                    	false
                    END
                AND
                    CASE WHEN opc.prpid IS NOT NULL THEN
                    	doc111.esdid = ".ESDID_AGUARDANDO_ENVIO_PROPONENTE."
                    WHEN opc.proid_par IS NOT NULL THEN
                    	doc222.esdid = ".ESDID_PC_OBRAS_EM_CADASTRAMENTO."
                    WHEN opc.proid_pac IS NOT NULL THEN
                    	doc333.esdid = ".ESDID_PC_OBRAS_EM_CADASTRAMENTO."
                    ELSE
                    	false
                    END
                AND 
                    opcstatus = 'A'
                /*VALIDAÇÃO SIGPC*/
		        AND 
			
                    CASE WHEN opc.prpid IS NOT NULL THEN
                    	opc.prpid NOT IN (select distinct sigpc.prpid from par.termossigpcpar sigpc WHERE sigpc.prpid is not null)
                    WHEN opc.proid_par IS NOT NULL THEN
                    	opc.proid_par NOT IN (select distinct sigpc.proid_par from par.termossigpcpar sigpc WHERE sigpc.proid_par is not null)
                    WHEN opc.proid_pac IS NOT NULL THEN
                    	opc.proid_pac NOT IN (select distinct sigpc.proid_pac from par.termossigpcpar sigpc WHERE sigpc.proid_pac is not null)
                    ELSE
                    	false
                    END
                
                AND 
                    COALESCE(((opcdataciencia::date - current_date)*-1),0) > {$dpcquantidadedias} -- passou_prazo_ciencia,
                /*and opc.proid_pac is not null --@todo apenas testes*/
                {$limite} 
                ";
                
                $arrRegistros = $this->carregar($sql); 
                
                // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
                $retornou = (is_array($arrRegistros)) ? TRUE : FALSE;
                $registros = (is_array($arrRegistros)) ? $arrRegistros : Array();
                $arrReturn = Array(
                    'existeRegistro' => $retornou,
                    'arrProcessos' => $registros
                );
        }else {
            // Caso de erro na data, retorna falso, para evitar erro na query
            $arrReturn = Array(
                'existeRegistro' => FALSE,
                'arrProcessos' => Array()
            );
        }
        return $arrReturn;
            
    }

    /**
     */
    public function verificaExistenciaOmissaoNaoAssinada($inuid)
    {
        $sql = "SELECT 
                    opcid 
                FROM
                    par.omissaopcente opc 
                WHERE 
                    inuid = {$inuid} 
                AND opcstatus = 'A' 
                AND opcciencia = FALSE
                ";
        
        $omissao = $this->pegaUm($sql);
        $omissao = ($omissao) ? TRUE : FALSE;
        return $omissao;
    }

    function getDadosUnidade($inuid = '')
    {
        global $db;
        include_once APPRAIZ . "www/par/_constantes.php";
        if ($inuid != '') {
            
            $arDadosProcesso = $this->pegaLinha("
                SELECT 
                    distinct iu.itrid, iu.inuid 
                FROM        
                    par.instrumentounidade iu 
                where iu.inuid = {$inuid}");
        }
        
        if ($arDadosProcesso['itrid'] == 2 && $arDadosProcesso['inuid'] != 1) {
            // CNPJ da prefeitura
            $arrSecretaria = $this->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome,
    
				CASE WHEN (length(ent2.entnumcpfcnpj) > 11) THEN
					substr(ent2.entnumcpfcnpj, 1, 2) || '.' || substr(ent2.entnumcpfcnpj, 3, 3) || '.' ||
					substr(ent2.entnumcpfcnpj, 6, 3) || '/' || substr(ent2.entnumcpfcnpj, 9, 4) || '-' ||
					substr(ent2.entnumcpfcnpj, 13)
				ELSE
                    ent2.entnumcpfcnpj
				END
					as entnumcpfcnpj, ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.mundescricao, mun.estuf, mun.muncod,
                                                                                                      ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
                                                                                            FROM  par.entidade ent
                                                                                            INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = " . DUTID_PREFEITURA . "
                                                                                            INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
                                                                                            where
                                                                                                    ent.dutid = " . DUTID_PREFEITO . " AND
                                                                                                    ent.entstatus='A' AND
                                                                                                    ent2.entstatus='A' AND
                                                                                                    ent.inuid = " . $arDadosProcesso['inuid']);
        } else {
            // CNPJ do municipio
            $arrSecretaria = $this->pegaLinha("SELECT ent.entid, ent2.entid, ent2.entnome,
    
				CASE WHEN (length(ent2.entnumcpfcnpj) > 11) THEN
					substr(ent2.entnumcpfcnpj, 1, 2) || '.' || substr(ent2.entnumcpfcnpj, 3, 3) || '.' ||
					substr(ent2.entnumcpfcnpj, 6, 3) || '/' || substr(ent2.entnumcpfcnpj, 9, 4) || '-' ||
					substr(ent2.entnumcpfcnpj, 13)
				ELSE
                    ent2.entnumcpfcnpj
				END
					as entnumcpfcnpj,
    
				ent2.endlog, ent2.endcep, ent2.endnum, ent2.endbai, mun.estdescricao as mundescricao, mun.estuf,
                                                                    ent.entnumcpfcnpj as cpfsecretario, ent.entnome as secretario
                                                            FROM  par.entidade ent
                                                            INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = " . DUTID_SECRETARIA_ESTADUAL . "
                                                            INNER JOIN territorios.estado mun on mun.estuf = ent2.estuf
                                                            where
                                                                    ent.dutid = " . DUTID_SECRETARIO_ESTADUAL . " AND
                                                                    ent.entstatus='A' AND
                                                                    ent2.entstatus='A' AND
                                                                    ent.inuid = " . $arDadosProcesso['inuid']);
        }
        
        $arrSecretaria = (is_array($arrSecretaria)) ? array('dadosUnidade' => $arrSecretaria, 'dadosInstrumento' => $arDadosProcesso) : Array();
        return $arrSecretaria;
    }

    /**
     */
    public function retornaInuidPar($cpf, $origem)
    {
        if (!empty($cpf) && !empty($origem)) {
            $sql = "
                SELECT 
                    CASE WHEN ent.tpcid = 1 THEN 
                        'estadual'
                    ELSE
                        'municipal'
                    END as esfera,
                    ende.estuf,
                    ende.muncod
                FROM 
                    {$origem}.usuarioresponsabilidade  rpu
                INNER JOIN entidade.entidade ent ON ent.entid = rpu.entid AND entstatus = 'A'
                INNER JOIN entidade.endereco ende ON ende.entid = rpu.entid AND ende.endstatus = 'A'
                WHERE
                    rpu.usucpf = '{$cpf}'
                AND 
                    rpustatus = 'A'
                ORDER BY endid DESC
                LIMIT 1
            ";

            $dadosObras = $this->pegaLinha($sql);
            $esfera = $dadosObras['esfera'];
            $estuf = $dadosObras['estuf'];
            $muncod = $dadosObras['muncod'];

            if ($esfera == 'estadual') {
                if (!empty($estuf)) {
                    $sqlInuid = "SELECT inuid FROM par.instrumentounidade  WHERE estuf = '{$estuf}'";
                }
            } else {
                if (!empty($muncod)) {
                    $sqlInuid = "SELECT inuid FROM par.instrumentounidade  WHERE muncod = '{$muncod}'";
                }
            }

            $inuid = '';

            if (!empty($sqlInuid)) {
                $inuid = $this->pegaUm($sqlInuid);
            }
            return ($inuid) ? $inuid : '';
        } else {
            return '';
        }
    }

    function pegaArrayPerfil($usucpf)
    {
        $sql = "SELECT
        pu.pflcod
        FROM
        seguranca.perfil AS p
        LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
        WHERE
        p.sisid = '{$_SESSION['sisid']}'
        AND pu.usucpf = '$usucpf'";
        
        $pflcod = $this->carregarColuna($sql);
        
        return $pflcod;
    }

    /**
     */
    public function retornaListaProcessosComOmissao($inuid, $naoAssinada = true)
    {
        if ($inuid != '') {
            
            $whereAssinada = $naoAssinada? " AND opcciencia = FALSE " : "";
            
            $sql = "
                
                  SELECT DISTINCT
                	CASE WHEN prp.prpid IS NOT NULL THEN
                		'PAR'
                	WHEN pro_par.proid IS NOT NULL THEN
                		'OBRAS PAR' 
                	WHEN pro_pac.proid IS NOT NULL THEN
                		'OBRAS PAC'
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
                	WHEN pro_pac.proid IS NOT NULL THEN
                		substring(pro_pac.pronumeroprocesso from 1 for 5)||'.'||
                		substring(pro_pac.pronumeroprocesso from 6 for 6)||'/'||
                		substring(pro_pac.pronumeroprocesso from 12 for 4)||'-'||
                		substring(pro_pac.pronumeroprocesso from 16 for 2) || ''
                	END as processo,
                	CASE WHEN prp.prpid IS NOT NULL THEN
                		substring(prpcnpj, 1, 2) || '.' || substring(prpcnpj, 3, 3) || '.' ||
                		substring(prpcnpj, 6, 3) || '/' || substring(prpcnpj, 9, 4) || '-' ||
                		substring(prpcnpj, 13)
                	WHEN pro_par.proid IS NOT NULL THEN
                		
                		substring(pro_par.procnpj, 1, 2) || '.' || substring(pro_par.procnpj, 3, 3) || '.' ||
                		substring(pro_par.procnpj, 6, 3) || '/' || substring(pro_par.procnpj, 9, 4) || '-' ||
                		substring(pro_par.procnpj, 13)
                	WHEN pro_pac.proid IS NOT NULL THEN
                		substring(pro_pac.procnpj, 1, 2) || '.' || substring(pro_pac.procnpj, 3, 3) || '.' ||
                		substring(pro_pac.procnpj, 6, 3) || '/' || substring(pro_pac.procnpj, 9, 4) || '-' ||
                		substring(pro_pac.procnpj, 13)
                	END as cnpj_entidade,
                	(SELECT ent.entnome FROM par.entidade ent WHERE ent.entstatus = 'A' AND ent.inuid = opc.inuid AND ent.dutid in( 9,6) limit 1) as nome_entidade,
                
                	CASE WHEN prp.prpid IS NOT NULL THEN
                		dop.dopnumerodocumento
                	WHEN pro_par.proid IS NOT NULL THEN
                		dop2.dopnumerodocumento
                	WHEN pro_pac.proid IS NOT NULL THEN
                		ter.ternumero
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

                	WHEN pro_pac.proid IS NOT NULL THEN

                		coalesce( (	SELECT foo2.totalpagamento FROM
					(SELECT
						sum(CASE WHEN pag3.pagsituacaopagamento = '2 - EFETIVADO' THEN ps3.pobvalorpagamento ELSE 0 END) AS totalpagamento
					FROM
						par.processoobra 	pro1 
					INNER JOIN par.empenho 			emp3 ON emp3.empnumeroprocesso = pro1.pronumeroprocesso AND emp3.empcodigoespecie NOT IN ('03', '13', '02', '04') AND emp3.empstatus = 'A'
					INNER JOIN par.pagamento 		pag3 ON pag3.empid = emp3.empid AND pag3.pagstatus = 'A' AND pag3.pagsituacaopagamento not ilike '%CANCELADO%' and pag3.pagsituacaopagamento not ilike '%vala siaf%' and pag3.pagsituacaopagamento not ilike '%devolvido%'
					INNER JOIN par.pagamentoobra 	ps3  ON ps3.pagid = pag3.pagid
					WHERE pro1.proid = pro_pac.proid and pro1.prostatus = 'A' GROUP BY pro1.proid ) foo2
				),0)
				
				
                	END
                	
                	 as valor_repassado,
                	CASE WHEN prp.prpid IS NOT NULL THEN
                		-- CASO TENHA SIDO VENCIDO ATÉ A DATA DO PRIMEIRO LOTE 02/07/2018
                		CASE WHEN (date_trunc('MONTH', to_date(dop.dopdatafimvigencia,'MM/YYYY')) + INTERVAL ' 1 MONTH - 1 day') < '2018-07-02' THEN
					-- LOTE 1 + 60
					to_char(  to_date('02/07/2018','DD/MM/YYYY') + 60 , 'DD/MM/YYYY')
				ELSE
				-- SENÃO, vigência + 60
					to_char( (date_trunc('MONTH', to_date(dop.dopdatafimvigencia,'MM/YYYY')) + INTERVAL ' 1 MONTH - 1 day')::date + 60 , 'DD/MM/YYYY')
				END
				
                	WHEN pro_par.proid IS NOT NULL THEN
				-- CASO TENHA SIDO VENCIDO ATÉ A DATA DO PRIMEIRO LOTE 02/07/2018
                		CASE WHEN to_date(par.retornavigenciaobrapar(pro_par.proid),'DD/MM/YYYY') < '2018-07-02' THEN
					-- LOTE 1 + 60
					to_char(  to_date('02/07/2018','DD/MM/YYYY') + 60, 'DD/MM/YYYY')
				ELSE
				-- SENÃO, vigência + 60
					to_char( to_date(par.retornavigenciaobrapar(pro_par.proid),'DD/MM/YYYY') + 60, 'DD/MM/YYYY')
				END
                	WHEN pro_pac.proid IS NOT NULL THEN
                		
                		-- CASO TENHA SIDO VENCIDO ATÉ A DATA DO SEGUNDO LOTE 02/08/2018
                		CASE WHEN to_char(ter.terdatafimvigencia,'DD/MM/YYYY') < '2018-08-02' THEN
					-- LOTE 2 + 60
					to_char((to_date('02/08/2018','DD/MM/YYYY') + 60)::DATE,  'DD/MM/YYYY')
				ELSE
					-- SENÃO, vigência + 60
					to_char( to_date(to_char( ter.terdatafimvigencia, 'DD/MM/YYYY'),'DD/MM/YYYY') + 60, 'DD/MM/YYYY')
				END
                	END as prazo_prestar_contas,

                	
                	CASE WHEN prp.prpid IS NOT NULL THEN
                		dop.dopdatainiciovigencia || ' - '|| dop.dopdatafimvigencia
                	WHEN pro_par.proid IS NOT NULL THEN 
                		dop2.dopdatainiciovigencia ||' - '|| par.retornavigenciaobrapar(pro_par.proid)
                	WHEN pro_pac.proid IS NOT NULL THEN
                		(select to_char(min(terdatainclusao), 'DD/MM/YYYY') from par.termocompromissopac ppp1 where  ppp1.proid = pro_pac.proid) ||' - '||  to_char(ter.terdatafimvigencia,'DD/MM/YYYY') 
                	END as prazo_vigencia, 

                	CASE WHEN prp.prpid IS NOT NULL THEN
                		par.retornasaldoprocesso(prpnumeroprocesso) 
                	WHEN pro_par.proid IS NOT NULL THEN
                		 par.retornasaldoprocesso(pro_par.pronumeroprocesso) 
                	WHEN pro_pac.proid IS NOT NULL THEN
                		 par.retornasaldoprocesso(pro_pac.pronumeroprocesso)
                	END as saldo, 
                	
                	CASE WHEN prp.prpid IS NOT NULL THEN
                		(SELECT mdonome FROM par.modelosdocumentos  where mdoid = dop.mdoid)
                	WHEN pro_par.proid IS NOT NULL THEN
                		(SELECT mdonome FROM par.modelosdocumentos  where mdoid = dop2.mdoid)
                	WHEN pro_pac.proid IS NOT NULL THEN
                		'PAC'
                	END as programa
                	
                	
                FROM
                	par.omissaopcente opc
                	LEFT JOIN par.processopar prp ON prp.prpid = opc.prpid 
                	LEFT JOIN par.documentopar dop ON dop.prpid = prp.prpid AND dop.dopstatus = 'A'
                	LEFT JOIN par.processoobraspar   pro_par ON pro_par.proid = opc.proid_par
                	LEFT JOIN par.documentopar dop2 ON dop2.proid = pro_par.proid AND dop2.dopstatus = 'A'
                	LEFT JOIN par.processoobra  pro_pac ON pro_pac.proid = opc.proid_pac
                	LEFT JOIN par.termocompromissopac ter ON ter.proid = pro_pac.proid AND terstatus = 'A'
                WHERE 
                    opc.inuid = {$inuid} 
                AND opcstatus = 'A'
                {$whereAssinada}
                
                ";
        } else {
            return Array();
        }
        
        $ListaOmissao = $this->carregar($sql);
        $ListaOmissao = ($ListaOmissao) ? $ListaOmissao : Array();
        
        return $ListaOmissao;
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados            
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'opcid' => array(
                'Digits'
            ),
            'opcdataomissao' => array(
                'allowEmpty' => true
            ),
            'prpid' => array(
                'allowEmpty' => true,
                'Digits'
            ),
            'inuid' => array(
                'allowEmpty' => true,
                'Digits'
            ),
            'opcciencia' => array(
                'allowEmpty' => true
            ),
            'opccpfciencia' => array(
                'allowEmpty' => true,
                new Zend_Validate_StringLength(array(
                    'max' => 11
                ))
            ),
            'opcdataciencia' => array(
                'allowEmpty' => true
            ),
            'opcstatus' => array(
                'allowEmpty' => true,
                new Zend_Validate_StringLength(array(
                    'max' => 1
                ))
            )
        );
    }

    /**
     * Validators.
     *
     * @param
     *            $arrParams
     * @return
     *
     */
    public function insereRegistro($arrParams = array())
    {
        $prpid = $arrParams['prpid'];
        $inuid = $arrParams['inuid'];
        if (($prpid != '') && ($inuid != '')) {
            $sql = "
                INSERT INTO par.omissaopcente
                ( opcdataomissao, prpid, inuid)
                VALUES
                ('now()', {$prpid}, {$inuid})
                RETURNING opcid
                ;";
            $id = $this->pegaUm($sql);
            return $id;
        } else {
            return false;
        }
    }
    
    /**
     * Validators.
     *
     * @param
     *            $arrParams
     * @return
     *
     */
    public function insereRegistroOmissaoCacs($arrParams = array())
    {
        $prpid = $arrParams['prpid'];
        $inuid = $arrParams['inuid'];
        if (($prpid != '') && ($inuid != '')) {
            $sql = "
                INSERT INTO par.omissaopccacs
                    ( occdataomissao, prpid, inuid, ropid)
                VALUES
                    ('now()', {$prpid}, {$inuid}, 1)
                RETURNING occid
            ;";
            $id1 = $this->pegaUm($sql);
            
            $sql = "
                INSERT INTO par.omissaopccacs
                    ( occdataomissao, prpid, inuid, ropid)
                VALUES
                    ('now()', {$prpid}, {$inuid}, 2)
                RETURNING occid
            ;";
            $id2 = $this->pegaUm($sql);
            
            $retorno = (($id1) && ($id2)) ? TRUE : FALSE;
            return $retorno;
        } else {
            return false;
        }
    }

    /**
     * Validators.
     *
     * @param
     *            $arrParams
     * @return
     *
     */
    public function insereRegistroObrasPar($arrParams = array())
    {
        $proid = $arrParams['proid'];
        $inuid = $arrParams['inuid'];
        if (($proid != '') && ($inuid != '')) {
            $sql = "
            INSERT INTO par.omissaopcente
            ( opcdataomissao, proid_par, inuid)
            VALUES
            ('now()', {$proid}, {$inuid})
            RETURNING opcid
            ;";
            $id = $this->pegaUm($sql);
            return $id;
        } else {
            return false;
        }
    }
    
    /**
     * Validators.
     *
     * @param
     *            $arrParams
     * @return
     *
     */
    public function inativaRegistroObrasParCACS($proid = '')
    {
        if( $proid != '' ) {
            $sql = "UPDATE par.omissaopccacs SET occstatus = 'I' WHERE proid_par = {$proid}";
            $this->executar($sql);
            return $this->commit();
        }
    }
    
    /**
     * Validators.
     *
     * @param
     *            $arrParams
     * @return
     *
     */
    public function insereRegistroObrasParCACS($arrParams = array())
    {
        $proid = $arrParams['proid'];
        $inuid = $arrParams['inuid'];
        if (($proid != '') && ($inuid != '')) {
            
            $sql = "
            INSERT INTO par.omissaopccacs
                ( occdataomissao, proid_par, inuid, ropid)
            VALUES
                ('now()', {$proid}, {$inuid}, 1)
            RETURNING occid
            ;";
            $id1 = $this->pegaUm($sql);
            
            $sql = "
            INSERT INTO par.omissaopccacs
                ( occdataomissao, proid_par, inuid, ropid)
            VALUES
                ('now()', {$proid}, {$inuid}, 2)
            RETURNING occid
            ;";
            $id2 = $this->pegaUm($sql);
            
            $retorno = (($id1) && ($id2)) ? TRUE : FALSE;
            return $retorno;
        } else {
            return false;
        }
    }

    /**
     * Validators.
     *
     * @param
     *            $arrParams
     * @return
     *
     */
    public function insereRegistroObrasPac($arrParams = array())
    {
        $proid = $arrParams['proid'];
        $inuid = $arrParams['inuid'];
        if (($proid != '') && ($inuid != '')) {
            $sql = "
            INSERT INTO par.omissaopcente
            ( opcdataomissao, proid_pac, inuid)
            VALUES
            ('now()', {$proid}, {$inuid})
            RETURNING opcid
            ;";
            $id = $this->pegaUm($sql);
            return $id;
        } else {
            return false;
        }
    }

    /**
     * Validators.
     *
     * @param
     *            $arrParams
     * @return
     *
     */
    public function confirmaRegistros()
    {
        return $this->commit();
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
}

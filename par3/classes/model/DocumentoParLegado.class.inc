<?php
/**
 * Classe de mapeamento da entidade par.documentopar
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-10-2015
 * @link     no link
 */



/**
 * Par3_Model_DocumentoparLegado
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Par3_Model_DocumentoParLegado extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par.documentopar';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'dopid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpfinclusao' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'usucpfalteracao' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'mdoid' => array('tabela' => 'par.modelosdocumentos', 'pk' => 'mdoid'),
        'prpid' => array('tabela' => 'par.processopar', 'pk' => 'prpid'),
        'iueid' => array('tabela' => 'par.instrumentounidadeentidade', 'pk' => 'iueid'),
        'arqid_documento' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dopid' => null,
        'prpid' => null,
        'doptexto' => null,
        'dopstatus' => null,
        'dopdiasvigencia' => null,
        'dopdatainicio' => null,
        'dopdatafim' => null,
        'mdoid' => null,
        'dopdatainclusao' => null,
        'usucpfinclusao' => null,
        'dopdataalteracao' => null,
        'usucpfalteracao' => null,
        'dopjustificativa' => null,
        'dopdatavalidacaofnde' => null,
        'dopusucpfvalidacaofnde' => null,
        'dopdatavalidacaogestor' => null,
        'dopusucpfvalidacaogestor' => null,
        'dopusucpfstatus' => null,
        'dopdatastatus' => null,
        'dopdatapublicacao' => null,
        'doppaginadou' => null,
        'dopnumportaria' => null,
        'proid' => null,
        'dopreformulacao' => null,
        'dopidpai' => null,
        'dopidaditivo' => null,
        'dopnumerodocumento' => null,
        'dopobservacao' => null,
        'dopdatafimvigencia' => null,
        'dopvalortermo' => null,
        'dopdatainiciovigencia' => null,
        'arqid' => null,
        'dopacompanhamento' => null,
        'dopano' => null,
        'dopdocumentocarregado' => null,
        'iueid' => null,
        'arqid_documento' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'dopid' => array(  'Digits'  ),
            'prpid' => array( 'allowEmpty' => true, 'Digits'  ),
            'doptexto' => array( 'allowEmpty' => true ),
            'dopstatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'dopdiasvigencia' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopdatainicio' => array( 'allowEmpty' => true ),
            'dopdatafim' => array( 'allowEmpty' => true ),
            'mdoid' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopdatainclusao' => array( 'allowEmpty' => true ),
            'usucpfinclusao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopdataalteracao' => array( 'allowEmpty' => true ),
            'usucpfalteracao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopjustificativa' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 4000))  ),
            'dopdatavalidacaofnde' => array( 'allowEmpty' => true ),
            'dopusucpfvalidacaofnde' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopdatavalidacaogestor' => array( 'allowEmpty' => true ),
            'dopusucpfvalidacaogestor' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopusucpfstatus' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
            'dopdatastatus' => array( 'allowEmpty' => true ),
            'dopdatapublicacao' => array( 'allowEmpty' => true ),
            'doppaginadou' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))  ),
            'dopnumportaria' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 50))  ),
            'proid' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopreformulacao' => array( 'allowEmpty' => true ),
            'dopidpai' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopidaditivo' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopnumerodocumento' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopobservacao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))  ),
            'dopdatafimvigencia' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
            'dopvalortermo' => array( 'allowEmpty' => true ),
            'dopdatainiciovigencia' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
            'arqid' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopacompanhamento' => array( 'allowEmpty' => true ),
            'dopano' => array( 'allowEmpty' => true, 'Digits'  ),
            'dopdocumentocarregado' => array( 'allowEmpty' => true ),
            'iueid' => array( 'allowEmpty' => true, 'Digits'  ),
            'arqid_documento' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }//end getCamposValidacao($dados)


    /**
     * Função motnarFiltroPAR
     * - monta a Filtro para a lista de documentos PAR.
     *
     * @return escreve a lista.
     *
     */
    public function motnarFiltroPAR($arrPost)
    {
        $where = array();

        if ($arrPost['dopstatus'] != '') $where[] = "dopstatus = '{$arrPost['dopstatus']}'";

        if ($arrPost['inuid'] != '') $where[] = "inuid = {$arrPost['inuid']}";

        if ($arrPost['id'] != '') {
            $where[] = "id = (SELECT dopidpai FROM par.documentopar = {$arrPost['id']})";
        }

        if ($arrPost['tipo_doc'] != '') $where[] = "tipo_doc = {$arrPost['tipo_doc']}";

        return $where;

    }//end motnarFiltroPAR()


    /**
     * Função montarSQLPAR
     * - monta a SQL da lista de documentos PAR.
     *
     * @return escreve a montarSQLPAR.
     *
     */
    public function montarSQLPAR($arrPost)
    {
        $where = self::motnarFiltroPAR($arrPost);

        $sql = "SELECT
                    id,
                    inuid,
                    estuf,
                    muncod,
                    itrid,
                    dopidpai,
                	prpnumeroprocesso as processo,
                	doc,
                	'<span class=\"dopid\" value=' || id || '></span>' || tipodocumento as tipodocumento,
                	CASE
                		WHEN	( (date_trunc('MONTH', to_date(data_vigencia,'MM/YYYY')) + INTERVAL '1 MONTH - 1 day')::date - current_date) < 90
                			AND ( (date_trunc('MONTH', to_date(data_vigencia,'MM/YYYY')) + INTERVAL '1 MONTH - 1 day')::date - current_date) > 0 THEN
                			'<span style=\"color:red\">'	|| data_vigencia || '</span>'
                		ELSE
                			data_vigencia
                	END as data_vigencia,
                	dopvalortermo as vt,
                	valorempenho as ve,
                	--valorpagamentosolicitado as ps,
                	valorpagamento as vp,
                	dados_bancarios,
                	(
                		SELECT COALESCE(saldo) as saldo
                		FROM
                                    ( select par.retornasaldoprocesso(prpnumeroprocesso) AS saldo ) as saldomes
                	)  as sb,
                    arqid,
					CASE WHEN EXISTS ( SELECT tspid FROM par.termossigpcpar tss where tss.prpid = foo.prpid ) THEN
						
						'<a href=\"http://www.fnde.gov.br/sigpc\"  target=\"_blank\"> SIGPC </a>'	
                    
                        WHEN EXISTS( select to_char(htddata, 'DD/MM/YYYY') from workflow.historicodocumento where docid = (select docid from par.documentoparprestacaodecontas dpc  where dpc.prpid = foo.prpid) AND aedid=4939 ) THEN
							CASE WHEN EXISTS (select esdid from par.documentoparprestacaodecontas dpp11 INNER JOIN workflow.documento doc11 ON doc11.docid = dpp11.docid where dpp11.prpid = foo.prpid AND doc11.esdid <> 2005)
							THEN 
								'<button class=\" visualizar_item btn btn-success btn-sm\" type=\"button\" onclick=\"redirecionarPrestacaoContasPar2(' || id || ', ' || inuid || ', \' ' || estuf || ' \', ' || coalesce(muncod::text || ',','') || itrid || ')\">Enviada</button>'
							ELSE
								'Não Enviada'
							END
					ELSE				
						 'Não Enviada'	  
					END as situacao_prestacao_contas
                FROM
                (
                	SELECT
                		dp.dopid as id,
                		dp.dopidpai as dopidpai,
                		pp.prpnumeroprocesso,
                		dp.dopnumerodocumento as doc,
                		mdonome as tipodocumento,
                		(
                			SELECT
                				to_char(vigencia, 'MM/YYYY') -- seleciona maior vigência entre documento validado e ex-ofício
                			FROM (
                			       SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia --Seleciona maior vigência entre termos validados
                				      FROM par.documentopar  d
                				      INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid AND v.dpvstatus = 'A'
                				      WHERE d.prpid = pp.prpid --8146
                				      AND dopstatus <> 'E' AND dopdatafimvigencia IS NOT NULL
                				      AND mdoid NOT IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                			       UNION ALL
                			       SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia -- Seleciona maior vigência de Ex-Ofício
                				      FROM par.documentopar  d
                				      WHERE d.prpid = pp.prpid
                				      AND dopstatus <> 'E' AND dopdatafimvigencia IS NOT NULL
                				      AND mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                			) as foo
                			GROUP BY vigencia
                			ORDER BY vigencia DESC LIMIT 1
                		) as data_vigencia,
                		(SELECT dopvalortermo::numeric(20,2) FROM par.documentopar  d
                			INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid
                			WHERE prpid = pp.prpid
                			AND dopstatus <> 'E'
                			AND dpvstatus = 'A'
                			AND mdoid NOT IN (79,65,66,68,76,80,67,73,82)
                			ORDER BY d.dopid DESC
                			LIMIT 1
                		) as dopvalortermo,
                		COALESCE((select sum(vrlempenhocancelado) from par.v_vrlempenhocancelado where processo = pp.prpnumeroprocesso ), 0.00) as valorempenho,
                		pgs.valor_pagamento as valorpagamentosolicitado,
                		pm.valor_pagamento as valorpagamento,
                		'Banco: '||coalesce(prpbanco, 'n/a')||'<br> Conta: '||coalesce(prpagencia, 'n/a')||'<br> Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,

                		dp.arqid,
                		d.tpdcod as tipo_doc,
                		pp.prpfinalizado,
                		iu.inuid,
                		iu.estuf,
                		iu.muncod,
                		dp.dopstatus,
                		dp.prpid,
                		CASE WHEN iu.estuf = 'DF' 
                		  THEN 1
                		  ELSE iu.itrid
                		END as itrid
                	FROM
                		par.documentopar dp
                	INNER JOIN par.modelosdocumentos  d  ON d.mdoid = dp.mdoid
                	INNER JOIN par.processopar 		  pp ON pp.prpid = dp.prpid
                	INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
                	LEFT  JOIN (
                		SELECT
                		    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
                		FROM
                		    par.documentopar d
                		INNER JOIN par.processopar 		prp on prp.prpid = d.prpid
                		INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                		INNER JOIN par.pagamento 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
                		INNER JOIN par.pagamentosubacao 	ps  ON ps.pagid = pag.pagid and pobstatus = 'A'
                		WHERE d.dopstatus = 'A'
                		GROUP BY d.dopid, pagsituacaopagamento
                		) 				              pm ON pm.dopid = dp.dopid
                	LEFT  JOIN (
                		SELECT
                		    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
                		FROM
                		    par.documentopar d
                		INNER JOIN par.processopar 		prp on prp.prpid = d.prpid
                		INNER JOIN par.empenho 			emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                		INNER JOIN par.pagamento 		pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
                		INNER JOIN par.pagamentosubacao ps  on ps.pagid = pag.pagid and pobstatus = 'A'
                		WHERE d.dopstatus = 'A'
                		GROUP BY d.dopid, pagsituacaopagamento
                		)                             pgs ON pgs.dopid = dp.dopid
                ) as foo
                WHERE
                	".implode(' AND ', $where);
        return $sql;
    }//end listaPAR()


    /**
     * Função motnarFiltroObrasPAR
     * - monta a Filtro para a lista de documentos PAR.
     *
     * @return escreve a lista.
     *
     */
    public function motnarFiltroObrasPAR($arrPost)
    {
        $where = array();

        if ($arrPost['inuid'] != '') $where[] = 'inuid = '.$arrPost['inuid'];

        if ($arrPost['id'] != '') {
            $where[] = "id = (SELECT dopidpai FROM par.documentopar = {$arrPost['id']})";
        }

        return $where;

    }//end motnarFiltroObrasPAR()


    /**
     * Função montarSQLHistoricoPAR
     * - monta a SQL da lista de documentos PAR.
     *
     * @return escreve a montarSQLPAR.
     *
     */
    public function montarSQLHistoricoPAR($arrPost)
    {

        $sql = "SELECT	DISTINCT
					mi.dopid as id,
				    mo.mdonome ||' - '|| mi.dopnumerodocumento as dopnumero,
				   'Reformulado' as situacao
				FROM par.documentopar mi
			    LEFT  JOIN par.modelosdocumentos     mo ON mo.mdoid = mi.mdoid
			    INNER JOIN seguranca.usuario         us ON us.usucpf = mi.usucpfinclusao
			    LEFT  JOIN par.documentoparvalidacao dv ON dv.dopid = mi.dopid and dv.dpvstatus = 'A'
				WHERE
                    (
                        mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = ".$arrPost['dopid']." )
    					OR mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = ".$arrPost['dopid']." )
                    )
					AND mi.dopstatus IN ('I')";

        return $sql;
    }//end montarSQLHistoricoPAR()


    /**
     * Função montarSQLObrasPAR
     * - monta a SQL da lista de documentos PAR.
     *
     * @return escreve a montarSQLPAR.
     *
     */
    public function montarSQLObrasPAR($arrPost)
    {
        $where = self::motnarFiltroObrasPAR($arrPost);

        $sql = "SELECT
                    id,
                    dopidpai,
                	pronumeroprocesso,
                	doc,
                	tipodocumento,
                	data_vigencia,
                	qtdObra,
                	dopvalortermo as vt,
                	valorempenho as ve,
                	valorpagamentosolicitado as ps,
                	valorpagamento as vp,
                	--dados_bancarios,
                	( SELECT par.retornasaldoprocesso(pronumeroprocesso) ) as sb
                FROM
                (
                	SELECT
                		dp.dopid as id,
                		dp.dopidpai,
                		 par.retornavigenciaobrapar(pp.proid) as data_vigencia,
                		d.mdonome as tipodocumento,
                		iu.inuid,
                		iu.estuf,
                		iu.muncod,
                		dp.dopusucpfvalidacaogestor,
                		d.mdoqtdvalidacao,
                		(select dopnumerodocumento from par.documentopar where proid = dp.proid and dopstatus <> 'E' order by dopid asc LIMIT 1) as doc,
                		'Banco: '||coalesce(probanco, 'n/a')||'
                		Conta: '||coalesce(proagencia, 'n/a')||'
                		Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,
                		(SELECT count(dopid) FROM par.documentoparvalidacao WHERE dopid = dp.dopid AND dpvstatus = 'A' ) as contagem,
                		dp.dopvalortermo::numeric(20,2),
                		em.valor as valorempenho,
                		pgs.valor_pagamento as valorpagamentosolicitado,
                		pm.valor_pagamento as valorpagamento,
                		pp.pronumeroprocesso,
                		(select count(pc.preid) from par.processoobraspar po inner join par.processoobrasparcomposicao pc on pc.proid = po.proid where pc.pocstatus = 'A' and po.proid = pp.proid) as qtdObra
                	FROM par.documentopar  dp
                	INNER JOIN par.modelosdocumentos   	d ON d.mdoid = dp.mdoid AND dp.dopstatus = 'A'
                	INNER JOIN par.processoobraspar 	pp ON pp.proid = dp.proid and pp.prostatus = 'A'
                	INNER JOIN par.instrumentounidade 	iu ON iu.inuid = pp.inuid
                	LEFT  JOIN
                	(
                		SELECT
                			dopid,
                			sum(valor) as valor
                		FROM
                		(
                			SELECT DISTINCT
                				d.dopid,
                				vve.empid,
                				vve.vrlempenhocancelado as valor
                			FROM
                				par.documentopar d
                			INNER JOIN par.processoobraspar prp on prp.proid = d.proid and prp.prostatus = 'A'
                			INNER JOIN par.empenho emp on emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                			INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = emp.empid
                			LEFT  JOIN
                			(
                				SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie
                				FROM par.empenho
                				WHERE empcodigoespecie IN ('02') AND empstatus = 'A'
                				GROUP BY empnumeroprocesso, empcodigoespecie, empidpai
                			) as emr ON emr.empidpai = emp.empid
                			INNER JOIN par.empenhoobrapar ems on ems.empid = emp.empid and eobstatus = 'A'
                		) as foo
                		GROUP BY dopid
                	) 					em ON em.dopid = dp.dopid
                	LEFT  JOIN
                	(
                		SELECT
                		    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
                		FROM
                		    par.documentopar d
                		INNER JOIN par.processopar 		prp on prp.prpid = d.prpid AND d.dopstatus = 'A'
                		INNER JOIN par.empenho 			emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                		INNER JOIN par.pagamento 		pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento = '2 - EFETIVADO'
                		INNER JOIN par.pagamentosubacao ps on ps.pagid = pag.pagid and pobstatus = 'A' group by d.dopid, pagsituacaopagamento
                	) 					pm ON pm.dopid = dp.dopid
                	LEFT  JOIN
                	(
                		SELECT
                		    d.dopid, sum( pobvalorpagamento ) as valor_pagamento
                		FROM
                		    par.documentopar d
                		INNER JOIN par.processopar 		prp on prp.prpid = d.prpid AND d.dopstatus = 'A'
                		INNER JOIN par.empenho 			emp on emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                		INNER JOIN par.pagamento 		pag on pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento in ('8 - SOLICITAÇÃO APROVADA', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', 'Enviado ao SIGEF')
                		INNER JOIN par.pagamentosubacao ps on ps.pagid = pag.pagid and pobstatus = 'A' group by d.dopid, pagsituacaopagamento
                	) 					pgs ON pgs.dopid = dp.dopid
                ) as foo
                WHERE ".implode(' AND ', $where)."";

        return $sql;
    }//end montarSQLObrasPAR()


    /**
     * Função formVizualizaDocumento
     * - monta tela de vizualização do documento PAR.
     *
     * @return documento.
     *
     */
    public function formVizualizaDocumento($arrPost){

        $sql = "select * from (
				SELECT dpv.dpvcpf as cpfgestor,
							to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
							us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, ( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
						FROM par.documentopar  dp
						INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
						INNER JOIN par.processopar pp ON pp.prpid = dp.prpid
						INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
						LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid and dpv.dpvstatus = 'A'
						left join seguranca.usuario us on us.usucpf = dpv.dpvcpf
				union
				SELECT dpv.dpvcpf as cpfgestor,
							to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
							us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, ( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
						FROM par.documentopar  dp
						INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
						INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
						INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
						LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid and dpv.dpvstatus = 'A'
						left join seguranca.usuario us on us.usucpf = dpv.dpvcpf
				) as foo
			 WHERE
				dopid = ".$arrPost['dopid'];

        $html = self::pegaLinha($sql);

        $sql = "SELECT
    				dpv.dpvcpf as cpfgestor,
    				to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
    				us.usunome,
    				us.usucpf
    			FROM par.documentopar  dp
    			INNER JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid
    			INNER JOIN seguranca.usuario us ON us.usucpf = dpv.dpvcpf
    			WHERE
    				dpv.dpvstatus = 'A' and
    				dp.dopid = ".$arrPost['dopid'];

        $dadosValidacao = self::carregar($sql);

        $textoValidacao = "";

        if(is_array($dadosValidacao)){
            foreach( $dadosValidacao as $dv ){
                $cpfvalidacao[] = $dv['cpfgestor'];
                $textoValidacao .= "<b>Validado por ".$dv['usunome']." - CPF: ".formatar_cpf($dv['usucpf'])." em ".$dv['data']." </b><br>";
            }
        }

        $cpfvalidacao = $cpfvalidacao ? $cpfvalidacao : array();

        $doptexto = self::pegarTermoCompromissoArquivo( $arrPost['dopid'], '');
        ?>
        <div style="background: #FFF;">
            <table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color:white;">
                <tr>
                    <td style="font-size: 12px; font-family:arial;">
                        <div id="divImpressao">
                            <table width='100%' border='0' cellpadding='0' cellspacing='0'>
                                <tr bgcolor='#FFF'>
									<td valign='top' align='center'>
                                        <img src='../imagens/brasao.gif' width='45' height='45' border='0'>
                                        <br>
										<b>
											MINISTÉRIO DA EDUCAÇÃO<br />
											FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO
										</b>
									</td>
                                </tr>
                            </table>
                            <?php echo simec_html_entity_decode($doptexto);?>
                        </div>
                    </td>
                </tr>
            </table>
			<br />
            <table width="95%" id="termo" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color:white;">
                <tr <?php echo $displayValidado; ?> style="text-align: center;">
                    <td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
                     <?php echo $textoValidacao; ?>
                    </td>
                </tr>
            </table>
        </div>
    <?php
    }//end formVizualizaDocumento()


    /**
     * Função pegarTermoCompromissoArquivo
     * - retorna o termo de compromisso.
     *
     * @return documento.
     *
     */
    function pegarTermoCompromissoArquivo( $dopid, $terid )
    {
        if( $dopid || $terid ){

            if( $terid ){
                $dtanomearquivo = self::pegaUm("select d.dtanomearquivo from par.documentotermoarquivo d where d.terid = {$terid} order by d.dtaid desc");
            } else {
                $dtanomearquivo = self::pegaUm("select d.dtanomearquivo from par.documentotermoarquivo d where d.dopid = {$dopid} order by d.dtaid desc");
            }

            if( $dtanomearquivo ){
                $diretorio = APPRAIZ . 'arquivos/par/documentoTermo/'.$dtanomearquivo;
                if( is_file($diretorio) ){
                    $doptexto = file_get_contents($diretorio);
                }
            }
        }

        return $doptexto;
    }//end pegarTermoCompromissoArquivo


    /**
     * Função formVizualizaDocumentoPAC
     * - retorna o termo de compromisso.
     *
     * @return documento.
     *
     */
    function formVizualizaDocumentoPAC($arrPost)
    {
        $terid = $arrPost['terid'];

        $sql = "SELECT
        			to_char(terdataassinatura, 'DD/MM/YYYY') as terdataassinatura,
        			usucpfassinatura,
        			terassinado, proid,
                    CASE WHEN muncod IS NOT NULL THEN TRUE ELSE FALSE END as esfera_municipal
        		FROM
        			par.termocompromissopac
        		WHERE
        			terid = ".$terid;

        $dados = $this->pegaLinha( $sql );

        if( $dados['terassinado'] == 't' ){
            $sql = "SELECT usunome FROM seguranca.usuario WHERE usucpf = '".$dados['usucpfassinatura']."'";
            $nome = $this->pegaUm( $sql );
        }

        $html = $this->pegarTermoCompromissoArquivo( '', $terid );

        if( $html == '' ){

            $html = $this->retornaHTMLTermo( $terid );

            $this->gravaHtmlDocumento($html, $terid, $dados['proid'], 'PAC');
        }

        if( $dados['esfera_municipal'] == 't' ){
            if( $nome == '' ){
                $nome = 'manualmente';
    	    } else {
        	    $nome = 'pelo(a) Prefeito(a) '.$nome.' - CPF: '.formatar_cpf($dados['usucpfassinatura']).' em '.$dados['terdataassinatura'];
        	}
        } else {
        	if( $nome == '' ){
        		$nome = 'manualmente';
        	} else {
        		$nome = 'Secretário(a) de Educação '.$nome.' - CPF: '.formatar_cpf($dados['usucpfassinatura']).' em '.$dados['terdataassinatura'];
        	}
        }

        include APPRAIZ.'par3/modulos/principal/planoTrabalho/acompanhamento/formTermoPAC.php';
    }//end formVizualizaDocumentoPAC

    public function gravaHtmlDocumento($doptexto, $idDocumento, $processo, $tipo)
    {
        ob_clean();

        if( $doptexto ){
            if( strpos($doptexto, '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
                $doptexto = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
            } else {
                $doptexto = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $doptexto );
            }
        }

        $doptexto = $doptexto ? simec_htmlspecialchars($doptexto) : 'null';

        $nomeArquivo 		= 'Minuta_Documento_'.$tipo.'_'.$idDocumento.'_'.$processo.'_'.date('YmdHis');
        $diretorio		 	= APPRAIZ . 'arquivos/par/documentoTermo';
        $diretorioArquivo 	= APPRAIZ . 'arquivos/par/documentoTermo/'.$nomeArquivo.'.txt';

        if( !is_dir($diretorio) ){
            mkdir($diretorio, 0777);
        }

        $fp = fopen($diretorioArquivo, "w");

        if ($fp) {
            stream_set_write_buffer($fp, 0);
            fwrite($fp, $doptexto);
            fclose($fp);
        }

        if( $tipo == 'PAC' ){
            $dopid = 'null';
            $terid = $idDocumento;
            $trpid = 'null';
            $this->executar("UPDATE par.documentotermoarquivo SET dtastatus = 'I' WHERE terid = $terid");
        }elseif( $tipo == 'R_PAC' ){
            $dopid = 'null';
            $terid = 'null';
            $trpid = $idDocumento;
            $this->executar("UPDATE par.documentotermoarquivo SET dtastatus = 'I' WHERE terid = $trpid");
        }else{
            $dopid = $idDocumento;
            $terid = 'null';
            $trpid = 'null';
            $this->executar("UPDATE par.documentotermoarquivo SET dtastatus = 'I' WHERE dopid = $dopid");
        }

        $sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
			    VALUES( '".$nomeArquivo."',
					'txt',
					'".$nomeArquivo."',
					'text/plain',
					'".filesize($diretorioArquivo)."',
					'".date('Y-m-d')."',
					'".date('H:i:s')."',
					'".$_SESSION["usucpf"]."',
					".($_SESSION['sisid'] ? $_SESSION['sisid'] : 'null').",
					'A')
                RETURNING arqid";
        $arqid = $this->pegaUm($sql);

        $sql = "INSERT INTO par.documentotermoarquivo(dopid, terid, trpid, arqid, dtaprocesso, dtatipo, dtanomearquivo, dtastatus)
                VALUES($dopid, $terid, $trpid, $arqid, $processo, '$tipo', '".$nomeArquivo.'.txt'."', 'A')";

        $this->executar($sql);
        $this->commit();

        return true;
    }


    public function retornaHTMLTermo( $terid )
    {
        $sql = "SELECT proid, estuf, muncod FROM par.termocompromissopac WHERE terid = ".$terid;
        $dados = $this->pegaLinha( $sql );

        $proid = $dados['proid'];

        if( $proid ){

            if( $dados['muncod'] ){
                $filtro = " muncod = '".$dados['muncod']."'";
                $esfera = 'M';
            } else {
                $estuf = $dados['estuf'];
                $filtro = " estuf = '".$estuf."'";
                $esfera = 'E';
            }

            if( $esfera == 'M' ){
                $sql = "SELECT
						upper(mun.mundescricao) as municipio,
						est.estuf,
						ent2.entnome as prefeitura,
						coalesce(ent2.endlog,'#') || '/' || coalesce(ent2.endbai,'#') as prefendereco,
						ent2.entnumcpfcnpj as cnpjprefeitura,
						ent.entnome as prefeito,
						ent.entnumrg as rg,
						ent.entnumcpfcnpj as cpf,
						est.estdescricao as estado
					FROM  par.entidade ent
					INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = 6
					INNER JOIN territorios.estado est on est.estuf = ent2.estuf
					LEFT JOIN territorios.municipio mun on mun.muncod = est.muncodcapital
					where
						ent.dutid = 7 AND
						ent.entstatus='A' AND
						ent2.entstatus='A' AND
						ent2.muncod = '".$dados['muncod']."'";

                $munDados = $this->carregar($sql);


                $sql = "SELECT
					pre.preid
				FROM
					obras.preobra pre
				INNER JOIN par.empenhoobra   emp ON emp.preid = pre.preid and eobstatus = 'A'
				INNER JOIN par.empenho   emn ON emn.empid = emp.empid and empstatus <> 'I'
				INNER JOIN par.processoobra   pro ON pro.pronumeroprocesso = emn.empnumeroprocesso and pro.prostatus = 'A'
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptoclassificacaoobra = pro.protipo
				WHERE
					pro.proid = '".$proid."' AND pre.muncod = '{$dados['muncod']}' AND pre.preesfera = 'M'
    					GROUP BY
    					pre.preid, pre.predescricao, pto.ptodescricao, prevalorobra
    					ORDER BY
    					pto.ptodescricao,pre.predescricao";

                $preids = $this->carregarColuna( $sql );

                } else {
			$sql = "SELECT
						upper(est.estdescricao) as municipio,
						est.estuf,
						ent2.entnome as prefeitura,
						coalesce(ent2.endlog,'#') || '/' || coalesce(ent2.endbai,'#') as prefendereco,
						ent2.entnumcpfcnpj as cnpjprefeitura,
						ent.entnome as prefeito,
						ent.entnumrg as rg,
						ent.entnumcpfcnpj as cpf,
						est.estdescricao as estado
					FROM  par.entidade ent
					INNER JOIN par.entidade ent2 ON ent.inuid = ent2.inuid AND ent2.dutid = 9
					INNER JOIN territorios.estado est on est.estuf = ent2.estuf
					where
						ent.dutid = 10 AND
    						ent.entstatus='A' AND
    						ent2.entstatus='A' AND
    						ent2.estuf = '$estuf'";

                $munDados = $this->carregar($sql);

			$sql = "SELECT
					pre.preid
				FROM
					obras.preobra pre
				INNER JOIN par.empenhoobra   emp ON emp.preid = pre.preid and eobstatus = 'A'
				INNER JOIN par.empenho   emn ON emn.empid = emp.empid and empstatus <> 'I'
				INNER JOIN par.processoobra   pro ON pro.pronumeroprocesso = emn.empnumeroprocesso and pro.prostatus = 'A'
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptoclassificacaoobra = pro.protipo
                    WHERE
                    pro.proid = '".$proid."' AND pre.estuf = '{$estuf}' AND pre.preesfera = 'E'
                    GROUP BY
                    pre.preid, pre.predescricao, pto.ptodescricao, prevalorobra
                    ORDER BY
                    pto.ptodescricao,pre.predescricao";

                    $preids = $this->carregarColuna( $sql );
        }

        $termo = $terid;

        $ano   = $this->pegaUm("SELECT to_char(terdatainclusao,'YYYY') FROM par.termocompromissopac WHERE terid = {$termo}");

        //$preids = explode(',',$_REQUEST['arPreid']);
		$tabela = "<div>";
		$x=1;
		foreach( $preids as $preid ){

			$sql = "SELECT
						preid || ' - ' || predescricao as obra,
						prelogradouro as logradouro,
						precomplemento as complemento,
						ptodescricao as tipoobra,
						prevalorobra as vlrobra,
                		ptoclassificacaoobra as tipoobra
					FROM
						obras.preobra pre
                    INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
                    WHERE
                    preid = ".$preid;
			$dado   = $this->pegaLinha($sql);
			$obra 		 = $dado['obra'] 		!= '' ? "".wordwrap ( $dado['obra'], 50, '<br />')."<br />" : "";
			$logradouro  = $dado['logradouro']  != '' ? "".wordwrap ( $dado['logradouro'], 50, '<br />')."<br />" : "";
			$complemento = $dado['complemento'] != '' ? "".wordwrap ( $dado['complemento'], 50, '<br />')."<br />" : "";
			$tabela .= "<div style=\"display: table;\">
							<div class=\"P_2\">{$x} )</div>
							<div class=\"P_4\">".$obra."
							".$logradouro."
										   ".$complemento."
                        ".$dado['tipoobra']."R$ ".number_format($dado['vlrobra'],2,',','.')."</div>
                        </div><br />";
            $x++;
            $tipoobra = $dado['tipoobra'];
        }

        $tabela .= "</div>";
	}

	$tipoObra = ($tipoobra == 'Q') ? 'à Quadras' : 'ao Pró-Infância';

	$tipoObra2 = ($tipoobra == 'Q') ? 'quadra(s) esportiva(s) escolar(es) coberta(s)' : 'unidade(s) de educação infantil';

	if( $esfera == 'M' ){

	return '<html xmlns="http://www.w3.org/1999/xhtml">
		<head>
			<meta http-equiv="content-type" content="text/html; charset=utf-8" />
			<title>Termo de Compomisso</title>
			<meta name="generator content="StarOffice/OpenOffice.org XSLT (http://xml.openoffice.org/sx2ml)" />
			<meta name="author" content="Neuza" />
			<meta name="created" content="2008-01-31T17:54:00" />
			<meta name="changedby" content="mec" />
			<meta name="changed" content="2008-02-07T18:09:00" />
			<style type="text/css">
				@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
				.divT_1{ margin-left: 2.5598in; text-align: center; }
				.P_1{ font-family: Arial; font-size: 12pt; margin-left: 1.5598in; margin-right: 0in; text-align: justify ! important; clear: both; }
				.P_2{ font-family: Arial; font-size: 10pt; float: left; margin-left: 2in; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
				.P_3{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
				.P_4{ font-family: Arial; font-size: 10pt; float: left; text-align: left ! important; Overflow: auto; width="100px"}
				.Data{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: right; }
				.T{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
				.T_1{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.T_11{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.N{ font-weight: bold; }
				.I{ font-style: italic; }
				.S{ text-decoration: underline; }
				p.quebra    { page-break-before: always }
			</style>
			<link rel="stylesheet" type="text/css href="chrome://firebug/content/highlighter.css" />
		</head>
		<body>
			<div>
				<p class="T_1">TERMO DE COMPROMISSO<br />PAC2'.str_pad($termo,5,0, STR_PAD_LEFT).'/'.$ano.'</p>
    				<p class="P_1">
					A Prefeitura Municipal de <strong>'.$munDados[0]['municipio'].'('.$munDados[0]['estuf'].')</strong>, com sede na <strong>'.$munDados[0]['prefendereco'].'</strong>,	inscrita no CNPJ/MF sob o n° <strong>'.$munDados[0]['cnpjprefeitura'].'</strong>,
    					    representada pelo(a) prefeito(a) <strong>'.$munDados[0]['prefeito'].'</strong>, brasileiro(a), portador(a) da carteira de identidade n° <strong>'.$munDados[0]['rg'].'</strong> e do
					CPF n° <strong>'.$munDados[0]['cpf'].'</strong>, residente e domiciliado(a) no estado de <strong>'.$munDados[0]['estado'].'</strong>, considerando o que dispõe a Lei n° 11.578,
					de 26 de novembro de 2007, compromete-se a executar as ações relativas a '.$tipoObra.',
					no âmbito do PAC 2, de acordo com as especificações do(s) projeto(s) fornecido(s) ou aprovado(s) pelo Fundo Nacional de
					Desenvolvimento da Educação  FNDE e em conformidade com os requisitos da lei supramencionada e demais condicionantes, a seguir descritas:
    					    </p>
    					    <p class="P_1">
					<br />
					I  Executar todas as atividades inerentes à construção de '.count($preids).' ('.strtolower(extenso(count($preids),true,false)).') '.$tipoObra2.', situada(s) em:
				</p>
					'.$tabela.'
				<p class="P_1">
					<br />
					II - Executar os recursos financeiros recebidos do Fundo Nacional de Desenvolvimento da Educação no âmbito do PAC 2
					em estrito acordo com os projetos executivos fornecidos ou aprovados pelo FNDE/MEC (desenhos técnicos, memoriais descritivos e especificações),
					observando os critérios de qualidade técnica que atendam as determinações da Associação Brasileira de Normas Técnicas (ABNT),
					bem como os prazos e os custos previstos;
				</p>
				<p class="P_1">
					<br />
					III - Utilizar os recursos financeiros transferidos pelo FNDE/MEC exclusivamente no cumprimento do objeto pactuado;
					responsabilizando-se para que a movimentação dos recursos ocorra somente para o pagamento das despesas previstas neste
					Termo de Compromisso ou para aplicação financeira, devendo a movimentação realizar-se, exclusivamente, mediante cheque
					nominativo ao credor ou ordem bancária, Transferência Eletrônica de Disponibilidade (TED) ou outra modalidade de saque
					autorizada pelo Banco Central do Brasil em que fique identificada a destinação e, no caso de pagamento, o credor;
				</p>
				<p class="P_1">
					<br />
					IV - Nomear profissional devidamente habilitado, da área de engenharia civil ou arquitetura, para exercer as funções de
					fiscalização da(s) obra(s), com emissão da respectiva Anotação de Responsabilidade Técnica (ART/CREA);
				</p>
				<p class="P_1">
					<br />
					V - Responsabilizar-se, com recursos próprios, por obras e serviços de terraplenagem e contenções, infraestrutura de
					redes (água potável, esgotamento sanitário, energia elétrica e telefonia), bem como por todos os serviços
					necessários à implantação do(s) empreendimento(s) no(s) terreno(s) tecnicamente aprovado(s), uma vez que os
					valores a serem repassados pelo FNDE/MEC referem-se exclusivamente aos serviços de engenharia constantes nas planilhas
					orçamentárias do(s) projeto(s) pactuado(s) e aprovado(s);
				</p>
				<p class="P_1">
					<br />
					VI - Garantir, com recursos próprios, a conclusão da(s) obra(s) acima pactuada(s) e sua entrega à população,
					no caso de os valores transferidos se revelarem insuficientes para cobrir todas as despesas relativas à implantação;
				</p>
				<p class="P_1">
					<br />
					VII - Indicar agência do Banco do Brasil S/A onde deverão ser depositados os recursos referentes à construção da(s)
					obra(s) pactuada(s) neste Termo de Compromisso, visando à abertura de conta corrente específica pelo FNDE/MEC,
					a qual estará isenta do pagamento de taxas e tarifas bancárias, em conformidade com o Acordo de Cooperação Mútua celebrado com o FNDE,
					disponível no <a href=\"www.fnde.gov.br\" style=\"color: blue\">sítio: www.fnde.gov.br</a>;
				</p>
				<p class="P_1">
					<br />
					VIII - Providenciar a regularização da referida conta corrente na agência indicada, procedendo à entrega e à chancela dos documentos
					necessários à sua movimentação, de acordo com as normas bancárias vigentes, outorgando ao FNDE/MEC a condição de, sempre que necessário,
					obter junto ao banco os saldos e extratos da referida conta, inclusive os das aplicações financeiras, bem como o direito de solicitar
					seu encerramento, bloqueio, estorno ou transferência de valores, nos casos estipulados na Resolução CD/FNDE Nº 69/2011, de que este
					Termo de Compromisso constitui anexo;
				</p>
				<p class="P_1">
					<br />
					IX - Responsabilizar-se pelo acompanhamento das transferências financeiras efetuadas pelo FNDE,
					de forma a garantir a aplicação tempestiva dos recursos creditados a seu favor.
				</p>
				<p class="P_1">
					<br />
					X - Aplicar os recursos recebidos, enquanto não forem utilizados em sua finalidade, obrigatoriamente em caderneta de poupança,
					aberta especificamente para o Programa, quando a previsão do seu uso for igual ou superior a um mês; ou aplicá-los em fundo de
					aplicação financeira de curto prazo ou operação de mercado aberto lastreada em títulos da dívida pública, se a sua utilização
					ocorrer em prazo inferior a um mês. Responsabilizar-se ainda por efetivar a aplicação financeira vinculada à mesma conta corrente
					na qual os recursos financeiros foram creditados pelo FNDE/MEC, inclusive quando se tratar de caderneta de poupança,
					cuja aplicação poderá se dar mediante vinculação do correspondente número de operação à conta já existente.
				</p>
				<p class="P_1">
					<br />
					XI - Destinar os rendimentos das aplicações financeiras exclusivamente às ações do presente Termo de Compromisso, incluindo-os nas mesmas condições
					de prestação de contas exigidas para os recursos transferidos, devendo tais rendimentos ser obrigatoriamente computados a crédito da conta corrente específica;
				</p>
				<p class="P_1">
					<br />
					XII - Realizar licitações para as contratações necessárias à execução da(s) obra(s) acima pactuadas, obedecendo à Lei n° 8.666, de 21 de
					junho de 1993, e observar que os preços unitários de materiais e serviços utilizados não sejam superiores à mediana daqueles constantes
					do Sistema Nacional de Pesquisa de Custos e Índices da Construção Civil  SINAPI, mantido pela Caixa Econômica Federal. Em condições
					especiais, devidamente justificadas em Relatório Técnico circunstanciado, aprovado pela Diretoria de Programas e Projetos Educacionais
					(DIRPE/FNDE), exclusivamente para itens não disponíveis no SINAPI poderão ser praticados preços específicos,
					sem prejuízo da avaliação dos órgãos de controle internos e externos;
				</p>
				<p class="P_1">
					<br />
					XIII - Cientificar mensalmente o FNDE/MEC sobre a aplicação dos recursos e a consecução do objeto conforme o previsto,
					por meio do preenchimento dos dados e informações sobre a(s) obra(s) no Módulo de Monitoramento de Obras do SIMEC
					(Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação), no endereço eletrônico <a href=\"http://simec.mec.gov.br\" style=\"color: blue\">http://simec.mec.gov.br</a>,
					utilizando para tanto a senha do Plano de Ações Articuladas (PAR), fornecida pela Secretaria de Educação Básica (SEB/MEC);
				</p>
				<p class="P_1">
					<br />
					XIV - Assegurar e destacar obrigatoriamente a participação do Governo Federal e do FNDE em toda e qualquer ação, promocional ou não,
					relacionada com a execução do objeto pactuado acima, obedecendo ao modelo-padrão estabelecido, bem como apor a marca do Governo Federal
					em placas, cartazes, faixas e painéis de identificação da(s) obra(s) custeada(s) com os recursos transferidos à conta do Programa,
					obedecendo ao que está disposto na Instrução Normativa nº 2, de 12 de dezembro de 2009, da Secretaria de Comunicação de Governo e
					Gestão Estratégica da Presidência da República;
				</p>
				<p class="P_1">
					<br />
					XV - Manter atualizada a escrituração contábil específica dos atos e fatos relativos à execução deste Termo de Compromisso,
					para fins de fiscalização, de acompanhamento e de avaliação dos resultados obtidos;
				</p>
				<p class="P_1">
					<br />
					XVI - Facilitar a supervisão e a fiscalização do FNDE/MEC, permitindo-lhe efetuar acompanhamento no local e fornecendo, sempre que solicitado, as informações e os
					documentos relacionados com a execução do objeto deste Instrumento, especialmente no que se refere ao exame da documentação relativa à licitação e aos contratos;
				</p>
				<p class="P_1">
					<br />
					XVII - Permitir o livre acesso de servidores do Sistema de Controle Interno do Poder Executivo Federal (Secretaria Federal de Controle
					 SFC/MF, Delegacia Federal de Controle  DFC ou sua representação no Estado, Secretaria de Controle Interno  CISET) e da Auditoria do
					FNDE, a qualquer tempo e lugar, a todos os atos administrativos e aos registros dos fatos relacionados direta ou indiretamente com o
					objeto pactuado no Termo de Compromisso (Anexo I), bem como às obras e serviços a ele referidas, colaborando na obtenção de dados e de
					informações junto à comunidade local sobre os benefícios advindos da implantação do(s) projeto(s), quando em missão de fiscalização e auditoria;
				</p>
				<p class="P_1">
					<br />
					XVIII - Apresentar ao FNDE/MEC ou a seu(s) representante(s) legalmente constituído(s) o original ou a cópia autenticada de todo e qualquer documento comprobatório
					de despesa efetuada à conta dos recursos transferidos à conta do Programa, a qualquer tempo e a critério daquela Autarquia Federal;
				</p>
				<p class="P_1">
					<br />
					XIX - Prestar todo e qualquer esclarecimento sobre a execução física e financeira do Programa, sempre que solicitado pelo FNDE/MEC, pela SEB/MEC, por órgão do Sistema
					de Controle Interno do Poder Executivo Federal, pelo Tribunal de Contas da União, pelo Ministério Público ou por órgão ou entidade com delegação para esse fim;
				</p>
				<p class="P_1">
					<br />
					XX - Incluir no orçamento anual do Município, ou do estado, os recursos recebidos para execução do objeto deste Termo de Compromisso, nos termos estabelecidos no
					§ 1º, do art. 6º, da Lei nº 4.320, de 17 de março de 1964;
				</p>
				<p class="P_1">
					<br />
					XXI - Não considerar os valores transferidos no cômputo dos 25% (vinte e cinco por cento) de impostos e transferências devidos à manutenção e ao
					desenvolvimento do ensino, por força do disposto no art. 212 da Constituição Federal;
				</p>
				<p class="P_1">
					<br />
					XXII - Emitir o(s) termo(s) de aceitação definitiva da(s) obra(s), ao final da execução dos recursos, remetendo cópia autenticada do(s) mesmo(s)
					à DIRPE/FNDE para a emissão do(s) termo(s) de conclusão da(s) obra(s) e consolidação deste Termo de Compromisso;
				</p>
				<p class="P_1">
					<br />
					XXIII - Prestar contas ao FNDE/MEC dos recursos recebidos, no prazo e nas condições estipuladas nos artigos 29 e 30
					da Resolução CD/FNDE Nº 13/2011;
				</p>
				<p class="P_1">
					<br />
					XXIV - Manter em seu poder, à disposição do FNDE/MEC, da SEB/MEC, dos órgãos de controle interno e externo e do
					Ministério Público, os comprovantes das despesas efetuadas à conta do Programa, pelo prazo de 10 (dez) anos, contados
					da data da aprovação da prestação de contas anual do FNDE/MEC pelo Tribunal de Contas da União (TCU) a que se refere o exercício do repasse dos recursos,
					a qual será divulgada no sítio eletrônico <a href=\"www.fnde.gov.br\" style=\"color: blue\">www.fnde.gov.br</a>;
				</p>
				<p class="P_1">
					<br />
					XXV - Responsabilizar-se por todos os encargos de natureza trabalhista e previdenciária, decorrentes de eventuais
					demandas judiciais relativas a recursos humanos utilizados na execução do objeto deste Termo de Compromisso, bem
					como por todos os ônus tributários ou extraordinários que incidam sobre o presente Instrumento, ressalvados aqueles de natureza compulsória,
					lançados automaticamente pela rede bancária arrecadadora;
				</p>
				<p class="P_1">
					<br />
					XXVI - Adotar todas as medidas necessárias à correta execução deste Termo de Compromisso.
				</p>
				<p class="quebra">&nbsp;</p>
				<p class="P_3">
					<br />
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro, em complementação, que o município
					cumpre com as exigências do art. 169 da Constituição Federal que trata dos limites de despesa com pessoal e,
					que os recursos próprios de responsabilidade do Município estão assegurados, conforme a
					Lei Orgânica Municipal.
				</p>
				<p class="Data">
					<br />
					Brasília/DF, ___ de ____________________ de ______.
    					    </p>
				<p class="T_1">
					<br />
					<br />
					<br />
					____________________________________________
    					    <br />
    					    <br />
					<strong>'.$munDados[0]['prefeito'].'</strong>
                    <br />
					PREFEITO(A) MUNICIPAL DE <strong>'.strtoupper($munDados[0]['municipio']).'/'.$munDados[0]['estuf'].'</strong>
				</p>
			</div>
		</body>
	</html>';
	} else {

	return '<html xmlns="http://www.w3.org/1999/xhtml">
		<head>
			<meta http-equiv="content-type" content="text/html; charset=utf-8" />
			<title>Termo de Compomisso</title>
			<meta name="generator content="StarOffice/OpenOffice.org XSLT (http://xml.openoffice.org/sx2ml)" />
			<meta name="author" content="Neuza" />
			<meta name="created" content="2008-01-31T17:54:00" />
			<meta name="changedby" content="mec" />
			<meta name="changed" content="2008-02-07T18:09:00" />
			<style type="text/css">
				@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
				.divT_1{ margin-left: 2.5598in; text-align: center; }
				.P_1{ font-family: Arial; font-size: 12pt; margin-left: 1.5598in; margin-right: 0in; text-align: justify ! important; clear: both; }
				.P_2{ font-family: Arial; font-size: 10pt; float: left; margin-left: 2in; margin-right: 0.3in; text-align: left ! important; text-indent: 0inch; }
				.P_3{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: justify ! important; text-indent: 1inch; }
				.P_4{ font-family: Arial; font-size: 10pt; float: left; text-align: left ! important; Overflow: auto; width="100px"}
				.Data{ font-family: Arial; font-size: 12pt; margin-left: 1in; margin-right: 0in; text-align: right; }
				.T{ font-family: Arial; font-size: 12pt; font-weight: bold; text-align: center; }
				.T_1{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.T_11{ font-family: Arial; font-size: 12pt; font-weight: bold; margin-left: 1.5598in; text-align: center; }
				.N{ font-weight: bold; }
				.I{ font-style: italic; }
				.S{ text-decoration: underline; }
				p.quebra    { page-break-before: always }
    				</style>
    				<link rel="stylesheet" type="text/css href="chrome://firebug/content/highlighter.css" />
		</head>
		<body>
    		<div>
    		<p class="T_1">TERMO DE COMPROMISSO<br />PAC2'.str_pad($termo,5,0, STR_PAD_LEFT).'/'.$ano.'</p>
				<p class="P_1">
					A Secretaria de Educação do Estado de <strong>'.$munDados[0]['municipio'].'</strong>, com sede na <strong>'.$munDados[0]['prefendereco'].'</strong>, inscrita no CNPJ/MF sob o n° <strong>'.$munDados[0]['cnpjprefeitura'].'</strong>,
					representada pelo(a) Secretário(a) <strong>'.$munDados[0]['prefeito'].'</strong>, brasileiro(a), portador(a) da carteira de identidade n° <strong>'.$munDados[0]['rg'].'</strong> e do
					CPF n° <strong>'.$munDados[0]['cpf'].'</strong>, residente e domiciliado(a) no estado de <strong>'.$munDados[0]['estado'].'</strong>, considerando o que dispõe a Lei n° 11.578,
					de 26 de novembro de 2007, compromete-se a executar as ações relativas a construção de quadra(s) poliesportiva(s) escolar(es),
					no âmbito do PAC 2, de acordo com as especificações do(s) projeto(s) fornecido(s) ou aprovado(s) pelo Fundo Nacional de
					Desenvolvimento da Educação  FNDE e em conformidade com os requisitos da lei supramencionada e demais condicionantes, a seguir descritas:
				</p>
				<p class="P_1">
					<br />
					I  Executar todas as atividades inerentes à construção de '.count($preids).' ('.strtolower(extenso(count($preids),true,false)).') quadra(s) poliesportiva(s) escolar(es) coberta(as), situada(s) em:
				</p>
					'.$tabela.'
				<p class="P_1">
					<br />
					II - Executar os recursos financeiros recebidos do Fundo Nacional de Desenvolvimento da Educação no âmbito do PAC 2 em estrito acordo
					com os projetos executivos fornecidos ou aprovados pelo FNDE/MEC (desenhos técnicos, memoriais descritivos e especificações),
					observando os critérios de qualidade técnica que atendam as determinações da Associação Brasileira de Normas Técnicas (ABNT),
					bem como os prazos e os custos previstos;
				</p>
				<p class="P_1">
					<br />
					III - Utilizar os recursos financeiros transferidos pelo FNDE/MEC exclusivamente no cumprimento do objeto pactuado;
					responsabilizando-se para que a movimentação dos recursos ocorra somente para o pagamento das despesas previstas neste
					Termo de Compromisso ou para aplicação financeira, devendo a movimentação realizar-se, exclusivamente, mediante cheque
					nominativo ao credor ou ordem bancária, Transferência Eletrônica de Disponibilidade (TED) ou outra modalidade de saque
					autorizada pelo Banco Central do Brasil em que fique identificada a destinação e, no caso de pagamento, o credor;
				</p>
				<p class="P_1">
					<br />
					IV - Nomear profissional devidamente habilitado, da área de engenharia civil ou arquitetura, para exercer as funções de
					fiscalização da(s) obra(s), com emissão da respectiva Anotação de Responsabilidade Técnica (ART/CREA);
				</p>
				<p class="P_1">
					<br />
					V - Responsabilizar-se, com recursos próprios, por obras e serviços de terraplenagem e contenções, infraestrutura de
					redes (água potável, esgotamento sanitário, energia elétrica e telefonia), bem como por todos os serviços
					necessários à implantação do(s) empreendimento(s) no(s) terreno(s) tecnicamente aprovado(s), uma vez que os
					valores a serem repassados pelo FNDE/MEC referem-se exclusivamente aos serviços de engenharia constantes nas planilhas
					orçamentárias do(s) projeto(s) pactuado(s) e aprovado(s);
				</p>
				<p class="P_1">
					<br />
					VI - Garantir, com recursos próprios, a conclusão da(s) obra(s) acima pactuada(s) e sua entrega à população,
					no caso de os valores transferidos se revelarem insuficientes para cobrir todas as despesas relativas à implantação;
				</p>
				<p class="P_1">
					<br />
					VII - Indicar agência do Banco do Brasil S/A onde deverão ser depositados os recursos referentes à construção da(s)
					obra(s) pactuada(s) neste Termo de Compromisso, visando à abertura de conta corrente específica pelo FNDE/MEC,
					a qual estará isenta do pagamento de taxas e tarifas bancárias, em conformidade com o Acordo de Cooperação Mútua celebrado com o FNDE,
					disponível no <a href=\"www.fnde.gov.br\" style=\"color: blue\">sítio: www.fnde.gov.br</a>;
				</p>
				<p class="P_1">
					<br />
					VIII - Providenciar a regularização da referida conta corrente na agência indicada, procedendo à entrega e à chancela dos documentos
					necessários à sua movimentação, de acordo com as normas bancárias vigentes, outorgando ao FNDE/MEC a condição de, sempre que necessário,
					obter junto ao banco os saldos e extratos da referida conta, inclusive os das aplicações financeiras, bem como o direito de solicitar
					seu encerramento, bloqueio, estorno ou transferência de valores, nos casos estipulados na Resolução CD/FNDE Nº 69/2011, de que este
					Termo de Compromisso constitui anexo;
				</p>
				<p class="P_1">
					<br />
					IX - Responsabilizar-se pelo acompanhamento das transferências financeiras efetuadas pelo FNDE,
					de forma a garantir a aplicação tempestiva dos recursos creditados a seu favor.
				</p>
				<p class="P_1">
					<br />
					X - Aplicar os recursos recebidos, enquanto não forem utilizados em sua finalidade, obrigatoriamente em caderneta de poupança,
					aberta especificamente para o Programa, quando a previsão do seu uso for igual ou superior a um mês; ou aplicá-los em fundo de
					aplicação financeira de curto prazo ou operação de mercado aberto lastreada em títulos da dívida pública, se a sua utilização
					ocorrer em prazo inferior a um mês. Responsabilizar-se ainda por efetivar a aplicação financeira vinculada à mesma conta corrente
					na qual os recursos financeiros foram creditados pelo FNDE/MEC, inclusive quando se tratar de caderneta de poupança,
					cuja aplicação poderá se dar mediante vinculação do correspondente número de operação à conta já existente.
				</p>
				<p class="P_1">
					<br />
					XI - Destinar os rendimentos das aplicações financeiras exclusivamente às ações do presente Termo de Compromisso, incluindo-os nas mesmas condições
					de prestação de contas exigidas para os recursos transferidos, devendo tais rendimentos ser obrigatoriamente computados a crédito da conta corrente específica;
				</p>
				<p class="P_1">
					<br />
					XII - Realizar licitações para as contratações necessárias à execução da(s) obra(s) acima pactuadas, obedecendo à Lei n° 8.666, de 21 de
					junho de 1993, e observar que os preços unitários de materiais e serviços utilizados não sejam superiores à mediana daqueles constantes
					do Sistema Nacional de Pesquisa de Custos e Índices da Construção Civil  SINAPI, mantido pela Caixa Econômica Federal. Em condições
					especiais, devidamente justificadas em Relatório Técnico circunstanciado, aprovado pela Diretoria de Programas e Projetos Educacionais
					(DIRPE/FNDE), exclusivamente para itens não disponíveis no SINAPI poderão ser praticados preços específicos,
					sem prejuízo da avaliação dos órgãos de controle internos e externos;
				</p>
				<p class="P_1">
					<br />
					XIII - Cientificar mensalmente o FNDE/MEC sobre a aplicação dos recursos e a consecução do objeto conforme o previsto,
					por meio do preenchimento dos dados e informações sobre a(s) obra(s) no Módulo de Monitoramento de Obras do SIMEC
					(Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação), no endereço eletrônico <a href=\"http://simec.mec.gov.br\" style=\"color: blue\">http://simec.mec.gov.br</a>,
					utilizando para tanto a senha do Plano de Ações Articuladas (PAR), fornecida pela Secretaria de Educação Básica (SEB/MEC);
				</p>
				<p class="P_1">
					<br />
					XIV - Assegurar e destacar obrigatoriamente a participação do Governo Federal e do FNDE em toda e qualquer ação, promocional ou não,
					relacionada com a execução do objeto pactuado acima, obedecendo ao modelo-padrão estabelecido, bem como apor a marca do Governo Federal
					em placas, cartazes, faixas e painéis de identificação da(s) obra(s) custeada(s) com os recursos transferidos à conta do Programa,
					obedecendo ao que está disposto na Instrução Normativa nº 2, de 12 de dezembro de 2009, da Secretaria de Comunicação de Governo e
					Gestão Estratégica da Presidência da República;
				</p>
				<p class="P_1">
					<br />
					XV - Manter atualizada a escrituração contábil específica dos atos e fatos relativos à execução deste Termo de Compromisso,
					para fins de fiscalização, de acompanhamento e de avaliação dos resultados obtidos;
				</p>
				<p class="P_1">
					<br />
					XVI - Facilitar a supervisão e a fiscalização do FNDE/MEC, permitindo-lhe efetuar acompanhamento no local e fornecendo, sempre que solicitado, as informações e os
					documentos relacionados com a execução do objeto deste Instrumento, especialmente no que se refere ao exame da documentação relativa à licitação e aos contratos;
				</p>
				<p class="P_1">
					<br />
					XVII - Permitir o livre acesso de servidores do Sistema de Controle Interno do Poder Executivo Federal (Secretaria Federal de Controle
					 SFC/MF, Delegacia Federal de Controle  DFC ou sua representação no Estado, Secretaria de Controle Interno  CISET) e da Auditoria do
					FNDE, a qualquer tempo e lugar, a todos os atos administrativos e aos registros dos fatos relacionados direta ou indiretamente com o
					objeto pactuado no Termo de Compromisso (Anexo I), bem como às obras e serviços a ele referidas, colaborando na obtenção de dados e de
					informações junto à comunidade local sobre os benefícios advindos da implantação do(s) projeto(s), quando em missão de fiscalização e auditoria;
				</p>
				<p class="P_1">
					<br />
					XVIII - Apresentar ao FNDE/MEC ou a seu(s) representante(s) legalmente constituído(s) o original ou a cópia autenticada de todo e qualquer documento comprobatório
					de despesa efetuada à conta dos recursos transferidos à conta do Programa, a qualquer tempo e a critério daquela Autarquia Federal;
				</p>
				<p class="P_1">
					<br />
					XIX - Prestar todo e qualquer esclarecimento sobre a execução física e financeira do Programa, sempre que solicitado pelo FNDE/MEC, pela SEB/MEC, por órgão do Sistema
					de Controle Interno do Poder Executivo Federal, pelo Tribunal de Contas da União, pelo Ministério Público ou por órgão ou entidade com delegação para esse fim;
				</p>
				<p class="P_1">
					<br />
					XX - Incluir no orçamento anual do Município, ou do estado, os recursos recebidos para execução do objeto deste Termo de Compromisso, nos termos estabelecidos no
					§ 1º, do art. 6º, da Lei nº 4.320, de 17 de março de 1964;
				</p>
				<p class="P_1">
					<br />
					XXI - Não considerar os valores transferidos no cômputo dos 25% (vinte e cinco por cento) de impostos e transferências devidos à manutenção e ao
					desenvolvimento do ensino, por força do disposto no art. 212 da Constituição Federal;
				</p>
				<p class="P_1">
					<br />
					XXII - Emitir o(s) termo(s) de aceitação definitiva da(s) obra(s), ao final da execução dos recursos, remetendo cópia autenticada do(s) mesmo(s)
					à DIRPE/FNDE para a emissão do(s) termo(s) de conclusão da(s) obra(s) e consolidação deste Termo de Compromisso;
				</p>
				<p class="P_1">
					<br />
					XXIII - Prestar contas ao FNDE/MEC dos recursos recebidos, no prazo e nas condições estipuladas nos artigos 29 e 30
					da Resolução CD/FNDE Nº 13/2011;
				</p>
				<p class="P_1">
					<br />
					XXIV - Manter em seu poder, à disposição do FNDE/MEC, da SEB/MEC, dos órgãos de controle interno e externo e do
					Ministério Público, os comprovantes das despesas efetuadas à conta do Programa, pelo prazo de 10 (dez) anos, contados
					da data da aprovação da prestação de contas anual do FNDE/MEC pelo Tribunal de Contas da União (TCU) a que se refere o exercício do repasse dos recursos,
					a qual será divulgada no sítio eletrônico <a href=\"www.fnde.gov.br\" style=\"color: blue\">www.fnde.gov.br</a>;
				</p>
				<p class="P_1">
					<br />
					XXV - Responsabilizar-se por todos os encargos de natureza trabalhista e previdenciária, decorrentes de eventuais
					demandas judiciais relativas a recursos humanos utilizados na execução do objeto deste Termo de Compromisso, bem
					como por todos os ônus tributários ou extraordinários que incidam sobre o presente Instrumento, ressalvados aqueles de natureza compulsória,
					lançados automaticamente pela rede bancária arrecadadora;
				</p>
				<p class="P_1">
					<br />
					XXVI - Adotar todas as medidas necessárias à correta execução deste Termo de Compromisso.
				</p>
				<p class="quebra">&nbsp;</p>
				<p class="P_3">
					<br />
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Declaro, em complementação, que a Secretaria de Educação
					cumpre com as exigências do art. 169 da Constituição Federal que trata dos limites de despesa com pessoal e,
					que os recursos próprios de responsabilidade do Secretaria estão assegurados, conforme a Lei Orgânica Municipal.
				</p>
				<p class="Data">
					<br />
					Brasília/DF, ___ de ____________________ de ______.
    		    </p>
				<p class="T_1">
					<br />
					<br />
					<br />
					____________________________________________
    		    <br />
    		    <br />
    		    <strong>'.$munDados[0]['prefeito'].'</strong>
    		        <br />
    					NOME DO(A) SECRETÁRIO(A) DE EDUCAÇÃO DO ESTADO <strong>'.strtoupper($munDados[0]['municipio']).'/'.$munDados[0]['estuf'].'</strong>
    				</p>
    			</div>
    		</body>
    	</html>';
    	}
    }

    /**
     * Monta o SQL de documentos de par e obras (par e pac) todos juntos
     *
     * @param $arrPost
     * @return string
     */

    public function montarSQLPAROBRAS($arrPost){

        $where = $this->motnarFiltroPAR($arrPost);

        /*  Trecho prazos e datas da omissao daprestaçao de contas */
        $listaDatasOmissao = $this->pegaLinha("SELECT dpcdatainiciocontagem1, dpcdatainiciocontagem2, dpcquantidadedias FROM par.datasprestacaocontas WHERE dpcid = 1");
        $dataInicioPCPar = $listaDatasOmissao['dpcdatainiciocontagem1'];
        $dataInicioPCPac = $listaDatasOmissao['dpcdatainiciocontagem2'];
        $qntDiasOmissao = $listaDatasOmissao['dpcquantidadedias'];

        $btnEnviada = "<button class=\" visualizar_item btn btn-success btn-sm\" type=\"button\" 
                    onclick=\"redirecionarPrestacaoContasPar2(' || id || ', ' || inuid || ', \'' || coalesce(estuf::text || ',','') || ' \', ' || 
                    coalesce(muncod::text || ',','') || itrid || ')\"
								 data-toggle=\"popover\" data-html=\"true\" title=\"\" data-trigger=\"hover\" data-placement=\"left\" data-content=\"Visualizar pretação de contas\">Enviada</button>";
        $btnEnviada = "";
        $colunaPrestacaoContasObrasPAC = "'<center>'||
					CASE WHEN EXISTS ( SELECT t.tspid FROM par.termossigpcpar t where t.proid_pac = pro.proid ) THEN
						'<a href=\"http://www.fnde.gov.br/sigpc\" target=\"_blank\"> <input type=\"button\" value=\"SIGPC\" title=\"SIGPC\"></a>'
                  WHEN(SELECT d.esdid FROM obras2.execucaofinanceira pc
                        INNER JOIN workflow.documento d ON pc.docid = d.docid WHERE pc.proid_pac = pro.proid AND pc.exestatus = 'A') <> ".ESDID_PC_OBRAS_EM_CADASTRAMENTO." THEN
                                        '<center><input type=\"button\" value=\"Enviada\" title=\"Visualizar Prestação de Contas no Obras 2.0\" onclick=\"\" disabled></center>'
                  ELSE
                    CASE WHEN (SELECT COALESCE( (SELECT
                                sum(CASE WHEN pag3.pagsituacaopagamento = '2 - EFETIVADO' THEN ps3.pobvalorpagamento ELSE 0 END) AS totalpagamento
                            FROM
                                par.processoobra 	pro1
                            INNER JOIN par.empenho 			emp3 ON emp3.empnumeroprocesso = pro1.pronumeroprocesso AND emp3.empcodigoespecie NOT IN ('03', '13', '02', '04') AND emp3.empstatus = 'A'
                            INNER JOIN par.pagamento 		pag3 ON pag3.empid = emp3.empid AND pag3.pagstatus = 'A' AND pag3.pagsituacaopagamento not ilike '%CANCELADO%' and pag3.pagsituacaopagamento not ilike '%vala siaf%' and pag3.pagsituacaopagamento not ilike '%devolvido%'
                            INNER JOIN par.pagamentoobra 	ps3  ON ps3.pagid = pag3.pagid
                            WHERE pro1.proid = pro.proid  GROUP BY pro1.proid) , 0 )) <= 0 THEN
                            cast(epc.esddsc as text)
                    WHEN (datafimvigencia > '".$dataInicioPCPac."') THEN
                          CASE WHEN (datafimvigencia + '".$qntDiasOmissao." day':: INTERVAL) :: DATE - current_date < 0 THEN
                              CASE WHEN dopc.esdid = ".ESDID_OBRAS_SITUACAO_OPC_INADIMPLENTE." THEN
                                cast(epc.esddsc as text) || ' - ' || cast(eopc.esddsc as text)
                              ELSE
                                cast(epc.esddsc as text)
                              END
                           ELSE
                                CASE WHEN (datafimvigencia + '".$qntDiasOmissao." day':: INTERVAL) :: DATE - current_date < 60 THEN
                                  'Restam ' || cast(
                                      ((datafimvigencia + '".$qntDiasOmissao." day':: INTERVAL):: DATE -
                                      current_date)  AS TEXT)
                                  || ' dias para envio da PC'
                                ELSE
                                  cast(epc.esddsc as text)
                                END
                          END
                    ELSE
                        CASE WHEN date '".$dataInicioPCPac."' + INTEGER '".$qntDiasOmissao."' - current_date < 0 THEN
                              CASE WHEN dopc.esdid = ".ESDID_OBRAS_SITUACAO_OPC_INADIMPLENTE." THEN
                                cast(epc.esddsc as text) || ' - ' || cast(eopc.esddsc as text)
                              ELSE
                                cast(epc.esddsc as text)
                              END
                        ELSE
                            CASE WHEN date '".$dataInicioPCPac."' + INTEGER '".$qntDiasOmissao."' - current_date < ".$qntDiasOmissao." THEN
                              'Restam ' || cast(
                                  (date '".$dataInicioPCPac."' + INTEGER '".$qntDiasOmissao."' - current_date) AS TEXT)
                              || ' dias para envio da PC'
                            ELSE
                              cast(epc.esddsc as text)
                            END
                        END
                    END
              END
          || '</center>' as situacao_prestacao_contas";

        $colunaPrestacaoContasObrasPar = "'<center>'||
				    CASE WHEN EXISTS ( SELECT tspid FROM par.termossigpcpar where prpid = idprocesso ) THEN
						'<a href=\"http://www.fnde.gov.br/sigpc\"  target=\"_blank\"
						   data-toggle = \"popover\" data-html = \"true\"data-trigger = \"hover\" data-placement = \"left\" data-content = \"Visualizar pretação de contas\" > SIGPC </a > '
                    WHEN EXISTS ( SELECT tspid FROM par.termossigpcpar where proid_par = idprocesso ) THEN
						'<a href=\"http://www.fnde.gov.br/sigpc\"  target=\"_blank\"
						   data-toggle = \"popover\" data-html = \"true\"data-trigger = \"hover\" data-placement = \"left\" data-content = \"Visualizar pretação de contas\" > SIGPC </a > ' 						   
                    WHEN ((SELECT d.esdid FROM obras2.execucaofinanceira pc
                            INNER JOIN workflow.documento d ON pc.docid = d.docid WHERE pc.proid_par = idprocesso AND pc.exestatus = 'A') <> situacao_cadastramento) 
                            AND obra = 1 THEN
                        '{$btnEnviada}'
            		WHEN ((SELECT d.esdid FROM par.documentoparprestacaodecontas pc
                            INNER JOIN workflow.documento d ON pc.docid = d.docid WHERE pc.prpid = foo.prpid) <> situacao_cadastramento) 
                            AND obra = 0
                    THEN
                       '{$btnEnviada}'
                  	ELSE
                    	CASE WHEN coalesce(valorpagamento, 0) <= 0 THEN
                        	cast(estado_documentopc as text)
                    	WHEN data_vigencia::date > '".$dataInicioPCPar."' THEN
                          CASE WHEN(data_vigencia::date + '".$qntDiasOmissao." day':: INTERVAL) :: DATE - current_date < 0
                            THEN
                              CASE WHEN id_opc = situacao_inadimplente THEN
                                cast(estado_documentopc as text) || ' - ' || cast(estado_opc as text)
                              ELSE
                                cast(estado_documentopc as text)
                              END
                         ELSE
						    /*CASE Formação*/
								CASE WHEN
	                        	(SELECT
	                            count(s.sbaid)
	                            FROM
	                            par.termocomposicao tc
	                            INNER JOIN par.subacaodetalhe sd ON sd.sbdid = tc.sbdid
	                            INNER JOIN par.subacao s ON s.sbaid = sd.sbaid AND s.sbastatus = 'A'
	                            WHERE
	                            dopid = id
	                            AND
	                            (s.prgid in (select DISTINCT prgid from par.programaformacao WHERE frmstatus = 'A')) ) > 0 THEN
		                        	'<center>Não Enviada</center>'  /* FIM Formação*/
		                        ELSE 	
		                            CASE WHEN(data_vigencia::date + '".$qntDiasOmissao." day':: INTERVAL) :: DATE - current_date < 60 THEN
		                              'Restam ' || cast(
		                                  ((data_vigencia::date + '".$qntDiasOmissao." day':: INTERVAL):: DATE -
		                                  current_date)  AS TEXT)
		                              || ' dias para envio da PC'
		                            ELSE
		                              estado_documentopc 
		                            END
					    		END
                        END 
                	ELSE
                CASE WHEN date '".$dataInicioPCPar."' + INTEGER '".$qntDiasOmissao."' - current_date < 0 THEN
                      CASE WHEN id_opc = situacao_inadimplente THEN
                        cast(estado_documentopc as text) || ' - ' || cast(estado_opc as text)
                      ELSE
                        cast(estado_documentopc as text)
                      END
                ELSE
                    CASE WHEN date '".$dataInicioPCPar."' + INTEGER '".$qntDiasOmissao."' - current_date < ".$qntDiasOmissao." THEN
                      'Restam ' || cast(
                          (date '".$dataInicioPCPar."' + INTEGER '".$qntDiasOmissao."' - current_date) AS TEXT)
                      || ' dias para envio da PC'
                    ELSE
                      cast(estado_documentopc as text)
                    END
                END
            END
          END
          || '</center>' as situacao_prestacao_contas";

        $sql = "(SELECT
                    id,
                    inuid,
                    estuf,
                    muncod,
                    itrid,
                    dopidpai,                   
                	processo,
                    CASE WHEN obra = 1
                        THEN docObra::text
                        ELSE doc::text
                	END as doc,
                	'<span class=\"dopid\" value=' || id || '></span>' || tipodocumento as tipodocumento,
                	CASE WHEN ( data_vigencia::date - current_date) < 90
                	          AND (data_vigencia::date - current_date) > 0 
                	        THEN
                		      '<span style=\"color:red\">' || data_vigencia || ' (' || (data_vigencia::date - CURRENT_DATE) || ' dias)'  || '</span>'
                		 WHEN EXISTS (select esdid from par.documentoparprestacaodecontas dpp11 INNER JOIN workflow.documento doc11 ON doc11.docid = dpp11.docid where dpp11.prpid = foo.prpid AND doc11.esdid <> ".ESDID_AGUARDANDO_ENVIO_PROPONENTE .")
                            THEN
                              data_vigencia || ' (Encerrado)'
                		 ELSE data_vigencia || ' (' || (data_vigencia::date - CURRENT_DATE) || ' dias)' 
                	END as data_vigencia,
                	CASE WHEN obra = 1 THEN qtdObra::text ELSE '-' END AS qtdObra,
                	dopvalortermo::text as vt,
                	valorempenho as ve,
                	valorpagamento as vp,
                	dados_bancarios,
                	(
                		SELECT COALESCE(saldo) as saldo
                		FROM
                        ( select par.retornasaldoprocesso(processo) AS saldo ) as saldomes
                	)  as sb,
                    arqid,
					{$colunaPrestacaoContasObrasPar},
					obra
                FROM
                (
                	SELECT
                		dp.dopid as id,
                		dp.dopidpai as dopidpai,
                		case when pop.proid > 0 then pronumeroprocesso else prpnumeroprocesso end as processo,
                		dp.dopnumerodocumento as doc,
                		(select dopnumerodocumento from par.documentopar where proid = dp.proid and dopstatus <> 'E' order by dopid asc LIMIT 1) as docObra,
                		mdonome as tipodocumento,
                		CASE WHEN pop.proid > 0 
                		    THEN par.retornavigenciaobrapar(pop.proid)
                		    ELSE (SELECT
                                        to_char((date_trunc('MONTH', vigencia::date) + INTERVAL '1 MONTH - 1 day')::date, 'DD/MM/YYYY') as vigencia
                                    FROM (
                                        SELECT
                                            to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia,
                                            CASE
                                                WHEN (dpv.dpvid IS NOT NULL AND d.dopstatus = 'A') THEN 0
                                                WHEN (dpv.dpvid IS NULL AND d.dopstatus = 'A' AND d.mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)) THEN 1
                                                WHEN (dpv.dpvid IS NULL AND d.dopstatus = 'I' AND d.mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)) THEN 2
                                                WHEN (dpv.dpvid IS NOT NULL AND d.dopstatus = 'I') THEN 3
                                                ELSE 4
                                            END AS prioridade
                                        FROM par.documentopar d
                                        LEFT JOIN par.documentoparvalidacao dpv ON d.dopid = dpv.dopid AND dpv.dpvstatus = 'A'
                                        WHERE d.prpid = pp.prpid AND d.dopstatus <> 'E'
                                        ORDER BY d.dopid DESC, prioridade
                                    ) AS foo LIMIT 1
                                )
                		 END as data_vigencia,	
                		(SELECT dopvalortermo::numeric(20,2) FROM par.documentopar  d
                			INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid
                			WHERE prpid = pp.prpid
                			AND dopstatus <> 'E'
                			AND dpvstatus = 'A'
                			AND mdoid NOT IN (79,65,66,68,76,80,67,73,82)
                			ORDER BY d.dopid DESC
                			LIMIT 1
                		) as dopvalortermo,
				        COALESCE((select sum(vrlempenhocancelado) from par.v_vrlempenhocancelado v where processo = pp.prpnumeroprocesso OR processo = pop.pronumeroprocesso), 0.00) as valorempenho,                	
                		pm.valor_pagamento as valorpagamento,
                		'Banco: '||coalesce(prpbanco, 'n/a')||'
                         Conta: '||coalesce(prpagencia, 'n/a')||'
                         Conta Corrente: '||coalesce( case when pop.proid > 0 then pop.nu_conta_corrente else pp.nu_conta_corrente end, 'n/a') as dados_bancarios,			  
                		dp.arqid,
                		d.tpdcod as tipo_doc,
                		pp.prpfinalizado,
                		iu.inuid,
                		iu.estuf,
                		iu.muncod,
                		dp.dopstatus,
                		dp.prpid,
                		CASE WHEN pop.proid > 0 THEN 1 ELSE 0 END AS obra, --diferenciar obra de par, obra = 1
                		CASE WHEN pop.proid > 0 THEN pop.proid ELSE pp.prpid END AS idprocesso,
                		CASE WHEN pop.proid > 0 THEN " .ESDID_PC_OBRAS_EM_CADASTRAMENTO . " ELSE " . ESDID_AGUARDANDO_ENVIO_PROPONENTE . " END as situacao_cadastramento,
                		CASE WHEN pop.proid > 0 THEN " .ESDID_OBRAS_SITUACAO_OPC_INADIMPLENTE . " ELSE " . ESDID_OPC_INADIMPLENTE . " END as situacao_inadimplente,
                		CASE WHEN pop.proid > 0 THEN epco.esddsc ELSE epc.esddsc END AS estado_documentopc,                	
                		CASE WHEN pop.proid > 0 THEN dopco.esdid  ELSE dopc.esdid END AS id_opc,                	
                		CASE WHEN pop.proid > 0 THEN eopco.esddsc ELSE eopc.esddsc END AS estado_opc,
                		CASE WHEN iu.estuf = 'DF' 
                		  THEN 1
                		  ELSE iu.itrid
                		END as itrid,
                		(select count(pc.preid) from par.processoobraspar po inner join par.processoobrasparcomposicao pc on pc.proid = po.proid where pc.pocstatus = 'A' and po.proid = pop.proid) as qtdObra
                	FROM
                		par.documentopar dp
                	INNER JOIN par.modelosdocumentos  d  ON d.mdoid = dp.mdoid
                	LEFT JOIN par.processopar 	pp ON pp.prpid = dp.prpid
                	LEFT JOIN par.processoobraspar 	pop ON pop.proid = dp.proid and pop.prostatus = 'A'
                	INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid OR iu.inuid = pop.inuid
                	LEFT JOIN par.situacaoprestacaocontas pc ON pp.prpid = pc.prpid
                    LEFT JOIN workflow.documento dpc ON dpc.docid = pc.docid
                    LEFT JOIN workflow.estadodocumento epc ON dpc.esdid = epc.esdid
                    LEFT JOIN par.situacaoopc opc ON pp.prpid = opc.prpid
                    LEFT JOIN workflow.documento dopc ON opc.docid = dopc.docid
                    LEFT JOIN workflow.estadodocumento eopc ON dopc.esdid = eopc.esdid
                    
                	LEFT JOIN obras2.situacaoprestacaocontasobras pco ON pop.proid = pco.proid_par
                    LEFT JOIN workflow.documento dpco ON dpco.docid = pco.docid
                    LEFT JOIN workflow.estadodocumento epco ON dpco.esdid = epco.esdid
                    LEFT JOIN obras2.situacaoopcobras opco ON pop.proid = opco.proid_par
                    LEFT JOIN workflow.documento dopco ON opco.docid = dopco.docid
                    LEFT JOIN workflow.estadodocumento eopco ON dopco.esdid = eopco.esdid
                    
                	LEFT JOIN LATERAL(
                		SELECT
                            coalesce(sum(pg.pagvalorparcela),0.00) AS valor_pagamento
                        FROM par.pagamento pg
                        JOIN par.empenho empe on empe.empid = pg.empid and empe.empstatus = 'A' AND pg.pagstatus = 'A' 
                        WHERE trim(pg.pagsituacaopagamento) ilike '%EFETIVADO%'
                        AND (empe.empnumeroprocesso = pp.prpnumeroprocesso OR empe.empnumeroprocesso = pop.pronumeroprocesso) 
                        AND pg.pagstatus='A'
                		) pm ON true

			        LEFT  JOIN LATERAL
                        (
                            SELECT
                                sum(valor) as valor
                            FROM
                            (
                                SELECT DISTINCT
                                    vve.vrlempenhocancelado as valor
                                FROM par.empenho emp2
                                INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = emp2.empid
                                LEFT JOIN LATERAL
                                ( SELECT sum(empvalorempenho) as vrlreforco
                                FROM par.empenho e
                                    WHERE empcodigoespecie IN ('02') AND empstatus = 'A' AND e.empidpai = emp2.empid
                                ) e ON true
                                INNER JOIN par.empenhoobrapar ems on ems.empid = emp2.empid and eobstatus = 'A'
                                WHERE emp2.empnumeroprocesso = pp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                            ) as foo
                        ) em ON true
                ) as foo
                WHERE
                	" .implode(' AND ', $where)."
                	--AND dopidpai IS NULL
                ORDER BY  data_vigencia::date --to_date(data_vigencia, 'DD/MM/YYYY'),
                )
                
                	UNION ALL
                	 
                (SELECT 
                    tc.terid,
                    0 as inuid,
                    tc.estuf,
                    tc.muncod,
                    0 as itrid,
                    tc.teridpai,
                    coalesce(pro.pronumeroprocesso::text, '-') as pronumeroprocesso,
                    to_char(tc.ternumero,'00000')||'/'||to_char(tc.terdatainclusao,'YYYY') as tipodocumento,
                    '<span class=\"terid\" value=' || tc.terid || '>PAC2</span>'  AS tipodocumento,
                    to_char(datafimvigencia, 'DD/MM/YYYY') || ' (' || datafimvigencia::date - CURRENT_DATE || ' dias)' as datafimvigencia,
                    
                    (   SELECT count(pc.preid) 
                        FROM par.processoobra po 
                        INNER JOIN par.processoobraspaccomposicao pc ON pc.proid = po.proid 
                        WHERE pc.pocstatus = 'A' AND po.proid = pro.proid
                    )::text as qtdObra,
                    
                    (   SELECT sum( prevalorobra )::text
                        FROM par.termoobra ter
                        INNER JOIN obras.preobra po on po.preid = ter.preid AND po.prestatus = 'A'
                        WHERE ter.terid = tc.terid 
                    ) as valor_termo,
                    emp.valor as valorempenho,
                    valor_pagamento,
                    'Banco: '||coalesce(probanco, 'n/a')||'
                     Conta: '||coalesce(proagencia, 'n/a')||'
                     Conta Corrente: '||coalesce(nu_conta_corrente, 'n/a') as dados_bancarios,            
                    par.retornasaldoprocesso(pronumeroprocesso) as sb,
                    tc.arqid,
                    {$colunaPrestacaoContasObrasPAC},
                    1 as obra
                    
                    FROM par.termocompromissopac tc 
                    INNER JOIN par.processoobra pro ON pro.proid = tc.proid AND pro.prostatus = 'A'
                    LEFT JOIN LATERAL (SELECT to_date((select to_char(tcp.terdatafimvigencia,'DD/MM/YYYY') from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' LIMIT 1),'DD/MM/YYYY')  as datafimvigencia) v ON true
                    JOIN obras2.situacaoopcobras opc ON opc.proid_pac = pro.proid
                    JOIN workflow.documento dopc ON opc.docid = dopc.docid
                    JOIN workflow.estadodocumento eopc ON dopc.esdid = eopc.esdid
                    LEFT JOIN obras2.situacaoprestacaocontasobras pc ON pro.proid = pc.proid_pac
                    JOIN workflow.documento dpc ON dpc.docid = pc.docid
                    JOIN workflow.estadodocumento epc ON dpc.esdid = epc.esdid
                    LEFT JOIN LATERAL (SELECT
                                            terid,
                                            sum(valor) as valor
                                        FROM
                                        (
                                            SELECT DISTINCT
                                                tc.terid,
                                                vve.empid,
                                                vve.vrlempenhocancelado + coalesce(emr.vrlreforco,0) as valor
                                            FROM
                                                par.termocompromissopac tc
                                            INNER JOIN par.processoobra 		prp ON prp.proid = tc.proid and prp.prostatus = 'A'
                                            INNER JOIN par.empenho 			emp ON emp.empnumeroprocesso = prp.pronumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                                            INNER JOIN par.v_vrlempenhocancelado 	vve ON vve.empid = emp.empid
                                            LEFT  JOIN (
                                                SELECT empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie
                                                FROM par.empenho
                                                WHERE empcodigoespecie in ('02') AND empstatus = 'A'
                                                GROUP BY empnumeroprocesso, empcodigoespecie, empidpai
                                            ) as emr on emr.empidpai = emp.empid
                                            INNER JOIN par.empenhoobra ems on ems.empid = emp.empid and eobstatus = 'A'
                                        ) as foo
                                        WHERE terid = tc.terid
                                        GROUP BY terid
                                      ) emp ON true
                    LEFT JOIN LATERAL (SELECT
                                        sum( pobvalorpagamento ) as valor_pagamento
                                       FROM par.empenho 		emp
                                       INNER JOIN par.pagamento 	pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
                                       INNER JOIN par.pagamentoobra 	ps  ON ps.pagid = pag.pagid
                                       WHERE pag.pagsituacaopagamento = '2 - EFETIVADO' 
                                         AND empcodigoespecie NOT IN ('03', '13', '02', '04') AND empstatus = 'A'
                                         AND emp.empnumeroprocesso = pro.pronumeroprocesso
                                             ) pm ON true 
                    
                    WHERE tc.terstatus = 'A'
                    AND  {$arrPost['wherepac']}
                ORDER BY
                	datafimvigencia)";

        return $sql;
    }

}//end Class
?>
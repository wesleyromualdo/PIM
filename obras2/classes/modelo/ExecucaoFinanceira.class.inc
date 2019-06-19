<?php

/**
 * Class ExecucaoFinanceira
 */
class ExecucaoFinanceira extends Modelo
{

    /**
     * Método responsável pela criação das sub-abas da aba "Execução Financeira".
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $url
     * @return string
     */
    public function criaSubAba($url, $habilitado, $obrid)
    {

        $processo = $this->retornarProcesso($obrid);
        

        $acao = substr($url, -1);
        if ($processo) {
            $arrAbas = array(
                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraLicitacao&acao={$acao}",
                    "mnutransacao" => "Licitação",
                    "mnudsc" => "Licitação"),
                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraContratacao&acao={$acao}",
                    "mnutransacao" => "Contratação",
                    "mnudsc" => "Contratação"),
                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceira&acao={$acao}",
                    "mnutransacao" => "Medição Execução Financeira",
                    "mnudsc" => "Medição"),

                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraNotas&acao={$acao}",
                    "mnutransacao" => "Nota Fiscal",
                    "mnudsc" => "Nota Fiscal"),
                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraPagamentos&acao={$acao}",
                    "mnutransacao" => "Pagamento",
                    "mnudsc" => "Pagamento"),
                array(
                    "mnulink" => "obras2.php?modulo=principal/listaDevolucaoGRU&acao={$acao}",
                    "mnutransacao" => "Devolução (GRU)",
                    "mnudsc" => "Devolução (GRU)"),
                array(
                    "mnulink" => "obras2.php?modulo=principal/finalizar&acao={$acao}",
                    "mnutransacao" => "Finalizar",
                    "mnudsc" => "Finalizar")
            );
        } else {
            $arrAbas = array(
                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraLicitacao&acao={$acao}",
                    "mnutransacao" => "Licitação",
                    "mnudsc" => "Licitação"),
                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraContratacao&acao={$acao}",
                    "mnutransacao" => "Contratação",
                    "mnudsc" => "Contratação"),
                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceira&acao={$acao}",
                    "mnutransacao" => "Medição Execução Financeira",
                    "mnudsc" => "Medição"),

                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraNotas&acao={$acao}",
                    "mnutransacao" => "Nota Fiscal",
                    "mnudsc" => "Nota Fiscal"),
                array(
                    "mnulink" => "obras2.php?modulo=principal/listaExecFinanceiraPagamentos&acao={$acao}",
                    "mnutransacao" => "Pagamento",
                    "mnudsc" => "Pagamento")
            );
        }


        $html = "";

        $html .= "
            <div class='row'>
                <br />
                <div class='col-md-12'>
                    <ul class='nav nav-tabs'>
        ";

        foreach ($arrAbas as $aba) {

            $classActive = $aba["mnulink"] == $url ? "class='active'" : null;
            $idLink = str_replace(" ", "", $arrAbas['mnutransacao']);
            $link = $aba["mnulink"];
            $linkDsc = $aba["mnudsc"];

            $html .= "<li {$classActive}>
                        <a id='{$idLink}' href='{$link}'>{$linkDsc}</a></li>";
        }

        $html .= "</ul></div></div>";

        return $html;
    }

    function getInuid($params)
    {

        $estuf = $params['estuf'];
        $muncod = $params['muncod'];

        if ($muncod == '' && $estuf != '') {
            $sqlInuid = "SELECT inuid FROM par.instrumentounidade  where estuf = '{$estuf}'";
        } else if ($muncod != '') {
            $sqlInuid = "SELECT inuid FROM par.instrumentounidade  where muncod = '{$muncod}'";

        } else {
            return '';
        }

        return $inuid = $this->pegaUm($sqlInuid);

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

        $datas = $this->pegaLinha("SELECT dpcdatainiciocontagem1, dpcdatainiciocontagem2, dpcdatalote1, dpcdatalote2, dpcquantidadedias from par3.datasprestacaocontas WHERE dpcid = 1");

        $dpcdatainiciocontagem1 = $datas['dpcdatainiciocontagem1'];
        $dpcdatainiciocontagem2 = $datas['dpcdatainiciocontagem2'];
        $dpcdatalote1 = $datas['dpcdatalote1'];
        $dpcquantidadedias = $datas['dpcquantidadedias'];
        $dpcdatalote2 = $datas['dpcdatalote2'];

        if (($dpcdatainiciocontagem2 != '') && ($dpcdatalote2 != '') && ($dpcquantidadedias != '')) {
            $sqlObrasPac = "
            
             WITH wf_documento_e_estado AS (
                	SELECT esd.esddsc, docid, doc.esdid FROM workflow.documento doc
                	INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                	where doc.tpdid in (344,328) 
        		)
                SELECT
                        pro.proid,
                        (select tcp.terdatafimvigencia from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' limit 1),
                        
                        doc.esdid as esdid_fase,
                        doc2.esdid as esdid_situacao,
                        doc.docid as docid_fase_pc,
                        doc2.docid as docid_situ_pc,
                        pro.estuf,
                        pro.muncod
                       
                FROM
                	par.processoobra pro   
                INNER JOIN obras2.execucaofinanceira exe ON exe.proid_pac = pro.proid AND exestatus = 'A'
                INNER JOIN wf_documento_e_estado doc ON doc.docid = exe.docid AND doc.esdid=2174
                INNER JOIN obras2.situacaoprestacaocontasobras  spo ON spo.proid_pac = pro.proid AND spostatus = 'A'
                INNER JOIN wf_documento_e_estado doc2 ON doc2.docid = spo.docid AND  doc2.esdid = 2273
                
                WHERE
                        pro.prostatus = 'A'
                        
                
                AND 
                	-- SE JÁ ESTIVER VENCIDO ANTES DE 01/08/2018 
                	CASE WHEN to_date((select to_char(tcp.terdatafimvigencia,'DD/MM/YYYY') from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' LIMIT 1),'DD/MM/YYYY') < '{$dpcdatainiciocontagem2}' THEN
                		-- ENTÃO CONTA OS DIAS A PARTIR DE 02/07/2018
                		/*(( date_trunc('MONTH', to_date('02/05/2018','DD/MM/YYYY'))  + INTERVAL ' 1 day')::date - current_date) * -1 > {$dpcquantidadedias} -- testes*/
                		(( date_trunc('MONTH', to_date('{$dpcdatalote2}','DD/MM/YYYY'))  + INTERVAL ' 1 day')::date - current_date) * -1 > {$dpcquantidadedias}
                	-- CASO NÃO ESTEJA VENCIDO
                	WHEN ( to_date((select to_char(tcp.terdatafimvigencia,'DD/MM/YYYY') from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' LIMIT 1),'DD/MM/YYYY') - current_date) > 0  THEN
                		-- RETORNA FALSO E ESTÁ OK
                		FALSE
                	-- CASO ESTEJA VENCIDO E NÃO ENTROU NA CONDIÇÃO DO 30/06/2018 
                	WHEN ( to_date((select to_char(tcp.terdatafimvigencia,'DD/MM/YYYY') from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' LIMIT 1),'DD/MM/YYYY') - current_date) < 0 THEN
                		-- CONTA {$dpcquantidadedias} DIAS A PARTIR DO DIA DO VENCIMENTO 
                		(( to_date((select to_char(tcp.terdatafimvigencia,'DD/MM/YYYY') from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' LIMIT 1),'DD/MM/YYYY') - current_date) * -1) > {$dpcquantidadedias}
                	ELSE
                		 FALSE
                	-- FIM
                	END
                AND
                        pro.proid not in (select distinct proid_pac from par.omissaopcente  where opcstatus = 'A' AND proid_pac IS NOT NULL)
                AND
                    	pro.proid not in (select distinct proid_pac from par.termossigpcpar WHERE proid_pac IS NOT NULL)
            {$offset}
            {$limite}
            ";
        }

        if (($dpcdatainiciocontagem1 != '') && (dpcdatalote1 != '') && ($dpcquantidadedias != '')) {
            $sqlObrasPar = "
                WITH wf_documento_e_estado AS (
                            	SELECT esd.esddsc, docid, doc.esdid FROM workflow.documento doc
                            	INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                            	where doc.tpdid in (344,328)
                    		)
                SELECT
                        pro.proid,
                        par.retornavigenciaobrapar(pro.proid),
                        
                        doc.esdid as esdid_fase,
                        doc2.esdid as esdid_situacao,
                        doc.docid as docid_fase_pc,
                        doc2.docid as docid_situ_pc,
                        pro.inuid
                FROM
                	par.documentopar dop 
                INNER JOIN  par.processoobraspar   pro on dop.proid = pro.proid 
                INNER JOIN obras2.execucaofinanceira exe ON exe.proid_par = pro.proid AND exestatus = 'A'
                INNER JOIN wf_documento_e_estado doc ON doc.docid = exe.docid AND doc.esdid=2174
                INNER JOIN obras2.situacaoprestacaocontasobras  spo ON spo.proid_par = pro.proid AND spostatus = 'A'
                INNER JOIN wf_documento_e_estado doc2 ON doc2.docid = spo.docid AND  doc2.esdid = 2273
                
                WHERE
                        dop.proid IS NOT NULL
                AND 
                	dopstatus = 'A'
                AND
                    pro.prostatus = 'A'
                AND 
                	-- SE JÁ ESTIVER VENCIDO ANTES DE 01/08/2018 
                	CASE WHEN to_date(par.retornavigenciaobrapar(pro.proid),'DD/MM/YYYY') < '{$dpcdatainiciocontagem1}' THEN
                		-- ENTÃO CONTA OS DIAS A PARTIR DE 02/07/2018
                		/* (( date_trunc('MONTH', to_date('02/05/2018','DD/MM/YYYY'))  + INTERVAL ' 1 day')::date - current_date) * -1 > {$dpcquantidadedias} -- testes */
                		(( date_trunc('MONTH', to_date('{$dpcdatalote1}','DD/MM/YYYY'))  + INTERVAL ' 1 day')::date - current_date) * -1 > {$dpcquantidadedias}
                	-- CASO NÃO ESTEJA VENCIDO
                	WHEN ( to_date(par.retornavigenciaobrapar(pro.proid),'DD/MM/YYYY') - current_date) > 0  THEN
                		-- RETORNA FALSO E ESTÁ OK
                		FALSE
                	-- CASO ESTEJA VENCIDO E NÃO ENTROU NA CONDIÇÃO DO 30/06/2018 
                	WHEN ( to_date(par.retornavigenciaobrapar(pro.proid),'DD/MM/YYYY') - current_date) < 0 THEN
                		-- CONTA {$dpcquantidadedias} DIAS A PARTIR DO DIA DO VENCIMENTO 
                		(( to_date(par.retornavigenciaobrapar(pro.proid),'DD/MM/YYYY') - current_date) * -1) > {$dpcquantidadedias}
                	ELSE
                		 FALSE
                	-- FIM
                	END
                AND
                        pro.proid not in (select distinct proid_par from par.omissaopcente  where opcstatus = 'A' AND proid_par IS NOT NULL)
                AND
                    	pro.proid not in (select distinct proid_par from par.termossigpcpar WHERE proid_par IS NOT NULL)
                {$offset}
                {$limite}
                ";
        }

        $arrProcessosObrasPar = $this->carregar($sqlObrasPar);
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou1 = (is_array($arrProcessosObrasPar)) ? TRUE : FALSE;
        $registrosPar = (is_array($arrProcessosObrasPar)) ? $arrProcessosObrasPar : Array();

        $arrProcessosObrasPac = $this->carregar($sqlObrasPac);
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou2 = (is_array($arrProcessosObrasPac)) ? TRUE : FALSE;
        $registrosPaC = (is_array($arrProcessosObrasPac)) ? $arrProcessosObrasPac : Array();

        $retornou = ($retornou1 || $retornou2) ? TRUE : FALSE;


        $arrReturn = Array(
            'existeRegistro' => $retornou,
            'cadastroRegistroPAR' => $registrosPar,
            'cadastroRegistroPAC' => $registrosPaC
        );
        return $arrReturn;
    }

    
    /**
     * 
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function retornaProcessosCACS($arrParams)
    {
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'includes/classes_simec.inc';
        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        $offset = ($arrParams['offset'] != '') ? "OFFSET {$arrParams['offset']} " : "";
        
        $sqlProcessos = "
          WITH wf_documento_e_estado AS (
            	SELECT esd.esddsc as descricao, docid, doc.esdid 
            	FROM workflow.documento doc
            	INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
            	where doc.tpdid in (".TPDID_FLUXO_SIT_PC_EXEC_FINAN_OBRAS.",".TPDID_FLUXO_PC_EXECUCAO_FINANCEIRA_OBRAS.")
    		)
            SELECT
                    pro.proid,
                    pro.pronumeroprocesso,
                    par.retornavigenciaobrapar(pro.proid),
                    doc.esdid as esdid_fase,
                    doc2.esdid as esdid_situacao,
                    doc.docid as docid_fase_pc,
                    doc2.docid as docid_situ_pc,
                    doc.descricao,
                    doc2.descricao as descricao2,
                    pro.inuid
            FROM
            	par.documentopar dop 
            INNER JOIN  par.processoobraspar   pro on dop.proid = pro.proid 
            INNER JOIN obras2.execucaofinanceira exe ON exe.proid_par = pro.proid AND exestatus = 'A'
            INNER JOIN wf_documento_e_estado doc ON doc.docid = exe.docid AND doc.esdid = ".ESDID_PC_CONTROLE_SOCIAL." 
            INNER JOIN obras2.situacaoprestacaocontasobras  spo ON spo.proid_par = pro.proid AND spostatus = 'A'
            INNER JOIN wf_documento_e_estado doc2 ON doc2.docid = spo.docid 
            
            WHERE
                    dop.proid IS NOT NULL
            AND 
            	dopstatus = 'A'
            AND
                pro.prostatus = 'A'
        		/*{$offset}
        		{$limite}*/
        		";
        		
        $arrProcessos = $this->carregar($sqlProcessos);
    
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
     * 
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
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
        // Busco as datas e populo as variáveis necessárias
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
                	SELECT esd.esddsc, (SELECT htddata FROM workflow.historicodocumento  where aedid = ".AEDID_OBRAS_ENVIAR_PRESTACAO_CONTAS." and docid = doc.docid ORDER BY hstid desc LIMIT 1) as data_tramitacao, docid, doc.esdid 
                	FROM workflow.documento doc
                	INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                	where doc.tpdid in (".TPDID_FLUXO_SIT_PC_EXEC_FINAN_OBRAS.",".TPDID_FLUXO_PC_EXECUCAO_FINANCEIRA_OBRAS.") AND doc.esdid in ( ".ESDID_PC_CONTROLE_SOCIAL.", ".ESDID_PC_ENVIADA_AO_CONTROLE_SOCIAL.")
        		)
                SELECT
                        pro.proid,
                        pro.pronumeroprocesso,
                        par.retornavigenciaobrapar(pro.proid),
                        doc.data_tramitacao,
                        doc.esdid as esdid_fase,
                        doc2.esdid as esdid_situacao,
                        doc.docid as docid_fase_pc,
                        doc2.docid as docid_situ_pc,
                        pro.inuid
                FROM
                	par.documentopar dop 
                INNER JOIN  par.processoobraspar   pro on dop.proid = pro.proid 
                INNER JOIN obras2.execucaofinanceira exe ON exe.proid_par = pro.proid AND exestatus = 'A'
                INNER JOIN wf_documento_e_estado doc ON doc.docid = exe.docid 
                INNER JOIN obras2.situacaoprestacaocontasobras  spo ON spo.proid_par = pro.proid AND spostatus = 'A'
                INNER JOIN wf_documento_e_estado doc2 ON doc2.docid = spo.docid AND doc2.esdid = ".ESDID_PC_ENVIADA_AO_CONTROLE_SOCIAL." /*Para trazer apenas as que ainda não foram notificadas*/
                
                WHERE
                        dop.proid IS NOT NULL
                AND 
                	dopstatus = 'A'
                AND
                    pro.prostatus = 'A'
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
        				CASE WHEN (((doc.data_tramitacao)::date - current_date)*-1) >  {$dpcquantidadedias} THEN 
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
     * Método responsável por obter os dados de determinada obra.
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @return array|bool|mixed|NULL
     */
    public function capturaDadosObra($obrid)
    {
        $obraOriginal = $this->pegaObraOriginal($obrid);

        if (!$obraOriginal) {
            return false;
        }

        $obraOriginal['obraVinculada'] = $this->pegaObrasVinculadas($obraOriginal['obrid'], array(), $obraOriginal['obrid']);
        return $obraOriginal;
    }


    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     */
    public function verificaExistenciaPagamentoObrasPar($proid)
    {
        if ($proid != '') {
            $sql = "
                SELECT coalesce(  (
            	select foo1.valor_pagamento FROM
            	(SELECT
            		SUM(pag1.pagvalorparcela) as valor_pagamento
            	from 
            		par.processoobraspar prp1
            	inner join par.empenho emp1 on emp1.empnumeroprocesso = prp1.pronumeroprocesso and emp1.empcodigoespecie not in ('03', '13', '02', '04') and emp1.empstatus = 'A'
            	inner join par.pagamento pag1 on pag1.empid = emp1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento = '2 - EFETIVADO'
            	where prp1.proid = $proid) as foo1
                ),0) 
            ";

            $resultado = $this->pegaUm($sql);

            return ($resultado > 0) ? TRUE : FALSE;


        } else {
            return false;
        }
    }


    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     */
    public function verificaExistenciaPagamentoObrasPac($proid)
    {
        if ($proid != '') {
            $sql = "
            	SELECT COALESCE( (SELECT
            		sum(CASE WHEN pag3.pagsituacaopagamento = '2 - EFETIVADO' THEN ps3.pobvalorpagamento ELSE 0 END) AS totalpagamento
            	FROM
            		par.processoobra 	pro1 
            	INNER JOIN par.empenho 			emp3 ON emp3.empnumeroprocesso = pro1.pronumeroprocesso AND emp3.empcodigoespecie NOT IN ('03', '13', '02', '04') AND emp3.empstatus = 'A'
            	INNER JOIN par.pagamento 		pag3 ON pag3.empid = emp3.empid AND pag3.pagstatus = 'A' AND pag3.pagsituacaopagamento not ilike '%CANCELADO%' and pag3.pagsituacaopagamento not ilike '%vala siaf%' and pag3.pagsituacaopagamento not ilike '%devolvido%'
            	INNER JOIN par.pagamentoobra 	ps3  ON ps3.pagid = pag3.pagid
            	WHERE pro1.proid = $proid  GROUP BY pro1.proid) , 0 )
            ";

            $resultado = $this->pegaUm($sql);
            $resultado = ($resultado > 0) ? TRUE : FALSE;
            return $resultado;

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
    public function retornaProcessosSemFluxo($arrParams)
    {
        // Recupera parametros

        $limite = ($arrParams['limit'] != '') ? "LIMIT {$arrParams['limit']} " : "";
        $offset = ($arrParams['offset'] != '') ? "OFFSET {$arrParams['offset']} " : "";

        // 'par.documentoparprestacaodecontas';
        /*
         * RN = Seleciona todos prpids que não possuam registro na tabela par3.documentoparprestacaodecontas
         * RN2 = Que não estajam contemplados pelo SIGPC
         */
        $sqlObrasPar = "
	    	SELECT DISTINCT
				proid,
				pronumeroprocesso
			FROM
				par.processoobraspar
			WHERE
				prostatus = 'A'
			AND
				proid not in (select distinct proid_par from obras2.execucaofinanceira WHERE proid_par IS NOT NULL AND exestatus = 'A')
			AND
				proid not in (select distinct proid_par from par.termossigpcpar WHERE proid_par IS NOT NULL)
	    	{$offset}
	    	{$limite}
    	";

        $sqlObrasPac = "
            SELECT DISTINCT
            	proid,
            	pronumeroprocesso
            FROM 
            	par.processoobra  
            WHERE
            	prostatus = 'A'
            AND
            	proid not in (select distinct proid_pac from obras2.execucaofinanceira WHERE proid_pac IS NOT NULL AND exestatus = 'A')
            AND
				proid not in (select distinct proid_pac from par.termossigpcpar WHERE proid_pac IS NOT NULL)
            {$offset}
            {$limite}
        ";

        $arrProcessosObrasPar = $this->carregar($sqlObrasPar);
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou1 = (is_array($arrProcessosObrasPar)) ? TRUE : FALSE;
        $registrosPar = (is_array($arrProcessosObrasPar)) ? $arrProcessosObrasPar : Array();

        $arrProcessosObrasPac = $this->carregar($sqlObrasPac);
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou2 = (is_array($arrProcessosObrasPac)) ? TRUE : FALSE;
        $registrosPaC = (is_array($arrProcessosObrasPac)) ? $arrProcessosObrasPac : Array();

        $retornou = ($retornou1 || $retornou2) ? TRUE : FALSE;


        $arrReturn = Array(
            'existeRegistro' => $retornou,
            'cadastroRegistroPAR' => $registrosPar,
            'cadastroRegistroPAC' => $registrosPaC
        );
        return $arrReturn;
    }
    
    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function retornaTotaisObrasCACS($arrDadosProcesso)
    {
        
        $proid = $arrDadosProcesso['proid'];
        
        if($proid != "") {
            $sql = "
                select
                	(
                		SELECT 
                		--po.proid, o.obrid, o.docid, doc.esdid
                		count(distinct obrid)
                		FROM par.processoobraspar po 
                		INNER JOIN par.documentopar dp ON dp.proid = po.proid AND dp.dopstatus = 'A'
                		INNER JOIN par.termocomposicao tc ON dp.dopid = tc.dopid
                		INNER JOIN obras2.obras o ON tc.preid = o.preid AND o.obridpai ISNULL AND o.obrstatus = 'A'  
                		INNER JOIN workflow.documento doc ON doc.docid = o.docid
                		WHERE 
                		po.proid = {$proid}
                		and
                		doc.esdid = ".OBR_ESDID_OBRA_CANCELADA."
                	) as canceladas,
                
                	-- total
                	(
                		SELECT 
                		count(obrid)
                
                		FROM par.processoobraspar po 
                		INNER JOIN par.documentopar dp ON dp.proid = po.proid AND dp.dopstatus = 'A'
                		INNER JOIN par.termocomposicao tc ON dp.dopid = tc.dopid
                		INNER JOIN obras2.obras o ON tc.preid = o.preid AND o.obridpai ISNULL AND o.obrstatus = 'A'  
                		INNER JOIN workflow.documento doc ON doc.docid = o.docid
                		WHERE 
                		po.proid = {$proid}
                
                
                	 ) 	 as total
                ";
            $arrTotais = $this->pegaLinha($sql);
            $existe = (is_array($arrTotais)) ? TRUE : FALSE;   
            
            return Array("existe" => $existe, "arrTotais" => $arrTotais);
            
        } else {
            
            return Array("existe" => FALSE);
        }
        
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
         * RN = Seleciona todos prpids que não possuam registro na tabela par3.documentoparprestacaodecontas
         * RN2 = Que não estajam contemplados pelo SIGPC
         */
        $sqlObrasPar = "
           WITH wf_documento_e_estado AS (
        		SELECT esd.esddsc, docid, doc.esdid FROM workflow.documento doc
        		INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
        		where doc.tpdid in (344,328, 345)  
    	   )
            SELECT 
            	pro.proid,
            	doc.docid as docid_fase_pc,
            	doc.esdid as esdid_fase,
            	doc2.docid as docid_situ_pc,
            	doc2.esdid as esdid_situacao,
            	doc3.docid as docid_opc,
            	doc3.esdid as esdid_opc
                            
            FROM 
            	par.documentopar dop
            INNER JOIN par.processoobraspar pro ON pro.proid = dop.proid AND prostatus = 'A'
            INNER JOIN obras2.execucaofinanceira exe ON exe.proid_par = pro.proid AND exestatus = 'A'
            INNER JOIN wf_documento_e_estado doc ON doc.docid = exe.docid 
            INNER JOIN obras2.situacaoprestacaocontasobras  spo ON spo.proid_par = pro.proid AND spostatus = 'A'
            INNER JOIN wf_documento_e_estado doc2 ON doc2.docid = spo.docid 
            INNER JOIN obras2.situacaoopcobras sitopc ON sitopc.proid_par = pro.proid
            INNER JOIN wf_documento_e_estado doc3 ON doc3.docid = sitopc.docid
            
            WHERE 
            	/*Tenha vigência*/
            	dopdatafimvigencia IS NOT NULL 
            AND 
            	/*Esteja vencido*/
            	to_date(dopdatafimvigencia,'DD/MM/YYYY') < current_date
            AND 
            	/*termoativo*/
            	dopstatus = 'A'
            AND
        	/*Não recebeu recurso */
            coalesce(  (
            	select foo1.valor_pagamento FROM
            	(SELECT
            		SUM(pag1.pagvalorparcela) as valor_pagamento
            	from 
            		par.processoobraspar prp1
            	inner join par.empenho emp1 on emp1.empnumeroprocesso = prp1.pronumeroprocesso and emp1.empcodigoespecie not in ('03', '13', '02', '04') and emp1.empstatus = 'A'
            	inner join par.pagamento pag1 on pag1.empid = emp1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento = '2 - EFETIVADO'
            	where prp1.proid = pro.proid) as foo1
                ),0)  <= 0
            
            
            AND
                /*Não tenha sido tramitada ainda pela rotina*/ 
        		((doc.esdid <> " . ESDID_PC_OBRAS_EM_CADASTRAMENTO . ") OR (doc2.esdid <> " . ESDID_SPC_OBR_SEM_VLR_A_COMPROVAR . ") OR (doc3.esdid <> " . ESDID_OPC_OBRAS_CONCLUIDA . "))
        	AND 
                pro.proid not in (select distinct proid_par from par.termossigpcpar  where  proid_par is not null)
            {$offset}
            {$limite}
        ";

        $sqlObrasPac = "
            WITH wf_documento_e_estado AS (
                	SELECT esd.esddsc, docid, doc.esdid FROM workflow.documento doc
                	INNER JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid
                	where doc.tpdid in (344,328, 345) 
        		)
            SELECT 
            	pro.proid,
            	doc.docid as docid_fase_pc,
            	doc.esdid as esdid_fase,
            	doc2.docid as docid_situ_pc,
            	doc2.esdid as esdid_situacao,
            	doc3.docid as docid_opc,
            	doc3.esdid as esdid_opc
            FROM 
            	par.termocompromissopac  ter
            INNER JOIN par.processoobra pro ON pro.proid = ter.proid AND terstatus = 'A'
            INNER JOIN obras2.execucaofinanceira exe ON exe.proid_pac = pro.proid AND exestatus = 'A'
            INNER JOIN wf_documento_e_estado doc ON doc.docid = exe.docid 
            INNER JOIN obras2.situacaoprestacaocontasobras  spo ON spo.proid_pac = pro.proid AND spostatus = 'A'
            INNER JOIN wf_documento_e_estado doc2 ON doc2.docid = spo.docid 
            INNER JOIN obras2.situacaoopcobras sitopc ON sitopc.proid_pac = pro.proid
            INNER JOIN wf_documento_e_estado doc3 ON doc3.docid = sitopc.docid
            
            WHERE 
            	/*Tenha vigência*/
            	terdatafimvigencia IS NOT NULL 
            AND 
            	/*Esteja vencido*/
            	terdatafimvigencia < current_date 
            AND 	
            	/*termoativo*/
            	terstatus = 'A' 
            AND 
            /*Não recebeu recurso */
                COALESCE( (SELECT
                    		sum(CASE WHEN pag3.pagsituacaopagamento = '2 - EFETIVADO' THEN ps3.pobvalorpagamento ELSE 0 END) AS totalpagamento
                    	FROM
                    		par.processoobra 	pro1 
                    	INNER JOIN par.empenho 			emp3 ON emp3.empnumeroprocesso = pro1.pronumeroprocesso AND emp3.empcodigoespecie NOT IN ('03', '13', '02', '04') AND emp3.empstatus = 'A'
                    	INNER JOIN par.pagamento 		pag3 ON pag3.empid = emp3.empid AND pag3.pagstatus = 'A' AND pag3.pagsituacaopagamento not ilike '%CANCELADO%' and pag3.pagsituacaopagamento not ilike '%vala siaf%' and pag3.pagsituacaopagamento not ilike '%devolvido%'
                    	INNER JOIN par.pagamentoobra 	ps3  ON ps3.pagid = pag3.pagid
                    	WHERE pro1.proid = pro.proid  GROUP BY pro1.proid) , 0 ) <= 0 
        
        	AND
                /*Não tenha sido tramitada ainda pela rotina*/ 
        		((doc.esdid <> " . ESDID_PC_OBRAS_EM_CADASTRAMENTO . ") OR (doc2.esdid <> " . ESDID_SPC_OBR_SEM_VLR_A_COMPROVAR . ") OR (doc3.esdid <> " . ESDID_OPC_OBRAS_CONCLUIDA . "))
            AND 
                pro.proid not in (select distinct proid_pac from par.termossigpcpar  where  proid_pac is not null)
        {$offset}
        {$limite}
        ";

        $arrProcessosObrasPar = $this->carregar($sqlObrasPar);
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou1 = (is_array($arrProcessosObrasPar)) ? TRUE : FALSE;
        $registrosPar = (is_array($arrProcessosObrasPar)) ? $arrProcessosObrasPar : Array();

        $arrProcessosObrasPac = $this->carregar($sqlObrasPac);
        // Caso retorne algum processo retorna os registros, e a variável 'retorno' = true, senão a váriável retorno = false
        $retornou2 = (is_array($arrProcessosObrasPac)) ? TRUE : FALSE;
        $registrosPaC = (is_array($arrProcessosObrasPac)) ? $arrProcessosObrasPac : Array();

        $retornou = ($retornou1 || $retornou2) ? TRUE : FALSE;


        $arrReturn = Array(
            'existeRegistro' => $retornou,
            'cadastroRegistroPAR' => $registrosPar,
            'cadastroRegistroPAC' => $registrosPaC
        );
        return $arrReturn;
    }


    /**
     * Função responsável por pesquisar as construtoras cadastradas para determinada obra.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @return array|mixed|NULL
     */

    function buscaConstrutoras($obrid, $tipo = 'padrao')
    {
        global $db;

        $execucaoFinanceira = new ExecucaoFinanceira();
        $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
        $strObrids = implode(",", $arrObrids);

        if ($tipo == 'padrao') {
            $sql = "
		SELECT
			ent.entid,
			ent.entnumcpfcnpj,
			ent.entnome,
			c.crtid,
			to_char(c.crtdtassinatura, 'DD/MM/YYYY') as crtdtassinatura
		FROM
			obras2.obrascontrato ocr
		INNER JOIN
			obras2.contrato c ON ocr.crtid = c.crtid OR ocr.crtid = c.crtidpai
		INNER JOIN
			entidade.entidade ent ON ent.entid = c.entidempresa
		WHERE
			ocr.ocrstatus = 'A'
		AND
			c.crtstatus = 'A'
		AND
			ent.entstatus = 'A'
		AND
			c.ttaid ISNULL
		AND
			ocr.obrid IN ({$strObrids})
			ORDER BY crtid ASC";

            $arrConstrutora = $db->carregar($sql);

            if (!is_array($arrConstrutora)) {
                $arrConstrutora = array();
            }

            return $arrConstrutora;
        } else {
            if ($tipo == 'extra') {
                $sql = "
		SELECT
			cex.cexid,
			cex.cexrazsocialconstrutora,
			cex.cexnumcnpj,
			cce.ccedataassinatura,
			cce.cceid
		FROM
			obras2.construtoraextra cex
		LEFT JOIN
			obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
		WHERE
			cexstatus = 'A'
		AND
			obrid IN ({$strObrids})
		AND
			cce.ttaid IS NULL
		AND
			cce.cceid_pai IS NULL
			ORDER BY cce.ccedataassinatura
		";

                $arrConstrutoraExtra = $db->carregar($sql);

                if (!is_array($arrConstrutoraExtra)) {
                    $arrConstrutoraExtra = array();
                }

                return $arrConstrutoraExtra;
            }
        }
    }

    /**
     * Função responsável por pesquisar as medições ligadas a determinado contrato de determinada obra.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @param null $crtid
     * @param null $cceid
     * @return array|mixed|NULL
     */
    function buscaMedicoes($obrid, $crtid = null, $cceid = null)
    {

        $arrObrids = $this->retornaObrids($obrid);
        $strObrids = implode(",", $arrObrids);
        $filtroEntid = "";
        $filtroCexid = "";

        if (!empty($crtid)) {
            $filtroCrtid = "AND crtid = {$crtid}";
        }
        if (!empty($cceid)) {
            $filtroCceid = "AND cceid = {$cceid}";
        }

        $sql = "SELECT
              medid,
              mednummedicao as nummedicao,
              to_char(meddtmedicao, 'DD/MM/YYYY') as dtmedicao,
              to_char(meddtinicioexec, 'DD/MM/YYYY') || ' à ' || to_char(meddtfimexec, 'DD/MM/YYYY') as periodomedicao,
              medvlrmedicao as valormedicao,
              arqid
            FROM
              obras2.medicoes
            WHERE
              medstatus = 'A'
            AND
              obrid IN ({$strObrids})
            {$filtroCrtid}
            {$filtroCceid}
            ORDER BY medid, meddtmedicao, mednummedicao";

        $arrMedicaoes = $this->carregar($sql);

        if (!is_array($arrMedicaoes)) {
            $arrMedicaoes = array();
        }
        elseif(empty($crtid) && empty($cceid)) {
            $arrMedicaoes = array();
        }


        return $arrMedicaoes;
    }

    /**
     * Função responsável por montar HTML das medições ligadas a determinado contrato de determinada obra.
     * @author Kelvim
     * @param $obrid
     * @param $exibirAcoes =true
     */
    public function montarMedicao($obrid, $exibirAcoes = true)
    {
        $html = "";
        $arrConstrutoras = $this->buscaConstrutoras($obrid);

        foreach ($arrConstrutoras as $construtora) {

            $html .= "
            <table class='table table-bordered' cellspacing='2' cellpadding='2' border='0' align='center' width='95%'>
                	<thead>
                    	<tr>
                        	<td colspan='8' style='text-align: left'>
                            	<p>Empresa Contratada:
                                	(" . formatar_cnpj($construtora['entnumcpfcnpj']) . ")&nbsp;" . $construtora['entnome'] . "&nbsp;(Contrato: " . $construtora['crtdtassinatura'] . ")
                                </p>
                            </td>
                        </tr>
                        <tr>
                            <td>Ações</td>
    		                <td>Número da Medição</td>
                            <td>Data da Medição</td>
                            <td>Período de Execução</td>
                            <td>Valor da Medição</td>
                        </tr>
            		</thead>
                <tbody>";

            $arrMedicoes = $this->buscaMedicoes($obrid, $construtora['crtid']);

            if (!empty($arrMedicoes)) {
                $valorTotal = 0;
                foreach ($arrMedicoes as $medicao) {
                    $html .= "<tr>
                                <td>";
                    if ($exibirAcoes) {

                        if (verificaPendenciaMedicao($medicao['medid'])) {
                            $html .= "<img src='../imagens/atencao.png' title='Pendência de preenchimento' width='16' style=cursor:pointer;'>";
                        }
                        if (!$execucaoFinanceiraFinalizada) {
                            $html .= "&nbsp;
                                    <img src='../imagens/alterar.gif' title='Editar Medição' width='16' style=cursor:pointer; onclick='editarMedicao(" . $medicao['medid'] . ")'>&nbsp;
                                    <img src='../imagens/excluir.gif' title='Excluir Medição' width='16' style=cursor:pointer; onclick='excluirMedicao(" . $medicao['medid'] . ")'>&nbsp;";
                        }
                    }

                    $html .= "<img src='../imagens/salvar.png' title='Download Medição' width='16' style=cursor:pointer; onclick='downloadArquivo(" . $medicao['arqid'] . ")'>&nbsp;
                            </td>";


                    $html .= "
                        	<td>" . $medicao['nummedicao'] . "</td>
                            <td>" . $medicao['dtmedicao'] . "</td>
                            <td>" . $medicao['periodomedicao'] . "</td>
                            <td>" . formata_numero_monetario($medicao['valormedicao']) . "</td>
                        </tr>";

                    $valorTotal += $medicao['valormedicao'];
                }
                if (count($arrMedicoes) > 0) {
                    $html .= "
    				<tr>
                        <td colspan='3'></td>
                        <td style='background-color: #DCDCDC' ><b>Valor Total:</b></td>
                        <td style='background-color: #DCDCDC'><b>" . number_format($valorTotal, 2, ",", ".") . "</b></td>
                    </tr>";
                }
            } else {

                $html .= "<tr>
                    <td colspan='5' style='text-align: center'>Não foram encontrados registros</td>
                  </tr>";
            }
            $html .= "
                    </tbody>
            	</table>";
        }

        $html .= "
        	 <table class='table table-bordered'>
                <thead>
                    <tr class='divFaixa'>
                         <td>        <p style='font-weight: bold; font-size: 16px;'>Construtoras e Contratos Incluídos</p></p></td>
                    </tr>
                 </thead>
             </table>";

        $arrConstrutorasEx = $this->buscaConstrutorasExtra($obrid);
        $semRegistro = "";
        if (empty($arrConstrutorasEx)) {
            $arrConstrutoras = array();
            $semRegistro = " <br>
                    <table class='table table-bordered'>
                        <thead>
                            <tr>
                                <td>Até o momento, não há registro de medições extras cadastradas.</td>
                            </tr>
                        </thead>
                    </table>";
        }

        foreach ($arrConstrutorasEx as $construtoraEx) {

            $imgEditarConstrutora = "<img src='../imagens/alterar.gif' title='Editar Construtora / Inserir aditivo de valor' width='16' style=cursor:pointer; onclick='editarConstrutora(" . $construtoraEx['cexid'] . ")'>&nbsp;";
            $imgExcluirConstrutora = "<img src='../imagens/excluir.gif' title='Excluir Construtora' width='16' style=cursor:pointer; onclick='excluirConstrutora(" . $construtoraEx['cexid'] . ")'>&nbsp;";;
            $imgPendenciaConstrutora = "";

            if ($construtoraEx['ccedataassinatura'] != "") {
                $contrato = "Contrato: " . formata_data($construtoraEx['ccedataassinatura']);
            } else {
                $imgPendenciaConstrutora = "<img src='../imagens/atencao.png' title='Pendência de Preenchimento Construtora' width='16' style=cursor:pointer; onclick='javascript:return false;'>&nbsp;";
                $contrato = "Construtora não possui contrato assinado";
            }
            $html .= "
                <table class='table table-bordered'>
                	<thead>
                    	<tr>
                        	<td colspan='5'>
                            	<div class='row' >
                                	<div class='col-md-8' style='text-align: left'>
                                        	<b>Empresa Contratada:</b>
                                               (" . formatar_cnpj($construtoraEx['cexnumcnpj']) . ")&nbsp;" . $construtoraEx['cexrazsocialconstrutora'] . "&nbsp;(" . $contrato . ")
                                    </div>
                                </div>
                            </td>
        				</tr>
                        <tr>
                            <td>Ações</td>
                        	<td>Número da Medição</td>
                            <td>Data da Medição</td>
                            <td>Período de Execução</td>
                            <td>Valor da Medição</td>
                        </tr>
        			</thead>
                    <tbody>";

            $arrMedicoes = $this->buscaMedicoes($obrid, null, $construtoraEx['cceid']);
            $valorTotal = 0;
            if (!empty($arrMedicoes)) {
                foreach ($arrMedicoes as $medicao) {
                    $html .= "
                            	<tr>
                                    <td>";
                    if (!$execucaoFinanceiraFinalizada && $exibirAcoes) {
                        $html .= "<img src='../imagens/alterar.gif' title='Editar Medição' width='16' style=cursor:pointer; onclick='editarMedicao(" . $medicao['medid'] . ")'>&nbsp;
                                  <img src='../imagens/excluir.gif' title='Excluir Medição' width='16' style=cursor:pointer; onclick='excluirMedicao(" . $medicao['medid'] . ")'>&nbsp;";
                    }
                    $html .= "<img src='../imagens/salvar.png' title='Download Medição' width='16' style=cursor:pointer; onclick='downloadArquivo(" . $medicao['arqid'] . ")'>&nbsp;
                                    </td>
                                	<td>" . $medicao['nummedicao'] . "</td>
                                    <td>" . $medicao['dtmedicao'] . "</td>
                                    <td>" . $medicao['periodomedicao'] . "</td>
                                    <td>" . formata_numero_monetario($medicao['valormedicao']) . "</td>
                                </tr>";
                    $valorTotal += $medicao['valormedicao'];
                }
                if (count($arrMedicoes) > 0) {
                    $html .= "
                                <tr>
                                	<td colspan='3'></td>
                                    <td style='background-color: #DCDCDC' ><b>Valor Total:</b></td>
                                    <td style='background-color: #DCDCDC'><b>" . number_format($valorTotal, 2, ",", ".") . "</b></td>
                                </tr>";
                }
            } else {
                $html .= "
                                <tr>
                                    <td colspan='5' style='text-align: center'>Não foram encontrados registros</td>
                                </tr>";
            }
            $html .= "
                    </tbody>
            	</table>";
        }
        $html .= $semRegistro;

        echo $html;

    }


    /**
     * retorna processos que não possuem cadastro no fluxo, nem foram para o SIGPC
     *
     * @param Array $arrParams
     * @return Array $arrReturn multitype:boolean(retorno), Array(registros)
     */
    public function insereRegistrosInexistenteObrasPar($arrRegistros)
    {
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'includes/classes_simec.inc';

        $arrValores = Array();

        $arrRegistros = (is_array($arrRegistros)) ? $arrRegistros : Array();

        foreach ($arrRegistros as $k => $v) {
            $docidFluxo = '';
            $proid = '';

            $docidFluxo = wf_cadastrarDocumento(TPDID_FLUXO_PC_EXECUCAO_FINANCEIRA_OBRAS, "Cadastro de documento do Fluxo de prestação de contas Obras PAR - Via acompanhamento", ESDID_PC_OBRAS_EM_CADASTRAMENTO);

            $proid = $v['proid'];
            $processoObras = $v['pronumeroprocesso'];


            if (($proid != '') && ($docidFluxo != '') && ($processoObras != '')) {
                $arrValores[] = "({$proid}, {$docidFluxo}, '{$processoObras}')";
            }
        }

        if (count($arrValores) > 0) {
            $sql = "
                INSERT INTO obras2.execucaofinanceira
	               (proid_par, docid, exeprocesso)
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
    public function insereRegistrosInexistenteObrasPac($arrRegistros)
    {
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'includes/classes_simec.inc';

        $arrValores = Array();

        $arrRegistros = (is_array($arrRegistros)) ? $arrRegistros : Array();

        foreach ($arrRegistros as $k => $v) {
            $docidFluxo = '';
            $proid = '';

            $docidFluxo = wf_cadastrarDocumento(TPDID_FLUXO_PC_EXECUCAO_FINANCEIRA_OBRAS, "Cadastro de documento do Fluxo de prestação de contas Obras PAR - Via acompanhamento", ESDID_PC_OBRAS_EM_CADASTRAMENTO);

            $proid = $v['proid'];
            $processoObras = $v['pronumeroprocesso'];


            if (($proid != '') && ($docidFluxo != '') && ($processoObras != '')) {
                $arrValores[] = "({$proid}, {$docidFluxo}, '{$processoObras}')";
            }
        }

        if (count($arrValores) > 0) {
            $sql = "
                INSERT INTO obras2.execucaofinanceira
	               (proid_pac, docid, exeprocesso)
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
     * Método responsável por obter os dados da obra original.
     * author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @return array|bool|mixed|NULL
     */
    private function pegaObraOriginal($obrid)
    {

        $strSQL = "
            SELECT
                obr.obrid,
                obr.obridvinculado,
                obr.obrstatus,
                oc.crtid,
                c.entidempresa,
                e.entnome,
                e.entnumcpfcnpj,
                cod.codid,
                co.coid
            FROM obras2.obras obr
            LEFT JOIN obras2.obrascontrato oc ON (obr.obrid = oc.obrid AND oc.ocrstatus = 'A')
            LEFT JOIN obras2.contrato c ON (oc.crtid = c.crtid)
            LEFT JOIN entidade.entidade e ON (c.entidempresa = e.entid)
            LEFT JOIN obras2.cumprimento_objeto co ON (obr.obrid = co.obrid)
            LEFT JOIN obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid AND cod.codstatus = 'A')
            WHERE obr.obrid = {$obrid}
                AND obrstatus IN ('A')
            ";
        $obra = $this->pegaLinha($strSQL);

        if ($obra) {
            return $obra;
        }

        $strSQL = "SELECT obrid, obridvinculado FROM obras2.obras WHERE obridvinculado = {$obrid}";
        $obra = $this->pegaLinha($strSQL);
        return $this->pegaObraOriginal($obra['obrid']);
    }

    /**
     * Método responsável por obter os dados da(s) obra(s) vinculada(s) a uma obra original.
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @param array $vinculados
     * @param $principal
     * @return array
     */
    private function pegaObrasVinculadas($obrid, $vinculados = array(), $principal)
    {
        $strSQL = "
            SELECT
                obr.obrid,
                obr.obridvinculado,
                obr.obrstatus,
                oc.crtid,
                c.entidempresa,
                e.entnome,
                e.entnumcpfcnpj,
                cod.codid
            FROM obras2.obras obr
            LEFT JOIN obras2.obrascontrato oc ON (obr.obrid = oc.obrid AND oc.ocrstatus = 'A')
            LEFT JOIN obras2.contrato c ON (oc.crtid = c.crtid)
            LEFT JOIN entidade.entidade e ON (c.entidempresa = e.entid)
            LEFT JOIN obras2.cumprimento_objeto co ON ($principal = co.obrid)
            LEFT JOIN obras2.cumprimento_objeto_documentacao cod ON (co.coid = cod.coid AND cod.codstatus = 'A' AND obr.obrid = cod.obridvinculado)
            WHERE obr.obridvinculado = {$obrid}
                AND obr.obrstatus IN ('P');
        ";
        $vinculada = $this->pegaLinha($strSQL);

        if ($vinculada) {
            $vinculados[] = $vinculada;
            return $this->pegaObrasVinculadas($vinculada['obrid'], $vinculados, $principal);
        }

        return $vinculados;
    }

    /**
     * Método responsável por obter os 'obrids' de determinada obra (Original + vinculadas).
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @return array
     */
    public function retornaObrids($obrid)
    {

        $dadosObra = $this->capturaDadosObra($obrid);
        $arrObrids = array();

        if (is_array($dadosObra) && !empty($dadosObra)) {

            $arrObrids[] = $dadosObra["obrid"];

            if (is_array($dadosObra["obraVinculada"]) && !empty($dadosObra["obraVinculada"])) {
                foreach ($dadosObra["obraVinculada"] as $obraVinculada) {
                    $arrObrids[] = $obraVinculada["obrid"];
                }
            }

        }

        return $arrObrids;
    }

    /**
     * Método responsável por obter informações de um arquivo de medição.
     * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $arqid
     * @return array
     */
    public function recuperaDadosArquivo($arqid)
    {

        if (ctype_digit($arqid)) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $files = new FilesSimec();
            return $files->getDadosArquivo($arqid);
        }
    }

    /**
     * @todo comentar.
     * @param $crtid
     * @param $cceid
     * @return bool
     */
    function verificaContratoOriginal($crtid, $cceid)
    {
        global $db;

        if (!$crtid && !$cceid) {
            return false;
        }

        if (ctype_digit($crtid) && !$cceid) {
            $sql = "SELECT crtid FROM obras2.contrato WHERE crtstatus = 'A' AND ttaid ISNULL AND crtid = {$crtid}";
        } elseif (ctype_digit($cceid) && !$crtid) {
            $sql = "SELECT ccevalor FROM obras2.contratoconstrutoraextra WHERE ccestatus = 'A' AND cceid_pai ISNULL AND ttaid ISNULL AND cceid = {$cceid}";
        }

        if ($db->pegaUm($sql)) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @todo COMENTAR.
     * @param $medid
     * @param $valor
     * @param $obrid
     * @param $dataNota
     * @param $valorObj
     * @return array
     */
    function calculaPerc($ntfid, $obrid, $dataNota)
    {

        global $db;

        $arrObrids = $this->retornaObrids($obrid);
        $strObrids = implode(",", $arrObrids);
        $dataNota = formata_data_sql($dataNota);

        $sqlMedFisica = "WITH vtotal AS (SELECT DISTINCT ON (ntm.ntfid)
                                  ntm.ntfid,
                                  CASE WHEN mec.cceid ISNULL AND mec.crtid NOTNULL
                                    THEN
                                      (SELECT crtvalorexecucao
                                       FROM obras2.contrato
                                       WHERE crtstatus = 'A' AND ttaid ISNULL AND crtid = mec.crtid)
                                  ELSE
                                    (SELECT ccevalor
                                     FROM obras2.contratoconstrutoraextra
                                     WHERE ccestatus = 'A' AND cceid_pai ISNULL AND ttaid ISNULL AND cceid = mec.cceid)
                                  END AS valorcontrato
                                FROM
                                  obras2.notamedicao ntm
                                  INNER JOIN
                                  obras2.medicoes med ON ntm.medid = med.medid AND med.medstatus = 'A'
                                  INNER JOIN
                                  obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
                                WHERE
                                  ntm.ntmstatus = 'A'
                                  AND ntm.ntfid = {$ntfid}
                                GROUP BY valorcontrato, ntfid),
                    valor AS (SELECT
                                sum(ntm.ntmvlrpago) AS valor,
                                vt.valorcontrato
                              FROM
                                vtotal vt
                                INNER JOIN
                                obras2.notamedicao ntm ON vt.ntfid = ntm.ntfid
                                INNER JOIN
                                obras2.contratoconstrutoraextra cce
                                  ON ntm.cceid = cce.cceid AND cce.cceid_pai ISNULL AND cce.ttaid ISNULL
                              WHERE
                                cce.ccestatus = 'A'
                              GROUP BY vt.valorcontrato
                
                              UNION ALL
                
                              SELECT
                                sum(ntm.ntmvlrpago) AS valor,
                                vt.valorcontrato
                              FROM
                                vtotal vt
                                INNER JOIN
                                obras2.notamedicao ntm ON vt.ntfid = ntm.ntfid
                                INNER JOIN
                                obras2.contrato crt ON ntm.crtid = crt.crtid AND ttaid ISNULL
                              WHERE
                                crt.crtstatus = 'A'
                              GROUP BY vt.valorcontrato
                  )
                    SELECT (v.valor * 100) / v.valorcontrato AS percentfisica
                    FROM valor v;
    ";

        $medicaoFisica = $db->pegaUm($sqlMedFisica);

        $sqlMedAcumulada = "WITH v as (
                          SELECT
                            c.crtvalorexecucao AS valorcontrato,
                            c.crtid as crtid,
                            NULL as cceid
                          FROM obras2.obrascontrato ocr INNER JOIN obras2.contrato c ON (ocr.crtid = c.crtid) OR (ocr.crtid = c.crtidpai)
                            INNER JOIN entidade.entidade ent ON ent.entid = c.entidempresa
                          WHERE ocr.ocrstatus = 'A' AND c.crtstatus = 'A' AND ent.entstatus = 'A' AND c.ttaid IS NULL AND ocr.obrid IN ({$strObrids})
                          GROUP BY c.crtid, valorcontrato
                        
                          UNION ALL
                        
                          SELECT
                            cce.ccevalor AS valorcontrato,
                            NULL as crtid,
                            cce.cceid as cceid
                          FROM obras2.construtoraextra cex
                            INNER JOIN obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
                          WHERE cex.cexstatus = 'A'
                                AND cce.ccedataassinatura IS NOT NULL
                                AND cce.ttaid IS NULL
                                AND
                                cex.obrid IN ({$strObrids})
                          GROUP BY cce.cceid, valorcontrato
                        ),
                        x AS (SELECT
                          sum(ntm.ntmvlrpago) AS vlrpago,
                          1 as codigo
                        FROM v
                        LEFT JOIN
                          obras2.notamedicao ntm ON v.cceid = ntm.cceid OR v.crtid = ntm.crtid
                        LEFT JOIN
                          obras2.notafiscal ntf ON ntm.ntfid = ntf.ntfid
                        WHERE
                          ntf.ntfdtnota <= '{$dataNota}'
                        ),
                        y as (
                            SELECT
                             sum(v.valorcontrato) as vlrcontrato,
                             1 as codigo
                            FROM v
                          )
                        SELECT
                          (x.vlrpago*100)/y.vlrcontrato AS percentAcumulada
                        FROM
                          x
                        INNER JOIN y ON x.codigo = y.codigo;
    ";

        $medicaoAcumulada = $db->pegaUm($sqlMedAcumulada);


        if (!empty($medicaoFisica) && !empty($medicaoAcumulada)) {
            $resposta = array("medicaoFisica" => number_format($medicaoFisica, 2, ",", ".") . "%",
                "medicaoAcumulada" => number_format($medicaoAcumulada, 2, ",", ".") . "%");
        } else {
            $resposta = array("medicaoFisica" => "Não foi possível calcular a % da Medição Física",
                "medicaoAcumulada" => "Não foi possível calcular a % da Medição Acumulada");

        }

        return $resposta;
    }

    /**
     * @todo Comentar depois.
     * @param $ntfid
     * @param null $medid
     * @return array|mixed|NULL
     */
    function getValoresPagosNotaMedicao($ntfid, $mecid = null, $ignoraAditivo = false)
    {

        global $db;

        $ntfid = empty($ntfid)?0:$ntfid;
        
        $filtroMedicao = ctype_digit($mecid) ? "AND ntm.mecid = {$mecid}" : null;
        $filtroAditivo = "
            INNER JOIN obras2.medicaocontrato mec ON mec.mecid = ntm.mecid AND mec.mecstatus = 'A'
            INNER JOIN obras2.
        ";

        $sql = "
            SELECT
              ntm.ntmvlrpago
            FROM obras2.notamedicao ntm
            WHERE
              ntm.ntmstatus = 'A'
            AND
              ntm.ntfid = {$ntfid}
            {$filtroMedicao};
        ";

        $retorno = array();
        $arrResult = $db->carregar($sql);
        if (is_array($arrResult) && !empty($arrResult)) {
            foreach ($arrResult as $r) {
                $retorno[] = (float)$r["ntmvlrpago"];
            }
        }

        return $retorno;

    }

    /**
     * @todo Comentar depois.
     * @param $mecid
     * @return bool|mixed|NULL|string
     */
    function getDadosMedicao($mecid)
    {

        global $db;

        $arrDadosMedicao = array();

        if ($mecid > 0) {
            $sql = "
                SELECT
                  med.medid,
                  med.mednummedicao,
                  to_char(med.meddtmedicao, 'DD/MM/YYYY') AS meddtmedicao,
                  to_char(med.meddtinicioexec, 'DD/MM/YYYY') AS meddtinicioexec,
                  to_char(med.meddtfimexec, 'DD/MM/YYYY') AS meddtfimexec,
                  med.medvlrmedicao
                FROM
                  obras2.medicoes med
                  INNER JOIN
                  obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
                WHERE
                  med.medstatus = 'A'
                  AND
                  mec.mecid = {$mecid};
            ";

            $result = $db->carregar($sql);
            if (is_array($result) && !empty($result)) {
                $arrDadosMedicao = current($result);
            }
        }

        return $arrDadosMedicao;

    }

    /**
     * @todo Comentar depois.
     * @param $crtid
     * @param $cceid
     * @return bool|int|mixed|NULL|string
     */
    function getValorContratoPrincipal($obrid)
    {

        global $db;

        if ($obrid) {
            $arrObras = $this->retornaObrids($obrid);
            if (is_array($arrObras) && !empty($arrObras)) {
                $strObrids = implode(",", $arrObras);
            }
        }

        $sql = "
            SELECT
              --crt.crtid,
              coalesce(ocr.ocrvalorexecucao, 0)
            FROM obras2.obras obr
              INNER JOIN obras2.obrascontrato ocr ON obr.obrid = ocr.obrid AND ocr.ocrstatus = 'A'
              INNER JOIN obras2.contrato crt ON ((ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)) AND crt.crtstatus = 'A'
            WHERE obr.obrstatus = 'A'
                  AND obr.obrid IN ({$strObrids})
                  AND crt.ttaid ISNULL;
            ";

        $valorContratoPrincipal = $db->pegaUm($sql);
        if (!$valorContratoPrincipal) {
            $valorContratoPrincipal = 0;
        }

        return $valorContratoPrincipal;
    }

    /**
     * @todo Comentar depois.
     * @param $origemContrato
     * @param $idContrato
     * @return bool|mixed|NULL|string
     */
    function getTipoContrato($origemContrato, $idContrato)
    {

        global $db;

        if ($origemContrato !== "cex" && $origemContrato !== "crt") {
            return false;
        } else {
            if ($origemContrato === "crt") {
                $sql = "
                    SELECT CASE WHEN contratoinfo.ttaid ISNULL
                      THEN 'original'
                           ELSE 'aditivo' END tipocontrato
                    FROM (
                           SELECT c.ttaid
                           FROM obras2.contrato c
                           WHERE c.crtid = {$idContrato}
                         ) contratoinfo;
                ";
            } else {
                $sql = "
                    SELECT CASE WHEN (contratoinfo.cceid_pai ISNULL AND contratoinfo.ttaid ISNULL)
                      THEN 'original'
                           ELSE 'aditivo' END tipocontrato
                    FROM (
                           SELECT
                             cce.cceid_pai,
                             cce.ttaid
                           FROM obras2.contratoconstrutoraextra cce
                           WHERE cce.cceid = {$idContrato}
                         ) contratoinfo;
                ";
            }

            return $db->pegaUm($sql);
        }

    }

    /**
     * @todo Comentar depois.
     * @param $obrid
     * @param $valorPagoNota
     * @param $valorContratoOriginal
     * @return float|int
     */
    function calcularPercentualMedicao($valorPagoMedicao, $valorContrato)
    {

        $valorPagoMedicao = (float)$valorPagoMedicao;
        $valorContrato = (float)$valorContrato;

        $percentualMedicao = 0;

        if ($valorContrato > 0) {
            $percentualMedicao = ($valorPagoMedicao * 100) / $valorContrato;
        }

        return $percentualMedicao;

    }

    /**
     * @todo Comentar depois.
     * @param $ntfid
     * @return int
     */
    function verificaNotaFiscalComPendencia($ntfid)
    {

        if (ctype_digit($ntfid)) {
            global $db;

            $sql = "
                WITH dadosnotafiscal AS
                (
                    SELECT
                      CASE
                      WHEN ntf.entid NOTNULL AND ntf.cexid ISNULL
                        THEN ntf.entid
                      WHEN ntf.entid ISNULL AND ntf.cexid NOTNULL
                        THEN ntf.cexid
                      END idfornecedor,
                      CASE
                      WHEN ntf.crtid NOTNULL AND ntf.cceid ISNULL
                        THEN ntf.crtid
                      WHEN ntf.crtid ISNULL AND ntf.cceid NOTNULL
                        THEN ntf.cceid
                      END idcontrato,
                      ntf.arqid,
                      ntf.ntfnumnota,
                      ntf.ntfdtnota,
                      ntf.ntfvalornota,
                      ntm.mecid,
                      ntm.ntmvlrpago,
                      med.mednummedicao
                    FROM
                      obras2.notafiscal ntf
                      INNER JOIN obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
                      INNER JOIN obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
                      INNER JOIN obras2.medicoes med ON mec.medid = med.medid AND med.medstatus = 'A'
                    WHERE ntf.ntfstatus = 'A'
                          AND ntf.ntfid = {$ntfid}
                )
                SELECT count(*)
                FROM dadosnotafiscal
                WHERE NOT (dadosnotafiscal NOTNULL);
            ";

            return (int)$db->pegaUm($sql);

        }

    }

    function getValoresPagosNotas($obrid, $dataNotaFiscal = null)
    {

        $valoresPagos = 0;
        $filtroData = null;

        $arrData = explode("-", $dataNotaFiscal);
        if (checkdate($arrData[1], $arrData[2], $arrData[0])) {
            $filtroData = "AND ntf.ntfdtnota <= '{$dataNotaFiscal}'";
        }

        if ($obrid) {

            $strObrid = $this->retornaObrids($obrid);
            $strObrid = implode(",", $strObrid);

            global $db;

            $sql = "
                SELECT COALESCE(sum(ntm.ntmvlrpago), 0) totalpago
                      FROM obras2.medicaocontrato mec
                        INNER JOIN obras2.medicoes med ON mec.medid = med.medid
                        INNER JOIN obras2.notamedicao ntm ON mec.mecid = ntm.mecid AND ntm.ntmstatus = 'A'
                        INNER JOIN obras2.notafiscal ntf ON ntm.ntfid = ntf.ntfid AND ntf.ntfstatus = 'A'
                        INNER JOIN obras2.obrascontrato ocr ON med.obrid = ocr.obrid AND ocr.ocrstatus = 'A'
                        INNER JOIN obras2.contrato crt ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                      WHERE mec.mecstatus = 'A'
                        AND crt.crtstatus = 'A'
                        AND med.obrid IN ({$strObrid})
                        {$filtroData}
                        AND crt.ttaid ISNULL
            ";

            $valoresPagos = $db->pegaUm($sql);

        }

        return $valoresPagos;

    }

    function getValorReferenciaMedicaoAcumulada($obrid, $dataNotaFiscal)
    {

        $valorReferencia = 0;

        if ($obrid) {

            $strObrid = $this->retornaObrids($obrid);
            $strObrid = implode(",", $strObrid);

            global $db;

            $sql = "
                WITH
                    qtdobrasenvolvidas AS (k
                      SELECT count(obr.obrid) AS qtdobrasenvolvidas
                      FROM obras2.obras obr
                      WHERE obr.obrid IN ({$strObrid})
                  ),
                    obraprincipal AS (
                      SELECT CASE WHEN qoe.qtdobrasenvolvidas > 1
                        THEN (
                          SELECT obr.obrid
                          FROM
                            obras2.obras obr
                          WHERE
                            obr.obridvinculado NOTNULL
                            AND obr.obrstatus = 'A'
                            AND obr.obrid IN ({$strObrid})
                            AND obr.obridpai ISNULL
                          ORDER BY obr.obrid ASC
                          LIMIT 1
                        )
                             ELSE ({$obrid})
                             END AS obrid
                      FROM qtdobrasenvolvidas qoe
                
                  ),
                
                    contratoobraprincipal AS (
                      SELECT
                        crt.crtid,
                        COALESCE(crt.crtvalorexecucao, 0) crtvalorexecucao
                      FROM obras2.contrato crt
                        INNER JOIN obras2.obrascontrato ocr ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                        INNER JOIN obraprincipal op ON op.obrid = ocr.obrid
                        INNER JOIN entidade.entidade ent ON ent.entid = crt.entidempresa
                      WHERE crt.crtstatus = 'A'
                            AND ocr.ocrstatus = 'A'
                            AND ent.entstatus = 'A'
                            AND crt.ttaid ISNULL
                            AND ocr.obrid = op.obrid
                  ),
                
                    notaspagasobrasvinculadas AS (
                      SELECT COALESCE(sum(ntm.ntmvlrpago), 0) totalpago
                      FROM obras2.medicaocontrato mec
                        INNER JOIN obras2.medicoes med ON mec.medid = med.medid
                        INNER JOIN obras2.notamedicao ntm ON mec.mecid = ntm.mecid AND ntm.ntmstatus = 'A'
                        INNER JOIN obras2.notafiscal ntf ON ntm.ntfid = ntf.ntfid AND ntf.ntfstatus = 'A'
                        LEFT JOIN contratoobraprincipal cop ON cop.crtid = mec.crtid
                        INNER JOIN obras2.obrascontrato ocr ON med.obrid = ocr.obrid AND ocr.ocrstatus = 'A'
                        INNER JOIN obras2.contrato crt ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                      WHERE mec.mecstatus = 'A'
                        AND crt.crtstatus = 'A'
                        AND med.obrid IN ({$strObrid})
                        AND med.obrid NOT IN (cop.crtid)
                        AND ntf.ntfdtnota <= '{$dataNotaFiscal}'
                        AND crt.ttaid ISNULL
                  )
                SELECT CASE WHEN qoe.qtdobrasenvolvidas > 1
                  THEN (
                    SELECT npo.totalpago FROM notaspagasobrasvinculadas npo
                  )
                       ELSE (SELECT cop.crtvalorexecucao
                             FROM contratoobraprincipal cop)
                       END
                FROM qtdobrasenvolvidas qoe;
            ";

            $valorReferencia = $db->pegaUm($sql);

        }

        return $valorReferencia;

    }

    function getSomatorioTodosContratos($obrid, $tipoFornecedor)
    {

        if ($obrid) {

            $arrObras = $this->retornaObrids($obrid);

            if (is_array($arrObras) && !empty($arrObras)) {

                $strObrids = implode(",", $arrObras);

                global $db;

                if ($tipoFornecedor === "ent") {
                    $sql = "
                        SELECT coalesce(sum(ocr.ocrvalorexecucao), 0) totalvalorcontratos
                        FROM
                          obras2.obrascontrato ocr
                          INNER JOIN obras2.contrato crt ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                        WHERE
                          crt.crtstatus = 'A'
                          AND crt.ttaid ISNULL
                          AND ocr.ocrstatus = 'A'
                          AND ocr.obrid IN ({$strObrids})
                    ";

                } elseif ($tipoFornecedor === "cex") {
                    $sql = "
                        SELECT
                          coalesce(sum(cce.ccevalor), 0) totalvalorcontratos
                        FROM obras2.contratoconstrutoraextra cce
                          INNER JOIN obras2.construtoraextra cex ON cce.cexid = cex.cexid AND cex.cexstatus = 'A'
                        WHERE cce.ccestatus = 'A'
                              AND cce.cceid_pai ISNULL
                              AND cce.ttaid ISNULL
                              AND cex.obrid IN ({$strObrids})
                    ";

                }

                return $db->pegaUm($sql);

            }
        }
    }

    /***************************************/

    function getValorContratoAtual($crtid, $cceid)
    {

        global $db;

        if (!$crtid && !$cceid) {
            return false;
        }

        if (ctype_digit($crtid) && !$cceid) {
            $sql = "
            SELECT 
              c.crtvalorexecucao 
            FROM obras2.contrato c
            WHERE
              c.crtstatus = 'A'
            AND crtid = {$crtid};
            ";

        } elseif (ctype_digit($cceid) && !$crtid) {
            $sql = "
            SELECT
              cce.ccevalor
            FROM obras2.contratoconstrutoraextra cce
            WHERE cce.ccestatus = 'A'
            AND cce.cceid_pai ISNULL AND cce.ttaid ISNULL
            AND cce.cceid = {$cceid};
            ";
        }

        $valorContrato = $db->pegaUm($sql);
        if (!$valorContrato) {
            $valorContrato = 0;
        }

        return $valorContrato;
    }

    function getSomatorioValoresPagosNota($obrid, $excluirContratoPrincipal = false)
    {

        if ($obrid) {

            $arrObras = $this->retornaObrids($obrid);

            if (is_array($arrObras) && !empty($arrObras)) {

                $strObrids = implode(",", $arrObras);

                global $db;

                $sql = "
                SELECT
                  coalesce(sum(ntm.ntmvlrpago), 0)
                FROM obras2.notafiscal ntf
                INNER JOIN (
                    SELECT crt.crtid
                    FROM obras2.obrascontrato ocr
                      INNER JOIN obras2.contrato crt ON (ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)
                      INNER JOIN entidade.entidade ent ON ent.entid = crt.entidempresa
                    WHERE ocr.ocrstatus = 'A'
                          AND ocr.obrid IN ({$strObrids})
                          AND crt.crtstatus = 'A'
                          AND crt.ttaid ISNULL
                          AND ent.entstatus = 'A'
                    ) x ON ntf.crtid = x.crtid
                INNER JOIN obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
                INNER JOIN obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
                INNER JOIN obras2.medicoes med ON mec.medid = med.medid AND med.medstatus = 'A'
                INNER JOIN obras2.obras obr ON med.obrid = obr.obrid AND obr.obrstatus = 'A'
                WHERE ntf.ntfstatus = 'A'
                AND mec.crtid = x.crtid
                AND med.obrid IN ({$strObrids})
                ";

                if ($excluirContratoPrincipal) {
                    $sql .= "AND x.crtid NOT IN (
                    SELECT crt.crtid
                    FROM obras2.obras obr
                        INNER JOIN obras2.obrascontrato ocr ON obr.obrid = ocr.obrid AND ocr.ocrstatus = 'A'
                        INNER JOIN obras2.contrato crt ON ((ocr.crtid = crt.crtid) OR (ocr.crtid = crt.crtidpai)) AND crt.crtstatus = 'A'
                    WHERE obr.obrstatus = 'A'
                        AND obr.obrid IN ({$strObrids})
                        AND crt.ttaid ISNULL
                    )";
                }

                return $db->pegaUm($sql);

            }
        }
    }

    public function gerarDocid($exeid, $obrid)
    {
        global $db;

        $docid = wf_cadastrarDocumento(TPDID_EXECUCAO_FINANCEIRA, "Execução Financeira Obras II - Obra: {$obrid}", ESDID_EM_CADASTRAMENTO);

        $sql = "UPDATE obras2.execucaofinanceira SET docid = {$docid} WHERE exeid = {$exeid}";

        $db->executar($sql);

        $db->commit();

        return true;
    }

    public function salvarExecucaoFinanceira($arrDados, $apenasDocId = null) :bool 
    {

        global $db;
        
        return true;

        $proid_pac_sql = "SELECT 
	               ptp.proid
                FROM obras2.obras o 
                INNER JOIN par.termoobra pto ON pto.preid = o.preid AND o.obridpai IS NULL
                INNER JOIN par.termocompromissopac ptp ON ptp.terid = pto.terid AND ptp.terstatus = 'A' 
                WHERE 
			o.obrnumprocessoconv  = '{$arrDados['exeprocesso']}'
			AND o.obridpai IS NULL
			AND o.obrstatus = 'A' ";
        $proid_pac = $this->pegaUm($proid_pac_sql);

        $proid_par_sql = "SELECT 
                        pd.proid
	               FROM obras2.obras o
                INNER JOIN par.termocomposicao pto ON pto.preid = o.preid
                INNER JOIN par.documentopar pd ON pd.dopid = pto.dopid AND pd.dopstatus = 'A'
                WHERE 
                    o.obrnumprocessoconv = '{$arrDados['exeprocesso']}'
                    AND o.obridpai IS NULL
                    AND o.obrstatus = 'A' ";
        $proid_par = $this->pegaUm($proid_par_sql);

        $proid_par3_sql = "SELECT 
                    poc.proid 
                FROM obras2.obras o
                INNER JOIN par3.obra po ON po.obrid = o.obrid_par3 AND o.obridpai IS NULL
                INNER JOIN par3.processoobracomposicao poc ON poc.obrid = po.obrid
                WHERE 
                    o.obrnumprocessoconv = '{$arrDados['exeprocesso']}' 
                    AND o.obridpai IS NULL
                    AND o.obrstatus = 'A'";
        $proid_par3 = $this->pegaUm($proid_par3_sql);

        $proid_par3 = ($proid_par3) ? $proid_par3 : 'null';
        $proid_pac = ($proid_pac) ? $proid_pac : 'null';
        $proid_par = ($proid_par) ? $proid_par : 'null';

        if (empty($arrDados['exeid'])) {
            $sql = "INSERT INTO obras2.execucaofinanceira 
                (exeprocesso, proid_pac, proid_par, proid_par3)VALUES({$arrDados['exeprocesso']}, $proid_pac, $proid_par, $proid_par3) RETURNING  exeid";

            $arrDados['exeid'] = $this->pegaUm($sql);
        }
        $exeid = $arrDados['exeid'];
        $obrid = $arrDados['obrid'];

        if ($apenasDocId == true) {
            $this->gerarDocid($exeid, $obrid);
            return true;
        }

//        $this->gerarDocid($exeid, $obrid);
        $this->salvarExecucaoFinanceiraComposicao($arrDados);

        return true;

    }

    function verificarExecucaoFinanceiraComposicao($obrid)
    {
        global $db;

        $sql = " SELECT 
                    obrid,
                    excfinalizada 
                 FROM obras2.execucaofinanceiracomposicao 
                 WHERE obrid={$obrid} AND excstatus = 'A'";

        $execucaoFinanceiraComposicao = $db->pegaLinha($sql);

        return $execucaoFinanceiraComposicao;
    }

    function salvarExecucaoFinanceiraComposicao($arrDados, $salvarGRU = true)
    {
        global $db;
        $obrid = $arrDados['obrid'];
        $exeid = $arrDados['exeid'];
        $usucpf = $arrDados['usucpf'];

        $sql = "INSERT INTO obras2.execucaofinanceiracomposicao 
                (obrid,exeid,excfinalizada, usucpf)
                VALUES
                ($obrid,$exeid,FALSE, $usucpf) RETURNING excid";

        $arrDados['excid'] = $this->pegaUm($sql);

        if ($salvarGRU) {
            $this->salvarDevolucaoGruExecucaoFinanceira($arrDados);
        }
    }

    function salvarDevolucaoGruExecucaoFinanceira($arrDados)
    {

        $devvalor = !empty($arrDados['req']['devvalor']) ? $arrDados['req']['devvalor'] : 0;
        $campos = array(
            'excid' => $arrDados['excid'],
            'devvalor' => $devvalor = str_replace(' ', '', MoedaToBd($devvalor))

        );

        $file = new FilesSimec("devolucao_gru_execucao_financeira", $campos, 'obras2');
        $file->setUpload('Devolução GRU', '', true, 'devid');
        $file->getCampoRetorno();
        $this->commit();

        return true;

    }

    public function retornarDevolucaoGruExecucaoFinanceira($obrid)
    {

        global $db;

        $sql = "SELECT
                    dev.devid,
                    dev.arqid,
                    dev.devvalor,
                    arq.arqnome,
                    arq.arqextensao
                FROM obras2.execucaofinanceira ex
                INNER JOIN obras2.execucaofinanceiracomposicao exc ON exc.exeid = ex.exeid AND exc.excstatus = 'A' 
                INNER JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                INNER JOIN public.arquivo arq ON arq.arqid = dev.arqid
                WHERE exc.obrid = {$obrid} AND ex.exestatus = 'A' ORDER BY dev.devid DESC";

        $retorno = $db->carregar($sql);

        return $retorno;

    }

    public function retornaExecucaoFinanceira($exprocesso)
    {
        global $db;
        $sql = "SELECT
                    ex.exeid,
                    exc.excid,
                    exc.obrid,
                    ex.docid,
                    ex.exeprocesso,
                    exc.excfinalizada,
                    dev.devid,
                    dev.arqid,
                    dev.devvalor
                
                FROM obras2.execucaofinanceira ex
                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.exeid = ex.exeid AND exc.excstatus = 'A' 
                LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE ex.exeprocesso = '{$exprocesso}'  AND ex.exestatus = 'A' ORDER BY dev.devid DESC";

        $retorno = $db->pegaLinha($sql);

        return $retorno;
    }

    public function downloadDevolucaoGruExecucaoFinanceira($arqid)
    {
        if (!$arqid) {
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }
        $file = new FilesSimec();
        $file->getDownloadArquivo($arqid);
    }

    public function excluirArquivoDevolucaoGruExecucaoFinanceira($arqid)
    {
        if (!$arqid) {
            echo "<script>alert('Arquivo não encontrado.')</script>";
            return;
        }

        $file = new FilesSimec('devolucao_gru_execucao_financeira', NULL, 'obras2');
        $file->excluiArquivoFisico($arqid);
        $sql = "DELETE FROM obras2.devolucao_gru_execucao_financeira  WHERE arqid = {$arqid} ";

        $this->executar($sql);
        $this->commit();

    }

    public function retornarValorDevolucaoGruExecucaoFinanceira($obrid)
    {
        global $db;
        $sql = "SELECT 
                  SUM(devvalor) AS somadevvalor
                FROM obras2.execucaofinanceira ex 
                INNER JOIN obras2.execucaofinanceiracomposicao exc ON exc.exeid = ex.exeid AND exc.excstatus = 'A' 
                INNER JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE exc.obrid = {$obrid} AND ex.exestatus = 'A'";

        $retorno = $db->pegaUm($sql);

        return $retorno;

    }

    /**
     * Função responsável por pesquisar as construtoras extras cadastradas para determinada obra.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param $obrid
     * @return array|mixed|NULL
     */
    function buscaConstrutorasExtra($obrid)
    {
        global $db;

        $arrObrids = $this->retornaObrids($obrid);
        $strObrids = implode(",", $arrObrids);
        $arrConstrutoraExtra = array();

        if ($strObrids != '') {
            $sql = "SELECT
              cex.cexid,
              cex.cexrazsocialconstrutora,
              cex.cexnumcnpj,
              cex.lieid,
              lie.lienumero,
              mod.moldsc,
              cce.ccedataassinatura,
              cce.cceid
            FROM 
              obras2.construtoraextra cex
            LEFT JOIN
              obras2.licitacaoextra lie ON cex.lieid = lie.lieid 
            LEFT JOIN
              obras2.modalidadelicitacao mod ON lie.molid = mod.molid
            LEFT JOIN 
              obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
            WHERE
              cexstatus = 'A'
            AND
              cex.obrid IN ({$strObrids})
            AND
              cce.ttaid IS NULL
            AND 
              cce.cceid_pai IS NULL 
            ORDER BY cce.ccedataassinatura
             ";

            $arrConstrutoraExtra = $db->carregar($sql);
        }

        if (!is_array($arrConstrutoraExtra)) {
            $arrConstrutoraExtra = array();
        }

        return $arrConstrutoraExtra;
    }

    function buscaContratoOriginalExtras($cexid)
    {
        global $db;

        $sql = "SELECT
                  cceid,
                  ccenumero,
                  ccedataassinatura,
                  ccevalor,
                  arqid,
                  cceid_pai,
                  ttaid
                FROM obras2.contratoconstrutoraextra WHERE cexid = {$cexid} 
                AND ttaid ISNULL AND cceid_pai ISNULL AND ccestatus = 'A'";

        $arrContratoExtra = $db->pegaLinha($sql);

        return $arrContratoExtra;
    }

    function buscaAditivoExtra($cexid, $cceid)
    {
        global $db;

        if ($cexid && $cceid) {
            $sql = "SELECT
                  cceid,
                  ccenumero,
                  ccedataassinatura,
                  ccevalor,
                  arqid,
                  cceid_pai,
                  cex.ttaid,
                  tta.ttadsc
                FROM obras2.contratoconstrutoraextra cex
                INNER JOIN obras2.tipotermoaditivo tta ON cex.ttaid = tta.ttaid
                WHERE cex.cexid = {$cexid}
                AND cex.ttaid NOTNULL AND cex.cceid_pai = {$cceid} AND cex.ccestatus = 'A'";

            $arrAditivoExtra = $db->carregar($sql);
            $arrAditivoExtra = $arrAditivoExtra ? $arrAditivoExtra : array();
        } else {
            $arrAditivoExtra = array();
        }
        return $arrAditivoExtra;
    }

    function verificarExecucaoFinanceira($obrid)
    {
        global $db;
        $sql = "SELECT 
                  COUNT(*) 
                FROM obras2.execucaofinanceira ex 
                INNER JOIN obras2.execucaofinanceiracomposicao exc ON exc.exeid = ex.exeid AND exc.excstatus = 'A' 
                INNER JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE exc.obrid = {$obrid} AND ex.exestatus = 'A'";

        $retorno = $db->pegaUm($sql);

        return $retorno;

    }

    function finalizarDevolucaoGru($obrid)
    {
        global $db;
        $sql = "UPDATE obras2.execucaofinanceiracomposicao SET excfinalizada = TRUE WHERE obrid = {$obrid} ";
        $db->executar($sql);
        $db->commit();
    }

    function retornarProcesso($obrid) :int 
    {

        global $db;
        /*
        TODO 
        refatorar par.termoobra para o par3.
        */
        return 0;

        $sql = "
            SELECT 
			    obrnumprocessoconv
		    FROM
		    (   
                (SELECT 
                    o.obrnumprocessoconv
                FROM obras2.obras o 
                INNER JOIN par.termoobra pto ON pto.preid = o.preid AND o.obridpai IS NULL
                INNER JOIN par.termocompromissopac ptp ON ptp.terid = pto.terid AND ptp.terstatus = 'A' 
   
                WHERE 
                    pto.terid IN (
                                    SELECT 
                                        terid 
                                    FROM  par.termoobra pto 
                                    WHERE 
                                        preid = (
                                                    SELECT 
                                                        preid 
                                                    FROM  obras2.obras 
                                                    WHERE obrid = {$obrid})
                                                    ) 
                    AND o.obridpai IS NULL
                GROUP BY 
                o.obrnumprocessoconv
                )UNION ALL(
                SELECT 
                	pp.pronumeroprocesso                   
                FROM obras2.obras o
                INNER JOIN par3.obra po ON po.obrid = o.obrid_par3 AND o.obridpai IS NULL
                INNER JOIN par3.processoobracomposicao poc ON poc.obrid = po.obrid
                INNER JOIN par3.processo pp ON pp.proid = poc.proid

                WHERE 
                    o.obrid = {$obrid}
                    AND o.obridpai IS NULL
                )UNION ALL(
                SELECT
                pd.dcoprocesso
                FROM obras2.obras o
                INNER JOIN painel.dadosconvenios pd ON pd.dcoprocesso = o.obrnumprocessoconv AND o.obridpai IS NULL
                
                WHERE 
                    o.obrid = {$obrid} 
                    AND o.obridpai IS NULL
                )UNION ALL(
                SELECT 
                o.obrnumprocessoconv
                FROM obras2.obras o
                INNER JOIN par.termocomposicao pto ON pto.preid = o.preid
                INNER JOIN par.documentopar pd ON pd.dopid = pto.dopid AND pd.dopstatus = 'A'

                WHERE 
                    o.obrid = {$obrid} 
                    AND o.obridpai IS NULL
                )UNION ALL(
                   SELECT 
                      o.obrnumprocessoconv
                    FROM obras2.obras o
                    WHERE
                      o.obrid  = '{$obrid}'
	                    AND o.obridpai IS NULL
                )) AS foo GROUP BY obrnumprocessoconv";

        $processo = $db->pegaUm($sql);
        return $processo;

    }

    function listarObrasFinalizar($obrid, $processo)
    {
        global $db;

        return <<<SQL
        SELECT  
              obrid,
              obrnome,
              esddsc,
              excfinalizada
           FROM((
                SELECT 
                    o.obrid,
                    '<a href="obras2.php?modulo=principal/cadObra&acao=A&obrid=' || o.obrid || '&empid=' || o.empid || ' ">' || o.obrnome || '</a>' AS obrnome,
                    wfe.esddsc,
                    CASE
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND exc.excfinalizada = TRUE THEN '<img src="/imagens/valida1.gif" title="Execução financeira concluída"  >'
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND (exc.excfinalizada = FALSE OR exc.excfinalizada IS NULL ) THEN  '<img src="/imagens/valida5.gif" title="É preciso finalizar a Execução Financeira"  >'  
                      ELSE  '<img src="/imagens/valida3.gif" title="Obra não está concluída "  >'
                    END AS excfinalizada  
                                            
                FROM obras2.obras o 
                INNER JOIN par.termoobra pto ON pto.preid = o.preid AND o.obridpai IS NULL
                INNER JOIN par.termocompromissopac ptp ON ptp.terid = pto.terid AND ptp.terstatus = 'A' 
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE 
                    o.obrnumprocessoconv  = '{$processo}'
                    AND o.obridpai IS NULL
                    AND o.obrstatus = 'A'
                GROUP BY 
                    o.obrid,
                    o.obrnome,
                    wfe.esddsc,
                    wfe.esdid,
                    excfinalizada
                )UNION ALL(
                SELECT 
                    o.obrid,
                    '<a href="obras2.php?modulo=principal/cadObra&acao=A&obrid=' || o.obrid || '&empid=' || o.empid || ' ">' || o.obrnome || '</a>' AS obrnome,
                    wfe.esddsc,
                    CASE
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND exc.excfinalizada = TRUE THEN '<img src="/imagens/valida1.gif" title="Execução financeira concluída"  >'
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND (exc.excfinalizada = FALSE OR exc.excfinalizada IS NULL ) THEN  '<img src="/imagens/valida5.gif" title="É preciso finalizar a Execução Financeira"  >'  
                      ELSE  '<img src="/imagens/valida3.gif" title="Obra não está concluída "  >'
                    END AS excfinalizada
                    
                FROM obras2.obras o
                INNER JOIN par3.obra po ON po.obrid = o.obrid_par3 AND o.obridpai IS NULL
                INNER JOIN par3.processoobracomposicao poc ON poc.obrid = po.obrid
                INNER JOIN par3.processo pp ON pp.proid = poc.proid
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE 
                    o.obrid = {$obrid}
                    AND o.obridpai IS NULL
                    AND o.obrstatus = 'A'
                )UNION ALL(
                SELECT
                    o.obrid,
                    '<a href="obras2.php?modulo=principal/cadObra&acao=A&obrid=' || o.obrid || '&empid=' || o.empid || ' ">' || o.obrnome || '</a>' AS obrnome,
                    wfe.esddsc,
                    CASE
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND exc.excfinalizada = TRUE THEN '<img src="/imagens/valida1.gif" title="Execução financeira concluída"  >'
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND (exc.excfinalizada = FALSE OR exc.excfinalizada IS NULL ) THEN  '<img src="/imagens/valida5.gif" title="É preciso finalizar a Execução Financeira"  >'  
                      ELSE  '<img src="/imagens/valida3.gif" title="Obra não está concluída "  >'
                    END AS excfinalizada  
                FROM obras2.obras o
                INNER JOIN painel.dadosconvenios pd ON pd.dcoprocesso = o.obrnumprocessoconv AND o.obridpai IS NULL
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE 
                    o.obrnumprocessoconv  = '{$processo}'
                    AND o.obridpai IS NULL
                    AND o.obrstatus = 'A'
                )UNION ALL(
                SELECT 
                    o.obrid,
                    '<a href="obras2.php?modulo=principal/cadObra&acao=A&obrid=' || o.obrid || '&empid=' || o.empid || ' ">' || o.obrnome || '</a>' AS obrnome,
                    wfe.esddsc,
                    CASE
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND exc.excfinalizada = TRUE THEN '<img src="/imagens/valida1.gif" title="Execução financeira concluída"  >'
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND (exc.excfinalizada = FALSE OR exc.excfinalizada IS NULL ) THEN  '<img src="/imagens/valida5.gif" title="É preciso finalizar a Execução Financeira"  >'  
                      ELSE  '<img src="/imagens/valida3.gif" title="Obra não está concluída "  >'
                    END AS excfinalizada  
                FROM obras2.obras o
                INNER JOIN par.termocomposicao pto ON pto.preid = o.preid
                INNER JOIN par.documentopar pd ON pd.dopid = pto.dopid AND pd.dopstatus = 'A'
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE 
                    o.obrnumprocessoconv  = '{$processo}'
                    AND o.obridpai IS NULL
                    AND o.obrstatus = 'A'
                )
                UNION ALL(
                SELECT 
                    o.obrid,
                    '<a href="obras2.php?modulo=principal/cadObra&acao=A&obrid=' || o.obrid || '&empid=' || o.empid || ' ">' || o.obrnome || '</a>' AS obrnome,
                    wfe.esddsc,
                    CASE
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND exc.excfinalizada = TRUE THEN '<img src="/imagens/valida1.gif" title="Execução financeira concluída"  >'
                      WHEN (wfe.esdid = 693 OR wfe.esdid = 769 OR wfe.esdid = 1084) AND (exc.excfinalizada = FALSE OR exc.excfinalizada IS NULL ) THEN  '<img src="/imagens/valida5.gif" title="É preciso finalizar a Execução Financeira"  >'                      
                      ELSE  '<img src="/imagens/valida3.gif" title="Obra não está concluída "  >'
                    END AS excfinalizada  
                    
                FROM obras2.obras o
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE 
                    o.obrnumprocessoconv  = '{$processo}'
                AND o.obridpai IS NULL
                AND o.obrstatus = 'A'
                GROUP BY 
                    o.obrid,
                    o.obrnome,
                    wfe.esddsc,
                    wfe.esdid,
                    excfinalizada
                  
                ))  AS foo  
                GROUP  BY
                  obrid,
                  obrnome,
                  esddsc,
                  excfinalizada
               ORDER BY excfinalizada
SQL;
 
    }

    function verificarQuantidadeObrasProcesso($obrid, $processo)
    {

        global $db;

        $sql = $this->listarObrasFinalizar($obrid, $processo);

        $result = $db->carregar($sql);

        $quantidade = count($result);

        return $quantidade;
    }


    function existeContratoExtra($obrid)
    {
        global $db;

        $sql = "SELECT
                  cce.cceid
                FROM
                  obras2.construtoraextra cex
                INNER JOIN obras2.contratoconstrutoraextra cce ON cex.cexid = cce.cexid AND cce.ccestatus = 'A'
                WHERE cex.cexstatus = 'A' AND cex.obrid = {$obrid}";

        $retorno = $db->pegaUm($sql);

        return $retorno ? true : false;
    }

    function existeLicitacaoExtra($obrid)
    {
        global $db;

        $sql = "SELECT lieid FROM obras2.licitacaoextra WHERE obrid = {$obrid} AND liestatus = 'A'";

        $retorno = $db->pegaUm($sql);

        return $retorno ? true : false;
    }

    function verificarExecucaoFinanceiraFinalizada($obrid)
    {
        global $db;
        $sql = "SELECT 
                  COUNT(*) 
                FROM obras2.execucaofinanceira ex 
                INNER JOIN obras2.execucaofinanceiracomposicao exc ON exc.exeid = ex.exeid AND exc.excstatus = 'A' 
                WHERE exc.obrid = {$obrid} AND exc.excfinalizada = TRUE AND ex.exestatus = 'A'";

        $retorno = $db->pegaUm($sql);

        return $retorno;

    }

    public function pegaContratoOriginal($obrid)
    {
        $sql = "SELECT c.crtid 
                FROM obras2.obrascontrato ocr INNER JOIN obras2.contrato c ON (ocr.crtid = c.crtid) OR (ocr.crtid = c.crtidpai)
                WHERE ocr.ocrstatus = 'A' AND c.crtstatus = 'A' AND c.ttaid IS NULL AND ocr.obrid = {$obrid}";

        return $this->pegaUm($sql);
    }


    public function pegaDadosAditivoExtra($crtid)
    {

        $retorno = array();
        if ($crtid) {
            $sql = "SELECT
              crtid,
              ttaid,
              crtdenominacao,
              crtdtassinatura,
              crtvalorexecucao,
              crtjustificativa,
              arqid
            FROM obras2.contrato WHERE crtidpai_execfinceira = {$crtid} AND crtstatus = 'A'";

            $retorno = $this->carregar($sql);
            $retorno = $retorno ? $retorno : array();
        }
        return $retorno;
    }

    public function retornaMedicaoPorObra($obrid)
    {
        $sql = "SELECT 
                  medid 
                FROM obras2.medicoes 
                WHERE 
                  obrid = {$obrid} AND 
                  medstatus = 'A'";
        $result = $db->carregar($sql);
        return $result;
    }

    function pendenciaContratacao($obrid)
    {
        $pendencia = 0;
        $construtora = $this->buscaConstrutorasExtra($obrid);
        foreach ($construtora as $res) {
            if ($res['ccedataassinatura'] == '') {
                $pendencia++;
            }
        }

        return $pendencia;
    }

    function pendenciaMedicao($obrid)
    {
        global $db;

        $pendencia = 0;
        $mednummedicao = array();

        $sql = "SELECT 
                  medid,
                  mednummedicao,
                  meddtmedicao,
                  meddtinicioexec,
                  meddtfimexec,
                  medvlrmedicao
                FROM obras2.medicoes 
                WHERE 
                  obrid = {$obrid} AND 
                  medstatus = 'A' ";

        $result = $db->carregar($sql);

        if ($result) {
            foreach ($result as $res) {
                array_push($mednummedicao, $res['medid']);
                if (empty($res['mednummedicao']) ||
                    empty($res['meddtmedicao']) ||
                    empty($res['meddtinicioexec']) ||
                    empty($res['meddtfimexec']) ||
                    empty($res['medvlrmedicao'])) {
                    $pendencia++;
                }
            }

            $medid = implode(',', $mednummedicao);

            $sql = "SELECT
                      crtid,
                      mecvlrreferencia
                    FROM obras2.medicaocontrato
                    WHERE
                      medid  IN ($medid) AND
                      mecstatus = 'A' AND cceid ISNULL";

            $result = $db->carregar($sql);

            if ($result) {
                foreach ($result as $res) {
                    if (empty($res['crtid']) || empty($res['mecvlrreferencia'])) {
                        $pendencia++;
                    }
                }
            }
        }

        return $pendencia;
    }

    public function pegaNotaFiscalPorObra($obrid)
    {
        global $db;

        $sql = "SELECT 
                    ntf.ntfid
                FROM
                    obras2.notafiscal ntf
                INNER JOIN obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
                INNER JOIN obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
                INNER JOIN obras2.medicoes med ON med.medid = mec.medid AND med.medstatus = 'A' 
                WHERE med.obrid = {$obrid}";
        $result = $db->carregar($sql);

        return $result;
    }

    public function pendenciaNotaFiscal($obrid)
    {
        $pendencia = 0;

        $result = $this->pegaNotaFiscalPorObra($obrid);

        $notas = count($result);

        for ($i = 0; $i < $notas; $i++) {
            $pendenciaNota = $this->verificaNotaFiscalComPendencia($result[$i]['ntfid']);

            if ($pendenciaNota > 0) {
                $pendencia++;
            };
        }

        return $pendencia;
    }

    public function pendenciaPagamento($obrid)
    {
        global $db;

        $pendencia = 0;
        $obras = $this->retornaObrids($obrid);

        $obras = implode(',', $obras);

        $sql = "SELECT 
                    pgt.tptid
                FROM obras2.pagamentotransacao pgt
                LEFT JOIN obras2.tipopagamentotransacao  tpt ON tpt.tptid = pgt.tptid
                WHERE
                    pgtstatus = 'A' AND
                    pgt.obrid IN ({$obras})";

        $result = $db->carregar($sql);

        if ($result) {
            foreach ($result as $res) {
                if (empty($res['tptid'])) {
                    $pendencia++;
                }
            }
        }

        return $pendencia;
    }

    public function verificaPendenciasExecucaoFinanceira($obrid)
    {
        global $db;
        $retorno = true;
        $pendenciaContratacao = $this->pendenciaContratacao($obrid);
        $pendenciaMedicao = $this->pendenciaMedicao($obrid);
        $pendenciaNotaFiscal = $this->pendenciaNotaFiscal($obrid);
        $pendenciaPagamento = $this->pendenciaPagamento($obrid);

        if ($pendenciaContratacao || $pendenciaMedicao || $pendenciaNotaFiscal || $pendenciaPagamento) {
            $retorno = false;
        }

        return $retorno;
    }


    /*
 *  Retorna array para listagem principal da tela
 */
    function getListaPrincipal($obrid, $dadosConstrutora, $tipo = 'padrao')
    {
        global $db;

        $execucaoFinanceira = new ExecucaoFinanceira();
        $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
        $strObrids = implode(",", $arrObrids);

        if ($tipo == 'padrao') {

            $crtId = $dadosConstrutora['crtid'];

            $sql = "
			SELECT array_to_string( array(
				SELECT
					crt.crtid
				FROM
					obras2.contrato crt
				WHERE
				(
					crt.crtid  = {$crtId}
					or
					(crt.crtidpai = (select crtidpai from obras2.contrato where crtid = {$crtId} order by crtid limit 1) AND ttaid in (2,3))
				)
			), ', ')
		";
            $resultadoIds = $db->pegaUm($sql);

            if ($resultadoIds != '') {
                $strWhereContratos = "AND pgt.crtid in ({$resultadoIds})";
            } else {
                $strWhereContratos = '';
            }


        } else {
            if ($tipo == 'extra') {
                $cceId = $dadosConstrutora['cceid'];

                if (!empty($cceId)) {
                    $sql = "
            SELECT array_to_string( array(
                SELECT
                    cce.cceid
                FROM
                    obras2.contratoconstrutoraextra cce
                WHERE
                    ccestatus = 'A'
                AND
                (
                    cce.cceid  = {$cceId}
                    or
                    (cce.cceid_pai = {$cceId} AND ttaid in (2,3))
                )
            ), ', ')
            ";
                    $resultadoIds = $db->pegaUm($sql);

                    if ($resultadoIds != '') {
                        $strWhereContratos = "AND pgt.cceid in ({$resultadoIds})";
                    } else {
                        $strWhereContratos = '';
                    }
                } else {
                    $strWhereContratos = '';
                }
            }
        }

        $sql = "
		SELECT
			--ID do pagamento
			pgtid as id_pagamento,
			-- id do arquivo
			arqid as id_arquivo,
			-- número da transação (pgt)
			pgtnumtransacao as numero_transacao,
			-- data da transação (pgt)
			to_char(pgtdtpagamento, 'DD/MM/YYYY') as data_pagamento,
			-- tipo de pagamento (pgt)
			tpt.tptdesc as tipo_de_pagamento,
			-- valor da transação ( pgt)
			pgtvalortransacao as valor_pagamento,
			-- favorecido ( pgt) (cnpj)
			CASE WHEN entid IS NOT NULL THEN
		
			(
				SELECT substr(e1.entnumcpfcnpj, 1, 2) || '.' || substr(e1.entnumcpfcnpj, 3, 3) || '.' || substr(e1.entnumcpfcnpj, 6, 3) || '/' || substr(e1.entnumcpfcnpj, 9, 4) ||
				'-' || substr(e1.entnumcpfcnpj, 13) AS cnpj
				FROM
				entidade.entidade e1 where e1.entid = pgt.entid
			)
		
			WHEN cexid IS NOT NULL THEN
			(
				SELECT substr(c1.cexnumcnpj, 1, 2) || '.' || substr(c1.cexnumcnpj, 3, 3) || '.' || substr(c1.cexnumcnpj, 6, 3) || '/' || substr(c1.cexnumcnpj, 9, 4) ||
				'-' || substr(c1.cexnumcnpj, 13) AS cnpj
				FROM
				obras2.construtoraextra c1 where c1.cexid = pgt.cexid
			)
			END as favorecido_pagamento,
			-- notas fiscais ( arr to string)
			array_to_string(
				array(
					select distinct '<img border=\"0\" src=\"../imagens/icone_lupa.png\" title=\"Visualizar Nota Fiscal\" onclick=\"visualizanota( \'' ||n1.ntfid|| ' \' )\">' || n1.ntfnumnota
					 from obras2.documentotransacao d1
					INNER JOIN obras2.notafiscal n1 ON n1.ntfid = d1.ntfid
					where d1.pgtid = pgt.pgtid
					AND n1.ntfstatus = 'A'
					AND d1.dotstatus = 'A'
				)
			, ', '
			) as notas_fiscais
		FROM
		obras2.pagamentotransacao pgt
		LEFT JOIN obras2.tipopagamentotransacao  tpt ON tpt.tptid = pgt.tptid
		WHERE
		pgtstatus = 'A'
		AND
		obrid IN ({$strObrids})
		{$strWhereContratos}
		ORDER BY pgtdtpagamento ASC  
	";

        $resultado = $db->carregar($sql);
        return (is_array($resultado)) ? $resultado : Array();
    }


    /**
     * todo. comentar
     * @param $obrid
     * @return array|mixed|NULL
     */
    function carregaDadosNotas($obrid, $tipoFornecedor, $idFornecedor)
    {
        global $db;

        $filtro = null;
        if ($tipoFornecedor === "ent") {
            $filtro = "WHERE a.entid = {$idFornecedor}";
        } elseif ($tipoFornecedor === "cex") {
            $filtro = "WHERE a.cexid = {$idFornecedor}";
        }

        $sql = "
    SELECT
      a.ntfid,
      a.fornecedor,
      a.ntfnumnota                       AS numnota,
      to_char(a.ntfdtnota, 'DD/MM/YYYY') AS datanota,
      a.ntfvalornota                     AS valornota,
      a.arqid,
      a.crtid,
      a.cceid,
      a.entid,
      a.cexid
    FROM
      (SELECT DISTINCT
         (ntf.ntfid),
         CASE WHEN ntf.entid NOTNULL AND ntf.cexid ISNULL
           THEN
             (SELECT (SELECT substr(entnumcpfcnpj, 1, 2) || '.' || SUBSTR(entnumcpfcnpj, 3, 3) || '.' ||
                             substr(entnumcpfcnpj, 6, 3) || '/' || SUBSTR(entnumcpfcnpj, 9, 4) || '-' ||
                             substr(entnumcpfcnpj, 13) AS cnpj
                      FROM (SELECT cast(entnumcpfcnpj AS VARCHAR) AS entnumcpfcnpj) a)
              FROM entidade.entidade
              WHERE entstatus = 'A' AND entid = ntf.entid)
         ELSE
           (SELECT (SELECT substr(cexnumcnpj, 1, 2) || '.' || SUBSTR(cexnumcnpj, 3, 3) || '.' ||
                           substr(cexnumcnpj, 6, 3) || '/' || SUBSTR(cexnumcnpj, 9, 4) || '-' ||
                           substr(cexnumcnpj, 13) AS cnpj
                    FROM (SELECT cast(cexnumcnpj AS VARCHAR) AS cexnumcnpj) a)
            FROM obras2.construtoraextra
            WHERE cexstatus = 'A' AND cexid = ntf.cexid)
         END AS fornecedor,
         ntf.ntfnumnota,
         ntf.ntfdtnota,
         ntf.ntfvalornota,
         ntf.arqid,
         ntf.crtid,
         ntf.cceid,
         ent.entid,
         cex.cexid
       FROM
         obras2.notafiscal ntf
         INNER JOIN
         obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
         INNER JOIN
         obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
         INNER JOIN
         obras2.medicoes med ON med.medid = mec.medid AND med.medstatus = 'A'
         LEFT JOIN
         entidade.entidade ent ON ent.entid = ntf.entid AND ent.entstatus = 'A'
         LEFT JOIN
         obras2.construtoraextra cex ON cex.cexid = ntf.cexid AND cex.cexstatus = 'A'
       WHERE
         ntf.ntfstatus = 'A'
         AND
         med.obrid IN ({$obrid})
      ) AS a
      {$filtro}
    ORDER BY a.ntfdtnota;";

        $resultado = $db->carregar($sql);

        if (!$resultado) {
            $resultado = array();
        }

        return $resultado;
    }


    function buscaMedicoesNota($ntfid)
    {
        global $db;

        $sql = "SELECT DISTINCT
              med.medid,
              med.mednummedicao
            FROM
              obras2.medicoes med
              INNER JOIN
              obras2.medicaocontrato mec ON med.medid = mec.medid AND mec.mecstatus = 'A'
              INNER JOIN
              obras2.notamedicao ntm ON mec.mecid = ntm.mecid AND ntm.ntmstatus = 'A'
            WHERE 
              med.medstatus = 'A'
            AND
              ntm.ntfid = {$ntfid}
            ";

        $resultado = $db->carregar($sql);

        if (!$resultado) {
            $resultado = array();
        }


        return $resultado;
    }

    function getFornecedores($obrid)
    {

//    $execucaoFinanceira = new ExecucaoFinanceira();
//    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
//    $strObrids = implode(",", $arrObrids);
        $arrFornecedores = array();

        if ($obrid) {

            global $db;

            $sql = "
        SELECT DISTINCT
          ent.entid                                     AS idfornecedor,
          'ent'                                         AS tipofornecedor,         
          (SELECT (SELECT substr(entnumcpfcnpj, 1, 2) || '.' || SUBSTR(entnumcpfcnpj, 3, 3) || '.' ||
                          substr(entnumcpfcnpj, 6, 3) || '/' || SUBSTR(entnumcpfcnpj, 9, 4) || '-' ||
                          substr(entnumcpfcnpj, 13) AS cnpj
                   FROM (SELECT cast(entnumcpfcnpj AS VARCHAR) AS entnumcpfcnpj) a)
           FROM entidade.entidade
           WHERE entstatus = 'A' AND entid = ntf.entid) AS fornecedor,
          crt.crtid                                     AS idcontratofornecedor,
          to_char(crt.crtdtassinatura, 'DD/MM/YYYY')    AS dataassinaturacontrato,
          ent.entnome                                   AS nomefornecedor
        FROM
          obras2.notafiscal ntf
          INNER JOIN
          obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
          INNER JOIN
          obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
          INNER JOIN
          obras2.medicoes med ON med.medid = mec.medid AND med.medstatus = 'A'
          INNER JOIN
          entidade.entidade ent ON ent.entid = ntf.entid AND ent.entstatus = 'A'
          INNER JOIN
          obras2.contrato crt ON ntf.crtid = crt.crtid AND crt.crtstatus = 'A'
        WHERE
          ntf.ntfstatus = 'A'
          AND
          med.obrid IN ({$obrid}) ORDER BY crt.crtid ASC        
        ";

            $retorno = $db->carregar($sql);
            if (is_array($retorno) && !empty($retorno)) {
                $arrFornecedores = $retorno;
            }
        }

        return $arrFornecedores;

    }


    /**
     * Função responsável por buscar fornecedores e notas fiscais inseridos como adicionais em uma obra
     * @param $obrid
     * @return array|mixed|NULL
     */
    function getFornecedoresExtras($obrid)
    {

        $arrFornecedores = array();

        if ($obrid) {

            global $db;

            $sql = "
        SELECT DISTINCT
          cex.cexid                                     AS idfornecedor,
          'cex'                                         AS tipofornecedor,
          (SELECT (SELECT substr(cexnumcnpj, 1, 2) || '.' || SUBSTR(cexnumcnpj, 3, 3) || '.' ||
                          substr(cexnumcnpj, 6, 3) || '/' || SUBSTR(cexnumcnpj, 9, 4) || '-' ||
                          substr(cexnumcnpj, 13) AS cnpj
                   FROM (SELECT cast(cexnumcnpj AS VARCHAR) AS cexnumcnpj) a)
           FROM obras2.construtoraextra
           WHERE cexstatus = 'A' AND cexid = ntf.cexid) AS fornecedor,
          cce.cceid                                     AS idcontratofornecedor,
          to_char(cce.ccedataassinatura, 'DD/MM/YYYY')  AS dataassinaturacontrato,
          cex.cexrazsocialconstrutora                   AS nomefornecedor
        FROM
          obras2.notafiscal ntf
          INNER JOIN
          obras2.notamedicao ntm ON ntf.ntfid = ntm.ntfid AND ntm.ntmstatus = 'A'
          INNER JOIN
          obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
          INNER JOIN
          obras2.medicoes med ON med.medid = mec.medid AND med.medstatus = 'A'
          INNER JOIN
          obras2.construtoraextra cex ON cex.cexid = ntf.cexid AND cex.cexstatus = 'A'
          INNER JOIN
          obras2.contratoconstrutoraextra cce ON ntf.cceid = cce.cceid AND cce.ccestatus = 'A'
        
        WHERE
          ntf.ntfstatus = 'A'
          AND
          med.obrid IN ({$obrid})
        ORDER BY idfornecedor ASC        
        ";

            $retorno = $db->carregar($sql);
            if (is_array($retorno) && !empty($retorno)) {
                $arrFornecedores = $retorno;
            }
        }

        return $arrFornecedores;

    }

    function getEmpid($obrid){
        if ($obrid) {
            global $db;
            $sql = "SELECT empid
                  FROM obras2.obras
               WHERE obrid = " . $obrid;

            $empid = $db->pegaUm($sql);
            return $empid;
        }
    }
}

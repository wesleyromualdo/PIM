<?php
	
class PreObraAnalise extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.preobraanalise";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "poaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'poaid' => null, 
									  	'preid' => null, 
									  	'poadataanalise' => null, 
    									'poastatus' => null,
    									'poausucpfinclusao' => null,
    									'qrpid' => null,
    									'poaindeferido' => null,
    									'poajustificativa' => null, 
    									'bopodeeditar' => null,     									
									  );
									  
	public function recuperarDadosAnaliseEngenharia($preid)
	{
		$sql = "SELECT
                            ent.entnome,
                            mun.estuf,
                            mun.mundescricao,
                            pre.prebairro,
                            pre.preid,
                            pre.ptoid,
                            pto.ptodescricao,
                            pto.ptoprojetofnde
                        FROM
                            obras.preobra pre
                        INNER JOIN par.instrumentounidade inu ON (CASE WHEN pre.muncod IS NOT NULL THEN inu.muncod = pre.muncod ELSE inu.estuf = pre.estuf END)
                        INNER JOIN par.entidade ent ON ent.entnumcpfcnpj = inu.inucnpj AND (CASE WHEN pre.muncod IS NOT NULL THEN ent.dutid = 6 ELSE ent.dutid = 9 END)
                        INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
                        INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
                        WHERE
                        pre.preid = {$preid} LIMIT 1";
		
		return $this->pegaLinha($sql);
	}
	
	public function recuperarRespostasQuestionario($qrpid)
	{
		$sql = "SELECT * FROM (
				(
				-- Perguntas vinculadas a grupos filhos de questionarios e de resposta textual
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.preid,					
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo AS pergunta,
					r.resdsc AS resposta	
				FROM
					obras.preobraanalise qq
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
			
			)UNION ALL(
				-- Perguntas vinculadas a grupos filhos de questionarios e que possuem item como resposta
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.preid,					
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo AS pergunta,
					ip.itptitulo AS resposta
					
				FROM
					obras.preobraanalise qq
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid = ip.itpid
			
			)UNION ALL(
				-- Perguntas de resposta textual filhas de itens de perguntas vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.preid,					
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
					r.resdsc AS resposta
					
				FROM
					obras.preobraanalise qq
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
				JOIN questionario.resposta r ON r.perid = p1.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
			
			)UNION ALL(
			-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.preid,					
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
					r.resdsc AS resposta
					
				FROM
					obras.preobraanalise qq
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.pergunta p ON p.grpid = gp.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.grupopergunta gp1 ON gp1.itpid = ip.itpid
				JOIN questionario.pergunta p1 ON p1.grpid = gp1.grpid
				JOIN questionario.resposta r ON r.perid = p1.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
			
			)UNION ALL(
				-- Perguntas de resposta textual filhas de grupos vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.preid,					
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo AS pergunta,
					r.resdsc AS resposta
					
				FROM
					obras.preobraanalise qq
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp1.grpid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
			
			)UNION ALL(
				-- Perguntas de resposta por itens filhas de grupos vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.preid,					
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo AS pergunta,
					ip.itptitulo AS resposta
					
				FROM
					obras.preobraanalise qq
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp1.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.resposta r ON r.perid = p.perid
								AND r.itpid = ip.itpid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NOT NULL
			
			)UNION ALL(
				-- Perguntas de resposta textual filhas de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.preid,
					
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
					r.resdsc AS resposta
					
				FROM
					obras.preobraanalise qq
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp1.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.pergunta p1 ON p1.itpid = ip.itpid
				JOIN questionario.resposta r ON r.perid = p1.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
				
			)UNION ALL(
			-- Perguntas de resposta textual filhas de grupos filhos de itens de perguntas filhas de grupos vinculadas a grupos filhos de questionarios
				SELECT
					qr.qrpid AS id,
					q.quetitulo as questionario,
					qq.preid,
					
					qr.qrpdata AS data_cadastro,
					p.perid as idpergunta,
					p.pertitulo ||' - '|| ip.itptitulo || ' - ' || p1.pertitulo AS pergunta ,
					r.resdsc AS resposta
					
				FROM
					obras.preobraanalise qq
				JOIN questionario.questionarioresposta qr USING (qrpid)
				JOIN questionario.questionario q ON q.queid = qr.queid
				JOIN questionario.grupopergunta gp ON gp.queid = q.queid
				JOIN questionario.grupopergunta gp1 ON gp1.gru_grpid = gp.grpid
				JOIN questionario.pergunta p ON p.grpid = gp1.grpid
				JOIN questionario.itempergunta ip ON ip.perid = p.perid
				JOIN questionario.grupopergunta gp2 ON gp2.itpid = ip.itpid
				JOIN questionario.pergunta p1 ON p1.grpid = gp2.grpid
				JOIN questionario.resposta r ON r.perid = p1.perid
								AND r.qrpid = qr.qrpid
								AND r.itpid IS NULL
				
			) ) as f where f.id = {$qrpid} order by f.questionario, f.idpergunta, f.pergunta";
		
		return $this->carregar($sql);
		
	}
	
	public function recuperarDadosPorPreid($preid)
	{
		$sql = "SELECT 
					* 
				FROM obras.preobraanalise 
				WHERE preid = {$preid}
				AND poastatus = 'A'";

		return $this->pegaLinha($sql);
	}
	
	public function verificaPodeEditarAnalise($preid)
	{
		$sql = "SELECT poaid, bopodeeditar FROM obras.preobraanalise WHERE preid = {$preid}";
		return $this->pegaLinha($sql);
	}
	
	public function recuperarNomeAnalistaObra($preid)
	{
		$sql = "SELECT					
					pre.preid,
					usu.usucpf, 
					usu.usunome,
					pto.ptodescricao,
					mun.estuf,
					mun.mundescricao,
					pre.prebairro,
					pre.docid,
					pre.ptoid,
					pre.pretipofundacao,
					doc.esdid,
					poa.poaindeferido,
					poa.poajustificativa,
					poa.qrpid
				FROM obras.preobra pre 
				INNER JOIN obras.preobraanalise poa ON poa.preid = pre.preid
				INNER JOIN seguranca.usuario usu ON poa.poausucpfinclusao = usu.usucpf 
				INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
				INNER JOIN territorios.municipio mun ON pre.muncod = mun.muncod	
				LEFT JOIN workflow.documento doc ON doc.docid = pre.docid			
				WHERE poa.preid = {$preid}";
		
		$arDados = $this->pegaLinha($sql);

		if($arDados['docid']){
                    
                    #Tratamento para definir o tipo de obras PAC ou PAR
                    $sqlTipoObra = "SELECT 
                                        COUNT(p.preid) 
                                    FROM obras.preobra p
                                    INNER JOIN obras.pretipoobra pt ON pt.ptoid = p.ptoid
                                    WHERE preid = {$preid} AND pt.tooid <> 1";
                    $tipoObra = $this->pegaUm($sqlTipoObra);
                    $arrSituacao = array();
                    if ($tipoObra > 0) {
                        $arrSituacao = array(WF_PAR_VALIDACAO_DILIGENCIA, WF_PAR_VALIDACAO_INDEFERIMENTO, WF_PAR_VALIDACAO_DEFERIMENTO, WF_PAR_OBRA_APROVADA);
                    }else{
                        $arrSituacao = array(WF_TIPO_VALIDACAO_DILIGENCIA, WF_TIPO_VALIDACAO_INDEFERIMENTO, WF_TIPO_VALIDACAO_DEFERIMENTO, WF_TIPO_OBRA_APROVADA);
                    }
			
                    #Data do cabecalho que deve ser a data da ultima tramitacao do parecer (diligencia, deferimento, indeferimento, aprovacao);
                    $sqlData = "select	
                                            max(hd.htddata) as data
                                    from workflow.historicodocumento hd
                                            inner join workflow.acaoestadodoc ac on
                                                    ac.aedid = hd.aedid
                                            inner join workflow.estadodocumento ed on
                                                    ed.esdid = ac.esdidorigem
                                            inner join seguranca.usuario us on
                                                    us.usucpf = hd.usucpf
                                            left join workflow.comentariodocumento cd on
                                                    cd.hstid = hd.hstid
                                    where
                                            hd.docid = {$arDados['docid']}
                                    and 
                                            ac.esdiddestino in (". implode(" , ", $arrSituacao) .")				
                                    group by us.usunome, hd.htddata
                                    order by hd.htddata desc ";
                    $data = $this->pegaUm($sqlData);
                    $arDados['data'] = $data;
                        
                    
                    #7)	Assinatura do Analista de Projetos - CGEST/FNDE: retornar o nome do analista que tramitou para a ULTIMA situacao 
                    # ?Em validacao de Diligencia? ?Validacao de Deferimento? ou para ?Validacao de Deferimento de Reformulacao? ou 
                    # para ?RMC - Validacao de Deferimento de Reformulacao MI para Convencional?.
                    $arrSituacaoUser = array();
                    if ($tipoObra > 0) {
                        $arrSituacaoUser = array(WF_PAR_VALIDACAO_DILIGENCIA, WF_PAR_VALIDACAO_DEFERIMENTO, WF_PAR_OBRA_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO, WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR);
                    }else{
                        $arrSituacaoUser = array(WF_TIPO_VALIDACAO_DILIGENCIA, WF_TIPO_VALIDACAO_DEFERIMENTO, WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO, WF_TIPO_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO_MI_PARA_CONVENCIONAL);
                    }
                    $sqlUsuNome = "select	
                                            usunome
                                    from workflow.historicodocumento hd
                                            inner join workflow.acaoestadodoc ac on
                                                    ac.aedid = hd.aedid
                                            inner join workflow.estadodocumento ed on
                                                    ed.esdid = ac.esdidorigem
                                            inner join seguranca.usuario us on
                                                    us.usucpf = hd.usucpf
                                            left join workflow.comentariodocumento cd on
                                                    cd.hstid = hd.hstid
                                    where
                                            hd.docid = {$arDados['docid']}
                                    and 
                                            ac.esdiddestino in (". implode(" , ", $arrSituacaoUser) .")				
                                    group by us.usunome, hd.htddata
                                    order by hd.htddata desc ";

                    $usunome = $this->pegaUm($sqlUsuNome);
                    if($htWorkflow['usunome']){
                            $arDados['usunome'] = $usunome;
                    } 
		}
		
		if($arDados['ptoid']){
//			Demanda 2415: Ajuste na análise técnica de engenharia, Igualado com a aba planilha orcamentaria
//			if($arDados['pretipofundacao'] == 'S' || $arDados['pretipofundacao'] == 'N'){
//				$stWhere = "and itctipofundacao = '{$arDados['pretipofundacao']}'";
//			}
			
			$sql = "select 
						sum(itc.itcquantidade*ppo.ppovalorunitario) as total
					from obras.preitenscomposicao itc
					inner join obras.preplanilhaorcamentaria ppo on ppo.itcid = itc.itcid
					where ppo.preid = {$arDados['preid']}
					and itc.itcstatus = 'A'
					and itc.ptoid = {$arDados['ptoid']}
					{$stWhere}";
//			ver($sql, d);
			$arDados['ppototal'] = $this->pegaUm($sql);
		}
		
		return $arDados;
	}
	
	public function verificaIndeferimentoQuestionario($preid)
	{
		$sql = "select 
					grp.grptitulo, 
					per.pertitulo, 
					itp.itpid,
					itp.itptitulo, 
					res.resdsc,
					case when res.itpid = 2700 then 1 
					else 0 end as indeferida,
					jus.resdsc as justificativa
				from questionario.questionario que 
				inner join questionario.grupopergunta grp 	ON que.queid = grp.queid
				inner join questionario.pergunta per 		ON per.grpid = grp.grpid
				inner join questionario.itempergunta itp 	ON itp.perid = per.perid
				inner join questionario.resposta res 		ON res.itpid = itp.itpid
				inner join obras.preobraanalise pre 		ON pre.qrpid = res.qrpid and pre.preid = {$preid}
				
				left join (select
								r.resdsc,
								r.qrpid
							from
								questionario.pergunta p
							left join
								questionario.resposta r ON r.perid = p.perid
							where
								p.itpid IN ( 2698,2700)
								) jus ON jus.qrpid = res.qrpid
								 
				where que.queid = 49 
				and res.itpid < 2701
				order by grp.grpordem, per.pertitulo, itp.itpid";
		
		return $this->carregar($sql);
	}
	
	public function verificarDataTramitacaoCoordenador($preid)
	{
            $sqlCoordenador = "
                SELECT 
                        MAX(TO_CHAR(hd.htddata, 'YYYY-MM-DD HH24:MI:SS'))
                FROM 
                        workflow.historicodocumento hd
                INNER JOIN obras.preobra pre ON pre.docid = hd.docid
                WHERE pre.preid = {$preid} 
                AND aedid IN (select aedid from workflow.acaoestadodoc  where esdidorigem in (329, 212, 627, 367, 1566, 1553))
                GROUP BY hd.htddata
                ORDER BY hd.htddata";

            $data = $this->pegaUm($sqlCoordenador);
            
            if (empty($data)) {
                $sqlCoordenador = "
                    SELECT 
                        MAX(TO_CHAR(hd.htddata, 'YYYY-MM-DD HH24:MI:SS'))
                    FROM workflow.historicodocumento hd
                    INNER JOIN obras.preobra pre ON pre.docid = hd.docid
                    WHERE pre.preid = {$preid} 
                    AND aedid IN (select aedid from workflow.acaoestadodoc  where esdiddestino in (228,337))
                    GROUP BY hd.htddata
                    ORDER BY hd.htddata";
                $data = $this->pegaUm($sqlCoordenador);
            }
            
            return $data;
	}
	
	public function recuperarConsideracoesFinais($preid)
	{
		$sql = "select 
					max(hd.hstid),
					hd.usucpf,
					cm.cmddsc
				from obras.preobra pre 
				inner join workflow.historicodocumento hd on pre.docid = hd.docid
				inner join workflow.comentariodocumento cm on hd.hstid = cm.hstid
				where pre.preid = {$preid}
				and hd.aedid = 535
				group by hd.usucpf, cm.cmddsc, hd.hstid
				order by hd.hstid desc";
		
		return $this->pegaLinha($sql);
	}
									  
}									  
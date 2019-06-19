<?php

class Par3_Model_AdesaoPrograma extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.prodesaoprograma";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "adpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'adpid'                     => null,
                                        'adpano'                    => null,
                                        'adpano_ciclo'              => null,
									  	'inuid'                     => null,
									  	'adpdataresposta'           => null,
    									'adpresposta'               => null,
                                        'usucpf'                    => null,
                                        'tapid'                     => null,
                                        'pfaid'                     => null,
                                        'docid'                     => null,
                                        'adpjustificativaresposta'  => null,
                                        'padtermopreenchido'        => null
									  );

	protected $stOrdem = null;

	public function recuperaPorInuid($inuid, $pfaid, $adpano_ciclo = null)
	{
	    $where = array("inuid = {$inuid}", "pfaid = {$pfaid}");
	    
	    if($adpano_ciclo) $where[] = "adpano_ciclo = '$adpano_ciclo'";
	    
		$sql = "SELECT *
			    FROM {$this->stNomeTabela}
			    WHERE
                    ".implode(' AND ', $where)."
			    --limit 1";

		return $this->pegaLinha($sql);
	}

	public function listaTermoAdesao( $dados )
	{		
		if( $dados['aceite'] ) $where = " and ap.adpresposta = '{$dados['aceite']}'";		
		if( $dados['estuf'] ) $where = " and iu.estuf = '{$dados['estuf']}'";
		if( $dados['mundescricao'] ) $where = " and iu.muncod = '{$dados['mundescricao']}'";
		
		$sql = "select 
					iu.muncod, 
				    iu.estuf, 
				    iu.inudescricao, 
					formata_cpf_cnpj(us.usucpf) as usucpf,
				    us.usunome, 
				    to_char(ap.adpdataresposta, 'DD/MM/YYYY') as data, 
				    case when ap.adpresposta = 'S' then 'Sim' else 'Não' end as aceite
				from par3.prodesaoprograma ap
					inner join par3.instrumentounidade iu on iu.inuid = ap.inuid
				    inner join seguranca.usuario us on us.usucpf = ap.usucpf
				where
				    iu.inustatus = 'A' $where
				order by estuf, inudescricao";
		
		$cabecalho = array('Ibge', 'UF', 'Municipio/Estado', "CPF", 'Quem', 'Quando', 'Se Aceitou');
		
		$list = new Simec_Listagem();
		$list->setCabecalho($cabecalho);
		$list->setQuery($sql);
		$list->setFormOff();
		
		$list->setFormFiltros('formulario');
		$list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
		$list->turnOnOrdenacao('estuf');
		$list->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
		
	}
	
	public function listaTermoAdesaoPnfcd( $dados, $retorno = 'render')
	{
		$where = " and ap.pfaid = 3";
		
		if( $dados['situacao'] ) $where.= " and est.esdid = '{$dados['situacao']}'";
		if( $dados['estuf'] ) $where.= " and iu.estuf = '{$dados['estuf']}'";
		if( $dados['esfera']) $where.= " and iu.itrid = {$dados['esfera']}";
		if( $dados['muncod'] ) $where.= " and iu.muncod = '{$dados['muncod']}'";
	
		$sql = "select distinct
					iu.inuid,
					iu.estuf,
					iu.inudescricao,
					est.esddsc as situacao,
					pnfcd.pppresp1arqid,
					pnfcd.pppresp1,
					pnfcd.pppresp2,
					pnfcd.pppresp2outros,
					pnfcd.pppresp3,
					pnfcd.pppresp3outros,
					pnfcd.pppresp0
				from par3.prodesaoprograma ap 
				inner join par3.proadesao_respostas_pnfcd pnfcd on ap.inuid = pnfcd.inuid 
				inner join par3.instrumentounidade iu on iu.inuid = ap.inuid 
				inner join seguranca.usuario us on us.usucpf = ap.usucpf
				inner join (select inuid, MAX(docid) as docid
                                from par3.proadesao_respostas_pnfcd pnfcd
                                    group by inuid) sub ON sub.inuid = iu.inuid
				inner join workflow.documento doc on doc.docid = sub.docid
				inner join workflow.estadodocumento est on est.esdid = doc.esdid 
				where 	iu.inustatus = 'A' $where
				order by estuf, inudescricao";
	
		if ($retorno == 'data') 
		{
			return $this->carregar($sql);
		} 
		else 
		{
			$cabecalho = array('UF', 'Municipio', 'Situação');
			
			$list = new Simec_Listagem();
			$list->setCabecalho($cabecalho);
			$list->setFormFiltros('formulario');
			$list->esconderColuna(array('inuid', 'pppresp0', 'pppresp1', 'pppresp1arqid', 'pppresp2', 'pppresp2outros', 'pppresp3', 'pppresp3outros'));
			$list->addAcao('plus', 'mostrarRespostas');
			$list->addAcao('download', 'downloadArquivo');
			$list->setAcaoComoCondicional('download', array(array('campo' => 'pppresp1arqid', 'valor' => 0, 'op' => 'diferente')));
			$list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
			$list->turnOnOrdenacao('estuf');
			$list->setQuery($sql);
			$list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
		}
	}
	
	
	public function listaAdesoesProemiAbcMais($ano)
	{
	    $sql = 'select 
    				programa,
    				estuf,
    				municipio,
    				cod_municipio,
    				esfera,
    				situacao_aceitacao_termo,
    				situacao_adesao,
    				total_5,
    				total_7,
    				total_15,
    				total_sem,
    				total_escolas
    				from (';
	    
	    if($ano == '2016'){
	        $pfaid = "pfaid = 6 AND";
    		$sql .= "
    					 (select 
    						'PROEMI' as programa,
    						inu.estuf,
    						case when itrid in (1,3) THEN
    							'-'
    						else
    							inu.muncod
    						end
    						as cod_municipio,
    						case when itrid in (1,3) THEN
    							''
    						else
    							inudescricao 
    						end
    						as municipio,
    						case when itrid in (1,3) THEN
    							'Estadual'
    						else
    							'Municipal'
    						end
    						as esfera ,
    						CASE WHEN adpresposta = 'C' THEN
    							'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
    						WHEN adpresposta is NULL then
    							'Entrou mas não fez registro (não aceitou, nem negou)'
    						WHEN adpresposta = 'S'  then
    							'Aceitou ao termo'
    						WHEN adpresposta = 'N'  then
    							'Não aceitou o termo'
    						END AS situacao_aceitacao_termo,
    						esddsc as situacao_adesao,
    						count( distinct aep.codinep ) as total_escolas ,
    						total_7::text, 
    						total_5::text, 
    						total_sem::text, 
    						''::text as total_15 
    						
    						
    						
    					 from par3.prodesaoprograma  ad 
    					 INNER JOIN par3.instrumentounidade inu ON ad.inuid = inu.inuid
    					 LEFT JOIN par3.adesaoescolaproemi aep ON aep.inuid = inu.inuid
    				         INNER JOIN workflow.documento doc ON ad.docid = doc.docid
    					INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
    					LEFT JOIN (
    						select 
    						count(distinct codinep) as total_7,
    						inuid
    						from par3.adesaoescolaproemi 
    						where aepcargahoraria = 7 
    						group by inuid
    						) tot7 ON tot7.inuid = ad.inuid 
    					LEFT JOIN (
    						select 
    						count(distinct codinep) as total_5,
    						inuid
    						from par3.adesaoescolaproemi 
    						where aepcargahoraria = 5
    						group by inuid
    						) tot5 ON tot5.inuid = ad.inuid 
    
    					LEFT JOIN 
    					(
    						select 
    						count(distinct codinep) as total_sem,
    						inuid
    						from par3.adesaoescolaproemi 
    						where aepcargahoraria IS NULL
    						group by inuid
    						) tot0 ON tot0.inuid = ad.inuid 
    					where pfaid = 7 
    					group by esfera, situacao_aceitacao_termo, situacao_adesao, municipio, inu.estuf, doc.esdid, cod_municipio, total_7, total_5, total_sem, total_15
    					order by 1,2,3,4,5,6,7, 8)
    				
    					UNION ALL";
	    }else{
	        $pfaid = "pfaid = 9 AND";
	    }
					
        $sql .= "(SELECT 
					
					'NOVO MAIS EDUCAÇÃO' as programa,
					inu.estuf,
					case when itrid in (1,3) THEN
						''
					else
						inu.muncod
					end
					as cod_municipio,
					case when itrid in (1,3) THEN
						''
					else
						inudescricao 
					end
					as municipio,
					case when itrid in (1,3) THEN
						'Estadual'
					else
						'Municipal'
					end
					as esfera ,
					CASE WHEN adpresposta = 'C' THEN
						'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
					WHEN adpresposta is NULL then
						'Entrou mas não fez registro (não aceitou, nem negou)'
					WHEN adpresposta = 'S'  then
						'Aceitou ao termo'
					WHEN adpresposta = 'N'  then
						'Não aceitou o termo'
					END AS situacao_aceitacao_termo,
					esddsc as situacao_adesao,
					count( distinct aea.codinep ) as total_escolas ,
					''::text  AS total_7,
					tot5.total_5::text, 
					total_sem::text,
					total_15::text
				FROM par3.prodesaoprograma  ad 
                INNER JOIN par3.instrumentounidade      inu ON ad.inuid = inu.inuid
                LEFT  JOIN par3.adesaoescolaabcmais     aea ON aea.adpid = ad.adpid
                INNER JOIN workflow.documento           doc ON ad.docid = doc.docid
                INNER JOIN workflow.estadodocumento     esd ON esd.esdid = doc.esdid
                LEFT JOIN 
                (
                	SELECT 
                    	count(DISTINCT codinep) as total_sem,
                    	inuid,
                        adpid
                	FROM par3.adesaoescolaabcmais 
                	WHERE aeacargahoraria IS NULL
                	GROUP BY inuid, adpid
                	) tot0 ON tot0.inuid = ad.inuid AND tot0.adpid = ad.adpid
				LEFT JOIN 
				(
					SELECT 
						count(DISTINCT codinep) as total_5,
						inuid,
                        adpid
					FROM par3.adesaoescolaabcmais  
					WHERE aeacargahoraria = 5
					GROUP BY inuid, adpid
				) tot5 ON tot5.inuid = ad.inuid AND tot5.adpid = ad.adpid 
				LEFT JOIN 
				(
					SELECT 
						count(DISTINCT codinep)as total_15,
						inuid,
                        adpid
					FROM par3.adesaoescolaabcmais  
					WHERE aeacargahoraria = 15 
					GROUP BY inuid, adpid
				) tot15 ON tot15.inuid = ad.inuid AND tot15.adpid = ad.adpid 
				where $pfaid ad.adpano_ciclo = $ano
				group by esfera, situacao_aceitacao_termo, situacao_adesao, municipio, inu.estuf, cod_municipio, total_7, total_5, total_sem, total_15
				order by 1,2,3,4,5,6,7, 8
				
				)
			
			) as f
		";
		
		$result = $this->carregar($sql);
		$result = (is_array($result)) ? $result : array();
		return  (count($result) > 0) ? $result : false;
		
	}
	
	public function carregarRespostas($inuid)
	{
		$respostas0 = array('A' => 'Sim', 'I' => 'Não');
		$respostas1 = array('1' => 'A ser elaborada', '2' => 'Em elaboração','3' =>'Já no legislativo');
		$respostas2 = array('1' => '2 anos', '2' => '3 anos','3' =>'4 anos', '4' => 'Outro', '5' => 'Não se aplica');
		$respostas3 = array('1' => '2016', '2' => '2017','3' =>'2018','4' =>'2019','5' =>'Outro','6' =>'Não se aplica');
		
		$prp = new Par3_Model_ProadesaoRespostasPnfcd();
		
		$ppp = $prp->pegarRespostasPorInuid($inuid);
		
		if (empty($ppp['pppresp0'])) {
			$html = "<div class='alert alert-info text-center'>Quetionário não foi respondido</div>";
		} else {
			$pppresp0 = $respostas0[$ppp['pppresp0']];
			$pppresp1 = $respostas1[$ppp['pppresp1']];
			$pppresp2 = $respostas2[$ppp['pppresp2']];
			$pppresp3 = $respostas3[$ppp['pppresp3']];
			
			$html = "<table class='table'>";
			$html.= "	<thead>";
			$html.= "		<tr>";
			$html.= "			<th style='text-align: left;'>Pergunta</th>";
			$html.= "			<th>Resposta</th>";
			$html.= "		</tr>";
			$html.= "	</thead>";
			$html.= "	<tbody>";
			$html.= "		<tr>";
			$html.= "			<td style='text-align: left;'>Já possui lei disciplinando a gestão democrática da educação pública conforme art. 9º do PNE?</td>";
			$html.= "			<td>{$pppresp0}</td>";
			$html.= "		</tr>";
			
			if ($ppp['pppresp1']) {
				$html.= "		<tr>";
				$html.= "			<td style='text-align: left;'>Em que fase se encontra?</td>";
				$html.= "			<td>{$pppresp1}</td>";
				$html.= "		</tr>";
			}
			
			$html.= "		<tr>";
			$html.= "			<td style='text-align: left;'>Qual o período de mandato de direção escolar disciplinado pela rede?</td>";
			$html.= "			<td>{$pppresp2}</td>";
			$html.= "		</tr>";
			
			if ($ppp['pppresp2'] == '4') {
				$html.= "		<tr>";
				$html.= "			<td style='text-align: left;'>Período de mandato de direção escolar:</td>";
				$html.= "			<td>{$ppp['pppresp2outros']}</td>";
				$html.= "		</tr>";
			}
				
			$html.= "		<tr>";
			$html.= "			<td style='text-align: left;'>Quando haverá a próxima seleção de diretores?</td>";
			$html.= "			<td>{$pppresp3}</td>";
			$html.= "		</tr>";
			
			if ($ppp['pppresp3'] == '4') {
				$html.= "		<tr>";
				$html.= "			<td style='text-align: left;'>Ano para seleção de diretores:</td>";
				$html.= "			<td>{$ppp['pppresp3outros']}</td>";
				$html.= "		</tr>";
			}
			
			$html.= "	</tbody>";
			$html.= "</table>";
		}
		
		return $html;
	}


    /**
    * @author Leo Kenzley <leokenzley@gmail.com>
    * @param $prgid
    * @param $inuid
    * @return bool
    * @throws Exception
    * @description Este método verifica se o município já iniciou o fluxo
    */
    public function fetchDadosDaAdesaoByIdInstrumentoUnidade($inuid){
        $sql = "select * from par3.prodesaoprograma 
                        where inuid = {$inuid};";
        try{
            return $this->pegaLinha($sql);
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }

    /**
     * @param $inuid
     * @param $pfaid
     * @description Este método retorna dados do Estado e documento do workflow
     */
    public function getDadosWorkFlowUnidadePrograma($inuid,$pfaid){
        $sql ="select 
                    * 
                    FROM par3.prodesaoprograma as proap
                    inner join workflow.documento as doc on doc.docid = proap.docid
                    inner join workflow.estadodocumento as esd on esd.esdid = doc.esdid
                    inner join par3.proadesao as proad on proad.pfaid = proap.pfaid
                    where 
                    proap.inuid = {$inuid}
                    and proap.pfaid = {$pfaid}";

        try{
            return $this->pegaLinha($sql);
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }

    /**
     * @param $inuid
     * @return array|bool|mixed|NULL
     * @throws Exception
     */
    public function getDadosDoUsuarioAssinou($inuid){
        $sql = "select * from par3.prodesaoprograma p
                inner join seguranca.usuario as usu on usu.usucpf = p.usucpf
                where inuid = {$inuid} and pfaid = 11";
        try{
            return $this->pegaLinha($sql);
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }
    }




}
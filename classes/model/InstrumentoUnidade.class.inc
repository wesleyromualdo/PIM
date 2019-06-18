<?php

class PIM_Model_InstrumentoUnidade extends Modelo
{
    /**
     * Nome da tabela especificada
     *
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'pim.instrumentounidade';

    /**
     * Chave primaria.
     *
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('inuid');

    /**
     * Chaves estrangeiras.
     *
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array(
            'tabela' => 'usuario',
            'pk' => 'usucpf',
        ),
        'muncod, mun_estuf' => array(
            'tabela' => 'territorios.municipio',
            'pk' => 'muncod, estuf',
        ),
        'itrid' => array(
            'tabela' => 'par3.instrumento',
            'pk' => 'itrid',
        ),
        'estuf' => array(
            'tabela' => 'territorios.estado',
            'pk' => 'estuf',
        )
    );

    /**
     * Atributos
     *
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'inuid' => null,
        'itrid' => null,
        'estuf' => null,
        'muncod' => null,
        'mun_estuf' => null,
        'docid' => null,
        'inudescricao' => null,
        'usucpf' => null,
        'inudata' => null,
        'inucnpj' => null,
        'qrpid' => null,
        'qrpidpne' => null,
        'qrpidbandalarga' => null,
        'inusemconselhoeducacao' => null,
    );

    const ESDID_ETAPA_PREPARATORIA      = 1638;
    const ESDID_DIAGNOSTICO             = 1637;
    const ESDID_DIAGNOSTICO_FINALIZADO  = 1683;
    const ESDID_PLANEJAMENTO_N_INICIADO = 1874;
    const ESDID_ELABORACAO              = 1999;
    const ESDID_ENVIADO_ANALISE         = 2000;

    public function retornaEstadosPreDiagnostico()
    {
        $arrEsdPreDIag= array(
            self::ESDID_ETAPA_PREPARATORIA,
        );
        return $arrEsdPreDIag;
    }

    public function retornaEstadosPrePlanejamento()
    {
        $arrEsdPreDIag = $this->retornaEstadosPreDiagnostico();
        $arrEsdPreDIag[] = self::ESDID_DIAGNOSTICO;
        $arrEsdPreDIag[] = self::ESDID_DIAGNOSTICO_FINALIZADO;
        return $arrEsdPreDIag;
    }

    /**
     * Função buscaInuidUF
     * - retorna o inuid do UF.
     *
     * @return integer inuid.
     *
     */
    public function buscaInuidUF()
    {
        $sql = "SELECT
                    inuid
                FROM
                    par3.instrumentounidade
                WHERE
                    itrid = 1
                    AND estuf = (
                            SELECT
                                estuf
                            FROM
                                par3.instrumentounidade
                            WHERE
                                inuid = {$this->inuid}
                            )";

        $inuid = $this->pegaUm($sql);

        return $inuid;
    }


    /**
     * Função testaPermissaoUnidade
     * - retorna o entid ativo.
     *
     * @return integer entid.
     *
     */
    public function testaPermissaoUnidade($inuid)
    {
        $unidade                 = new Par3_Model_InstrumentoUnidade($inuid);
        $usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();

        $arrPermissao = array(
            'booVisualizar' => true,
            'booEscrita'    => false,
        );

        if ($this->testa_superuser()) {
            $arrPermissao = array(
                'booVisualizar' => true,
                'booEscrita'    => true,
            );
            return $arrPermissao;
        }

        $muncod = $unidade->muncod;
        $estuf  = $unidade->estuf;
        
        // Valida SOMENTE perfis com responsabilidade
        $arResponsabilidade = pegarResponsabilidadeUsuario($_SESSION['usucpf']);
        if ($arResponsabilidade['sem_responsabilidade_atribuida'] == true) {
            return false;
        } elseif (is_array($arResponsabilidade)) {
            if (in_array($estuf, $arResponsabilidade['estuf']) == false && in_array($muncod, $arResponsabilidade['muncod']) == false) {
                $arrPermissao = array(
                    'booVisualizar' => false,
                    'booEscrita'    => false,
                );
                return $arrPermissao;
                //Validar o perfil de gestor da unidade
            } elseif(Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) {
                $arrPermissao = array(
                    'booVisualizar' => true,
                    'booEscrita'    => true,
                );
            }
        }
        
        return $arrPermissao;
    }//end testaPermissaoUnidade()



    /**
     * Função prepararFiltro
     * - monta o filtro SQL para lista de intstrumentos unidade.
     *
     * @return array com dados principais,
     * itrid = 1 // Estadual
     * itrid = 2 // Municipal
     */
    public function getDadosUnidade($inuid)
    {
        if (!$inuid) {
            return array();
        }
        $sql = "SELECT
			itrid,
			estuf,
			inudescricao,
			muncod
		FROM
			par3.instrumentounidade
		WHERE
			inuid = {$inuid}";

        return $this->pegaLinha($sql);
    }


    /**
     * Função prepararFiltro
     * - monta o filtro SQL para lista de intstrumentos unidade.
     *
     * @return retorna um array de filtros.
     *
     */
    public function prepararFiltro($arrPost)
    {
        $where = array();
       
        $arResponsabilidade = pegarResponsabilidadeUsuario($_SESSION['usucpf']);
        if ($arResponsabilidade['sem_responsabilidade_atribuida'] == true) {
            $where[] = "inu.estuf = '0'";
            $where[] = "inu.muncod = '0'";
        } elseif (is_array($arResponsabilidade)) {
            $arWhereResp = array();
            if ($arResponsabilidade['estuf']) {
                $arWhereResp[] = "inu.estuf IN ('". implode("', '", $arResponsabilidade['estuf']) ."')";
            }
            if ($arResponsabilidade['muncod']) {
                $arWhereResp[] = "inu.muncod IN ('". implode("', '", $arResponsabilidade['muncod']) ."')";
            }
            
            if ($arWhereResp) {
                $where[] = "(".implode(" OR ", $arWhereResp).")";
            }
        }
        
        if ($arrPost['itrid'] != '' && !is_array($arrPost['itrid'])) {
            $where[] = "itrid = {$arrPost['itrid']}";
        }

        if ($arrPost['itrid'] != '' && is_array($arrPost['itrid'])) {
            $itrids = implode(',', $arrPost['itrid']);
            $where[] = "itrid IN ({$itrids})";
        }

        if ($arrPost['mundescricao'] != '') {
            $arrPost['mundescricao'] = trim($arrPost['mundescricao']);
            $where[] = "UPPER(public.removeacento(inudescricao)) ILIKE UPPER(public.removeacento('%{$arrPost['mundescricao']}%'))";
        }

        if ($arrPost['muncod'] != "") {
            $where[] = "inu.muncod = '". $arrPost['muncod'] ."'";
        }

        if ($arrPost['estuf'] != "") {
            $where[] = "inu.estuf = '".$arrPost['estuf']."'";
        }

        if ($arrPost['esdid'] != '') {
            $where[] = $arrPost['esdid'] > 0 ? "esd.esdid = {$arrPost['esdid']}" : "esd.esdid IS NULL";
        }
        
        return $where;
    }//end prepararFiltro()

    
    public function pegarInuidAcessivel($arrPost)
    {
        $where = array();
        
        if ($arrPost['itrid']) {
            if ($arrPost['estuf'] == 'DF') {
                $arrPost['itrid'] = 3;
            }
            $arrPost['itrid'] = (array) $arrPost['itrid'];
            $where[] = "itrid IN ('" . implode("', '", $arrPost['itrid']) . "')";
        }
        
        if ($arrPost['estuf']) {
            $arrPost['estuf'] = (array) $arrPost['estuf'];
            $where[] = "estuf IN ('" . implode("', '", $arrPost['estuf']) . "')";
        }
        
        if ($arrPost['muncod']) {
            $arrPost['muncod'] = (array) $arrPost['muncod'];
            $where[] = "muncod IN ('" . implode("', '", $arrPost['muncod']) . "')";
        }
        
        $sql = "SELECT
                    inu.inuid
                FROM
                    par3.instrumentounidade inu
                WHERE
                    " . implode(' AND ', $where);
        $inuid = $this->pegaUm($sql);
        
        return ($inuid ? $inuid : '');
    }

    /**
     * Função pegarSQLLista
     * - monta SQL para lista de intstrumentos unidade.
     *
     * @return retorna a query.
     *
     */
    public function pegarSQLLista($arrPost)
    {
        $where = self::prepararFiltro($arrPost);

        return '
            SELECT
                inu.inuid AS id,
                inu.inuid,
                inu.itrid,
                COALESCE(inu.estuf, \'-\') AS estuf,
                inu.inudescricao,
                CASE
                    WHEN inu.docid IS NULL
                     THEN \'Não Iniciado\'
                    ELSE esd.esddsc
                END AS situacao,
                esd.esdid,
                ROUND((
                    SELECT DISTINCT
                        CASE
                            WHEN count(are.dimid) > 0
                              THEN (((
                                SELECT COUNT(*)
                                FROM par3.pontuacao p2
                                WHERE p2.inuid = inu.inuid
                              ) * 100 ) / count(ind.indid))
                            ELSE 0
                        END AS percentual
                    FROM 
                        par3.dimensao dim
                        INNER JOIN par3.area are ON are.dimid = dim.dimid AND are.arestatus = \'A\'
                        INNER JOIN par3.indicador ind ON ind.areid = are.areid
                    WHERE
                        dim.dimstatus = \'A\' AND
                        dim.itrid = inu.itrid
                ), 1) AS percentual,
                esd.esdid AS planejamento
            FROM
                par3.instrumentounidade inu
                LEFT JOIN workflow.documento doc
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                  ON doc.docid = inu.docid AND doc.tpdid = ' . PAR3_WORKFLOW_FLUXO_DO_PAR . '
            WHERE
                ' . implode(' AND ', $where) . '
            ORDER BY
              estuf, inudescricao
        ';
    }

    /**
     * Função recuperarInuidPar
     * - recupera inuid do PAR antigo.
     *
     * @return integer $inuid.
     *
     */
    public function recuperarInuidPar($inuid)
    {
        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $where = " AND estuf = '".$instrumentoUnidade->estuf."'";

        if ($instrumentoUnidade->itrid === '2') {
            $where = " AND muncod = '".$instrumentoUnidade->muncod."'";
        }

        $itrid = $instrumentoUnidade->itrid == 1 ? 1 : 2;

        $sql = 'SELECT
    	           inuid
    	       FROM
    	           par.instrumentounidade
    	       WHERE
    	           itrid = '.$itrid.$where;

        return $instrumentoUnidade->pegaUm($sql, 0, 30);
    }//end recuperarInuidPar()


    public function verificaInuidMunicipio($muncod)
    {
        $sql = "select inuid from {$this->stNomeTabela} where muncod = '{$muncod}'";
        return $this->pegaUm($sql);
    }

    public function verificaInuidEstado($estuf)
    {
        $sql = "select inuid from {$this->stNomeTabela} where estuf = '{$estuf}' and itrid in (1,3) ";
        return $this->pegaUm($sql);
    }

    /**
     * Função retornarQrpid
     * - recupera retorna qrpid.
     *
     * @return integer $qrpid.
     *
     */
    public function retornarQrpidUnidade($queid)
    {
        $sql = "SELECT qrpid FROM par3.questionario_unidade WHERE inuid = {$this->inuid} AND queid = $queid";
        $qrpid = $this->pegaUm($sql);

        if (!$qrpid) {
            $sql = "INSERT INTO questionario.questionarioresposta(queid, qrptitulo, qrpdata)
            		VALUES({$queid}, 'Reposta Questões PNE - inuid: {$this->inuid}', now())
            		RETURNING qrpid;";

            $qrpid = $this->pegaUm($sql);
            $this->commit();

            $sql = "INSERT INTO par3.questionario_unidade(inuid, queid, qrpid, usucpf)
    		        VALUES({$this->inuid}, $queid, $qrpid, '{$_SESSION['usucpf']}');";
            $this->executar($sql);
            $this->commit();
        }

        return $qrpid;
    }//end retornarQrpid()


    /**
     * Função retornarDocidQuestionario
     * - recupera retorna docid.
     *
     * @return integer $docid.
     *
     */
    public function retornarDocidQuestionario($queid)
    {
        $sql = "SELECT docid FROM par3.questionario_unidade WHERE inuid = {$this->inuid} AND queid = $queid";
        $docid = $this->pegaUm($sql);

        require_once APPRAIZ.'includes/workflow.php';

        if (!$docid) {
            switch ($queid) {
                case QUESTIONARIO_BANDA_LARGA:
                default:
                    $tpdid = PAR3_WORKFLOW_FLUXO_BANDA_LARGA;
                    break;
            }
            $docid = wf_cadastrarDocumento($tpdid, "Fluxo Banda Larga");

            $sql = "UPDATE par3.questionario_unidade SET
                        docid = $docid
                    WHERE inuid = {$this->inuid} AND queid = $queid;";
            $this->executar($sql);
            $this->commit();
        }

        return $docid;
    }//end retornarDocidQuestionario()


    /**
     * Função retornaPreenchimentoQuestoes
     * - recupera um arrat com o total de questões e o total de questões preenchidas.
     *
     * @return array.
     *
     */
    public function retornaPreenchimentoQuestoes($inuid)
    {
        if ($this->itrid) {
            if ($this->itrid == 1) {
                $queid = QUESTIONARIO_QUESTOES_ESTRATEGICAS_EST;
            } elseif ($this->itrid == 2) {
                $queid = QUESTIONARIO_QUESTOES_ESTRATEGICAS_MUN;
            } else {
                $queid = QUESTIONARIO_QUESTOES_ESTRATEGICAS_DF;
            }
        }

        $qrpid = $this->retornarQrpidUnidade($queid);

        $sql = "
    	        SELECT DISTINCT
    	            foo.qrpid,
					sum(total) as total,
					sum(respondido) as respondido,
                    SUM(qtd_sem_anexo) as qtd
    	        FROM
    	        (
                    (
                    SELECT DISTINCT
                		qrp.qrpid,
                		count(distinct per.perid) as total,
                		count(distinct res.perid) as respondido,
                        (SELECT count(itp.itpid) as qtd
                        FROM questionario.itempergunta itp
                        INNER JOIN questionario.pergunta                pei ON pei.perid = itp.perid
                        INNER JOIN questionario.resposta                res ON res.itpid = itp.itpid
            			INNER JOIN par3.questionario_unidade            qun ON qun.qrpid = res.qrpid AND qun.queid = $queid
                        INNER JOIN questionario.questionarioresposta    qrp ON qrp.qrpid = res.qrpid
                        INNER JOIN questionario.pergunta                per ON per.itpid = itp.itpid
                        LEFT  JOIN par3.questoesestrategicasanexos      qea ON qea.perid = per.perid AND qea.qrpid = qun.qrpid
                        WHERE
                            qrp.queid IN (128, 129, 130, 131)
                            AND per.pertitulo ILIKE '%arquivo%'
                            AND qea.arqid IS NULL
                            AND pei.perid NOT IN (/*df*/4795, 4799, /*e*/4691, 4695, /*m*/4631, 4635)
                            AND qrp.qrpid in ($qrpid) ) as qtd_sem_anexo
                	FROM
                		questionario.questionarioresposta qrp
                	INNER JOIN questionario.pergunta per on per.queid = qrp.queid
                	LEFT  JOIN questionario.resposta res on res.perid = per.perid and res.qrpid = qrp.qrpid
                	WHERE
                		qrp.qrpid in ($qrpid)
                		AND per.perid NOT IN (/*df*/4795, 4799, /*e*/4691, 4695, /*m*/4631, 4635)
                	GROUP BY
                		qrp.qrpid
                    )
    	            UNION ALL
    	            (
            		SELECT DISTINCT
                    	$qrpid as qrpid,
                    	CASE WHEN
                    	   (SELECT COUNT(perid) FROM questionario.resposta WHERE qrpid in ($qrpid) AND itpid IN (6542, 6397, 6307)) = 0
                    	   THEN 1
                    	   ELSE 0 END as total,
                    	CASE WHEN
                    	   (SELECT COUNT(perid) FROM questionario.resposta WHERE qrpid in ($qrpid) AND itpid IN (6542, 6397, 6307)) = 0 AND
                    	   (SELECT COUNT(perid) FROM questionario.resposta WHERE qrpid in ($qrpid) AND perid IN (/*df*/4795, 4799, /*e*/4691, 4695, /*m*/4631, 4635)) > 0
                    	   THEN 1 ELSE 0 END as respondido,
                    	CASE
                    		WHEN (SELECT count(itp.itpid) as qtd
                    			FROM questionario.itempergunta itp
                    			INNER JOIN questionario.pergunta                pei ON pei.perid = itp.perid
                    			LEFT  JOIN questionario.resposta                res ON res.itpid = itp.itpid AND res.perid IN (/*df*/4795, 4799, /*e*/4691, 4695, /*m*/4631, 4635)
                    			LEFT  JOIN par3.questionario_unidade            qun ON qun.qrpid = res.qrpid AND qun.queid = $queid
                    			LEFT  JOIN questionario.questionarioresposta    qrp ON qrp.qrpid = res.qrpid
                    			INNER JOIN questionario.pergunta                per ON per.itpid = itp.itpid
                    			LEFT  JOIN par3.questoesestrategicasanexos      qea ON qea.perid = per.perid AND qea.qrpid = qun.qrpid
                    			WHERE
                    			    qrp.queid IN (128, 129, 130, 131)
                    			    AND per.pertitulo ILIKE '%arquivo%'
                    			    AND qea.arqid IS NULL
                    			    AND qrp.qrpid in ($qrpid) ) > 0
                    		THEN 1 ELSE 0
                    	END as qtd_sem_anexo
                    )
                ) as foo
                GROUP BY
                    foo.qrpid";
        return $this->pegaLinha($sql);
    }

    /**
     * Função retornaQuestoes
     * - recupera um arrat com o total de questões e o total de questões preenchidas.
     *
     * @return array.
     *
     */
    public function retornaQuestoesParaPreenchimento($inuid)
    {
        if ($this->itrid) {
            if ($this->itrid == 1) {
                $queid = QUESTIONARIO_QUESTOES_ESTRATEGICAS_EST;
            } elseif ($this->itrid == 2) {
                $queid = QUESTIONARIO_QUESTOES_ESTRATEGICAS_MUN;
            } else {
                $queid = QUESTIONARIO_QUESTOES_ESTRATEGICAS_DF;
            }
        }

        $qrpid = $this->retornarQrpidUnidade($queid);

        $sql = "(
    	            (
        	        SELECT
    					qrp.qrpid,
        				per.pertitulo,
    					res.perid as respondido
                    FROM questionario.questionarioresposta qrp
                    INNER JOIN questionario.pergunta per on per.queid = qrp.queid
                    LEFT  JOIN questionario.resposta res on res.perid = per.perid and res.qrpid = qrp.qrpid
                    WHERE
        	           qrp.qrpid in ($qrpid)
        	           AND per.perid NOT IN (/*df*/4795, 4799, /*e*/4691, 4695, /*m*/4631, 4635)
                    ORDER BY per.perordem
        	        )
        	        UNION ALL
        	        (
        	        SELECT
    					qrp.qrpid,
        				'As perguntas 4.1.1 e 4.1.2 não foram respondidas. Responder pelo menos uma.' as pertitulo,
    					CASE WHEN
                    	   (SELECT COUNT(perid) FROM questionario.resposta WHERE qrpid in ($qrpid) AND itpid IN (6542, 6397, 6307)) = 0 AND
    					   (SELECT COUNT(perid) FROM questionario.resposta WHERE qrpid in ($qrpid) AND perid IN (/*df*/4795, 4799, /*e*/4691, 4695, /*m*/4631, 4635)) > 0
    					   THEN 100
    					   WHEN (SELECT COUNT(perid) FROM questionario.resposta WHERE qrpid in ($qrpid) AND itpid IN (6307) AND perid IN (4448)) > 0
    					   THEN 100
    					   ELSE 0
    					END as respondido
                    FROM questionario.questionarioresposta qrp
                    INNER JOIN questionario.pergunta per on per.queid = qrp.queid
                    LEFT  JOIN questionario.resposta res on res.perid = per.perid and res.qrpid = qrp.qrpid AND res.perid IN (/*df*/4795, 4799, /*e*/4691, 4695, /*m*/4631, 4635)
                    WHERE
        	           qrp.qrpid in ($qrpid)
                    GROUP BY qrp.qrpid
                    )
                )
                UNION ALL
                (
                SELECT
                    qrp.qrpid,
    				pei.pertitulo||' - Item: '''||itp.itptitulo||''' sem anexo.',
					0 as respondido
                FROM questionario.itempergunta itp
                INNER JOIN questionario.pergunta                pei ON pei.perid = itp.perid
                INNER JOIN questionario.resposta                res ON res.itpid = itp.itpid
                INNER JOIN par3.questionario_unidade            qun ON qun.qrpid = res.qrpid AND qun.queid = $queid
                INNER JOIN par3.instrumentounidade              inu ON inu.inuid = qun.inuid
                INNER JOIN questionario.questionarioresposta    qrp ON qrp.qrpid = res.qrpid
                INNER JOIN questionario.pergunta                per ON per.itpid = itp.itpid
                LEFT  JOIN par3.questoesestrategicasanexos      qea ON qea.perid = per.perid AND qea.qrpid = qun.qrpid
                WHERE
                    qrp.queid IN (128, 129, 130, 131)
                    AND per.pertitulo ILIKE '%arquivo%'
                    AND qea.arqid IS NULL
                    AND qrp.qrpid in ($qrpid)
                ORDER BY
                    inu.estuf, inu.inudescricao)";

        $dados = $this->carregar($sql);
        //ver($sql, d);
        return $dados;
    }

    /**
     * Função retornaQuestoes
     * - recupera um arrat com o total de questões e o total de questões preenchidas.
     *
     * @return array.
     *
     */
    public function retornaQuestoesParaPreenchimentoPNE($inuid)
    {
        $sql = "select
					qrp.qrpid,
    				per.pertitulo,
					res.perid as respondido
				from questionario.questionarioresposta qrp
                inner join questionario.pergunta per on per.queid = qrp.queid
                left  join questionario.resposta res on res.perid = per.perid and res.qrpid = qrp.qrpid
                where qrp.qrpid in (".$this->retornarQrpidUnidade(QUESTIONARIO_QUESTOES_PNE).")
                order by per.perordem";

        return $this->carregar($sql);
    }

    /**
     * Função retornaPreenchimentoQuestoesPNE
     * - recupera um arrat com o total de questões e o total de questões preenchidas.
     *
     * @return array.
     *
     */
    public function retornaPreenchimentoQuestoesPNE($inuid)
    {

        $sql = "select distinct
					qrp.qrpid,
					count(distinct per.perid) as total,
					count(distinct res.perid) as respondido
				from
					questionario.questionarioresposta qrp
				inner join questionario.pergunta per on per.queid = qrp.queid
				left  join questionario.resposta res on res.perid = per.perid and res.qrpid = qrp.qrpid
				where
					qrp.qrpid in (".$this->retornarQrpidUnidade(QUESTIONARIO_QUESTOES_PNE).")
				group by
					qrp.qrpid";

        return $this->pegaLinha($sql);
    }

    /**
     * Função recuperarEmailEsponsaveis
     * - recupera um arrat com os emaisl dos repsonsaveis.
     *
     * @return array.
     */
    public function recuperarEmailEsponsaveis($inuid)
    {
        if ($this->inuid == '') {
            $this->carregarPorId($inuid);
        }

        $sql = "SELECT
                	usuemail
                FROM
                	seguranca.usuario usu
                INNER JOIN par3.usuarioresponsabilidade usr ON usr.usucpf = usu.usucpf
                WHERE
                	(usr.estuf = '{$this->estuf}' AND usr.pflcod IN (1434, 1437))
                	OR (usr.muncod = '{$this->muncod}' AND usr.pflcod IN (1433, 1436))";

        return $this->carregarColuna($sql);
    }//end recuperarEmailEsponsaveis()

    public function salvarJustificativaTreinamento($inuid)
    {

        if ($inuid && $_REQUEST['justificativa']) {
            $sql = "UPDATE ".$this->stNomeTabela." SET inujustificativa = ". $justificativa." WHERE inuid = ".$inuid;
            $this->executar($sql);
            $this->commit();
        }
        return true;
    }

    //Municipios que transmitiram o SIOPE 2015
    public function verificaMunicipioSIOP($inuid)
    {
        $sql = "SELECT inusiop
		    	FROM par3.instrumentounidade
		    	WHERE inuid = $inuid";

        $boP = $this->pegaUm($sql);
        $boP = ($boP == 'f' ? false : true);
        return $boP ;
    }

    public function carregaOrcamentoUnidade($inuid)
    {
        $sql = "SELECT
                	tor.torid,
                	iuo.iuovalor,
                	tor.tordescricao,
                	SUM(
                	CASE
                		WHEN pps.ppscronograma = 1 THEN ico.icovalor*ico.icoquantidade
                		WHEN pps.ppscronograma = 2 THEN ico.icovalor*sei.seiqtd
                	END) as valor_utilizado,
                	iuo.iuovalor-coalesce(
                	SUM(
                	CASE
                		WHEN pps.ppscronograma = 1 THEN ico.icovalor*ico.icoquantidade
                		WHEN pps.ppscronograma = 2 THEN ico.icovalor*sei.seiqtd
                	END)
                	, 0) as valor_restante
                FROM
                	par3.instrumentounidade_orcamento iuo
                INNER JOIN par3.tipoorcamento                  tor ON tor.torid = iuo.torid
                LEFT  JOIN par.propostasubacao                 pps ON pps.torid = tor.torid
                LEFT  JOIN par.subacao 		                   sba ON sba.ppsid = pps.ppsid AND sba.inuid_par3 = iuo.inuid AND sba.sbastatus = 'A'
            	LEFT  JOIN par.subacaodetalhe 	               sbd ON sbd.sbaid = sba.sbaid
            	LEFT  JOIN par.subacaoitenscomposicao          ico ON ico.sbaid = sbd.sbaid AND ico.icoano = sbd.sbdano AND ico.icostatus = 'A'
            	LEFT  JOIN par.subacaoescolas                  ses ON ses.sbaid = sba.sbaid AND ses.sesano = sbd.sbdano AND ses.sesstatus = 'A'
            	LEFT  JOIN par.subescolas_subitenscomposicao   sei ON sei.sesid = ses.sesid AND sei.icoid = ico.icoid
                WHERE
                	iuo.inuid = $inuid
    	            AND iuo.iuostatus = 'A'
                GROUP BY
    	            tor.torid,
                	iuo.iuovalor,
                	tor.tordescricao";

        $arrOrcamento = $this->carregar($sql);
        $arrOrcamento = is_array($arrOrcamento) ? $arrOrcamento : array();

        $arrDados = array();

        foreach ($arrOrcamento as $orçamento) {
            $arrDados[$orçamento['torid']]['descricao']        = $orçamento['tordescricao'];
            $arrDados[$orçamento['torid']]['valor']            = $orçamento['iuovalor'];
            $arrDados[$orçamento['torid']]['valor_utilizado']  = $orçamento['valor_utilizado'];
            $arrDados[$orçamento['torid']]['valor_restante']   = $orçamento['valor_restante'];
        }

        return $arrDados;
    }

    public function retornaBooPendencias()
    {
        $sql = "SELECT * FROM par3.vm_relatorio_quantitativo_pendencias WHERE inuid = {$this->inuid}";

        $arrDados = $this->pegaLinha($sql);

        return $arrDados;
    }

    /**
     * @param $inuid
     * @return array|mixed|NULL
     */
    public function getInstrumentoUnidadeById($inuid)
    {
        $sql = "SELECT
                    *
                FROM
                    par3.instrumentounidade
                WHERE
                    itrid = 1
                    AND estuf = (
                            SELECT
                                estuf
                            FROM
                                par3.instrumentounidade
                            WHERE
                                inuid = {$inuid}
                            )";

        $result = $this->carregar($sql);

        return $result;
    }

    public function getInstrumentoUnidadeByIdMunic($inuid)
    {
        $sql = "SELECT * FROM par3.instrumentounidade WHERE inuid = {$inuid}";
        $result = $this->pegaLinha($sql);
        return $result;
    }

    public function pegarNomeEstado($esdid)
    {
        return $this->pegaUm("SELECT esddsc FROM workflow.estadodocumento WHERE esdid = " . $esdid);
    }

    public function create($inuid)
    {
        $sql = "SELECT * from par3.instrumentounidade where inuid = {$inuid}";
        return $this->pegaLinha($sql);
    }
    
    public function atualizarPendencia($inuid){
        return $this->pegaLinha(
                <<<sql
                select * from par3.vm_relatorio_quantitativo_pendencias where inuid = {$inuid}
                
sql
                );
    }
    
    public function pegarEscopoPendencias(){
        return $this->carregar(<<<sql
            SELECT 
	            b.rebid as "id", 
	            p.tipdsc as "descricao", 
	            b.rebplanejamento as "planejamento", 
	            b.rebempenho as "empenho", 
	            b.rebtermo as "termo", 
	            b.rebpagamento as "pagamento" 
            FROM 
            	par3.regra_bloqueio b
            	INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
            WHERE 
	            p.tipstatus = 'A'
				and (
		            b.rebplanejamento = true 
		            or b.rebempenho = true 
		            or b.rebtermo = true 
		            or b.rebpagamento = true 
                )
            ORDER BY b.rebid
sql
            );
    }
    public function pegarEscopoAlertas(){
        return $this->carregar(<<<sql
            SELECT 
	            b.rebid as "id", 
	            p.tipdsc as "descricao", 
	            b.rebplanejamento as "planejamento", 
	            b.rebempenho as "empenho", 
	            b.rebtermo as "termo", 
	            b.rebpagamento as "pagamento" 
            FROM 
            	par3.regra_bloqueio b
            	INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
            WHERE 
	            p.tipstatus = 'A'
				and (
		            b.rebplanejamento = false 
		            and b.rebempenho = false 
		            and b.rebtermo = false 
		            and b.rebpagamento = false 
                )
            ORDER BY b.rebid
sql
            );
    }
}//end class

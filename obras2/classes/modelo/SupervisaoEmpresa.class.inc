<?php


class SupervisaoEmpresa extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisaoempresa";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sueid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sueid' => null,
        'sobid' => null,
        'empid' => null,
        'entidvistoriador' => null,
        'usucpf' => null,
        'endid' => null,
        'sosid' => null,
        'docid' => null,
        'suedtsupervisao' => null,
        'suedtcadastro' => null,
        'suefuncionamento' => null,
        'suecargovistoriador' => null,
        'sueendcorreto' => null,
        'sueacordo' => null,
        'suedtatualizacao' => null,
        'suestatus' => null ,
        'tplid' => null,
        'sueobservacao' => null,
        'sueruralurbano' => null,
        'sueprojetofnde' => null,
        'suejustificativa' => null,
        'suepercentualexe' => null,
        'suearqmemcalc' => null,
        'sueobsretificacaoos' => null,
        'sueproblema' => null,
        'sueobsproblema' => null,
    );

	public function  listaDados( Array $param = array() )
	{

		$where = array();
		$join  = array();

		if ( $param['empid'] ){
			$where[] = "se.empid = {$param['empid']}";
		}

		if ( $param['sueid'] ){
			$where[] = "se.sueid = {$param['sueid']}";
		}

		if( $param['obrid'] ){
			$join[] = "
			JOIN obras2.obras obr ON obr.empid = soo.empid AND obr.obrid = {$param['obrid']}";
		}

		if ( !possui_perfil( array( PFLCOD_SUPER_USUARIO,
                                            PFLCOD_EMPRESA_MI_FISCAL,
                                            PFLCOD_EMPRESA_MI_GESTOR,
                                            PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                                            PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                                            PFLCOD_GESTOR_UNIDADE,
                                            PFLCOD_GESTOR_MEC,
                                            PFLCOD_SUPERVISOR_UNIDADE,
                                            PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                                            PFLCOD_EMPRESA_CONTRATADA ) ) ){
			$where[] = "ed.esdid = " . ESDID_VISTORIA_EMP_LAUDO;
		}

        $sql = "SELECT DISTINCT
                    se.sueid,
                    os.sosid,
                    sos.sosnum,
                    TO_CHAR(se.suedtsupervisao, 'dd/mm/yyyy') AS suedtsupervisao,
                    TO_CHAR(se.suedtcadastro, 'dd/mm/yyyy') AS suedtcadastro,
                    --so.sobdsc,
                    ed.esdid,
                    CASE WHEN ed.esddsc IS NOT NULL
                            THEN ed.esddsc
                            ELSE 'Não iniciado'
                    END AS esddsc,
                    e.entnome,
                    e.entnumcpfcnpj AS entcpf,
                    se.suecargovistoriador,
                    CASE WHEN se.suefuncionamento = 'n'
                            THEN 'Não'
                          WHEN se.suefuncionamento = 's'
                            THEN 'Sim' ELSE ''
                    END AS suefuncionamento,
                    u.usucpf,
                    u.usunome,
                    se.suepercentualexe as percentual
                FROM
                    obras2.supervisao_os_obra soo
                INNER JOIN obras2.supervisao_os     sos ON sos.sosid = soo.sosid AND sos.sosstatus = 'A'
                ".(count($join) ? implode(' ',$join) : "") . "
                LEFT JOIN obras2.supervisaoempresa  se  ON se.sosid = sos.sosid AND se.empid = soo.empid AND se.suestatus = 'A'
                LEFT JOIN obras2.supervisao         s   ON s.sueid = se.sueid AND s.supstatus = 'A'
                LEFT JOIN workflow.documento        d   ON d.docid = se.docid
                LEFT JOIN workflow.estadodocumento  ed  ON ed.esdid = d.esdid
                LEFT JOIN obras2.supervisao_os      os  ON os.sosid = se.sosid AND os.sosstatus = 'A'
                LEFT JOIN seguranca.usuario         u   ON u.usucpf = se.usucpf
                LEFT JOIN entidade.entidade         e   ON e.entid = se.entidvistoriador
                WHERE soo.soostatus = 'A'".
                    (count($where) ? " AND " . implode(' AND ',$where) : "") . "
                ORDER BY
                    se.sueid";

// 		ver(simec_htmlentities($sql),d);
		$dados = $this->carregar( $sql );
		return ($dados ? $dados : array());
	}

	public function excluir( $sueid ){
		$sql = "UPDATE obras2.supervisao SET
					supstatus='I'
				WHERE
				 	sueid = {$sueid};";
		$this->executar( $sql );

		$sql = "UPDATE obras2.supervisaoempresa SET
					suestatus='I'
				WHERE
				 	sueid = {$sueid};";
		$this->executar( $sql );

	}

	public function pegaUltSueidByEmpreendimento( $empid ){
		$arJoin  = array();
		$arWhere = array();

//		if ( $param['not(emsid)'] ){
//			$arWhere[] = "s.emsid IS NULL";
//		}
//
//		if ( $param['not(sueid)'] ){
//			$arWhere[] = "s.sueid IS NULL";
//		}
//
//		if ( $param['empid'] && $param['is(sueid)'] ){
//			$arJoin[] = "JOIN obras2.supervisaoempresa se ON se.sueid = s.sueid";
//			$arWhere[] = "se.empid = {$param['empid']}";
//		}elseif ( $param['empid'] ){
//			$arJoin[] = "JOIN obras2.empreendimentosupervisao es ON es.emsid = s.emsid";
//			$arWhere[] = "es.empid = {$param['empid']}";
//		}

		$sql = "SELECT
                	se.sueid
                FROM
                	obras2.supervisaoempresa se
                " . (count($arJoin) ? implode(" ", $arJoin) : "") . "
                WHERE
                	se.suestatus = 'A' AND
                	" . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . "
                    se.empid = {$empid}
                ORDER BY
                	se.suedtsupervisao DESC, se.sueid DESC
                LIMIT 1";

		return $this->pegaUm( $sql );
	}

	public function pegaNomeResponsavelPorSueid( $sueid ){
		$sql = "SELECT
                                entnome
                        FROM
                                obras2.supervisaoempresa se
                        JOIN entidade.entidade e ON e.entid = se.entidvistoriador
                        WHERE
                                sueid = {$sueid}";

		return $this->pegaUm( $sql );
	}

	public function carregaDadosHomologacao( $sueid ){
		$sql = "SELECT
					MAX(hstid) AS hstid,
					TO_CHAR(htddata, 'DD/MM/YYYY') AS htddata,
					u.usucpf,
					u.usunome
				FROM
					obras2.supervisaoempresa se
				JOIN workflow.historicodocumento hd ON hd.docid = se.docid
				JOIN seguranca.usuario u ON u.usucpf = hd.usucpf
				WHERE
					se.sueid = {$sueid} AND
					hd.aedid = " . WF_AEDID_LAUDO_SUPERVISAO_HOMOLOGAR . "
				GROUP BY
					htddata,
					u.usucpf,
					u.usunome";

		$dados = $this->pegaLinha( $sql );

		return ($dados ? $dados : array());
	}

        public function pegaDataOsSupEmpresa($sueid){
            if(empty($sueid)){
                $sueid = $this->sueid;
                if(empty($sueid)){
                    return date('Y-m-d');
                }
            }
            $sql = "
                    SELECT sosdtinicio
                    FROM obras2.supervisao_os sos
                    INNER JOIN obras2.supervisaoempresa sue ON sue.sosid = sos.sosid
                    WHERE sue.sueid = ".$sueid;

            $data = $this->pegaUm($sql);
            return $data;

        }

        public function pegaStatusSupEmpresa($docid){
            $sql = "SELECT wd.docid, wd.tpdid, wd.esdid, ed.esddsc
                    FROM workflow.documento wd
                    INNER JOIN workflow.estadodocumento ed ON (ed.esdid = wd.tpdid)
                    WHERE wd.docid = {$docid}";

            $dados = $this->pegaLinha($sql);

            return ($dados ? $dados : array());
        }

    /**
     * Verifica se existe uma divergencia entre a situação da obra de acordo com a vistoria e a supervisão.
     * Caso exita, envia um email e cria uma restrição
     *
     * @param $obrid
     */
    public function verificaDivergenciaSituacao($obrid)
    {
        $obra = new Obras($obrid);

        /*
            1;Em Execução
            2;Paralisado
            3;Concluído
            99;Em Planejamento pelo Proponente
        */
        $sql = "
                SELECT
                    st.sobid as id,
                    st.sobdsc as situacao
                FROM obras2.supervisaoempresa sue
                JOIN obras2.supervisao s ON s.sueid = sue.sueid
                JOIN  obras2.situacaoobra st ON st.sobid = sue.sobid
                WHERE
                    sue.empid = {$obra->empid}
                    AND sue.suestatus = 'A'
                ORDER BY sue.suedtsupervisao DESC
                LIMIT 1";

        $situacaoSup = $this->pegaLinha($sql);


        /*
            1;Em Execução
            3;Concluída
            4;Paralisada
            5;Não Iniciada
         */
        $sql = "
                SELECT
                s.staid as id,
                si.stadesc as situacao
                FROM
                    obras2.supervisao s
                INNER JOIN
                    seguranca.usuario u ON u.usucpf = s.usucpf
                LEFT JOIN obras2.situacaoatividade si ON si.staid = s.staid
                WHERE
                    s.obrid = {$obrid} AND
                    s.emsid IS NULL AND s.smiid IS NULL AND
                    s.supstatus = 'A'
                    AND validadaPeloSupervisorUnidade = 'S'
                ORDER BY s.supdata DESC
                LIMIT 1
                ";
        $situacaoVis = $this->pegaLinha($sql);

        // Quando a obra não possui vistoria
        if(empty($situacaoVis)) {
            /*
                1;Em Execução
                3;Concluída
                4;Paralisada
                5;Não Iniciada
            */
            $sql = "
            SELECT
              CASE WHEN ed.esdid IN (763, 764, 689) THEN 5
                WHEN ed.esdid IN (690) THEN 1
                WHEN ed.esdid IN (691) THEN 4
                WHEN ed.esdid IN (693) THEN 3
                END id,
                ed.esddsc situacao
            FROM obras2.obras o
            LEFT JOIN workflow.documento d ON d.docid = o.docid
            LEFT JOIN workflow.estadodocumento ed ON ed.esdid   = d.esdid
            WHERE o.obrid = {$obrid}
            AND ed.esdid IN (
                689, /* Planejamento pelo proponente */
                763, /* Licitação */
                764, /* Contratação */
                690, /* Execução */
                693, /* Concluída */
                691 /* Paralisada */
                )
        ";
            $situacaoVis = $this->pegaLinha($sql);
        }

        // Se a obra não possui uma das situações mapeadas.
        if(empty($situacaoVis)) {
            return;
        }

        if(
            ($situacaoSup['id'] == 1 && $situacaoVis['id'] != 1) ||
            ($situacaoSup['id'] == 2 && $situacaoVis['id'] != 4) ||
            ($situacaoSup['id'] == 3 && $situacaoVis['id'] != 3) ||
            ($situacaoSup['id'] == 99 && $situacaoVis['id'] != 5)
        ){
            $dados['fsrid'] = 1;
            $dados['rstitem'] = "R";
            $dados['rstdtprevisaoregularizacao'] = date("d/m/Y",mktime (0, 0, 0, date("m"), date("d") + 15, date("Y")));
            $dados['rstdsc'] = "Em decorrência do monitoramento realizado por meio do Sistema Integrado de Monitoramento,
                                Execução e Controle do Ministério da Educação (Simec) e de supervisão realizada por empresa contratada pelo FNDE,
                                verificamos que existe uma divergência entre a situação da obra informado em vistoria pelo fiscal da entidade responsável
                                pela obra (" . $situacaoVis['situacao'] . ") e o da empresa de supervisão (" . $situacaoSup['situacao'] . ").";

            $dados['rstdscprovidencia'] =  "1. Solicitamos o fiscal reveja a situação da obra, corrigindo-a se for o caso, inserindo nova vistoria,
                                            com fotos atualizadas da obra, que comprovem a situação atual.
                                            2. Solicitamos, no prazo máximo de 15 dias, que o cumprimento das providências requeridas seja informado a esta Autarquia,
                                            por meio do Sistema Integrado de Monitoramento, Execução e Controle do Ministério da Educação (Simec). O não atendimento das
                                            providências solicitadas causará a suspensão do repasse de recursos dessa obra até a sua resolução.";
            $dados['tprid'] = 17;

            $restricao = new Restricao();
            $restricao->salvaRestricaoInconformidade($dados, $obrid);
            $restricao->atualizaDocidNullRetricao();
            $this->commit();

            $email = new Email();
            $email->enviaEmailDivergenciaSituacaoSupervisao($obrid, $situacaoSup['situacao'], $situacaoVis['situacao']);
        }

    }


    /**
     * Verifica se a restrição criada pela função verificaDivergenciaSituacao ainda se encontra válida
     * @param $obrid
     */
    public function verificaDivergenciaSituacaoRestricao($obrid)
    {

        if(!$obrid) return false;

        $restricoes = "SELECT
                            r.rstid, r.docid
                        FROM obras2.restricao  r
                        JOIN workflow.documento d ON d.docid = r.docid
                        WHERE
                            r.obrid = 32674
                            AND r.rstitem = 'I'
                            AND r.tprid = 17
                            AND r.rstdsc ILIKE '%divergência entre a situação da obra informado%'
                            AND rststatus = 'A'
                            AND d.esdid IN (1140 /*Aguardando Providência*/, 1144 /*Aguardando correção*/ , 1141 /*Aguardando análise FNDE*/)

                      ";

        $restricoes = $this->carregar($restricoes);

        if($restricoes){
            $obra = new Obras($obrid);

            /*
                1;Em Execução
                2;Paralisado
                3;Concluído
                99;Em Planejamento pelo Proponente
            */
            $sql = "
                SELECT
                    st.sobid as id,
                    st.sobdsc as situacao
                FROM obras2.supervisaoempresa sue
                JOIN obras2.supervisao s ON s.sueid = sue.sueid
                JOIN  obras2.situacaoobra st ON st.sobid = sue.sobid
                WHERE
                    sue.empid = {$obra->empid}
                    AND sue.suestatus = 'A'
                ORDER BY sue.suedtsupervisao DESC
                LIMIT 1";

            $situacaoSup = $this->pegaLinha($sql);

            /*
                1;Em Execução
                3;Concluída
                4;Paralisada
                5;Não Iniciada
             */
            $sql = "
                SELECT
                s.staid as id,
                si.stadesc as situacao
                FROM
                    obras2.supervisao s
                INNER JOIN
                    seguranca.usuario u ON u.usucpf = s.usucpf
                LEFT JOIN obras2.situacaoatividade si ON si.staid = s.staid
                WHERE
                    s.obrid = {$obrid} AND
                    s.emsid IS NULL AND s.smiid IS NULL AND
                    s.supstatus = 'A'
                    AND validadaPeloSupervisorUnidade = 'S'
                ORDER BY s.supdata DESC
                LIMIT 1
                ";
            $situacaoVis = $this->pegaLinha($sql);

            if(($situacaoSup['id'] == 1 && $situacaoVis['id'] == 1) || ($situacaoSup['id'] == 2 && $situacaoVis['id'] == 4) || ($situacaoSup['id'] == 3 && $situacaoVis['id'] == 3) || ($situacaoSup['id'] == 99 && $situacaoVis['id'] == 5)){
                // Supera restrições
                foreach($restricoes as $restricao){
                    $doc = wf_pegarDocumento($restricao['docid']);

                    if ($doc['esdid'] == ESDID_AGUARDANDO_PROVIDENCIA)
                        wf_alterarEstado($restricao['docid'], 3866, 'Divergência corrigida.', array('rstid' => $restricao['rstid']));

                    else if ($doc['esdid'] == ESDID_AGUARDANDO_CORRECAO)
                        wf_alterarEstado($restricao['docid'], 3868, 'Divergência corrigida.', array('rstid' => $restricao['rstid']));

                    else if ($doc['esdid'] == ESDID_AGUARDANDO_ANALISE_FNDE)
                        wf_alterarEstado($restricao['docid'], 2651, 'Divergência corrigida.', array('rstid' => $restricao['rstid']));

                    $sql = "UPDATE workflow.historicodocumento SET usucpf = '00000000191' WHERE docid = {$restricao['docid']} AND aedid IN (3866, 3868, 2651)";
                    $this->executar($sql);
                    $this->commit();
                }

            }

        }

    }

}
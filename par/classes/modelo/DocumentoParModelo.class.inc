<?php

class DocumentoParModelo extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.documentopar";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dopid");

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
        'dopprorrogacaovigenciaobra' => null
    );

    public function relatorioTermoCompromisso($arrPost = array(), $booSQL = FALSE) {
        #armazena as condicoes da consulta
        //$arrWhere = array("foo.status = 'A'");

        #numero termo
        if (!empty($arrPost['numerotermo'])) {
            $arrWhere[] = "foo.termo ilike '%{$arrPost['numerotermo']}%'";
        }

        #ano termo
        if (!empty($arrPost['anotermo'])) {
            $arrWhere[] = "foo.dopano = '{$arrPost['anotermo']}'";
        }

        #processo
        if (!empty($arrPost['processo'])) {
            $arrPost['processo'] = str_replace(array('/', '.', '-'), '', $arrPost['processo']);
            $arrWhere[] = "foo.processo = '{$arrPost['processo']}'";
        }

        #uf
        if (!empty($arrPost['estuf'][0])) {
            $arrWhere[] = "foo.uf IN ('" . implode("', '", $arrPost['estuf']) . "')";
        }

        #municipio
        if (!empty($arrPost['listaMunicipio'][0])) {
            $arrWhere[] = "foo.muncod IN ('" . implode("', '", $arrPost['listaMunicipio']) . "')";
        }

        #Processo
        $processo = "'<span class=\"processo_detalhe\" >' || to_char(foo.processo::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>'";
        if (isset($arrPost['btnExcel'])) {
            $processo = "(TO_CHAR(foo.processo::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00'))";
        }

        if (!empty($arrPost['tipo']) && $arrPost['tipo'] != 'T') {
        	$filtroTipo = "where tipo = '{$arrPost['tipo']}'";
        }

        #Possui Termos
        if( $arrPost['tem_termo'] == 'S' ) {
        	$arrWhere[]	= "foo.codigo in (select distinct
											codigo
										from(
										    select prpid as codigo, 'SUBACAO' as tipo from par.documentopar where dopstatus = 'A' and prpid is not null
										    union all
										    select proid as codigo, 'PAR' as tipo from par.documentopar where dopstatus = 'A' and proid is not null
										    union all
										    select proid as codigo, 'PAC' as tipo from par.termocompromissopac where terstatus = 'A'
										) as fo $filtroTipo)";
        }
        if( $arrPost['tem_termo'] == 'N' ) {
        	$arrWhere[]	= "foo.codigo not in (select distinct
												codigo
											from(
											    select prpid as codigo, 'SUBACAO' as tipo from par.documentopar where dopstatus = 'A' and prpid is not null
											    union all
											    select proid as codigo, 'PAR' as tipo from par.documentopar where dopstatus = 'A' and proid is not null
											    union all
											    select proid as codigo, 'PAC' as tipo from par.termocompromissopac where terstatus = 'A'
											) as fo $filtroTipo)";
        }

        $sql = " WITH
                    temp_documentoparreprogramacao as
                    (SELECT dopid FROM par.documentoparreprogramacao WHERE (dprstatus = 'P' OR dprstatus = 'A') AND dprvalidacao NOT IN ('f')),
                    temp_documentoparreprogramacaosubacao as
                    (SELECT dopid FROM par.documentoparreprogramacaosubacao WHERE (dpsstatus = 'P' OR dpsstatus = 'A') AND dpsvalidacao NOT IN ('f')) ";

        $sql .= " SELECT
                    uf,
                    entidade,
                    esfera,
                    termo,
                    dopano,
                    $processo as processo,
                    documento,
                    vigencia,
                    COALESCE(saldo::numeric, 0.00) as saldo,
                    subacao,
                    repro_prazo,
                    repro_sub,
                    rescisao,
                    tipo
                FROM (";
        if ($arrPost['tipo'] != 'T') {
            if ($arrPost['tipo'] == 'SUBACAO') {
                $sql .= $this->relatorioTermoCompromissoSubacao($arrPost, TRUE);
            } else if ($arrPost['tipo'] == 'PAC') {
                $sql .= $this->relatorioTermoCompromissoPac($arrPost, TRUE);
            } else {
                $sql .= $this->relatorioTermoCompromissoPar($arrPost, TRUE);
            }
        } else {
            $sql .= $this->relatorioTermoCompromissoSubacao(array(), TRUE);
            $sql .= " UNION ALL ";
            $sql .= $this->relatorioTermoCompromissoPac(array(), TRUE);
            $sql .= " UNION ALL ";
            $sql .= $this->relatorioTermoCompromissoPar(array(), TRUE);
        }
        $sql .=" ) foo
            " . (!empty($arrWhere) ? ' WHERE '.implode(' AND ', $arrWhere) : '' ) . "
         ORDER BY tipo, uf, entidade, documento";
        return ($booSQL) ? $sql : $this->carregar($sql);
    }

    public function relatorioTermoCompromissoSubacao($arrPost = array(), $booSQL = FALSE) {

        $sql = "SELECT DISTINCT
                    dp.dopstatus as status,
                    m.muncod,
                    CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END AS uf,
                    CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE m.mundescricao END AS entidade,
                    CASE WHEN iu.itrid = 1 THEN 'Estadual' ELSE 'Municipal' END AS esfera,
                    prp.prpid as codigo,
                    prp.prpnumeroprocesso as processo,
                    COALESCE(dp.dopdatafimvigencia, '-') as vigencia,
                    par.retornasaldoprocesso(prp.prpnumeroprocesso) as saldo,
                    mdo.mdonome AS documento,
                    CAST((SELECT dp.dopnumerodocumento FROM par.documentopar WHERE prpid = dp.prpid AND dopstatus <> 'E' ORDER BY dopid ASC LIMIT 1) AS text) AS termo,
                    (SELECT dopano FROM par.documentopar WHERE prpid = dp.prpid ORDER BY dopid ASC LIMIT 1) as dopano,
                    CASE
                        WHEN (
                            SELECT
                                count(sbaid)
                            FROM par.termocomposicao tc
                            INNER JOIN par.subacaodetalhe sd ON sd.sbdid = tc.sbdid
                            WHERE tc.dopid = dp.dopid
                            ) > 0 THEN 'Sim'
                        ELSE 'Não'
                    END as subacao,
                    CASE WHEN EXISTS( SELECT 1 FROM temp_documentoparreprogramacao WHERE dopid = dp.dopid)  THEN 'Sim' ELSE 'Não' END as repro_prazo,
                    CASE WHEN EXISTS( SELECT 1 FROM temp_documentoparreprogramacaosubacao WHERE dopid = dp.dopid) THEN 'Sim' ELSE 'Não' END as repro_sub,
                    CASE WHEN dopdatafimvigenciarescisao IS NOT NULL
                        THEN 'Sim'
                        ELSE 'Não'
                    END as rescisao,
                    'SUBAÇÃO' as tipo
                FROM par.processopar prp
                INNER JOIN  par.instrumentounidade iu on iu.inuid = prp.inuid
                left JOIN {$this->stNomeTabela} dp on dp.prpid = prp.prpid and dp.dopstatus = 'A'
                left JOIN  par.modelosdocumentos mdo ON mdo.mdoid = dp.mdoid
                LEFT JOIN  territorios.municipio m ON m.muncod = iu.muncod";
        return ($booSQL) ? $sql : $this->carregar($sql);
    }

    /**
     * @TODO Verificar campos
     */
    public function relatorioTermoCompromissoPac($arrPost = array(), $booSQL = FALSE) {
        $sql = "SELECT DISTINCT
                   CAST(ter.terstatus AS character(1)) as status,
                    m.muncod,
                    CASE WHEN pro.estuf IS NOT NULL THEN pro.estuf ELSE m.estuf END AS uf,
                    CASE WHEN pro.muncod IS NOT NULL THEN m.mundescricao ELSE pro.estuf END AS entidade,
                    CASE WHEN pro.estuf IS NOT NULL THEN 'Estadual' ELSE 'Municipal' END AS esfera,
        			pro.proid as codigo,
                    pro.pronumeroprocesso as processo,
                    to_char(ter.terdatafimvigencia,'DD/MM/YYYY') as vigencia,
                    par.retornasaldoprocesso(pro.pronumeroprocesso) as saldo,
                    'Termo de Compromisso' AS documento,
                    (SELECT par.retornanumerotermopac(ter.proid)) AS termo,
                    ter.terano as dopano,
                    'Não' as subacao,
                    CASE
                        WHEN
                            (SELECT
                                COUNT(pop.preid)
                            FROM par.processoobraspaccomposicao popc
                            LEFT JOIN obras.preobraprorrogacao pop ON pop.preid = popc.preid
                            WHERE popc.proid = ter.proid) > 0 THEN 'Sim'
                        ELSE 'Não'
                    END as repro_prazo,
                    'Não' as repro_sub,
                    CASE WHEN terdatafimrescisao IS NOT NULL
                        THEN 'Sim'
                        ELSE 'Não'
                    END as rescisao,
                    'PAC' as tipo
                FROM par.processoobra pro
                LEFT JOIN  territorios.municipio m ON m.muncod = pro.muncod
                left JOIN par.termocompromissopac ter on ter.proid = pro.proid and ter.terstatus = 'A'
                LEFT JOIN  territorios.estado e ON e.estuf = pro.estuf";
        return ($booSQL) ? $sql : $this->carregar($sql);
    }

    public function relatorioTermoCompromissoPar($arrPost = array(), $booSQL = FALSE) {
        $sql = "SELECT DISTINCT
                    dp.dopstatus as status,
                    m.muncod,
                    CASE WHEN pro.estuf IS NOT NULL THEN pro.estuf ELSE m.estuf END AS uf,
                    CASE WHEN pro.muncod IS NOT NULL THEN m.mundescricao ELSE pro.estuf END AS entidade,
                    CASE WHEN pro.estuf IS NOT NULL THEN 'Estadual' ELSE 'Municipal' END AS esfera,
                    pro.proid as codigo,
                    pro.pronumeroprocesso as processo,
                    ( SELECT par.retornavigenciaobrapar(dp.proid) ) as vigencia,
                    par.retornasaldoprocesso(pro.pronumeroprocesso) as saldo,
                    mdo.mdonome AS documento,
                    CAST((SELECT dp.dopnumerodocumento FROM par.documentopar WHERE proid = dp.proid AND dopstatus <> 'E' ORDER BY dopid ASC LIMIT 1) AS text) AS termo,
                    (SELECT dopano FROM par.documentopar WHERE proid = dp.proid ORDER BY dopid ASC LIMIT 1) as dopano,
                    CASE
                        WHEN (
                            SELECT
                                count(sd.sbaid)
                            FROM par.termocomposicao tc
                            INNER JOIN par.subacaoobra so on so.preid = tc.preid
                            INNER JOIN par.subacaodetalhe sd on sd.sbaid = so.sbaid
                            WHERE tc.dopid = dp.dopid
                            ) > 0 THEN 'Sim'
                        ELSE 'Não'
                    END as subacao,
                    CASE WHEN EXISTS( SELECT 1 FROM temp_documentoparreprogramacao WHERE dopid = dp.dopid)  THEN 'Sim' ELSE 'Não' END as repro_prazo,
                    CASE WHEN EXISTS( SELECT 1 FROM temp_documentoparreprogramacaosubacao WHERE dopid = dp.dopid) THEN 'Sim' ELSE 'Não' END as repro_sub,
                    CASE WHEN dopdatafimvigenciarescisao IS NOT NULL
                        THEN 'Sim'
                        ELSE 'Não'
                    END as rescisao,
                    'PAR' as tipo
                FROM par.processoobraspar pro
                left JOIN {$this->stNomeTabela} dp on dp.proid = pro.proid and dp.dopstatus = 'A'
                left JOIN  par.modelosdocumentos mdo ON mdo.mdoid = dp.mdoid
                LEFT JOIN  territorios.municipio m ON m.muncod = pro.muncod
                LEFT JOIN  territorios.estado e ON e.estuf = pro.estuf
                ORDER BY
                    uf, entidade, documento";
        return ($booSQL) ? $sql : $this->carregar($sql);
    }

    public function regerarTermo(array $arrDados) {
        try {
            #dopid
            $dopid = (int) $arrDados['dopid'];

            /*A Excessão acontece devido mudanca de requisito, onde previa o bloqueio da funcionalidade de Reformular Termo de Compromisso
            enquanto houvesse uma Solicitacao de Prazo - Termo em Aberto. Apos remover essa regra o sistema mantem dopids Inativo na tabela par.solicitacaoprorrogacaoprazoobra
            Ocasionando a consulta para resgatar o tero atual ativo.
            */
            if (!empty($arrDados['gerarTermoSolicitacao'])) {
                $dopid = $this->retornarDopidAtivo($arrDados['proid']);
            }

            #Retorna Todas as Informacoes do DocumentoPar
            $arrDadosDoc = reset($this->recuperarTodos('*', array("dopid = {$dopid}", "dopstatus = 'A'")));

            #Verifica se possui um processo e verifica empenho Sigef
            $proid = (int) $arrDadosDoc['proid'];

            if ($proid) {
            	include_once APPRAIZ .'par/classes/modelo/ProcessoObrasPar.class.inc';
            	include_once APPRAIZ .'includes/classes/Modelo.class.inc';
                $processoObrasPar = new ProcessoObrasPar();
                $arrProcessoObraPar = $processoObrasPar->carregarPorId($proid);
                verificaEmpenhoSigef($arrProcessoObraPar->pronumeroprocesso, 'PAR');
            }

            #Se o Botão for Reformular, Devera ser setado o Termo de Compromisso - Reformulado = 103 3 o Modelo de documento 85.
            #Solicitante Thiago: 25/09 (Sexta-feira final de tarde) - Jair Foro
            $tipoRegera = $arrDados['tipoRegera'];
            if ($tipoRegera == 'reformular' && $arrDados['booEmenda'] == 't') {
                $arrDadosDoc['mdoid'] = ModelosDocumentos::MODELO_EMENDA_2015;
            }

            #Verifica se possui modelo e ex-oficio
            $mdoid = (int) $arrDadosDoc['mdoid'];
            if ($mdoid) {
                $modeloDocumento = new ModelosDocumentos();
                $arrModelo = reset($modeloDocumento->recuperarTodos('*', array("mdoid = {$mdoid}", "mdostatus = 'A'")));
                $tpdcod = $arrModelo['tpdcod'];
                $mdoconteudo = $arrModelo['mdoconteudo'];

                if (($arrModelo['mdodocumentoex'] == 't') && ($tipoRegera == 'reformular' || $tipoRegera == 'prorrogacaoVigencia')) {
                    $arrModeloPai = reset($modeloDocumento->recuperarTodos('*', array("mdoid = {$arrModelo['mdotipovinculado']}", "mdostatus = 'A'")));
                    $mdoconteudo = $arrModeloPai['mdoconteudo'];
                    $tpdcod = (int) $arrModeloPai['tpdcod'];
                    $arrDadosDoc['mdoid'] = $arrModelo['mdotipovinculado'];
                    $mdoid = $arrModelo['mdotipovinculado'];
                }
            }

            #Retorna as obras que composicao do Documento
            $termoComposicao = new TermoComposicao();
            $arrTermoComposicao = $termoComposicao->recuperarTodos('preid', array("dopid = {$dopid}"));

            $arrPreid = array();
            foreach ($arrTermoComposicao as $preid) {
                $arrPreid[] = $preid['preid'];
            }

            $dopdatafimvigencia = $this->dataFinalVigencia($proid);

            #Regra pra tramitar Solicitacao de Prorrogacao de Prazo - Termo de Aguardando Geracao de Termo para Solicitacao de Prazo Finalizada.
            if ($tipoRegera == 'reformular') {
                include_once APPRAIZ . 'includes/workflow.php';
                $solProrrogacaoPrazo = (new SolicitacaoProrrogacaoPrazoObra())->buscarSituacaoProrrogacaoTermo($proid, 'OBRAS', $_REQUEST['sppid']);
                if ($solProrrogacaoPrazo['esdid'] == ESDID_AGUARDANDO_GERACAO_TERMO) {
                    wf_alterarEstado($solProrrogacaoPrazo['docid'], 4122, '', array('cache_estado_atual'=>0), array('commit' => false));
                    $dopdatafimvigencia = $solProrrogacaoPrazo['sppdtfimaprovada'];
                }
            }

            #Armazena em sessao para utilizar nas macros da minuta.
            $_SESSION['par']['cronogramainicial'] = $this->dataInicialVigencia($proid);
            $_SESSION['par']['cronogramaFinal'] = $dopdatafimvigencia;

            #Parametros para alterar dos dados da Minuta.
            $post = array(
                'mdoid' => $mdoid,
                'proid' => $proid,
                'dopdatainiciovigencia' => $this->dataInicialVigencia($proid),
                'dopdatafimvigencia' => $dopdatafimvigencia,
                'tpdcod' => $tpdcod,
                'dopid' => $dopid,
                'chk' => $arrPreid,
                'tipoRegera' => $tipoRegera
            );

            #texto de modelo para alteracao das informacoes
            $doptexto = alteraMacrosMinutaObras($mdoconteudo, $proid, $post);

            $arrDadosDoc['regerar'] = true;
            $post['regerar'] = true;

            if ($_SESSION['par']['totalVLR']) {
                $arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
                unset($_SESSION['par']['totalVLR']);
            }

            #Novo documento gerado
            $dopidNovo = salvarDadosMinutaObras($post, $doptexto, $dopid);

            #Insere a composicao de obras para o novo documento.
            foreach ($arrPreid as $preid) {
                $arrInsert = array(
                    'dopid' => $dopidNovo,
                    'preid' => $preid
                );
                $termoComposicao->inserirComposicao($arrInsert);
            }

            # Muda o Status da Obra para Aprovada caso for prorrogacao de vigencia
            if ($tipoRegera == 'prorrogacaoVigencia') {

                $sqlPreoObra = "SELECT
                                pop.preid
                        FROM obras.preobraprorrogacao pop
                        INNER JOIN par.termocomposicao tc ON tc.preid = pop.preid
                        WHERE tc.dopid = {$dopid} AND pop.popstatus = 'A' AND pop.popvalidacao = 't'
                        ORDER BY popid ASC";

                $arrPreObra = $this->carregar($sqlPreoObra);
                if ($arrPreObra) {
                    foreach ($arrPreObra as $obra) {
                        $docid = $this->pegaUm("SELECT docid FROM obras.preobra WHERE preid='" . $obra['preid'] . "'");

                        $sql = "SELECT tooid FROM obras.preobra WHERE preid = {$obra['preid']}";
                        $tooid = $this->pegaUm($sql);

                        // Se a obra for PAC
                        $esdidorigem = WF_PAR_OBRA_AGUARDANDO_PRORROGACAO;
                        if ($tooid == 1) {
                            $esdidorigem = WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO;
                        }

                        $esdiddestino = $this->pegaUm("SELECT esdidorigem FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid=" . $obra ['preid']);
                        $aedid = $this->pegaUm("SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem = " . $esdidorigem . " AND esdiddestino = " . $esdiddestino);

                        if ($aedid == '' && $esdidorigem != $esdiddestino) {

                            $sql = "SELECT esddsc FROM workflow.estadodocumento WHERE esdid = $esdiddestino";
                            $esddsc = $this->pegaUm($sql);

                            $sql = "INSERT INTO workflow.acaoestadodoc
		                        		(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada, esdsncomentario, aedvisivel, aedcodicaonegativa)
		                        	VALUES
		                        		($esdidorigem, $esdiddestino, 'Enviar para $esddsc', 'A', 'Enviada para $esddsc', true, false, false )
		                        	RETURNING
		                        		aedid";

                            $aedid = $this->pegaUm($sql);
                        }

                        wf_alterarEstado($docid, $aedid, 'teste', $d = array(
                            'preid' => $obra ['preid']
                        ));
                    }
                }
            }

            $this->commit();

            enviaEmailNovoTermoObrasPAR($dopid, 'regerar');

            unset($_SESSION['par']['cronogramainicial']);
            unset($_SESSION['par']['cronogramaFinal']);

            return $dopidNovo;
        } catch (Exception $exception) {
            $this->rollback();
            return false;
        }
    }

    public function dataInicialVigencia($proid) {

        $sql = "WITH RECURSIVE documentopar_tree AS (
            SELECT
                dopid,
                dopidpai,
                dopdatainiciovigencia,
                dopstatus
            FROM par.documentopar WHERE proid = {$proid} AND dopstatus <> 'E'

            UNION ALL

            SELECT
                dop.dopid,
                dop.dopidpai,
                dop.dopdatainiciovigencia,
                dop.dopstatus
            FROM par.documentopar dop
            INNER JOIN documentopar_tree dopt ON dop.dopidpai = dopt.dopid
            WHERE dop.dopid IN (
                SELECT d.dopidpai FROM par.documentopar d WHERE proid = {$proid} AND dopstatus <> 'E'
            )
        )
        SELECT dopdatainiciovigencia FROM documentopar_tree ORDER BY dopid LIMIT 1";
        return $this->pegaUm($sql);
    }

    public function dataFinalVigencia($proid) {
        $dtFimVigencia = $this->pegaUm("SELECT par.retornavigenciaobrapar({$proid})");
        return $dtFimVigencia;
    }

    public function dataFinalVigenciaDopid($dopid) {
        $dtFimVigencia = $this->pegaUm("SELECT dopdatafimvigencia FROM par.documentopar WHERE dopid = {$dopid}");
        return $dtFimVigencia;
    }

    public function retornarDopidAtivo($proid) {
        $dopid = $this->pegaUm("SELECT max(dopid) FROM par.documentopar where proid = {$proid} AND dopstatus = 'A'");
        return $dopid;
    }
}

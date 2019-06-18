<?php

class Par3_Model_Pendencias extends Modelo
{
	/*
	 * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.vm_total_pendencias";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
	  	'obrid' => null,
	  	'preid' => null,
	  	'predescricao' => null,
	  	'estuf' => null,
	  	'descricao' => null,
	  	'muncod' => null,
	  	'descricao' => null,
	  	'situacao' => null,
	  	'pendencia' => null
    );

    public function __construct()
    {

    }

    /**
     * Função montarFiltroPAR
     * - monta a Filtro para a lista de documentos PAR.
     *
     * @return escreve a lista.
     *
     */
    public function montarFiltroPendencias($arrPost){

    	$where = array();

    	if ($_SESSION['par']['estuf'] != '') $where[] = "estuf = '{$_SESSION['par']['estuf']}'";

    	if ($_SESSION['par']['muncod'] != '') $where[] = "muncod = '{$_SESSION['par']['muncod']}'";

    	if ($_SESSION['par']['obrid'] != '') $where[] = "obrid = '{$_SESSION['par']['obrid']}'";

    	if ($_SESSION['par']['preid'] != '') $where[] = "dopstatus = '{$_SESSION['par']['preid']}'";

    	return $where;

    }//end motnarFiltroPAR()

	public function recuperarPendenciasEntidade($inuid, $estuf = null, $muncod = null, $itrid = null)
	{
	   // $where = Array("pendencia = 'par'");

		if ($itrid === '2'){
		  //  $where[] = "instrumentounidade.muncod = '$muncod'";
		    $esfera = 'M';
		}else{
		   // $where[] = "instrumentounidade.estuf = '$estuf'";
		    $esfera = 'E';
		}

	    $where[] = "empesfera = '$esfera'";
	    $where[] = "instrumentounidade.inuid = {$inuid}";

//
//		$sql = "SELECT obrid,preid,predescricao,estuf,descricao,muncod,descricao,situacao,pendencia
//				FROM obras2.vm_total_pendencias
//				WHERE ".implode(' AND ', $where)."
//				ORDER BY pendencia";

		$sql = "select *, 'par' as pendencia,esddsc as tipopendencia from obras2.v_pendencia_obras_base as mfv
                      inner join territorios.estado as estuf on estuf.estuf = mfv.estuf
                      left join territorios.municipio as municipio on municipio.muncod = mfv.muncod
                      inner join par3.instrumentounidade as instrumentounidade on instrumentounidade.estuf = estuf.estuf and instrumentounidade.muncod = municipio.muncod
                      WHERE ".implode(' AND ', $where)." ORDER BY mfv.obrid";
		//ver($sql);
		$dados = $this->carregar($sql,0,600000);
		return $dados;
	}

	public function recuperaProcessosRegraMonitoramento2010($inuid)
	{
	    $entidade = new Par3_Model_InstrumentoUnidade($inuid);

	    if($entidade->itrid = '2'){
	        $where = "muncod = '{$entidade->muncod}'";
	    }else{
	        $where = "estuf = '{$entidade->estuf}'";
	    }
	    
		$inuid_par = $entidade->recuperarInuidPar($inuid);

		$sql = "SELECT
                	sbaid,
                	SUM(valor_validado_ano) as valor_validado,
                	SUM(valor_pago_ano) as valor_pago,
                	NOT(SUM(contratos_item_ano) = SUM(item_ano)) as item_sem_contrato,
                	NOT(SUM(item_contrato_recebido_ano) = SUM(contratos_item_ano)) as item_contrato_sem_qtd_recebido,
                	SUM(item_contrato_recebido_detalhado_ano) = 0 as item_sem_detalhar,
                	(SUM(item_contrato_recebido_ano) > SUM(item_contrato_recebido_detalhado_ano)) as item_detalhado_menor_recebido,
                	NOT(SUM(item_com_nota_ano) = SUM(item_ano)) as item_sem_nota,
                	SUM(contrato_diligencia_ano) > 0 as contrato_diligencia,
                	SUM(nota_diligencia_ano) > 0 as nota_diligencia
                FROM
                    par.vm_temporaria_pega_pendencias_existentes
                WHERE
                    $where
                GROUP BY
                	sbaid
                HAVING
                	SUM(valor_validado_ano) = SUM(valor_pago_ano)";

		$arrDados = $this->carregar($sql,0,600000);
		$arrDados = is_array($arrDados) ? $arrDados : array();

		$arrPendencias = Array(
			'item_sem_contrato'               => 'Há item(ns) sem contrato anexado, na aba Contratos. Para finalizar o processo, todos os itens devem ter seus respectivos contratos anexados, ou o item deve ser cancelado.',
		    'item_contrato_sem_qtd_recebido'  => 'Há item(ns) com contrato(s) anexado(a) para o(s)  qual(is) não foi informada a "Quantidade recebida", na aba "Monitoramento".',
		    'item_sem_detalhar'               => 'Há item(ns) que não foram detalhados na aba "Detalhamento do Serviço/Itens"',
		    'item_detalhado_menor_recebido'   => 'Há item(ns) que a quantidade detalhada do item/serviço é menor que a recebida na aba de "Detalhamento do Serviço/Itens"',
		    'item_sem_nota'                   => 'Há item(ns) sem nota fiscal anexada, na aba Notas Fiscais. Para finalizar o processo, todos os itens devem ter suas respectivas notas fiscais anexadas, ou o item deve ser cancelado.',
		    'contrato_diligencia'             => 'Existem Contratos em Diligência.',
		    'nota_diligencia'                 => 'Existem Notais Fiscais em Diligência.',
		);

		$arrRetorno = array();
		$arrRetorno['dados'] = $arrDados;
		foreach($arrDados as $dados){
		    foreach($dados as $indice => $dado){
		        if($dado == 't'){
                    $arrRetorno['pendencias'][$indice] = $arrPendencias[$indice];
		        }
		    }
		}

		return $arrRetorno;
	}

	public function recuperaProcessosRegraMonitoramento2010Termos($inuid)
	{
	    $entidade = new Par3_Model_InstrumentoUnidade($inuid);

		$inuid_par = $entidade->recuperarInuidPar($inuid);

		$qtd_termos = $this->recuperaQtdTermosNaoAssinados5dias($inuid_par);

		if($qtd_termos > 0){
            $arrRetorno['pendencias']['termos_n_assinados'] = 'Detalhar temos com pendência';
		}

		return $arrRetorno;
	}

	public function recuperarTiposPendencia(){
		$sql = "select
				distinct pendencia
				from obras2.vm_total_pendencias where pendencia = 'par' order by pendencia";

		$dados = $this->carregarColuna($sql);

		return $dados;
	}

    public function existePendenciaPorTipo($pendencia, $inuid, $estuf = null, $muncod = null, $itrid = null){
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
        $instrumentoUnidade = $modelInstrumentoUnidade->getDadosUnidade($inuid);
        if(!$instrumentoUnidade){
            return array();
        }
        $unidade = '';
        if($instrumentoUnidade['itrid'] == 1){ $unidade = 'E';}elseif($instrumentoUnidade['itrid'] == 2){$unidade = 'M';}elseif ($instrumentoUnidade['itrid'] == 3){$unidade = 'F';}

        $where = " estuf = '{$GLOBALS['estuf']}' AND empesfera = '{$unidade}'";
        if ($itrid === '2') $where = " muncod = '".$GLOBALS['muncod']."' AND empesfera = '{$unidade}'";



        $sql = "select
				obrid
				from obras2.vm_total_pendencias
				WHERE ".$where." AND pendencia = '$pendencia' order by pendencia ";
        //var_dump($sql);
        $dados = $this->pegaUm($sql);

        return $dados;
    }

	public function recuperaCNPJEntidade($inuid, $muncod = null){
        if(!$inuid || $inuid == ''){
            return array();
        }
		$sql = "SELECT iue.iuecnpj
		FROM par.instrumentounidade iu
		inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		WHERE
		iu.inuid = {$inuid}
		and iue.iuestatus = 'A'
		and iue.iuedefault = true";

		$dados = $this->pegaUm($sql);

		return $dados;
	}

	public function recuperaProcessosRegraPagamento($inuid, $estuf = null, $muncod = null, $itrid = null,$retornaQuery = false)
	{
	    $where = array("estuf = '$estuf'");

		if ($itrid === '2') $where[] = "muncod = '$muncod'";

		$sql = "SELECT * FROM par3.v_pendencia_regras_pagamento WHERE ".implode(' AND ', $where);

		if($retornaQuery){
			return $sql; exit;
		}

		$dados = $this->carregar($sql,0,600000);

		return $dados;
	}

	public function recuperaProcessosRegraSaldoConta($inuid, $estuf = null, $muncod = null, $itrid = null,$retornaQuery = false)
	{
	    $where = array();
	    
		if ($itrid === '2'){
            $where[] = "muncod = '$muncod'";
		}else{
		    $where[] = "estuf = '$estuf'";
		}

		$sql = "SELECT * FROM par3.v_pendencia_regras_saldo_conta WHERE ".implode(' AND ', $where);

		if($retornaQuery){
			return $sql; exit;
		}

		$dados = $this->carregar($sql);

		return $dados;
	}

	public function recuperaArquivoSasePne($inuid, $estuf = null, $muncod = null, $itrid = null){
		if ($itrid == '2') {
			$sql = "select assleipne from sase.assessoramento where muncod = '{$muncod}'";
		} else {
			$sql = "select aseleipne from sase.assessoramentoestado where estuf = '{$estuf}'";
		}

		$arqid = $this->pegaUm($sql);
		return $arqid;
	}

	public function recuperaQtdTermosNaoAssinados5dias($inuid_par2010)
	{
	    $sql = sprintf('
	        SELECT
                qtd_termos
            FROM
                par3.v_pendencia_regras_termos_nao_assinados_5_dias
            WHERE
                inuid = %d
	        ',
            $inuid_par2010
        );

		return $this->pegaUm($sql);
	}

	public function buscaDetalheItemSemContrato($dados)
	{
	    $dados['filtro'] = 'NOT(SUM(contratos_item_ano) = SUM(item_ano))';

        $sql = $this->montaSQLPendenciarMonitoramento($dados);

        return $this->carregar($sql);
	}

	public function buscaDetalheItemContratoSqmQtdRecebido($dados)
	{
	    $dados['filtro'] = 'NOT(SUM(item_contrato_recebido_ano) = SUM(contratos_item_ano))';

        $sql = $this->montaSQLPendenciarMonitoramento($dados);

        return $this->carregar($sql);
	}

	public function buscaDetalheItemSemDetalhar($dados)
	{
	    $dados['filtro'] = "SUM(item_contrato_recebido_detalhado_ano) = 0 AND UPPER(ico.icodescricao) ILIKE '%ÔNIBUS%'";

        $sql = $this->montaSQLPendenciarMonitoramento($dados);

        return $this->carregar($sql);
	}

	public function buscaDetalheItemDetalhadoMenosRecebido($dados)
	{
	    $dados['filtro'] = '(SUM(item_contrato_recebido_ano) > SUM(item_contrato_recebido_detalhado_ano))';

        $sql = $this->montaSQLPendenciarMonitoramento($dados);

        return $this->carregar($sql);
	}

	public function buscaDetalheItemSemNota($dados)
	{
	    $dados['filtro'] = 'NOT(SUM(item_com_nota_ano) = SUM(item_ano))';

        $sql = $this->montaSQLPendenciarMonitoramento($dados);

        return $this->carregar($sql);
	}

	public function buscaDetalheContratoDiligencia($dados)
	{
	    $dados['filtro'] = 'SUM(contrato_diligencia_ano) > 0';

        $sql = $this->montaSQLPendenciarMonitoramento($dados);

        return $this->carregar($sql);
	}

	public function buscaDetalheNotaDiligencia($dados)
	{
	    $dados['filtro'] = 'SUM(nota_diligencia_ano) > 0';

        $sql = $this->montaSQLPendenciarMonitoramento($dados);

        return $this->carregar($sql);
	}

	public function montaSQLPendenciarMonitoramento($dados)
	{
	    $sql = "SELECT DISTINCT
                	substring(prp.prpnumeroprocesso from 1 for 5)||'.'||
    		   		substring(prp.prpnumeroprocesso from 6 for 6)||'/'||
    		   		substring(prp.prpnumeroprocesso from 12 for 4)||'-'||
    		   		substring(prp.prpnumeroprocesso from 16 for 2) as prpnumeroprocesso,
                	dop.dopnumerodocumento,
                	mdo.mdonome,
                	dop.dopdatafimvigencia
                FROM
                    par.vm_temporaria_pega_pendencias_existentes as foo
                INNER JOIN par.termocomposicao 		tec ON tec.sbdid = foo.sbdid
                INNER JOIN par.documentopar 		dop ON dop.dopid = tec.dopid AND 
                                                           dop.dopstatus = 'A'
                INNER JOIN par.processopar 		    prp ON prp.prpid = dop.prpid
                INNER JOIN par.modelosdocumentos 	mdo ON mdo.mdoid = dop.mdoid
                INNER JOIN par.subacaoitenscomposicao	ico ON ico.sbaid = foo.sbaid AND 
                                                               ico.icoano = foo.sbdano AND 
                                                               ico.icostatus = 'A'
                WHERE
                    foo.inuid_par = {$dados['inuid_par']}
                GROUP BY
                	prp.prpnumeroprocesso,
                	dop.dopnumerodocumento,
                	mdo.mdonome,
                	dop.dopdatafimvigencia,
                	ico.icodescricao
                HAVING
                	SUM(valor_validado_ano) = SUM(valor_pago_ano)
                	AND ".$dados['filtro'];
// ver($sql, d);
	    return $sql;
	}

	public function buscaDetalheTermosNAssinados($dados)
	{
	    $sql = "SELECT
                	substring(prp.prpnumeroprocesso from 1 for 5)||'.'||
    		   		substring(prp.prpnumeroprocesso from 6 for 6)||'/'||
    		   		substring(prp.prpnumeroprocesso from 12 for 4)||'-'||
    		   		substring(prp.prpnumeroprocesso from 16 for 2) as prpnumeroprocesso,
                	dop.dopnumerodocumento,
                	mdo.mdonome,
                	dop.dopdatafimvigencia
                FROM
                	par.processopar prp
                INNER JOIN par.documentopar         dop ON dop.prpid = prp.prpid
                INNER JOIN par.modelosdocumentos 	mdo ON mdo.mdoid = dop.mdoid
                WHERE
                	inuid = {$dados['inuid_par']}
                	AND dop.mdoid NOT IN (SELECT mdoid FROM par.modelosdocumentos WHERE mdonome ilike '%EX%')
                	AND dop.dopid NOT IN (SELECT dopid FROM par.documentoparvalidacao)
                	AND (now()::date - dopdatainclusao::date) > 5

                	AND
                        case when length(dop.dopdatafimvigencia) < 10 then
                            ((('01/'||dop.dopdatafimvigencia)::date + INTERVAL '1 month' )::date -1 ) >= now()
                        else
                            (((dop.dopdatafimvigencia)::date + INTERVAL '1 month' )::date -1 ) >= now()
                        end

                	AND prp.prpstatus = 'A'
                	AND dop.dopstatus = 'A'";

        return $this->carregar($sql);
	}
    public function existeRestricoesPendenciaInstrumentoUnidadeById($inuid){
        if(isset($inuid) && $inuid != null){
            $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
            $instrumentoUnidade = $modelInstrumentoUnidade->getDadosUnidade($inuid);
            if($instrumentoUnidade['itrid'] == 1){

            }
        }
    }



    /**
     * @author <leo.oliveira@cast.com.br>
     * @description < Descobre se a unidade é do tipo 1(estado) ou 2(municipio), retorna array das obras que possuem restrições tuleap hstória 12936>
     * @param $inuid
     * @return array;
     */
    private function getObrasRestricoesInstrumentoUnidadeById($inuid){
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
        $instrumentoUnidade = $modelInstrumentoUnidade->getDadosUnidade($inuid);
        $unidade = '';

        if($instrumentoUnidade['itrid'] == 1){ $unidade = 'E';}elseif($instrumentoUnidade['itrid'] == 2){$unidade = 'M';}elseif ($instrumentoUnidade['itrid'] == 3){$unidade = 'F';}



        $sql = '';
        if(is_array($instrumentoUnidade)){
            //se for uma unidade do tipo  Estado
            if ($instrumentoUnidade['itrid'] == 1) {
                $sql = "select DISTINCT obra.obrid,obra.obrnome,obra.obrdsc from obras2.obras as obra
                                    inner join obras2.restricao as restricao on obra.obrid = restricao.obrid AND restricao.rststatus = 'A'  AND restricao.rstitem = 'R'
                                    inner JOIN workflow.documento d2 ON d2.docid = restricao.docid AND d2.esdid NOT IN (1142, 1497)
                                    inner join entidade.endereco as endereco on endereco.endid = obra.endid
                                    inner join par3.instrumentounidade as instrumentounidade on instrumentounidade.estuf = endereco.estuf and instrumentounidade.muncod isnull
                                    inner join obras2.empreendimento as empreendimento on empreendimento.empid = obra.empid
                                    where
                                    instrumentounidade.inuid = {$inuid}
                                    and
                                    empreendimento.empesfera IN('{$unidade}')
                                    order by obra.obrid ASC";
            } //se for uma unidade do tipo  Municipio
            elseif ($instrumentoUnidade['itrid'] == 2) {
                $sql = "select DISTINCT obra.* from obras2.obras as obra
                                        inner join obras2.restricao as restricao on obra.obrid = restricao.obrid AND restricao.rststatus = 'A'  AND restricao.rstitem = 'R'
                                        inner JOIN workflow.documento d2 ON d2.docid = restricao.docid AND d2.esdid NOT IN (1142, 1497)
                                        inner join entidade.endereco as endereco on endereco.endid = obra.endid
                                        /*inner join territorios.municipio as municipio on municipio.estuf = endereco.estuf  and endereco.muncod = municipio.muncod*/
                                        left join par3.instrumentounidade as instrumentounidade on instrumentounidade.estuf = endereco.estuf and endereco.muncod = instrumentounidade.muncod
                                        inner join obras2.empreendimento as empreendimento on empreendimento.empid = obra.empid
                                        where
                                        restricao.rststatus = 'A'
                                        and
                                        restricao.rstitem = 'R'
                                        and
                                        instrumentounidade.inuid = {$inuid} -- id da unidade
                                        and
                                        empreendimento.empesfera IN('{$unidade}')";
            }
            $result = $this->carregar($sql);
            return $result;
        }else{
            return array();
        }


    }

    /**
     * @author Leo Kenzley <leo.oliveira@grupocast.com.br>
     * @param $inuid
     * @return array|mixed|NULL
     * @description <Esta função retorna as pendencias das obras, utilizando o id instrumentounidade como filtro $inuid>
     */
    public function getPendenciasObrasIntrumentoUnidadeById($inuid) {

        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
        $instrumentoUnidade = $modelInstrumentoUnidade->getDadosUnidade($inuid);
        $unidade = '';

        if($instrumentoUnidade['itrid'] == 1){ $unidade = 'E';}elseif($instrumentoUnidade['itrid'] == 2){$unidade = 'M';}elseif ($instrumentoUnidade['itrid'] == 3){$unidade = 'F';}

        $sql = "SELECT *
                FROM obras2.vm_total_pendencias as vm
                inner join par3.instrumentounidade as instrumentounidade on instrumentounidade.muncod =  vm.muncod
                left join territorios.estado as estado on estado.estuf = instrumentounidade.estuf
				  WHERE pendencia = 'par' AND empesfera = '{$unidade}' and instrumentounidade.inuid = {$inuid}
				  ORDER BY pendencia";
        //ver($sql);
        $result = $this->carregar($sql,0,600000);
        return $result;
    }

    public function sqlPendenciasGrafico($arrPost = null)
    {
        $limitVal  = $arrPost['limit']?" LIMIT   {$arrPost['limit']} ":'';
        $offsetVal = $arrPost['offset']?" OFFSET {$arrPost['offset']} ":'';
        $where = ' WHERE 1=1 ';
        $where .= $arrPost['descricao'] ? " and descricao = '{$arrPost['descricao']}' ": '';
        $where .= $arrPost['inuid'] ? " AND inuid = {$arrPost['inuid']} ":"";
        $where .= $arrPost['inudescricao'] ? " AND inudescricao ilike '%{$arrPost['inudescricao']}%' ":"";

        $column = $arrPost['detalhar']? '*':'count(*) as valor';
        $group = 'group by descricao';
        $columnEntidade = '';
        $joinentidade = '';

        $joinentidade = 'inner join par3.instrumentounidade inu on inu.inuid = vm.inuid';
        if($arrPost['detalhar']) {
            $group = '';
            $columnEntidade = 'inu.inudescricao||\' - \'||inu.estuf as entidade,inu.inuid,inu.inudescricao,inu.itrid,inu.estuf,inu.muncod,';
        }

        $where  .= $arrPost['itrid'] && $arrPost['itrid'] != 0 ?($arrPost['itrid'] == 1 ? "AND itrid in(1,3)" :"AND itrid = {$arrPost['itrid']}"): "";

        if($arrPost['muncod'] && count($arrPost['muncod']) != 0 ) {
            $arrMuncod = tratarArrayParaMultiSelect($arrPost['muncod']);
            if(count($arrMuncod) != 0) {
                $where .= " and muncod::integer in (".implode(',',$arrMuncod).")";
            }
        }

        if($arrPost['estuf'] && count($arrPost['estuf']) != 0 ) {
            $arrEstuf = implode(',',array_map(function($estuf){return (string)"'".$estuf."'";},$arrPost['estuf']));
            $where .= $arrPost['estuf'] ? " and estuf in ({$arrEstuf})":'';
        }

        $arrPendencias = [
            1 => 'cae',
            2 => 'cacs',
            3 => 'habilitacao',
            4 => 'monitoramento_par',
            5 => 'obras_par',
            6 => 'pne',
            7 => 'contas',
            8 => 'siope'
        ];

        if($arrPost['tipid']) {
            foreach($arrPost['tipid'] as $tipid) {
                if(key_exists($tipid,$arrPendencias)) {
                    $pendencias[] = $arrPendencias[$tipid];
                }
            }
            $arrPendencias = $pendencias;
        }

        if(count($arrPendencias) == 0 ){
            return '';
        }
        $implodeSemPendencia = implode(' AND ',array_map(function($tip){return ' '.$tip.' = 1 ';},$arrPendencias));
        $implodePendencia = '('.implode(' OR ',array_map(function($tip){return ' '.$tip.' = 3 ';},$arrPendencias)).')';
        $implodePendencia .= ' AND '.'('.implode(' AND ',array_map(function($tip){return ' '.$tip.' <> 2 ';},$arrPendencias)).')';

        $sql = "
        with regra_bloqueio as (
            select
                p.tipid,
                (case when
                    b.rebplanejamento = true or
                    b.rebempenho      = true or
                    b.rebtermo        = true or
                    b.rebpagamento
                then
                    true 
                else
                    false
                end) as pendencia
                FROM par3.regra_bloqueio b
                INNER JOIN par3.tipos_pendencias p ON p.tipid = b.tipid
            WHERE p.tipstatus = 'A'
            ORDER BY b.rebid
        ),
        pendencias as (
            select
                {$columnEntidade}
                (case when cae = true then
                    (case when (select pendencia from regra_bloqueio where tipid = 1) = true then 2 else 3 end)
                else 1 end) as cae,
            
                (case when cacs = true then
                    (case when (select pendencia from regra_bloqueio where tipid = 2) = true then 2 else 3 end)
                else 1 end) as cacs,
            
                (case when habilitacao = true then
                    (case when (select pendencia from regra_bloqueio where tipid = 3) = true then 2 else 3 end)
                else 1 end) as habilitacao,
                
                (case when monitoramento_par = true then
                    (case when (select pendencia from regra_bloqueio where tipid = 4) = true then 2 else 3 end)
                else 1 end) as monitoramento_par,
            
                (case when obras_par = true then
                    (case when (select pendencia from regra_bloqueio where tipid = 5) = true then 2 else 3 end)
                else 1 end) as obras_par,
            
                (case when pne = true then
                    (case when (select pendencia from regra_bloqueio where tipid = 6) = true then 2 else 3 end)
                else 1 end) as pne,
            
                (case when contas = true then
                    (case when (select pendencia from regra_bloqueio where tipid = 7) = true then 2 else 3 end)
                else 1 end) as contas,
            
                (case when siope = true then
                    (case when (select pendencia from regra_bloqueio where tipid = 8) = true then 2 else 3 end)
                else 1 end) as siope
            from 
            par3.vm_relatorio_quantitativo_pendencias vm
            {$joinentidade}
        )
        
        select 
            *,
            count(*) OVER(PARTITION BY descricao) AS full_count
        from (
                select
                    (case when 
                        {$implodeSemPendencia}
                    then 'Sem Pendência'
                    else
                        (case when 
                            {$implodePendencia}
                        then 'Alerta'
                        else
                            'Pendência'
                        end) 
                    end) as descricao,
                    {$column}
                from pendencias
                {$group}
        ) as pendencia
        {$where}
        order by pendencia.descricao desc
        ";

        if($arrPost['detalhar']) {
            $sql = $sql.$offsetVal.$limitVal;
            $res = $this->carregar($sql);
            if (count($res) == 0) {
                $sql = $sql.$limitVal;
                $res = $this->carregar($sql);
            }
            $retorno['resultset'] = $res;
            $retorno['sql']       = $sql;
            return $retorno;
        }

        return $sql;
    }

    public function sqlSituacoesPar($arParams = null)
    {
        $where  = ' WHERE 1=1 ';
        $where .= $arParams['esdid'] ?" AND esdid in (".implode(',',array_map(function($tipo){ return "'$tipo'"; },$arParams['esdid'])).") ":'';

        $whereSituacoes  = ' ';
        $whereSituacoes .= $arParams['inuid']?" AND ana_obr.inuid = {$arParams['inuid']}":'';
        $whereSituacoes .= $arParams['ano']? " AND ana_obr.ano::numeric =  {$arParams['ano']}":'';

        $whereInta = " WHERE anastatus = 'A' ";
        if($arParams) {
            if (is_array($arParams['intaidsituacoes'])) {
                $arInta = implode(',',$arParams['intaidsituacoes']);
                $whereInta .= " AND intaid in ({$arInta}) ";
            }
        }
        $sql = "
        select * from
        (select
            es.esddsc as situacao,
            count(es.esddsc) as quantidade,
            (CASE  WHEN doc.tpdid = 304
                THEN 'OBRA'
                ELSE 'PAR'
            end) as tipo,
            doc.esdid
        FROM workflow.documento doc
        INNER JOIN workflow.estadodocumento es on es.esdid = doc.esdid
        inner join (
            select
              docid,
              obrano as ano,
              inuid
            from par3.obra obr where obr.obrstatus = 'A'
          union all
            select
              ana.docid,
              anaano as ano,
              inuid
            from par3.analise ana
            inner join par3.iniciativa_planejamento using(inpid)
            {$whereInta}
		) as ana_obr on ana_obr.docid = doc.docid
        LEFT JOIN workflow.historicodocumento hd on hd.hstid = doc.hstid
        left join par3.obra obr on obr.docid = doc.docid
        left join par3.analise ana on ana.docid = doc.docid
        WHERE doc.tpdid in (304,310)
        {$whereSituacoes}
        GROUP BY es.esddsc,  doc.tpdid, doc.esdid
        ORDER BY tipo, doc.esdid
        ) as situacoes
        {$where}
        ";
        return $sql;
    }

    public function listarSituacoesPorTpdid($tpdid)
    {
        $where = "where tpdid = {$tpdid}";
        if(is_array($tpdid)) {
            $arTpdid = array_filter($tpdid);
             $where = "where tpdid in (".implode(',',$arTpdid).")";
        }
        $sql = "
        select  esdid as codigo ,esddsc as descricao from workflow.estadodocumento {$where} 
        ";
        return $this->carregar($sql);
    }

    public function getPendenciaContas($inuid)
    {
        $sql = "select contas from par3.vm_relatorio_quantitativo_pendencias where inuid = {$inuid}";

        if ($this->pegaUm($sql) == 't') {
            return 1;
        } else {
            return 0;
        }
    }

    function sqlDetalharSituacoesPar($arParams)
    {
        $where  = $arParams['esdid'] ?" AND doc.esdid = {$arParams['esdid']}":'';
        $where .= $arParams['inuid'] ?" AND inp.inuid = {$arParams['inuid']}":'';
        $where .= $arParams['ano']   ?" AND a.anaano  = {$arParams['ano']}":'';
        $sql =<<<SQL
   SELECT 
     ini.iniid ||' - '|| ind.inddsc as iniciativa, 
     count (a.anaid) as quantidade
    FROM par3.analise a
        INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = a.inpid AND inp.inpstatus = 'A'
        INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid 
        INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid 
        INNER JOIN workflow.documento doc ON a.docid = doc.docid
        INNER JOIN workflow.estadodocumento es ON es.esdid = doc.esdid
    WHERE a.anastatus = 'A' 
             {$where}
    group by ind.inddsc, ini.iniid
    order by ini.iniid
SQL;
    return $sql;
    }
}
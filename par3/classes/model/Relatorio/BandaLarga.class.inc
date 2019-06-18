<?php
/**
 * Par_Model_Questionario
 *
 * @category Class
 * @package  A1
 * @author   Fellipe Esteves <fellipesantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 21-09-2015
 * @link     no link
 */

class Par3_Model_Relatorio_BandaLarga extends Modelo {
    private $perguntas = array();
    private $respostas = array();
    private $tabela;
    protected $arAtributos = array(
        'estuf' => null,
        'muncod' => null,
        'queid' => null,
    );

	function carregarInformacoesMunicipio($filtros) {
	    $where = '1 = 1 ';

	    foreach ($filtros as $filtro) {
	        $where.= $filtro;
	    }

	    $sql = "SELECT
                	iu.estuf,
                	mun.mundescricao,
                	mun.muncod,
                	iu.inuid
                FROM par3.instrumentounidade iu
                LEFT JOIN territorios.municipio mun ON mun.muncod = iu.muncod
	            WHERE {$where}
                ORDER BY
                	iu.estuf,
                	mun.mundescricao";

	    return $this->carregar($sql);
	}

	function carregarGruposPerguntasQuestionario()
	{
	    $queid = QUESTIONARIO_BANDA_LARGA;

	    $sql = "SELECT  grpid, grptitulo
	            FROM    questionario.grupopergunta
	            WHERE   queid = {$queid}
                ORDER BY grptitulo";

	    return $this->carregar($sql);
	}

	function carregarPerguntasQuestionario($grpid)
	{
	    $sql = "SELECT p.perid, p.pertitulo, ip.itpid
	            FROM questionario.pergunta p
	            LEFT JOIN questionario.itempergunta ip ON ip.perid = p.perid
	            WHERE
                    p.grpid = {$grpid}
	                AND p.itpid IS NULL
                ORDER BY p.perordem";

	    return $this->carregar($sql);
	}

	function carregarPerguntasItemQuestionario($itpid) {
		$itens = null;

		if ($itpid)
		{
			$sql = "   SELECT p.perid, p.pertitulo, ip.itpid
		                 FROM questionario.pergunta p
		            LEFT JOIN questionario.itempergunta ip ON ip.itpid = p.itpid
		                WHERE p.itpid = {$itpid}
		                  AND p.itpid IS NOT NULL
		             ORDER BY p.perordem";

		    $itens = $this->carregar($sql);
		}

	    return is_array($itens) ? $itens : array();
	}

	function carregarPerguntasItemQuestionarioRecursive($grpid, $itpid, $tabela) {
	    $itensPergunta = $this->carregarPerguntasItemQuestionario($itpid);

	    if (count($itensPergunta))
	    {
	        foreach ($itensPergunta as $itemPergunta)
	        {
	            $tabela[$grpid]['perguntas'][$itemPergunta['perid']] = $itemPergunta;

				if ($itemPergunta['itpid'])
				{
	            	$existeNovosItens = $this->carregarPerguntasItemQuestionario($itemPergunta['itpid']);

            		if (count($existeNovosItens) && $itpid != $itemPergunta['itpid']) {
                		$tabela = $this->carregarPerguntasItemQuestionarioRecursive($grpid, $itemPergunta['itpid'], $tabela);
            		}
            	}
	        }
	    }

	    return $tabela;
	}

	function carregarRespostasMunicipio($inuid, $perguntas) {
		if ($perguntas) {
		    $perguntas = implode(',', $perguntas);

		    $sql = "SELECT
	                    p.perid, p.pertitulo,
	                	array_to_string(
	                		ARRAY(SELECT DISTINCT
	                			coalesce(res.resdsc, itp.itptitulo) as resp
	                		FROM
	                			questionario.resposta res
	                		LEFT JOIN questionario.itempergunta itp ON itp.itpid = res.itpid
	                		WHERE
	                			res.perid = p.perid
	                			AND res.qrpid = qun.qrpid
	                		), '; '
	                	) as itptitulo
	                FROM questionario.pergunta p
	                LEFT JOIN questionario.resposta r ON r.perid = p.perid
	                LEFT JOIN questionario.questionarioresposta qr ON qr.qrpid = r.qrpid
	                LEFT JOIN questionario.itempergunta         ip ON ip.itpid = r.itpid
	                LEFT JOIN par3.questionario_unidade         qun ON qun.qrpid = qr.qrpid AND qun.queid = ".QUESTIONARIO_BANDA_LARGA."
	                WHERE
	                    p.perid IN ({$perguntas})
	                    AND qun.inuid = {$inuid}
	                ORDER BY p.perordem";

		    $itens = $this->carregar($sql);

		    $respostas = array();

		    if (is_array($itens) && count($itens))
		        foreach ($itens as $item)
		            $respostas[$item['perid']] = $item;

		    return $respostas;
		}
	}

	function carregarQtdRespostasMunicipio($inuid)
	{
	    $sql = "SELECT
                	count(res.*) as qtd_respondidas
                FROM
                	questionario.grupopergunta grp
                INNER JOIN questionario.pergunta        per ON per.grpid = grp.grpid
                INNER JOIN questionario.resposta        res ON res.perid = per.perid
                INNER JOIN par3.questionario_unidade    qun ON qun.qrpid = res.qrpid AND qun.queid = ".QUESTIONARIO_BANDA_LARGA."
                WHERE
                	grp.queid = ".QUESTIONARIO_BANDA_LARGA."
                	AND qun.inuid = $inuid";

	    $qtd = $this->pegaUm($sql);

	    return $qtd;
	}

	function recuperaVigenciaQuetionario()
	{
	    $sql = "SELECT
                    to_char(quedtinicio, 'DD/MM/YYYY') as dt_inicio,
	                to_char(quedtfim, 'DD/MM/YYYY') as dt_fim
	            FROM questionario.questionario
	            WHERE queid = ".QUESTIONARIO_BANDA_LARGA;

	    return $this->pegaLinha($sql);
	}

	function recuperaQuetionarioVigente()
	{
	    $sql = "SELECT (quedtinicio::date <= now()::date) AND (quedtfim::date >= now()::date)
    	        FROM questionario.questionario
    	        WHERE queid = ".QUESTIONARIO_BANDA_LARGA;

	    $ativo = $this->pegaUm($sql);

	    return $ativo == 't';
	}

	function carregarBotaoWorkflow($inuid)
	{

	    $modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);
	    $docid           = $modelUnidade->retornarDocidQuestionario(QUESTIONARIO_BANDA_LARGA);
        $modelWorkflow   = new Workflow_Model_Documento($docid);

        $descricao  = 'Finalizar Preenchimento';
        $classe     = 'finalizar';
        if($modelWorkflow->esdid == WF_ESDID_FIANLIZADO){
            $descricao  = 'Reabrir Preenchimento';
            $classe     = 'reabrir';
        }

        $btn         = 'btn-success';
        $msgDisabled = '';
	    if(!$this->recuperaQuetionarioVigente() && $modelWorkflow->esdid == WF_ESDID_FIANLIZADO){
    	    $btn            = 'btn-danger';
            $msgDisabled    = '<br><label style="color:red;width:500px;">Prazo de preenchimento encerrado.<label/>';
            $classe         = 'vencido';
	    }

	    $btnFinalizaQuestionario = '
            <div class="row">
                <div class="col-md-6">
                    <h5>Banda Larga</h5>
                </div>
                <div class="col-md-6">
                    <input class="btn '.$btn.' '.$classe.'" type="button" value="'.$descricao.'" />
                    '.$msgDisabled.'
                </div>
            </div>';
	    $btnFinalizaQuestionario = '
                    <input class="btn '.$btn.' '.$classe.'" type="button" value="'.$descricao.'" />
                    '.$msgDisabled;

	    $arrPerfilValido = array(
            PAR3_PERFIL_SUPER_USUARIO,
            PAR3_PERFIL_ADMINISTRADOR,
            PAR3_PERFIL_PREFEITO,
            PAR3_PERFIL_DIRIGENTE_MUNICIPAL_DE_EDUCACAO,
            PAR3_PERFIL_EQUIPE_MUNICIPAL,
            PAR3_PERFIL_SECRETARIO_ESTADUAL,
            PAR3_PERFIL_EQUIPE_ESTADUAL,
	    );

	    $usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();
	    $retorno = $usuarioResponsabilidade->possuiPerfil($arrPerfilValido);

	    if($this->carregarQtdRespostasMunicipio($inuid) < 20 || !$retorno){
	        $btnFinalizaQuestionario = '';
	    }

	    echo $btnFinalizaQuestionario;
	}

	function finalizarPreenchimentoQuestionario($inuid)
	{
	    require_once APPRAIZ.'includes/workflow.php';

	    $modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);
	    $docid           = $modelUnidade->retornarDocidQuestionario(QUESTIONARIO_BANDA_LARGA);

	    $teste = wf_alterarEstado($docid, WF_AEDID_FINALIZAR, 'Finalizado por CPF: '.$_SESSION['usucpf'], array('docid' => $docid));

	    if($teste){
    	    $url = 'par3.php?modulo=principal/planoTrabalho/bandaLargaQuestionario&acao=A&inuid='.$inuid;
    	    simec_redirecionar($url, 'success');
	    }else{
    	    $url = 'par3.php?modulo=principal/planoTrabalho/bandaLargaQuestionario&acao=A&inuid='.$inuid;
    	    simec_redirecionar($url, 'error');
	    }
	}

	function reabrirPreenchimentoQuestionario($inuid)
	{
	    require_once APPRAIZ.'includes/workflow.php';

	    $modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);
	    $docid           = $modelUnidade->retornarDocidQuestionario(QUESTIONARIO_BANDA_LARGA);

	    wf_alterarEstado($docid, WF_AEDID_REABRIR, 'Reaberto por CPF: '.$_SESSION['usucpf'], array('docid' => $docid));

	    $url = 'par3.php?modulo=principal/planoTrabalho/bandaLargaQuestionario&acao=A&inuid='.$inuid;
	    simec_redirecionar($url, 'success');
	}

	function gerarPlanilha($data) {
		$filtros = array();

		if ($data['estuf'])
		{    $filtros[] = " AND iu.estuf = '{$data['estuf']}'";
		}

		if ($data['muncod']) {
		    $filtros[] = " AND iu.muncod = '{$data['muncod']}'";
		}

	    $tabela        = array();
	    $municipios    = $this->carregarInformacoesMunicipio($filtros);
	    $grupos        = $this->carregarGruposPerguntasQuestionario();

	    foreach ($grupos as $grupo)
	    {
	        $grpid = $grupo['grpid'];

	        $tabela[$grpid] = $grupo;

	        $tabela[$grpid]['perguntas'] = array();

	        $perguntas = $this->carregarPerguntasQuestionario($grupo['grpid']);

	        if (is_array($perguntas))
	        {
	            foreach ($perguntas as $pergunta)
	            {
	                $tabela[$grpid]['perguntas'][$pergunta['perid']] = $pergunta;

	                $tabela = $this->carregarPerguntasItemQuestionarioRecursive($grpid, $pergunta['itpid'], $tabela);
	            }
	        }
	    }

	    $perguntas = array();

	    foreach ($grupos as $grupo)
	    {
	        foreach ($tabela[$grupo['grpid']]['perguntas'] as $data)
	        {
	            $perguntas[$data['perid']] = $data['perid'];
	        }
	    }

	    require(APPRAIZ . 'par3/modulos/relatorio/relatorioBandaLargaXls.inc');
	    die;
	}
}
?>
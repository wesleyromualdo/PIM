<?php
/**
 * Classe de mapeamento da entidade par3.obra_x_questionario.
 *
 * @version $Id$
 * @since 2017.07.10
 */

/**
 * Par3_Model_Obra_x_questionario: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_x_questionario($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_x_questionario($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $obrid  - default: nextval('par3.obra_x_questionario_obrid_seq'::regclass)
 * @property int $qrpid
 * @property int $queid
 */
class Par3_Model_ObraXQuestionario extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_x_questionario';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array('obqeid');

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'queid' => array('tabela' => 'questionario.questionario', 'pk' => 'queid'),
        'escid' => array('tabela' => 'par3.escola', 'pk' => 'escid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'obqeid' => null,
        'obrid' => null,
        'qrpid' => null,
        'queid' => null,
        'beneficiada_fnde' => null,
        'obrid_reforma' => null,
        'cod_escola' => null,
        'apliacao_mesmo_terreno' => null,
        'tipo_obra_ampliacao' => null,
        'escid' => null,
    );

    public $modelObraTipo;
    public $modelPlanejamento;
    public $modelObra;
    public $modelObraCategoria;
    public $modelObraAba;

    public function verificaObra($obrid, $inuid)
    {
        $mdUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        if($this->modelObraTipo->octid == Par3_Model_ObraTipo::REFORMA) {
            $where = array("o.obrid = $obrid", "str.strid IN(6)", "o.obridpai IS NULL", "o.obrstatus = 'A'");
        }else{
            $where = array(
                "o.obrid = $obrid",
                "(
                    str.strid NOT IN (7,8)
                    OR
                    (
                    str.strid IN(7) AND
                    COALESCE(obrpercentultvistoria, 0) >= 40
                    )
                )",
                "o.obridpai IS NULL", "o.obrstatus = 'A'");
        }

        if($mdUnidade->itrid == '2'){
            $where[] = "mun.muncod IN ('{$mdUnidade->muncod}')";
        }else{
            $where[] = "mun.estuf IN ('{$mdUnidade->estuf}')";
        }

        $sql = "SELECT *
                FROM (
                    SELECT DISTINCT ON (o.obrid)
                    	true
                    FROM obras2.obras o
                    LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                    LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                    LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                    LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
                    LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                    LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                    LEFT JOIN obras2.contrato c                          ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                    LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
                    LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                    LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                    LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade
                    LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                    LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid
                    LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = d.esdid
                    LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid
                    WHERE
                        ".implode(' AND ', $where)."
                ) as f
                    ";
        $booObraOK = $this->pegaUm($sql);
        return $booObraOK == 't' ? true : false;
    }

    public function __construct($id = null, $obrid = null, $oabid = null)
    {
        if (is_null($id)&&$obrid) {
            $dados = $this->recuperarTodos('*', array(" obrid = $obrid "));
            $this->popularDadosObjeto($dados[0]);
        } else {
            parent::__construct($id);
        }

        $modelObra = new Par3_Model_Obra();
        $modelObra->carregarPorId($obrid);

        $this->modelObraTipo = new Par3_Model_ObraTipo($modelObra->otpid);
        $this->modelObraCategoria = new Par3_Model_ObraCategoria($this->modelObraTipo->octid);

        if ($obrid) {
            $this->modelObra = new Par3_Model_Obra($obrid);

            if ($this->modelObra->inpid)
                $this->modelPlanejamento = new Par3_Model_IniciativaPlanejamento($this->modelObra->inpid);

            if ($modelObra->otpid)
                $this->modelObraTipo = new Par3_Model_ObraTipo($this->modelObra->otpid);

            if ($oabid)
                $this->modelObraAba = new Par3_Model_ObraAba($oabid);
        }

        $this->getRegraQuestionario();

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
            'obrid' => array('Digits'),
            'qrpid' => array('Digits'),
            'queid' => array('allowEmpty' => true, 'Digits'),
        );
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
        if(!$this->obrid_reforma){ //para não tentar salvar com valor vazio
            $this->obrid_reforma = null;
        }

        if(!$this->qrpid){
            $this->qrpid = null;
        }
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    /************* QUESTIONARIO ***********/

	public function getRegraQuestionario()
    { //ver($this->modelObraAba->oabid);
        switch ($this->modelObraAba->oabid) {

        	case '3':
                switch ($this->modelObraTipo->octid) {
                    case Par3_Model_ObraTipo::CONSTRUCAO:
                        $this->queid = QUESTIONARIO_OBRA_VISTORIA;
                        break;

                    case Par3_Model_ObraTipo::AMPLIACAO:
                            if (($this->beneficiada_fnde =='S' && $this->apliacao_mesmo_terreno == 'N') ||
                                $this->modelObraTipo->otptipoampliacao == Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO_QUADRA ||
                                $this->modelObraTipo->otptipoampliacao == Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO_COBERTURA
                              )
                             {
                                //ver("Escola FNDE","Outro terreno", "MODELO 01", $this->queid);
                                $this->queid = QUESTIONARIO_OBRA_VISTORIA;
                            } else if ($this->modelObraTipo->otptipoampliacao == Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO_ESCOLA) {
                                // ver( "MODELO 02", $this->queid);
                                $this->queid = QUESTIONARIO_OBRA_VISTORIA2;
                            }
                           break;
                    case Par3_Model_ObraTipo::REFORMA:
                        if ($this->beneficiada_fnde == 'N')
                            $this->queid = QUESTIONARIO_OBRA_VISTORIA2;
                        break;
                }
                break;
        }
    }

    /**
     * Função retornarQrpid
     * - recupera retorna qrpid.
     *
     * @return integer $qrpid.
     *
     */
    public function getQuestionario($obrid)
    {
        if (!empty($this->queid)) {
            $sql = "SELECT qrpid FROM par3.obra_x_questionario WHERE obrid = {$obrid} AND queid = $this->queid";
            $qrpid = $this->pegaUm($sql);
            
            if (!$qrpid) {
                $sql = "INSERT INTO questionario.questionarioresposta(queid, qrptitulo, qrpdata)
                		VALUES({$this->queid}, 'Reposta Questões Vistoria - obrid: {$obrid}', now())
                		RETURNING qrpid;";
                $qrpid = $this->pegaUm($sql);

                $sql = "UPDATE par3.obra_x_questionario SET qrpid = {$qrpid} WHERE obrid = {$obrid} AND queid = {$this->queid};";
            } else {
                // Garanto a atualização do QUEID na relação questionario.questionarioresposta, caso tenha chegado aqui com problemas.
                $sql = "UPDATE questionario.questionarioresposta SET queid = {$this->queid} WHERE  qrpid =  {$qrpid};";
            }

            $this->executar($sql);
            $this->commit();
           // ver($qrpid);
            return $qrpid;
        }
        return false;

    }//end retornarQrpid()

    function carregarGruposPerguntasQuestionario()
    {
        $sql = "SELECT  grpid, grptitulo
	            FROM    questionario.grupopergunta
	            WHERE   queid = {$this->queid}
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

    function carregarPerguntasItemQuestionario($itpid)
    {
        $itens = null;

        if ($itpid) {
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

    function carregarPerguntasItemQuestionarioRecursive($grpid, $itpid, $tabela)
    {
        $itensPergunta = $this->carregarPerguntasItemQuestionario($itpid);

        if (count($itensPergunta)) {
            foreach ($itensPergunta as $itemPergunta) {
                $tabela[$grpid]['perguntas'][$itemPergunta['perid']] = $itemPergunta;

                if ($itemPergunta['itpid']) {
                    $existeNovosItens = $this->carregarPerguntasItemQuestionario($itemPergunta['itpid']);

                    if (count($existeNovosItens) && $itpid != $itemPergunta['itpid']) {
                        $tabela = $this->carregarPerguntasItemQuestionarioRecursive($grpid, $itemPergunta['itpid'], $tabela);
                    }
                }
            }
        }

        return $tabela;
    }

    function carregarRespostasMunicipio($obrid, $perguntas)
    {
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
	                LEFT JOIN par3.obra_x_questionario         qun ON qun.qrpid = qr.qrpid AND qun.queid = {$this->queid}
	                WHERE
	                    p.perid IN ({$perguntas})
	                    AND qun.obrid = {$obrid}
	                ORDER BY p.perordem";

            $itens = $this->carregar($sql);

            $respostas = array();

            if (is_array($itens) && count($itens))
                foreach ($itens as $item)
                    $respostas[$item['perid']] = $item;

            return $respostas;
        }
    }

    function carregarQtdRespostasMunicipio($obrid)
    {
        $sql = "SELECT
                	count(res.*) as qtd_respondidas
                FROM
                	questionario.grupopergunta grp
                INNER JOIN questionario.pergunta        per ON per.grpid = grp.grpid
                INNER JOIN questionario.resposta        res ON res.perid = per.perid
                INNER JOIN par3.obra_x_questionario    qun ON qun.qrpid = res.qrpid AND qun.queid = {$this->queid}
                WHERE
                	grp.queid = " . QUESTIONARIO_OBRA_VISTORIA . "
                	AND qun.obrid = $obrid";

        $qtd = $this->pegaUm($sql);

        return $qtd;
    }

    function recuperarVigenciaQuestionario()
    {
        $sql = "SELECT
                    to_char(quedtinicio, 'DD/MM/YYYY') as dt_inicio,
	                to_char(quedtfim, 'DD/MM/YYYY') as dt_fim
	            FROM questionario.questionario
	            WHERE queid = {$this->queid}";

        return $this->pegaLinha($sql);
    }

    function carregarQtdRespostasVistoria($obrid)
    {
        $sql = "SELECT
                	count(res.*) as qtd_respondidas
                FROM
                	questionario.grupopergunta grp
                INNER JOIN questionario.pergunta        per ON per.grpid = grp.grpid
                INNER JOIN questionario.resposta        res ON res.perid = per.perid
                INNER JOIN par3.obra_x_questionario    qun ON qun.qrpid = res.qrpid AND qun.queid = {$this->queid}
                WHERE
                	grp.queid = {$this->queid}
                	AND qun.obrid = $obrid";

        $qtd = $this->pegaUm($sql);

        return $qtd;
    }

    function recuperaVigenciaQuestionario()
    {
        $sql = "SELECT
                    to_char(quedtinicio, 'DD/MM/YYYY') as dt_inicio,
	                to_char(quedtfim, 'DD/MM/YYYY') as dt_fim
	            FROM questionario.questionario
	            WHERE queid = {$this->queid}";

        return $this->pegaLinha($sql);
    }

    function finalizarPreenchimentoQuestionario($obrid, $inuid)
    {
        require_once APPRAIZ . 'includes/workflow.php';

        $modelUnidade = new Par3_Model_InstrumentoUnidade($obrid);
        $docid = $modelUnidade->retornarDocidQuestionario(QUESTIONARIO_OBRA_VISTORIA);

        $teste = wf_alterarEstado($docid, WF_AEDID_FINALIZAR, 'Finalizado por CPF: ' . $_SESSION['usucpf'], array('docid' => $docid));

        if ($teste) {
            $url = 'par3.php?modulo=principal/planoTrabalho/obras&menu=relatorio_vistoria&acao=A&obrid=' . $obrid . 'inuid=' . $inuid;
            simec_redirecionar($url, 'success');
        } else {
            $url = 'par3.php?modulo=principal/planoTrabalho/bandaLargaQuestionario&acao=A&obrid=' . $obrid;
            simec_redirecionar($url, 'error');
        }
    }

    function reabrirPreenchimentoQuestionario($obrid, $inuid)
    {
        require_once APPRAIZ . 'includes/workflow.php';

        $modelUnidade = new Par3_Model_InstrumentoUnidade($obrid);
        $docid = $modelUnidade->retornarDocidQuestionario($this->queid);

        wf_alterarEstado($docid, WF_AEDID_REABRIR, 'Reaberto por CPF: ' . $_SESSION['usucpf'], array('docid' => $docid));

        $url = 'par3.php?modulo=principal/planoTrabalho/obras&menu=relatorio_vistoria&acao=A&obrid=' . $obrid . 'inuid=' . $inuid;
        simec_redirecionar($url, 'success');
    }

    function gerarPlanilha($data)
    {
        $filtros = array();

        if ($data['estuf']) {
            $filtros[] = " AND iu.estuf = '{$data['estuf']}'";
        }

        if ($data['muncod']) {
            $filtros[] = " AND iu.muncod = '{$data['muncod']}'";
        }

        $tabela = array();
        $municipios = $this->carregarInformacoesMunicipio($filtros);
        $grupos = $this->carregarGruposPerguntasQuestionario();

        foreach ($grupos as $grupo) {
            $grpid = $grupo['grpid'];

            $tabela[$grpid] = $grupo;

            $tabela[$grpid]['perguntas'] = array();

            $perguntas = $this->carregarPerguntasQuestionario($grupo['grpid']);

            if (is_array($perguntas)) {
                foreach ($perguntas as $pergunta) {
                    $tabela[$grpid]['perguntas'][$pergunta['perid']] = $pergunta;

                    $tabela = $this->carregarPerguntasItemQuestionarioRecursive($grpid, $pergunta['itpid'], $tabela);
                }
            }
        }

        $perguntas = array();

        foreach ($grupos as $grupo) {
            foreach ($tabela[$grupo['grpid']]['perguntas'] as $data) {
                $perguntas[$data['perid']] = $data['perid'];
            }
        }

        require(APPRAIZ . 'par3/modulos/relatorio/relatorioBandaLargaXls.inc');
        die;
    }

    function recuperaQuetionarioVigente()
    {
        $sql = "SELECT (quedtinicio::date <= now()::date) AND (quedtfim::date >= now()::date)
    	        FROM questionario.questionario
    	        WHERE queid = {$this->queid}";

        $ativo = $this->pegaUm($sql);

        return $ativo == 't';
    }

    function getEscolaRelatoriaVistoria($inuid, $obrid)
    {
        $sql = "SELECT
                    escola.no_entidade AS descricao,
                    escola.co_entidade AS codigo
                FROM par3.instrumentounidade ins
                INNER JOIN territorios.municipio                    mun     ON ((ins.muncod IS NOT NULL AND ins.muncod = mun.muncod) OR (ins.muncod IS NULL AND ins.estuf = mun.estuf))
                INNER JOIN educacenso_2016.ts_censo_basico_escola   escola  ON (mun.muncod :: BIGINT = escola.co_municipio)
                WHERE
                    ins.inuid = {$inuid}
                    AND (
                        (ins.muncod IS NOT NULL AND escola.tp_dependencia = 3) OR
                        (ins.muncod IS NULL AND escola.tp_dependencia = 2)
                    )
                    AND co_entidade  NOT IN (SELECT cod_escola FROM par3.obra_x_questionario  WHERE cod_escola IS NOT NULL AND obrid != {$obrid} )";
        $dados = $this->carregar($sql);
        $arr = array();
        if (is_array($dados))
            foreach ($dados as $valor) {
                $arr[$valor['codigo']] = $valor['descricao'];
            }
        return $arr;
    }

    public function listarEscolas($obrid, $inuid,$escid = null)
    {
        $orEscid = "";
        if($escid) {
            $orEscid = "or esc.escid = {$escid}";
        }
        $mdInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $colunaTipoEscolaOedXesc = '';
        $colunaTipoEscolaEsc     = '';

        $inp = new Par3_Controller_IniciativaPlanejamento();
        $desdobramentos = $inp->getDesdobramentos($_REQUEST['inpid']);
        $filtroLocalizacao = '';

        $filtroLocalizacao = 'AND esc.escco_localizacao IN(1,2)';
        //Verifica se existe desdobramento urbano
        if(in_array(DESDOBRAMENTO_URBANO,$desdobramentos)){
            $filtroLocalizacao = 'AND esc.escco_localizacao = 1';
        }

        //Verifica se existe desdobramento rural
        if(in_array(DESDOBRAMENTO_RURAL,$desdobramentos)){
            $filtroLocalizacao = 'AND esc.escco_localizacao = 2';
        }

        //Verifica se existe desdobramento urbano e rural na iniciativa
        if(in_array(array(DESDOBRAMENTO_URBANO,DESDOBRAMENTO_RURAL),$desdobramentos)){
            $filtroLocalizacao = 'AND esc.escco_localizacao IN(1,2)';
        }
        $mInp = $inp->recuperar();

        switch ($mInp->etaid){
            case 1:
                $colunaTipoEscolaEsc     = 'escqtd_alunos_infantil';
                $colunaTipoEscolaOedXesc = 'ederemanejamento_infantil';
                break;
            case 2:
                $colunaTipoEscolaEsc     = 'escqtd_alunos_fundamental';
                $colunaTipoEscolaOedXesc = 'ederemanejamento_fundamental';
                break;
            case 3:
                $colunaTipoEscolaEsc     = 'escqtd_alunos_medio';
                $colunaTipoEscolaOedXesc = 'ederemanejamento_medio';
                break;
            default:
                break;
        }

        $arrEsc = array();

        if($mdInstrumentoUnidade->itrid == 2){
            $where[] = "esc.escco_esfera = 'M'";
            $where[] = "esc.muncod = '{$mdInstrumentoUnidade->muncod}'";
        }else{
            $where[] = "esc.escco_esfera = 'E'";
            $where[] = "esc.estuf = '{$mdInstrumentoUnidade->estuf}'";
        }

        /**
         * REGRA PARA VALIDAR PELA CATEGORIA
         * CASO SEJA AMPLIAÇÃO O CAMPO VALIDADO É escid
         * CASO SEJA REFORMA O CAMPO VALIDADO É cod_escola
         */
        $obr   = new Par3_Model_Obra($obrid);
        $otp   = new Par3_Model_ObraTipo();
        $arOct = $otp->getCategoriaPorTipo($obr->otpid);

        $campoEscola = 'escid';
        if($arOct['octid'] == Par3_Model_ObraCategoria::CATEGORIA_REFORMA) {
            $campoEscola = 'cod_escola';
        }

        $sql = "
          --alternar para validar escolas pela coluna cod_escola para obras com categoria reforma
          --alternar para validar escolas pela coluna escid para obras com categoria ampliação
            SELECT DISTINCT
                esc.escid as codigo, esc.esccodinep||' - '||esc.escnome as descricao, esc.escnome
            FROM par3.escola esc
            WHERE ".implode(' AND ', $where)."
            {$filtroLocalizacao}
            AND (esc.escco_situacao_funcoinamento in(1,2) {$orEscid})
            AND escid  NOT IN (
                SELECT {$campoEscola}
                    FROM par3.obra_x_questionario 
                    JOIN par3.obra obr USING (obrid) 
                    join par3.obra_tipo otp using (otpid)
                    join workflow.documento documento using(docid)
                    join workflow.estadodocumento ed ON ed.esdid = documento.esdid
                    WHERE obr.obrstatus = 'A' 
                    AND {$campoEscola} IS NOT NULL 
                    AND obrid != {$obrid}
                    AND otp.octid in(select octid from par3.obra_categoria where octid = otp.octid)
                    AND ed.esdid not in(2061,2053)
                )
            ORDER BY
            esc.escnome";
        $rsEsc = $this->carregar($sql);
        if(is_array($rsEsc)){
            foreach ($rsEsc as $esc){
                $arrEsc[] = $esc;
            }
        }
        return is_array($arrEsc)?$arrEsc:array();

    }

    public function carregarBotaoWorkflow($obrid)
    {

//        $modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);
//        $docid           = $modelUnidade->retornarDocidQuestionario(QUESTIONARIO_BANDA_LARGA);
//        $modelWorkflow   = new Workflow_Model_Documento($docid);
//
//        $descricao  = 'Finalizar Preenchimento';
//        $classe     = 'finalizar';
//        if($modelWorkflow->esdid == WF_ESDID_FIANLIZADO){
//            $descricao  = 'Reabrir Preenchimento';
//            $classe     = 'reabrir';
//        }

        if (!empty($this->queid)) {
            $btn = 'btn-success';
            $msgDisabled = '';
            if (!$this->recuperaQuetionarioVigente()) { // && $modelWorkflow->esdid == WF_ESDID_FIANLIZADO
                $btn = 'btn-danger';
                $msgDisabled = '<br><label style="color:red;width:500px;">Prazo de preenchimento encerrado.<label/>';
                $classe = 'vencido';
            }

            $btnFinalizaQuestionario = '
            <div class="row">
                <div class="col-md-6">
                    <h5>Banda Larga</h5>
                </div>
                <div class="col-md-6">
                    <input class="btn ' . $btn . ' ' . $classe . '" type="button" value="' . $descricao . '" />
                    ' . $msgDisabled . '
                </div>
            </div>';
            $btnFinalizaQuestionario = '
                    <input class="btn ' . $btn . ' ' . $classe . '" type="button" value="' . $descricao . '" />
                    ' . $msgDisabled;

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

            if ($this->carregarQtdRespostasMunicipio($obrid) < 20 || !$retorno) {
                $btnFinalizaQuestionario = '';
            }

            echo $btnFinalizaQuestionario;
        }
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $obrquid
     * @description Esta função cria um objeto populado pelo id da linha "obqeid"
     */
    public function create($obrquid){
        $sql = " select * from par3.obra_x_questionario where obqeid = {$obrquid}";
        try{
            $dados = $this->pegaLinha($sql);
            $this->popularDadosObjeto($dados);
        }catch (Exception $e){
            echo $e->getMessage();
        }
    }

    /************************/

}

<?php
/**
 * Classe de mapeamento da entidade par3.obra.
 *
 * @version $Id$
 * @since 2017.07.07
 */

/**
 * Par3_Model_Obra: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $obrid  - default: nextval('par3.obra_obrid_seq'::regclass)
 * @property int $otpid
 * @property int $ovpid
 * @property int $tplid
 * @property int $inuid
 * @property string $obrdsc
 * @property string $obrstatus
 * @property string $obrcep
 * @property string $obrlogradouro
 * @property smallint $obrnumero
 * @property string $obrcomplemento
 * @property string $obrponto_referencia
 * @property string $obrbairro
 * @property string $obrlatitude
 * @property string $obrlongitude
 * @property numeric $obrvalor
 * @property numeric $obrmetragem
 */
class Par3_Model_Obra extends Modelo
{
    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

     const ESDID_AGUARDANDO_PROVIDENCIA  = 1140;
     const ESDID_AGUARDANDO_ANALISE_FNDE = 1141;
     const ESDID_SUPERADA                = 1142;
     const ESDID_CANCELADA               = 1143;
     const ESDID_AGUARDANDO_CORRECAO     = 1144;
     const ESDID_JUSTIFICADA             = 1497;

    /**
     * CONSTANTES DO ESTADO DOCUMENTO DA OBRA NOVO
     * @TODO definir tramitação da obra para obra 2.0
     */
     const ESDID_EM_CADASTRAMENTO                           = 2007;
     const ESDID_PAR3_AGUARDANDO_ANALISE_FNDE               = 2039;
     const ESDID_EM_ANALISE                                 = 2040;//ao começar a escrever a análise
     const ESDID_VALIDACAO_DILIGENCIA                       = 2041;
     const ESDID_REVISAO_ANALISE                            = 2042;
     const ESDID_EM_REVISAO_ANALISE_RETORNO_PROFE           = 2043;
     const ESDID_EM_DILIGENCIA                              = 2044;//prefeito/secretário
     const ESDID_AGUARDANDO_ANALISE_FNDE_RETORNO_DILIGENCIA = 2045;
     const ESDID_VALIDACAO_DEFERIMENTO                      = 2046;
     const ESDID_AGUARDANDO_ANALISE_PROFE                   = 2047;
     const ESDID_VALIDACAO_RETORNO_PROFE                    = 2048;
     const ESDID_OBRA_DEFERIDA                              = 2049;
     const ESDID_OBRA_APROVADA                              = 2050;
     const ESDID_VALIDACAO_INDEFERIMENTO                    = 2051;
     const ESDID_OBRA_INDEFERIDA                            = 2052;
     const ESDID_OBRA_ARQUIVADA                             = 2053;
     const ESDID_EM_CADASTRAMENTO_CONDICIONAL               = 2054;
     const ESDID_APROVACAO_CONDICIONAL                      = 2055;
     const ESDID_EM_REFORMULACAO                            = 2056;
     const ESDID_ANALISE_DE_REFORMULACAO                    = 2057;
     const ESDID_EM_DILIGENCIA_DE_REFORMULACAO              = 2058;
     const ESDID_VALIDACAO_DE_DEFERIMENTO_DE_REFORMULACAO   = 2059;
     const ESDID_VALIDACAO_DE_INDEFERIMENTO_DE_REFORMULACAO = 2060;
     const ESDID_OBRA_CANCELADA                             = 2061;
//     const ESDID_CONLUIDO                                   = 2061;
     const TPD_FLUXO_OBRAS                                  = 304;

    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'obrid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
        'inpid' => array('tabela' => 'par3.iniciativa_planejamento', 'pk' => 'inpid'),
        'tplid' => array('tabela' => 'par3.obra_tipo_versao_planilha', 'pk' => 'tplid'),
        'ovpid' => array('tabela' => 'par3.obra_tipo_versao_projeto', 'pk' => 'ovpid'),
        'otpid' => array('tabela' => 'par3.obra_tipo', 'pk' => 'otpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'obrid' => null,
        'otpid' => null,
        'ovpid' => null,
        'tplid' => null,
        'inuid' => null,
        'inpid' => null,
        'obrdsc' => null,
        'obrstatus' => null,
        'obrcep' => null,
        'obrlogradouro' => null,
        'obrnumero' => null,
        'obrcomplemento' => null,
        'obrponto_referencia' => null,
        'obrbairro' => null,
        'estuf' => null,
        'docid' => null,
        'muncod' => null,
        'obrlatitude' => null,
        'obrlongitude' => null,
        'obrvalor' => null,
        'obralunos_atendidos' => null,
        'obrvalor_estimado' => null,
        'obrano'            =>null,
        'obrqtd_alunos_projeto_proprio' => null,
        'obrvalor_contrapartida' => null,
        'obrmetragem'            => null
    );

    public function __construct($id = null, $force = null, $arrDadosForce = array() )
    {
        if(!$id&&$force&&$arrDadosForce['inuid']&&$arrDadosForce['inpid']){
            $id = $this->criaObraVazia($arrDadosForce);
        }
        parent::__construct($id);
    }

    public function criaObraVazia($arrDados)
    {

        include_once APPRAIZ . "includes/workflow.php";

        $docid = wf_cadastrarDocumento( PAR3_WORKFLOW_FLUXO_OBRAS, 'Obra iniciativa PAR3');

        $sql = "INSERT INTO par3.obra(inuid, inpid, obrano, docid) VALUES({$arrDados['inuid']}, {$arrDados['inpid']}, {$arrDados['obrano']}, $docid) RETURNING obrid;";
        $obrid = $this->pegaUm($sql);
        $this->commit();

        return $obrid;
    }
    
 	public function validaExclusaoObraPlanejamento($obrid = null)
    {
    	if( $obrid == null )
    	{
			return FALSE;
    	}
    	else
    	{
	        include_once APPRAIZ . "includes/workflow.php";
	
	        $docid = wf_cadastrarDocumento( PAR3_WORKFLOW_FLUXO_OBRAS, 'Obra iniciativa PAR3');
	
	        $sql = "
	        	SELECT
	        		doc.esdid
				FROM
					par3.obra obr
				INNER JOIN  workflow.documento doc on doc.docid = obr.docid
				
				WHERE
					obrid = {$obrid}";
	        
	        $esdid = $this->pegaUm($sql);
	        
	        if( $esdid == PAR3_ESDID_OBRA_EM_CADASTRAMENTO)
	        {
	        	return TRUE;
	        }
	        else
	        {
	        	return FALSE;
	        }
    	}
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
            'otpid' => array('allowEmpty' => true, 'Digits'),
            'ovpid' => array('allowEmpty' => true, 'Digits'),
            'tplid' => array('allowEmpty' => true, 'Digits'),
            'inuid' => array('allowEmpty' => true, 'Digits'),
            'inpid' => array('allowEmpty' => true, 'Digits'),
            'obrdsc' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'obrstatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'obrcep' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 8))),
            'obrlogradouro' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'obrnumero' => array('allowEmpty' => true),
            'obrcomplemento' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'obrponto_referencia' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'obrbairro' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'obrlatitude' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 12))),
            'obrlongitude' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 12))),
            'obrvalor' => array('allowEmpty' => true),
            'obrvalor_contrapartida' => array('allowEmpty' => true),
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
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function getByInuid($inuid)
    {
        $dados = $this->lista('*', array(" inuid = $inuid "));

        if ($dados)
            $this->popularDadosObjeto($dados[0]);
    }

    public function getDescricaoResumidaObra($obrid, $uf = null, $muncod = null, $octid = null)
    {
        $where = " WHERE o.obrid = {$obrid} ";
        if($octid == Par3_Model_ObraTipo::AMPLIACAO)  {
            $where .= "
                AND (
                    str.strid NOT IN (7,8)
                    OR
                    (
                    str.strid IN(7) AND
                    COALESCE(obrpercentultvistoria, 0) >= 40
                    )
                )
                AND o.obridpai IS NULL
                AND o.obrstatus = 'A'";
        } else{
            $where .= " AND str.strid IN(6) AND o.obridpai IS NULL AND o.obrstatus = 'A'";
        }



        if (!empty($uf)) {
            $where .= " AND mun.estuf = '{$uf}' ";
        } elseif (!empty($muncod)) {
            $where .= " AND mun.muncod = '{$muncod}' ";
        }

        $esdidSuperada = Par3_Model_Obra::ESDID_SUPERADA;
        $esdidJustificada = Par3_Model_Obra::ESDID_JUSTIFICADA;

        $restricoesSuperadas = <<<SQL
 CASE WHEN (SELECT COUNT(*)
                FROM obras2.restricao r
                JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN ({$esdidSuperada}, {$esdidJustificada})
                WHERE r.rststatus = 'A' AND r.rstitem = 'R' AND r.obrid = o.obrid
            ) > 0
    THEN '<img title="Restrições pendentes" style="cursor:pointer; width:15px;" onclick="abreRestricao(' || o.obrid || ');" src="/imagens/obras/atencao_vermelho.png">'
    ELSE '<img title="Restrições superadas" style="cursor:pointer; width:15px;" onclick="abreRestricao(' || o.obrid || ');" src="/imagens/obras/atencao_verde.png">'
    END as restricoesSuperadas
SQL;

        $restricoesPendentes = <<<SQL
 CASE WHEN (SELECT COUNT(*)
    FROM obras2.restricao r
      JOIN workflow.documento d ON d.docid = r.docid AND d.esdid NOT IN ({$esdidSuperada}, {$esdidJustificada})
        WHERE r.rststatus = 'A' AND r.rstitem = 'I' AND r.obrid = o.obrid) > 0
        THEN '<img title="Restrições pendentes" style="cursor:pointer; width:15px;" onclick="abreRestricao(' || o.obrid || ');" src="/imagens/obras/atencao_vermelho.png">'
        ELSE '<img title="Restrições superadas" style="cursor:pointer; width:15px;" onclick="abreRestricao(' || o.obrid || ');" src="/imagens/obras/atencao_verde.png">'
  END as restricoesPendentes
SQL;

        $sql = <<<SQL
          SELECT
          {$restricoesSuperadas},
          {$restricoesPendentes},
          o.obrid,
          o.preid,
          p_conv.termo_convenio || ' / ' || p_conv.ano_termo_convenio AS termo_ano,
          obrnome,
          ed.esddsc,
          o.obrpercentultvistoria || '%'                              AS obrpercentultvistoria,
          tpo.tpodsc,
          mun.muncod,
          mun.estuf
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
        LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid
        {$where}
SQL;

//         ver(simec_htmlentities($sql));exit;
        return $sql;

    }


    public function montarSQLSimples($arrPost){

        $sql = "SELECT obr.obrid as situacao,obr.obrid,otp.otpdsc,obr.obrdsc,obr.obrvalor,est.esddsc,

                   CASE  WHEN UPPER(est.esddsc)  = 'CONCLUÍDO' THEN TRUE
                   ELSE false
                   END AS situacaoworkflow

                from  par3.obra obr
                inner join par3.obra_tipo otp on otp.otpid = obr.otpid
                inner join par3.obra_categoria oct on oct.octid = otp.octid
                inner join  workflow.documento doc on doc.docid = obr.docid
                inner join workflow.estadodocumento est on est.esdid = doc.esdid
                where inpid = {$arrPost['inpid']} and obrano = {$arrPost['ano']} and inuid = {$arrPost['inuid']} and obr.obrstatus = 'A' ";
//ver($sql);
        return $sql;
    }

    public function detalharPendenciaEstudoDemanda($obrid)
    {
        //Verificar Estudo de Demanda
        if (!$this->verificarEstudoDemanda($obrid)) {
            return 'O Estudo de Demanda não foi preenchido';
        }
        $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
        if(!$mAnalise->verificarExistePendencia($obrid,Par3_Model_ObraAba::ESTUDO_DEMANDA)) {
            return 'Os itens da Análise de Engenharia não foram corrigidos';
        }
    }

    public function detalharPendenciaCronogramaContrapartida($obrid)
    {
        $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
        if(!$mAnalise->verificarExistePendencia($obrid,Par3_Model_ObraAba::CRONOGRAMA_CONTRAPARTIDA)) {
            return 'Os itens da Análise de Engenharia não foram corrigidos';
        }
    }


    public function detalharPendenciaDadosTerreno($obrid)
    {
        if(!$this->verificarPreenchimentoDadosTerreno($obrid)){
            $mObra = new Par3_Model_Obra($obrid);
            if(empty($mObra->obrdsc)){$arrPendencias[]              = 'Nome da Obra';}
            if(empty($mObra->tplid)){$arrPendencias[]               = 'Tipo de Obra';}
            if(empty($mObra->ovpid)){$arrPendencias[]               = 'Versão da Planilha';}
            if(empty($mObra->obrcep)){$arrPendencias[]              = 'CEP da Obra';}
            if(empty($mObra->obrlogradouro)){$arrPendencias[]       = 'Logradouro';}
            if(empty($mObra->obrnumero)){$arrPendencias[]           = 'Número do endereço';}
            if(empty($mObra->obrcomplemento)){$arrPendencias[]      = 'Complemento';}
            if(empty($mObra->obrponto_referencia)){$arrPendencias[] = 'Ponto de Referência';}
            if(empty($mObra->obrbairro)){$arrPendencias[]           = 'Bairro';}
            if(empty($mObra->obrlatitude)){$arrPendencias[]         = 'Latitude';}
            if(empty($mObra->obrlongitude)){$arrPendencias[]        = 'Longitude';}
            return $arrPendencias;
        }

        $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
        if(!$mAnalise->verificarExistePendencia($obrid,Par3_Model_ObraAba::DADOS_TERRENO)){
            return 'Os itens da Análise de Engenharia não foram corrigidos';
        }
    }

    public function detalharPendenciaFotosTerreno($obrid)
    {
        if(!$this->verificarFotosTerreno($obrid)){
            $mObra = new $this($obrid);
            $mFotosTerreno  = new Par3_Model_ObraFotosTerreno();
            $rsFotosTerreno = $mFotosTerreno->recuperarTodos('*',array("obrid = {$obrid}","oftstatus ='A' and arqid is not null"));
            $mObraConfAba   = new Par3_Model_ObraConfiguracaoAba();
            $obraAba        = $mObraConfAba->recuperarPorTipoObra($mObra->otpid);
            if(empty($rsFotosTerreno)){
                return 'Não há fotos cadastradas';
            }
            if(count($rsFotosTerreno) < $obraAba->ocaqtd_minimo ) {
                return 'É preciso cadastrar no mínimo '.$obraAba->ocaqtd_minimo.' fotos.';
            }
            if (count($rsFotosTerreno)  > $obraAba->ocaqtd_maximo){
                return 'Quantidade máxima de fotos permitidas: '.$obraAba->ocaqtd_maximo.' fotos.';
            }
        }

        //VALIDAÇÃO DA ANÁLISE
        $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
        if(!$mAnalise->verificarExistePendencia($obrid,Par3_Model_ObraAba::FOTOS_TERRENO)){
            return 'Os itens da Análise de Engenharia não foram corrigidos';
        }
    }

    public function detalharDocumentoAnexo($obrid)
    {
        $mDocumentos  = new Par3_Model_ObraDocumentos();
        $sql = <<<DML
        SELECT
            oct.octordem as docordem,
            otd.otddsc,
            otd.otdqtd_minima,
            count(arq.arqid) as arquivos_anexados
        FROM par3.obra obr
        INNER JOIN par3.obra_tipo 					otp ON otp.otpid = obr.otpid
        INNER JOIN par3.obra_configuracao_aba 		oca ON oca.otpid = otp.otpid AND oca.ocastatus = 'A'
        INNER JOIN par3.obra_confi_aba_x_tipo_doc 	oct ON oct.ocaid = oca.ocaid AND oct.octstatus = 'A'
        INNER JOIN par3.obra_tipo_documento 		otd ON otd.otdid = oct.otdid
        LEFT  JOIN par3.obra_documentos 			odo ON odo.otdid = otd.otdid AND odo.obrid = obr.obrid AND odo.odostatus = 'A'
        LEFT  JOIN public.arquivo 					arq ON arq.arqid = odo.arqid
        WHERE obr.obrid = {$obrid} AND otd.otdsituacao = 'A' AND otd.otdstatus = 'A'
        GROUP BY oct.octordem, otd.otddsc, otd.otdid, otd.otdajuda, otd.arqid, obr.obrid
        ORDER BY oct.octordem
DML;
        $rsDocumentos = $mDocumentos->carregar($sql);
        if(empty($rsDocumentos)){
            return 'Não há documentos para anexar';
        }

        $documentosPendentes = array();
        foreach($rsDocumentos as $documento){
            if($documento['arquivos_anexados'] == 0){
                $documentosPendentes[] = 'O Documento "'.$documento['otddsc'].'" Não possui documentos anexados.';
            }

            if($documento['arquivos_anexados'] != 0 && $documento['arquivos_anexados'] < $documento['otdqtd_minima'] && $documento['otdqtd_minima'] != 0){
                $documentosPendentes[] = 'O Documento "'.$documento['otddsc'].'" precisa ter no mínimo '.$documento['otdqtd_minima'].' documentos anexados.';
            }
        }

        if(!empty($documentosPendentes)){
            return $documentosPendentes;
        }

        $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
        if(!$mAnalise->verificarExistePendenciaDocumentos($obrid)){
            return 'Os itens da Análise de Engenharia não foram corrigidos';
        }
    }

    public function verificaObraQuestionarioPorObra($obrid)
    {
        $mObraQuestionario = new Par3_Model_ObraXQuestionario(null,$obrid,3);
        $sql = "SELECT * FROM par3.obra_x_questionario WHERE obrid = {$this->obrid}";
        $obraQuestionario = $mObraQuestionario->carregar($sql);
        if(empty($obraQuestionario)){
            return 'O relatório de Vistoria não foi preenchido';
        }
        return array_filter($obraQuestionario[0], function($k) {
            return $k == '';
        });
    }

    public function recuperarPerguntasQuestionarioObra($obrid)
    {
        $mObraQuestionario = new Par3_Model_ObraXQuestionario(null,$obrid,3);
        $mOtp = new Par3_Model_ObraTipo($this->otpid);

        $sql = "
            with recursive recursivo(perid, itpid, pertitulo, profundidade) as (
                select distinct on(p1.perid) p1.perid, i1.itpid, p1.pertitulo, 1
                from questionario.pergunta p1
                left join questionario.itempergunta i1 on i1.perid = p1.perid
                left join questionario.grupopergunta grp on grp.grpid = p1.grpid
                where gru_grpid in (
                    select grp.grpid from questionario.grupopergunta gru
                        inner join questionario.grupopergunta grp on grp.gru_grpid = gru.grpid
                        where gru.queid = {$mObraQuestionario->queid}
                    union all
                    select gru.grpid from questionario.grupopergunta gru
                        where gru.queid = {$mObraQuestionario->queid}
                )
                union all
                select distinct on(p1.perid) p1.perid, i1.itpid, p1.pertitulo, r.profundidade+1
                from questionario.pergunta p1
                left join questionario.itempergunta i1 on i1.perid = p1.perid
                inner join recursivo                r on p1.itpid = r.itpid
            )
            select * from recursivo as rec
            left JOIN questionario.resposta res ON res.perid = rec.perid and res.qrpid = {$mObraQuestionario->qrpid}
            where rec.profundidade = 1
            order by rec.pertitulo;
            ";

            $arrQuestoesPendentes = $this->carregar($sql);
        //Dependendo do Questionário não precisa realizar a função recursiva para recuperar as questões
        if(!$arrQuestoesPendentes){
            $sql = "
                SELECT  distinct per.perid,per.pertitulo,* FROM questionario.grupopergunta que
                INNER JOIN questionario.pergunta per ON per.grpid = que.grpid
                WHERE que.queid = {$mObraQuestionario->queid}
                and per.perid NOT IN(
                    --perguntas respondidas
                    SELECT  distinct on (perid) per.perid FROM questionario.grupopergunta que
                    INNER JOIN questionario.pergunta per ON per.grpid = que.grpid
                    INNER JOIN questionario.resposta res ON res.perid = per.perid
                    WHERE que.queid = {$mObraQuestionario->queid}
                    AND res.qrpid = {$mObraQuestionario->qrpid}
                )
                order by per.pertitulo;
            ";
            $arrQuestoesPendentes = $this->carregar($sql);
        }
        return $arrQuestoesPendentes;
    }

    public function detalharPendenciaRelatorioVistoria($obrid)
    {
        if(!$this->verificarRelatorioVistoria($obrid)){
            $mObraQuestionario = new Par3_Model_ObraXQuestionario(null,$obrid,3);
            $mOtp = new Par3_Model_ObraTipo($this->otpid);
            if(!$mObraQuestionario->qrpid){
                return 'O Relatório de Vistoria não foi preenchido';
            }

            $arrQuestoesPendentes = $this->recuperarPerguntasQuestionarioObra($obrid);
            if(empty($arrQuestoesPendentes)){
                return array();
            }
            return array_filter($arrQuestoesPendentes,function($key){
                return(!$key['resid']);
            });
        }

        //VALIDAÇÃO DA ANÁLISE
        $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
        if(!$mAnalise->verificarExistePendencia($obrid,Par3_Model_ObraAba::RELATORIO_VISTORIA)){
            return 'Os itens da Análise de Engenharia não foram corrigidos';
        }
    }

    public function detalharPendenciaPlanilhaOrcamentaria($obrid)
    {
        if(!$this->verificarPlanilhaOrcamentaria($obrid)) {
            global $db;
            $sql = "select distinct * from par3.obra_planilha_orcamentaria where obrid = {$obrid}";
            $rsPlanilha = $db->recuperar($sql);
            if(!$rsPlanilha){
                return 'Não há planilha orçamentária cadastrada para esta Obra';
            }

            $sqlPlanilhaValor = "
        select distinct
            count(oiv.*) as qtd_planejado
        from
            par3.obra obr
        inner join par3.instrumentounidade        inu on inu.inuid = obr.inuid
        inner join par3.instrumentounidade        est on est.estuf = inu.estuf and est.itrid <> 2
        inner join par3.obra_tipo_versao_planilha tpl on tpl.tplid = obr.tplid and tpl.tplstatus = 'A'
        inner join par3.obra_tipo_item_planilha   oip on oip.tplid = tpl.tplid and oip.oipstatus = 'A'
        inner join par3.obra_tipo_item_planilha_valores oiv on oiv.oipid = oip.oipid
            and oiv.oivstatus = 'A'
            and ( tpl.tpltipo_regiao = 'N' or (tpl.tpltipo_regiao = 'E'
            and oiv.inuid = est.inuid) or  (tpl.tpltipo_regiao = 'M'
            and oiv.inuid = inu.inuid) )
        where
            obr.obrid =  {$obrid}
        ";
            $rsPlanilhaValor = $db->recuperar($sqlPlanilhaValor);
            if(!$rsPlanilha['count'] != $rsPlanilhaValor['qtd_planejado']){
                return 'A Planilha está com informações incompletas';
            }
        }
        //VALIDAÇÃO DA ANÁLISE
        $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
        if(!$mAnalise->verificarExistePendencia($obrid,Par3_Model_ObraAba::PLANILHA_ORCAMENTARIA)){
            return 'Os itens da Análise de Engenharia não foram corrigidos';
        }
    }

    public function verificarPreenchimentoDadosTerreno($obrid)
    {
//        //VERIFICAR PENDÊNCIA DE ANÁLISE DE ENGENHARIA
//        $mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
//        $pendencia1 = $mpergunta->verificarAnalisePendencia(Par3_Model_ObraAba::DADOS_TERRENO,1,$_GET['obrid']);
//        $pendencia2 = $mpergunta->verificarAnalisePendencia(Par3_Model_ObraAba::DADOS_TERRENO,2,$_GET['obrid']);
//        $pendencia3 = $mpergunta->verificarAnalisePendencia(Par3_Model_ObraAba::DADOS_TERRENO,3,$_GET['obrid']);
//        if(!$pendencia1 || !$pendencia2 || !$pendencia3){
//            return false;
//        }
        $arrPendencias = array();
        $mObra = new Par3_Model_Obra($obrid);
        if(
            empty($mObra->obrdsc)
            && empty($mObra->ovpid)
            && empty($mObra->tplid)
            && empty($mObra->obrcep)
            && empty($mObra->obrlogradouro)
            && empty($mObra->obrnumero)
            && empty($mObra->obrcomplemento)
            && empty($mObra->obrponto_referencia)
            && empty($mObra->obrbairro)
            && empty($mObra->obrlatitude)
            && empty($mObra->obrlongitude)
        ){
            return false;
        }
        return true;
    }

    public function verificarPreenchimentoFotosTerreno($obrid)
    {
        $mFotosObra = new Par3_Model_ObraFotosTerreno();
        if(empty($mFotosObra->oftdsc) && empty($mFotosObra->arqid)){
            return false;
        }
        return true;
    }


    public function verificarRelatorioVistoria($obrid)
    {
        $mObr = new Par3_Model_Obra($obrid);
        $mOtp = new Par3_Model_ObraTipo($mObr->otpid);
        $mRelatorio  = new Par3_Model_ObraXQuestionario();
        $mObraQuestionario = new Par3_Model_ObraXQuestionario(null,$obrid,3);
//        ver($mObraQuestionario);
        if($mObraQuestionario->obqeid){
            if($mObraQuestionario->beneficiada_fnde == 'S' && $mObraQuestionario->apliacao_mesmo_terreno == 'S' && $mOtp->octid == Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO){
                return true;
            }
            if($mObraQuestionario->beneficiada_fnde == 'S' && $mOtp->octid == Par3_Model_ObraCategoria::CATEGORIA_REFORMA){
                return true;
            }
            if($mObraQuestionario->qrpid){
                $arrQuestoesPendentes = $this->recuperarPerguntasQuestionarioObra($obrid);
                if(!$arrQuestoesPendentes){
                    return true;
                }
                $pendencias =  array_filter($arrQuestoesPendentes,function($key){
                    return(!$key['resid']);
                });
                if(!$pendencias){
                    return true;
                }
            }
        }
        return false;
    }

    public function verificarDocumentoAnexo($obrid)
    {
        $mDocumentos  = new Par3_Model_ObraDocumentos();
        $sql = <<<DML
        SELECT
            count(arq.arqid) as anexos,
            otd.otdqtd_minima
        FROM par3.obra obr
        INNER JOIN par3.obra_tipo 					otp ON otp.otpid = obr.otpid
        INNER JOIN par3.obra_configuracao_aba 		oca ON oca.otpid = otp.otpid AND oca.ocastatus = 'A'
        INNER JOIN par3.obra_confi_aba_x_tipo_doc 	oct ON oct.ocaid = oca.ocaid AND oct.octstatus = 'A'
        INNER JOIN par3.obra_tipo_documento 		otd ON otd.otdid = oct.otdid
        LEFT  JOIN par3.obra_documentos 			odo ON odo.otdid = otd.otdid AND odo.obrid = obr.obrid AND odo.odostatus = 'A'
        LEFT  JOIN public.arquivo 					arq ON arq.arqid = odo.arqid
        WHERE obr.obrid = {$obrid} AND otd.otdsituacao = 'A' AND otd.otdstatus = 'A'
        GROUP BY oct.octordem, otd.otddsc, otd.otdid, otd.otdajuda, otd.arqid, obr.obrid
        ORDER BY oct.octordem
DML;
        $rsDocumentos = $mDocumentos->carregar($sql);
        if(empty($rsDocumentos)){
            return false;
        }
        foreach($rsDocumentos as $documento){
            if($documento['anexos'] == 0 || $documento['anexos'] < $documento['otdqtd_minima']){
                return false;
            }
        }
        return true;
    }

    public function verificarFotosTerreno($obrid)
    {
        $mFotosTerreno  = new Par3_Model_ObraFotosTerreno();
        $rsFotosTerreno = $mFotosTerreno->recuperarTodos('*',array("obrid = {$obrid}","oftstatus ='A' and arqid is not null"));
        $mObraConfAba   = new Par3_Model_ObraConfiguracaoAba();
        $obraAba        = $mObraConfAba->recuperarPorTipoObra($this->otpid);
        if(empty($rsFotosTerreno) || count($rsFotosTerreno) < $obraAba->ocaqtd_minimo || count($rsFotosTerreno)  > $obraAba->ocaqtd_maximo){
            return false;
        }
        return true;
    }

    public function verificarPlanilhaOrcamentaria($obrid)
    {
        global $db;
//        $sqlPlanilha = "
//                        select distinct
//                        count(*)
//                        from
//                            par3.obra_planilha_orcamentaria
//                        where
//                            obrid = {$obrid}
//                            and opoqtd is not null
//                            and opovalor is not null";

        $sqlPlanilhaValor = "
        select distinct
            count(oiv.*) as qtd_planejado
        from
            par3.obra obr
        inner join par3.instrumentounidade        inu on inu.inuid = obr.inuid
        inner join par3.instrumentounidade        est on est.estuf = inu.estuf and est.itrid <> 2
        inner join par3.obra_tipo_versao_planilha tpl on tpl.tplid = obr.tplid and tpl.tplstatus = 'A'
        inner join par3.obra_tipo_item_planilha   oip on oip.tplid = tpl.tplid and oip.oipstatus = 'A'
        inner join par3.obra_tipo_item_planilha_valores oiv on oiv.oipid = oip.oipid
            and oiv.oivstatus = 'A'
            and ( tpl.tpltipo_regiao = 'N' or (tpl.tpltipo_regiao = 'E'
            and oiv.inuid = est.inuid) or  (tpl.tpltipo_regiao = 'M'
            and oiv.inuid = inu.inuid) )
        where
            obr.obrid =  {$obrid}
        and
            obrvalor is null
        ";
//        $rsPlanilha      = $db->recuperar($sqlPlanilha);
        $rsPlanilhaValor = $db->recuperar($sqlPlanilhaValor);
//        ver($rsPlanilha,$rsPlanilhaValor);
//        if($rsPlanilha['count'] == 0){
//            return false;
//        }
        if(count($rsPlanilhaValor) == 0){
            return true;
        }
        return true;
    }

    public function verificarEstudoDemanda($obrid)
    {
        $mObra = new Par3_Model_Obra($obrid);
        if($mObra->otpid){
            return true;
        }
        return false;
    }

    public function validarPendencias($obrid = null,$oabid = null)
    {
        $mObraAba     = new Par3_Model_ObraAba();
        if(!$obrid){
            return false;
        }

        $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
        switch($oabid){
            case $mObraAba::ESTUDO_DEMANDA:           return ($this->verificarEstudoDemanda($obrid) == true && $mAnalise->verificarExistePendencia($obrid,$mObraAba::ESTUDO_DEMANDA) == true);break;
            case $mObraAba::DADOS_TERRENO:            return ($this->verificarPreenchimentoDadosTerreno($obrid) && $mAnalise->verificarExistePendencia($obrid,$mObraAba::DADOS_TERRENO));break;
            case $mObraAba::FOTOS_TERRENO:            return ($this->verificarFotosTerreno($obrid) && $mAnalise->verificarExistePendencia($obrid,$mObraAba::FOTOS_TERRENO));break;
            case $mObraAba::DOCUMENTOS_ANEXOS:        return ($this->verificarDocumentoAnexo($obrid) && $mAnalise->verificarExistePendenciaDocumentos($obrid) == true);break;
            case $mObraAba::PLANILHA_ORCAMENTARIA:    return ($this->verificarPlanilhaOrcamentaria($obrid) && $mAnalise->verificarExistePendencia($obrid,$mObraAba::PLANILHA_ORCAMENTARIA));break;
            case $mObraAba::RELATORIO_VISTORIA:       return ($this->verificarRelatorioVistoria($obrid) && $mAnalise->verificarExistePendencia($obrid,$mObraAba::RELATORIO_VISTORIA));break;
            case $mObraAba::CRONOGRAMA_CONTRAPARTIDA: return ($mAnalise->verificarExistePendencia($obrid,$mObraAba::CRONOGRAMA_CONTRAPARTIDA));break;
            default: return true;break;
        }
    }

    public function verificarPendencias($obrid)
    {
        $mObraAba     = new Par3_Model_ObraAba();
        $mObraAbaConf = new Par3_Model_ObraConfiguracaoAba();
        $arrPendencias = array();
        $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();

        $rsObraAbaConf = $mObraAbaConf->recuperarTodos('oabid',array("otpid = {$this->otpid}","ocastatus = 'A'"));

        $arrObraAba = array_map(function($key){
            return $key['oabid'];
        },$rsObraAbaConf);

        //Verificar Estudo de Demanda
        if ($this->verificarEstudoDemanda($obrid) == false || $mAnalise->verificarExistePendencia($obrid,$mObraAba::ESTUDO_DEMANDA) == false){
            $arrPendencias[] = ['descricao' => 'Estudo de Demanda','label' => 'estudodemanda', 'oabid' => $mObraAba::ESTUDO_DEMANDA];
        }


        if(in_array($mObraAba::RELATORIO_VISTORIA,$arrObraAba)){
            //Verificar Relatório de Vistoria
            if ($this->verificarRelatorioVistoria($obrid) == false || $mAnalise->verificarExistePendencia($obrid,$mObraAba::RELATORIO_VISTORIA) == false){
                $arrPendencias[] = ['descricao' => 'Relatório de Vistoria','label' => 'vistoria', 'oabid' => $mObraAba::RELATORIO_VISTORIA];
            }
        }

        //Verificar Preenchimento de Dados do Terreno
        if($this->verificarPreenchimentoDadosTerreno($obrid) == false || $mAnalise->verificarExistePendencia($obrid,$mObraAba::DADOS_TERRENO) == false){
            $arrPendencias[] = ['descricao' => 'Dados do Terreno','label' => 'dadosterreno', 'oabid' => $mObraAba::DADOS_TERRENO];
        }

        if(in_array($mObraAba::DOCUMENTOS_ANEXOS,$arrObraAba)){
            //Verificar Preenchimento do Documento Anexo
            if($this->verificarDocumentoAnexo($obrid) == false || $mAnalise->verificarExistePendenciaDocumentos($obrid) == false){
                $arrPendencias[] = ['descricao' => 'Documentos Anexos','label' => 'documentosanexos', 'oabid' => $mObraAba::DOCUMENTOS_ANEXOS];
            }
        }

        if(in_array($mObraAba::FOTOS_TERRENO,$arrObraAba)){
            //Verificar Fotos do Terreno
            if($this->verificarFotosTerreno($obrid) == false || $mAnalise->verificarExistePendencia($obrid,$mObraAba::FOTOS_TERRENO) == false){
                $arrPendencias[] = ['descricao' => 'Fotos do Terreno','label' => 'fotosterreno', 'oabid' => $mObraAba::FOTOS_TERRENO];
            }
        }

        if(in_array($mObraAba::PLANILHA_ORCAMENTARIA,$arrObraAba)) {
            //Verificar Planilha Orçamentária
            if ($this->verificarPlanilhaOrcamentaria($obrid) == false || $mAnalise->verificarExistePendencia($obrid,$mObraAba::PLANILHA_ORCAMENTARIA) == false) {
                $arrPendencias[] = ['descricao' => 'Planilha Orçamentária', 'label' => 'planilhaorcamentaria', 'oabid' => $mObraAba::PLANILHA_ORCAMENTARIA];
            }
        }

        if(in_array($mObraAba::CRONOGRAMA_CONTRAPARTIDA,$arrObraAba)) {
            //Verificar Planilha Orçamentária
            if ($mAnalise->verificarExistePendencia($obrid,$mObraAba::CRONOGRAMA_CONTRAPARTIDA) == false) {
                $arrPendencias[] = ['descricao' => 'Cronograma de Contrapartida', 'label' => 'cronogramacontrapartida', 'oabid' => $mObraAba::CRONOGRAMA_CONTRAPARTIDA];
            }
        }
        
        return $arrPendencias;
    }

    /**
     * função utilizada para montar combo com os anos que possuem obras no par
     */
    public function getSqlAnosExistentesObrasPar(){
        $sql = "select distinct obra.obrano as codigo, obra.obrano as descricao from par3.obra as obra where obrano is not null order by obrano asc";
        return $sql;
    }

    public function listarHistoricoTramitacao($arrPost,$docid)
    {
        $sql = <<<DML
SELECT DISTINCT
       hd.hstid,
       ed.esddsc,
       ac.aeddscrealizada,
       us.usunome,
       TO_CHAR(hd.htddata, 'DD/MM/YYYY HH24:MI:SS') as datahora,
       hd.docid,
       un.unidsc,
       cd.cmddsc,
       COALESCE(cd.cmddsc, '-') AS cmddsc,
      (select aegid from par3.analise_engenharia_obra where hstid = hd.hstid limit 1) as aegid
    FROM workflow.historicodocumento hd
    INNER JOIN workflow.acaoestadodoc ac ON (ac.aedid = hd.aedid)
    INNER JOIN workflow.estadodocumento ed ON (ed.esdid = ac.esdidorigem)
    INNER JOIN seguranca.usuario us ON (us.usucpf = hd.usucpf)
    LEFT  JOIN workflow.comentariodocumento cd ON (cd.hstid = hd.hstid)
    LEFT  JOIN public.unidade un USING(unicod)
  WHERE hd.docid = {$docid}
    group by
        hd.hstid,
        ed.esddsc,
        ac.aeddscrealizada,
        us.usunome,
        hd.htddata,
        hd.docid,
        un.unidsc,
        cd.cmddsc,
        cd.cmddsc,
        aegid
  ORDER by
        hd.hstid asc,
        ed.esddsc,
        ac.aeddscrealizada,
        us.usunome,
        datahora asc,
        hd.docid,
        un.unidsc,
        cd.cmddsc,
        aegid
DML;

        $listagem = $arrPost['requisicao'] == 'imprimir'? new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO): new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);

        $arrEsconder  = $arrPost['requisicao']  == 'imprimir'? array('docid', 'unidsc','hstid','aegid'): array('docid', 'unidsc','cmddsc','aegid');
        $arrCabecalho = array('Onde Estava', 'O que aconteceu', 'Quem fez', 'Quando fez');
        if($arrPost['requisicao']  == 'imprimir'){
            $arrCabecalho[] = 'Comentário';
        }
//        $listagem->addCallbackDeCampo('aeddscrealizada', 'alinhaParaEsquerda')
//            ->addCallbackDeCampo('esddsc', 'formatarEstadoDocumento');
//        $listagem->addCallbackDeCampo('unicod', 'formatarLocalizacao');
        $listagem->setCabecalho($arrCabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('cmddsc','utf8Decode');
        $listagem->setId('historicotramitacao');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        if($arrPost['requisicao']  != 'imprimir') {
            $listagem->addAcao('comment', 'historicoBootstrapComentario')
                ->setAcaoComoCondicional('comment', array(array('op' => 'diferente', 'campo' => 'cmddsc', 'valor' => '-')));
            $listagem->addAcao('print2', ['func' => 'imprimirAnalise','extra-params' => ['aegid']])
                ->setAcaoComoCondicional('print2', array(array('op' => 'diferente', 'campo' => 'aegid', 'valor' => '')));
        }
        $listagem->esconderColunas($arrEsconder);
        $tabela = $listagem->render();
        return $tabela;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $arrPost
     * @type array
     * @description Função utilizada para retornar o SQL que mosta um 'Simec_Listagem' na lista de obras
     * @return string
     */
    public function getSqlListagemObras($arrayPost = null, $boolEsdidCadastramento = true ,$impressao = false){

        $fluxoObraspar = PAR3_WORKFLOW_FLUXO_OBRAS;
        $where = $this->createSQLFiltroObras($arrayPost, $boolEsdidCadastramento); //monta WHERE
        //if($where != ''){ $existeWHERE = " WHERE ".$where;}else{$existeWHERE = '';}
        $innerTecnico = '';
        if(!empty($arrayPost['tecnicoanalista']) && isset($arrayPost['tecnicoanalista'])) {
            $innerTecnico = "
                INNER JOIN par3.analise_engenharia_obra aeg on aeg.obrid = obra.obrid
                INNER JOIN seguranca.usuario usu on usu.usucpf = aeg.usucpf
            ";

        }
        $dadosEstadoDocumento = '';
        if($impressao){
            $dadosEstadoDocumento = "
                    COALESCE((select to_char(max(htddata),'DD/MM/YYYY') from workflow.historicodocumento where docid = obra.docid),
                     			(select to_char(max(docdatainclusao),'DD/MM/YYYY') from workflow.documento where docid = obra.docid)
                     			) as tramitaca,
                    COALESCE((SELECT usunome FROM(
						SELECT max(htddata), u.usunome from workflow.historicodocumento hd
							INNER JOIN seguranca.usuario u ON u.usucpf = hd.usucpf
						WHERE docid = obra.docid
						GROUP BY u.usunome LIMIT 1
					) AS foo),
					(SELECT usunome FROM(
							SELECT max(hd.hobid), u.usunome FROM par3.obra_historico hd
								INNER JOIN seguranca.usuario u ON u.usucpf = hd.hobcpf
							WHERE hd.obrid = obra.obrid
							GROUP BY u.usunome LIMIT 1
						) AS foo)
					) AS ultimo_analista,
                    case when obra2.docid is not null then obra2.obrid::VARCHAR else ' ' end as obra2,
                    case when obra2.docid is not null
                    then (
                            select ed.esddsc from workflow.documento documento
                            inner join workflow.estadodocumento ed ON ed.esdid = documento.esdid
                            where documento.docid = obra2.docid
                        )else ' ' end as obra2_esddsc,
                    ";
        }


        $sql = "
                SELECT
                    DISTINCT
                    inu.inuid as unidade,
                    obra.obrid as obrcheck,-- ID da obra para tramitar em lote
                    obra.inpid as iniciativapla,
                    obra.obrid as edit, --ID da obra para o btn editar
                    obra.obrid as preid,-- ID da obra
                    obra.obrdsc, --Descrição do tipo de obra
                    obra.obrid||'#%#'||obra.inuid||'#%#'||obra.inpid||'#%#'||obra.obrid||'#%#'||obra.obrdsc as linkeditarobr,
                    obra_tipo.otpdsc, -- Descrição do tipo de obra
                    case
                        when inu.itrid = 1 then  'E'
                        when inu.itrid = 2 then 'M'
                        when inu.itrid = 3 then 'F'
                    end as iniesfera, --esfera
                    municipio.mundescricao, -- Descrição do Município
                    estado.estuf as estuf, -- Sigla Estado
                    (select esc.escnome from par3.escola as esc
                        inner join par3.obra_x_questionario as obrque on obrque.escid = esc.escid
                        where obrque.obrid = obra.obrid	limit 1) as escnome, -- Nome da escola
                    esd.esddsc as situacao_estado_documento,
                    obra.docid,
                    {$dadosEstadoDocumento}
                    obra2.obrid||'#%#'||obra2.docid as linkobra2,
                    processo.pronumeroprocesso  AS \"Processo\",
                   -- ' - ' convenio,
                    (select etadsc from par3.iniciativa_planejamento as inpetapa
                    inner join par3.obra as obraetapa on obraetapa.inpid = inpetapa.inpid
                    inner join par3.ensino_etapa as enseta on enseta.etaid = inpetapa.etaid
                    where obraetapa.obrid = obra.obrid) as etapa,
                    obra.obrano,
                    COALESCE(obra.obrvalor_contrapartida,0) as obrvalor_contrapartida,
                    (obra.obrvalor - COALESCE(obra.obrvalor_contrapartida,0)) as valor_fnde,
                    obra.obrvalor
                    ,esd.esdid as situacao_estado_documento_id,
                     documentotermo.dotid
                    ,coalesce(vpp.vlrpago,0) as vlrpago
                    ,documentotermo.dotnumero||'-'||documentotermo.intoid AS \"Termo\"
                    ,vlrempenho AS \"Empenho\"
                FROM par3.obra as obra
                    {$innerTecnico}
                    INNER JOIN par3.obra_tipo as obra_tipo on obra.otpid = obra_tipo.otpid
                    INNER JOIN par3.instrumentounidade as inu on inu.inuid = obra.inuid
                    INNER JOIN workflow.documento as doc on doc.docid = obra.docid
                    INNER JOIN workflow.estadodocumento as esd on esd.esdid = doc.esdid
                    INNER JOIN territorios.estado as estado on estado.estuf = inu.estuf
                    LEFT JOIN territorios.municipio as municipio on inu.muncod = municipio.muncod
                    INNER JOIN par3.iniciativa_planejamento as inp on inp.inpid = obra.inpid and inp.inpstatus = 'A'
                    JOIN par3.iniciativa as ini on ini.iniid = inp.iniid
                    INNER JOIN par3.ensino_etapa as etapa on etapa.etaid = inp.etaid
                    INNER JOIN par3.modalidade as modalidade on modalidade.modid = inp.modid
                    LEFT JOIN (
                      SELECT
                          ve.proid,
                          ve.obrid,
                          sum(ve.saldo) AS vlrempenho,
                          sum(ve.vlrpago) AS vlrpago
                      FROM par3.v_saldo_empenho_por_obra ve
                      GROUP BY ve.obrid, ve.proid
                    ) as vpp on vpp.obrid = obra.obrid
                    LEFT JOIN (
                      select distinct
                          tec.obrid,
                          dot.proid,
                          dot.dotid,
                          dot.dotnumero,
                          dot.intoid,
                          dot.dotstatus
                      from par3.documentotermo dot
                          left join par3.termocomposicao tec on tec.dotid = dot.dotid
                      where dot.dotstatus = 'A'
                      and tec.tecstatus   = 'A'
                    ) documentotermo on documentotermo.obrid = obra.obrid AND documentotermo.proid = vpp.proid
                    LEFT JOIN (
                        select DISTINCT pro.pronumeroprocesso,poc.obrid
                        from par3.processoobracomposicao poc
                        inner join par3.processo pro using (proid)
                        where poc.pocstatus = 'A'
                    )processo on processo.obrid = obra.obrid
                    LEFT JOIN (
                        SELECT ve.obrid, p.pagsituacaopagamento as situacao, p.empid
						FROM par3.v_saldo_empenho_composicao_obra ve
							INNER JOIN par3.pagamentoobracomposicao pac ON pac.eocid = ve.eocid AND pac.pmcstatus = 'A'
							INNER JOIN par3.pagamento p ON p.pagid = pac.pagid
						WHERE p.pagstatus = 'A'
                    ) pagamento ON pagamento.obrid = obra.obrid
                    LEFT JOIN obras2.obras obra2 on obra2.obrid_par3 = obra.obrid and (obra2.obrstatus = 'A' or obra2.obridpai is not null)
                WHERE obra.obrstatus = 'A'
                    AND esd.tpdid = 304 ";
        /* Se existir algum WHERE, é montado junto ao SQL da query */
        $sql .= $where."\n" ;
//        $sql .="  ORDER BY estado.estuf,municipio.mundescricao,obra.obrano,esd.esddsc,tramitaca,obra.obrid ASC;";
        $sql .= " group by obra.obrid,inu.inuid,obra_tipo.otpdsc,municipio.mundescricao, estado.estuf, esd.esddsc,processo.pronumeroprocesso,esd.esdid,vlrempenho,obra2.obrid,documentotermo.dotid, vpp.vlrpago,documentotermo.dotnumero,documentotermo.intoid, obra2.docid";
        $sql .="  ORDER BY estado.estuf,municipio.mundescricao,obra.obrano,esd.esddsc,obra.obrid ASC;";
        return $sql;
    }
//
//LEFT JOIN par3.processoobracomposicao processoobracomposicao ON processoobracomposicao.obrid = obra.obrid AND processoobracomposicao.pocstatus = 'A'
//LEFT JOIN par3.empenhoobracomposicao empenhoobracomposicao ON empenhoobracomposicao.pocid = processoobracomposicao.pocid AND empenhoobracomposicao.eocstatus = 'A'
//LEFT JOIN par3.empenho empenho ON empenho.empid = empenhoobracomposicao.empid AND empenho.empstatus = 'A'
//LEFT JOIN par3.processo processo ON processo.proid = processoobracomposicao.proid
//LEFT JOIN par3.documentotermo documentotermo ON documentotermo.proid = processo.proid
//LEFT JOIN ( SELECT SUM(pac.pmcvalorpagamento) AS vlrpago,
//pac.eocid,
//pag_1.empid,
//pag_1.pagsituacaopagamento::text as situacao
//FROM par3.pagamento pag_1
//JOIN par3.pagamentoobracomposicao pac ON pac.pagid = pag_1.pagid AND pac.pmcstatus = 'A'::bpchar
//WHERE
//pag_1.pagstatus = 'A'::bpchar
//GROUP BY pac.eocid, pag_1.empid,  pag_1.pagsituacaopagamento
//) pagamento ON pagamento.empid = empenho.empid AND pagamento.eocid = empenhoobracomposicao.eocid

    /**
     *
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param array $arrayPOST
     * @return string
     * @description Esta função monta o WHERE da listagem de obras
     */
    public function createSQLFiltroObras($arrayPOST = null, $boolEsdidCadastramento = true ){
        /*
            Pre-Obra => obrid
            Tipo de Obra => otpid*/
        $where = '';
        $WHERE_EXISTS = 1;
        if(is_array($arrayPOST)){
            /*Limpa os arrays dos valores nulos*/
            $arrFromPostPreid=(isset($arrayPOST['preid']) && $arrayPOST['preid'] != '')?array_values(array_filter(explode(',',$arrayPOST['preid']))):null;
            //$arrayFromPostEstuf=(isset($arrayPOST['estuf']))?array_values(array_filter($arrayPOST['estuf'])):null;
            //$arrayFromMunicipio=(isset($arrayPOST['muncod']))?array_values(array_filter($arrayPOST['muncod'])):null;
            $arrayFromEstadoDocumento=(isset($arrayPOST['esdid']))?array_values(array_filter($arrayPOST['esdid'])):null;
            $arrayFromObraTipo=(isset($arrayPOST['otpid']))?array_values(array_filter($arrayPOST['otpid'])):null;
            $arrayFromObraCategoria=(isset($arrayPOST['octid']))?array_values(array_filter($arrayPOST['octid'])):null;
            $arrayEnsinoEtapa=(isset($arrayPOST['etaid']))?array_values(array_filter($arrayPOST['etaid'])):null;
            $arrayModalidade=(isset($arrayPOST['modid']))?array_values(array_filter($arrayPOST['modid'])):null;
            $arrayAgrupador=(isset($arrayPOST['agrid']))?array_values(array_filter($arrayPOST['agrid'])):null;
            $arrayProcesso=(isset($arrayPOST['pronumeroprocesso']))?($arrayPOST['pronumeroprocesso']):null;
            $arrayNumeroTermo=(isset($arrayPOST['dotnumero']))?($arrayPOST['dotnumero']):null;
            $arrayTermoStatus=(isset($arrayPOST['dotstatus']))?($arrayPOST['dotstatus']):null;
            $arrayEmpenhoStatus=(isset($arrayPOST['empenhosituacao']))?($arrayPOST['empenhosituacao']):null;
            $arrayPagamentoSituacao=(isset($arrayPOST['pagamento']))?($arrayPOST['pagamento']):null;
            $arrayTipoAssistencia=(isset($arrayPOST['intaid']))?$arrayPOST['intaid']:null;
            $obrid2=(isset($arrayPOST['obrid2']))?$arrayPOST['obrid2']:null;
            /* Situação do pagamento */
            if( isset($arrayPagamentoSituacao) && $arrayPagamentoSituacao != '' ){

                if( $WHERE_EXISTS > 0 ){
                    $where .= ' AND ';
                }

                if( in_array('enviado ao sigef', $arrayPagamentoSituacao)){
                    $where .= " (pagamento.situacao ILIKE '%8 - SOLICITAÇÃO APROVADA%' OR pagamento.situacao ILIKE '%Enviado ao SIGEF%')";
                    if(end($arrayPagamentoSituacao) != 'enviado ao sigef'){
                        $where .= ' OR ';
                    }
                }

                if( in_array('não solicitado', $arrayPagamentoSituacao) ){
                    $where .= "pagamento.situacao ILIKE '%não solicitado%'";
                    if(end($arrayPagamentoSituacao) != 'não solicitado') {
                        $where .= ' OR ';
                    }
                }

                if( in_array('efetivado', $arrayPagamentoSituacao) ){
                    $where .= "pagamento.situacao ILIKE '%efetivado%'";
                    if(end($arrayPagamentoSituacao) != 'efetivado') {
                        $where .= ' OR ';
                    }
                }

                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' Status Termo "Gerado, Não Gerado e Todos" */
            if(isset($arrayEmpenhoStatus) && $arrayEmpenhoStatus != ''){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                if( $arrayEmpenhoStatus == 'A'){
                    $where .= " vlrempenho IS NOT NULL \n";
                } else {
                     $where .= " vlrempenho IS NULL \n";
                }
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' Status Termo "Gerado, Não Gerado e Todos" */
            if(isset($arrayTermoStatus) && $arrayTermoStatus != ''){
                if($arrayTermoStatus == 'A'){
                    $validacao = 'is not null AND documentotermo.dotstatus = \'A\'';
                }else{
                    $validacao = 'is null';
                }

                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $where .= " documentotermo.dotnumero {$validacao}  \n";
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' Nº Processo */
            if (isset($arrayProcesso) && $arrayProcesso != '') {
                // remove qualquer caracter não numérico
                $arrayProcesso = preg_replace('/\D+/', '', $arrayProcesso);

                if ($WHERE_EXISTS > 0) {
                    $where .= ' AND ';
                }
                $where .= " processo.pronumeroprocesso ILIKE '%{$arrayProcesso}%'  \n";
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' Nº Termo */
            if(isset($arrayNumeroTermo) && $arrayNumeroTermo != ''){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $dotnumero = preg_replace('/\D+/', '', $arrayNumeroTermo);
                $where .= " documentotermo.dotnumero = '{$dotnumero}'  \n";
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' pré-obra */
            if(is_array($arrFromPostPreid) && count($arrayPOST['preid']) > 0 ){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $obrasId = implode(",",$arrFromPostPreid);
                $where .= "obra.obrid IN({$obrasId}) \n";
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' do tipo de obra */
            if(is_array($arrayFromObraTipo) && count($arrayFromObraTipo) > 0){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $obratipoImplode = implode(',',$arrayFromObraTipo);
                $where .= "obra_tipo.otpid IN({$obratipoImplode})  \n";
                $WHERE_EXISTS++;
            }

            if($_POST['octid'] && count($_POST['octid']) != 0){
                $arrOct = array_filter($_POST['octid'],function($val){return !empty($val);});
                if(count($arrOct) != 0){
                    if($WHERE_EXISTS > 0){
                        $where .= ' AND ';
                    }
                    $where  .= "obra_tipo.octid in (".implode(',',$arrOct).")";
                    $WHERE_EXISTS++;
                }
            }

            if( $arrayPOST['estuf'] ){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
               // $estadosIN = retornValuesStringINSQL($arrayFromPostEstuf);
                if(is_array($arrayPOST['estuf'])){
                    $where .="estado.estuf in (".implode(',',array_map(function($estuf){return "'$estuf'";},$arrayPOST['estuf'])).")\n";
                }else{
                    $where .="estado.estuf = '{$arrayPOST['estuf']}'\n";
                }
                $WHERE_EXISTS++;
            }
            if( $arrayPOST['muncod'] ){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                if(is_array($arrayPOST['muncod'])){
                    $where  .= "municipio.muncod in (".implode(',',array_map(function($muncod){return "'$muncod'";},$arrayPOST['muncod'])).")  \n";

                }else{
                    $where  .= "municipio.muncod = '{$arrayPOST['muncod']}'  \n";
                }
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' das obras de acordo com o ano desejado */
            if(isset($arrayPOST['obrano']) && $arrayPOST['obrano'] != ''){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $arrAno = array_values(array_filter(explode(',',$arrayPOST['obrano'])));
                if(count($arrAno) > 0){
                    $stringAno = implode(',',$arrAno);
                }else{
                    $stringAno = str_replace(',','',$arrayPOST['obrano']);
                }
                $where .= " obra.obrano IN ({$stringAno})  \n";
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' situação (estadodocumento esdid) */
            if(is_array($arrayFromEstadoDocumento) && count($arrayFromEstadoDocumento)){

                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }

                $esdIN =  implode(',',$arrayFromEstadoDocumento);
                $where  .= "esd.esdid IN({$esdIN}) \n ";
                $WHERE_EXISTS++;

            // Não informou omite o estado "Em cadastramento" se boolEsdidCadastramento = true
            } else {

                if( $boolEsdidCadastramento ) {
                    if ($WHERE_EXISTS > 0) {
                        $where .= ' AND ';
                    }
                    $where .= "esd.esdid <> " . $this::ESDID_EM_CADASTRAMENTO . " \n ";
                    $WHERE_EXISTS++;
                }
            }

            /* Monta o 'WHERE' Técnicos - analises efetuadas (usucpf)*/
            if(isset($arrayPOST['tecnicoanalista']) && $arrayPOST['tecnicoanalista'] != ''){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }

                $where .= " usu.usucpf = '{$arrayPOST['tecnicoanalista']}' ";
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' Esfera: (ini.iniesfera) se for selecionado filtro TODOS, nem entra no WHERE*/
            if(isset($arrayPOST['iniesfera']) && $arrayPOST['iniesfera'] != '' && $arrayPOST['iniesfera'] != 'T'){
                $qualEsfera ='';
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $where .= " ini.iniesfera = '{$arrayPOST['iniesfera']}'  \n";
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' Etapa etaid*/
            if(is_array($arrayEnsinoEtapa) && count($arrayEnsinoEtapa) > 0){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $ensinoEtapa = implode(',',$arrayEnsinoEtapa);
                $where .= " etapa.etaid in({$ensinoEtapa}) \n";
                $WHERE_EXISTS++;
            }

            //FILTRO DO TIPO DE ASSISTÊNCIA
            if($arrayTipoAssistencia){
                switch ($arrayTipoAssistencia){
                    case 1:
                        $where .= "
                        AND obra.obrid not in (
                            SELECT obrid FROM par3.iniciativa_emenda_obra ieo
                            inner join par3.iniciativa_emenda ine using(ineid)
                            where ieo.ieostatus = 'A'
                            and ieo.ieovalor > 0
                            and ieo.obrid = obra.obrid
                            and ine.inestatus = 'A'
                        ) \n";
                        break;
                    case 2:
                        $where .= "AND obra.obrid in (
                        select ieo.obrid FROM par3.iniciativa_emenda_obra ieo
                        inner join par3.iniciativa_emenda ine using(ineid)
                        where ieo.ieostatus = 'A'
                        and ieo.ieovalor > 0
                        and ieo.obrid = obra.obrid
                        and ine.inestatus = 'A'
                        ) \n";
                        break;
                    default:
                        $where .='';
                        break;
                }
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' Modalidade modid*/
            if(is_array($arrayModalidade) && count($arrayModalidade) > 0){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $modalidades = implode(',',$arrayModalidade);
                $where .= " modalidade.modid in({$modalidades}) \n";
                $WHERE_EXISTS++;
            }

            /* Monta o 'WHERE' com o técnico analista desejado */
            if(isset($arrayPOST['usucpf']) && $arrayPOST['usucpf'] != ''){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $where .= " usu.hobcpf = '{$arrayPOST['usucpf']}'  \n";
                $WHERE_EXISTS++;
            }

            /*Monta o 'WHERE' com o preid das obras  */
            if(isset($arrayAgrupador) && count($arrayAgrupador) > 0){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $modelAgrupador = new Par3_Model_Agrupador();
                $where .= " obra.obrid IN ({$modelAgrupador->getIdObrasDoAgrupadorByIdInSQL($arrayAgrupador)})  \n";
                $WHERE_EXISTS++;
            }

            if(isset($arrayPOST['inuid']) && $arrayPOST['inuid'] != ''){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                $where .= " inu.inuid = {$arrayPOST['inuid']}  \n";
                $WHERE_EXISTS++;
            }

            if(isset($obrid2) && $obrid2 != '' && $obrid2 !='T'){
                if($WHERE_EXISTS > 0){
                    $where .= ' AND ';
                }
                if( $obrid2 == 'S'){
                    $where .= " obra2.obrid IS NOT NULL \n";
                } else {
                    $where .= " obra2.obrid IS NULL \n";
                }
                $WHERE_EXISTS++;
            }
        }
        return $where;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $sql string
     * @description Esta função retorna a quantidade de linhas que o sql passado na assinatura do método retorna
     * @return int
     */
    public function getCountResultSql($sql){
        $executa = $this->executar($sql);
        $pgNumRows = (($this->conta_linhas($executa)) + 1);
        return $pgNumRows; //função conta linhas retorna QT de linhas - 1
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $preids
     */
    public function listaObrasIsValid($preids){
        $result = 'true';
        $arrPreIds  = '';
        $exists = 0;
        $listObraNaoCadastrada = null;
        if(!is_array($preids)){
           $arrPreIds = explode(',',$preids);
        }else{
            $arrPreIds = $preids;
        }

        //verifica se as obras são válidas
        foreach($arrPreIds as $arr){
            $result = $this->obraIsValid($arr);
            if(!isset($result['obrid'])){
               $listObraNaoCadastrada .= "Pre-Obra ID: ".$arr." Não Cadastrado \n";
            }
        }

        if($listObraNaoCadastrada != null){
            return $listObraNaoCadastrada;
        }else{
            return 'true';
        }
    }

    /**
     *
     */
    public function obraIsValid($obrid){
        $sql = "select obrid from par3.obra where obrid = {$obrid};";
        $result = $this->pegaLinha($sql);
        return $result;

    }

    public function create($obraid){
        $sql  = "select * from par3.obra as obra
                  inner join par3.obra_tipo as obratipo on obratipo.otpid = obra.otpid
                  inner join par3.obra_categoria as obracat on obracat.octid = obratipo.octid
                where obra.obrid = {$obraid}";
        try{
        $result = $this->pegaLinha($sql);
        return $result;
        }catch(Exception $e){
            throw new Exception($e->getCode()." - ".$e->getMessage());
        }
    }

    public function habilitarEdicaoPorPerfilEstadoDocumento($obrid)
    {
        $mObra = new Par3_Model_Obra($obrid);
        if(!$mObra->obrid){
            return true;
        }
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);

        $arrPerfisSuperUsuario = [
            Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
            Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR
        ];
        if(array_intersect($pflid,$arrPerfisSuperUsuario)){
            return true;
        }

        $perfisHabilitados       = [
            Par3_Model_UsuarioResponsabilidade::PREFEITO ,
            Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
            Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL,
            Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST,
        ];

        $arrSituacoesHabilitadas = [
            $this::ESDID_EM_CADASTRAMENTO,
            $this::ESDID_EM_DILIGENCIA,
            $this::ESDID_EM_CADASTRAMENTO_CONDICIONAL,
        ];

        if(array_intersect($pflid,$perfisHabilitados) && in_array($esd['esdid'],$arrSituacoesHabilitadas)){
            return true;
        }
        return false;
    }

    public function habilitarEdicaoPorPerfil()
    {
        require_once APPRAIZ . '/includes/workflow.php';
        $esd = wf_pegarEstadoAtual( $mObra->docid);
        $pflid = pegaPerfilGeral($_SESSION['usucpf']);
        $mAnalise = new Par3_Model_AnaliseEngenhariaObra();
        $arrPerfisSuperUsuario = [Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR];
        if(array_intersect($pflid,$arrPerfisSuperUsuario)){
            return true;
        }
        $perfisHabilitados = [Par3_Model_UsuarioResponsabilidade::PREFEITO ,Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL];
        if(array_intersect($pflid,$perfisHabilitados) && in_array($esd['esdid'],$mAnalise->estadosCorrecaoAnalise())){
            return true;
        }
        return false;
    }
    
    public function verificaEdicaoObras( $obrid ){
    	$sql = "select o.obrdsc, o.obrano, d.esdid from par3.obra o
					inner join workflow.documento d on d.docid = o.docid
				where o.obrid = $obrid";
    	$arDados = $this->pegaLinha($sql);
    	
    	if( $arDados['esdid'] == $this::ESDID_EM_CADASTRAMENTO && $arDados['obrano'] < date('Y') ){
    		
    		$urlA = 'window.location.href = "'.$_SERVER['HTTP_REFERER'].'"';
    		
    		echo '<script>
						swal({
			                title: "Confirmar",
			                text: "Não é possível editar a obra <b>('.$arDados['obrdsc'].')</b> para o ano <b>'.$arDados['obrano'].'</b>, deseja enviá-la para o ano de <b>'.date('Y').'</b>?",
			                type: "success",
			                html: true,
			                showCancelButton: true,
			                cancelButtonText: "Não",
			                confirmButtonText: "Sim",
			                closeOnConfirm: false
			            }, function (isConfirm) {
			                if (isConfirm) {
			                    js_alterarAnoObra( '.$obrid.' );
			                } else {
			                	'.$urlA.'
			                }
			            }
			        );
			</script>';
    	} else {
    		return true;
    	}
    }

     /**
     * Pegar ultimo comentário de tramitação para apresentar na aba "Enviar para análise"
     * quando a situação for uma situação "final":
     * Obra Aprovada
     * Obra Arquivada
     * Obra Cancelada
     * Obra Indeferida
     */
    public function pegarDadosComentarioSituacaoFinal($obrid){


        $sql = "
            SELECT
                ed.esddsc,
                cdoc.cmddsc,
                TO_CHAR(cdoc.cmddata, 'DD/MM/YYYY') AS datacomentario
            FROM
                workflow.documento doc
                INNER JOIN par3.obra obr ON obr.docid = doc.docid
                INNER JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
                INNER JOIN workflow.historicodocumento hist ON hist.docid = doc.docid
                INNER JOIN workflow.comentariodocumento cdoc ON cdoc.hstid = hist.hstid
            WHERE
                obr.obrid = {$obrid} AND
                ed.esdid IN (2061,2053,2050,2052)
            ORDER BY
                hist.htddata DESC
            LIMIT 1
            ";
        //monta a tabela com a query
        $listagem =  new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);


        $arrCabecalho = array('Situação', 'Comentário', 'Data');
        
        $listagem->setCabecalho($arrCabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('cmddsc','utf8Decode');

        return $listagem;

    }

    /**
     * Pegar Estado Anterior para tramitacao reversa. A ação deve voltar os estados continuamente (e nao ficar em loop)
     * esdiddestino -> estado anterior ao atual de acordo com o fluxo (e de acordo com o histórico feito pelo usuário)
     */
    public function pegarDadosEstadoAnterior($docid, $esdidAtual){

        $resultado = array();
        $esdHistorico = array();
        $acoesPossiveis = array();

        //recupera os esdids pelo historico para saber qual deles será o destino
        $sql = "SELECT
			a.esdidorigem AS esdid,
			max(hd.htddata), --apenas para não colocar no group by
			ed.esddsc AS esddscdestino,
			eo.esddsc AS esddscorigem
		FROM workflow.historicodocumento hd
		JOIN workflow.acaoestadodoc a ON a.aedid = hd.aedid
		INNER JOIN workflow.estadodocumento eo ON eo.esdid = a.esdidorigem
		INNER JOIN workflow.estadodocumento ed ON ed.esdid = a.esdiddestino
		WHERE
			hd.docid = {$docid}
	    AND a.aeddscrealizar NOT LIKE '%Retornar%'
	    AND a.esdidorigem <> {$esdidAtual}
	    GROUP BY a.esdidorigem, ed.esddsc, eo.esddsc
		ORDER BY 2 DESC";
        $esdHistorico = $this->carregar($sql);

        //pegar açoes de retorno possiveis do estado atual
        $sql = "SELECT
				a.aedid,
				a.esdidorigem,
				a.esdiddestino,
				a.aeddscrealizar,
				a.aeddscrealizada,
				ed.esddsc AS esddscdestino,
				eo.esddsc AS esddscorigem
			FROM workflow.acaoestadodoc a
				INNER JOIN workflow.estadodocumento eo ON eo.esdid = a.esdidorigem
				INNER JOIN workflow.estadodocumento ed ON ed.esdid = a.esdiddestino
			WHERE
				ed.esdstatus   = 'A' AND
				a.aedstatus    = 'A' AND
				a.aeddscrealizar ILIKE '%retornar%' AND
				a.esdidorigem  = {$esdidAtual}";

        $acoesPossiveis = $this->carregar($sql);

        if(!empty($esdHistorico) && !empty($acoesPossiveis)) {
            $sair = false;
            foreach ($esdHistorico as $key => $linhaHist) {
                foreach ($acoesPossiveis as $acao) {
                    if ($linhaHist['esdid'] == $acao['esdiddestino']) {
                        $resultado['aedid'] = $acao['aedid'];
                        $resultado['esddscdestino'] = $acao['esddscdestino'];
                        $sair = true; //para sair do loop externo
                        break;
                    }
                }
                if ($sair) break;
            }
        }

        return $resultado;
    }

    public function carregarDadosExtratoObraDadosTerreno($obrid)
    {
        $sqlDadosTerreno = "
        select  ovp.ovpdsc,
        from par3.obra obr
        inner join par3.obra_tipo_versao_projeto ovp using(ovpid)
        where obr.obrid = {$obrid}
        ";
        $dadosTerreno = $this->recuperar($sqlDadosTerreno);
        return $dadosTerreno;

    }

    public function carregarDadosExtratoObraEstudoDemanda($obrid)
    {
        $sqlEstudoDemanda = "
                select distinct
                obr.obrid,
                otp.otpdsc,
                obr.obrdsc,
                omo.omodsc,
                omo.omoid,
                oed.*,
                oem.oemdsc,
                oed.oedapliacao_justificativa,
                case when oed.oedquadra = 'Q' then 'Quadra'
                     when oed.oedquadra = 'C' then 'Cobertura'
                     when oed.oedquadra = 'N' then 'Não informado'
                end as tipo_ampliacao,
                
                case when oed.oedturnos_atendimento = 'I' then 'Integral'
                     when oed.oedturnos_atendimento = 'D' then 'Dois Turnos'
                end as turno_atendimento,
                
                case when oed.oednoturno = 'S' then 'Sim'
                     when oed.oednoturno = 'N' then 'Não'
                end as turno_noturno,
                
                case when oed.oedremanejamento_justificativa = 'S' then 'Sim'
                     when oed.oedremanejamento_justificativa = 'N' then 'Não'
                end as justificativa_remanejamento
                
                from par3.obra obr
                inner join par3.obra_tipo otp using(otpid)
                inner join par3.obra_estudo_demanda              oed       using (obrid)
                inner join par3.obra_estudo_demanda_modalidade   oem       using (oemid)
                inner join par3.obra_estudo_demanda_motivo       omo       using (omoid)
                where obrid = {$obrid}
        ";

        $estudoDemanda = $this->recuperar($sqlEstudoDemanda);

        if($estudoDemanda['omoid'] == 1){
            $sqlEscolas = "
            select esc.*,oed_x_esc.* from par3.obra_estudo_demanda_x_escola oed_x_esc
            inner join par3.escola esc              using(escid)
            inner join par3.obra_estudo_demanda oed using(oedid)
            where oed.obrid = {$obrid}
            ";
            $estudoDemanda['escolas'] = $this->carregar($sqlEscolas);
        }

        if($estudoDemanda['oedquadra'] == 'A'){
            $sqlOutrasAmpliacoes = "
                    select
                    obr.obrid,
                    array_to_string(array_agg(eda.edadsc),',') as demanda_ampliacao
                    from par3.obra obr
                    inner join par3.obra_tipo otp using(otpid)
                    inner join par3.obra_estudo_demanda              oed       using (obrid)
                    inner join par3.obra_estudo_demanda_x_ampliacao  oed_x_eda using (oedid)
                    inner join par3.obra_estudo_demanda_ampliacao    eda       using (edaid)
                    where obrid =  {$obrid}
                    group by obrid
            ";
            $estudoDemanda['outras_ampliacoes'] = $this->recuperar($sqlOutrasAmpliacoes);
        }

        return $estudoDemanda;
    }


    public function recuperaEmpenhoTermoProcessoPorObra($obrid)
    {
        $sql="SELECT
                DISTINCT
                processo.pronumeroprocesso as processo,
                array_to_string(array_agg(documentotermo.dotnumero||'-'||documentotermo.intoid),',') as termo,
                vlrempenho AS empenho
            FROM par3.obra as obra
                LEFT JOIN (
                    SELECT ve.proid, ve.obrid, sum(ve.saldo) AS vlrempenho, sum(ve.vlrpago) AS vlrpago FROM par3.v_saldo_empenho_por_obra ve GROUP BY ve.obrid, ve.proid
                ) as vpp on vpp.obrid = obra.obrid
                
                LEFT JOIN par3.documentotermo documentotermo ON documentotermo.proid = vpp.proid and documentotermo.dotstatus = 'A'
                
                LEFT JOIN (
                    select DISTINCT pro.pronumeroprocesso,poc.obrid
                    from par3.processoobracomposicao poc
                    inner join par3.processo pro using (proid)
                    where poc.pocstatus = 'A'
                )processo on processo.obrid = obra.obrid
                LEFT JOIN obras2.obras obra2 on obra2.obrid_par3 = obra.obrid
            WHERE obra.obrstatus = 'A'
                and obra.obrid  = {$obrid}
            group by  processo.pronumeroprocesso,vlrempenho";
        return $this->recuperar($sql);
    }

    public function umEhIgualAum()
    {
        return 1;
    }

    public function obterHistoricoAnalise ($arrParams)
    {
        $sql = "SELECT hd.hstid,
                   ed.esddsc,
                   ac.aeddscrealizada,
                   us.usunome,
                   TO_CHAR(hd.htddata, 'DD/MM/YYYY HH24:MI:SS') as datahora,
                   hd.docid,
                   un.unidsc,
                   cd.cmddsc,
                   COALESCE(cd.cmddsc, '-') AS cmddsc
        FROM workflow.historicodocumento hd
        INNER JOIN workflow.acaoestadodoc ac ON (ac.aedid = hd.aedid)
        INNER JOIN workflow.estadodocumento ed ON (ed.esdid = ac.esdidorigem)
        INNER JOIN seguranca.usuario us ON (us.usucpf = hd.usucpf)
        LEFT JOIN workflow.comentariodocumento cd ON (cd.hstid = hd.hstid)
        LEFT JOIN public.unidade un USING(unicod)
              WHERE hd.hstid = {$arrParams['hstid']}
              ORDER BY hd.htddata ASC,
                       hd.hstid ASC";
        return $sql;
    }

    public function obterAnalisePorHistorico($aegid,$obrid)
    {
        $sql = "
        select aeg.aegid
        from par3.analise_engenharia_obra aeg
        where aeg.obrid = {$obrid}
        and aeg.aegid = {$aegid}
        ";
        return $sql;
    }

    public function obterComentariosHistoricoAnalise ($arrParams)
    {
        $sql  = "SELECT cmddsc FROM workflow.comentariodocumento WHERE hstid = {$arrParams['hstid']}";
        return $sql;
    }
}

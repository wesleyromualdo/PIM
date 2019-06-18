<?php

/**
 * Classe de mapeamento da entidade spoemendas.cargatesourogerencial.
 *
 * @version $Id$
 * @since 2017.05.30
 */

/**
 * Spoemendas_Model_Cargatesourogerencial: Informações consolidadas e verificadas depois da carga
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author Jeferson Carlos AraÚjo <jeferson.araujo@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Cargatesourogerencial($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Cargatesourogerencial($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property bool $ctgimportado Verifica se a carga foi importada
 * @property bool $ctgprocessado Verifica se a carga foi processada
 * @property int $ctgidentcarga 
 * @property \Datetime(Y-m-d H:i:s) $ctgdataimportacao Data da importação dos dados - default: now()
 * @property string $usucpf Usuario responsavel pela importação
 * @property double precision $ctgvalliquidopagar Valores Liquidados a Pagar (Exercício + RAP)
 * @property double precision $ctgrappagprocnproc RAP a Pagar (Processados e Não Processados)
 * @property double precision $ctgpagprocnproc RAP Pagos (Processados e Não Processados)
 * @property double precision $ctgrapprocnproc RAP Inscritos (Processados e Não Processados)
 * @property double precision $ctgrapnprocessliquipagar RAP não processado liquidados a pagar
 * @property double precision $ctgrapprocesspagar RAP Processados a Pagar
 * @property double precision $ctgdesppagas Despesas Pagas
 * @property double precision $ctgdespliquidadaspagar Despesas Liquidadas a pagar
 * @property double precision $ctgdespliquidas Despesas Liquidas
 * @property double precision $ctgdespempenhadas Despesas Empenhadas
 * @property int $ctgconvsiafi Convenio SIAFI
 * @property string $ctgsisorigem Sistema de origem
 * @property string $ctgnreferencia Nº de Referencia 
 * @property string $ctgmunicbeneficiado Descrição do municipio beneficiado
 * @property string $ctgdescuf Descrição da UF
 * @property string $ctgufbeneficiada UF da unidade beneficiada
 * @property string $ctgdescfonterecurso Descrição fonte de recursos detalhada
 * @property int $ctgfonterecursodetalhada Fonte de Recursos detalhada
 * @property int $ctgfontesof Fonte da SOF
 * @property string $ctgne Numero da NE 
 * @property int $ctgemeano Ano da emenda
 * @property string $ctggnddsc Descrição do grupo de despesa
 * @property int $ctggnd Grupo de natureza de despesa
 * @property int $ctgptres Numero do PTRES
 * @property string $ctgdsceof Descrição indentificação EOF
 * @property int $ctgresulteof Resultado EOF
 * @property string $ctgsigla Sigla da unidade orçamentaria
 * @property int $ctgugprincipal UG principal 
 * @property string $gestaocod Numero indentificador gestão
 * @property string $ctgugexecutoradesc Descrição da ug executora
 * @property int $ctgugexecutora Identificação Unidade gestora execução
 * @property string $ctgorgaodesc Descrição do orgão
 * @property int $ctgorgao Codigo indentificação orgão
 * @property string $ctgunicoddesc Descrição da Unidade Orçamentaria
 * @property int $ctgunicod indentificador da unidade orçamentaria
 * @property int $ctgid Identificador da tabela - default: nextval('spoemendas.cargatesourogerencial_ctgid_seq'::regclass)
 * @property string $ctgpo Codigo do PO
 */
class Spoemendas_Model_Cargatesourogerencial extends Modelo {

	/**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.cargatesourogerencial';

	/**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ctgid',
    );

	/**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

	/**
	 * @var mixed[] Atributos da tabela.
	 */
	protected $arAtributos = array(
		'ctgimportado' => null,
		'ctgprocessado' => null,
		'ctgidentcarga' => null,
		'ctgdataimportacao' => null,
		'usucpf' => null,
		'ctgvalliquidopagar' => null,
		'ctgrappagprocnproc' => null,
		'ctgpagprocnproc' => null,
		'ctgrapprocnproc' => null,
		'ctgrapnprocessliquipagar' => null,
		'ctgrapprocesspagar' => null,
		'ctgdesppagas' => null,
		'ctgdespliquidadaspagar' => null,
		'ctgdespliquidas' => null,
		'ctgdespempenhadas' => null,
		'ctgconvsiafi' => null,
		'ctgsisorigem' => null,
		'ctgnreferencia' => null,
		'ctgmunicbeneficiado' => null,
		'ctgdescuf' => null,
		'ctgufbeneficiada' => null,
		'ctgdescfonterecurso' => null,
		'ctgfonterecursodetalhada' => null,
		'ctgfontesof' => null,
		'ctgne' => null,
		'ctgemeano' => null,
		'ctggnddsc' => null,
		'ctggnd' => null,
		'ctgptres' => null,
		'ctgdsceof' => null,
		'ctgresulteof' => null,
		'ctgsigla' => null,
		'ctgugprincipal' => null,
		'gestaocod' => null,
		'ctgugexecutoradesc' => null,
		'ctgugexecutora' => null,
		'ctgorgaodesc' => null,
		'ctgorgao' => null,
		'ctgunicoddesc' => null,
		'ctgunicod' => null,
		'ctgid' => null,
		'ctgpo' => null
	);

	/**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array()) {
        return array(
            'ctgimportado' => array('allowEmpty' => true),
            'ctgprocessado' => array('allowEmpty' => true),
            'ctgidentcarga' => array('allowEmpty' => true,'Digits'),
            'ctgdataimportacao' => array('allowEmpty' => true),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'ctgvalliquidopagar' => array('allowEmpty' => true),
            'ctgrappagprocnproc' => array('allowEmpty' => true),
            'ctgpagprocnproc' => array('allowEmpty' => true),
            'ctgrapprocnproc' => array('allowEmpty' => true),
            'ctgrapnprocessliquipagar' => array('allowEmpty' => true),
            'ctgrapprocesspagar' => array('allowEmpty' => true),
            'ctgdesppagas' => array('allowEmpty' => true),
            'ctgdespliquidadaspagar' => array('allowEmpty' => true),
            'ctgdespliquidas' => array('allowEmpty' => true),
            'ctgdespempenhadas' => array('allowEmpty' => true),
            'ctgconvsiafi' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'ctgsisorigem' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'ctgnreferencia' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'mundsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'descricaouf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 45))),
            'regcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'ctgdescfonterecurso' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'ctgfonterecursodetalhada' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'ctgfontesof' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'ne' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 40))),
            'emeano' => array('allowEmpty' => true,'Digits'),
            'gnddsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'gndcod' => array('allowEmpty' => true),
            'ptres' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
	        'po' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
	        'ctgdesceof' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
	        'ctgresulteof' => array('allowEmpty' => true,'Digits'),
	        'uniabrev' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
	        'ungcodprincipal' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
	        'gestaocod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
	        'ungdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
	        'ungcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
	        'orgdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 110))),
	        'orgcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
	        'unidsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 110))),
	        'unicod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
	        'ctgid' => array('Digits'),
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
    public function antesSalvar() {
        // -- Implemente suas transformações de dados aqui
        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }
	/**
     * Salva a diferença que existe no tesouro gerencial da carga
     * @param int $idCarga
     */
    public function saveDifTesouroGerencial($idCarga,$arqid){
        $sql = <<<DML
   INSERT INTO spoemendas.diffcargatesourogerencial (
    unicod, unidsc, orgcod, orgdsc, ungcod, ungdsc, gestaocod,ungcodprincipal,
	uniabrev , dctresulteof, dctdesceof ,ptres ,gndcod, gnddsc ,emeano, ne ,
	dctfontesof, dctfonterecursodetalhada, dctdescfonterecurso ,regcod ,descricaouf, 
	mundsc ,dctnreferencia ,dctsisorigem ,dctconvsiafi , dctdespempenhadas, dctdespliquidas, dctdespliquidadaspagar, dctdesppagas, dctrapprocesspagar, 
    dctrapnprocessliquipagar, dctrapprocnproc, dctpagprocnproc, dctrappagprocnproc, dctvalliquidopagar,dctdespempenhadasdif, 
    dctdespliquidasdif, dctdespliquidadaspagardif, dctdesppagasdif, dctrapprocesspagardif, 
    dctrapnprocessliquipagardif, dctrapprocnprocdif, dctpagprocnprocdif, dctrappagprocnprocdif, dctvalliquidopagardif, dctpo, usucpf, 
    dctidentcarga,dcttipoddl,arqid)
    SELECT
	  ctg.unicod 
        , ctg.unidsc 
        , ctg.orgcod 
        , ctg.orgdsc 
        , ctg.ungcod 
        , ctg.ungdsc 
        , ctg.gestaocod
        , ctg.ungcodprincipal
        , ctg.uniabrev 
        , ctg.ctgresulteof 
        , ctg.ctgdesceof 
        , ctg.ptres
        , ctg.gndcod 
        , ctg.gnddsc
        , ctg.emeano 
        , ctg.ne
        , ctg.ctgfontesof 
        , ctg.ctgfonterecursodetalhada 
        , ctg.ctgdescfonterecurso
        , ctg.regcod
        , ctg.descricaouf 
        , ctg.mundsc
        , ctgnreferencia
        , ctgsisorigem
        , ctgconvsiafi
        , ctgdespempenhadas           AS dctdespempenhadas
        , ctgdespliquidas             AS dctdespliquidas
        , ctgdespliquidadaspagar      AS dctdespliquidadaspagar
        , ctgdesppagas                AS dctdesppagas
        , ctgrapprocesspagar          AS dctrapprocesspagar
        , ctgrapnprocessliquipagar    AS dctrapnprocessliquipagar
        , ctgrapprocnproc             AS dctrapprocnproc
        , ctgpagprocnproc             AS dctpagprocnproc
        , ctgrappagprocnproc          AS dctrappagprocnproc
        , ctgvalliquidopagar          AS dctvalliquidopagar
        , ctgdespempenhadas - COALESCE(tgdespempenhadas, 0)               AS dctdespempenhadasdif
        , ctgdespliquidas - COALESCE(tgdespliquidas, 0)                   AS dctdespliquidasdif
        , ctgdespliquidadaspagar - COALESCE(tgdespliquidadaspagar, 0)     AS dctdespliquidadaspagardif
        , ctgdesppagas - COALESCE(tgdesppagas, 0)                         AS dctdesppagasdif
        , ctgrapprocesspagar - COALESCE(tgrapprocesspagar, 0)             AS dctrapprocesspagardif
        , ctgrapnprocessliquipagar - COALESCE(tgrapnprocessliquipagar, 0) AS dctrapnprocessliquipagardif
        , ctgrapprocnproc - COALESCE(tgrapprocnproc, 0)                   AS dctrapprocnprocdif
        , ctgpagprocnproc - COALESCE(tgpagprocnproc, 0)                   AS dctpagprocnprocdif
        , ctgrappagprocnproc - COALESCE(tgrappagprocnproc, 0)             AS dctrappagprocnprocdif
        , ctgvalliquidopagar - COALESCE(tgvalliquidopagar, 0)             AS dctvalliquidopagardif      
        , ctg.ctgpo
        , '{$_SESSION['usucpf']}'                                         AS usucpf
        , {$idCarga}
        ,CASE WHEN tgid IS NULL THEN 'I' ELSE 'U' END                     AS ctgtipoddl 
        ,{$arqid}
    FROM spoemendas.cargatesourogerencial ctg
        LEFT JOIN spoemendas.tesourogerencial tg ON tg.unicod = ctg.unicod 
               AND tg.orgcod = ctg.orgcod 
               AND tg.emeano = ctg.emeano 
               AND tg.gndcod = ctg.gndcod
               AND tg.necompleta = ctg.ne
    WHERE
        ctgidentcarga = {$idCarga} AND tgid IS NULL
        OR ctgdespempenhadas != tgdespempenhadas
        OR ctgdespliquidas != tgdespliquidas
        OR ctgdespliquidadaspagar != tgdespliquidadaspagar
        OR ctgdesppagas != tgdesppagas
        OR ctgrapprocesspagar != tgrapprocesspagar
        OR ctgrapnprocessliquipagar != tgrapnprocessliquipagar
        OR ctgrapprocnproc != tgrapprocnproc
        OR ctgpagprocnproc != tgpagprocnproc
        OR ctgrappagprocnproc != tgrappagprocnproc
        OR ctgvalliquidopagar != tgvalliquidopagar
DML;
        $this->executar($sql);
    }
	/**
     * Atualiza a carga para Processado
     * @param int $idCarga
     */
    public function processado($idCarga){
        $sqlCarg = "UPDATE spoemendas.cargatesourogerencial SET ctgprocessado = 'TRUE' WHERE ctgidentcarga = {$idCarga}";
        $this->executar($sqlCarg);
        $this->commit();
    }
	/**
     * Função que traz os responsavel que fizeram carga
     * @return type Array
     */
    public function responsavelCarga(){
        $sql = "
                SELECT DISTINCT usucpf AS codigo,
                                usunome AS descricao
                FROM spoemendas.cargatesourogerencial
                INNER JOIN seguranca.usuario USING(usucpf) WHERE ctgprocessado = TRUE";
        return $this->carregar($sql);
    }
	public function getCargaByIdCarga($idcarga){
        $sql = <<<DML
           SELECT
                arqid
        FROM
                spoemendas.diffcargatesourogerencial
        WHERE
                dctidentcarga = {$idcarga}
                AND arqid IS NOT NULL
        GROUP BY
                arqid
DML;
        return $this->pegaUmObjeto($sql);
    }
	public function getArqidByCarga($idcarga){
        $sql = <<<DML
         SELECT
                arqid
        FROM
                spoemendas.cargatesourogerencial
        WHERE
                ctgidentcarga = {$idcarga}
                AND arqid IS NOT NULL
        GROUP BY
                arqid
DML;
        return $this->pegaUmObjeto($sql);
    }
	/**
     * Função verifica o tamanho dos valores dos campos que foi extraido do excel
     * @param array $dados Array
     * @return string
     */
    public function validaTamanhoCampos($dados, $validacao, $cabecalho) {
        $msg = '';
        foreach ($dados as $key => $dado) {
            for ($i = 0; $i <= sizeof($validacao) - 1; $i++) {
                $linha = 0;
                if (sizeof($dado[$i]) < 25 && strlen($dado[$i]) > $validacao[$i]) {
                    $linha = $key + 2;
                    $msg .= 'O valor informado na coluna <b>' . $cabecalho[$i] . '</b> linha <b>' . $linha . '</b> é maior do que o banco consegue armazenar <br>';
                }
            }
        }
        if (!empty($msg)) {
            $msgView['msg'] = $msg;
            $msgView['bool'] = true;

            return $msgView;
        } else {
            return EXCEL_TAMANHO_CAMPOS_CORRETO;
        }
    }

	/**
     * Função com tamanho com campos
     * @return type Array
     */
    public function camposValidacao() {
        $sql = "
                SELECT
                column_name
                , COALESCE (character_maximum_length, numeric_precision) as max_length
            FROM information_schema.columns c
            WHERE
                c.table_name = 'cargatesourogerencial'
                AND table_schema = 'spoemendas'
            ORDER BY
                ordinal_position";
        $result =  $this->carregar($sql);
        foreach($result as $key => $dado){
            if($key > 0 && $key < 36 || $key == 42){
            $tamanhoCampo[] = $dado['max_length'];
            }
        }
	    $i=36;
	    while ($i >= 12){
		    $tamanhoCampo[$i] = $tamanhoCampo[$i-1];
		    $i--;
	    }
	    $tamanhoCampo[11] = $tamanhoCampo[36];
	    unset($tamanhoCampo[36]);
	    return $tamanhoCampo;
    }

}
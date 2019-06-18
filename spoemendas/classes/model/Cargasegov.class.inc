<?php
/**
 * Classe de mapeamento da entidade spoemendas.cargasegov.
 *
 * @version $Id$
 * @since 2017.09.12
 */

/**
 * Spoemendas_Model_Cargasegov: Guarda informacoes da carga SEGOV
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author Guilherme Félix Da Silva Maciel <guilherme.maciel@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Cargasegov($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Cargasegov($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $casid Identificador da tabela - default: nextval('spoemendas.cargasegov_casid_seq'::regclass)
 * @property string $casmesdereferencia Mes de referencia
 * @property string $unicod codigo da UO
 * @property string $unidsc descricao da UO
 * @property string $ungcodprincipal codigo da Unidade gestora principal
 * @property string $ungcodexecutora codigo da unidade gestora executora
 * @property string $ungdscexecutora descricao da unidade gestora executora
 * @property string $ptres ptres
 * @property int $gndcod GND
 * @property int $rpcod codigo RP
 * @property string $fonterecursodetalhada fonte de recursos detalhadas
 * @property string $autnome autor
 * @property string $parnome partido
 * @property string $casinteressado interessado
 * @property int $emeano ano
 * @property string $emecod numero da emenda
 * @property string $casne numero da nota de empenho
 * @property string $casgestor gestor
 * @property string $ugexecutoracompleta UGE
 * @property string $estuf UF
 * @property string $mundsc Municipio
 * @property string $cascontratorepasse Contrato de Repasse
 * @property string $caspropostasiconv Proposta no SICONV
 * @property string $casconveniosiafi Convênio SIAFI
 * @property string $casnumreferencia Numero de Referencia
 * @property string $casequipamento Maquina ou Equipamento
 * @property string $casobjeto Objeto
 * @property int $cascodvinculacao Vinculacao
 * @property string $foncod Fonte
 * @property string $casgrauprioridade Grau de prioridade
 * @property numeric $casvlrsolicitado valor solicitado
 * @property numeric $casvlrautorizado valor autorizado
 * @property \Datetime(Y-m-d H:i:s) $casdtaautorizacao data de autorizacao
 */
class Spoemendas_Model_Cargasegov extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.cargasegov';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'casid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'casid' => null,
        'casmesdereferencia' => null,
        'unicod' => null,
        'unidsc' => null,
        'ungcodprincipal' => null,
        'ungcodexecutora' => null,
        'ungdscexecutora' => null,
        'ptres' => null,
        'gndcod' => null,
        'rpcod' => null,
        'fonterecursodetalhada' => null,
        'autnome' => null,
        'parnome' => null,
        'casinteressado' => null,
        'emeano' => null,
        'emecod' => null,
        'casne' => null,
        'casgestor' => null,
        'ugexecutoracompleta' => null,
        'estuf' => null,
        'mundsc' => null,
        'cascontratorepasse' => null,
        'caspropostasiconv' => null,
        'casconveniosiafi' => null,
        'casnumreferencia' => null,
        'casequipamento' => null,
        'casobjeto' => null,
        'cascodvinculacao' => null,
        'foncod' => null,
        'casgrauprioridade' => null,
        'casvlrsolicitado' => null,
        'casvlrautorizado' => null,
        'casdtaautorizacao' => null,
        'hcaid' => null,
        'casempenhado' => null,
        'caspago' => null,
        'casstatus' => null
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'casid' => array('Digits'),
            'casmesdereferencia' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'unicod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'unidsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 110))),
            'ungcodprincipal' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'ungcodexecutora' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'ungdscexecutora' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'ptres' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'gndcod' => array('allowEmpty' => true,'Digits'),
            'rpcod' => array('allowEmpty' => true,'Digits'),
            'fonterecursodetalhada' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'autnome' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'parnome' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'casinteressado' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'emeano' => array('allowEmpty' => true,'Digits'),
            'emecod' => array(new Zend_Validate_StringLength(array('max' => 8))),
            'casne' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 500))),
            'casgestor' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'ugexecutoracompleta' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 150))),
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'mundsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'cascontratorepasse' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 150))),
            'caspropostasiconv' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 150))),
            'casconveniosiafi' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 150))),
            'casnumreferencia' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 150))),
            'casequipamento' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'casobjeto' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 250))),
            'cascodvinculacao' => array('allowEmpty' => true,'Digits'),
            'foncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'casgrauprioridade' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'casvlrsolicitado' => array('allowEmpty' => true),
            'casvlrautorizado' => array('allowEmpty' => true),
            'casdtaautorizacao' => array(),
            'hcaid' => array('Digits'),
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

    public function getQuantidadeColunas(){
	    $sql = <<<DML
			SELECT count(column_name)-1 FROM information_schema.columns c
            WHERE c.table_name = 'cargasegov'
            AND table_schema = 'spoemendas';
DML;
	    $colunas = $this->pegaUm($sql);
	    return $colunas;
    }

    public function  getTamanhoColunas(){
	    $sql = <<<DML
			SELECT COALESCE (character_maximum_length, numeric_precision) as max_length
            FROM information_schema.columns c
            WHERE c.table_name = 'cargasegov'
            AND table_schema = 'spoemendas'
            AND column_name NOT IN ('casid')
            ORDER BY ordinal_position;
DML;
	    $tamanho = $this->carregarColuna($sql);
	    return $tamanho;
    }

    public function getNomeCampos(){
	    $sql = <<<DML
			SELECT column_name
            FROM information_schema.columns c
            WHERE c.table_name = 'cargasegov'
            AND table_schema = 'spoemendas'
            AND column_name NOT IN ('casid');
DML;
	    $tamanho = $this->carregarColuna($sql);
	    return $tamanho;
    }

    public function cargaSegov(){
    	$sql = <<<DML
			SELECT max(casid) AS carga FROM {$this->stNomeTabela}
DML;
    	$idCarga = $this->pegaUm($sql);
    	return $idCarga;

    }

    public function getAll($idCarga, $retornaSQL = false)
    {
        $strSQL = <<<DML
        SELECT
            casid,
            casmesdereferencia,
            casne,
            unicod,
            unidsc,
            casvlrsolicitado,
            casvlrautorizado,
            emecod,
            emeano,
            ptres,
            casdtaautorizacao
        FROM spoemendas.cargasegov
        WHERE casstatus = 'A' and hcaid = {$idCarga}
DML;
        if ($retornaSQL) {
            return $strSQL;
        }

        return $this->carregar($strSQL);
    }

    public function alteraStatus($casid)
    {
        $this->executar("UPDATE {$this->stNomeTabela} SET casstatus = 'P' WHERE casid = {$casid}");
        return $this->commit();
    }
}

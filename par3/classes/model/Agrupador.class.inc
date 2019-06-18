<?php
/**
 * Classe de mapeamento da entidade par3.agrupador.
 *
 * @version $Id$
 * @since 2017.10.18
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Agrupador: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Agrupador($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Agrupador($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $agrid Chave Primária da Tabela par3.agrupador - default: nextval('par3.agrupador_agrid_seq'::regclass)
 * @property string $agrdesc 
 * @property string $agrstatus
 */
class Par3_Model_Agrupador extends Modelo
{
    private $modelHistorico;

    public function __construct(){
        $this->modelHistorico = new Par3_Model_AgrupadorHistorico();
    }


    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.agrupador';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'agrid',
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
        'agrid' => null,
        'agrdesc' => null,
        'agrstatus' => null,
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
            'agrid' => array('Digits'),
            'agrdesc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'agrstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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

    public function salvar(){
        $this->agrstatus = 'A';
        $acao = ($this->agrid == '')?Par3_Model_AgrupadorHistorico::CREATE:Par3_Model_AgrupadorHistorico::UPDATE;
        $ok = parent::salvar();
        if($ok){
            $this->modelHistorico->gravarHistorico($this,$acao);
        }
        return $ok;
    }

    public function fetchAllList(){
        $sql = "select agr.agrid as agr,agr.agrid as codigo, array_agg(agob.preid) as preid, agrdesc 
                    from par3.agrupador as agr
                    inner join par3.agrupador_obra as agob on agob.agrid = agr.agrid
                    where agr.agrstatus = 'A'
                    group by agr.agrid ORDER BY agr.agrid DESC";
        return $sql;
    }

    public function create($agrid){
        $sql = "select agr.agrid, array_agg(agob.preid) as preid, agrdesc 
                    from par3.agrupador as agr
                    inner join par3.agrupador_obra as agob on agob.agrid = agr.agrid
                    WHERE  agr.agrid = {$agrid}
                    group by agr.agrid";
        return $this->pegaLinha($sql);
    }

    public function inativaAgrupador($agrid){
        $sql = "UPDATE {$this->stNomeTabela} set agrstatus = 'I' WHERE agrid = {$agrid}";
            $this->executar($sql);
            $this->commit();
            $arrAgrupador = $this->create($agrid);
            $this->popularDadosObjeto($arrAgrupador);
            $acao = Par3_Model_AgrupadorHistorico::DELETE;
            $this->agrstatus = 'I';
            $this->modelHistorico->gravarHistorico($this,$acao);

    }

    public function validaDescricaoAgrupador($agrdesc){
        $agrdesc = ($agrdesc);
        $sql = "select count(*) as agrdesc  from par3.agrupador as agrupador where agrupador.agrdesc like '%{$agrdesc}%';";
        $result = $this->pegaLinha($sql);
        return $result;
    }

    /**
     * @return string
     */
    public function getListSelectForm(){
        $sql = "select agrid as codigo, agrdesc as descricao  from par3.agrupador where agrupador.agrstatus = 'A' order by agrdesc asc";
        return $sql;

    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $agrid
     * @description esta função retorna uma string contendo os ids das pre-obras separado por vírgulas
     */
    public function getIdObrasDoAgrupadorByIdInSQL($array = array()){
        $preidobras = '';
        foreach($array as $values){
            $agrupador = $this->create($values);
            $stringPreObras = str_replace(array('{','}','\''),'',$agrupador['preid']);
            $preidobras .= $stringPreObras;
        }
        return $preidobras;

    }

}

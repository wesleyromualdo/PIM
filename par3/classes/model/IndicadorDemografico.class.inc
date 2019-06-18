<?php
/**
 * Classe de mapeamento da entidade par3.indicador_demografico.
 *
 * @version $Id$
 * @since 2017.07.18
 */

/**
 * Par3_Model_Indicador_demografico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Indicador_demografico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Indicador_demografico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $indcod 
 * @property int $indtabcorede 
 * @property string $indtabmunic 
 * @property string $indtabuf 
 * @property string $indddesc 
 * @property int $inddid  - default: nextval('par3.indicador_demografico_inddid_seq'::regclass)
 */
class Par3_Model_IndicadorDemografico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.indicador_demografico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'inddid',
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
        'indcod' => null,
        'indtabcorede' => null,
        'indtabmunic' => null,
        'indtabuf' => null,
        'indddesc' => null,
        'inddid' => null,
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
            'indcod' => array(new Zend_Validate_StringLength(array('max' => 10))),
            'indtabcorede' => array('Digits'),
            'indtabmunic' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'indtabuf' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'indddesc' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'inddid' => array('Digits'),
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

    /**
     * @return array
     * Lista todos os indicadores demograficos
     */
    public function fetchAll(){
        $sql = "SELECT * FROM par3.indicador_demografico";
        $resultado = $this->carregar($sql);
        return $resultado;
    }


    public function getIndicadorDemograficoByCod($indicadorTabela){
        ///var_dump($indicadorTabela); exit();
        $sql = "SELECT * FROM par3.indicador_demografico WHERE indcod = '{$indicadorTabela}';";
        $result = $this->pegaLinha($sql);
        return $result;

    }

    public function arrayToInputSelect(){
        $sql = "select indcod, indddesc from par3.indicador_demografico;";
        $arrPronto = $this->carregar($sql);
        if($arrPronto > count($arrPronto)){
            foreach($arrPronto as $key => $value){
                $result[trim(strtolower($value['indcod']))] = $value['indddesc'];
            }
            return $result;
        }else{
            $result = array();
            return $result;
        }
    }


}

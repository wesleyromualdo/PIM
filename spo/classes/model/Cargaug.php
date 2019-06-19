<?php
/**
 * Classe de mapeamento da entidade spo.cargaug.
 *
 * @version $Id$
 * @since 2018.09.05
 */

/**
 * Spo_Model_Cargaug: sem descricao
 *
 * @package Spo\Model
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <victormachado@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spo_Model_Cargaug($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spo_Model_Cargaug($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $cauid  - default: nextval('spo.cargaug_cauid_seq'::regclass)
 * @property string $ungcod 
 * @property string $ungdsc 
 * @property string $ungabrev 
 * @property string $ungstatus 
 * @property string $unicod 
 * @property string $unidsc 
 * @property string $uniabrev
 * @property string $pendencia
 */
class Spo_Model_Cargaug extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spo.cargaug';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'cauid',
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
        'cauid' => null,
        'ungcod' => null,
        'ungdsc' => null,
        'ungabrev' => null,
        'ungstatus' => null,
        'unicod' => null,
        'unidsc' => null,
        'uniabrev' => null,
        'pendencia' => null
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
            'cauid' => array('Digits'),
            'ungcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'ungdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'ungabrev' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 20))),
            'ungstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'unicod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'unidsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 110))),
            'uniabrev' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'pendencia' => array('allowEmpty' => true),
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

    public function limpaTabela(){
        $sql = "TRUNCATE {$this->stNomeTabela}";
        return $this->executar( $sql ) ? true : false;
    }
}

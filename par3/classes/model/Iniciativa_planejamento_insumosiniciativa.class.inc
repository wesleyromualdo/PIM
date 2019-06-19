<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_insumosiniciativa.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamento_insumosiniciativa: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_insumosiniciativa($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_insumosiniciativa($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ipiinid  - default: nextval('par3.iniciativa_planejamento_insumosiniciativa_ipiinid_seq'::regclass)
 * @property int $inpid 
 * @property string $ipiindescricao 
 * @property string $ipiininsumo1 
 * @property string $ipiininsumo2 
 * @property string $ipiinunidadeinsumo1 
 * @property string $ipiinunidadeinsumo2 
 * @property numeric $ipiinqtdinsumo1 
 * @property numeric $ipiinqtdinsumo2 
 * @property numeric $ipiinqtd 
 * @property numeric $ipiinvalunit 
 * @property numeric $ipiinvaltotal 
 * @property string $ipiinstatus  - default: 'A'::bpchar
 */
class Par3_Model_Iniciativa_planejamento_insumosiniciativa extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_insumosiniciativa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ipiinid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inpid' => array('tabela' => 'par3.iniciativa_planejamento', 'pk' => 'inpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ipiinid' => null,
        'inpid' => null,
        'ipiindescricao' => null,
        'ipiininsumo1' => null,
        'ipiininsumo2' => null,
        'ipiinunidadeinsumo1' => null,
        'ipiinunidadeinsumo2' => null,
        'ipiinqtdinsumo1' => null,
        'ipiinqtdinsumo2' => null,
        'ipiinqtd' => null,
        'ipiinvalunit' => null,
        'ipiinvaltotal' => null,
        'ipiinstatus' => null,
        'ipiindtinativacao'=> null,
        'ipiinusucpfinativacao' =>null
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
            'ipiinid' => array('Digits'),
            'inpid' => array('Digits'),
            'ipiindescricao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiininsumo1' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiininsumo2' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiinunidadeinsumo1' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiinunidadeinsumo2' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiinqtdinsumo1' => array('allowEmpty' => true),
            'ipiinqtdinsumo2' => array('allowEmpty' => true),
            'ipiinqtd' => array('allowEmpty' => true),
            'ipiinvalunit' => array('allowEmpty' => true),
            'ipiinvaltotal' => array('allowEmpty' => true),
            'ipiinstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
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

}

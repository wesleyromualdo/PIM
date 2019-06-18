<?php
/**
 * Classe de mapeamento da entidade public.unidade.
 *
 * @version $Id$
 * @since 2016.11.25
 */

/**
 * Public_Model_Unidade: sem descricao
 *
 * @package Public\Model
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <victormachado@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Public_Model_Unidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Public_Model_Unidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property \Datetime(Y-m-d H:i:s) $unidataatualiza 
 * @property string $uniemail 
 * @property string $unitelefone 
 * @property string $uniddd 
 * @property string $orgcodsupervisor 
 * @property string $gstcod 
 * @property string $ungcodresponsavel 
 * @property int $gunid 
 * @property string $uniabrev 
 * @property int $uniid  - default: nextval('unidade_uniid_seq'::regclass)
 * @property string $unistatus  - default: 'A'::bpchar
 * @property string $unidsc 
 * @property string $uniano 
 * @property string $tpocod 
 * @property string $organo 
 * @property string $orgcod 
 * @property string $unitpocod 
 * @property string $unitpocod 
 * @property string $unicod 
 * @property string $unicod 
 */
class Public_Model_Unidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'public.unidade';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'unicod',
        'unitpocod',
        'unicod',
        'unitpocod',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'gstcod' => array('tabela' => 'financeiro.gestao', 'pk' => 'gstcod'),
        'gunid' => array('tabela' => 'grupounidade', 'pk' => 'gunid'),
        'orgcod, organo' => array('tabela' => 'orgao', 'pk' => 'orgcod, organo'),
        'tpocod' => array('tabela' => 'tipoorgao', 'pk' => 'tpocod'),
        'ungcodresponsavel' => array('tabela' => 'unidadegestora', 'pk' => 'ungcod'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'unidataatualiza' => null,
        'uniemail' => null,
        'unitelefone' => null,
        'uniddd' => null,
        'orgcodsupervisor' => null,
        'gstcod' => null,
        'ungcodresponsavel' => null,
        'gunid' => null,
        'uniabrev' => null,
        'uniid' => null,
        'unistatus' => null,
        'unidsc' => null,
        'uniano' => null,
        'tpocod' => null,
        'organo' => null,
        'orgcod' => null,
        'unitpocod' => null,
        'unitpocod' => null,
        'unicod' => null,
        'unicod' => null,
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
            'unidataatualiza' => array('allowEmpty' => true),
            'uniemail' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 150))),
            'unitelefone' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 8))),
            'uniddd' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'orgcodsupervisor' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'gstcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'ungcodresponsavel' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'gunid' => array('allowEmpty' => true,'Digits'),
            'uniabrev' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'uniid' => array('Digits'),
            'unistatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'unidsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 110))),
            'uniano' => array(new Zend_Validate_StringLength(array('max' => 4))),
            'tpocod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'organo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'orgcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'unitpocod' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'unitpocod' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'unicod' => array(new Zend_Validate_StringLength(array('max' => 5))),
            'unicod' => array(new Zend_Validate_StringLength(array('max' => 5))),
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

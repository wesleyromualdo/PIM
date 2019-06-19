<?php
/**
 * Classe de mapeamento da entidade public.orgao.
 *
 * @version $Id$
 * @since 2016.11.25
 */

/**
 * Public_Model_Orgao: sem descricao
 *
 * @package Public\Model
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <victormachado@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Public_Model_Orgao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Public_Model_Orgao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $orgcodsuperior 
 * @property string $orgsit 
 * @property string $gstcod 
 * @property string $orgcodvinculacao 
 * @property string $orgcnpj 
 * @property string $ungcodcoordenadora 
 * @property string $ungcodsetorialorcamentaria 
 * @property string $ungcodsetorialfinanceira 
 * @property int $orgid  - default: nextval('orgao_orgid_seq'::regclass)
 * @property string $orgstatus  - default: 'A'::bpchar
 * @property string $orgsigla 
 * @property string $orgdsc 
 * @property string $tpocod 
 * @property string $organo 
 * @property string $orgcod 
 */
class Public_Model_Orgao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'public.orgao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'orgcod',
        'organo',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'tpocod' => array('tabela' => 'tipoorgao', 'pk' => 'tpocod'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'orgcodsuperior' => null,
        'orgsit' => null,
        'gstcod' => null,
        'orgcodvinculacao' => null,
        'orgcnpj' => null,
        'ungcodcoordenadora' => null,
        'ungcodsetorialorcamentaria' => null,
        'ungcodsetorialfinanceira' => null,
        'orgid' => null,
        'orgstatus' => null,
        'orgsigla' => null,
        'orgdsc' => null,
        'tpocod' => null,
        'organo' => null,
        'orgcod' => null,
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
            'orgcodsuperior' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'orgsit' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'gstcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'orgcodvinculacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'orgcnpj' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 15))),
            'ungcodcoordenadora' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'ungcodsetorialorcamentaria' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'ungcodsetorialfinanceira' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'orgid' => array('Digits'),
            'orgstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'orgsigla' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'orgdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 110))),
            'tpocod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'organo' => array(new Zend_Validate_StringLength(array('max' => 4))),
            'orgcod' => array(new Zend_Validate_StringLength(array('max' => 5))),
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

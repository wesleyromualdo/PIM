<?php
/**
 * Classe de mapeamento da entidade seguranca.usuario.
 *
 * @version $Id$
 * @since 2016.08.31
 */

/**
 * Seguranca_Model_Usuario: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author  <>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Usuario($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Usuario($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $usucpf 
 * @property string $regcod 
 * @property string $usunome 
 * @property string $usuemail 
 * @property string $usustatus 
 * @property string $usufoneddd 
 * @property string $usufonenum 
 * @property string $ususenha 
 * @property \Datetime(Y-m-d H:i:s) $usudataultacesso 
 * @property int $usunivel 
 * @property string $usufuncao 
 * @property string $ususexo 
 * @property string $orgcod 
 * @property string $unicod 
 * @property bool $usuchaveativacao 
 * @property smallint $usutentativas 
 * @property string $usuprgproposto 
 * @property string $usuacaproposto 
 * @property string $usuobs 
 * @property string $ungcod 
 * @property \Datetime(Y-m-d H:i:s) $usudatainc  - default: ('now'::text)::timestamp(6) without time zone
 * @property bool $usuconectado  - default: false
 * @property int $pflcod 
 * @property string $suscod  - default: 'A'::bpchar
 * @property string $usunomeguerra 
 * @property string $orgao 
 * @property string $muncod 
 * @property \Datetime(Y-m-d H:i:s) $usudatanascimento 
 * @property \Datetime(Y-m-d H:i:s) $usudataatualizacao  - default: now()
 * @property int $entid 
 * @property string $tpocod 
 * @property int $carid 
 */
class Seguranca_Model_Usuario extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.usuario';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'usucpf',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'entid' => array('tabela' => 'entidade.entidade', 'pk' => 'entid'),
        'carid' => array('tabela' => 'cargo', 'pk' => 'carid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'usucpf' => null,
        'regcod' => null,
        'usunome' => null,
        'usuemail' => null,
        'usustatus' => null,
        'usufoneddd' => null,
        'usufonenum' => null,
        'ususenha' => null,
        'usudataultacesso' => null,
        'usunivel' => null,
        'usufuncao' => null,
        'ususexo' => null,
        'orgcod' => null,
        'unicod' => null,
        'usuchaveativacao' => null,
        'usutentativas' => null,
        'usuprgproposto' => null,
        'usuacaproposto' => null,
        'usuobs' => null,
        'ungcod' => null,
        'usudatainc' => null,
        'usuconectado' => null,
        'pflcod' => null,
        'suscod' => null,
        'usunomeguerra' => null,
        'orgao' => null,
        'muncod' => null,
        'usudatanascimento' => null,
        'usudataatualizacao' => null,
        'entid' => null,
        'tpocod' => null,
        'carid' => null,
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
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'regcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'usunome' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'usuemail' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'usustatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'usufoneddd' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'usufonenum' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'ususenha' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'usudataultacesso' => array('allowEmpty' => true),
            'usunivel' => array('allowEmpty' => true,'Digits'),
            'usufuncao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'ususexo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'orgcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'unicod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'usuchaveativacao' => array('allowEmpty' => true),
            'usutentativas' => array('allowEmpty' => true),
            'usuprgproposto' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'usuacaproposto' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'usuobs' => array('allowEmpty' => true),
            'ungcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 6))),
            'usudatainc' => array('allowEmpty' => true),
            'usuconectado' => array('allowEmpty' => true),
            'pflcod' => array('allowEmpty' => true,'Digits'),
            'suscod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'usunomeguerra' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 20))),
            'orgao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'usudatanascimento' => array('allowEmpty' => true),
            'usudataatualizacao' => array('allowEmpty' => true),
            'entid' => array('allowEmpty' => true,'Digits'),
            'tpocod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'carid' => array('allowEmpty' => true,'Digits'),
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

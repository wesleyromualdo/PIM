<?php
/**
 * Classe de mapeamento da entidade par3.escola.
 *
 * @version $Id$
 * @since 2017.08.17
 */

/**
 * Model_Escola: sem descricao
 *
 * @package Model\Escola
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Escola($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Escola($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $escqtd_alunos_medio
 * @property int $escqtd_alunos_fundamental
 * @property int $escqtd_alunos_infantil
 * @property int $escqtd_alunos
 * @property int $escqtd_salas_utilizadas
 * @property int $escqtd_salas
 * @property numeric $esccoordenadas_longitude
 * @property numeric $esccoordenadas_latitude
 * @property string $escendereco_cep
 * @property string $escendereco_bairro
 * @property string $escendereco_complemento
 * @property string $escendereco_numero
 * @property string $escendereco
 * @property string $escno_localizacao_diferenciada
 * @property int $escco_localizacao_diferenciada
 * @property string $escno_localizacao
 * @property int $escco_localizacao
 * @property string $escno_situacao_imovel
 * @property int $escco_situacao_imovel
 * @property string $escno_situacao_funcionamento
 * @property int $escco_situacao_funcoinamento
 * @property string $escano_censo
 * @property string $escnome
 * @property string $escno_esfera
 * @property string $escco_esfera
 * @property string $muncod
 * @property string $estuf
 * @property int $esccodinep
 * @property int $escid  - default: nextval('par3.escola_escid_seq'::regclass)
 */
class Par3_Model_Escola extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.escola';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'escid',
        'esccodinep',
        'escano_censo',
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
        'escqtd_alunos_medio' => null,
        'escqtd_alunos_fundamental' => null,
        'escqtd_alunos_infantil' => null,
        'escqtd_alunos' => null,
        'escqtd_salas_utilizadas' => null,
        'escqtd_salas' => null,
        'esccoordenadas_longitude' => null,
        'esccoordenadas_latitude' => null,
        'escendereco_cep' => null,
        'escendereco_bairro' => null,
        'escendereco_complemento' => null,
        'escendereco_numero' => null,
        'escendereco' => null,
        'escno_localizacao_diferenciada' => null,
        'escco_localizacao_diferenciada' => null,
        'escno_localizacao' => null,
        'escco_localizacao' => null,
        'escno_situacao_imovel' => null,
        'escco_situacao_imovel' => null,
        'escno_situacao_funcionamento' => null,
        'escco_situacao_funcoinamento' => null,
        'escano_censo' => null,
        'escnome' => null,
        'escno_esfera' => null,
        'escco_esfera' => null,
        'muncod' => null,
        'estuf' => null,
        'esccodinep' => null,
        'escid' => null,
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
            'escqtd_alunos_medio' => array('allowEmpty' => true,'Digits'),
            'escqtd_alunos_fundamental' => array('allowEmpty' => true,'Digits'),
            'escqtd_alunos_infantil' => array('allowEmpty' => true,'Digits'),
            'escqtd_alunos' => array('allowEmpty' => true,'Digits'),
            'escqtd_salas_utilizadas' => array('allowEmpty' => true,'Digits'),
            'escqtd_salas' => array('allowEmpty' => true,'Digits'),
            'esccoordenadas_longitude' => array('allowEmpty' => true),
            'esccoordenadas_latitude' => array('allowEmpty' => true),
            'escendereco_cep' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 8))),
            'escendereco_bairro' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'escendereco_complemento' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'escendereco_numero' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'escendereco' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1000))),
            'escno_localizacao_diferenciada' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'escco_localizacao_diferenciada' => array('allowEmpty' => true,'Digits'),
            'escno_localizacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'escco_localizacao' => array('allowEmpty' => true,'Digits'),
            'escno_situacao_imovel' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'escco_situacao_imovel' => array('allowEmpty' => true,'Digits'),
            'escno_situacao_funcoinamento' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'escco_situacao_funcoinamento' => array('allowEmpty' => true,'Digits'),
            'escano_censo' => array(new Zend_Validate_StringLength(array('max' => 4))),
            'escnome' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'escno_esfera' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'escco_esfera' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'estuf' => array(new Zend_Validate_StringLength(array('max' => 2))),
            'esccodinep' => array('Digits'),
            'escid' => array('Digits'),
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

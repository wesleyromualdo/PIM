<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_atividadeetapa.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamento_atividadeetapa: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_atividadeetapa($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_atividadeetapa($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ipiaid  - default: nextval('par3.iniciativa_planejamento_atividadeetapa_ipiaid_seq'::regclass)
 * @property int $ipidid 
 * @property string $ipiadescicao 
 * @property int $ipiaquanteventos 
 * @property int $ipiaquantdias 
 * @property numeric $ipiaquanthoras 
 * @property numeric $ipiatotal 
 * @property string $ipiastatus  - default: 'A'::bpchar
 */
class Par3_Model_Iniciativa_planejamento_atividadeetapa extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_atividadeetapa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ipiaid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ipidid' => array('tabela' => 'par3.iniciativa_planejamento_detalheetapa', 'pk' => 'ipidid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ipiaid' => null,
        'ipidid' => null,
        'ipiadescicao' => null,
        'ipiadescicaoatividade' => null,
        'ipiaquanteventos' => null,
        'ipiaquantdias' => null,
        'ipiaquanthoras' => null,
        'ipiatotal' => null,
        'ipiastatus' => null,
        'ipiausucpfinativacao'=>null,
        'ipiausucpfinativacao'=>null
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
            'ipiaid' => array('Digits'),
            'ipidid' => array('Digits'),
            'ipiadescicao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiadescicaoatividade' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiaquanteventos' => array('allowEmpty' => true,'Digits'),
            'ipiaquantdias' => array('allowEmpty' => true,'Digits'),
            'ipiaquanthoras' => array('allowEmpty' => true),
            'ipiatotal' => array('allowEmpty' => true),
            'ipiastatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
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

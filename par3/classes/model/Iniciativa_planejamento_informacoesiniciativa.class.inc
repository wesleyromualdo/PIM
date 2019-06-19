<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_informacoesiniciativa.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamento_informacoesiniciativa: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_informacoesiniciativa($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_informacoesiniciativa($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ipifid  - default: nextval('par3.iniciativa_planejamento_informacoesiniciativa_ipifid_seq'::regclass)
 * @property int $inpid 
 * @property string $ipifprofessores 
 * @property string $ipiforientadores 
 * @property string $ipifformadores 
 * @property string $ipiftutores 
 * @property string $ipifsupervisores 
 * @property string $ipifpreparadores 
 * @property string $ipifstatus  - default: 'A'::bpchar
 */
class Par3_Model_Iniciativa_planejamento_informacoesiniciativa extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_informacoesiniciativa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ipifid',
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
        'ipifid' => null,
        'inpid' => null,
        'ipifprofessores' => null,
        'ipiforientadores' => null,
        'ipifformadores' => null,
        'ipiftutores' => null,
        'ipifsupervisores' => null,
        'ipifpreparadores' => null,
        'ipifstatus' => null,
        'ipifstatus' => null,
        'ipifdtinativacao' => null,
        'ipifusucpfinativacao' => null,


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
            'ipifid' => array('Digits'),
            'inpid' => array('Digits'),
            'ipifprofessores' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiforientadores' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipifformadores' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiftutores' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipifsupervisores' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipifpreparadores' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipifstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
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

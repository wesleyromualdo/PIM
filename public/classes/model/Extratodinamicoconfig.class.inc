<?php
/**
 * Classe de mapeamento da entidade public.extratodinamicoconfig.
 *
 * @version $Id: Extratodinamicoconfig.class.inc 109329 2016-03-21 17:57:09Z maykelbraz $
 * @since 2016.03.17
 */

/**
 * Public_Model_Extratodinamicoconfig: Armazena as configuracoes dos relatorios de extrato dinamico
 *
 * @package Public\Model
 * @uses Simec\Db\Modelo
 * @author Maykel Santos Braz <maykel.braz@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Public_Model_Extratodinamicoconfig($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Public_Model_Extratodinamicoconfig($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $edostatus Indica se a coluna esta ativa ou nao para o relatorio - default: 'A'::bpchar
 * @property bool $edototalizador Indica se a coluna deve ser totalizada no relatorio - geralmente associada ao tipo QT - default: false
 * @property string $edocallback Formatacao de saida da coluna, ao ser listada no relatorio - deve ser uma funcao PHP dentro do escopo do relatorio
 * @property string $edoexpressao Ao montar a query, substitui o valor de edocoluna, tambem e utilizado para formatar o dado dentro da query
 * @property string $edoquery Query utilizada para montar as opcoes de filtro da coluna
 * @property string $edolabel Label da coluna quando ela e utilizada como filtro
 * @property string $edocoluna Identificador de um tipo de dado da view - geralmente sera uma de suas colunas - QL: modificado por edoexpqualitativo - QT: modificado por edoexpquantitativo
 * @property string $edotipo Indica o tipo da coluna, se eh (QL)qualitativa ou (QT)quantitativa
 * @property int $exdid referencia para public.extratodinamico
 * @property int $edoid  - default: nextval('extratodinamicoconfig_edoid_seq'::regclass)
 */
class Public_Model_Extratodinamicoconfig extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'public.extratodinamicoconfig';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'edoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'exdid' => array('tabela' => 'extratodinamico', 'pk' => 'exdid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'edostatus' => null,
        'edototalizador' => null,
        'edocallback' => null,
        'edoexpressao' => null,
        'edoquery' => null,
        'edolabel' => null,
        'edocoluna' => null,
        'edotipo' => null,
        'exdid' => null,
        'edoid' => null,
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
            'edostatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'edototalizador' => array(),
            'edocallback' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'edoexpressao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 250))),
            'edoquery' => array('allowEmpty' => true),
            'edolabel' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'edocoluna' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'edotipo' => array(new Zend_Validate_StringLength(array('max' => 2))),
            'exdid' => array('Digits'),
            'edoid' => array('Digits'),
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

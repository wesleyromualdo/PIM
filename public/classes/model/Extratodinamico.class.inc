<?php
/**
 * Classe de mapeamento da entidade public.extratodinamico.
 *
 * @version $Id: Extratodinamico.class.inc 109431 2016-03-24 12:59:23Z maykelbraz $
 * @since 2016.03.16
 */

/**
 * Public_Model_Extratodinamico: Tabela de relatorios do tipo extrato dinamico
 *
 * @package Public\Model
 * @uses Simec\Db\Modelo
 * @author Maykel Santos Braz <maykel.braz@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Public_Model_Extratodinamico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Public_Model_Extratodinamico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $exdview Nome da view utilizada pelo relatorio
 * @property string $exdrelatorio Identificador do relatorio dentro do modulo - preferencialmente nao utilize espacos e acentos
 * @property string $exdmodulo Nome do modulo ao qual o relatorio pertence - utilize o valor de sisdiretorio
 * @property int $exdid  - default: nextval('extratodinamico_exdid_seq'::regclass)
 */
class Public_Model_Extratodinamico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'public.extratodinamico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'exdid',
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
        'exdview' => null,
        'exdrelatorio' => null,
        'exdmodulo' => null,
        'exdid' => null,
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
            'exdview' => array(new Zend_Validate_StringLength(array('max' => 50))),
            'exdrelatorio' => array(new Zend_Validate_StringLength(array('max' => 15))),
            'exdmodulo' => array(new Zend_Validate_StringLength(array('max' => 15))),
            'exdid' => array('Digits'),
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

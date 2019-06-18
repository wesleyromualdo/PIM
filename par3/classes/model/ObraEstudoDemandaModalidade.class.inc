<?php
/**
 * Classe de mapeamento da entidade par3.obra_estudo_demanda_modalidade.
 *
 * @version $Id$
 * @since 2017.08.17
 */

/**
 * Model_Obra_estudo_demanda_modalidade: sem descricao
 *
 * @package Model\Obra
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Obra_estudo_demanda_modalidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Obra_estudo_demanda_modalidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $oemstatus  - default: 'A'::bpchar
 * @property string $oemdsc
 * @property int $oemid  - default: nextval('par3.obra_estudo_demanda_modalidade_oemid_seq'::regclass)
 */
class Par3_Model_ObraEstudoDemandaModalidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_estudo_demanda_modalidade';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'oemid',
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
        'oemstatus' => null,
        'oemdsc' => null,
        'oemid' => null,
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
            'oemstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'oemdsc' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'oemid' => array('Digits'),
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

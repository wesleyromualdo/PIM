<?php
/**
 * Classe de mapeamento da entidade par3.obra_estudo_demanda_motivo.
 *
 * @version $Id$
 * @since 2017.08.24
 */

/**
 * Model_Obra_estudo_demanda_motivo: sem descricao
 *
 * @package Model\Obra
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Obra_estudo_demanda_motivo($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Obra_estudo_demanda_motivo($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $omoid  - default: nextval('par3.obra_estudo_demanda_motivo_omoid_seq'::regclass)
 * @property string $omodsc
 * @property string $omostatus  - default: 'A'::bpchar
 */
class Par3_Model_ObraEstudoDemandaMotivo extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_estudo_demanda_motivo';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'omoid',
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
        'omoid' => null,
        'omodsc' => null,
        'omostatus' => null,
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
            'omoid' => array('Digits'),
            'omodsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'omostatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
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

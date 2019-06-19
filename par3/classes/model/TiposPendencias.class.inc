<?php
/**
 * Classe de mapeamento da entidade par3.tipos_pendencias.
 *
 * @version $Id$
 * @since 2018.08.24
 */

/**
 * Par3_Model_Tipos_pendencias: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Tipos_pendencias($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Tipos_pendencias($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $tipid  - default: nextval('par3.tipos_pendencias_tipid_seq'::regclass)
 * @property string $tipdsc
 * @property string $tipstatus  - default: 'A'::bpchar
 */
class Par3_Model_TiposPendencias extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.tipos_pendencias';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'tipid',
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
        'tipid' => null,
        'tipdsc' => null,
        'tipstatus' => null,
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
            'tipid' => array('Digits'),
            'tipdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'tipstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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

    public function selectTiposPendencias()
    {
        $sql  = "select tipid as codigo, tipdsc as descricao from {$this->stNomeTabela} order by tipdsc asc";
        return simec_preparar_array($this->carregar($sql));
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_areas.
 *
 * @version $Id$
 * @since 2017.07.04
 */

/**
 * Par3_Model_IniciativaIniciativasAreas: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasAreas($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasAreas($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iiarid Chave Primária - default: nextval('par3.iniciativa_iniciativas_areas_iiarid_seq'::regclass)
 * @property "char" $iiarstatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_areas_iniid_seq'::regclass)
 * @property int $iarid FK do código da área (par3.iniciativa_areas) - default: nextval('par3.iniciativa_iniciativas_areas_iarid_seq'::regclass)
 */
class Par3_Model_IniciativaIniciativasAreas extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_areas';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iiarid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iarid' => array('tabela' => 'par3.iniciativa_areas', 'pk' => 'iarid'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iiarid' => null,
        'iiarstatus' => null,
        'iniid' => null,
        'iarid' => null,
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
            'iiarid' => array('Digits'),
            'iiarstatus' => array('allowEmpty' => true),
            'iniid' => array('Digits'),
            'iarid' => array('Digits'),
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

    public function recuperarPorIniciativa($iniid)
    {
         return $this->recuperarTodos('*',array("iniid = {$iniid}"));
    }
}

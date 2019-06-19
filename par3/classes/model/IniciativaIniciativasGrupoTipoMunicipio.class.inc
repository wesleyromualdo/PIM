<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_grupo_tipo_municipio.
 *
 * @version $Id$
 * @since 2017.07.04
 */

/**
 * Par3_Model_IniciativaIniciativasGrupoTipoMunicipio: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipio($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iigtmid Chave Primária - default: nextval('par3.iniciativa_iniciativas_grupo_tipo_municipio_iigtmid_seq'::regclass)
 * @property "char" $iigtmstatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_grupo_tipo_municipio_iniid_seq'::regclass)
 * @property int $gtmid FK do código grupo tipo município (territorios.grupotipomunicipio) - default: nextval('par3.iniciativa_iniciativas_grupo_tipo_municipio_gtmid_seq'::regclass)
 */
class Par3_Model_IniciativaIniciativasGrupoTipoMunicipio extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_grupo_tipo_municipio';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iigtmid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'gtmid' => array('tabela' => 'territorios.grupotipomunicipio', 'pk' => 'gtmid'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iigtmid' => null,
        'iigtmstatus' => null,
        'iniid' => null,
        'gtmid' => null,
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
            'iigtmid' => array('Digits'),
            'iigtmstatus' => array('allowEmpty' => true),
            'iniid' => array('Digits'),
            'gtmid' => array('Digits'),
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

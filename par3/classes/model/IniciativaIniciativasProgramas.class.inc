<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_programas.
 *
 * @version $Id$
 * @since 2017.07.04
 */

/**
 * Par3_Model_IniciativaIniciativasProgramas: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasProgramas($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasProgramas($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iiprgid Chave Primária - default: nextval('par3.iniciativa_iniciativas_programas_iiprgid_seq'::regclass)
 * @property "char" $iiprgstatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_programas_iniid_seq'::regclass)
 * @property int $prgid FK do código do programa (par3.programa) - default: nextval('par3.iniciativa_iniciativas_programas_prgid_seq'::regclass)
 */
class Par3_Model_IniciativaIniciativasProgramas extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_programas';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iiprgid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'prgid' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iiprgid' => null,
        'iiprgstatus' => null,
        'iniid' => null,
        'prgid' => null,
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
            'iiprgid' => array('Digits'),
            'iiprgstatus' => array('allowEmpty' => true),
            'iniid' => array('Digits'),
            'prgid' => array('Digits'),
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

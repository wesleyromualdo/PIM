<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_municipios.
 *
 * @version $Id$
 * @since 2017.07.04
 */

/**
 * Par3_Model_IniciativaIniciativasMunicipios: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasMunicipios($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasMunicipios($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iimunid Chave Primária - default: nextval('par3.iniciativa_iniciativas_municipios_iimunid_seq'::regclass)
 * @property "char" $iimunidstatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_municipios_iniid_seq'::regclass)
 * @property string $muncod FK do código do município (territorios.municipio)
 */
class Par3_Model_IniciativaIniciativasMunicipios extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_municipios';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iimunid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iimunid' => null,
        'iimunidstatus' => null,
        'iniid' => null,
        'muncod' => null,
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
            'iimunid' => array('Digits'),
            'iimunidstatus' => array('allowEmpty' => true),
            'iniid' => array('Digits'),
            'muncod' => array(new Zend_Validate_StringLength(array('max' => 7))),
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


    public function getListaMunicipiosByIniciativaEstado($iniid,$estuf){
        $sql = "SELECT *
                    FROM par3.iniciativa_iniciativas_municipios as iim
                    INNER JOIN territorios.municipio as mu ON mu.muncod = iim.muncod
                    INNER JOIN territorios.estado as estado ON estado.estuf = mu.estuf
                    WHERE iim.iniid = {$iniid} AND estado.estuf = '{$estuf}'";
        $result = $this->carregar($sql);
        if(count($result)> 0){
                return $result;
        }else{
            $arr = array();
            return $arr;
        }
    }

}

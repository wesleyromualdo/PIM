<?php
/**
 * Classe de mapeamento da entidade par3.agrupador_obra.
 *
 * @version $Id$
 * @since 2017.10.18
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Agrupador_obra: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Agrupador_obra($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Agrupador_obra($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $agobid  - default: nextval('par3.agrupador_obra_agobid_seq'::regclass)
 * @property bigint $preid
 * @property bigint $agrid 
 */
class Par3_Model_AgrupadorObra extends Modelo
{

     /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.agrupador_obra';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'agobid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'agrid' => array('tabela' => 'par3.agrupador', 'pk' => 'agrid'),
        'preid' => array('tabela' => 'par3.obra', 'pk' => 'obrid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'agobid' => null,
        'preid' => null,
        'agrid' => null,
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
            'agobid' => array('Digits'),
            'preid' => array('allowEmpty' => true),
            'agrid' => array('allowEmpty' => true),
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
        $this->deleteByAgrupador($this->agrid);
        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function salvar(){
        if(isset($this->agrid) && isset($this->preid)){
            $this->antesSalvar();
            $arrValuesObras = explode(',',$this->preid);
            foreach($arrValuesObras as $arr){
                $this->preid = $arr;
                $this->insert();
            }
        }
    }

    public function deleteByAgrupador($id){
        $sql = "DELETE FROM {$this->stNomeTabela} where agrid = {$id}";
        $this->executar($sql);
        $this->commit();
    }

    public function insert(){
        $sql = "insert into par3.agrupador_obra (agobid, preid, agrid) values (nextval('par3.agrupador_obra_agobid_seq'),{$this->preid}, {$this->agrid})";
        $this->executar($sql);
    }

}

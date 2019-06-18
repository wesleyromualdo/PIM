<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_detalheetapa.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamento_detalheetapa: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_detalheetapa($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_detalheetapa($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ipidid  - default: nextval('par3.iniciativa_planejamento_detalheetapa_ipidid_seq'::regclass)
 * @property int $inpid 
 * @property string $ipidatividade 
 * @property string $ipiddescricao 
 * @property string $ipidunidade 
 * @property numeric $ipidquantidadehoras 
 * @property numeric $ipidtotalhoras 
 * @property string $ipidstatus  - default: 'A'::bpchar
 */
class Par3_Model_Iniciativa_planejamento_detalheetapa extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_detalheetapa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ipidid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inpid' => array('tabela' => 'par3.iniciativa_planejamento', 'pk' => 'inpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ipidid' => null,
        'inpid' => null,
        'ipidatividade' => null,
        'ipiddescricao' => null,
        'ipidunidade' => null,
        'ipidquantidadehoras' => null,
        'ipidtotalhoras' => null,
        'ipidstatus' => null,
        'ipiddtinativacao'=>null,
        'ipidusucpfinativacao'=>null
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
            'ipidid' => array('Digits'),
            'inpid' => array('Digits'),
            'ipidatividade' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipiddescricao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipidunidade' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ipidquantidadehoras' => array('allowEmpty' => true),
            'ipidtotalhoras' => array('allowEmpty' => true),
            'ipidstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
        );
    }



    public function pegarSQLSelectCombo($inpid)
    {
        $where = "ipidstatus = 'A' AND inpid = {$inpid}";


        $sql = "SELECT ipidid as codigo, ipidatividade as descricao 
                FROM ".$this->stNomeTabela."
                WHERE $where";



        return $sql;
    }

    public function pegarSQLSelectCombo2($inpid)
    {
        $where = "ipidstatus = 'A' AND inpid = {$inpid}";


        $sql = "SELECT ipiddescricao as codigo, ipiddescricao as descricao 
                FROM ".$this->stNomeTabela."
                WHERE $where";



        return $sql;
    }


    public function pegaDescricao($ipidid){
        global $db;
        $result = pg_fetch_array(($db->executar("SELECT ipidatividade FROM par3.iniciativa_planejamento_detalheetapa WHERE ipidid = $ipidid")));
        return $result[0];

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

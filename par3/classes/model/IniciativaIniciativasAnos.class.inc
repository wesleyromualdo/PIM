<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_anos.
 *
 * @version $Id$
 * @since 2017.07.03
 */

/**
 * Par3_Model_IniciativaIniciativasAnos: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author  <>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasAnos($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasAnos($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iiano Chave Primária - default: nextval('par3.iniciativa_iniciativas_anos_iiano_seq'::regclass)
 * @property "char" $iianostatus Status do ano
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_anos_iniid_seq'::regclass)
 * @property int $iniano Ano da iniciativa
 */
class Par3_Model_IniciativaIniciativasAnos extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_anos';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iiano',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iiano' => null,
        'iianostatus' => null,
        'iniid' => null,
        'iniano' => null,
    );

    public function pegarSQLSelectCombo($iniid = array(), $cicid = array())
    {
    	$sql = '';
    	// Se existir pesquias pela PK do par3.ciclo_par:
    	if($cicid){
    		$cicid = is_array($cicid) ? implode(',',$cicid):$cicid;
    		$modelCicloPar = new Par3_Model_CicloPar();
    		// Obtem os anos dos ciclos - Ex: array([0] => Array([anos] => 2017, 2018, 2019), [1] => Array([anos]=> 2019))
    		$rsAnosCiclo = $modelCicloPar->recuperarTodos('cicanos as anos', array("cicid in({$cicid})"), 'cicanos ASC');
    		$anos = array();
    		// Junta todos os anos de todos os clicos no array $anos
    		foreach($rsAnosCiclo as $ciclo){
    			$anosCicloAtual = $ciclo['anos'] ? explode(',', $ciclo['anos']) : array();
    			//print_r($anosCicloAtual); echo '<br><br>';
    			$anos = array_merge($anos, $anosCicloAtual);
    		}
    		// Remove os anos duplicados e ordena de forma crescente.
    		$anos = array_unique($anos);
    		// Cria SQL do retorno para que o input (select) do SIMEC possa utilizar.
    		$sql = '';
    		$countAnos = count($anos);
    		foreach($anos as $ano){
    			$sql .= "SELECT $ano as codigo, $ano as descricao";
    			$countAnos --;
    			if($countAnos > 0){
    				$sql .= ' UNION ';
    			}
    		}
    		// Caso tenha retornado algum ano, deixa o SELECT criado como subquery para que possa ser ordenado corretamente.
    		$sql = $anos ? "SELECT * FROM ($sql) anos ORDER BY descricao" : '';
    	} else {
    		$iniid = is_array($iniid) ? implode(',',$iniid):$iniid;
    		$where = " WHERE iianostatus = 'A' ";
    		$where  .= $iniid ? " AND iniid in({$iniid})":'';
    		$sql = "SELECT iniano as codigo, iniano as descricao FROM {$this->stNomeTabela} $where";
    	}

    	return $sql;
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'iiano' => array('Digits'),
            'iianostatus' => array('allowEmpty' => true),
            'iniid' => array('Digits'),
            'iniano' => array('Digits'),
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
        return $this->recuperarTodos('*',array("iniid = {$iniid}"), 'iniano');
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_unidade.
 *
 * @version $Id$
 * @since 2017.07.08
 */

/**
 * Par3_Model_IniciativaUnidade: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaUnidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaUnidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iinuid Chave Primária - default: nextval('par3.iniciativa_unidade_iinuid_seq'::regclass)
 * @property "char" $iinustatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_unidade_iniid_seq'::regclass)
 * @property int $inuid FK do código do instrumentounidade (par3.iniciativa_unidade) - default: nextval('par3.iniciativa_unidade_inuid_seq'::regclass)
 */
class Par3_Model_IniciativaUnidade extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_unidade';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iinuid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iinuid' => null,
        'iinustatus' => null,
        'iniid' => null,
        'inuid' => null,
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
            'iinuid' => array('Digits'),
            'iinustatus' => array('allowEmpty' => true),
            'iniid' => array('Digits'),
            'inuid' => array('Digits'),
        );
    }
    // Obtem informação do município a partir dos campos da tabela
    // $opcoes = array('tipoRetorno' => 'sql' / 'array')
    public function getMunicipio($campos, $opcoes = array('tipoRetorno'=>'sql')){
    	
    	$where = '';
    	
    	foreach($campos as $i=>$campo){
    		$where .= $where ? " AND " : '';
    		$where .= $campo ? " $i IN (" .implode(', ', $campo) .")" : '';
    	}
    	
    	$sql = "
    		SELECT *
    		FROM territorios.municipio m
    		INNER JOIN par3.instrumentounidade insu ON insu.muncod = m.muncod
    		INNER JOIN par3.iniciativa_unidade iniu ON iniu.inuid = insu.inuid
    		WHERE {$where}
    	";
    	
    	$retorno = $opcoes['tipoRetorno'] == 'sql' ? $sql : $this->carregar($sql);
    	
    	return $retorno;
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

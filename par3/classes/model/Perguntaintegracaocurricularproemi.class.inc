<?php
/**
 * Classe de mapeamento da entidade par3.perguntaintegracaocurricularproemi.
 *
 * @version $Id$
 * @since 2016.10.03
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Perguntaintegracaocurricularproemi: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Perguntaintegracaocurricularproemi($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Perguntaintegracaocurricularproemi($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $pecdesc 
 * @property int $pecid  - default: nextval('par3.perguntaintegracaocurricularproemi_pecid_seq'::regclass)
 */
class Par3_Model_Perguntaintegracaocurricularproemi extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.perguntaintegracaocurricularproemi';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
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
        'pecdesc' => null,
        'pecid' => null,
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
            'pecdesc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'pecid' => array('Digits'),
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
    
    /**
     *
     *
     */
    public function selectDescricoes()
    {
    	$sql = "
    		select 
    			pecid,
    			pecdesc
     		FROM 
    			par3.perguntaintegracaocurricularproemi";
    	
    	$resultado = $this->carregar($sql);
    	
    	$resultado = is_array($resultado) ? $resultado : Array();
    	
    	
    	if( is_array($resultado) > 0)
    	{
    		foreach($resultado as $v)
    		{
    			$retorno[$v['pecid']] = $v;
    		}
    	}
    	
    	return $retorno;
    }

}

<?php
/**
 * Classe de mapeamento da entidade par3.respostacargahorariaprograma.
 *
 * @version $Id$
 * @since 2016.09.28
 */

/**
 * Par3_Model_Respostacargahorariaprograma: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Respostacargahorariaprograma($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Respostacargahorariaprograma($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $prgid 
 * @property int $inuid 
 * @property string $rchcpf 
 * @property string $rchstatus 
 * @property string $rchresposta 
 * @property int $rchid  - default: nextval('par3.respostacargahorariaprograma_rchid_seq'::regclass)
 */
class Par3_Model_Respostacargahorariaprograma extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.respostacargahorariaprograma';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    		'rchid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
        'prgid' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'prgid' => null,
        'inuid' => null,
        'adpid' => null,
        'rchgrupo' => null,
        'rchcpf' => null,
        'rchstatus' => null,
        'rchresposta' => null,
        'rchid' => null,
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
            'prgid' => array('allowEmpty' => true,'Digits'),
            'inuid' => array('allowEmpty' => true,'Digits'),
            'rchcpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'rchstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'rchresposta' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'rchid' => array('Digits'),
        );
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getResposta( $dados )
    {
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
    	$prgid = $dados['prgid'];
    	$grupo = $dados['grupo'];
    	$adpid = $dados['adpid'];
    	
    	if($adpid){
        	if( $inuid  && $prgid )
                $sql = "SELECT 
    				        rchid, 
    				        rchresposta
    				
                        FROM par3.respostacargahorariaprograma
    			        WHERE 
        				    inuid = {$inuid} AND
                            adpid = $adpid AND
        				    prgid = {$prgid} AND
        				    rchgrupo =  '{$grupo}' AND
        				    rchstatus = 'A'";
        	
        	$resposta = $this->pegaLinha($sql);
    	}
    	
        if(is_array($resposta))
        {
    		return $resposta;
        }
        else
        {
        	return false;
        }
    }
    /**
     *
     */
    public function getRchid( $dados )
    {
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
    	$prgid = $dados['prgid'];
    	$grupo = $dados['grupo'];
    	
    	if( $inuid  && $prgid )
            $sql = "SELECT 
        				rchid
        			FROM par3.respostacargahorariaprograma
        			WHERE 
        				inuid = {$inuid} AND
        				prgid = {$prgid} AND
        				rchgrupo =  '{$grupo}' AND
        				rchstatus = 'A'
        			ORDER BY 
                        rchid DESC
        			LIMIT 1";
    	
    	$resposta = $this->pegaUm($sql);
    	
        if($resposta != '')
        {
    		return $resposta;
        }
        else
        {
        	return false;
        }
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

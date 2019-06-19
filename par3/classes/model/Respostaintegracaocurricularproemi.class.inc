<?php
/**
 * Classe de mapeamento da entidade par3.respostaintegracaocurricularproemi.
 *
 * @version $Id$
 * @since 2016.10.03
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Respostaintegracaocurricularproemi: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Respostaintegracaocurricularproemi($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Respostaintegracaocurricularproemi($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $inuid 
 * @property int $prgid 
 * @property int $ricpecid4 
 * @property int $ricpecid3 
 * @property int $ricpecid2 
 * @property int $ricpecid1 Resposta 4 - default: 1
 * @property int $ricid  - default: nextval('par3.respostaintegracaocurricularproemi_ricid_seq'::regclass)
 */
class Par3_Model_Respostaintegracaocurricularproemi extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.respostaintegracaocurricularproemi';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    		'ricid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'prgid' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'inuid' => null,
        'prgid' => null,
        'rchgrupo' => null,
        'ricpecid4' => null,
        'riceditaveis' => null,
        'ricpecid3' => null,
        'ricpecid2' => null,
        'ricpecid1' => null,
        'ricid' => null,
    );

    public function retornaPadraoOriginal($inuid)
    {
    	if($inuid){
    		$this->executar("UPDATE par3.respostaintegracaocurricularproemi SET ricpecid1 = 1, ricpecid2 = 2, ricpecid3 = 3, ricpecid4 = 4, riceditaveis= NULL where inuid  = {$inuid}");
    		$this->commit();
    	}
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
            'inuid' => array('allowEmpty' => true,'Digits'),
            'prgid' => array('allowEmpty' => true,'Digits'),
            'riceditaveis' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'ricpecid4' => array('allowEmpty' => true,'Digits'),
            'ricpecid3' => array('allowEmpty' => true,'Digits'),
            'ricpecid2' => array('allowEmpty' => true,'Digits'),
            'ricpecid1' => array('allowEmpty' => true,'Digits'),
            'ricid' => array('Digits'),
        );
    }
    /**
     * 
     * 
     */
    public function retornaOriginal( $dados = array() )
    {
    	
    	$ricid = $this->retornaIdByDados($dados);
    	
    	if( $ricid )
    	{
    		$sql = "UPDATE
    			par3.respostaintegracaocurricularproemi
    		SET
    			ricpecid1 = 1, ricpecid2 = 2, ricpecid3 = 3, ricpecid4 = 4, riceditaveis = NULL
    		WHERE 
    			ricid =  {$ricid}";
    		
	    	$this->executar($sql);
	    	$this->salvar();
	    	if($this->commit())
	    	{
	    		return true;
	    	}
	    	else 
	    	{
	    		return false;
	    	}
    	}
    }
    
    public function existeCampos($inuid)
    {
    	$sql = "select ricid from par3.respostaintegracaocurricularproemi where inuid = {$inuid} AND ricstatus = 'A'";
    	$result = $this->pegaUm($sql);
    	return ($result != '' )? true : false;
    }
    
    /**
     * Valida o email do coordenador
     *
     * @param $email
     * @return boolean
     */
    public function retornaListagemRespostas($inuid)
    {
    	$sqlDados =  "
    	select
		(SELECT pecdesc from par3.perguntaintegracaocurricularproemi where pecid = ricpecid1) as descricao
		from par3.respostaintegracaocurricularproemi 
		WHERE INUID = {$inuid} AND ricstatus = 'A'
		UNION ALL
		select
		(SELECT pecdesc from par3.perguntaintegracaocurricularproemi where pecid = ricpecid2) as descricao
		from par3.respostaintegracaocurricularproemi 
		WHERE INUID = {$inuid} AND ricstatus = 'A'
		UNION ALL
		select
		(SELECT pecdesc from par3.perguntaintegracaocurricularproemi where pecid = ricpecid3) as descricao
		from par3.respostaintegracaocurricularproemi 
		WHERE INUID = {$inuid} AND ricstatus = 'A'
		UNION ALL
		select 
		(SELECT pecdesc from par3.perguntaintegracaocurricularproemi where pecid = ricpecid4) as descricao
		from par3.respostaintegracaocurricularproemi 
		WHERE INUID = {$inuid} AND ricstatus = 'A'
    	";
    	 
    	$cabecalho 	= array('Campo');
    	$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    	$listagem->setCabecalho($cabecalho);
    	$listagem->setQuery($sqlDados);
    	$listagem->turnOffForm();
    	$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    	return ;
    }
    
    public function salvaModificacao( $dados = array() )
    {
    	
    	$ricid = $this->retornaIdByDados($dados);
    	
    	if( $ricid )
    	{
	    	
    		$dadosResposta['ricid'] = $ricid;
    		
	    	if($dados['questao_integracao_2'])
	    	{
	    		$dadosResposta['ricpecid2'] = $dados['questao_integracao_2'];
	    	}	
	    	if($dados['questao_integracao_3'])
	    	{
	    		$dadosResposta['ricpecid3'] = $dados['questao_integracao_3'];
	    	}	
	    	if($dados['questao_integracao_4'])
	    	{
	    		$dadosResposta['ricpecid4'] = $dados['questao_integracao_4'];
	    	}	
	    	if($dados['integracao_curricular'])
	    	{
	    		$dadosResposta['riceditaveis'] = "{$dados['integracao_curricular']}";
	    	}	
	    	
	    	$this->popularDadosObjeto($dadosResposta);
	    	$this->salvar();
	    	
	    	if($this->commit())
	    	{
	    		return true;
	    	}
	    	else 
	    	{
	    		return false;
	    	}
    	}
    	else
    	{
    		return false;
    	}
    }
    
    public function retornaIdByDados( $dados = array() )
    {
    	$inuid = $dados['inuid'];
    	$prgid = $dados['prgid'];
    
    	$sql = "
	    	SELECT
	    		ricid
	    	FROM
	    		par3.respostaintegracaocurricularproemi
	    	WHERE
	    		inuid = {$inuid}
	    	AND
	    		prgid = {$prgid}
    	";
    	
    	$resultado = $this->pegaUm($sql); 
    	
    	if($resultado)
    	{
    		return $resultado;
    	}
    	else
    	{
    		return false;
    	}
    	
    }
    
    /**
     * 
     * 
     */
    public function verificaRespostas( $dados = array() )
    {
    	$inuid = $dados['inuid'];
    	$prgid = $dados['prgid'];
        
        $sql = "
       		SELECT 
				ricpecid1, ricpecid2, ricpecid3, ricpecid4, riceditaveis
			FROM 
				par3.respostaintegracaocurricularproemi	
			WHERE
				inuid = {$inuid}
			AND
				prgid = {$prgid} 		
        
       ";
    	$resultado = $this->carregar($sql);
    	
    	$resultado = (is_array($resultado)) ? $resultado : false ;
    	
    	return $resultado;
    }
    
    /**
     * 
     * 
     */
    public function foiModificado( $respostas )
    {
    	
    	if( 
            ($respostas[0]['ricpecid2'] == 2 ) &&
            ($respostas[0]['ricpecid3'] == 3 ) &&
            ($respostas[0]['ricpecid4'] == 4 )
    	)
    	{
    		return false;
    	}
    	else 
    	{
    		return true;	
    	}
    	
    }
    
    /**
     *
     *
     */
    public function insereRespostasPadrao( $dados = array() )
    {
    	$inuid = $dados['inuid'];
    	$prgid = $dados['prgid'];
    
    	$sql = "
	    	INSERT INTO
			par3.respostaintegracaocurricularproemi	
				(ricpecid1, ricpecid2, ricpecid3, ricpecid4, prgid, inuid)
			VALUES
				(1, 2, 3, 4, {$prgid}, {$inuid})
    	";
    	
    	$resultado = $this->executar($sql);
    	
    	if($this->commit())
    	{
    		return true;
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

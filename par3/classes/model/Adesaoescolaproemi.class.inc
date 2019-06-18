<?php
/**
 * Classe de mapeamento da entidade par3.adesaoescolaproemi.
 *
 * @version $Id$
 * @since 2016.09.30
 */

/**
 * Par3_Model_Adesaoescolaproemi: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Adesaoescolaproemi($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Adesaoescolaproemi($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $aepid  - default: nextval('par3.adesaoescolaproemi_aepid_seq'::regclass)
 * @property int $codinep 
 * @property int $inuid 
 * @property string $muncod 
 * @property string $estuf 
 * @property string $eapcpf 
 * @property string $aepstatus  - default: 'A'::bpchar
 */
class Par3_Model_Adesaoescolaproemi extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.adesaoescolaproemi';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    		'aepid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'aepid' => null,
        'codinep' => null,
        'inuid' => null,
        'muncod' => null,
        'estuf' => null,
        'aepgrupo' => null,
        'eapcpf' => null,
        'aepinseridaaposenvio' => null,
        'aepcargahoraria' => null,
        'aepstatus' => null,
    );

    public function existeEscolas($inuid , $grupo = false)
    {
    	
    	if( $grupo )
    	{
    		$whereGrupo = "AND aepgrupo = {$grupo}";
    	}
    	
    	$sql = "select aepid from par3.adesaoescolaproemi   WHERE inuid = {$inuid} AND aepstatus = 'A' {$whereGrupo} LIMIT 1";
    	
    	$result = $this->pegaUm($sql);
    	return ($result != '' )? true : false;
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
            'aepid' => array('Digits'),
            'codinep' => array('allowEmpty' => true,'Digits'),
            'inuid' => array('allowEmpty' => true,'Digits'),
            'aepgrupo' => array('allowEmpty' => true,'Digits'),
            'aepinseridaaposenvio' => array(),
            'aepcargahoraria' => array('allowEmpty' => true,'Digits'),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'eapcpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'aepstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
    public function retornaQtdSalvoGrupo($dados)
    {
    	$inuid = $dados['inuid'];
    	$grupo = $dados['grupo'];
    	
    	$sql = "
    		SELECT 
				count(aepid) 
			FROM 
				par3.adesaoescolaproemi
			WHERE 
				aepgrupo = {$grupo}
			AND 
				inuid = {$inuid}";
    	
    	$resultado = $this->pegaUm($sql);
    	
    	return (! $resultado) ? '0' : $resultado; 
    	
    }
    
    public function deletaGeralPorUnidade($inuid, $prgid)
    {
    	if($inuid){
    		$this->executar("DELETE from par3.adesaoescolaproemi  where inuid = {$inuid};
    						DELETE from par3.respostacargahorariaprograma  WHERE inuid =  {$inuid} AND prgid = {$prgid}
    		");
    		$this->commit();
    	}
    }
    
    public function apagaCargaHoraria($inuid)
    {
    	if($inuid)
    	{
    		$this->executar("UPDATE par3.adesaoescolaproemi SET aepcargahoraria = NULL where inuid = {$inuid}");
    		$this->commit();
    	}
    }
    
    public function retornaCodEntidadeMarcados($dados)
    {
    	$inuid = $dados['inuid'];
    	$grupo = $dados['grupo'];
    	 
    	$sql = "
	    	SELECT
	    		codinep
	    	FROM
	    		par3.adesaoescolaproemi
	    	WHERE
	    		aepgrupo = {$grupo}
	    	AND
	    		inuid = {$inuid}";
    	
    	$resultado = $this->carregar($sql);
    	 
   		$resultado = (is_array($resultado)) ? $resultado : array();
    	
    	if(count($resultado) > 0)
    	{
    		foreach($resultado as $k => $v)
    		{
    			$arrReturn[] = $v['codinep'];
    		}
    	}
    	return $arrReturn;
    }
    
    public function retornaValoresCargaHoraria($dados)
    {
    	$inuid = $dados['inuid'];
    	$grupo = $dados['grupo'];
    	 
    	$sql = "
	    	SELECT
	    		codinep,
	    		aepcargahoraria
	    	FROM
	    		par3.adesaoescolaproemi
	    	WHERE
	    		aepgrupo = {$grupo}
	    	AND
	    		inuid = {$inuid}";
    	
    	$resultado = $this->carregar($sql);
    	 
   		$resultado = (is_array($resultado)) ? $resultado : array();
    	
    	if(count($resultado) > 0)
    	{
    		foreach($resultado as $k => $v)
    		{
    			$arrReturn[$v['codinep']] = $v['aepcargahoraria'];
    		}
    	}
    	return $arrReturn;
    }
	
    /**
     * 
     * 
     * */
    public function escolasSemCargaForaDoContexto($arrParam)
    {
    	$listaEscolasdesteContexto = $arrParam['array_codinep'];
    	$inuid = $arrParam['inuid'];
    	$grupo = $arrParam['grupo'];
    	if(is_array($listaEscolasdesteContexto) && count($listaEscolasdesteContexto) > 0)
    	{
	    	$clauseCodInep = implode(', ', $listaEscolasdesteContexto);
	    	
	    	$sql = "
	    		SELECT 
					aepid
				FROM 
					par3.adesaoescolaproemi
				WHERE 
					inuid = {$inuid} 
				AND 
					aepgrupo = {$grupo}  
				AND 
					codinep NOT IN ( {$clauseCodInep} )  
				AND 
					aepcargahoraria IS NULL	
	    	";
	    	return($this->pegaUm($sql));
    	}
    	else
    	{
    		return false;
    	}
    	
    }
    /**
     *
     * */
    public function salvaFormularioEscolas($arrParametros)
    {
    	
    	// Objetos utilizados
    	$objIstrumentoUnidade 	= new Par3_Model_InstrumentoUnidade();
    	$objEscolasProemi		= new Par3_Model_Escolasproemi2015();
    	
    	// Dados enviados por parametro
    	$inuid 				= $arrParametros['inuid'];
    	$grupo 				= $arrParametros['grupo'];
    	$salvaCargaHoraria 	= $arrParametros['carga_horaria_escolas'];
    	$cpf 				= $_SESSION['usucpf'];
    	$prgid 				= $arrParametros['prgid'];
    	$epemunicipio		= $arrParametros['epemunicipio'];
    	$epecod				= $arrParametros['epecod'];
    	$epenome			= $arrParametros['epenome'];
    	
    	$docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma WHERE inuid = {$inuid} and pfaid = (select pfaid from par3.proadesao where prgid = {$prgid})");
    	 
    	$enviadoApos = FALSE;
    	if($docid)
    	{
    		$situacaoAtual = wf_pegarEstadoAtual( $docid );
    		 
    		if( ($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_PROEMI) )
    		{
    			$enviadoApos = 'TRUE';
    		}
    	}
    	
    	// Busca os dados da unidade
    	$dadosUnidade 			= $objIstrumentoUnidade->getDadosUnidade($inuid);
    	if( !  is_array($dadosUnidade) )
    	{
    		$arrReturn['retorno'] = 'error';
    		$arrReturn['mensagem'] = "Erro ao Salvar";
    		return $arrReturn;
    	}
    	// Dados retornados da unidade
    	$itrid 			= $dadosUnidade['itrid'];
    	$estuf 			= $dadosUnidade['estuf'];
    	$inudescricao 	= $dadosUnidade['inudescricao'];
    	$muncod			= $dadosUnidade['muncod'];
    	 
    	// Parametros para buscar contexto que as escolas estão sendo salvas
    	$arrParamContexto['itrid'] = $itrid;
    	$arrParamContexto['grupo'] = $grupo;
    	$arrParamContexto['prgid'] = $prgid;
    	$arrParamContexto['inuid'] = $inuid;
    	$arrParamContexto['estuf'] = $estuf;
    	$arrParamContexto['muncod'] = $muncod;
    	$arrParamContexto['epemunicipio'] = $epemunicipio;
    	$arrParamContexto['epecod'] = $epecod;
    	$arrParamContexto['epenome'] = $epenome;
    	$arrParamContexto['aepcargahoraria'] = $arrParametros['aepcargahoraria'];
    	$arrParamContexto['checado'] = $arrParametros['checado'];
    	
    	if( $salvaCargaHoraria == 'N' )
    	{
    			
    		$dadosDetele['inuid'] = $inuid;
    		$dadosDetele['grupo'] = $grupo;
    		$this->deletaCargaPorGrupo( $dadosDetele );
    	
    	}
    	if( $salvaCargaHoraria == 'S' )
    	{
    		
    		$listaEscolasdesteContexto =  $objEscolasProemi->getEscolasContexto($arrParamContexto);
    		
    		if((count($listaEscolasdesteContexto)) > 0 && (is_array($listaEscolasdesteContexto)) )
    		{
    			$arrParam['array_codinep'] = $listaEscolasdesteContexto;
    			$arrParam['inuid'] = $inuid;
    			$arrParam['grupo'] = $grupo;
    			
    			if($this->escolasSemCargaForaDoContexto($arrParam))
    			{
    				$arrReturn['retorno'] = 'error';
    				$arrReturn['mensagem'] = "Já existem neste grupo escolas selecionadas sem carga horária definida e que não estão inclusas na última tentativa de salvar o formulário";
    				return $arrReturn;
    			}
    			
    		}
    	}
    	
    	$objRespostaCargaHoraria	= new Par3_Model_Respostacargahorariaprograma();
    	$arrParamCarga['inuid'] = $inuid;
    	$arrParamCarga['prgid'] = $prgid;
    	$arrParamCarga['grupo'] = $grupo;
    	$rchid = $objRespostaCargaHoraria->getRchid($arrParamCarga);
    	
    	if($rchid)
    	{
    		$dadosResposta['rchid'] = $rchid;
    	}
    	$dadosResposta['rchresposta'] 	= strtoupper($arrParametros['carga_horaria']);
    	$dadosResposta['inuid'] 		= $inuid;
    	$dadosResposta['prgid'] 		= $prgid;
    	$dadosResposta['rchgrupo'] 		= $grupo;
    	$dadosResposta['rchcpf'] 		= $_SESSION['usucpf'];
    	
    	$objRespostaCargaHoraria->popularDadosObjeto($dadosResposta);
    	$objRespostaCargaHoraria->salvar();
    	
    	
		
		
		$listaEscolasContexto =  $objEscolasProemi->getEscolasContexto($arrParamContexto);
		
		if((count($listaEscolasContexto)) > 0 && (is_array($listaEscolasContexto)) && (!$enviadoApos))
		{
			
			$clauseCodInep = implode(', ', $listaEscolasContexto);
			
			$this->executar("DELETE FROM  par3.adesaoescolaproemi WHERE aepgrupo = {$grupo} AND inuid = {$inuid} AND codinep in ( $clauseCodInep )");
		}
		else
		{
			if((!$enviadoApos)){
				$arrReturn['retorno'] = 'error';
				$arrReturn['mensagem'] = "Erro ao Salvar";
				return $arrReturn;
			}
		}
    	
    	
    	$arrEscolas = $arrParametros['escola_checada'];
    	if(is_array($arrEscolas))
    	{
    		foreach($arrEscolas as $k => $codInep )
    		{
    			 $codInep;
    			 
    			 $arrDadosSalvar = array();
    			 $arrDadosSalvar['codinep']		= $codInep;
    			 $arrDadosSalvar['inuid']		= $inuid;
    			 $arrDadosSalvar['estuf']		= $estuf;
    			 $arrDadosSalvar['aepgrupo']	= $grupo;
    			 $arrDadosSalvar['eapcpf']		= $cpf;
    			 $arrDadosSalvar['aepinseridaaposenvio']		= $enviadoApos;
    			 
    			 
    			 if($itrid == 2)
    			 {
    			 	$arrDadosSalvar['muncod'] = $muncod;
    			 }
    			 if( $salvaCargaHoraria == 'S' )
    			 {
    			 	$arrDadosSalvar['aepcargahoraria'] = $arrParametros['carga_horaria_'.$codInep];
    			 }

    			$salvo = $this->insereEscola($arrDadosSalvar);
    				
    			if($salvo)
    			{
    				continue;
    			}
    			else
    			{
    				$arrReturn['retorno'] = 'error';
    				$arrReturn['mensagem'] = "Erro ao Salvar";
    				return $arrReturn;
    			}
    			 
    			 
    		}
    		if( $this->commit() )
    		{
    			$arrReturn['retorno'] = 'success'; 
    			$arrReturn['mensagem'] = "Formulário referente as escolas do grupo {$grupo} salvo com sucesso";
    			return $arrReturn;
    		}
    	}
    	else
    	{
    		$arrReturn['retorno'] = 'error';
    		$arrReturn['mensagem'] = "Erro ao Salvar";
    		return $arrReturn;
    	}
    	
    }
    
    public function deletaCargaPorGrupo( $dados )
    {
    	$inuid = $dados['inuid'];
    	
    	$grupo = $dados['grupo'];
    	 
    	 
    	if( $inuid  &&  $grupo )
    	{
    		$sql = "
	    		UPDATE 
	    			par3.adesaoescolaproemi 
	    		SET 
	    			aepcargahoraria = NULL
	    		WHERE
	    			inuid = '{$inuid}'
	    		AND
	    			aepgrupo = '{$grupo}'
	    		";
    		 
    		$this->executar($sql);
    
    	}
    }
    
    function insereEscola($arrDadosSalvar)
    {
    	$codinep 			= $arrDadosSalvar['codinep'];
    	$inuid 				= $arrDadosSalvar['inuid'];
    	$estuf 				= $arrDadosSalvar['estuf'];
    	$aepgrupo			= $arrDadosSalvar['aepgrupo'];
    	$eapcpf				= $arrDadosSalvar['eapcpf'];
    	$muncod				= ($arrDadosSalvar['muncod']) ? "'{$arrDadosSalvar['muncod']}'" : 'NULL';
    	$aepcargahoraria	= ($arrDadosSalvar['aepcargahoraria']) ? $arrDadosSalvar['aepcargahoraria'] : 'NULL';
    	$aepinseridaaposenvio = ($arrDadosSalvar['aepinseridaaposenvio']) ? 'TRUE' : 'FALSE';
    	 
    	$sqlInsert = "
    	INSERT INTO
	    	par3.adesaoescolaproemi
	    		(codinep, inuid, muncod, estuf, eapcpf , aepcargahoraria, aepgrupo, aepinseridaaposenvio)
	    	VALUES
    			({$codinep}, {$inuid}, {$muncod}, '{$estuf}','{$eapcpf}',$aepcargahoraria, $aepgrupo, $aepinseridaaposenvio )
    	";
    	 
    	$this->executar($sqlInsert);
    	 
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

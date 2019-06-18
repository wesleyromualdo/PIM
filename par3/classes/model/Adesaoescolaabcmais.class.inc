<?php
/**
 * Classe de mapeamento da entidade par3.adesaoescolaabcmais.
 *
 * @version $Id$
 * @since 2016.09.30
 */

/**
 * Par3_Model_Adesaoescolaabcmais: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Adesaoescolaabcmais($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Adesaoescolaabcmais($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $aeaid  - default: nextval('par3.adesaoescolaabcmais_aeaid_seq'::regclass)
 * @property int $codinep 
 * @property int $inuid 
 * @property string $muncod 
 * @property string $estuf 
 * @property string $aeacpf 
 * @property string $aeastatus  - default: 'A'::bpchar
 */
class Par3_Model_Adesaoescolaabcmais extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.adesaoescolaabcmais';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    		'aeaid'
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
        'aeaid' => null,
        'codinep' => null,
        'inuid' => null,
        'muncod' => null,
        'aeagrupo' => null,
        'estuf' => null,
        'aeacpf' => null,
        'aeacargahoraria' => null,
        'aeastatus' => null,
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
            'aeaid' => array('Digits'),
            'codinep' => array('allowEmpty' => true,'Digits'),
            'inuid' => array('allowEmpty' => true,'Digits'),
            'aeagrupo' => array('allowEmpty' => true,'Digits'),
            'aeacargahoraria' => array('allowEmpty' => true,'Digits'),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'aeacpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'aeastatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
    
    public function existeEscolas($inuid, $adpid, $grupo = false)
    {
        if($adpid){
        	if( $grupo )
        	{
        		$whereGrupo = "AND aeagrupo = {$grupo}";
        	}
        
        	$sql = "SELECT aeaid FROM par3.adesaoescolaabcmais 
                    WHERE inuid = {$inuid} AND aeastatus = 'A' AND adpid = $adpid {$whereGrupo} 
                    LIMIT 1";
        	$result = $this->pegaUm($sql);
        }
    	return ($result != '' )? true : false;
    }
    
	
    
    public function deletaGeralPorUnidade($adpid, $prgid)
    {
    	if($inuid)
    	{
    	    $this->executar("DELETE FROM par3.adesaoescolaabcmais WHERE adpid = {$adpid};
    						 DELETE FROM par3.respostacargahorariaprograma WHERE adpid =  {$adpid} AND prgid = {$prgid}");
    		
    		$this->commit();
    	}
    }

    public function apagaCargaHoraria($inuid)
    {
    	if($inuid)
    	{
    		$this->executar("UPDATE par3.adesaoescolaabcmais SET aeacargahoraria = NULL where inuid = {$inuid}");
    		$this->commit();
    	}
    }
    /**
     *
     *
     */
    public function retornaQtdSalvoGrupo($dados)
    {
    	$inuid = $dados['inuid'];
    	$grupo = $dados['grupo'];
    	$adpid = $dados['adpid'];
    	
    	if($adpid){
        	$sql = "
        		SELECT 
    				count(aeaid) 
    			FROM 
    				par3.adesaoescolaabcmais
    			WHERE 
    				aeagrupo = {$grupo} AND 
    				inuid = {$inuid} AND
                    adpid = $adpid";
        	
        	$resultado = $this->pegaUm($sql);
    	}
    	
    	return (! $resultado) ? '0' : $resultado; 
    }
    
    public function deletaCargaPorGrupo( $dados )
    {
    	$inuid = $dados['inuid'];
    	$adpid = $dados['adpid'];
    	$grupo = $dados['grupo'];
    
    	if( $inuid  &&  $grupo )
    	{
    		$sql = "
    			UPDATE par3.adesaoescolaabcmais SET 
	    			aeacargahoraria = NULL
	    		WHERE
	    			inuid = '{$inuid}' AND
                    adpid = $adpid AND
	    			aeagrupo = '{$grupo}'";
    		 
    		$this->executar($sql);
    	}
    }
    
    /*
     * 
     * @return retorna os códigos das escolas que já foram selecionadas na tela que está para abrir
     * */
    public function retornaCodEntidadeMarcados($dados)
    {
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
    	$grupo = $dados['grupo'];
    	 
    	if($adpid){
        	$sql = "SELECT
                		codinep
                	FROM
                		par3.adesaoescolaabcmais
                	WHERE
                		aeagrupo = {$grupo} AND
                        adpid = $adpid AND
                		inuid = {$inuid}";
        	 
        	$resultado = $this->carregar($sql);
    	}
    	 
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
        $adpid = $dados['adpid'];
    	$grupo = $dados['grupo'];
    	 
    	if($adpid){
        	$sql = "SELECT
                		codinep,
                		aeacargahoraria
                	FROM
                		par3.adesaoescolaabcmais
                	WHERE
                		aeagrupo = {$grupo} AND
                        adpid = $adpid AND
                		inuid = {$inuid}";
        	 
        	$resultado = $this->carregar($sql);
    	}
    	 
    	$resultado = (is_array($resultado)) ? $resultado : array();
    	
    	if(count($resultado) > 0)
    	{
    		foreach($resultado as $k => $v)
    		{
    			$arrReturn[$v['codinep']] = $v['aeacargahoraria'];
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
    	$adpid = $arrParam['adpid'];
    	$grupo = $arrParam['grupo'];
    	
    	if(is_array($listaEscolasdesteContexto) && count($listaEscolasdesteContexto) > 0){
    		$clauseCodInep = implode(', ', $listaEscolasdesteContexto);
    
	    		$sql = "SELECT 
        					aeaid
        				FROM 
        					par3.adesaoescolaabcmais 
        				WHERE 
        					inuid = {$inuid} AND 
                            adpid = $adpid AND
        					aeagrupo = {$grupo} AND
        					codinep NOT IN ( $clauseCodInep ) AND
        					aeacargahoraria IS NULL";
	    		
    		return($this->pegaUm($sql));
    	}else{
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
    	$objEscolasAbcMais		= new Par3_Model_Escolasabcmais2015();
    	
    	// Dados enviados por parametro
    	$inuid 				= $arrParametros['inuid'];
    	$adpid 				= $arrParametros['adpid'];
    	$grupo 				= $arrParametros['grupo'];
    	$salvaCargaHoraria 	= $arrParametros['carga_horaria_escolas'];
    	$cpf 				= $_SESSION['usucpf'];
    	$prgid 				= $arrParametros['prgid'];
    	$eammunicipio		= $arrParametros['eammunicipio'];
    	$eamrede			= $arrParametros['eamrede'];
    	$eamcod				= $arrParametros['eamcod'];
    	$eamnome			= $arrParametros['eamnome'];
    	
    	
    	$docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma WHERE adpid = $adpid");
    	
    	$enviadoApos = FALSE;
    	if($docid)
    	{
    		$situacaoAtual = wf_pegarEstadoAtual( $docid );
    		 
    		if( ($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_ABCMAIS) )
    		{
    			$enviadoApos = TRUE;
    		}
    	}
    	
    	// Busca os dados da unidade
    	$dadosUnidade = $objIstrumentoUnidade->getDadosUnidade($inuid);
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
    	$arrParamContexto['itrid']             = $itrid;
    	$arrParamContexto['grupo']             = $grupo;
    	$arrParamContexto['prgid']             = $prgid;
    	$arrParamContexto['inuid']             = $inuid;
    	$arrParamContexto['adpid']             = $adpid;
    	$arrParamContexto['estuf']             = $estuf;
    	$arrParamContexto['muncod']            = $muncod;
    	$arrParamContexto['eammunicipio']      = $eammunicipio;
    	$arrParamContexto['eamrede']           = $eamrede;
    	$arrParamContexto['eamcod']            = $eamcod;
    	$arrParamContexto['eamnome']           = $eamnome;
    	$arrParamContexto['aeacargahoraria']   = $arrParametros['aeacargahoraria'];
    	$arrParamContexto['checado']           = $arrParametros['checado'];
    	
    	if( $salvaCargaHoraria == 'N' )
    	{
    	    $dadosDetele['inuid'] = $inuid;
    	    $dadosDetele['adpid'] = $adpid;
    		$dadosDetele['grupo'] = $grupo;
    		$this->deletaCargaPorGrupo( $dadosDetele );
    	
    	}
    	if( $salvaCargaHoraria == 'S' )
    	{

    		$listaEscolasdesteContexto =  $objEscolasAbcMais->getEscolasContexto($arrParamContexto);
    		
    		if((count($listaEscolasdesteContexto)) > 0 && (is_array($listaEscolasdesteContexto)) )
    		{
    			$arrParam['array_codinep'] = $listaEscolasdesteContexto;
    			$arrParam['inuid'] = $inuid;
    			$arrParam['adpid'] = $adpid;
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
    	$arrParamCarga['adpid'] = $adpid;
    	$arrParamCarga['prgid'] = $prgid;
    	$arrParamCarga['grupo'] = $grupo;
    	$rchid = $objRespostaCargaHoraria->getRchid($arrParamCarga);
    	
    	if($rchid)
    	{
    		$dadosResposta['rchid'] = $rchid;
    	}
    	
    	$dadosResposta['rchresposta'] 	= strtoupper($arrParametros['carga_horaria']);
    	$dadosResposta['inuid'] 		= $inuid;
    	$dadosResposta['adpid'] 		= $adpid;
    	$dadosResposta['prgid'] 		= $prgid;
    	$dadosResposta['rchgrupo'] 		= $grupo;
    	$dadosResposta['rchcpf'] 		= $_SESSION['usucpf'];
    	
    	$objRespostaCargaHoraria->popularDadosObjeto($dadosResposta);
    	$objRespostaCargaHoraria->salvar();
		
		$listaEscolasContexto =  $objEscolasAbcMais->getEscolasContexto($arrParamContexto);
		
		if((count($listaEscolasContexto)) > 0 && (is_array($listaEscolasContexto)) && (!$enviadoApos))
		{
			
			$clauseCodInep = implode(', ', $listaEscolasContexto);
			
			$this->executar("DELETE FROM  par3.adesaoescolaabcmais WHERE aeagrupo = {$grupo} AND adpid = $adpid AND inuid = {$inuid} AND codinep in ( $clauseCodInep )");
		}
		else
		{
			if((!$enviadoApos))
			{
				$arrReturn['retorno'] = 'error';
	    		$arrReturn['mensagem'] = "Erro ao Salvar";
	    		return $arrReturn;
			}
		}
		
		
    	$arrEscolas = $arrParametros['escola_checada'];
    	//  [prgid] => 8
    	
    	if(is_array($arrEscolas))
    	{
    		foreach($arrEscolas as $k => $codInep )
    		{
    			$arrDadosSalvar = array(); 
    			
    			$arrDadosSalvar['codinep'] = $codInep;
    			$arrDadosSalvar['inuid'] = $inuid;
    			$arrDadosSalvar['adpid'] = $adpid;
    			$arrDadosSalvar['estuf'] = $estuf;
    			$arrDadosSalvar['aeagrupo'] = $grupo;
    			$arrDadosSalvar['aeacpf'] = $cpf;
    			$arrDadosSalvar['aeainseridaaposenvio'] = $enviadoApos;
    			
    			if($itrid == 2)
    			{
    				$arrDadosSalvar['muncod'] = $muncod;
    			}
    			if( $salvaCargaHoraria == 'S' )
    			{
    				$arrDadosSalvar['aeacargahoraria'] = $arrParametros['carga_horaria_'.$codInep];
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

    		if($this->commit()){
	    		
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
    
    
    function insereEscola($arrDadosSalvar)
    {
    	$codinep 			   = $arrDadosSalvar['codinep']; 
    	$inuid 				   = $arrDadosSalvar['inuid'];
    	$adpid 				   = $arrDadosSalvar['adpid']; 
    	$estuf 				   = $arrDadosSalvar['estuf'];
    	$aeagrupo 			   = $arrDadosSalvar['aeagrupo']; 
    	$aeacpf				   = $arrDadosSalvar['aeacpf']; 
    	$muncod				   = ($arrDadosSalvar['muncod']) ? "'{$arrDadosSalvar['muncod']}'" : 'NULL'; 
    	$aeacargahoraria       = ($arrDadosSalvar['aeacargahoraria']) ? $arrDadosSalvar['aeacargahoraria'] : 'NULL'; 
    	$aeainseridaaposenvio  = ($arrDadosSalvar['aeainseridaaposenvio']) ? 'TRUE' : 'FALSE'; 
    	
    	$sqlInsert = "
    		INSERT INTO 
				par3.adesaoescolaabcmais (codinep, inuid, adpid, muncod, estuf, aeacpf, aeacargahoraria, aeagrupo, aeainseridaaposenvio )
			VALUES
                ({$codinep}, {$inuid}, $adpid, {$muncod}, '{$estuf}','{$aeacpf}',$aeacargahoraria, $aeagrupo, $aeainseridaaposenvio )		
    	";
    	
    	$this->executar($sqlInsert);
    	
    	if($this->commit()){
    		return true;
    	}
    	else
    	{
    		return false;
    	}
    }
    
}

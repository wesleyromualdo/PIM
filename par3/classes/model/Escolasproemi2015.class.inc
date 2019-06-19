<?php
/**
 * Classe de mapeamento da entidade par3.escolasproemi2015.
 *
 * @version $Id$
 * @since 2016.09.29
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Escolasproemi2015: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo´p
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Escolasproemi2015($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Escolasproemi2015($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $estuf 
 * @property string $muncod 
 * @property string $epegrupo 
 * @property string $eperecebeurecursoproemi2014 
 * @property numeric $eperecurseproemi2014 
 * @property string $epeinse 
 * @property string $epemotivonaoapta 
 * @property string $epeapta 
 * @property string $epepossuiuex 
 * @property string $epepossuiensinomedio 
 * @property int $epematriculamedio 
 * @property int $epetotalmatricula 
 * @property string $epecod 
 * @property string $epenome 
 * @property string $eperede 
 * @property string $epelocalizacao 
 * @property string $epemunicipio 
 * @property int $epeid  - default: nextval('par3.escolasproemi2015_epeid_seq'::regclass)
 */
class Par3_Model_Escolasproemi2015 extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.escolasproemi2015';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    		'epeid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'estuf' => null,
        'muncod' => null,
        'epegrupo' => null,
        'eperecebeurecursoproemi2014' => null,
        'eperecurseproemi2014' => null,
        'epeinse' => null,
        'epemotivonaoapta' => null,
        'epeapta' => null,
        'epepossuiuex' => null,
        'epepossuiensinomedio' => null,
        'epematriculamedio' => null,
        'epetotalmatricula' => null,
        'epecod' => null,
        'epenome' => null,
        'eperede' => null,
        'epelocalizacao' => null,
        'epemunicipio' => null,
        'epeid' => null,
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
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'epegrupo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'eperecebeurecursoproemi2014' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'eperecurseproemi2014' => array('allowEmpty' => true),
            'epeinse' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'epemotivonaoapta' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'epeapta' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'epepossuiuex' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'epepossuiensinomedio' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'epematriculamedio' => array('allowEmpty' => true,'Digits'),
            'epetotalmatricula' => array('allowEmpty' => true,'Digits'),
            'epecod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 12))),
            'epenome' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'eperede' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 12))),
            'epelocalizacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 12))),
            'epemunicipio' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'epeid' => array('Digits'),
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
    
    public function retornaInfoEscola($arrParam)
    {
    	$inuid = $arrParam['inuid'];
    	$codInep = $arrParam['cod_inep'];
    	$prgId = $arrParam['prgid'];
    	
		$sql = "
			SELECT
				epe.epenome as nome,
				rch.rchresposta as resposta_carga_horaria,
				epe.epegrupo as grupo
			FROM
				par3.escolasproemi2015  epe
			LEFT JOIN par3.respostacargahorariaprograma  rch ON rch.rchgrupo = epe.epegrupo and inuid = {$inuid} AND rchstatus = 'A' AND rch.prgid = $prgId
			WHERE 
				epecod = '{$codInep}'
			AND	
				CASE WHEN eperede = 'Estadual' THEN
					$inuid = (SELECT inuid from par3.instrumentounidade where estuf =  (select estuf from par3.escolasproemi2015 where epecod = '{$codInep}') and itrid in (1,3) LIMIT 1)
				ELSE
					
					$inuid = (SELECT inuid from par3.instrumentounidade where muncod =  (select muncod from par3.escolasproemi2015 where epecod = '{$codInep}') and itrid in (2))
				END
			
		";
		
		$retorno = $this->pegaLinha($sql);
		$retorno = (is_array($retorno)) ? $retorno : Array();
		 
		return $retorno;
    	 
    }
    
	public function existeEscolaNoGrupo($arrParam)
    {
    	
    	$inuid = $arrParam['inuid'];
    	$grupo = $arrParam['grupo'];
    	
    	$sql1 = "select 
    				itrid,
    				estuf,
    				muncod 
    			from 
    				par3.instrumentounidade  where inuid = {$inuid}";
    	
    	$entidade = $this->pegaLinha($sql1);
    	
    	$itrid = $entidade['itrid'];
    	
    	
    	
    	if( ($itrid == 1) || ($itrid == 3) )
    	{
    		$rede =   'Estadual';
    		$clauseLocal = "estuf = '{$entidade['estuf']}'";
    		
    	}
    	else
    	{
    		$rede =   'Municipal';
    		$clauseLocal = "muncod = '{$entidade['muncod']}'";
    	}
    	
    	$sql2 = "select 
    				count(epeid) from par3.escolasproemi2015  
    			WHERE
					eperede = '{$rede}'
				AND
    				epegrupo = '{$grupo}'
    			AND
    				{$clauseLocal}
    			";
    				
    	$total = $this->pegaUm($sql2);			
    	
    	return ($total > 0 ) ? true : false;
    }
    
    /**
     * Método para retornar a lista de escolar passíveis de serem eleitas
     * itrid = 1 // Estadual
     * itrid = 2 // Municipal
     * @return array
     */
    public function getListaEscolasElegiveis($arrParametros = array())
    {
    	require_once APPRAIZ.'includes/workflow.php';
    	
    	 $statusReturn = 'null';
    	 
    	// CArrega variáveis
    	$itrid 			= $arrParametros['itrid'];
    	$grupo 			= $arrParametros['grupo'];
    	$prgid 			= $arrParametros['prgid'];
    	$inuid 			= $arrParametros['inuid'];
    	$cargaHoraria	= $arrParametros['carga_horaria'];
    	$rchid 			= $arrParametros['rchid'];
    	// Inicializa arr de escolas
    	$arrEscolas = array();
    	
    	
    	$docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma WHERE inuid = {$inuid} and pfaid = (select pfaid from par3.proadesao where prgid = {$prgid})");
    	
    	$bloqueiaEdicao = FALSE;
    	
    	$modelProadesao = new Par3_Model_Proadesao();
    	$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($prgid);
    	
    	if($docid)
    	{
    		$situacaoAtual = wf_pegarEstadoAtual( $docid );
    		
    		if($situacaoAtual['esdid'] == WF_ESDID_TERMONAOACEITO_PROEMI  || ($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_PROEMI))
    		{
    			$bloqueiaEdicao = TRUE;
    		}
    	}
    	
    	$bloqueiaEdicaoData = false;
    	
    	if( strtotime($proadesaoDados['pfadatafinal']. ' 12:00:00') < strtotime(date('Y-m-d G:i:s')) )
    	{
    		$bloqueiaEdicao = TRUE;
    		$bloqueiaEdicaoData = TRUE;
    	}
    	
    	$objExcecao = 	new Par3_Model_Excecaoescolasprograma();
    	//Instancia carga horária
    	$objRespostaCargaHoraria = new Par3_Model_Respostacargahorariaprograma();
    	// Trata dados para consultar carga horária
    	$dados['inuid'] = $inuid;
    	$dados['prgid'] = $prgid;
    	$dados['grupo'] = $grupo;
    	// Consulta resposta de carga horária
    	$arrRespostaCargaHoraria 	= $objRespostaCargaHoraria->getResposta( $dados );
    	$respostaCargaHoraria 		= $arrRespostaCargaHoraria['rchresposta'] ;
    	
    	// Estadual
    	if( ($itrid == 1) || ($itrid == 3))
    	{
    		$arrWhere[] = "estuf	= '{$arrParametros['estuf']}' ";
    		$arrWhere[] = "epegrupo = '{$grupo}' ";
    		$arrWhere[] = "eperede 	= 'Estadual' "; 
    	
    	}
    	else
    	{
    		$arrWhere[] = "muncod	= '{$arrParametros['muncod']}' ";
    		$arrWhere[] = "epegrupo = '{$grupo}' ";
    		$arrWhere[] = "eperede 	= 'Municipal' ";
    	}
    	
    	
    	//Filtar filtros
    	if( $arrParametros['epemunicipio'] != '')
    	{
    		$arrWhere[] = "epemunicipio	ilike '%{$arrParametros['epemunicipio']}%' ";
    	}
		if( $arrParametros['epecod']!= '')
		{
			$arrWhere[] = "epecod	= '{$arrParametros['epecod']}' ";
		}
		if( $arrParametros['epenome']!= '')
		{
			$arrWhere[] = "epenome	ilike '%{$arrParametros['epenome']}%' ";
		}
    	
    	//@TODO VERIFICA SAVES de check
    	// Verifica as colunas
    	if(($grupo == 1) || ($grupo == 3))
    	{
    		// Acao, Estado, Municipio, Localização, Nome da Escola, Código Inep, Carga horária diária
    		$colunas = "
				estuf,
				epemunicipio,
    			eperede,
    			epelocalizacao,
				epenome ,
				epecod ,
				eperede,
				'escolher' as cargahorariadiaria
    		";
    	}else if($grupo == 2)
    	{
    		//  Acao, Estado, Municipio, Localização, Nome da Escola, Código Inep, INSE e Carga horária diária
    		$colunas = "
				estuf,
				epemunicipio,
    			eperede,
				epelocalizacao,
				epenome ,
				epecod ,
				epeinse,
				'escolher' as cargahorariadiaria
    		";
    	}
    	
    	$objAdesaoProemi = new Par3_Model_Adesaoescolaproemi();
    	$totalInseridos = $objAdesaoProemi->retornaQtdSalvoGrupo($arrParametros);
    	 
    	$arrCodInepInseridos = $objAdesaoProemi->retornaCodEntidadeMarcados($arrParametros);
    	$arrCodInepInseridos = (is_array($arrCodInepInseridos)) ? $arrCodInepInseridos : Array();
    	$arrCargaHoraria	 = $objAdesaoProemi->retornaValoresCargaHoraria($arrParametros);
    	$arrCargaHoraria	 = (is_array($arrCargaHoraria)) ? $arrCargaHoraria : Array();
    	
    	// Verifica o filtro que traz somente os marcados
    	
    	// Verifica o filtro que traz somente os marcados
    	if( $arrParametros['aepcargahoraria'] != '' )
    	{
    		if( count($arrCargaHoraria) > 0 )
    		{
    			
    			foreach($arrCargaHoraria as $k => $v )
    			{
    				if($v == $arrParametros['aepcargahoraria'])
    				{
    					$arrCargaHoraria[] = $k;
    				}
    			}
    			
    			if( count($arrCargaHoraria) > 0 )
    			{
    				$arrWhere[] = "epecod::integer in ( ". implode(', ', $arrCargaHoraria) ." )";
    				
    			}
    			else
    			{
    				$arrWhere[] = "false";
    			}	
    		}
    		else
    		{
    			$arrWhere[] = "false";
    		}
    	}
    	
    	if( $arrParametros['checado'] != '' )
    	{
    		$not = '';
    		if( $arrParametros['checado'] == 'N' )
    		{
    			$not = 'not';
    		}
    		if( count($arrCodInepInseridos) > 0 )
    		{
    			$arrWhere[] = "epecod::integer {$not} in ( ". implode(', ', $arrCodInepInseridos) ." )";
    		}
    		else
    		{
    			$arrWhere[] = "false=false";
    		}
    		
    	}
    	if($arrParametros['ordenacao'])
    	{
    		$orderBy = "";
    		
    		switch ($arrParametros['ordenacao']) 
    		{
    			case "epemunicipio":
    					$orderBy = "ORDER BY epemunicipio ASC";
    				break;
    			case "epelocalizacao":
    					$orderBy = "ORDER BY epelocalizacao ASC";
    				break;
    			case "epenome":
    					$orderBy = "ORDER BY epenome ASC";
    				break;
    			case "epecod":
    					$orderBy = "ORDER BY epecod ASC";
    				break;
    			case "epeinse":
    					$orderBy = "ORDER BY epeinse ASC";
    				break;
    		}
    		
    	}
    	
    	// Trata clausulas
    	if( is_array($arrWhere) )
    	{
    		$WHERE = (is_array($arrWhere) ? '('.implode(' AND ', $arrWhere).')' : '');
    	}
    	// Monta select
    	$sql = "
	    	SELECT
	    		{$colunas}
	    	FROM
	    		{$this->stNomeTabela}
	    	WHERE
	    		{$WHERE}
	    		{$orderBy}
    	";
	    		
    	// Busca resultado
    	$resultado = $this->carregar($sql);
    	// Trata retorno
    	$resultado = (is_array($resultado)) ? $resultado : array();
    	// incremento
    	$i = 0;
    	
    	
    	
    	if( count( $resultado ) > 0 ) 
    	{
    		
    		$disabledJainseridas = '';
    		// Existe registro
    		$statusReturn = 'success';
    		
    		$disabledEdicao = '';
    		
    		if($bloqueiaEdicao)
    		{
    			$disabledEdicao = 'disabled';
    		}
    		
    		
    		$pfaid = 7; // proemi
    		$modelAdesaoPrograma =  new Par3_Model_AdesaoPrograma();
    		$result = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid);
    		$docidInserido = $result['docid'];
    		$situacaoAtual = wf_pegarEstadoAtual( $docidInserido);
    		$esdid = $situacaoAtual['esdid'];
    		if(($esdid) &&  ($esdid == WF_ESDID_ENVIADOPARAOMEC_PROEMI )) 
    		{
    			
    			$arrParamExcecao['inuid'] = $inuid;
    			$arrParamExcecao['grupo'] = $grupo;
    			$arrParamExcecao['prgid'] = prgid;
    			
    			$temSolicitacao = $objExcecao->possuiSolicitacaoPorGrupo($arrParamExcecao);
    			
    			if( ($temSolicitacao == true) && ( ! $bloqueiaEdicaoData) )
    			{
    				$disabledEdicao = '';
    				$disabledJainseridas = 'disabled';
    				
    			}
    		}
    		
    		
    		foreach($resultado as $k => $escola)
    		{
    			$i++;
    			// Trata o retorno
    			
    			if( (($totalInseridos == 0) && (($grupo == 2) || ($grupo == 1))) || (in_array($escola['epecod'], $arrCodInepInseridos)) ) // @todo verificar se já foi marcado tb 
    			{
    				if( $cargaHoraria)
    				{
    					$arrEscolas[$i]['acao'] 			= "<input {$disabledEdicao} {$disabledJainseridas} class=\"checkbox-primary\" checked type=checkbox name=\"escola_checada[]\" id=\"check_{$escola['epecod']}\" value=\"{$escola['epecod']}\">";
    				}
    			}
    			else
    			{
    				if( $cargaHoraria)
    				{
    					$arrEscolas[$i]['acao'] 			= "<input {$disabledEdicao} class=\"checkbox-primary\" type=checkbox name=\"escola_checada[]\" id=\"check_{$escola['epecod']}\" value=\"{$escola['epecod']}\">";
    				}
    			}
    			
    			
    			$arrEscolas[$i]['estuf'] 			= $escola['estuf'];
    			$arrEscolas[$i]['epemunicipio'] 	= $escola['epemunicipio'];
    			$arrEscolas[$i]['eperede'] 			= $escola['eperede'];
    			$arrEscolas[$i]['epelocalizacao']	= $escola['epelocalizacao'];
    			$arrEscolas[$i]['epenome'] 			= $escola['epenome'];
    			$arrEscolas[$i]['epecod'] 			= $escola['epecod'];
    			if($grupo == 2)
    			{
    				$arrEscolas[$i]['epeinse'] 			= $escola['epeinse'];
    			}
    			//@todo tratar carga horária
    			if($respostaCargaHoraria == 'S' || $cargaHoraria == 'S')
    			{
    				if($cargaHoraria == 'S')
    				{
	    				if( (in_array($escola['epecod'], $arrCodInepInseridos)) ) 
	    				{
	    					
	    					$cheked5 = '';
	    					$cheked7 = '';
	    					if($arrCargaHoraria[$escola['epecod']] == '7')
	    					{
	    						$cheked7 = 'selected';
	    					}
	    					elseif($arrCargaHoraria[$escola['epecod']] == '5')
	    					{
	    						$cheked5 = 'selected';
	    					}
	    					if($cheked7 != '' || $cheked5 != '')
	    					{
	    						$disabledJainseridas2 = $disabledJainseridas;
	    					}
	    					else
	    					{
	    						$disabledJainseridas2 = '';
	    					}
	    					 					
	    					$arrEscolas[$i]['carga_horaria'] = "<select {$disabledEdicao} {$disabledJainseridas2} id=\"select_{$escola['epecod']}\" name=\"carga_horaria_{$escola['epecod']}\" >
	    													 	<option value=\"\">-</option>
	    														<option {$cheked5} value=\"5\" >5</option>
	    														<option {$cheked7} value=\"7\" >7</option>
	    													</select>";
	    				}
	    				else
	    				{
	    					$arrEscolas[$i]['carga_horaria'] = "<select {$disabledEdicao} id=\"select_{$escola['epecod']}\" name=\"carga_horaria_{$escola['epecod']}\" >
	    					<option value=\"\">-</option>
	    					<option value=\"5\" >5</option>
	    					<option value=\"7\" >7</option>
	    					</select>";
	    				}
	    			
    				}
    			}
    		}
    	}

    	if( $cargaHoraria)
    	{
    		$cabecalho[] = 'Selecionar';
    	}
    	$cabecalho[] = 'Estado';
    	$cabecalho[] = 'Município';
    	$cabecalho[] = 'Dependência Administrativa';
    	$cabecalho[] = 'Localização';
    	$cabecalho[] = 'Nome da Escola';
    	$cabecalho[] = 'Código INEP';
    	if($grupo == 2)
    	{
    		$cabecalho[] = 'INSE';
    	}
    	//@todo tratar carga horária
    	if($respostaCargaHoraria == 'S' || $cargaHoraria == 'S')
    	{
    		if($cargaHoraria == 'S')
    		{
    			$cabecalho[] = 'Carga Horária Diária';
    		}
    	}
    	
    	$arrayRetorno['cabecalho'] 	= $cabecalho;
    	$arrayRetorno['arrayEscolas'] = $arrEscolas;
    	$arrayRetorno['status'] 	= $statusReturn;
    	$arrayRetorno['bloqueio'] 	= $bloqueiaEdicao;
    	
    	return $arrayRetorno;
    	
    }
    /**
     * Método para retornar a lista de escolar passíveis de serem eleitas
     * itrid = 1 // Estadual
     * itrid = 2 // Municipal
     * itrid = 3 // Distrito Federal
     * @return array
     */
    public function getListaEscolasNaoElegiveis($arrParametros = array())
    {
    	require_once APPRAIZ.'includes/workflow.php';
    	
    	 $statusReturn = 'null';
    	 
    	// CArrega variáveis
    	$itrid = $arrParametros['itrid'];
    	$grupo = $arrParametros['grupo'];
    	$prgid = $arrParametros['prgid'];
    	$inuid = $arrParametros['inuid'];
    	// Inicializa arr de escolas
    	$arrEscolas = array();
    	
    	
    	$docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma WHERE inuid = {$inuid} and pfaid = (select pfaid from par3.proadesao where prgid = {$prgid})");
    	
    	$bloqueiaEdicao = FALSE;
    	
    	if($docid)
    	{
    		$situacaoAtual = wf_pegarEstadoAtual( $docid );
    		
    		if($situacaoAtual['esdid'] == WF_ESDID_TERMONAOACEITO_PROEMI  || ($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_PROEMI))
    		{
    			$bloqueiaEdicao = TRUE;
    		}
    	}
    	
    	
    	//Instancia carga horária
    	$objRespostaCargaHoraria = new Par3_Model_Respostacargahorariaprograma();
    	// Trata dados para consultar carga horária
    	$dados['inuid'] = $inuid;
    	$dados['prgid'] = $prgid;
    	$dados['grupo'] = $grupo;
    	// Consulta resposta de carga horária
    	$arrRespostaCargaHoraria 	= $objRespostaCargaHoraria->getResposta( $dados );
    	$respostaCargaHoraria 		= $arrRespostaCargaHoraria['rchresposta'] ;
    	
    	// Estadual
    	if(($itrid == 1) || ($itrid == 3))
    	{
    		$arrWhere[] = "epeapta = 'N' ";
    		$arrWhere[] = "eperede = 'Estadual' ";
    		$arrWhere[] = "estuf	= '{$arrParametros['estuf']}' ";
    	
    	}
    	else
    	{
    		$arrWhere[] = "epeapta = 'N' ";
    		$arrWhere[] = "eperede = 'Municipal' ";
    		$arrWhere[] = "muncod	= '{$arrParametros['muncod']}' ";
    	}
    	
    
    	
    	//Filtar filtros
    	if( $arrParametros['epemunicipio'] != '')
    	{
    		$arrWhere[] = "epemunicipio	ilike '%{$arrParametros['epemunicipio']}%' ";
    	}
		if( $arrParametros['epecod']!= '')
		{
			$arrWhere[] = "epecod	= '{$arrParametros['epecod']}' ";
		}
		if( $arrParametros['epenome']!= '')
		{
			$arrWhere[] = "epenome	ilike '%{$arrParametros['epenome']}%' ";
		}
		
		if($arrParametros['ordenacao'])
		{
			$orderBy = "";
		
			switch ($arrParametros['ordenacao'])
			{
				case "epemunicipio":
					$orderBy = "ORDER BY epemunicipio ASC";
					break;
				case "epelocalizacao":
					$orderBy = "ORDER BY epelocalizacao ASC";
					break;
				case "epenome":
					$orderBy = "ORDER BY epenome ASC";
					break;
				case "epecod":
					$orderBy = "ORDER BY epecod ASC";
					break;
				case "epeinse":
					$orderBy = "ORDER BY epeinse ASC";
					break;
			}
		
		}
    	
    	// Trata clausulas
    	if( is_array($arrWhere) )
    	{
    		$WHERE = (is_array($arrWhere) ? '('.implode(' AND ', $arrWhere).')' : '');
    	}
    	// Monta select
    	$sql = "
	    	SELECT 
				estuf,
				epemunicipio,
				eperede,
				epecod,
				epenome,
				epemotivonaoapta
			FROM 
				par3.escolasproemi2015  
			WHERE 
				{$WHERE}
				{$orderBy}
    	
    	";
    	
    	// Busca resultado
    	$resultado = $this->carregar($sql);
    	// Trata retorno
    	$resultado = (is_array($resultado)) ? $resultado : array();
    	// incremento
    	$i = 0;
    	
    	if( count( $resultado ) > 0 ) 
    	{

    		// Existe registro
    		$statusReturn = 'success';
    		
    	}
    	
    	$cabecalho = array( 'UF', 'Município', 'Dependência administrativa', 'Código da Escola', 'Escola', 'Motivo');
    	//@todo tratar carga horária
    	if($respostaCargaHoraria == 'S')
    	{
    		$cabecalho[] = 'Carga Horária Diária';
    	}
    	
    	$arrayRetorno['cabecalho'] 	= $cabecalho;
    	$arrayRetorno['arrayEscolas'] = $resultado;
    	$arrayRetorno['status'] 	= $statusReturn;
    	
    	return $arrayRetorno;
    	
    }
    
    /**
     * Método para retornar a lista de escolar passíveis de serem eleitas
     * itrid = 1 // Estadual
     * itrid = 2 // Municipal
     * @return array
     */
    public function getListaEscolasElegiveisExcel($arrParametros = array())
    {
    	require_once APPRAIZ.'includes/workflow.php';
    	
    	 $statusReturn = 'null';
    	 
    	// CArrega variáveis
    	$itrid = $arrParametros['itrid'];
    	$grupo = $arrParametros['grupo'];
    	$prgid = $arrParametros['prgid'];
    	$inuid = $arrParametros['inuid'];
    	// Inicializa arr de escolas
    	$arrEscolas = array();
    	
    	
    	$docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma WHERE inuid = {$inuid} and pfaid = (select pfaid from par3.proadesao where prgid = {$prgid})");
    	
    	$bloqueiaEdicao = FALSE;
    	
    	if($docid)
    	{
    		$situacaoAtual = wf_pegarEstadoAtual( $docid );
    		
    		if($situacaoAtual['esdid'] == WF_ESDID_TERMONAOACEITO_PROEMI  || ($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_PROEMI))
    		{
    			$bloqueiaEdicao = TRUE;
    		}
    	}
    	
    	
    	//Instancia carga horária
    	$objRespostaCargaHoraria = new Par3_Model_Respostacargahorariaprograma();
    	// Trata dados para consultar carga horária
    	$dados['inuid'] = $inuid;
    	$dados['prgid'] = $prgid;
    	$dados['grupo'] = $grupo;
    	// Consulta resposta de carga horária
    	$arrRespostaCargaHoraria 	= $objRespostaCargaHoraria->getResposta( $dados );
    	$respostaCargaHoraria 		= $arrRespostaCargaHoraria['rchresposta'] ;
    	
    	// Estadual
    	if(( $itrid == 1) || ($itrid == 3))
    	{
    		$arrWhere[] = "estuf	= '{$arrParametros['estuf']}' ";
    		$arrWhere[] = "epegrupo = '{$grupo}' ";
    		$arrWhere[] = "eperede 	= 'Estadual' "; 
    	
    	}
    	else
    	{
    		$arrWhere[] = "muncod	= '{$arrParametros['muncod']}' ";
    		$arrWhere[] = "epegrupo = '{$grupo}' ";
    		$arrWhere[] = "eperede 	= 'Municipal' ";
    	}
    	
    	//Filtar filtros
    	if( $arrParametros['epemunicipio'] != '')
    	{
    		$arrWhere[] = "epemunicipio	ilike '%{$arrParametros['epemunicipio']}%' ";
    	}
		if( $arrParametros['epecod']!= '')
		{
			$arrWhere[] = "epecod	= '{$arrParametros['epecod']}' ";
		}
		if( $arrParametros['epenome']!= '')
		{
			$arrWhere[] = "epenome	ilike '%{$arrParametros['epenome']}%' ";
		}
    	
    	//@TODO VERIFICA SAVES de check
    	// Verifica as colunas
    	if(($grupo == 1) || ($grupo == 3))
    	{
    		// Acao, Estado, Municipio, Localização, Nome da Escola, Código Inep, Carga horária diária
    		$colunas = "
				estuf,
				epemunicipio,
    			eperede,
    			epelocalizacao,
				epenome ,
				epecod ,
				eperede,
				'escolher' as cargahorariadiaria
    		";
    	}else if($grupo == 2)
    	{
    		//  Acao, Estado, Municipio, Localização, Nome da Escola, Código Inep, INSE e Carga horária diária
    		$colunas = "
				estuf,
				epemunicipio,
    			eperede,
				epelocalizacao,
				epenome ,
				epecod ,
				epeinse,
				'escolher' as cargahorariadiaria
    		";
    	}
    	
    	$objAdesaoProemi = new Par3_Model_Adesaoescolaproemi();
    	$totalInseridos = $objAdesaoProemi->retornaQtdSalvoGrupo($arrParametros);
    	 
    	$arrCodInepInseridos = $objAdesaoProemi->retornaCodEntidadeMarcados($arrParametros);
    	$arrCodInepInseridos = (is_array($arrCodInepInseridos)) ? $arrCodInepInseridos : Array();
    	$arrCargaHoraria	 = $objAdesaoProemi->retornaValoresCargaHoraria($arrParametros);
    	$arrCargaHoraria	 = (is_array($arrCargaHoraria)) ? $arrCargaHoraria : Array();
    	
    	// Verifica o filtro que traz somente os marcados
    	if( $arrParametros['aepcargahoraria'] != '' )
    	{
    		if( count($arrCargaHoraria) > 0 )
    		{
    			 
    			foreach($arrCargaHoraria as $k => $v )
    			{
    				if($v == $arrParametros['aepcargahoraria'])
    				{
    					$arrCargaHoraria[] = $k;
    				}
    			}
    			 
    			if( count($arrCargaHoraria) > 0 )
    			{
    				$arrWhere[] = "epecod::integer in ( ". implode(', ', $arrCargaHoraria) ." )";
    	
    			}
    			else
    			{
    				$arrWhere[] = "false";
    			}
    		}
    		else
    		{
    			$arrWhere[] = "false";
    		}
    	}
    	 
    	if( $arrParametros['checado'] != '' )
    	{
    		$not = '';
    		if( $arrParametros['checado'] == 'N' )
    		{
    			$not = 'not';
    		}
    		if( count($arrCodInepInseridos) > 0 )
    		{
    			$arrWhere[] = "epecod::integer {$not} in ( ". implode(', ', $arrCodInepInseridos) ." )";
    		}
    		else
    		{
    			$arrWhere[] = "false=false";
    		}
    	
    	}
    	
    	// Trata clausulas
    	if( is_array($arrWhere) )
    	{
    		$WHERE = (is_array($arrWhere) ? '('.implode(' AND ', $arrWhere).')' : '');
    	}
    	// Monta select
    	$sql = "
	    	SELECT
	    		{$colunas}
	    	FROM
	    		{$this->stNomeTabela}
	    	WHERE
	    		{$WHERE}
    	";
    	// Busca resultado
    	$resultado = $this->carregar($sql);
    	// Trata retorno
    	$resultado = (is_array($resultado)) ? $resultado : array();
    	// incremento
    	$i = 0;
    	 
    	
    	
    	
    	if( count( $resultado ) > 0 ) 
    	{
    		// Existe registro
    		$statusReturn = 'success';
    		
    		$disabledEdicao = '';
    		
    		if($bloqueiaEdicao)
    		{
    			$disabledEdicao = 'disabled';
    		}
    		
    		foreach($resultado as $k => $escola)
    		{
    			$i++;
    			// Trata o retorno
    			
    			if( (($totalInseridos == 0) && (($grupo == 2) || ($grupo == 1))) || (in_array($escola['epecod'], $arrCodInepInseridos)) ) // @todo verificar se já foi marcado tb 
    			{
    				$arrEscolas[$i]['acao'] 			= "SIM";
    			}
    			else
    			{
    				$arrEscolas[$i]['acao'] 			= "";
    			}
    			
    			
    			$arrEscolas[$i]['estuf'] 			= $escola['estuf'];
    			$arrEscolas[$i]['epemunicipio'] 	= $escola['epemunicipio'];
    			$arrEscolas[$i]['eperede'] 			= $escola['eperede'];
    			$arrEscolas[$i]['epelocalizacao']	= $escola['epelocalizacao'];
    			$arrEscolas[$i]['epenome'] 			= $escola['epenome'];
    			$arrEscolas[$i]['epecod'] 			= $escola['epecod'];
    			if($grupo == 2)
    			{
    				$arrEscolas[$i]['epeinse'] 			= $escola['epeinse'];
    			}
    			//@todo tratar carga horária
    			if($respostaCargaHoraria == 'S')
    			{
    				if( (in_array($escola['epecod'], $arrCodInepInseridos)) ) 
    				{
    					   					
    					$arrEscolas[$i]['carga_horaria'] = $arrCargaHoraria[$escola['epecod']];
    				}
    				else
    				{
    					$arrEscolas[$i]['carga_horaria'] = "";
    				}
    			}
	    		
    		}
    	}

    	$cabecalho[] = 'Selecionada';
    	$cabecalho[] = 'Estado';
    	$cabecalho[] = 'Município';
    	$cabecalho[] = 'Dependência Administrativa';
    	$cabecalho[] = 'Localização';
    	$cabecalho[] = 'Nome da Escola';
    	$cabecalho[] = 'Código INEP';
    	if($grupo == 2)
    	{
    		$cabecalho[] = 'INSE';
    	}
    	//@todo tratar carga horária
    	if($respostaCargaHoraria == 'S')
    	{
    		$cabecalho[] = 'Carga Horária Diária';
    	}
    	
    	$arrayRetorno['cabecalho'] 	= $cabecalho;
    	$arrayRetorno['arrayEscolas'] = $arrEscolas;
    	$arrayRetorno['status'] 	= $statusReturn;
    	$arrayRetorno['bloqueio'] 	= $bloqueiaEdicao;
    	
    	return $arrayRetorno;
    	
    }
    /**
     * 
     */
    public function getListaEscolasEscolhidas($arrParametros = array())
    {
    	
		$inuid = $arrParametros['inuid'];
		$grupo = $arrParametros['grupo'];
		
    	$sqlDados =  "SELECT 
				distinct
				epe.estuf,
				epe.epemunicipio,
				epe.eperede,
				aep.codinep,
				epe.epenome,
				CASE WHEN aepcargahoraria IS NOT NULL THEN
					aepcargahoraria::varchar
				ELSE
					'Não'
				END as carga_horaria,
				CASE WHEN aepinseridaaposenvio = TRUE THEN
					'Sim'
				ELSE
					''
				END
			FROM par3.escolasproemi2015  epe
			INNER JOIN par3.adesaoescolaproemi aep ON aep.codinep::VARCHAR = epe.epecod 
			WHERE 
				inuid = {$inuid}
			AND
				aepgrupo = {$grupo}";
    	
    	$cabecalho 	= array('UF','Município','Dependência Administrativa','Código da Escola','Escola', 'Carga Horária Diária', 'Inserido após o Envio ao MEC');
    	$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    	$listagem->setCabecalho($cabecalho);
    	$listagem->setQuery($sqlDados);
    	$listagem->turnOffForm();
    	$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    	
    }
    /**
     *  Método para retornar as escolas que estão no contexto que está sendo inserido
     * @return array
     */
    public function getEscolasContexto($arrParametros = array())
    {
    	 $statusReturn = 'null';
    	 
    	// CArrega variáveis
    	$itrid = $arrParametros['itrid'];
    	$grupo = $arrParametros['grupo'];
    	$prgid = $arrParametros['prgid'];
    	$inuid = $arrParametros['inuid'];
    	// Inicializa arr de escolas
    	$arrEscolas = array();
    	
    	//Instancia carga horária
    	$objRespostaCargaHoraria = new Par3_Model_Respostacargahorariaprograma();
    	// Trata dados para consultar carga horária
    	$dados['inuid'] = $inuid;
    	$dados['prgid'] = $prgid;
    	// Consulta resposta de carga horária
    	$arrRespostaCargaHoraria 	= $objRespostaCargaHoraria->getResposta( $dados );
    	$respostaCargaHoraria 		= $arrRespostaCargaHoraria['rchresposta'] ;
    	
    	// Estadual
    	if(($itrid == 1) || ($itrid == 3))
    	{
    		$arrWhere[] = "estuf	= '{$arrParametros['estuf']}' ";
    		$arrWhere[] = "epegrupo = '{$grupo}' ";
    		$arrWhere[] = "eperede 	= 'Estadual' "; 
    	
    	}
    	else
    	{
    		$arrWhere[] = "muncod	= '{$arrParametros['muncod']}' ";
    		$arrWhere[] = "epegrupo = '{$grupo}' ";
    		$arrWhere[] = "eperede 	= 'Municipal' ";
    	}
    	
    	//Filtar filtros
    	if( $arrParametros['epemunicipio'] != '')
    	{
    		$arrWhere[] = "epemunicipio	ilike '%{$arrParametros['epemunicipio']}%' ";
    	}
		if( $arrParametros['epecod']!= '')
		{
			$arrWhere[] = "epecod	= '{$arrParametros['epecod']}' ";
		}
		if( $arrParametros['epenome']!= '')
		{
			$arrWhere[] = "epenome	ilike '%{$arrParametros['epenome']}%' ";
		}
    	
		$objAdesaoProemi = new Par3_Model_Adesaoescolaproemi();
		$totalInseridos = $objAdesaoProemi->retornaQtdSalvoGrupo($arrParametros);
		
		$arrCodInepInseridos = $objAdesaoProemi->retornaCodEntidadeMarcados($arrParametros);
		$arrCodInepInseridos = (is_array($arrCodInepInseridos)) ? $arrCodInepInseridos : Array();
		$arrCargaHoraria	 = $objAdesaoProemi->retornaValoresCargaHoraria($arrParametros);
		$arrCargaHoraria	 = (is_array($arrCargaHoraria)) ? $arrCargaHoraria : Array();
		 
		// Verifica o filtro que traz somente os marcados
		 
		// Verifica o filtro que traz somente os marcados
		if( $arrParametros['aepcargahoraria'] != '' )
		{
			if( count($arrCargaHoraria) > 0 )
			{
				 
				foreach($arrCargaHoraria as $k => $v )
				{
					if($v == $arrParametros['aepcargahoraria'])
					{
						$arrCargaHoraria[] = $k;
					}
				}
				 
				if( count($arrCargaHoraria) > 0 )
				{
					$arrWhere[] = "epecod::integer in ( ". implode(', ', $arrCargaHoraria) ." )";
		
				}
				else
				{
					$arrWhere[] = "false";
				}
			}
			else
			{
				$arrWhere[] = "false";
			}
		}
		 
		if( $arrParametros['checado'] != '' )
		{
			$not = '';
			if( $arrParametros['checado'] == 'N' )
			{
				$not = 'not';
			}
			if( count($arrCodInepInseridos) > 0 )
			{
				$arrWhere[] = "epecod::integer {$not} in ( ". implode(', ', $arrCodInepInseridos) ." )";
			}
			else
			{
				$arrWhere[] = "false=false";
			}
		
		}
		
    	
    	$colunas = "
			epecod
    		";
    	
    	// Trata clausulas
    	if( is_array($arrWhere) )
    	{
    		$WHERE = (is_array($arrWhere) ? '('.implode(' AND ', $arrWhere).')' : '');
    	}
    	// Monta select
    	$sql = "
	    	SELECT
	    		{$colunas}
	    	FROM
	    		{$this->stNomeTabela}
	    	WHERE
	    		{$WHERE}
    	";
    	// Busca resultado
    	$resultado = $this->carregar($sql);
    	
    	// Trata retorno
    	$resultado = (is_array($resultado)) ? $resultado : array();
    	
    	foreach( $resultado as $k => $v)
    	{
    		$arrRetorno[] = $v['epecod'];
    	}
    	
    	return $arrRetorno;
    	
    }
    
    
    
}

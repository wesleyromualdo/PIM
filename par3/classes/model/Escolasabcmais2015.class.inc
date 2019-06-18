<?php
/**
 * Classe de mapeamento da entidade par3.escolasabcmais2015.
 *
 * @version $Id$
 * @since 2016.09.29
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Escolasabcmais2015: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Escolasabcmais2015($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Escolasabcmais2015($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $eamgrupo 
 * @property string $eamrecebeurecursopddeintegral 
 * @property numeric $eamrecursopddeintegral 
 * @property string $eamidebbaixo 
 * @property string $eamidebanosfinaisbaixo 
 * @property string $eamidebanosiniciaisbaixo 
 * @property numeric $eamidebanosfinais 
 * @property numeric $eamidebanosiniciais 
 * @property string $eaminse 
 * @property string $eammotivonaoapta 
 * @property string $eamapta 
 * @property string $eampossuiuexnaoconsorciada 
 * @property string $eampossuivintematfundamental 
 * @property int $eammatriculafundamental 
 * @property int $eamtotalmatricula 
 * @property string $eamcod 
 * @property string $eamnome 
 * @property string $eamrede 
 * @property string $eamlocalizacao 
 * @property string $muncod 
 * @property string $eammunicipio 
 * @property string $estuf 
 * @property int $eamid  - default: nextval('par3.escolasabcmais2015_eamid_seq'::regclass)
 */
class Par3_Model_Escolasabcmais2015 extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.escolasabcmais2015';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    		'eamid'
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
        'eamgrupo' => null,
        'eamrecebeurecursopddeintegral' => null,
        'eamrecursopddeintegral' => null,
        'eamidebbaixo' => null,
        'eamidebanosfinaisbaixo' => null,
        'eamidebanosiniciaisbaixo' => null,
        'eamidebanosfinais' => null,
        'eamidebanosiniciais' => null,
        'eaminse' => null,
        'eammotivonaoapta' => null,
        'eamapta' => null,
        'eampossuiuexnaoconsorciada' => null,
        'eampossuivintematfundamental' => null,
        'eammatriculafundamental' => null,
        'eamtotalmatricula' => null,
        'eamcod' => null,
        'eamnome' => null,
        'eamrede' => null,
        'eamlocalizacao' => null,
        'muncod' => null,
        'eammunicipio' => null,
        'estuf' => null,
        'eamid' => null,
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
            'eamgrupo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'eamrecebeurecursopddeintegral' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'eamrecursopddeintegral' => array('allowEmpty' => true),
            'eamidebbaixo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'eamidebanosfinaisbaixo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'eamidebanosiniciaisbaixo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'eamidebanosfinais' => array('allowEmpty' => true),
            'eamidebanosiniciais' => array('allowEmpty' => true),
            'eaminse' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'eammotivonaoapta' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'eamapta' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'eampossuiuexnaoconsorciada' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 9))),
            'eampossuivintematfundamental' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'eammatriculafundamental' => array('allowEmpty' => true,'Digits'),
            'eamtotalmatricula' => array('allowEmpty' => true,'Digits'),
            'eamcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 12))),
            'eamnome' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'eamrede' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 12))),
            'eamlocalizacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 12))),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'eammunicipio' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'eamid' => array('Digits'),
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
     * Método para retornar a lista de escolar passíveis de serem eleitas
     * itrid = 1 // Estadual
     * itrid = 2 // Municipal
     * itrid = 3 // df
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
    	$adpid 			= $arrParametros['adpid'];
    	$adpano_ciclo   = $_SESSION['par3']['adpano_ciclo'];
    	$cargaHoraria	= $arrParametros['carga_horaria'];
    	$rchid 			= $arrParametros['rchid'];
    	// Inicializa arr de escolas
    	$arrEscolas = array();
    	 
    	if($adpid){
        	$docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma 
                                    WHERE adpid = $adpid AND pfaid = (  SELECT pfaid FROM par3.proadesao 
                                                                        WHERE prgid = {$prgid} AND pfaano = $adpano_ciclo AND pfastatus = 'A')");
    	}
    	 
    	$bloqueiaEdicao = FALSE;
    	
    	$modelProadesao = new Par3_Model_Proadesao();
    	$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($prgid);
    	
    	if($docid){
    	    
    		$situacaoAtual = wf_pegarEstadoAtual( $docid );
    
    		if( 
                ($situacaoAtual['esdid'] == WF_ESDID_TERMONAOACEITO_ABCMAIS) || 
    		    ($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_ABCMAIS)
		    ){
    			$bloqueiaEdicao = TRUE;
    		}
    	}
    	
    	$bloqueiaEdicaoData = false;
    	
    	if( strtotime($_SESSION['par3']['mais_escolas']['vigencia'][$adpano_ciclo] . ' 12:00:00') < strtotime(date('Y-m-d G:i:s')) )
    	{
    		$bloqueiaEdicao = TRUE;
    		$bloqueiaEdicaoData = TRUE;
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
    	 
    	$arrWhere[] = "aemano_ciclo = $adpano_ciclo "; 
    	
    	// Estadual
    	if( ($itrid == 1) || ($itrid == 3) )
    	{
    		$arrWhere[] = "estuf	= '{$arrParametros['estuf']}' ";
    		$arrWhere[] = "eamgrupo = '{$grupo}' ";
    		$arrWhere[] = "eamrede 	= 'Estadual' ";
    		$campomunicipio = 'eammunicipio,';
    	}
    	else
    	{
    		$arrWhere[] = "muncod	= '{$arrParametros['muncod']}' ";
    		$arrWhere[] = "eamgrupo = '{$grupo}' ";
    		$arrWhere[] = "eamrede 	= 'Municipal' ";
    		$campomunicipio = '';
    		//@todo verifica $arrWhere[] = "eamrede 	= 'Estadual' ";
    	}
    	
    	
    	//Filtar filtros
    	if( $arrParametros['eammunicipio'] != '')
    	{
    		// Caso seja municipal, pode ignorar, já que só aparecera a nível de informação, pois já teremos o muncod
    		if($itrid == 1)
    		{
    			$arrWhere[] = "eammunicipio	ilike '%{$arrParametros['eammunicipio']}%' ";
    		}
    
    	}
    	if( $arrParametros['eamrede'] != '')
    	{
    		$arrWhere[] = "eamrede	= '{$arrParametros['eamrede']}' ";
    	}
    	if( $arrParametros['eamcod'] != '')
    	{
    		$arrWhere[] = "eamcod	= '{$arrParametros['eamcod']}' ";
    	}
    	if( $arrParametros['eamnome'] != '')
    	{
    		$arrWhere[] = "eamnome	ilike '%{$arrParametros['eamnome']}%' ";
    	}
    	 
    	 
    	//@TODO VERIFICA SAVES de check
    	// Verifica as colunas
    	if(($grupo == 1) || ($grupo == 3))
    	{
    		// Acao, Estado, Municipio, Localização, Nome da Escola, Código Inep, Carga horária diária
    		$colunas = "
    		estuf,
    		{$campomunicipio}
    		eamlocalizacao,
    		eamnome,
    		eamcod
    		";
    			
    			
    	}else if($grupo == 2)
    	{
    		//  Acao, Estado, Municipio, Localização, Nome da Escola, Código Inep, ideb anos iniciais, idebanosfinais, e Carga horária diária
    		$colunas = "
    		estuf,
    		{$campomunicipio}
    		eamnome,
    		eamcod,
    		eamlocalizacao,
    		eamidebanosiniciais,
    		eamidebanosfinais,
    		eaminse
    		";
    	}
    	
    	$objAdesaoAbcmais = new Par3_Model_Adesaoescolaabcmais();
    	$totalInseridos = $objAdesaoAbcmais->retornaQtdSalvoGrupo($arrParametros);
    	
    	$objExcecao = 	new Par3_Model_Excecaoescolasprograma();
    	
    	$arrCodInepInseridos =	$objAdesaoAbcmais->retornaCodEntidadeMarcados($arrParametros);
    	$arrCodInepInseridos = (is_array($arrCodInepInseridos)) ? $arrCodInepInseridos : Array();
    	
    	$arrCargaHoraria	 = $objAdesaoAbcmais->retornaValoresCargaHoraria($arrParametros);
    	$arrCargaHoraria	 = (is_array($arrCargaHoraria)) ? $arrCargaHoraria : Array();
    	
    	
    	// Verifica o filtro que traz somente os marcados
    	if( $arrParametros['aeacargahoraria'] != '' )
    	{
    		if( count($arrCargaHoraria) > 0 )
    		{
    			 
    			foreach($arrCargaHoraria as $k => $v )
    			{
    				if($v == $arrParametros['aeacargahoraria'])
    				{
    					$arrCargaHorariaClause[] = $k;
    				}
    			}
    			 
    			if( count($arrCargaHorariaClause) > 0 )
    			{
    				$arrWhere[] = "eamcod::integer in ( ". implode(', ', $arrCargaHorariaClause) ." )";
    				
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
    			$arrWhere[] = "eamcod::integer {$not} in ( ". implode(', ', $arrCodInepInseridos) ." )";
    			
    		}
    	
    	}
    	
    	
    	if($arrParametros['ordenacao'])
    	{
    		 
			 $orderBy = "";
			 
			 switch ($arrParametros['ordenacao'])
			 {
			 	case "eammunicipio":
			 		$orderBy = "ORDER BY eammunicipio ASC";
			 		break;
			 	case "eamlocalizacao":
			 		$orderBy = "ORDER BY eamlocalizacao ASC";
			 		break;
			 	case "eamnome":
			 		$orderBy = "ORDER BY eamnome ASC";
			 		break;
			 	case "eamcod":
			 		$orderBy = "ORDER BY eamcod ASC";
			 		break;
			 	case "eamidebanosiniciais":
			 		$orderBy = "ORDER BY eamidebanosiniciais ASC";
			 		break;
			 	case "eamidebanosfinais":
			 		$orderBy = "ORDER BY eamidebanosfinais ASC";
			 		break;
			 	case "eaminse":
			 		$orderBy = "ORDER BY eaminse ASC";
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
    	
    	if( count( $resultado ) > 0 ){
    		
    		$disabledJainseridas = '';
    		$disabledEdicao = '';
    
    		if($bloqueiaEdicao) $disabledEdicao = 'disabled';
    		
    		$pfaid = 6; // abcmais
    		$modelAdesaoPrograma =  new Par3_Model_AdesaoPrograma();
    		$result = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid);
    		$docidInserido = $result['docid'];
    		$situacaoAtual = wf_pegarEstadoAtual( $docidInserido);
    		$esdid = $situacaoAtual['esdid'];
    		if(($esdid) &&  ($esdid == WF_ESDID_ENVIADOPARAOMEC_ABCMAIS ))
    		{
    			 
    			$arrParamExcecao['inuid'] = $inuid;
    			$arrParamExcecao['grupo'] = $grupo;
    			$arrParamExcecao['prgid'] = $prgid;
    			 
    			$temSolicitacao = $objExcecao->possuiSolicitacaoPorGrupo($arrParamExcecao);
    			 
    			if(($temSolicitacao == true) && ( ! $bloqueiaEdicaoData) )
    			{
    				$disabledEdicao = '';
    				$disabledJainseridas = 'disabled';
    			}
    		}
    		
    		
    		// Existe registro
    		$statusReturn = 'success';
    		foreach($resultado as $k => $escola)
    		{
    			$i++;
    			// Trata o retorno
    			 
    			//@todo para montar veriricar os checks na tabela nova
    			if( (( $totalInseridos == 0) && (($grupo == 2) || ($grupo == 1))) || (in_array($escola['eamcod'], $arrCodInepInseridos)) ) // @todo verificar se já foi marcado tb
    			{
    				if(  $cargaHoraria)
    				{
    					$arrEscolas[$i]['acao'] 			= "<input {$disabledJainseridas} {$disabledEdicao} class=\"checkbox-primary\" checked type=checkbox name=\"escola_checada[]\" id=\"{$escola['eamcod']}\" value=\"{$escola['eamcod']}\">";
    				}
    			}
    			else
    			{
    				if(  $cargaHoraria )
    				{
    					$arrEscolas[$i]['acao'] 			= "<input {$disabledEdicao} class=\"checkbox-primary\" type=checkbox name=\"escola_checada[]\" id=\"{$escola['eamcod']}\" value=\"{$escola['eamcod']}\">";
    				}
    			}
    			 
    			$arrEscolas[$i]['estuf'] 			= $escola['estuf'];
    			if($campomunicipio != '')
    			{
    				$arrEscolas[$i]['eammunicipio'] 	= $escola['eammunicipio'];
    			}
    			$arrEscolas[$i]['eamlocalizacao']	= $escola['eamlocalizacao'];
    			$arrEscolas[$i]['eamnome'] 			= $escola['eamnome'];
    			$arrEscolas[$i]['eamcod'] 			= $escola['eamcod'];
    			 
    			if($grupo == 2)
    			{
    				$arrEscolas[$i]['eamidebanosiniciais'] 			= $escola['eamidebanosiniciais'];
    				$arrEscolas[$i]['eamidebanosfinais'] 			= $escola['eamidebanosfinais'];
    				$arrEscolas[$i]['eaminse'] 			= $escola['eaminse'];
    			}
    			 
    			if($respostaCargaHoraria == 'S' || $cargaHoraria == 'S')
    			{
    				if($cargaHoraria == 'S')
    				{
	    				if((in_array($escola['eamcod'], $arrCodInepInseridos)))
	    				{
	    					$cheked5 = '';
	    					$cheked15 = '';
	    					if($arrCargaHoraria[$escola['eamcod']] == '15')
	    					{
	    						$cheked15 = 'selected';
	    					}
	    					elseif($arrCargaHoraria[$escola['eamcod']] == '5')
	    					{
	    						$cheked5 = 'selected';
	    					}
	    					if($cheked15 != '' || $cheked5 != '')
	    					{
	    						$disabledJainseridas2 = $disabledJainseridas;
	    					}
	    					else 
	    					{
	    						$disabledJainseridas2 = '';
	    					}
	    						
	    					$arrEscolas[$i]['carga_horaria'] = "<select {$disabledEdicao} {$disabledJainseridas2} id=\"select_{$escola['eamcod']}\" name=\"carga_horaria_{$escola['eamcod']}\" >
	    					<option value=\"\" >-</option>
	    					<option {$cheked5}  value=\"5\" >5</option>
	    					<option {$cheked15} value=\"15\" >15</option>
	    					</select>";
	    				}
	    				else
	    				{
	    					$arrEscolas[$i]['carga_horaria'] =
	    					"<select {$disabledEdicao} id=\"select_{$escola['eamcod']}\" name=\"carga_horaria_{$escola['eamcod']}\" >
	    					<option value=\"\" >-</option>
	    					<option value=\"5\" >5</option>
	    					<option value=\"15\" >15</option>
	    					</select>";
	    				}
    				}
    			}
    		}
    	}
    	 
    	//Monta cabeçalho
    	if($cargaHoraria) $cabecalho[] = 'Selecionar';
    	
    	$cabecalho[] = 'Estado';
    	if($campomunicipio != '') $cabecalho[] = 'Município';

    	$cabecalho[] = 'Localização';
    	$cabecalho[] = 'Nome da Escola';
    	$cabecalho[] = 'Código INEP';
    	if($grupo == 2)
    	{
    		$cabecalho[] = 'IDEB (Anos Iniciais)';
    		$cabecalho[] = 'IDEB (Anos Finais)';
    		$cabecalho[] = 'INSE';
    	}
    	if($respostaCargaHoraria == 'S' || $cargaHoraria == 'S')
    	{
    		if($cargaHoraria == 'S')
    		{
    			$cabecalho[] = 'Carga Horária Semanal';
    		}
    	}
    	 
    	$arrayRetorno['cabecalho'] = $cabecalho;
    	$arrayRetorno['arrayEscolas'] = $arrEscolas;
    	$arrayRetorno['status'] 	= $statusReturn;
    	$arrayRetorno['bloqueio'] 	= $bloqueiaEdicao;
    	return $arrayRetorno;
    }
    /**
     * Método para retornar a lista de escolar passíveis de serem eleitas
     * itrid = 1 // Estadual
     * itrid = 2 // Municipal
     * itrid = 3 // DF
     * @return array
     */
    public function getListaEscolasNaoElegiveis($arrParametros = array())
    {

    	require_once APPRAIZ.'includes/workflow.php';
    	
    	$statusReturn = 'null';
    	
    	// CArrega variáveis
    	$itrid         = $arrParametros['itrid'];
    	$grupo         = $arrParametros['grupo'];
    	$prgid         = $arrParametros['prgid'];
    	$inuid         = $arrParametros['inuid'];
    	$adpid         = $arrParametros['adpid'];
    	$adpano_ciclo  = $arrParametros['adpano_ciclo'];
    	// Inicializa arr de escolas
    	$arrEscolas = array();
    	
    	if($adpid){
        	$docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma 
                                    WHERE adpid = {$adpid} AND pfaid = (SELECT pfaid FROM par3.proadesao 
                                                                        WHERE prgid = {$prgid} AND pfaano = $adpano_ciclo AND pfastatus = 'A')");
    	}
    	
    	$bloqueiaEdicao = FALSE;
    	if($docid){
    	    
    		$situacaoAtual = wf_pegarEstadoAtual( $docid );
    		
    		if( 
                ($situacaoAtual['esdid'] == WF_ESDID_TERMONAOACEITO_ABCMAIS) || 
    		    ($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_ABCMAIS)
		    ){
    			 $bloqueiaEdicao = TRUE;
    		}
    	}
    	
    	//Instancia carga horária
    	$objRespostaCargaHoraria = new Par3_Model_Respostacargahorariaprograma();
    	// Trata dados para consultar carga horária
    	$dados['inuid'] = $inuid;
    	$dados['adpid'] = $adpid;
    	$dados['prgid'] = $prgid;
    	// Consulta resposta de carga horária
    	$arrRespostaCargaHoraria 	= $objRespostaCargaHoraria->getResposta( $dados );
    	$respostaCargaHoraria 		= $arrRespostaCargaHoraria['rchresposta'] ;
    	
    	// Estadual
   		if(($itrid == 1) || ($itrid == 3))
    	{
    		$arrWhere[] = "eamapta = 'N' ";
    		$arrWhere[] = "eamrede = 'Estadual' ";
    		$arrWhere[] = "estuf	= '{$arrParametros['estuf']}' ";
    	
    	}
    	else
    	{
    		$arrWhere[] = "eamapta = 'N' ";
    		$arrWhere[] = "eamrede = 'Municipal' ";
    		$arrWhere[] = "muncod	= '{$arrParametros['muncod']}' ";
    	}
    	
    	//Filtar filtros
    	if( $arrParametros['eammunicipio'] != '')
    	{
    		// Caso seja municipal, pode ignorar, já que só aparecera a nível de informação, pois já teremos o muncod
    		if($itrid == 1)
    		{
    			$arrWhere[] = "eammunicipio	ilike '%{$arrParametros['eammunicipio']}%' ";
    		}
    		
    	}
    	if( $arrParametros['eamrede'] != '')
    	{
    		$arrWhere[] = "eamrede	= '{$arrParametros['eamrede']}' ";
    	}
    	if( $arrParametros['eamcod'] != '')
    	{
    		$arrWhere[] = "eamcod	= '{$arrParametros['eamcod']}' ";
    	}
    	if( $arrParametros['eamnome'] != '')
    	{
    		$arrWhere[] = "eamnome	ilike '%{$arrParametros['eamnome']}%' ";
    	}
    	
    	/*Trata ano do Ciclo*/
    	$arrWhere[] = "aemano_ciclo = $adpano_ciclo";
    	
    	if($arrParametros['ordenacao'])
    	{
    		 
    		$orderBy = "";
    	
    		switch ($arrParametros['ordenacao'])
    		{
    			case "eammunicipio":
    				$orderBy = "ORDER BY eammunicipio ASC";
    				break;
    			case "eamlocalizacao":
    				$orderBy = "ORDER BY eamlocalizacao ASC";
    				break;
    			case "eamnome":
    				$orderBy = "ORDER BY eamnome ASC";
    				break;
    			case "eamcod":
    				$orderBy = "ORDER BY eamcod ASC";
    				break;
    			case "eamidebanosiniciais":
    				$orderBy = "ORDER BY eamidebanosiniciais ASC";
    				break;
    			case "eamidebanosfinais":
    				$orderBy = "ORDER BY eamidebanosfinais ASC";
    				break;
    			case "eaminse":
    				$orderBy = "ORDER BY eaminse ASC";
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
				eammunicipio,
				eamrede,
				eamcod,
				eamnome,
				eammotivonaoapta
			FROM 
				par3.escolasabcmais2015  
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
    	
    	//Monta cabeçalho
    	$cabecalho = array( 'UF', 'Município', 'Dependência administrativa', 'Código da Escola', 'Escola', 'Motivo');
    	$arrayRetorno['cabecalho'] = $cabecalho;
    	$arrayRetorno['arrayEscolas'] = $resultado;
    	$arrayRetorno['status'] 	= $statusReturn;
    	return $arrayRetorno;
    }
    
    /**
     * 
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
    	$adpid 			= $arrParametros['adpid'];
    	$adpano_ciclo   = $_SESSION['par3']['adpano_ciclo'];
    	
    	// Inicializa arr de escolas
    	$arrEscolas = array();
    	
    	$docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma WHERE inuid = {$inuid} and pfaid = (select pfaid from par3.proadesao where prgid = {$prgid})");
    	
    	$bloqueiaEdicao = FALSE;
    	if($docid)
    	{
    		$situacaoAtual = wf_pegarEstadoAtual( $docid );
    		
    		if( ($situacaoAtual['esdid'] == WF_ESDID_TERMONAOACEITO_ABCMAIS) || ($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_ABCMAIS))
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
    	
    	$arrWhere[] = "aemano_ciclo = $adpano_ciclo "; 
    	
    	// Estadual
    	if( ($itrid == 1) || ($itrid == 3))
    	{
    		$arrWhere[] = "estuf	= '{$arrParametros['estuf']}' ";
    		$arrWhere[] = "eamgrupo = '{$grupo}' ";
    		$arrWhere[] = "eamrede 	= 'Estadual' ";
    		$campomunicipio = 'eammunicipio,';
    	}
    	else
    	{
    		$arrWhere[] = "muncod	= '{$arrParametros['muncod']}' ";
    		$arrWhere[] = "eamgrupo = '{$grupo}' ";
    		$campomunicipio = '';
    		//@todo verifica $arrWhere[] = "eamrede 	= 'Estadual' ";
    	}
    	
    	//Filtar filtros
    	if( $arrParametros['eammunicipio'] != '')
    	{
    		// Caso seja municipal, pode ignorar, já que só aparecera a nível de informação, pois já teremos o muncod
    		if($itrid == 1)
    		{
    			$arrWhere[] = "eammunicipio	ilike '%{$arrParametros['eammunicipio']}%' ";
    		}
    		
    	}
    	if( $arrParametros['eamrede'] != '')
    	{
    		$arrWhere[] = "eamrede	= '{$arrParametros['eamrede']}' ";
    	}
    	if( $arrParametros['eamcod'] != '')
    	{
    		$arrWhere[] = "eamcod	= '{$arrParametros['eamcod']}' ";
    	}
    	if( $arrParametros['eamnome'] != '')
    	{
    		$arrWhere[] = "eamnome	ilike '%{$arrParametros['eamnome']}%' ";
    	}
    	
    	
    	//@TODO VERIFICA SAVES de check
    	// Verifica as colunas
    	if(($grupo == 1) || ($grupo == 3))
    	{
    		// Acao, Estado, Municipio, Localização, Nome da Escola, Código Inep, Carga horária diária
    		$colunas = "
				 estuf,
				 {$campomunicipio}
				 eamlocalizacao, 
				 eamnome,
				 eamcod
    		";
			
				 
    	}else if($grupo == 2)
    	{
    		//  Acao, Estado, Municipio, Localização, Nome da Escola, Código Inep, ideb anos iniciais, idebanosfinais, e Carga horária diária
    		$colunas = "
				  estuf,
				  {$campomunicipio} 
				 eamnome,
				 eamcod,
				 eamlocalizacao,
				 eamidebanosiniciais,
				 eamidebanosfinais,
				 eaminse
    		";
    	}
    	

    	$objAdesaoAbcmais = new Par3_Model_Adesaoescolaabcmais();
    	$totalInseridos = $objAdesaoAbcmais->retornaQtdSalvoGrupo($arrParametros);
    	 
    	$arrCodInepInseridos =	$objAdesaoAbcmais->retornaCodEntidadeMarcados($arrParametros);
    	$arrCodInepInseridos = (is_array($arrCodInepInseridos)) ? $arrCodInepInseridos : Array();
    	$arrCargaHoraria	 = $objAdesaoAbcmais->retornaValoresCargaHoraria($arrParametros);
    	$arrCargaHoraria	 = (is_array($arrCargaHoraria)) ? $arrCargaHoraria : Array();
    	
    	

    	// Verifica o filtro que traz somente os marcados
    	if( $arrParametros['aeacargahoraria'] != '' )
    	{
    		if( count($arrCargaHoraria) > 0 )
    		{
    	
    			foreach($arrCargaHoraria as $k => $v )
    			{
    				if($v == $arrParametros['aeacargahoraria'])
    				{
    					$arrCargaHorariaClause[] = $k;
    				}
    			}
    	
    			if( count($arrCargaHorariaClause) > 0 )
    			{
    				$arrWhere[] = "eamcod::integer in ( ". implode(', ', $arrCargaHorariaClause) ." )";
    	
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
    			$arrWhere[] = "eamcod::integer {$not} in ( ". implode(', ', $arrCodInepInseridos) ." )";
    			 
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
    		$disabledEdicao = '';
    		
    		if($bloqueiaEdicao)
    		{
    			$disabledEdicao = 'disabled';
    		}
    		
    		// Existe registro
    		$statusReturn = 'success';
    		foreach($resultado as $k => $escola)
    		{
    			$i++;
    			// Trata o retorno
    			
    			//@todo para montar veriricar os checks na tabela nova
    			if( (( $totalInseridos == 0) && (($grupo == 2) || ($grupo == 1))) || (in_array($escola['eamcod'], $arrCodInepInseridos)) ) // @todo verificar se já foi marcado tb
    			{
    				$arrEscolas[$i]['acao'] 			= "SIM";
    			}
    			else 
    			{
    				$arrEscolas[$i]['acao'] 			= "";
    			}
    			
    			$arrEscolas[$i]['estuf'] 			= $escola['estuf'];
    			if($campomunicipio != '')
    			{
    				$arrEscolas[$i]['eammunicipio'] 	= $escola['eammunicipio'];
    			}
    			$arrEscolas[$i]['eamlocalizacao']	= $escola['eamlocalizacao'];
    			$arrEscolas[$i]['eamnome'] 			= $escola['eamnome'];
    			$arrEscolas[$i]['eamcod'] 			= $escola['eamcod'];
    			
    			if($grupo == 2)
    			{
    				$arrEscolas[$i]['eamidebanosiniciais'] 			= $escola['eamidebanosiniciais'];
    				$arrEscolas[$i]['eamidebanosfinais'] 			= $escola['eamidebanosfinais'];
    				$arrEscolas[$i]['eaminse'] 			= $escola['eaminse'];
    			}
    			
    			if($respostaCargaHoraria == 'S')
    			{
    				if((in_array($escola['eamcod'], $arrCodInepInseridos)))
    				{
    					$cheked5 = '';
    					$cheked15 = '';
    					if($arrCargaHoraria[$escola['eamcod']] == '15')
    					{
    						$cheked15 = 'selected';
    					}
    					elseif($arrCargaHoraria[$escola['eamcod']] == '5')
    					{
    						$cheked5 = 'selected';
    					}
    					
    					$arrEscolas[$i]['carga_horaria'] = $arrCargaHoraria[$escola['eamcod']];
    				}
    				else
    				{
    					$arrEscolas[$i]['carga_horaria'] = "";
    				}
    			}
    		}
    	}
    	
    	//Monta cabeçalho
    	$cabecalho[] = 'Selecionada';
    	$cabecalho[] = 'Estado';
    	if($campomunicipio != '')
    	{
    		$cabecalho[] = 'Município';
    	}
    	$cabecalho[] = 'Localização';
    	$cabecalho[] = 'Nome da Escola';
    	$cabecalho[] = 'Código INEP';
    	if($grupo == 2)
    	{
    		$cabecalho[] = 'IDEB (Anos Iniciais)';
    		$cabecalho[] = 'IDEB (Anos Finais)';
    		$cabecalho[] = 'INSE';
    	}
    	if($respostaCargaHoraria == 'S')
    	{
    		$cabecalho[] = 'Carga Horária Semanal';
    	}
    	
    	$arrayRetorno['cabecalho'] = $cabecalho;
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
        $adpid = $arrParametros['adpid'];
    	$grupo = $arrParametros['grupo'];
    	
    	$sqlDados =  "
    			SELECT DISTINCT 
				    eam.estuf,
					eam.eammunicipio,
					eam.eamrede,
					aea.codinep::integer as codinep,
					eam.eamnome,
    				CASE WHEN aeacargahoraria IS NOT NULL THEN
    					aeacargahoraria::varchar
    				ELSE
    					'Não'
    				END as carga_horaria,
    				CASE WHEN aeainseridaaposenvio = TRUE THEN
    					'Sim'
    				ELSE
    					''
    				END as enviado_apos_envio
				FROM 
					par3.escolasabcmais2015  eam
				INNER JOIN par3.adesaoescolaabcmais aea ON aea.codinep::VARCHAR = eam.eamcod 
                INNER JOIN par3.prodesaoprograma adp ON adp.adpid = aea.adpid AND adpano_ciclo = eam.aemano_ciclo
				WHERE 
					adp.inuid = $inuid AND
                    adp.adpid = $adpid AND
					aeagrupo = $grupo AND
                    eam.eamcod NOT IN ( SELECT codinep FROM par3.escolasexcecaoprograma exp
                                        INNER JOIN par3.excecaoescolasprograma eep ON eep.eepid = exp.eepid
                                        INNER JOIN par3.prodesaoprograma adp ON adp.adpid = eep.adpid AND adpano_ciclo = 2017
                                        WHERE exp.expstatus = 'A' AND eep.eepstatus = 'A' AND eep.eepdecisao = 'S')

				UNION ALL

    			SELECT DISTINCT
					eam.estuf,
					eam.eammunicipio,
					eam.eamrede,
					exp.codinep::integer as codinep,
					eam.eamnome,
    				CASE WHEN exp.expcargahoraria IS NOT NULL THEN
    					exp.expcargahoraria::varchar
    				ELSE
    					'Não'
    				END as carga_horaria,
                    'Sim' as enviado_apos_envio
				FROM par3.prodesaoprograma adp
            	INNER JOIN workflow.documento                   doc ON doc.docid = adp.docid
            	INNER JOIN par3.excecaoescolasprograma          eep ON eep.adpid = adp.adpid AND eep.eepstatus = 'A' AND eep.eepdecisao = 'S'
            	INNER JOIN par3.escolasexcecaoprograma          exp ON exp.eepid = eep.eepid AND expstatus = 'A'
                INNER JOIN par3.escolasabcmais2015              eam ON exp.codinep::integer = eam.eamcod::integer AND eam.aemano_ciclo = adp.adpano_ciclo
            	LEFT  JOIN par3.respostacargahorariaprograma    rch ON rch.adpid = adp.adpid AND rch.rchgrupo::integer = exp.expgrupo AND rch.prgid = 8
				WHERE
					adp.inuid = $inuid AND
                    adp.adpid = $adpid AND
					exp.expgrupo = $grupo";
//     	ver($sqlDados);
    	$cabecalho 	= array('UF','Município','Dependência Administrativa','Código da Escola','Escola', 'Carga Horária Semanal', 'Inserido após o Envio ao MEC');
    	$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    	$listagem->setCabecalho($cabecalho);
    	$listagem->setQuery($sqlDados);
    	$listagem->turnOffForm();
    	$listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    	return ;
    }
    
    
    public function existeEscolaNoGrupo($arrParam)
    {
    	$inuid         = $arrParam['inuid'];
    	$grupo         = $arrParam['grupo'];
    	$adpano_ciclo  = $arrParam['adpano_ciclo'];
    	
    	$sql = "SELECT 
    				itrid,
    				estuf,
    				muncod 
    			FROM 
    				par3.instrumentounidade  
                WHERE inuid = {$inuid}";
    	
    	$entidade = $this->pegaLinha($sql);
    	
    	$itrid = $entidade['itrid'];
    	
    	if( ($itrid == 1) || ($itrid == 3)){
    		$rede =   'Estadual';
    		$clauseLocal = "estuf = '{$entidade['estuf']}'";
    	}else{
    		$rede =   'Municipal';
    		$clauseLocal = "muncod = '{$entidade['muncod']}'";
    	}
    	
    	$sql = "SELECT 
    				count(eamid) FROM par3.escolasabcmais2015  
    			WHERE
					eamrede = '{$rede}' AND
    				eamgrupo = '{$grupo}' AND
                    aemano_ciclo = $adpano_ciclo AND
    				{$clauseLocal}";
    	
    	$total = $this->pegaUm($sql);			
    	
    	return ( $total > 0 ) ? true : false;
    }


    public function retornaInfoEscola($arrParam)
    {
        $inuid     = $arrParam['inuid'];
        $adpid     = $arrParam['adpid'];
    	$codInep   = $arrParam['cod_inep'];
    	$prgId 	   = $arrParam['prgid'];
    	
    	$sql = "	
			SELECT
				eam.eamnome as nome,
				rch.rchresposta as resposta_carga_horaria,
				eam.eamgrupo as grupo
			FROM 
				par3.escolasabcmais2015  eam
			LEFT JOIN par3.respostacargahorariaprograma  rch ON rch.rchgrupo = eam.eamgrupo AND 
                                                                adpid = $adpid AND inuid = {$inuid} AND rchstatus = 'A' AND 
                                                                rch.prgid = $prgId
			WHERE
				eamcod = '{$codInep}' AND
                aemano_ciclo = {$_SESSION['par3']['adpano_ciclo']} AND
				CASE WHEN eamrede = 'Estadual' THEN
					   {$inuid} = ( SELECT inuid FROM par3.instrumentounidade 
                                    WHERE estuf =  (SELECT estuf FROM par3.escolasabcmais2015 
                                                    WHERE eamcod = '{$codInep}' AND aemano_ciclo = {$_SESSION['par3']['adpano_ciclo']}) AND itrid IN (1,3) LIMIT 1)
				    ELSE
					   {$inuid} = ( SELECT inuid FROM par3.instrumentounidade 
                                    WHERE muncod = (SELECT muncod FROM par3.escolasabcmais2015 
                                                    WHERE eamcod = '{$codInep}' AND aemano_ciclo = {$_SESSION['par3']['adpano_ciclo']}) AND itrid IN (2))
				END    	
    	";
    	$retorno = $this->pegaLinha($sql);
    	$retorno = (is_array($retorno)) ? $retorno : Array(); 
    	
    	return $retorno;
    }
    
    /**
     * Método para retornar as escolas que estão no contexto que está sendo inserido
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
    	$adpid = $arrParametros['adpid'];
    	// Inicializa arr de escolas
    	$arrEscolas = array();
    	
    	//Instancia carga horária
    	$objRespostaCargaHoraria = new Par3_Model_Respostacargahorariaprograma();
    	// Trata dados para consultar carga horária
    	$dados['inuid'] = $inuid;
    	$dados['adpid'] = $adpid;
    	$dados['prgid'] = $prgid;
    	// Consulta resposta de carga horária
    	$arrRespostaCargaHoraria 	= $objRespostaCargaHoraria->getResposta( $dados );
    	$respostaCargaHoraria 		= $arrRespostaCargaHoraria['rchresposta'] ;
    	
    	// Estadual
    	if( ($itrid == 1 ) || ($itrid == 3))
    	{
    		$arrWhere[] = "estuf	= '{$arrParametros['estuf']}' ";
    		$arrWhere[] = "eamgrupo = '{$grupo}' ";
    		$arrWhere[] = "eamrede 	= 'Estadual' ";
    		$campomunicipio = 'eammunicipio,';
    	}
    	else
    	{
    		$arrWhere[] = "muncod	= '{$arrParametros['muncod']}' ";
    		$arrWhere[] = "eamgrupo = '{$grupo}' ";
    		$campomunicipio = '';
    		$arrWhere[] = "eamrede 	= 'Municipal' ";
    	}
    	
    	//Filtar filtros
    	if( $arrParametros['eammunicipio'] != '')
    	{
    		$arrWhere[] = "eammunicipio	ilike '%{$arrParametros['eammunicipio']}%' ";
    	}
    	if( $arrParametros['eamrede'] != '')
    	{
    		$arrWhere[] = "eamrede	= '{$arrParametros['eamrede']}' ";
    	}
    	if( $arrParametros['eamcod'] != '')
    	{
    		$arrWhere[] = "eamcod	= '{$arrParametros['eamcod']}' ";
    	}
    	if( $arrParametros['eamnome'] != '')
    	{
    		$arrWhere[] = "eamnome	ilike '%{$arrParametros['eamnome']}%' ";
    	}
    	
    	$objAdesaoAbcmais = new Par3_Model_Adesaoescolaabcmais();
    	$totalInseridos = $objAdesaoAbcmais->retornaQtdSalvoGrupo($arrParametros);
    	 
    	$arrCodInepInseridos =	$objAdesaoAbcmais->retornaCodEntidadeMarcados($arrParametros);
    	$arrCodInepInseridos = (is_array($arrCodInepInseridos)) ? $arrCodInepInseridos : Array();
    	
    	$arrCargaHoraria	 = $objAdesaoAbcmais->retornaValoresCargaHoraria($arrParametros);
    	$arrCargaHoraria	 = (is_array($arrCargaHoraria)) ? $arrCargaHoraria : Array();
    	 
    	 
    	// Verifica o filtro que traz somente os marcados
    	if( $arrParametros['aeacargahoraria'] != '' )
    	{
    		if( count($arrCargaHoraria) > 0 )
    		{
    	
    			foreach($arrCargaHoraria as $k => $v )
    			{
    				if($v == $arrParametros['aeacargahoraria'])
    				{
    					$arrCargaHorariaClause[] = $k;
    				}
    			}
    	
    			if( count($arrCargaHorariaClause) > 0 )
    			{
    				$arrWhere[] = "eamcod::integer in ( ". implode(', ', $arrCargaHorariaClause) ." )";
    	
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
    			$arrWhere[] = "eamcod::integer {$not} in ( ". implode(', ', $arrCodInepInseridos) ." )";
    			 
    		}
    		 
    	}
    	
		$colunas = "
			eamcod
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
    		$arrRetorno[] = $v['eamcod'];
    	}
    	return $arrRetorno;
    	
    }
   
}

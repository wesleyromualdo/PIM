<?php
/**
 * Classe de controle do  entidade
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@castgroup.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-01-2017
 * @link     no link
 */


include_once APPRAIZ . "par/classes/modelo/InstrumentoUnidade.class.inc";
include_once APPRAIZ . "par3/classes/model/seguranca/Usuario.class.inc";

/**
 * Entidade_Controller_Entidade
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-01-2017
 * @link     no link
 */
class Par_Controller_Entidade
{
    private $model;

    private $dutIdsCarga = array(
    			'02' => DUTID_PREFEITO, /*PREFEITO*/
    			'10' => DUTID_SECRETARIO_ESTADUAL, /*SECRETARIO ESTADUAL*/
    			'28' => DUTID_DIRIGENTE /*DIRIGENTE MUNICIPAL*/
    	);
    
    public function __construct()
    {
        $this->model = new Par_Model_Entidade($_GET['entcodent']);
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Entidade_Controller_Entidade();
        $this->model->popularDadosObjeto();
        $url .= '&entcodent=' . $this->model->entcodent;

        try{
            $sucesso = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }


    public function verificaUltimaVezRotinaPrefeitos()
    {
    	return  $this->model->verificaUltimaVezRotinaPrefeitos();
    }

    
    public function formataDadosDirigentePar3ParaPar2($arrDados) {
    	
    	// Trata o número do telefone
    	$numeroComDDD = trim(str_replace(array('(',')', '-', ' '), '', $arrDados['enttelefonecomercial']));
    	$ddd     = substr($numeroComDDD, 0, 2);
    	$telefone = substr($numeroComDDD, 2, 9);
    	
    	// Trata o número do celular
    	$celularComDDD = trim(str_replace(array('(',')', '-', ' '), '', $arrDados['enttelefonecelular']));
    	$dddCelular     = substr($celularComDDD, 0, 2);
    	$telefoneCelular = substr($celularComDDD, 2, 9);
    	
    	$cpfFormatado    =  str_replace(array('.','-'), '', $arrDados['entcpf']);
    	
    	
        /*** TRATA VARIAVEIS */
        $arrNovaEntidade['entnome']						= $arrDados['entnome'];
        $arrNovaEntidade['entemail']					= $arrDados['entemail'];
        $arrNovaEntidade['entrg']						= $arrDados['entrg'];
        $arrNovaEntidade['entorgexpedidor']				= $arrDados['entorgexpedidor']; 
        $arrNovaEntidade['entdtnascimento']				= $arrDados['entdtnascimento'];
        $arrNovaEntidade['entnumdddcomercial']			= $ddd;
        $arrNovaEntidade['entnumcomercial']				= $telefone;
        $arrNovaEntidade['entnumdddcelular']			= $dddCelular;
        $arrNovaEntidade['entnumcelular']				= $telefoneCelular;
        $arrNovaEntidade['entcpf']                      = $cpfFormatado;
        $arrNovaEntidade['entsexo']						= $arrDados['entsexo'];
    	 
    	
    	return $arrNovaEntidade ;
    }
    
    /**
     *
     * */
    public function atualizaDadosDirigenteMunicipalViaPar3( $arrDados )
    {
    	$dadosEntidadePar      = $this->buscaDadosDirigenteMunicipalAtual($arrDados);
    	$arrDadosEntidadePar   = ( is_array($dadosEntidadePar['dadosEntidade']) ) ? $dadosEntidadePar['dadosEntidade'] : Array();
    	
    	   
    	if(($arrDados['entcpf'] != "") && (is_array($dadosEntidadePar))) {
    	   
    		$arrEntidade        = $this->formataDadosDirigentePar3ParaPar2($arrDados);
    		$acaoAtual 	        = $this->comparaDirigenteVigente($arrEntidade, $dadosEntidadePar);
    	    
    		$arrParams =
    		array
    		(
    		    'inuid' => $dadosEntidadePar['inuid'],
    		    'dutid' => $dadosEntidadePar['dutid'],
    		    'arrNovaEntidade' 	=> $arrEntidade,
    		    'arrEntidadeAntiga' => $arrDadosEntidadePar,
    		);
    		
    	   /**ATUALIZA*/
    	   if($acaoAtual == 'A') {
    	       $att = $this->model->attDadosDirigentesEntidadeRotina($arrParams);
    	        
    	   }/**SUBSTITUI*/ else {
    	       
    	       $att = $this->model->insereNovoDirigentesEntidadeRotina($arrParams);
    	   }
    	   return $att;
    	} else {
    		return false;
    	}
    	
    }
    /**
     *
     * */
    public function atualizaDadosDirigente( $arrEntidade )
    {
    	$modelSAPEDirigente     = new Par3_Model_SAPEDirigente();
    	/** BUSCA OS DADOS DO DIRIGENTE DA ENTIDADE*/
    	$dadosEntidadePar 	= $this->buscaDadosDirigenteRotina($arrEntidade);
    	$arrDadosEntidadePar 	= ( is_array($dadosEntidadePar['dadosEntidade']) ) ? $dadosEntidadePar['dadosEntidade'] : Array();
    	
    	/**VERIFICA SE É O MESMO VIGENTE OU SE SERÁ SUBSTITUIDO, ONDE O RETORNO É S = Substitui e A = atualiza */
    	$acaoAtual 			= $this->comparaDirigenteVigente($arrEntidade, $dadosEntidadePar);
    	
    	/** MONTA PARAMETROS PARA ATUALIZAR A ENTIDADE */
    	$arrParams =
	    	array
	    	(
	    			'inuid' => $dadosEntidadePar['inuid'],
	    			'dutid' => $dadosEntidadePar['dutid'],
	    			'arrNovaEntidade' 	=> $arrEntidade,
	    			'arrEntidadeAntiga' => $arrDadosEntidadePar,
	    	);
    	
    	$attRoll = $this->model->atualizaDadosRollDosResponsaveis($arrParams);
    	/**ATUALIZA*/
    	if($acaoAtual == 'A')
    	{
    		$idLogRotina = 2;
    		$att = $this->model->attDadosDirigentesEntidadeRotina($arrParams);
    		
    	}/**SUBSTITUI*/
    	else
    	{
    		$idLogRotina = 4;
    		$att = $this->model->insereNovoDirigentesEntidadeRotina($arrParams);
    	}
    	/**EXECUTA AS MODIFICAÇÕES E RETORNA O ENTID*/
    	if( $att )
    	{
    		if( $this->model->commit() )
    		{
    			$modelSAPEDirigente->insereLogSucesso($arrEntidade, $idLogRotina);
    			return $att;
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
    
    /** 
     * 
     * */
    public function buscaDadosDirigenteRotina( $arrEntidade )
    {
    	/** CARREGA O ID DA CARGA DO FNDE */
    	$id = $arrEntidade['co_tp_entidade'];
    	
    	/** INSTANCIA INTRUMENTO UNIDADE */
    	$objInu = new InstrumentoUnidade();
    	
    	/**BUSCA O INUID*/
    	if( ($id == '02') || ($id == '28') )
    	{
    		$inuid = $objInu->verificaInuidMunicipio($arrEntidade['muncod']);
    	}
    	else if($id == '10')
    	{
    		$inuid = $objInu->verificaInuidEstado($arrEntidade['estuf']);
    	}
     	/** BUSCA O DUTID (ID DO CARGO)*/
    	$dutid = $this->dutIdsCarga[$id];
    	
    	/** BUSCA OS DADOS DO CARGO DA ENTIDADE */
    	$dadosEntidadePar = $this->model->getDadosEntidade( $inuid, $dutid );
    	
    	$retorno['dadosEntidade']	= $dadosEntidadePar;
    	$retorno['inuid']			= $inuid;
    	$retorno['dutid']			= $dutid;
    	
    	return $retorno;
    	
    }
    
    /**
     *
     * */
    public function buscaDadosDirigenteMunicipalAtual( $arrEntidade )
    {
        
        /** INSTANCIA INTRUMENTO UNIDADE */
        $objInu = new InstrumentoUnidade();
        /**BUSCA dados da unidade pelo Inuid do par3*/
        $inuidPar3 = $arrEntidade['inuid'];
        if($inuidPar3 != '') {
            $dadosEntidadePar = $this->model->getdadosUnidadePar3( $inuidPar3 );
            
            $muncod = $dadosEntidadePar['muncod'];
            if($muncod) {
            	
            	$inuid = $objInu->verificaInuidMunicipio($muncod);
            	
            	$dutid = 2;
            	/** BUSCA OS DADOS DO CARGO DA ENTIDADE */
            	$dadosEntidadePar = $this->model->getDadosEntidade( $inuid, $dutid );
            	 
            	$retorno['dadosEntidade']	= $dadosEntidadePar;
            	$retorno['inuid']			= $inuid;
            	$retorno['dutid']			= $dutid;
            	 
            	return $retorno;
            }   
        } else {
        	return Array();
        }
    }
    
    
    /**
     *
     * @return S = substitui; A = Atualiza
     * */
    public function comparaDirigenteVigente( $arrEntidade, $arrEntidadeAtual )
    {
    	$cpfCarga = $arrEntidade['entcpf'];
    	$cpfAtual = $arrEntidadeAtual['dadosEntidade']['entnumcpfcnpj'];
    	
    	if( (count($arrEntidadeAtual) > 0) && ($cpfCarga) && ($cpfAtual) )
    	{
    		if($cpfCarga == $cpfAtual)
    		{
    			return 'A';
    		}
    		else
    		{
    			return 'S';
    		}
    	}
    	else 
    	{
    		return 'S';
    	}
    }
    


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['entcodent'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&entcodent={$id}";
        $entidade = new Entidade_Model_Entidade($id);
        try{
             $entidade->Xstatus = 'I';
             $entidade->Xdtinativacao = date('Y-m-d H:i:s');
             $entidade->Xusucpfinativacao = $_SESSION['usucpf'];

             $entidade->salvar();
             $entidade->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }



}
?>
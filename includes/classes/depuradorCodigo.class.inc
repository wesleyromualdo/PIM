<?php 
require_once(APPRAIZ.'includes/FirePHPCore/FirePHP.class.php');

/*
 * @desc Classe de Debug de Códigos e Queries
 * @author Thiago Tasca Barbosa
 * @since 03/05/2010
 */
class DepuradorCodigo extends FirePHP  {
	
	/**
     * @desc Array de query.
     * @var array
     * @access protected
     */
	protected 	$listaQuery;
	
	/**
     * @desc Array de query Erradas.
     * @var array
     * @access protected
     */
	protected 	$listaQueryInaceitaveis;
	
	/**
     * @desc Tempo da query mais pesada da página
     * @var float
     * @access protected
     */
	protected 	$TempoQueryMaisPesada = 0;
	
	/**
     * @desc Query mais pesada da página
     * @var string
     * @access protected
     */
    protected	$queryMaisPesada = "";
    
    /**
     * @desc Tempo total de execução das Queries.
     * @var float
     * @access protected
     */
    protected	$TempoTotalQuery = 0;
    
    /**
     * @desc Objeto com a instancia do Banco de Dados.
     * @var array
     * @access protected
     */
    protected 	$InstanciaDaClasseDB;
    
    /**
     * Objeto do Fire PHP
     * @var object
     * @access protected
     */
    protected 	$firephp;
	
    /**
	 * Função __construct
	 * @desc Método usado quando a classe é instanciada
	 * @access public
	 * @author Thiago Tasca Barbosa
	 * @since 03/05/2010
	 */
	public function __construct($debugQueryDoBanco, $InstanciaDaClasseDB = NULL){
		$this->__setFirePHP();
		
		if($debugQueryDoBanco === true ){
			if($this->__setDB($InstanciaDaClasseDB) === true){
				$this->debugConsultas();
				$this->InstanciaDaClasseDB->arrayQuery = array();
			}else{
				$this->firephp->warn("Erro ao tentar recuperar a classe de banco de dados da página.");
			}
		}
	}
	
	/**
	 * Função __setFirePHP
	 * @desc Seta uma instancia nova do Fire PHP;
	 * @access private
	 * @author Thiago Tasca Barbosa
	 * @since 03/05/2010
	 */
	private function __setFirePHP(){
		if($this->firephp = FirePHP::getInstance(true)){
			return true;
		}else{
			return false;
		}
	}
	
	/**
	 * Função __setDB
	 * @desc Seta o valor do atributo da classe.(seta o objeto do banco)
	 * @access private
	 * @author Thiago Tasca Barbosa
	 * @since 03/05/2010
	 */
	private function __setDB($InstanciaDaClasseDB){
		if($this->InstanciaDaClasseDB = $InstanciaDaClasseDB){
			return true;
		}else{
			return false;
		}
	}
	
	/**
	 * Função debugConsultas
	 * @desc Monta no FirePHP a lista de resposta com as queries do Banco de dados.
	 * @access private
	 * @author Thiago Tasca Barbosa
	 * @since 03/05/2010
	 */
	private function debugConsultas(){
		$this->listaQuery[] 				= array("SQL", "Tempo de Execução em Segundos");
		$this->listaQueryInaceitaveis[] 	= array("SQL", "Tempo de Execução em Segundos");
		$existemPendencia					= false;
		
		foreach($this->InstanciaDaClasseDB->arrayQuery as $query){
			$this->listaQuery[] = array($query["query"], $query["tempo"]);
	    	$this->TempoTotalQuery += $query["tempo"];
	    	
			if($query["tempo"] > $this->TempoQueryMaisPesada){
	            $this->TempoQueryMaisPesada = $query["tempo"];
	            $this->queryMaisPesada 		= $query["query"];  
	        }

	        if($query["query"]){
	        	$queryLista = str_replace(" ", "", $query["query"]);
	        	$teste1 = strpos($queryLista, 'select*');
	        	$teste2 = strpos($queryLista, ',*'); 
	        	$teste3 = strpos($queryLista, '.*'); 
	        	$teste4 = strpos($queryLista, 'count(*)'); 
	        	if($teste1 !== false || $teste2 !== false  || $teste3 !== false || $teste4 !== false  ){
	        		$this->listaQueryInaceitaveis[] = array($query["query"], $query["tempo"]);
	        		$existemPendencia = true;
	        	}
	        }
	        
	    }
	    
	    $total_queries = count($this->InstanciaDaClasseDB->arrayQuery);
	    
		$dbug = $this->firephp;
	    $dbug->group('Query', array( 'Collapsed' => false,  
	                                    'Color' => '#000'));
	    $dbug->table("Total de Queries: ".$total_queries . ". Para rodar todas as consultas levou: " . 
	                    $this->TempoTotalQuery . " segundos.", $this->listaQuery);
	 
	    $dbug->group('Resumo da Consulta mais pesada da página', array( 	'Collapsed' => false,  
	                                           				'Color' => '#000'));
	                                         
	    $dbug->log('', 'Query mais pesada da página');
	    $dbug->log($this->queryMaisPesada,'');
	    $dbug->log($this->TempoQueryMaisPesada, 'Tempo total da consulta mais pesada (Query)');

	    if($existemPendencia){
	    	$dbug->group('Query fora do padrão SIMEC', array( 	'Collapsed' => false,  
	        						                            'Color' => 'red'));
	    	$dbug->table("Query fora do padrão SIMEC ", $this->listaQueryInaceitaveis);
	    }
	    $dbug->groupEnd();
	    $dbug->groupEnd();
	 
	    return true;
	}
	
	/**
	 * Função dbug
	 * @desc Debug em ponto específico do código e insere dentro do FirePHP. (Obs: Tipo um print_r na tela.)
	 * @access public
	 * @author Thiago Tasca Barbosa
	 * @since 03/05/2010
	 */
	public function dbug($objeto,$descricao = null){
		
		if($descricao != null){
			$descricao = $descricao;
		}else{
			$descricao = "Debugando código";
		}
		
		$dbug = $this->firephp;
		$dbug->group('Debug', array( 'Collapsed' => false,  
	                                 'Color' => '#0E3355'));
		$dbug->log($objeto, $descricao);
		$dbug->groupEnd();
	}
	
}
/*
 * Ver exemplo desta classe
class fbTimer{
  protected static $timerStart = 0;
  protected static $timerEnd = 0;
  public static function start(){
    self::$timerStart = microtime();
    self::$timerEnd = 0;
  }

  public static function stop(){
    self::$timerEnd =microtime();
  }

  public static function get(){
    if(self::$timerEnd == 0) self::stop();
      return self::$timerEnd - self::$timerStart;
    }
  }
// this class is simple to use
fbTimer::start();
// some code you want to benchmark
fbTimer::stop();
fb::log("execution time :" . fbTimer::get() . ' seconds' );

  */ 
?>
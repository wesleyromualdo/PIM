<?php
namespace mvc;

class Visao {

	private $config;
	private $atributoReservado;
	private $db;
	
	final public function __construct( $nomeControle ){

		$this->atributoReservado = array("config", "visao", "db", "msgPositiva", "msgNegativa", "msgInformativa");
		
		$this->db     = new \cls_banco();
		$this->config = array(
		    "dadosJs"               => array(),
// 		    "dadosVisao"            => array(),
			"namespaceHelper" 	 	=> 'visao\Helper\\',
			"tituloCabecalho" 	 	=> 0,
			"subTituloCabecalho" 	=> 0,
			"qtdIncluirVisao" 		=> 0,
			"erro" 					=> array(),
			"caminho"				=> APPRAIZ,
			"caminhoVisao"			=> APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/visao/" . $nomeControle . "/",
			"caminhoWWW"			=> APPRAIZ . "www/" . $_SESSION['sisdiretorio'],
		    "caminhoHelper"			=> APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/visao/Helper/",
			"cabecalho"			    => array(
			                             "caminho" => APPRAIZ . 'includes/cabecalho.inc',
    			                         "limpo"   => false
			                         ),
			"rodape"			    => array(
			                             "caminho" => APPRAIZ . 'includes/rodape.inc',
    			                         "limpo"   => false
			                         ),
		    "include"              => array(
    		                              'inicio' => array(
        		                              'js'  => array(),
        		                              'css' => array()
    		                              ),
    		                              'fim' => array(
        		                              'js'  => array(),
        		                              'css' => array()
    		                              )
		                              )
		);
		
		require_once $this->config['caminho'] . "includes/classes/mvc/HelperVisao.class.inc";
	}
	
	final public function helper($nomeHelper, $outroCaminho=null, $outroNamespace=null){
	    $nomeHelper        = ucfirst($nomeHelper);
	    $caminhoHelper     = ($outroCaminho ? APPRAIZ . $outroCaminho : $this->config['caminhoHelper']);
	    $namespaceHelper   = ($namespaceHelper ? $namespaceHelper : $this->config['namespaceHelper']);
	    
	    $includeHelper = $caminhoHelper . $nomeHelper . '.class.inc';
        $includeHelper = str_replace("\\", "/", $includeHelper);

	    if (file_exists($includeHelper)) {
	        require_once $includeHelper;

            $nomeHelper = $namespaceHelper . $nomeHelper;
            $helper = new $nomeHelper($this);
	        
	        return $helper;
	    } else {
	        $this->setarErro('Helper não encontrado: ' . $includeHelper);
	    }
	}
	
	final public function setarIncludeJS($arquivo, $insercaoInicio = false)
	{
	   
	    if (
	        !file_exists($this->config['caminho'] . 'www' . $arquivo) &&
            !file_exists($this->config['caminho']. $arquivo)
        ) {
            $this->setarErro("O arquivo JS não foi encontrado: {$arquivo}");
	    }
	    
	    if ($insercaoInicio == false) {
	        $this->config['include']['fim']['js'][] = "<script src=\"{$arquivo}\"></script>";
	    } else {
	        $this->config['include']['inicio']['js'][] = "<script src=\"{$arquivo}\"></script>";
	    }
	}
	
	final public function setarIncludeCSS($arquivo, $insercaoInicio = true)
	{
// 	    $arquivo = $this->config['caminho'] . $arquivo;
	    if (
            !file_exists($this->config['caminho'] . 'www' . $arquivo) &&
            !file_exists($this->config['caminho']. $arquivo)
        ) {
            $this->setarErro("O arquivo CSS não foi encontrado: {$arquivo}");
	    }
	    
	    if ($insercaoInicio == false) {
	        $this->config['include']['fim']['css'][] = "<link href=\"{$arquivo}\" rel=\"stylesheet\">\n";
	    } else {
	        $this->config['include']['inicio']['css'][] = "<link href=\"{$arquivo}\" rel=\"stylesheet\">\n";
	    }
	}
	
	final public function montarIncludeJS($insercaoInicio)
	{
	    $arInclude = (array) $this->config['include'][($insercaoInicio == true ? 'inicio' : 'fim')]['js'];
	    
	    return (implode("\n", $arInclude));
	}
	
	final public function montarIncludeCSS($insercaoInicio)
	{
	    $arInclude = (array) $this->config['include'][($insercaoInicio == true ? 'inicio' : 'fim')]['css'];
	    
	    return (implode("\n", $arInclude));
	}
	
// 	final public function dadoVisao($nome, $valor)
// 	{
// 	    $this->config['dadosVisao'][$nome] = $valor;
// 	}

	final public function dadoJs($nome, $valor)
	{
	    $this->config['dadosJs'][$nome] = $valor;
	}
	
	final public function carregarDados( array $dados = array() ){
		foreach ($dados as $k => $v){
			if ( !in_array($k, $this->atributoReservado) ){
				$this->$k = $v;
			}else{
				$this->config['erro'][] = "Você usou um nome de atributo ({$k}) reservado à classe de visão, desenvolvedor, modifique-o por favor.";			
			}
		}
		return $this;
	}
	
	final public function incluirVisao($arquivo, $incluirCabecalho = true, $outroCaminho = '')
	{
        global $simec;
	    $db = $this->db;
	    
		$this->config['qtdIncluirVisao']++;
		
		if ( $this->config['qtdIncluirVisao'] > 1 ){
			$this->config['erro'][] = "Desenvolvedor, use o método - incluirVisao - somente uma vez, para adicionar outras visões, utilize o método - incluirVisaoAcessoria ou capturarHtmlVisaoAcessoria.";
			return;
		}
		
		$arquivo = trim($arquivo);
		
		if ( $outroCaminho != '' ){
			$fileVisao = $this->config['caminho'] . $outroCaminho . $arquivo .".inc";
		}else{
			$fileVisao = $this->config['caminhoVisao'] . $arquivo .".inc";
		}
		
		if ( file_exists( $fileVisao ) ){
			if ( $incluirCabecalho == true ){
				$this->incluirCabecalho();
			}
			$this->apresentarMensagemSistema();

			$this->incluirVisaoAcessoria('partialDadosJS', 'includes/classes/mvc/visao/');

			include $fileVisao;

			if ( $incluirCabecalho == true ){
				$this->incluirRodape();
			}
		}else{
			$this->config['erro'][] = "O arquivo de visão ({$fileVisao}) não existe.";;
		}
	}
	
	final public function incluirVisaoAcessoria($arquivo, $outroCaminho = '')
	{
        global $simec;
	    $db = $this->db;
	    
		$arquivo = trim($arquivo);
		
		if ( $outroCaminho != '' ) {
			$fileVisao = $this->config['caminho'] . $outroCaminho . $arquivo .".inc";
		} else {
			$fileVisao = $this->config['caminhoVisao'] . $arquivo .".inc";
		}
		
		if ( file_exists( $fileVisao ) ) {
			include $fileVisao;
		} else {
			$this->config['erro'][] = "O arquivo de visão ({$fileVisao}) não existe.";;
		}
	}
	
	final public function capturarHtmlVisaoAcessoria($arquivo, $outroCaminho = '')
	{
        $db = $this->db;
	    
		$arquivo = trim($arquivo);
		
		if ( $outroCaminho != '' ) {
			$fileVisao = $this->config['caminho'] . $outroCaminho . $arquivo .".inc";
		} else {
			$fileVisao = $this->config['caminhoVisao'] . $arquivo .".inc";
		}
		
		if ( file_exists( $fileVisao ) ) {
			ob_clean();
			include $fileVisao;
			
			$html = ob_get_contents();
			ob_clean();
			
			return $html;
		} else {
			$this->config['erro'][] = "O arquivo de visão ({$fileVisao}) não existe.";;
		}
	}
		
	final public function existeErro()
	{
		return ( count($this->config['erro']) > 0 );
	}
	
	final public function pegarErro()
	{
		$erro = $this->config['erro'];
		$this->config['erro'] = array();
		 
		return $erro;	
	}
	
	final public function setarErro($erro)
	{
		$this->config['erro'][] = $erro;
	}
	
	final private function incluirCabecalho()
	{
		// Necessário torar a variável global para correto funcionamento do CABEÇALHO
	    global $cabecalho_sistema, $nome_bd;
		global $abacod_tela, $titulo_modulo, $modulo_atual;
		
		$db = $this->db;
		// inclusão do cabeçalho padrão do SIG
		include $this->config['cabecalho']['caminho'];
		
		$variaveisCabecalho = get_defined_vars();
		unset($variaveisCabecalho['db']);
		$this->carregarDados( $variaveisCabecalho );
	}
	
	public function setCabecalho($caminho, $limpo){
	    $this->config['cabecalho']['caminho'] = APPRAIZ . $caminho;
	    $this->config['cabecalho']['limpo']   = ($limpo ? true : false);
	}
	
	public function setRodape($caminho, $limpo){
	    $this->config['rodape']['caminho'] = APPRAIZ . $caminho;
	    $this->config['rodape']['limpo']   = ($limpo ? true : false);
	}
	
	final private function incluirRodape(){
	    // Necessário torar a variável global para correto funcionamento do CABEÇALHO
	    global $cabecalho_sistema;
	    global $abacod_tela, $titulo_modulo, $modulo_atual;
	    
	    $db = $this->db;
	    
	    include $this->config['rodape']['caminho'];
	}
	
	final private function apresentarMensagemSistema(){

		$msg = $_SESSION['mvc']['mensagem'];
		
		$this->msgPositiva 		= implode("\n", $msg['positiva']);
		$this->msgNegativa 		= implode("\n", $msg['negativa']);
		$this->msgInformativa 	= implode("\n", $msg['informativa']);
		
		$_SESSION['mvc']['mensagem']['positiva']	= array();
		$_SESSION['mvc']['mensagem']['negativa']	= array();
		$_SESSION['mvc']['mensagem']['informativa'] = array();
		
		$this->incluirVisaoAcessoria('mensagemDeErro', 'includes/classes/mvc/visao/');
	}
	
	final public function setarTituloCabecalho( $txt ){
		$this->config['tituloCabecalho'] = $txt;
	}
	
	final public function setarSubTituloCabecalho( $txt ){
		$this->config['subTituloCabecalho'] = $txt;
	}
}
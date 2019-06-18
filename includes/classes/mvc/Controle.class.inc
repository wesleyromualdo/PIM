<?php
namespace mvc;

// require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . 'includes/classes/mvc/Visao.class.inc';

abstract class Controle
{
	const msgPositiva 	 = 'positiva';
	const msgNegativa 	 = 'negativa';
	const msgInformativa = 'informativa';
	
// 	protected $modelo; 
	protected $db; 
	protected $visao; 
	protected $erro; 
	
	final public function __construct($acao)
	{
// 		$this->modelo = new \Modelo();
		$this->db	  = new \cls_banco();
		$this->visao  = new Visao( get_class( $this ) );
		$this->erro   = array();
		
		if ( empty( $_SESSION['mvc']['mensagem'] ) ){
			$_SESSION['mvc']['mensagem'][self::msgPositiva]	= array();
			$_SESSION['mvc']['mensagem'][self::msgNegativa]	= array();
			$_SESSION['mvc']['mensagem'][self::msgInformativa] = array();
		}
				
		$this->iniciarFluxo();
		
		if ( $acao && method_exists($this, "acao{$acao}") ){
			$acao = "acao{$acao}";
			$this->$acao();
		}else{
			$this->visao->setarErro("Ação do controle inexistente: ". get_class($this) . '::' . "acao{$acao}();");
		}
		
		if ( $this->visao->existeErro() ){
			ob_clean();
			
			dbg( $this->visao->pegarErro(), d);
		}
	}
	
	abstract protected function iniciarFluxo();
	
	final protected function sucesso($url='')
	{
	    $url = ($url ? $url : $_SERVER['REQUEST_URI']);
	    echo "<script>alert('Operação realizada com sucesso!'); location.href='{$url}'</script>";
	}
	
	final protected function redirecionarTela( $pagina = null )
	{
		ob_clean();
		
		$pagina = ($pagina ? $pagina : $_SERVER['REQUEST_URI']);
		header("Location:" . $pagina);
		die;
	}
	
	final protected function redirecionarTelaPai( $pagina = null, $fecharPopUp = true )
	{
		ob_clean();
		
		$pagina = ($pagina ? "'".$pagina."'" : 'opener.location.href');
		die('<script type="text/javascript">opener.location.href=' . $pagina . ';'.($fecharPopUp ? ' window.close();' : '').'</script>');
	}
	
	final protected function registrarMensagemSistema( $msg, $tipoMsg )
	{
		// positiva, informativa, negativa
		$_SESSION['mvc']['mensagem'][$tipoMsg][] = $msg;
		
		return $this;
	}
	
// 	/**
// 	 * Armazena o valor para a apresentação na visualização
// 	 * @param string $var nome da variável a ser utilizada da visualização
// 	 * @param mixed $values valor da variável
// 	 * @return $this
// 	 */
// 	protected function toView($var, $values)
// 	{
// 	    $this->visao->dadoVisao($var, $values);
	    
// 	    return $this;
// 	}
	
// 	/**
// 	 * Armazena o valor para registrar no script da vizualização
// 	 * @param string $var nome da variável a ser utilizada no namespace "_JS" do script
// 	 * (Exemplo: a variável "bla" será acessível no script como "_JS.bla")
// 	 * @param mixed $values valor da variável
// 	 * @return $this
// 	 */
// 	protected function toJs($var, $values)
// 	{
// 	    $this->visao->dadoJs($var, $values);
	    
// 	    return $this;
// 	}
	
// 	/**
// 	 * Retorna dados da postagem recebida
// 	 * @param string $var nome da variável
// 	 * @return mixed
// 	 */
// 	protected function post($var = null)
// 	{
// 	    if ($var && isset($_POST[$var])) {
// 	        return $_POST[$var];
// 	    }
// 	    return $_POST;
// 	}
	
// 	/**
// 	 * Retorna dados da url requisitada
// 	 * @param string $var nome da variável
// 	 * @return mixed
// 	 */
// 	protected function get($var = null)
// 	{
// 	    if ($var && isset($_GET[$var])) {
// 	        return $_GET[$var];
// 	    }
// 	    return $_GET;
// 	}
	
// 	/**
// 	 * Retorna dados dos arquivos recebidos
// 	 * @param string $var nome da variável
// 	 * @return array
// 	 */
// 	protected function files($var = null)
// 	{
// 	    if ($var && isset($_FILES[$var])) {
// 	        return $_FILES[$var];
// 	    }
// 	    return $_FILES;
// 	}
	
// 	/**
// 	 * Retorna da sessão ou armazena dados na sessão
// 	 * @param string $var nome da variável
// 	 * @param mixed $val valor para armazenamento
// 	 * @return mixed valor armazenado
// 	 */
// 	protected function session($var, $val = null)
// 	{
// 	    if ($var) {
// 	        if ($val) {
// 	            return $_SESSION[$var] = $val;
// 	        }
// 	        if (isset($_SESSION[$var])) {
// 	            return $_SESSION[$var];
// 	        }
// 	    }
// 	}
	
// 	final protected function setarErro( $erro ){
// 		$this->erro[] = $erro;
// 	}
	
// 	final protected function pegarErro(){
// 		$erro = $this->erro;
// 		$this->erro = array();
		
// 		return $erro;
// 	}
}
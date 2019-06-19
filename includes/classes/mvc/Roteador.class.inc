<?php
namespace mvc;

class Roteador {
	
	private $config;
	
	final public function __construct(){
		$this->config = array(
			"erro" 				=> array(),
			"caminhoControle"	=> APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/controle/",
		);
		
	}
    public function rotaPossivel(){
		require_once APPRAIZ . 'includes/classes/mvc/Controle.class.inc';

		list($controle, $acao) 	= explode("/", ucfirst( $_GET['modulo'] ));
		$acao 					= ($acao ? ucfirst($acao) : 'Index');
		
// 		$controle = ucfirst( $_GET['modulo'] );
		$arquivo = $this->config['caminhoControle'] . $controle . ".class.inc";
        return file_exists( $arquivo );
    }
	
	public function incluirRota(){
		require_once APPRAIZ . 'includes/classes/mvc/Controle.class.inc';

		list($controle, $acao) 	= explode("/", ucfirst( $_GET['modulo'] ));
		$acao 					= ($acao ? ucfirst($acao) : 'Index');
		
// 		$controle = ucfirst( $_GET['modulo'] );
		$arquivo = $this->config['caminhoControle'] . $controle . ".class.inc";
		if ( file_exists( $arquivo ) ){
			require_once $arquivo;
			$class = new $controle( $acao );
		}else{
			die("Controle não encontrada: {$arquivo}");	
		}	
	}
}
	

<?php
class Pim_Controller_FuncaoEntidade extends Controle
{
	private $model;

	public function __construct()
	{
	    $this->model = new Pim_Model_FuncaoEntidade();
	}
	
	public function pegaFuncaoEntidade( $enfid ){
	    $sql = "select enfid, enfdsc from corporativo.entidades_funcao where enfid = $enfid";
	    $arDados = $this->model->pegaLinha($sql);
	    
	    return $arDados;
	}
}
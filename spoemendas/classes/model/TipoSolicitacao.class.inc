<?php

class Spoemendas_Model_TipoSolicitacao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "spoemendas.tiposolicitacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tpsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
	  	'tpsdsc' => null,
	  	'tpsstatus' => null
	);
    
    public function listChoice()
    {
    	$sql = "SELECT tpsid, tpsdsc FROM spoemendas.tiposolicitacao WHERE tpsstatus = 'A'";
    	
    	$list = [];
    	
    	foreach ($this->carregar($sql) as $values) {
    		$list[$values['tpsdsc']] = $values['tpsid'];
    	}
    	
    	return $list;
    }
    
    public function addOficioTipoSolicitacao($ofiid, $tpsid)
    {	
    	$sql = "INSERT INTO spoemendas.oficiotiposolicitacao (ofiid, tpsid) VALUES ({$ofiid}, {$tpsid})";
    	
    	return $this->executar($sql);
    }
    
    public function tipoSelected($ofiid)
    {
    	$list = [];
    	
    	if($ofiid) {
    		$sql = "SELECT tpsid FROM spoemendas.oficiotiposolicitacao WHERE ofiid = {$ofiid}";
    		 
    		$dados = $this->carregar($sql);
    		 
    		if(is_array($dados)) {
    			foreach ($this->carregar($sql) as $values) {
    				$list[] = $values['tpsid'];
    			}
    		}	
    	}
    	 
    	return $list;
    }
    
    public function pegaDocid($ofiid)
    {
    	$sql = "SELECT docid FROM emenda.oficio WHERE ofiid = {$ofiid}";
    	
    	return $this->pegaUm($sql);
    }
}
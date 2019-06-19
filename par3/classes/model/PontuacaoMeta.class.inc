<?php

class Par3_Model_PontuacaoMeta extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.pontuacaometa";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pmtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'pmtid' => null,
									  	'ptoid' => null,
									  	'pmtmeta' => null,
									  );

	protected $stOrdem = null;

	public function recuperarDados($ptoid){

	    $sql = "SELECT
	               pmtid,
	               ptoid,
	               pmtmeta
	           FROM
	               {$this->stNomeTabela}
	           WHERE
	               ptoid = {$ptoid}";

	    return $this->carregar($sql);
	}

	public function preparaSalvar($ptoid, $mt){

	    $pmtid = $_POST['pmtid'] ? $_POST['pmtid'] : null;

	    $this->pmtid 	 = $pmtid;
	    $this->ptoid 	 = $ptoid;
	    $this->pmtmeta   = $mt;
	    $this->salvar();
	    $this->commit();
	}

	public function preparaExcluir($pmtid){

	    $this->pmtid = $pmtid;
	    $this->excluir();
	    $this->commit();
	}

}
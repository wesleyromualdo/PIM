<?php
	
class ItensComposicaoSubacao extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.itenscomposicaosubacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "icoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'icoid' => null, 
									  	'icoano' => null, 
									  	'icoordem' => null,
    									'icodescricao' => null,
    									'icoquantidade' => null,
									    'icovalor' => null,
									    'icovalortotal' => null,
									    'icostatus' => null,
									    'sbaid' => null,
									    'unddid' => null,
    									'icodetalhe' => null
									  );
									  
	public function carregaItensComposicaoPorSbaid($sbaid, $anoref)
	{
		$sql = "SELECT 
					unddid || '_' || icoid || '' as codigo,
					icoid, 
				  	icoano, 
				  	icoordem,
		    		icodescricao as descricao,
		    		icodescricao,
		    		icoquantidade,
				    icovalor,
				    icovalortotal,
				    icostatus,
				    sbaid,
				    unddid,
				    und.unddsc,
				    ics.icodetalhe
				FROM {$this->stNomeTabela} ics
				INNER JOIN par.unidademedida und ON und.undid = ics.unddid
				WHERE ics.sbaid = {$sbaid}
				AND icoano = '{$anoref}'
				ORDER BY icodescricao";
		
		return $this->carregar($sql);
	}
	
	public function recuperaDescricaoItem($iditem)
	{
		return $this->pegaUm("select icodescricao from {$this->stNomeTabela} where icoid = {$iditem}");
	}	
									  
}									  
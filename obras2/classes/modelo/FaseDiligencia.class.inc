<?php
	
class FaseDiligencia extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.fasediligencia";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "fsdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'fsdid' => null,
                                        'fsddsc' => null,
                                        'fsdstatus' => null,
                                      );

	public function listaCombo(){
		$sql = "SELECT
					fsdid AS codigo,
					fsddsc AS descricao
				FROM
					obras2.fasediligencia
				WHERE
					fsdstatus = 'A'
				ORDER BY
					fsddsc";

		$dados = $this->carregar($sql, null, 7200);
		
		return (is_array($dados) ? $dados : array());
	}
									  
}
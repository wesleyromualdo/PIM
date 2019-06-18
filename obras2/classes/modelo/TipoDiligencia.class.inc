<?php
	
class TipoDiligencia extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipodiligencia";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tpdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tpdid' => null,
									  	'tpddsc' => null,
									  	'tpddtinclusao' => null,
									  	'liberaanexo' => null,
									  	'tpdstatus' => null,
									  );

	public function listaCombo(){
		$sql = "SELECT
					tpdid AS codigo,
					tpddsc AS descricao
				FROM
					obras2.tipodiligencia
				WHERE
					tpdstatus = 'A'
				ORDER BY
					tpddsc";

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}

    public function listaComboFlag(){
        $sql = "SELECT
					tpdid AS codigo,
					tpddsc AS descricao,
					liberaanexo
				FROM
					obras2.tipodiligencia
				WHERE
					tpdstatus = 'A'
				ORDER BY
					tpddsc";

        $dados = $this->carregar($sql, null, 7200);

        return (is_array($dados) ? $dados : array());
    }
									  
}
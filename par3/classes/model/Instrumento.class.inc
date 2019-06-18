<?php

class Par3_Model_Instrumento extends Modelo
{
	/*
	 * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.instrumento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'itrid' => null,
									  	'itrdsc' => null,
									  );

	public function recuperarIntrumentosGuia($itrid = null)
	{
		$where = $itrid ? "AND itrid = {$itrid} " : " ";
		$sql = "SELECT DISTINCT
					it.itrid,
					it.itrdsc
			    FROM par3.instrumento it
			    WHERE it.itrid IN ( 1, 2, 3 )
					{$where}
			    ORDER BY it.itrid
			    ";

		return $this->carregar($sql);
	}

    public function pegarSQLSelectCombo($arrPost = array())
    {
        $sql = "SELECT '' as codigo, '' as descricao";
        if($arrPost['itrid'] != ''){
            $sql = "SELECT
                    	itrid as codigo,
                    	itrdsc as descricao
                    FROM
                    	par3.instrumento
                    WHERE
                    	itrid = {$arrPost['itrid']}
            		ORDER BY descricao";
        }
        return $sql;
    }

}
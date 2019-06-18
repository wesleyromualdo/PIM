<?php

class Par3_Model_IndicadorINEP extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.indicadoresinep";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "iniid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
	  	'iniid' => null,
		'inuid' => null,
		'muncod' => null,
		'estuf' => null,
		'iniano' => null,
		'initipolocal' => null,
		'initipodependencia' => null,
		'inieducacaoinfantilgrupo1valor' => null,
		'inieducacaoinfantilgrupo2valor' => null,
		'inieducacaoinfantilgrupo3valor' => null,
		'inieducacaoinfantilgrupo4valor' => null,
		'inieducacaoinfantilgrupo5valor' => null,
		'inifundamentalinicialgrupo1valor' => null,
		'inifundamentalinicialgrupo2valor' => null,
		'inifundamentalinicialgrupo3valor' => null,
		'inifundamentalinicialgrupo4valor' => null,
		'inifundamentalinicialgrupo5valor' => null,
		'inifundamentalfinalgrupo1valor' => null,
		'inifundamentalfinalgrupo2valor' => null,
		'inifundamentalfinalgrupo3valor' => null,
		'inifundamentalfinalgrupo4valor' => null,
		'inifundamentalfinalgrupo5valor' => null,
		'inimediogrupo1valor' => null,
		'inimediogrupo2valor' => null,
		'inimediogrupo3valor' => null,
		'inimediogrupo4valor' => null,
		'inimediogrupo5valor' => null,
    	'inieducacaoinfantilgrupo1porcent' => null,
    	'inieducacaoinfantilgrupo2porcent' => null,
    	'inieducacaoinfantilgrupo3porcent' => null,
    	'inieducacaoinfantilgrupo4porcent' => null,
    	'inieducacaoinfantilgrupo5porcent' => null,
    	'inifundamentalinicialgrupo1porcent' => null,
    	'inifundamentalinicialgrupo2porcent' => null,
    	'inifundamentalinicialgrupo3porcent' => null,
    	'inifundamentalinicialgrupo4porcent' => null,
    	'inifundamentalinicialgrupo5porcent' => null,
    	'inifundamentalfinalgrupo1porcent' => null,
    	'inifundamentalfinalgrupo2porcent' => null,
    	'inifundamentalfinalgrupo3porcent' => null,
    	'inifundamentalfinalgrupo4porcent' => null,
    	'inifundamentalfinalgrupo5porcent' => null,
    	'inimediogrupo1porcent' => null,
   		'inimediogrupo2porcent' => null,
   		'inimediogrupo3porcent' => null,
   		'inimediogrupo4porcent' => null,
   		'inimediogrupo5porcent' => null,
  	);

	protected $stOrdem = null;

	/**
	 * Função prepararFiltro
	 * - monta o filtro SQL para lista de indicadores.
	 *
	 * @return retorna um array de filtros.
	 *
	 */
	public function prepararFiltro($arrPost)
	{
		$where = array();

		if ($arrPost['muncod'] != '') {
			$where[] = "muncod = '{$arrPost['muncod']}'";
		}

		if ($arrPost['estuf'] != '') {
			$where[] = "estuf = '{$arrPost['estuf']}'";
		}

		if ($arrPost['iniano'] != '') {
			$where[] = "iniano = '{$arrPost['iniano']}'";
		}

		if ($arrPost['itrid'] != '') {
			$where[] = "initipodependencia = '".($arrPost['itrid'] == '2' ? 'Municipal' : 'Estadual')."'";
		}

		return $where;

	}//end prepararFiltro()


	/**
	 * Função pegarSQLLista
	 * - monta SQL para lista de indicadores.
	 *
	 * @return retorna a query.
	 *
	 */
	public function pegarSQLLista($arrPost)
	{
		$where = self::prepararFiltro($arrPost);

		$sql = "SELECT *
    	        FROM {$this->stNomeTabela}
                WHERE ".implode(' AND ', $where)."";

		return $sql;

	}//end pegarSQLLista()
}
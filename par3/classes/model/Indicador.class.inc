<?php

class Par3_Model_Indicador extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.indicador";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "indid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'indid'     	=> null,
									  	'areid'     	=> null,
									  	'inddsc'    	=> null,
									  	'indlei'    	=> null,
									  	'indajuda'  	=> null,
									  	'indstatus' 	=> null,
    									'indcod'    	=> null,
										'indrequerido' 	=> null
									  );

	protected $stOrdem = null;

	public function recuperarIndicadoresGuia($areid)
	{
		$sql = "SELECT distinct
				    ind.indid,
				    ind.indcod,
				    ind.inddsc,
				    ind.indajuda,
				    ind.areid,
				    ind.indrequerido
				FROM {$this->stNomeTabela} ind
				INNER JOIN par3.area ard on ind.areid = ard.areid
				INNER JOIN par3.dimensao dim on dim.dimid = ard.dimid
				    	AND dim.dimstatus = 'A'
				WHERE ind.indstatus = 'A'
				AND ind.areid = {$areid}
				ORDER BY ind.indcod
				--limit 1";

		return $this->carregar($sql);
	}

	public function recuperarDados($indid)
	{
		$sql = "SELECT distinct
				    ind.indid,
				    ind.indcod,
				    ind.inddsc,
				    ind.indajuda,
				    ind.areid,
				    ind.indlei,
				    ind.indrequerido
				FROM {$this->stNomeTabela} ind
				WHERE
				    ind.indid = {$indid}";

		return $this->pegaLinha($sql);
	}

	public function recuperaCodigoIndicador($indid){

		$sql = "SELECT
					dim.dimcod || '.' || ard.arecod || '.' || ind.indcod as codigo
				FROM {$this->stNomeTabela} ind
				INNER JOIN par3.area ard on ind.areid = ard.areid
				INNER JOIN par3.dimensao dim on dim.dimid = ard.dimid
				WHERE
					ind.indid = {$indid}";

		return $this->pegaUm($sql);
	}

	public function recuperarOrdemIndicadorPorAreid($areid)
	{
		$sql = "SELECT
					max(indcod) as ordem
				FROM {$this->stNomeTabela}
				WHERE areid = {$areid}";

		$this->stOrdem = $this->pegaUm($sql)+1;
	}

	public function verificaSubitensGuia($indid)
	{
		$sql = "SELECT
					count(c.crtid)
				FROM par3.criterio c
				WHERE c.indid = {$indid} AND c.crtstatus = 'A'";

		$nrCriterio = $this->pegaUm($sql);

		if($nrCriterio > 0){
			return false;
		} else {
			return true;
		}
	}

	public function verificaMetaGuia($indid)
	{
		$sql = "SELECT
					count(im.inmid)
				FROM par3.indicadormeta im
				WHERE im.indid = {$indid}";

		$nrCriterio = $this->pegaUm($sql);

		if($nrCriterio > 0){
			return false;
		} else {
			return true;
		}
	}

	public function deletarIndicadorGuia($indid)
	{
		if(!$this->verificaSubitensGuia($indid)){
		    simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'error', 'Não foi possível excluir o indicador, existem subitens cadastrados.');
			return false;
		}

		if(!$this->verificaMetaGuia($indid)){
			simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A', 'error', 'Não foi possível excluir o indicador, existem metas já informadas para este indicador.');
			return false;
		}

		$this->indid = $indid;
		$this->excluir();
		$this->commit();
	}

	public function preparaSalvar(){

	    $indid = $_POST['indid'] ? $_POST['indid'] : null;

	    $this->indid 		= $indid;
	    $this->indcod 	    = $_POST['ordcod'];
	    $this->inddsc 	    = $_POST['inddsc'];
	    
	    if(!$this->indlei || $this->indlei == '' )	
	    	$arCamposNulos = array('indlei');
	    
	    $this->indlei 	    = $_POST['indlei'];
	    $this->indajuda	    = $_POST['indajuda'];
		$this->indrequerido = $_POST['indrequerido'];
	    $this->indstatus 	= 'A';
	    $this->areid 		= $_POST['areid'];

	    $this->salvar('', '', $arCamposNulos);
	    $this->commit();

	    return $this->indid;
	}


	/**
	 * Função retornaQueryCodigoIndicador
	 * - retora a query para buscar o código do indicador
	 *
	 * @return $sql string.
	 *
	 */
	public function retornaQueryCodigoIndicador($arrParam){

	    $where = array();
	    $filtra = false;

	    if($arrParam['itrid']){
            $filtra = true;
	    	$where[] = "itrid = {$arrParam['itrid']}";
	    }

	    if($arrParam['indid']){
            $filtra = true;
	    	$where[] = "ind.indid = {$arrParam['indid']}";
	    }

	    $sql = "SELECT
                	dimcod||'.'||arecod||'.'||indcod as codigo,
                	indid,
                	are.areid,
                	dim.dimid,
                	dim.itrid
                FROM par3.indicador ind
                INNER JOIN par3.area are ON are.areid = ind.areid
                INNER JOIN par3.dimensao dim ON dim.dimid = are.dimid
                ".(($filtra) ? "WHERE ".implode(" AND ", $where) : "")."
                ORDER BY codigo";

	    return $sql;
	}// end retornaQueryCodigoIndicador()


	/**
	 * Função retornaCodigoIndicador
	 * - retora a o código do indicador e seus ids principais
	 *
	 * @return array(codigo=>string, indid=>integer, areid=>integer, dimid=>integer).
	 *
	 */
	public function retornaCodigoIndicador($indid){

	     $sqlPrincipal = $this->retornaQueryCodigoIndicador(Array('indid'=>$indid));

        $arrDados = $this->pegaLinha($sqlPrincipal);

	    return $arrDados;
	}// end retornaQueryCodigoIndicador()


	/**
	 * Função buscaIndicadorAnterior
	 * - retora o código do indicador anterior, se existir.
	 *
	 * @return retorna um array(codigo=>string, indid=>integer, areid=>integer, dimid=>integer).
	 *
	 */
	public function buscaIndicadorAnterior($indid)
	{
        $arrDados = $this->retornaCodigoIndicador($indid);

	    $sqlPrincipal = $this->retornaQueryCodigoIndicador(Array('itrid'=>$arrDados['itrid']));

	    $sql = "SELECT
                	*
                FROM
                (
                $sqlPrincipal
                ) as foo
                WHERE
                	codigo < '{$arrDados['codigo']}'
                ORDER BY codigo DESC";

	    $arrIndicadorAnterior = $this->pegaLinha($sql);

	    return $arrIndicadorAnterior;
	}


	/**
	 * Função buscaIndicadorProximo
	 * - retora o código do próximo indicador, se existir.
	 *
	 * @return retorna um array(codigo=>string, indid=>integer, areid=>integer, dimid=>integer).
	 *
	 */
	public function buscaIndicadorProximo($indid)
	{
        $arrDados = $this->retornaCodigoIndicador($indid);

	    $sqlPrincipal = $this->retornaQueryCodigoIndicador(Array('itrid'=>$arrDados['itrid']));

	    $sql = "SELECT
                	*
                FROM
                (
                $sqlPrincipal
                ) as foo
                WHERE
                	codigo > '{$arrDados['codigo']}'
                ORDER BY codigo";

	    $arrIndicadorAnterior = $this->pegaLinha($sql);

	    return $arrIndicadorAnterior;
	}// end buscaIndicadorProximo();

    /**
     * Lista de Indicadores
     *
     * @param $areid
     * @return array|mixed|NULL
     */
    public function listaIndicadores($areid)
    {
        return $this->carregar("SELECT 
                                    indid as codigo, 
                                    indcod||' - '||inddsc as descricao
                                FROM {$this->stNomeTabela}
                                WHERE indstatus = 'A' AND areid =  $areid ORDER BY indcod");

    }

}//end class

?>
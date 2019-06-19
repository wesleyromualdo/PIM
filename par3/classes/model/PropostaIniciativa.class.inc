<?php

class Par3_Model_PropostaIniciativa extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.propostainiciativa";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "priid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'priid' => null,
									  	'pricronograma' => null,
									  	'pridesc' => null,
    									'priorientacao' => null,
                                        'indid' => null,
                                        'obiid' => null,
										'aciid' => null,
									  );

	protected $stOrdem = null;

	public function recuperarIniciativaGuia($indid){
	    $sql = "SELECT *
        	    FROM {$this->stNomeTabela} pi
        	    WHERE pi.indid = {$indid}
	            ORDER BY pi.priid DESC";

	    return $this->carregar($sql);
	}

	public function recuperarIniciativa($priid){
        $sql = "SELECT
                    *
                FROM par3.propostainiciativa pi
        	    WHERE
        	       pi.priid = {$priid}";

        return $this->pegaLinha($sql);
	}

	public function recuperarOrdemCriterioPorIndid($indid){
	    $sql = "SELECT
                    max(crtcod) as ordem
        	    FROM {$this->stNomeTabela}
        	    WHERE indid = {$indid}";

	    $this->stOrdem = $this->pegaUm($sql)+1;
	}

	public function recuperarDadosListaVinculacao($indid, $crtid){
        $sql = "SELECT
                    cr.crtcod,
                    cr.crtid,
                    cr.crtdsc,
                    cv.crvid as checked,
                    cv.crvvinculo
                FROM {$this->stNomeTabela} cr
                LEFT JOIN par3.criterio_vinculacao cv ON cv.crtid = cr.crtid AND cv.crtidpai = {$crtid}
                WHERE
                    cr.indid = {$indid} AND
                    cr.crtid NOT IN ({$crtid})
                ORDER BY
                    cr.crtcod";

        return $this->carregar($sql);
	}

	public function preparaSalvar(){



	    $priid = $_POST['codigo'] ? $_POST['codigo'] : null;

	    $this->priid 		    = $priid;
	    $this->pridesc          = $_POST['pridesc'];
        $this->pricronograma    = $_POST['pricronograma'];
	    $this->priorientacao    = $_POST['priorientacao'];
	    $this->obiid  	        = $_POST['obiid'];
        $this->aciid   	        = $_POST['aciid'];
	    $this->indid 	    	= $_POST['indid'];
	    $this->salvar();
	    $this->commit();
	}

	public function deletarCriterioGuia($crtid){

	    $this->crtid = $crtid;
	    $this->excluir();
	    $this->commit();
	}

	public function atualizarOrdemCriterioGuia($crtid, $ordem){
	    $this->crtid  = $crtid;
	    $this->crtcod = $ordem;
	    $this->salvar();
	    $this->commit();
	}

}
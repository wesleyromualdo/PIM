<?php

class Par3_Model_Criterio extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.criterio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "crtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'crtid' => null,
									  	'indid' => null,
									  	'crtdsc' => null,
    									'crtstatus' => null,
                                        'crtcod' => null,
                                        'crtpontuacao' => null,
										'crtrequerido' => null,
									  );

	protected $stOrdem = null;

	public function recuperarCriteriosGuia($indid){
	    $sql = "SELECT DISTINCT
            	    cr.crtid,
            	    cr.crtdsc,
            	    cr.crtcod,
            	    cr.indid,
            	    cr.crtpontuacao,
            	    cr.crtid NOT IN ( SELECT crtidpai FROM par3.criterio_vinculacao WHERE crvvinculo = 2 ) as crvvinculo
        	    FROM {$this->stNomeTabela} cr
        	    WHERE cr.indid = {$indid} AND cr.crtstatus = 'A'
	            ORDER BY cr.crtcod";

	    return $this->carregar($sql);
	}

	public function recuperarCriterio($crtid){
        $sql = "SELECT
                    *
                FROM {$this->stNomeTabela} cr
        	    WHERE
        	       cr.crtid = {$crtid}";

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

	    $crtid = $_POST['crtid'] ? $_POST['crtid'] : null;

	    $this->crtid 		= $crtid;
	    $this->crtdsc 	    = $_POST['crtdsc'];
	    $this->crtcod       = $_POST['ordcod'];
	    $this->crtstatus 	= 'A';
	    $this->crtpontuacao	= $_POST['crtpontuacao'] == 1 ? $_POST['crtpontuacao'] : 2;
	    $this->indid 		= $_POST['indid'];
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


    /**
     * Lista Componentes do Indicador
     *
     * @param $indid
     * @return array|mixed|NULL
     */
    public function listaComponentesIndicador($indid)
    {
        return $this->carregar("SELECT DISTINCT
                                    crtid as codigo,
                                    crtcod||' - '||crtdsc as descricao
                                FROM {$this->stNomeTabela}
                                WHERE crtstatus = 'A' AND indid =  $indid ORDER BY 2");

    }

}
<?php

class Par3_Model_PropostaItemComposicao extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.propostaitemcomposicao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "picid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'picid' => null,
									  	'picdescricao' => null,
									  	'picdetalhe' => null,
    									'pictipodespesa' => null,
                                        'umiid' => null,
                                        'picstatus' => null,
										'picemendas' => null,
                                        'idsigarp' => null,
                                        'picpregao' => null
									  );

	protected $stOrdem = null;

	public function recuperarLista(){
	  $sql = "SELECT picid,picdescricao,picdetalhe,

      CASE WHEN picemendas= '1' THEN 'Sim'
            WHEN picemendas='0' THEN 'Não'
       END as pictipodespesa,
       umidescricao, picstatus,
       CASE WHEN picemendas= '1' THEN 'Sim'
            WHEN picemendas='0' THEN 'Não'
       END as picemendas

       ,
       CASE WHEN picpregao= 't' THEN 'Sim'
            WHEN picpregao= 'f' THEN 'Não'
       END
       as picpregao

        	    FROM {$this->stNomeTabela} pi
        	    join par3.unidademedidadetalhamentoitem umi on umi.umiid = pi.umiid
        	    ORDER BY pi.picid DESC";

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
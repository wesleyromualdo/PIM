<?php
class Par3_Model_RemanejamentoEmpenho extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.empenho";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array();
                                  
    function __construct($id = null)
    {
        parent::__construct($id);
    }
    
    public function recuperar($arrPost)
    {
        return $this->model;
    }
    
    public function sqlListaComposicaoProcesso( $arrPost )
    {
        $where = '';
        if( $arrPost['proid'] ) $where .= " and v.proid = {$arrPost['proid']} ";
        if( $arrPost['numeroprocesso'] ){
            $arrPost['numeroprocesso'] = str_replace(".","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("/","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("-","", $arrPost['numeroprocesso']);
            
            $where .= " and v.proid = (SELECT proid FROM par3.processo WHERE pronumeroprocesso = '{$arrPost['numeroprocesso']}') ";
        }
        
        if( $arrPost['tipo_processo'] == 'P' ){
            $sql = "SELECT id,
                	descricaoitem, anoitem, totalitem,  (totalitem - sum(contrapartida)) as valorfnde, sum(contrapartida) AS contrapartida,
                	codigo, sum(valorempenhado) AS valorempenhado, sum(percentual) AS percentual, sum(saldo_disponivel) AS saldo_disponivel, vlrpago, esddsc, anaid_processo, anaid_analise
                FROM(
                	SELECT DISTINCT
                		v.ppcid as id,
                		v.iniciativa as descricaoitem,
                		v.ppcano as anoitem,
                		v.vlriniciativa as totalitem,
                	    COALESCE(v.contrapartida,0) as contrapartida,
                		v.inpid as codigo,
                		coalesce(sum(v.saldo),0) as valorempenhado,
                	    CASE WHEN v.vlriniciativa > 0 THEN ((coalesce(sum(v.saldo),0)/v.vlriniciativa)*100)::numeric(20,2) ELSE 0 END AS percentual,
                	    (v.vlriniciativa - sum(v.saldo) ) AS saldo_disponivel,
                	    sum(v.vlrpago) AS vlrpago,
                	    es.esddsc,
                	    v.anaid AS anaid_processo,
                	    a.anaid AS anaid_analise
                	FROM par3.v_iniciativa_planejamento_processo_empenho v
                		LEFT JOIN par3.analise a
                		      INNER JOIN workflow.documento d ON d.docid = a.docid
                		      INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                		ON a.inpid = v.inpid AND a.anaano = v.ppcano
                	WHERE 1=1 $where
                	GROUP BY v.ppcid, v.iniciativa, v.ppcano, v.vlriniciativa, v.contrapartida,v.inpid, v.vlriniciativa,
                	    es.esddsc, v.anaid, a.anaid
                ) AS foo
                GROUP BY id, descricaoitem, anoitem, totalitem, codigo, vlrpago, esddsc, anaid_processo, anaid_analise";
        } else {
            $sql = "SELECT DISTINCT
                    	   v.pocid as id,
                    	   v.iniciativa as descricaoitem,
                    	   v.obrano as anoitem,
                    	   v.obrvalor as totalitem,
                           v.valorobra as valorfnde,
                           COALESCE(v.obrvalor_contrapartida,0) as contrapartida,
                    	   v.obrid as codigo,
                    	   sum(v.saldo) as valorempenhado,
                           CASE WHEN v.valorobra > 0 THEN ((sum(v.saldo)/v.valorobra)*100)::numeric(20,2) ELSE 0 END AS percentual,
                           (v.valorobra - sum(v.saldo)) AS saldo_disponivel,
                           v.vlrpago,
                           es.esddsc
                    FROM par3.v_obras_planejamento_processo_empenho v
                    	INNER JOIN par3.obra o ON o.obrid = v.obrid
                    	INNER JOIN workflow.documento d ON d.docid = o.docid
                    	INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                    WHERE 1=1 $where
                    GROUP BY v.pocid, v.valorobra, v.iniciativa, v.obrano, v.obrid, v.obrvalor_contrapartida, v.obrvalor, v.vlrpago, es.esddsc";
        }
        
        return $sql;
    }
}

<?php
/**
 * Classe de mapeamento da entidade emenda.emenda
 *
 * @version $Id: Emenda.class.inc 140947 2018-06-14 13:27:17Z victormachado $
 */

/**
 * Emenda_Model_Emenda
 *
 * @package Spoemendas\Model\Emenda
 * @author MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 */
class Spoemendas_Model_Emenda extends Modelo
{
    /**
     * @var string Nome da tabela especificada
     */
    protected $stNomeTabela = 'emenda.emenda';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'emeid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'autid' => array('tabela' => 'emenda.autor', 'pk' => 'autid'),
        'etoid' => array('tabela' => 'emenda.emendatipoorigem', 'pk' => 'etoid'),
        'resid' => array('tabela' => 'emenda.responsavel', 'pk' => 'resid'),
        'acaid' => array('tabela' => 'acao', 'pk' => 'acaid'),
        'ploid' => array('tabela' => 'monitora.planoorcamentario', 'pk' => 'ploid'),
    );

    /**
     * @var mixed[] Atributos
     */
    protected $arAtributos = array(
        'emeid' => null,
        'emecod' => null,
        'autid' => null,
        'acaid' => null,
        'resid' => null,
        'emetipo' => null,
        'emeano' => null,
        'emedescentraliza' => null,
        'emelibera' => null,
        'emevalor' => null,
        'etoid' => null,
        'emerelator' => null,
        'ploid' => null
    );

    /**
     * @var mixed[] Validação
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'emeid' => array(  'Digits'  ),
            'emecod' => array(  new Zend_Validate_StringLength(array('max' => 8))  ),
            'autid' => array(  'Digits'  ),
            'acaid' => array( 'allowEmpty' => true, 'Digits'  ),
            'resid' => array( 'allowEmpty' => true, 'Digits'  ),
            'emetipo' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'emeano' => array( 'allowEmpty' => true, 'Digits'  ),
            'emedescentraliza' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'emelibera' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
            'emevalor' => array( 'allowEmpty' => true ),
            'etoid' => array( 'allowEmpty' => true, 'Digits'  ),
            'emerelator' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
            'ploid' => array( 'allowEmpty' => true, 'Digits'  ),
        );
    }

    public function queryListaEmendas(array $params = [])
    {
        $emendaImpositiva = EMENDA_IMPOSITIVA;

        $where = [];
        foreach (array_filter($params, 'strlen') as $campo => $valor) {
            switch ($campo) {
                case 'autid':
                    $where[] = "eme.{$campo} = '{$valor}'";
                    break;
                case 'unicod':
                    $where[] = "vfp.{$campo} = '{$valor}'";
                    break;
                default:
                    $where[] = "{$campo} = '{$valor}'";

            }
        }

        if (!empty($where)) {
            $where = ' AND ' . implode(' AND ', $where);
        } else {
            $where = '';
        }

        return <<<DML
SELECT eme.emeid,
       eme.emecod,
       CASE WHEN eme.emerelator = 'S' THEN aut.autnome || ' - Relator Geral'
            ELSE aut.autnome
            END AS autnome,
       vfp.unicod,
       uni.unidsc,
       vfp.fupfuncionalprogramatica,
       vfp.fupdsc,
       COALESCE(res.resdsc, 'Não informado') AS resdsc,
       eto.etodescricao,
       CASE WHEN (SELECT COUNT(emd.emdid)
                    FROM emenda.emendadetalhe emd
                    WHERE emd.emeid = eme.emeid
                    	and emdstatus = 'A'
                      AND emd.emdimpositiva = '{$emendaImpositiva}') > 0 THEN 'Sim'
            ELSE 'Não'
            END AS impositivo
  FROM {$this->stNomeTabela} eme
    INNER JOIN emenda.autor aut USING(autid)
    INNER JOIN emenda.v_funcionalprogramatica vfp USING(acaid)
    INNER JOIN public.unidade uni USING(unicod)
    LEFT JOIN emenda.responsavel res USING(resid)
    LEFT JOIN emenda.emendatipoorigem eto ON (eme.etoid = eto.etoid AND eto.etostatus = 'A')
  WHERE emeano = '{$_SESSION['exercicio']}'
    {$where}
DML;
    }

    /**
     * Retorna o autor da emenda
     *
     * @param integer $emeid
     * @return array
     */
    public function autorEmenda($emeid)
    {
    	$sql = <<<DML
SELECT aut.autid,
       aut.autnome || ' (' || par.parsigla || ')' AS autor
  FROM emenda.autor aut
    JOIN emenda.partido par ON aut.parid = par.parid
  WHERE EXISTS (SELECT 1
                              FROM emenda.emenda eme
                              INNER JOIN monitora.acao USING(acaid)
                              WHERE eme.autid = aut.autid AND eme.emeid = {$emeid})
  AND aut.autstatus = 'A'
DML;

    	return $this->pegaLinha($sql);
    }

    public function carregarPorCodigo($emecod, $emeano = null)
    {
        if (is_null($emeano)) {
            $emeano = $_SESSION['exercicio'];
        }

        $sql = <<<SQL
SELECT emeid
  FROM emenda.emenda
  WHERE emecod = '{$emecod}'
    AND emeano = '{$emeano}'
SQL;

        return $this->pegaUm($sql);
    }

    public function antesSalvar()
    {
        if (strstr($this->emevalor, ',')) {
            $this->emevalor = str_replace(['.', ','], ['', '.'], $this->emevalor);
        }
        return parent::antesSalvar();
    }

    //** TODO: não precisa de emenda.emenda
    public function carregarBeneficiariosPorEmeid($emeid, $cnpj = null)
    {
        $sql = <<<DML
SELECT emd.*,
       ede.*,
       enb.*
  FROM emenda.emenda eme
    INNER JOIN emenda.emendadetalhe emd using(emeid)
    INNER JOIN emenda.emendadetalheentidade ede using(emdid)
    INNER JOIN emenda.entidadebeneficiada enb using(enbid)
  WHERE eme.emeid = {$emeid}
    --AND emd.emdstatus = 'A'
    --AND ede.edestatus = 'A'
    AND enb.enbstatus = 'A'
DML;
        if (!empty($cnpj)) {
            $sql .= <<<DML
    AND enb.enbcnpj = '{$cnpj}'
DML;
        }

        $beneficiarios = $this->carregar($sql);
        return $beneficiarios?$beneficiarios:[];
    }
    /**
     * retorna SQL que busca os oficios das emendas
     * @param $emeid
     * @return string
     */
    public function oficiosEmenda($emeid)
    {
        return <<<SQL
				SELECT
					autnome || ' (' || par.parsigla || ')' AS solicitante,
					(
						SELECT 
							string_agg(tps.tpsdsc, ' / ')
						FROM 
							spoemendas.oficiotiposolicitacao oft
						INNER JOIN 
							spoemendas.tiposolicitacao tps ON tps.tpsid = oft.tpsid 
						WHERE 
							oft.ofiid = ofi.ofiid 
						GROUP BY oft.ofiid
					) AS tiposolicitacao,
					usu.usunome,
					TO_CHAR(ofi.oficriacao, 'DD/MM/YYYY HH24:MI:SS'),
					esd.esddsc
				FROM 
					emenda.oficio ofi
				INNER JOIN 
					emenda.autor aut ON ofi.autid = aut.autid
				INNER JOIN 
					emenda.partido par ON aut.parid = par.parid
				INNER JOIN 
					seguranca.usuario usu ON ofi.usucpf = usu.usucpf
				INNER JOIN 
					workflow.documento doc ON ofi.docid = doc.docid
				INNER JOIN 
					workflow.estadodocumento esd ON doc.esdid = esd.esdid
				INNER JOIN
				  public.arquivo arq ON arq.arqid = ofi.arqid
				WHERE arq.arqstatus = 'A' AND ofi.emeid = {$emeid}
SQL;
    }
    /**
     * Tramita automatico emendas para preenchimento limite liberado
     * Quando houver beneficiario a liberar limite
     */
    public function tramitaAutomaticoEmendalimitLiberado()
    {
        global $db;
        $sql = <<<SQL
            SELECT
		            ee.emeid,
		            sei.docid
        	FROM
            		emenda.emenda ee
        	INNER JOIN spoemendas.emendainformacoes sei ON ee.emeid = sei.emeid
        	INNER JOIN workflow.documento doc ON doc.docid = sei.docid
        WHERE
            emeano = {$_SESSION['exercicio']}
            AND doc.esdid = '1701';
SQL;
        
        $result = $db->carregar($sql);
		
        if(is_array($result)){
	        foreach ($result as $infoEmenda) {
	            $dados = (new Spoemendas_Model_Emendaimpedimentobeneficiario())->pegaImpedimentos(['emeid' => $infoEmenda['emeid']]);
	            $nh = 0;
	            $possui = 0;
	            $perfis = pegaPerfilGeral($_SESSION['usucpf']);
	            
	            if(is_array($dados)){
		            foreach ($dados as $dado) {
		                if (trim($dado['ediimpositivo']) == 'NH') {
		                    $nh++;
		                }
		                if (trim($dado['ediimpositivo']) == 'TO' || trim($dado['ediimpositivo']) == 'PA') {
		                    $possui++;
		                }
		            }
	            }

	            $msg = "Tramitação automatica quando emenda possui beneficiario para ser liberado limite, estado limite liberado";
	            if ($nh > 0 && $possui > 0) {
	                wf_alterarEstado($infoEmenda['docid'], 4990, $msg, [], []);
	            }
	        }
        }
    }
}

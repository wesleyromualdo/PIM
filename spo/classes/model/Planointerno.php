<?php
/**
 * Classe de mapeamento da entidade monitora.pi_planointerno.
 *
 * $Id: Planointerno.php 125045 2017-06-16 12:11:38Z thiagofrinhani $
 */

/**
 * Mapeamento da entidade monitora.pi_planointerno.
 *
 * @see Modelo
 */
class Spo_Model_Planointerno extends Modelo
{
    const CADASTRADO_SIAFI_SIMEC = 1;
    const CADASTRADO_SIMEC = 2;
    const CADASTRADO_SIAFI = 3;

    const PI_VALIDO = 'v';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.pi_planointerno';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pliid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'capid' => array('tabela' => 'monitora.pi_categoriaapropriacao', 'pk' => 'capid'),
        'eqdid' => array('tabela' => 'monitora.pi_enquadramentodespesa', 'pk' => 'eqdid'),
        'mdeid' => array('tabela' => 'monitora.pi_modalidadeensino', 'pk' => 'mdeid'),
        'neeid' => array('tabela' => 'monitora.pi_niveletapaensino', 'pk' => 'neeid'),
        'obrid' => array('tabela' => 'obras.obrainfraestrutura', 'pk' => 'obrid'),
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
        'sbaid' => array('tabela' => 'monitora.pi_subacao', 'pk' => 'sbaid'),
        'unicod' => array('tabela' => 'public.unidade', 'pk' => 'unicod'),
        'ungcod' => array('tabela' => 'public.unidadegestora', 'pk' => 'ungcod')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pliid' => null,
        'mdeid' => null,
        'eqdid' => null,
        'neeid' => null,
        'capid' => null,
        'sbaid' => null,
        'obrid' => null,
        'plisituacao' => null,
        'plititulo' => null,
        'plidata' => null,
        'plistatus' => null,
        'plicodsubacao' => null,
        'plicod' => null,
        'plilivre' => null,
        'plidsc' => null,
        'usucpf' => null,
        'unicod' => null,
        'ungcod' => null,
        'pliano' => null,
        'plicadsiafi' => null,
    );

    public static function queryInstituicoesFederais(array $params, $obrigatorias = false, $perfis = array())
    {
        self::checarParametros($params, array('exercicio'));

        if ($obrigatorias) {
            $obrigatoriasPli = ' AND uni.unicod NOT IN(' . Spo_Model_Unidade::getObrigatorias(true) . ')';
            $obrigatoriasSex = ' AND sex.unicod NOT IN(' . Spo_Model_Unidade::getObrigatorias(true) . ')';
        } else {
            $obrigatoriasPli = $obrigatoriasSex = '';
        }

        if(isset($params['planointerno']) && $params['planointerno'] != NULL){
            $plicod = " AND pli.plicod LIKE '%{$params['planointerno']}%' ";
            $sPlicod = " AND sex.plicod  LIKE '%{$params['planointerno']}%' ";
        }else{
            $plicod = '';
            $sPlicod = " AND sex.plicod != '' ";
        }
        
        $sql = <<<DML
WITH pli AS (SELECT pli.pliid::varchar AS pliid,
                    pli.plicod,
                    COALESCE(pli.plititulo, 'N/A') AS plititulo,
                    uni.unicod,
                    uni.unidsc,
                    COALESCE(pli.obrid::varchar, 'N/A') AS obrid,
                    array_to_json(ARRAY_AGG(DISTINCT poc.tcpid))::varchar AS tcpid,
                    CASE WHEN pli.plicadsiafi = 't' THEN 1 -- cadastrado no SIAFI E SIMEC
                         WHEN COALESCE(pli.plicadsiafi, 'f') = 'f' THEN 2 -- cadastrado no SIMEC
                         ELSE 4 -- não identificado
                      END AS cadastramento,
                    pli.pliano,
                    ptr.ptres,
                    SUM(COALESCE(ppt.pipvalor, 0.00)) AS vlrdotacao,
                    SUM(COALESCE(sex.vlrempenhado, 0.00)) AS vlrempenhado,
                    (SUM(COALESCE(sex.vlrdotacaoatual, 0.00) - COALESCE(sex.vlrempenhado, 0.00))) AS vlrnaoempenhado
               FROM monitora.pi_planointerno pli
                 INNER JOIN public.unidade uni USING(unicod)
                 LEFT JOIN monitora.pi_planointernoptres ppt USING(pliid)
                 LEFT JOIN monitora.ptres ptr ON (ppt.ptrid = ptr.ptrid AND pli.pliano = ptr.ptrano)
                 LEFT JOIN spo.siopexecucao sex
	               ON (pli.unicod = sex.unicod
                       AND pli.plicod = sex.plicod
                       AND pli.pliano = sex.exercicio
                       AND ptr.ptres = sex.ptres)
                 LEFT JOIN ted.orcamentario poc USING(pliid)
               WHERE pli.pliano = '{$params['exercicio']}'
                 AND pli.plistatus = 'A'
                 {$obrigatoriasPli}
                 {$plicod}
                 
                 __WHERE_SIMEC__
               GROUP BY pli.pliid,
                        pli.plicod,
                        pli.plititulo,
                        uni.unicod,
                        uni.unidsc,
                        pli.obrid,
                        pli.plicadsiafi,
                        pli.pliano,
                        ptr.ptres
               ORDER BY pli.plicod)
DML;

        $sqlUnion['simec'] = <<<DML
SELECT pli.pliid,
       pli.plicod,
       pli.plititulo,
       pli.unicod,
       pli.unidsc,
       pli.obrid,
       pli.tcpid,
       pli.cadastramento,
       'v' AS valido,
       pli.vlrdotacao,
       pli.vlrempenhado,
       pli.vlrnaoempenhado
  FROM pli
DML;

        $sqlUnion['siafi'] = <<<DML
SELECT sex.plicod AS pliid,
       sex.plicod,
       'N/A' AS plititulo,
       sex.unicod,
       uni.unidsc,
       'N/A' AS obrid,
       '[null]' AS tcpid,
       3 AS cadastramento,
       COALESCE(piv.pivid::varchar, 'v') AS valido,
       SUM(COALESCE(sex.vlrdotacaoatual, 0.00)) AS vlrdotacao,
       SUM(COALESCE(sex.vlrempenhado, 0.00)) AS vlrempenhado,
       (SUM(COALESCE(sex.vlrdotacaoatual, 0.00) - COALESCE(sex.vlrempenhado, 0.00))) AS vlrnaoempenhado
  FROM spo.siopexecucao sex
    INNER JOIN public.unidade uni USING(unicod)
    LEFT JOIN spo.planointernoinvalido piv
      ON (piv.plicod = sex.plicod
          AND piv.unicod = sex.unicod
          AND piv.pivano = sex.exercicio)
  WHERE sex.exercicio = '{$params['exercicio']}'
    {$sPlicod}
    {$obrigatoriasSex}
    AND NOT EXISTS (SELECT 1
                      FROM pli
                      WHERE pli.plicod = sex.plicod
                        AND pli.unicod = sex.unicod
                        AND pli.pliano = sex.exercicio
                        AND pli.ptres = sex.ptres)
    __WHERE_SIAFI__
  GROUP BY sex.plicod,
           sex.unicod,
           uni.unidsc,
           piv.pivid
DML;

        $where = array(
            'simec' => array(),
            'siafi' => array()
        );

        // -- Filtro de cadastramento
        switch ($params['cadastramento']) {
            case self::CADASTRADO_SIAFI_SIMEC:
                unset($sqlUnion['siafi']);
                $where['simec'][] = "pli.plicadsiafi = 't'";
                break;
            case self::CADASTRADO_SIMEC:
                unset($sqlUnion['siafi']);
                $where['simec'][] = "COALESCE(pli.plicadsiafi, 'f') = 'f'";
                break;
            case self::CADASTRADO_SIAFI:
                unset($sqlUnion['simec']);
                break;
        }

        // -- Filtro de obras, apenas SIMEC e SIMEC/SIAFI
        switch ($params['obras']) {
            case 'S':
                unset($sqlUnion['siafi']);
                $where['simec'][] = "pli.obrid IS NOT NULL";
                break;
            case 'N':
                unset($sqlUnion['siafi']);
                $where['simec'][] = "pli.obrid IS NULL";
                break;
        }

        // -- Filtro do TED, apenas SIMEC e SIMEC/SIAFI
        switch ($params['ted']) {
            case 'S':
                unset($sqlUnion['siafi']);
                $where['simec'][] = "poc.tcpid IS NOT NULL";
                break;
            case 'N':
                unset($sqlUnion['siafi']);
                $where['simec'][] = "poc.tcpid IS NULL";
                break;
        }

        // -- Filtro de pi compatível, apenas SIAFI
        switch ($params['compativel']) {
            case 'S':
                unset($sqlUnion['simec']);
                $where['siafi'][] = "piv.pivid IS NULL";
                break;
            case 'N':
                unset($sqlUnion['simec']);
                $where['siafi'][] = "piv.pivid IS NOT NULL";
                break;
        }

        // -- Filtro de like no titulo do PI, combinado com a classe Dml logo abaixo
        if (!empty($params['descricao'])) {
            unset($sqlUnion['siafi']);
            $where['simec'][] = <<<DML
public.removeacento(pli.plititulo) ilike :descricao
DML;
        }

        // -- Filtros incompatíveis, que descartaram os dois sqls
        if (empty($sqlUnion)) {
            return 'SELECT 1 WHERE 1 = 2';
        }

        $sql .= implode(' UNION ', $sqlUnion) . <<<DML
  ORDER BY cadastramento,
           plicod,
           unicod
DML;

        // -- Filtro de unicod
        if (!empty($params['unicod'])) {
            $where['simec'][] = sprintf("pli.unicod = '%s'", $params['unicod']);
            $where['siafi'][] = sprintf("sex.unicod = '%s'", $params['unicod']);
        }

        // -- Filtro de ptres, combinado com a classe Dml logo abaixo
        if (!empty($params['ptres']) && !empty($params['ptres'][0])) {
            $where['simec'][] = "ptr.ptres = :ptres";
            $where['siafi'][] = "sex.ptres = :ptres";
        }
        
        $perfis =  is_array($perfis) ? $perfis : array();
        
        // -- Filtros de perfil: PFL_GESTAO_ORCAMENTARIA_IFS
        if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
            $sqlPerfil = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
            $where['simec'][] = $where['siafi'][] = sprintf($sqlPerfil, $params['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
        }
      
       
        
        // -- Filtros de perfil: PFL_GABINETE, apenas Simec e Simec/Siafi
        if (in_array(PFL_GABINETE, $perfis)) {
            unset($where['siafi']);
            $sqlPerfil = <<<DML
sbaid IN (SELECT sbaid
            FROM monitora.pi_subacaounidade sbu
            WHERE sbu.ungcod IN (SELECT ungcod
                                   FROM public.unidadegestora
                                   WHERE ungcod IN (SELECT DISTINCT ungcod
                                                      FROM planacomorc.usuarioresponsabilidade usr
                                                      WHERE usr.usucpf = '%s'
                                                        AND usr.pflcod = %d
                                                        AND usr.rpustatus = 'A')))
DML;
            $where['simec'][] = sprintf($sqlPerfil, $params['usucpf'], PFL_GABINETE);
        }

        // -- Aplicando filtros WHERE
        $where['simec'] = $where['simec']?' AND ' . implode(' AND ', $where['simec']):'';
        $where['siafi'] = $where['siafi']?' AND ' . implode(' AND ', $where['siafi']):'';

        $sql = str_replace(
            array('__WHERE_SIMEC__', '__WHERE_SIAFI__'),
            array($where['simec'], $where['siafi']),
            $sql
        );
//ver($sql);
        $dml = new Simec_DB_DML($sql);
        if (!empty($params['ptres']) && !empty($params['ptres'][0])) {
            $dml->addParam('ptres', $params['ptres']);
        }
        if (!empty($params['descricao'])) {
            $dml->addParam('descricao', '%' . removeAcentos(str_replace("-", "", $params['descricao'])) . '%');
        }

        return (string)$dml;
    }

    public static function queryGraficoFinanceiro($params)
    {
        self::checarParametros($params, array('unicod', 'plicod', 'exercicio'));

        $sql = <<<DML
SELECT 'Dotação' AS descricao,
       'Total' AS categoria,
       valor AS valor
  FROM (SELECT SUM(pip.pipvalor) AS valor
          FROM monitora.pi_planointernoptres pip
            INNER JOIN monitora.pi_planointerno pli using(pliid)
          WHERE pliano = :exercicio
            AND plicod  = :plicod
            AND unicod = :unicod) foo
UNION ALL
SELECT 'Empenhado'  AS descricao,
       'Total' as categoria,
       vlrempenhado AS valor
  FROM (SELECT SUM(vlrempenhado) AS vlrempenhado
          FROM spo.siopexecucao sex
          WHERE sex.exercicio = :exercicio
            AND plicod = :plicod
            AND unicod = :unicod
          GROUP BY plicod) foo
UNION ALL
SELECT 'Liquidado'  AS descricao,
       'Total' as categoria,
       vlrliquidado AS valor
  FROM (SELECT SUM(vlrliquidado) AS vlrliquidado
          FROM spo.siopexecucao sex
          WHERE sex.exercicio = :exercicio
            AND plicod = :plicod
            AND unicod = :unicod
          GROUP BY plicod) foo
UNION ALL
SELECT 'Pago'  AS descricao,
       'Total' as categoria,
       vlrpago AS valor
  FROM (SELECT SUM(vlrpago) AS vlrpago
          FROM spo.siopexecucao sex
          WHERE sex.exercicio = :exercicio
            AND plicod = :plicod
            AND unicod = :unicod
          GROUP BY plicod) foo
DML;
        $dml = new Simec_DB_DML($sql);
        $dml->addParams($params)
            ->addParam('exercicio', $params['exercicio'], true);

        return (string)$dml;
    }

    public static function queryAcoes(array $params)
    {
        self::checarParametros($params, array('pliid', 'exercicio'));

        $sql = <<<DML
SELECT pli.pliid,
       ptr.ptres,
       pip.ptrid,
       ptr.acaid,
       TRIM(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc) AS descricao,
       SUM(ptr.ptrdotacao) AS dotacaoinicial,
       ROUND(SUM(COALESCE(sad.sadvalor, 0)), 2) AS dotacaosubacao,
       ROUND(SUM(COALESCE(sex.vlrdotacaoatual, 0.00)), 2) AS empenhado,
       pip.pipvalor as detalhadoptres
  FROM monitora.pi_planointerno pli
    INNER JOIN monitora.pi_planointernoptres pip USING(pliid)
    LEFT JOIN monitora.ptres ptr
      ON (pip.ptrid = ptr.ptrid
          AND pli.pliano = ptr.ptrano)
    LEFT JOIN monitora.acao aca USING(acaid)
    LEFT JOIN monitora.pi_subacaodotacao sad
      ON(ptr.ptrid = sad.ptrid AND pli.sbaid = sad.sbaid)
    LEFT JOIN spo.siopexecucao sex
      ON (sex.ptres = ptr.ptres
          AND sex.unicod = pli.unicod
          AND sex.plicod = pli.plicod
          AND sex.exercicio = pli.pliano)
        WHERE pli.pliid = :pliid
          AND pli.pliano = :exercicio
          AND pli.plistatus = 'A'
        GROUP BY pli.pliid,
                 pip.ptrid,
                 ptr.ptres,
                 pli.plistatus,
                 pip.pipvalor,
                 aca.prgcod,
                 ptr.acaid,
                 aca.acacod,
                 aca.unicod,
                 aca.loccod,
                 aca.acadsc
        ORDER BY ptr.ptres
DML;
        $dml = new Simec_DB_DML($sql);
        $dml->addParam('exercicio', $params['exercicio'], true)
            ->addParam('pliid', $params['pliid']);

        return (string)$dml;
    }
}

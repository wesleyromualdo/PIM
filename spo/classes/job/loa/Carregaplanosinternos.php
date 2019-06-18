<?php
/**
 * Job para carregar a tabela monitora.pi_planointerno.
 *
 * @version $Id: Carregaplanosinternos.php 136899 2018-01-25 00:03:17Z victormachado $
 */

class Spo_Job_Loa_Carregaplanosinternos extends Simec_Job_Abstract
{
    protected $params = [];

    /**
     * Guarda referencia ao anoExercicio que será usado para duplicação dos dados
     * @var
     */
    private $exercicioReferencia;

    public function getName()
    {
        return 'Carregando Plano Interno...';
    }

    protected function init()
    {
        $this->params = array_merge(
            $this->params,
            $this->loadParams()
        );

        $this->exercicioReferencia = ($this->params['exercicio']-1);
    }

    protected function execute()
    {
        echo "Apagando Planos Internos do exercício {$this->params['exercicio']}...<br />";
        $this->salvarOutput();
        $this->apagarPlanoInterno();

//        echo 'Carregando novos Planos Internos...<br />';
//        $this->salvarOutput();
        $this->replicarDadosTabelasAuxiliares();

        echo 'Carregando novos Planos Internos...<br />';
        $this->salvarOutput();
        $this->insertPlanosInternos();

        $this->updateEnqudramentos();

        echo 'Carregando novos PTRES...<br />';
        $this->salvarOutput();
        $this->inserirPlanoInternoPTRES();

        echo $this->getTotalPlanoInterno() . " Planos Internos inseridos...";
        $this->salvarOutput();
    }

    protected function shutdown()
    {
    }

    private function getTotalPlanoInterno()
    {
        return $this->pegaUm(
            "SELECT COUNT(*) FROM monitora.pi_planointerno WHERE pliano = '{$this->params['exercicio']}'");
    }

    protected function apagarPlanoInterno()
    {
        try {
            $this->limpaTabelaPlanoInternoPTRES();
            $this->limpaTabelaPIEnquadramentos();
        } catch (Exception $e) {
            throw new Exception(
                'Não foi possível limpar as tabelas procedimento de pre-carga para o plano interno.');
        }
    }

    /**
     * Faz o vinculo dos PI's com os PTRES
     */
    protected function inserirPlanoInternoPTRES()
    {
        $strSQL = <<<DML
         
        
INSERT INTO monitora.pi_planointernoptres
(
    pliid,
    ptrid,
    pipvalor,
    pipvalorinicial
)
SELECT 
	(SELECT pliid FROM monitora.pi_planointerno WHERE plicod = pi.plicod AND unicod = pi.unicod AND usucpf = pi.usucpf AND pliano = '{$this->params['exercicio']}') as _pliid,
	(select ptrid from monitora.ptres where ptres = pt.ptres AND ptrano = '{$this->params['exercicio']}') as _ptrid,
	pipvalor,
	pipvalorinicial
FROM monitora.pi_planointernoptres pptres
JOIN monitora.pi_planointerno pi ON (pi.pliid = pptres.pliid AND pi.pliano = '{$this->exercicioReferencia}' AND pi.plistatus = 'A')
JOIN monitora.ptres pt ON (pt.ptrid = pptres.ptrid AND pt.ptrano = '{$this->exercicioReferencia}')
JOIN monitora.acao aca ON (aca.acaid = pt.acaid AND aca.prgano = '{$this->exercicioReferencia}')
WHERE (plicod NOT IN ('LPP02P419E3', 'LPP02P419D2', 'LPP02P42925', 'LPP02P42386'))
AND EXISTS(select 1 from monitora.ptres where ptres = pt.ptres AND ptrano = '2018');
DML;
        try {
            $this->executar($strSQL);
        } catch (Exception $e) {
            throw new Exception('Não foi possível associar os Planos Internos aos PTRES.');
        }
    }

    /**
     * Limpa a tabela de vinculacao de PI e PTRES
     *
     * @return resource
     * @throws Exception
     */
    private function limpaTabelaPlanoInternoPTRES()
    {
        $strSQL = <<<DML
DELETE FROM monitora.pi_planointernoptres WHERE pliid IN (
	SELECT 
		(SELECT pliid 
		    FROM monitora.pi_planointerno 
		        WHERE plicod = pi.plicod 
		            AND unicod = pi.unicod 
		                AND usucpf = pi.usucpf 
		                    AND pliano = '{$this->params['exercicio']}') as _pliid
	FROM monitora.pi_planointernoptres pptres
	JOIN monitora.pi_planointerno pi ON (pi.pliid = pptres.pliid AND pi.pliano = '{$this->exercicioReferencia}' AND pi.plistatus = 'A')
	JOIN monitora.ptres pt ON (pt.ptrid = pptres.ptrid AND pt.ptrano = '{$this->exercicioReferencia}')
	JOIN monitora.acao aca ON (aca.acaid = pt.acaid AND aca.prgano = '{$this->exercicioReferencia}')
	WHERE plicod NOT IN ('LPP02P419E3', 'LPP02P419D2', 'LPP02P42925', 'LPP02P42386')
)
DML;
        return $this->executar($strSQL);
    }

    /**
     * Limpa a tabela de carga em PI
     * Limpa a tabela auxiliares a carga de Plano Interno
     *
     * @return resource
     * @throws Exception
     */
    private function limpaTabelaPIEnquadramentos()
    {
        $strSQL = <<<DML
DELETE FROM monitora.pi_planointerno WHERE pliano = '{$this->params['exercicio']}' AND plicod NOT IN ('LPP02P419E3', 'LPP02P419D2', 'LPP02P42925', 'LPP02P42386');
DELETE FROM monitora.pi_modalidadeensino WHERE mdeano = '{$this->params['exercicio']}';
DELETE FROM monitora.pi_enquadramentodespesa WHERE eqdano = '{$this->params['exercicio']}';
DELETE FROM monitora.pi_niveletapaensino WHERE neeano = '{$this->params['exercicio']}';
DELETE FROM monitora.pi_categoriaapropriacao WHERE capano = '{$this->params['exercicio']}';
DELETE FROM monitora.pi_subacaounidade WHERE sbaano = '{$this->params['exercicio']}';
DML;
        return $this->executar($strSQL);
    }

    /**
     * Carga em Modalidade de Ensino
     *
     * @return resource
     * @throws Exception
     */
    private function insertModalidadeEnsino()
    {
        $strSQL = <<<DML
INSERT
INTO
    monitora.pi_modalidadeensino
SELECT
    nextval('monitora.pi_modalidadeensino_mdeid_seq'::regclass),
    moe.mdecod,
    moe.mdedsc,
    '{$this->params['exercicio']}',
    'A'
FROM
    monitora.pi_modalidadeensino moe
WHERE
    moe.mdeano = '{$this->exercicioReferencia}'
AND moe.mdestatus = 'A'
DML;
        return $this->executar($strSQL);
    }

    /**
     * Carga em Enquadramento Despesa
     *
     * @return resource
     * @throws Exception
     */
    private function insertEnquadramentoDespesas()
    {
        $strSQL = <<<DML
INSERT
INTO
    monitora.pi_enquadramentodespesa
SELECT
    nextval('monitora.pi_enquadramentodespesa_eqdid_seq'::regclass),
    eqd.eqdcod,
    eqd.eqddsc,
    '{$this->params['exercicio']}',
    'A'
FROM
    monitora.pi_enquadramentodespesa eqd
WHERE
    eqd.eqdano = '{$this->exercicioReferencia}'
AND eqdstatus = 'A'
DML;
        return $this->executar($strSQL);
    }

    /**
     * Carga em Nivel e etapa de ensino
     *
     * @return resource
     * @throws Exception
     */
    private function insertNivelEtapaEnsino()
    {
        $strSQL = <<<DML
INSERT
INTO
    monitora.pi_niveletapaensino
SELECT
    nextval('monitora.pi_niveletapaensino_neeid_seq'::regclass),
    nee.neecod,
    nee.needsc,
    '{$this->params['exercicio']}',
    'A'
FROM
    monitora.pi_niveletapaensino nee
WHERE
    nee.neeano = '{$this->exercicioReferencia}'
AND nee.neestatus = 'A'
DML;
        return $this->executar($strSQL);
    }

    /**
     * Carga em Categoria de Apropriacao (PI)
     *
     * @return resource
     * @throws Exception
     */
    private function insertCategoriaApropriacaoPI()
    {
        $strSQL = <<<DML
INSERT
INTO
    monitora.pi_categoriaapropriacao
SELECT
    nextval('monitora.pi_categoriaapropriacao_capid_seq'::regclass),
    a.capcod,
    a.capdsc,
    '{$this->params['exercicio']}',
    a.capstatus
FROM
    monitora.pi_categoriaapropriacao a
WHERE
    a.capstatus ='A'
AND a.capano = '{$this->exercicioReferencia}'
DML;
        return $this->executar($strSQL);
    }

    /**
     * Carga em Subacao Unidade
     *
     * @return resource
     * @throws Exception
     */
    private function insertSubacaoUnidade()
    {
        $strSQL = <<<DML
INSERT
INTO
    monitora.pi_subacaounidade
    (
        sbaid,
        unicod,
        unitpocod,
        ungcod,
        sbaano
    )
SELECT
    *
FROM
    (
        SELECT
            novo AS sbaid,
            unicod,
            'U',
            ungcod,
            '{$this->params['exercicio']}' AS sbaano
        FROM
            monitora.pi_subacaounidade a
        JOIN
            (
                SELECT
                    sbaid AS velho,
                    (
                        SELECT
                            sbaid
                        FROM
                            monitora.pi_subacao
                        WHERE
                            sbaano = '{$this->params['exercicio']}'
                        AND sbacod = a.sbacod limit 1) AS novo
                FROM
                    monitora.pi_subacao a
                WHERE
                    sbaano = '{$this->exercicioReferencia}') foo
        ON
            a.sbaid = foo.velho
        WHERE
            unicod IS NOT NULL
        OR  ungcod IS NOT NULL  ) z
WHERE
    z.sbaid IS NOT NULL
DML;
        return $this->executar($strSQL);
    }

    /**
     * @throws Exception
     */
    private function replicarDadosTabelasAuxiliares()
    {
        try {
            echo 'Carregando novas Modalidades de Ensino...<br />';
            $this->salvarOutput();
            $this->insertModalidadeEnsino();

            echo 'Carregando novos Enquadramentos de Despesas...<br />';
            $this->salvarOutput();
            $this->insertEnquadramentoDespesas();

            echo 'Carregando novos Níveis de etapa de ensino...<br />';
            $this->salvarOutput();
            $this->insertNivelEtapaEnsino();

            echo 'Carregando novas Categorias de Apropriação de PI...<br />';
            $this->salvarOutput();
            $this->insertCategoriaApropriacaoPI();

            echo 'Carregando novas Subações para a Unidade...<br />';
            $this->salvarOutput();
            $this->insertSubacaoUnidade();
        } catch (Exception $e) {
            throw new Exception('Não foi possível carregar as tabelas auxiliares a carga de Plano Interno.');
        }
    }

    /**
     * Carga de Plano Interno
     *
     * @throws Exception
     */
    private function insertPlanosInternos()
    {
        $strSQL = <<<DML
SELECT setval('monitora.pi_planointerno_pliid_seq', COALESCE((SELECT MAX(pliid)+1 FROM monitora.pi_planointerno), 1), false);
        
INSERT
INTO
    monitora.pi_planointerno
    (
        mdeid,
        eqdid,
        neeid,
        capid,
        sbaid,
        plisituacao,
        plititulo,
        plidata,
        plistatus,
        plicodsubacao,
        plicod,
        plilivre,
        plidsc,
        usucpf,
        unicod,
        ungcod,
        pliano,
        plicadsiafi
    )
SELECT
    mdeid,
    eqdid,
    neeid,
    capid,
    sbaid,
    plisituacao,
    plititulo,
    NOW(),
    plistatus,
    plicodsubacao,
    plicod,
    plilivre,
    plidsc,
    usucpf,
    unicod,
    ungcod,
    '{$this->params['exercicio']}',
    plicadsiafi
FROM
    monitora.pi_planointerno pli
WHERE
    pliano = '{$this->exercicioReferencia}'
AND plistatus = 'A'
AND plicod IS NOT NULL
AND plicod <>''
AND plicod NOT IN ('LPP02P419E3', 'LPP02P419D2', 'LPP02P42925', 'LPP02P42386')
AND unicod||'.'||plicod NOT IN
    (
        SELECT
            unicod||'.'||plicod
        FROM
            monitora.pi_planointerno
        WHERE
            pliano = '{$this->params['exercicio']}'
        AND plicod <>''
        AND plicod IS NOT NULL )
DML;
        try {
            $this->executar($strSQL);
        } catch (Exception $e) {
            throw new Exception('Não foi possível carregar os Planos Intenos e seus enquadramentos.');
        }
    }

    private function updateEnqudramentos()
    {
        try {
            echo 'Atualizando os enquadramentos das subações...<br />';
            $this->salvarOutput();
            $this->updateEnquadramentoSubacao();

            echo 'Atualizando os enquadramentos das modalidades de ensino...<br />';
            $this->salvarOutput();
            $this->updateEnquadramentoModalidadeEnsino();

            echo 'Atualizando os enquadramentos das despesas...<br />';
            $this->salvarOutput();
            $this->updateEnquadramentoDespesas();

            echo 'Atualizando os níveis de etapas de ensino...<br />';
            $this->salvarOutput();
            $this->updateNivelEtapaEnsino();

            echo 'Atualizando as categorias de apropriação de PI...<br />';
            $this->salvarOutput();
            $this->updateCategoriaApropriacao();
        } catch (Exception $e) {
            throw new Exception('Falha ao atualizar os enquadramentos para carga de Plano Interno.');
        }
    }

    private function updateCategoriaApropriacao()
    {
        $strSQL = <<<DML
UPDATE
    monitora.pi_planointerno pli
SET
    capid = foo.novo
FROM
    (
        SELECT
            capid AS velho,
            (
                SELECT
                    capid
                FROM
                    monitora.pi_categoriaapropriacao
                WHERE
                    capano = '{$this->params['exercicio']}'
                AND capcod = a.capcod) AS novo
        FROM
            monitora.pi_categoriaapropriacao a
        WHERE
            capano = '{$this->exercicioReferencia}' ) foo
WHERE
    pliano = '{$this->params['exercicio']}'
AND pli.capid = foo.velho
AND pli.plicod NOT IN ('LPP02P419E3', 'LPP02P419D2', 'LPP02P42925', 'LPP02P42386')
DML;
        return $this->executar($strSQL);
    }

    private function updateNivelEtapaEnsino()
    {
        $strSQL = <<<DML
UPDATE
    monitora.pi_planointerno pli
SET
    neeid = foo.novo
FROM
    (
        SELECT
            neeid AS velho,
            (
                SELECT
                    neeid
                FROM
                    monitora.pi_niveletapaensino
                WHERE
                    neeano = '{$this->params['exercicio']}'
                AND neecod = a.neecod) AS novo
        FROM
            monitora.pi_niveletapaensino a
        WHERE
            neeano = '{$this->exercicioReferencia}' ) foo
WHERE
    pliano = '{$this->params['exercicio']}'
AND pli.neeid = foo.velho
AND pli.plicod NOT IN ('LPP02P419E3', 'LPP02P419D2', 'LPP02P42925', 'LPP02P42386')
DML;
        return $this->executar($strSQL);
    }

    private function updateEnquadramentoDespesas()
    {
        $strSQL = <<<DML
UPDATE
    monitora.pi_planointerno pli
SET
    eqdid = foo.novo
FROM
    (
        SELECT
            eqdid AS velho,
            (
                SELECT
                    eqdid
                FROM
                    monitora.pi_enquadramentodespesa
                WHERE
                    eqdano = '{$this->params['exercicio']}'
                AND eqdcod = a.eqdcod) AS novo
        FROM
            monitora.pi_enquadramentodespesa a
        WHERE
            eqdano = '{$this->exercicioReferencia}' ) foo
WHERE
    pliano = '{$this->params['exercicio']}'
AND pli.eqdid = foo.velho
AND pli.plicod NOT IN ('LPP02P419E3', 'LPP02P419D2', 'LPP02P42925', 'LPP02P42386')
DML;
        return $this->executar($strSQL);
    }

    private function updateEnquadramentoModalidadeEnsino()
    {
        $strSQL = <<<DML
UPDATE
    monitora.pi_planointerno pli
SET
    mdeid = foo.novo
FROM
    (
        SELECT
            mdeid AS velho,
            (
                SELECT
                    mdeid
                FROM
                    monitora.pi_modalidadeensino
                WHERE
                    mdeano = '{$this->params['exercicio']}'
                AND mdecod = a.mdecod) AS novo
        FROM
            monitora.pi_modalidadeensino a
        WHERE
            mdeano = '{$this->exercicioReferencia}' ) foo
WHERE
    pliano = '{$this->params['exercicio']}'
AND pli.mdeid = foo.velho
AND pli.plicod NOT IN ('LPP02P419E3', 'LPP02P419D2', 'LPP02P42925', 'LPP02P42386')
DML;
        return $this->executar($strSQL);
    }

    private function updateEnquadramentoSubacao()
    {
        $strSQL = <<<DML
UPDATE
    monitora.pi_planointerno pli
SET
    sbaid = foo.novo
FROM
    (
        SELECT
            sbaid AS velho,
            (
                SELECT
                    sbaid
                FROM
                    monitora.pi_subacao
                WHERE
                    sbaano = '{$this->params['exercicio']}'
                AND sbacod = a.sbacod limit 1) AS novo
        FROM
            monitora.pi_subacao a
        WHERE
            sbaano = '{$this->exercicioReferencia}' ) foo
WHERE
    pliano = '{$this->params['exercicio']}'
AND pli.sbaid = foo.velho
AND pli.plicod NOT IN ('LPP02P419E3', 'LPP02P419D2', 'LPP02P42925', 'LPP02P42386')
DML;
        return $this->executar($strSQL);
    }

}
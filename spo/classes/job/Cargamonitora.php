<?php
/**
 * Created by PhpStorm.
 * User: saulocorreia
 * Date: 22/11/2017
 * Time: 14:46
 */

class Spo_Job_Cargamonitora extends Simec_Job_Abstract
{
    /**
     * @var $array
     */
    var $params;

    /**
     * Retorna o label dada para este Job
     *
     * @return string
     */
    public function getName()
    {
        return 'Atualizando Bases';
    }

    protected function init()
    {
        $this->params = $this->loadParams();
    }

    protected function execute()
    {
        try {
            echo "Processamento Iniciado.<br>";
            $this->salvarOutput();

            foreach ($this->params['atualizar'] as $atualizar) {

                switch ($atualizar) {
                    case 'Ação':
                        echo 'Atualizando Tabela de Ação.<br>';
                        $this->salvarOutput();
                        $this->atualizarAcao();

                        echo 'Atualizando Tabela de Localizadores.<br>';
                        $this->salvarOutput();
                        $this->atualizarLocalizadores();

                        break;
                    case 'Plano Orçamentario':
                        echo 'Atualizando Tabela de Plano Orçamentário.<br>';
                        $this->salvarOutput();

                        $this->atualizarPlanoOrcamentario();
                        break;
                    default :
                        throw  new Exception("Comando para atualizar tabela {$atualizar} desconhecido.");
                }
            }

            $this->commit();

            echo "<br>Base atualizada com sucesso.";
        } catch (Exception $e) {
            $this->rollback();

            throw $e;
        }
    }

    protected function shutdown()
    {
    }

    /**
     * @throws Exception
     */
    protected function atualizarAcao()
    {
        try {
            $this->montaUpdateAcao($this->params['exercicio']);

            $this->montaInsertAcao($this->params['exercicio']);
        } catch (Exception $e) {
            throw $e;
        }
    }

    protected function atualizarPlanoOrcamentario()
    {
        try {
            $insert = 0;
            $update = 0;

            foreach ($this->buscarPlanosOrcamentarios() as $linhaPO) {
                $acao = $this->procurarAcao($linhaPO['identificadorunicoacao']);

                if ($acao) {
                    $metaFisica = $this->procurarMetafisica($linhaPO['identificadorunico']);
                    $po = $this->procurarPO($linhaPO['identificadorunico'], $acao['acaid']);

                    if (empty($po)) {
                        $this->inserirPO($acao, $linhaPO, $metaFisica);
                        $insert++;
                    } else {
                        $this->updatePO($acao, $linhaPO, $metaFisica);
                        $update++;
                    }
                }
            }

            echo "{$insert} registros inseridos de Plano Orçamentário.<br>{$update} registros atualizados de Plano Orçamentário.<br>";
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @return array|mixed|NULL
     * @throws Exception
     */
    protected function buscarPlanosOrcamentarios()
    {
        try {
            $resultado = $this->carregar(
                <<<DML
        SELECT *
          FROM {$this->params['schemaDb']}.{$this->params['prefixoTabela']}planosorcamentariosdto
         WHERE planoorcamentario <> '0000'
        ORDER BY identificadorunicoacao
DML
            );

            return $resultado ? $resultado : [];
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $identificadorunicoacao
     *
     * @return array|bool|mixed|NULL
     * @throws Exception
     */
    private function procurarAcao($identificadorunicoacao)
    {
        try {
            return $this->pegaLinha(
                sprintf(
                    <<<DML
        SELECT acaid, acacod, unicod, acacod, loccod, prgcod
          FROM monitora.acao
          WHERE ididentificadorunicosiop::INT = %d
            AND prgano = '{$this->params['exercicio']}'
DML
                    , $identificadorunicoacao
                )
            );
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $identificadorunico
     *
     * @return bool|mixed|string
     * @throws Exception
     */
    private function procurarMetafisica($identificadorunico)
    {
        try {
            $metaFisica = $this->pegaUm(
                sprintf(<<<DML
        SELECT
            DISTINCT quantidadefisico
            FROM {$this->params['schemaDb']}.{$this->params['prefixoTabela']}metaplanoorcamentariodto
            WHERE identificadorunicoplanoorcamentario = %s
DML
                    , $identificadorunico
                )
            );

            return (!$metaFisica) ? 'null' : $metaFisica;
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $identificadorunicosiop
     * @param $identificadorunicoacao
     *
     * @return bool|mixed|NULL|string
     * @throws Exception
     */
    private function procurarPO($identificadorunicosiop, $identificadorunicoacao)
    {
        try {
            return $this->pegaUm(
                <<<DML
        SELECT DISTINCT ploid
          FROM monitora.planoorcamentario
         WHERE ploidentificadorunicosiop = '{$identificadorunicosiop}'
           AND acaid = {$identificadorunicoacao}
           AND exercicio = '{$this->params['exercicio']}'
DML
            );
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $acao
     * @param $linha
     * @param $metaFisica
     *
     * @throws Exception
     */
    private function inserirPO($acao, $linha, $metaFisica)
    {
        try {
            $this->executar(<<<DML
        INSERT INTO monitora.planoorcamentario
        ( prgcod, acacod, unicod, plocodigo, ploidentificadorunicosiop, plotitulo, plodetalhamento,
          ploproduto, plounidademedida, ploobrigatorio, plostatus, acaid, exercicio, prddsc, unmdsc,
          metafisica, loccod )
        VALUES (
            '{$acao['prgcod']}',
            '{$acao['acacod']}',
            '{$acao['unicod']}',
            '{$linha['planoorcamentario']}',
            '{$linha['identificadorunico']}',
            '{$linha['titulo']}',
            '{$linha['detalhamento']}',
            '{$linha['codigoproduto']}',
            '{$linha['codigounidademedida']}',
            'f',
            'A',
            {$acao['acaid']},
            '{$this->params['exercicio']}',
            '{$linha['descproduto']}',
            '{$linha['descunidademedida']}',
            $metaFisica,
            '{$acao['loccod']}'
        )
DML
            );
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $acao
     * @param $linha
     * @param $metaFisica
     *
     * @throws Exception
     */
    private function updatePO($acao, $linha, $metaFisica)
    {
        try {
            $this->executar(<<<DML
        UPDATE monitora.planoorcamentario SET
            prgcod           = '{$acao['prgcod']}',
            acacod           = '{$acao['acacod']}',
            unicod           = '{$acao['unicod']}',
            plocodigo        = '{$linha['planoorcamentario']}',
            plotitulo        = '{$linha['titulo']}',
            plodetalhamento  = '{$linha['detalhamento']}',
            ploproduto       = '{$linha['codigoproduto']}',
            plounidademedida = '{$linha['codigounidademedida']}',
            ploobrigatorio   = 'f',
            plostatus        = 'A',
            exercicio        = '{$this->params['exercicio']}',
            prddsc           = '{$linha['descproduto']}',
            unmdsc           = '{$linha['descunidademedida']}',
            metafisica       = $metaFisica,
            loccod           = '{$acao['loccod']}'
        WHERE ploidentificadorunicosiop = '{$linha['identificadorunico']}'
          AND acaid = {$acao['acaid']}
DML
            );
        } catch (Exception $e) {
            throw $e;
        }
    }


    /**
     * @param $exercicio
     *
     * @throws Exception
     */
    private function montaInsertAcao($exercicio)
    {
        try {
            $this->executar(<<<SQL
INSERT INTO monitora.acao (
    acacod, saccod, loccod, esfcod, unicod, unitpocod, funcod, regcod, taccod, procod,
    prgano, prgcod, sitcodestagio, sitcodandamento, sitcodcronograma, acadsc, sacdsc,
    acasnmetanaocumulativa, acadscsituacaoatual, acasnrap, acadescricao, acabaselegal,
    acasntransfoutras, acadetalhamento, sfucod, acastatus, acasnestrategica, acasnbgu,
    acaptres, acadataatualizacao, acatipoinclusao, acatipoinclusaolocalizador, unmdsc,
    prodsc, descricaomomento, unidaderesponsavel, acatitulo, ididentificadorunicosiop,
    unmcodsof, procodsof, acatipocod, acatipodsc, acainiciativacod, acainiciativadsc,
    acaobjetivocod, acaobjetivodsc, prgdsc, esfdsc, fundsc, sfundsc) {$this->montaSelect(false, $exercicio)}
SQL
            );
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param $exercicio
     *
     * @throws Exception
     */
    private function montaUpdateAcao($exercicio)
    {
        try {
            $this->executar(<<<SQL
WITH subquery AS (
{$this->montaSelect(true, $exercicio)})
UPDATE monitora.acao
SET
    acabaselegal               = subquery.acabaselegal,
    acadataatualizacao         = subquery.acadataatualizacao,
    acadescricao               = subquery.acadescricao,
    acadetalhamento            = subquery.acadetalhamento,
    acadsc                     = subquery.acadsc,
    acadscsituacaoatual        = subquery.acadscsituacaoatual,
    acainiciativacod           = subquery.acainiciativacod,
    acainiciativadsc           = subquery.acainiciativadsc,
    acaobjetivocod             = subquery.acaobjetivocod,
    acaobjetivodsc             = subquery.acaobjetivodsc,
    acaptres                   = subquery.acaptres,
    acasnbgu                   = subquery.acasnbgu,
    acasnestrategica           = subquery.acasnestrategica,
    acasnmetanaocumulativa     = subquery.acasnmetanaocumulativa,
    acasnrap                   = subquery.acasnrap,
    acasntransfoutras          = subquery.acasntransfoutras,
    acastatus                  = subquery.acastatus,
    acatipocod                 = subquery.acatipocod,
    acatipodsc                 = subquery.acatipodsc,
    acatipoinclusao            = subquery.acatipoinclusao,
    acatipoinclusaolocalizador = subquery.acatipoinclusaolocalizador,
    acatitulo                  = subquery.acatitulo,
    descricaomomento           = subquery.descricaomomento,
    esfdsc                     = subquery.esfdsc,
    fundsc                     = subquery.fundsc,
    ididentificadorunicosiop   = subquery.ididentificadorunicosiop,
    prgdsc                     = subquery.prgdsc,
    procod                     = subquery.procod,
    procodsof                  = subquery.procodsof,
    prodsc                     = subquery.prodsc,
    regcod                     = subquery.regcod,
    saccod                     = subquery.saccod,
    sacdsc                     = subquery.sacdsc,
    sfundsc                    = subquery.sfundsc,
    sitcodandamento            = subquery.sitcodandamento,
    sitcodcronograma           = subquery.sitcodcronograma,
    sitcodestagio              = subquery.sitcodestagio,
    taccod                     = subquery.taccod,
    unidaderesponsavel         = subquery.unidaderesponsavel,
    unitpocod                  = subquery.unitpocod,
    unmcodsof                  = subquery.unmcodsof,
    unmdsc                     = subquery.unmdsc
FROM subquery
WHERE
    monitora.acao.esfcod = subquery.esfcod
    AND monitora.acao.funcod = subquery.funcod
    AND monitora.acao.sfucod = subquery.sfucod
    AND monitora.acao.unicod = subquery.unicod
    AND monitora.acao.acacod = subquery.acacod
    AND monitora.acao.prgcod = subquery.prgcod
    AND monitora.acao.loccod = subquery.loccod;
SQL
            );
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param bool $existe
     * @param      $exercicio
     *
     * @return string
     */
    private function montaSelect($existe, $exercicio)
    {
        $existe = $existe ? '' : ' NOT ';

        return <<<SQL
SELECT
          aca.codigoacao                        AS acacod
        , lpad (loc.codigolocalizador, 4, '0')  AS saccod
        , lpad (loc.codigolocalizador, 4, '0')  AS loccod
        , aca.codigoesfera :: INTEGER           AS esfcod
        , aca.codigoorgao                       AS unicod
        , 'U'                                   AS unitpocod
        , aca.codigofuncao                      AS funcod
        , CASE
          WHEN TRIM (loc.uf) <> ''
              THEN UPPER (TRIM (loc.uf))
          END                                   AS regcod
        , 1                                     AS taccod
        , CASE WHEN aca.codigoproduto = '' THEN NULL
          ELSE aca.codigoproduto :: INTEGER END AS procod
        , aca.exercicio                         AS prgano
        , aca.codigoprograma                    AS prgcod
        , '00'                                  AS sitcodestagio
        , '00'                                  AS sitcodandamento
        , '00'                                  AS sitcodcronograma
        , aca.titulo                            AS acadsc
        , loc.descricao                         AS sacdsc
        , TRUE                                  AS acasnmetanaocumulativa
        , FALSE                                 AS acadscsituacaoatual
        , FALSE                                 AS acasnrap
        , aca.descricao                         AS acadescricao
        , aca.baselegal                         AS acabaselegal
        , FALSE                                 AS acasntransfoutras
        , aca.detalhamentoimplementacao         AS acadetalhamento
        , aca.codigosubfuncao                   AS sfucod
        , 'A'                                   AS acastatus
        , FALSE                                 AS acasnestrategica
        , FALSE                                 AS acasnbgu
        , '@@@'                                 AS acaptres
        , CURRENT_DATE                          AS acadataatualizacao
        , aca.codigotipoinclusaoacao            AS acatipoinclusao
        , loc.codigotipoinclusao                AS acatipoinclusaolocalizador
        , unm.descricao                         AS unmdsc
        , pro.descricao                         AS prodsc
        , aca.codigomomento                     AS descricaomomento
        , aca.unidaderesponsavel                AS unidaderesponsavel
        , aca.titulo                            AS acatitulo
        , aca.identificadorunico                AS ididentificadorunicosiop
        , aca.codigounidademedida               AS unmcodsof
        , pro.codigoproduto                     AS procodsof
        , aca.codigotipoacao                    AS acatipocod
        , tac.descricao                         AS acatipodsc
        , aca.codigoiniciativa                  AS acainiciativacod
        , ini.titulo                            AS acainiciativadsc
        , aca.codigoobjetivo                    AS acaobjetivocod
        , obj.enunciado                         AS acaobjetivodsc
        , prg.titulo                            AS prgdsc
        , esf.descricao                         AS esfdsc
        , fun.descricao                         AS fundsc
        , sfun.descricao                        AS sfundsc
    FROM
        {$this->params['schemaDb']}.{$this->params['prefixoTabela']}acoesdto aca
        JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}localizadoresdto loc    ON aca.identificadorunico = loc.identificadorunicoacao
        JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}produtosdto pro         ON aca.codigoproduto = pro.codigoproduto :: VARCHAR
        JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}unidadesmedidadto unm   ON aca.codigounidademedida = unm.codigounidademedida
        JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}tiposacaodto tac        ON aca.codigotipoacao = tac.codigotipoacao
        LEFT JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}iniciativasdto ini ON aca.codigoiniciativa = ini.codigoiniciativa
        LEFT JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}objetivosdto obj   ON aca.codigoobjetivo = obj.codigoobjetivo
        LEFT JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}programasdto prg   ON prg.codigoprograma = aca.codigoprograma
        LEFT JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}esferasdto esf     ON esf.codigoesfera = aca.codigoesfera
        LEFT JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}funcoesdto fun     ON fun.codigofuncao = aca.codigofuncao
        LEFT JOIN {$this->params['schemaDb']}.{$this->params['prefixoTabela']}subfuncoesdto sfun ON sfun.codigosubfuncao = aca.codigosubfuncao
    WHERE
        aca.codigoproduto <> ''
        AND {$existe} exists (SELECT 1
                        FROM
                            monitora.acao a
                        WHERE
                            a.prgano = '{$exercicio}'
                            AND aca.codigoesfera :: INTEGER = a.esfcod
                            AND aca.codigofuncao = a.funcod
                            AND aca.codigosubfuncao = a.sfucod
                            AND aca.codigoorgao = a.unicod
                            AND aca.codigoacao = a.acacod
                            AND aca.codigoprograma = a.prgcod
                            AND lpad (loc.codigolocalizador, 4, '0') = a.loccod)
SQL;

    }

    /**
     * @throws Exception
     */
    private function atualizarLocalizadores()
    {
        try {
            $sql = <<<DML
INSERT INTO public.localizador (loccod, locdsc)
    SELECT
        codigolocalizador
        , (
              SELECT CASE
                     WHEN length (descricao) > 97
                         THEN substr (descricao, 0, 97) || '...'
                     ELSE descricao END
              FROM acomporc.job_localizadoresdto dt
              WHERE
                  dt.codigolocalizador = dto.codigolocalizador
                  AND dt.datahoraalteracao = max (dto.datahoraalteracao)) descricao
    FROM acomporc.job_localizadoresdto dto
    WHERE
        NOT EXISTS (SELECT 1
                    FROM public.localizador l
                    WHERE
                        l.loccod = dto.codigolocalizador)
    GROUP BY
        dto.codigolocalizador
DML;
            $this->executar($sql);
        } catch (Exception $e) {
            throw $e;
        }
    }
}
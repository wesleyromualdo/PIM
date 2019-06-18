<?php
/**
 * Implementação da consulta de dados da Ficha de Monitoramento Externo.
 *
 * @version $Id: DadoFichaMonitoramentoExterno.php 117405 2017-01-11 17:03:58Z victormachado $
 */

/**
 * Dado - Macro-Categoria
 */
class DadoFichaMonitoramentoExterno extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados da Ficha de Monitoramento Externo
     *
     * @return array
     */
    public function carregaDado()
    {
        global $db;

        $where = '';
        if (is_array($this->EstadosSolicitados)) {
            $where = "WHERE estuf IN ('" . implode("','", $this->EstadosSolicitados) . "')";
        }
        $sql = "
 	       SELECT
                    CASE
                            WHEN usucpf IS NOT NULL 
                                AND fmtdatainclusao is not null 
                                AND fmtpme is not null
                                AND (fmtperavalanual  = 't' OR fmtperavalbianual = 't' OR fmtperavaltrianual = 't' OR fmtperavalquadrienal = 't' OR fmtperavalquinquenal = 't' OR fmtperavalnaoprevisto = 't')
                                AND fmtperavalano1 is not null
                                AND fmtcomcoordenadora is not null
                                AND fmtcomnumanoatolegal is not null
                                AND fmtequipetecnica is not null
                                AND fmtequipeatolegal is not null
                                AND ( fmtequipetelefone  is not null OR fmtequipeemail  is not null ) 
                            THEN '#006600'
                            ELSE '#FFFFFF'
                    END as cor,
                    CASE
                        WHEN usucpf IS NOT NULL 
                            AND fmtdatainclusao is not null 
                            AND fmtpme is not null
                            AND (fmtperavalanual  = 't' OR fmtperavalbianual = 't' OR fmtperavaltrianual = 't' OR fmtperavalquadrienal = 't' OR fmtperavalquinquenal = 't' OR fmtperavalnaoprevisto = 't')
                            AND fmtperavalano1 is not null
                            AND fmtcomcoordenadora is not null
                            AND fmtcomnumanoatolegal is not null
                            AND fmtequipetecnica is not null
                            AND fmtequipeatolegal is not null
                            AND ( fmtequipetelefone  is not null OR fmtequipeemail  is not null ) 
                        THEN 'Preenchido'
                            ELSE 'Não preenchido'
                    END as situacao,
                    fm.muncod as muncod,
                    fm.estuf as estuf
                FROM sase.fichamonitoramento fm
               {$where}
        ";
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $and = '';
        if (count($this->estuf) > 0) {
            $and = " AND a.estuf IN('" . implode($this->estuf) . "')";
        }
        $sql = <<<DML
           SELECT 
                COUNT(1) AS total,
                descricao AS descricao,
                CASE 
                    WHEN descricao = 'Preenchido' THEN '#006600'
                    ELSE '#FFFFFF' 
                END AS cor
            FROM (
               SELECT

                    CASE
                      WHEN
                            usucpf is not null 
                        AND fmtdatainclusao is not null 
                        AND fmtpme is not null
                        AND (fmtperavalanual  = 't' OR fmtperavalbianual = 't' OR fmtperavaltrianual = 't' OR fmtperavalquadrienal = 't' OR fmtperavalquinquenal = 't' OR fmtperavalnaoprevisto = 't')
                        AND fmtperavalano1 is not null
                        AND fmtcomcoordenadora is not null
                        AND fmtcomnumanoatolegal is not null
                        AND fmtequipetecnica is not null
                        AND fmtequipeatolegal is not null
                        AND ( fmtequipetelefone  is not null OR fmtequipeemail  is not null )
                      THEN     'Preenchido'
                      ELSE     'Não preenchido'
                    END as descricao
                FROM sase.fichamonitoramento a
                JOIN territorios.estado e ON e.estuf = a.estuf
                JOIN territorios.municipio m ON m.muncod = a.muncod WHERE 1=1 AND fmtstatus = 'A'
                {$and}
                )a
              GROUP BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}

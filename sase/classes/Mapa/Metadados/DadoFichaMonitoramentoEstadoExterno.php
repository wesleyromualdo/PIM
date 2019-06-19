<?php
/**
 * Implementação da consulta de dados da FichaMonitoramentoEstadoExterno.
 *
 * @version $Id: DadoFichaMonitoramentoEstadoExterno.php 113997 2016-10-10 20:47:08Z mariluciaqueiroz $
 */

/**
 * Dado - Macro-Ficha Monitoramento Externo
 */
class DadoFichaMonitoramentoEstadoExterno extends DadosAbstract implements DadosInterface, DadosLegendaInterface
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
                            WHEN usucpf is not null 
                                AND fmedatainclusao is not null 
                                AND fmepme is not null
                                AND (fmeperavalanual = 't' OR fmeperavalbianual = 't' OR fmeperavaltrianual = 't' OR fmeperavalquadrienal = 't' OR fmeperavalquinquenal = 't' OR fmeperavalnaoprevisto = 't' )
                                AND fmeperavalano1 is not null
                                AND fmecomcoordenadora is not null
                                AND fmecomnumanoatolegal is not null
                                AND fmeequipetecnica is not null
                                AND fmeequipeatolegal is not null
                                AND (fmeequipetelefone is not null OR fmeequipeemail is not null)
                            THEN '#006600'
                            ELSE '#FFFFFF'
                    END as cor,
                    CASE
                            WHEN usucpf IS NOT NULL AND fmedatainclusao IS NOT NULL THEN 'Preenchido'
                            ELSE 'Não preenchido'
                    END as situacao,
                    fme.estuf as estuf
                FROM sase.fichamonitoramentoestado fme
               {$where}
        ";
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $and = '';
        if (count($this->estuf) > 0) {
            $and = " AND estuf IN('" . implode($this->estuf) . "')";
        }
        
        $sql = <<<DML
            SELECT 
                COUNT(1) AS total,
                descricao AS descricao,
                CASE 
                    WHEN descricao = 'Preenchido' THEN '#006600'
                    ELSE '#FFFFFF' 
                END AS cor
            FROM (SELECT CASE WHEN (fmepme IS NULL
                              OR fmeperavalanual IS NULL
                              OR fmeperavaltrianual IS NULL
                              OR fmeperavalquadrienal IS NULL
                              OR fmeperavalano1 IS NULL
                              OR fmecomcoordenadora IS NULL
                              OR fmecomnumanoatolegal IS NULL
                              OR fmeequipetecnica IS NULL
                              OR fmeequipeatolegal IS NULL
                              OR fmeperavalquinquenal IS NULL
                              OR fmeperavalnaoprevisto IS NULL
                              OR COALESCE(fmeequipetelefone, fmeequipeemail) IS NULL) THEN 'Não preenchido'
                        ELSE 'Preenchido' END AS descricao,
                   *
              FROM sase.fichamonitoramentoestado a 
              WHERE fmestatus = 'A' 
              {$and}) a
              GROUP BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}

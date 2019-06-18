<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação da Lei PNE dos estados.
 *
 * @version $Id: DadosEstadosPlanoCarreiraPCR.php 113443 2016-09-19 14:22:02Z mariluciaqueiroz $
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */

/**
 * Classe de dados da Lei PNE.
 */
class DadosEstadosPlanoCarreiraPCR extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados com informações da Lei PNE dos estados.
     *
     * @return void
     */
    public function carregaDado()
    {
        global $db;

        $sql = <<<DML
SELECT 
	CASE WHEN pcepcr IS NOT NULL
		THEN '#006600'
		ELSE '#FFFFFF' END AS cor,
	CASE WHEN pcepcr IS NOT NULL
		THEN 'Com PCR'
		ELSE 'Sem PCR' END AS situacao,
       estuf
FROM sase.planocarreiraprofessorestado pcpe
DML;
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $sql = <<<DML
SELECT count(pcepcr) AS total,
       'Com PCR' AS descricao,
       '#006600' AS cor
  FROM sase.planocarreiraprofessorestado pcpe
UNION ALL
SELECT COUNT(1),
       'Sem PCR' as descricao,
       '#FFFFFF' as cor
  FROM sase.planocarreiraprofessorestado pcpe
  WHERE pcepcr IS NULL
  ORDER BY descricao
DML;
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}

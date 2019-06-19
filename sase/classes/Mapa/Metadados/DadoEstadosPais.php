<?php
/**
 * Implementação da classe de metadados do Pais exibindo a informação de adequação dos estados.
 *
 * @version $Id: DadoEstadosPais.php 110582 2016-04-29 14:02:39Z maykelbraz $
 */

/**
 * Classe de dados de adequação/acessoramento.
 *
 */
class DadoEstadosPais extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados da SituaÃ§Ã£o de MesoregiÃ£o
     *
     * @return void
     * @internal carrrega $this->metaDados
     */
    public function carregaDado()
    {
        global $db;

        $sql = <<<DML
SELECT s.stacor AS cor,
       CASE WHEN s.stacod IS NULL
              THEN 'Não Cadastrado'
            ELSE s.stadsc END AS situacao,
       a.estuf
  FROM sase.assessoramentoestado a
    INNER JOIN sase.situacaoassessoramento s USING(stacod)
DML;

        $this->dado = $db->carregar( $sql );
    }

    public function dadosDaLegenda()
    {
        global $db;
				$sql = "
						SELECT s.stacod,
                               s.stadsc AS descricao,
							   s.stacor AS cor,
							   count(a.aseid) as total
						FROM sase.situacaoassessoramento s
						  LEFT JOIN sase.assessoramentoEstado a on a.stacod = s.stacod
						WHERE stastatus = 'A'
						GROUP BY 1,2,3
						ORDER BY stacod ASC ";
				$legenda = $db->carregar( $sql );
                return $legenda?$legenda:[];
    }
}

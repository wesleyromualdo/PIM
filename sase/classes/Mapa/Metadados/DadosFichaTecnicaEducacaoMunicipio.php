<?php
/**
 * Created by PhpStorm.
 * User: victor
 * Date: 09/01/17
 * Time: 15:55
 */

/**
 * Dado - Situação Assessoramento Estado
 */
class DadosFichaTecnicaEducacaoMunicipio extends DadosAbstract implements DadosInterface
{
    /**
     * Carrega metadados da Situação de Assessoramento por Estado
     *
     * @return void
     * @internal carrrega $this->metaDados
     * @todo buscar da Modelo
     */
    public function carregaDado(){
        global $db;

        $sql = "
                SELECT
                    s.stacor as cor,
                    CASE
                        WHEN s.stacod IS NULL THEN 'Não Cadastrado'
                        ELSE s.stadsc
                    END as situacao,
                    a.estuf as estuf
                FROM sase.assessoramentoestado a
                --INNER JOIN workflow.documento d ON d.docid = a.docid
                --INNER JOIN sase.situacaoassessoramento s ON s.esdid = d.esdid
                INNER JOIN sase.situacaoassessoramento s ON a.stacod = s.stacod
                WHERE 1=1
                ".( (is_array($this->estuf))?" AND a.estuf IN ('".(implode("','",$this->estuf))."') ":"" )."
        ";
        $this->dado = $db->carregar( $sql );
    }
}
<?php
/**
 * Classe de mapeamento da entidade spo.tiporecurso.
 *
 * @version $Id: Tiporecurso.php 104762 2015-11-12 20:32:40Z maykelbraz $
 */

/**
 * Spo_Model_Tiporecurso
 *
 * @package  Spo\Model
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
class Spo_Model_Tiporecurso extends Modelo
{
    /**
     * @var string Nome da tabela especificada
     */
    protected $stNomeTabela = 'spo.tiporecurso';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'tprano',
        'tprcod',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'tprdsc' => null,
        'tprano' => null,
        'tprcod' => null,
    );

    public static function getQueryCombo()
    {
        $sql = <<<DML
SELECT tprcod AS codigo,
       tprcod || ' - ' || tprdsc AS descricao
  FROM spo.tiporecurso
DML;
        return $sql;
    }
}

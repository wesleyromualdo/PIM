<?php
/**
 * Classe de mapeamento da entidade spo.gruponaturezadespesa.
 *
 * @version $Id: Gruponaturezadespesa.php 108670 2016-02-29 20:01:18Z maykelbraz $
 */

/**
 * Mapeamento da tabela de Grupo de natureza de despesa, também conhecido
 * como Categoria de gastos (SIAFI).
 *
 * @package  Spo\Model
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
class Spo_Model_Gruponaturezadespesa extends Modelo
{
    /**
     * @var string Nome da tabela especificada.
     */
    protected $stNomeTabela = 'spo.gruponaturezadespesa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'gndcod',
        'gndano',
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
        'gndcod' => null,
        'gnddsc' => null,
        'gndano' => null,
    );

    public static function getQueryCombo()
    {
        $sql = <<<DML
SELECT gndcod AS codigo,
       gndcod || ' - ' ||gnddsc AS descricao
  FROM spo.gruponaturezadespesa
DML;
        return $sql;
    }

    public static function getQueryComboDoPublic()
    {
        $sql = <<<DML
SELECT gnd.gndcod AS codigo,
       gnd.gndcod || ' - ' || gnd.gnddsc AS descricao
  FROM public.gnd
  WHERE gnd.gndstatus = 'A'
DML;
        return $sql;
    }
}

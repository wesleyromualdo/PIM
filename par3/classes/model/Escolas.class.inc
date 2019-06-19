<?php
/**
 * Classe de mapeamento da entidade par3.escola.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade par3.escola.
 *
 * @see Modelo
 */
class Par3_Model_Escola extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.escola';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'endid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'endid' => null,
        'endcep' => null,
        'endlogradouro' => null,
        'endcomplemento' => null,
        'endnumero' => null,
        'endbairro' => null,
        'entid' => null,
    );



    public function pegarSQLSelectCombo($inuid)
    {
        $where = $inuid && validarInteiro($inuid) ? "  ins.inuid in({$inuid})" : '';
        return "	  SELECT distinct esc.escid as codigo, esc.escnome as descricao FROM par3.escola esc
                INNER JOIN par3.instrumentounidade ins ON ins.muncod = esc.muncod
                INNER join par3.iniciativa_planejamento_item_composicao_escola esc1 on esc1.escid = esc.escid
                inner join par3.iniciativa_planejamento_item_composicao ipi on ipi.ipiid = esc1.ipiid
                WHERE {$where}";
    }


    public function recuperaEscolasMunicipioParAntigo($inuid_parantigo)
    {
        $sql = "";

        return $arrEscolas;
    }





}//end Class
?>
<?php
/**
 * Classe de mapeamento da entidade par3.endereco.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade par3.endereco.
 *
 * @see Modelo
 */
class Par3_Model_Endereco extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.endereco';

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
        'endmunicipio' => null,
    );


    public function preparaDadosGravacao()
    {
        foreach($this->arAtributos as $attr => $value){
            switch($attr){
            	case 'endcep':
            	    $_POST[$attr] = str_replace(Array('.','-'), '', $_POST[$attr]);
            	    break;
            }
        }
    }

    public function pegaDadosPorEntidade($entid)
    {
        $sql ="select * from {$this->stNomeTabela} where entid = %d";
        return $this->pegaLinha(sprintf($sql,$entid));
    }
}//end Class
?>
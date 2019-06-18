<?php
/**
 * Classe de mapeamento da entidade sac.perfiltipo
 *
 * @category Class
 * @package  A1
 * @author   ADALTO ALEXANDRE DO AMARAL PEREIRA <Adalto_2015@outlook.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-02-2016
 * @link     no link
 */



/**
 * Sac__ModelPerfiltipo
 *
 * @category Class
 * @package  A1
 * @author   ADALTO ALEXANDRE DO AMARAL PEREIRA <Adalto_2015@outlook.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-02-2016
 * @link     no link
 */
class Sac_Model_PerfilTipo extends Modelo
{
    const K_TIPO_SUPORTE = 'S';
    const K_TIPO_USUARIO = 'U';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'sac.perfiltipo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'petid',
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
        'petid' => null,
        'pflcod' => null,
        'pettipo' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'petid' => array(  'Digits'  ),
            'pflcod' => array(  'Digits'  ),
            'pettipo' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
        );
    }//end getCamposValidacao($dados)

    public function recuperarTipoPerfil($perfis)
    {
        $perfis = (array) $perfis;

        if(count($perfis)){
            $sql = "select distinct pettipo
                    from sac.perfiltipo
                    where pflcod in (" . implode(', ', $perfis) . ")";

            $tipos = $this->carregar($sql);
            $tipos = $tipos ? $tipos : array();

            foreach($tipos as $tipo){
                if(in_array(self::K_TIPO_SUPORTE, $tipo)){
                    return self::K_TIPO_SUPORTE;
                }
            }
        }
        return self::K_TIPO_USUARIO;
    }

}//end Class
?>
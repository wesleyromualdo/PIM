<?php
/**
 * Classe de mapeamento da entidade par3.dadosnutricionista
 *
 * @category Class
 * @package  A1
 * @author   LOTAVINO LOGRADO DUNICE <lotavino.dunice@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-12-2015
 * @link     no link
 */



/**
 * Dadosnutricionista
 *
 * @category Class
 * @package  A1
 * @author   LOTAVINO LOGRADO DUNICE <lotavino.dunice@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-12-2015
 * @link     no link
 */
class Par3_Model_DadosNutricionista extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.dadosnutricionista';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'danid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'endid' => array('tabela' => 'par3.endereco', 'pk' => 'endid'),
        'dancpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'danid' => null,
        'dancpf' => null,
        'dandatanascimento' => null,
        'dannomemae' => null,
        'dansexo' => null,
        'dancrn' => null,
        'dancrnuf' => null,
        'dancrnprovisorio' => null,
        'danemailprincipal' => null,
        'danemailalternativo' => null,
        'dantelefonefixo' => null,
        'dantelefonecelular' => null,
        'endid' => null,
        'estuf' =>null,
        'muncod' =>null,
    );


    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'danid' => array( 'allowEmpty' => true, 'Digits'  ),
            'dancpf' => array(  new Zend_Validate_StringLength(array('max' => 11))  ),
            'dandatanascimento' => array(  ),
            'dannomemae' => array(  new Zend_Validate_StringLength(array('max' => 225))  ),
            'dansexo' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'dancrn' => array(  new Zend_Validate_StringLength(array('max' => 13))  ),
            'dancrnuf' => array(  new Zend_Validate_StringLength(array('max' => 2))  ),
            'dancrnprovisorio' => array(  ),
            'danemailprincipal' => array(  new Zend_Validate_StringLength(array('max' => 225))  ),
            'danemailalternativo' => array(  new Zend_Validate_StringLength(array('max' => 225))  ),
            'dantelefonefixo' => array(  new Zend_Validate_StringLength(array('max' => 15))  ),
            'dantelefonecelular' => array(  new Zend_Validate_StringLength(array('max' => 15))  ),
            'endid' => array( 'allowEmpty' => true, 'Digits'  ),
            'estuf' => array(  new Zend_Validate_StringLength(array('max' => 2))  ),
            'muncod' => array(  new Zend_Validate_StringLength(array('max' => 6))  ),
        );
    }//end getCamposValidacao($dados)


    /** carregarDadosPorCPF()
     *
     * Atributos
     * @var     $cpf string
     * @return  $obj objeto
     */
    public function carregarDadosPorCPF($cpf)
    {
        $sql = "SELECT danid
                FROM {$this->stNomeTabela}
                WHERE dancpf = '$cpf'";

        $danid = $this->pegaUm($sql);

        if($danid){
            $this->carregarPorId($danid);
        }
    }//end carregarDadosPorCPF($dados)


    /** preparaDadosGravacao()
     *
     * Atributos
     * @var     $_POST
     * @return  void
     */
    public function preparaDadosGravacao()
    {
        foreach($this->arAtributos as $attr => $value){
            switch($attr){
            	case 'dancpf':
            	case 'dantelefonefixo':
            	case 'dantelefonecelular':
            	    $_POST[$attr] = str_replace(Array('.','-','(',')',' '), '', $_POST[$attr]);
            	    break;
            }
        }
    }

    public function populateData(Seguranca_Model_Usuario $usuario)
    {
        $this->carregarDadosPorCPF($usuario->usucpf);
        $this->arAtributos['danemailprincipal'] = empty($this->arAtributos['danemailprincipal']) ? $usuario->usuemail : $this->arAtributos['danemailprincipal'];
        $this->arAtributos['dansexo'] = empty($this->arAtributos['dansexo']) ? $usuario->ususexo : $this->arAtributos['dansexo'];
        $this->arAtributos['muncod'] = empty($this->arAtributos['muncod']) ? $usuario->muncod : $this->arAtributos['muncod'];
        $this->arAtributos['estuf'] = empty($this->arAtributos['estuf']) ? $usuario->regcod : $this->arAtributos['estuf'];
        $this->arAtributos['dansexo'] = strtolower($this->arAtributos['dansexo']);
    }

}//end Class
?>
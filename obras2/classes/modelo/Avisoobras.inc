<?php
/**
 * Obras2_Model_Avisoobras
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Obras2_Modelo_Avisoobras extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'obras2.avisoobras';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'avsoid',
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
        'avsodescricao' => null,
        'avsoordem' => null,
        'avsoalert' => null,
        'avsovalidade' => null,
        'avsoid' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'avsovalidade' => array( 'allowEmpty' => true  ),
            'avsodescricao' => array( 'allowEmpty' => true  ),
            'avsoordem' => array( 'allowEmpty' => true, 'Digits'  ),
            'avsoalert' => array( 'allowEmpty' => true ),
            'avsoid' => array(  'Digits'  ),
        );
    }//end getCamposValidacao($dados)


    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function gravar()
    {
        global $url;
        $this->popularDadosObjeto();
        $url .= '&avsoid=' . $this->avsoid;

        try{
            $sucesso = $this->salvar();
            $this->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }//end gravar()


   /**
     * Função excluir
     * - deleta os dados
     *
     */
    public function excluirAviso($avsoid)
    {
        global $url;
        $url = 'obras2.php?modulo=principal/editarAviso&acao=A';
        try{
            $this->excluir($avsoid);
            $this->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }//end excluir()
    
    public function ordemExiste($avsoordem){
        $sql = "SELECT avsoordem FROM {$this->stNomeTabela} WHERE avsoordem = {$avsoordem}";
        $avsoordem = $this->pegaUm($sql);
        if($avsoordem){
            return $avsoordem;
        }
    }//end OrdemJaExiste
    
    //function para exibir avisos tela inicioLista.inc
    public function recuperaDados(){
        $sql = "SELECT * FROM {$this->stNomeTabela}  WHERE avsovalidade >= NOW() ORDER BY avsoordem ASC";
        $aviso = $this->carregar($sql);
        if($aviso){
            return $aviso;
        }
    }//end recuperaDados
    //
    //function para exibir avisos tela inicioLista.inc
    public function recuperaAlerta(){
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE avsoalert = 'S' AND  avsovalidade >= NOW()";
        $alerta = $this->carregar($sql);
        if($alerta){
            return $alerta;
        }
    }//end recuperaAlerta



}//end Class
?>
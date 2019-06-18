<?php
include APPRAIZ . "includes/classes/fileSimec.class.inc";
/**
 * Classe de mapeamento da entidade obras2.anexoaviso
 *
 * @category Class
 * @package  A1
 * @author   MARILÚCIA CARDOZO DE QUEIROZ <marilucia.queiroz@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 11-11-2015
 * @link     no link
 */



/**
 * Obras2_Modelo_Anexoaviso
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Obras2_Modelo_Anexoaviso extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'obras2.anexoaviso';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'anxavid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'avsoid' => array('tabela' => 'obras2.avisoobras', 'pk' => 'avsoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'anxavid' => null,
        'anxavdsc' => null,
        'anxavnome' => null,
        'anxavtipo' => null,
        'anxavdata' => null,
        'anxavcpf' => null,
        'anxavstatus' => null,
        'avsoid' => null,
        'arqid' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'anxavid' => array(  'Digits'  ),
            'anxavnome' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 500))  ),
            'anxavdsc' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 500))  ),
            'anxavtipo' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 150))  ),
            'anxavdata' => array( 'allowEmpty' => true ),
            'anxavcpf' => array( 'allowEmpty' => true, 'Digits'  ),
            'anxavstatus' => array( 'allowEmpty' => true ),
            'avsoid' => array(  'Digits'  ),
            'arqid' => array(  'Digits'  ),
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
        $url .= '&anxavid=' . $this->anxavid;

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
     * Função excluir Arquivo selecionado
     * - deleta os dados
     *
     */
    public function deletarFile($arqid){
        $file = new FilesSimec("anexoaviso", array(''), "obras2");
        $testa = $file->setRemoveUpload( $arqid );
//         $sql = "DELETE FROM {$this->stNomeTabela} WHERE arqid = {$arqid}";
//         $this->executar($sql);
//        if($this->commit()){
        if($testa){
            return true;
        }else{
            return false;
        }
     }//end deletarFile()

    public function deletarFilePorAvsoid( $avsoid ){
        $sql = "SELECT arqid FROM {$this->stNomeTabela}  as anxav INNER JOIN obras2.avisoobras avso ON anxav.avsoid = avso.avsoid WHERE anxav.avsoid = {$avsoid}";
        $avsoid = $this->carregar($sql);
        if(is_array($avsoid)){
            foreach( $avsoid as $id){
                $sucesso = $this->deletarFile($id['arqid']);
            }
            return true;
        }else{
            return false;
        }
    }
    
    public function pegaArquivosPorAvsoid($avsoid)
    {
        $sql = "
            SELECT a.*, anxav.anxavid, anxav.anxavdsc FROM {$this->stNomeTabela}  anxav
            JOIN arquivo a ON a.arqid = anxav.arqid
            WHERE anxav.anxavstatus = 'A' AND avsoid = $avsoid
        ";
        return $this->carregar($sql);
    }
    
    public function pegaArquivosPorOrdem($ordem)
    {
        $sql = "
            SELECT a.*, anxav.anxavid, anxav.anxavdsc FROM {$this->stNomeTabela}  anxav
            JOIN arquivo a ON a.arqid = anxav.arqid
            JOIN obras2.avisoobras avso ON avso.avsoid = anxav.avsoid
            WHERE anxav.anxavstatus = 'A' AND avsoordem = $ordem
        ";
        return $this->carregar($sql);
    }
    
    public function buscaDadosPorArqid( $arqid ){
            $sql = "SELECT
                            *
                    FROM
                            {$this->stNomeTabela}
                    WHERE
                            anxavstatus = 'A' 
                    AND
                            arqid = {$arqid}";
            $dado = $this->carregar( $sql );

            return ($dado ? $dado : array());
    }
    
    public function updateDescricaoFile($anxavdsc, $arqid){
        $sql = "UPDATE  {$this->stNomeTabela} SET anxavdsc = '{$anxavdsc}' WHERE arqid = ".$arqid;
        $retorno = $this->executar($sql);
        return $retorno;
    }

}//end Class
?>
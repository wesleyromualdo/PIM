<?php
/**
 * Classe de mapeamento da entidade par3.tipodocumento.
 *
 * @version $Id$
 * @since 2018.06.26
 */

/**
 * Par3_Model_HabilitacaoTipoDocumento: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Thiago Tasca Barbosa <thiago.barbosa@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_HabilitacaoTipoDocumento($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_TipoDocumento($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 */
class Par3_Model_HabilitacaoDocumentoObrigatorio extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'habilita.documentoobrigatorio';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'dcoid'
    );


    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'tdcid'  => array('tabela' => 'habilita.tipodocumento', 'pk' => 'tdcid'),
        'cteid'  => array('tabela' => 'habilita.categoriaentidades', 'pk' => 'cteid'),
        'dcocpfalteracao' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf')
   );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'dcoid' => null,
        'cteid' => null,
        'tdcid' => null,
        'dcoobrigatorio' => null,
        'dcohabilitacao' => null,
        'dcostatus' => null,
        'dcocpfalteracao' => null,
        'dcodataalteracao' => null
    );

    /**
     * Validators.Par3_Model_HabilitacaoTipoDocumento
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
//    public function getCamposValidacao($dados = array())
//    {
//        return array(
//            'tpeid' => array('Digits'),
//            'tpenome' => array('Digits'),
//            'tpehabilitacao' => array('Digits'), //array('allowEmpty' => true),
//            'tpestatus' => array('Digits'),
//        );
//    }
    

    public function sqlRecuperarDocTipoObrigatorio($id = null)
    {  
        $sqlCategoriaEntidade = "SELECT  ce.cteid, ce.ctenome,
                                    td.tdcid, td.tdcnome,
                                    dco.dcoid, dco.cteid, dco.dcoobrigatorio, dco.dcohabilitacao
                                FROM habilita.tipodocumento td  
                                INNER JOIN habilita.documentoobrigatorio dco ON dco.tdcid = td.tdcid AND dco.dcostatus = 'A'
                                INNER JOIN habilita.categoriaentidades ce ON ce.cteid = dco.cteid AND ce.ctestatus = 'A' AND ce.cteid = ".$id."
                                WHERE td.tdcstatus = 'A' ";

                                // -- INNER JOIN habilita.naturezajuridica nj ON nj.ntjid = dco.ntjid AND nj.ntjstatus = 'A' AND nj.ntjid = ".$id."

        return $sqlCategoriaEntidade;
    }

    public function sqllistarTiposDocumentos()
    {
        $sql = "SELECT  td.tdcid, td.tdcnome    
                                FROM habilita.tipodocumento td  
                                WHERE td.tdcstatus = 'A' ";

        return $sql;
    }

    public function inativarPorCategoriaEntidade($id = null)
    {
        $sql = "UPDATE habilita.documentoobrigatorio
                SET dcostatus = 'I'
                WHERE cteid = ".$id;
        if($this->executar( $sql )){
            return true;
        }else{
            return false;
        }

    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {


        return parent::antesSalvar();
    }

    public function recuperarPorTipoDocumento($dcoid)
    {
        return $this->recuperarTodos('*',array("dcoid = {$dcoid}"));
    }
    
    
    public function pegarSelectCombo()
    {
        $sql = "SELECT
                  tdcid as codigo,
                  tdcnome as descricao
                FROM {$this->stNomeTabela}
                WHERE
                tdcstatus = 'A'";
                
        return $sql;
        
    }
    
    public function pegarDocumentosPorTipo($cteid){
        
        $sql = "SELECT
                    tdc.tdcid,
                    tdc.tdcnome,
                    tdc.tdcvigencia,
                    tdc.tdcdica,
                    dco.dcohabilitacao,
                    dco.dcoobrigatorio
                FROM
                    {$this->stNomeTabela} dco
                INNER JOIN habilita.tipodocumento tdc ON tdc.tdcid = dco.tdcid AND tdc.tdcstatus = 'A'
                WHERE
                    dco.dcostatus = 'A' AND
                    dco.cteid = {$cteid}";
                    
        return $this->carregar($sql);
    }
}

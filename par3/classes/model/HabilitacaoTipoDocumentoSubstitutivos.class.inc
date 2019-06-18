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
class Par3_Model_HabilitacaoTipoDocumentoSubstitutivos extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'habilita.documentosubstitutos';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'dcsid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
   protected $arChaveEstrangeira = array(
        'tdcid'  => array('tabela' => 'habilita.tipodocumento', 'pk' => 'tdcid'),
        'dcstdcidvinculado' => array('tabela' => 'habilita.tipodocumento', 'pk' => 'tdcid'),
        'dcscpfalteracao' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf')
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'dcsid' => null,
        'tdcid' => null,
        'dcstdcidvinculado' => null,
        'dcsstatus' => null,
        'dcscpfalteracao' => null,
        'dcsdataalteracao' => null
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
    
    public function montarSQLSimples($idDocumento)
    {

        $sql = "SELECT
                dcsid AS id,
                tdcid AS documentoprincipal,
                dcstdcidvinculado AS documentovinculado
                FROM {$this->stNomeTabela}
                WHERE dcsstatus = 'A' AND dcsid = $idDocumento
                ";
//                 ver($sql);
        return $sql;
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
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function recuperarPorTipoDocumento($tdcid)
    {
        if($tdcid){
            return $this->recuperarTodos('*',array("tdcid = {$tdcid}"));
        }else{
            return false;
        }
    
    }

    public function inativarPorTipoDocumento($id)
    {
        //{$this->stNomeTabela}
        $sql = "UPDATE habilita.documentosubstitutos 
                SET dcsstatus = 'I'
                WHERE tdcid = ".$id;
                //ver($sql); die();
        if($this->executar( $sql )){
            $this->commit();
            return true;
        }else{
            return false;
        }

    }

    public function getTipoDocumentosVinculados($tdcid){
        
        $sql = "SELECT 
                   (SELECT array(
                      SELECT DISTINCT
                         ds.dcstdcidvinculado as tipo
                      FROM
                         {$this->stNomeTabela} ds  
                      WHERE
                         ds.dcsstatus = 'A' AND
                         ds.tdcid = td.tdcid)
                      ) as tipos
                FROM
                   habilita.tipodocumento td                     
                WHERE
                   td.tdcid = {$tdcid} AND
                   td.tdcstatus = 'A'";
                   
        return $this->recuperar($sql);
        
    }

    /*
    public function recuperaTipoDocumentosVinculados($tdcid)
    {
        $sql = "SELECT
                  dcsid as codigo,
                  tdcid as descricao
                FROM {$this->stNomeTabela}
                WHERE
                dcsstatus = 'A'";
                
        return $sql;
        
    }
    */
    
    
    public function pegarSelectCombo()
    {
        $sql = "SELECT
                  dcsid as codigo,
                  tdcid as descricao
                FROM {$this->stNomeTabela}
                WHERE
                dcsstatus = 'A'";
                
        return $sql;
        
    }
}

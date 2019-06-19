<?php
/**
 * Classe de mapeamento da entidade par3.naturezajuridica.
 *
 * @version $Id$
 * @since 2018.09.22
 */

/**
 * Par3_Model_HabilitacaoNaturezaJuridicaRelacionada: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Victor de Mello Benzi <victorbenzi@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_HabilitacaoNaturezaJuridicaRelacionada($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_HabilitacaoNaturezaJuridicaRelacionada($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 */
class Par3_Model_HabilitacaoNaturezaJuridicaRelacionada extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'habilita.naturezajuridicarelacionada';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'njrid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    //protected $arChaveEstrangeira = array(
    //    'tnjid'  => array('tabela' => 'habilita.naturezajuridicarelacionada', 'pk' => 'njrid')
    //);

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'njrid' => null,
        'ntjid' => null,
        'tnjid' => null,
        'njrstatus' => null
    );

    /**
     * Validators.Par3_Model_HabilitacaoNaturezaJuridicaRelacionada
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

    public function salvarRelacao($ntjid, $arrtnjid)
    {
        $this->inativarRelacao($ntjid);
        foreach ($arrtnjid as $tnjid){
            $sql = "INSERT INTO {$this->stNomeTabela}(ntjid, tnjid, njrstatus) VALUES ($ntjid, $tnjid, 'A')";
            $this->executar($sql);
            $this->commit();
        }
    }
    
    public function inativarRelacao($ntjid){
        $sql = "UPDATE {$this->stNomeTabela} SET njrstatus = 'I' WHERE ntjid = {$ntjid}";
        $this->executar($sql);
        $this->commit();
    }
    
    public function getTipoNaturezasRelacionadas($ntjid){
        
        $sql = "SELECT 
                   (SELECT array(
                      SELECT DISTINCT
                         rel.tnjid as tipo
                      FROM
                         {$this->stNomeTabela} rel  
                      WHERE
                         rel.njrstatus = 'A' AND
                         ntj.ntjid = rel.ntjid)
                      ) as tipos
                FROM
                   habilita.naturezajuridica ntj                     
                WHERE
                   ntj.ntjid = {$ntjid} AND
                   ntj.ntjstatus = 'A'";
                   
        return $this->recuperar($sql);
        
    }
}

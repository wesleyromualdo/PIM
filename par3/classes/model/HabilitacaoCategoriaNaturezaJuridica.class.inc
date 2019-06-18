<?php
/**
 * Classe de mapeamento da entidade par3.categorianaturezajuridica.
 *
 * @version $Id$
 * @since 2018.11.08
 */

/**
 * Par3_Model_HabilitacaoCategoriaNaturezaJuridica: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Victor de Mello Benzi <victorbenzi@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_HabilitacaoCategoriaNaturezaJuridica($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_HabilitacaoCategoriaNaturezaJuridica($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 */
class Par3_Model_HabilitacaoCategoriaNaturezaJuridica extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'habilita.categorianaturezajuridica';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'cnjid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'cteid'  => array('tabela' => 'habilita.categoriaentidades', 'pk' => 'cteid'),
        'ntjid'  => array('tabela' => 'habilita.naturezajuridica', 'pk' => 'ntjid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'cnjid' => null,
        'cteid' => null,
        'ntjid' => null,
        'cnjstatus' => null
    );

    /**
     * Validators.Par3_Model_HabilitacaoCategoriaNaturezaJuridica
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

    public function salvarRelacao($cteid, $arrntjid)
    {
        $this->inativarRelacao($cteid);
        if(is_array($arrntjid)){
            foreach ($arrntjid as $ntjid){
                $sql = "INSERT INTO {$this->stNomeTabela} (cteid, ntjid, cnjstatus) VALUES ($cteid, $ntjid, 'A')";
                $this->executar($sql);
                $this->commit();
            }
        }
    }
    
    public function inativarRelacao($cteid){
        $sql = "UPDATE {$this->stNomeTabela} SET cnjstatus = 'I' WHERE cteid = {$cteid}";
        $this->executar($sql);
        $this->commit();
    }
    
    public function getNaturezasRelacionadas($cteid){
        
        $sql = "SELECT 
                   (SELECT array(
                      SELECT DISTINCT
                         cnj.ntjid as tipo
                      FROM
                         {$this->stNomeTabela} cnj  
                      WHERE
                         cnj.cnjstatus = 'A' AND
                         cte.cteid = cnj.cteid)
                      ) as naturezas
                FROM
                   habilita.categoriaentidades cte
                WHERE
                   cte.cteid = {$cteid} AND
                   cte.ctestatus = 'A'";
                   
        return $this->recuperar($sql);
        
    }
}

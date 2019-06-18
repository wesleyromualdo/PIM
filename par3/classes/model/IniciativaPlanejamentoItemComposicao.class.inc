<?php
/**
 * Classe de mapeamento da entidade par3.Iniciativa_planejamentoItemComposicao.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamentoItemComposicao: Tabela utilizada para cadastro dos Itens de composição das Iniciativas
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamentoItemComposicao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_item_composicao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ipiid Chave Primária - default: nextval('par3.iniciativa_planejamento_item_composicao_ipiid_seq'::regclass)
 * @property int $inpid FK iniciativa_planejamento
 * @property numeric $ipivalorreferencia valor de referência do item no momendo do primeiro cadastro
 * @property int $ipiquantidade Quantidade de itens
 * @property int $ipiano Ano de referência
 * @property int $iigid FK fk_itenscomposicao_grupos_iniciativa
 * @property string $ipistatus status A- ativo, I - inativo - default: 'A'::bpchar
 */
class Par3_Model_IniciativaPlanejamentoItemComposicao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.Iniciativa_planejamento_item_composicao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ipiid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iigid' => array('tabela' => 'par3.iniciativa_itenscomposicao_grupo', 'pk' => 'iigid'),
        'inpid' => array('tabela' => 'par3.iniciativa_planejamento', 'pk' => 'inpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ipiid' => null,
        'inpid' => null,
        'ipivalorreferencia' => null,
        'ipiquantidade' => null,
        'ipiano' => null,
        'iigid' => null,
        'ipistatus' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'ipiid' => array('Digits'),
            'inpid' => array('Digits'),
            'ipivalorreferencia' => array('allowEmpty' => true),
            'ipiquantidade' => array('allowEmpty' => true, 'Digits'),
            'ipiano' => array('allowEmpty' => true, 'Digits'),
            'iigid' => array('allowEmpty' => true, 'Digits'),
            'ipistatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
        );
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



    public function atualizaValorPlanejadoPlanejamentoIniciativa($inpid)
    {
        if($inpid){
            $sql = "SELECT par3.atualiza_valor_planejado_planejamento_itemcomposicao({$inpid});";
            $this->executar($sql);
            $this->commit();
        }
    }

    public function dadosComboItens($inpid)
    {
        $sql = "SELECT iicg.iigid AS codigo, ig.igrnome as descricao FROM par3.iniciativa_planejamento 
                JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                JOIN par3.itenscomposicao_grupos ig USING (igrid)
                WHERE inpid = {$inpid} AND ig.igrstatus = 'A' AND ig.igrsituacao = 'A' /*AND iicg.iigsituacao = 'A'*/
                UNION ALL
                SELECT iicg.iigid, ic.itcdsc FROM par3.iniciativa_planejamento 
                JOIN par3.instrumentounidade inu USING (inuid)
                JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                JOIN par3.itenscomposicao ic USING (itcid)
                JOIN par3.itenscomposicao_detalhamento id USING (itcid)
                JOIN par3.itenscomposicao_detalhamento_estado ide USING (itdid)
                WHERE inpid = {$inpid} AND ic.itcstatus = 'A' AND ic.itcsituacao = 'A' /*AND iicg.iigsituacao = 'A'*/
                AND ide.estuf = inu.estuf";
        return $sql;
    }

    public function dadosComboKitProinfancia($inpid){
        $sql = "SELECT iicg.iigid AS codigo, ig.igrnome as descricao 
                 FROM par3.iniciativa_planejamento inp
                  INNER JOIN obras2.obras o ON o.obrid = inp.obrid
                  INNER JOIN par3.obra_tipo ot ON ot.tpoid = o.tpoid
                  INNER JOIN par3.itenscomposicao_grupos ig ON ig.igrid = ot.igrid
                  INNER JOIN par3.iniciativa_itenscomposicao_grupo iicg ON iicg.igrid = ig.igrid
                WHERE inpid = {$inpid} 
                  AND ig.igrstatus = 'A' AND ig.igrsituacao = 'A' /*AND iicg.iigsituacao = 'A'*/ AND iicg.itcid IS NULL";
        return $sql;
    }

    public function anosPlanejamento($inpid)
    {
        $sql = "SELECT iniano
                FROM par3.iniciativa_planejamento ip
                JOIN par3.iniciativa_iniciativas_anos iia USING (iniid) 
                WHERE ip.inpid = {$inpid} 
                AND iia.iianostatus = 'A'
                ORDER BY iniano;";
        return $this->carregar($sql);
    }

    public function getItensCadastrados($inpid)
    {
        $sql = "select iigid from par3.iniciativa_planejamento_item_composicao where inpid = {$inpid} and ipistatus = 'A' group by iigid;";
        return $this->carregar($sql);
    }

    public function getIdIniciativaPlanejamentoItemComposicao($dados)
    {
        if($dados['vlreferencia'] == '') {
            return false;
        }

        $arConsulta[] = 'inpid = ' . $dados['inpid'];
        $arConsulta[] = 'iigid = ' . $dados['iigid'];
        $arConsulta[] = 'ipiano = ' . $dados['iniano'];
        $arConsulta[] = "ipistatus = 'A'";
        $arIpic = $this->recuperarTodos('ipiid', $arConsulta);
        
        if (!$arIpic[0]['ipiid']) {
            $arInsert['inpid']     = $dados['inpid'];
            $arInsert['iigid']     = $dados['iigid'];
            $arInsert['ipiano']    = $dados['iniano'];
            $arInsert['ipistatus'] = 'A';
            $arInsert['ipivalorreferencia'] = $dados['vlreferencia']?$dados['vlreferencia']:0;
            $this->popularDadosObjeto($arInsert);
            $ipiid = $this->salvar();
            $this->commit();
//             $dadosHistoricos = new Par3_Model_IniciativaPlanejamentoItemComposicao($ipiid);
//             $historico = new Par3_Model_IniciativaPlanejamentoItemComposicaoHistorico();
//             $historico->gravarHistorico($dadosHistoricos,Par3_Model_IniciativaPlanejamentoItemComposicaoHistorico::CREATE );
        } else {
            $ipiid = $arIpic[0]['ipiid'];
        }
        return $ipiid;
    }

    public function recuperarItensKitProinfancia($iigid){
        $sql = "SELECT iigid, itcid 
                FROM par3.iniciativa_itenscomposicao_grupo 
                WHERE itcid IS NOT NULL AND igrid = (SELECT igrid 
                                                     FROM par3.iniciativa_itenscomposicao_grupo 
                                                     WHERE iigid = {$iigid})";
        return $this->carregar($sql);
    }

    public function recuperaQtdItemProinfancia($dados){
        $sql = "SELECT gitqtd, itdvalor 
                FROM par3.itenscomposicao_grupos_itens git
                INNER JOIN par3.itenscomposicao_detalhamento itd ON itd.itcid = git.itcid
                WHERE igrid = {$dados['igrid']} AND itd.itcid = {$dados['itcid']}";
        return $this->pegaLinha($sql);
    }
}

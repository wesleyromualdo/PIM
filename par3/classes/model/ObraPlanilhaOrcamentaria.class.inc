<?php
/**
 * Classe de mapeamento da entidade par3.obra_planilha_orcamentaria.
 *
 * @version $Id$
 * @since 2017.07.07
 */

/**
 * Par3_Model_Obra_planilha_orcamentaria: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_planilha_orcamentaria($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_planilha_orcamentaria($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $opoid  - default: nextval('par3.obra_planilha_orcamentaria_opoid_seq'::regclass)
 * @property int $obrid
 * @property int $oivid
 * @property numeric $opoqtd
 * @property numeric $opovalor
 * @property string $opostatus
 */
class Par3_Model_ObraPlanilhaOrcamentaria extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_planilha_orcamentaria';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'opoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'oivid' => array('tabela' => 'par3.obra_tipo_item_planilha_valores', 'pk' => 'oivid'),
        'obrid' => array('tabela' => 'par3.obra', 'pk' => 'obrid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'opoid' => null,
        'obrid' => null,
        'oivid' => null,
        'opoqtd' => null,
        'opovalor' => null,
        'opostatus' => null,
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
            'opoid' => array('Digits'),
            'obrid' => array('allowEmpty' => true, 'Digits'),
            'oivid' => array('allowEmpty' => true, 'Digits'),
            'opoqtd' => array('allowEmpty' => true),
            'opovalor' => array('allowEmpty' => true),
            'opostatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
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

    public function carregaValoresIniciais($dados)
    {

        $dados['tplid'] = $this->pegaUm("SELECT tplid FROM par3.obra WHERE obrid = {$dados['obrid']} ");

        $sql = "INSERT INTO par3.obra_planilha_orcamentaria(obrid, oivid, opoqtd, opovalor, opostatus)
                SELECT {$dados['obrid']} as obrid, otvar.oivid, otvar.oivqtd, otvar.oivvalor, 'A' as opostatus
                FROM par3.obra_tipo_versao_planilha otvp
                JOIN par3.obra_tipo_item_planilha otip USING (tplid)
                JOIN par3.obra_tipo_item_planilha_valores otvar ON (otvar.oipid = otip.oipid AND ((otvp.tpltipo_regiao = 'E' AND (otvar.inuid = '{$dados['inuid']}' or otvar.inuid in (select inuid from par3.instrumentounidade where estuf = (select estuf from par3.instrumentounidade where inuid =  '{$dados['inuid']}') and itrid <> 2)) OR otvp.tpltipo_regiao = 'N')))
                WHERE otvar.oivid not in (SELECT oivid FROM par3.obra_planilha_orcamentaria WHERE obrid = {$dados['obrid']}) AND otvp.tplid = {$dados['tplid']};";
        $this->executar($sql);
        $this->commit();
    }

    public function carregaValoresLista($dados)
    {
        $dados['tplid'] = $this->pegaUm("SELECT tplid FROM par3.obra WHERE obrid = {$dados['obrid']} ");

        $sql = "WITH RECURSIVE tabela_recursiva AS
                  ( SELECT oipid,
                           oipid_pai,
                           oipdsc,
                           oipordem,
                           oipcodigo,
                           1 AS nivel,
                           oipid as paigeral,
                           uniid,
                           oippossui_valor
                   FROM par3.obra_tipo_item_planilha
                   WHERE oipid_pai IS NULL
                     AND tplid = {$dados['tplid']}  AND oipstatus = 'A'
                     UNION ALL
                     SELECT t.oipid,
                            t.oipid_pai,
                            t.oipdsc,
                            t.oipordem,
                            t.oipcodigo,
                            nivel+1,
                            paigeral,
                            t.uniid,
                            t.oippossui_valor
                     FROM par3.obra_tipo_item_planilha t
                     JOIN tabela_recursiva p ON t.oipid_pai = p.oipid AND t.tplid = {$dados['tplid']} AND t.oipstatus = 'A')
                SELECT paigeral,
                       otvar.oivid,
                       tr.oipid,
                       tr.oipid_pai,
                       tr.oipdsc,
                       tr.oipordem,
                       tr.oipcodigo,
                       tr.nivel,
                       otvar.oivqtd_editavel,
                       otvar.oivvalor_editavel,
                       opo.opoqtd,
                       opo.opovalor,
                       opo.opoid,
                       unm.unidsc,
                       ROUND((opo.opoqtd * opo.opovalor),2) as total
                FROM tabela_recursiva tr
                JOIN par3.obra_tipo_versao_planilha otvp ON (otvp.tplid = {$dados['tplid']})
                LEFT JOIN par3.obra_tipo_item_planilha_valores otvar ON (otvar.oipid = tr.oipid)
                LEFT JOIN par3.obra_planilha_orcamentaria opo on (opo.oivid = otvar.oivid AND opo.obrid = {$dados['obrid']} AND opo.opostatus = 'A')
                LEFT JOIN par3.unidade_medida unm USING(uniid)
                WHERE ((otvp.tpltipo_regiao = 'E' AND (otvar.inuid = '{$dados['inuid']}' or otvar.inuid in (select inuid from par3.instrumentounidade where estuf = (select estuf from par3.instrumentounidade where inuid =  '{$dados['inuid']}') and itrid <> 2) OR oippossui_valor = 'f')) OR otvp.tpltipo_regiao = 'N')
                ORDER BY tr.oipcodigo;";
        return $this->carregar($sql);
    }


    public function carregaTotalLista($dados){

        global $db;
        $dados['tplid'] = $this->pegaUm("SELECT tplid FROM par3.obra WHERE obrid = {$dados['obrid']} ");
        $result = pg_fetch_array(($db->executar("WITH RECURSIVE tabela_recursiva AS
                  ( SELECT oipid,
                           oipid_pai,
                           oipdsc,
                           oipordem,
                           oipcodigo,
                           1 AS nivel,
                           oipid as paigeral,
                           uniid,
                           oippossui_valor
                   FROM par3.obra_tipo_item_planilha
                   WHERE oipid_pai IS NULL
                     AND tplid = {$dados['tplid']}  AND oipstatus = 'A'
                     UNION ALL
                     SELECT t.oipid,
                            t.oipid_pai,
                            t.oipdsc,
                            t.oipordem,
                            t.oipcodigo,
                            nivel+1,
                            paigeral,
                            t.uniid,
                            t.oippossui_valor
                     FROM par3.obra_tipo_item_planilha t
                     JOIN tabela_recursiva p ON t.oipid_pai = p.oipid AND t.tplid = {$dados['tplid']} AND t.oipstatus = 'A')
                SELECT 
                        SUM(ROUND((opo.opoqtd * opo.opovalor),2)) as total
                FROM tabela_recursiva tr
                JOIN par3.obra_tipo_versao_planilha otvp ON (otvp.tplid = {$dados['tplid']})
                LEFT JOIN par3.obra_tipo_item_planilha_valores otvar ON (otvar.oipid = tr.oipid)
                LEFT JOIN par3.obra_planilha_orcamentaria opo on (opo.oivid = otvar.oivid AND opo.obrid = {$dados['obrid']} AND opo.opostatus = 'A')
                LEFT JOIN par3.unidade_medida unm USING(uniid)
                WHERE ((otvp.tpltipo_regiao = 'E' AND (otvar.inuid = '{$dados['inuid']}' or otvar.inuid in (select inuid from par3.instrumentounidade where estuf = (select estuf from par3.instrumentounidade where inuid =  '{$dados['inuid']}') and itrid <> 2) OR oippossui_valor = 'f')) OR otvp.tpltipo_regiao = 'N')")));
        return $result[0];

    }

}

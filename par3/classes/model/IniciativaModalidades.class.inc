<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_modalidades.
 *
 * @version $Id$
 * @since 2017.06.27
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_IniciativaModalidades: Tabela utilizada para cadastro da Iniciativa Detalhe - Modalidades (3° Aba).
Story #11025
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Philipe Allan Almeida <philipe.almeida@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaModalidades($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaModalidades($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $imoid Chave Primária - default: nextval('par3.iniciativa_modalidades_imoid_seq'::regclass)
 * @property int $iniid FK da Iniciativa (par3.iniciativa)
 * @property int $nivid FK da Nível (par3.ensino_nivel)
 * @property int $etaid FK da Etapa (par3.ensino_etapa)
 * @property int $modid FK da Modalidade (par3.modalidade)
 * @property int $desid FK da Desdobramento (par3.iniciativa_desdobramento)
 */
class Par3_Model_IniciativaModalidades extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_modalidades';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'imoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'modid' => array('tabela' => 'par3.modalidade', 'pk' => 'modid'),
        'desid' => array('tabela' => 'par3.iniciativa_desdobramento', 'pk' => 'desid'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
        'nivid' => array('tabela' => 'par3.ensino_nivel', 'pk' => 'nivid'),
        'etaid' => array('tabela' => 'par3.ensino_etapa', 'pk' => 'etaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'imoid' => null,
        'iniid' => null,
        'nivid' => null,
        'etaid' => null,
        'modid' => null,
        'desid' => null,
        'imostatus' => null,
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
            'imoid' => array('Digits'),
            'iniid' => array('Digits'),
            'nivid' => array('Digits'),
            'etaid' => array('Digits'),
            'modid' => array('Digits'),
            'desid' => array('Digits'),
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

    public function recuperarPorIniciativa($iniid)
    {
        return $this->recuperarTodos('*',array("iniid = {$iniid} AND imostatus = 'A'"));
    }

    public function recuperarPorIniciativaModalidade($arrPost)
    {
        return $this->recuperarTodos('*',array("iniid = {$arrPost['iniid']} AND modid = {$arrPost['modid']} AND imostatus = 'A'"));
    }

    public function recuperarIniciativaModalidade($arrPost)
    {
        $where  = $arrPost['iniid']? "AND iniid = {$arrPost['iniid']} ":'';
        $where .= $arrPost['nivid']? "AND nivid = {$arrPost['nivid']} ":'';
        $where .= $arrPost['etaid']? "AND etaid = {$arrPost['etaid']} ":'';
        $where .= $arrPost['desid']? "AND desid = {$arrPost['desid']} ":'';
        $where .= $arrPost['modid']? "AND modid = {$arrPost['modid']} ":'';

        $sql = "SELECT * 
                FROM {$this->stNomeTabela}
                WHERE imostatus = 'A'
                {$where}
        ";
        $arrImod = $this->pegaUm($sql);//retorna imoid
        return $arrImod;
    }
}

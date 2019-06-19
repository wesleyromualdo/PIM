<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_estados.
 *
 * @version $Id$
 * @since 2017.07.04
 */

/**
 * Par3_Model_IniciativaIniciativasEstados: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasEstados($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasEstados($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iiestufid Chave Primária - default: nextval('par3.iniciativa_iniciativas_estados_iiestufid_seq'::regclass)
 * @property "char" $iiestufstatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_estados_iniid_seq'::regclass)
 * @property string $estuf FK do código do estado (territorio.estado)
 */
class Par3_Model_IniciativaIniciativasEstados extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_estados';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iiestufid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iiestufid' => null,
        'iiestufstatus' => null,
        'iniid' => null,
        'estuf' => null,
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
            'iiestufid' => array('Digits'),
            'iiestufstatus' => array('allowEmpty' => true),
            'iniid' => array('Digits'),
            'estuf' => array(new Zend_Validate_StringLength(array('max' => 2))),
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

    public function recuperarIniciativaEstado($arrPost = array())
    {
        $where  = $arrPost['iniid']?" AND iniid = {$arrPost['iniid']}":'';
        $where .= $arrPost['iiestufid']?"  AND iiestufid = {$arrPost['iiestufid']}":'';
        $where .= $arrPost['estuf']?"  AND estuf IN ({$arrPost['estuf']})":'';

        return "
                SELECT  * FROM {$this->stNomeTabela} 
                WHERE iiestufstatus = 'A'
                {$where}
                ";
    }

    public function recuperarIniciativaEstadoSelect($arrPost)
    {
        $sql = $this->recuperarIniciativaEstado($arrPost);
        $resultado = $this->carregar($sql);
        return $resultado ? simec_preparar_array($resultado) : array();

    }

    /**
     * @description <esta função foi criada pois a função "recuperarIniciativaEstadoSelect" mesmo com um sql correto está devolvendo o array vazio>
     * @param $arrPost
     * @return array
     */
    public function recuperarDadosIniciativaEstadoSelect($arrPost)
    {
        $sql = $this->recuperarIniciativaEstado($arrPost);
        $resultado = $this->carregar($sql);
        return $resultado;
    }

    public function getListaEstadosByIniciativa($iniid){
        $sql = "SELECT *
                        FROM par3.iniciativa_iniciativas_estados as iie
                        INNER JOIN territorios.estado as estado ON iie.estuf = estado.estuf
                        INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iie.iniid
                        WHERE iie.iniid = {$iniid}";

        $result = $this->carregar($sql);
        return $result;
    }

}

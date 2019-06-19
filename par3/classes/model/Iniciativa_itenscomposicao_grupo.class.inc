<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_itenscomposicao_grupo.
 *
 * @version $Id$
 * @since 2017.07.03
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Iniciativa_itenscomposicao_grupo: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_itenscomposicao_grupo($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_itenscomposicao_grupo($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iigid  - default: nextval('par3.iniciativa_itenscomposicao_grupo_iigid_seq'::regclass)
 * @property int $iniid
 * @property int $itcid
 * @property int $igrid
 */
class Par3_Model_Iniciativa_itenscomposicao_grupo extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_itenscomposicao_grupo';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iigid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'igrid' => array('tabela' => 'par3.itenscomposicao_grupos', 'pk' => 'igrid'),
        'itcid' => array('tabela' => 'par3.itenscomposicao', 'pk' => 'itcid'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iigid' => null,
        'iniid' => null,
        'itcid' => null,
        'igrid' => null,
        'iigsituacao' => null,
        'iigdtinativacao' => null,
        'iigcpfinativacao' => null
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
            'iigid' => array('Digits'),
            'iniid' => array('Digits'),
            'itcid' => array('allowEmpty' => true,'Digits'),
            'igrid' => array('allowEmpty' => true,'Digits'),
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

    public function salvarTiposObra($arrPost, $iniid)
    {
        $sql = "DELETE FROM par3.iniciativa_tipo_obra WHERE iniid = $iniid AND otpid = {$arrPost['otpid']};
                INSERT INTO par3.iniciativa_tipo_obra(iniid, otpid) VALUES($iniid, {$arrPost['otpid']});";

        $this->executar($sql);
        $this->commit();
    }

    public function excluirTipoObra($arrPost)
    {
        $sql = "DELETE FROM par3.iniciativa_tipo_obra WHERE iniid = {$arrPost['iniid']} AND otpid = {$arrPost['otpid']};";

        $this->executar($sql);
        $this->commit();
    }

    public function buscaCategoria($iniid)
    {
        $sql = "select distinct
                	oct.octid
                from
                	par3.obra_tipo otp
                inner join par3.obra_categoria oct on oct.octid = otp.octid
                inner join par3.iniciativa_tipo_obra ini on ini.otpid = otp.otpid
                where
                	ini.iniid = $iniid";

        return $this->pegaUm($sql);
    }

    /**
     * Para kits do proinfância, que deverão gerar planejamento a partir de obra do obras2.
     * #pseudo-gambiarra: Ao adicionar grupo de itens, deve-se adicionar também todos os itens do grupo, para que os itens de cada kit possa ser mostrado no planejamento.
     * @param $arrIniciativa
     * @return bool
     */
    public function salvarItensKitProinfancia($arrIniciativa){
        $insertSelect = "INSERT INTO par3.iniciativa_itenscomposicao_grupo (iniid, itcid, igrid)
                         SELECT {$arrIniciativa['iniid']}, itcid, {$arrIniciativa['igrid']}
                         FROM par3.itenscomposicao_grupos_itens WHERE igrid = {$arrIniciativa['igrid']}";
        $this->executar($insertSelect);
        if($this->commit()){
            return true;
        }
        return false;
    }
}

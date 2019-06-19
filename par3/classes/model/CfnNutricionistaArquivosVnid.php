<?php
/**
 * Classe de mapeamento da entidade par3.cfn_nutricionista_arquivos_vnid.
 *
 * @version $Id$
 * @since 2018.05.29
 */

/**
 * Par3_Model_Cfn_nutricionista_arquivos_vnid: Tabela utilizada para registrar todos os nutricionistas que foram
 * atualizados pela carga
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Cfn_nutricionista_arquivos_vnid($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Cfn_nutricionista_arquivos_vnid($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int     $vnid
 * @property int     $cfnid
 * @property int     $inuid
 * @property string  $cfvdscmotivo
 * @property boolean $cfvprocessado
 */
class Par3_Model_CfnNutricionistaArquivosVnid extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.cfn_nutricionista_arquivos_vnid';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        null
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'vnid'  => array('tabela'  => 'par3.vinculacaonutricionista',   'pk' => 'vnid'),
        'cfnid' => array('tabela' => 'par3.cfn_nutricionista_arquivos', 'pk' => 'cfnid'),
        'inuid' => array('tabela' => 'par3.instrumentounidade',         'pk' => 'inuid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
                'vnid'          => null,
        'cfnid'         => null,
        'inuid'         => null,
        'cfvdscmotivo'  => null,
        'cfvprocessado' => null
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
        'vnid'  => array('Digits'),
        'cfnid' => array('Digits'),
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

    public function salvarNutricionistasAtualizados($cfnid, $arrVnid)
    {
        $arrVnid = array_filter($arrVnid);
        if (count($arrVnid) == 0) {
            return false;
        }
        foreach ($arrVnid as $vnid) {
            $dados = [$cfnid,$vnid['vnid'],$vnid['inuid'],addslashes($vnid['cfvdscmotivo']),$vnid['cfvprocessado']];
            $sql = "INSERT INTO {$this->stNomeTabela} values (%d,%d,%d,'%s',%s)";
            $this->executar(vsprintf($sql, $dados));
            $this->commit();
        }
    }
}

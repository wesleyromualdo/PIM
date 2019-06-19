<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_areasrelacionadas_programas_estadodocumento.
 *
 * @version $Id$
 * @since 2017.06.28
 */

/**
 * Par3_Model_Iniciativa_areasrelacionadas_programas_estadodocumento: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_areasrelacionadas_programas_estadodocumento($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_areasrelacionadas_programas_estadodocumento($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $iawid Chave Primária - default: nextval('par3.iniciativa_areasrelacionadas_programas_estadodocument_iawid_seq'::regclass)
 * @property int $iapid chave estrangeira par3.iniciativas_areasrelacionadas_programas
 * @property int $esdid chave estrangeira workflow.estadodocumento
 * @property "char" $iawstatus Status (Soft Delete)
 * @property \Datetime(Y-m-d H:i:s) $iawdtinclusao data de inclusão
 * @property string $iawcpfinclusao cpf inclusão
 * @property \Datetime(Y-m-d H:i:s) $iawdtinativacao data de inativação
 * @property string $iawcpfinativacao cpf inativação
 */
class Par3_Model_IniciativaAreasRelacionadasProgramasEstadoDocumento extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_areasrelacionadas_programas_estadodocumento';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'iawid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'esdid' => array('tabela' => 'workflow.estadodocumento',                    'pk' => 'esdid'),
        'iapid' => array('tabela' => 'par3.iniciativa_areasrelacionadas_programas', 'pk' => 'iapid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'iawid'            => null,
        'iapid'            => null,
        'esdid'            => null,
        'iawstatus'        => null,
        'iawdtinclusao'    => null,
        'iawcpfinclusao'   => null,
        'iawdtinativacao'  => null,
        'iawcpfinativacao' => null,
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
            'iawid' => array('Digits'),
            'iapid' => array('allowEmpty' => true,'Digits'),
            'esdid' => array('allowEmpty' => true,'Digits'),
            'iawstatus' => array('allowEmpty' => true),
            'iawdtinclusao' => array('allowEmpty' => true),
            'iawcpfinclusao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'iawdtinativacao' => array('allowEmpty' => true),
            'iawcpfinativacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
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

    public function recuperarAreaRelaciondada($arr)
    {
        $sql = "SELECT * FROM par3.iniciativa_areasrelacionadas_programas_estadodocumento WHERE iapid = {$arr['iapid']} AND esdid = {$arr['esdid']}";
        return $this->recuperar($sql);
    }
}

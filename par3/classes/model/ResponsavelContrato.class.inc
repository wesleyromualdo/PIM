<?php
/**
 * Classe de mapeamento da entidade par3.ResponsavelContrato.
 *
 * @version $Id$
 * @since 2017.03.03
 */

/**
 * Par3_Model_ResponsavelContrato: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ResponsavelContrato($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ResponsavelContrato($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $rcoid pk do responsavel - default: nextval('par3.ResponsavelContrato_rcoid_seq'::regclass)
 * @property int $conid fk contrato do responsavel
 * @property string $rconome nome do responsavel
 * @property string $rcocpf cpf do responsavel
 * @property string $rcorg rg do responsavel
 * @property string $rcocargo cargo do responsavel
 * @property string $rcostatus status do responsavel - default: 'A'::bpchar
 * @property \Datetime(Y-m-d H:i:s) $rcodtcriacao data de criação do responsavel - default: now()
 * @property string $usucpf
 */
class Par3_Model_ResponsavelContrato extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.responsavel_contrato';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'rcoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'conid' => array('tabela' => 'par3.contratos', 'pk' => 'conid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'rcoid' => null,
        'conid' => null,
        'rconome' => null,
        'rcocpf' => null,
        'rcorg' => null,
        'rcocargo' => null,
        'rcostatus' => null,
        'rcodtcriacao' => null,
        'usucpf' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getPK($dados = array())
    {
        return $this->rcoid;
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'rcoid' => array('Digits'),
            'conid' => array('allowEmpty' => true,'Digits'),
            'rconome' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'rcocpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'rcorg' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'rcocargo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'rcostatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'rcodtcriacao' => array('allowEmpty' => true),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
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

    public function montarSQLSimples($arrPost)
    {
        $where = array("rcostatus = 'A'");

        if($arrPost['inuid']){
            $where[] = "con.inuid = {$arrPost['inuid']}";
        }

        $sql = "SELECT rcoid as id, con.connumero, rconome, rcocpf, rcocargo
                FROM ".$this->stNomeTabela." as rco
                INNER JOIN par3.contratos as con ON con.conid = rco.conid
                WHERE ".implode(' AND ', $where)."
                ORDER BY con.connumero, rconome";

        return $sql;
    }

}

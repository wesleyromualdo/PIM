<?php
/**
 * Classe de mapeamento da entidade par3.veiculo_contrato.
 *
 * @version $Id$
 * @since 2017.03.02
 */

/**
 * Par3_Model_Veiculo_contrato: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Veiculo_contrato($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Veiculo_contrato($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $vcoid chave primária do veiculo - default: nextval('par3.veiculo_contrato_vcoid_seq'::regclass)
 * @property int $conid fk contrato do veiculo
 * @property string $usucpf fk criador do veiculo
 * @property int $mveid fk modelo do veiculo
 * @property string $vcoplaca placa do veiculo
 * @property string $vcochassi chassi do veiculo
 * @property string $vcorenavam renavam do veiculo
 * @property string $vcoano ano do veiculo
 * @property int $vcoqtdassentos quantidade de assentos do veiculo
 * @property bool $vcopossuirenavam boo possui renavam do veiculo
 * @property string $vcostatus status do veiculo
 * @property \Datetime(Y-m-d H:i:s) $vcodtcriacao data de criação do veiculo - default: now()
 */
class Par3_Model_VeiculoContrato extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.veiculo_contrato';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'vcoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'mveid' => array('tabela' => 'par3.modelo_veiculo', 'pk' => 'mveid'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'conid' => array('tabela' => 'par3.contratos', 'pk' => 'conid'),
        'arqid' => array('tabela' => 'public.arquivos', 'pk' => 'arqid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'vcoid' => null,
        'conid' => null,
        'usucpf' => null,
        'mveid' => null,
        'vcoplaca' => null,
        'vcochassi' => null,
        'vcorenavam' => null,
        'vcoano' => null,
        'vcoqtdassentos' => null,
        'vcopossuirenavam' => null,
        'vcostatus' => null,
        'vcodtcriacao' => null,
        'arqid' => null,
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
            'vcoid' => array('Digits'),
            'conid' => array('allowEmpty' => true,'Digits'),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'mveid' => array('allowEmpty' => true,'Digits'),
            'vcoplaca' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 8))),
            'vcochassi' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 17))),
            'vcorenavam' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'vcoano' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'vcoqtdassentos' => array('allowEmpty' => true,'Digits'),
            'vcopossuirenavam' => array('allowEmpty' => true),
            'vcostatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'vcodtcriacao' => array('allowEmpty' => true),
        );
    }

    public function montarSQLSimplesVeiculos($arrPost)
    {
        $where = array("vco.vcostatus = 'A'");

        if($arrPost['inuid']) $where[] = "con.inuid = {$arrPost['inuid']}";
        if($arrPost['vcopossuirenavam']) $where[] = "vco.vcopossuirenavam = ".($arrPost['vcopossuirenavam'] == 't' ? 'TRUE' : 'FALSE');

        $sql = "SELECT vco.vcoid as id,con.connumero,vco.vcoplaca,vco.vcochassi,vco.vcorenavam,fab.fabdsc,mve.mvedsc,vco.vcoano,vco.vcoqtdassentos,vco.arqid
                FROM ".$this->stNomeTabela." as vco
                INNER JOIN par3.contratos           as con ON con.conid = vco.conid
                LEFT  JOIN par3.modelo_veiculo      as mve ON mve.mveid = vco.mveid
                LEFT  JOIN par3.fabricante_veiculo  as fab ON fab.fabid = mve.fabid
                WHERE ".implode(' AND ', $where)."
                ORDER BY con.connumero,vco.vcochassi";

        return $sql;
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

}

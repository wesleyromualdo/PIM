<?php
/**
 * Classe de mapeamento da entidade par3.modelo_veiculo.
 *
 * @version $Id$
 * @since 2017.03.02
 */

/**
 * Par3_Model_Modelo_veiculo: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Modelo_veiculo($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Modelo_veiculo($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $mveid pk do modelo - default: nextval('par3.modelo_veiculo_mveid_seq'::regclass)
 * @property string $usucpf fk do criador do modelo
 * @property int $fabid fk do fabricante do modelo
 * @property string $mvedsc descricao do modelo
 * @property string $mvestatus status do modelo
 * @property \Datetime(Y-m-d H:i:s) $mvedtcriacao data de criação do modelo - default: now()
 */
class Par3_Model_ModeloVeiculo extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.modelo_veiculo';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'mveid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'fabid' => array('tabela' => 'par3.fabricante_veiculo', 'pk' => 'fabid'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'mveid' => null,
        'usucpf' => null,
        'fabid' => null,
        'mvedsc' => null,
        'mvestatus' => null,
        'mvedtcriacao' => null,
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
            'mveid' => array('Digits'),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'fabid' => array('allowEmpty' => true,'Digits'),
            'mvedsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'mvestatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'mvedtcriacao' => array('allowEmpty' => true),
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

    public function verificarModeloExiste($arrPost)
    {
        if($arrPost['mveid']){return false;}

        $this->mvedsc = $arrPost['mvedsc'];
        $this->fabid  = $arrPost['fabid'];
        $sql="SELECT * FROM $this->stNomeTabela as mve 
              INNER JOIN par3.fabricante_veiculo as fab ON fab.fabid = mve.mveid
              WHERE upper(mve.mvedsc)  = upper('{$this->mvedsc}')
              AND mve.fabid  = $this->fabid
              AND mve.mvestatus = 'A' ";
       $retorno = $this->pegaUm($sql);
       return $retorno;
    }

    public function pegarSQLSelectCombo($arrPost = array())
    {
        $arrPost['fabid'] = $arrPost['fabid'] ? $arrPost['fabid'] : $this->fabid;

        $where = array("mvestatus = 'A'");

        if($arrPost['fabid']) $where[] = "fabid = {$arrPost['fabid']}";

        $sql = "SELECT mveid as codigo, mvedsc as descricao
                FROM ".$this->stNomeTabela."
                WHERE ".implode(' AND ', $where)."
                ORDER BY 2";

        return $sql;
    }

}

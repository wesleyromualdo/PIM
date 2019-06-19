<?php
/**
 * Classe de mapeamento da entidade par3.fabricante_veiculo.
 *
 * @version $Id$
 * @since 2017.03.02
 */

/**
 * Par3_Model_Fabricante_veiculo: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Fabricante_veiculo($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Fabricante_veiculo($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $fabid pk do fabricante - default: nextval('par3.fabricante_veiculo_fabid_seq'::regclass)
 * @property string $usucpf fk quem criou o registro
 * @property string $fabdsc descricao fabricante
 * @property string $fabstatus data de inclusao fabricante
 * @property \Datetime(Y-m-d H:i:s) $fabdtcriacao  - default: now()
 */
class Par3_Model_FabricanteVeiculo extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.fabricante_veiculo';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'fabid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'fabid' => null,
        'usucpf' => null,
        'fabdsc' => null,
        'fabstatus' => null,
        'fabdtcriacao' => null,
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
            'fabid' => array('Digits'),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'fabdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'fabstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'fabdtcriacao' => array('allowEmpty' => true),
        );
    }

    public function pegarSQLSelectListagem($arrPost = array())
    {
        $sql = "SELECT fabid as codigo, fabdsc as descricao FROM ".$this->stNomeTabela;
        if ($arrPost['fbid'] != '') {
            $sql = "SELECT
                    	fbid as id,
                    	fabdsc as descricao
                    FROM
                    	".$this->stNomeTabela."
                    WHERE
                    	fabid = {$arrPost['fabid']}
            		ORDER BY descricao";
        }
        return $sql;
    }

    public function verificarFabricanteExiste($arrPost)
    {
        if($arrPost['fabid']){return false;}
        $this->fabdsc = $arrPost['fabdsc'];
        $sql="SELECT * FROM $this->stNomeTabela as fab 
              WHERE fab.fabstatus = 'A'
              AND upper(fab.fabdsc)  = upper('{$this->fabdsc}')";
        $retorno = $this->pegaUm($sql);
        return $retorno;
    }

    public function pegarSQLSelectListagemCombo($arrPost = array())
    {
        $sql = "SELECT fabid as codigo, fabdsc as descricao FROM ".$this->stNomeTabela;
        if ($arrPost['fbid'] != '') {
            $sql = "SELECT
                    	fbid as codigo,
                    	fabdsc as descricao
                    FROM
                    	".$this->stNomeTabela."
                    WHERE
                    	fabid = {$arrPost['fabid']}
            		ORDER BY descricao";
        }
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

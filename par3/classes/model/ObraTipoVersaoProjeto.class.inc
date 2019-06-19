<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_versao_projeto.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_ObraTipoVersaoProjeto: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraTipoVersaoProjeto($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraTipoVersaoProjeto($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ovpid  - default: nextval('par3.obra_tipo_versao_projeto_ovpid_seq'::regclass)
 * @property int $otpid
 * @property string $ovpdsc
 * @property string $ovpsituacao
 * @property string $ovpstatus
 * @property float $ovpmetragem
 */
class Par3_Model_ObraTipoVersaoProjeto extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_versao_projeto';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ovpid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'otpid' => array('tabela' => 'par3.obra_tipo', 'pk' => 'otpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ovpid' => null,
        'otpid' => null,
        'ovpdsc' => null,
        'ovpsituacao' => null,
        'ovpstatus' => null,
        'ovpmetragem' => null,
    );

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function verificarExiste(array $arrPost)
    {
        $where = $arrPost['ovpid'] ? " AND ovpid != {$arrPost['ovpid']}":'';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(ovpdsc) = '".str_to_upper($arrPost['ovpdsc'])."' AND ovpstatus = 'A' AND otpid = {$arrPost['otpid']} {$where}";
        return $this->pegaLinha($sql,0,0);
    }

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['ovpdsc'] = array();

        //Validações
        if($campos['ovpdsc'] == '' || $campos['ovpdsc'] == null){array_push($erros['erros']['ovpdsc'],'O campo não pode ser vazio');}
        if($this->verificarExiste($campos)){array_push($erros['erros']['ovpdsc'], 'Já existe Tipo de Desdobramento cadastrado com essa descrição');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function pegarSQLSelectCombo($otpid = null)
    {
        $where = array("ovpstatus = 'A'");
        if($otpid != '') $where[] = "otpid = $otpid";
        return "SELECT  ovpid as codigo, ovpid||' - '||ovpdsc as descricao FROM {$this->stNomeTabela} WHERE ".implode(' AND ', $where);
    }

    public function montarSQLSimples($arrPost)
    {
        $where = array("ovp.ovpstatus = 'A'");
        if($arrPost['otpid']) $where[] = "ovp.otpid = '{$arrPost['otpid']}'";

        $orderBy = $arrPost['ordenacao'] ? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                    ovp.ovpid||'_proj' as id,
                    ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                    ovp.ovpid,
                    ovp.ovpid as codigo,
                    ovpdsc as descricao,
                    otp.otpdsc,
                    CASE WHEN ovp.ovpsituacao = 'A'
                        THEN 'Ativo'
                        ELSE 'Inativo'
                    END as ovpsituacao
                FROM
                    par3.obra_tipo_versao_projeto ovp
                INNER JOIN par3.obra_tipo otp ON otp.otpid = ovp.otpid
                WHERE
                    ".implode(' AND ', $where)."
                {$orderBy}";

        return $sql;
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
            'ovpid' => array('Digits'),
            'otpid' => array('Digits'),
            'ovpdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ovpsituacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'ovpstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'ovpmetragem'
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes fin alidades:
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

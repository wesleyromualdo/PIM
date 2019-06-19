<?php
/**
 * Classe de mapeamento da entidade par3.obra_aba.
 *
 * @version $Id$
 * @since 2017.07.12
 */

/**
 * Par3_Model_ObraAba: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraAba($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraAba($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $oabid  - default: nextval('par3.obra_aba_oabid_seq'::regclass)
 * @property string $oabdsc
 * @property string $oabarquivo
 * @property string $oabstatus
 */
class Par3_Model_ObraAba extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_aba';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'oabid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'oabid' => null,
        'oabdsc' => null,
        'oabarquivo' => null,
        'oabstatus' => null,
    );

    const ESTUDO_DEMANDA           = 1;
    const DADOS_TERRENO            = 2;
    const RELATORIO_VISTORIA       = 3;
    const FOTOS_TERRENO            = 4;
    const PLANILHA_ORCAMENTARIA    = 5;
    const CRONOGRAMA_CONTRAPARTIDA = 6;
    const DOCUMENTOS_ANEXOS        = 7;
    const ENVIAR_ANALISE           = 8;
    const ANALISE_ENGENHARIA       = 9;
    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'oabid' => array('Digits'),
            'oabdsc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'oabarquivo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'oabstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
        );
    }

    public function recuperaAbas($arrPost = array())
    {
        $sql = "SELECT
                	oab.oabid,
                	oca.ocaid,
                	oab.oabdsc,
                	oab.oabstatus
                FROM
                	par3.obra_aba oab
                LEFT JOIN par3.obra_configuracao_aba oca ON oca.oabid = oab.oabid AND oca.ocastatus = 'A' AND oca.otpid = {$arrPost['otpid']}
                WHERE
                	oabstatus = 'A'";
// ver($sql);
        return $this->carregar($sql);
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

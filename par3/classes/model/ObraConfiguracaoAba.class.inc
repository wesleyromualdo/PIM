<?php
/**
 * Classe de mapeamento da entidade par3.obra_configuracao_aba.
 *
 * @version $Id$
 * @since 2017.07.12
 */

/**
 * Par3_Model_ObraConfiguracaoAba: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraConfiguracaoAba($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraConfiguracaoAba($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ocaid  - default: nextval('par3.obra_configuracao_aba_ocaid_seq'::regclass)
 * @property int $oabid
 * @property int $atfid
 * @property int $otpid
 * @property smallint $ocaordem
 * @property smallint $ocaqtd_minimo
 * @property smallint $ocaqtd_maximo
 * @property numeric $ocatamanho_maximo
 */
class Par3_Model_ObraConfiguracaoAba extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_configuracao_aba';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array('ocaid');

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'atfid' => array('tabela' => 'par3.obra_aba_tipo_foto', 'pk' => 'atfid'),
        'otpid' => array('tabela' => 'par3.obra_tipo', 'pk' => 'otpid'),
        'oabid' => array('tabela' => 'par3.obra_aba', 'pk' => 'oabid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ocaid' => null,
        'oabid' => null,
        'atfid' => null,
        'otpid' => null,
        'ocaordem' => null,
        'ocaqtd_minimo' => null,
        'ocaqtd_maximo' => null,
        'ocatamanho_maximo' => null,
        'ocastatus' => null,
    );

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['ocaqtd_minimo']        = array();
        $erros['erros']['ocaqtd_maximo']        = array();
        $erros['erros']['ocatamanho_maximo']    = array();
        $erros['erros']['atfid']                = array();

        //Validações
        if($campos['ocaqtd_minimo'] == '' || $campos['ocaqtd_minimo'] == null){array_push($erros['erros']['ocaqtd_minimo'],'O campo não pode ser vazio');}
        if($campos['ocaqtd_maximo'] == '' || $campos['ocaqtd_maximo'] == null){array_push($erros['erros']['ocaqtd_maximo'],'O campo não pode ser vazio');}
        if($campos['ocaqtd_minimo'] > $campos['ocaqtd_maximo']){
            array_push($erros['erros']['ocaqtd_minimo'],'O campo mínimo deve ser menor que o campo máximo');
            array_push($erros['erros']['ocaqtd_maximo'],'O campo máximo deve ser maior que o cmapo mínimo');
        }
        if($campos['ocatamanho_maximo'] == '' || $campos['ocatamanho_maximo'] == null){array_push($erros['erros']['ocatamanho_maximo'],'O campo não pode ser vazio');}
        if($campos['atfid'] == '' || $campos['atfid'] == null){array_push($erros['erros']['atfid'],'O campo não pode ser vazio');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }
    public function recuperaTiposFotoVinculados($ocaid)
    {
        $sql = "SELECT atfid FROM par3.obra_configuracao_aba_x_tipo_foto WHERE ocaid = $ocaid;";

        return $this->carregarColuna($sql);
    }

    public function recuperarPorTipoObra($otpid)
    {
        $sql = "SELECT * FROM par3.obra_configuracao_aba WHERE otpid = $otpid AND oabid = 4;";
        return (Object)$this->recuperar($sql);
    }

    public function excluirTiposFoto($ocaid)
    {
        $sql = "DELETE FROM par3.obra_configuracao_aba_x_tipo_foto WHERE ocaid = $ocaid;";

        $this->executar($sql);
        $this->commit();
    }

    public function incluirTodosTiposFoto($ocaid)
    {
    	$sql = "INSERT INTO par3.obra_configuracao_aba_x_tipo_foto(atfid, ocaid)
    	        SELECT atfid, $ocaid as ocaid FROM par3.obra_aba_tipo_foto WHERE atfstatus = 'A';";

    	$this->executar($sql);
        $this->commit();
    }

    public function incluirTipoFoto($ocaid, $aftid)
    {
    	$sql = "INSERT INTO par3.obra_configuracao_aba_x_tipo_foto(atfid, ocaid) VALUES($aftid, $ocaid);";

    	$this->executar($sql);
        $this->commit();
    }

    public function pegarSelectComboTipoFotos($otpid = null)
    {
        return "SELECT
                	atfid::text as codigo,
                	atfdsc as descricao
                FROM
                	par3.obra_aba_tipo_foto";
    }

    public function retornarDocumentos($dados)
    {
    	$sql = "SELECT
                	oct.octid,
                	otd.otdid,
                	otd.otddsc,
                	oct.octordem
                FROM
                	par3.obra_tipo_documento otd
                LEFT JOIN par3.obra_confi_aba_x_tipo_doc oct ON oct.otdid = otd.otdid AND octstatus = 'A' and ocaid = '{$dados['ocaid']}'
                WHERE
                	otd.otdsituacao = 'A'
                	AND otd.otdstatus = 'A';";

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

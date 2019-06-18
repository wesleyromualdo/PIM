<?php
/**
 * Classe de mapeamento da entidade par3.obra_configuracao_aba_historico.
 *
 * @version $Id$
 * @since 2017.07.12
 */

/**
 * Par3_Model_ObraConfiguracaoAbaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraConfiguracaoAbaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraConfiguracaoAbaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hocid chave primária da tabelaobra_configuracao_aba_historico
 * @property  $hoccpf CPF do respnsável pela alteração
 * @property  $hocacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hocdtcriacao Data de criação do log
 * @property int $ocaid  - default: nextval('par3.obra_configuracao_aba_ocaid_seq'::regclass)
 * @property int $oabid
 * @property int $atfid
 * @property int $otpid
 * @property smallint $ocaordem
 * @property smallint $ocaqtd_minimo
 * @property smallint $ocaqtd_maximo
 * @property numeric $ocatamanho_maximo
 */
class Par3_Model_ObraConfiguracaoAbaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_configuracao_aba_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hocid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ocaid' => array('tabela' => 'par3.obra_configuracao_aba', 'pk' => 'ocaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hocid' => null,
        'hoccpf' => null,
        'hocacao' => null,
        'hocdtcriacao' => null,
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

            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraConfiguracaoAba $model,$acao)
    {

        $this->arAtributos['hoccpf']            = $_SESSION['usucpf'];
        $this->arAtributos['hocacao']           = $acao;
        $this->arAtributos['hocdtcriacao']      = date('Y-m-d H:m:s');
        $this->arAtributos['ocaid']             = $model->ocaid;
        $this->arAtributos['oabid']             = $model->oabid;
        $this->arAtributos['atfid']             = $model->atfid;
        $this->arAtributos['otpid']             = $model->otpid;
        $this->arAtributos['ocaordem']          = $model->ocaordem;
        $this->arAtributos['ocaqtd_minimo']     = $model->ocaqtd_minimo;
        $this->arAtributos['ocaqtd_maximo']     = $model->ocaqtd_maximo;
        $this->arAtributos['ocatamanho_maximo'] = $model->ocatamanho_maximo;
        $this->arAtributos['ocastatus']         = $model->ocastatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

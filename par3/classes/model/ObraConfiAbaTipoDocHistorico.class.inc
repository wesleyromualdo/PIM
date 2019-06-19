<?php
/**
 * Classe de mapeamento da entidade par3.obra_confi_aba_x_tipo_doc_historico.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_ObraConfiAbaTipoDocHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraConfiAbaTipoDocHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraConfiAbaTipoDocHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hocid chave primária da tabelaobra_confi_aba_x_tipo_doc_historico
 * @property  $hoccpf CPF do respnsável pela alteração
 * @property  $hocacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hocdtcriacao Data de criação do log
 * @property string $octid
 * @property int $otdid
 * @property int $ocaid
 * @property string $octstatus
 * @property string $octordem
 */
class Par3_Model_ObraConfiAbaTipoDocHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_confi_aba_x_tipo_doc_historico';

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
        'octid' => array('tabela' => 'par3.obra_confi_aba_x_tipo_doc', 'pk' => 'octid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hocid' => null,
        'hoccpf' => null,
        'hocacao' => null,
        'hocdtcriacao' => null,
        'octid' => null,
        'otdid' => null,
        'ocaid' => null,
        'octstatus' => null,
        'octordem' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraConfiAbaTipoDoc $model,$acao)
    {

        $this->arAtributos['hoccpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hocacao'] = $acao;
        $this->arAtributos['hocdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['octid'] = $model->octid;
        $this->arAtributos['otdid'] = $model->otdid;
        $this->arAtributos['ocaid'] = $model->ocaid;
        $this->arAtributos['octstatus'] = $model->octstatus;
        $this->arAtributos['octordem'] = $model->octordem;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

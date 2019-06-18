<?php
/**
 * Classe de mapeamento da entidade par3.obra_aba_tipo_foto_historico.
 *
 * @version $Id$
 * @since 2017.07.12
 */

/**
 * Par3_Model_ObraAbaTipoFotoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraAbaTipoFotoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraAbaTipoFotoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hatid chave primária da tabelaobra_aba_tipo_foto_historico
 * @property  $hatcpf CPF do respnsável pela alteração
 * @property  $hatacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hatdtcriacao Data de criação do log
 * @property int $atfid  - default: nextval('par3.obra_aba_tipo_foto_atfid_seq'::regclass)
 * @property string $atfdsc
 * @property string $atfstatus
 */
class Par3_Model_ObraAbaTipoFotoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_aba_tipo_foto_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hatid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'atfid' => array('tabela' => 'par3.obra_aba_tipo_foto', 'pk' => 'atfid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hatid' => null,
        'hatcpf' => null,
        'hatacao' => null,
        'hatdtcriacao' => null,
        'atfid' => null,
        'atfdsc' => null,
        'atfstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraAbaTipoFoto $model,$acao)
    {

        $this->arAtributos['hatcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hatacao'] = $acao;
        $this->arAtributos['hatdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['atfid'] = $model->atfid;
        $this->arAtributos['atfdsc'] = $model->atfdsc;
        $this->arAtributos['atfstatus'] = $model->atfstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_versao_planilha_historico.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_ObraTipoVersaoPlanilhaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraTipoVersaoPlanilhaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraTipoVersaoPlanilhaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $htpid chave primária da tabelaobra_tipo_versao_planilha_historico
 * @property  $htpcpf CPF do respnsável pela alteração
 * @property  $htpacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $htpdtcriacao Data de criação do log
 * @property int $tplid  - default: nextval('par3.obra_tipo_versao_planilha_tplid_seq'::regclass)
 * @property int $ovpid
 * @property string $tpldsc
 * @property string $tplsituacao
 * @property string $tplstatus
 */
class Par3_Model_ObraTipoVersaoPlanilhaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_versao_planilha_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'htpid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'tplid' => array('tabela' => 'par3.obra_tipo_versao_planilha', 'pk' => 'tplid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'htpid' => null,
        'htpcpf' => null,
        'htpacao' => null,
        'htpdtcriacao' => null,
        'tplid' => null,
        'ovpid' => null,
        'tpldsc' => null,
        'tplsituacao' => null,
        'tplstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraTipoVersaoPlanilha $model,$acao)
    {

        $this->arAtributos['htpcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['htpacao'] = $acao;
        $this->arAtributos['htpdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['tplid'] = $model->tplid;
        $this->arAtributos['ovpid'] = $model->ovpid;
        $this->arAtributos['tpldsc'] = $model->tpldsc;
        $this->arAtributos['tplsituacao'] = $model->tplsituacao;
        $this->arAtributos['tplstatus'] = $model->tplstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

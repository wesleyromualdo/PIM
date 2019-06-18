<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_item_planilha_historico.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_ObraTipoItemPlanilhaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraTipoItemPlanilhaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraTipoItemPlanilhaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hoiid chave primária da tabelaobra_tipo_item_planilha_historico
 * @property  $hoicpf CPF do respnsável pela alteração
 * @property  $hoiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hoidtcriacao Data de criação do log
 * @property int $oipid  - default: nextval('par3.obra_tipo_item_planilha_oipid_seq'::regclass)
 * @property int $unmcod
 * @property int $oipid_pai
 * @property int $tplid
 * @property string $oipdsc
 * @property string $oipordem
 * @property string $oipcodigo
 * @property string $oipregiao
 */
class Par3_Model_ObraTipoItemPlanilhaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_item_planilha_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hoiid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'oipid' => array('tabela' => 'par3.obra_tipo_item_planilha', 'pk' => 'oipid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hoiid' => null,
        'hoicpf' => null,
        'hoiacao' => null,
        'hoidtcriacao' => null,
        'oipid' => null,
        'unmcod' => null,
        'oipid_pai' => null,
        'tplid' => null,
        'oipdsc' => null,
        'oipordem' => null,
        'oipcodigo' => null,
        'oipregiao' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraTipoItemPlanilha $model,$acao)
    {

        $this->arAtributos['hoicpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hoiacao'] = $acao;
        $this->arAtributos['hoidtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['oipid'] = $model->oipid;
        $this->arAtributos['unmcod'] = $model->unmcod;
        $this->arAtributos['oipid_pai'] = $model->oipid_pai;
        $this->arAtributos['tplid'] = $model->tplid;
        $this->arAtributos['oipdsc'] = $model->oipdsc;
        $this->arAtributos['oipordem'] = $model->oipordem;
        $this->arAtributos['oipcodigo'] = $model->oipcodigo;
        $this->arAtributos['oipregiao'] = $model->oipregiao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

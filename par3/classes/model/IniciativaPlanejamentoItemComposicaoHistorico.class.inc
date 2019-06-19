<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_item_composicao_historico.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamento_item_composicaoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_item_composicaoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_item_composicaoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hipid chave primária da tabelainiciativa_planejamento_item_composicao_historico
 * @property  $hipcpf CPF do respnsável pela alteração
 * @property  $hipacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hipdtcriacao Data de criação do log
 * @property int $ipiid Chave Primária - default: nextval('par3.Iniciativa_planejamentoItemComposicao_ipiid_seq'::regclass)
 * @property int $inpid FK iniciativa_planejamento
 * @property numeric $ipivalorreferencia valor de referência do item no momendo do primeiro cadastro
 * @property int $ipiquantidade Quantidade de itens
 * @property int $ipiano Ano de referência
 * @property int $iigid FK fk_itenscomposicao_grupos_iniciativa
 * @property string $ipistatus status A- ativo, I - inativo - default: 'A'::bpchar
 */
class Par3_Model_IniciativaPlanejamentoItemComposicaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.Iniciativa_planejamento_item_composicao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hipid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ipiid' => array('tabela' => 'par3.Iniciativa_planejamento_item_composicao', 'pk' => 'ipiid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hipid' => null,
        'hipcpf' => null,
        'hipacao' => null,
        'hipdtcriacao' => null,
        'ipiid' => null,
        'inpid' => null,
        'ipivalorreferencia' => null,
        'ipiquantidade' => null,
        'ipiano' => null,
        'iigid' => null,
        'ipistatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaPlanejamentoItemComposicao $model,$acao)
    {
    
        $this->arAtributos['hipcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hipacao'] = $acao;
        $this->arAtributos['hipdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['ipiid'] = $model->ipiid;
        $this->arAtributos['inpid'] = $model->inpid;
        $this->arAtributos['ipivalorreferencia'] = $model->ipivalorreferencia;
        $this->arAtributos['ipiquantidade'] = $model->ipiquantidade;
        $this->arAtributos['ipiano'] = $model->ipiano;
        $this->arAtributos['iigid'] = $model->iigid;
        $this->arAtributos['ipistatus'] = $model->ipistatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos)
             ->salvar();
        $this->clearDados();
        $this->commit();
    }
}

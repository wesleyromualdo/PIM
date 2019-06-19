<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_item_composicao_escola_historico.
 *
 * @version $Id$
 * @since 2017.07.17
 */

/**
 * Par3_Model_Iniciativa_planejamento_item_composicao_escolaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_item_composicao_escolaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_item_composicao_escolaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hipid chave primária da tabelainiciativa_planejamento_item_composicao_escola_historico
 * @property  $hipcpf CPF do respnsável pela alteração
 * @property  $hipacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hipdtcriacao Data de criação do log
 * @property string $ipestatus status A- ativo, I - inativo - default: 'A'::bpchar
 * @property int $ipequantidade Quantidade de itens
 * @property int $ipiid FK iniciativa_planejamento_item_composicao
 * @property int $ipeid Chave Primária - default: nextval('par3.iniciativa_planejamento_item_composicao_escola_ipeid_seq'::regclass)
 */
class Par3_Model_IniciativaPlanejamentoItemComposicaoEscolaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_item_composicao_escola_historico';

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
        'ipeid' => array('tabela' => 'par3.iniciativa_planejamento_item_composicao_escola', 'pk' => 'ipeid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hipid' => null,
        'hipcpf' => null,
        'hipacao' => null,
        'hipdtcriacao' => null,
        'ipestatus' => null,
        'ipequantidade' => null,
        'ipiid' => null,
        'ipeid' => null,
        'co_entidade' => null,
        'escid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaPlanejamentoItemComposicaoEscola $model,$acao)
    {
    
        $this->arAtributos['hipcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hipacao'] = $acao;
        $this->arAtributos['hipdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['ipestatus'] = $model->ipestatus;
        $this->arAtributos['ipequantidade'] = $model->ipequantidade;
//        $this->arAtributos['co_entidade'] = $model->co_entidade;
        $this->arAtributos['ipiid'] = $model->ipiid;
        $this->arAtributos['ipeid'] = $model->ipeid;
        $this->arAtributos['escid'] = $model->escid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos)
             ->salvar();
        $this->clearDados();
        $this->commit();
    }
}

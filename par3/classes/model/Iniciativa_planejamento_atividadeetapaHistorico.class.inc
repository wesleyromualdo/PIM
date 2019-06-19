<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_atividadeetapa_historico.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamento_atividadeetapaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_atividadeetapaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_atividadeetapaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hipid chave primária da tabelainiciativa_planejamento_atividadeetapa_historico
 * @property  $hipcpf CPF do respnsável pela alteração
 * @property  $hipacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hipdtcriacao Data de criação do log
 * @property int $ipiaid  - default: nextval('par3.iniciativa_planejamento_atividadeetapa_ipiaid_seq'::regclass)
 * @property int $ipidid 
 * @property string $ipiadescicao 
 * @property int $ipiaquanteventos 
 * @property int $ipiaquantdias 
 * @property numeric $ipiaquanthoras 
 * @property numeric $ipiatotal 
 * @property string $ipiastatus  - default: 'A'::bpchar
 */
class Par3_Model_Iniciativa_planejamento_atividadeetapaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_atividadeetapa_historico';

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
        'ipiid' => array('tabela' => 'par3.iniciativa_planejamento_atividadeetapa', 'pk' => 'ipiid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hipid' => null,
        'hipcpf' => null,
        'hipacao' => null,
        'hipdtcriacao' => null,
        'ipiaid' => null,
        'ipidid' => null,
        'ipiadescicao' => null,
        'ipiaquanteventos' => null,
        'ipiaquantdias' => null,
        'ipiaquanthoras' => null,
        'ipiatotal' => null,
        'ipiastatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Iniciativa_planejamento_atividadeetapa $model,$acao)
    {
    
        $this->arAtributos['hipcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hipacao'] = $acao;
        $this->arAtributos['hipdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['ipiaid'] = $model->ipiaid;
        $this->arAtributos['ipidid'] = $model->ipidid;
        $this->arAtributos['ipiadescicao'] = $model->ipiadescicao;
        $this->arAtributos['ipiaquanteventos'] = $model->ipiaquanteventos;
        $this->arAtributos['ipiaquantdias'] = $model->ipiaquantdias;
        $this->arAtributos['ipiaquanthoras'] = $model->ipiaquanthoras;
        $this->arAtributos['ipiatotal'] = $model->ipiatotal;
        $this->arAtributos['ipiastatus'] = $model->ipiastatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

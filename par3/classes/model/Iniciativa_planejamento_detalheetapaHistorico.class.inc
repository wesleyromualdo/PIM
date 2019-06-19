<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_detalheetapa_historico.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamento_detalheetapaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_detalheetapaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_detalheetapaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hipid chave primária da tabelainiciativa_planejamento_detalheetapa_historico
 * @property  $hipcpf CPF do respnsável pela alteração
 * @property  $hipacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hipdtcriacao Data de criação do log
 * @property int $ipidid  - default: nextval('par3.iniciativa_planejamento_detalheetapa_ipidid_seq'::regclass)
 * @property int $inpid 
 * @property string $ipidatividade 
 * @property string $ipiddescricao 
 * @property string $ipidunidade 
 * @property numeric $ipidquantidadehoras 
 * @property numeric $ipidtotalhoras 
 * @property string $ipidstatus  - default: 'A'::bpchar
 */
class Par3_Model_Iniciativa_planejamento_detalheetapaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_detalheetapa_historico';

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
        'ipiid' => array('tabela' => 'par3.iniciativa_planejamento_detalheetapa', 'pk' => 'ipiid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hipid' => null,
        'hipcpf' => null,
        'hipacao' => null,
        'hipdtcriacao' => null,
        'ipidid' => null,
        'inpid' => null,
        'ipidatividade' => null,
        'ipiddescricao' => null,
        'ipidunidade' => null,
        'ipidquantidadehoras' => null,
        'ipidtotalhoras' => null,
        'ipidstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Iniciativa_planejamento_detalheetapa $model,$acao)
    {
    
        $this->arAtributos['hipcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hipacao'] = $acao;
        $this->arAtributos['hipdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['ipidid'] = $model->ipidid;
        $this->arAtributos['inpid'] = $model->inpid;
        $this->arAtributos['ipidatividade'] = $model->ipidatividade;
        $this->arAtributos['ipiddescricao'] = $model->ipiddescricao;
        $this->arAtributos['ipidunidade'] = $model->ipidunidade;
        $this->arAtributos['ipidquantidadehoras'] = $model->ipidquantidadehoras;
        $this->arAtributos['ipidtotalhoras'] = $model->ipidtotalhoras;
        $this->arAtributos['ipidstatus'] = $model->ipidstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_informacoesiniciativa_historico.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamento_informacoesiniciativaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_informacoesiniciativaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_informacoesiniciativaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hipid chave primária da tabelainiciativa_planejamento_informacoesiniciativa_historico
 * @property  $hipcpf CPF do respnsável pela alteração
 * @property  $hipacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hipdtcriacao Data de criação do log
 * @property int $ipifid  - default: nextval('par3.iniciativa_planejamento_informacoesiniciativa_ipifid_seq'::regclass)
 * @property int $inpid 
 * @property string $ipifprofessores 
 * @property string $ipiforientadores 
 * @property string $ipifformadores 
 * @property string $ipiftutores 
 * @property string $ipifsupervisores 
 * @property string $ipifpreparadores 
 * @property string $ipifstatus  - default: 'A'::bpchar
 */
class Par3_Model_Iniciativa_planejamento_informacoesiniciativaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_informacoesiniciativa_historico';

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
        'ipiid' => array('tabela' => 'par3.iniciativa_planejamento_informacoesiniciativa', 'pk' => 'ipiid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hipid' => null,
        'hipcpf' => null,
        'hipacao' => null,
        'hipdtcriacao' => null,
        'ipifid' => null,
        'inpid' => null,
        'ipifprofessores' => null,
        'ipiforientadores' => null,
        'ipifformadores' => null,
        'ipiftutores' => null,
        'ipifsupervisores' => null,
        'ipifpreparadores' => null,
        'ipifstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Iniciativa_planejamento_informacoesiniciativa $model,$acao)
    {
    
        $this->arAtributos['hipcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hipacao'] = $acao;
        $this->arAtributos['hipdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['ipifid'] = $model->ipifid;
        $this->arAtributos['inpid'] = $model->inpid;
        $this->arAtributos['ipifprofessores'] = $model->ipifprofessores;
        $this->arAtributos['ipiforientadores'] = $model->ipiforientadores;
        $this->arAtributos['ipifformadores'] = $model->ipifformadores;
        $this->arAtributos['ipiftutores'] = $model->ipiftutores;
        $this->arAtributos['ipifsupervisores'] = $model->ipifsupervisores;
        $this->arAtributos['ipifpreparadores'] = $model->ipifpreparadores;
        $this->arAtributos['ipifstatus'] = $model->ipifstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

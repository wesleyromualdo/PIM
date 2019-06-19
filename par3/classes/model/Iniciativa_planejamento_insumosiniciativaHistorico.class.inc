<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_insumosiniciativa_historico.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_Iniciativa_planejamento_insumosiniciativaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamento_insumosiniciativaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamento_insumosiniciativaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hipid chave primária da tabelainiciativa_planejamento_insumosiniciativa_historico
 * @property  $hipcpf CPF do respnsável pela alteração
 * @property  $hipacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hipdtcriacao Data de criação do log
 * @property int $ipiinid  - default: nextval('par3.iniciativa_planejamento_insumosiniciativa_ipiinid_seq'::regclass)
 * @property int $inpid 
 * @property string $ipiindescricao 
 * @property string $ipiininsumo1 
 * @property string $ipiininsumo2 
 * @property string $ipiinunidadeinsumo1 
 * @property string $ipiinunidadeinsumo2 
 * @property numeric $ipiinqtdinsumo1 
 * @property numeric $ipiinqtdinsumo2 
 * @property numeric $ipiinqtd 
 * @property numeric $ipiinvalunit 
 * @property numeric $ipiinvaltotal 
 * @property string $ipiinstatus  - default: 'A'::bpchar
 */
class Par3_Model_Iniciativa_planejamento_insumosiniciativaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_insumosiniciativa_historico';

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
        'ipiid' => array('tabela' => 'par3.iniciativa_planejamento_insumosiniciativa', 'pk' => 'ipiid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hipid' => null,
        'hipcpf' => null,
        'hipacao' => null,
        'hipdtcriacao' => null,
        'ipiinid' => null,
        'inpid' => null,
        'ipiindescricao' => null,
        'ipiininsumo1' => null,
        'ipiininsumo2' => null,
        'ipiinunidadeinsumo1' => null,
        'ipiinunidadeinsumo2' => null,
        'ipiinqtdinsumo1' => null,
        'ipiinqtdinsumo2' => null,
        'ipiinqtd' => null,
        'ipiinvalunit' => null,
        'ipiinvaltotal' => null,
        'ipiinstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Iniciativa_planejamento_insumosiniciativa $model,$acao)
    {
    
        $this->arAtributos['hipcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hipacao'] = $acao;
        $this->arAtributos['hipdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['ipiinid'] = $model->ipiinid;
        $this->arAtributos['inpid'] = $model->inpid;
        $this->arAtributos['ipiindescricao'] = $model->ipiindescricao;
        $this->arAtributos['ipiininsumo1'] = $model->ipiininsumo1;
        $this->arAtributos['ipiininsumo2'] = $model->ipiininsumo2;
        $this->arAtributos['ipiinunidadeinsumo1'] = $model->ipiinunidadeinsumo1;
        $this->arAtributos['ipiinunidadeinsumo2'] = $model->ipiinunidadeinsumo2;
        $this->arAtributos['ipiinqtdinsumo1'] = $model->ipiinqtdinsumo1;
        $this->arAtributos['ipiinqtdinsumo2'] = $model->ipiinqtdinsumo2;
        $this->arAtributos['ipiinqtd'] = $model->ipiinqtd;
        $this->arAtributos['ipiinvalunit'] = $model->ipiinvalunit;
        $this->arAtributos['ipiinvaltotal'] = $model->ipiinvaltotal;
        $this->arAtributos['ipiinstatus'] = $model->ipiinstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

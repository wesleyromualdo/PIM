<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_historico.
 *
 * @version $Id$
 * @since 2017.06.23
 */

/**
 * Par3_Model_IniciativaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hinid chave primária da tabelainiciativa_historico
 * @property  $hincpf CPF do respnsável pela alteração
 * @property  $hinacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hindtcriacao Data de criação do log
 * @property int $iniid Chave Primária - default: nextval('par3.iniciativa_iniid_seq'::regclass)
 * @property "char" $inistatus Status da iniciativa
 * @property "char" $inisituacao 
 * @property int $indid  - default: nextval('par3.iniciativa_indid_seq'::regclass)
 * @property int $intaid FK do Tipo de Atendimento (par3.iniciativa_tipos_atendimento) - default: nextval('par3.iniciativa_intaid_seq'::regclass)
 * @property int $cicid FK do Ciclo (par3.ciclo_par) - default: nextval('par3.iniciativa_cicid_seq'::regclass)
 * @property int $intoid FK do Tipo de Objeto (par3.iniciativa_tipos_objeto) - default: nextval('par3.iniciativa_intoid_seq'::regclass)
 * @property "char" $iniesfera Esfera da iniciativa. "M" => Municipal, "E" => Estadual, "T" => Todos
 * @property string $initodos_estuf S ou N. Indica se possui todos os estados
 * @property string $initodos_muncod S ou N. Indica se possui todos os municípios
 */
class Par3_Model_IniciativaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hinid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hinid' => null,
        'hincpf' => null,
        'hinacao' => null,
        'hindtcriacao' => null,
        'iniid' => null,
        'inistatus' => null,
        'inisituacao' => null,
        'indid' => null,
        'intaid' => null,
        'cicid' => null,
        'intoid' => null,
        'iniesfera' => null,
        'initodos_estuf' => null,
        'initodos_muncod' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

public function gravarHistorico(Par3_Model_Iniciativa $model,$acao)
    {
    
        $this->arAtributos['hincpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hinacao'] = $acao;
        $this->arAtributos['hindtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['iniid'] = $model->iniid;
        $this->arAtributos['inistatus'] = $model->inistatus;
        $this->arAtributos['inisituacao'] = $model->inisituacao;
        $this->arAtributos['indid'] = $model->indid;
        $this->arAtributos['intaid'] = $model->intaid;
        $this->arAtributos['cicid'] = $model->cicid;
        $this->arAtributos['intoid'] = $model->intoid;
        $this->arAtributos['iniesfera'] = $model->iniesfera;
        $this->arAtributos['initodos_estuf'] = $model->initodos_estuf;
        $this->arAtributos['initodos_muncod'] = $model->initodos_muncod;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

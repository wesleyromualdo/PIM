<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_planejamento_historico.
 *
 * @version $Id$
 * @since 2017.07.10
 */

/**
 * Par3_Model_Iniciativa_planejamentoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_planejamentoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_planejamentoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hinid chave primária da tabelainiciativa_planejamento_historico
 * @property  $hincpf CPF do respnsável pela alteração
 * @property  $hinacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hindtcriacao Data de criação do log
 * @property int $inpid Chave Primária - default: nextval('par3.iniciativa_planejamento_inpid_seq'::regclass)
 * @property int $dimid FK par3.dimensao
 * @property int $iniid FK par3.iniciativa
 * @property int $imoid
 * @property int $docid
 * @property string $inpstatus Status (SOFT DELETE) - default: 'A'::bpchar
 * @property string $inpsituacaocadastro  - default: 'A'::bpchar
 */
class Par3_Model_IniciativaPlanejamentoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_planejamento_historico';

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
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'iniid' => array('tabela' => 'par3.iniciativa', 'pk' => 'iniid'),
        'dimid' => array('tabela' => 'par3.dimensao', 'pk' => 'dimid'),
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hinid'                         => null,
        'hincpf'                        => null,
        'hinacao'                       => null,
        'hindtcriacao'                  => null,
        'inpid'                         => null,
        'dimid'                         => null,
        'inuid'                         => null,
        'iniid'                         => null,
        'docid'                         => null,
        'inpstatus'                     => null,
        'inpsituacaocadastro'           => null,
        'inpcronogramamesinicial'       => null,
        'inpcronogramamesfinal'         => null,
        'inpcronogramaano'              => null,
        'inpitemcomposicaodetalhamento' => null,
        'nivid'                         => null,
        'etaid'                         => null,
        'modid'                         => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaPlanejamento $model,$acao)
    {

        $this->arAtributos['hincpf']              = $_SESSION['usucpf'];
        $this->arAtributos['hinacao']             = $acao;
        $this->arAtributos['hindtcriacao']        = date('Y-m-d H:m:s');
        $this->arAtributos['inpid']               = $model->inpid;
        $this->arAtributos['inuid']               = $model->inuid;
        $this->arAtributos['dimid']               = $model->dimid;
        $this->arAtributos['iniid']               = $model->iniid;
        $this->arAtributos['docid']               = $model->docid;
        $this->arAtributos['nivid']               = $model->nivid;
        $this->arAtributos['etaid']               = $model->etaid;
        $this->arAtributos['modid']               = $model->modid;

        $this->arAtributos['inpstatus']           = $model->inpstatus;
        $this->arAtributos['inpsituacaocadastro'] = $model->inpsituacaocadastro;
        $this->arAtributos['inpcronogramamesinicial'] = $model->inpcronogramamesinicial;
        $this->arAtributos['inpcronogramamesfinal'] = $model->inpcronogramamesfinal;
        $this->arAtributos['inpcronogramaano'] = $model->inpcronogramaano;
        $this->arAtributos['inpitemcomposicaodetalhamento'] = $model->inpitemcomposicaodetalhamento;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

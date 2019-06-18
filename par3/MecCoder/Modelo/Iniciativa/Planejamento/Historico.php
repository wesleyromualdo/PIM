<?php
namespace Simec\Par3\Modelo\Iniciativa\Planejamento;

use Simec\Par3\Dado\Iniciativa\Planejamento\Historico as dadosIniciativaPlanejamentoHistorico;
use Simec\Par3\Modelo\Iniciativa\Planejamento\Planejamento as modeloIniciativaPlanejamento;

class Historico extends \Simec\AbstractModelo
{
    /**
     * @var dadosIniciativaPlanejamentoHistorico
     */
    public $dados;

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

    public function __construct()
    {
        parent::__construct();
        $this->dados = new dadosIniciativaPlanejamentoHistorico();
    }

    public function gravarHistorico(modeloIniciativaPlanejamento $model,$acao)
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
        $this->dados = $this->arAtributos;
        $this->commit();
    }
}

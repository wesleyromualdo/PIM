<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_detalhamento_historico.
 *
 * @version $Id$
 * @since 2017.06.12
 */

/**
 * Model_Itenscomposicao_detalhamento_historico: sem descricao
 *
 * @package Model\Itenscomposicao
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Itenscomposicao_detalhamento_historico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Itenscomposicao_detalhamento_historico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $htdid  - default: nextval('par3.itenscomposicao_detalhamento_historico_htdid_seq'::regclass)
 * @property string $htdcpf
 * @property string $htdacao
 * @property \Datetime(Y-m-d H:i:s) $htddtcriacao
 * @property int $itdid  - default: nextval('par3.itenscomposicao_detalhamento_historico_itdid_seq'::regclass)
 * @property int $itcid  - default: nextval('par3.itenscomposicao_detalhamento_historico_itcid_seq'::regclass)
 * @property int $sitid  - default: nextval('par3.itenscomposicao_detalhamento_historico_sitid_seq'::regclass)
 * @property int $atpid  - default: nextval('par3.itenscomposicao_detalhamento_historico_atpid_seq'::regclass)
 * @property "char" $itdpregao
 * @property "char" $itdpregaopublicado
 * @property numeric $itdvalor
 * @property \Datetime(Y-m-d H:i:s) $itdperiodoinicio
 * @property \Datetime(Y-m-d H:i:s) $itdperiodofim
 * @property "char" $itdstatus
 * @property "char" $itdsituacao
 * @property \Datetime(Y-m-d H:i:s) $itddtinclusao
 * @property string $itdcpfinclusao
 * @property \Datetime(Y-m-d H:i:s) $itddtinativacao
 * @property string $itdcpfinativacao
 */
class Par3_Model_ItensComposicaoDetalhamentoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_detalhamento_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'htdid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'atpid' => array('tabela' => 'par3.itenscomposicao_ata_pregao', 'pk' => 'atpid'),
        'sprid' => array('tabela' => 'par.sigarppregao', 'pk' => 'sprid'),
        'itcid' => array('tabela' => 'par3.itenscomposicao', 'pk' => 'itcid'),
        'itdid' => array('tabela' => 'par3.itenscomposicao_detalhamento', 'pk' => 'itdid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'htdid'              => null,
        'htdcpf'             => null,
        'htdacao'            => null,
        'htddtcriacao'       => null,
        'itdid'              => null,
        'itcid'              => null,
        'sprid'              => null,
        'atpid'              => null,
        'itdpregao'          => null,
        'itdpregaopublicado' => null,
        'itdvalor'           => null,
        'itdperiodoinicio'   => null,
        'itdperiodofim'      => null,
        'itdstatus'          => null,
        'itdsituacao'        => null,
        'itddtinclusao'      => null,
        'itdcpfinclusao'     => null,
        'itddtinativacao'    => null,
        'itdcpfinativacao'   => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ItensComposicaoDetalhamento $model,$acao)
    {

        $this->arAtributos['htdcpf']             = $_SESSION['usucpf'];
        $this->arAtributos['htdacao']            = $acao;
        $this->arAtributos['htddtcriacao']       = date('Y-m-d H:m:s');
        $this->arAtributos['itdid']              = $model->itdid;
        $this->arAtributos['itcid']              = $model->itcid;
        $this->arAtributos['sprid']              = $model->sprid;
        $this->arAtributos['atpid']              = $model->atpid;
        $this->arAtributos['itdpregao']          = $model->itdpregao;
        $this->arAtributos['itdpregaopublicado'] = $model->itdpregaopublicado;
        $this->arAtributos['itdvalor']           = $model->itdvalor;
        $this->arAtributos['itdperiodoinicio']   = $model->itdperiodoinicio;
        $this->arAtributos['itdperiodofim']      = $model->itdperiodofim;
        $this->arAtributos['itdstatus']          = $model->itdstatus;
        $this->arAtributos['itdsituacao']        = $model->itdsituacao;
        $this->arAtributos['itddtinclusao']      = $model->itddtinclusao;
        $this->arAtributos['itdcpfinclusao']     = $model->itdcpfinclusao;
        $this->arAtributos['itddtinativacao']    = $model->itddtinativacao;
        $this->arAtributos['itdcpfinativacao']   = $model->itdcpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

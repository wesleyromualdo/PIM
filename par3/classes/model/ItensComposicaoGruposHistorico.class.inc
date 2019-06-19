<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_grupos_historico.
 *
 * @version $Id$
 * @since 2017.06.16
 */

/**
 * Par3_Model_Itenscomposicao_gruposHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Itenscomposicao_gruposHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Itenscomposicao_gruposHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $higid chave primária da tabelaitenscomposicao_grupos_historico
 * @property  $higcpf CPF do respnsável pela alteração
 * @property  $higacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $higdtcriacao Data de criação do log
 * @property int $igrid Chave Primária - default: nextval('par3.itenscomposicao_grupos_igrid_seq'::regclass)
 * @property string $igrnome Nome do grupo de item de composição
 * @property string $igrdsc Descrição do grupo de item de composição
 * @property "char" $igrstatus Status do grupo de item de composição (Soft Delete)
 * @property "char" $igrsituacao Situação do grupo de item de composição
 * @property \Datetime(Y-m-d H:i:s) $igrdtinclusao Data de Inclusão do grupo de item de composição
 * @property string $igrcpfinclusao
 * @property \Datetime(Y-m-d H:i:s) $igrdtinativacao Data de Inativação do grupo de item de composição
 * @property string $igrcpfinativacao CPF do responsável do grupo de item de composição
 */
class Par3_Model_ItenscomposicaoGruposHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_grupos_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'higid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'igrid' => array('tabela' => 'par3.itenscomposicao_grupos', 'pk' => 'igrid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'higid'            => null,
        'higcpf'           => null,
        'higacao'          => null,
        'higdtcriacao'     => null,
        'igrid'            => null,
        'igrnome'          => null,
        'igrdsc'           => null,
        'igrstatus'        => null,
        'igrsituacao'      => null,
        'igrdtinclusao'    => null,
        'igrcpfinclusao'   => null,
        'igrdtinativacao'  => null,
        'igrcpfinativacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ItensComposicaoGrupos $model,$acao)
    {
        $this->arAtributos['higcpf']           = $_SESSION['usucpf'];
        $this->arAtributos['higacao']          = $acao;
        $this->arAtributos['higdtcriacao']     = date('Y-m-d H:m:s');
        $this->arAtributos['igrid']            = $model->igrid;
        $this->arAtributos['igrnome']          = $model->igrnome;
        $this->arAtributos['igrdsc']           = $model->igrdsc;
        $this->arAtributos['igrstatus']        = $model->igrstatus;
        $this->arAtributos['igrsituacao']      = $model->igrsituacao;
        $this->arAtributos['igrdtinclusao']    = $model->igrdtinclusao;
        $this->arAtributos['igrcpfinclusao']   = $model->igrcpfinclusao;
        $this->arAtributos['igrdtinativacao']  = $model->igrdtinativacao;
        $this->arAtributos['igrcpfinativacao'] = $model->igrcpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_historico.
 *
 * @version $Id$
 * @since 2017.06.09
 */

/**
 * Model_ItenscomposicaoHistorico: sem descricao
 *
 * @package Model\ItenscomposicaoHistorico
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_ItenscomposicaoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_ItenscomposicaoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hitid chave primária da tabelaitenscomposicao_historico
 * @property  $hitcpf CPF do respnsável pela alteração
 * @property  $hitacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hitdtcriacao Data de criação do log
 * @property int $itcid Chave Primária - default: nextval('par3.itenscomposicao_itcid_seq'::regclass)
 * @property int $ctdid Chave estrangeira par3.categoriadespesa - default: nextval('par3.itenscomposicao_ctdid_seq'::regclass)
 * @property int $uniid Chave estrangeira par3.unidade_medida - default: nextval('par3.itenscomposicao_uniid_seq'::regclass)
 * @property string $itcdsc
 * @property string $itcespecificacao Especificação Técnica
 * @property "char" $itcstatus Status Item de Composição (Soft Delete)
 * @property "char" $itcsituacao Situação Item de Composição
 * @property \Datetime(Y-m-d H:i:s) $itcdtinclusao Data de inclusão do Item de Composição
 * @property string $itccpfinclusao CPF do responsável pela inclusão de um Item de Composição
 * @property \Datetime(Y-m-d H:i:s) $itcdtinativacao Data de Inativação de um Item de Composição
 * @property string $itccpfinativacao
 */
class Par3_Model_ItensComposicaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hitid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'itcid' => array('tabela' => 'par3.itenscomposicao',       'pk' => 'itcid'),
        'ctdid' => array('tabela' => 'par3.par3.categoriadespesa', 'pk' => 'ctdid'),
        'uniid' => array('tabela' => 'par3.unidade_medida',        'pk' => 'uniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hitid'            => null,
        'hitcpf'           => null,
        'hitacao'          => null,
        'hitdtcriacao'     => null,
        'itcid'            => null,
        'ctdid'            => null,
        'uniid'            => null,
        'itcdsc'           => null,
        'itcespecificacao' => null,
        'itcstatus'        => null,
        'itcsituacao'      => null,
        'itcdtinclusao'    => null,
        'itccpfinclusao'   => null,
        'itcdtinativacao'  => null,
        'itccpfinativacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ItensComposicao $model,$acao)
    {
        $this->arAtributos['hitcpf']           = $_SESSION['usucpf'];
        $this->arAtributos['hitacao']          = $acao;
        $this->arAtributos['hitdtcriacao']     = date('Y-m-d H:m:s');
        $this->arAtributos['itcid']            = $model->itcid;
        $this->arAtributos['ctdid']            = $model->ctdid;
        $this->arAtributos['uniid']            = $model->uniid;
        $this->arAtributos['itcdsc']           = $model->itcdsc;
        $this->arAtributos['itcespecificacao'] = $model->itcespecificacao;
        $this->arAtributos['itcstatus']        = $model->itcstatus;
        $this->arAtributos['itcsituacao']      = $model->itcsituacao;
        $this->arAtributos['itcdtinclusao']    = $model->itcdtinclusao;
        $this->arAtributos['itccpfinclusao']   = $model->itccpfinclusao;
        $this->arAtributos['itcdtinativacao']  = $model->itcdtinativacao;
        $this->arAtributos['itccpfinativacao'] = $model->itccpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

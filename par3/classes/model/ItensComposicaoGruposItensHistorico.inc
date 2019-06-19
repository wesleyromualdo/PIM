<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_grupo_itens_historico.
 *
 * @version $Id$
 * @since 2017.06.16
 */

/**
 * Par3_Model_Itenscomposicao_grupo_itensHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Itenscomposicao_grupo_itensHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Itenscomposicao_grupo_itensHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hgiid chave primária da tabelaitenscomposicao_grupo_itens_historico
 * @property  $hgicpf CPF do respnsável pela alteração
 * @property  $hgiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hgidtcriacao Data de criação do log
 * @property int $gitid  - default: nextval('par3.itenscomposicao_grupo_itens_gitid_seq'::regclass)
 * @property int $igrid
 * @property int $itcid
 * @property int $gitqtd
 */
class Par3_Model_ItensComposicaoGruposItensHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_grupos_itens_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hgiid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'gitid' => array('tabela' => 'par3.itenscomposicao_grupos_itens', 'pk' => 'gitid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hgiid'        => null,
        'hgicpf'       => null,
        'hgiacao'      => null,
        'hgidtcriacao' => null,
        'gitid'        => null,
        'igrid'        => null,
        'itcid'        => null,
        'gitqtd'       => null,
    );

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Itenscomposicao_grupo_itens $model,$acao)
    {
        $this->arAtributos['hgicpf']       = $_SESSION['usucpf'];
        $this->arAtributos['hgiacao']      = $acao;
        $this->arAtributos['hgidtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['gitid']        = $model->gitid;
        $this->arAtributos['igrid']        = $model->igrid;
        $this->arAtributos['itcid']        = $model->itcid;
        $this->arAtributos['gitqtd']       = $model->gitqtd;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

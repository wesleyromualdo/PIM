<?php
/**
 * Classe de mapeamento da entidade par3.unidade_medida_historico.
 *
 * @version $Id$
 * @since 2017.05.29
 */

/**
 * Par3_Model_Unidade_medidaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Unidade_medidaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Unidade_medidaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hunid chave primária da tabelaunidade_medida_historico
 * @property  $huncpf CPF do respnsável pela alteração
 * @property  $hunacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hundtcriacao Data de criação do log
 * @property int $uniid Chave Primária - default: nextval('par3.unidade_medida_uniid_seq'::regclass)
 * @property string $unidsc Descrição da unidade de medida
 * @property "char" $unistatus status da unidade de medida: indidica se a unidade de medida foi removida
 * @property "char" $unisituacao situação da unidade de medida: indidica se a unidade de medida está ativa/inativa
 * @property \Datetime(Y-m-d H:i:s) $unidtinclusao data de inclusão da unidade de medida
 * @property string $unicpfinclusao cpf do responsável pela inclusao da unidade de medida
 * @property \Datetime(Y-m-d H:i:s) $unidtinativacao data de inativação da unidade de medida
 * @property string $unicpfinativacao cpf do responsável pela inativação da unidade de ,medida
 */
class Par3_Model_UnidadeMedidaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.unidade_medida_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hunid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'uniid' => array('tabela' => 'par3.unidade_medida', 'pk' => 'uniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hunid'            => null,
        'huncpf'           => null,
        'hunacao'          => null,
        'hundtcriacao'     => null,
        'uniid'            => null,
        'unidsc'           => null,
        'unistatus'        => null,
        'unisituacao'      => null,
        'unidtinclusao'    => null,
        'unicpfinclusao'   => null,
        'unidtinativacao'  => null,
        'unicpfinativacao' => null,
    );

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_UnidadeMedida $model,$acao)
    {
        $this->arAtributos['huncpf']           = $_SESSION['usucpf'];
        $this->arAtributos['hunacao']          = $acao;
        $this->arAtributos['hundtcriacao']     = date('Y-m-d H:m:s');
        $this->arAtributos['uniid']            = $model->uniid;
        $this->arAtributos['unidsc']           = $model->unidsc;
        $this->arAtributos['unistatus']        = $model->unistatus;
        $this->arAtributos['unisituacao']      = $model->unisituacao;
        $this->arAtributos['unidtinclusao']    = $model->unidtinclusao;
        $this->arAtributos['unicpfinclusao']   = $model->unicpfinclusao;
        $this->arAtributos['unidtinativacao']  = $model->unidtinativacao;
        $this->arAtributos['unicpfinativacao'] = $model->unicpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

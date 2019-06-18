<?php
/**
 * Classe de mapeamento da entidade par3.classe_unidade_medida_historico.
 *
 * @version $Id$
 * @since 2017.05.30
 */


/**
 * Model_Classe_unidade_medidaHistorico: sem descricao
 *
 * @package Model\Classe
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Classe_unidade_medidaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Classe_unidade_medidaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hcnid chave primária da tabelaclasse_unidade_medida_historico
 * @property  $hcncpf CPF do respnsável pela alteração
 * @property  $hcnacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hcndtcriacao Data de criação do log
 * @property int $cniid Chave Primária - default: nextval('par3.classe_unidade_medida_cniid_seq'::regclass)
 * @property string $cnidsc Descrição da classe de unidade de medida
 * @property "char" $cnistatus status da classe de unidade de medida: indidica se a unidade de medida foi removida
 * @property "char" $cnisituacao situação da classe de unidade de medida: indidica se a unidade de medida está ativa/inativa
 * @property \Datetime(Y-m-d H:i:s) $cnidtinclusao data de inclusão da classe de unidade de medida
 * @property string $cnicpfinclusao cpf do responsável pela inclusao da classe de unidade de medida
 * @property \Datetime(Y-m-d H:i:s) $cnidtinativacao data de inativação da classe de unidade de medida
 * @property string $cnicpfinativacao cpf do responsável pela inativação da classe de unidade de ,medida
 */
class Par3_Model_ClasseUnidadeMedidaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.classe_unidade_medida_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hcnid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'cniid' => array('tabela' => 'par3.classe_unidade_medida', 'pk' => 'cniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hcnid' => null,
        'hcncpf' => null,
        'hcnacao' => null,
        'hcndtcriacao' => null,
        'cniid' => null,
        'cnidsc' => null,
        'cnistatus' => null,
        'cnisituacao' => null,
        'cnidtinclusao' => null,
        'cnicpfinclusao' => null,
        'cnidtinativacao' => null,
        'cnicpfinativacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ClasseUnidadeMedida $model,$acao)
    {

        $this->arAtributos['hcncpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hcnacao'] = $acao;
        $this->arAtributos['hcndtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['cniid'] = $model->cniid;
        $this->arAtributos['cnidsc'] = $model->cnidsc;
        $this->arAtributos['cnistatus'] = $model->cnistatus;
        $this->arAtributos['cnisituacao'] = $model->cnisituacao;
        $this->arAtributos['cnidtinclusao'] = $model->cnidtinclusao;
        $this->arAtributos['cnicpfinclusao'] = $model->cnicpfinclusao;
        $this->arAtributos['cnidtinativacao'] = $model->cnidtinativacao;
        $this->arAtributos['cnicpfinativacao'] = $model->cnicpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_areasrelacionadas_programas_historico.
 *
 * @version $Id$
 * @since 2017.06.29
 */

/**
 * Par3_Model_Iniciativa_areasrelacionadas_programasHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_areasrelacionadas_programasHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_areasrelacionadas_programasHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hiaid chave primária da tabelainiciativa_areasrelacionadas_programas_historico
 * @property  $hiacpf CPF do respnsável pela alteração
 * @property  $hiaacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hiadtcriacao Data de criação do log
 * @property int $iapid Chave Primária - default: nextval('par3.iniciativa_areasrelacionadas_programas_iapid_seq'::regclass)
 * @property int $iuoid chave estrangeira par3.iniciativa_unidade_orcamentaria
 * @property int $iarid chave estrangeira par3.iniciativa_areas
 * @property int $prgid chave estrangeira par3.programa
 * @property "char" $iapsituacao situação iniciativa área relacionada
 * @property "char" $iapstatus Status (Soft Delete)
 * @property \Datetime(Y-m-d H:i:s) $iapdtinclusao data de inclusão
 * @property string $iapcpfinclusao cpf inclusão
 * @property \Datetime(Y-m-d H:i:s) $iapdtinativacao data de inativação
 * @property string $iapcpfinativacao cpf inativação
 */
class Par3_Model_IniciativaAreasRelacionadasProgramasHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_areasrelacionadas_programas_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hiaid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iapid' => array('tabela'  => 'par3.iniciativa_areasrelacionadas_programas', 'pk' => 'iapid'),
        'prgid'  => array('tabela' => 'par3.programa',                               'pk' => 'prgid'),
        'iuoid' => array('tabela'  => 'par3.iniciativa_unidade_orcamentaria',        'pk' => 'iuoid'),
        'iarid' => array('tabela'  => 'par3.iniciativa_areas',                       'pk' => 'iarid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hiaid'            => null,
        'hiacpf'           => null,
        'hiaacao'          => null,
        'hiadtcriacao'     => null,
        'iapid'            => null,
        'iuoid'            => null,
        'iarid'            => null,
        'prgid'            => null,
        'iapsituacao'      => null,
        'iapstatus'        => null,
        'iapdtinclusao'    => null,
        'iapcpfinclusao'   => null,
        'iapdtinativacao'  => null,
        'iapcpfinativacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaAreasRelacionadasProgramas $model,$acao)
    {
        $this->arAtributos['hiacpf']           = $_SESSION['usucpf'];
        $this->arAtributos['hiaacao']          = $acao;
        $this->arAtributos['hiadtcriacao']     = date('Y-m-d H:m:s');
        $this->arAtributos['iapid']            = $model->iapid;
        $this->arAtributos['iuoid']            = $model->iuoid;
        $this->arAtributos['iarid']            = $model->iarid;
        $this->arAtributos['prgid']            = $model->prgid;
        $this->arAtributos['iapsituacao']      = $model->iapsituacao;
        $this->arAtributos['iapstatus']        = $model->iapstatus;
        $this->arAtributos['iapdtinclusao']    = $model->iapdtinclusao;
        $this->arAtributos['iapcpfinclusao']   = $model->iapcpfinclusao;
        $this->arAtributos['iapdtinativacao']  = $model->iapdtinativacao;
        $this->arAtributos['iapcpfinativacao'] = $model->iapcpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

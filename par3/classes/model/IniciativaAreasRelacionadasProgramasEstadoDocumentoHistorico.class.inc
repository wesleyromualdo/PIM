<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_areasrelacionadas_programas_estadodocumento_historico.
 *
 * @version $Id$
 * @since 2017.06.28
 */

/**
 * Par3_Model_Iniciativa_areasrelacionadas_programas_estadodocumentoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_areasrelacionadas_programas_estadodocumentoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_areasrelacionadas_programas_estadodocumentoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hiaid chave primária da tabelainiciativa_areasrelacionadas_programas_estadodocumento_historico
 * @property  $hiacpf CPF do respnsável pela alteração
 * @property  $hiaacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hiadtcriacao Data de criação do log
 * @property int $iawid Chave Primária - default: nextval('par3.iniciativa_areasrelacionadas_programas_estadodocument_iawid_seq'::regclass)
 * @property int $iapid chave estrangeira par3.iniciativas_areasrelacionadas_programas
 * @property int $esdid chave estrangeira workflow.estadodocumento
 * @property "char" $iawstatus Status (Soft Delete)
 * @property \Datetime(Y-m-d H:i:s) $iawdtinclusao data de inclusão
 * @property string $iawcpfinclusao cpf inclusão
 * @property \Datetime(Y-m-d H:i:s) $iawdtinativacao data de inativação
 * @property string $iawcpfinativacao cpf inativação
 */
class Par3_Model_IniciativaAreasRelacionadasProgramasEstadoDocumentoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_areasrelacionadas_programas_estadodocumento_historico';

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
        'iawid' => array('tabela' => 'par3.iniciativa_areasrelacionadas_programas_estadodocumento', 'pk' => 'iawid'),
        'esdid' => array('tabela' => 'workflow.estadodocumento',                                    'pk' => 'esdid'),
        'iapid' => array('tabela' => 'par3.iniciativa_areasrelacionadas_programas',                 'pk' => 'iapid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hiaid'            => null,
        'hiacpf'           => null,
        'hiaacao'          => null,
        'hiadtcriacao'     => null,
        'iawid'            => null,
        'iapid'            => null,
        'esdid'            => null,
        'iawstatus'        => null,
        'iawdtinclusao'    => null,
        'iawcpfinclusao'   => null,
        'iawdtinativacao'  => null,
        'iawcpfinativacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaAreasRelacionadasProgramasEstadoDocumento $model,$acao)
    {

        $this->arAtributos['hiacpf']           = $_SESSION['usucpf'];
        $this->arAtributos['hiaacao']          = $acao;
        $this->arAtributos['hiadtcriacao']     = date('Y-m-d H:m:s');
        $this->arAtributos['iawid']            = $model->iawid;
        $this->arAtributos['iapid']            = $model->iapid;
        $this->arAtributos['esdid']            = $model->esdid;
        $this->arAtributos['iawstatus']        = $model->iawstatus;
        $this->arAtributos['iawdtinclusao']    = $model->iawdtinclusao;
        $this->arAtributos['iawcpfinclusao']   = $model->iawcpfinclusao;
        $this->arAtributos['iawdtinativacao']  = $model->iawdtinativacao;
        $this->arAtributos['iawcpfinativacao'] = $model->iawcpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

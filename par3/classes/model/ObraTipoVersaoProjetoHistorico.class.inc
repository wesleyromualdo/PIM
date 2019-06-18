<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_versao_projeto_historico.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_ObraTipoVersaoProjetoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraTipoVersaoProjetoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraTipoVersaoProjetoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hovid chave primária da tabelaobra_tipo_versao_projeto_historico
 * @property  $hovcpf CPF do respnsável pela alteração
 * @property  $hovacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hovdtcriacao Data de criação do log
 * @property int $ovpid  - default: nextval('par3.obra_tipo_versao_projeto_ovpid_seq'::regclass)
 * @property int $otpid
 * @property string $ovpdsc
 * @property string $ovpsituacao
 * @property string $ovpstatus
 * @property float $ovpmetragem
 */
class Par3_Model_ObraTipoVersaoProjetoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_versao_projeto_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hovid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ovpid' => array('tabela' => 'par3.obra_tipo_versao_projeto', 'pk' => 'ovpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hovid' => null,
        'hovcpf' => null,
        'hovacao' => null,
        'hovdtcriacao' => null,
        'ovpid' => null,
        'otpid' => null,
        'ovpdsc' => null,
        'ovpsituacao' => null,
        'ovpstatus' => null,
        'ovpmetragem' => null
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraTipoVersaoProjeto $model,$acao)
    {

        $this->arAtributos['hovcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hovacao'] = $acao;
        $this->arAtributos['hovdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['ovpid'] = $model->ovpid;
        $this->arAtributos['otpid'] = $model->otpid;
        $this->arAtributos['ovpdsc'] = $model->ovpdsc;
        $this->arAtributos['ovpsituacao'] = $model->ovpsituacao;
        $this->arAtributos['ovpstatus'] = $model->ovpstatus;
        $this->arAtributos['ovpmetragem'] = $model->ovpmetragem;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

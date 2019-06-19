<?php
/**
 * Classe de mapeamento da entidade par3.agrupador_historico.
 *
 * @version $Id$
 * @since 2017.10.18
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_AgrupadorHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_AgrupadorHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_AgrupadorHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hagid chave primária da tabelaagrupador_historico
 * @property  $hagcpf CPF do respnsável pela alteração
 * @property  $hagacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hagdtcriacao Data de criação do log
 * @property int $agrid Chave Primária da Tabela par3.agrupador - default: nextval('par3.agrupador_agrid_seq'::regclass)
 * @property string $agrdesc 
 * @property string $agrstatus 
 */
class Par3_Model_AgrupadorHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.agrupador_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hagid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'agrid' => array('tabela' => 'par3.agrupador', 'pk' => 'agrid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hagid' => null,
        'hagcpf' => null,
        'hagacao' => null,
        'hagdtcriacao' => null,
        'agrid' => null,
        'agrdesc' => null,
        'agrstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Agrupador $model,$acao)
    {
    
        $this->arAtributos['hagcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hagacao'] = $acao;
        $this->arAtributos['hagdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['agrid'] = $model->agrid;
        $this->arAtributos['agrdesc'] = $model->agrdesc;
        $this->arAtributos['agrstatus'] = $model->agrstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

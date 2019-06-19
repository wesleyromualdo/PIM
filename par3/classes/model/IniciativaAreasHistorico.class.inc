<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_areas_historico.
 *
 * @version $Id$
 * @since 2017.06.13
 */

/**
 * Par3_Model_Iniciativa_areasHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_areasHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_areasHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hiaid chave primária da tabelainiciativa_areas_historico
 * @property  $hiacpf CPF do respnsável pela alteração
 * @property  $hiaacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hiadtcriacao Data de criação do log
 * @property int $iarid  - default: nextval('par3.iniciativa_areas_iarid_seq'::regclass)
 * @property int $iuoid
 * @property string $iardsc
 * @property string $iuostatus  - default: 'A'::bpchar
 * @property string $iuosituacao  - default: 'A'::bpchar
 */
class Par3_Model_IniciativaAreasHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_areas_historico';

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
        'iarid' => array('tabela' => 'par3.iniciativa_areas', 'pk' => 'iarid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hiaid' => null,
        'hiacpf' => null,
        'hiaacao' => null,
        'hiadtcriacao' => null,
        'iarid' => null,
        'iuoid' => null,
        'iarsigla' => null,
        'iardsc' => null,
        'iarstatus' => null,
        'iarsituacao' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaAreas $model,$acao)
    {

        $this->arAtributos['hiacpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hiaacao'] = $acao;
        $this->arAtributos['hiadtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['iarid'] = $model->iarid;
        $this->arAtributos['iuoid'] = $model->iuoid;
        $this->arAtributos['iarsigla'] = $model->iarsigla;
        $this->arAtributos['iardsc'] = $model->iardsc;
        $this->arAtributos['iarstatus'] = $model->iarstatus;
        $this->arAtributos['iarsituacao'] = $model->iarsituacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

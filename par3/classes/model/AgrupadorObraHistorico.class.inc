<?php
/**
 * Classe de mapeamento da entidade par3.agrupador_obra_historico.
 *
 * @version $Id$
 * @since 2017.10.18
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Agrupador_obraHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Agrupador_obraHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Agrupador_obraHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hagid chave primária da tabelaagrupador_obra_historico
 * @property  $hagcpf CPF do respnsável pela alteração
 * @property  $hagacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hagdtcriacao Data de criação do log
 * @property int $agobid  - default: nextval('par3.agrupador_obra_agobid_seq'::regclass)
 * @property bigint $obrid 
 * @property bigint $agrid 
 */
class Par3_Model_Agrupador_obraHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.agrupador_obra_historico';

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
        'agoid' => array('tabela' => 'par3.agrupador_obra', 'pk' => 'agoid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hagid' => null,
        'hagcpf' => null,
        'hagacao' => null,
        'hagdtcriacao' => null,
        'agobid' => null,
        'obrid' => null,
        'agrid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Agrupador_obra $model,$acao)
    {
    
        $this->arAtributos['hagcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hagacao'] = $acao;
        $this->arAtributos['hagdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['agobid'] = $model->agobid;
        $this->arAtributos['obrid'] = $model->obrid;
        $this->arAtributos['agrid'] = $model->agrid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

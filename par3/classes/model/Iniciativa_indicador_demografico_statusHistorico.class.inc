<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_indicador_demografico_status_historico.
 *
 * @version $Id$
 * @since 2017.07.18
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Iniciativa_indicador_demografico_statusHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_indicador_demografico_statusHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_indicador_demografico_statusHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hinid chave primária da tabelainiciativa_indicador_demografico_status_historico
 * @property  $hincpf CPF do respnsável pela alteração
 * @property  $hinacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hindtcriacao Data de criação do log
 * @property string $iniidsdesc 
 * @property int $iniidsid  - default: nextval('par3.iniciativa_indicador_demografico_status_iniidsid_seq'::regclass)
 */
class Par3_Model_Iniciativa_indicador_demografico_statusHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_indicador_demografico_status_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hinid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iniid' => array('tabela' => 'par3.iniciativa_indicador_demografico_status', 'pk' => 'iniid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hinid' => null,
        'hincpf' => null,
        'hinacao' => null,
        'hindtcriacao' => null,
        'iniidsdesc' => null,
        'iniidsid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Iniciativa_indicador_demografico_status $model,$acao)
    {
    
        $this->arAtributos['hincpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hinacao'] = $acao;
        $this->arAtributos['hindtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['iniidsdesc'] = $model->iniidsdesc;
        $this->arAtributos['iniidsid'] = $model->iniidsid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

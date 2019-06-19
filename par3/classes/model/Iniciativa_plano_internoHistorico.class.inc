<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_plano_interno_historico.
 *
 * @version $Id$
 * @since 2017.10.11
 */

/**
 * Par3_Model_Iniciativa_plano_internoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Yuri Cavalcante <yuri.cavalcante@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_plano_internoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_plano_internoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hinid chave primária da tabelainiciativa_plano_interno_historico
 * @property  $hincpf CPF do respnsável pela alteração
 * @property  $hinacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hindtcriacao Data de criação do log
 * @property string $inplintcpfcriacao 
 * @property \Datetime(Y-m-d H:i:s) $inplintdtcriacao 
 * @property string $inplintsituacao 
 * @property string $inplintstatus 
 * @property int $inplintano 
 * @property int $inplintptres 
 * @property string $inplintdsc 
 * @property int $inplinprog 
 * @property int $inplintid  - default: nextval('par3.iniciativa_plano_interno_inplintid_seq'::regclass)
 */
class Par3_Model_Iniciativa_plano_internoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_plano_interno_historico';

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
        'inpid' => array('tabela' => 'par3.iniciativa_plano_interno', 'pk' => 'inpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hinid' => null,
        'hincpf' => null,
        'hinacao' => null,
        'hindtcriacao' => null,
        'inplintcpfcriacao' => null,
        'inplintdtcriacao' => null,
        'inplintsituacao' => null,
        'inplintstatus' => null,
        'inplintano' => null,
        'inplintptres' => null,
        'inplintdsc' => null,
        'inplinprog' => null,
        'inplintid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Iniciativa_plano_interno $model,$acao)
    {
    
        $this->arAtributos['hincpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hinacao'] = $acao;
        $this->arAtributos['hindtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['inplintcpfcriacao'] = $model->inplintcpfcriacao;
        $this->arAtributos['inplintdtcriacao'] = $model->inplintdtcriacao;
        $this->arAtributos['inplintsituacao'] = $model->inplintsituacao;
        $this->arAtributos['inplintstatus'] = $model->inplintstatus;
        $this->arAtributos['inplintano'] = $model->inplintano;
        $this->arAtributos['inplintptres'] = $model->inplintptres;
        $this->arAtributos['inplintdsc'] = $model->inplintdsc;
        $this->arAtributos['inplinprog'] = $model->inplinprog;
        $this->arAtributos['inplintid'] = $model->inplintid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

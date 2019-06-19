<?php
/**
 * Classe de mapeamento da entidade par3.obra_historico.
 *
 * @version $Id$
 * @since 2017.07.07
 */

/**
 * Par3_Model_ObraHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hobid chave primária da tabelaobra_historico
 * @property  $hobcpf CPF do respnsável pela alteração
 * @property  $hobacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hobdtcriacao Data de criação do log
 * @property int $obrid  - default: nextval('par3.obra_obrid_seq'::regclass)
 * @property int $otpid 
 * @property int $ovpid 
 * @property int $tplid 
 * @property int $inuid 
 * @property string $obrdsc 
 * @property string $obrstatus 
 * @property string $obrcep 
 * @property string $obrlogradouro 
 * @property smallint $obrnumero 
 * @property string $obrcomplemento 
 * @property string $obrponto_referencia 
 * @property string $obrbairro 
 * @property string $obrlatitude 
 * @property string $obrlongitude 
 * @property numeric $obrvalor 
 * @property numeric $obrmetragem
 */
class Par3_Model_ObraHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hobid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'obrid' => array('tabela' => 'par3.obra', 'pk' => 'obrid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hobid' => null,
        'hobcpf' => null,
        'hobacao' => null,
        'hobdtcriacao' => null,
        'obrid' => null,
        'otpid' => null,
        'ovpid' => null,
        'tplid' => null,
        'inuid' => null,
        'obrdsc' => null,
        'obrstatus' => null,
        'obrcep' => null,
        'obrlogradouro' => null,
        'obrnumero' => null,
        'obrcomplemento' => null,
        'obrponto_referencia' => null,
        'obrbairro' => null,
        'obrlatitude' => null,
        'obrlongitude' => null,
        'obrvalor' => null,
        'obrmetragem' => null,
        'obrid'     =>null
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Obra $model,$acao)
    {
        $this->arAtributos['hobcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hobacao'] = $acao;
        $this->arAtributos['hobdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['obrid'] = $model->obrid;
        $this->arAtributos['otpid'] = $model->otpid;
        $this->arAtributos['ovpid'] = $model->ovpid;
        $this->arAtributos['tplid'] = $model->tplid;
        $this->arAtributos['inuid'] = $model->inuid;
        $this->arAtributos['obrdsc'] = $model->obrdsc;
        $this->arAtributos['obrstatus'] = $model->obrstatus;
        $this->arAtributos['obrcep'] = $model->obrcep;
        $this->arAtributos['obrlogradouro'] = $model->obrlogradouro;
        $this->arAtributos['obrnumero'] = $model->obrnumero;
        $this->arAtributos['obrcomplemento'] = $model->obrcomplemento;
        $this->arAtributos['obrponto_referencia'] = $model->obrponto_referencia;
        $this->arAtributos['obrbairro'] = $model->obrbairro;
        $this->arAtributos['obrlatitude'] = $model->obrlatitude;
        $this->arAtributos['obrlongitude'] = $model->obrlongitude;
        $this->arAtributos['obrvalor'] = $model->obrvalor;
        $this->arAtributos['obrmetragem'] = $model->obrmetragem;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.programa_historico.
 *
 * @version $Id$
 * @since 2017.06.26
 */

/**
 * Par3_Model_ProgramaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ProgramaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ProgramaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hprid chave primária da tabelaprograma_historico
 * @property  $hprcpf CPF do respnsável pela alteração
 * @property  $hpracao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hprdtcriacao Data de criação do log
 * @property int $prgid  - default: nextval('par3.programa_prgid_seq'::regclass)
 * @property string $prgdsc 
 * @property string $prgstatus  - default: 'A'::bpchar
 * @property int $pgoid 
 * @property int $pgoid 
 * @property string $prglink 
 * @property int $prgtipoprograma 
 * @property string $prgabreviatura 
 * @property string $prgarquivo  - default: 'padrao.inc'::character varying
 * @property string $prgresumo 
 * @property bool $prgatd_extraordinario 
 * @property string $prgsituacao 
 * @property \Datetime(Y-m-d H:i:s) $prgdtinclusao 
 * @property string $prgcpfinclusao 
 * @property \Datetime(Y-m-d H:i:s) $prgdtinativacao 
 * @property string $prgcpfinativacao 
 * @property \Datetime(Y-m-d H:i:s) $prgperiodoinicio 
 * @property \Datetime(Y-m-d H:i:s) $prgperiodofim 
 * @property string $prgesfera 
 * @property int $arqid 
 * @property string $prganoreferencia 
 */
class Par3_Model_ProgramaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.programa_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hprid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'prgid' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hprid' => null,
        'hprcpf' => null,
        'hpracao' => null,
        'hprdtcriacao' => null,
        'prgid' => null,
        'prgdsc' => null,
        'prgstatus' => null,
        'pgoid' => null,
        'pgoid' => null,
        'prglink' => null,
        'prgtipoprograma' => null,
        'prgabreviatura' => null,
        'prgarquivo' => null,
        'prgresumo' => null,
        'prgatd_extraordinario' => null,
        'prgsituacao' => null,
        'prgdtinclusao' => null,
        'prgcpfinclusao' => null,
        'prgdtinativacao' => null,
        'prgcpfinativacao' => null,
        'prgperiodoinicio' => null,
        'prgperiodofim' => null,
        'prgesfera' => null,
        'arqid' => null,
        'prganoreferencia' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Programa $model,$acao)
    {
    
        $this->arAtributos['hprcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hpracao'] = $acao;
        $this->arAtributos['hprdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['prgid'] = $model->prgid;
        $this->arAtributos['prgdsc'] = $model->prgdsc;
        $this->arAtributos['prgstatus'] = $model->prgstatus;
        $this->arAtributos['pgoid'] = $model->pgoid;
        $this->arAtributos['pgoid'] = $model->pgoid;
        $this->arAtributos['prglink'] = $model->prglink;
        $this->arAtributos['prgtipoprograma'] = $model->prgtipoprograma;
        $this->arAtributos['prgabreviatura'] = $model->prgabreviatura;
        $this->arAtributos['prgarquivo'] = $model->prgarquivo;
        $this->arAtributos['prgresumo'] = $model->prgresumo;
        $this->arAtributos['prgatd_extraordinario'] = $model->prgatd_extraordinario;
        $this->arAtributos['prgsituacao'] = $model->prgsituacao;
        $this->arAtributos['prgdtinclusao'] = $model->prgdtinclusao;
        $this->arAtributos['prgcpfinclusao'] = $model->prgcpfinclusao;
        $this->arAtributos['prgdtinativacao'] = $model->prgdtinativacao;
        $this->arAtributos['prgcpfinativacao'] = $model->prgcpfinativacao;
        $this->arAtributos['prgperiodoinicio'] = $model->prgperiodoinicio;
        $this->arAtributos['prgperiodofim'] = $model->prgperiodofim;
        $this->arAtributos['prgesfera'] = $model->prgesfera;
        $this->arAtributos['arqid'] = $model->arqid;
        $this->arAtributos['prganoreferencia'] = $model->prganoreferencia;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

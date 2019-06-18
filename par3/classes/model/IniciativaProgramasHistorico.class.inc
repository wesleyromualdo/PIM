<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_programas_historico.
 *
 * @version $Id$
 * @since 2017.04.17
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Iniciativa_programas_historico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Anisio Neto <anisioneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_programas_historico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_programas_historico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hid chave primária da tabelainiciativa_programas_historico
 * @property  $hcpf CPF do respnsável pela alteração
 * @property  $hacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hdtcriacao Data de criação do log
 * @property int $inprgid  - default: nextval('par3.iniciativa_programas_inprgid_seq'::regclass)
 * @property string $inprgsigla
 * @property string $inprgdsc
 * @property string $inprglink
 * @property string $inprgresumo
 * @property bool $inprgatd_extraordinario
 * @property bool $inprgstatus
 * @property bool $inprgsituacao
 * @property \Datetime(Y-m-d H:i:s) $inprgdtinclucao
 * @property string $inprgcpfinclusao
 * @property \Datetime(Y-m-d H:i:s) $inprgdtinativacao
 * @property string $inprgcpfinativacao
 */
class Par3_Model_IniciativaProgramasHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.programa_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hpgid',
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
        'hpgid'                 => null,
        'hpgcpf'                => null,
        'hpgacao'               => null,
        'hpgdtcriacao'          => null,
        'prgid'                 => null,
        'pgoid'                 => null,
        'prgabreviatura'        => null,
        'prgdsc'                => null,
        'prglink'               => null,
        'prgabreviatura'        => null,
        'prgarquivo'            => null,
        'prgresumo'             => null,
        'prgatd_extraordinario' => null,
        'prgsituacao'           => null,
        'prgdtinclusao'         => null,
        'prgcpfinclusao'        => null,
        'prgdtinativacao'       => null,
        'prgcpfinativacao'      => null,
        'prgperiodoinicio'      => null,
        'prgperiodofim'         => null,
        'prgesfera'             => null,
        'prganoreferencia'      => null,
        'arqid'                 => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaProgramas $model,$acao)
    {

        $this->arAtributos['hpgcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hpgacao'] = $acao;
        $this->arAtributos['hpgdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['prgid'] = $model->prgid;
        $this->arAtributos['prgabreviatura'] = $model->prgabreviatura;
        $this->arAtributos['prgdsc'] = $model->prgdsc;
        $this->arAtributos['pgoid'] = $model->pgoid;
        $this->arAtributos['prglink'] = $model->prglink;
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
        $this->arAtributos['prganoreferencia'] = $model->prganoreferencia;
        $this->arAtributos['arqid'] = $model->arqid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_historico.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_Obra_tipoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Junio Pereira Dos Santos <junio.santos@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_tipoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_tipoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hotid chave primária da tabelaobra_tipo_historico
 * @property  $hotcpf CPF do respnsável pela alteração
 * @property  $hotacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hotdtcriacao Data de criação do log
 * @property int $otpid  - default: nextval('par3.obra_tipo_otpid_seq'::regclass)
 * @property int $octid 
 * @property string $otpdsc 
 * @property string $otpsituacao 
 * @property string $otpstatus 
 * @property string $otpesfera
 * @property string $otpdetalhe
 * @property smallint $otpalunosatendidos 
 */
class Par3_Model_ObraTipoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hotid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'otpid' => array('tabela' => 'par3.obra_tipo', 'pk' => 'otpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hotid' => null,
        'hotcpf' => null,
        'hotacao' => null,
        'hotdtcriacao' => null,
        'otpid' => null,
        'octid' => null,
        'otpdsc' => null,
        'otpsituacao' => null,
        'otpstatus' => null,
        'otpesfera' => null,
        'otpalunosatendidos' => null,
        'tpoid' => null,
        'otpdetalhe' => null, //#add
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraTipo $model,$acao)
    {
    
        $this->arAtributos['hotcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hotacao'] = $acao;
        $this->arAtributos['hotdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['otpid'] = $model->otpid;
        $this->arAtributos['octid'] = $model->octid;
        $this->arAtributos['otpdsc'] = $model->otpdsc;
        $this->arAtributos['otpsituacao'] = $model->otpsituacao;
        $this->arAtributos['otpstatus'] = $model->otpstatus;
        $this->arAtributos['otpesfera'] = $model->otpesfera;
        $this->arAtributos['otpalunosatendidos'] = $model->otpalunosatendidos;
        $this->arAtributos['tpoid'] = $model->tpoid;
        $this->arAtributos['otpdetalhe'] = $model->otpdetalhe;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

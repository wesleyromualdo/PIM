<?php
/**
 * Classe de mapeamento da entidade par3.modalidade_historico.
 *
 * @version $Id$
 * @since 2017.04.17
 */

/**
 * Par3_Model_ModalidadeHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ModalidadeHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ModalidadeHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hid chave primária da tabelamodalidade_historico
 * @property  $hcpf CPF do respnsável pela alteração
 * @property  $hacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hdtcriacao Data de criação do log
 * @property int $modid  - default: nextval('par3.modalidade_modid_seq'::regclass)
 * @property string $moddsc 
 * @property "char" $modstatus 
 * @property "char" $modsituacao 
 * @property \Datetime(Y-m-d H:i:s) $moddtinclusao 
 * @property string $modcpfinclusao 
 * @property \Datetime(Y-m-d H:i:s) $moddtinativacao 
 * @property string $modcpfinativacao 
 */
class Par3_Model_ModalidadeHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.modalidade_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hmoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'modid' => array('tabela' => 'par3.modalidade', 'pk' => 'modid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hmoid' => null,
        'hmocpf' => null,
        'hmoacao' => null,
        'hmodtcriacao' => null,
        'modid' => null,
        'moddsc' => null,
        'modstatus' => null,
        'modsituacao' => null,
        'moddtinclusao' => null,
        'modcpfinclusao' => null,
        'moddtinativacao' => null,
        'modcpfinativacao' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Modalidade $model,$acao)
    {
    
        $this->arAtributos['hmocpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hmoacao'] = $acao;
        $this->arAtributos['hmodtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['modid'] = $model->modid;
        $this->arAtributos['moddsc'] = $model->moddsc;
        $this->arAtributos['modstatus'] = $model->modstatus;
        $this->arAtributos['modsituacao'] = $model->modsituacao;
        $this->arAtributos['moddtinclusao'] = $model->moddtinclusao;
        $this->arAtributos['modcpfinclusao'] = $model->modcpfinclusao;
        $this->arAtributos['moddtinativacao'] = $model->moddtinativacao;
        $this->arAtributos['modcpfinativacao'] = $model->modcpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

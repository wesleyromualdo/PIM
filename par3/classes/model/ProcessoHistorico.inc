<?php
/**
 * Classe de mapeamento da entidade par3.processo_historico.
 *
 * @version $Id$
 * @since 2018.06.22
 */

/**
 * Model_ProcessoHistorico: sem descricao
 *
 * @package Model\ProcessoHistorico
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_ProcessoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_ProcessoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hinid chave primária da tabelaprocesso_historico
 * @property  $hincpf CPF do respnsável pela alteração
 * @property  $hinacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hindtcriacao Data de criação do log
 * @property int $intaid 
 * @property \Datetime(Y-m-d H:i:s) $prodatapagamentosigef Data da ultima consulta na base de historico de pagamentos do SIGEF
 * @property bool $profinalizado indica se o registro está finalizado
 * @property \Datetime(Y-m-d H:i:s) $prodataconsultasigef Data da ultima consulta na base de historico do SIGEF
 * @property string $prostatusmotivo Status A - Ativo I - Inativo
 * @property int $arqidanexodoc Chave estrangeira da tabela public.arquivo
 * @property string $ds_razao_social Descrição da razão social
 * @property string $nu_identificador numero identificador da conta corrente
 * @property string $situacao_conta descrição da situação da conta corrente
 * @property string $co_situacao_conta codigo da situação da conta corrente
 * @property string $fase_solicitacao fase de solicitação
 * @property string $dt_movimento data da movimentação do processo
 * @property string $prostatus Status A - Ativo I - Inativo - default: 'A'::character varying
 * @property string $procnpj cnpj da entidade vinculada ao processo
 * @property string $prodocumenta Quarda o número do processo do documento no FNDE. O FNDE utiliza este número para recuperar aonde o processo físico se encontra.
 * @property string $pronumeroconveniosiafi numero do convenio siaf
 * @property int $proanoconveniofnde ano do convenio do fnde
 * @property string $pronumeroconveniofnde Numero do convenio do fnde
 * @property string $protipoexecucao Tipo de execução do processo
 * @property int $inuid Chave estrangeira da tabela par3.instrumentounidade
 * @property string $nu_conta_corrente numero da conta corrente
 * @property string $seq_conta_corrente sequencial da conta corrente
 * @property string $protipo tipo do processo - default: 'P'::bpchar
 * @property string $proseqconta nr da conta
 * @property string $usucpf Chave estrangeira da tabela seguranca.usuario
 * @property \Datetime(Y-m-d H:i:s) $prodatainclusao Data da inclusão do registro
 * @property string $proagencia codigo da agencia
 * @property string $probanco codigo do banco
 * @property string $muncod codigo do município
 * @property string $pronumeroprocesso numero do processo
 * @property int $proid Chave primaria da tabela par3.processo - default: nextval('par3.processo_proid_seq'::regclass)
 */
class Par3_Model_ProcessoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.processo_historico';

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
        'proid' => array('tabela' => 'par3.processo', 'pk' => 'proid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hinid' => null,
        'hincpf' => null,
        'hinacao' => null,
        'hindtcriacao' => null,
        'intaid' => null,
        'prodatapagamentosigef' => null,
        'profinalizado' => null,
        'prodataconsultasigef' => null,
        'prostatusmotivo' => null,
        'arqidanexodoc' => null,
        'ds_razao_social' => null,
        'nu_identificador' => null,
        'situacao_conta' => null,
        'co_situacao_conta' => null,
        'fase_solicitacao' => null,
        'dt_movimento' => null,
        'prostatus' => null,
        'procnpj' => null,
        'prodocumenta' => null,
        'pronumeroconveniosiafi' => null,
        'proanoconveniofnde' => null,
        'pronumeroconveniofnde' => null,
        'protipoexecucao' => null,
        'inuid' => null,
        'nu_conta_corrente' => null,
        'seq_conta_corrente' => null,
        'protipo' => null,
        'proseqconta' => null,
        'usucpf' => null,
        'prodatainclusao' => null,
        'proagencia' => null,
        'probanco' => null,
        'muncod' => null,
        'pronumeroprocesso' => null,
        'proid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Processo $model,$acao)
    {
    
        $this->arAtributos['hincpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hinacao'] = $acao;
        $this->arAtributos['hindtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['intaid'] = $model->intaid;
        $this->arAtributos['prodatapagamentosigef'] = $model->prodatapagamentosigef;
        $this->arAtributos['profinalizado'] = $model->profinalizado;
        $this->arAtributos['prodataconsultasigef'] = $model->prodataconsultasigef;
        $this->arAtributos['prostatusmotivo'] = $model->prostatusmotivo;
        $this->arAtributos['arqidanexodoc'] = $model->arqidanexodoc;
        $this->arAtributos['ds_razao_social'] = $model->ds_razao_social;
        $this->arAtributos['nu_identificador'] = $model->nu_identificador;
        $this->arAtributos['situacao_conta'] = $model->situacao_conta;
        $this->arAtributos['co_situacao_conta'] = $model->co_situacao_conta;
        $this->arAtributos['fase_solicitacao'] = $model->fase_solicitacao;
        $this->arAtributos['dt_movimento'] = $model->dt_movimento;
        $this->arAtributos['prostatus'] = $model->prostatus;
        $this->arAtributos['procnpj'] = $model->procnpj;
        $this->arAtributos['prodocumenta'] = $model->prodocumenta;
        $this->arAtributos['pronumeroconveniosiafi'] = $model->pronumeroconveniosiafi;
        $this->arAtributos['proanoconveniofnde'] = $model->proanoconveniofnde;
        $this->arAtributos['pronumeroconveniofnde'] = $model->pronumeroconveniofnde;
        $this->arAtributos['protipoexecucao'] = $model->protipoexecucao;
        $this->arAtributos['inuid'] = $model->inuid;
        $this->arAtributos['nu_conta_corrente'] = $model->nu_conta_corrente;
        $this->arAtributos['seq_conta_corrente'] = $model->seq_conta_corrente;
        $this->arAtributos['protipo'] = $model->protipo;
        $this->arAtributos['proseqconta'] = $model->proseqconta;
        $this->arAtributos['usucpf'] = $model->usucpf;
        $this->arAtributos['prodatainclusao'] = $model->prodatainclusao;
        $this->arAtributos['proagencia'] = $model->proagencia;
        $this->arAtributos['probanco'] = $model->probanco;
        $this->arAtributos['muncod'] = $model->muncod;
        $this->arAtributos['pronumeroprocesso'] = $model->pronumeroprocesso;
        $this->arAtributos['proid'] = $model->proid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

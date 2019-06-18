<?php
/**
 * Classe de mapeamento da entidade par3.analise_historico_historico.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_AnaliseHistoricoHistorico: sem descricao
 *
 * @package Model\AnaliseHistoricoHistorico
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_AnaliseHistoricoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_AnaliseHistoricoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hanid chave primária da tabelaanalise_historico_historico
 * @property  $hancpf CPF do respnsável pela alteração
 * @property  $hanacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $handtcriacao Data de criação do log
 * @property int $anhid  - default: nextval('par3.analise_historico_anhid_seq'::regclass)
 * @property int $anaid Chave primaria da tabela par3.analise
 * @property int $inpid FK da tabela par3.iniciativa - default: nextval('par3.analise_historico_inpid_seq'::regclass)
 * @property int $anaano Ano da analise
 * @property string $anaconsideracaoent Considerações sobre a Entidade Proponente
 * @property string $anaconsideracaoprop Considerações sobre a Proposta
 * @property string $anaconsideracaoproj Considerações sobre a Projeto
 * @property string $anaconsideracaoobj Considerações sobre o Objetivo
 * @property string $anaconsideracaojus Considerações sobre a Justificativa
 * @property string $anaconsideracaoval Considerações sobre os Valores
 * @property string $anaoutrasconsideracoes Outras Considerações Cabíveis
 * @property string $anaparecer Parecer. "A" - aprovado; "R" - Rejeitado
 * @property string $anatextoparecer Texto do parecer pré definido no sistema
 * @property string $cpftecnico FK da tabela seguranca.usuario
 * @property string $cpfcoordenador FK da tabela seguranca.usuario
 * @property int $docid FK da tabela workflow.documento
 * @property \Datetime(Y-m-d H:i:s) $anadatacriacao Data de criação da analise
 * @property \Datetime(Y-m-d H:i:s) $anadataalteracao Data de atualização da analise
 * @property string $usucpfcriacao CPF do responsável por efetuar a tramitação da análise.
 * @property string $anastatus Status
 */
class Par3_Model_AnalisePlanejamentoIniciativaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.analise_planejamento_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hanid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'anaid' => array('tabela' => 'par3.analise', 'pk' => 'anaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hanid' => null,
        'hancpf' => null,
        'hanacao' => null,
        'handtcriacao' => null,
        'anaid' => null,
        'inpid' => null,
        'anaano' => null,
        'anaconsideracaoent' => null,
        'anaconsideracaoprop' => null,
        'anaconsideracaoproj' => null,
        'anaconsideracaoobj' => null,
        'anaconsideracaojus' => null,
        'anaconsideracaoval' => null,
        'anaoutrasconsideracoes' => null,
        'anaparecer' => null,
        'anatextoparecer' => null,
        'cpftecnico' => null,
        'cpfcoordenador' => null,
        'docid' => null,
        'anadatacriacao' => null,
        'anadataalteracao' => null,
        'usucpfcriacao' => null,
        'anastatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_AnalisePlanejamentoIniciativa $model,$acao)
    {

        $this->arAtributos['hancpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hanacao'] = $acao;
        $this->arAtributos['handtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['anaid'] = $model->anaid;
        $this->arAtributos['inpid'] = $model->inpid;
        $this->arAtributos['anaano'] = $model->anaano;
        $this->arAtributos['anaconsideracaoent'] = $model->anaconsideracaoent;
        $this->arAtributos['anaconsideracaoprop'] = $model->anaconsideracaoprop;
        $this->arAtributos['anaconsideracaoproj'] = $model->anaconsideracaoproj;
        $this->arAtributos['anaconsideracaoobj'] = $model->anaconsideracaoobj;
        $this->arAtributos['anaconsideracaojus'] = $model->anaconsideracaojus;
        $this->arAtributos['anaconsideracaoval'] = $model->anaconsideracaoval;
        $this->arAtributos['anaoutrasconsideracoes'] = $model->anaoutrasconsideracoes;
        $this->arAtributos['anaparecer'] = $model->anaparecer;
        $this->arAtributos['anatextoparecer'] = $model->anatextoparecer;
        $this->arAtributos['cpftecnico'] = $model->cpftecnico;
        $this->arAtributos['cpfcoordenador'] = $model->cpfcoordenador;
        $this->arAtributos['docid'] = $model->docid;
        $this->arAtributos['anadatacriacao'] = $model->anadatacriacao;
        $this->arAtributos['anadataalteracao'] = $model->anadataalteracao;
        $this->arAtributos['anastatus'] = $model->anastatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

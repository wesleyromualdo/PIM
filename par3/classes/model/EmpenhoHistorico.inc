<?php
/**
 * Classe de mapeamento da entidade par3.empenho_historico.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_EmpenhoHistorico: sem descricao
 *
 * @package Model\EmpenhoHistorico
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_EmpenhoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_EmpenhoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hemid chave primária da tabelaempenho_historico
 * @property  $hemcpf CPF do respnsável pela alteração
 * @property  $hemacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hemdtcriacao Data de criação do log
 * @property int $empid Chave primaria da tabela par3.empenho - default: nextval('par3.empenho_empid_seq'::regclass)
 * @property string $empcnpj CNPJ do favorecido
 * @property numeric $empnumerooriginal Numero do empenho original
 * @property numeric $empanooriginal Ano do empenho original
 * @property string $empnumeroprocesso Número do processo do empenho
 * @property string $empcodigoespecie clasificação do empenho
 * @property string $empcodigopi Código do PI
 * @property numeric $empcodigoesfera Código da esfera orçamentaris 1- Fiscal , 2- Seguridade
 * @property string $empcodigoptres codigo PTRES
 * @property string $empfonterecurso código da Fonte de recurso
 * @property numeric $empcodigonatdespesa Código da natureza de despesa
 * @property string $empcentrogestaosolic Código de centro de gestão solicitante
 * @property numeric $empanoconvenio Ano do convênio
 * @property numeric $empnumeroconvenio número do convênio
 * @property string $empcodigoobs Codigo da observação do convênio
 * @property string $empcodigotipo tipo de empenho
 * @property string $empdescricao Descrição da empenho
 * @property string $empgestaoeminente Gestão Eminente
 * @property string $empunidgestoraeminente Codgio da unidade gestora
 * @property string $empprogramafnde Programa FNDE
 * @property string $empnumerosistema Codigo do Sistema
 * @property string $empsituacao Descrição da situação
 * @property string $usucpf Chave estrangeira da tabela seguranca.usuario
 * @property string $empprotocolo nr do protocolo
 * @property string $empnumero numero do empenho
 * @property numeric $empvalorempenho Valor do empenho
 * @property string $ds_problema Descrição do problema
 * @property numeric $valor_total_empenhado Valor empenhado
 * @property numeric $valor_saldo_pagamento Saldo do pagamento
 * @property \Datetime(Y-m-d H:i:s) $empdata Data do Empenho - default: now()
 * @property string $tp_especializacao Tipo de especialização
 * @property string $co_diretoria Codigo da diretoria
 * @property int $empidpai Liga a chave primária para vincular um empenho do tipo cancelamento, reforço, anulação e empenho parcial ao empenho original.
 * @property int $teeid cheve estrangeira da tabela execucaofinanceira.tipoespecieempenho
 * @property string $empcarga Indica se o empenho foi feito por carga - default: 'N'::bpchar
 * @property string $empstatus coluna utilizada para cancelar processo quando não estão no SIGEF - default: 'A'::bpchar
 * @property \Datetime(Y-m-d H:i:s) $empdataatualizacao Data de atualização do empenho pela rotina noturna
 */
class Par3_Model_EmpenhoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.empenho_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hemid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'empid' => array('tabela' => 'par3.empenho', 'pk' => 'empid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hemid' => null,
        'hemcpf' => null,
        'hemacao' => null,
        'hemdtcriacao' => null,
        'empid' => null,
        'empcnpj' => null,
        'empnumerooriginal' => null,
        'empanooriginal' => null,
        'empnumeroprocesso' => null,
        'empcodigoespecie' => null,
        'empcodigopi' => null,
        'empcodigoesfera' => null,
        'empcodigoptres' => null,
        'empfonterecurso' => null,
        'empcodigonatdespesa' => null,
        'empcentrogestaosolic' => null,
        'empanoconvenio' => null,
        'empnumeroconvenio' => null,
        'empcodigoobs' => null,
        'empcodigotipo' => null,
        'empdescricao' => null,
        'empgestaoeminente' => null,
        'empunidgestoraeminente' => null,
        'empprogramafnde' => null,
        'empnumerosistema' => null,
        'empsituacao' => null,
        'usucpf' => null,
        'empprotocolo' => null,
        'empnumero' => null,
        'empvalorempenho' => null,
        'ds_problema' => null,
        'valor_total_empenhado' => null,
        'valor_saldo_pagamento' => null,
        'empdata' => null,
        'tp_especializacao' => null,
        'co_diretoria' => null,
        'empidpai' => null,
        'teeid' => null,
        'empcarga' => null,
        'empstatus' => null,
        'empdataatualizacao' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Empenho $model,$acao)
    {
    
        $this->arAtributos['hemcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hemacao'] = $acao;
        $this->arAtributos['hemdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['empid'] = $model->empid;
        $this->arAtributos['empcnpj'] = $model->empcnpj;
        $this->arAtributos['empnumerooriginal'] = $model->empnumerooriginal;
        $this->arAtributos['empanooriginal'] = $model->empanooriginal;
        $this->arAtributos['empnumeroprocesso'] = $model->empnumeroprocesso;
        $this->arAtributos['empcodigoespecie'] = $model->empcodigoespecie;
        $this->arAtributos['empcodigopi'] = $model->empcodigopi;
        $this->arAtributos['empcodigoesfera'] = $model->empcodigoesfera;
        $this->arAtributos['empcodigoptres'] = $model->empcodigoptres;
        $this->arAtributos['empfonterecurso'] = $model->empfonterecurso;
        $this->arAtributos['empcodigonatdespesa'] = $model->empcodigonatdespesa;
        $this->arAtributos['empcentrogestaosolic'] = $model->empcentrogestaosolic;
        $this->arAtributos['empanoconvenio'] = $model->empanoconvenio;
        $this->arAtributos['empnumeroconvenio'] = $model->empnumeroconvenio;
        $this->arAtributos['empcodigoobs'] = $model->empcodigoobs;
        $this->arAtributos['empcodigotipo'] = $model->empcodigotipo;
        $this->arAtributos['empdescricao'] = $model->empdescricao;
        $this->arAtributos['empgestaoeminente'] = $model->empgestaoeminente;
        $this->arAtributos['empunidgestoraeminente'] = $model->empunidgestoraeminente;
        $this->arAtributos['empprogramafnde'] = $model->empprogramafnde;
        $this->arAtributos['empnumerosistema'] = $model->empnumerosistema;
        $this->arAtributos['empsituacao'] = $model->empsituacao;
        $this->arAtributos['usucpf'] = $model->usucpf;
        $this->arAtributos['empprotocolo'] = $model->empprotocolo;
        $this->arAtributos['empnumero'] = $model->empnumero;
        $this->arAtributos['empvalorempenho'] = $model->empvalorempenho;
        $this->arAtributos['ds_problema'] = $model->ds_problema;
        $this->arAtributos['valor_total_empenhado'] = $model->valor_total_empenhado;
        $this->arAtributos['valor_saldo_pagamento'] = $model->valor_saldo_pagamento;
        $this->arAtributos['empdata'] = $model->empdata;
        $this->arAtributos['tp_especializacao'] = $model->tp_especializacao;
        $this->arAtributos['co_diretoria'] = $model->co_diretoria;
        $this->arAtributos['empidpai'] = $model->empidpai;
        $this->arAtributos['teeid'] = $model->teeid;
        $this->arAtributos['empcarga'] = $model->empcarga;
        $this->arAtributos['empstatus'] = $model->empstatus;
        $this->arAtributos['empdataatualizacao'] = $model->empdataatualizacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

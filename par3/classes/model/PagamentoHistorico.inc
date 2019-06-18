<?php
/**
 * Classe de mapeamento da entidade par3.pagamento_historico.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_PagamentoHistorico: sem descricao
 *
 * @package Model\PagamentoHistorico
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_PagamentoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_PagamentoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hpaid chave primária da tabelapagamento_historico
 * @property  $hpacpf CPF do respnsável pela alteração
 * @property  $hpaacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hpadtcriacao Data de criação do log
 * @property int $pagid  - default: nextval('par3.pagamento_pagid_seq'::regclass)
 * @property numeric $pagparcela 
 * @property numeric $paganoexercicio 
 * @property numeric $pagvalorparcela 
 * @property numeric $paganoparcela 
 * @property numeric $pagmes 
 * @property string $pagnumeroempenho 
 * @property int $empid 
 * @property string $usucpf 
 * @property string $pagsituacaopagamento 
 * @property \Datetime(Y-m-d H:i:s) $pagdatapagamento 
 * @property int $pagnumseqob 
 * @property string $pagstatus  - default: 'A'::bpchar
 * @property \Datetime(Y-m-d H:i:s) $pagdatapagamentosiafi 
 * @property string $pagnumeroob 
 * @property string $pagcarga  - default: 'N'::bpchar
 * @property string $pagverificacarga  - default: 'A'::bpchar
 * @property string $pagusucpfexclusao 
 * @property \Datetime(Y-m-d H:i:s) $pagdataexclusao 
 * @property \Datetime(Y-m-d H:i:s) $pagdataatualizacao 
 * @property string $pagjustificaitiva 
 */
class Par3_Model_PagamentoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.pagamento_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hpaid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pagid' => array('tabela' => 'par3.pagamento', 'pk' => 'pagid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hpaid' => null,
        'hpacpf' => null,
        'hpaacao' => null,
        'hpadtcriacao' => null,
        'pagid' => null,
        'pagparcela' => null,
        'paganoexercicio' => null,
        'pagvalorparcela' => null,
        'paganoparcela' => null,
        'pagmes' => null,
        'pagnumeroempenho' => null,
        'empid' => null,
        'usucpf' => null,
        'pagsituacaopagamento' => null,
        'pagdatapagamento' => null,
        'pagnumseqob' => null,
        'pagstatus' => null,
        'pagdatapagamentosiafi' => null,
        'pagnumeroob' => null,
        'pagcarga' => null,
        'pagverificacarga' => null,
        'pagusucpfexclusao' => null,
        'pagdataexclusao' => null,
        'pagdataatualizacao' => null,
        'pagjustificaitiva' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Pagamento $model,$acao)
    {
    
        $this->arAtributos['hpacpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hpaacao'] = $acao;
        $this->arAtributos['hpadtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['pagid'] = $model->pagid;
        $this->arAtributos['pagparcela'] = $model->pagparcela;
        $this->arAtributos['paganoexercicio'] = $model->paganoexercicio;
        $this->arAtributos['pagvalorparcela'] = $model->pagvalorparcela;
        $this->arAtributos['paganoparcela'] = $model->paganoparcela;
        $this->arAtributos['pagmes'] = $model->pagmes;
        $this->arAtributos['pagnumeroempenho'] = $model->pagnumeroempenho;
        $this->arAtributos['empid'] = $model->empid;
        $this->arAtributos['usucpf'] = $model->usucpf;
        $this->arAtributos['pagsituacaopagamento'] = $model->pagsituacaopagamento;
        $this->arAtributos['pagdatapagamento'] = $model->pagdatapagamento;
        $this->arAtributos['pagnumseqob'] = $model->pagnumseqob;
        $this->arAtributos['pagstatus'] = $model->pagstatus;
        $this->arAtributos['pagdatapagamentosiafi'] = $model->pagdatapagamentosiafi;
        $this->arAtributos['pagnumeroob'] = $model->pagnumeroob;
        $this->arAtributos['pagcarga'] = $model->pagcarga;
        $this->arAtributos['pagverificacarga'] = $model->pagverificacarga;
        $this->arAtributos['pagusucpfexclusao'] = $model->pagusucpfexclusao;
        $this->arAtributos['pagdataexclusao'] = $model->pagdataexclusao;
        $this->arAtributos['pagdataatualizacao'] = $model->pagdataatualizacao;
        $this->arAtributos['pagjustificaitiva'] = $model->pagjustificaitiva;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

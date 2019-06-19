<?php
/**
 * Classe de mapeamento da entidade par3.pagamentoobracomposicao_historico.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_PagamentoobracomposicaoHistorico: sem descricao
 *
 * @package Model\PagamentoobracomposicaoHistorico
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_PagamentoObraComposicaoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_PagamentoObraComposicaoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hpmid chave primária da tabelapagamentoobracomposicao_historico
 * @property  $hpmcpf CPF do respnsável pela alteração
 * @property  $hpmacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hpmdtcriacao Data de criação do log
 * @property int $pmcid  - default: nextval('par3.pagamentoobracomposicao_pmcid_seq'::regclass)
 * @property int $pagid 
 * @property int $eocid 
 * @property numeric $pmcpercentualpag 
 * @property numeric $pmcvalorpagamento 
 * @property string $pmcstatus  - default: 'A'::bpchar
 */
class Par3_Model_PagamentoObraComposicaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.pagamentoobracomposicao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hpmid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pmcid' => array('tabela' => 'par3.pagamentoobracomposicao', 'pk' => 'pmcid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hpmid' => null,
        'hpmcpf' => null,
        'hpmacao' => null,
        'hpmdtcriacao' => null,
        'pmcid' => null,
        'pagid' => null,
        'eocid' => null,
        'pmcpercentualpag' => null,
        'pmcvalorpagamento' => null,
        'pmcstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_PagamentoObraComposicao $model,$acao)
    {
    
        $this->arAtributos['hpmcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hpmacao'] = $acao;
        $this->arAtributos['hpmdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['pmcid'] = $model->pmcid;
        $this->arAtributos['pagid'] = $model->pagid;
        $this->arAtributos['eocid'] = $model->eocid;
        $this->arAtributos['pmcpercentualpag'] = $model->pmcpercentualpag;
        $this->arAtributos['pmcvalorpagamento'] = $model->pmcvalorpagamento;
        $this->arAtributos['pmcstatus'] = $model->pmcstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

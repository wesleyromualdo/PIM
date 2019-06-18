<?php
/**
 * Classe de mapeamento da entidade par3.pagamentoparcomposicao_historico.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_PagamentoparcomposicaoHistorico: sem descricao
 *
 * @package Model\PagamentoparcomposicaoHistorico
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_PagamentoParComposicaoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_PagamentoParComposicaoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hppid chave primária da tabelapagamentoparcomposicao_historico
 * @property  $hppcpf CPF do respnsável pela alteração
 * @property  $hppacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hppdtcriacao Data de criação do log
 * @property int $ppcid  - default: nextval('par3.pagamentoparcomposicao_ppcid_seq'::regclass)
 * @property int $pagid 
 * @property int $epcid 
 * @property numeric $ppcpercentualpag 
 * @property numeric $ppcvalorpagamento 
 * @property string $ppcstatus  - default: 'A'::bpchar
 */
class Par3_Model_PagamentoParComposicaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.pagamentoparcomposicao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hppid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ppcid' => array('tabela' => 'par3.pagamentoparcomposicao', 'pk' => 'ppcid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hppid' => null,
        'hppcpf' => null,
        'hppacao' => null,
        'hppdtcriacao' => null,
        'ppcid' => null,
        'pagid' => null,
        'epcid' => null,
        'ppcpercentualpag' => null,
        'ppcvalorpagamento' => null,
        'ppcstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_PagamentoParComposicao $model,$acao)
    {
    
        $this->arAtributos['hppcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hppacao'] = $acao;
        $this->arAtributos['hppdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['ppcid'] = $model->ppcid;
        $this->arAtributos['pagid'] = $model->pagid;
        $this->arAtributos['epcid'] = $model->epcid;
        $this->arAtributos['ppcpercentualpag'] = $model->ppcpercentualpag;
        $this->arAtributos['ppcvalorpagamento'] = $model->ppcvalorpagamento;
        $this->arAtributos['ppcstatus'] = $model->ppcstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

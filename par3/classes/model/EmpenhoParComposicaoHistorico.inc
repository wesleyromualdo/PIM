<?php
/**
 * Classe de mapeamento da entidade par3.empenhoparcomposicao_historico.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_EmpenhoparcomposicaoHistorico: sem descricao
 *
 * @package Model\EmpenhoparcomposicaoHistorico
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_EmpenhoParComposicaoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_EmpenhoParComposicaoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hepid chave primária da tabelaempenhoparcomposicao_historico
 * @property  $hepcpf CPF do respnsável pela alteração
 * @property  $hepacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hepdtcriacao Data de criação do log
 * @property int $epcid Chave primaria da tabela par3.empenhoparcomposicao - default: nextval('par3.empenhoparcomposicao_epcid_seq'::regclass)
 * @property int $empid Chave estrangeira da tabela par3.empenho
 * @property int $ppcid Chave estrangeira da tabela par3.processoparcomposicao
 * @property numeric $epcpercentualemp Percentual empenhado
 * @property numeric $epcvalorempenho Valor do empenho
 * @property string $epcstatus Status A - Ativo I - Inativo - default: 'A'::bpchar
 */
class Par3_Model_EmpenhoParComposicaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.empenhoparcomposicao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hepid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'epcid' => array('tabela' => 'par3.empenhoparcomposicao', 'pk' => 'epcid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hepid' => null,
        'hepcpf' => null,
        'hepacao' => null,
        'hepdtcriacao' => null,
        'epcid' => null,
        'empid' => null,
        'ppcid' => null,
        'epcpercentualemp' => null,
        'epcvalorempenho' => null,
        'epcstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_EmpenhoParComposicao $model,$acao)
    {

        $this->arAtributos['hepcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hepacao'] = $acao;
        $this->arAtributos['hepdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['epcid'] = $model->epcid;
        $this->arAtributos['empid'] = $model->empid;
        $this->arAtributos['ppcid'] = $model->ppcid;
        $this->arAtributos['epcpercentualemp'] = $model->epcpercentualemp;
        $this->arAtributos['epcvalorempenho'] = $model->epcvalorempenho;
        $this->arAtributos['epcstatus'] = $model->epcstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

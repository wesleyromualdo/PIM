<?php
/**
 * Classe de mapeamento da entidade par3.empenhoobracomposicao_historico.
 *
 * @version $Id$
 * @since 2018.06.25
 */

/**
 * Model_EmpenhoobracomposicaoHistorico: sem descricao
 *
 * @package Model\EmpenhoobracomposicaoHistorico
 * @uses Simec\Db\Modelo
 * @author Nayra Cristine Lopes Teixeira <nayrateixeira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_EmpenhoObraComposicaoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_EmpenhoObraComposicaoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $heoid chave primária da tabelaempenhoobracomposicao_historico
 * @property  $heocpf CPF do respnsável pela alteração
 * @property  $heoacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $heodtcriacao Data de criação do log
 * @property int $eocid  - default: nextval('par3.empenhoobracomposicao_eocid_seq'::regclass)
 * @property int $empid 
 * @property int $pocid 
 * @property numeric $eocpercentualemp 
 * @property numeric $eocvalorempenho 
 * @property string $eocstatus  - default: 'A'::bpchar
 */
class Par3_Model_EmpenhoObraComposicaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.empenhoobracomposicao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'heoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'eocid' => array('tabela' => 'par3.empenhoobracomposicao', 'pk' => 'eocid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'heoid' => null,
        'heocpf' => null,
        'heoacao' => null,
        'heodtcriacao' => null,
        'eocid' => null,
        'empid' => null,
        'pocid' => null,
        'eocpercentualemp' => null,
        'eocvalorempenho' => null,
        'eocstatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_EmpenhoObraComposicao $model,$acao)
    {
    
        $this->arAtributos['heocpf'] = $_SESSION['usucpf'];
        $this->arAtributos['heoacao'] = $acao;
        $this->arAtributos['heodtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['eocid'] = $model->eocid;
        $this->arAtributos['empid'] = $model->empid;
        $this->arAtributos['pocid'] = $model->pocid;
        $this->arAtributos['eocpercentualemp'] = $model->eocpercentualemp;
        $this->arAtributos['eocvalorempenho'] = $model->eocvalorempenho;
        $this->arAtributos['eocstatus'] = $model->eocstatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

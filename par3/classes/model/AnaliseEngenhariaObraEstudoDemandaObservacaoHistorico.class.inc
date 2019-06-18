<?php
/**
 * Classe de mapeamento da entidade par3.analise_engenharia_obra_estudo_demanda_observacao_historico.
 *
 * @version $Id$
 * @since 2017.11.17
 */

/**
 * Par3_Model_Analise_engenharia_obra_estudo_demanda_observacaoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Analise_engenharia_obra_estudo_demanda_observacaoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Analise_engenharia_obra_estudo_demanda_observacaoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $haeid chave primária da tabelaanalise_engenharia_obra_estudo_demanda_observacao_historico
 * @property  $haecpf CPF do respnsável pela alteração
 * @property  $haeacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $haedtcriacao Data de criação do log
 * @property int $aegid FK par3.analise_engenharia_obra_aba_respostas
 * @property string $aed_observacao Observação da análise de engeniaria do estudo de demanda
 */
class Par3_Model_AnaliseEngenhariaObraEstudoDemandaObservacaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.analise_engenharia_obra_estudo_demanda_observacao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'haeid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'aedid' => array('tabela' => 'par3.analise_engenharia_obra_estudo_demanda_observacao', 'pk' => 'aedid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'haeid' => null,
        'haecpf' => null,
        'haeacao' => null,
        'haedtcriacao' => null,
        'aedid' => null,
        'aegid' => null,
        'aed_observacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_AnaliseEngenhariaObraEstudoDemandaObservacao $model,$acao)
    {

        $this->arAtributos['haecpf'] = $_SESSION['usucpf'];
        $this->arAtributos['haeacao'] = $acao;
        $this->arAtributos['haedtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['aedid'] = $model->aedid;
        $this->arAtributos['aegid'] = $model->aegid;
        $this->arAtributos['aed_observacao'] = $model->aed_observacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

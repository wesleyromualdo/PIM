<?php
/**
 * Classe de mapeamento da entidade par3.analise_engenharia_obra_aba_respostas_historico.
 *
 * @version $Id$
 * @since 2017.11.07
 */

/**
 * Par3_Model_Analise_engenharia_obra_aba_respostasHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Analise_engenharia_obra_aba_respostasHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Analise_engenharia_obra_aba_respostasHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $haeid chave primária da tabelaanalise_engenharia_obra_aba_respostas_historico
 * @property  $haecpf CPF do respnsável pela alteração
 * @property  $haeacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $haedtcriacao Data de criação do log
 * @property int $aerid Chave Primária - default: nextval('par3.analise_engenharia_obra_aba_respostas_aerid_seq'::regclass)
 * @property int $aegid
 * @property int $aepid FK da Análise (par3.analise_engenharia_obra_aba_perguntas)
 * @property int $otdid
 * @property string $aer_pendencia campo para informar se há ou não pendência
 * @property string $aer_observacao campo para informar a descrição
 */
class Par3_Model_AnaliseEngenhariaObraAbaRespostasHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.analise_engenharia_obra_aba_respostas_historico';

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
        'aerid' => array('tabela' => 'par3.analise_engenharia_obra_aba_respostas', 'pk' => 'aerid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'haeid'          => null,
        'haecpf'         => null,
        'haeacao'        => null,
        'haedtcriacao'   => null,
        'aerid'          => null,
        'aegid'          => null,
        'aepid'          => null,
        'aer_pendencia'  => null,
        'aer_observacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_AnaliseEngenhariaObraAbaRespostas $model,$acao)
    {
        $this->arAtributos['haecpf']         = $_SESSION['usucpf'];
        $this->arAtributos['haeacao']        = $acao;
        $this->arAtributos['haedtcriacao']   = date('Y-m-d H:m:s');
        $this->arAtributos['aerid']          = $model->aerid;
        $this->arAtributos['aegid']          = $model->aegid;
        $this->arAtributos['aepid']          = $model->aepid;
        $this->arAtributos['aer_pendencia']  = $model->aer_pendencia;
        $this->arAtributos['aer_observacao'] = $model->aer_observacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

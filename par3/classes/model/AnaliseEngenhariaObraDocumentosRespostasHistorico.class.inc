<?php
/**
 * Classe de mapeamento da entidade par3.analise_engenharia_obra_documentos_respostas_historico.
 *
 * @version $Id$
 * @since 2017.11.07
 */

/**
 * Par3_Model_Analise_engenharia_obra_documentos_respostasHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Analise_engenharia_obra_documentos_respostasHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Analise_engenharia_obra_documentos_respostasHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $haeid chave primária da tabelaanalise_engenharia_obra_documentos_respostas_historico
 * @property  $haecpf CPF do respnsável pela alteração
 * @property  $haeacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $haedtcriacao Data de criação do log
 * @property int $aedid Chave Primária - default: nextval('par3.analise_engenharia_obra_documentos_respostas_aedid_seq'::regclass)
 * @property int $aegid FK da análise de engenharia
 * @property int $otdid FK do tipo de documento (par3.obra_tipo_documento)
 * @property string $aed_pendencia campo para informar se há ou não pendência
 * @property string $aed_observacao campo para informar a descrição
 */
class Par3_Model_AnaliseEngenhariaObraDocumentosRespostasHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.analise_engenharia_obra_documentos_respostas_historico';

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
        'aedid' => array('tabela' => 'par3.analise_engenharia_obra_documentos_respostas', 'pk' => 'aedid'),
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
        'otdid' => null,
        'aed_pendencia' => null,
        'aed_observacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_AnaliseEngenhariaObraDocumentosRespostas $model,$acao)
    {

        $this->arAtributos['haecpf'] = $_SESSION['usucpf'];
        $this->arAtributos['haeacao'] = $acao;
        $this->arAtributos['haedtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['aedid'] = $model->aedid;
        $this->arAtributos['aegid'] = $model->aegid;
        $this->arAtributos['otdid'] = $model->otdid;
        $this->arAtributos['aed_pendencia'] = $model->aed_pendencia;
        $this->arAtributos['aed_observacao'] = $model->aed_observacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

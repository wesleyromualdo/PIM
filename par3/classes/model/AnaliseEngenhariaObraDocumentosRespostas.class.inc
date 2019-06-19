<?php
/**
 * Classe de mapeamento da entidade par3.analise_engenharia_obra_documentos_respostas.
 *
 * @version $Id$
 * @since 2017.11.03
 */

/**
 * Par3_Model_Analise_engenharia_obra_documentos_respostas: Tabela utilizada para cadastro de perguntas para o questionário de análise de engenharia
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Analise_engenharia_obra_documentos_respostas($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Analise_engenharia_obra_documentos_respostas($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $aedid Chave Primária - default: nextval('par3.analise_engenharia_obra_documentos_respostas_aedid_seq'::regclass)
 * @property int $obrid FK da Análise (par3.obra)
 * @property int $otdid FK da Análise (par3.obra_tipo_documento)
 * @property int $aegid FK da Análise (par3.analise_engenharia_obra)
 * @property string $aed_pendencia campo para informar se há ou não pendência
 * @property string $aed_observacao campo para informar a descrição
 */
class Par3_Model_AnaliseEngenhariaObraDocumentosRespostas extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.analise_engenharia_obra_documentos_respostas';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aedid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'otdid' => array('tabela' => 'par3.obra_tipo_documento',     'pk' => 'otdid'),
        'otdid' => array('tabela' => 'par3.analise_engenharia_obra', 'pk' => 'aegid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'aedid' => null,
        'aegid' => null,
        'otdid' => null,
        'aed_pendencia' => null,
        'aed_observacao' => null,
        'aer_corrigido' => null,
    );

    public function __construct($aegid = null,$otdid = null)
    {
        global $db;
        $id = null;
        if($aegid && $otdid){
            $aedid = $db->recuperar("SELECT aedid FROM {$this->stNomeTabela} WHERE aegid = {$aegid} AND otdid = {$otdid}");
            $id = $aedid['aedid'];
        }
        parent::__construct($id);
    }

    public function listarPendencias($aegid)
    {
        return $this->recuperarTodos('*',array('aegid = '.$aegid));
    }

    public function atualizarPendenciaCorrigida($aedid){
        $sql = "UPDATE {$this->stNomeTabela} SET aed_pendencia = 'N', aer_corrigido = 'S' WHERE aedid = {$aedid}";
        $this->executar($sql);
        $this->commit();
    }

}

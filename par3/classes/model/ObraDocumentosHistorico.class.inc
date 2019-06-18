<?php
/**
 * Classe de mapeamento da entidade par3.obra_documentos_historico.
 *
 * @version $Id$
 * @since 2017.07.10
 */

/**
 * Par3_Model_Obra_documentosHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraDocumentosHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_documentosHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hodid chave primária da tabelaobra_documentos_historico
 * @property  $hodcpf CPF do respnsável pela alteração
 * @property  $hodacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hoddtcriacao Data de criação do log
 * @property int $odoid  - default: nextval('par3.obra_documentos_odoid_seq'::regclass)
 * @property int $obrid 
 * @property int $arqid 
 * @property int $otdid 
 * @property string $odostatus  - default: 'A'::bpchar
 */
class Par3_Model_ObraDocumentosHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_documentos_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hodid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'odoid' => array('tabela' => 'par3.obra_documentos', 'pk' => 'odoid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hodid' => null,
        'hodcpf' => null,
        'hodacao' => null,
        'hoddtcriacao' => null,
        'odoid' => null,
        'obrid' => null,
        'arqid' => null,
        'otdid' => null,
        'odostatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraDocumentos $model,$acao)
    {
    
        $this->arAtributos['hodcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hodacao'] = $acao;
        $this->arAtributos['hoddtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['odoid'] = $model->odoid;
        $this->arAtributos['obrid'] = $model->obrid;
        $this->arAtributos['arqid'] = $model->arqid;
        $this->arAtributos['otdid'] = $model->otdid;
        $this->arAtributos['odostatus'] = $model->odostatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

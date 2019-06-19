<?php
/**
 * Classe de mapeamento da entidade par3.obra_documento_apoio_historico.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_Obra_documento_apoioHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_documento_apoioHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_documento_apoioHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hodid chave primária da tabelaobra_documento_apoio_historico
 * @property  $hodcpf CPF do respnsável pela alteração
 * @property  $hodacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hoddtcriacao Data de criação do log
 * @property int $odaid  - default: nextval('par3.obra_documento_apoio_odaid_seq'::regclass)
 * @property int $arqid 
 * @property string $odadsc 
 * @property string $odasituacao 
 * @property string $odastatus 
 */
class Par3_Model_ObraDocumentoApoioHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_documento_apoio_historico';

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
        'odaid' => array('tabela' => 'par3.obra_documento_apoio', 'pk' => 'odaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hodid' => null,
        'hodcpf' => null,
        'hodacao' => null,
        'hoddtcriacao' => null,
        'odaid' => null,
        'arqid' => null,
        'odadsc' => null,
        'odasituacao' => null,
        'odastatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Obra_documento_apoio $model,$acao)
    {
    
        $this->arAtributos['hodcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hodacao'] = $acao;
        $this->arAtributos['hoddtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['odaid'] = $model->odaid;
        $this->arAtributos['arqid'] = $model->arqid;
        $this->arAtributos['odadsc'] = $model->odadsc;
        $this->arAtributos['odasituacao'] = $model->odasituacao;
        $this->arAtributos['odastatus'] = $model->odastatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_x_documento_apoi_historico.
 *
 * @version $Id$
 * @since 2017.07.06
 */

/**
 * Par3_Model_Obra_tipo_x_documento_apoiHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_tipo_x_documento_apoiHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_tipo_x_documento_apoiHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hodid chave primária da tabelaobra_tipo_x_documento_apoi_historico
 * @property  $hodcpf CPF do respnsável pela alteração
 * @property  $hodacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hoddtcriacao Data de criação do log
 * @property int $odaid 
 * @property int $otpid 
 */
class Par3_Model_ObraTipoXDocumentoApoioHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_x_documento_apoi_historico';

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
        'odaid' => array('tabela' => 'par3.obra_tipo_x_documento_apoi', 'pk' => 'odaid'),
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
        'otpid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Obra_tipo_x_documento_apoi $model,$acao)
    {
    
        $this->arAtributos['hodcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hodacao'] = $acao;
        $this->arAtributos['hoddtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['odaid'] = $model->odaid;
        $this->arAtributos['otpid'] = $model->otpid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

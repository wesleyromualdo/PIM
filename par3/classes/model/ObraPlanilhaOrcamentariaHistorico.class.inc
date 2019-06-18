<?php
/**
 * Classe de mapeamento da entidade par3.obra_planilha_orcamentaria_historico.
 *
 * @version $Id$
 * @since 2017.07.07
 */

/**
 * Par3_Model_Obra_planilha_orcamentariaHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_planilha_orcamentariaHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_planilha_orcamentariaHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hopid chave primária da tabelaobra_planilha_orcamentaria_historico
 * @property  $hopcpf CPF do respnsável pela alteração
 * @property  $hopacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hopdtcriacao Data de criação do log
 * @property int $opoid  - default: nextval('par3.obra_planilha_orcamentaria_opoid_seq'::regclass)
 * @property int $obrid 
 * @property int $oivid 
 * @property numeric $opoqtd 
 * @property numeric $opovalor 
 * @property string $opostatus 
 */
class Par3_Model_ObraPlanilhaOrcamentariaHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_planilha_orcamentaria_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hopid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'opoid' => array('tabela' => 'par3.obra_planilha_orcamentaria', 'pk' => 'opoid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hopid' => null,
        'hopcpf' => null,
        'hopacao' => null,
        'hopdtcriacao' => null,
        'opoid' => null,
        'obrid' => null,
        'oivid' => null,
        'opoqtd' => null,
        'opovalor' => null,
        'opostatus' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraPlanilhaOrcamentaria $model,$acao)
    {
    
        $this->arAtributos['hopcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hopacao'] = $acao;
        $this->arAtributos['hopdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['opoid'] = $model->opoid;
        $this->arAtributos['obrid'] = $model->obrid;
        $this->arAtributos['oivid'] = $model->oivid;
        $this->arAtributos['opoqtd'] = $model->opoqtd;
        $this->arAtributos['opovalor'] = $model->opovalor;
        $this->arAtributos['opostatus'] = $model->opostatus;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

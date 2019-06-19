<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_item_planilha.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_Obra_tipo_item_planilha_valoresHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_tipo_item_planilha_valoresHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_tipo_item_planilha_valoresHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hoiid chave primária da tabelaobra_tipo_item_planilha
 * @property  $hoicpf CPF do respnsável pela alteração
 * @property  $hoiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hoidtcriacao Data de criação do log
 * @property int $oivid  - default: nextval('par3.obra_tipo_item_planilha_valores_oivid_seq'::regclass)
 * @property int $oipid
 * @property int $inuid
 * @property numeric $oivqtd
 * @property string $oivqtd_editavel
 * @property numeric $oivvalor
 * @property string $oivvalor_editavel
 */
class Par3_Model_ObraTipoItemPlanilhaValoresHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_item_planilha';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hoiid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'oivid' => array('tabela' => 'par3.obra_tipo_item_planilha_valores', 'pk' => 'oivid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hoiid' => null,
        'hoicpf' => null,
        'hoiacao' => null,
        'hoidtcriacao' => null,
        'oivid' => null,
        'oipid' => null,
        'inuid' => null,
        'oivqtd' => null,
        'oivqtd_editavel' => null,
        'oivvalor' => null,
        'oivvalor_editavel' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ObraTipoItemPlanilhaValores $model,$acao)
    {

        $this->arAtributos['hoicpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hoiacao'] = $acao;
        $this->arAtributos['hoidtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['oivid'] = $model->oivid;
        $this->arAtributos['oipid'] = $model->oipid;
        $this->arAtributos['inuid'] = $model->inuid;
        $this->arAtributos['oivqtd'] = $model->oivqtd;
        $this->arAtributos['oivqtd_editavel'] = $model->oivqtd_editavel;
        $this->arAtributos['oivvalor'] = $model->oivvalor;
        $this->arAtributos['oivvalor_editavel'] = $model->oivvalor_editavel;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

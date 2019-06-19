<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_areas_historico.
 *
 * @version $Id$
 * @since 2017.07.04
 */

/**
 * Par3_Model_IniciativaIniciativasAreasHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasAreasHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasAreasHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hiiid chave primária da tabelainiciativa_iniciativas_areas_historico
 * @property  $hiicpf CPF do respnsável pela alteração
 * @property  $hiiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hiidtcriacao Data de criação do log
 * @property int $iiarid Chave Primária - default: nextval('par3.iniciativa_iniciativas_areas_iiarid_seq'::regclass)
 * @property "char" $iiarstatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_areas_iniid_seq'::regclass)
 * @property int $iarid FK do código da área (par3.iniciativa_areas) - default: nextval('par3.iniciativa_iniciativas_areas_iarid_seq'::regclass)
 */
class Par3_Model_IniciativaIniciativasAreasHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_areas_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hiiid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iiaid' => array('tabela' => 'par3.iniciativa_iniciativas_areas', 'pk' => 'iiaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hiiid' => null,
        'hiicpf' => null,
        'hiiacao' => null,
        'hiidtcriacao' => null,
        'iiarid' => null,
        'iiarstatus' => null,
        'iniid' => null,
        'iarid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaIniciativasAreas $model,$acao)
    {
    
        $this->arAtributos['hiicpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hiiacao'] = $acao;
        $this->arAtributos['hiidtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['iiarid'] = $model->iiarid;
        $this->arAtributos['iiarstatus'] = $model->iiarstatus;
        $this->arAtributos['iniid'] = $model->iniid;
        $this->arAtributos['iarid'] = $model->iarid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

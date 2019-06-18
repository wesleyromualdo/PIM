<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_projetos_historico.
 *
 * @version $Id$
 * @since 2017.07.04
 */

/**
 * Par3_Model_IniciativaIniciativasProjetosHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasProjetosHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasProjetosHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hiiid chave primária da tabelainiciativa_iniciativas_projetos_historico
 * @property  $hiicpf CPF do respnsável pela alteração
 * @property  $hiiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hiidtcriacao Data de criação do log
 * @property int $iiproid Chave Primária - default: nextval('par3.iniciativa_iniciativas_projetos_iiproid_seq'::regclass)
 * @property "char" $iiprostatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_projetos_iniid_seq'::regclass)
 * @property int $proid FK do código do projeto (par3.projeto) - default: nextval('par3.iniciativa_iniciativas_projetos_proid_seq'::regclass)
 */
class Par3_Model_IniciativaIniciativasProjetosHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_projetos_historico';

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
        'iipid' => array('tabela' => 'par3.iniciativa_iniciativas_projetos', 'pk' => 'iipid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hiiid' => null,
        'hiicpf' => null,
        'hiiacao' => null,
        'hiidtcriacao' => null,
        'iiproid' => null,
        'iiprostatus' => null,
        'iniid' => null,
        'proid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaIniciativasProjetos $model,$acao)
    {
    
        $this->arAtributos['hiicpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hiiacao'] = $acao;
        $this->arAtributos['hiidtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['iiproid'] = $model->iiproid;
        $this->arAtributos['iiprostatus'] = $model->iiprostatus;
        $this->arAtributos['iniid'] = $model->iniid;
        $this->arAtributos['proid'] = $model->proid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

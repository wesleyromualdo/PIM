<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_grupo_tipo_municipio_historico.
 *
 * @version $Id$
 * @since 2017.07.04
 */

/**
 * Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hiiid chave primária da tabelainiciativa_iniciativas_grupo_tipo_municipio_historico
 * @property  $hiicpf CPF do respnsável pela alteração
 * @property  $hiiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hiidtcriacao Data de criação do log
 * @property int $iigtmid Chave Primária - default: nextval('par3.iniciativa_iniciativas_grupo_tipo_municipio_iigtmid_seq'::regclass)
 * @property "char" $iigtmstatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_grupo_tipo_municipio_iniid_seq'::regclass)
 * @property int $gtmid FK do código grupo tipo município (territorios.grupotipomunicipio) - default: nextval('par3.iniciativa_iniciativas_grupo_tipo_municipio_gtmid_seq'::regclass)
 */
class Par3_Model_IniciativaIniciativasGrupoTipoMunicipioHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_grupo_tipo_municipio_historico';

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
        'iigid' => array('tabela' => 'par3.iniciativa_iniciativas_grupo_tipo_municipio', 'pk' => 'iigid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hiiid' => null,
        'hiicpf' => null,
        'hiiacao' => null,
        'hiidtcriacao' => null,
        'iigtmid' => null,
        'iigtmstatus' => null,
        'iniid' => null,
        'gtmid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaIniciativasGrupoTipoMunicipio $model,$acao)
    {
    
        $this->arAtributos['hiicpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hiiacao'] = $acao;
        $this->arAtributos['hiidtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['iigtmid'] = $model->iigtmid;
        $this->arAtributos['iigtmstatus'] = $model->iigtmstatus;
        $this->arAtributos['iniid'] = $model->iniid;
        $this->arAtributos['gtmid'] = $model->gtmid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

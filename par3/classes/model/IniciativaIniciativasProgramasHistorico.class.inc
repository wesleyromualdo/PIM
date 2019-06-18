<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_iniciativas_programas_historico.
 *
 * @version $Id$
 * @since 2017.07.04
 */

/**
 * Par3_Model_IniciativaIniciativasProgramasHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIniciativasProgramasHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIniciativasProgramasHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hiiid chave primária da tabelainiciativa_iniciativas_programas_historico
 * @property  $hiicpf CPF do respnsável pela alteração
 * @property  $hiiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hiidtcriacao Data de criação do log
 * @property int $iiprgid Chave Primária - default: nextval('par3.iniciativa_iniciativas_programas_iiprgid_seq'::regclass)
 * @property "char" $iiprgstatus Status
 * @property int $iniid FK do código da iniciativa (par3.iniciativa) - default: nextval('par3.iniciativa_iniciativas_programas_iniid_seq'::regclass)
 * @property int $prgid FK do código do programa (par3.programa) - default: nextval('par3.iniciativa_iniciativas_programas_prgid_seq'::regclass)
 */
class Par3_Model_IniciativaIniciativasProgramasHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_iniciativas_programas_historico';

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
        'iipid' => array('tabela' => 'par3.iniciativa_iniciativas_programas', 'pk' => 'iipid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hiiid' => null,
        'hiicpf' => null,
        'hiiacao' => null,
        'hiidtcriacao' => null,
        'iiprgid' => null,
        'iiprgstatus' => null,
        'iniid' => null,
        'prgid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaIniciativasProgramas $model,$acao)
    {
    
        $this->arAtributos['hiicpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hiiacao'] = $acao;
        $this->arAtributos['hiidtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['iiprgid'] = $model->iiprgid;
        $this->arAtributos['iiprgstatus'] = $model->iiprgstatus;
        $this->arAtributos['iniid'] = $model->iniid;
        $this->arAtributos['prgid'] = $model->prgid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_itenscomposicao_grupo_historico.
 *
 * @version $Id$
 * @since 2017.07.03
 */

/**
 * Par3_Model_Iniciativa_itenscomposicao_grupoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_itenscomposicao_grupoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_itenscomposicao_grupoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hiiid chave primária da tabelainiciativa_itenscomposicao_grupo_historico
 * @property  $hiicpf CPF do respnsável pela alteração
 * @property  $hiiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hiidtcriacao Data de criação do log
 * @property int $iigid  - default: nextval('par3.iniciativa_itenscomposicao_grupo_iigid_seq'::regclass)
 * @property int $iniid 
 * @property int $itcid 
 * @property int $igrid 
 */
class Par3_Model_Iniciativa_itenscomposicao_grupoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_itenscomposicao_grupo_historico';

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
        'iigid' => array('tabela' => 'par3.iniciativa_itenscomposicao_grupo', 'pk' => 'iigid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hiiid' => null,
        'hiicpf' => null,
        'hiiacao' => null,
        'hiidtcriacao' => null,
        'iigid' => null,
        'iniid' => null,
        'itcid' => null,
        'igrid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Iniciativa_itenscomposicao_grupo $model,$acao)
    {
    
        $this->arAtributos['hiicpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hiiacao'] = $acao;
        $this->arAtributos['hiidtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['iigid'] = $model->iigid;
        $this->arAtributos['iniid'] = $model->iniid;
        $this->arAtributos['itcid'] = $model->itcid;
        $this->arAtributos['igrid'] = $model->igrid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

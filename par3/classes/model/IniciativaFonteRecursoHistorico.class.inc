<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_fonte_recurso_historico.
 *
 * @version $Id$
 * @since 2017.10.09
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Iniciativa_fonte_recursoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_fonte_recursoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_fonte_recursoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hifid chave primária da tabelainiciativa_fonte_recurso_historico
 * @property  $hifcpf CPF do respnsável pela alteração
 * @property  $hifacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hifdtcriacao Data de criação do log
 * @property string $ifrusucpfinativacao 
 * @property \Datetime(Y-m-d H:i:s) $ifrdtinativacao 
 * @property string $ifrstatus  - default: 'A'::bpchar
 * @property string $ifrdescicaofonte 
 * @property string $ifrcodigofonte 
 * @property int $inplintid 
 * @property int $ifrid  - default: nextval('par3.iniciativa_fonte_recurso_ifrid_seq'::regclass)
 */
class Par3_Model_IniciativaFonteRecursoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_fonte_recurso_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hifid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'ifrid' => array('tabela' => 'par3.iniciativa_fonte_recurso', 'pk' => 'ifrid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hifid' => null,
        'hifcpf' => null,
        'hifacao' => null,
        'hifdtcriacao' => null,
        'ifrusucpfinativacao' => null,
        'ifrdtinativacao' => null,
        'ifrstatus' => null,
        'ifrdescicaofonte' => null,
        'ifrcodigofonte' => null,
        'inplintid' => null,
        'ifrid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaFonteRecurso $model,$acao)
    {
        $this->arAtributos['hifcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hifacao'] = $acao;
        $this->arAtributos['hifdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['ifrusucpfinativacao'] = $model->ifrusucpfinativacao;
        $this->arAtributos['ifrdtinativacao'] = $model->ifrdtinativacao;
        $this->arAtributos['ifrstatus'] = $model->ifrstatus;
        $this->arAtributos['ifrdescicaofonte'] = $model->ifrdescicaofonte;
        $this->arAtributos['ifrcodigofonte'] = $model->ifrcodigofonte;
        $this->arAtributos['inplintid'] = $model->inplintid;
        $this->arAtributos['ifrid'] = $model->ifrid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

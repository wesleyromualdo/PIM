<?php
/**
 * Classe de mapeamento da entidade par3.indicador_demografico_historico.
 *
 * @version $Id$
 * @since 2017.07.18
 */

/**
 * Par3_Model_Indicador_demograficoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Leo Kenzley Beserra De Oliveira <leokenzley@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Indicador_demograficoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Indicador_demograficoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hinid chave primária da tabelaindicador_demografico_historico
 * @property  $hincpf CPF do respnsável pela alteração
 * @property  $hinacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hindtcriacao Data de criação do log
 * @property string $indcod 
 * @property int $indtabcorede 
 * @property string $indtabmunic 
 * @property string $indtabuf 
 * @property string $indddesc 
 * @property int $inddid  - default: nextval('par3.indicador_demografico_inddid_seq'::regclass)
 */
class Par3_Model_Indicador_demograficoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.indicador_demografico_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hinid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'indid' => array('tabela' => 'par3.indicador_demografico', 'pk' => 'indid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hinid' => null,
        'hincpf' => null,
        'hinacao' => null,
        'hindtcriacao' => null,
        'indcod' => null,
        'indtabcorede' => null,
        'indtabmunic' => null,
        'indtabuf' => null,
        'indddesc' => null,
        'inddid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Indicador_demografico $model,$acao)
    {
    
        $this->arAtributos['hincpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hinacao'] = $acao;
        $this->arAtributos['hindtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['indcod'] = $model->indcod;
        $this->arAtributos['indtabcorede'] = $model->indtabcorede;
        $this->arAtributos['indtabmunic'] = $model->indtabmunic;
        $this->arAtributos['indtabuf'] = $model->indtabuf;
        $this->arAtributos['indddesc'] = $model->indddesc;
        $this->arAtributos['inddid'] = $model->inddid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

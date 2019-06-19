<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_modalidades_historico.
 *
 * @version $Id$
 * @since 2017.06.27
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_IniciativaModalidadesHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Philipe Allan Almeida <philipe.almeida@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaModalidadesHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaModalidadesHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $himid chave primária da tabelainiciativa_modalidades_historico
 * @property  $himcpf CPF do respnsável pela alteração
 * @property  $himacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $himdtcriacao Data de criação do log
 * @property int $imoid Chave Primária - default: nextval('par3.iniciativa_modalidades_imoid_seq'::regclass)
 * @property int $iniid FK da Iniciativa (par3.iniciativa)
 * @property int $nivid FK da Nível (par3.ensino_nivel)
 * @property int $etaid FK da Etapa (par3.ensino_etapa)
 * @property int $modid FK da Modalidade (par3.modalidade)
 * @property int $desid FK da Desdobramento (par3.iniciativa_desdobramento)
 */
class Par3_Model_IniciativaModalidadesHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_modalidades_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'himid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'imoid' => array('tabela' => 'par3.iniciativa_modalidades', 'pk' => 'imoid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'himid' => null,
        'himcpf' => null,
        'himacao' => null,
        'himdtcriacao' => null,
        'imoid' => null,
        'iniid' => null,
        'nivid' => null,
        'etaid' => null,
        'modid' => null,
        'desid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaModalidades $model,$acao)
    {
    
        $this->arAtributos['himcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['himacao'] = $acao;
        $this->arAtributos['himdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['imoid'] = $model->imoid;
        $this->arAtributos['iniid'] = $model->iniid;
        $this->arAtributos['nivid'] = $model->nivid;
        $this->arAtributos['etaid'] = $model->etaid;
        $this->arAtributos['modid'] = $model->modid;
        $this->arAtributos['desid'] = $model->desid;

        if ($acao == 'CREATE'){
            $this->arAtributos['imostatus'] = 'A';
        }

        if ($acao == 'DELETE'){
            $this->arAtributos['imostatus'] = 'I';
        }

        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php

/**
 * Classe de mapeamento da entidade par3.iniciativa_desdobramento_historico.
 *
 * @version $Id$
 * @since 2017.05.12
 */

/**
 * Model_Iniciativa_desdobramentoHistorico: sem descricao
 *
 * @package Model\Iniciativa
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Iniciativa_desdobramentoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Iniciativa_desdobramentoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hdiid chave primária da tabelainiciativa_desdobramento_historico
 * @property  $hdicpf CPF do respnsável pela alteração
 * @property  $hdiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hdidtcriacao Data de criação do log
 * @property int $disid  - default: nextval('par3.iniciativa_desdobramento_disid_seq'::regclass)
 * @property int $tipid  - default: nextval('par3.iniciativa_desdobramento_tipid_seq'::regclass)
 * @property string $disdsc
 * @property "char" $disstatus
 * @property "char" $dissituacao
 * @property \Datetime(Y-m-d H:i:s) $disdtinclusao
 * @property string $discpfinclusao
 * @property \Datetime(Y-m-d H:i:s) $disdtinativacao
 * @property string $discpfinativacao
 */
class Par3_Model_IniciativaDesdobramentosHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_desdobramento_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hdeid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'desid' => array('tabela' => 'par3.iniciativa_desdobramento', 'pk' => 'desid'),
        'tipid' => array('tabela' => 'par3.iniciativa_desdobramento_tipo', 'pk' => 'tipid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hdeid' => null,
        'hdecpf' => null,
        'hdeacao' => null,
        'hdedtcriacao' => null,
        'desid' => null,
        'tipid' => null,
        'desdsc' => null,
        'desstatus' => null,
        'dessituacao' => null,
        'desdtinclusao' => null,
        'descpfinclusao' => null,
        'desdtinativacao' => null,
        'descpfinativacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaDesdobramentos $model,$acao)
    {
        $this->arAtributos['hdecpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hdeacao'] = $acao;
        $this->arAtributos['hdedtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['desid'] = $model->desid;
        $this->arAtributos['tipid'] = $model->tipid;
        $this->arAtributos['desdsc'] = $model->desdsc;
        $this->arAtributos['desstatus'] = $model->desstatus;
        $this->arAtributos['dessituacao'] = $model->dessituacao;
        $this->arAtributos['desdtinclusao'] = $model->desdtinclusao;
        $this->arAtributos['descpfinclusao'] = $model->descpfinclusao;
        $this->arAtributos['desdtinativacao'] = $model->desdtinativacao;
        $this->arAtributos['descpfinativacao'] = $model->descpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

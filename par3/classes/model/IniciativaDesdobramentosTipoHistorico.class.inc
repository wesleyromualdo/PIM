<?php

/**
 * Classe de mapeamento da entidade par3.iniciativa_desdobramento_tipo_historico.
 *
 * @version $Id$
 * @since 2017.05.12
 */

/**
 * Model_Iniciativa_desdobramento_tipoHistorico: sem descricao
 *
 * @package Model\Iniciativa
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Iniciativa_desdobramento_tipoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Iniciativa_desdobramento_tipoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $htiid chave primária da tabelainiciativa_desdobramento_tipo_historico
 * @property  $hticpf CPF do respnsável pela alteração
 * @property  $htiacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $htidtcriacao Data de criação do log
 * @property int $tipid  - default: nextval('par3.iniciativa_desdobramento_tipo_tipid_seq'::regclass)
 * @property string $tipdsc
 * @property "char" $tipstatus
 * @property "char" $tipsituacao
 * @property \Datetime(Y-m-d H:i:s) $tipdtinclusao
 * @property string $tipcpfinclusao
 * @property \Datetime(Y-m-d H:i:s) $tipdtinativacao
 * @property string $tipcpfinativacao
 */
class Par3_Model_IniciativaDesdobramentosTipoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_desdobramento_tipo_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'htiid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'tipid' => array('tabela' => 'par3.iniciativa_desdobramento_tipo', 'pk' => 'tipid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'htiid' => null,
        'hticpf' => null,
        'htiacao' => null,
        'htidtcriacao' => null,
        'tipid' => null,
        'tipdsc' => null,
        'tipstatus' => null,
        'tipsituacao' => null,
        'tipdtinclusao' => null,
        'tipcpfinclusao' => null,
        'tipdtinativacao' => null,
        'tipcpfinativacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaDesdobramentosTipo $model,$acao)
    {
        $this->arAtributos['hticpf'] = $_SESSION['usucpf'];
        $this->arAtributos['htiacao'] = $acao;
        $this->arAtributos['htidtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['tipid'] = $model->tipid;
        $this->arAtributos['tipdsc'] = $model->tipdsc;
        $this->arAtributos['tipstatus'] = $model->tipstatus;
        $this->arAtributos['tipsituacao'] = $model->tipsituacao;
        $this->arAtributos['tipdtinclusao'] = $model->tipdtinclusao;
        $this->arAtributos['tipcpfinclusao'] = $model->tipcpfinclusao;
        $this->arAtributos['tipdtinativacao'] = $model->tipdtinativacao;
        $this->arAtributos['tipcpfinativacao'] = $model->tipcpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

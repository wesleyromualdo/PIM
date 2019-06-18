<?php

class Par3_Model_IniciativaAreasRelacionadasHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_areasrelacionadas_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hirid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'irrid'  => array('tabela' => 'par3.iniciativa_areasrelacionadas',    'pk' => 'irrid'),
        'iuoid'  => array('tabela' => 'par3.iniciativa_unidade_orcamentaria', 'pk' => 'iuoid'),
        'iarid'  => array('tabela' => 'par3.iniciativa_areas',                'pk' => 'iarid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hirid'           => null,
        'hircpf'          => null,
        'hiracao'         => null,
        'hirdtcriacao'    => null,
        'irrid'           => null,
        'iuoid'           => null,
        'iarid'           => null,
        'irrsituacao'     => null,
        'irrstatus'       => null,
        'irrdtinclusao'   => null,
        'irrcpfinclusao'  => null,
        'irrdtinativacao' => null,
        'irrcpfinativacao'=> null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaAreasRelacionadas $model,$acao)
    {
        $this->arAtributos['hircpf']           = $_SESSION['usucpf'];
        $this->arAtributos['hiracao']          = $acao;
        $this->arAtributos['hirdtcriacao']     = date('Y-m-d H:m:s');
        $this->arAtributos['irrid']            = $model->irrid;
        $this->arAtributos['iuoid']            = $model->iuoid;
        $this->arAtributos['iarid']            = $model->iarid;
        $this->arAtributos['irrsituacao']      = $model->irrsituacao;
        $this->arAtributos['irrstatus']        = $model->irrstatus;
        $this->arAtributos['irrdtinclusao']    = $model->irrdtinclusao;
        $this->arAtributos['irrdtinativacao']  = $model->irrdtinativacao;
        $this->arAtributos['irrcpfinativacao'] = $model->irrcpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

<?php

class Par3_Model_ObraFotosTerrenoHistorico extends Modelo
{
    protected $stNomeTabela = 'par3.obra_fotos_terreno_historico';

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    protected $arChavePrimaria = array(
        'hofid',
    );

    protected $arChaveEstrangeira = array(
        'oftid' => array('tabela' => 'par3.obra_fotos_terreno', 'pk' => 'oftid'),
    );

    protected $arAtributos = array(
        'hofid' => null,
        'hofcpf' => null,
        'hofacao' => null,
        'hofdtcriacao' => null,
        'oftid' => null,
        'obrid' => null,
        'otfid' => null,
        'oftdsc' => null,
        'oftstatus' => null,
        'arqid' => null,
    );


    public function gravarHistorico(Par3_Model_Obra_fotos_terreno $model,$acao)
    {
    
        $this->arAtributos['hofcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hofacao'] = $acao;
        $this->arAtributos['hofdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['oftid'] = $model->oftid;
        $this->arAtributos['obrid'] = $model->obrid;
        $this->arAtributos['otfid'] = $model->otfid;
        $this->arAtributos['oftdsc'] = $model->oftdsc;
        $this->arAtributos['oftstatus'] = $model->oftstatus;
        $this->arAtributos['arqid'] = $model->arqid;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}

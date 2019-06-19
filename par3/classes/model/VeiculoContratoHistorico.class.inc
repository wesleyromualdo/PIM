<?php

/**
 * Created by PhpStorm.
 * User: danielfiuza
 * Date: 07/04/17
 * Time: 11:53
 */
class Par3_Model_VeiculoContratoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.veiculo_contrato_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hvcid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'mveid' => array('tabela' => 'par3.modelo_veiculo', 'pk' => 'mveid'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'conid' => array('tabela' => 'par3.contratos', 'pk' => 'conid'),
        'arqid' => array('tabela' => 'public.arquivos', 'pk' => 'arqid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hvcid'            => null,
        'hvcacao'          => null,
        'hvccpf'           => null,
        'hvcdtcriacao'    => null,
        'vcoid'            => null,
        'conid'            => null,
        'usucpf'           => null,
        'mveid'            => null,
        'vcoplaca'         => null,
        'vcochassi'        => null,
        'vcorenavam'       => null,
        'vcoano'           => null,
        'vcoqtdassentos'   => null,
        'vcopossuirenavam' => null,
        'vcostatus'        => null,
        'vcodtcriacao'     => null,
        'arqid'            => null,
    );

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_VeiculoContrato $modelVeiculoContrato,$acao)
    {
        //grava no histórico
        $arrHistorico['vcoid']             = $modelVeiculoContrato->vcoid;
        $arrHistorico['hvcdtcriacao']            = date('Y-m-d H:m:s');
        $arrHistorico['hvccpf']             = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
        $arrHistorico['vcoid']              = $modelVeiculoContrato->vcoid;
        $arrHistorico['conid']              = $modelVeiculoContrato->conid;
        $arrHistorico['usucpf']             = $modelVeiculoContrato->usucpf;
        $arrHistorico['mveid']              = $modelVeiculoContrato->mveid;
        $arrHistorico['vcoplaca']           = $modelVeiculoContrato->vcoplaca;
        $arrHistorico['vcochassi']          = $modelVeiculoContrato->vcochassi;
        $arrHistorico['vcorenavam']         = $modelVeiculoContrato->vcorenavam;
        $arrHistorico['vcoano']             = $modelVeiculoContrato->vcoano;
        $arrHistorico['vcoqtdassentos']     = $modelVeiculoContrato->vcoqtdassentos;
        $arrHistorico['vcostatus']          = $modelVeiculoContrato->vcostatus;
        $arrHistorico['vcodtcriacao']      = $modelVeiculoContrato->vcodtcriacao;
        $arrHistorico['arqid']              = $modelVeiculoContrato->arqid;
        $arrHistorico['hvcacao']            = $acao;

        $this->popularDadosObjeto($arrHistorico);
        $this->salvar();
        $this->commit();
    }
}
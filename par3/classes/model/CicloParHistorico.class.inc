<?php

/**
 * Created by PhpStorm.
 * User: danielfiuza
 * Date: 31/03/17
 * Time: 16:01
 */
class Par3_Model_CicloParHistorico extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.ciclo_par_historico";

    protected $arChaveEstrangeira = array(
        'cicid' => array(
            'tabela' => 'ciclo_par',
            'pk' => 'cicid',
        ),
    );
    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('cihid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'cihid'            => null,
        'cicid'            => null,
        'cihcpf'           => null,//CPF do usuário da sessão
        'cihdt'            => null,//Data de gravação do registro
        'cihacao'          => null,//acão realizada
        'cicdsc'           => null,
        'cicdtinicio'      => null,
        'cicdtfim'         => null,
        'cicstatus'        => null,
        'cicsituacao'      => null,
        'cicvigencia'      => null,
        'cicdtinclusao'    => null,
        'ciccpfinclusao'   => null,
        'cicdtinativacao'  => null,
        'ciccpfinativacao' => null,
        'cicanos'          => null,
        'cicduracao'       => null,
    );

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_CicloPar $modelCiclo,$acao)
    {
        //grava no histórico
        $arrHistorico['cicid']            = $modelCiclo->cicid;
        $arrHistorico['cihdt']            = date('Y-m-d H:m:s');
        $arrHistorico['cihcpf']           = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
        $arrHistorico['cicdsc']           = $modelCiclo->cicdsc;
        $arrHistorico['cicsituacao']      = $modelCiclo->cicsituacao;
        $arrHistorico['cicstatus']        = $modelCiclo->cicstatus;
        $arrHistorico['cicvigencia']      = $modelCiclo->cicvigencia;
        $arrHistorico['cicdtinicio']      = $modelCiclo->cicdtinicio;
        $arrHistorico['cicdtfim']         = $modelCiclo->cicdtfim;
        $arrHistorico['cicdtinativacao']  = $modelCiclo->cicdtinativacao;
        $arrHistorico['ciccpfinativacao'] = $modelCiclo->ciccpfinativacao;
        $arrHistorico['cicdtinclusao']    = $modelCiclo->cicdtinclusao;
        $arrHistorico['ciccpfinclusao']   = $modelCiclo->ciccpfinclusao;
        $arrHistorico['cicanos']          = $modelCiclo->cicanos;
        $arrHistorico['cicduracao']       = $modelCiclo->cicduracao;
        $arrHistorico['cihacao']          = $acao;

        $this->popularDadosObjeto($arrHistorico);
        $this->salvar();
        $this->commit();
    }

}
<?php

class Par3_Model_IniciativaTiposObjetoHistorico extends Modelo
{
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
    */
    protected $stNomeTabela = "par3.iniciativa_tipos_objeto_historico";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('intohid');

    protected $arChaveEstrangeira = array(
        'intoid' => array(
            'tabela' => 'iniciativa_tipos_objeto',
            'pk' => 'intoid',
        ),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'intohid'           => null,
        'intohcpf'          => null,//CPF do usuário da sessão
        'intohdt'           => null,//Data de gravação do registro
        'intohacao'         => null,//acão realizada C,A,D
        'intoid'            => null,
        'intodsc'           => null,
        'intostatus'        => null,
        'intodtinclusao'    => null,
        'intocpfinclusao'   => null,
        'intodtinativacao'  => null,
        'intocpfinativacao' => null,
        'intosituacao'      => null
    );

    public function salvarHistorico(Par3_Model_IniciativaTiposObjeto $into, $status){
        $arrHistorico['intohdt']           = date('Y-m-d H:m:s');
        $arrHistorico['intohcpf']          = str_replace(array('.', '-', '/', ' '), '', $_SESSION['usucpf']);
        $arrHistorico['intoid']            = $into->intoid;
        $arrHistorico['intodsc']           = $into->intodsc;
        $arrHistorico['intostatus']        = $into->intostatus;
        $arrHistorico['intosituacao']      = $into->intosituacao;
        $arrHistorico['intodtinclusao']    = $into->intodtinclusao;
        $arrHistorico['intocpfinclusao']   = $into->intocpfinclusao;
        $arrHistorico['intodtinativacao']  = $into->intodtinativacao;
        $arrHistorico['intocpfinativacao'] = $into->intocpfinativacao;
        $arrHistorico['intohacao']         = $status;

        $this->popularDadosObjeto($arrHistorico);
        $this->salvar();
        $this->commit();
    }
}
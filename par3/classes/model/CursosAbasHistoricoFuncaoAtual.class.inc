<?php
/**
 * Classe de mapeamento da entidade par3.curso_aba_historico.
 *
 * @version $Id$
 * @since 2017.04.25
 */

class Par3_Model_CursosAbasHistoricoFuncaoAtual extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.curso_aba_pff_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hcurid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'curpffid' => array('tabela' => 'par3.curso_aba_pff', 'pk' => 'curpffid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hcurpffid' => null,
        'hcurcpf' => null,
        'hcuracao' => null,
        'curid' => null,
        'curpffid' => null,
        'pffid' => null,
        'curdtinclusao' => null,
        'curcpfinclusao' => null,
        'curdtinativacao' => null,
        'curcpfinativacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_CursosAbasFuncaoAtual $model,$acao)
    {
    
        $this->arAtributos['hcurcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hcuracao'] = $acao;
        $this->arAtributos['hcurdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['curid'] = $model->curid;
        $this->arAtributos['curdsc'] = $model->curdsc;
        $this->arAtributos['curtitulo'] = $model->curtitulo;
        $this->arAtributos['curqtd'] = $model->curqtd;
        $this->arAtributos['funid'] = $model->funid;
        $this->arAtributos['curorientacao'] = $model->curorientacao;
        $this->arAtributos['curmsg'] = $model->curmsg;
        $this->arAtributos['pffid'] = $model->pffid;
        $this->arAtributos['tfoid'] = $model->tfoid;
        $this->arAtributos['curscript'] = $model->curscript;
        $this->arAtributos['curstatus'] = $model->curstatus;
        $this->arAtributos['cursituacao'] = $model->cursituacao;
        $this->arAtributos['curdtinclusao'] = $model->curdtinclusao;
        $this->arAtributos['curcpfinclusao'] = $model->curcpfinclusao;
        $this->arAtributos['curdtinativacao'] = $model->curdtinativacao;
        $this->arAtributos['curcpfinativacao'] = $model->curcpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }


}

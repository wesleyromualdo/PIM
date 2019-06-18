<?php

/**
 * Classe de controle do  abasacesso
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   JEFERSON CARLOS ARAÚJO <jeferson.araujo@castgroup.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 30-06-2017
 * @link     no link
 */
class Spoemendas_Controller_Permissaoedicaoemenda
{
    private $model;
    private $emeid;
    private $emendaInformacoes;
    private $dadoEmenda;
    private $perfis;
    private $responsabilidade;

    public function __construct($emeid = NULL)
    {
        $this->emeid = ($emeid)? $emeid : '0';
        $this->model = new Spoemendas_Model_Periodopreenchimentoemenda();
        $this->responsabilidade = new Spoemendas_Model_UsuarioResponsabilidade();
        $this->emendaInformacoes = new Spoemendas_Model_Emendainformacoes();
        $this->dadoEmenda = $this->emendaInformacoes->findByEmeid($this->emeid);
        $this->perfis = pegaPerfilGeral($_SESSION['usucpf']);
    }

    /**
     * Busca o esdid do estado atual do documento
     * @return integer
     */
    private function getEsdid()
    {
        return wf_pegarEstadoAtual($this->dadoEmenda['docid'])['esdid'];
    }

    /**
     * Verifica se o usuario esta dentro do periodo de preenchimento
     * @return bool
     */
    public function getAutorizacaoEditarPeriodo()
    {
        $perfis = pegaPerfilGeral();
        $perfis = implode(",", $perfis);
        $dados = $this->model->getPermissaoPreenchimentoModel($perfis);
        if ($dados) {
            return true;
        }
        return false;
    }

    /**
     * Verifica perfil e estado do documento para permissao de edicao
     * @return bool
     */
    public function getPermissoesWorkFlow()
    {
        if ($this->getEsdid() == ESD_LIMITE_LIBERADO) {
            return false;
        }
        if($this->verificaSuperUser()){
            return true;
        }

        //Fluxo do TED
        if (in_array(PFL_SECRETARIAS, $this->perfis)
            && $this->getEsdid() == ESD_ANALISA_SECRETARIA
            || $this->getEsdid() == ESD_EM_PREENCHIMENTO
        ) {
            return true;
        }
        //Fluxo da UO Equipe Tecnica
        if (in_array(PFL_REITOR, $this->perfis) && $this->getEsdid() == ESD_AGUARDANDO_REITOR) {
            return true;
        }
        if (in_array(PFL_UO_EQUIPE_TECNICA, $this->perfis) && $this->getEsdid() == ESD_AGUARD_AJUST_UO || $this->getEsdid() == ESD_EM_PREENCHIMENTO) {
            return true;
        }

        // Fluxo do FNDE (Parlamentar)
        if (in_array(PFL_ASPAR, $this->perfis) && $this->getEsdid() == ESD_EM_ANALISE) {
            return false;
        }
        if (in_array(PFL_PARLAMENTAR, $this->perfis)
            && $this->getEsdid() == ESD_EM_PREENCHIMENTO
            || $this->getEsdid() == ESD_AGUARDANDO_AJUSTES_PARLAMENTAR
            || $this->getEsdid() == ESD_AGUARDANDO_AJUSTES_PARLAMENTAR_PAR_CONV
            || $this->getEsdid() == ESD_AGUARDANDO_AJUSTES_PARLAMENTAR_TED
        ) {
            return true;
        }
        if (in_array(PFL_FNDE, $this->perfis) && $this->getEsdid() == ESD_EM_ANALISE) {
            return false;
        }
    }

    /**
     * Da permissão para FNDE apenas preencher o impedimento
     * @return bool
     */
    public function getPermissaoImpedimentoFnde(){
        if (in_array(PFL_FNDE, $this->perfis) && $this->getEsdid() == ESD_EM_ANALISE) {
            return true;
        }
        return false;
    }
    /**
     * Verifica se o usuario e super usuario
     * @return bool
     */
    public function verificaSuperUser()
    {
        if ($this->responsabilidade->superUsuario()) {
            return true;
        }
        return false;
    }

    /**
     * Gera as permissoes para o pefil logado
     * @return bool
     */
    public function geraPermissoesEdicao()
    {
        if (!$this->getAutorizacaoEditarPeriodo()) {
            return false;
        }
        if (!$this->getPermissoesWorkFlow()) {
            return false;
        }
        return true;
    }


}

?>
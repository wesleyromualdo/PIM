<?php

class Representante extends mvc\Controle
{
    public function iniciarFluxo()
    {
        $this->visao->setCabecalho('includes/cabecalho_bootstrap_v4.inc', true);
        $this->visao->setRodape('includes/rodape_bootstrap_v4.inc', true);
        $this->visao->setarIncludeJS('/estrutura/js/funcoes.js');
        
        switch ($_REQUEST['requisicao']) {
            case 'editar-representante':
                    $arDado = array();
                    $entidadeRepresentante = new EntidadesRepresentantes_Model($_POST['enrid']);
                    $arDado['dado'] = $entidadeRepresentante->getDados();
                    $arDado['arComboRepresentante'] = (new EntidadesFuncao_Model())->getRepresentanteParaAssociacaoComboSql($entidadeRepresentante->enfidpai, ['enfid not in' => $entidadeRepresentante->enfidpai, 'inclui-representante' => $entidadeRepresentante->enfidfilho], false);
                    echo json_encode($arDado);
                die;
            case 'salvar-funcao':
                $entidadesFuncao = new EntidadesFuncao_Model($_GET['enfid']);
                $enfid = $entidadesFuncao->popularDadosObjeto($_POST)
                                         ->salvar();
                $entidadesFuncao->commit();
                
                $this->registrarMensagemSistema("Operação realizada com sucesso!", mvc\Controle::msgPositiva);
                $this->redirecionarTela('?modulo=representante/cadastrarFuncao&acao=A&enfid='.$enfid);                
                die;
            case 'salvar-representante':
                if ($this->salvarRepresentante()) {
                    $this->db->commit();
                    $this->registrarMensagemSistema("Operação realizada com sucesso!", mvc\Controle::msgPositiva);
                } else {
                    $this->registrarMensagemSistema("Falha na execução da operação! Tente novamente.", mvc\Controle::msgNegativa);                        
                }
                
                $this->redirecionarTela();
                die;
            case 'excluir-representante-entidade':
                $entidadeRepresentantes = new EntidadesRepresentantes_Model($_POST['enrid']);
                if ($entidadeRepresentantes->enrstatus == 1) {
                    $entidadeRepresentantes->enrstatus = 0;
                    $entidadeRepresentantes->salvar();
                    $entidadeRepresentantes->commit();
                    
                    $this->registrarMensagemSistema("Operação realizada com sucesso!", mvc\Controle::msgPositiva);
                } else {
                    $this->registrarMensagemSistema("Falha na execução da operação! Tente novamente.", mvc\Controle::msgNegativa);                        
                }
                $this->redirecionarTela();
                die;
            case 'excluir-funcao-entidade':
                    $entidadeFuncao = new EntidadesFuncao_Model($_POST['enfid']);
                    if ($entidadeFuncao->enfstatus == 'A') {
                        $entidadeFuncao->enfstatus = 'I';
                        $entidadeFuncao->salvar();
                        $entidadeFuncao->commit();
                        
                        $this->registrarMensagemSistema("Operação realizada com sucesso!", mvc\Controle::msgPositiva);
                    } else {
                        $this->registrarMensagemSistema("Falha na execução da operação! Tente novamente.", mvc\Controle::msgNegativa);                        
                    }
                    $this->redirecionarTela();
                die;
        }
    }
    
    public function acaoCadastrarFuncao()
    {
        $arView = array();
        $arView['arTipoEntidade']      = $this->getArrayRadioTipoEntidade();
        $arView['edicao']              = (new EntidadesFuncao_Model($_GET['enfid']))->getDados();
        $arView['arAba']               = $this->criarAbaCadastro($_GET['enfid']);
        
        $this->visao->carregarDados($arView);
        $this->visao->incluirVisao('cadastroFuncao');
    }
    
    public function acaoListar()
    {
        $arView = $_POST;
        $arView['arComboEntidadesFuncaoSql'] = (new EntidadesFuncao_Model())->getComboSql();
        $arView['listaEntidadesFuncao']      = (new EntidadesFuncao_Model())->listarEntidadesFuncao($_POST['pesquisa'] ? $_POST['pesquisa'] : array());
        $arView['arTipoEntidade']            = $this->getArrayRadioTipoEntidade(); 

        
        $this->visao->carregarDados($arView);
        $this->visao->incluirVisao('lista');
    }
    
    public function acaoCadastrar()
    {
        $arView = array();
        $arView['arTipoEntidade']      = $this->getArrayRadioTipoEntidade();
        $arView['listaRepresentantes'] = (new EntidadesRepresentantes_Model())->listarEntidadesRepresentantes(['enfidpai' => $_GET['enfid']]);
        $arView['edicao']              = (new EntidadesFuncao_Model($_GET['enfid']))->getDados();
        $arView['arAba']               = $this->criarAbaCadastro($_GET['enfid']);
        $arView['modalRepresentante']  = $this->criarModalCadastro($_GET['enfid']);
        
        $this->visao->carregarDados($arView);
        $this->visao->incluirVisao('cadastro');
    }
    
    private function salvarRepresentante()
    {
        if ($_GET['enfid'] && $_POST['enfidfilho']) {
            $entidadeRepresentante = new EntidadesRepresentantes_Model();
            $entidadeRepresentante->carregarPorRelacionamento($_GET['enfid'], $_POST['enfidfilho']);
            
            $entidadeRepresentante->enfidpai            = $_GET['enfid'];
            $entidadeRepresentante->enfidfilho          = $_POST['enfidfilho'];
            $entidadeRepresentante->enrqtdcadastrados   = $_POST['enrqtdcadastrados'];
            $entidadeRepresentante->enrtipo             = $_POST['enrtipo'];
            $entidadeRepresentante->enrgestor           = ($_POST['enrgestor'] == 'S' ? 1 : 0);
            $entidadeRepresentante->salvar();
            
            return true;
        } else {
            return false;
        }
    }
    
    private function criarModalCadastro($enfidpai)
    {
        $arView = array();
        $arView['arComboRepresentanteSql'] = (new EntidadesFuncao_Model())->getRepresentanteParaAssociacaoComboSql($enfidpai, ['enfid not in' => $enfidpai]);
        $arView['arTipoRepresentante']     = ['E' => 'Por Entidade', 'G' => 'Global'];
        $arView['arRadioGestor']           = ['S' => 'Sim', 'N' => 'Não'];
        
        $this->visao->dadoJs('arComboRepresentanteDado', (new EntidadesFuncao_Model())->getRepresentanteParaAssociacaoComboSql($enfidpai, ['enfid not in' => $enfidpai], false));
        $this->visao->carregarDados($arView);
        
        return $this->visao->capturarHtmlVisaoAcessoria('modalRepresentante');
    }
    
    private function criarAbaCadastro($enfid)
    {        
        
        $url = ($enfid
                    ? "demandas.php?modulo=".$_GET['modulo']."&acao=A&enfid={$enfid}"
                    : "demandas.php?modulo=".$_GET['modulo']."&acao=A");
        
        $this->visao->carregarDados(['url_pagina' => $url]);
        return ($enfid ? $this->criarAbaCadastroEdicao($enfid) : $this->criarAbaCadastroNovo());
    }
    
    private function criarAbaCadastroNovo()
    {        
        $aba[] = array("descricao" => "Lista de Funções", "link" => "demandas.php?modulo=representante/listar&acao=A");        
        $aba[] = array("descricao" => "Cadastro de Função", "link" => "demandas.php?modulo=representante/cadastrarFuncao&acao=A");
        return $aba;
    }
    
    private function criarAbaCadastroEdicao($enfid)
    {        
        $aba[] = array("descricao" => "Lista de Funções", "link" => "demandas.php?modulo=representante/listar&acao=A");        
        $aba[] = array("descricao" => "Cadastro de Função", "link" => "demandas.php?modulo=representante/cadastrarFuncao&acao=A&enfid={$enfid}");
        $aba[] = array("descricao" => "Vínculo de tipo de representante", "link" => "demandas.php?modulo=representante/cadastrar&acao=A&enfid={$enfid}");
        return $aba;
    }
    
    private function getArrayRadioTipoEntidade()
    {
        return ['F' => 'Pessoa Física', 'J' => 'Pessoa Jurídica'];
    }
}
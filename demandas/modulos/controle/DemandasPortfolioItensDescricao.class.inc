<?php
require APPRAIZ . 'demandas/model/DemandasPortfolioItensDescricao.class.inc';

class DemandasPortfolioItensDescricao extends mvc\Controle
{
    public function iniciarFluxo()
    {
        switch ($_REQUEST['requisicao']) {
            case 'salvarPortfolioItemDescricao':
                $this->acaoSalvar();
                $this->sucesso('demandas.php?modulo=demandasPortfolioItensDescricao/listar');
                die;
            case 'deletarPortfolioItemDescricao':
                $this->acaoDeletar($_REQUEST);
                $this->sucesso('demandas.php?modulo=demandasPortfolioItensDescricao/listar');
                die;
                
        }
    }
    
    public function acaoListar()
    {
        
        $moDemandasPortfolioItensDescricao = new DemandasPortfolioItensDescricao_Model();
        
        $arDado = array();
        
        $_POST['returnsql'] = true;
        
        $arDado['sql_portfolio_itens_descricao'] = $moDemandasPortfolioItensDescricao->listaPortfolioItensDescricao($_POST);
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('lista');
    }
    
    public function acaoCadastrar()
    {
        
        $moDemandasPortfolioItensDescricao = new DemandasPortfolioItensDescricao_Model();
        
        $arDado = array();
        
        if($_REQUEST['cidid']) $arDado = $moDemandasPortfolioItensDescricao->getPortfolioItensDescricaoPorId($_REQUEST['cidid']);
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('cadastro');
    }
    
    
    public function acaoDeletar()
    {
        $moDemandasPortfolioItensDescricao = new DemandasPortfolioItensDescricao_Model();
        
        $_REQUEST['cidstatus'] = '0';
        $moDemandasPortfolioItensDescricao->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasPortfolioItensDescricao->commit();
        
    }
    
    public function acaoSalvar()
    {
        $moDemandasPortfolioItensDescricao = new DemandasPortfolioItensDescricao_Model();
        
        $moDemandasPortfolioItensDescricao->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasPortfolioItensDescricao->commit();
        
    }
    
    
}
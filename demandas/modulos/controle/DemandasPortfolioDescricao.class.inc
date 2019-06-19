<?php
require APPRAIZ . 'demandas/model/DemandasPortfolioDescricao.class.inc';

class DemandasPortfolioDescricao extends mvc\Controle
{
    public function iniciarFluxo()
    {
        switch ($_REQUEST['requisicao']) {
            case 'salvarPortfolioDescricao':
                $this->acaoSalvar();
                $this->sucesso('demandas.php?modulo=demandasPortfolioDescricao/listar');
                die;
            case 'deletarPortfolioDescricao':
                $this->acaoDeletar($_REQUEST);
                $this->sucesso('demandas.php?modulo=demandasPortfolioDescricao/listar');
                die;
                
        }
    }
    
    public function acaoListar()
    {
        
        $moDemandasPortfolioDescricao = new DemandasPortfolioDescricao_Model();
        
        $arDado = array();
        
        $_POST['returnsql'] = true;
        $pesquisar = $_POST['pesquisar'];
        $this->visao->pesquisar = $pesquisar;
        
        $arDado['sql_portfolio_descricao'] = $moDemandasPortfolioDescricao->listaPortfolioDescricao($_POST);
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('lista');
    }
    
    public function acaoCadastrar()
    {
        
        $moDemandasPortfolioDescricao = new DemandasPortfolioDescricao_Model();
        
        $arDado = array();
        
        if($_REQUEST['dcdid']) $arDado = $moDemandasPortfolioDescricao->getPortfolioDescricaoPorId($_REQUEST['dcdid']);
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('cadastro');
    }
    
    
    public function acaoDeletar()
    {
        $moDemandasPortfolioDescricao = new DemandasPortfolioDescricao_Model();
        
        $_REQUEST['dcdstatus'] = '0';
        $moDemandasPortfolioDescricao->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasPortfolioDescricao->commit();
        
    }
    
    public function acaoSalvar()
    {
        $moDemandasPortfolioDescricao = new DemandasPortfolioDescricao_Model();
        
        $moDemandasPortfolioDescricao->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasPortfolioDescricao->commit();
        
    }
    
    
}
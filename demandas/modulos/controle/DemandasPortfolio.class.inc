<?php
require APPRAIZ . 'demandas/model/DemandasPortfolio.class.inc';

class DemandasPortfolio extends mvc\Controle
{
    public function iniciarFluxo()
    {
        switch ($_REQUEST['requisicao']) {
            case 'deletarPortfolio':
                $this->acaoDeletar();
                $this->sucesso('demandas.php?modulo=demandasPortfolio/listar');
                die;
            case 'deletarPortfolioItem':
                $this->deletarPortfolioItem();
                $this->sucesso('demandas.php?modulo=demandasPortfolio/cadastrar&dcoid='.$_REQUEST['dcoid']);
                die;
            case 'adicionarPortfolioItem':
                $this->acaoAdicionarPortfolioItem();
                $this->sucesso('demandas.php?modulo=demandasPortfolio/cadastrar&dcoid='.$_REQUEST['dcoid']);
                die;
            case 'salvarPortfolio':
                $dcoid = $this->acaoSalvar();
                $this->sucesso('demandas.php?modulo=demandasPortfolio/cadastrar&dcoid='.$dcoid);
                die;
            case 'deletarPortfolioItemDescricao':
                $this->acaoDeletar($_REQUEST);
                $this->sucesso('demandas.php?modulo=demandasPortfolioItensDescricao/listar');
                die;
                
        }
    }
    
    public function acaoAdicionarPortfolioItem()
    {
        
        require APPRAIZ . 'demandas/model/DemandasPortfolioItens.class.inc';
        
        $moDemandasPortfolioItens = new DemandasPortfolioItens_Model();
        
        $moDemandasPortfolioItens->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasPortfolioItens->commit();
    }
    
    public function acaoListar()
    {
        
        require APPRAIZ . 'demandas/model/DemandasPortfolioTipo.class.inc';
        require APPRAIZ . 'demandas/model/DemandasPortfolioDescricao.class.inc';
        require APPRAIZ . 'demandas/model/DemandasDemandados.class.inc';
        
        $moDemandasPortfolio = new DemandasPortfolio_Model();
        $moDemandasPortfolioTipo = new DemandasPortfolioTipo_Model();
        $moDemandasPortfolioDescricao = new DemandasPortfolioDescricao_Model();
        $moDemandasDemandados = new DemandasDemandados_Model();
        
        $arDado = array();
        
        $_POST['returnsql'] = true;
        
        $arDado['sql_portfolio'] = $moDemandasPortfolio->listaPortfolio($_POST);
        
        $arDado['sql_combo_portfolio_tipo'] = $moDemandasPortfolioTipo->getPortfolioTipoCombo(array('returnsql' => true));
        $arDado['sql_combo_portfolio_descricao'] = $moDemandasPortfolioDescricao->getPortfolioDescricaoCombo(array('returnsql' => true));
        $arDado['sql_combo_demandados'] = $moDemandasDemandados->getDemandadosCombo(array('returnsql' => true));
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('lista');
    }
    
    
    
    public function acaoCadastrar()
    {
        global $arDado;
        
        require APPRAIZ . 'demandas/model/DemandasPortfolioTipo.class.inc';
        require APPRAIZ . 'demandas/model/DemandasPortfolioDescricao.class.inc';
        require APPRAIZ . 'demandas/model/DemandasDemandados.class.inc';
        
        $moDemandasPortfolio = new DemandasPortfolio_Model();
        $moDemandasPortfolioTipo = new DemandasPortfolioTipo_Model();
        $moDemandasPortfolioDescricao = new DemandasPortfolioDescricao_Model();
        $moDemandasDemandados = new DemandasDemandados_Model();
        
        $arDado = array();
        
        if($_REQUEST['dcoid']) {
            
            $arDado = $moDemandasPortfolio->getPortfolioPorId($_REQUEST['dcoid']);
            
            require APPRAIZ . 'demandas/model/DemandasPortfolioItens.class.inc';
            
            $moDemandasPortfolioItens = new DemandasPortfolioItens_Model();
            
            $arDado['portfolioItens'] = $moDemandasPortfolioItens->listaPortfolioItens($_REQUEST);
            
        }
        
        $arDado['sql_combo_portfolio_tipo'] = $moDemandasPortfolioTipo->getPortfolioTipoCombo(array('returnsql' => true));
        $arDado['sql_combo_portfolio_descricao'] = $moDemandasPortfolioDescricao->getPortfolioDescricaoCombo(array('returnsql' => true));
        $arDado['sql_combo_demandados'] = $moDemandasDemandados->getDemandadosCombo(array('returnsql' => true));
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('cadastro');
    }
    
    
    public function acaoDeletar()
    {
        $moDemandasPortfolio = new DemandasPortfolio_Model();
        
        $_REQUEST['dcostatus'] = '0';
        $moDemandasPortfolio->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasPortfolio->commit();
        
    }
    
    public function deletarPortfolioItem()
    {
        
        require APPRAIZ . 'demandas/model/DemandasPortfolioItens.class.inc';
        
        $moDemandasPortfolioItens = new DemandasPortfolioItens_Model();
        
        $_REQUEST['dcistatus'] = '0';
        $moDemandasPortfolioItens->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasPortfolioItens->commit();
        
        
    }
    
    public function acaoSalvar()
    {

        $moDemandasPortfolio = new DemandasPortfolio_Model();
        
        $dcoid = $moDemandasPortfolio->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasPortfolio->commit();
        
        return $dcoid;
        
    }
    
    
}
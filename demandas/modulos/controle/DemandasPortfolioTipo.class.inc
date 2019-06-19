<?php
require APPRAIZ . 'demandas/model/DemandasPortfolioTipo.class.inc';

class DemandasPortfolioTipo extends mvc\Controle
{
	public function iniciarFluxo()
	{	    
	    switch ($_REQUEST['requisicao']) {
	        case 'salvarPortfolioTipo':
	            $this->acaoSalvar();
	            $this->sucesso('demandas.php?modulo=demandasPortfolioTipo/listar');
	            die;
	        case 'deletarPortfolioTipo':
	            $this->acaoDeletar($_REQUEST);
	            $this->sucesso('demandas.php?modulo=demandasPortfolioTipo/listar');
	            die;

	    }
	}	
	
	public function acaoListar()
	{    
	    
	    $moDemandasPortfolioTipo = new DemandasPortfolioTipo_Model();

	    $arDado = array();
	    
	    $_POST['returnsql'] = true;
        $pesquisar = $_POST['pesquisar'];
        $this->visao->pesquisar = $pesquisar;
	    
	    $arDado['sql_portfolio_tipo'] = $moDemandasPortfolioTipo->listaPortfolioTipo($_POST);
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('lista');
	}
	
	public function acaoCadastrar()
	{
	    
	    $moDemandasPortfolioTipo = new DemandasPortfolioTipo_Model();
	    
	    $arDado = array();
	    
	    if($_REQUEST['dctid']) $arDado = $moDemandasPortfolioTipo->getPortfolioTipoPorId($_REQUEST['dctid']);
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('cadastro');
	}

	
	public function acaoDeletar()
	{
	    $moDemandasPortfolioTipo = new DemandasPortfolioTipo_Model();
	    
	    $_REQUEST['dctstatus'] = '0';
	    $moDemandasPortfolioTipo->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moDemandasPortfolioTipo->commit();
	    
	}
	
	public function acaoSalvar()
	{
	    
	    $moDemandasPortfolioTipo = new DemandasPortfolioTipo_Model();
	    
	    $moDemandasPortfolioTipo->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moDemandasPortfolioTipo->commit();
	    
	}
	
	
}
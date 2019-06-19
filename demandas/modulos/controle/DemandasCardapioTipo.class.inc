<?php
require APPRAIZ . 'demandas/model/DemandasCardapioTipo.class.inc';

class DemandasCardapioTipo extends mvc\Controle
{
	public function iniciarFluxo()
	{	    
	    switch ($_REQUEST['requisicao']) {
	        case 'salvarCardapioTipo':
	            $this->acaoSalvar();
	            $this->sucesso('demandas.php?modulo=demandasCardapioTipo/listar&acao=A');
	            die;
	        case 'deletarCardapioTipo':
	            $this->acaoDeletar($_REQUEST);
	            $this->sucesso('demandas.php?modulo=demandasCardapioTipo/listar&acao=A');
	            die;

	    }
	}	
	
	public function acaoListar()
	{    
	    
	    $moDemandasCardapioTipo = new DemandasCardapioTipo_Model();

	    $arDado = array();
	    
	    $_POST['returnsql'] = true;
	    
	    $arDado['sql_cardapio_tipo'] = $moDemandasCardapioTipo->listaCardapioTipo($_POST);
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('lista');
	}
	
	public function acaoCadastrar()
	{
	    
	    $moDemandasCardapioTipo = new DemandasCardapioTipo_Model();
	    
	    $arDado = array();
	    
	    if($_REQUEST['dctid']) $arDado = $moDemandasCardapioTipo->getCardapioTipoPorId($_REQUEST['dctid']);
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('cadastro');
	}

	
	public function acaoDeletar()
	{
	    $moDemandasCardapioTipo = new DemandasCardapioTipo_Model();
	    
	    $_REQUEST['dctstatus'] = 'I';
	    $moDemandasCardapioTipo->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moDemandasCardapioTipo->commit();
	    
	}
	
	public function acaoSalvar()
	{
	    $moDemandasCardapioTipo = new DemandasCardapioTipo_Model();
	    
	    $moDemandasCardapioTipo->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moDemandasCardapioTipo->commit();
	    
	}
	
	
}
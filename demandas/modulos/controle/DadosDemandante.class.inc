<?php
class Demandantes extends mvc\Controle
{
	public function iniciarFluxo()
	{	    
	    switch ($_REQUEST['requisicao']) {
	        case 'pesquisar':
	            $this->pesquisar();
	            die;
	        case 'buscarListaAcessoRapido':
	            //$this->buscarListaAcessoRapido();
	            die;
	        case 'salvar':
	            //$this->salvar();
	            //$this->sucesso();
	            die;
	        case 'editar':
// 	            $this->buscarDadoEdicao();
	            die;
	        case 'inativar':
// 	            $this->inativar();
	            die;
	    }
	}
	
	public function acaoListar()
	{
	    require APPRAIZ . 'demandas/model/Demandantes.class.inc';
	    
	    
	    if( $_REQUEST['enfid'] ){
	        $_POST['enfid'] = $_REQUEST['enfid'];
	    } else {
	        $_POST['enfid'] = 1;
	    }
	    
	    $moDemandante = new Demandantes_Model();
	    $arDado = $moDemandante->getFuncaoEntidade($_POST['enfid']);
	    $arDado['enfid'] = $_REQUEST['enfid'];
	    
	    $arDado['abas'] = criaAbaPar ();	    
	    
	    if( $_POST['pesquisar'] ){
	        $arDado['sql_demandantes'] = $moDemandante->listaEntidade($_POST);
	    }
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('formFiltros');
	}
}
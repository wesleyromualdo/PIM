<?php

class UnidadeMedida extends mvc\Controle
{
    public function iniciarFluxo()
	{
	    switch ($_REQUEST['requisicao']) {
	        case 'salvarUnidadeMedida':
	            $this->acaoSalvar();
	            $this->sucesso('demandas.php?modulo=unidadeMedida/listar');
	            die;
	        case 'deletarUnidadeMedida':
	            $this->acaoDeletar($_REQUEST);
	            $this->sucesso('demandas.php?modulo=unidadeMedida/listar');
	            die;

	    }
    }	
    
    public function acaoListar()
	{    
	    $moUnMed = new UnidadeMedida_Model();

	    $arDado = array();
	    
	    $_POST['returnsql'] = true;
        $pesquisar = $_POST['pesquisar'];
        $this->visao->pesquisar = $pesquisar;
	    
		$arDado['sql_unidade_medida'] = $moUnMed->listaUnidadeMedida($_POST);
		$arDado['sql_co_sigla'] = $moUnMed->getSiglas();
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('lista');
    }
    
    public function acaoCadastrar()
	{
		$moUnMed = new UnidadeMedida_Model();

		$arDado = array();

		if($_REQUEST['co_unidade_medida'])
		{
			$arDado = $moUnMed->getUnidadeMedidaPorId($_REQUEST['co_unidade_medida']);
		}
		
		$this->visao->carregarDados($arDado);
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('cadastrar');
	}

	public function acaoSalvar()
	{
	    $moUnMed = new UnidadeMedida_Model();

        $_REQUEST['nu_cpf_inclusao'] = $_SESSION['usucpf'];

	    $moUnMed->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moUnMed->commit();
	    
	}

	public function acaoDeletar()
	{
	    $moUnMed = new UnidadeMedida_Model();

        $_REQUEST['co_status'] = '0';

	    $moUnMed->popularDadosObjeto($_REQUEST)->salvar();
	    
	    $moUnMed->commit();
	    
	}

}
?>
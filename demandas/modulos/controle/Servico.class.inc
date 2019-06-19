<?php

class Servico extends mvc\Controle
{
    public function iniciarFluxo()
	{
	    switch ($_REQUEST['requisicao']) {
	        case 'salvarServico':
	            $this->acaoSalvar();
	            $this->sucesso('demandas.php?modulo=servico/listar');
	            die;
	        case 'deletarServico':
	            $this->acaoDeletar($_REQUEST);
	            $this->sucesso('demandas.php?modulo=servico/listar');
	            die;

	    }
    }	

    public function acaoListar()
	{    
	    $moServico = new Servico_Model();
        $moUnidadeMedida = new UnidadeMedida_Model();
        $moGrupoServico = new GrupoServico_Model();

	    $arDado = array();
        
        $_POST['returnsql'] = true;
        $pesquisar = $_POST['pesquisar'];
        $this->visao->pesquisar = $pesquisar;
	    
	    $arDado['sql_servico'] = $moServico->listaServico($_POST);
        $arDado['sql_unidade_medida'] = $moUnidadeMedida->getComboUnidadeMedida();
        $arDado['sql_grupo_servico'] = $moGrupoServico->listaGrupoServicoFilhos();
	    
	    $this->visao->carregarDados($arDado);
	    
	    $this->visao->setarTituloCabecalho(' ');
	    $this->visao->incluirVisao('lista');
    }

    public function acaoCadastrar()
    {
        $moServico = new Servico_Model();
        $moUnidadeMedida = new UnidadeMedida_Model();
        $moGrupoServico = new GrupoServico_Model();

        $arDado = array();

        if($_REQUEST['co_servico'])
        {
            $arDado = $moServico->getServicoPorId($_REQUEST['co_servico']);
        }

        $arDado['sql_unidade_medida'] = $moUnidadeMedida->getComboUnidadeMedida();
        $arDado['sql_grupo_servico'] = $moGrupoServico->listaGrupoServicoFilhos();


        $this->visao->carregarDados($arDado);
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('cadastrar');
    }

    public function acaoSalvar()
    {
        $moServico = new Servico_Model();
        
        $_REQUEST['nu_cpf_inclusao'] = $_SESSION['usucpf'];

        $moServico->popularDadosObjeto($_REQUEST)->salvar();

        $moServico->commit();
    }

    public function acaoDeletar()
    {
        $moServico = new Servico_Model();
        
        $_REQUEST['co_status'] = '0';

        $moServico->popularDadosObjeto($_REQUEST)->salvar();

        $moServico->commit();
    }
}
?>